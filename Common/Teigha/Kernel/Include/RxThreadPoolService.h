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

#ifndef _ODRXTHREADPOOLSERVICE_INCLUDED_
#define _ODRXTHREADPOOLSERVICE_INCLUDED_ /* { Secret } **/

#include "TD_PackPush.h"

#include "RxModule.h"
#include "ThreadsCounter.h"
#include "OdArray.h"
#include "StaticRxObject.h"

#include <algorithm>

class OdRxThreadPoolService;

typedef ptrdiff_t OdApcParamType;

typedef void (*OdApcEntryPointVoidParam)( OdApcParamType parameter );

typedef void (*OdApcEntryPointRxObjParam)( OdRxObject* parameter );

/** \details
    This interface represents APC thread object.
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
*/
class ODRX_ABSTRACT OdApcThread : public OdRxObject {
public:
  /** \details
      Execute atomic portion of code encapsulated as OdApcEntryPointVoidParam and returns immediately.
      \param ep [in]  Function pointer to execute.
      \param parameter [in]  Argument which will be passed into executed function.
  */
  virtual void asyncProcCall( OdApcEntryPointVoidParam ep, OdApcParamType parameter ) = 0;

  /** \details
      Execute atomic portion of code encapsulated as OdApcEntryPointRxObjParam and returns immediately.
      \param ep [in]  Function pointer to execute.
      \param parameter [in]  Argument which will be passed into executed function.
  */
  virtual void asyncProcCall( OdApcEntryPointRxObjParam ep, OdRxObject* parameter ) = 0;

  /** \details
      Waits for completion of function execution in the current thread.
      \param bNoThrow [in]  Disable exceptions throwing from inside this method.
  */
  virtual void wait(bool bNoThrow = false) = 0;

  /** \details
      Returns thread's unique system identifier.
  */
  virtual unsigned int getId() const = 0;

  /** \details
      Returns true in case if thread catched exception during execution.
  */
  virtual bool hasException() const = 0;

  /** \details
      Process exception, catched during thread exection.
      \param bReThrow [in]  Throw exception (if it is exists) from inside this method.
      \param bClear [in]  Reset accumulated exception.
  */
  virtual void processException(bool bReThrow = true, bool bClear = true) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class template for OdApcThread object pointers.
*/
typedef OdSmartPtr<OdApcThread> OdApcThreadPtr;


/** \details
    This interface represents client's atomic portion of code to execute via asynchronous call.
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
*/
class ODRX_ABSTRACT OdApcAtom : public OdRxObject {
public:
  /** \details
      Entry point for parallel thread.
  */
  virtual void apcEntryPoint( OdRxObject* pMessage ) {}

  /** \details
      Entry point for parallel thread.
  */
  virtual void apcEntryPoint( OdApcParamType pMessage ) {}
};

/** \details
    This template class is a specialization of the OdSmartPtr class template for OdRxApcQueue object pointers.
*/
typedef OdSmartPtr<OdApcAtom> OdApcAtomPtr;


/** \details
    This interface represents APC object pool.
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
*/
class OdApcObjectPool : public OdRxObject {
public:
  /** \details
      Reserve specified number of objects in object pool.
  */
  virtual void reserve( OdUInt32 n ) = 0;

  /** \details
      Take object from object pool.
  */
  virtual OdRxObjectPtr take() = 0;

  /** \details
      Put object into object pool.
  */
  virtual void put( OdRxObject* pObj ) = 0;
};

typedef OdSmartPtr<OdApcObjectPool> OdApcObjectPoolPtr;


class OdApcObjectPool;

/** \details
    This interface represents APC framework queue.
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
*/
class ODRX_ABSTRACT OdApcQueue : public OdRxObject {
public:
  /** \details
      Returns reference to the threading framework.
  */
  virtual OdRxThreadPoolService& framework() = 0;

  /** \details
      Sets reference to helper atom pool object, where every atom object is being put after APC call is returned.
  */
  virtual void setAtomPoolRef( OdApcObjectPool* pAtomPool ) = 0;

  /** \details
      Adds atomic portion of code encapsulated in OdRxAtom to queue and returns immediately.
  */
  virtual void addEntryPoint( OdApcAtom* pRecipient, OdRxObject* pMessage = 0 ) = 0;

  /** \details
      Adds atomic portion of code encapsulated in OdRxAtom to queue and returns immediately.
  */
  virtual void addEntryPoint( OdApcAtom* pRecipient, OdApcParamType pMessage ) = 0;

