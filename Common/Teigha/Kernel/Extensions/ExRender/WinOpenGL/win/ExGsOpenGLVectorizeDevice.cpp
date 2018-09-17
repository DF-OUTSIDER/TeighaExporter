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
#include "ExGsOpenGLVectorizeDevice.h"
#include "TD_GL.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiMetafile.h"
#include "OdRound.h"
#include "SaveState.h"
#include "OpenGLExtensions.h"
#include "OpenGLDrawBuffer.h"

#define ROUND(n) OdRoundToLong(n)

//#include "../ExOwnerDrawDc.cpp"
#include "../win/GdiBmp.h"

OD_OPENGL_DECLARE_COMMON_PROPS(ExGsOpenGLVectorizeDeviceBase, _undefined_, ExGsWin32Device);

ODRX_DECLARE_PROPERTY2(WGLContext, ExGsOpenGLVectorizeDevice)
ODRX_DECLARE_PROPERTY2(DepthBufferBits, ExGsOpenGLVectorizeDevice)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsOpenGLVectorizeDevice);
  ODRX_INHERIT_PROPERTIES(ExGsOpenGLVectorizeDeviceBase)
  ODRX_GENERATE_PROPERTY2(WGLContext, ExGsOpenGLVectorizeDevice)
  ODRX_GENERATE_PROPERTY2(DepthBufferBits, ExGsOpenGLVectorizeDevice)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsOpenGLVectorizeDevice);

