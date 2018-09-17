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

#include "OdaCommon.h"
#include "ExGsCompositeView.h"
#include "RxDictionary.h"
#include "RxObjectImpl.h"
#include "RxVariantValue.h"
#include "ExGsCompositeModuleInterface.h"
#include "ExWinGsCompositeDevice.h"

#if (_MSC_VER >= 1400)
#define ODGSDEV_SUPER __super
#else
#define ODGSDEV_SUPER ExGsCompositeDevice
#endif

ODRX_DECLARE_PROPERTY(ClearScreen)
ODRX_DECLARE_PROPERTY(DiscardBackFaces)
ODRX_DECLARE_PROPERTY(UseTextOut)
ODRX_DECLARE_PROPERTY(UseTTFCache)
ODRX_DECLARE_PROPERTY(UseLutPalette)
ODRX_DECLARE_PROPERTY(RegenCoef)
ODRX_DECLARE_PROPERTY(ModuleFactory)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExWinGsCompositeDevice);
  ODRX_INHERIT_PROPERTIES(ExGsWin32Device);
  ODRX_GENERATE_PROPERTY(ClearScreen)
  ODRX_GENERATE_PROPERTY(DiscardBackFaces)
  ODRX_GENERATE_PROPERTY(UseTextOut)
  ODRX_GENERATE_PROPERTY(UseTTFCache)
  ODRX_GENERATE_PROPERTY(UseLutPalette)
  ODRX_GENERATE_PROPERTY(RegenCoef)
  ODRX_GENERATE_PROPERTY(ModuleFactory)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExWinGsCompositeDevice);

ODRX_DEFINE_PROPERTY_METHODS(ClearScreen, ExWinGsCompositeDevice, clearScreen, setClearScreen, getBool);
ODRX_DEFINE_PROPERTY_METHODS(DiscardBackFaces, ExWinGsCompositeDevice, discardBackFaces, setDiscardBackFaces, getBool);
ODRX_DEFINE_PROPERTY_METHODS(UseTextOut, ExWinGsCompositeDevice, useTextOut, setUseTextOut, getBool);
ODRX_DEFINE_PROPERTY_METHODS(UseTTFCache, ExWinGsCompositeDevice, useTTFCache, setUseTTFCache, getBool);
ODRX_DEFINE_PROPERTY_METHODS(UseLutPalette, ExWinGsCompositeDevice, useLutPalette, setUseLutPalette, getUInt32);
ODRX_DEFINE_PROPERTY_METHODS(RegenCoef, ExWinGsCompositeDevice, getRegenCoef, setRegenCoef, getDouble);
ODRX_DEFINE_PROPERTY_METHODS(ModuleFactory, ExWinGsCompositeDevice, getModuleFactoryProp, setModuleFactoryProp, getIntPtr);

ExWinGsCompositeDevice::ExWinGsCompositeDevice()
  : ExGsCompositeDevice()
  , ExGsWin32Device()
  , m_hOffscreenDC(NULL)
  , m_hScreenBmp(NULL)
  , m_bClearScreen(true)
  , m_bDiscardBackFaces(false)
  , m_bUseTextOut(false)
  , m_bUseTTFCache(false)
  , m_nUseLutPalette(3)
{
}

ExWinGsCompositeDevice::~ExWinGsCompositeDevice()
{
  detachAllViews();
  if (isContextInitialized())
    deleteContext();
}

OdRxDictionaryPtr ExWinGsCompositeDevice::properties()
{
  return this;
}

HDC ExWinGsCompositeDevice::hdc() const
{
  return m_hDC;
}

HDC ExWinGsCompositeDevice::drawDc() const
{
  return (m_hOffscreenDC) ? m_hOffscreenDC : hdc();
}

