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
// GLES2 device local context (Emscripten)

#include "OdaCommon.h"
#include "TrGL2LocalContext.h"
#include "GLES2Include.h"

#include <EGL/egl.h>
#include <emscripten/html5.h>

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext {
protected:
  int handle;
  bool m_bContextCreated;
public:
  OdGLES2LocalContextImpl()
    : OdTrGL2LocalContext()
    , handle(-1)
    , m_bContextCreated(false)
  {
  }

  ~OdGLES2LocalContextImpl() { destroyContext(); }

  void createContext(OdTrVisRenderClient *pDevice) {
    if (handle < 0) {
      if (pDevice->hasProperty(OD_T("CreateContext")) && !pDevice->getProperty(OD_T("CreateContext"))->getBool()) {
        m_bContextCreated = true;
        return;
      }
      EmscriptenWebGLContextAttributes attrs;
      emscripten_webgl_init_context_attributes(&attrs);

      attrs.alpha = false;
      attrs.depth = true;
      attrs.stencil = true;
      attrs.antialias = false;

      handle = emscripten_webgl_create_context(0, &attrs);
    }
    m_bContextCreated = true;
  }

  void destroyContext() {
    if (handle > 0) {
      EMSCRIPTEN_RESULT res = emscripten_webgl_destroy_context(handle);
      handle = -1;
    }
    m_bContextCreated = false;
  }

  bool isContextCreated() const { return m_bContextCreated; }

  void makeCurrentContext() {
    if (handle > 0) {
      EMSCRIPTEN_RESULT res = emscripten_webgl_make_context_current(handle);
    }
  }

  void presentContext() { }

  bool isExtensionSupported(const char * pExtensionName) { 
    if (handle > 0) {
      return emscripten_webgl_enable_extension(handle, pExtensionName);
    }
    return false;
	}

  void *acquireExtensionFunctionPtr(const char *pFunctionName)
  {
    return (void*)::eglGetProcAddress(pFunctionName);
  }
};

OdSmartPtr<OdTrGL2LocalContext> OdTrGL2LocalContext::createLocalContext(OdTrVisRenderClient *pDevice)
{
  OdTrGL2LocalContextPtr pContext = OdRxObjectImpl<OdGLES2LocalContextImpl, OdTrGL2LocalContext>::createObject();
  pContext->createContext(pDevice);
  return pContext;
}

//
