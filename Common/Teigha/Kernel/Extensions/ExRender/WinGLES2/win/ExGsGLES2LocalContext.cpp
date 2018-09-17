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
// GLES2 device local context (Windows)

#include "OdaCommon.h"
#include "TrGL2LocalContext.h"
#include "GLES2Include.h"
#include "OpenGL/OpenGLDrawBuffer.h"

#if TRGL2EMUL_ENABLED

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext
{
  protected:
#ifndef OD_TRGL2_NOEGL
    EGLNativeDisplayType m_nativeDisplay;
    EGLNativeWindowType m_nativeWindow;
    EGLDisplay m_eglDisplay;
    EGLSurface m_eglSurface;
    EGLContext m_eglContext;
#endif
    bool m_bContextCreated;
  public:
    OdGLES2LocalContextImpl()
      : OdTrGL2LocalContext()
      , m_nativeDisplay(NULL)
      , m_nativeWindow(NULL)
      , m_eglDisplay(EGL_NO_DISPLAY)
      , m_eglSurface(EGL_NO_SURFACE)
      , m_eglContext(EGL_NO_CONTEXT)
      , m_bContextCreated(false)
    {
    }

    ~OdGLES2LocalContextImpl()
    {
      destroyContext();
    }

    void createContext(OdTrVisRenderClient *pDevice)
    {
#ifndef OD_TRGL2_NOEGL
      if (m_eglContext == EGL_NO_CONTEXT)
      {
        if (pDevice->hasProperty(OD_T("CreateContext")) &&
            !pDevice->getProperty(OD_T("CreateContext"))->getBool())
        {
          m_bContextCreated = true;
          return;
        }
        // For raster
        const bool bDirectBuffer = pDevice->hasDirectRenderBuffer();
        // Aquire native window
        EGLNativeDisplayType nativeDisplay = NULL;
        if (pDevice->hasProperty(OD_T("WindowDisplay"))) // Have meaning on X11
          nativeDisplay = (EGLNativeDisplayType)pDevice->getProperty(OD_T("WindowDisplay"))->getIntPtr();
        // Get EGL display handle
        EGLDisplay eglDisplay;
        eglDisplay = ::eglGetDisplay(nativeDisplay);
        if (eglDisplay == EGL_NO_DISPLAY)
          pDevice->emitError("Could not get EGL display.");
        m_eglDisplay = eglDisplay;
        // Initialize display
        EGLint nMajor = 0, nMinor = 0;
        if (!::eglInitialize(eglDisplay, &nMajor, &nMinor))
          pDevice->emitError("Could not initialize EGL display.");
        if (nMajor < 1 || nMinor < 4) // Does not support EGL 1.4
          pDevice->emitError("System doesn't support at least EGL 1.4.");
        // Obtain required configuration
        EGLint pAttrs[] = { EGL_DEPTH_SIZE, 24, // Depth buffer
                            EGL_STENCIL_SIZE, 8, // Stencil buffer required
                            EGL_NONE, EGL_NONE, EGL_NONE };
        if (bDirectBuffer)
        {
          pAttrs[4] = EGL_SURFACE_TYPE; pAttrs[5] = EGL_PBUFFER_BIT;
        }
        EGLint nConfig = 0;
        EGLConfig eglConfig = 0;
        if (!::eglChooseConfig(eglDisplay, pAttrs, &eglConfig, 1, &nConfig))
          pDevice->emitError("Could not find valid EGL config.");
        // Get native visual Id
        int nativeVid; // @@@TODO: for X11 must be obtained from properties, or must be set?
        if (!::eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &nativeVid))
          pDevice->emitError("Could not get native visual id.");
        if (!bDirectBuffer)
        {
          // Get native window
          EGLNativeWindowType nativeWin;
          if (pDevice->hasProperty(OD_T("WindowHWND"))) // Have meaning on Win
            nativeWin = (EGLNativeWindowType)pDevice->getProperty(OD_T("WindowHWND"))->getIntPtr();
          else if (pDevice->hasProperty(OD_T("WindowHDC"))) // Have meaning on Win
          {
            HDC hDc = (HDC)pDevice->getProperty(OD_T("WindowHDC"))->getIntPtr();
            nativeWin = ::WindowFromDC(hDc);
          }
          if (!nativeWin)
            pDevice->emitError("Could not get native window.");
          // Create a surface for the main window
          EGLSurface eglSurface;
          eglSurface = ::eglCreateWindowSurface(eglDisplay, eglConfig, nativeWin, NULL);
          if (eglSurface == EGL_NO_SURFACE)
            pDevice->emitError("Could not create EGL surface.");
          m_eglSurface = eglSurface;
          m_nativeWindow = nativeWin;
        }
        else
        { // @@@TODO: updateContext could be useful to resize image if it is changed
          OdUInt32 pixelWidth, pixelHeight;
          pDevice->getDirectRenderBuffer(&pixelWidth, &pixelHeight);
          EGLint pAttrsRI[] = { EGL_WIDTH, (EGLint)pixelWidth, // PBO width
                                EGL_HEIGHT, (EGLint)pixelHeight, // PBO height
                                EGL_LARGEST_PBUFFER, EGL_TRUE,
                                EGL_NONE };
          // Create a surface for the raster image
          EGLSurface eglSurface;
          eglSurface = ::eglCreatePbufferSurface(eglDisplay, eglConfig, pAttrsRI);
          if (eglSurface == EGL_NO_SURFACE)
            pDevice->emitError("Could not create EGL surface.");
          m_eglSurface = eglSurface;
          m_nativeWindow = NULL;
        }
        // Create an OpenGL ES context
        EGLContext eglContext;
        eglContext = ::eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, NULL);
        if (eglContext == EGL_NO_CONTEXT)
          pDevice->emitError("Could not create EGL context.");
        m_eglContext = eglContext;
        m_nativeDisplay = nativeDisplay;
      }