  /** \details
      Waits until all OdApcAtom objects in main and synchronized queues are processed.
      \remarks
      Calling thread also utilized to process queued asynchronous calls.
  */
  virtual void wait() = 0;

  /** \details
      Execute action in the main thread (doesn't depend on multithread queues count).
      \param mtFunc [in]  Function to execute in the main thread.
      \param pArg [in]  Function argument to execute in the main thread.
      \remarks
      Simply redirects call into threading framework.
  */
  virtual void executeMainThreadAction( MainThreadFunc mtFunc, void *pArg ) = 0;

  /** \details
      Returns number of threads in the queue.
  */
  virtual int numThreads() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class template for OdApcQueue object pointers.
*/
typedef OdSmartPtr<OdApcQueue> OdApcQueuePtr;


/** \details
    This interface represents APC framework gateway.
    It provides functionality used for protecting data that
    usually is being accessed by many reading threads and few writing threads.
    It allows access to protecting data simultaneously by reading threads
    until no thread gains write access.
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
*/
class OdApcDataReadWriteDispatcher : public OdRxObject {
public:
  /** \details
      A calling thread waits until area is unlocked and enters it to read only.
  */
  virtual void enter() = 0;

  /** \details
      A calling thread leaves protected area.
  */
  virtual void leave() = 0;

  /** \details
      1. Locks the entrance.
      2. Waits until all threads exit read state.
      3. Returns.
      \remarks
      IMPORTANT: If calling thread calls enter() without leave(), it should call lockFromInside() to be not deadlocked.
  */
  virtual void lock() = 0;

  /** \details
      Unlocks the entrance.
  */
  virtual void unlock() = 0;

  /** \details
      Locks the entrance while being in read state.
      \remarks
      To unlock protected area, the calling thread may either call unlock() or unlockFromInside() to stay in read state.
  */
  virtual void lockFromInside() = 0;

  /** \details
      Unlocks the entrance while staying in read state.
  */
  virtual void unlockFromInside() = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class template for OdApcDataReadWriteDispatcher object pointers.
*/
typedef OdSmartPtr<OdApcDataReadWriteDispatcher> OdApcDataReadWriteDispatcherPtr;


/** \details
    This interface represents APC framework event.
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
*/
class ODRX_ABSTRACT OdApcEvent : public OdRxObject {
public:
  /** \details
      Sets the event object to the signaled state.
  */
  virtual void set() = 0;

  /** \details
      Sets the event object to the nonsignaled state.
  */
  virtual void reset() = 0;

  /** \details
      Waits until event object doesn't set to the signaled state.
  */
  virtual void wait() = 0;

  /** \details
      Waits until event object doesn't set to the signaled state and sets it to the nonsignaled state.
  */
  virtual void waitAndReset() = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class template for OdApcEvent object pointers.
*/
typedef OdSmartPtr<OdApcEvent> OdApcEventPtr;


/** \details
    This interface represents APC framework check point.
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
*/
class ODRX_ABSTRACT OdApcGateway : public OdRxObject {
public:
  /** \details
      Controlled thread locks while passing through check point.
      When specified number of controlled threads enters sync point, event is triggered.
      See OdApcSyncPoint::wait().
  */
  virtual void lockByMain( OdUInt32 numThreads ) = 0;

  /** \details
      Controlled thread waits until number of threads specified by last call to lock() enters sync point.
  */
  virtual void waitByMain() = 0;

  /** \details
      Controlled thread unlocks while passing through sync point.
  */
  virtual void unlockByMain() = 0;

  ///////////////////////////////////////////////////////////////////////////////
  /** \details
      Controlled thread enters sync point by calling this method.
      Controlled thread is blocked inside this method until controlling thread calls unlock().
  */
  virtual void passBySecondary() = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class template for OdApcGateway object pointers.
*/
typedef OdSmartPtr<OdApcGateway> OdApcGatewayPtr;

/** \details
    This interface represents APC framework looped gateway point.
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
*/
class ODRX_ABSTRACT OdApcLoopedGateway : public OdRxObject {
public:

  /** \details
  */
  virtual void init( OdUInt32 numThreads ) = 0;

  /** \details
      Controlled thread unlocks while passing through sync point.
  */
  virtual void passByMain() = 0;

  /** \details
      Controlled thread locks while passing through check point.
      When specified number of controlled threads enters sync point, event is triggered.
      See OdApcSyncPoint::wait().
  */
  virtual void waitByMain() = 0;