void ExWinGsCompositeDevice::createContext()
{
  ExGsCompositeDevice::createContext();
  ExGsWin32Device::createPalette(&m_logPalette);
  // create offscreen surface if it is required
  if (doubleBufferEnabled() && !userGiContext()->isPlotGeneration())
  {
    ODA_VERIFY(m_hOffscreenDC = ::CreateCompatibleDC(hdc()));
    if (getDeviceColorDepth() > 8)
    {
      m_hScreenBmp = ::CreateCompatibleBitmap(hdc(), width(), height());
    }
    else
    {
      HBITMAP devBitmap = (HBITMAP)::GetCurrentObject(hdc(), OBJ_BITMAP);
      BITMAP bm; ::GetObject(devBitmap, sizeof(BITMAP), &bm);
      m_hScreenBmp = ::CreateBitmap(bm.bmWidth, bm.bmHeight, ::GetDeviceCaps(hdc(), PLANES), ::GetDeviceCaps(hdc(), BITSPIXEL), NULL);
    }
    ODA_ASSERT(m_hScreenBmp);
    ::SelectObject(m_hOffscreenDC, m_hScreenBmp);
    ::SetWindowOrgEx(m_hOffscreenDC, OdRoundToLong(odmin(m_outputRect.m_min.x, m_outputRect.m_max.x)),
                                     OdRoundToLong(odmin(m_outputRect.m_min.y, m_outputRect.m_max.y)), NULL);
  }
}

void ExWinGsCompositeDevice::deleteContext()
{
  if (m_hScreenBmp)
  {
    ::DeleteObject((HGDIOBJ)m_hScreenBmp);
    m_hScreenBmp = NULL;
  }
  if (m_hOffscreenDC)
  {
    ::DeleteDC(m_hOffscreenDC);
    m_hOffscreenDC = NULL;
  }
  ExGsWin32Device::deleteContext();
  ExGsCompositeDevice::deleteContext();
}

void ExWinGsCompositeDevice::onSize(const OdGsDCRect& outputRect)
{
  ODGSDEV_SUPER::onSize(outputRect);
  if (m_hScreenBmp)
  {
    if (getDeviceColorDepth() > 8)
    {
      m_hScreenBmp = ::CreateCompatibleBitmap(hdc(), width(), height());
    }
    else
    {
      HBITMAP devBitmap = (HBITMAP)::GetCurrentObject(hdc(), OBJ_BITMAP);
      BITMAP bm; ::GetObject(devBitmap, sizeof(BITMAP), &bm);
      m_hScreenBmp = ::CreateBitmap(bm.bmWidth, bm.bmHeight, ::GetDeviceCaps(hdc(), PLANES), ::GetDeviceCaps(hdc(), BITSPIXEL), NULL);
    }
    ODA_ASSERT(m_hScreenBmp);
    ::DeleteObject(::SelectObject(m_hOffscreenDC, m_hScreenBmp));
    ::SetWindowOrgEx(m_hOffscreenDC, OdRoundToLong(odmin(m_outputRect.m_min.x, m_outputRect.m_max.x)),
                                     OdRoundToLong(odmin(m_outputRect.m_min.y, m_outputRect.m_max.y)), NULL);
  }
  int nViews = numViews();
  for (int nView = 0; nView < nViews; nView++)
  {
    ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
    if (pView->compositeDevice() != NULL)
    {
      if ((!isRedirection() && !GETBIT(pView->moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableCompDoubleBuffer)) ||
          (isRedirection() && !GETBIT(pView->moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableRedirDoubleBuffer)))
      {
        if (isWindowRequired(*pView))
        {
          OdRxDictionaryPtr pProps = pView->compositeDevice()->properties();
          if (!pProps.isNull())
          {
            HWND hWnd = (HWND)OdRxVariantValue(pProps->getAt(OD_T("WindowHWND")))->getIntPtr();
            ::MoveWindow(hWnd, 0, 0, width(), height(), FALSE);
          }
          else
          {
            ODA_FAIL();
          }
        }
        else
        {
          OdRxDictionaryPtr pProps = pView->compositeDevice()->properties();
          if (!pProps.isNull())
          {
            HDC hDc = (HDC)OdRxVariantValue(pProps->getAt(OD_T("WindowHDC")))->getIntPtr();
            HBITMAP hBmp;
            if (getDeviceColorDepth() > 8)
            {
              hBmp = ::CreateCompatibleBitmap(hdc(), width(), height());
            }
            else
            {
              HBITMAP devBitmap = (HBITMAP)::GetCurrentObject(hdc(), OBJ_BITMAP);
              BITMAP bm; ::GetObject(devBitmap, sizeof(BITMAP), &bm);
              hBmp = ::CreateBitmap(bm.bmWidth, bm.bmHeight, ::GetDeviceCaps(hdc(), PLANES), ::GetDeviceCaps(hdc(), BITSPIXEL), NULL);
            }
            ODA_ASSERT(hBmp);
            ::DeleteObject(::SelectObject(hDc, hBmp));
          }
          else
          {
            ODA_FAIL();
          }
        }
      }
    }
  }
}

