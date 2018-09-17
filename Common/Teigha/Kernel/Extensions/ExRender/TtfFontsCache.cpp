/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "RxObjectImpl.h"
#include "RxDictionary.h"
#include "Ge/GeScale3d.h"
#include "OdTrueTypeFontBase.h"
#include "Gs/GsDbRootLinkage.h"
#include "Gs/GsFiler.h"
#include "TtfFontsCache.h"

#ifdef OD_TTFFONTSCACHE_SHAREABLENAMESPACE
namespace OD_TTFFONTSCACHE_SHAREABLENAMESPACE {
#endif // OD_TTFFONTSCACHE_SHAREABLENAMESPACE

OdTtfFontsCache::OdTtfFontsCache()
  : m_pCallback(NULL)
{
  m_pCallback = this;
}

OdTtfFontsCache::OdTtfFontsCache(OdTtfFontsCacheCallback *pCallback)
  : m_pCallback(pCallback)
{
}

OdTtfFontsCache::~OdTtfFontsCache()
{
}

OdSmartPtr<OdTtfFontsCache> OdTtfFontsCache::createObject()
{
  return OdRxObjectImpl<OdTtfFontsCache>::createObject();
}

OdSmartPtr<OdTtfFontsCache> OdTtfFontsCache::createObject(OdTtfFontsCacheCallback *pCallback)
{
  OdTtfFontsCachePtr pObj = createObject();
  pObj->setCallback(pCallback);
  return pObj;
}

// Initialize cache and process
bool OdTtfFontsCache::processText(const OdGePoint3d &position, const OdGeVector3d &u, const OdGeVector3d &v,
                                  const OdChar *pMsg, OdInt32 nLength, bool bRaw, const OdGiTextStyle *pTextStyle,
                                  OdGiConveyorContext *pDrawContext, TextInfo &textOutInfo, void *pSessionId)
{
  if (!pMsg || !pMsg[0] || !nLength)
  {
    textOutInfo.m_fontKey.first = textOutInfo.m_fontKey.second = 0;
    return true;
  }
  // Check does text processible
  if (!pTextStyle->isTtfFont() || pTextStyle->isVertical())
    return false;
  // Initialize cache for font
  createFontKey(pTextStyle, textOutInfo.m_fontKey);
  FontCache *fontCache = &getFontCache(textOutInfo.m_fontKey, pTextStyle->getFont());
  // Run processing loop
  if (nLength == -1)
    nLength = (OdInt32)odStrLen((wchar_t*)pMsg);
  // Prepare Xform
  OdGeMatrix3d charXForm;
  charXForm.setCoordSystem(position, u, v, u.crossProduct(v));
  OdTrueTypeFontBase* pFont = static_cast<OdTrueTypeFontBase*>(pTextStyle->getFont());
  OdGeScale3d scale(1. / pFont->fontAbove());
  charXForm.setToProduct(OdGeMatrix3d::scaling(scale, position), charXForm);
  // Create text iterator
  OdGiContext &pGiContext = pDrawContext->giContext();
  OdRxObjectPtr pIterStorage = createTextIterator(pDrawContext, pMsg, nLength, bRaw, pTextStyle);
  OdBaseTextIterator *pIter = reinterpret_cast<OdBaseTextIterator*>(pIterStorage.get());
  if (!pIter)
    return false;
  // Fill text properties
  OdTextProperties textFlags;
  textFlags.setVerticalText(pTextStyle->isVertical());
  textFlags.setTrackingPercent(pTextStyle->trackingPercent());
  textFlags.setIncludePenups(false);
  textFlags.setZeroNormals(false/*pGiContext.isZeroTextNormals()*/);
  textFlags.setTextQuality(pGiContext.textQuality());
  textFlags.setTtfPolyDraw(pGiContext.ttfPolyDraw());
  if (bRaw)
  {
    textFlags.setUnderlined(pTextStyle->isUnderlined());
    textFlags.setOverlined(pTextStyle->isOverlined());
    textFlags.setStriked(pTextStyle->isStriked());
  }
  if (pFont->isSupportReorder())
  {
    OdCharArray srcArray, dstArray;
    { // getTextAsDByte
      OdChar ch = pIter->nextChar();
	    while (ch)
	    {
		    srcArray.push_back(ch);
		    ch = pIter->nextChar();
      }
    }
    pFont->getGlyhpIndexes(srcArray, dstArray);
    for (OdUInt32 k = 0; k < dstArray.size(); k++)
    {
      procCharacter(pFont, textOutInfo.m_fontKey, *fontCache, dstArray[k], textFlags, pSessionId);
      textOutInfo.m_textString.push_back(dstArray[k]);
    }
	}
  else
  {
    bool bFirstChar = true;
    for(;;)
    {
      OdChar ch = pIter->nextChar()/*, ch2 = 0*/;
      OdCharacterProperties props = pIter->currProperties();
      if (!bRaw)
      {
        if ((textFlags.isUnderlined() != props.bUnderlined) ||
            (textFlags.isOverlined() != props.bOverlined) ||
            (textFlags.isStriked() != props.bStriked))
        {
          textFlags.setUnderlined(props.bUnderlined);
          textFlags.setOverlined(props.bOverlined);
          textFlags.setStriked(props.bStriked);
          // It's OK if this change is on first character, but we couldn't process if change comes inside text string
          if (bFirstChar)
          {
            createFontKey(pTextStyle, textOutInfo.m_fontKey, &textFlags);
            fontCache = &getFontCache(textOutInfo.m_fontKey, pTextStyle->getFont());
          }
          else
            return false; // Couldn't change fonts inside string
        }
      }
      //if (ch >= 0xD800 && ch <= 0xDFFF)
      //{ // #5870
      //  ch2 = pIter->nextChar();
      //}
      if (ch && props.bValid)
      {
        procCharacter(pFont, textOutInfo.m_fontKey, *fontCache, ch, textFlags, pSessionId);
        textOutInfo.m_textString.push_back(ch);
      }
      else if (!ch)
        break;
      if (props.bLastChar)
        break;
      bFirstChar = false;
    }
  }
  textOutInfo.m_baseTransform = charXForm;
  textOutInfo.m_stepDir = OdGeVector3d::kXAxis;
  textOutInfo.m_textString.setPhysicalLength(textOutInfo.m_textString.size());
  return true;
}

// Play characters cache
void OdTtfFontsCache::playText(const TextInfo &textInInfo, void *pSessionId) const
{
  playText(textInInfo.m_fontKey, textInInfo.m_textString.getPtr(), textInInfo.m_textString.size(),
           textInInfo.m_baseTransform, textInInfo.m_stepDir, pSessionId);
}

void OdTtfFontsCache::playText(const FontKey &fontKey, const CharKey *pText, OdUInt32 nText,
                               const OdGeMatrix3d &baseXform, const OdGeVector3d &sideVec, void *pSessionId) const
{
  if (fontKey.first == 0)
    return;
  OdGeMatrix3d tf = baseXform;
  OdUInt32 nChars = nText;
  const CharKey *pChars = pText;
  FontMap::const_iterator it = m_cache.find(fontKey);
  if (it != m_cache.end())
  {
    for (OdUInt32 n = 0; n < nChars; n++)
    {
      CharMap::const_iterator chIt = it->second->m_cache.find(pChars[n]);
      if (chIt != it->second->m_cache.end())
      {
        m_pCallback->tfcPlayMetafile(tf, chIt->second.m_pMetafile, pSessionId);
        if (n != nChars - 1)
          tf.setToProduct(tf, OdGeMatrix3d::translation(sideVec * chIt->second.m_sideMult));
      }
    }
  }
}

// Clear entire cache
void OdTtfFontsCache::clearCache()
{
  m_cache.clear();
}

// GsState cache processing
bool OdTtfFontsCache::saveFontCache(OdGsFiler *pFiler) const
{
  pFiler->wrUInt32((OdUInt32)m_cache.size());
  FontMap::const_iterator it = m_cache.begin();
  OdTtfDescriptor ttfDesc;
  while (it != m_cache.end())
  {
    it->second->m_pFont->getDescriptor(ttfDesc);
    pFiler->wrUInt64(it->first.first);
    pFiler->wrUInt64(it->first.second);
    pFiler->wrString(ttfDesc.typeface());
    pFiler->wrString(ttfDesc.fileName());
    pFiler->wrBool(ttfDesc.isBold());
    pFiler->wrBool(ttfDesc.isItalic());
    pFiler->wrUInt16(ttfDesc.charSet());
    pFiler->wrInt(ttfDesc.pitchAndFamily());
    pFiler->wrUInt32((OdUInt32)it->second->m_cache.size());
    CharMap::const_iterator itCh = it->second->m_cache.begin();
    while (itCh != it->second->m_cache.end())
    {
      pFiler->wrUInt32(itCh->first);
      pFiler->wrDouble(itCh->second.m_sideMult);
      if (!itCh->second.m_pMetafile.isNull() && pFiler->isWriteSection(OdGsFiler::kClientMetafileSection))
      {
        pFiler->wrSectionBegin(OdGsFiler::kClientMetafileSection);
        if (!m_pCallback->tfcSaveMetafile(itCh->second.m_pMetafile, pFiler))
          return false;
        pFiler->wrSectionEnd(OdGsFiler::kClientMetafileSection);
      }
      else
        pFiler->wrEOFSection();
      itCh++;
    }
    it++;
  }
  return true;
}

bool OdTtfFontsCache::loadFontCache(OdGsFiler *pFiler, OdDbBaseDatabase *pDb)
{
  OdUInt32 nCacheElems = pFiler->rdUInt32();
  for (OdUInt32 nElem = 0; nElem < nCacheElems; nElem++)
  {
    OdUInt64 nStableKey = pFiler->rdUInt64();
    OdUInt64 nStableFlags = pFiler->rdUInt64();
    OdGiTextStyle tStyle;
    OdTtfDescriptor &ttfDesc = tStyle.ttfdescriptor();
    ttfDesc.setTypeFace(pFiler->rdStringRet());
    ttfDesc.setFileName(pFiler->rdStringRet());
    ttfDesc.setBold(pFiler->rdBool());
    ttfDesc.setItalic(pFiler->rdBool());
    ttfDesc.setCharSet((int)pFiler->rdUInt16());
    ttfDesc.setPitchAndFamily(pFiler->rdInt());
    tStyle.setUnderlined(GETBIT(nStableFlags, 4));
    tStyle.setOverlined(GETBIT(nStableFlags, 8));
    tStyle.setStriked(GETBIT(nStableFlags, 16));
    tStyle.loadStyleRec(pDb);
    FontKey newFontKey;
    createFontKey(&tStyle, newFontKey);
    m_aliases[newFontKey.first] = nStableKey;
    OdTtfFontsCache::FontCache &fontCsh = getFontCache(newFontKey, tStyle.getFont());
    OdUInt32 nSyms = pFiler->rdUInt32();
    for (OdUInt32 nSym = 0; nSym < nSyms; nSym++)
    {
      OdTtfFontsCache::CharCache &chrCsh = fontCsh.m_cache[pFiler->rdUInt32()];
      chrCsh.m_sideMult = pFiler->rdDouble();
      if (pFiler->rdSection() == OdGsFiler::kClientMetafileSection)
      {
        chrCsh.m_pMetafile = m_pCallback->tfcLoadMetafile(pFiler);
        if (chrCsh.m_pMetafile.isNull())
          return false;
        if (!pFiler->checkEOF())
          return false;
      }
    }
  }
  return true;
}

void OdTtfFontsCache::createFontKey(const OdGiTextStyle *pTextStyle, FontKey &fontKey, OdTextProperties *pTextProperties) const
{
  fontKey.first = getFontKey(pTextStyle);
  fontKey.second = 0;
  SETBIT(fontKey.second, 1, pTextStyle->ttfdescriptor().isBold());
  SETBIT(fontKey.second, 2, pTextStyle->ttfdescriptor().isItalic());
  if (!pTextProperties)
  {
    SETBIT(fontKey.second, 4, pTextStyle->isUnderlined());
    SETBIT(fontKey.second, 8, pTextStyle->isOverlined());
    SETBIT(fontKey.second, 16, pTextStyle->isStriked());
  }
  else
  {
    SETBIT(fontKey.second, 4, pTextProperties->isUnderlined());
    SETBIT(fontKey.second, 8, pTextProperties->isOverlined());
    SETBIT(fontKey.second, 16, pTextProperties->isStriked());
  }
}

OdUInt64 OdTtfFontsCache::getFontKey(const OdGiTextStyle *pTextStyle) const
{
  return (OdUInt64)(OdIntPtr)pTextStyle->getFont();
}

OdTtfFontsCache::FontCache &OdTtfFontsCache::getFontCache(FontKey &fontKey, OdFont *pFont)
{
  TD_AUTOLOCK_P_DEF(m_mutex)
  AliasMap::iterator it = m_aliases.find(fontKey.first);
  if (it == m_aliases.end())
  {
    OdUInt64 uniqueId = fontKey.first;
    FontMap::iterator itx = m_cache.find(fontKey);
    while (itx != m_cache.end())
    {
      fontKey.first++;
      itx = m_cache.find(fontKey);
    }
    m_aliases[uniqueId] = fontKey.first;
  }
  else
    fontKey.first = it->second;
  OdSharedPtr<FontCache> &pPtr = m_cache[fontKey];
  if (pPtr.isNull()) pPtr = new FontCache(pFont);
  return *pPtr.get();
}

OdRxObjectPtr OdTtfFontsCache::createTextIterator(OdGiConveyorContext *pDrawContext, const OdChar* textString, int length, bool raw, const OdGiTextStyle* pTextStyle) const
{
  OdDbBaseDatabase *pDb = pDrawContext->giContext().database();
  OdDbBaseDatabasePE *pDbPE = OdGsDbRootLinkage::getDbBaseDatabasePE(pDb);
  if (pDbPE)
    return pDbPE->createTextIterator(pDb, textString, length, raw, pTextStyle).get();
  return OdRxObjectPtr();
}

void OdTtfFontsCache::procCharacter(OdFont *pFont, FontKey &fontKey, FontCache &fontCache, CharKey chr, OdTextProperties& textProperties, void *pSessionId)
{
  CharCache *chrCache = NULL;
  {
    TD_AUTOLOCK_P_DEF(fontCache.m_mutex)
    CharMap::iterator it = fontCache.m_cache.find(chr);
    if (it == fontCache.m_cache.end())
      chrCache = &fontCache.m_cache[chr];
  }
  if (chrCache)
  {
    chrCache->m_pMetafile = m_pCallback->tfcNewMetafile(pSessionId);
    OdGiConveyorGeometry *pGeom = m_pCallback->tfcBeginMetafile(chrCache->m_pMetafile, pSessionId);
    OdGePoint2d advance;
    pFont->drawCharacter((OdChar)chr, advance, pGeom, textProperties);
    tfcFinalizeMetafileExt(fontKey, chr, chrCache->m_pMetafile, pSessionId);
    m_pCallback->tfcFinalizeMetafile(chrCache->m_pMetafile, pSessionId);
    chrCache->m_sideMult = advance.x;
  }
}

#ifdef OD_TTFFONTSCACHE_SHAREABLENAMESPACE
}
#endif // OD_TTFFONTSCACHE_SHAREABLENAMESPACE

//
