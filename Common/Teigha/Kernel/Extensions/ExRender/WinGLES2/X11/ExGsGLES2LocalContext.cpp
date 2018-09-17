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
// GLES2 device local context (X11)

#include "OdaCommon.h"
#include "TrGL2LocalContext.h"
#include "GLES2Include.h"
#include "ExGsXWindowConnection.h"

#include <GL/glx.h>

#ifdef GLX_VERSION_1_3
#define PBUFFER_WAY
#endif
#ifndef GLX_VERSION_1_2
#error "GLX lower v1.2 doesn't supported, recommeded 1.3 or later"
#endif

OdIntPtr odGLES2X11ChooseVisual(OdIntPtr display, bool bDoubleBufferEnabled)
{
  int attributes[] =
  {
    GLX_USE_GL,
    GLX_RGBA,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_STENCIL_SIZE, 8,
    (bDoubleBufferEnabled) ? GLX_DOUBLEBUFFER : 0,
    0
  };
  ODA_ASSERT(display != 0);
  return (OdIntPtr)::glXChooseVisual((Display*)display, DefaultScreen((Display*)display), attributes);
}

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext
{
  protected:
    struct GLContext : public ExGsXWindowConnection
    {
      GLXContext m_glxContext;
#ifndef PBUFFER_WAY
      Pixmap m_xPixmap;
#endif
      void release()
      {
        display = NULL;
        window = 0;
        vinfo = NULL;
        m_glxContext = 0;
#ifndef PBUFFER_WAY
        m_xPixmap = 0;
#endif
      }
      GLContext() { release(); }
      GLContext(GLXContext glxContext, Display *_display, Window _window)
        : m_glxContext(glxContext)
      {
        display = _display;
        window = _window;
      }
    };
    struct GLContextEntry : public GLContext
    {
      GLContextEntry *m_pNext;
      GLContextEntry(GLContextEntry *pNext = NULL) : GLContext(), m_pNext(pNext) { }
      GLContextEntry(GLXContext glxContext, Display *_display, Window _window, GLContextEntry *pNext = NULL)
        : GLContext(glxContext, _display, _window), m_pNext(pNext) { }
    };
  protected:
    GLContext m_context;
    GLContextEntry *m_pEntry;
    bool m_bMustReleaseDisplay;
    bool m_bMustReleaseWindow;
    bool m_bMustReleaseVisual;
    bool m_bMustReleaseContext;
    bool m_bDoubleBuffer;
    bool m_bContextCreated;
  public:
    OdGLES2LocalContextImpl()
      : OdTrGL2LocalContext()
      , m_context()
      , m_pEntry(NULL)
      , m_bMustReleaseDisplay(false)
      , m_bMustReleaseWindow(false)
      , m_bMustReleaseVisual(false)
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
      if (!m_context.m_glxContext)
      {
        if (pDevice->hasProperty(OD_T("CreateContext")) &&
            !pDevice->getProperty(OD_T("CreateContext"))->getBool())
        {
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
        if (pDevice->hasProperty(OD_T("XDisplay")))
          m_context.display = (Display*)pDevice->getProperty(OD_T("XDisplay"))->getIntPtr();
        if (!m_context.display)
        {
          m_context.display = ::XOpenDisplay(/*getenv("DISPLAY")*/NULL);
          m_bMustReleaseDisplay = true;
        }
        if (!m_context.display)
          pDevice->emitError("Unable to connect to display.");
        if (bDirectBuffer)
        {
#ifdef PBUFFER_WAY
          int fbAttribs[] =
          {
            GLX_DOUBLEBUFFER,  (m_bDoubleBuffer) ? True : False,
            GLX_RED_SIZE,      8,
            GLX_GREEN_SIZE,    8,
            GLX_BLUE_SIZE,     8,
            GLX_ALPHA_SIZE,    8,
            GLX_DEPTH_SIZE,    24,
            GLX_STENCIL_SIZE,  8,
            GLX_RENDER_TYPE,   GLX_RGBA_BIT,
            GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
            None
          };
          int count = 0;
          GLXFBConfig *config = ::glXChooseFBConfig(m_context.display, DefaultScreen(m_context.display),
                                                    fbAttribs, &count);
          if ((config == NULL) || (count == 0))
          {
            if (m_bMustReleaseDisplay)
            {
              ::XCloseDisplay(m_context.display);
              m_bMustReleaseDisplay = false;
            }
            pDevice->emitError("Can't choose frame buffer configuration.");
          }
          int pbAttribs[] =
          {
            GLX_PBUFFER_WIDTH,   pDevice->outputWindowWidth(),
            GLX_PBUFFER_HEIGHT,  pDevice->outputWindowHeight(),
            GLX_LARGEST_PBUFFER, False,
            None
          };
          m_context.window = (Window)::glXCreatePbuffer(m_context.display, config[0], pbAttribs);
          m_bMustReleaseWindow = true;
          m_context.m_glxContext = ::glXCreateNewContext(m_context.display, config[0], GLX_RGBA_TYPE, NULL, True);
          ::XFree(config);
          if (m_context.m_glxContext)
          {
            pushCurrentContext();
            ODA_VERIFY(::glXMakeContextCurrent(m_context.display, m_context.window, m_context.window, m_context.m_glxContext));
          }
          else
          {
            ::glXDestroyPbuffer(m_context.display, m_context.window);
            m_bMustReleaseWindow = false;
            if (m_bMustReleaseDisplay)
            {
              ::XCloseDisplay(m_context.display);
              m_bMustReleaseDisplay = false;
            }
            pDevice->emitError("Could not create context.");
          }
#else
          m_context.vinfo = (XVisualInfo*)::odGLES2X11ChooseVisual((OdIntPtr)m_context.display, m_bDoubleBuffer = false);
          if (!m_context.vinfo)
            m_context.vinfo = (XVisualInfo*)::odGLES2X11ChooseVisual((OdIntPtr)m_context.display, m_bDoubleBuffer = true);
          if (!m_context.vinfo)
          {
            if (m_bMustReleaseDisplay)
            {
              ::XCloseDisplay(m_context.display);
              m_bMustReleaseDisplay = false;
            }
            pDevice->emitError("Can't choose visual.");
          }
          m_bMustReleaseVisual = true;
          m_context.m_xPixmap = ::XCreatePixmap(m_context.display, DefaultRootWindow(m_context.display),
                                                pDevice->outputWindowWidth(), pDevice->outputWindowHeight(), m_context.vinfo->depth);
          m_context.window = (Window)::glXCreateGLXPixmap(m_context.display, m_context.vinfo, m_context.m_xPixmap);
          m_bMustReleaseWindow = true;
          // Create context
          m_context.m_glxContext = ::glXCreateContext(m_context.display, m_context.vinfo, 0, true);
          if (m_context.m_glxContext)
          {
            pushCurrentContext();
            ODA_VERIFY(::glXMakeCurrent(m_context.display, m_context.window, m_context.m_glxContext));
          }
          else
          {
            m_bMustReleaseWindow = m_bMustReleaseVisual = false;
            ::XFree(m_context.vinfo);
            ::glXDestroyGLXPixmap(m_context.display, m_context.window);
            ::XFreePixmap(m_context.display, m_context.m_xPixmap);
            if (m_bMustReleaseDisplay)
            {
              ::XCloseDisplay(m_context.display);
              m_bMustReleaseDisplay = false;
            }
            pDevice->emitError("Could not create context.");
          }
#endif
          m_bMustReleaseContext = true;
        }
        else
        {
          if (pDevice->hasProperty(OD_T("XWindow")))
            m_context.window = (Window)pDevice->getProperty(OD_T("XWindow"))->getIntPtr();
          if (!pDevice->hasProperty(OD_T("GLXContext")) ||
              !pDevice->getProperty(OD_T("GLXContext"))->getIntPtr())
          { // Allocate new context
            if (pDevice->hasProperty(OD_T("XChooseVisual")))
              m_context.vinfo = (XVisualInfo*)pDevice->getProperty(OD_T("XChooseVisual"))->getIntPtr();
            else
              m_context.vinfo = (XVisualInfo*)::odGLES2X11ChooseVisual((OdIntPtr)m_context.display, m_bDoubleBuffer);
            if (!m_context.vinfo)
            {
              if (m_bMustReleaseDisplay)
              {
                ::XCloseDisplay(m_context.display);
                m_bMustReleaseDisplay = false;
              }
              pDevice->emitError("Can't choose visual.");
            }
            m_bMustReleaseVisual = true;
            // Create context
            m_context.m_glxContext = ::glXCreateContext(m_context.display, m_context.vinfo, 0, true);
            if (m_context.m_glxContext)
            {
              pushCurrentContext();
              ODA_VERIFY(::glXMakeCurrent(m_context.display, m_context.window, m_context.m_glxContext));
            }
            else
            {
              if (m_bMustReleaseDisplay)
              {
                ::XFree(m_context.vinfo);
                ::XCloseDisplay(m_context.display);
                m_bMustReleaseDisplay = m_bMustReleaseVisual = false;
              }
              pDevice->emitError("Could not create context.");
            }
            m_bMustReleaseContext = true;
          }
          else
          {
            m_context.m_glxContext = (GLXContext)pDevice->getProperty(OD_T("GLXContext"))->getIntPtr();
            pushCurrentContext();
            ODA_VERIFY(::glXMakeCurrent(m_context.display, m_context.window, m_context.m_glxContext));
          }
        }
        // Register context data
        if (m_bMustReleaseDisplay && pDevice->hasProperty(OD_T("XDisplay")))
          pDevice->setProperty(OD_T("XDisplay"), OdRxVariantValue((OdIntPtr)m_context.display));
        if (m_bMustReleaseWindow && pDevice->hasProperty(OD_T("XWindow")))
          pDevice->setProperty(OD_T("XWindow"), OdRxVariantValue((OdIntPtr)m_context.window));
        if (m_bMustReleaseContext && pDevice->hasProperty(OD_T("GLXContext")))
          pDevice->setProperty(OD_T("GLXContext"), OdRxVariantValue((OdIntPtr)m_context.m_glxContext));
        // Run extensions initialization
        GLES2_Extensions_Initialize(pDevice);
        popCurrentContext();
      }
      m_bContextCreated = true;
    }
    void updateContext(OdTrVisRenderClient * /*pDevice*/)
    {
      // @@@TODO: reallocate pixmap or pbuffer storage in case if context dimensions was changed
      // if (m_context.m_hContext && != pDevice->outputWindowWidth() && != pDevice->outputWindowHeight()))
    }
    void destroyContext()
    {
      if (m_context.m_glxContext)
      {
        pushCurrentContext();
        if (m_context.m_glxContext != ::glXGetCurrentContext())
#ifdef PBUFFER_WAY
          ::glXMakeContextCurrent(m_context.display, m_context.window, m_context.window, m_context.m_glxContext);
#else
          ::glXMakeCurrent(m_context.display, m_context.window, m_context.m_glxContext);
#endif
        // . . . Do required uninits
        if (!popCurrentContext() && m_bMustReleaseContext)
#ifdef PBUFFER_WAY
          ::glXMakeContextCurrent(m_context.display, None, None, NULL);
#else
          ::glXMakeCurrent(m_context.display, None, NULL);
#endif
        // Delete the rendering context
        if (m_bMustReleaseContext)
          ::glXDestroyContext(m_context.display, m_context.m_glxContext);
        if (m_bMustReleaseVisual)
          ::XFree(m_context.vinfo);
        if (m_bMustReleaseWindow)
        {
#ifdef PBUFFER_WAY
          glXDestroyPbuffer(m_context.display, m_context.window);
#else
          ::glXDestroyGLXPixmap(m_context.display, m_context.window);
          ::XFreePixmap(m_context.display, m_context.m_xPixmap);
#endif
        }
        if (m_bMustReleaseDisplay)
          XCloseDisplay(m_context.display);
        m_context.release();
        m_bMustReleaseDisplay = m_bMustReleaseWindow = m_bMustReleaseVisual = m_bMustReleaseContext = false;
      }
      m_bContextCreated = false;
    }

    bool isContextCreated() const
    {
      return m_bContextCreated;
    }

    void makeCurrentContext()
    {
      if (m_context.m_glxContext && (m_context.m_glxContext != ::glXGetCurrentContext()))
#ifdef PBUFFER_WAY
        ::glXMakeContextCurrent(m_context.display, m_context.window, m_context.window, m_context.m_glxContext);
#else
        ::glXMakeCurrent(m_context.display, m_context.window, m_context.m_glxContext);
#endif
    }

    bool pushCurrentContext()
    {
      if (m_context.m_glxContext && (m_context.m_glxContext != ::glXGetCurrentContext()))
      {
        m_pEntry = new GLContextEntry(::glXGetCurrentContext(),  m_context.display, ::glXGetCurrentDrawable(), m_pEntry);
        return true;
      }
      return false;
    }

    bool popCurrentContext()
    {
      if (m_pEntry)
      {
        GLContextEntry *pEntry = m_pEntry; m_pEntry = pEntry->m_pNext;
#ifdef PBUFFER_WAY
        ::glXMakeContextCurrent(pEntry->display, pEntry->window, pEntry->window, pEntry->m_glxContext);
#else
        ::glXMakeCurrent(pEntry->display, pEntry->window, pEntry->m_glxContext);
#endif
        delete pEntry; return true;
      }
      return false;
    }

    void presentContext()
    {
      if (m_context.m_glxContext)
        ::glXSwapBuffers(m_context.display, m_context.window);
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

OdSmartPtr<OdTrGL2LocalContext> OdTrGL2LocalContext::createLocalContext(OdTrVisRenderClient *pDevice)
{
  OdTrGL2LocalContextPtr pContext = OdRxObjectImpl<OdGLES2LocalContextImpl, OdTrGL2LocalContext>::createObject();
  pContext->createContext(pDevice);
  return pContext;
}

//