ODRX_DEFINE_PROPERTY_METHODS2(WGLContext, ExGsOpenGLVectorizeDevice, ExGsOpenGLVectorizeDevice, getWGLContext, setWGLContext, getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS2(DepthBufferBits, ExGsOpenGLVectorizeDevice, ExGsOpenGLVectorizeDevice, getDepthBufferBits, setDepthBufferBits, getUInt8);

// Implementation of class ExGsOpenGLVectorizeDevice

//*******************************************************************************/
// Constructor
//*******************************************************************************/
ExGsOpenGLVectorizeDevice::ExGsOpenGLVectorizeDevice()
  : m_bContextCreated(false), m_hGLRC(NULL), m_hPrevDC(NULL)
#ifdef OD_OGL_USE_PBUFFER
  , m_pPBuffer(NULL)
#endif
{
#ifdef OD_OGL_USE_PBO
  m_pPBOs[3] = m_pPBOs[2] = m_pPBOs[1] = m_pPBOs[0] = 0;
#endif
  m_uDepthBufferBits = 24;
  setSupportInteractiveViewMode( true );
}

//*******************************************************************************/
// Destructor
//*******************************************************************************/
ExGsOpenGLVectorizeDevice::~ExGsOpenGLVectorizeDevice()
{
  deleteContext();
}

//*******************************************************************************/
//  createContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::createContext()
{
  DWORD dwFlags = (renderToScreen()) ? PFD_DRAW_TO_WINDOW : PFD_DRAW_TO_BITMAP;
  m_bDoubleBufferEnabled &= (renderToScreen() == true);

  //Support OpenGL
  dwFlags |= PFD_SUPPORT_OPENGL;
  int nPixelType = PFD_TYPE_RGBA;

  if (m_bDoubleBufferEnabled)
  {
    dwFlags |= PFD_DOUBLEBUFFER;
  }
  m_bColorIndexMode = false;
  if (m_nColorDepth < 16)
  {
    dwFlags |= PFD_NEED_PALETTE;
    m_bColorIndexMode = true;
  }

  PIXELFORMATDESCRIPTOR pfd;
  ::memset(&pfd,0, sizeof(PIXELFORMATDESCRIPTOR));
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;                          // Version number
  pfd.dwFlags = dwFlags;
  pfd.iPixelType = (OdUInt8)nPixelType;
  pfd.cColorBits = (OdUInt8)m_nColorDepth;
  pfd.cDepthBits = m_uDepthBufferBits;       // 32-bit depth buffer
  pfd.iLayerType = PFD_MAIN_PLANE;           // Layer type
  pfd.cStencilBits = 2; //1;                 // Extended stencil buffer for hide mode

  // Choose the pixel format.
  int nPixelFormat = ::ChoosePixelFormat(m_hDC, &pfd);
  ODA_ASSERT(nPixelFormat != 0);

#if 0
  PIXELFORMATDESCRIPTOR pfdChk;
  pfdChk.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfdChk.nVersion = 1;
  ::DescribePixelFormat(m_hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfdChk);
#endif

  // Set the pixel format.
  if (::SetPixelFormat(m_hDC, nPixelFormat, &pfd) == FALSE)
  {
    SETBIT(pfd.dwFlags, PFD_NEED_PALETTE, false);
    SETBIT(pfd.dwFlags, PFD_DRAW_TO_WINDOW, false);
    SETBIT(pfd.dwFlags, PFD_DRAW_TO_BITMAP, true);
    pfd.cColorBits = 24;
    m_dibSection.createDib(width(), height(), 24);
    nPixelFormat = ::ChoosePixelFormat(m_dibSection.getHDC(), &pfd);
    ODA_ASSERT(nPixelFormat != 0);
    ODA_VERIFY(::SetPixelFormat(m_dibSection.getHDC(), nPixelFormat, &pfd));
    m_hPrevDC = m_hDC;
  }

  // Create the palette
  createPalette(&m_logPalette);

  if (renderToScreen())
  {
    if (m_pPal)
    {
      SelectPalette(m_hDC, m_pPal, 0);
      RealizePalette(m_hDC);
    }
  }

  if (m_hPrevDC)
    m_hDC = m_dibSection.getHDC();

  if (!m_hGLRC)
  {
    m_hGLRC = ::wglCreateContext(m_hDC);
    m_bContextCreated = true;

    // Composition require some extensions, elsewhere we not able to readback FB pixels and turn back onto bitmap renderer
    if (compositionOrRedirectionEnabled() && !m_hPrevDC && !GETBIT(pfd.dwFlags, PFD_DRAW_TO_BITMAP))
    {
      ODA_VERIFY(::wglMakeCurrent(m_hDC, m_hGLRC));
#ifdef OD_OGL_USE_PBO
      // The best for us is a PBO
      if (isExtensionSupported(OpenGLExtension_ARB_VertexBuf) && isExtensionSupported(OpenGLExtension_ARB_PixelBuf))
      {
        ((OdGLFn_GenBuffers_ARB)getExtensionFunc(OpenGLExtFunc_ARB_GenBuffers))(
          4, m_pPBOs);
        // we can't allocate size here - it's unknown
        getExtensionFunc(OpenGLExtFunc_ARB_DeleteBuffers);
        if (!isRedirectionModeEnabled())
          m_bDoubleBufferEnabled = false;
      }
      else
#endif
#ifdef OD_OGL_USE_PBUFFER
      // Elsewhere we could try pbuffer
      if (isExtensionSupported(OpenGLExtension_ARB_PixelFormat) && isExtensionSupported(OpenGLExtension_ARB_PBuffer))
      {
        // Initialize PBuffer
        int pf_attr[] =
        {
          WGL_SUPPORT_OPENGL_ARB , TRUE,  // P-buffer will be used with OpenGL
          WGL_DRAW_TO_PBUFFER_ARB, TRUE,  // Enable render to p-buffer
          WGL_RED_BITS_ARB       , 8,     // At least 8 bits for RED channel
          WGL_GREEN_BITS_ARB     , 8,     // At least 8 bits for GREEN channel
          WGL_BLUE_BITS_ARB      , 8,     // At least 8 bits for BLUE channel
          WGL_ALPHA_BITS_ARB     , 8,     // At least 8 bits for ALPHA channel
          WGL_DEPTH_BITS_ARB     , 24,    // At least 24 bits for depth buffer
          WGL_STENCIL_BITS_ARB   , 2,     // At least 2 bits for stencil buffer
          WGL_DOUBLE_BUFFER_ARB  , FALSE, // We don't require double buffering
          0                               // Zero terminates the list
        };
        UINT nCount = 0;
        ((OdGLFn_ChoosePixelFormat_ARB)getExtensionFunc(OpenGLExtFunc_ARB_ChoosePixelFormat))(
          m_hDC, (const int*)pf_attr, NULL, 1, &nPixelFormat, &nCount);
        ODA_ASSERT(nCount != 0);
        m_pPBuffer = ((OdGLFn_CreatePBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_CreatePBuffer))(
          m_hDC, nPixelFormat, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN), NULL);
        m_hPrevDC = m_hDC;
        m_hDC = ((OdGLFn_GetPBufferDC_ARB)getExtensionFunc(OpenGLExtFunc_ARB_GetPBufferDC))(
          m_pPBuffer);
        ODA_ASSERT(m_pPBuffer);
        m_dibSection.createDib(width(), height(), 24);
        // precache routines
        getExtensionFunc(OpenGLExtFunc_ARB_ReleasePBufferDC);
        getExtensionFunc(OpenGLExtFunc_ARB_DestroyPBuffer);
        //
        if (m_bContextCreated)
        {
          m_bContextCreated = false;
          ::wglMakeCurrent(m_hPrevDC, NULL);
          ::wglDeleteContext(m_hGLRC);
          m_hGLRC = NULL;
        }
        m_bDoubleBufferEnabled = false;
        if (!m_hGLRC)
        {
          m_hGLRC = ::wglCreateContext(m_hDC);
          m_bContextCreated = true;
        }
      }
      else
#endif
      // Elsewhere we couldn't use glReadPixels from non top-level window. Rollback to bitmap context.
      {
        if (m_bContextCreated)
        {
          m_bContextCreated = false;
          ::wglMakeCurrent(m_hPrevDC, NULL);
          ::wglDeleteContext(m_hGLRC);
          m_hGLRC = NULL;
        }
        SETBIT(pfd.dwFlags, PFD_NEED_PALETTE, false);
        SETBIT(pfd.dwFlags, PFD_DRAW_TO_WINDOW, false);
        SETBIT(pfd.dwFlags, PFD_DRAW_TO_BITMAP, true);
        SETBIT(pfd.dwFlags, PFD_DOUBLEBUFFER, false);
        m_bDoubleBufferEnabled = false;
        pfd.cColorBits = 24;
        m_dibSection.createDib(width(), height(), 24);
        nPixelFormat = ::ChoosePixelFormat(m_dibSection.getHDC(), &pfd);
        ODA_ASSERT(nPixelFormat != 0);
        ODA_VERIFY(::SetPixelFormat(m_dibSection.getHDC(), nPixelFormat, &pfd));
        m_hPrevDC = m_hDC;
        m_hDC = m_dibSection.getHDC();
        if (!m_hGLRC)
        {
          m_hGLRC = ::wglCreateContext(m_hDC);
          m_bContextCreated = true;
        }
      }
    }
  }
  if (m_hGLRC)
  {
    ODA_VERIFY(::wglMakeCurrent(m_hDC, m_hGLRC));
  }

  onContextCreate();
}

