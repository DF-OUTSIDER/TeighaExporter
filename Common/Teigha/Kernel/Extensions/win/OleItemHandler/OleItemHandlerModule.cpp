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

// DD_OLEItemHandler_dll.cpp : Defines the entry point for the DLL application.
//

#include "OdaCommon.h"
#include "OleItemHandlerModule.h"
#include "OleItemHandlerImpl.h"
#include "RxDynamicModule.h"
#include "RxSysRegistry.h"

ODRX_DEFINE_DYNAMIC_MODULE(OdOleItemHandlerModuleImpl);

static OdOleItemHandlerModuleImpl* g_pModule = 0;

static OdRxObjectPtr createOleItemHandler()
{
  return OdRxObjectImpl<OdOleItemHandlerImpl>::createObject().get();
}

void OdOleItemHandlerModuleImpl::initApp()
{
  g_pModule = this;

  m_pOleInitMutex.attach();
  
  OdOleItemHandler::desc()->module()->addRef();

  OdOleItemHandler::desc()->setConstructor(createOleItemHandler);
}

void OdOleItemHandlerModuleImpl::uninitApp()
{
  if(OdOleItemHandler::desc()->constructor() == createOleItemHandler)
  {
    OdOleItemHandler::desc()->setConstructor(OdOleItemHandler::pseudoConstructor);
  }

  OdOleItemHandler::desc()->module()->release();

  m_pOleInitMutex.detach();

  g_pModule = 0;
}

OdMutexPtr &OdOleItemHandlerModuleImpl::globalOleInitMutex()
{
  if (!g_pModule || !g_pModule->m_pOleInitMutex.m_pMutex)
    throw OdError(eNullPtr);
  return *(g_pModule->m_pOleInitMutex.m_pMutex);
}

void OdOleItemHandlerModuleImpl::internalLockModule()
{
  g_pModule->addRef();
}

void OdOleItemHandlerModuleImpl::internalUnlockModule()
{
  g_pModule->release();
}
