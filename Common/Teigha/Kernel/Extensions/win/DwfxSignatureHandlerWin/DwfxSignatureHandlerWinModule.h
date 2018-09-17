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

#ifndef _OD_DWFXSIGNATUREHANDLER_WIN_MODULE_H_
#define _OD_DWFXSIGNATUREHANDLER_WIN_MODULE_H_

#include "TD_PackPush.h"

#include "OdDwfxSignatureHandler.h"
#include "ThreadsCounter.h"
#include <objbase.h>

class DwfxSignatureHandlerWinModule : public OdDwfxSignatureHandlerModule
{
public:
  virtual OdDwfxSignatureHandlerPtr getDwfxSignatureHandler() const;

  virtual int getSuitableCertificatesList(OdArray<OdCertParameters>& certificates) const;

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
  } m_pCOMInitMutex;

public:
  static OdMutexPtr& globalCOMInitMutex();
};

class COMInit
{
  HRESULT hr;
  bool m_bLocked;
public:
  COMInit()
  {
    m_bLocked = odThreadsCounter();
    if (m_bLocked)
      DwfxSignatureHandlerWinModule::globalCOMInitMutex().get()->lock();
    
    if ( (hr = ::CoInitializeEx( 0, COINIT_APARTMENTTHREADED )) == RPC_E_CHANGED_MODE )
      hr = ::CoInitializeEx( 0, COINIT_MULTITHREADED );
    if (FAILED(hr))
    {
      throw OdError(OD_T("Error in DwfxSignatureHandlerWin : Can't initialize COM"));
    }
  }
  ~COMInit()
  {
    if(SUCCEEDED(hr) && hr != S_FALSE)
      ::CoUninitialize();

    if (m_bLocked)
      DwfxSignatureHandlerWinModule::globalCOMInitMutex().get()->unlock();
  }
};

#include "TD_PackPop.h"

#endif