//*******************************************************************************/
// deleteContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::deleteContext()
{
  ExGsWin32Device::deleteContext();
  if(m_hGLRC)
  {
    if (m_hGLRC != ::wglGetCurrentContext())
      ::wglMakeCurrent(m_hDC, m_hGLRC);
    onContextDelete();

#ifdef OD_OGL_USE_PBO
    if (m_pPBOs[0])
    {
      ((OdGLFn_DeleteBuffers_ARB)getExtensionFunc(OpenGLExtFunc_ARB_DeleteBuffers))(
        4, m_pPBOs);
      m_pPBOs[3] = m_pPBOs[2] = m_pPBOs[1] = m_pPBOs[0] = 0;
    }
#endif

    // make the rendering context not current
    ::wglMakeCurrent(m_hDC, NULL);

    // delete the rendering context
    if (m_bContextCreated)
      ::wglDeleteContext(m_hGLRC);
    m_hGLRC = NULL;

#ifdef OD_OGL_USE_PBUFFER
    if (m_pPBuffer)
    {
      ((OdGLFn_ReleasePBufferDC_ARB)getExtensionFunc(OpenGLExtFunc_ARB_ReleasePBufferDC))(
        m_pPBuffer, m_hDC);
      m_hDC = m_hPrevDC;
      ((OdGLFn_DestroyPBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_DestroyPBuffer))(
        m_pPBuffer);
      m_pPBuffer = NULL;
    }
#endif

    if (m_hPrevDC)
      m_hPrevDC = NULL;
  }
}

void ExGsOpenGLVectorizeDevice::preReleaseResource()
{
  if (m_hGLRC && (::wglGetCurrentContext() != m_hGLRC))
    ::wglMakeCurrent(m_hDC, m_hGLRC);
}
void ExGsOpenGLVectorizeDevice::postReleaseResource() { }

OdIntPtr ExGsOpenGLVectorizeDevice::getWGLContext() const
{
  return (OdIntPtr)m_hGLRC;
}

void ExGsOpenGLVectorizeDevice::setWGLContext(OdIntPtr ctx)
{
  m_hGLRC = (HGLRC)ctx;
}

OdUInt8 ExGsOpenGLVectorizeDevice::getDepthBufferBits() const
{
  return m_uDepthBufferBits;
}

void ExGsOpenGLVectorizeDevice::setDepthBufferBits(OdUInt8 uSet)
{
  m_uDepthBufferBits = uSet;
}

OdGsOpenGLVectorizeDevice::OpenGLExtSupport ExGsOpenGLVectorizeDevice::checkExtensionSupport(const char *pExtName, OpenGLExtension extIdx) const
{
  OpenGLExtSupport retBase = OdGsOpenGLVectorizeDevice::checkExtensionSupport(pExtName, extIdx);
  if (retBase == kExtNotSupported && pExtName && pExtName[0] == 'W' && pExtName[1] == 'G' && pExtName[2] == 'L')
  {
    OdGLFn_GetExtensionsString_ARB pWGLExt = (OdGLFn_GetExtensionsString_ARB)getExtensionFuncPtrFromListInt("wglGetExtensionsStringARB");
    if (pWGLExt)
    {
      if (OpenGLParseExtension_base(pExtName, pWGLExt(m_hDC)))
      {
        return checkExtensionFuncSupport(extIdx) ? kExtSupported : kExtNotSupported;
      }
    }
  }
  return retBase;
}

