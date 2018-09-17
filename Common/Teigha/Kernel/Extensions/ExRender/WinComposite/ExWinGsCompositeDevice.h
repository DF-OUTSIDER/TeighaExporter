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
// Composite device

#ifndef EXWINGSCOMPOSITEDEVICE
#define EXWINGSCOMPOSITEDEVICE

#include "TD_PackPush.h"

#include "ExWinGsCompositeDefs.h"
#include "ExGsCompositeDevice.h"
#include "ExGsWin32Device.h"
#include "IntArray.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class ExWinGsCompositeDevice
  : public ExGsCompositeDevice
  , public ExGsWin32Device
{
  protected:
    friend class ExWinGsCompositeView;
    HDC m_hOffscreenDC;
    HBITMAP m_hScreenBmp;
    OdIntArray m_clipStack;
    bool m_bClearScreen;
    bool m_bDiscardBackFaces;
    bool m_bUseTextOut;
    bool m_bUseTTFCache;
    OdUInt32 m_nUseLutPalette;
  public:
    ODRX_USING_HEAP_OPERATORS(ExGsCompositeDevice);

    ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExWinGsCompositeDevice);

    ExWinGsCompositeDevice();
    ~ExWinGsCompositeDevice();

    // OdGsDevice overrides

    virtual OdRxDictionaryPtr properties();

    virtual void onSize(const OdGsDCRect& outputRect);

    virtual bool setBackgroundColor(ODCOLORREF color);

    virtual void setLogicalPalette(const ODCOLORREF* palette, int nCount);

    // ExGsWin32Device overrides

    virtual void HDCWasChanged();

    // Clipping

    void pushViewportClip(const OdGsDCPoint* points, const int *pointCounts, int numPolygons);
    void pushViewportClip(const OdGsDCPoint& min, const OdGsDCPoint& max);
    void pushViewportClip(OdUInt32 nNumRects, const OdGsDCRect* rects);

    void popViewportClip();

    // Device props

    void setClearScreen(bool bSet);
    bool clearScreen() const;

    void setDiscardBackFaces(bool bSet);
    bool discardBackFaces() const;

    void setUseTextOut(bool bSet);
    bool useTextOut() const;

    void setUseTTFCache(bool bSet);
    bool useTTFCache() const;

    void setUseLutPalette(OdUInt32 nSet);
    OdUInt32 useLutPalette() const;

    // Snapshot support
    void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);
    void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView);
    virtual OdGiRasterImagePtr snapshotImageParams() const;
    virtual OdGiRasterImagePtr snapshotRegion(const OdGsDCRect &area, bool bCrop) const;
  protected:
    // Initialize only our context which is attached to window
    virtual void createContext();
    virtual void deleteContext();

    // Process synchronization with all dependent devices/views/modules
    virtual void sync();

    // Make preprocessing (something like beginViewVectorization but for entire surface)
    virtual void preprocess();
    // Make postprocessing (something like endViewVectorization but for entire surface)
    virtual void postprocess();
    // Make surface presentation
    virtual void present();

    // Manage composite view context
    virtual void compositeViewAttached(ExGsCompositeView &pView);
    virtual void compositeViewDetached(ExGsCompositeView &pView);
    // Make presentation of updated viewport onto surface
    virtual void presentViewportSurface(ExGsCompositeView &pView, const OdGsDCRect &pUpdatedRect);
    virtual void presentRedirectedSurface(ExGsCompositeDevice &pDevice, const OdGsDCRect *pUpdatedRect = NULL);
    void presentRect(ExGsCompositeView &pView, const OdGsDCRect &screenRect);

    HDC hdc() const;
    HDC drawDc() const;

    bool isWindowRequired(ExGsCompositeView &pView) const;
};

#include "TD_PackPop.h"

#endif // EXWINGSCOMPOSITEDEVICE
