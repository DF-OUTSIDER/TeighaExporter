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

// WinDirectX.cpp : Defines the initialization routines for the DLL.
//

// to satisfy VC2005, else it refuses to compile WinDirectX project
#if _MSC_VER >= 1400
  #define POINTER_64 __ptr64
#endif

#include "OdaCommon.h"
#include "Gs/GsBaseVectorizer.h"
#include "ExGsScreenDevice.h"
#include "ExGsBitmapDevice.h"
#include "ExGsDirectXDevice.h"
#include "RxDynamicModule.h"

#include "Gi/GiRasterWrappers.h"
#include "ColorMapping.h"
#include "RxRasterServices.h"
#include "DynamicLinker.h"

#ifdef _TOOLKIT_IN_DLL_
#define VC_EXTRALEAN
#include "windows.h"

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
    break;
  }
  return TRUE;
}

#endif //_TOOLKIT_IN_DLL_

#define OUR_WND_NAME OD_T("Teigha DirectXRender Window")

class ExGsDirectXBitmapDevice : public ExGsBitmapDevice<ExGsDirectXDevice>
{
  bool m_bHwndCreated;
  OdUInt8Array m_scanLines;
  OdUInt32 m_outBitsPerPixel;
  OdGiRasterImagePtr m_pOutRaster;

ODRX_DECLARE_PROPERTY(RasterImage)
ODRX_DECLARE_PROPERTY(BitPerPixel)
ODRX_DECLARE_PROPERTY(DiscardBackFaces)
ODRX_DECLARE_PROPERTY(D3DSurface)
ODRX_DECLARE_PROPERTY(UseTTFCache)
ODRX_DECLARE_PROPERTY(UseLutPalette)
ODRX_DECLARE_PROPERTY(LineSmoothingSupported)
ODRX_DECLARE_PROPERTY(RefDevice)
public:

  void put_BitPerPixel(OdUInt32 bitsPerPixel)
  {
    m_outBitsPerPixel = bitsPerPixel; // store users bpp
    putBitPerPixel(24);               // create 24bpp (DirectX is always 24bpp)
  }

  ExGsDirectXBitmapDevice()
    : m_bHwndCreated(false)
    , m_outBitsPerPixel(24)
  {
  }
  static OdGsDevicePtr createObject()
  {
    return OdRxObjectImpl<ExGsDirectXBitmapDevice, OdGsDevice>::createObject();
  }
  void generatePropMap (OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES(ExGsWin32Device);
    ODRX_GENERATE_PROPERTY(RasterImage)
    ODRX_GENERATE_PROPERTY(BitPerPixel)
    ODRX_GENERATE_PROPERTY(DiscardBackFaces)
    ODRX_GENERATE_PROPERTY(D3DSurface)
    ODRX_GENERATE_PROPERTY(UseTTFCache)
    ODRX_GENERATE_PROPERTY(UseLutPalette)
    ODRX_GENERATE_PROPERTY(LineSmoothingSupported)
    ODRX_GENERATE_PROPERTY(RefDevice)
  }
  void update(OdGsDCRect* pUpdatedRect=0)
  {
    // do not remove throw. DirectX works with 24bpp images only!
    if(m_nColorDepth != 24)
      throw OdError(eNotImplemented);

    m_pOutRaster = OdGiRasterImagePtr();

    setLockableBackBuffer(true);

    int pxWidth(width()), pxHeight(height());

    if(!m_hWnd)
    {
      m_bHwndCreated = true;
      HINSTANCE instance = GetModuleHandle(0);

      // Direct3D requires non-null HWND
      WNDCLASSEX          wc;
      // Register Main Window Class
      wc.cbSize           = sizeof(WNDCLASSEX);
      wc.style            = CS_NOCLOSE; 
      wc.lpfnWndProc      = DefWindowProc;
      wc.cbClsExtra       = 0; 
      wc.cbWndExtra       = 4;  // Reserve space for lpd3dWindow pointer
      wc.hInstance        = instance;
      wc.hIcon            = 0;
      wc.hCursor          = 0;
      wc.hbrBackground    = (HBRUSH)GetStockObject (WHITE_BRUSH);
      wc.lpszMenuName     = 0;
      wc.hIconSm          = 0;
      wc.lpszClassName    = OUR_WND_NAME;

      /*ATOM wndClass = */::RegisterClassEx(&wc);

      m_hWnd = ::CreateWindowEx(0, OUR_WND_NAME, OUR_WND_NAME, WS_POPUP, 
        0, 0, pxWidth, pxHeight, GetDesktopWindow(), 0, instance, 0);
    }
    ExGsBitmapDevice<ExGsDirectXDevice>::update(pUpdatedRect);

    // create the image surface to store the front buffer image
    // note that call to GetFrontBuffer will always convert format to A8R8G8B8
    CComPtr<IDirect3DSurface> pSurf;
    DXError::check(d3dDevice()->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurf));