  /** \details
      Controlled thread waits until number of threads specified by last call to lock() enters sync point.
  */
  virtual void passByMainNoWait() = 0;

  ///////////////////////////////////////////////////////////////////////////////
  /** \details
      Controlled thread enters sync point by calling this method.
      Controlled thread is blocked inside this method until controlling thread calls unlock().
  */
  virtual void passBySecondary() = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class template for OdApcLoopedGateway object pointers.
*/
typedef OdSmartPtr<OdApcLoopedGateway> OdApcLoopedGatewayPtr;


/** \details
    Represents set of flags for new APC StQueue construction.
    Corresponding C++ library: TD_Db
    <group OdRx_Classes>
*/
enum OdApcStQueueFlags
{
  kStQueueNoFlags         = 0,        // Empty StQueue flags.

  kStQueueExecByMain      = (1 << 0), // Use main thread for queue tasks execution.
  kStQueueForceTopLevel   = (1 << 1), // Make StQueue top level even if other registered threads already run.

  kStQueueLastFlag        = kStQueueForceTopLevel
};


/** \details
    Represents set of flags for new APC MtQueue construction.
    
    <group OdRx_Classes>
*/
enum OdApcMtQueueFlags
{
  kMtQueueNoFlags         = 0,        // Empty MtQueue flags.

  kMtQueueForceNewThreads = (1 << 0), // Spawn new threads and add them to the pool if there are not enough free threads.
  kMtQueueAllowExecByMain = (1 << 1), // Allow to use main thread for execution if there are no free threads.
  kMtQueueForceTopLevel   = (1 << 2), // Make MtQueue top level even if other registered threads already run.

  kMtQueueLastFlag        = kMtQueueForceTopLevel
};


/** \details
    
    <group OdRx_Classes>
*/
class FIRSTDLL_EXPORT ODRX_ABSTRACT OdRxThreadPoolService : public OdRxModule {
public:
  ODRX_DECLARE_MEMBERS(OdRxThreadPoolService);

  /** \details
      Returns number of logical CPUs installed on this machine.
  */
  virtual int numCPUs() const = 0;

  /** \details
      Returns number of physical CPU cores installed on this machine.
  */
  virtual int numPhysicalCores() const = 0;

  /** \details
      Returns number of all threads in the pool.
  */
  virtual int numThreads() const = 0;

  /** \details
      Returns number of currently available free threads in the pool.
      \remarks
      (numThreads() - numFreeThreads()) - count of currently running threads.
  */
  virtual int numFreeThreads() const = 0;

  /** \details
      Creates APC thread object.
  */
  virtual OdApcThreadPtr newThread() = 0;

  /** \details
      Creates synchronized (single thread) queue.
      \remarks
      Returned queue is processed by one thread at one moment.
  */
  virtual OdApcQueuePtr newSTQueue( unsigned nThreadAttributes = ThreadsCounter::kNoAttributes,
                                    OdUInt32 nFlags = kStQueueNoFlags ) = 0;

  /** \details
      Creates mutiple thread processing queue.
      \param nThreadAttributes [in]  Set of the attributes for the threads to run.
      \param numThreads [in]  Number of threads for this queue.
      \param nFlags [in]  Set of flags for new MtQueue construction.
      \remarks
      If numThreads argument is zero, then the queue uses all available free threads.
  */
  virtual OdApcQueuePtr newMTQueue( unsigned nThreadAttributes = ThreadsCounter::kNoAttributes,
                                    int numThreads = 0, OdUInt32 nFlags = kMtQueueNoFlags ) = 0;

  /** \details
      Creates APC object pool object.
  */
  virtual OdApcObjectPoolPtr newObjectPool() = 0;

  /** \details
      Creates APC event object.
  */
  virtual OdApcEventPtr newEvent() = 0;

  /** \details
      Creates APC gateway object.
  */
  virtual OdApcGatewayPtr newGateway() = 0;

  /** \details
      Creates APC sync point object.
  */
  virtual OdApcLoopedGatewayPtr newLoopedGateway() = 0;

  /** \details
      Execute action in the main thread (doesn't depend on multithread queues count).
      \param mtFunc [in]  Function to execute in the main thread.
      \param pArg [in]  Function argument to execute in the main thread.
  */
  virtual void executeMainThreadAction( MainThreadFunc mtFunc, void *pArg ) = 0;

  /* Support for external threads manager */

