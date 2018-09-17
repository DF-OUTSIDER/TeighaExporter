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
// GLES2Module.cpp : Defines the initialization routines for the DLL.

#include "OdaCommon.h"
#include "RxDispatchImpl.h"
#include "RxDynamicModule.h"
#include "Gs/GsBaseVectorizeDevice.h" // for OdSmartPtr<OdGsBaseVectorizeDevice>
#include "Gs/GsViewImpl.h"            // for OdSmartPtr<OdGsViewImpl>
#include "../vec/TrVecBaseModule.h"
#include "ExGsGLES2LocalRenditionGsClient.h"
#include "render/gl2/GLES2Include.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)

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

#define OD_GLES2_WIN_RESOURCES

#endif //_TOOLKIT_IN_DLL_

#ifdef OD_GLES2_WIN_RESOURCES
#include "win/ExGsGLES2RSTestDialog.h"
#endif // OD_GLES2_WIN_RESOURCES

// Base device for platform-dependent screen and bitmap devices
class OdGLES2PlatformBaseDevice : public OdTrVectorizerModuleHost
{
protected:
  OdTrGL2LocalRenditionClientPtr m_pRenditionClient;
  OdUInt32 m_nCheckVersion;
public:
  ODRX_DECLARE_PROPERTY_TRV(RenderSettings)
  ODRX_DECLARE_PROPERTY_TRV(SaveContextData)
  ODRX_DECLARE_PROPERTY_TRV(CheckRendererVersion)

  OdGLES2PlatformBaseDevice() : m_nCheckVersion(0) { }

  void setRenditionClient(OdTrGL2LocalRenditionClient *pRenditionClient) { m_pRenditionClient = pRenditionClient; }

  OdTrVisRenditionPtr createRendition() { return OdTrGL2LocalRenditionHostPtr().create(m_pRenditionClient)->rendition(); }
  OdTrGL2LocalRenditionHostPtr renditionHost() const
  { return reinterpret_cast<OdTrVisRendition*>(renderClient()->getProperty(OD_T("Rendition"))->getIntPtr()); }

  virtual void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen = false, bool bMobile = false, bool bDynamicVBO = false)
  {
    if (bMobile)
    {
      pRenderClient->setProperty(OD_T("ShareableDevice"), OdRxVariantValue(false));
      pRenderClient->setProperty(OD_T("LightSourcesLimit"), OdRxVariantValue(OdInt32(-2)));
    }
    else
      pRenderClient->setProperty(OD_T("CombinedShadersStrategy"), OdRxVariantValue(true));
    pRenderClient->setProperty(OD_T("UseOverlays"), OdRxVariantValue(!bOffScreen));
    if (bDynamicVBO)
    {
      pRenderClient->setProperty(OD_T("VBOLevel"), OdRxVariantValue(OdUInt32(3)));
      pRenderClient->setProperty(OD_T("StaticVBOLimit"), OdRxVariantValue(OdUInt32(0)));
    }
  }
  void configureForPlatform(OdTrVisRenderClient *pRenderClient, DeviceSetupType devType)
  { configureForPlatform(pRenderClient, devType == kDtOffScreen); }

  void put_RenderSettings(OdIntPtr) { }
  OdIntPtr get_RenderSettings() const { return (OdIntPtr)renditionHost()->renderSettingsManager(); }

  void put_SaveContextData(bool bSet) { renditionHost()->processContextData(bSet); }
  bool get_SaveContextData() const { return renditionHost()->hasContextData(); }

  void put_CheckRendererVersion(OdUInt32 nCheckVersion) { m_nCheckVersion = nCheckVersion; }
  OdUInt32 get_CheckRendererVersion() const { return renditionHost().checkRendererVersion(renderClient(), m_nCheckVersion); }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_GENERATE_PROPERTY_TRV(RenderSettings)
    ODRX_GENERATE_PROPERTY_TRV(SaveContextData)
    ODRX_GENERATE_PROPERTY_TRV(CheckRendererVersion)
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdGLES2PlatformBaseDevice::, RenderSettings      , OdGLES2PlatformBaseDevice, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdGLES2PlatformBaseDevice::, SaveContextData     , OdGLES2PlatformBaseDevice, getBool)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdGLES2PlatformBaseDevice::, CheckRendererVersion, OdGLES2PlatformBaseDevice, getUInt32)

