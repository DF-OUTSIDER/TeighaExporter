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

// to satisfy VC2005, else it refuses to compile WinDirectX project
#if _MSC_VER >= 1400
  #define POINTER_64 __ptr64
#endif

#include "OdaCommon.h"
#include "ExGsDirectXDevice.h"
#include "StaticRxObject.h"
#include "CmColorBase.h"
#include "SaveState.h"
#include <Gs/LineWtIndex.h>
#include "Gs/GsBaseModel.h"

#include "D3DInclude.h"
#include "../ExGsHelpers.h"

#define STL_USING_LIMITS
#include "OdaSTL.h"

#include "OdRound.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeCircArc2d.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiRasterWrappers.h"
#include "Gi/GiLightTraitsData.h"
#include "Gs/GsViewportProperties.h"

#include "ExOwnerDrawDc.cpp"

// Use DX9 errors, because they are compatible with DX8 and more informative
#define DXGetErrorString      DXGetErrorString9W
#define DXGetErrorDescription DXGetErrorDescription9W
#define DXERROR9(v,n,d)       {v, L##n, L##d},
#define DXERROR9LAST(v,n,d)   {v, L##n, L##d}
#include "DxErr.h"

ODRX_DECLARE_PROPERTY2(DiscardBackFaces, ExGsDirectXDevice)
ODRX_DECLARE_PROPERTY2(D3DSurface, ExGsDirectXDevice)
ODRX_DECLARE_PROPERTY2(LockableBackBuffer, ExGsDirectXDevice)
ODRX_DECLARE_PROPERTY2(RegenCoef, ExGsDirectXDevice)
ODRX_DECLARE_PROPERTY2(UseTTFCache, ExGsDirectXDevice)
ODRX_DECLARE_PROPERTY2(UseLutPalette, ExGsDirectXDevice)
ODRX_DECLARE_PROPERTY2(LineSmoothingSupported, ExGsDirectXDevice)
ODRX_DECLARE_PROPERTY2(RefDevice, ExGsDirectXDevice)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsDirectXDevice);
  ODRX_INHERIT_PROPERTIES(ExGsWin32Device)
  ODRX_GENERATE_PROPERTY2(DiscardBackFaces, ExGsDirectXDevice)
  ODRX_GENERATE_PROPERTY2(D3DSurface, ExGsDirectXDevice)
  ODRX_GENERATE_PROPERTY2(LockableBackBuffer, ExGsDirectXDevice)
  ODRX_GENERATE_PROPERTY2(RegenCoef, ExGsDirectXDevice)
  ODRX_GENERATE_PROPERTY2(UseTTFCache, ExGsDirectXDevice)
  ODRX_GENERATE_PROPERTY2(UseLutPalette, ExGsDirectXDevice)
  ODRX_GENERATE_PROPERTY2(LineSmoothingSupported, ExGsDirectXDevice)
  ODRX_GENERATE_PROPERTY2(RefDevice, ExGsDirectXDevice)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsDirectXDevice);

ODRX_DEFINE_PROPERTY_METHODS2(DiscardBackFaces, ExGsDirectXDevice, ExGsDirectXDevice, discardBackFaces, setDiscardBackFaces, getBool);
ODRX_DEFINE_PROPERTY_METHODS2(D3DSurface, ExGsDirectXDevice, ExGsDirectXDevice, backBufferSurface, setBackBufferSurface, getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS2(LockableBackBuffer, ExGsDirectXDevice, ExGsDirectXDevice, lockableBackBuffer, setLockableBackBuffer, getBool);
ODRX_DEFINE_PROPERTY_METHODS2(RegenCoef, ExGsDirectXDevice, ExGsDirectXDevice, getRegenCoef, setRegenCoef, getDouble);
ODRX_DEFINE_PROPERTY_METHODS2(UseTTFCache, ExGsDirectXDevice, ExGsDirectXDevice, useTTFCache, setUseTTFCache, getBool);
ODRX_DEFINE_PROPERTY_METHODS2(UseLutPalette, ExGsDirectXDevice, ExGsDirectXDevice, useLutPalette, setUseLutPalette, getUInt32);
ODRX_DEFINE_PROPERTY_METHODS2(LineSmoothingSupported, ExGsDirectXDevice, ExGsDirectXDevice, isLinesAntiAliasingSupported, setLinesAntiAliasingSupported, getBool);
ODRX_DEFINE_PROPERTY_METHODS2(RefDevice, ExGsDirectXDevice, ExGsDirectXDevice, isRefDevice, setRefDevice, getBool);

#ifndef ODA_NON_TRACING
static void OdTraceMtx(const OdGeMatrix3d &mtx)
{
  ODA_TRACE(L"OdGeMaterix3d: /* %p */\n", &mtx);
  const double *ptr = reinterpret_cast<const double*>(&mtx);
  ODA_TRACE(L"row01: { %f, %f, %f, %f }\n", ptr[0],  ptr[1],  ptr[2],  ptr[3]);
  ODA_TRACE(L"row02: { %f, %f, %f, %f }\n", ptr[4],  ptr[5],  ptr[6],  ptr[7]);
  ODA_TRACE(L"row03: { %f, %f, %f, %f }\n", ptr[8],  ptr[9],  ptr[10], ptr[11]);
  ODA_TRACE(L"row04: { %f, %f, %f, %f }\n", ptr[12], ptr[13], ptr[14], ptr[15]);
  ODA_TRACE(L"/* - - - */\n");
}
#define ODA_TRACE_MTX(mtx) OdTraceMtx(mtx)
#else
#define ODA_TRACE_MTX(mtx)
#endif

OdString DXError::DXErrorCtx::formatHRESULTErrorInfo() const
{
  OdString s, type;
  if (m_bDXAPI)
    type = OD_T("DirectX API error");
  else
    type = OD_T("WinDirectX vectorizer internal error");
  s.format(OD_T("%s (%s (HRESULT: 0x%X (%s)))"), type.c_str(), DXGetErrorDescription(m_hrCode), m_hrCode, DXGetErrorString(m_hrCode));
  return s;
}

ExGsDirectXDevice::ExGsDirectXDevice()
: m_bContextValid(false)
, m_bDiscardBackFaces(false)
, m_bLockableBackBuffer(false)
, m_bUseTTFCache(false)
, m_bRefDevice(false)
, m_nUseLutPalette(3)
, m_d3dInst(NULL)
{
  setSortRenderTypes(true);
#ifdef OD_D3D_DYNAMICSUBENTHLT // for compatibility with previous behavior (enable dynamicSubEntHlt via OD_D3D_DYNAMICSUBENTHLT preprocessor definition)
  setDynamicSubEntHltEnabled(true);
#endif
  SETBIT_1(m_flags, kDynamicHighlight);
  m_pTexManager = OdGiMaterialTextureManager::createObject();
  m_lsManager.appendEntry(&m_sharingProv);
  setSupportInteractiveViewMode( true );
}

ExGsDirectXDevice::~ExGsDirectXDevice()
{
  if(m_bContextValid)
    deleteContext();
  m_lsManager.removeEntry(&m_sharingProv);
}

void ExGsDirectXDevice::setD3DDllHInst(HINSTANCE hInst)
{
  m_d3dInst = hInst;
}

typedef IDirect3D* (CALLBACK*INITPROC)(UINT );

void ExGsDirectXDevice::createContext()
{
#ifndef OD_USE_DIRECT3D9
  INITPROC initProc = (INITPROC)::GetProcAddress(m_d3dInst, "Direct3DCreate8");
#else
  INITPROC initProc = (INITPROC)::GetProcAddress(m_d3dInst, "Direct3DCreate9");
#endif
  if(!initProc || (NULL == (m_pD3D = (*initProc)(D3D_SDK_VERSION))))
    DXError::fail(E_POINTER);
  else
  { // #11838 : memory leak in DirectX device. IDirect3D interface doesn't released and keep GDI Objects.
    // Reference on usage of DirectX with CComPtr: http://doc.51windows.net/Directx9_SDK/?url=/directx9_sdk/intro/program/com/usingatlwithdx9.htm
    ODA_VERIFY(((IDirect3D*)m_pD3D)->Release() == 1); // Release (1 AddRef in Direct3DCreate, 2 AddRef in CComPtr)
  }

  // Get the current desktop display mode
  D3DDISPLAYMODE d3ddm;
  DXError::check( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) );
  
  // Set up the structure used to create the D3DDevice. Most parameters are
  // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
  // window, and then set the SwapEffect to "D3DSWAPEFFECT_COPY_VSYNC", 
  // because it is necessary in DD 1.14.02 - back buffer is used now as frame cache
  D3DPRESENT_PARAMETERS d3dpp; 
  ZeroMemory( &d3dpp, sizeof(d3dpp) );
  d3dpp.Windowed = TRUE;
#ifndef OD_USE_DIRECT3D9
  d3dpp.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
#else
  d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
  d3dpp.BackBufferCount = 1;
#endif
  d3dpp.BackBufferFormat = d3ddm.Format;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//D3DFMT_D16;
  d3dpp.Flags = lockableBackBuffer() ? D3DPRESENTFLAG_LOCKABLE_BACKBUFFER : 0;

  // Try to get HWND from HDC if available.
  if (m_hWnd == NULL && m_hDC != NULL)
    m_hWnd = ::WindowFromDC(m_hDC);

  {
    // This fix error on CreateDevice, caused by OdaDgnApp (#7276)
    RECT rect;
    ::GetWindowRect(m_hWnd, &rect);
    if (rect.right - rect.left < 1 || rect.bottom - rect.top < 1)
    {
      if (rect.right == rect.left)
        rect.right++;
      if (rect.bottom == rect.top)
        rect.bottom++;
      ::SetWindowPos(m_hWnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
                     SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    }
  }

  // Create the Direct3D device. Here we are using the default adapter (most
  // systems only have one, unless they have multiple graphics hardware cards
  // installed) and requesting the HAL (which is saying we want the hardware
  // device rather than a software one). We specify hardware vertex processing - on cards 
  // that support it we will see a big performance gain; on cards that doesn't support
  // hardware vertex processing this won't work.
  D3DDEVTYPE devType = (!isRefDevice()) ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF;
  if(FAILED( m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, devType, m_hWnd,
    D3DCREATE_FPU_PRESERVE | D3DCREATE_HARDWARE_VERTEXPROCESSING |
    ((m_vectPerfData.enableParallelVectorization()) ? D3DCREATE_MULTITHREADED : 0),
    &d3dpp, &m_pD3Ddevice) ) )
  {
    // some videocards (e.g. Intel 82915G/82910GL) doesn't support hardware vertexprocessing
    DXError::check( m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, devType, m_hWnd,
      D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING |
      ((m_vectPerfData.enableParallelVectorization()) ? D3DCREATE_MULTITHREADED : 0),
      &d3dpp, &m_pD3Ddevice) );
  }

  /* D3DCAPS caps;
  m_pD3Ddevice->GetDeviceCaps(&caps);
  if(caps.VertexShaderVersion < D3DVS_VERSION(1,1))
    DXError::fail();*/

  m_stateManager.SetDevice(m_pD3Ddevice);
  m_stateManager.Init();

  m_lsManager.setD3DDevice(m_pD3Ddevice);
  m_stateManager.SetLostStateManager(&m_lsManager);
  m_lsManager.enableManager(true); // Disable lost state management if device can't be losted or memory low for resources copying

  // Turn off culling, so we see the front and back of the triangle
  m_stateManager.SetCullModeRenderState(D3DCULL_NONE);

  // Turn off D3D lighting, since we are providing our own vertex colors
  m_stateManager.SetLightingRenderState(FALSE);
  
  m_stateManager.SetZFuncRenderState(D3DCMP_LESSEQUAL);
  
  m_bContextValid = true;
}

void ExGsDirectXDevice::deleteContext()
{
  m_pTexManager->clear();
  m_bContextValid = false;
}

void ExGsDirectXDevice::onSize(const OdGsDCRect& outputRect)
{
  OdGsBaseVectorizeDevice::onSize(outputRect);
  
  if(m_bContextValid)
  {
    HRESULT coLevel = m_pD3Ddevice->TestCooperativeLevel();
    if (coLevel == D3DERR_DRIVERINTERNALERROR)
      DXError::fail(coLevel);
    else if (coLevel == D3DERR_DEVICELOST)
      return; // Can't render anything in Lost Device state

    // Get the current desktop display mode
    D3DDISPLAYMODE d3ddm;
    DXError::check( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) );
    
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = d3ddm.Format;
    
    d3dpp.BackBufferWidth  = odmax(outputRect.m_max.x, outputRect.m_min.x) - odmin(outputRect.m_max.x, outputRect.m_min.x);
    d3dpp.BackBufferHeight = odmax(outputRect.m_max.y, outputRect.m_min.y) - odmin(outputRect.m_max.y, outputRect.m_min.y);
    d3dpp.Flags = lockableBackBuffer() ? D3DPRESENTFLAG_LOCKABLE_BACKBUFFER : 0;
    
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//D3DFMT_D16;
    if (m_pD3Ddevice->Reset(&d3dpp) == D3DERR_DEVICELOST)
      coLevel = D3DERR_DEVICELOST;
    m_stateManager.Init();
    m_stateManager.SetCullModeRenderState(D3DCULL_NONE); // restore setting that is default for us
    // Restore D3D resources
    if (coLevel == D3DERR_DEVICENOTRESET)
    {
      m_lsManager.executeRecreation();
    }
  }
}

