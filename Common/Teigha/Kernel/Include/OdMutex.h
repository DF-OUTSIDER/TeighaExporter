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

#ifndef _OD_MUTEX_H_
#define _OD_MUTEX_H_

#include "RootExport.h"
#include "TD_PackPush.h"
#ifdef OD_POSIX_THREADS
#include <pthread.h>
#elif defined(ODA_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

/** \details
    This class implements Mutex objects in a Teigha context.
    
    \remarks
    Mutex objects are used to synchronize between threads and across processes.
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdMutex 
{
#ifdef OD_POSIX_THREADS
  pthread_mutex_t _mutex;
public:
  OdMutex()
  {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); 
    pthread_mutex_init(&_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
  }
  ~OdMutex()
  {
    pthread_mutex_destroy((pthread_mutex_t*)&_mutex);
  }
  /** \details
  Locks this Mutex object.
  */
  void lock()
  {
    pthread_mutex_lock(&_mutex);
  }
  /** \details
  Unlocks this Mutex object.
  */
  void unlock()
  {
    pthread_mutex_unlock(&_mutex);
  }
#elif (defined(ODA_WINDOWS)) && !defined(_WIN32_WCE) && !defined(_WINRT)
  CRITICAL_SECTION _mutex;
public:
  OdMutex()
  {
    InitializeCriticalSection(&_mutex);
  }
  ~OdMutex()
  {
    DeleteCriticalSection(&_mutex);
  }
  /** \details
  Locks this Mutex object.
  */
  void lock()
  {
    EnterCriticalSection(&_mutex);
  }
  /** \details
  Unlocks this Mutex object.
  */
  void unlock()
  {
    LeaveCriticalSection(&_mutex);
  }
#else
public:
  OdMutex() {}
  ~OdMutex() {}
  /** \details
  Locks this Mutex object.
  */
  void lock() {}
  /** \details
  Unlocks this Mutex object.
  */
  void unlock() {}
#endif
};

/** \details
    This class implements Mutex AutoLock objects in a Teigha context.
    
    \remarks
    Mutex objects are used to synchronize between threads and across processes.
    
    MutexAutoLock objects automatically lock the specified OdMutex object
    when constructed and unlock it when destroyed.
    
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
class OdMutexAutoLock
{
public:
  /** \param mutex [in]  Mutex to be Autolocked.
  */
  OdMutexAutoLock(OdMutex& mutex) : m_Mutex(mutex)
  {
    m_Mutex.lock();
  }
  ~OdMutexAutoLock()
  { 
    m_Mutex.unlock();
  }
private:
  OdMutex& m_Mutex;
};

#ifndef TD_SINGLE_THREAD
#define TD_AUTOLOCK(Mutex) OdMutexAutoLock autoLock(Mutex);
#else
#define TD_AUTOLOCK(Mutex)
#endif


