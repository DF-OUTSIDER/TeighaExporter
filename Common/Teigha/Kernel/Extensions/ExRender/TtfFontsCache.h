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

#ifndef ODTTFFONTSCACHE_INCLUDED
#define ODTTFFONTSCACHE_INCLUDED

#include "TD_PackPush.h"

#include "RxObject.h"
#include "Gi/GiTextStyle.h"
#include "OdVector.h"
#include "Ge/GeMatrix3d.h"
#include "Gi/GiConveyorGeometry.h"
#include "Gi/GiEmptyGeometry.h"
#include "ThreadsCounter.h"
#include "SharedPtr.h"

#define STL_USING_MAP
#include "OdaSTL.h"

class OdGsFiler;

#ifdef OD_TTFFONTSCACHE_SHAREABLENAMESPACE
namespace OD_TTFFONTSCACHE_SHAREABLENAMESPACE {
#endif // OD_TTFFONTSCACHE_SHAREABLENAMESPACE

// Callbacks interface for OdTtfFontsCache

/** \details
  <group ExRender_Classes> 
*/
class OdTtfFontsCacheCallback
{
  public:
    virtual OdRxObjectPtr tfcNewMetafile(void *pSessionId) = 0;
    virtual OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject *pMetafile, void *pSessionId) = 0;
    virtual void tfcFinalizeMetafile(OdRxObject *pMetafile, void *pSessionId) = 0;

    virtual void tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile, void *pSessionId) const = 0;

    virtual bool tfcSaveMetafile(const OdRxObject* /*pMetafile*/, OdGsFiler * /*pFiler*/) { return false; }
    virtual OdRxObjectPtr tfcLoadMetafile(OdGsFiler * /*pFiler*/) { return OdRxObjectPtr(); }
};

// OdTtfFontsCache

/** \details
  <group ExRender_Classes> 
*/
class OdTtfFontsCache : public OdRxObject,
                        public OdTtfFontsCacheCallback
{
  public:
    // Key for entire font
    typedef std::pair<OdUInt64, OdUInt64> FontKey;
    // Key for font character
    typedef OdUInt32 CharKey;
  protected:
    // Character cache
    struct CharCache
    {
      OdRxObjectPtr m_pMetafile; // Character metafile
      double m_sideMult; // Side movement multiplier
    };
    // Map for characters cache
    typedef std::map<CharKey, CharCache> CharMap;
    // Fonts cache
    struct FontCache
    {
      OdFont *m_pFont;
      CharMap m_cache;
      OdMutexPtr m_mutex;
      FontCache() : m_pFont(NULL) { }
      FontCache(OdFont *pFont) : m_pFont(pFont) { }
    };
    // Map for fonts
    typedef std::map<FontKey, OdSharedPtr<FontCache> > FontMap;
    FontMap m_cache;
    OdTtfFontsCacheCallback *m_pCallback;
    // Alias map
    typedef std::map<OdUInt64, OdUInt64> AliasMap;
    AliasMap m_aliases;
    // MTRegen mutex
    OdMutexPtr m_mutex;
  public:
    // Information about vectorized text (can be stored inside Gs cache)
    struct TextInfo
    {
      FontKey m_fontKey; // Key for font
      OdVector<CharKey, OdMemoryAllocator<CharKey> > m_textString; // String of text
      OdGeMatrix3d m_baseTransform; // Initial transformation
      OdGeVector3d m_stepDir; // Scaled step direction for each character
    };
  public:
    OdTtfFontsCache();
    OdTtfFontsCache(OdTtfFontsCacheCallback *pCallback);
    ~OdTtfFontsCache();

    ODRX_USING_HEAP_OPERATORS(OdRxObject);

    static OdSmartPtr<OdTtfFontsCache> createObject();
    static OdSmartPtr<OdTtfFontsCache> createObject(OdTtfFontsCacheCallback *pCallback);

    void setCallback(OdTtfFontsCacheCallback *pCallback);
    OdTtfFontsCacheCallback *callback() const { return m_pCallback; }

    // Initialize cache and process (returns false if this specific text couldn't be processed)
    bool processText(const OdGePoint3d &position, const OdGeVector3d &u, const OdGeVector3d &v,
                     const OdChar *pMsg, OdInt32 nLength, bool bRaw, const OdGiTextStyle *pTextStyle,
                     OdGiConveyorContext *pDrawContext, TextInfo &textOutInfo, void *pSessionId = NULL);

    // Play characters cache
    void playText(const TextInfo &textInInfo, void *pSessionId = NULL) const;
    void playText(const FontKey &fontKey, const CharKey *pText, OdUInt32 nText,
                  const OdGeMatrix3d &baseXform, const OdGeVector3d &sideVec, void *pSessionId = NULL) const;

    // Clear entire cache
    void clearCache();

    // GsState cache processing
    bool saveFontCache(OdGsFiler *pFiler) const;
    bool loadFontCache(OdGsFiler *pFiler, OdDbBaseDatabase *pDb);
  protected:
    void createFontKey(const OdGiTextStyle *pTextStyle, FontKey &fontKey, OdTextProperties *pTextProperties = NULL) const;
    virtual OdUInt64 getFontKey(const OdGiTextStyle *pTextStyle) const;
    FontCache &getFontCache(FontKey &fontKey, OdFont *pFont);
    OdRxObjectPtr createTextIterator(OdGiConveyorContext *pDrawContext, const OdChar* textString, int length, bool raw, const OdGiTextStyle* pTextStyle) const;
    void procCharacter(OdFont *pFont, FontKey &fontKey, FontCache &fontCache, CharKey chr, OdTextProperties& textProperties, void *pSessionId);
    // Stub implementation
    virtual OdRxObjectPtr tfcNewMetafile(void * /*pSessionId*/) { return OdRxObjectPtr(); }
    virtual OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject * /*pMetafile*/, void * /*pSessionId*/) { return &OdGiEmptyGeometry::kVoid; }
    virtual void tfcFinalizeMetafileExt(FontKey & /*fontKey*/, CharKey /*chr*/, OdRxObject * /*pMetafile*/, void * /*pSessionId*/) { }
    virtual void tfcFinalizeMetafile(OdRxObject * /*pMetafile*/, void * /*pSessionId*/) { }
    virtual void tfcPlayMetafile(const OdGeMatrix3d & /*xForm*/, const OdRxObject * /*pMetafile*/, void * /*pSessionId*/) const { }
};

typedef OdSmartPtr<OdTtfFontsCache> OdTtfFontsCachePtr;

// Inlines section

inline void OdTtfFontsCache::setCallback(OdTtfFontsCacheCallback *pCallback)
{
  m_pCallback = pCallback;
}

#ifdef OD_TTFFONTSCACHE_SHAREABLENAMESPACE
}
#endif // OD_TTFFONTSCACHE_SHAREABLENAMESPACE

#include "TD_PackPop.h"

#endif // ODTTFFONTSCACHE_INCLUDED
