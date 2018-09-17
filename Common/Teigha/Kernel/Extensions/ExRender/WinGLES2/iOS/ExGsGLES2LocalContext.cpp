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
// GLES2 device local context (iOS)

#include "OdaCommon.h"
#include "TrGL2LocalContext.h"
#include "GLES2Include.h"
#include "ExGsGLES2LocalContextImpl.h"

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext
{
  protected:
    void *m_pLocalContext;
    bool m_bContextCreated;
    void *m_pExternalContext;
  public:
    OdGLES2LocalContextImpl()
      : m_pLocalContext(NULL)
      , m_bContextCreated(false)
      , m_pExternalContext(NULL)
    {
    }

    ~OdGLES2LocalContextImpl()
    {
      destroyContext();
    }

    void createContext(OdTrVisRenderClient *pDevice)
    {
      if (!m_pLocalContext)
      {
        if (pDevice->hasProperty(OD_T("CreateContext")) &&
            !pDevice->getProperty(OD_T("CreateContext"))->getBool())
        {
          m_bContextCreated = true;
          return;
        }
        const bool bDirectBuffer = pDevice->hasDirectRenderBuffer();
        if (!bDirectBuffer)
        {
          if (pDevice->hasProperty(OD_T("EAGLContext")) &&
              (pDevice->getProperty(OD_T("EAGLContext"))->getIntPtr() != 0))
          {
            m_pExternalContext = (void*)pDevice->getProperty(OD_T("EAGLContext"))->getIntPtr();
            ::odGLES2MakeCurrentExternalContext(m_pExternalContext);
          }
          else if (pDevice->hasProperty(OD_T("EAGLLayer")))
          {
            void *pLayer = (void*)pDevice->getProperty(OD_T("EAGLLayer"))->getIntPtr();
            if (!pLayer)
              pDevice->emitError("EAGLLayer not set.");
            m_pLocalContext = ::odGLES2AttachLocalContext(pLayer);
            if (!m_pLocalContext)
              pDevice->emitError("OpenGL context cannot be created.");
            if (pDevice->hasProperty(OD_T("EAGLContext")))
              pDevice->setProperty(OD_T("EAGLContext"), OdRxVariantValue((OdIntPtr)::odGLES2ExtractPointerForLocalContext(m_pLocalContext)));
          } else
            pDevice->emitError("No \"EAGLLayer\" property found.");
        }
        else
        {
          OdUInt32 pixelWidth, pixelHeight;
          pDevice->getDirectRenderBuffer(&pixelWidth, &pixelHeight);
          m_pLocalContext = ::odGLES2AttachLocalContext((long)pixelWidth, (long)pixelHeight);
          if (!m_pLocalContext)
            pDevice->emitError("OpenGL context cannot be created.");
        }
      }
      m_bContextCreated = true;
    }
    void updateContext(OdTrVisRenderClient *pDevice)
    {
      if (m_pLocalContext &&
          !::odGLES2CheckDimensionsOfLocalContext(m_pLocalContext, pDevice->outputWindowWidth(), pDevice->outputWindowHeight()))
      {
        void *pLayer = NULL;
        if (!::odGLES2IsOffscreenLocalContext(m_pLocalContext) &&
            pDevice->hasProperty(OD_T("EAGLLayer")))
        {
          pLayer = (void*)pDevice->getProperty(OD_T("EAGLLayer"))->getIntPtr();
          if (!pLayer)
            pDevice->emitError("EAGLLayer not set.");
        }
        ::odGLES2UpdateLocalContext(m_pLocalContext, pLayer, pDevice->outputWindowWidth(), pDevice->outputWindowHeight());
      }
    }
    void destroyContext()
    {
      if (m_pLocalContext)
      {
        ::odGLES2DetachLocalContext(m_pLocalContext);
        m_pLocalContext = NULL;
      }
      m_bContextCreated = false;
      m_pExternalContext = NULL;
    }

    bool isContextCreated() const
    {
      return m_bContextCreated;
    }

    virtual void makeCurrentContext()
    {
      if (m_pLocalContext)
        ::odGLES2MakeCurrentLocalContext(m_pLocalContext);
      else if (m_pExternalContext)
        ::odGLES2MakeCurrentExternalContext(m_pExternalContext);
    }

    virtual void presentContext()
    {
      if (m_pLocalContext)
        ::odGLES2PresentLocalContext(m_pLocalContext);
      else if (m_pExternalContext)
        ::odGLES2PresentExternalContext(m_pExternalContext);
    }
};

OdSmartPtr<OdTrGL2LocalContext> OdTrGL2LocalContext::createLocalContext(OdTrVisRenderClient *pDevice)
{
  OdTrGL2LocalContextPtr pContext = OdRxObjectImpl<OdGLES2LocalContextImpl, OdTrGL2LocalContext>::createObject();
  pContext->createContext(pDevice);
  return pContext;
}

//
