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

#ifndef _THREADS_COUNTER_H_
#define _THREADS_COUNTER_H_

#include "TD_PackPush.h"
#include "OdMutex.h"

/** \details
    This class implements pointer to a Mutex in a Teigha context,
    which is initialized on demand.

    Corresponding C++ library: Root
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdMutexPtr
{
public:
  OdMutexPtr(): m_ptr(0){}
  ~OdMutexPtr();
  OdMutex* get() { if(!m_ptr) create(); return m_ptr; }
  void clear();
private:
  void create();

private:
  static OdMutex s_mt;
  OdMutex* m_ptr;
};

/** \details
    This class implements AutoLock object for OdMutex*.
    
    \remarks
    Mutex objects are used to synchronize between threads and across processes.
    
    MutexAutoLock objects automatically lock the specified OdMutex object
    when constructed and unlock it when destroyed.
    
    Corresponding C++ library: Root
    <group Other_Classes>
*/
class OdMutexPtrAutoLock
{
public:
  /** \param mutex [in]  Mutex to be Autolocked.
  */
  OdMutexPtrAutoLock(OdMutex* mutex) : m_mutex(mutex), m_bEnable(false)
  {
    lock();
  }

  ~OdMutexPtrAutoLock()
  {
    unlock();
  }

  void lock() { set(true); }
  void unlock() { set(false); }

protected:
  OdMutexPtrAutoLock(): m_mutex(NULL), m_bEnable(false){}
  void set(bool bEnable)
  {
    if(m_mutex && (m_bEnable != bEnable))
    {
      m_bEnable = bEnable;
      if(bEnable)
        m_mutex->lock();
      else
        m_mutex->unlock();
    }
  }
protected:
  OdMutex* m_mutex;
private:
  bool m_bEnable;
};

/** \details
        
    Corresponding C++ library: Root

    <group Other_Classes> 
*/
class ThreadsCounterReactor
{
  public:
    ThreadsCounterReactor() { }
    virtual ~ThreadsCounterReactor() { }

    /** \details
      Called from initial thread to increase execution threads count.
      \param nThreads [in]  Count of threads in aThreads array.
      \param aThreads [in]  Thread ID's array.
      \param nThreadAttributes [in]  Set of attributes for running threads.
    */
    virtual void increase(unsigned /*nThreads*/, const unsigned* /*aThreads*/, unsigned /*nThreadAttributes*/) { }
    /** \details
      Called from initial thread to decrease execution threads count.
      \param nThreads [in]  Count of threads in aThreads array.
      \param aThreads [in]  Thread ID's array.
      \param nThreadAttributes [in]  Set of attributes for running threads.
    */
    virtual void decrease(unsigned /*nThreads*/, const unsigned* /*aThreads*/, unsigned /*nThreadAttributes*/) { }

    /** \details
      Called from running thread immediately before any operations processing.
      \param nThreadId [in]  Running Thread ID.
      \param nThreadAttributes [in]  Set of attributes for running thread.
    */
    virtual void startThread(unsigned /*nThreadId*/, unsigned /*nThreadAttributes*/) { }
    /** \details
      Called from running thread immediately after thread operations processed.
      \param nThreadId [in]  Running Thread ID.
      \param nThreadAttributes [in]  Set of attributes for running thread.
    */
    virtual void stopThread(unsigned /*nThreadId*/, unsigned /*nThreadAttributes*/) { }
};

// Function for execute in the main thread
typedef void (*MainThreadFunc)(void*);
// Function for initiate execution in the main thread by external process
typedef void (*ExecuteMainThreadFunc)(MainThreadFunc, void*);

class OdRxThreadPoolService;

/** \details
        
    Corresponding C++ library: Root

    <group Other_Classes> 
*/
class FIRSTDLL_EXPORT ThreadsCounter
{
public:
  enum ThreadAttributes
  {
    kNoAttributes = 0, // Simple Mt-process which doesn't require any special initializations

    kMtLoadingAttributes = (1 << 0), // Actual for both MtLoading and loading of DB's in multiple threads
    kMtRegenAttributes   = (1 << 1), // Multithread regeneration
    kStRegenAttributes   = (1 << 2), // Regeneration w/o cache (local heaps not required)
    kMtDisplayAttributes = (1 << 3), // Multithread display
    kMtModelerAttributes = (1 << 4), // Modeling operations in parallel threads
    kAllAttributes       = 0xFFFFFFFF // Applicable for Mt functionality tests
  };
protected:
  ThreadsCounter()
    : m_count(1)
    , m_mainThreadFunc(NULL)
    , m_threadPool(NULL) { }
  ~ThreadsCounter() { }
public:
  /** \details
      Returns true if ThreadsCounter has registered threads and Teigha works in multithreading mode.
  */
  operator bool() const { return m_count > 1; }

