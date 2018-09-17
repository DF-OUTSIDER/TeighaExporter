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

#ifndef _QhMutex_h_Included_
#define _QhMutex_h_Included_

#include "OdaCommon.h"

#ifdef OD_POSIX_THREADS
  #include <pthread.h>
#elif defined(ODA_WINDOWS)
  #include <windows.h>
#endif

#include "TD_PackPush.h"

class QhMutex
{
#ifdef OD_POSIX_THREADS
  pthread_mutex_t _mutex;
public:
  QhMutex()
  {
    pthread_mutex_init(&_mutex, 0);
  }
  ~QhMutex()
  {
    pthread_mutex_destroy((pthread_mutex_t*)&_mutex);
  }
  void lock()
  {
    pthread_mutex_lock(&_mutex);
  }
  void unlock()
  {
    pthread_mutex_unlock(&_mutex);
  }
#elif defined(ODA_WINDOWS) && !defined(_WINRT)
  CRITICAL_SECTION _mutex;
public:
  QhMutex()
  {
    InitializeCriticalSection(&_mutex);
  }
  ~QhMutex()
  {
    DeleteCriticalSection(&_mutex);
  }
  void lock()
  {
    EnterCriticalSection(&_mutex);
  }
  void unlock()
  {
    LeaveCriticalSection(&_mutex);
  }
#else
public:
  QhMutex() {}
  ~QhMutex() {}
  void lock() {}
  void unlock() {}
#endif
};

#include "TD_PackPop.h"
#endif // _QhMutex_h_Included_