bool ExGsOpenGLVectorizeDevice::checkExtensionFuncSupport(OpenGLExtension /*extIdx*/) const
{
  // All extension functions provided
  return true;
}

void *ExGsOpenGLVectorizeDevice::getExtensionFuncPtrFromListInt(const char *pFuncName) const
{
  // We've always have similar internal and external prototypes on Windows
  return (void*)::wglGetProcAddress(pFuncName);
}

// Fix OdaDgnApp reentrance
#define EX_OGL_PREV_CTX_WRAP_BEGIN() \
  HGLRC hPrevCtx = ::wglGetCurrentContext(); \
  HDC hPrevDC = ::wglGetCurrentDC()
#define EX_OGL_PREV_CTX_WRAP_END(curGlRC) \
  if (hPrevCtx != NULL && hPrevCtx != curGlRC) \
    ::wglMakeCurrent(hPrevDC, hPrevCtx)

//*******************************************************************************/
//  update()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::update(OdGsDCRect* pRect)
{
  EX_OGL_PREV_CTX_WRAP_BEGIN();

  m_vectPerfData.m_options = (supportMultithreading()) ? eEnableParallelVectorization : 0;
  if (forcePartialUpdateForMt()) m_vectPerfData.m_options |= eForcePartialUpdateForTest;
  mtDeviceSync().setEnabled(m_vectPerfData.enableParallelVectorization());
  if (m_vectPerfData.enableParallelVectorization() && !gsMultithreadEnabled())
    setMaxMtRegenThreads(getMtRegenThreadsCount(userGiContext()));

  if(!m_hGLRC)
  {
    /* Java wrappers issue: if applicated was created on Java it is already will have window, dc, and WGL context, but
       them is inaccessible, so we could try to obtain them all from current process.                                  */
    if (!m_hDC && !m_hWnd && renderToScreen())
    {
      m_hGLRC = ::wglGetCurrentContext();
      if (m_hGLRC)
      {
        m_hDC = ::wglGetCurrentDC();
        m_hWnd = ::WindowFromDC(m_hDC);
      }
    }
    if (!m_hGLRC)
      createContext();
  }
  else
  {
    if (m_hPrevDC)
      m_dibSection.createDib(width(), height(), 24);
#ifdef OD_OGL_USE_PBUFFER
    if (m_pPBuffer)
    {
      int testWidth = 0, testHeight = 0;
      ((OdGLFn_QueryPBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_QueryPBuffer))(
        m_pPBuffer, WGL_PBUFFER_WIDTH_ARB, &testWidth);
      ((OdGLFn_QueryPBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_QueryPBuffer))(
        m_pPBuffer, WGL_PBUFFER_HEIGHT_ARB, &testHeight);
      if ((testWidth != ::GetSystemMetrics(SM_CXSCREEN)) || (testHeight != ::GetSystemMetrics(SM_CYSCREEN)))
      {
        deleteContext();
        createContext();
      }
    }
#endif
    ODA_VERIFY(::wglMakeCurrent(m_hDC, m_hGLRC));
  }

  ExGsOpenGLVectorizeDeviceBase::update(pRect);

  if (isClearScreenEnabled() && !isCompositionUpdateEnabled())
  {
    ::SwapBuffers(m_hDC);

    if (supportPartialUpdate() && m_bDoubleBufferEnabled)
    {
      // Some video drivers can't provide partial swap and always swaps entire back buffer. But data isn't synchronized between buffers,
      // and garbage could be visible in this case. This workaround require some time on additional copying, but there is no way to
      // detect if driver supports partial swap or not.
      { // Before doing copy we must clear alpha component which can influence onto copied image unpredictably.
        OdOpenGLFrontDrawBuffer _backRead;
        ::glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
        ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        ::glClear(GL_COLOR_BUFFER_BIT);
        ::glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      }
      OdOpenGLBackDrawBuffer _backBuf;
      OdOpenGLFrontReadBuffer _frontBuf;
      ::glCopyPixels(0, 0, width(), height(), GL_COLOR);
    }
  }

  ::glFinish();

  if (m_hPrevDC && !compositionOrRedirectionEnabled())
  {
    ::BitBlt(m_hPrevDC, 0, 0, width(), height(), m_hDC, 0, 0, SRCCOPY);
  }

  EX_OGL_PREV_CTX_WRAP_END(m_hGLRC);
}

void ExGsOpenGLVectorizeDevice::updateScreen()
{
  if(!isValid())
    OdGsOpenGLVectorizeDevice::updateScreen();
}