struct D3DIdentityMatrix : public D3DMATRIX
{
  D3DIdentityMatrix()
  {
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 =
    _21 = _23 = _24 =
    _31 = _32 = _34 =
    _41 = _42 = _43 = 0.0f;
  }
};

D3DIdentityMatrix kD3DIdentityMatrix;

bool ExGsDirectXDevice::supportPartialUpdate() const
{
  return true;
}

void ExGsDirectXDevice::update(OdGsDCRect* pUpdatedRect)
{
  m_vectPerfData.m_options = (supportMultithreading()) ? eEnableParallelVectorization : 0;
  if (forcePartialUpdateForMt()) m_vectPerfData.m_options |= eForcePartialUpdateForTest;
  mtDeviceSync().setEnabled(m_vectPerfData.enableParallelVectorization());
  if (m_vectPerfData.enableParallelVectorization() && !gsMultithreadEnabled())
    setMaxMtRegenThreads(getMtRegenThreadsCount(userGiContext()));

  if(!m_bContextValid)
    createContext();
  else
  {
    HRESULT coLevel = m_pD3Ddevice->TestCooperativeLevel();
    switch (coLevel)
    {
      case D3DERR_DRIVERINTERNALERROR:
        DXError::fail(coLevel);
      break;
      case D3DERR_DEVICELOST:
      return; // Can't render anything in Lost Device state
      case D3DERR_DEVICENOTRESET:
        onSize(m_outputRect);
      break;
    };
  }

  if (!supportPartialUpdate())
  {
    setDoubleBufferEnabled(true); // Direct3D device is always with double buffer
    DWORD bgc = getBackgroundColor();
    m_pD3Ddevice->Clear( 0L, NULL, D3DCLEAR_TARGET, bgc, 1.0f, 0L );
    m_pD3Ddevice->BeginScene();
  }
  ExGsBaseDirectXDevice::update(pUpdatedRect);
  if (!supportPartialUpdate())
  {
    m_pD3Ddevice->EndScene();
    if(!userGiContext()->regenAbort())
      m_pD3Ddevice->Present( NULL, NULL, NULL, NULL );
  }
}

void ExGsDirectXDevice::updateScreen()
{
  if(!isValid())
  {
    setDoubleBufferEnabled(true); // Direct3D device is always with double buffer
    
    IDirect3DDevice* p3DD = m_pD3Ddevice;
    OdD3DStateManager* pSM = &m_stateManager;
    p3DD->BeginScene();
    
    DWORD bgc = getBackgroundColor();

    // for debugging
    //bgc = 0xFF000000 | SWAPBYTES(RGB(rand() * 255 / RAND_MAX, rand() * 255 / RAND_MAX, rand() * 255 / RAND_MAX));

    const OdGsDCRectArray& invRects = invalidRects();
    OdUInt32 i = invRects.size();
    if(i)
    {
      OdGeMatrix3d deviceToNormDC;
      OdGsDCRect screenRect = outputRect();
      deviceToNormDC(0,0) = (screenRect.m_max.x - screenRect.m_min.x);
      deviceToNormDC(1,1) = (screenRect.m_max.y - screenRect.m_min.y);
      deviceToNormDC(0,3) = screenRect.m_min.x;
      deviceToNormDC(1,3) = screenRect.m_min.y;
      deviceToNormDC.invert();
      OdGeMatrix3d correction;
      correction(0,0) = 2.0;
      correction(1,1) = 2.0;
      // correction(2,2) = -1.0;
      correction(0,3) = correction(1,3) = -1.0;
      //correction(2,3) = std::numeric_limits<float>::epsilon();
      
      OdGeMatrix3d pixels2DxCoords = correction * deviceToNormDC;
      
      DXError::check( p3DD->SetTransform(D3DTS_PROJECTION, &kD3DIdentityMatrix) );
      DXError::check( p3DD->SetTransform(D3DTS_WORLD, &kD3DIdentityMatrix) );
      
      pSM->SetZEnableRenderState(D3DZB_FALSE);
      pSM->SetStencilEnableRenderState(FALSE);
      pSM->SetSrcBlendRenderState(D3DBLEND_ONE);
      pSM->SetDestBlendRenderState(D3DBLEND_ZERO);

      DWORD lastLightingState = pSM->GetLightingRenderState();
      pSM->SetLightingRenderState(FALSE);
      
      OdD3DVertexCol triData[4];
      const OdGsDCRect* pInvRect = invRects.getPtr();
      OdGePoint3d pt;
      pSM->SetTexture(0, NULL);
      pSM->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
      
      triData[0].color =
        triData[1].color =
        triData[2].color =
        triData[3].color = bgc;
      
      do
      {
        pt.set(pInvRect->m_min.x, pInvRect->m_min.y, 1.0).transformBy(pixels2DxCoords);
        triData[0].x = float(pt.x);
        triData[0].y = float(pt.y);
        triData[0].z = float(pt.z);
        pt.set(pInvRect->m_min.x, pInvRect->m_max.y, 1.0).transformBy(pixels2DxCoords);
        triData[1].x = float(pt.x);
        triData[1].y = float(pt.y);
        triData[1].z = float(pt.z);
        pt.set(pInvRect->m_max.x, pInvRect->m_max.y, 1.0).transformBy(pixels2DxCoords);
        triData[2].x = float(pt.x);
        triData[2].y = float(pt.y);
        triData[2].z = float(pt.z);
        pt.set(pInvRect->m_max.x, pInvRect->m_min.y, 1.0).transformBy(pixels2DxCoords);
        triData[3].x = float(pt.x);
        triData[3].y = float(pt.y);
        triData[3].z = float(pt.z);
        ++pInvRect;
        
        p3DD->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, triData, sizeof(OdD3DVertexCol));
      }
      while(--i);
      pSM->SetZFuncRenderState(D3DCMP_LESSEQUAL);
      pSM->SetLightingRenderState(lastLightingState);
    }
    else
    {
      p3DD->Clear( 0L, NULL, D3DCLEAR_TARGET, bgc, 1.0f, 0L );
    }
    
    ExGsBaseDirectXDevice::updateScreen();
    
    // End the scene
    p3DD->EndScene();
  }
  // Present the backbuffer contents to the display
  if(!userGiContext()->regenAbort())
    m_pD3Ddevice->Present( NULL, NULL, NULL, NULL );
}

#include "../WinComposite/ExWinCompositeBitBltOp.h"

void ExGsDirectXDevice::makeBitBltOp(const ExWinCompositeBitBltOp &bitBltOp)
{
  CComPtr<IDirect3DSurface> pSurf;
  DXError::check(d3dDevice()->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurf));
  HDC hDC;
  DXError::check(pSurf->GetDC(&hDC));
  ::BitBlt(bitBltOp.m_hdc, bitBltOp.m_x, bitBltOp.m_y, bitBltOp.m_cx, bitBltOp.m_cy, hDC, bitBltOp.m_x1, bitBltOp.m_y1, SRCCOPY);
  pSurf->ReleaseDC(hDC);
}

CComPtr<IDirect3DDevice> ExGsDirectXDevice::d3dDevice()
{
  return m_pD3Ddevice;
}

OdD3DStateManager &ExGsDirectXDevice::d3dStateManager()
{
  return m_stateManager;
}

OdColorConverterCallback *ExGsDirectXDevice::getColorConverter()
{
  if (!properties().isNull() && properties()->has(OD_T("ColorConverter")))
    return OdColorConverterCallbackPtr(properties()->getAt(OD_T("ColorConverter")));
  return NULL;
}

DWORD ExGsDirectXDevice::getBackgroundColor()
{
  ODCOLORREF bkgndColor = OdGsBaseVectorizeDevice::getBackgroundColor();
  OdColorConverterCallback *pConverter = getColorConverter();
  if (pConverter && pConverter->convertBackgroundColors())
    bkgndColor = pConverter->convert(bkgndColor);
  return ExGiDirectXGeometry::getD3DColor(bkgndColor);
}

OdIntPtr ExGsDirectXDevice::backBufferSurface() const
{
  if (m_pD3Ddevice)
  {
    IDirect3DSurface *pSurf = NULL;
    m_pD3Ddevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurf);
    return (OdIntPtr)pSurf;
  }
  return NULL;
}

double ExGsDirectXDevice::getRegenCoef() const
{
  return ExFPPrecisionFix::getRegenCoef(const_cast<ExGsDirectXDevice&>(*this),
                                        ExFPPrecisionFix::GsViewImplToFPFixCvtImpl<ExGsDirectXView>());
}

bool ExGsDirectXDevice::isLinesAntiAliasingSupported() const
{
#ifndef OD_USE_DIRECT3D9
  return false;
#else
  if (m_pD3Ddevice)
  {
    D3DCAPS9 devCaps;
    ZeroMemory(&devCaps, sizeof(D3DCAPS));
    m_pD3Ddevice->GetDeviceCaps(&devCaps);
    return GETBIT(devCaps.LineCaps, D3DLINECAPS_ANTIALIAS);
  }
  return false;
#endif
}

///////////////// View ////////////////////////////////////////////////

ExGsDirectXView::ExGsDirectXView()
  : OdGsBaseMaterialViewMT(kProcessMappersAndMaterials | kEnableDelayCache)
  , m_bSelecting(false)
  , m_numLights(0)
  , m_bFading(false)
  , m_nCurFading(0)
  , m_bDrawTransparency(true)
{
  m_pGeometry = ExGiDirectXGeometry::createObject();
  if (m_pSelectXfm.isNull())
  {
    m_pSelectXfm = OdGiXform::createObject();
    m_pSelectXfm->input().addSourceNode(m_mfEntryPoint);
  }
}

ExGsDirectXView::~ExGsDirectXView()
{
}

ExGiDirectXGeometry& ExGsDirectXView::geometry()
{
  return *(m_pGeometry.get());
}

// delegate to geometry
OdRxObjectPtr ExGsDirectXView::newGsMetafile()
{
  return m_pGeometry->newGsMetafile();
}

void ExGsDirectXView::beginMetafile(OdRxObject* pMetafile)
{
  OdGsBaseMaterialVectorizer::beginMetafile(pMetafile);
  m_pGeometry->beginMetafile(pMetafile, this);
}

void ExGsDirectXView::endMetafile(OdRxObject* pMetafile)
{
  if( baseDevice()->getUpdateManager().isNull() )
  {
    m_pGeometry->endMetafile(pMetafile, NULL);
  }
  else
  {
    m_pGeometry->endMetafile(pMetafile, &m_nMetafileSize );
  }
  OdGsBaseMaterialVectorizer::endMetafile(pMetafile);
  if (gsWriter().gsModel())
  { // Avoid caching of object with invalid extents: #6193
    OdGeExtents3d extents;
    const double dLimit = 1.e30;
    if (gsExtentsAccum().getExtents(extents))
    {
      // m_pOutputExtents->getExtents(extents); // if model disabled
      extents.transformBy(worldToMetafileMatrix());
      double diaLen = (extents.maxPoint() - extents.minPoint()).lengthSqrd();
      if (diaLen > dLimit * dLimit)
        m_pGeometry->clearMetafile(pMetafile);
    }
  }
}

void ExGsDirectXView::playMetafile(const OdRxObject* pMetafile)
{
  switch( metafilePlayMode() )
  {
    case kMfDisplay:
      resetFading(isFaded());
      m_pGeometry->playMetafile(pMetafile);
      m_pGeometry->present();
    break;
    case kMfSelect:
    case kMfExtents:
      {
        D3dPlay4SelectContext ctx;
        ctx.pGeometry = &metafileEntryPoint();
        ctx.pGeometryContext = OdGiBaseVectorizer::drawContext();
        ctx.bCheckMarkFlag = ( metafilePlayMode() == kMfSelect ) ? true : false;
        m_pGeometry->play4Select(pMetafile, ctx);
      }
    break;
    default:
      ODA_FAIL();
  }
}

inline void GeMtxToD3DMtx(const OdGeMatrix3d &mtx, D3DXMATRIX &d3dMtx)
{
  int i, j;
  for(i = 0; i < 4; i ++)
  {
    for(j = 0; j < 4; j ++)
    {
      d3dMtx(i, j) = (float)mtx[j][i];
    }
  }
}

inline const DWORD &DxFloatAsDword(const FLOAT &tmp)
{
  return *((const DWORD*)(&tmp));
}

void ExGsDirectXView::pushMetafileTransform(const OdGeMatrix3d& mtx, OdUInt32 uFlags)
{
  bool isTransformForSelect = false;
  isTransformForSelect = dynamicSubEntHlt() && ( m_pSelectProc || (&(m_pSelectXfm->output().destGeometry()) != &(OdGiEmptyGeometry::kVoid)) );
  if( !isTransformForSelect )
  {
    pushMetafileTransformForUpdate(*this, mtx, uFlags);
    if (pushMetafileTransformForDisplay(*this, mtx, uFlags))
    {
      D3DXMATRIX viewMtx;
      GeMtxToD3DMtx(mfTransformStack().getCompositeTransform(), viewMtx);
      DXError::check( device()->d3dDevice()->SetTransform(D3DTS_WORLD, &viewMtx) );
    }
  }
  else
  {
    if( pushMetafileTransformForSelect(*this, mtx, uFlags) )
      m_pSelectXfm->setTransform(mfTransformStack().getCompositeTransform());
  }
}