// Base device for platform-dependent screen devices
class _OdPlatformGLES2Device : public OdGLES2PlatformBaseDevice
{
protected:
  bool m_bCreateContext;
public:
  ODRX_DECLARE_PROPERTY_TRV(CreateContext)

  _OdPlatformGLES2Device() : m_bCreateContext(true) {
  }

  void put_CreateContext(bool bSet) { m_bCreateContext = bSet; }
  bool get_CreateContext() const { return m_bCreateContext; }

  DeviceSetupType deviceSetupOverride() const { return kDtOnScreen; }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(OdGLES2PlatformBaseDevice);
    ODRX_GENERATE_PROPERTY_TRV(CreateContext)
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(_OdPlatformGLES2Device::, CreateContext , _OdPlatformGLES2Device, getBool)

#if defined(TD_USE_QT_LIB)

/* Qt-based cross-platform device implementation */

//#include <QtCore>
//#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
//#include <QtWidgets>
//#else
//#include <QtGui>
//#endif
#include <QGLWidget>
#include <QPointer>
#include <QObject>

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
protected:
  QPointer<QGLWidget> m_pGLWidget;

public:
  ODRX_DECLARE_PROPERTY_TRV(QGLWidget)
  OdPlatformGLES2Device() : m_pGLWidget(NULL) { }
  QGLWidget* put_QGLWidget(QGLWidget* pGLWidget)
  {
    if (pGLWidget == m_pGLWidget)
      return pGLWidget;
    QGLWidget* pGLWidgetPrev = m_pGLWidget;
    m_pGLWidget = pGLWidget;
    return pGLWidgetPrev;
  }

  OdIntPtr put_QGLWidget(OdIntPtr ptrGLWidget)
  {
    QGLWidget* pGLWidget = qobject_cast<QGLWidget*>((QObject*) ptrGLWidget);
    return (OdIntPtr)(QObject*) put_QGLWidget(pGLWidget);
  }

  OdIntPtr get_QGLWidget() const
  {
    return (OdIntPtr)(QObject*)m_pGLWidget;
  }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
    ODRX_GENERATE_PROPERTY_TRV(QGLWidget)
  }

  void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen, bool bMobile, bool bDynamicVBO)
  {
#if defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
    bMobile = true;
#endif
    _OdPlatformGLES2Device::configureForPlatform(pRenderClient, bOffScreen, bMobile, bDynamicVBO);
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, QGLWidget, OdPlatformGLES2Device, getIntPtr)

class OdqOffscreenGles : public QGLWidget
{
protected:
  OdTrVectorizerUpdateHost *m_pCb;
  OdGsDCRect *m_pRect;

  void paintGL()
  {
    m_pCb->hostUpdate(m_pRect);
  }

public:
  explicit OdqOffscreenGles(const QGLFormat& format, Qt::WindowFlags f = 0)
    : QGLWidget(format, 0, 0, f)
    , m_pCb(NULL)
    , m_pRect(NULL)
  {
  }

public:
  QPixmap renderPixmap(OdTrVectorizerUpdateHost *pCb,
                       OdTrVisRenderClient *pClient,
                       OdGsDCRect *pRect)
  {
    m_pCb = pCb;
    m_pRect = pRect;
    return QGLWidget::renderPixmap(pClient->outputWindowWidth(), pClient->outputWindowHeight()); //, true);
  }