  /** \details
      Register externally executed threads.
      \param nThreads [in]  Count of threads in aThreads array.
      \param aThreads [in]  Thread ID's array.
      \param nThreadAttributes [in]  Set of attributes for running threads.
      \remarks
      If application starts own threads, uses system threads, or executes threads accessed by newThread()
      without using Mt queue. it must register ID's of these threads to enable library Mt mode, initialization
      of local heaps, etc.
  */
  virtual void registerExternalThreads( unsigned nThreads, const unsigned* aThreads, unsigned nThreadAttribs = ThreadsCounter::kNoAttributes ) = 0;
  /** \details
      Unregister externally executed threads.
      \param nThreads [in]  Count of threads in aThreads array.
      \param aThreads [in]  Thread ID's array.
      \remarks
      If application starts own threads, uses system threads, or executes threads accessed by newThread()
      without using Mt queue, it must register ID's of these threads to enable library Mt mode, initialization
      of local heaps and etc.
  */
  virtual void unregisterExternalThreads( unsigned nThreads, const unsigned* aThreads ) = 0;
  /** \details
      Inform Teigha that external thread started.
  */
  virtual void externalThreadStart() = 0;
  /** \details
      Inform Teigha that external thread stopped.
  */
  virtual void externalThreadStop() = 0;
  /** \details
      Setup function for provide execution in the external main thread.
      \param execFunc [in]  Main thread execution function.
  */
  virtual void setExternalMainThreadFunc( ExecuteMainThreadFunc execFunc ) = 0;
  /** \details
      Returns current function for execution in the external main thread.
  */
  virtual ExecuteMainThreadFunc getExternalMainThreadFunc() const = 0;
};

typedef OdSmartPtr<OdRxThreadPoolService> OdRxThreadPoolServicePtr;


/** \details
    
    <group OdApc_Classes>
*/
class OdApcQueueHelper : public OdSmartPtr<OdApcQueue> {
public:
  OdApcQueueHelper( ) {}
  OdApcQueueHelper( const OdApcQueue* pObject, OdRxObjMod m ) : OdSmartPtr<OdApcQueue>( pObject, m ) {}
  OdApcQueueHelper( const OdApcQueue* pObject ) : OdSmartPtr<OdApcQueue>( pObject ) {}
  OdApcQueueHelper( const OdRxObject* pObject ) : OdSmartPtr<OdApcQueue>( pObject ) {}
  OdApcQueueHelper( OdRxObject* pObject, OdRxObjMod m ) : OdSmartPtr<OdApcQueue>( pObject, m ) {}
  OdApcQueueHelper( const OdSmartPtr<OdApcQueue>& pObject ) : OdSmartPtr<OdApcQueue>( pObject ) {}
  OdApcQueueHelper( const OdRxObjectPtr& pObject ) : OdSmartPtr<OdApcQueue>( pObject ) {}
  OdApcQueueHelper( const OdBaseObjectPtr& pObject ) : OdSmartPtr<OdApcQueue>( pObject ) {}

  using OdSmartPtr<OdApcQueue>::operator =;

  void initST( OdRxThreadPoolService* pThreadPool ) {
    *this = pThreadPool ? pThreadPool->newSTQueue().get() : 0;
  }

  void initMT( OdRxThreadPoolService* pThreadPool ) {
    *this = pThreadPool ? pThreadPool->newMTQueue().get() : 0;
  }

  void call( OdApcAtom* pAction, OdRxObject* pParam = 0 ) {
    if( m_pObject ) {
      get()->addEntryPoint( pAction, pParam );
    }
    else {
      pAction->apcEntryPoint( pParam );
    }
  }

  void call( OdApcAtom* pAction, OdApcParamType param ) {
    if( m_pObject ) {
      get()->addEntryPoint( pAction, param );
    }
    else {
      pAction->apcEntryPoint( param );
    }
  }

  void setAtomPoolRef( OdApcObjectPool* pAtomPool ) {
    if( m_pObject ) {
      get()->setAtomPoolRef( pAtomPool );
    }
  }