void ExGsDirectXView::popMetafileTransform(OdUInt32 uFlags)
{
  bool isTransformForSelect = false;
  isTransformForSelect = dynamicSubEntHlt() && ( m_pSelectProc || (&(m_pSelectXfm->output().destGeometry()) != &(OdGiEmptyGeometry::kVoid)) );
  if( !isTransformForSelect )
  {
    popMetafileTransformForUpdate(*this, uFlags);
    if (popMetafileTransformForDisplay(*this, uFlags))
    {
      D3DXMATRIX viewMtx;
      GeMtxToD3DMtx(mfTransformStack().getCompositeTransform(), viewMtx);
      DXError::check( device()->d3dDevice()->SetTransform(D3DTS_WORLD, &viewMtx) );
    }
  }
  else
  {
    if( popMetafileTransformForSelect(*this, uFlags) )
      m_pSelectXfm->setTransform(mfTransformStack().getCompositeTransform());
  }
}

void ExGsDirectXView::xformByMetafileTransform(OdGeMatrix3d &mtx) const
{
  if (mfTransformStack().hasStack())
    mtx.preMultBy(mfTransformStack().inverseMetafileTransform());
}

void ExGsDirectXView::xformByMetafileTransform(OdGeVector3d &vec) const
{
  if (mfTransformStack().hasStack())
    vec.transformBy(mfTransformStack().inverseMetafileTransform());
}

bool ExGsDirectXView::usesDept(double* pMinDeptSupported, double* pMaxDeptSupported) const
{
  if(pMinDeptSupported)
    *pMinDeptSupported = -1e20;
  if(pMaxDeptSupported)
    *pMaxDeptSupported =  1e20;
  return true;
}

bool ExGsDirectXView::sceneDept(double& zNear, double& zFar) const
{
  if(view().mode() == k2DOptimized && view().isFrontClipped()==view().isBackClipped())
  {
    zFar  = -1e20;
    zNear =  1e20;
    return true;
  }
  return OdGsViewImpl::sceneDept(zNear, zFar);
}

OdGeMatrix3d ExGsDirectXView::projectionMatrix_D3D() const
{
  OdGeMatrix3d res;
  double dNear, dFar;
  if(!sceneDept(dNear, dFar))
  {
    dNear = 1e20;
    dFar = -1e20;
  }

  double floatEpsilon = std::numeric_limits<float>::epsilon();
  dNear += floatEpsilon;
  dFar -= floatEpsilon;

  res(0,0) = (view().dcUpperRight().x - view().dcLowerLeft().x) / view().fieldWidth();
  res(1,1) = (view().dcUpperRight().y - view().dcLowerLeft().y) / view().fieldHeight();
  res(2,2) = 1.0 / (dFar - dNear);
  res(0,3) = (view().dcUpperRight().x + view().dcLowerLeft().x) / 2.0;
  res(1,3) = (view().dcUpperRight().y + view().dcLowerLeft().y) / 2.0;
  res(2,3) = -dNear / (dFar - dNear);

  if(view().isPerspective())
    res *= view().perspectiveMatrix();

  return res;
}

inline void ExGsDirectXView::getDXScreenRect(double& left, double& top, double& width, double& height)
{
  // get client screen rect (take into account DC subdividing)
  OdGePoint3d ll, ur;
  view().getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
  OdGeMatrix3d x = view().screenMatrix();
  ll.transformBy(x);
  ur.transformBy(x);

  left   = odmin(ll.x, ur.x);
  top    = odmin(ll.y, ur.y);
  width  = odmax(ll.x, ur.x) - left;
  height = odmax(ll.y, ur.y) - top;
}

void ExGsDirectXView::setViewportClip(const OdGeMatrix3d& pixels2DxCoords)
{
  IDirect3DDevice* p3DD = device()->d3dDevice();
  OdD3DStateManager *pSM = &(device()->d3dStateManager());
  DWORD zState = pSM->GetZEnableRenderState();

  OdGsDCRectArray invRects = view().invalidRects();
  OdUInt32 i = invRects.size();
  pSM->SetSrcBlendRenderState(D3DBLEND_ZERO);
  pSM->SetDestBlendRenderState(D3DBLEND_ONE);
  pSM->SetTexture(0, NULL);
  pSM->SetAlphaTestEnableRenderState(FALSE);

  if(device()->supportPartialUpdate() && i)
  {
    pSM->SetZWriteEnableRenderState(TRUE);
    pSM->SetStencilEnableRenderState(FALSE);

    p3DD->Clear( 0L, NULL, D3DCLEAR_STENCIL|D3DCLEAR_ZBUFFER, 0L, 0.0f, 0L );
    pSM->SetZEnableRenderState(D3DZB_TRUE);
    pSM->SetZFuncRenderState(D3DCMP_GREATER);

    D3DVECTOR triData[4];
    const OdGsDCRect* pInvRect = invRects.getPtr();
    OdGePoint3d pt;
    pSM->SetFVF(D3DFVF_XYZ);

    do
    {
      pt.set(pInvRect->m_min.x, pInvRect->m_min.y, 1.0).transformBy(pixels2DxCoords);
      triData[0].x = float(pt.x);
      triData[0].y = float(pt.y);
      triData[0].z = float(pt.z);
      pt.set(pInvRect->m_min.x, pInvRect->m_max.y, 1.0).transformBy(pixels2DxCoords);
      triData[1].x = float(pt.x);
      triData[1].y = float(pt.y);
      triData[1].z = float(pt.z);
      pt.set(pInvRect->m_max.x, pInvRect->m_max.y, 1.0).transformBy(pixels2DxCoords);
      triData[2].x = float(pt.x);
      triData[2].y = float(pt.y);
      triData[2].z = float(pt.z);
      pt.set(pInvRect->m_max.x, pInvRect->m_min.y, 1.0).transformBy(pixels2DxCoords);
      triData[3].x = float(pt.x);
      triData[3].y = float(pt.y);
      triData[3].z = float(pt.z);
      ++pInvRect;

      p3DD->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, triData, sizeof(D3DVECTOR));
    }
    while(--i);
    pSM->SetZFuncRenderState(D3DCMP_LESSEQUAL);
  }
  else
  {
    p3DD->Clear( 0L, NULL, D3DCLEAR_STENCIL|D3DCLEAR_ZBUFFER, 0L, 1.0f, 0L );
  }
  int j;
  OdGePoint2dArray clipPoints;
  OdIntArray ptCounts;
  view().viewportClipRegion(ptCounts, clipPoints);
  if(clipPoints.empty())
  {
    double left, top, width, height;
    getDXScreenRect(left, top, width, height);
    left   -= 1.0; // to prevent artifacts
    top    -= 1.0;
    width  += 1.0;
    height += 1.0;
    clipPoints.append()->set(left, top);
    clipPoints.append()->set(left, top+height);
    clipPoints.append()->set(left+width, top+height);
    clipPoints.append()->set(left+width, top);
    ptCounts.push_back(4);
  }
  ExGiDirectXGeometry& geom = geometry();

  OdCmEntityColor prevTrueColor = subEntityTraits().trueColor();
  OdCmTransparency prevTransparency = subEntityTraits().transparency();
  OdGiFillType prevFillType = subEntityTraits().fillType();
  OdDb::LineWeight prevLWD = subEntityTraits().lineWeight();

  subEntityTraits().setTrueColor(OdCmEntityColor(255, 0, 255));
  subEntityTraits().setTransparency(OdCmTransparency(OdUInt8(0xFF)));
  subEntityTraits().setFillType(kOdGiFillAlways);
  subEntityTraits().setLineWeight(prevLWD);
  onTraitsModified();
  geom.setForceFill(true); // temporary set 2d optimized render mode, to make
                           // polygon (stencil) filled even in case Wireframe
  pSM->SetZFuncRenderState(D3DCMP_LESSEQUAL);
  pSM->SetZEnableRenderState(D3DZB_TRUE);
  pSM->SetZWriteEnableRenderState(FALSE);

  pSM->SetStencilEnableRenderState(TRUE);
  pSM->SetStencilFuncRenderState(D3DCMP_GREATER);
  pSM->SetStencilRefRenderState(0x1);
  pSM->SetStencilMaskRenderState(0xffffffff);
  pSM->SetStencilWriteMaskRenderState(0xffffffff);
  pSM->SetStencilZFailRenderState(D3DSTENCILOP_KEEP); // AND invalid area
  pSM->SetStencilFailRenderState(D3DSTENCILOP_ZERO); // XOR itself
  pSM->SetStencilPassRenderState(D3DSTENCILOP_REPLACE);

  pSM->SetAlphaBlendEnableRenderState(TRUE);

  OdUInt32 numVerts;
  OdGePoint3dArray newVerts(clipPoints.size());
  for(j = 0, i = 0; j < (int)ptCounts.size(); j ++)
  {
    numVerts = ptCounts[j];
    newVerts.resize(numVerts);
    OdUInt32 i0 = i;
    for(; i < i0+numVerts; i ++)
      newVerts[i-i0].set(clipPoints[i].x, clipPoints[i].y, 1.0).transformBy(pixels2DxCoords);

    // Draw into the stencil and dept buffer (polygons are tesselated if necessary)
    geom.polygonOut(numVerts, newVerts.asArrayPtr());
  }
  
  // turn off writing to stencil, turn on writing to frame buffer
  pSM->SetStencilFuncRenderState(D3DCMP_EQUAL);
  pSM->SetStencilPassRenderState(D3DSTENCILOP_KEEP);
  pSM->SetStencilFailRenderState(D3DSTENCILOP_KEEP);
  pSM->SetStencilZFailRenderState(D3DSTENCILOP_KEEP);
  pSM->SetSrcBlendRenderState(D3DBLEND_ONE);
  pSM->SetDestBlendRenderState(D3DBLEND_ZERO);

  pSM->SetZWriteEnableRenderState(TRUE);
  pSM->SetZEnableRenderState(zState);

  geom.setForceFill(false);
  subEntityTraits().setLineWeight(prevLWD);
  subEntityTraits().setFillType(prevFillType);
  subEntityTraits().setTransparency(prevTransparency);
  subEntityTraits().setTrueColor(prevTrueColor);
  onTraitsModified();

  // Setup clipping for hidden line
  if (view().mode() == kHiddenLine)
  {
    pSM->SetStencilFuncRenderState(D3DCMP_LESSEQUAL);
    pSM->SetStencilPassRenderState(D3DSTENCILOP_REPLACE);
  }
  //
}

void ExGsDirectXView::viewportTransforms(D3DXMATRIX &matView, D3DXMATRIX &matProjection, OdGeMatrix3d &pixels2DxCoords, bool bNoDept)
{
  OdGeMatrix3d deviceToNormDC;
  OdGsDCRect screenRect = device()->outputRect();
  //deviceToNormDC(0,0) = (screenRect.m_max.x - screenRect.m_min.x);
  //deviceToNormDC(1,1) = (screenRect.m_max.y - screenRect.m_min.y);
  //deviceToNormDC(0,3) = screenRect.m_min.x;
  //deviceToNormDC(1,3) = screenRect.m_min.y;
  // #9099 : inversion will be included in screenMatrix, so we must skip it here
  deviceToNormDC(0,0) = odmax(screenRect.m_min.x, screenRect.m_max.x) - odmin(screenRect.m_min.x, screenRect.m_max.x);
  deviceToNormDC(1,1) = odmin(screenRect.m_min.y, screenRect.m_max.y) - odmax(screenRect.m_min.y, screenRect.m_max.y);
  deviceToNormDC(0,3) = odmin(screenRect.m_min.x, screenRect.m_max.x);
  deviceToNormDC(1,3) = odmax(screenRect.m_min.y, screenRect.m_max.y);
  // eof #9099
  deviceToNormDC.invert();
  OdGeMatrix3d correction;
  correction(0,0) = 2.0;
  correction(1,1) = 2.0;
  // correction(2,2) = -1.0;
  correction(0,3) = correction(1,3) = -1.0;
  //correction(2,3) = std::numeric_limits<float>::epsilon();
  OdGeMatrix3d viewMtx, projectionMtx;
  viewMtx = viewportToOutputMatrix();
  pixels2DxCoords = correction * deviceToNormDC;
  projectionMtx = pixels2DxCoords * view().screenMatrix() * view().projectionMatrix((bNoDept) ? kProjectionIncludeAllNoDept : kProjectionIncludeAll);

  int i, j;
  for(i = 0; i < 4; i ++)
  {
    for(j = 0; j < 4; j ++)
    {
      matView(i, j) = (float)viewMtx[j][i];
      matProjection(i, j) = (float)projectionMtx[j][i];
    }
  }
}