  void renderToImage(OdTrVectorizerUpdateHost *pCb,
                     OdTrVisRenderClient *pClient,
                     OdGsDCRect *pRect)
  {
    m_pCb = pCb;
    m_pRect = pRect;

    makeCurrent();
    QGLContext* pContext = const_cast<QGLContext*>(context());
    ODA_ASSERT_ONCE(pContext);
    if (pContext)
      pContext->create();

    makeCurrent();
    initializeGL();
    int w = pClient->outputWindowWidth(),
        h = pClient->outputWindowHeight();
    resizeGL(w, h);
    paintGL();
//#  ifdef _DEBUG
//    QImage img = grabFrameBuffer();
//    int w_ = img.width();
//    int h_ = img.height();
//    ODA_ASSERT_ONCE(w == w_ && h == h_);
//    img.save("e:\\_Oda\\data\\_print_plot\\SUP-926 Whiteprint\\grab.bmp", "BMP");
//#  endif
  }

  virtual QSize sizeHint() const
  {
    if (m_pRect)
      return QGLWidget::sizeHint();
    QSize sz(Od_abs(m_pRect->m_max.x - m_pRect->m_min.x),
             Od_abs(m_pRect->m_max.y - m_pRect->m_min.y));
    return sz;
  }
};

#elif defined(_WIN32)