  /** \details
      Adds reactor into threads counter reactors chain.
      \param pReactor [in]  Function to add into reactors chain.
      \returns
      Returns true if reactor added; returns false if reactor already available in chain.
  */
  bool addReactor(ThreadsCounterReactor *pReactor);
  /** \details
      Removes reactor from threads counter reactors chain.
      \param pReactor [in]  Reactor to remove from reactors chain.
      \returns
      Returns true if reactor removed; returns false if reactor isn't available in chain.
  */
  bool removeReactor(ThreadsCounterReactor *pReactor);
  /** \details
      Checks whether reactor is available in threads counter reactors chain.
      \param pReactor [in]  Reactor to check.
  */
  bool hasReactor(ThreadsCounterReactor *pReactor) const;
  /** \details
      Returns count of reactors in threads counter reactors chain.
  */
  int nReactors() const;

  /* Following methods are intended for internal usage by Teigha libraries and not recommended for usage outside. */

  /** \details
      Save pointer for current OdRxThreadPoolService.
      \param pServices [in]  OdRxThreadPoolService pointer.
  */
  void setThreadPoolService(OdRxThreadPoolService *pService) { m_threadPool = pService; }
  /** \details
      Returns pointer for current OdRxThreadPoolService.
  */
  OdRxThreadPoolService *getThreadPoolService() { return m_threadPool; }

  /** \details
      Increase execution threads count.
      \param nThreads [in]  Count of threads in aThreads array.
      \param aThreads [in]  Thread ID's array.
      \param nThreadAttributes [in]  Set of attributes for running threads.
  */
  void increase(unsigned nThreads, const unsigned* aThreads, unsigned nThreadAttributes = ThreadsCounter::kNoAttributes)
  { ++m_count; increaseProc(nThreads, aThreads, nThreadAttributes); }
  /** \details
      Decrease execution threads count.
      \param nThreads [in]  Count of threads in aThreads array.
      \param aThreads [in]  Thread ID's array.
  */
  void decrease(unsigned nThreads, const unsigned* aThreads)
  { --m_count; decreaseProc(nThreads, aThreads); }

  /** \details
      Called when execution thread started.
  */
  void startThread() { startThreadProc(); }
  /** \details
      Called when execution thread stopped.
  */
  void stopThread() { stopThreadProc(); }

  /** \details
      Returns true if increase called for specified thread. Optionally returns thread attributes.
      \param nThreadId [in]  Requested thread ID.
      \param pThreadAttributes [out]  If set takes requested thread attributes.
  */
  bool hasThread(unsigned nThreadId, unsigned *pThreadAttributes)
  { return hasThreadProc(nThreadId, pThreadAttributes); }

  /** \details
      Setup function for execution in the external main thread.
      \param func [in]  Function for execution in the main thread.
  */
  void setMainThreadFunc(ExecuteMainThreadFunc func) { m_mainThreadFunc = func; }
  /** \details
      Returns function for execution in the external main thread.
  */
  ExecuteMainThreadFunc getMainThreadFunc() { return m_mainThreadFunc; }

protected:
  void increaseProc(unsigned nThreads, const unsigned* aThreads, unsigned nThreadAttributes);
  void decreaseProc(unsigned nThreads, const unsigned* aThreads);
  void startThreadProc();
  void stopThreadProc();
  bool hasThreadProc(unsigned nThreadId, unsigned *pThreadAttributes);

protected:
  OdRefCounter           m_count;
  ExecuteMainThreadFunc  m_mainThreadFunc;
  OdRxThreadPoolService *m_threadPool;
};

FIRSTDLL_EXPORT ThreadsCounter&   odThreadsCounter();

FIRSTDLL_EXPORT bool              odExecuteMainThreadAction(MainThreadFunc mtFunc, void *pArg, bool bExecST = true);

FIRSTDLL_EXPORT unsigned          odGetCurrentThreadId();
FIRSTDLL_EXPORT void              odThreadYield();

#define TD_AUTOLOCK_P(Mutex)      OdMutexPtrAutoLock autoLock(Mutex);
#define TD_AUTOLOCK_P_DEF(Mutex)  OdMutexPtrAutoLock autoLock(odThreadsCounter() ? Mutex.get() : NULL);

#include "TD_PackPop.h"

#endif //_THREADS_COUNTER_H_