void ExWinGsCompositeDevice::sync()
{
  if (supportBlocks() != blocksCacheEnabled())
  {
    SETBIT(m_flags, kSupportBlocks, blocksCacheEnabled());
    for (int nView = 0; nView < numViews(); nView++)
    {
      ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
      if (pView->compositeView() != NULL)
      {
        OdRxDictionaryPtr pProps = pView->compositeDevice()->properties();
        if (!pProps.isNull())
        {
          if (pProps->has(OD_T("BlocksCache")))
            pProps->putAt(OD_T("BlocksCache"), OdRxVariantValue(supportBlocks()));
        }
      }
    }
  }
  if (supportDynamicSubhighlight() != dynamicSubEntHltEnabled())
  {
    SETBIT(m_flags, kDynamicSubhighlight, dynamicSubEntHltEnabled());
    for (int nView = 0; nView < numViews(); nView++)
    {
      ExGsCompositeView *pView = static_cast<ExGsCompositeView*>(viewAt(nView));
      if (pView->compositeView() != NULL)
      {
        OdRxDictionaryPtr pProps = pView->compositeDevice()->properties();
        if (!pProps.isNull())
        {
          if (pProps->has(OD_T("DynamicSubEntHlt")))
            pProps->putAt(OD_T("DynamicSubEntHlt"), OdRxVariantValue(supportBlocks()));
        }
      }
    }
  }
  ODGSDEV_SUPER::sync();
}

void ExWinGsCompositeDevice::preprocess()
{
  ODGSDEV_SUPER::preprocess();

  if (isInvalidRects())
    pushViewportClip(invalidRects().size(), invalidRects().getPtr());

  if (clearScreen() && !isValid() && (!isRedirection() || (numViews() == 0) ||
      GETBIT(static_cast<ExGsCompositeView*>(viewAt(0))->moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableRedirClearScreen)))
  {
    // for debugging
    // m_Background = ODRGB(BYTE(rand() * 255 / RAND_MAX), BYTE(rand() * 255 / RAND_MAX), BYTE(rand() * 255 / RAND_MAX));

#if (_WIN32_WINNT >= 0x0500)
    HGDIOBJ prevPen = ::SelectObject(drawDc(), ::GetStockObject(DC_PEN));
    HGDIOBJ prevBrush = ::SelectObject(drawDc(), ::GetStockObject(DC_BRUSH));
    ::SetDCPenColor(drawDc(), getBackgroundColor());
    ::SetDCBrushColor(drawDc(), getBackgroundColor());
    ::Rectangle(drawDc(), m_outputRect.m_min.x, m_outputRect.m_max.y, m_outputRect.m_max.x, m_outputRect.m_min.y);
    ::SelectObject(drawDc(), prevBrush);
    ::SelectObject(drawDc(), prevPen);
#else
    HPEN hPen = ::CreatePen(PS_SOLID, 1, getBackgroundColor());
    HBRUSH hBrush = ::CreateSolidBrush(getBackgroundColor());
    HGDIOBJ pPrevPen = ::SelectObject(drawDc(), (HGDIOBJ)hPen);
    HGDIOBJ pPrevBrush = ::SelectObject(drawDc(), (HGDIOBJ)hBrush);
    ::Rectangle(drawDc(), m_outputRect.m_min.x, m_outputRect.m_max.y, m_outputRect.m_max.x, m_outputRect.m_min.y);
    ::DeleteObject(::SelectObject(drawDc(), pPrevBrush));
    ::DeleteObject(::SelectObject(drawDc(), pPrevPen));
#endif
  }
}