/* Windows platform device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
protected:
  OdIntPtr m_WindowHDC;
  OdIntPtr m_WindowHWND;
  OdIntPtr m_WGLContext;
#ifdef OD_GLES2_WIN_RESOURCES
  OdGLES2RSTestDialog m_rsTestDlg;
#endif // OD_GLES2_WIN_RESOURCES
public:
  ODRX_DECLARE_PROPERTY_TRV(WindowHDC)
  ODRX_DECLARE_PROPERTY_TRV(WindowHWND)
  ODRX_DECLARE_PROPERTY_TRV(WGLContext)
#ifdef OD_GLES2_WIN_RESOURCES
  ODRX_DECLARE_PROPERTY_TRV(RenderSettingsDlg)
#endif // OD_GLES2_WIN_RESOURCES

  OdPlatformGLES2Device() : m_WindowHDC(NULL), m_WindowHWND(NULL), m_WGLContext(NULL) { }
  void put_WindowHDC(OdIntPtr hDc) { m_WindowHDC = hDc; }
  OdIntPtr get_WindowHDC() const { return m_WindowHDC; }
  void put_WindowHWND(OdIntPtr hWnd) { m_WindowHWND = hWnd; }
  OdIntPtr get_WindowHWND() const { return m_WindowHWND; }
  void put_WGLContext(OdIntPtr glCtx) { m_WGLContext = glCtx; }
  OdIntPtr get_WGLContext() const { return m_WGLContext; }
#ifdef OD_GLES2_WIN_RESOURCES
  ~OdPlatformGLES2Device()
  {
    m_rsTestDlg.destroyDialogWindow();
  }
  void put_RenderSettingsDlg(OdIntPtr hParentWnd)
  {
    if (!m_rsTestDlg.isDialogPresent())
      m_rsTestDlg.createDialogWindow((HWND)hParentWnd,
                                     (HWND)m_WindowHWND,
                                     (OdTrRndRenderSettingsManager*)get_RenderSettings());
  }
  OdIntPtr get_RenderSettingsDlg() const
  {
    if (m_rsTestDlg.isDialogPresent())
      return 1;
    return 0;
  }
#endif // OD_GLES2_WIN_RESOURCES

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
    ODRX_GENERATE_PROPERTY_TRV(WindowHDC)
    ODRX_GENERATE_PROPERTY_TRV(WindowHWND)
    ODRX_GENERATE_PROPERTY_TRV(WGLContext)
#ifdef OD_GLES2_WIN_RESOURCES
    ODRX_GENERATE_PROPERTY_TRV(RenderSettingsDlg)
#endif // OD_GLES2_WIN_RESOURCES
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, WindowHDC        , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, WindowHWND       , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, WGLContext       , OdPlatformGLES2Device, getIntPtr)
#ifdef OD_GLES2_WIN_RESOURCES
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, RenderSettingsDlg, OdPlatformGLES2Device, getIntPtr)
#endif // OD_GLES2_WIN_RESOURCES

#elif defined(OD_TRGL2_IOS)

/* iOS platform device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
protected:
  OdIntPtr m_EAGLLayer;
  OdIntPtr m_EAGLContext;
public:
  ODRX_DECLARE_PROPERTY_TRV(EAGLLayer)
  ODRX_DECLARE_PROPERTY_TRV(EAGLContext)

  OdPlatformGLES2Device() : m_EAGLLayer(NULL), m_EAGLContext(NULL) { }
  void put_EAGLLayer(OdIntPtr pLayer) { m_EAGLLayer = pLayer; }
  OdIntPtr get_EAGLLayer() const { return m_EAGLLayer; }
  void put_EAGLContext(OdIntPtr pContext) { m_EAGLContext = pContext; }
  OdIntPtr get_EAGLContext() const { return m_EAGLContext; }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
    ODRX_GENERATE_PROPERTY_TRV(EAGLLayer)
    ODRX_GENERATE_PROPERTY_TRV(EAGLContext)
  }

  void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen, bool bMobile, bool bDynamicVBO)
  { bMobile = true;
    _OdPlatformGLES2Device::configureForPlatform(pRenderClient, bOffScreen, bMobile, bDynamicVBO);
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EAGLLayer  , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EAGLContext, OdPlatformGLES2Device, getIntPtr)

#elif defined(ANDROID)

/* Android platform device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
protected:
  OdIntPtr m_NativeWindow;
  OdIntPtr m_WindowDisplay;
  OdIntPtr m_EGLDisplay;
  OdIntPtr m_EGLSurface;
  OdIntPtr m_EGLContext;
public:
  ODRX_DECLARE_PROPERTY_TRV(ANativeWindow)
  ODRX_DECLARE_PROPERTY_TRV(WindowDisplay)
  ODRX_DECLARE_PROPERTY_TRV(EGLDisplay)
  ODRX_DECLARE_PROPERTY_TRV(EGLSurface)
  ODRX_DECLARE_PROPERTY_TRV(EGLContext)

  OdPlatformGLES2Device() : m_NativeWindow(NULL), m_WindowDisplay(NULL),
                            m_EGLDisplay(NULL), m_EGLSurface(NULL), m_EGLContext(NULL) { }
  void put_ANativeWindow(OdIntPtr pWindow) { m_NativeWindow = pWindow; }
  OdIntPtr get_ANativeWindow() const { return m_NativeWindow; }
  void put_WindowDisplay(OdIntPtr pDisplay) { m_WindowDisplay = pDisplay; }
  OdIntPtr get_WindowDisplay() const { return m_WindowDisplay; }
  void put_EGLDisplay(OdIntPtr pDisplay) { m_EGLDisplay = pDisplay; }
  OdIntPtr get_EGLDisplay() const { return m_EGLDisplay; }
  void put_EGLSurface(OdIntPtr pSurf) { m_EGLSurface = pSurf; }
  OdIntPtr get_EGLSurface() const { return m_EGLSurface; }
  void put_EGLContext(OdIntPtr pCtx) { m_EGLContext = pCtx; }
  OdIntPtr get_EGLContext() const { return m_EGLContext; }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
    ODRX_GENERATE_PROPERTY_TRV(ANativeWindow)
    ODRX_GENERATE_PROPERTY_TRV(WindowDisplay)
    ODRX_GENERATE_PROPERTY_TRV(EGLDisplay)
    ODRX_GENERATE_PROPERTY_TRV(EGLSurface)
    ODRX_GENERATE_PROPERTY_TRV(EGLContext)
  }

  void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen, bool bMobile, bool bDynamicVBO)
  { bMobile = true;
    _OdPlatformGLES2Device::configureForPlatform(pRenderClient, bOffScreen, bMobile, bDynamicVBO);
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, ANativeWindow, OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, WindowDisplay, OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EGLDisplay   , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EGLSurface   , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, EGLContext   , OdPlatformGLES2Device, getIntPtr)

#elif defined(EMCC)

/* Emscripten cross-compiler device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device {
public:
  OdPlatformGLES2Device() {}

  DeviceSetupType deviceSetupOverride() const { return kDtOffScreen; } // Avoid partial update generation

  void configureForPlatform(OdTrVisRenderClient *pRenderClient, bool bOffScreen, bool bMobile, bool bDynamicVBO)
  { bOffScreen = false; /* To enable overlays */ bMobile = true; /* To disable sharing */ bDynamicVBO = true; /* WebGL specific */
    _OdPlatformGLES2Device::configureForPlatform(pRenderClient, bOffScreen, bMobile, bDynamicVBO);
  }
};

