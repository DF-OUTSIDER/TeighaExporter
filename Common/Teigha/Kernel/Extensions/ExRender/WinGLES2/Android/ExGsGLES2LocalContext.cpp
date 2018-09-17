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
// GLES2 device local context (Android)

#include "OdaCommon.h"
#include "TrGL2LocalContext.h"
#include "GLES2Include.h"

#ifdef OD_TRGL2_NOEGL

// Without EGL we can't do anything
typedef OdTrGL2LocalContextStub OdGLES2LocalContextImpl;

#else

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext
{
  protected:
    EGLNativeDisplayType m_nativeDisplay;
    EGLNativeWindowType m_nativeWindow;
    EGLDisplay m_eglDisplay;
    EGLSurface m_eglSurface;
    EGLContext m_eglContext;
    bool m_bContextCreated;
    bool m_bSurfaceCreated;
    bool m_bDisplayCreated;
    bool m_bContextExternal;
    long m_deviceDimensions[2];
  public:
    OdGLES2LocalContextImpl()
      : OdTrGL2LocalContext()
      , m_nativeDisplay(NULL)
      , m_nativeWindow(NULL)
      , m_eglDisplay(EGL_NO_DISPLAY)
      , m_eglSurface(EGL_NO_SURFACE)
      , m_eglContext(EGL_NO_CONTEXT)
      , m_bContextCreated(false)
      , m_bSurfaceCreated(false)
      , m_bDisplayCreated(false)
      , m_bContextExternal(false)
    {
      m_deviceDimensions[0] = m_deviceDimensions[1] = 0;
    }

    ~OdGLES2LocalContextImpl()
    {
      destroyContext();
    }

    void createContext(OdTrVisRenderClient *pDevice)
    {
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
        EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;
        if (pDevice->hasProperty(OD_T("WindowDisplay")))
          nativeDisplay = (EGLNativeDisplayType)pDevice->getProperty(OD_T("WindowDisplay"))->getIntPtr();
        // Get EGL display handle
        EGLDisplay eglDisplay;
        if (pDevice->hasProperty(OD_T("EGLDisplay")) &&
            (pDevice->getProperty(OD_T("EGLDisplay"))->getIntPtr() != 0))
          m_eglDisplay = eglDisplay = (EGLDisplay)pDevice->getProperty(OD_T("EGLDisplay"))->getIntPtr();
        else
        {
          eglDisplay = ::eglGetDisplay(nativeDisplay);
          if (eglDisplay == EGL_NO_DISPLAY)
            pDevice->emitError("Could not get EGL display.");
          m_eglDisplay = eglDisplay;
          m_bDisplayCreated = true;
          // Initialize display
          if (!::eglInitialize(eglDisplay, NULL, NULL))
            pDevice->emitError("Could not initialize EGL display.");
        }
        EGLConfig eglConfig = 0;
        if (pDevice->hasProperty(OD_T("EGLSurface")) &&
            (pDevice->getProperty(OD_T("EGLSurface"))->getIntPtr() != 0))
        {
          m_eglSurface = (EGLSurface)pDevice->getProperty(OD_T("EGLSurface"))->getIntPtr();
          if (!bDirectBuffer)
            m_nativeWindow = (EGLNativeWindowType)pDevice->getProperty(OD_T("ANativeWindow"))->getIntPtr();
          else
            m_nativeWindow = NULL;
          EGLint configId[] = { EGL_CONFIG_ID, 0, EGL_NONE, EGL_NONE };
          ::eglQuerySurface(eglDisplay, m_eglSurface, EGL_CONFIG_ID, &configId[1]);
          ::eglChooseConfig(eglDisplay, configId, &eglConfig, 1, configId + 3);
        }
        else
        {
          // Obtain required configuration
          EGLint pAttrs[] = {
              EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
              EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8,
              EGL_DEPTH_SIZE, 24, EGL_STENCIL_SIZE, 8,
              EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
              EGL_NONE
          };
          if (bDirectBuffer)
            pAttrs[13] = EGL_PBUFFER_BIT;
          EGLint nConfig = 0;
          if (!::eglChooseConfig(eglDisplay, pAttrs, &eglConfig, 1, &nConfig) || (nConfig <= 0))
          {
            pAttrs[3] = 5; pAttrs[5] = 6; pAttrs[7] = 5;
            if (!::eglChooseConfig(eglDisplay, pAttrs, &eglConfig, 1, &nConfig) || (nConfig <= 0))
              pDevice->emitError("Could not find valid EGL config.");
          }
          // Get native visual Id
          EGLint nativeVid = 0;
          if (!::eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &nativeVid))
            pDevice->emitError("Could not get native visual id.");
          if (!bDirectBuffer)
          {
            // Get native window
            EGLNativeWindowType nativeWin = 0;
            if (pDevice->hasProperty(OD_T("ANativeWindow")))
              nativeWin = (EGLNativeWindowType)pDevice->getProperty(OD_T("ANativeWindow"))->getIntPtr();
            if (!nativeWin)
              pDevice->emitError("Could not get native window.");
#ifndef __ANDROID_API__
            // Do android specific
            ANativeWindow_setBuffersGeometry(nativeWin, 0, 0, nativeVid);
#endif
            // Create a surface for the main window
            EGLSurface eglSurface;
            eglSurface = ::eglCreateWindowSurface(eglDisplay, eglConfig, nativeWin, NULL);
            if (eglSurface == EGL_NO_SURFACE)
              pDevice->emitError("Could not create EGL surface.");
            m_eglSurface = eglSurface;
            m_nativeWindow = nativeWin;
          }
          else
          {
            OdUInt32 pixelWidth, pixelHeight;
            pDevice->getDirectRenderBuffer(&pixelWidth, &pixelHeight);
            const EGLint srfPbufferAttr[] = {
              EGL_WIDTH, (EGLint)pixelWidth,
              EGL_HEIGHT, (EGLint)pixelHeight,
              EGL_LARGEST_PBUFFER, EGL_TRUE,
              EGL_NONE
            };
            // Create a surface for the raster image
            EGLSurface eglSurface;
            eglSurface = ::eglCreatePbufferSurface(eglDisplay, eglConfig, srfPbufferAttr);
            if (eglSurface == EGL_NO_SURFACE)
              pDevice->emitError("Could not create EGL surface.");
            m_eglSurface = eglSurface;
            m_nativeWindow = NULL;
          }
          m_bSurfaceCreated = true;
        }
        if (pDevice->hasProperty(OD_T("EGLContext")) &&
            (pDevice->getProperty(OD_T("EGLContext"))->getIntPtr() != 0))
        {
          m_eglContext = (EGLSurface)pDevice->getProperty(OD_T("EGLContext"))->getIntPtr();
          m_bContextExternal = true;
        }
        else
        {
          const EGLint pCAttrs[] = {
              EGL_CONTEXT_CLIENT_VERSION, 2,
              EGL_NONE
          };
          // Create an OpenGL ES context
          EGLContext eglContext;
          eglContext = ::eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, pCAttrs);
          if (eglContext == EGL_NO_CONTEXT)
            pDevice->emitError("Could not create EGL context.");
          m_eglContext = eglContext;
        }
        m_nativeDisplay = nativeDisplay;
        // Put properties back
        if (pDevice->hasProperty(OD_T("WindowDisplay")))
          pDevice->setProperty(OD_T("WindowDisplay"), OdRxVariantValue((OdIntPtr)m_nativeDisplay));
        if (m_bDisplayCreated && pDevice->hasProperty(OD_T("EGLDisplay")))
          pDevice->setProperty(OD_T("EGLDisplay"), OdRxVariantValue((OdIntPtr)m_eglDisplay));
        if (m_bSurfaceCreated && pDevice->hasProperty(OD_T("EGLSurface")))
          pDevice->setProperty(OD_T("EGLSurface"), OdRxVariantValue((OdIntPtr)m_eglSurface));
        if (!m_bContextExternal && pDevice->hasProperty(OD_T("EGLContext")))
          pDevice->setProperty(OD_T("EGLContext"), OdRxVariantValue((OdIntPtr)m_eglContext));
        m_deviceDimensions[0] = pDevice->outputWindowWidth();
        m_deviceDimensions[1] = pDevice->outputWindowHeight();
      }
      m_bContextCreated = true;
    }
    void updateContext(OdTrVisRenderClient *pDevice)
    {
      if ((m_deviceDimensions[0] != pDevice->outputWindowWidth()) && (m_deviceDimensions[1] != pDevice->outputWindowHeight()))
      {
        // In case if "CreateContext" disabled m_deviceDimensions[] is permanently null
        if (pDevice->hasProperty(OD_T("CreateContext")) &&
            !pDevice->getProperty(OD_T("CreateContext"))->getBool())
          return;
        // We need completely recreate OpenGL context in Android case
        if (m_bSurfaceCreated && pDevice->hasProperty(OD_T("EGLSurface")))
          pDevice->setProperty(OD_T("EGLSurface"), OdRxVariantValue((OdIntPtr)NULL));
        if (!m_bContextExternal && pDevice->hasProperty(OD_T("EGLContext")))
          pDevice->setProperty(OD_T("EGLContext"), OdRxVariantValue((OdIntPtr)NULL));
        if (pDevice->hasProperty(OD_T("SaveContextData"))) // Save OpenGL context data
          pDevice->setProperty(OD_T("SaveContextData"), OdRxVariantValue(true));
        destroyContext();
        createContext(pDevice);
        if (pDevice->hasProperty(OD_T("SaveContextData"))) // Load OpenGL context data
          pDevice->setProperty(OD_T("SaveContextData"), OdRxVariantValue(false));
      }
    }
    void destroyContext()
    {
      if (m_eglContext != EGL_NO_CONTEXT)
      {
        ::eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (!m_bContextExternal)
          ::eglDestroyContext(m_eglDisplay, m_eglContext);
      }
      if (m_bSurfaceCreated)
        ::eglDestroySurface(m_eglDisplay, m_eglSurface);
      if (m_bDisplayCreated)
        ::eglTerminate(m_eglDisplay);
      m_nativeDisplay = NULL;
      m_nativeWindow = NULL;
      m_eglDisplay = EGL_NO_DISPLAY;
      m_eglSurface = EGL_NO_SURFACE;
      m_eglContext = EGL_NO_CONTEXT;
      m_bDisplayCreated = false;
      m_bSurfaceCreated = false;
      m_bContextExternal = false;
      m_bContextCreated = false;
    }

    bool isContextCreated() const
    {
      return m_bContextCreated;
    }

    void makeCurrentContext()
    {
      if (m_eglContext != EGL_NO_CONTEXT)
      {
        ODA_VERIFY(::eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext));
      }
    }

    void presentContext()
    {
      if (m_eglContext != EGL_NO_CONTEXT)
      {
        ::eglSwapBuffers(m_eglDisplay, m_eglSurface);
      }
    }

    bool isExtensionSupported(const char *pExtensionName)
    {
      return OdTrVisInfoString::checkExtensionExternal((const char*)::glGetString(GL_EXTENSIONS), pExtensionName);
    }
    void *acquireExtensionFunctionPtr(const char *pFunctionName)
    {
      return (void*)::eglGetProcAddress(pFunctionName);
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