#endif
      m_bContextCreated = true;
    }
    void destroyContext()
    {
#ifndef OD_TRGL2_NOEGL
      if (m_eglContext != EGL_NO_CONTEXT)
      {
        ::eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        ::eglDestroyContext(m_eglDisplay, m_eglContext);
        ::eglDestroySurface(m_eglDisplay, m_eglSurface);
        ::eglTerminate(m_eglDisplay);
        m_nativeDisplay = NULL;
        m_nativeWindow = NULL;
        m_eglDisplay = EGL_NO_DISPLAY;
        m_eglSurface = EGL_NO_SURFACE;
        m_eglContext = EGL_NO_CONTEXT;
      }
#endif
      m_bContextCreated = false;
    }

    bool isContextCreated() const
    {
      return m_bContextCreated;
    }

    void makeCurrentContext()
    {
#ifndef OD_TRGL2_NOEGL
      if (m_eglContext != EGL_NO_CONTEXT)
      {
        ODA_VERIFY(::eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext));
      }
#endif
    }

    void presentContext()
    {
#ifndef OD_TRGL2_NOEGL
      if (m_eglContext != EGL_NO_CONTEXT)
      {
        ::eglSwapBuffers(m_eglDisplay, m_eglSurface);
      }
#endif
    }
};