void ExGsDirectXView::loadViewport()
{
  OdGeMatrix3d xEyeToWorld = getEyeToWorldTransform();
  setEyeToOutputTransform(xEyeToWorld);

  geometry().setVectorizer(this);
  OdGeMatrix3d pixels2DxCoords;
  D3DXMATRIX matView, matProjection;
  viewportTransforms(matView, matProjection, pixels2DxCoords);

  RenderMode rm = view().mode();
  IDirect3DDevice* p3DD = device()->d3dDevice();
  OdD3DStateManager *pSM = &(device()->d3dStateManager());
  if(rm > k2DOptimized)
  {
    pSM->SetZEnableRenderState(D3DZB_TRUE);
    if(rm > kHiddenLine)
    {
      pSM->SetLightingRenderState(TRUE);
      pSM->SetNormalizeNormalsRenderState(TRUE);      
      updateLights();
    }
  }
  else
  {
    pSM->SetZEnableRenderState(D3DZB_FALSE);
  }

  if(rm < kHiddenLine)
  {
    pSM->SetLightingRenderState(FALSE);
    pSM->SetNormalizeNormalsRenderState(FALSE);
  }

  DXError::check( p3DD->SetTransform(D3DTS_PROJECTION, &kD3DIdentityMatrix) );

  DXError::check( p3DD->SetTransform(D3DTS_WORLD, &kD3DIdentityMatrix) );

  setViewportClip(pixels2DxCoords);

  if(rm == k2DOptimized)
  {
    pSM->SetAlphaBlendEnableRenderState(TRUE);
    pSM->SetSrcBlendRenderState(D3DBLEND_SRCALPHA);
    pSM->SetDestBlendRenderState(D3DBLEND_INVSRCALPHA);
  }
  else
  {
    pSM->SetSrcBlendRenderState(D3DBLEND_ONE);
    pSM->SetDestBlendRenderState(D3DBLEND_ZERO);
  }

  DXError::check( p3DD->SetTransform(D3DTS_PROJECTION, &matProjection) );

  DXError::check( p3DD->SetTransform(D3DTS_WORLD, &matView) );
  mfTransformStack().setOutputTransform(viewportToOutputMatrix());
}

void ExGsDirectXView::updateLights()
{
  IDirect3DDevice* p3DD = device()->d3dDevice();
  OdD3DStateManager *pSM = &(device()->d3dStateManager());
  // only default case for now
  {
    D3DXVECTOR3 vecDir;
    D3DLIGHT light;
    ZeroMemory(&light, sizeof(D3DLIGHT));
    light.Type       = D3DLIGHT_DIRECTIONAL;
    light.Specular.r = light.Diffuse.r  = 1.0f;
    light.Specular.g = light.Diffuse.g  = 1.0f;
    light.Specular.b = light.Diffuse.b  = 1.0f;
    OdGeVector3d dir(1.0f, -1.0f, -2.0f);
    dir.normalize();
    light.Direction = D3DXVECTOR3((float)dir.x, (float)dir.y, (float)dir.z);
    p3DD->SetLight(0, &light);
    p3DD->LightEnable(0, TRUE);

    // disable all additional lights
    for(OdUInt32 i = 1; i <= m_numLights; i ++)
      p3DD->LightEnable(i, FALSE);
    m_numLights = 0;
  }
  pSM->SetAmbientRenderState(0x00333333);
  pSM->SetSpecularEnableRenderState(TRUE);
  pSM->SetLocalViewerRenderState(TRUE);

  D3DMATERIAL mtrl;
  ZeroMemory( &mtrl, sizeof(D3DMATERIAL) );
  mtrl.Specular.r = 1.0f;
  mtrl.Specular.g = 1.0f;
  mtrl.Specular.b = 1.0f;
  mtrl.Specular.a = 1.0f;
  mtrl.Ambient.r = 0.2f;
  mtrl.Ambient.g = 0.2f;
  mtrl.Ambient.b = 0.2f;
  mtrl.Ambient.a = 1.0f;
  mtrl.Power = 50.0f;
  pSM->SetMaterial(&mtrl);
  pSM->SetAmbientMaterialSourceRenderState(D3DMCS_COLOR1);
  pSM->SetDiffuseMaterialSourceRenderState(D3DMCS_COLOR1);
  pSM->SetSpecularMaterialSourceRenderState(D3DMCS_MATERIAL);
}

void ExGsDirectXView::updatePaletteOverride(const OdGiPalette *pPalette)
{
  OdGsBaseMaterialVectorizer::updatePaletteOverride(pPalette);
  int numColors = 0;
  const ODCOLORREF *pPalColors = getPalette(&numColors);
  m_pGeometry->setLogicalPalette(pPalColors, (OdUInt16)numColors);
}

void ExGsDirectXView::beginViewVectorization()
{
  // Device state would normally be set here
  m_pGeometry->setD3DDevice(device()->d3dDevice());
  m_pGeometry->setD3DStateManager(&(device()->d3dStateManager()));
  m_pGeometry->setDoubleBufferEnabled(device()->doubleBufferEnabled());
  updatePaletteOverride(NULL);
  m_pGeometry->setMaterialViewRedirect(this, this);
  m_pGeometry->setupIntermediateArrays(view().cachedDrawables() > 0);

  OdGsBaseMaterialVectorizer::beginViewVectorization();

  setupForFPVectorization(*this, geometry(), false, isOutputSuppressed(),
                          (isMTView()) ? static_cast<ExGsDirectXView*>(&view()) : NULL);

  geometry().setDrawContext(OdGsBaseVectorizer::drawContext());
  setDrawContextFlags(drawContextFlags() | kForceMarkersOnModified, true);
  geometry().setDeviation(&m_pModelToEyeProc->eyeDeviation());
  setEyeToOutputTransform( getEyeToWorldTransform() );

  m_pGeometry->m_bProcessingTTF = m_pGeometry->m_bShadingDisabled = false;
  m_bDrawTransparency = view().isPlotTransparency() || !giContext().isPlotGeneration();

  if (device()->useTTFCache()) // If TtfFonts cache enabled
    setDrawContextFlags(kSpatialFilterSimplText | kEmbranchmentSimplText, false);
}

void ExGsDirectXView::endViewVectorization()
{
  resetFading(false);
  if (!isOutputSuppressed())
  {
    finalizeFPVectorization(*this);
  }

  OdGsBaseMaterialVectorizer::endViewVectorization();
}

bool ExGsDirectXView::forceMetafilesDependence() const
{
  return OdGsBaseVectorizer::forceMetafilesDependence() || metafileTransformOverrided();
}

bool ExGsDirectXView::metafileTransformOverrided() const
{
  return ExFPPrecisionFix::metafileTransformOverrided();
}

const OdGeMatrix3d &ExGsDirectXView::metafileToWorldMatrix() const
{
  return ExFPPrecisionFix::metafileToWorldMatrix();
}

const OdGeMatrix3d &ExGsDirectXView::worldToMetafileMatrix() const
{
  return ExFPPrecisionFix::worldToMetafileMatrix();
}

const OdGeMatrix3d &ExGsDirectXView::viewportToOutputMatrix() const
{
  return ExFPPrecisionFix::viewportToOutputMatrix();
}

const OdGeMatrix3d &ExGsDirectXView::outputToViewportMatrix() const
{
  return ExFPPrecisionFix::outputToViewportMatrix();
}

double ExGsDirectXView::regenCoef()
{
  return ExFPPrecisionFix::regenCoef(view());
}

//SEA
bool ExGsDirectXView::useMetafileAsGeometry() const
{
  return dynamicSubEntHlt();
}


OdGiConveyorOutput& ExGsDirectXView::outputForMetafileGeometry()
{
  if ( !dynamicSubEntHlt() )
    return OdGsBaseVectorizer::outputForMetafileGeometry();
  else
    return m_pSelectXfm->output();
}


void ExGsDirectXView::setTransformForMetafileGeometry(const OdGeMatrix3d& mtx)
{
  if (!metafileTransformOverrided())
  {
    mfTransformStack().setOutputTransform(mtx);
    m_pSelectXfm->setTransform(mtx);
  }
  else
  {
    // Remove mf sub
    OdGeMatrix3d fixed = mtx * metafileToWorldMatrix();
    mfTransformStack().setOutputTransform(fixed);
    m_pSelectXfm->setTransform(fixed);
  }
}


OdGeMatrix3d ExGsDirectXView::getTransformForMetafileGeometry() const
{
  return mfTransformStack().outputTransform();
}


//////////////// graphics data
DWORD ExGsDirectXView::getD3DColor(ODCOLORREF col)
{
  return 0xFF000000 | SWAPBYTES(col);
}

DWORD ExGsDirectXView::getD3DColor(const OdCmEntityColor& col)
{
  OdCmEntityColor color;
  if(col.isByACI() || col.isByDgnIndex())
    return getD3DColor(paletteColor(col.colorIndex()));
  else
    return getD3DColor(ODTOCOLORREF(col));
}

void ExGsDirectXView::onTraitsModified()
{
  OdGsBaseMaterialVectorizer::onTraitsModified();
  if (isOutputSuppressed())
    return;

  ExGiDirectXGeometry& geom = geometry();
  OdGsView::RenderMode rm = view().mode();

  if( dynamicSubEntHlt() )
  {
    const OdGiSubEntityTraitsData &pTraits = effectiveTraits();
    geom.d3dSelectionFlags( (~(OdUInt8)pTraits.geomVisibilityFlags()) & OdDirectXSelFlag_FlagsMask ); 

    //return;

  }

  if (!isSupportPlotStyles())
  {
    const OdGiSubEntityTraitsData& entTraits = effectiveTraits();

    if (rm == k2DOptimized)
    {
      DWORD clr = getD3DColor(entTraits.trueColor()) & 0x00FFFFFF;
      clr |= (m_bDrawTransparency) ? (((DWORD)entTraits.transparency().alpha()) << 24) : 0xFF000000;
      geom.draw_color(clr);
      geom.draw_transparency(0);
    }
    else
    {
      geom.draw_color(getD3DColor(entTraits.trueColor()));
      if ((rm != OdGsView::kHiddenLine) && m_bDrawTransparency)
        geom.draw_transparency(255 - entTraits.transparency().alpha());
      else
        geom.draw_transparency(0);
    }
    bool bLwDisplay = view().hasLweights() || view().lineweightToDcScale() != 0.0 || hasLineweightOverride();
    if (bLwDisplay)
    {
      if (!hasLineweightOverride())
        geom.draw_lineweight(entTraits.lineWeight(), lineweightToPixels(entTraits.lineWeight()));
      else
        geom.draw_lineweightOverride(lineweightToPixelsOverride(entTraits.lineWeight(), true), lineweightToPixels(entTraits.lineWeight()));
    }
    else
      geom.draw_lineweight(OdDb::kLnWt000, 1); // Avoid dynamic sublists creation if 'lwdisplay' disabled, but now
                                               // regeneration needed if 'lwdisplay' sysvar changed.
    geom.draw_fillStyle(entTraits.fillType()==kOdGiFillAlways ? OdPs::kFsSolid : OdPs::kFsUseObject);
    if (!hasLinestyleOverride())
      geom.draw_lineStyle(true);
    else
      geom.draw_lineStyle(false, currentLineweightOverride()->endStyleOverride(), currentLineweightOverride()->joinStyleOverride());

    const OdUInt32 drawFlags = entTraits.drawFlags();
    geom.m_bProcessingTTF = GETBIT(drawFlags, OdGiSubEntityTraits::kDrawPolygonFill);
    geom.m_bShadingDisabled = geom.m_bProcessingTTF || GETBIT(drawFlags, OdGiSubEntityTraits::kDrawSolidFill);
    geom.draw_noColorMod(GETBIT(drawFlags, OdGiSubEntityTraits::kDrawNoPlotstyle));
  }
  else
  {
    const OdPsPlotStyleData& plotStyle = effectivePlotStyle();

    if (m_bDrawTransparency)
      geom.draw_color((getD3DColor(plotStyle.color()) & 0x00FFFFFF) | (((DWORD)effectiveTraits().transparency().alpha()) << 24));
    else
      geom.draw_color(getD3DColor(plotStyle.color()));
    geom.draw_transparency(0);
    if (!hasLineweightOverride())
      geom.draw_lineweight(plotStyle.lineweight(), (int)OdTruncateToLong(lineweightToPixels(plotStyle.lineweight())));
    else
      geom.draw_lineweightOverride(lineweightToPixelsOverride(plotStyle.lineweight(), true), (int)OdTruncateToLong(lineweightToPixels(plotStyle.lineweight())));
    geom.draw_fillStyle(plotStyle.fillStyle());
    geom.draw_linetype(plotStyle.linetype(), plotStyle.isAdaptiveLinetype(), plotStyle.linePatternSize());
    if (!hasLinestyleOverride())
      geom.draw_lineStyle(false, plotStyle.endStyle(), plotStyle.joinStyle());
    else
      geom.draw_lineStyle(false, currentLineweightOverride()->endStyleOverride(), currentLineweightOverride()->joinStyleOverride());
  }

  if (!view().getViewportPropertiesForType(OdGsProperties::kVisualStyle).isNull())
    geom.draw_visualStyle(((OdGsVisualStylePropertiesPtr)view().getViewportPropertiesForType(OdGsProperties::kVisualStyle))->visualStyleTraitsData()->odgiVisualStyle());
}