#elif defined(__linux__)

/* Linux platform device implementation */

OdIntPtr odGLES2X11ChooseVisual(OdIntPtr display, bool bDoubleBufferEnabled);

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
protected:
  OdIntPtr m_xDisplay;
  OdIntPtr m_xWindow;
  mutable OdIntPtr m_xVisual;
  OdIntPtr m_xContext;
public:
  ODRX_DECLARE_PROPERTY_TRV(XDisplay)
  ODRX_DECLARE_PROPERTY_TRV(XWindow)
  ODRX_DECLARE_PROPERTY_TRV(XChooseVisual)
  ODRX_DECLARE_PROPERTY_TRV(GLXContext)

  OdPlatformGLES2Device()
  {
    m_xDisplay = 0;
    m_xWindow = 0;
    m_xVisual = 0;
    m_xContext = 0;
  }
  OdIntPtr getXDisplay() const { return m_xDisplay; }
  void setXDisplay(OdIntPtr dp) { m_xDisplay = dp; }
  OdIntPtr getXWindow() const { return m_xWindow; }
  void setXWindow(OdIntPtr win) { m_xWindow = win; }
  void setChooseVisual(OdIntPtr /*vinfo*/) { /* Could be answered only */ }
  OdIntPtr chooseVisual() const
  {
    if (!m_xVisual)
      m_xVisual = ::odGLES2X11ChooseVisual(m_xDisplay, m_pRenderClient->getProperty(OD_T("DoubleBufferEnabled"))->getBool());
    return m_xVisual;
  }
  OdIntPtr getXContext() const { return m_xContext; }
  void setXContext(OdIntPtr ctx) { m_xContext = ctx; }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
    ODRX_GENERATE_PROPERTY_TRV(XDisplay)
    ODRX_GENERATE_PROPERTY_TRV(XWindow)
    ODRX_GENERATE_PROPERTY_TRV(XChooseVisual)
    ODRX_GENERATE_PROPERTY_TRV(GLXContext)
  }
};

ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdPlatformGLES2Device::, XDisplay     , OdPlatformGLES2Device, getXDisplay , setXDisplay    , getIntPtr)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdPlatformGLES2Device::, XWindow      , OdPlatformGLES2Device, getXWindow  , setXWindow     , getIntPtr)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdPlatformGLES2Device::, XChooseVisual, OdPlatformGLES2Device, chooseVisual, setChooseVisual, getIntPtr)
ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(OdPlatformGLES2Device::, GLXContext   , OdPlatformGLES2Device, getXContext , setXContext    , getIntPtr)

#elif defined(__APPLE__)

/* MacOS platform device implementation */

class OdPlatformGLES2Device : public _OdPlatformGLES2Device
{
protected:
  OdIntPtr m_NSView;
  OdIntPtr m_NSOpenGLContext;
public:
  ODRX_DECLARE_PROPERTY_TRV(NSView)
  ODRX_DECLARE_PROPERTY_TRV(NSOpenGLContext)