#else

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext
{
  protected:
    struct GLContext
    {
      HGLRC m_hContext;
      HDC   m_hDc;
      void release() { m_hContext = NULL; m_hDc = NULL; }
      GLContext() { release(); }
      GLContext(HGLRC hContext, HDC hDc) : m_hContext(hContext), m_hDc(hDc) { }
    };
    struct GLContextEntry : public GLContext
    {
      GLContextEntry *m_pNext;
      GLContextEntry(GLContextEntry *pNext = NULL) : GLContext(), m_pNext(pNext) { }
      GLContextEntry(HGLRC hContext, HDC hDc, GLContextEntry *pNext = NULL) : GLContext(hContext, hDc), m_pNext(pNext) { }
    };
  protected:
    GLContext m_context;
    GLContextEntry *m_pEntry;
    HWND m_hWnd;
    bool m_bMustReleaseDc;
    bool m_bMustReleaseContext;
    bool m_bDoubleBuffer;
    bool m_bContextCreated;
  public:
    OdGLES2LocalContextImpl()
      : OdTrGL2LocalContext()
      , m_context()
      , m_pEntry(NULL)
      , m_hWnd(NULL)
      , m_bMustReleaseDc(false)
      , m_bMustReleaseContext(false)
      , m_bDoubleBuffer(false)
      , m_bContextCreated(false)
    {
    }

    ~OdGLES2LocalContextImpl()
    {
      destroyContext();
      while (popCurrentContext()) ;
    }

    void createContext(OdTrVisRenderClient *pDevice)
    {
      if (!m_context.m_hContext)
      {
        if (pDevice->hasProperty(OD_T("CreateContext")) &&
            !pDevice->getProperty(OD_T("CreateContext"))->getBool())
        {
          GLES2_Extensions_Initialize(pDevice);
          m_bContextCreated = true;
          return;
        }
        // For raster
        const bool bDirectBuffer = pDevice->hasDirectRenderBuffer();
        // Double buffer
        m_bDoubleBuffer = false;
        if (!bDirectBuffer && pDevice->hasProperty(OD_T("DoubleBufferEnabled")))
          m_bDoubleBuffer = pDevice->getProperty(OD_T("DoubleBufferEnabled"))->getBool();
        // Initialize context
        if (!pDevice->hasProperty(OD_T("WGLContext")) ||
            !pDevice->getProperty(OD_T("WGLContext"))->getIntPtr())
        {
          PIXELFORMATDESCRIPTOR pfd;
          ::memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
          pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
          pfd.nVersion = 1;                  // Since PFD_DRAW_TO_BITMAP is software and doesn't support extensions
          pfd.dwFlags = PFD_DRAW_TO_WINDOW | //((pRaster.isNull()) ? PFD_DRAW_TO_WINDOW : PFD_DRAW_TO_BITMAP) |
                        ((m_bDoubleBuffer) ? PFD_DOUBLEBUFFER : 0) |
                        PFD_SUPPORT_OPENGL;
          pfd.iPixelType = PFD_TYPE_RGBA;
          pfd.cColorBits = 24;
          pfd.cDepthBits = 24;
          pfd.iLayerType = PFD_MAIN_PLANE;
          pfd.cStencilBits = 8;
          // Choose the pixel format
          if (pDevice->hasProperty(OD_T("WindowHDC")))
            m_context.m_hDc = (HDC)pDevice->getProperty(OD_T("WindowHDC"))->getIntPtr();
          if (!m_context.m_hDc && pDevice->hasProperty(OD_T("WindowHWND")))
          {
            HWND hWnd = (HWND)pDevice->getProperty(OD_T("WindowHWND"))->getIntPtr();
            if (hWnd)
            {
              m_context.m_hDc = ::GetDC(hWnd);
              m_bMustReleaseDc = true;
            }
          }
          if (!m_context.m_hDc)
          {
            // Bitmap device window
            #define OUR_WND_NAME OD_T("Teigha GLES2Render Window")
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
            m_hWnd = ::CreateWindowEx(0, OUR_WND_NAME, OUR_WND_NAME, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                                      0, 0, pDevice->outputWindowWidth(), pDevice->outputWindowHeight(), ::GetDesktopWindow(), 0, hInstance, 0);
            m_context.m_hDc = ::GetDC(m_hWnd);
            m_bMustReleaseDc = true;
          }
          int nPixelFormat = ::ChoosePixelFormat(m_context.m_hDc, &pfd);
#define CLEAN_UNCOMPLETED_INIT() \
          if (m_bMustReleaseDc) \
            ::ReleaseDC(::WindowFromDC(m_context.m_hDc), m_context.m_hDc); \
          m_context.release(); \
          m_bMustReleaseDc = false; \
          if (m_hWnd) \
            ::DestroyWindow(m_hWnd); \
          m_hWnd = NULL
//
          if (!nPixelFormat)
          {
            CLEAN_UNCOMPLETED_INIT();
            pDevice->emitError("Could not choose valid pixel format.");
          }
          // Set the pixel format.
          if (::SetPixelFormat(m_context.m_hDc, nPixelFormat, &pfd) == FALSE)
          {
            CLEAN_UNCOMPLETED_INIT();
            pDevice->emitError("Could not set valid pixel format.");
          }
          // Create context
          m_context.m_hContext = ::wglCreateContext(m_context.m_hDc);
          if (m_context.m_hContext)
          {
            m_bMustReleaseContext = true;
            pushCurrentContext();
            ODA_VERIFY(::wglMakeCurrent(m_context.m_hDc, m_context.m_hContext));
          }
          else
          {
            CLEAN_UNCOMPLETED_INIT();
            pDevice->emitError("Could not create GL context.");
          }
#undef CLEAN_UNCOMPLETED_INIT
        }
        else
        { // Get exist GL context
          m_context.m_hContext = (HGLRC)pDevice->getProperty(OD_T("WGLContext"))->getIntPtr();
          // Get DC for exist GL context
          m_context.m_hDc = (HDC)pDevice->getProperty(OD_T("WindowHDC"))->getIntPtr();
          if (!m_context.m_hDc)
          {
            HWND hWnd = (HWND)pDevice->getProperty(OD_T("WindowHWND"))->getIntPtr();
            if (hWnd)
            {
              m_context.m_hDc = ::GetDC(hWnd);
              m_bMustReleaseDc = true;
            }
          }
          if (!m_context.m_hDc && (m_context.m_hContext == ::wglGetCurrentContext()))
          {
            m_context.m_hDc = ::wglGetCurrentDC();
          }
          if (!m_context.m_hDc)
            pDevice->emitError("Could not get DC for exist GL context.");
        }
        if (m_bMustReleaseDc && pDevice->hasProperty(OD_T("WindowHDC")))
          pDevice->setProperty(OD_T("WindowHDC"), OdRxVariantValue((OdIntPtr)m_context.m_hDc));
        if (m_bMustReleaseContext && pDevice->hasProperty(OD_T("WGLContext")))
          pDevice->setProperty(OD_T("WGLContext"), OdRxVariantValue((OdIntPtr)m_context.m_hContext));
        // Run extensions initialization
        GLES2_Extensions_Initialize(pDevice);
        popCurrentContext();
      }
      m_bContextCreated = true;
    }
    void updateContext(OdTrVisRenderClient *pDevice)
    {
      if (m_context.m_hContext && m_hWnd)
      {
        // Resize window to be compatible with output bitmap dimensions
        RECT cliRect;
        ::GetClientRect(m_hWnd, &cliRect);
        if (((cliRect.right - cliRect.left) != pDevice->outputWindowWidth()) ||
            ((cliRect.bottom - cliRect.top) != pDevice->outputWindowHeight()))
          ::MoveWindow(m_hWnd, 0, 0, pDevice->outputWindowWidth(), pDevice->outputWindowHeight(), FALSE);
      }
    }
    void destroyContext()
    {
      if (m_context.m_hContext)
      {
        pushCurrentContext();
        if (m_context.m_hContext != ::wglGetCurrentContext())
          ::wglMakeCurrent(m_context.m_hDc, m_context.m_hContext);
        // . . . Do required uninits
        if (!popCurrentContext())
          ::wglMakeCurrent(m_context.m_hDc, NULL);
        // Delete the rendering context
        if (m_bMustReleaseContext)
          ::wglDeleteContext(m_context.m_hContext);
        if (m_bMustReleaseDc)
          ::ReleaseDC(::WindowFromDC(m_context.m_hDc), m_context.m_hDc);
        m_context.release();
        m_bMustReleaseDc = m_bMustReleaseContext = false;
        if (m_hWnd)
          ::DestroyWindow(m_hWnd);
        m_hWnd = NULL;
      }
      m_bContextCreated = false;
    }

    bool isContextCreated() const
    {
      return m_bContextCreated;
    }

    void makeCurrentContext()
    {
      if (m_context.m_hContext && (m_context.m_hContext != ::wglGetCurrentContext()))
       ::wglMakeCurrent(m_context.m_hDc, m_context.m_hContext);
    }

    bool pushCurrentContext()
    {
      if (m_context.m_hContext && (m_context.m_hContext != ::wglGetCurrentContext()))
      {
        m_pEntry = new GLContextEntry(::wglGetCurrentContext(), ::wglGetCurrentDC(), m_pEntry);
        return true;
      }
      return false;
    }
    
    bool popCurrentContext()
    {
      if (m_pEntry)
      {
        GLContextEntry *pEntry = m_pEntry; m_pEntry = pEntry->m_pNext;
        ::wglMakeCurrent(pEntry->m_hDc, pEntry->m_hContext);
        delete pEntry; return true;
      }
      return false;
    }

    void presentContext()
    {
      if (m_context.m_hContext)
      {
        ::SwapBuffers(m_context.m_hDc);
#if 1
        if (m_bDoubleBuffer)
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
          RECT cliRect;
          ::GetClientRect(::WindowFromDC(m_context.m_hDc), &cliRect);
          ::glCopyPixels(0, 0, cliRect.right - cliRect.left, cliRect.bottom - cliRect.top, GL_COLOR);
        }
#endif
      }
    }

    bool isExtensionBasedEmulation() const
    {
      return true;
    }
    bool isExtensionSupported(const char *pExtensionName)
    {
      return GLES2_ParseExtension(pExtensionName, NULL);
    }
    void *acquireExtensionFunctionPtr(const char *pFunctionName)
    {
      return GLES2_GetProcAddress(pFunctionName);
    }
};

#endif

OdSmartPtr<OdTrGL2LocalContext> OdTrGL2LocalContext::createLocalContext(OdTrVisRenderClient *pDevice)
{
  OdTrGL2LocalContextPtr pContext = OdRxObjectImpl<OdGLES2LocalContextImpl, OdTrGL2LocalContext>::createObject();
  pContext->createContext(pDevice);
  return pContext;
}

//