#ifndef TD_SINGLE_THREAD
#if defined(_MSC_VER) && _M_IX86 >= 400 && !defined(_WIN32_WCE)
// Intel486 platform with Microsoft compiler
#pragma warning(push)
#pragma warning(disable:4035)
#pragma warning(disable:4793)
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  __asm
  {
    mov edx, dest
    mov eax, val
    lock xchg [edx], eax
  }
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  __asm
  {
    mov edx, dest
    mov eax, incr
    lock xadd [edx], eax
  }
}
inline int OdInterlockedIncrement(volatile int* dest)
{
  __asm
  {
    mov edx, dest
    mov eax, 1
    lock xadd [edx], eax
    inc eax
  }
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  __asm
  {
    mov edx, dest
    mov eax, -1
    lock xadd [edx], eax
    dec eax
  }
}
#pragma warning(pop)
#elif (defined(_WIN32) || defined(_WIN64)) && !defined(_WIN32_WCE) && !defined(_WINRT) && !defined(ODA_WINDOWS_GCC)
// Windows platform with generic compiler - use interlocked functions from Win32 API
#define OdInterlockedExchange(dest, val) InterlockedExchange((LONG*)(dest), val)
#define OdInterlockedExchangeAdd(dest, incr) InterlockedExchangeAdd((LONG*)(dest), incr)
#define OdInterlockedIncrement(dest) InterlockedIncrement((LONG*)(dest))
#define OdInterlockedDecrement(dest) InterlockedDecrement((LONG*)(dest))
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
// Intel platform with GCC compiler
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  int ret;
  __asm__ __volatile__ (
    "lock; xchgl %0, (%1)"
    : "=r" (ret)
    : "r" (dest), "0" (val)
    : "memory");
  return ret;
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  int ret;
  __asm__ __volatile__ (
    "lock; xaddl %0, (%1)"
    : "=r" (ret)
    : "r" (dest), "0" (incr)
    : "memory");
  return ret;
}
inline int OdInterlockedIncrement(volatile int* dest)
{
  return OdInterlockedExchangeAdd(dest, 1) + 1;
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return OdInterlockedExchangeAdd(dest, -1) - 1;
}
#elif defined(__GNUC__) && defined(__POWERPC__)
// Macintosh PowerPC platform with GCC compiler
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  // Assembler code is taken from Wine 0.9.4 sources
  // See http://cvs.winehq.org/cvsweb/wine/libs/port/interlocked.c?rev=1.7&content-type=text/x-cvsweb-markup
  int ret = 0;
  __asm__ __volatile__ (
    "0:  lwarx %0, 0, %1\n"
    "    stwcx. %2, 0, %1\n"
    "    bne- 0b\n"
    "    isync\n"
    : "=&r"(ret)
    : "r"(dest), "r"(val)
    : "cr0", "memory", "r0");
  return ret;
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  // Assembler code is taken from Wine 0.9.4 sources
  // See http://cvs.winehq.org/cvsweb/wine/libs/port/interlocked.c?rev=1.7&content-type=text/x-cvsweb-markup
  int ret = 0;
  int zero = 0;
  __asm__ __volatile__ (
    "0:  lwarx %0, %3, %1\n"
    "    add %0, %2, %0\n"
    "    stwcx. %0, %3, %1\n"
    "    bne- 0b\n"
    "    isync\n"
    : "=&r" (ret)
    : "r"(dest), "r"(incr), "r"(zero)
    : "cr0", "memory", "r0");
  return ret - incr;
}
inline int OdInterlockedIncrement(volatile int* dest)
{
  return OdInterlockedExchangeAdd(dest, 1) + 1;
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return OdInterlockedExchangeAdd(dest, -1) - 1;
}
#elif defined(__APPLE__)
// iOS or Mac platform
#include <libkern/OSAtomic.h>
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  int oldValue = *dest;
  while (!OSAtomicCompareAndSwapIntBarrier(oldValue, val, dest))
    oldValue = *dest;
  return oldValue;
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  return OSAtomicAdd32Barrier(incr, dest) - incr;
}
inline int OdInterlockedIncrement(volatile int* dest)
{
  return OSAtomicIncrement32Barrier(dest);
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return OSAtomicDecrement32Barrier(dest);
}
#elif defined(ANDROID) && !defined(ANDROID_GOOGLE)
//TODO Android r10c NDK doesn't provide such functionality
// Android platform with GCC or MinGW compiler
#include <sys/atomics.h>
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  return __atomic_swap(val, dest);
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  // There is no atomic instruction to add. This implementation isn't completely safe.
  return __atomic_swap(*dest + incr, dest);
}
inline int OdInterlockedIncrement(volatile int* dest)
{
  return __atomic_inc(dest) + 1;
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return __atomic_dec(dest) - 1;
}
/* 
#elif defined(__sun)
// sparc on Sun Studio compiler, solaris
#include <atomic.h>
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  return (int)atomic_swap_uint((volatile uint_t*)dest, (uint_t)val);
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  return (int)atomic_add_int_nv((volatile uint_t*)dest, incr) - incr;
}
inline int OdInterlockedIncrement(volatile int* dest)
{
  return (int)atomic_inc_uint_nv((volatile uint_t*)dest);
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return (int)atomic_dec_uint_nv((volatile uint_t*)dest);
}
#elif defined (__hpux)
// HPUX (require libatomic: https://h20392.www2.hp.com/portal/swdepot/displayProductInfo.do?productNumber=Atomic)
#include <atomic.h>
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  return (int)atomic_swap_32((volatile uint32_t*)dest, (uint32_t)val);
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  // There is no atomic instruction to add. This implementation isn't completely safe.
  return (int)atomic_swap_32((volatile uint32_t*)dest, (uint32_t)(*desc + incr));
}
inline int OdInterlockedIncrement(volatile int* dest)
{
  return (int)atomic_inc_32((volatile uint32_t*)dest) + 1;
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return (int)atomic_dec_32((volatile uint32_t*)dest) - 1;
}*/
#else
// here should go other platforms
// synchronization is disabled if no atomic functions are defined
#define TD_SINGLE_THREAD
#endif //architecture
#endif //TD_SINGLE_THREAD

/** \details
    This class implements reference counter with interlocked operations for Teigha objects.
    
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
#ifndef TD_SINGLE_THREAD
#ifdef _MANAGED
#pragma managed(push, off)
#endif
/** <group !!RECORDS_tkernel_apiref>
*/
  struct OdRefCounter
{
  typedef int RefCounterType;
  volatile RefCounterType _ref_count;
  OdRefCounter& operator = (RefCounterType n) { _ref_count = 0; OdInterlockedExchange(&_ref_count, n); return *this; }
  operator RefCounterType () const { return OdInterlockedExchangeAdd(const_cast<RefCounterType*>(&_ref_count), 0); }
  RefCounterType operator ++ () { return OdInterlockedIncrement(&_ref_count); }
  RefCounterType operator -- () { return OdInterlockedDecrement(&_ref_count); }
  // 1 as default is not correct for all classes 
  // (see for example OdArrayBuffer, CAllocator)
  OdRefCounter() : _ref_count(-1) {} // undefined
  OdRefCounter(int n) : _ref_count(n) {}
};

/** <group !!RECORDS_tkernel_apiref>
*/
  struct OdVolatile
{
  typedef int VolatileType;
  volatile VolatileType _val;
  OdVolatile& operator = (VolatileType n) { _val = 0; OdInterlockedExchange(&_val, n); return *this; }
  operator VolatileType () const { return OdInterlockedExchangeAdd(const_cast<VolatileType*>(&_val), 0); }
  VolatileType operator|=(VolatileType n) { return OdInterlockedExchange(&_val, _val|n); }
  VolatileType operator&=(VolatileType n) { return OdInterlockedExchange(&_val, _val&n); }
  OdVolatile() : _val(0) {}
  OdVolatile(int n): _val(n) {}
};

#ifdef _MANAGED
#pragma managed(pop)
#endif

#else
typedef int OdRefCounter;
typedef int OdVolatile;
#endif

#include "TD_PackPop.h"
#endif