ExGsOpenGLVectorizeView::ExGsOpenGLVectorizeView()
  : ExGsOpenGLVectorizeViewBase()
  , m_bSelecting(false)
{
}

//*******************************************************************************/
//  polylineProc()
//*******************************************************************************/
void ExGsOpenGLVectorizeView::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  ExGsOpenGLVectorizeViewBase::polylineOut(nPoints, pPoints);
  if (!device()->doubleBufferEnabled() && !device()->forcePartialUpdateForMt())
    ::glFlush();
}

void ExGsOpenGLVectorizeView::meshProc(OdInt32 rows,
                                       OdInt32 columns,
                                       const OdGePoint3d* pVertexList,
                                       const OdGiEdgeData* pEdgeData,
                                       const OdGiFaceData* pFaceData,
                                       const OdGiVertexData* pVertexData)
{
  ExGsOpenGLVectorizeViewBase::meshProc(rows, columns, pVertexList, pEdgeData, pFaceData, pVertexData);
  if (!device()->doubleBufferEnabled() && !device()->forcePartialUpdateForMt())
    ::glFlush();
}

void ExGsOpenGLVectorizeView::shellProc(OdInt32 nbVertex,
                                        const OdGePoint3d* pVertexList,
                                        OdInt32 faceListSize,
                                        const OdInt32* pFaceList,
                                        const OdGiEdgeData* pEdgeData,
                                        const OdGiFaceData* pFaceData,
                                        const OdGiVertexData* pVertexData)
{
  ExGsOpenGLVectorizeViewBase::shellProc(nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData);
  if (!device()->doubleBufferEnabled() && !device()->forcePartialUpdateForMt())
    ::glFlush();
}

void ExGsOpenGLVectorizeView::rasterImageProc(const OdGePoint3d& origin,
                                              const OdGeVector3d& u,
                                              const OdGeVector3d& v,
                                              const OdGiRasterImage* pImage, // image object
                                              const OdGePoint2d* uvBoundary, // may not be null
                                              OdUInt32 numBoundPts,
                                              bool transparency,
                                              double brightness,
                                              double contrast,
                                              double fade)
{
  OdGsOpenGLVectorizeView::rasterImageProc(origin, u, v, pImage, uvBoundary, 
                                           numBoundPts, transparency, brightness, 
                                           contrast, fade);

  if (!device()->doubleBufferEnabled() && !device()->forcePartialUpdateForMt())
    ::glFlush();
}

void ExGsOpenGLVectorizeView::metafileProc(const OdGePoint3d& origin,
                                           const OdGeVector3d& u,
                                           const OdGeVector3d& v,
                                           const OdGiMetafile* pMetafile,
                                           bool /*bDcAligned*/,
                                           bool /*bAllowClipping*/)
{
  if(!pMetafile || renderMode() != OdGsView::k2DOptimized)
    return;

  OdGePoint3d origin2;
  OdGeVector3d u2;
  OdGeVector3d v2;
  RECT rect;
  //if(bDcAligned)
  //{
    OdGeExtents3d ext(origin, origin);
    ext.addPoint(origin + u);
    ext.addPoint(origin + v);
    ext.addPoint(origin + u + v);

    OdGePoint3d imgMin = ext.minPoint();
    OdGePoint3d imgMax = ext.maxPoint();

    OdGePoint3d vpMin = view().viewingMatrix() * view().target();
    OdGeVector3d halfEyeDiag(view().fieldWidth() / 2., view().fieldHeight() / 2., 0.);
    OdGePoint3d vpMax = vpMin + halfEyeDiag;
    vpMin -= halfEyeDiag;

    if(imgMin.x < vpMin.x)
      imgMin.x = vpMin.x;
    if(imgMin.y < vpMin.y)
      imgMin.y = vpMin.y;
    if(imgMax.x > vpMax.x)
      imgMax.x = vpMax.x;
    if(imgMax.y > vpMax.y)
      imgMax.y = vpMax.y;

    origin2 = imgMin;
    v2 = u2 = imgMax - imgMin;
    u2.z = u2.y =0.;
    v2.z = v2.x =0.;

    OdGeMatrix3d x = view().screenMatrix() * view().projectionMatrix();
    ext.transformBy(x);
    imgMax.transformBy(x);
    imgMin.transformBy(x);
    vpMax.transformBy(x);
    vpMin.transformBy(x);

    rect.left   = ROUND(ext.minPoint().x);
    rect.right  = ROUND(ext.maxPoint().x);
    rect.top    = ROUND(ext.minPoint().y);
    rect.bottom = ROUND(ext.maxPoint().y);
  //}
  //else
  //{
    //TODO: explode WMF here...
  //}

  OdSmartPtr<GDI_BMP> pBMP = GDI_BMP::createObject(OdRoundToLong(fabs(imgMax.x - imgMin.x)), OdRoundToLong(fabs(imgMax.y - imgMin.y)));
  if(pBMP.get())
  {
    OdUInt32 nSize = pMetafile->dataSize();
    const OdUInt8* pData = pMetafile->bitsData();
    OdUInt8Array data;
    if(!pData)
    {
      data.resize(nSize);
      pMetafile->bitsData(data.asArrayPtr());
      pData = data.getPtr();
    }

    HENHMETAFILE hEmf = ::SetEnhMetaFileBits(nSize, pData);
    if (!hEmf) // It was apparently not an enhanced metafile, so try 16bit windows metafile
    {
      METAFILEPICT mfp = { MM_ANISOTROPIC, Od_abs(rect.right - rect.left), Od_abs(rect.top - rect.bottom), 0 };
      mfp.xExt *= 100;
      mfp.yExt *= 100;
      hEmf = ::SetWinMetaFileBits(nSize, pData, NULL, &mfp);
    }

    if(hEmf)
    {
      ::SetWindowOrgEx(pBMP->m_hDC, OdRoundToLong(imgMin.x - vpMin.x), OdRoundToLong(imgMax.y - vpMax.y), 0);
      ::FillRect(pBMP->m_hDC, &rect, ::GetSysColorBrush(COLOR_WINDOW));
      ::PlayEnhMetaFile(pBMP->m_hDC, hEmf, (LPRECT)&rect);
      ::DeleteEnhMetaFile(hEmf);
      rasterImageProc(origin2, u2 / pBMP->pixelWidth(), v2 / pBMP->pixelHeight(), pBMP, 0, 0);
    }
  }
}