void ExWinGsCompositeDevice::postprocess()
{
  if (isInvalidRects())
    popViewportClip();

  ODGSDEV_SUPER::postprocess();
}

void ExWinGsCompositeDevice::present()
{
  ODGSDEV_SUPER::present();
  if (m_hScreenBmp && !userGiContext()->regenAbort())
  {
    ::SetWindowOrgEx(m_hOffscreenDC, 0, 0, NULL);
    ::BitBlt(hdc(), OdRoundToLong(odmin(m_outputRect.m_min.x, m_outputRect.m_max.x)),
                    OdRoundToLong(odmin(m_outputRect.m_min.y, m_outputRect.m_max.y)),
                    width(), height(), drawDc(), 0, 0, SRCCOPY);
  }
}

#define OUR_WND_NAME OD_T("Teigha CompositeRender Window")

void ExWinGsCompositeDevice::compositeViewAttached(ExGsCompositeView &pView)
{
  ExGsCompositeDevice::compositeViewAttached(pView);
  OdRxDictionaryPtr pProps = pView.compositeDevice()->properties();
  if (!pProps.isNull())
  {
    // Propagate blocks cache property
    if (supportBlocks())
    {
      if (pProps->has(OD_T("BlocksCache")))
        pProps->putAt(OD_T("BlocksCache"), OdRxVariantValue(true));
    }
    // Propagate dynamic highlighting property
    if (supportDynamicSubhighlight())
    {
      if (pProps->has(OD_T("DynamicSubEntHlt")))
        pProps->putAt(OD_T("DynamicSubEntHlt"), OdRxVariantValue(true));
    }
    // Propagate device properties
    if (gsMultithreadEnabled())
    {
      if (pProps->has(OD_T("EnableMultithread")))
        pProps->putAt(OD_T("EnableMultithread"), OdRxVariantValue(gsMultithreadEnabled()));
    }
    if (forcePartialUpdateForMt())
    {
      if (pProps->has(OD_T("ForcePartialUpdate")))
        pProps->putAt(OD_T("ForcePartialUpdate"), OdRxVariantValue(forcePartialUpdateForMt()));
    }
    if (pProps->has(OD_T("ColorConverter")))
      pProps->putAt(OD_T("ColorConverter"), properties()->getAt(OD_T("ColorConverter")));
    if (pProps->has(OD_T("DiscardBackFaces")))
      pProps->putAt(OD_T("DiscardBackFaces"), OdRxVariantValue(discardBackFaces()));
    if (pProps->has(OD_T("UseTextOut")))
      pProps->putAt(OD_T("UseTextOut"), OdRxVariantValue(useTextOut()));
    if (pProps->has(OD_T("UseTTFCache")))
      pProps->putAt(OD_T("UseTTFCache"), OdRxVariantValue(useTTFCache()));
    if (pProps->has(OD_T("UseLutPalette")))
      pProps->putAt(OD_T("UseLutPalette"), OdRxVariantValue(useLutPalette()));
  }
  // Setup rendering contexts
  if ((!isRedirection() && GETBIT(pView.moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableCompDoubleBuffer)) ||
      (isRedirection() && GETBIT(pView.moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableRedirDoubleBuffer)))
  {
    if (!pProps.isNull())
    {
      if (pProps->has(OD_T("WindowHDC")))
        pProps->putAt(OD_T("WindowHDC"), OdRxVariantValue((OdIntPtr)drawDc()));
      if (pProps->has(OD_T("DoubleBufferEnabled")))
        pProps->putAt(OD_T("DoubleBufferEnabled"), OdRxVariantValue(false));
    }
  }
  else
  {
    if (isWindowRequired(pView))
    {
      HINSTANCE hInstance = ::GetModuleHandle(NULL);
      WNDCLASSEX wc;
      wc.cbSize        = sizeof(WNDCLASSEX);
      wc.style         = CS_NOCLOSE;
      wc.lpfnWndProc   = DefWindowProc;
      wc.cbClsExtra    = 0; 
      wc.cbWndExtra    = 0;
      wc.hInstance     = hInstance;
      wc.hIcon         = 0;
      wc.hCursor       = 0;
      wc.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
      wc.lpszMenuName  = 0;
      wc.hIconSm       = 0;
      wc.lpszClassName = OUR_WND_NAME;
      /*ODA_VERIFY(*/::RegisterClassEx(&wc)/*)*/; // Don't check, because could be already registered
      HWND hWnd = ::CreateWindowEx(0, OUR_WND_NAME, OUR_WND_NAME, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                   0, 0, width(), height(), ::GetDesktopWindow(), 0, hInstance, 0);
      if (!pProps.isNull())
      {
        if (pProps->has(OD_T("WindowHWND")))
          pProps->putAt(OD_T("WindowHWND"), OdRxVariantValue((OdIntPtr)hWnd));
        if (pProps->has(OD_T("WindowHDC")))
          pProps->putAt(OD_T("WindowHDC"), OdRxVariantValue((OdIntPtr)::GetDC(hWnd)));
        if (pProps->has(OD_T("LockableBackBuffer")))
          pProps->putAt(OD_T("LockableBackBuffer"), OdRxVariantValue(true));
      }
      else
      {
        ODA_FAIL();
      }
    }
    else
    {
      HDC hDc; HBITMAP hBmp;
      ODA_VERIFY(hDc = ::CreateCompatibleDC(hdc()));
      if (getDeviceColorDepth() > 8)
      {
        hBmp = ::CreateCompatibleBitmap(hdc(), width(), height());
      }
      else
      {
        HBITMAP devBitmap = (HBITMAP)::GetCurrentObject(hdc(), OBJ_BITMAP);
        BITMAP bm; ::GetObject(devBitmap, sizeof(BITMAP), &bm);
        hBmp = ::CreateBitmap(bm.bmWidth, bm.bmHeight, ::GetDeviceCaps(hdc(), PLANES), ::GetDeviceCaps(hdc(), BITSPIXEL), NULL);
      }
      ODA_ASSERT(hBmp);
      ::SelectObject(hDc, hBmp);
      if (!pProps.isNull())
      {
        if (pProps->has(OD_T("WindowHDC")))
          pProps->putAt(OD_T("WindowHDC"), OdRxVariantValue((OdIntPtr)hDc));
      }
      else
      {
        ODA_FAIL();
      }
    }
  }
}