  void wait() {
    if( m_pObject ) {
      get()->wait();
    }
  }
};


/** \details
    Corresponding C++ library: TD_Db
    <group OdApc_Classes>
class OdApcDataReadWriteDispatcherHelper : public OdSmartPtr<OdApcDataReadWriteDispatcher> {
public:
  OdApcDataReadWriteDispatcherHelper( ) {}
  OdApcDataReadWriteDispatcherHelper( const OdApcDataReadWriteDispatcher* pObject, OdRxObjMod m ) : OdSmartPtr<OdApcDataReadWriteDispatcher>( pObject, m ) {}
  OdApcDataReadWriteDispatcherHelper( const OdApcDataReadWriteDispatcher* pObject ) : OdSmartPtr<OdApcDataReadWriteDispatcher>( pObject ) {}
  OdApcDataReadWriteDispatcherHelper( const OdRxObject* pObject ) : OdSmartPtr<OdApcDataReadWriteDispatcher>( pObject ) {}
  OdApcDataReadWriteDispatcherHelper( OdRxObject* pObject, OdRxObjMod m ) : OdSmartPtr<OdApcDataReadWriteDispatcher>( pObject, m ) {}
  OdApcDataReadWriteDispatcherHelper( const OdSmartPtr<OdApcDataReadWriteDispatcher>& pObject ) : OdSmartPtr<OdApcDataReadWriteDispatcher>( pObject ) {}
  OdApcDataReadWriteDispatcherHelper( const OdRxObjectPtr& pObject ) : OdSmartPtr<OdApcDataReadWriteDispatcher>( pObject ) {}
  OdApcDataReadWriteDispatcherHelper( const OdBaseObjectPtr& pObject ) : OdSmartPtr<OdApcDataReadWriteDispatcher>( pObject ) {}

  using OdSmartPtr<OdApcDataReadWriteDispatcher>::operator =;

  void init( OdRxThreadPoolService* pThreadPool ) {
    *this = pThreadPool ? pThreadPool->newDataReadWriteDispatcher().get() : 0;
  }

  void enter() {
    if( m_pObject ) {
      get()->enter();
    }
  }

  void leave() {
    if( m_pObject ) {
      get()->leave();
    }
  }

  void lock() {
    if( m_pObject ) {
      get()->lock();
    }
  }

  void unlock() {
    if( m_pObject ) {
      get()->unlock();
    }
  }

  void lockFromInside() {
    if( m_pObject ) {
      get()->lockFromInside();
    }
  }

  void unlockFromInside() {
    if( m_pObject ) {
      get()->unlockFromInside();
    }
  }
};

class OdReadWriteDispatcherAutoPass {
public:
  OdReadWriteDispatcherAutoPass( OdApcDataReadWriteDispatcherHelper& lock ) : m_lock( lock ) { m_lock.enter(); }
  ~OdReadWriteDispatcherAutoPass() { m_lock.leave(); }
private:
  OdApcDataReadWriteDispatcherHelper& m_lock;
};

class OdReadWriteDispatcherAutoLock {
public:
  OdReadWriteDispatcherAutoLock( OdApcDataReadWriteDispatcherHelper& lock ) : m_lock( lock ) { m_lock.lock(); }
  ~OdReadWriteDispatcherAutoLock() { m_lock.unlock(); }
private:
  OdApcDataReadWriteDispatcherHelper& m_lock;
};

class OdReadWriteDispatcherAutoLockFromInside {
public:
  OdReadWriteDispatcherAutoLockFromInside( OdApcDataReadWriteDispatcherHelper& lock ) : m_lock( lock ) { m_lock.lockFromInside(); }
  ~OdReadWriteDispatcherAutoLockFromInside() { m_lock.unlockFromInside(); }
private:
  OdApcDataReadWriteDispatcherHelper& m_lock;
};
*/


/** \details
    <group OdApc_Classes>
*/
class OdApcQueueHelperArray : public OdArray<OdApcQueueHelper> {
public:
  void initST( int n, OdRxThreadPoolService* pTP ) {
    resize( n );
    if ( pTP ) {
      for( int i=0; i<n; ++i ) {
        setAt( i, pTP->newSTQueue() );
      }
    }
  }
  void wait() {
    size_type n = size();
    for( size_type i=0; i<n; ++i ) {
      at( i ).wait();
    }
  }
};


/** \details
    Corresponding C++ library: ThreadPool
    <group OdApc_Classes>
*/
class ODRX_ABSTRACT OdAsyncForEachBase : public OdApcAtom {
  void* sync_next( OdMutex* mutex, OdUInt32 threadIndex, OdUInt32& itemIndex ) {
    OdMutexAutoLock lock( *mutex );
    return next( threadIndex, itemIndex );
  }
protected:
  virtual OdMutex* mutexForNext() = 0;

  virtual void* next( OdUInt32 threadIndex, OdUInt32& itemIndex ) = 0;

  virtual void doAction( OdUInt32 threadIndex, OdUInt32 itemIndex, void* pItem ) = 0;

