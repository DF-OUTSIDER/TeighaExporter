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
// GLES2 device TTF fonts cache support

#ifndef ODTRVECTTFFONTSCACHE
#define ODTRVECTTFFONTSCACHE

#include "TD_PackPush.h"

#include "TrVecDefs.h"
#include "../TrVisTtfFontsCacheClient.h"
#ifndef OD_TTFFONTSCACHE_SHAREABLENAMESPACE
#define OD_TTFFONTSCACHE_SHAREABLENAMESPACE OdTrVecShareableNS
#endif // OD_TTFFONTSCACHE_SHAREABLENAMESPACE
#include "TtfFontsCache.h"
#ifdef OD_TTFFONTSCACHE_SHAREABLENAMESPACE
using namespace OD_TTFFONTSCACHE_SHAREABLENAMESPACE;
#endif // OD_TTFFONTSCACHE_SHAREABLENAMESPACE
#include "ExDrawOrderBase.h"

#include "StaticRxObject.h"

class OdTrVecDevice;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVecTtfFontsCache : public OdStaticRxObject<OdTtfFontsCache>, public OdTrVisTtfFontsCacheClient
{
  protected:
    OdStaticRxObject<OdDrawOrderMetafileBase> m_deviceRefs;
    bool m_bStableKey;
  public:
    OdTrVecTtfFontsCache();
    ~OdTrVecTtfFontsCache();

    OdDrawOrderMetafileBase &deviceRefs() { return m_deviceRefs; }

    bool useStableFontKey() const { return m_bStableKey; }
    void setUseStableFontKey(bool bStableKey) { m_bStableKey = bStableKey; }
    void setUseStableFontKey(OdTrVecDevice *pDevice);

    const void *getFontCache(const FontKey &fontKey) const;
    OdTrVisDisplayId getCharMetafile(const void *pFont, OdUInt32 uChar, double *pSideMult = NULL);

    void shareToNew(OdTrVecDevice *pDevice);

    OdRxObjectPtr tfcNewMetafile(void *pSessionId);
    OdGiConveyorGeometry *tfcBeginMetafile(OdRxObject *pMetafile, void *pSessionId);
    void tfcFinalizeMetafileExt(FontKey &fontKey, CharKey chr, OdRxObject *pMetafile, void *pSessionId);
    void tfcFinalizeMetafile(OdRxObject *pMetafile, void *pSessionId);
    void tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile, void *pSessionId) const;
  protected:
    OdUInt64 getFontKey(const OdGiTextStyle *pTextStyle) const;
    TtfFont getFontCache(OdUInt64 fontKeyPartA, OdUInt64 fontKeyPartB) const
    { return (TtfFont)getFontCache(FontKey(fontKeyPartA, fontKeyPartB)); }
    OdTrVisDisplayId getCharMetafile(TtfFont pFont, OdUInt32 uChar, double *pSideMult = NULL) const
    { return const_cast<OdTrVecTtfFontsCache*>(this)->getCharMetafile((const void *)pFont, uChar, pSideMult); }
};

#include "TD_PackPop.h"

#endif // ODTRVECTTFFONTSCACHE
