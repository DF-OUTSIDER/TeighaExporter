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



//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////


#include "PdfExportCommon.h"
#include "PdfType3Optimizer.h"
#include "PdfContentStream4Type3.h"
#include "PdfCharProcDictionary.h"
#include "PdfEncodingDictionary.h"
#include "PdfDifferencesArray.h"
#include "PdfFontWidthsArray.h"
#include "PdfFontMatrix.h"
#include "PdfToUnicodeCMap.h"
#include "PdfAux.h"

#include "OdFont.h"
#include "Gi/GiTextStyle.h"
#include "Ge/GePoint2d.h"

#include "OdCharMapper.h"
namespace TD_PDF_2D_EXPORT {

const OdUInt32 PDFType3Optimizer::PDFType3OptElem::nMaxCharactersInType3Font = 0xff;

PDFType3Optimizer::PDFType3Optimizer() 
{
}

void PDFType3Optimizer::clear()
{
  m_pFonts.clear();
}

PDFType3Optimizer::PDFType3OptElem *PDFType3Optimizer::Find(PDFType3FontPtr pFont)
{
  PDFType3OptElemArray::iterator pIt = m_pFonts.begin();
  PDFType3OptElemArray::iterator pE  = m_pFonts.end();

  while(pIt != pE)
  {
    if (pIt->pFont == pFont)
      return pIt;
    ++pIt;
  }

  return 0;
}

PDFType3Optimizer::PDFType3OptElem *PDFType3Optimizer::AddNewElem(PDFType3FontPtr pFont)
{
  PDFType3OptElem *pElem = m_pFonts.append();
  pElem->pFont = pFont;
  
  return pElem;
}

void PDFType3Optimizer::addUnicodeChar(const OdGiTextStyle& pTextStyle, PDFType3FontPtr pFont, OdChar pUnicode, OdAnsiString &text, OdBool isInBigFont)
{
  ODA_ASSERT(!pFont.isNull());

  text = ("");

  PDFType3OptElem *pElem = Find(pFont);
  if (!pElem)
    pElem = AddNewElem(pFont);
  
  OdFont *pOdFont = pTextStyle.getFont();
  OdFont *pBigFont = pTextStyle.getBigFont();

  double dBigFontScale = pBigFont ? pOdFont->getAbove() / pBigFont->getAbove() : 1.; // it can be != 1. and here is getAbove, not getHeight

  {
    OdChar realUnicode = pUnicode;
//     Disabled for CORE-11418 (different symbols 49 and 24113 were handled as one)
//     if (isInBigFont)
//     {
//       OdString bigFontName = pTextStyle.bigFontFileName();
//       OdCodePageId bigFontCodePage = OdCharMapper::getCpByBigFont(bigFontName);
//       OdCharMapper::codepageToUnicode(realUnicode, bigFontCodePage, realUnicode);
//     }

    OdUInt8 singleByteCode;
    if (!pElem->hasCharacter(realUnicode, singleByteCode))
    {
      OdGePoint2d adv;

      PDFContentStream4Type3Ptr pStream = PDFContentStream4Type3::createObject(*pFont->document(), true);

      PDFShxGeomStore ShxGeom(pStream, pStream);

      OdTextProperties textFlags;
      textFlags.setVerticalText(pTextStyle.isVertical());
      textFlags.setTrackingPercent(1.);
      textFlags.setIncludePenups(false);

      if (isInBigFont)
      {
        ODA_ASSERT(pBigFont);
        ODA_VERIFY(pBigFont->drawCharacter(pUnicode, adv, &ShxGeom, textFlags) == ::eOk);
        ShxGeom.setScale( dBigFontScale );
      }
      else
      {
        ODA_VERIFY(pOdFont->drawCharacter(pUnicode, adv, &ShxGeom, textFlags) == ::eOk);
        ShxGeom.setScale( 1. );
      }
      ShxGeom.setAdvance(adv);
      ShxGeom.fillContent(pFont->getLineWeigth());

      PDFBBoxStore BBox;
      ShxGeom.getBBox( BBox );
      pElem->m_BBox.add(BBox);

      if (!pElem->AddUChar(realUnicode, ShxGeom.getCharWidth(), pStream, singleByteCode))
      {
        ODA_FAIL_ONCE(); // TODO : font has more than 255 characters! Type3 font must have maximum 255 characters.
      }
    }
    
    // TODO : optimization by size, after optimization check usage of next characters ')(\'
    //if (singleByteCode >= 32 && singleByteCode <= 128)
    //  text += singleByteCode;
    //else
    {
      OdAnsiString tmp; tmp.format(("\\%03o"), singleByteCode);
      text += tmp;
    }
  }
}

void PDFType3Optimizer::addUnicodeText(const OdGiTextStyle& pTextStyle, PDFType3FontPtr pFont, const OdUInt16Array &pUnicode, OdAnsiString &text, const OdBoolArray &isInBigFont)
{
  ODA_ASSERT(!pFont.isNull());

  if (pUnicode.isEmpty())
    return;

  text = ("");

  for(OdUInt32 f=0; f<pUnicode.size(); ++f)
  {
    OdAnsiString tmp;
    addUnicodeChar(pTextStyle, pFont, pUnicode[f], tmp, isInBigFont[f]);
    text += tmp;
  }
}

void PDFType3Optimizer::Optimize()
{
  PDFType3OptElemArray::iterator pIt = m_pFonts.begin();
  PDFType3OptElemArray::iterator pE  = m_pFonts.end();

  while(pIt != pE)
  {
    PDFType3FontPtr pT3Font(pIt->pFont);
    PDFDocument &PDFDoc = *pT3Font->document();

    PDFRectanglePtr pRect = pT3Font->getFontBBox( );
    
    double llx, lly, urx, ury;
    pIt->m_BBox.get(llx, lly, urx, ury);
    pRect->set(OdUInt32(llx), OdUInt32(lly), OdUInt32(urx), OdUInt32(ury));

    PDFCharProcDictionaryPtr pCharProcDict = PDFCharProcDictionary::createObject(PDFDoc, true);
    PDFEncodingDictionaryPtr pEncoding = PDFEncodingDictionary::createObject(PDFDoc, true);
    PDFDifferencesArrayPtr pDiff = PDFDifferencesArray::createObject(PDFDoc, true);

    OdUInt32 nNumChars = pIt->UnicodeChars.size();
    OdUInt32 f;
    for(f=0; f<nNumChars; ++f)
    {
      OdUInt16 uCh = pIt->UnicodeChars[f];
      PDFContentStream4Type3Ptr pContentStream = pIt->ContStreams[f];
      
      OdAnsiString CharName;
      CharName = pCharProcDict->addChar(uCh, pContentStream); 
      
      ODA_ASSERT(!CharName.isEmpty());
      ODA_VERIFY( pDiff->add(OdUInt16(f), CharName) );
    }

    pEncoding->setDifferences(pDiff);

    pT3Font->setCharProcs(pCharProcDict);
    pT3Font->setEncoding(pEncoding);
    pT3Font->setFirstChar(0);
    pT3Font->setLastChar(nNumChars-1);

    PDFFontWidthsArrayPtr pWidths = PDFFontWidthsArray::createObject(PDFDoc, true);
    pWidths->FillWidthsArray(pIt->nWidths.asArrayPtr(), pIt->nWidths.size());
    pT3Font->setWidths(pWidths);

    PDFCharPairArray charPairs;
    charPairs.resize(nNumChars);
    for(f=0; f<nNumChars; ++f)
    {
      charPairs[f].nCharCode = OdUInt16(f);
      charPairs[f].nUnicodeCode = pIt->UnicodeChars[f];
    }

    PDFToUnicodeCMapPtr pToUnicodeCMap = PDFToUnicodeCMap::createObject(PDFDoc, true);
    PDFResult res = pToUnicodeCMap->fillStream(charPairs);
    ODA_ASSERT(res == TD_PDF::eOk); // AlexR : it can be skipped, just you cannot copy text.
    if (res == TD_PDF::eOk)
    {
      pT3Font->setToUnicode(pToUnicodeCMap);
    }

    /*
        pT3Font->setFontDescriptor(); , PDFDictionary );*/
    

    ++pIt;
  }

}

PDFResultEx PDFType3Optimizer::getFontForCharacter(PDFDocument &PDFDoc, const OdGiTextStyle& pTextStyle, OdChar nUChar, PDFFontPtr &T3Font, OdAnsiString &fontName, double lineWeight,
  OdGeVector2d u2d, OdGeVector2d v2d, PDFResourceDictionaryPtr pResDict)
{
  OdUInt32 nIndx = 0;

  for(;nIndx < 0xffff;++nIndx)
  {
    fontName = TD_PDF_HELPER_FUNCS::addType3Font(PDFDoc, pTextStyle, &T3Font, nIndx, lineWeight, u2d, v2d, pResDict);
    ODA_ASSERT(!fontName.isEmpty());
    PDFType3OptElem* pT3Elem = Find(T3Font);
    if (!pT3Elem)
    {
      T3Font = T3Font;
      return exOk;
    }
    else
    {
      OdUInt8 ch;
      if (pT3Elem->hasFreeSpace(1) || pT3Elem->hasCharacter(nUChar, ch))
      {
        T3Font = T3Font;
        return exOk;
      }
    }
  }

  ODA_ASSERT(0);
  return exHugeShxFont;
}

PDFType3Optimizer::~PDFType3Optimizer()
{
}
}