void ExWinGsCompositeDevice::compositeViewDetached(ExGsCompositeView &pView)
{
  if ((!isRedirection() && !GETBIT(pView.moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableCompDoubleBuffer)) ||
      (isRedirection() && !GETBIT(pView.moduleFactoryFlags(), ExGsCompositeModuleProperty::kDisableRedirDoubleBuffer)))
  {
    if (isWindowRequired(pView))
    {
      OdRxDictionaryPtr pProps = pView.compositeDevice()->properties();
      if (!pProps.isNull())
      {
        HWND hWnd = (HWND)OdRxVariantValue(pProps->getAt(OD_T("WindowHWND")))->getIntPtr();
        HDC hDc = (HDC)OdRxVariantValue(pProps->getAt(OD_T("WindowHDC")))->getIntPtr();
        ::ReleaseDC(hWnd, hDc);
        ::DestroyWindow(hWnd);
      }
      else
      {
        ODA_FAIL();
      }
    }
    else
    {
      OdRxDictionaryPtr pProps = pView.compositeDevice()->properties();
      if (!pProps.isNull())
      {
        HDC hDc = (HDC)OdRxVariantValue(pProps->getAt(OD_T("WindowHDC")))->getIntPtr();
        HBITMAP hBmp = (HBITMAP)::GetCurrentObject(hDc, OBJ_BITMAP);
        ::DeleteObject((HGDIOBJ)hBmp);
        ::DeleteDC(hDc);
      }
      else
      {
        ODA_FAIL();
      }
    }
  }
  ExGsCompositeDevice::compositeViewDetached(pView);
}