    D3DLOCKED_RECT lockedRect;
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = pxWidth;
    rect.bottom = pxHeight;
    DXError::check(pSurf->LockRect(&lockedRect, 0, D3DLOCK_READONLY));
    OdUInt8* pSfBits = (OdUInt8*)lockedRect.pBits;
    OdUInt32 scLnSz = scanLineSize();

    // copy from RGBA to RGB raster.
    m_scanLines.resize(scLnSz*pxHeight);
    int i, j;
    for(i = 0; i < pxHeight; i ++)
    {
      OdUInt8* pTo = m_scanLines.asArrayPtr() + scLnSz*i;
      OdUInt8* pFrom = pSfBits + lockedRect.Pitch*(pxHeight-i-1);
      for(j = 0; j < pxWidth; j ++)
      {
#ifdef _DEBUG
        // To prevent memory corruption of m_scanLines array.
        // Exception appears inside operator[] in case of out of valid interval array index.
        m_scanLines[scLnSz*i+2] = m_scanLines[scLnSz*i+2];
#endif // _DEBUG

        *pTo     = *(pFrom+0); // todo - it may be working not always
        *(pTo+1) = *(pFrom+1);
        *(pTo+2) = *(pFrom+2);

        pTo += 3;
        pFrom += 4;
      }
    }
    pSurf->UnlockRect();

    // ExGsDirectXBitmapDevice are 24 bpp, but user can request 8bpp (for example for preview)
    // So generates user defined copy.
    if (m_outBitsPerPixel != (OdUInt32)m_nColorDepth)
    {
      // Get output in 24bpp
      OdGiRasterImagePtr pRaster = ExGsBitmapDevice<ExGsDirectXDevice>::getRasterImage();

      // Fill raster description like user requested.
      double xPelsPerUnit(0.0), yPelsPerUnit(0.0);
      OdGiRasterImage::Units unuts = pRaster->defaultResolution(xPelsPerUnit, yPelsPerUnit);
      OdUInt32 pixelWidth(pRaster->pixelWidth()), pixelHeight(pRaster->pixelHeight());

      OdSmartPtr<OdGiRasterImageDesc> pDesc =
        OdGiRasterImageDesc::createObject(pixelWidth, pixelHeight, unuts, xPelsPerUnit, yPelsPerUnit);
      
      ODCOLORREF clrBackground(ODRGB(255,255,255));
      pDesc->setPalette(256 * sizeof(ODCOLORREF), (OdUInt8*)(odcmAcadPalette(clrBackground)));
      OdGiRasterImage::PixelFormatInfo pf = pRaster->pixelFormat();
      pDesc->pixelFormat().setBGRA();

      pDesc->setScanLinesAlignment(pRaster->scanLinesAlignment());
      pDesc->setColorDepth(m_outBitsPerPixel);

      // load raster services module
      OdRxRasterServicesPtr pRS = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
      // try to convert, in case of unavailable raster module, m_pOutRaster will be null
      m_pOutRaster = pRaster->convert(false, 50.,50.,0.0, clrBackground, false, false, false, pDesc);
    }
    else
    {
      //m_pOutRaster = ExGsBitmapDevice<ExGsDirectXDevice>::getRasterImage();
    }
  }

  OdGiRasterImagePtr getRasterImage() 
  {
    return m_pOutRaster.isNull() ? ExGsBitmapDevice<ExGsDirectXDevice>::getRasterImage() : m_pOutRaster; 
  }

  virtual ~ExGsDirectXBitmapDevice()
  {
    if(m_bHwndCreated)
    {
      /*BOOL res = */::DestroyWindow(m_hWnd);

      ::UnregisterClass(OUR_WND_NAME, GetModuleHandle(0));
    }
  }
  const OdUInt8* scanLines() const
  {
    return m_pOutRaster.isNull() ? m_scanLines.asArrayPtr() : m_pOutRaster->scanLines();
  }
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const
  {
    if (m_pOutRaster.isNull()) 
      memcpy(scnLines, m_scanLines.asArrayPtr() + firstScanline*scanLineSize(), numLines*scanLineSize());
    else
      m_pOutRaster->scanLines(scnLines, firstScanline, numLines);
  }
};