void ExGsOpenGLVectorizeView::ownerDrawDc(const OdGePoint3d& _origin,
                                          const OdGeVector3d& _u,
                                          const OdGeVector3d& _v,
                                          const OdGiSelfGdiDrawable* pDrawable,
                                          bool bDcAligned,
                                          bool bAllowClipping)
{
  if (m_bSelecting)
    return;

  if (baseDevice()->supportDynamicHighlight())
  {
    onTraitsModified();
    //::exOwnerDrawDc(_origin, _u, _v, pDrawable, (getRenderMode() == k2DOptimized) && bDcAligned, bAllowClipping, *this,
    //                GETBIT(m_flags, kModelCache), GETBIT(m_flags, kModelCache));
    ExGsOpenGLVectorizeViewBase::ownerDrawDc(_origin, _u, _v, pDrawable, bDcAligned, bAllowClipping);
  }
  else
  {
    OdSaveState<OdUInt32> ssHLT(m_flags);
    highlight(false);

    onTraitsModified();
    //::exOwnerDrawDc(_origin, _u, _v, pDrawable, (getRenderMode() == k2DOptimized) && bDcAligned, bAllowClipping, *this,
    //                GETBIT(m_flags, kModelCache), GETBIT(m_flags, kModelCache));
    ExGsOpenGLVectorizeViewBase::ownerDrawDc(_origin, _u, _v, pDrawable, bDcAligned, bAllowClipping);
  }
}

void ExGsOpenGLVectorizeView::select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode)
{
  EX_OGL_PREV_CTX_WRAP_BEGIN();
  if (device()->getWGLContext() != NULL)
    ODA_VERIFY(::wglMakeCurrent((HDC)device()->getHDC(), (HGLRC)device()->getWGLContext()));
  OdSaveState<bool> ss(m_bSelecting, true);
  ExGsOpenGLVectorizeViewBase::select(pts, nPoints, pReactor, mode);
  EX_OGL_PREV_CTX_WRAP_END((HGLRC)device()->getWGLContext());
}