#include "ExWinCompositeBitBltOp.h"

void ExWinGsCompositeDevice::presentRect(ExGsCompositeView &pView, const OdGsDCRect &screenRect)
{
  if (!isWindowRequired(pView))
  {
    OdRxDictionaryPtr pProps = pView.compositeDevice()->properties();
    if (!pProps.isNull())
    {
      HDC hDc = (HDC)OdRxVariantValue(pProps->getAt(OD_T("WindowHDC")))->getIntPtr();
      ::BitBlt(drawDc(),
               screenRect.m_min.x, screenRect.m_max.y,
               screenRect.m_max.x - screenRect.m_min.x,
               screenRect.m_min.y - screenRect.m_max.y,
               hDc,
               screenRect.m_min.x, screenRect.m_max.y,
               SRCCOPY);
    }
    else
    {
      ODA_FAIL();
    }
  }
  else
  {
    ExWinCompositeBitBltOp bbop;
    bbop.m_hdc = drawDc();
    bbop.m_x = bbop.m_x1 = screenRect.m_min.x;
    bbop.m_y = bbop.m_y1 = screenRect.m_max.y;
    bbop.m_cx = screenRect.m_max.x - screenRect.m_min.x;
    bbop.m_cy = screenRect.m_min.y - screenRect.m_max.y;
    dynamic_cast<ExGsCompositeClientDeviceIface*>(pView.compositeDevice())->makeBitBltOp(bbop);
  }
}

void ExWinGsCompositeDevice::presentViewportSurface(ExGsCompositeView &pView, const OdGsDCRect &pUpdatedRect)
{
  ExGsCompositeDevice::presentViewportSurface(pView, pUpdatedRect);
  OdGsDCRect screenRect;
  pView.compositeView()->screenRect/*Norm*/(screenRect.m_min, screenRect.m_max);
  if (screenRect.m_min.x > screenRect.m_max.x)
    std::swap(screenRect.m_min.x, screenRect.m_max.x);
  if (screenRect.m_min.y < screenRect.m_max.y)
    std::swap(screenRect.m_min.y, screenRect.m_max.y);
  //screenRect.intersectWith(pUpdatedRect, false);
  if (screenRect.m_min.x < pUpdatedRect.m_min.x)
    screenRect.m_min.x = pUpdatedRect.m_min.x;
  if (screenRect.m_max.x > pUpdatedRect.m_max.x)
    screenRect.m_max.x = pUpdatedRect.m_max.x;
  if (screenRect.m_min.y > pUpdatedRect.m_max.y)
    screenRect.m_min.y = pUpdatedRect.m_max.y;
  if (screenRect.m_max.y < pUpdatedRect.m_min.y)
    screenRect.m_max.y = pUpdatedRect.m_min.y;
  if (screenRect.m_max.x > screenRect.m_min.x &&
      screenRect.m_max.y < screenRect.m_min.y)
  {
    OdIntArray nrcCounts; OdGsDCPointArray nrcPoints;
    pView.viewportClipRegion(nrcCounts, nrcPoints);
    if (!nrcCounts.empty())
      pushViewportClip(nrcPoints.getPtr(), nrcCounts.getPtr(), nrcCounts.size());
    presentRect(pView, screenRect);
    if (!nrcCounts.empty())
      popViewportClip();
  }
}

void ExWinGsCompositeDevice::presentRedirectedSurface(ExGsCompositeDevice &pDevice, const OdGsDCRect *pUpdatedRect)
{
  if (pDevice.numViews() > 0)
  {
    OdGsDCRect normRect = (pUpdatedRect) ? *pUpdatedRect : outputRect();
    if (normRect.m_max.y > normRect.m_min.y)
      std::swap(normRect.m_max.y, normRect.m_min.y);
    presentRect(*static_cast<ExGsCompositeView*>(pDevice.viewAt(0)), normRect);
  }
}

bool ExWinGsCompositeDevice::setBackgroundColor(ODCOLORREF color)
{
  return ODGSDEV_SUPER::setBackgroundColor(color & ODRGB(255, 255, 255));
}

