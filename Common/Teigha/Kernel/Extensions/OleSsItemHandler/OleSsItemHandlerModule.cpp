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

// DD_OleSsItemHandler_dll.cpp : Defines the entry point for the DLL application.
//

#include "OdaCommon.h"
#include "OleSsItemHandlerModule.h"
#include "OleSsItemHandlerImpl.h"
#include "RxDynamicModule.h"
#include "RxSysRegistry.h"
#include "StaticRxObject.h"

ODRX_DEFINE_DYNAMIC_MODULE(OdOleSsItemHandlerModuleImpl);

static OdOleSsItemHandlerModuleImpl* g_pModule = 0;

static OdRxObjectPtr createOleSsItemHandler()
{
  return OdRxObjectImpl<OdOleSsItemHandlerImpl>::createObject().get();
}

void OdOleSsItemHandlerModuleImpl::initApp()
{
  g_pModule = this;
  m_pOleInitMutex.attach();
  OdOleItemHandler::desc()->module()->addRef();
  OdOleItemHandler::desc()->setConstructor(createOleSsItemHandler);

  OdOleSsItemHandlerImpl::rxInit();
  OdGiSelfGiDrawablePEImpl::rxInit();
  static OdStaticRxObject<OdGiSelfGiDrawablePEImpl> s_GiSelfGiDrawablePE;
  OdOleSsItemHandlerImpl::desc()->addX(OdGiSelfGiDrawablePE::desc(), &s_GiSelfGiDrawablePE);
}

void OdOleSsItemHandlerModuleImpl::uninitApp()
{
  OdOleSsItemHandlerImpl::desc()->delX(OdGiSelfGiDrawablePE::desc());
  OdGiSelfGiDrawablePEImpl::rxUninit();
  OdOleSsItemHandlerImpl::rxUninit();

  if (OdOleItemHandler::desc()->constructor() == createOleSsItemHandler)
    OdOleItemHandler::desc()->setConstructor(OdOleItemHandler::pseudoConstructor);
  OdOleItemHandler::desc()->module()->release();
  m_pOleInitMutex.detach();
  g_pModule = 0;
}

OdMutexPtr& OdOleSsItemHandlerModuleImpl::globalOleInitMutex()
{
  if (!g_pModule || !g_pModule->m_pOleInitMutex.m_pMutex)
    throw OdError(eNullPtr);
  return *(g_pModule->m_pOleInitMutex.m_pMutex);
}

void OdOleSsItemHandlerModuleImpl::internalLockModule()
{
  g_pModule->addRef();
}

void OdOleSsItemHandlerModuleImpl::internalUnlockModule()
{
  g_pModule->release();
}