void ExGsDirectXView::renderTypeOnChange(OdGsModel::RenderType renderType)
{
  OdGsBaseVectorizer::renderTypeOnChange(renderType);
  if (view().mode() != k2DOptimized) // 2D mode have always disabled Z-buffer
  {
    OdD3DStateManager &stateManager = device()->d3dStateManager();
    const OdUInt32 overlayFlags = OdGsOverlayMapping::overlayFlags(
      OdGsOverlayMapping::overlayRenderingOrderToIndex(device()->renderTypeWeight(renderType)));
    if (!GETBIT(overlayFlags, OdGsOverlayMapping::kNoDepth))
    {
      stateManager.SetZEnableRenderState(D3DZB_TRUE);
      stateManager.SetZWriteEnableRenderState(TRUE);
      if (m_renderMode > kHiddenLine && m_numLights > 0)
      {
        for (OdUInt32 i = 0; i <= m_numLights; i++)
          device()->d3dDevice()->LightEnable(i, (i == 0) ? FALSE : TRUE);
      }
    }
    else
    {
      stateManager.SetZEnableRenderState(D3DZB_FALSE);
      stateManager.SetZWriteEnableRenderState(FALSE);
      if (m_renderMode > kHiddenLine && m_numLights > 0)
      {
        for (OdUInt32 i = m_numLights; i > 0; i--)
        {
          // Should be in reverse order, elsewhere doesn't work on limits
          device()->d3dDevice()->LightEnable(i, FALSE);
        }
        device()->d3dDevice()->LightEnable(0, TRUE);
      }
    }
  }
}

// Materials support

bool ExGsDirectXView::skipMaterialProcess(OdDbStub *materialId) const
{
  return const_cast<ExGsDirectXView*>(this)->geometry().skipMaterialProcess(materialId);
}

OdGiMaterialItemPtr ExGsDirectXView::fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData)
{
  return geometry().fillMaterialCache(prevCache, materialId, materialData);
}

void ExGsDirectXView::renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId)
{
  geometry().renderMaterial(pCache, materialId);
}

void ExGsDirectXView::playDelayCacheEntry(const DelayCacheEntry *pEntry)
{
  geometry().playDelayCacheEntry(pEntry);
}

void ExGsDirectXView::mapperChangedForDelayCache()
{
  OdGsBaseMaterialVectorizer::mapperChangedForDelayCache();
  geometry().mapperChangedForDelayCache();
}

OdGiMaterialItemPtr ExGsDirectXView::geometryCurrentMaterial() const
{
  return currentMaterial();
}

void ExGsDirectXView::geometryResetCurrentMaterial(OdGiMaterialItemPtr pMaterial)
{
  resetCurrentMaterial(pMaterial);
}

bool ExGsDirectXView::geometryIsMaterialEnabled() const
{
  return isMaterialEnabled();
}

bool ExGsDirectXView::geometryIsMaterialAvailable() const
{
  return isMaterialAvailable();
}

void ExGsDirectXView::geometryResetCurrentMaterial()
{
  resetCurrentMaterial();
}

OdGiMapperItemPtr ExGsDirectXView::geometryCurrentMapper(bool bForCoords) const
{
  return currentMapper(bForCoords);
}

void ExGsDirectXView::geometryResetCurrentMapper(OdGiMapperItemPtr pMapper)
{
  resetCurrentMapper(pMapper);
}

bool ExGsDirectXView::geometryIsMapperEnabled() const
{
  return isMapperEnabled();
}

bool ExGsDirectXView::geometryIsMapperAvailable() const
{
  return isMapperAvailable();
}

bool ExGsDirectXView::geometryIsMappingDelayed() const
{
  return isMappingDelayed();
}

void ExGsDirectXView::geometryAppendDelayCacheEntry(void *pEntry)
{
  appendDelayCacheEntry((DelayCacheEntry*)pEntry);
}

void ExGsDirectXView::select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode)
{
  OdSaveState<bool> ss(m_bSelecting, true);
  OdGsBaseMaterialViewMT::select(pts, nPoints, pReactor, mode);
}

void ExGsDirectXView::ownerDrawDc(const OdGePoint3d& _origin,
                                  const OdGeVector3d& _u,
                                  const OdGeVector3d& _v,
                                  const OdGiSelfGdiDrawable* pDrawable,
                                  bool bDcAligned,
                                  bool bAllowClipping)
{
  // todo -- refactor OpenGL device; move "isSelecting" to common rendering
  // functionality
  if (m_bSelecting)
    return;

  onTraitsModified();
  ::exOwnerDrawDc(_origin, _u, _v, pDrawable, (getRenderMode() == k2DOptimized) && bDcAligned, bAllowClipping, *this,
                  GETBIT(m_flags, kModelCache), GETBIT(m_flags, kModelCache));
}

void ExGsDirectXView::text(const OdGePoint3d& position,
                           const OdGeVector3d& normal,
                           const OdGeVector3d& direction,
                           const OdChar* msg,
                           OdInt32 length,
                           bool raw,
                           const OdGiTextStyle* pTextStyle)
{
  m_pGeometry->m_bProcessingTTF = m_pGeometry->m_bShadingDisabled = pTextStyle->isTtfFont();
  OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pTextStyle);
  m_pGeometry->m_bProcessingTTF = m_pGeometry->m_bShadingDisabled = false;
}

void ExGsDirectXView::shell(OdInt32 numVertices,
                            const OdGePoint3d* vertexList,
                            OdInt32 faceListSize,
                            const OdInt32* faceList,
                            const OdGiEdgeData* edgeData,
                            const OdGiFaceData* faceData,
                            const OdGiVertexData* vertexData)
{
  if ((view().mode() >= kFlatShaded) && ::odExDiscardBackfaces(*this, device()->discardBackFaces()) &&
      (!currentHighlightBranch() || device()->discardBackFaces()) &&
      ((faceData && faceData->normals()) || (vertexData && vertexData->normals())))
    m_pGeometry->setTwoSidesMode(false);
  OdGsBaseVectorizer::shell(numVertices, vertexList, faceListSize, faceList, edgeData, faceData, vertexData);
  m_pGeometry->setTwoSidesMode(true);
}

void ExGsDirectXView::appendLight(const OdGiLightTraitsData& traits, D3DLIGHT& light)
{
  ODA_ASSERT(view().mode() > kHiddenLine);

  // Diffuse
  OdCmEntityColor color = traits.color();
  if(color.isByACI() || color.isByDgnIndex())
  {
    ODCOLORREF clrref = paletteColor(color.colorIndex());
    light.Diffuse.r = (float)ODGETRED(clrref) / 255.0f;
    light.Diffuse.g = (float)ODGETGREEN(clrref) / 255.0f;
    light.Diffuse.b =  (float)ODGETBLUE(clrref)  / 255.0f;
  }
  else
  {
    light.Diffuse.r = (float)color.red()   / 255.0f;
    light.Diffuse.g = (float)color.green() / 255.0f;
    light.Diffuse.b = (float)color.blue()  / 255.0f;
  }
  light.Diffuse.a = 1.0f;
  if (traits.type() == OdGiLightTraitsData::kDistantLight)
  {
    // Attenuation doesn't used for directional lights, use color modification instead
    light.Diffuse.r *= (float)traits.intensity();
    light.Diffuse.g *= (float)traits.intensity();
    light.Diffuse.b *= (float)traits.intensity();
  }

  // Specular
  light.Specular.r = light.Specular.g = light.Specular.b = 0.0;

  // Ambient
  light.Ambient.r = light.Ambient.g = light.Ambient.b = 0.0;

  // Range 
  light.Range = (float)sqrt(FLT_MAX);

  // Falloff (exp)
  // light.Falloff = 1.0f;

  m_numLights ++;  
  IDirect3DDevice* p3DD = device()->d3dDevice();
  p3DD->SetLight(m_numLights, &light);
  p3DD->LightEnable(m_numLights, TRUE);
  
  // Disable default light
  if(m_numLights == 1)
    p3DD->LightEnable(0, FALSE);
}

static void setAttenuation(D3DLIGHT& light, const OdGiLightAttenuation& atten, double intensity)
{
  switch(atten.attenuationType())
  {
  case OdGiLightAttenuation::kInverseLinear:
    light.Attenuation1 = (float)(1.0/intensity);
    break;
  case OdGiLightAttenuation::kInverseSquare:
    light.Attenuation2 = (float)(1.0/intensity);
    break;
  case OdGiLightAttenuation::kNone:
    light.Attenuation0 = (float)(1.0/intensity);
    break;
  default:
    ODA_FAIL(); // some unknown attenuation
  }
}

void ExGsDirectXView::addPointLight(const OdGiPointLightTraitsData& traits)
{
  D3DLIGHT light;
  memset(&light, 0, sizeof(light));

  light.Type = D3DLIGHT_POINT;

  OdGePoint3d pos = traits.position();
  pos.transformBy(view().viewingMatrix());
  light.Position.x = (float)pos.x;
  light.Position.y = (float)pos.y;
  light.Position.z = (float)pos.z;

  setAttenuation(light, traits.attenuation(), traits.intensity());

  appendLight(traits, light);
}

void ExGsDirectXView::addSpotLight(const OdGiSpotLightTraitsData& traits)
{
  D3DLIGHT light;
  memset(&light, 0, sizeof(light));

  light.Type = D3DLIGHT_SPOT;

  OdGePoint3d pos = traits.position();
  pos.transformBy(view().viewingMatrix());
  light.Position.x = (float)pos.x;
  light.Position.y = (float)pos.y;
  light.Position.z = (float)pos.z;

  OdGeVector3d direction = traits.target() - traits.position();
  direction.transformBy(view().viewingMatrix());
  direction.normalizeGetLength();
  light.Direction.x = (float)direction.x;
  light.Direction.y = (float)direction.y;
  light.Direction.z = (float)direction.z;

  light.Theta = (float)traits.hotspot();
  light.Phi   = (float)traits.falloff();

  setAttenuation(light, traits.attenuation(), traits.intensity());

  appendLight(traits, light);
}

void ExGsDirectXView::addDistantLight(const OdGiDistantLightTraitsData& traits)
{
  D3DLIGHT light;
  memset(&light, 0, sizeof(light));

  light.Type = D3DLIGHT_DIRECTIONAL;

  OdGeVector3d direction = traits.direction();
  direction.transformBy(view().viewingMatrix());
  direction.normalizeGetLength();
  light.Direction.x = (float)direction.x;
  light.Direction.y = (float)direction.y;
  light.Direction.z = (float)direction.z;

  // Directional lights doesn't apply any attenuation paramaters
  //OdGiLightAttenuation atten;
  //atten.setAttenuationType(OdGiLightAttenuation::kNone);
  //setAttenuation(light, atten, traits.intensity());

  appendLight(traits, light);
}

void ExGsDirectXView::resetFading(bool bFlag)
{
  if ((m_bFading != bFlag) || (bFlag && (m_nCurFading != (OdUInt8)fadingIntensity())))
  {
    m_bFading = bFlag;
    if (bFlag)
    {
      OdGeExtents3d we;
      GsViewImplHelper::getExtents(*this, kGsMainOverlay, we); // in output coordinates - i. e. world
      double zNear, zFar;
      if (!we.isValidExtents())
        zNear = zFar = 0.0;
      else
      {
        we.transformBy(worldToEyeMatrix());
        zNear = we.minPoint().z; zFar = we.maxPoint().z;
      }
      if (zFar < zNear)
        std::swap(zNear, zFar);
      if (OdEqual(zNear, zFar))
        zNear -= 1.0, zFar += 1.0;
      if (zNear < -1e20) zNear = -1e20;
      if (zFar > 1e20) zFar = 1e20;
      double zScale = zFar - zNear;
      double coef = (m_nCurFading = (OdUInt8)fadingIntensity()) * 0.01;
      double fogNear = zNear - zScale * coef * 255;
      double fogFar = zFar + zScale * (1.0 - coef) * 255;
      if (fogFar < 0.0)
        fogNear = -fogNear, fogFar = -fogFar;
      DWORD bkgndColor = device()->getPaletteBackground();
      FLOAT Start = ((fogNear < -1e20f) ? -1e20f : ((FLOAT)fogNear));
      FLOAT End = ((fogFar > 1e20f) ? 1e20f : ((FLOAT)fogFar));
      IDirect3DDevice* g_pDevice = device()->d3dDevice();
      g_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
      g_pDevice->SetRenderState(D3DRS_FOGCOLOR, bkgndColor);
      g_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
      g_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&Start));
      g_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&End));
    }
    else
    {
      device()->d3dDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
      m_nCurFading = 0;
    }
  }
}