void ExGsOpenGLVectorizeDevice::createPalette(ODGSPALETTE *logPalette)
{
  OdUInt32 i, j;
  if (!m_bColorIndexMode)
  {
    return;
  }
  if (m_hPrevDC)
  {
    ExGsWin32Device::createPalette(logPalette);
    return;
  }
  PIXELFORMATDESCRIPTOR pfd;
  int n = GetPixelFormat(m_hDC);
  DescribePixelFormat(m_hDC, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  int nLogPaletteSz = 256; // openGL supports only 256 color 
    // (and probably monochrome) palette
  int iPalSize = sizeof(LOGPALETTE) + nLogPaletteSz * sizeof(PALETTEENTRY);
  
  OdUInt8Array palBuf;
  palBuf.resize(iPalSize);
  LOGPALETTE* pPal = (LOGPALETTE*)palBuf.asArrayPtr();
  if(!pPal)
  {
    ODA_TRACE0("Out of memory for logpal");
    return;
  }
  pPal->palVersion = 0x300; // Windows 3.0
  pPal->palNumEntries = (WORD)nLogPaletteSz; // table size

  int redMask = (1 << pfd.cRedBits) - 1;
  int greenMask = (1 << pfd.cGreenBits) - 1;
  int blueMask = (1 << pfd.cBlueBits) - 1;

  for(i = 0; i < (OdUInt32)nLogPaletteSz; i ++)
  {
    int indexRed   = (((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
    int indexGreen = (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
    int indexBlue  = (((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;

    OdUInt32 dif(256*256*100), colToMap(0);
    for(j = 0; j < logPalette->size(); j ++)
    {
      OdUInt8 logPalRed   = ODGETRED(logPalette->getAt(j));
      OdUInt8 logPalGreen = ODGETGREEN(logPalette->getAt(j));
      OdUInt8 logPalBlue  = ODGETBLUE(logPalette->getAt(j));

      OdInt32 difRed   = logPalRed   - indexRed;
      OdInt32 difGreen = logPalGreen - indexGreen;
      OdInt32 difBlue  = logPalBlue  - indexBlue;

      OdUInt32 curDif = 30*difRed*difRed + 59*difGreen*difGreen + 11*difBlue*difBlue;

      if(curDif < dif)
      {
        colToMap = j;
        dif = curDif;
      }
    }
    
    pPal->palPalEntry[i].peRed    = ODGETRED(logPalette->getAt(colToMap));
    pPal->palPalEntry[i].peGreen  = ODGETGREEN(logPalette->getAt(colToMap));
    pPal->palPalEntry[i].peBlue   = ODGETBLUE(logPalette->getAt(colToMap));
    pPal->palPalEntry[i].peFlags  = ODGETALPHA(logPalette->getAt(colToMap));
  }

  if (m_pPal)
  {
    DeleteObject(m_pPal);
  }
  m_pPal = ::CreatePalette(pPal);
}

#include "../WinComposite/ExWinCompositeBitBltOp.h"

inline OdUInt32 RGBScanLineLen(OdUInt32 nPxWdth)
{
  nPxWdth *= 24; // nbits
  // returns bmp-compatible scanline size
  return ((nPxWdth + 31) & ~31) >> 3;
}

void ExGsOpenGLVectorizeDevice::makeBitBltOp(const ExWinCompositeBitBltOp &bitBltOp)
{
  EX_OGL_PREV_CTX_WRAP_BEGIN();
  ODA_VERIFY(::wglMakeCurrent(m_hDC, m_hGLRC));
#ifdef OD_OGL_USE_PBO
  if (m_pPBOs[0])
  {
    OdUInt32 nPBOs = ((bitBltOp.m_cx < 768) || (bitBltOp.m_cy < 768)) ? 1 : 4;
    RECT subRects[4];
    int xDif = bitBltOp.m_x - bitBltOp.m_x1;
    int yDif = bitBltOp.m_y - bitBltOp.m_y1;
    if (nPBOs == 1)
    {
      ::SetRect(subRects, bitBltOp.m_x1, bitBltOp.m_y1, bitBltOp.m_x1 + bitBltOp.m_cx, bitBltOp.m_y1 + bitBltOp.m_cy);
    }
    else
    {
      const SIZE divs = { bitBltOp.m_cx >> 1, bitBltOp.m_cy >> 1 };
      // Use following configuration:
      // -----
      // |0|2|
      // -----
      // |1|3|
      // -----
      ::SetRect(subRects + 0, bitBltOp.m_x1, bitBltOp.m_y1, bitBltOp.m_x1 + divs.cx, bitBltOp.m_y1 + divs.cy);
      ::SetRect(subRects + 1, bitBltOp.m_x1, bitBltOp.m_y1 + divs.cy, bitBltOp.m_x1 + divs.cx, bitBltOp.m_y1 + bitBltOp.m_cy);
      ::SetRect(subRects + 2, bitBltOp.m_x1 + divs.cx, bitBltOp.m_y1, bitBltOp.m_x1 + bitBltOp.m_cx, bitBltOp.m_y1 + divs.cy);
      ::SetRect(subRects + 3, bitBltOp.m_x1 + divs.cx, bitBltOp.m_y1 + divs.cy, bitBltOp.m_x1 + bitBltOp.m_cx, bitBltOp.m_y1 + bitBltOp.m_cy);
    }
    // Init buffers and run async read
    OdOpenGLFrontReadBuffer _frontBuf;
    ::glPixelStorei(GL_PACK_ALIGNMENT, 4);
    OdUInt32 nPass = 0;
    for ( ; nPass < nPBOs; nPass++)
    {
      ((OdGLFn_BindBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_BindBuffer))(
        GL_PIXEL_PACK_BUFFER_ARB, m_pPBOs[nPass]);
      GLint checkSize = 0, newSize = RGBScanLineLen(subRects[nPass].right - subRects[nPass].left) * (subRects[nPass].bottom - subRects[nPass].top);
      ((OdGLFn_GetBufferParameteriv_ARB)getExtensionFunc(OpenGLExtFunc_ARB_GetBufferParameteriv))(
        GL_PIXEL_PACK_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &checkSize);
      if (checkSize != newSize)
        ((OdGLFn_BufferData_ARB)getExtensionFunc(OpenGLExtFunc_ARB_BufferData))(
          GL_PIXEL_PACK_BUFFER_ARB, newSize, NULL, GL_STREAM_READ_ARB);
      // run async read
      ::glReadPixels(subRects[nPass].left, height() - subRects[nPass].bottom, subRects[nPass].right - subRects[nPass].left,
        subRects[nPass].bottom - subRects[nPass].top, GL_BGR_EXT, GL_UNSIGNED_BYTE, 0);
    }
    // Draw separate buffers via SetDIBits
    BITMAPINFO bi;
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 24;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;
    bi.bmiHeader.biXPelsPerMeter = 0;
    bi.bmiHeader.biYPelsPerMeter = 0;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;
    for (nPass = 0; nPass < nPBOs; nPass++)
    {
      ((OdGLFn_BindBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_BindBuffer))(
        GL_PIXEL_PACK_BUFFER_ARB, m_pPBOs[nPass]);
      GLvoid *pData = ((OdGLFn_MapBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_MapBuffer))(
        GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
      bi.bmiHeader.biWidth = subRects[nPass].right - subRects[nPass].left;
      bi.bmiHeader.biHeight = subRects[nPass].bottom - subRects[nPass].top;
      ::SetDIBitsToDevice(bitBltOp.m_hdc, subRects[nPass].left + xDif, subRects[nPass].top + yDif,
        subRects[nPass].right - subRects[nPass].left, subRects[nPass].bottom - subRects[nPass].top,
        0, 0, 0, subRects[nPass].bottom - subRects[nPass].top, pData, &bi, DIB_RGB_COLORS);
      ((OdGLFn_UnmapBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_UnmapBuffer))(
        GL_PIXEL_PACK_BUFFER_ARB);
    }
    // unbind all
    ((OdGLFn_BindBuffer_ARB)getExtensionFunc(OpenGLExtFunc_ARB_BindBuffer))(
      GL_PIXEL_PACK_BUFFER_ARB, 0);
  }
  else
#endif
  {
    if (m_hPrevDC == NULL)
      m_dibSection.createDib(bitBltOp.m_cx, bitBltOp.m_cy, 24);
//    ::glPixelStorei(GL_PACK_ALIGNMENT, 4);
//    ::glReadPixels(bitBltOp.m_x1, height() - (bitBltOp.m_y1 + bitBltOp.m_cy), bitBltOp.m_cx, bitBltOp.m_cy,
//                   GL_BGR_EXT, GL_UNSIGNED_BYTE, m_dibSection.m_pBits);
//    EX_OGL_PREV_CTX_WRAP_END(m_hGLRC);
//    ::BitBlt(bitBltOp.m_hdc, bitBltOp.m_x, bitBltOp.m_y, bitBltOp.m_cx, bitBltOp.m_cy, m_dibSection.getHDC(), 0, 0, SRCCOPY);
#ifdef OD_OGL_USE_PBUFFER
    if (m_pPBuffer)
    {
      ::glPixelStorei(GL_PACK_ALIGNMENT, 4);
      ::glReadPixels(0, 0, width(), height(),
                     GL_BGR_EXT, GL_UNSIGNED_BYTE, m_dibSection.m_pBits);
    }
#endif
    ::BitBlt(bitBltOp.m_hdc, bitBltOp.m_x, bitBltOp.m_y, bitBltOp.m_cx, bitBltOp.m_cy, m_dibSection.getHDC(), bitBltOp.m_x1, bitBltOp.m_y1, SRCCOPY);
  }
  EX_OGL_PREV_CTX_WRAP_END(m_hGLRC);
}

OdGiRasterImagePtr ExGsOpenGLVectorizeDevice::snapshotRegion(const OdGsDCRect &area, bool bCrop) const
{
  if (!m_hDC || !m_hGLRC)
    return OdGiRasterImagePtr();
  EX_OGL_PREV_CTX_WRAP_BEGIN();
  ODA_VERIFY(::wglMakeCurrent(m_hDC, m_hGLRC));
  OdGiRasterImagePtr pImage = OdGsOpenGLVectorizeDevice::snapshotRegion(area, bCrop);
  EX_OGL_PREV_CTX_WRAP_END(m_hGLRC);
  return pImage;
}
