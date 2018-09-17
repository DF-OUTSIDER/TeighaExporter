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
// GLES2 device local context

#ifndef ODTRGL2LOCALCONTEXT
#define ODTRGL2LOCALCONTEXT

#include "TD_PackPush.h"

#include "../TrVisRenderClient.h"

class OdTrGL2ExtensionsRegistry;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrGL2LocalContext : public OdRxObject
{
  protected:
    mutable OdRxObjectPtr m_pExtensionsRegistry;
  public:
    OdTrGL2LocalContext() { }
    ~OdTrGL2LocalContext() { }

    // Each platform implementation must implement last one method
    static OdSmartPtr<OdTrGL2LocalContext> createLocalContext(OdTrVisRenderClient *pDevice);

    virtual void createContext(OdTrVisRenderClient *pDevice) = 0;
    virtual void updateContext(OdTrVisRenderClient * /*pDevice*/) { }
    virtual void destroyContext() = 0;

    virtual bool isContextCreated() const = 0;

    virtual void makeCurrentContext() = 0;

    virtual bool pushCurrentContext() { return false; }
    virtual bool popCurrentContext() { return false; }

    virtual void presentContext() = 0;

    virtual bool isExtensionBasedEmulation() const { return false; }
    virtual bool isExtensionSupported(const char * /*pExtensionName*/) { return false; }
    virtual void *acquireExtensionFunctionPtr(const char * /*pFunctionName*/) { return NULL; }

    // Search OpenGL extension in cross-platform way using our own extensions registry
    OdTrGL2ExtensionsRegistry &extensionsRegistry() const;
};

typedef OdSmartPtr<OdTrGL2LocalContext> OdTrGL2LocalContextPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrGL2LocalContextStub : public OdTrGL2LocalContext
{
  public:
    OdTrGL2LocalContextStub() : OdTrGL2LocalContext() { }
    ~OdTrGL2LocalContextStub() { }

    virtual void createContext(OdTrVisRenderClient * /*pDevice*/) { }
    virtual void destroyContext() { }

    virtual bool isContextCreated() const { return true; }

    virtual void makeCurrentContext() { }

    virtual void presentContext() { }
};

#include "TD_PackPop.h"

#endif // ODTRGL2LOCALCONTEXT