// Raster image caching (returns 0xFFFFFFFF if raster image is not cached)
OdUInt32 ExGsDirectXView::findRasterImage(const OdGiRasterImage *pImg, bool bTransparency, ODCOLORREF fgColor, bool bSecColor, ODCOLORREF bkColor, double fBrightness, double fContrast, double fFade) const
{
  OdArray<ExGsDirectXDevice::RasterImageEntry> &riEntry = const_cast<ExGsDirectXView*>(this)->device()->rasterImageCache();
  for (OdUInt32 i = 0; i < riEntry.size(); i++)
  {
    if (riEntry.at(i).m_pImg == pImg &&
        riEntry.at(i).m_bTransparency == bTransparency &&
        ((bTransparency) ? (fgColor == riEntry.at(i).m_fgColor) : true) &&
        bSecColor == riEntry.at(i).m_bSecColor &&
        ((bSecColor) ? (bkColor == riEntry.at(i).m_bkColor) : true) &&
        riEntry.at(i).m_fBrightness == fBrightness &&
        riEntry.at(i).m_fContrast == fContrast &&
        riEntry.at(i).m_fFade == fFade)
      return i;
  }
  return 0xFFFFFFFF;
}

OdUInt32 ExGsDirectXView::addRasterImage(RasterImageRef &pTexture, const OdGiRasterImage *pImg, bool bTransparency, ODCOLORREF fgColor, bool bSecColor, ODCOLORREF bkColor, double fBrightness, double fContrast, double fFade, bool bIncrement)
{
  OdArray<ExGsDirectXDevice::RasterImageEntry> &riEntries = device()->rasterImageCache();
  ExGsDirectXDevice::RasterImageEntry riEntry;
  riEntry.m_texture = pTexture;
  riEntry.m_pImg = pImg;
  riEntry.m_bTransparency = bTransparency;
  riEntry.m_fgColor = fgColor;
  riEntry.m_bSecColor = bSecColor;
  riEntry.m_bkColor = bkColor;
  riEntry.m_fBrightness = fBrightness;
  riEntry.m_fContrast = fContrast;
  riEntry.m_fFade = fFade;
  riEntry.m_uRefCounter = (bIncrement) ? 1 : 0;
  riEntries.append(riEntry);
  return riEntries.size() - 1;
}

D3dMaterialViewRedirect::RasterImageRef ExGsDirectXView::incrementRasterImageRef(OdUInt32 uNum)
{
  OdArray<ExGsDirectXDevice::RasterImageEntry> &riEntries = device()->rasterImageCache();
  riEntries.at(uNum).m_uRefCounter++;
  return riEntries.at(uNum).m_texture;
}

bool ExGsDirectXView::incrementRasterImageRef(CComPtr<IDirect3DTexture> pTexture)
{
  OdArray<ExGsDirectXDevice::RasterImageEntry> &riEntries = device()->rasterImageCache();
  for (OdUInt32 i = 0; i < riEntries.size(); i++)
  {
    if (riEntries.at(i).m_texture.m_pTexture == pTexture)
    {
      riEntries.at(i).m_uRefCounter++;
      return true;
    }
  }
  return false;
}

bool ExGsDirectXView::decrementRasterImageRef(OdUInt32 uNum)
{
  OdArray<ExGsDirectXDevice::RasterImageEntry> &riEntries = device()->rasterImageCache();
  riEntries.at(uNum).m_uRefCounter--;
  if (riEntries.at(uNum).m_uRefCounter == 0)
  {
    riEntries.removeAt(uNum);
    return true;
  }
  return false;
}

bool ExGsDirectXView::decrementRasterImageRef(CComPtr<IDirect3DTexture> pTexture)
{
  OdArray<ExGsDirectXDevice::RasterImageEntry> &riEntries = device()->rasterImageCache();
  for (OdUInt32 i = 0; i < riEntries.size(); i++)
  {
    if (riEntries.at(i).m_texture.m_pTexture == pTexture)
    {
      riEntries.at(i).m_uRefCounter--;
      if (riEntries.at(i).m_uRefCounter == 0)
      {
        riEntries.removeAt(i);
        return true;
      }
    }
  }
  return false;
}

bool ExGsDirectXView::isSecondaryColorNeed() const
{
  return GETBIT(effectiveTraits().drawFlags(), OdGiSubEntityTraits::kDrawContourFill);
}

ODCOLORREF ExGsDirectXView::getSecondaryColor() const
{
  const OdGiSubEntityTraitsData &effTraits = effectiveTraits();
  if(effTraits.secondaryTrueColor().isByACI() || effTraits.secondaryTrueColor().isByDgnIndex())
    return paletteColor(effTraits.secondaryTrueColor().colorIndex());
  else
    return ODTOCOLORREF(effTraits.secondaryTrueColor());
}

OdGiMaterialTextureManager *ExGsDirectXView::materialTextureManager()
{
  return device()->materialTextureManager();
}

D3dResourceSharingProvider *ExGsDirectXView::sharingProvider()
{
  return device()->sharingProvider();
}

OdGsBaseDeviceMTHelpers *ExGsDirectXView::mtDeviceSync()
{
  return &(device()->mtDeviceSync());
}

void ExGsDirectXView::setVisualStyle(const OdGiVisualStyle &visualStyle)
{
  // For test only
  OdGsBaseVectorizeView::setVisualStyle(visualStyle);
}

static void smoothCap(const OdD3DVertexCol &baseVtx, double dRadius, int nPx, IDirect3DDevice *pDevice)
{
  if (nPx < 3)
    return;
  OdGeCircArc2d circArc(OdGePoint2d(baseVtx.x, baseVtx.y), dRadius);
  OdGePoint2dArray pntArray;
  circArc.appendSamplePoints(nPx + 1, pntArray);
  OdUInt32 nSize = pntArray.size();
  OdD3DVertexColArray vtxArray(nSize, 1);
  vtxArray.resize(nSize);
  const OdGePoint2d *pFromPts = pntArray.getPtr();
  OdD3DVertexCol *pToPts = vtxArray.asArrayPtr();
  for (OdUInt32 n = 0; n < nSize; n++)
  {
    pToPts[n].x = (FLOAT)pFromPts[n].x;
    pToPts[n].y = (FLOAT)pFromPts[n].y;
    pToPts[n].z = 0.0f;
    pToPts[n].color = baseVtx.color;
  }
  pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, nSize - 2, pToPts, sizeof(OdD3DVertexCol));
}

void ExGsDirectXView::drawViewportFrame()
{
  if (isViewportBorderVisible())
  {
    OdD3DStateManager& pSM = device()->d3dStateManager();
    IDirect3DDevice* pDevice = device()->d3dDevice();

    // Disable previously set modes
    pSM.SetZEnableRenderState(D3DZB_FALSE);
    pSM.SetZWriteEnableRenderState(FALSE);
    pSM.SetLightingRenderState(FALSE);
    pSM.SetTexture(0, NULL);
    pSM.SetStencilEnableRenderState(FALSE);
    resetFading(false);

    bool bNrcClip = isNonRectClipped();

    // Setup transforms
    OdGeMatrix3d x;
    {
      OdGsDCRect dcrc = baseDevice()->outputRect();
      x(0, 0) = (dcrc.m_max.x - dcrc.m_min.x);
      x(1, 1) = (dcrc.m_max.y - dcrc.m_min.y);
      x(0, 3) = dcrc.m_min.x;
      x(1, 3) = dcrc.m_min.y;
      x.invert();

      OdGePoint2d lowerLeft, upperRight;
      OdGeMatrix3d vptMtx;
      getViewport(lowerLeft, upperRight);
      vptMtx(0, 0) = (upperRight.x - lowerLeft.x);
      vptMtx(1, 1) = (upperRight.y - lowerLeft.y);
      vptMtx(0, 3) = lowerLeft.x;
      vptMtx(1, 3) = lowerLeft.y;
      x.preMultBy(vptMtx);
    }
    x.preMultBy(OdGeMatrix3d::scaling(OdGeScale3d(2., 2., 1.), OdGePoint3d(1., 1., 0.)));

    D3DXMATRIX xMtx;
    GeMtxToD3DMtx(x, xMtx);
    pDevice->SetTransform(D3DTS_PROJECTION, &kD3DIdentityMatrix);
    pDevice->SetTransform(D3DTS_WORLD, &xMtx);

    // Load viewports
    OdIntArray nrcLoops;
    OdGePoint2dArray nrcPoints;
    if (bNrcClip)
    {
      viewportClipRegion(nrcLoops, nrcPoints);
    }
    else
    {
      nrcLoops.resize(1);
      nrcLoops[0] = 5;
      nrcPoints.resize(5);
      {
        OdGsDCRectDouble dcRect;
        getViewport(dcRect);
        nrcPoints[0] = dcRect.m_min; nrcPoints[2] = dcRect.m_max;
      }
      nrcPoints[1].set(nrcPoints[2].x, nrcPoints[0].y);
      nrcPoints[3].set(nrcPoints[0].x, nrcPoints[2].y);
      nrcPoints[4] = nrcPoints[0];
    }

    // Get border properties
    ODCOLORREF borderColor; int borderWidth;
    getViewportBorderProperties(borderColor, borderWidth);
    DWORD d3dColor = getD3DColor(borderColor);

    // Make data conversion
    OdUInt32 nLoops = nrcPoints.size(), nLoop;
    OdD3DVertexColArray vertexes(nLoops, 1);
    vertexes.resize(nLoops);
    for (nLoop = 0; nLoop < nLoops; nLoop++)
    {
      OdGePoint2d &pnt = nrcPoints[nLoop];
      OdD3DVertexCol &vtx = vertexes[nLoop];
      vtx.x = (FLOAT)pnt.x;
      vtx.y = (FLOAT)pnt.y;
      vtx.z = 0.0f;
      vtx.color = d3dColor;
    }

    // Render loops
    const OdD3DVertexCol *pCurBuf = vertexes.getPtr();
    OdUInt32 prevFirst = 0; nLoops = nrcLoops.size();
    pSM.SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    if (borderWidth == 1)
    {
      for (nLoop = 0; nLoop < nLoops; prevFirst += nrcLoops[nLoop++])
      {
        pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, nrcLoops[nLoop] - 1, pCurBuf + prevFirst, sizeof(OdD3DVertexCol));
      }
    }
    else
    {
      OdGeVector3d tmpVecX(OdGeVector3d::kXAxis), tmpVecY(OdGeVector3d::kYAxis);
      x.invert();
      tmpVecX.transformBy(x);
      tmpVecY.transformBy(x);
      FLOAT multCoefX = 0.0f, multCoefY = 0.0f;
      if (baseDevice()->width() != 0 && baseDevice()->height() != 0)
      {
        multCoefX = (FLOAT)(tmpVecX.length() * ((double)borderWidth / baseDevice()->width()));
        multCoefY = (FLOAT)(tmpVecY.length() * ((double)borderWidth / baseDevice()->height()));
      }
      OdD3DVertexCol rect[4];
      for (nLoop = 0; nLoop < nLoops; prevFirst += nrcLoops[nLoop++])
      {
        for (OdUInt32 nPt = 1; nPt < (OdUInt32)nrcLoops[nLoop]; nPt++)
        {
          OdGeVector3d dir(pCurBuf[prevFirst + nPt].x - pCurBuf[prevFirst + nPt - 1].x,
                           pCurBuf[prevFirst + nPt].y - pCurBuf[prevFirst + nPt - 1].y, 0.0);
          if (!dir.isZeroLength())
            dir.normalize();
          OdGeVector3d perp(-dir.y * multCoefX, dir.x * multCoefY, dir.z);
          rect[0].x = pCurBuf[prevFirst + nPt - 1].x + (FLOAT)perp.x;
          rect[0].y = pCurBuf[prevFirst + nPt - 1].y + (FLOAT)perp.y;
          rect[0].z = 0.0f;
          rect[0].color = d3dColor;
          rect[1].x = pCurBuf[prevFirst + nPt - 1].x - (FLOAT)perp.x;
          rect[1].y = pCurBuf[prevFirst + nPt - 1].y - (FLOAT)perp.y;
          rect[1].z = 0.0f;
          rect[1].color = d3dColor;
          rect[2].x = pCurBuf[prevFirst + nPt].x - (FLOAT)perp.x;
          rect[2].y = pCurBuf[prevFirst + nPt].y - (FLOAT)perp.y;
          rect[2].z = 0.0f;
          rect[2].color = d3dColor;
          rect[3].x = pCurBuf[prevFirst + nPt].x + (FLOAT)perp.x;
          rect[3].y = pCurBuf[prevFirst + nPt].y + (FLOAT)perp.y;
          rect[3].z = 0.0f;
          rect[3].color = d3dColor;
          pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, rect, sizeof(OdD3DVertexCol));
          if (borderWidth > 2)
            smoothCap(pCurBuf[prevFirst + nPt], perp.length(), borderWidth, pDevice);
        }
      }
    }
  }
}