void ExWinGsCompositeDevice::setLogicalPalette(const ODCOLORREF* palette, int nCount)
{
  ODGSDEV_SUPER::setLogicalPalette(palette, nCount);
  { // Windows GDI needs 0 in alpha component
    ODCOLORREF cMask = ODRGB(255, 255, 255);
    ODCOLORREF *pArray = m_logPalette.asArrayPtr();
    for (int i = 0; i < nCount; i++)
    {
      pArray[i] &= cMask;
    }
  }
}

void ExWinGsCompositeDevice::HDCWasChanged()
{
  if (isContextInitialized())
  {
    deleteContext();
    createContext();
  }
}

void ExWinGsCompositeDevice::pushViewportClip(const OdGsDCPoint* pPoints, const int *nrcCounts, int nCount)
{
  HDC hdc = drawDc();
  m_clipStack.push_back(::SaveDC(hdc));
  ::BeginPath(hdc);
  ::PolyPolygon(hdc, (CONST POINT*)pPoints, (CONST INT*)nrcCounts, nCount);
  ::EndPath(hdc);
  ::SelectClipPath(hdc, RGN_AND);
}

void ExWinGsCompositeDevice::pushViewportClip(const OdGsDCPoint& min, const OdGsDCPoint& max)
{
  m_clipStack.push_back(::SaveDC(drawDc()));
  ::IntersectClipRect(drawDc(), min.x, max.y, max.x, min.y);
}

void ExWinGsCompositeDevice::pushViewportClip(OdUInt32 nNumRects, const OdGsDCRect* rects)
{
  OdGsDCPointArray points(nNumRects * 4);
  points.resize(nNumRects * 4);
  OdIntArray counts(nNumRects);
  counts.insert(counts.end(), nNumRects, 4);
  OdGsDCPoint* cp = points.asArrayPtr();
  while (nNumRects--)
  {
    cp[0] = rects->m_min;
    cp[1].x = rects->m_min.x;
    cp[1].y = rects->m_max.y;
    cp[2] = rects->m_max;
    cp[3].x = rects->m_max.x;
    cp[3].y = rects->m_min.y;
    cp += 4;
    ++rects;
  }
  HDC hdc = drawDc();
  m_clipStack.push_back(::SaveDC(hdc));
  int pfm =::SetPolyFillMode(hdc, WINDING);
  ::BeginPath(hdc);
  ::PolyPolygon(hdc, (CONST POINT*)points.getPtr(), (CONST INT*)counts.getPtr(), counts.size());
  ::EndPath(hdc);
  ::SelectClipPath(hdc, RGN_AND);
  ::SetPolyFillMode(hdc, pfm);
}

void ExWinGsCompositeDevice::popViewportClip()
{
  ODA_ASSERT(!m_clipStack.empty());

  HPEN hPen = (HPEN)::GetCurrentObject(drawDc(), OBJ_PEN);
  HBRUSH hBrush = (HBRUSH)::GetCurrentObject(drawDc(), OBJ_BRUSH);

  ::RestoreDC(drawDc(), m_clipStack.last());

  ::SelectObject(drawDc(), hPen);
  ::SelectObject(drawDc(), hBrush);

  m_clipStack.removeLast();
}

bool ExWinGsCompositeDevice::isWindowRequired(ExGsCompositeView &pView) const
{
  if (pView.compositeDevice()->supportComposition())
  {
    return dynamic_cast<ExGsCompositeClientDeviceIface*>(pView.compositeDevice())->requireWindow();
  }
  return false;
}

void ExWinGsCompositeDevice::setClearScreen(bool bSet)
{
  m_bClearScreen = bSet;
}

bool ExWinGsCompositeDevice::clearScreen() const
{
  return m_bClearScreen;
}

void ExWinGsCompositeDevice::setDiscardBackFaces(bool bSet)
{
  m_bDiscardBackFaces = bSet;
}

bool ExWinGsCompositeDevice::discardBackFaces() const
{
  return m_bDiscardBackFaces;
}

