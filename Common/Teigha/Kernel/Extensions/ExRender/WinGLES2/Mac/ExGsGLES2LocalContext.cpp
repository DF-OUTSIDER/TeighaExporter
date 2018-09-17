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
// GLES2 device local context (MacOS)

#include "OdaCommon.h"
#include "TrGL2LocalContext.h"
#include "GLES2Include.h"

// Offscreen Objc wrappers
void *odGLES2AttachCocoaPB(unsigned width, unsigned height);
void odGLES2DetachCocoaPB(void *pObj);
void odGLES2MakeCurrentCocoaPB(void *pObj);
bool odGLES2CheckDimsCocoaPB(void *pObj, unsigned width, unsigned height);
// Onscreen Objc wrappers
void *odGLES2AttachCocoaNS(void *pNSView, void *pGLContext, bool bDoubleBuffer, unsigned width, unsigned height);
void odGLES2DetachCocoaNS(void *pObj);
void odGLES2MakeCurrentCocoaNS(void *pObj);
void *odGLES2ExtractContextCocoaNS(void *pObj);
void odGLES2PresentCocoaNS(void *pObj);
bool odGLES2CheckDimsCocoaNS(void *pObj, unsigned width, unsigned height);

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext
{
  protected:
    void *m_pOffscreen;
    void *m_pOnscreen;
    bool m_bDoubleBuffer;
    bool m_bContextCreated;
  public:
    OdGLES2LocalContextImpl()
      : OdTrGL2LocalContext()
      , m_pOffscreen(NULL)
      , m_pOnscreen(NULL)
      , m_bDoubleBuffer(false)
      , m_bContextCreated(false)
    {
    }

    ~OdGLES2LocalContextImpl()
    {
      destroyContext();
    }

    void createContext(OdTrVisRenderClient *pDevice)
    {
      if (!m_pOffscreen && !m_pOnscreen)
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
        void *pNSView = NULL, *pGLContext = NULL;
        if (pDevice->hasProperty(OD_T("NSView")))
          pNSView = (void*)pDevice->getProperty(OD_T("NSView"))->getIntPtr();
        if (pDevice->hasProperty(OD_T("NSOpenGLContext")))
          pGLContext = (void*)pDevice->getProperty(OD_T("NSOpenGLContext"))->getIntPtr();
        if (bDirectBuffer)
        {
          m_pOffscreen = ::odGLES2AttachCocoaPB((unsigned)pDevice->outputWindowWidth(), (unsigned)pDevice->outputWindowHeight());
          if (!m_pOffscreen)
            pDevice->emitError("Can't initialize offscreen renderer.");
        }
        else
        {
          if (!pNSView && !pGLContext)
            pDevice->emitError("No window specified to attach OpenGL context.");
          m_pOnscreen = ::odGLES2AttachCocoaNS(pNSView, pGLContext, m_bDoubleBuffer, (unsigned)pDevice->outputWindowWidth(), (unsigned)pDevice->outputWindowHeight());
          if (!m_pOnscreen)
            pDevice->emitError("Can't initialize onscreen renderer.");
        }
        // Register context data
        if (m_pOnscreen && !pGLContext && pDevice->hasProperty(OD_T("NSOpenGLContext")))
          pDevice->setProperty(OD_T("NSOpenGLContext"), OdRxVariantValue((OdIntPtr)::odGLES2ExtractContextCocoaNS(m_pOnscreen)));
        // Run extensions initialization
        GLES2_Extensions_Initialize(pDevice);
      }
      m_bContextCreated = true;
    }
    void updateContext(OdTrVisRenderClient *pDevice)
    {
      if (m_pOffscreen && !::odGLES2CheckDimsCocoaPB(m_pOffscreen, (unsigned)pDevice->outputWindowWidth(), (unsigned)pDevice->outputWindowHeight()))
      {
        if (pDevice->hasProperty(OD_T("SaveContextData"))) // Save OpenGL context data
          pDevice->setProperty(OD_T("SaveContextData"), OdRxVariantValue(true));
        ::odGLES2DetachCocoaPB(m_pOffscreen);
        m_pOffscreen = ::odGLES2AttachCocoaPB((unsigned)pDevice->outputWindowWidth(), (unsigned)pDevice->outputWindowHeight());
        if (pDevice->hasProperty(OD_T("SaveContextData"))) // Load OpenGL context data
          pDevice->setProperty(OD_T("SaveContextData"), OdRxVariantValue(false));
      }
      if (m_pOnscreen)
        ::odGLES2CheckDimsCocoaNS(m_pOnscreen, (unsigned)pDevice->outputWindowWidth(), (unsigned)pDevice->outputWindowHeight());
    }
    void destroyContext()
    {
      if (m_pOffscreen)
      {
        ::odGLES2DetachCocoaPB(m_pOffscreen);
        m_pOffscreen = NULL;
      }
      if (m_pOnscreen)
      {
        ::odGLES2DetachCocoaNS(m_pOnscreen);
        m_pOnscreen = NULL;
      }
      m_bContextCreated = false;
    }

    bool isContextCreated() const
    {
      return m_bContextCreated;
    }

    void makeCurrentContext()
    {
      if (m_pOffscreen)
        ::odGLES2MakeCurrentCocoaPB(m_pOffscreen);
      if (m_pOnscreen)
        ::odGLES2MakeCurrentCocoaNS(m_pOnscreen);
    }

    void presentContext()
    {
      if (m_pOnscreen)
        ::odGLES2PresentCocoaNS(m_pOnscreen);
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