void ExGsDirectXView::setupDepthBias(bool bEnable)
{
  RenderMode rm = view().mode();
  OdD3DStateManager *pSM = &(device()->d3dStateManager());
  if (bEnable && (rm == kHiddenLine || rm == kFlatShadedWithWireframe || rm == kGouraudShadedWithWireframe))
  {
    // Offset = m * D3DRS_SLOPESCALEDEPTHBIAS + D3DRS_DEPTHBIAS
    // m = max(abs(delta z / delta x), abs(delta z / delta y))
    // Slope calculated similarly as in OpenGL. But OpenGL multiplies DepthBias by implementation-specific variable:
    // minimum resolvable depth buffer value. 1.e-7 computed experimentally. I have similar results as by glPolygonOffset
    // currently, but this constant may depend from hardware.
    pSM->SetSlopeScaleDepthBiasRenderState(DxFloatAsDword(1.0f));
    pSM->SetDepthBiasRenderState(DxFloatAsDword(1.e-7f * 2.0f));
  }
  else
  {
    pSM->SetSlopeScaleDepthBiasRenderState(0);
    pSM->SetDepthBiasRenderState(0);
  }
}

void ExGsDirectXView::setupDisplayOpts(bool bEnable)
{
  setupDepthBias(bEnable);
  if (userGiContext() && GETBIT(userGiContext()->antiAliasingMode(), 1) &&
      view().mode() == k2DOptimized) // Could be checked using REF if HAL don't support
    device()->d3dStateManager().SetAntiAliasedLineEnableRenderState((bEnable) ? TRUE : FALSE);
}

void ExGsDirectXView::display(bool update)
{
  renderBackground();
  //
  setupDisplayOpts(true);
  OdGsBaseVectorizer::display(update);
  setupDisplayOpts(false);
  //
  if (mode() == kHiddenLine)
  {
    DWORD sfState = device()->d3dStateManager().GetStencilFuncRenderState();
    device()->d3dStateManager().SetStencilFuncRenderState(D3DCMP_LESS);
    DWORD passState = device()->d3dStateManager().GetStencilPassRenderState();
    device()->d3dStateManager().SetStencilPassRenderState(D3DSTENCILOP_REPLACE);
    renderBackground(true);
    device()->d3dStateManager().SetStencilPassRenderState(passState);
    device()->d3dStateManager().SetStencilFuncRenderState(sfState);
  }
  processMaterialNode(NULL, NULL);
}

void ExGsDirectXView::renderBackground(bool secondPass)
{
  // Draw viewport background and etc.
  if (viewportObjectId() != NULL)
  {
    // Prepare for background drawing
    OdD3DStateManager* pSM = &(device()->d3dStateManager());
    DWORD zState = pSM->GetZEnableRenderState();
    pSM->SetZEnableRenderState(D3DZB_FALSE);
    DWORD zWState = pSM->GetZWriteEnableRenderState();
    pSM->SetZWriteEnableRenderState(FALSE);
    DWORD lState = pSM->GetLightingRenderState();
    pSM->SetLightingRenderState(FALSE);
    OdGiFillType ftState = subEntityTraits().fillType();
    subEntityTraits().setFillType(kOdGiFillAlways);
    OdDbStub *layerState = subEntityTraits().layer();
    subEntityTraits().setLayer(NULL);
    geometry().setForceFill(true);
    // AMark : #5365 : Skip dept at background vectorizing
    OdGeMatrix3d pixels2DxCoords;
    D3DXMATRIX matView, matProjection;
    D3DMATRIX prjState;
    viewportTransforms(matView, matProjection, pixels2DxCoords, true);
    device()->d3dDevice()->GetTransform(D3DTS_PROJECTION, &prjState);
    device()->d3dDevice()->SetTransform(D3DTS_PROJECTION, &matProjection);
    //
    if (!secondPass)
    {
      updateViewportProperties(OdGsProperties::kDeviceNormal);
    }
    //
    if (mode() != OdGsView::k2DOptimized)
    {
      OdGiDrawFlagsHelper saveDrawFlag(subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
      displayViewportProperties(this, OdGsProperties::kDeviceNormal);
    }
    // Check if no background rendered - remove all background cache elements
    OdGsPropertiesPtr props = getViewportPropertiesForType(OdGsProperties::kBackground);
    if (props.isNull())
    {
      for (OdUInt32 i = 0; i < m_driCache.size(); i++)
      {
        const DRICacheElement &elem = m_driCache.at(i);
        if (elem.isInitialized() && elem.isBackground())
        {
          m_driCache.removeAt(i);
        }
      }
    }
    // Resore render settings
    // AMark : #5365 : Restore dept after background vectorizing
    device()->d3dDevice()->SetTransform(D3DTS_PROJECTION, &prjState);
    //
    geometry().setForceFill(false);
    subEntityTraits().setFillType(ftState);
    subEntityTraits().setLayer(layerState);
    pSM->SetLightingRenderState(lState);
    pSM->SetZWriteEnableRenderState(zWState);
    pSM->SetZEnableRenderState(zState);
    //
  }
}

void ExGsDirectXView::directRenderOutputImage(const OdGePoint3d *pRect, const OdGiRasterImage *pImage,
                                              const OdGsPropertiesDirectRenderOutput::DirectRenderImageParams &driParams)
{
  ODA_ASSERT(driParams.pDrawable != NULL && pRect != NULL && pImage != NULL);
  // Search in cache for already cached texture
  DRICacheElement *elem = NULL;
  OdGiDrawable::DrawableType dType = driParams.pDrawable->drawableType();
  bool background = (dType == OdGiDrawable::kSolidBackground) || (dType == OdGiDrawable::kGradientBackground) ||
                    (dType == OdGiDrawable::kImageBackground) || (dType == OdGiDrawable::kGroundPlaneBackground) ||
                    (dType == OdGiDrawable::kSkyBackground);
  for (OdUInt32 i = 0; i < m_driCache.size(); i++)
  {
    const DRICacheElement &cem = m_driCache.at(i);
    if (cem.isInitialized())
    {
      if (cem.isEqual(driParams.pDrawable, pImage))
      {
        elem = &m_driCache.at(i);
      }
      else if (background && cem.isBackground())
      {
        m_driCache.removeAt(i);
      }
    }
  }
  if (elem == NULL)
  {
    elem = new DRICacheElement;
    elem->initialize(driParams.pDrawable, pImage, *this);
    m_driCache.push_back(*elem);
    delete elem;
    elem = &m_driCache.last();
  }
  // Render texture
  IDirect3DDevice* pDev = device()->d3dDevice();
  OdD3DStateManager *pSM = &(device()->d3dStateManager());
  pSM->SetTexture(0, elem->pTexture);
  pSM->SetColorOpTextureStageState(0, D3DTOP_SELECTARG1);
  pSM->SetColorArg1TextureStageState(0, D3DTA_TEXTURE);
  pSM->SetColorArg2TextureStageState(0, D3DTA_DIFFUSE);
  pSM->SetMagFilterTextureStageState(0, D3DTEXF_LINEAR);
  pSM->SetMinFilterTextureStageState(0, D3DTEXF_LINEAR);
  pSM->SetTextureMatrixIdentity(0);
  pSM->SetTextureTransformFlagsTextureStageState(0, D3DTTFF_COUNT2);
  pSM->SetTexCoordIndexTextureStageState(0, D3DTSS_TCI_PASSTHRU);
  DWORD fvfState = pSM->GetFVF();
  pSM->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_DIFFUSE*/);

  OdGePoint3d tmp[4];
  if (metafileTransformOverrided())
  {
    const OdGeMatrix3d &pMat = worldToMetafileMatrix();
    for (int i = 0; i < 4; i++)
      tmp[i] = pMat * pRect[i];
    pRect = tmp;
  }

  struct fvfOutput_ {
    FLOAT x, y, z;
    //ODCOLORREF color;
    FLOAT u, v;
  } fvfOutput[6] =
  {
    { (FLOAT)pRect[0].x, (FLOAT)pRect[0].y, (FLOAT)pRect[0].z, 0.0f, 0.0f },
    { (FLOAT)pRect[1].x, (FLOAT)pRect[1].y, (FLOAT)pRect[1].z, 1.0f, 0.0f },
    { (FLOAT)pRect[2].x, (FLOAT)pRect[2].y, (FLOAT)pRect[2].z, 1.0f, 1.0f },
    { (FLOAT)pRect[0].x, (FLOAT)pRect[0].y, (FLOAT)pRect[0].z, 0.0f, 0.0f },
    { (FLOAT)pRect[2].x, (FLOAT)pRect[2].y, (FLOAT)pRect[2].z, 1.0f, 1.0f },
    { (FLOAT)pRect[3].x, (FLOAT)pRect[3].y, (FLOAT)pRect[3].z, 0.0f, 1.0f }
  };
  if (driParams.uvCoords != NULL)
  {
    fvfOutput[0].u = (FLOAT)driParams.uvCoords[0].u; fvfOutput[0].v = (FLOAT)driParams.uvCoords[0].v;
    fvfOutput[1].u = (FLOAT)driParams.uvCoords[1].u; fvfOutput[1].v = (FLOAT)driParams.uvCoords[1].v;
    fvfOutput[2].u = (FLOAT)driParams.uvCoords[2].u; fvfOutput[2].v = (FLOAT)driParams.uvCoords[2].v;
    fvfOutput[3].u = (FLOAT)driParams.uvCoords[0].u; fvfOutput[3].v = (FLOAT)driParams.uvCoords[0].v;
    fvfOutput[4].u = (FLOAT)driParams.uvCoords[2].u; fvfOutput[4].v = (FLOAT)driParams.uvCoords[2].v;
    fvfOutput[5].u = (FLOAT)driParams.uvCoords[3].u; fvfOutput[5].v = (FLOAT)driParams.uvCoords[3].v;
    pSM->SetAddressUTextureStageState(0, D3DTADDRESS_WRAP);
    pSM->SetAddressVTextureStageState(0, D3DTADDRESS_WRAP);
  }
  else
  {
    pSM->SetAddressUTextureStageState(0, D3DTADDRESS_CLAMP);
    pSM->SetAddressVTextureStageState(0, D3DTADDRESS_CLAMP);
  }
  pDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, fvfOutput, sizeof(fvfOutput_));
  pSM->SetTexture(0, NULL);
  pSM->SetFVF(fvfState);
}

OdUInt32 ExGsDirectXView::directRenderOutputFlags() const
{
  // Background image must be rendered directly for this device, for more optimized rendering way,
  // caching of D3D textures and optimal textures wrapping.
  return DirectRender_Image;
}

ExGsDirectXView::DRICacheElement::DRICacheElement() : pDrawable(NULL), pImage(NULL), dType(OdGiDrawable::kViewport)
{
}

ExGsDirectXView::DRICacheElement::~DRICacheElement()
{
  destroy();
}

void ExGsDirectXView::DRICacheElement::initialize(const OdGiDrawable *pDrawable, const OdGiRasterImage *pImage, ExGsDirectXView &view)
{
  ODA_ASSERT(pDrawable != NULL && pImage != NULL && !isInitialized());
  this->pDrawable = pDrawable;
  this->pImage = pImage;
  dType = pDrawable->drawableType();
  OdUInt32 srcWidth  = pImage->pixelWidth();
  OdUInt32 srcHeight = pImage->pixelHeight();
  OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
  pDesc->setPixelWidth(srcWidth);
  pDesc->setPixelHeight(srcHeight);
  pDesc->setColorDepth(32);
  pDesc->pixelFormat().setBGRA();
  OdGiRasterImagePtr pCopyImg;
  pCopyImg = pImage->convert(true, 50.0, 50.0, 0.0, view.device()->getBackgroundColor(), false, false, false, pDesc);
  pImage = pCopyImg;
  // Create texture
  // Image must be in power-of-two, if we need correctly use repeat texture mode
  // (maximum texture size is 2048 for DirectX8 compatible hardware, but may be 4096 for modern
  // devices)
  D3DCAPS devCaps;
  ZeroMemory(&devCaps, sizeof(D3DCAPS));
  view.device()->d3dDevice()->GetDeviceCaps(&devCaps);
  //bool capPow2 = (devCaps.TextureCaps & D3DPTEXTURECAPS_POW2) == D3DPTEXTURECAPS_POW2; // Always set, as I know.
  DWORD maxTextureWidth = devCaps.MaxTextureWidth;
  DWORD maxTextureHeight = devCaps.MaxTextureHeight;
  DWORD destWidth = 1;
  while (destWidth < srcWidth)
  {
    destWidth <<= 1;
    if (destWidth == maxTextureWidth)
    {
      break;
    }
  }
  DWORD destHeight = 1;
  while (destHeight < srcHeight)
  {
    destHeight <<= 1;
    if (destHeight == maxTextureHeight)
    {
      break;
    }
  }
  if (/*!capPow2 ||*/ destWidth == srcWidth && destHeight == srcHeight)
  {
    view.device()->d3dDevice()->CreateTexture(srcWidth, srcHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture);
    D3DLOCKED_RECT lockedRect;
    pTexture->LockRect(0, &lockedRect, NULL, 0);
    OdUInt8 *pBits = (OdUInt8*)lockedRect.pBits;
    pImage->scanLines(pBits, 0, srcHeight);
    pTexture->UnlockRect(0);
  }
  else
  {
    // Simple rescale texture
    view.device()->d3dDevice()->CreateTexture(destWidth, destHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture);
    D3DLOCKED_RECT lockedRect;
    pTexture->LockRect(0, &lockedRect, NULL, 0);
    OdUInt8 *pBits = (OdUInt8*)lockedRect.pBits;
    OdUInt8 *pBitsTmp = new OdUInt8[srcWidth * srcHeight * 4];
    pImage->scanLines(pBitsTmp, 0, srcHeight);
    double aspectX = double(srcWidth - 1) / (destWidth - 1),
           aspectY = double(srcHeight - 1) / (destHeight - 1);
    for (OdUInt32 iy = 0; iy < destHeight; iy++)
    {
      for (OdUInt32 ix = 0; ix < destWidth; ix++)
      {
        OdUInt8 *pPtr = pBitsTmp + ((OdUInt32(aspectY * iy) * srcWidth + OdUInt32(aspectX * ix)) << 2);
        *(reinterpret_cast<OdUInt32*>(pBits)) = *(reinterpret_cast<OdUInt32*>(pPtr));
        pBits += 4;
      }
    }
    delete []pBitsTmp;
    pTexture->UnlockRect(0);
  }
}