  OdPlatformGLES2Device() : m_NSView(NULL), m_NSOpenGLContext(NULL) { }
  void put_NSView(OdIntPtr pWindow) { m_NSView = pWindow; }
  OdIntPtr get_NSView() const { return m_NSView; }
  void put_NSOpenGLContext(OdIntPtr pContext) { m_NSOpenGLContext = pContext; }
  OdIntPtr get_NSOpenGLContext() const { return m_NSOpenGLContext; }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    ODRX_INHERIT_PROPERTIES_TRV(_OdPlatformGLES2Device);
    ODRX_GENERATE_PROPERTY_TRV(NSView)
    ODRX_GENERATE_PROPERTY_TRV(NSOpenGLContext)
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, NSView         , OdPlatformGLES2Device, getIntPtr)
ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformGLES2Device::, NSOpenGLContext, OdPlatformGLES2Device, getIntPtr)

#else

#pragma MARKMESSAGE("Unknown platform.");

typedef _OdPlatformGLES2Device OdPlatformGLES2Device;

#endif

#ifndef TD_USE_QT_LIB

class OdPlatformBitmapGLES2Device : public OdGLES2PlatformBaseDevice
{
  public:
    OdPlatformBitmapGLES2Device() {}

    DeviceSetupType deviceSetupOverride() const
    {
      if (renderClient()->hasProperty(OD_T("ForcePartialUpdate")) &&
          renderClient()->getProperty(OD_T("ForcePartialUpdate"))->getBool())
        return kDtOnScreen;
      return kDtOffScreen;
    }
};

#else // TD_USE_QT_LIB

class OdPlatformBitmapGLES2Device : public OdPlatformGLES2Device
{
  ODRX_DECLARE_PROPERTY_TRV(QImageFrame)
protected:
  QImage* m_pFrame;
  OdSharedPtr<OdqOffscreenGles> m_pCtx;
public:
  ODRX_USING_HEAP_OPERATORS(OdPlatformGLES2Device);

  OdPlatformBitmapGLES2Device()
    : m_pFrame(NULL)
  {
  }

  void createContext()
  {
    if (m_pCtx.get())
      return;

    m_pRenderClient->setProperty(OD_T("DoubleBufferEnabled"), OdRxVariantValue(false));
    QGLFormat format = QGLFormat::defaultFormat();
    format.setDoubleBuffer(false);
    m_pCtx = new OdqOffscreenGles(format);
    m_pGLWidget = m_pCtx;
  }

  void generateProperties(OdRxDictionary* pInfo) const
  {
    if (pInfo->has("RasterImage")) pInfo->remove(OD_T("RasterImage"));
    if (pInfo->has("BitPerPixel")) pInfo->remove(OD_T("BitPerPixel"));
    ODRX_INHERIT_PROPERTIES_TRV(OdPlatformGLES2Device);
    ODRX_GENERATE_PROPERTY_TRV(QImageFrame)
  }

  bool onRenderingBegin(OdGsDCRect* pUpdatedRect = NULL, OdTrVectorizerUpdateHost* pUpdateHost = NULL)
  {
    ODA_ASSERT(m_pFrame);
    if (m_pFrame)
    {
      if (m_pCtx.isNull())
        createContext();
      int w = m_pRenderClient->outputWindowWidth(),
          h = m_pRenderClient->outputWindowHeight();
      if (pUpdatedRect)
      {
        w = Od_abs(pUpdatedRect->m_max.x - pUpdatedRect->m_min.x);
        h = Od_abs(pUpdatedRect->m_max.y - pUpdatedRect->m_min.y);
      }

      // resize gives us incorrect result without it
      m_pCtx->setMaximumWidth(w);  m_pCtx->setMinimumWidth(w); 
      m_pCtx->setMaximumHeight(h); m_pCtx->setMinimumHeight(h); 
      //
      m_pCtx->resize(w, h);
      ODA_ASSERT_VAR(QSize sz = m_pCtx->size();)
      ODA_ASSERT_ONCE(w == sz.width() && h == sz.height());

      m_pCtx->renderToImage(pUpdateHost, m_pRenderClient, pUpdatedRect);
      *m_pFrame = m_pFrame->mirrored();
      // @@@test
      //if (false)
      //  m_pFrame->save("e:\\_Oda\\data\\_print_plot\\SUP-926 Whiteprint\\test.bmp", "BMP");
    }
    return false;
  }