ODRX_DEFINE_PROPERTY_OBJECT_PREFIX(ExGsDirectXBitmapDevice::, RasterImage, ExGsDirectXBitmapDevice, getRasterImage().get, putRasterImage, OdGiRasterImage)
ODRX_DEFINE_PROPERTY_PREFIX(ExGsDirectXBitmapDevice::, BitPerPixel, ExGsDirectXBitmapDevice, getInt32)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsDirectXBitmapDevice::, DiscardBackFaces, ExGsDirectXBitmapDevice, discardBackFaces, setDiscardBackFaces, getBool)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsDirectXBitmapDevice::, D3DSurface, ExGsDirectXBitmapDevice, backBufferSurface, setBackBufferSurface, getIntPtr)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsDirectXBitmapDevice::, UseTTFCache, ExGsDirectXDevice, useTTFCache, setUseTTFCache, getBool)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsDirectXBitmapDevice::, UseLutPalette, ExGsDirectXDevice, useLutPalette, setUseLutPalette, getUInt32)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsDirectXBitmapDevice::, LineSmoothingSupported, ExGsDirectXDevice, isLinesAntiAliasingSupported, setLinesAntiAliasingSupported, getBool)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX(ExGsDirectXBitmapDevice::, RefDevice, ExGsDirectXDevice, isRefDevice, setRefDevice, getBool)

class WinDirectXModule : public OdGsBaseModule
{
  HINSTANCE m_d3dInst;
  // HINSTANCE m_d3dxInst;
protected:
  void initApp()
  {
    OdGsBaseModule::initApp();

#ifndef OD_USE_DIRECT3D9
    m_d3dInst  = ::LoadLibrary(OD_T("d3d8"));
    // m_d3dxInst = ::LoadLibrary("d3dx8d");
#else
    m_d3dInst  = ::LoadLibrary(OD_T("d3d9"));
    // m_d3dxInst = ::LoadLibrary("d3dx9d");
#endif

    ExGiDirectXGeometry::rxInit();
    ExGiDirectXMetafile::rxInit();
    ExD3dMaterialCacheEntry::rxInit();
    ExGiD3dMaterialTextureData::rxInit();
  }
  void uninitApp()
  {
    if(m_d3dInst)
    {
      ::FreeLibrary(m_d3dInst);
      m_d3dInst = NULL;
      /* ::FreeLibrary(m_d3dxInst);
      m_d3dxInst = NULL;*/
    }
    ExGiDirectXGeometry::rxUninit();
    ExGiDirectXMetafile::rxUninit();
    ExD3dMaterialCacheEntry::rxUninit();
    ExGiD3dMaterialTextureData::rxUninit();

    OdGsBaseModule::uninitApp();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  {
    OdGsDevicePtr pDev = ExGsScreenDevice<ExGsDirectXDevice>::createObject();
    static_cast<ExGsDirectXDevice*>(pDev.get())->setD3DDllHInst(m_d3dInst);
    // static_cast<ExGsDirectXDevice*>(pDev.get())->setD3DxDllHInst(m_d3dxInst);
    return pDev;
  }

  OdSmartPtr<OdGsViewImpl> createViewObject()
  {
    return OdRxObjectImpl<ExGsDirectXView, OdGsViewImpl>::createObject();
  }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  {
    OdGsDevicePtr pDev = ExGsDirectXBitmapDevice::createObject();
    (static_cast<ExGsDirectXBitmapDevice*>(pDev.get()))->setD3DDllHInst(m_d3dInst);
    // (static_cast<ExGsDirectXBitmapDevice*>(pDev.get()))->setD3DxDllHInst(m_d3dxInst);
    return pDev;
  }

  OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
  {
    return OdRxObjectImpl<ExGsDirectXView, OdGsViewImpl>::createObject();
  }
};

ODRX_DEFINE_DYNAMIC_MODULE(WinDirectXModule);