  virtual void apcEntryPoint( OdApcParamType threadIndex ) {
    OdMutex* mutex = mutexForNext();
    OdUInt32 itemIndex;
    while( void* pItem = sync_next( mutex, (OdUInt32)threadIndex, itemIndex ) ) {
      doAction( (OdUInt32)threadIndex, itemIndex, pItem );
    }
  }
public:
  void for_each( OdApcQueue* pQueue, OdUInt32 nThreads = 0 ) {
    if ( pQueue!=0 ) {
      OdUInt32 n = nThreads==0 ? pQueue->framework().numCPUs() : nThreads;
      if( n > 1 ) {
        while( --n ) {
          pQueue->addEntryPoint( this, (OdApcParamType)n );
        }
        apcEntryPoint( (OdApcParamType)n );
        pQueue->wait();
        return;
      }
    }
    apcEntryPoint( 0 );
  }
};

/** <group OdApc_Classes>
*/
template< class It, class Fn >
class OdAsyncForEach : public OdStaticRxObject<OdApcAtom> {
  OdMutex                 m_mutex;
  It                      m_cur, m_last;
  Fn                      m_fn;

  bool next( It& cur ) {
    OdMutexAutoLock lock( m_mutex );
    if ( m_cur < m_last ) {
      cur = m_cur;
      ++m_cur;
      return true;
    }
    return false;
  }

  void apcEntryPoint( OdRxObject* ) {
    It cur;
    if( next( cur ) ) {
      do {
        m_fn( *cur );
      }
      while( next( cur ) );
    }
  }
public:
  void for_each( OdApcQueue* pQueue, It first, It last, Fn fn ) {
    unsigned long n = last-first;
    if( pQueue && n>1 ) {
      m_cur = first;
      m_last = last;
      m_fn = fn;

      unsigned long numCPUs = pQueue->framework().numCPUs();
      n = ( n < numCPUs ) ? n : numCPUs;
      while( n-- ) {
        pQueue->addEntryPoint( this );
      }
      pQueue->wait();
    }
    else {
      std::for_each( first, last, fn );
    }
  }

};

template< class It, class Fn >
void od_async_for_each( OdApcQueue* pQueue, It first, It last, Fn fn ) {
  ODA_ASSERT_ONCE( first <= last );
  if( first < last ) {
    OdAsyncForEach< It, Fn >().for_each( pQueue, first, last, fn );
  }
}
/** <group !!RECORDS_tkernel_apiref>
*/
template< class TObject >
struct OdApcObjectPoolHelperDummyInitFn {
  void operator () ( TObject& ) const {
    // does nothing
  }
};

/** <group OdApc_Classes>
*/
template< class TObject, class TInitFn = OdApcObjectPoolHelperDummyInitFn<TObject> >
class OdApcObjectPoolHelper : public OdApcObjectPoolPtr {
  TObject* m_pEntries;
  OdUInt32 m_nCount;
public:
  OdApcObjectPoolHelper() : m_pEntries( NULL ), m_nCount(0) {}
  ~OdApcObjectPoolHelper() { release(); ::delete [] m_pEntries; m_pEntries = NULL; m_nCount=0; }
  void init( OdRxThreadPoolService* pTP, int n = 0, const TInitFn* pInitFn = 0 ) {
    if ( pTP ) {
      if( n==0 ) {
        n = pTP->numCPUs();
      }
      m_nCount = n;
      m_pEntries = ::new TObject[n];
      (*(OdApcObjectPoolPtr*)this) = pTP->newObjectPool();
      while( --n ) {
        TObject& obj = m_pEntries[ n ];
        if( pInitFn ) {
          (*pInitFn)( obj );
        }
        get()->put( &obj );
      }
    }
    else {
      m_nCount = 1;
      m_pEntries = ::new TObject[1];
      if( pInitFn ) {
        (*pInitFn)( *m_pEntries );
      }
    }
  }

  TObject* take() {
    if( m_pObject ) {
      return static_cast<TObject*>( get()->take().get() );
    }
    return m_pEntries;
  }

  OdUInt32 size() const {
    return m_nCount;
  }

  TObject& at( OdUInt32 i ) const {
    if( i < m_nCount ) {
      return m_pEntries[i];
    }
    throw OdError_InvalidIndex();
  }

  operator OdApcObjectPool* () { return get(); }
  operator const OdApcObjectPool* () const { return get(); }
};

#include "TD_PackPop.h"

#endif //_ODRXTHREADPOOLSERVICE_INCLUDED_