void ExWinGsCompositeDevice::setUseTextOut(bool bSet)
{
  m_bUseTextOut = bSet;
}

bool ExWinGsCompositeDevice::useTextOut() const
{
  return m_bUseTextOut;
}

void ExWinGsCompositeDevice::setUseTTFCache(bool bSet)
{
  m_bUseTTFCache = bSet;
}

bool ExWinGsCompositeDevice::useTTFCache() const
{
  return m_bUseTTFCache;
}

void ExWinGsCompositeDevice::setUseLutPalette(OdUInt32 nSet)
{
  m_nUseLutPalette = nSet;
}

OdUInt32 ExWinGsCompositeDevice::useLutPalette() const
{
  return m_nUseLutPalette;
}

// Snapshot support

void ExWinGsCompositeDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  getSnapShot(pImage, region, NULL);
}

#include "../SnapshotSupport.h"
#include "Gi/GiRasterWrappers.h"
#include "ExGsBitmapDevice.h"

void ExWinGsCompositeDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView)
{
  struct SnapshotCallback : public OdGiSnapshotImageCallback
  {
    OdGiRasterImagePtr snapshotImageParams(OdGsBaseVectorizeDevice &runDevice) const
    {
      return static_cast<ExWinGsCompositeDevice&>(runDevice).snapshotImageParams();
    }
    OdGiRasterImagePtr snapshotRegion(OdGsBaseVectorizeDevice &runDevice, const OdGsDCRect &area, bool bCrop) const
    {
      return static_cast<ExWinGsCompositeDevice&>(runDevice).snapshotRegion(area, bCrop);
    }
  } _cbk;
  OdGiSnapshotImageImpl::getSnapshot(*this, &_cbk, pImage, region, pView);
}

OdGiRasterImagePtr ExWinGsCompositeDevice::snapshotImageParams() const
{
  OdSmartPtr<OdGiRasterImageDesc> pInfo = OdGiRasterImageDesc::createObject((OdUInt32)width(), (OdUInt32)height());
  HDC hDc = drawDc();
  if (!hDc)
    return OdGiRasterImagePtr();
  pInfo->setColorDepth(::GetDeviceCaps(hDc, BITSPIXEL));
  pInfo->pixelFormat().setBGR();
  pInfo->setScanLinesAlignment(4);
  if (pInfo->colorDepth() <= 8)
    pInfo->setPalette(m_logPalette.size() * sizeof(ODCOLORREF), (OdUInt8*)m_logPalette.asArrayPtr());
  else if (pInfo->colorDepth() == 32)
    pInfo->pixelFormat().setBGRA();
  return pInfo;
}

OdGiRasterImagePtr ExWinGsCompositeDevice::snapshotRegion(const OdGsDCRect &area, bool /*bCrop*/) const
{
  HDC hDc = drawDc();
  if (!hDc || !width() || !height())
    return OdGiRasterImagePtr();
  OdGiRasterImagePtr pInfo = snapshotImageParams();
  OdSmartPtr<OdGiSnapshotImageImpl> pImage = OdRxObjectImpl<OdGiSnapshotImageImpl>::createObject();
  pImage->setupForOriginal(snapshotImageParams());
  pImage->setupSurface(OdUInt32(area.m_max.x - area.m_min.x), OdUInt32(area.m_max.y - area.m_min.y), pImage->colorDepth());
  HDC copyDc = ::CreateCompatibleDC(hDc);
  {
    GsDIBSection dibSec; dibSec.setHDC(copyDc);
    dibSec.createDib((int)pImage->pixelWidth(), (int)pImage->pixelHeight(), (int)pImage->colorDepth());
    ::BitBlt(copyDc, 0, 0, (int)dibSec.getDibWidth(), (int)dibSec.getDibHeight(), hDc, area.m_min.x, height() - area.m_max.y, SRCCOPY);
    ::memcpy(pImage->scanLines(), dibSec.m_pBits, pImage->scanLineSize() * pImage->pixelHeight());
  }
  ::DeleteDC(copyDc);
  return pImage;
}

//