  bool hasDirectRenderBuffer() const
  {
    return true;
  }
  OdUInt8 *getDirectRenderBuffer(OdUInt32* pWidth, OdUInt32* pHeight, OdUInt32* pFormat = NULL, OdUInt32* pAlignment = NULL)
  {
    OdUInt32 width = m_pRenderClient->outputWindowWidth(),
             height = m_pRenderClient->outputWindowHeight(),
             format = OdTrVisRenderClient::kRBufFmtRGB,
             alignment = 4;
    if (pWidth) *pWidth = width;
    if (pHeight) *pHeight = height;
    if (pFormat) *pFormat = format;
    if (pAlignment) *pAlignment = alignment;
    ODA_ASSERT(m_pFrame);
    if (m_pFrame)
    {
      ODA_ASSERT_ONCE(alignment == 4 || alignment == 1);

//#  if QT_VERSION < QT_VERSION_CHECK(5,0,0)
      enum QImage::Format fmt = QImage::Format_RGB888; // = QImage::Format_RGB32; 
      //format = OdTrVisRenderClient::kRBufFmtRGB;
//#  else
//    enum QImage::Format fmt = QImage::Format_RGBA8888;
//    format = OdTrVisRenderClient::kRBufFmtRGBA;
//#  endif
      if (pFormat) *pFormat = format,

      *m_pFrame = QImage(width, height, fmt);
      uchar* pBits = m_pFrame->bits();

      ODA_ASSERT_VAR(int szColors = m_pFrame->colorCount();)
      ODA_ASSERT_ONCE(!szColors);
      ODA_ASSERT_VAR(int szPerLine = m_pFrame->bytesPerLine();)
      ODA_ASSERT_ONCE(   (alignment == 4 && szPerLine == OdGiRasterImage::calcBMPScanLineSize(width, 24))
                      || (alignment == 1 && szPerLine == width * 3));
      m_pFrame->fill(Qt::white);

      return pBits;
    }
    return NULL;
  }

  void put_QImageFrame(OdIntPtr i) {
    m_pFrame = (QImage*)i;
  }
  OdIntPtr get_QImageFrame() const {
    return (OdIntPtr)m_pFrame;
  }
};

ODRX_DEFINE_PROPERTY_PREFIX_TRV(OdPlatformBitmapGLES2Device::, QImageFrame, OdPlatformBitmapGLES2Device, getIntPtr)

#endif // TD_USE_QT_LIB

class GLES2Module : public BaseGLES2Module
{
protected:
  OdTrGL2LocalRenditionClientPtr m_pRenditionClient;
  OdTrVectorizerModuleHostPtr setupModuleHost(OdTrVectorizerModuleHost *pModuleHost)
  {
    if (m_pRenditionClient.isNull())
      m_pRenditionClient = OdTrGL2LocalRenditionGsClient::createObject();
    static_cast<OdGLES2PlatformBaseDevice*>(pModuleHost)->setRenditionClient(m_pRenditionClient);
    return pModuleHost;
  }
protected:
  OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  { return createVectorizeDevice(setupModuleHost(OdRxObjectImpl<OdPlatformGLES2Device, OdTrVectorizerModuleHost>::createObject()), OdTrVectorizerModuleHost::kDtOnScreen); }
  OdSmartPtr<OdGsViewImpl> createViewObject() { return createVectorizeView(); }

  OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  { return createVectorizeDevice(setupModuleHost(OdRxObjectImpl<OdPlatformBitmapGLES2Device, OdTrVectorizerModuleHost>::createObject()), OdTrVectorizerModuleHost::kDtOffScreen); }
  OdSmartPtr<OdGsViewImpl> createBitmapViewObject() { return createVectorizeView(); }

  virtual void initApp();
  virtual void uninitApp();
};

ODRX_DEFINE_DYNAMIC_MODULE(GLES2Module);

void GLES2Module::initApp() {
  BaseGLES2Module::initApp();
}

void GLES2Module::uninitApp() {
  BaseGLES2Module::uninitApp();
}

//