void ExGsDirectXView::DRICacheElement::destroy()
{
  if (isInitialized())
  {
    pDrawable = NULL;
    pImage = NULL;
    dType = OdGiDrawable::kViewport;
    CComPtr<IDirect3DDevice> device;
    pTexture->GetDevice(&device);
    device->SetTexture(0, NULL);
    pTexture = 0;
  }
}

bool ExGsDirectXView::DRICacheElement::isBackground() const
{
  return (dType == OdGiDrawable::kSolidBackground) || (dType == OdGiDrawable::kGradientBackground) ||
         (dType == OdGiDrawable::kImageBackground) || (dType == OdGiDrawable::kGroundPlaneBackground) ||
         (dType == OdGiDrawable::kSkyBackground);
}

bool ExGsDirectXView::DRICacheElement::isInitialized() const
{
  return (pDrawable != NULL) && (pImage != NULL) && (dType != OdGiDrawable::kViewport);
}

bool ExGsDirectXView::DRICacheElement::isEqual(const OdGiDrawable *pDrawable, const OdGiRasterImage *pImage) const
{
  return (this->pDrawable == pDrawable) && (this->pImage == pImage) && (dType == pDrawable->drawableType());
}

// TtfFonts cache support

OdRxObjectPtr ExGsDirectXDevice::tfcNewMetafile(void *pSessionId)
{
  ExGsDirectXView *pView = static_cast<ExGsDirectXView*>(reinterpret_cast<D3dMaterialViewRedirect*>(pSessionId));
  return pView->tfcNewMetafile();
}

OdGiConveyorGeometry *ExGsDirectXDevice::tfcBeginMetafile(OdRxObject *pMetafile, void *pSessionId)
{
  ExGsDirectXView *pView = static_cast<ExGsDirectXView*>(reinterpret_cast<D3dMaterialViewRedirect*>(pSessionId));
  return pView->tfcBeginMetafile(pMetafile);
}

void ExGsDirectXDevice::tfcFinalizeMetafile(OdRxObject *pMetafile, void *pSessionId)
{
  ExGsDirectXView *pView = static_cast<ExGsDirectXView*>(reinterpret_cast<D3dMaterialViewRedirect*>(pSessionId));
  return pView->tfcFinalizeMetafile(pMetafile);
}

void ExGsDirectXDevice::tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile, void *pSessionId) const
{
  ExGsDirectXView *pView = static_cast<ExGsDirectXView*>(reinterpret_cast<D3dMaterialViewRedirect*>(pSessionId));
  return pView->tfcPlayMetafile(xForm, pMetafile);
}

OdRxObjectPtr ExGsDirectXView::tfcNewMetafile()
{
  return newGsMetafile();
}

OdGiConveyorGeometry *ExGsDirectXView::tfcBeginMetafile(OdRxObject *pMetafile)
{
  beginMetafile(pMetafile);
  return &geometry();
}

void ExGsDirectXView::tfcFinalizeMetafile(OdRxObject *pMetafile)
{
  endMetafile(pMetafile);
}

void ExGsDirectXView::tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile) const
{
  ExGsDirectXView* pThis = const_cast<ExGsDirectXView*>(this);
  pThis->pushMetafileTransform(xForm, kSharedRefTransform);
  pThis->geometry().playMetafile(pMetafile);
  pThis->popMetafileTransform(kSharedRefTransform);
}

OdTtfFontsCache *ExGsDirectXView::getTtfFontsCache()
{
  if (device()->useTTFCache())
    return device()->ttfFontsCache();
  return NULL;
}

OdColorConverterCallback *ExGsDirectXView::getColorConverter()
{
  return device()->getColorConverter();
}

DWORD ExGsDirectXView::getBackgroundColor()
{
  return device()->getBackgroundColor();
}

bool ExGsDirectXView::useLutPaletteMonochrome() const
{
  return GETBIT(const_cast<ExGsDirectXView*>(this)->device()->useLutPalette(), 1);
}

bool ExGsDirectXView::useLutPaletteColor() const
{
  return GETBIT(const_cast<ExGsDirectXView*>(this)->device()->useLutPalette(), 2);
}

// Snapshot support

void ExGsDirectXView::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  device()->getSnapShot(pImage, region, this);
}

void ExGsDirectXDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  getSnapShot(pImage, region, NULL);
}

#include "../SnapshotSupport.h"
#include "Gi/GiRasterWrappers.h"

void ExGsDirectXDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView)
{
  struct SnapshotCallback : public OdGiSnapshotImageCallback
  {
    OdGiRasterImagePtr snapshotImageParams(OdGsBaseVectorizeDevice &runDevice) const
    {
      return static_cast<ExGsDirectXDevice&>(runDevice).snapshotImageParams();
    }
    OdGiRasterImagePtr snapshotRegion(OdGsBaseVectorizeDevice &runDevice, const OdGsDCRect &area, bool bCrop) const
    {
      return static_cast<ExGsDirectXDevice&>(runDevice).snapshotRegion(area, bCrop);
    }
  } _cbk;
  OdGiSnapshotImageImpl::getSnapshot(*this, &_cbk, pImage, region, pView);
}

OdGiRasterImagePtr ExGsDirectXDevice::snapshotImageParams() const
{
  OdSmartPtr<OdGiRasterImageDesc> pInfo = OdGiRasterImageDesc::createObject((OdUInt32)width(), (OdUInt32)height(), 32);
  pInfo->pixelFormat().setBGRA();
  pInfo->setScanLinesAlignment(4);
  return pInfo;
}

OdGiRasterImagePtr ExGsDirectXDevice::snapshotRegion(const OdGsDCRect &area, bool /*bCrop*/) const
{
  if (!width() || !height() || !m_pD3Ddevice)
    return OdGiRasterImagePtr();
  CComPtr<IDirect3DTexture> pRT;
  CComPtr<IDirect3DSurface> pSurfFrom;
  CComPtr<IDirect3DSurface> pSurfTo;
  CComPtr<IDirect3DTexture> pRT2;
  CComPtr<IDirect3DSurface> pSurfTo2;
  RECT sourceRect, destRect;
  ::SetRect(&sourceRect, area.m_min.x, height() - area.m_max.y, area.m_max.x, height() - area.m_min.y);
  ::SetRect(&destRect, 0, 0, area.m_max.x - area.m_min.x, area.m_max.y - area.m_min.y);
  m_pD3Ddevice->CreateTexture((UINT)(area.m_max.x - area.m_min.x), (UINT)(area.m_max.y - area.m_min.y),
                              1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pRT);
  m_pD3Ddevice->CreateTexture((UINT)(area.m_max.x - area.m_min.x), (UINT)(area.m_max.y - area.m_min.y),
                              1, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &pRT2);
  m_pD3Ddevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurfFrom);
  pRT->GetSurfaceLevel(0, &pSurfTo);
  ODA_VERIFY(m_pD3Ddevice->StretchRect(pSurfFrom, &sourceRect, pSurfTo, &destRect, D3DTEXF_NONE) == D3D_OK);
  pRT2->GetSurfaceLevel(0, &pSurfTo2);
  ODA_VERIFY(m_pD3Ddevice->GetRenderTargetData(pSurfTo, pSurfTo2) == D3D_OK);
  OdSmartPtr<OdGiSnapshotImageImpl> pImage = OdRxObjectImpl<OdGiSnapshotImageImpl>::createObject();
  pImage->pixelFormat().setBGRA();
  pImage->setAlignment(4);
  pImage->setupSurface(OdUInt32(area.m_max.x - area.m_min.x), OdUInt32(area.m_max.y - area.m_min.y), 32);
  D3DLOCKED_RECT lockRect;
  ODA_VERIFY(pSurfTo2->LockRect(&lockRect, NULL, D3DLOCK_READONLY) == D3D_OK);
  {
    OdUInt32 nH = pImage->pixelHeight(), nS = pImage->scanLineSize();
    OdUInt8 *pTo = pImage->scanLines() + (nH - 1) * nS, *pFrom = (OdUInt8*)lockRect.pBits;
    for (OdUInt32 nSc = 0; nSc < nH; nSc++, pTo -= nS, pFrom += lockRect.Pitch)
      ::memcpy(pTo, pFrom, nS);
  }
  pSurfTo2->UnlockRect();
  return pImage;
}

// Resource sharing

IDirect3DTexture *D3dResourceSharingProvider::D3dResourceShareRef::getShared(IDirect3DDevice* pCurDevice) const
{
  sharedDefs::iterator it = m_sharedDefs.find(pCurDevice);
  if (it != m_sharedDefs.end())
    return it->second->m_pEntry;
  return NULL;
}

IDirect3DTexture *D3dResourceSharingProvider::D3dResourceShareRef::createShared(IDirect3DDevice* pCurDevice, IDirect3DTexture* pBaseResource, D3dResourceSharingProvider *pProv) const
{
  D3dResourceShareEntry *pEntry = pProv->appendEntry(const_cast<D3dResourceShareRef*>(this), pCurDevice, pBaseResource);
  m_sharedDefs[pCurDevice] = pEntry;
  return pEntry->m_pEntry;
}

void D3dResourceSharingProvider::D3dResourceShareRef::unlinkShared(IDirect3DDevice* pDevice)
{
  sharedDefs::iterator it = m_sharedDefs.find(pDevice);
  if (it != m_sharedDefs.end())
  {
    it->second->m_pProv->unlinkEntry(this, pDevice, it->second);
    m_sharedDefs.erase(it);
  }
}

void D3dResourceSharingProvider::D3dResourceShareRef::unlinkShared()
{
  sharedDefs::iterator it = m_sharedDefs.begin();
  while (it != m_sharedDefs.end())
  {
    it->second->m_pProv->unlinkEntry(this, it->first, it->second);
    it++;
  }
  m_sharedDefs.clear();
}

void D3dResourceSharingProvider::recreate(D3dLostStateManager * /*pManager*/)
{
}

void D3dResourceSharingProvider::onManagerDestroy(D3dLostStateManager * /*pManager*/)
{
  // Detach all provided resources
  shareEntries::iterator it = m_sharedEntries.begin();
  while (it != m_sharedEntries.end())
  {
    OdList<D3dResourceShareRef*>::iterator it2 = it->m_refs.begin();
    while (it2 != it->m_refs.end())
    {
      IDirect3DDevice *pDevice;
      it->m_pEntry->GetDevice(&pDevice);
      (*it2)->m_sharedDefs.erase(pDevice);
      if (pDevice) pDevice->Release();
      it2++;
    }
    it++;
  }
  m_sharedEntries.clear();
}

D3dResourceSharingProvider::D3dResourceShareEntry *D3dResourceSharingProvider::appendEntry(D3dResourceShareRef *pRef, IDirect3DDevice* pCurDevice, IDirect3DTexture* pBaseResource)
{
  shareEntries::iterator it = m_sharedEntries.begin();
  while (it != m_sharedEntries.end())
  {
    if (it->m_pBasePtr == pBaseResource)
      break;
    it++;
  }
  if (it == m_sharedEntries.end())
  {
    it = m_sharedEntries.append();
    it->m_pBasePtr = pBaseResource;
    { // Copy texture
      D3DSURFACE_DESC desc;
      pBaseResource->GetLevelDesc(0, &desc);
      pCurDevice->CreateTexture(desc.Width, desc.Height, 1, desc.Usage, desc.Format, desc.Pool, &(it->m_pEntry));
      D3DLOCKED_RECT lockRect2, lockRect4;
      it->m_pEntry->LockRect(0, &lockRect2, NULL, 0);
      pBaseResource->LockRect(0, &lockRect4, NULL, 0);
      memcpy(lockRect2.pBits, lockRect4.pBits, lockRect2.Pitch * desc.Height);
      pBaseResource->UnlockRect(0);
      it->m_pEntry->UnlockRect(0);
    }
    it->m_pProv = this;
  }
  *(it->m_refs.append()) = pRef;
  return &*it;
}

void D3dResourceSharingProvider::unlinkEntry(D3dResourceShareRef *pRef, IDirect3DDevice* /*pCurDevice*/, D3dResourceShareEntry *pEntry)
{
  pEntry->m_refs.remove(pRef);
  if (pEntry->m_refs.empty())
  {
    m_sharedEntries.remove(*pEntry);
  }
}

//
