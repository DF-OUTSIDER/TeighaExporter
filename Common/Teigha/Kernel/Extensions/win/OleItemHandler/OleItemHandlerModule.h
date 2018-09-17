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

#if !defined(_ODOLEITEMHANDLERMODULE_INCLUDED_)
#define _ODOLEITEMHANDLERMODULE_INCLUDED_

#include "TD_PackPush.h"

#include "RxDynamicModule.h"
#include "ThreadsCounter.h"
#include "COMErrorCtx.h"
#include "comdef.h"

/** \details
  This class implements on OLE item handler module for Windows.
  Library: Source code provided.
  <group Win_Classes> 
*/
class OdOleItemHandlerModuleImpl : public OdRxModule
{
protected:
  void initApp();

  void uninitApp();
protected:
  struct OdMutexPtr2
  {
    OdMutexPtr *m_pMutex;
    OdMutexPtr2() : m_pMutex(NULL) { }
    void detach() { if (m_pMutex) { delete m_pMutex; m_pMutex = NULL; } }
    void attach() { if (!m_pMutex) { m_pMutex = new OdMutexPtr(); } }
    ~OdMutexPtr2() { detach(); }
  } m_pOleInitMutex;
public:
  static OdMutexPtr &globalOleInitMutex();
  static void internalLockModule();
  static void internalUnlockModule();
};

/** \details
  This class implements a safe OleInitialize and OleUninitialize.
  \sa
  Source code provided.
  <group Win_Classes> 
*/
class OleInit
{
  HRESULT hr;
  bool m_bLocked;
public:
  OleInit()
  {
    m_bLocked = odThreadsCounter();
    if (m_bLocked)
      OdOleItemHandlerModuleImpl::globalOleInitMutex().get()->lock(); // #11870 : prevent init/uninit conflicts
    // OleInitialize will initialize COM as single-thread apartment model. This is enough for us because all
    // manipulations with Ole object is localized by single methods and doesn't interfere with other threads.
    hr = ::OleInitialize(NULL);
    if(FAILED(hr) && hr != RPC_E_CHANGED_MODE)
      ErrorCtx::fire(hr);
  }
  ~OleInit()
  {
    if(SUCCEEDED(hr) && hr != S_FALSE)
      ::OleUninitialize();
    if (m_bLocked)
      OdOleItemHandlerModuleImpl::globalOleInitMutex().get()->unlock(); // #11870 : prevent init/uninit conflicts
  }
};

#include "TD_PackPop.h"

#endif //#if !defined(_ODOLEITEMHANDLERMODULE_INCLUDED_)
