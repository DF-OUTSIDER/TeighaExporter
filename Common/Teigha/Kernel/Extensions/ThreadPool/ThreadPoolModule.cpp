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

#include "OdaCommon.h"
#include "RxThreadPoolService.h"
#include "StaticRxObject.h"
#include "UInt8Array.h"
#include "RxDynamicModule.h"
#include "RxObjectImpl.h"
#include "SaveState.h"
#include "ThreadsCounter.h"
#include "OdVector.h"
#include "SharedPtr.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include <queue>
#ifdef _WIN32
#include <process.h>
#endif

// Section of platform-dependent wrappers. Probably move into separate file(s)?

#ifdef OD_POSIX_THREADS

#if defined(ODA_WINDOWS)
// We supports pthreads-win32
#define OD_WIN_PTHREADS 1
#endif

// *nix POSIX threads. Or pthread-win32.
#include <pthread.h>
#ifndef OD_WIN_PTHREADS
// For sysconf declaration
#include <unistd.h>
#endif

namespace OdTP
{

// Events

// Define type of event handle
struct EventHandle {
  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;
  bool m_bSignaled;
};

// Create event function
inline void createEvent(EventHandle &handle, bool bSignaled = true) {
  ::pthread_mutex_init(&handle.m_mutex, NULL);
  ::pthread_cond_init(&handle.m_cond, NULL);
  handle.m_bSignaled = bSignaled;
}
// Close event function
inline void closeEvent(EventHandle &hEvent) {
  ::pthread_mutex_destroy(&hEvent.m_mutex);
  ::pthread_cond_destroy(&hEvent.m_cond);
#ifdef OD_WIN_PTHREADS
  hEvent.m_mutex = 0; hEvent.m_cond = 0;
#endif
  hEvent.m_bSignaled = false;
}
// Set event to signaled state function
inline bool setEvent(EventHandle &hEvent) {
  ::pthread_mutex_lock(&hEvent.m_mutex);
  hEvent.m_bSignaled = true;
  int rVal = ::pthread_cond_broadcast(&hEvent.m_cond);
  ::pthread_mutex_unlock(&hEvent.m_mutex);
  return !rVal;
}
// Set event to non-signaled state function
inline bool resetEvent(EventHandle &hEvent) {
  ::pthread_mutex_lock(&hEvent.m_mutex);
  hEvent.m_bSignaled = false;
  ::pthread_mutex_unlock(&hEvent.m_mutex);
  return true;
}
// Wait for event completion function
inline bool waitEvent(EventHandle &hEvent) {
  ::pthread_mutex_lock(&hEvent.m_mutex);
  while (!hEvent.m_bSignaled)
    ::pthread_cond_wait(&hEvent.m_cond, &hEvent.m_mutex);
  ::pthread_mutex_unlock(&hEvent.m_mutex);
  return true;
}
// Alterable wait for event completion function
inline void alterableWaitEvent(EventHandle &hEvent) {
  /* pthreads haven't alterable wait function. But it normally works with simple condition wait. */
  waitEvent(hEvent);
}

// Threads

// Define thread ID type
#ifndef OD_WIN_PTHREADS
typedef pthread_t ThreadID;
#else
typedef unsigned int ThreadID;
#endif

// Define thread handle
typedef pthread_t ThreadHandle;

// Define return thread function type
typedef void *ThreadFuncRVal;

// Define thread function argument type
typedef void *ThreadFuncArg;

// Thread func calling modifier
#define OD_TP_TFUNCMODIFIER

#ifdef ANDROID
static void thread_exit_handler(int sig)
{
  ::pthread_exit(0);
}
static void installThreadAction()
{
  struct sigaction actions;
  memset(&actions, 0, sizeof(actions));
  sigemptyset(&actions.sa_mask);
  actions.sa_flags = 0;
  actions.sa_handler = thread_exit_handler;
  sigaction(SIGUSR2, &actions, NULL);
}
#endif

// Thread creation function
inline ThreadHandle createThread(ThreadFuncRVal (OD_TP_TFUNCMODIFIER *pThreadProc)(ThreadFuncArg),
                                 ThreadFuncArg pArg, ThreadID &id) {
  ThreadHandle handle;
  pthread_attr_t attr;
  ::pthread_attr_init(&attr);
  ::pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  ::pthread_create(&handle, &attr, pThreadProc, pArg);
  ::pthread_attr_destroy(&attr);
#ifndef OD_WIN_PTHREADS
  id = handle;
#else
  id = (unsigned int)::pthread_getw32threadid_np(handle);
#endif
#ifdef ANDROID
  installThreadAction();
#endif
  return handle;
}
// Close thread function
inline void closeThread(ThreadHandle &hThread) {
  // We stop thread (if it is still wait something) and destroy because thread pool doesn't require
  // to reuse it any more. This is solve memory leaks problem.
#ifndef ANDROID
  ::pthread_cancel(hThread);
#else
  // Used SIGUSR2 because Dalvik VM could invoke SIGUSR1
  ::pthread_kill(hThread, SIGUSR2);
#endif
  ::pthread_detach(hThread);
}
// Current thread ID function
inline ThreadID currentThreadId() {
#ifndef OD_WIN_PTHREADS
  return ::pthread_self();
#else
  return (unsigned int)::pthread_getw32threadid_np(::pthread_self());
#endif
}

#ifndef OD_WIN_PTHREADS
#define OD_TP_INVALID_HANDLE_VALUE 0
#if !defined(__APPLE__)
#define OD_TP_ID_TO_UINT(id) (unsigned int)id
#else
#define OD_TP_ID_TO_UINT(id) (unsigned int)((OdIntPtr)(id))
#endif
#else
static const ptw32_handle_t OD_TP_INVALID_HANDLE_VALUE = { 0, 0 };
bool operator ==(const ptw32_handle_t& t1, const ptw32_handle_t& t2) {
  return t1.p == t2.p;
}
bool operator !=(const ptw32_handle_t& t1, const ptw32_handle_t& t2) {
  return t1.p != t2.p;
}
#define OD_TP_ID_TO_UINT(id) id
#endif

// Processors count detection

static int numCPUs(int &nPhysicalCores)
{
#ifndef OD_WIN_PTHREADS
  return nPhysicalCores = (int)::sysconf(_SC_NPROCESSORS_CONF);
#else
  return nPhysicalCores = ::pthread_num_processors_np();
#endif
}

} // namespace OdTP

#ifdef OD_WIN_PTHREADS
using OdTP::OD_TP_INVALID_HANDLE_VALUE;
using OdTP::operator ==;
using OdTP::operator !=;
#endif

#elif defined(ODA_WINDOWS)

// Windows threads

#if _MSC_VER == 1200

#if defined(_WIN64)
    typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;
#else
    typedef unsigned long ULONG_PTR, *PULONG_PTR;
#endif

typedef enum _LOGICAL_PROCESSOR_RELATIONSHIP {
    RelationProcessorCore,
    RelationNumaNode,
    RelationCache
} LOGICAL_PROCESSOR_RELATIONSHIP;

typedef enum _PROCESSOR_CACHE_TYPE {
    CacheUnified,
    CacheInstruction,
    CacheData,
    CacheTrace
} PROCESSOR_CACHE_TYPE;

typedef struct _CACHE_DESCRIPTOR {
    BYTE   Level;
    BYTE   Associativity;
    WORD   LineSize;
    DWORD  Size;
    PROCESSOR_CACHE_TYPE Type;
} CACHE_DESCRIPTOR, *PCACHE_DESCRIPTOR;

typedef struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION {
    ULONG_PTR   ProcessorMask;
    LOGICAL_PROCESSOR_RELATIONSHIP Relationship;
    union {
        struct {
            BYTE  Flags;
        } ProcessorCore;
        struct {
            DWORD NodeNumber;
        } NumaNode;
        CACHE_DESCRIPTOR Cache;
        ULONGLONG  Reserved[2];
    };
} SYSTEM_LOGICAL_PROCESSOR_INFORMATION, *PSYSTEM_LOGICAL_PROCESSOR_INFORMATION;

#define STACK_SIZE_PARAM_IS_A_RESERVATION 0x00010000

#endif

namespace OdTP
{

// Events

// Define type of event handle
typedef HANDLE EventHandle;

// Create event function
inline void createEvent(EventHandle &handle, bool bSignaled = true) {
  handle = ::CreateEvent( NULL, TRUE, (bSignaled) ? TRUE : FALSE, NULL );
}
// Close event function
inline void closeEvent(EventHandle hEvent) {
  ::CloseHandle( hEvent );
}
// Set event to signaled state function
inline bool setEvent(EventHandle hEvent) {
  return ::SetEvent( hEvent ) != FALSE;
}
// Set event to non-signaled state function
inline bool resetEvent(EventHandle hEvent) {
  return ::ResetEvent( hEvent ) != FALSE;
}
// Wait for event completion function
inline bool waitEvent(EventHandle hEvent) {
  return ::WaitForSingleObject( hEvent, INFINITE )==WAIT_OBJECT_0;
}
// Alterable wait for event completion function
inline void alterableWaitEvent(EventHandle hEvent) {
  ::WaitForSingleObjectEx( hEvent, INFINITE, TRUE );
}

// Threads

#if defined(_MSC_VER) && defined(_MT)
// Little bit differs from WinAPI threads
#define MSVC_CLR_THREADS 1
#endif

// Define thread ID type
#ifdef MSVC_CLR_THREADS
typedef unsigned int ThreadID;
#else
typedef DWORD ThreadID;
#endif

// Define thread handle
#ifdef MSVC_CLR_THREADS
typedef uintptr_t ThreadHandle;
#else
typedef HANDLE ThreadHandle;
#endif

// Define return thread function type
#ifdef MSVC_CLR_THREADS
typedef unsigned int ThreadFuncRVal;
#else
typedef DWORD ThreadFuncRVal;
#endif

// Define thread function argument type
#ifdef MSVC_CLR_THREADS
typedef void *ThreadFuncArg;
#else
typedef LPVOID ThreadFuncArg;
#endif

// Thread func calling modifier
#define OD_TP_TFUNCMODIFIER WINAPI

// Thread creation function
inline ThreadHandle createThread(ThreadFuncRVal (OD_TP_TFUNCMODIFIER *pThreadProc)(ThreadFuncArg),
                                 ThreadFuncArg pArg, ThreadID &id) {
#ifdef MSVC_CLR_THREADS
  return ::_beginthreadex( 0, 0, pThreadProc, pArg, 0, &id );
#else
  return ::CreateThread( 0, 1024*1000, pThreadProc, pArg, STACK_SIZE_PARAM_IS_A_RESERVATION, &id );
#endif
}
// Close thread function
inline void closeThread(ThreadHandle hThread) {
  ::WaitForSingleObject( (HANDLE) hThread, INFINITE );
  ::CloseHandle( (HANDLE)hThread );
}
// Current thread ID function
inline ThreadID currentThreadId() {
  return (ThreadID)::GetCurrentThreadId();
}

#ifndef MSVC_CLR_THREADS
#define OD_TP_INVALID_HANDLE_VALUE INVALID_HANDLE_VALUE
#else
#define OD_TP_INVALID_HANDLE_VALUE 0
#endif

#define OD_TP_ID_TO_UINT(id) (unsigned int)id

// Wait for multiple objects supported

#define OD_TP_MULTIWAIT

#ifdef OD_TP_MULTIWAIT
class MultiWait {
  OdArray<EventHandle, OdMemoryAllocator<EventHandle> > m_events;
  public:
    explicit MultiWait(OdUInt32 nReserved = 0)
      : m_events(nReserved, 4) { }
    void add(EventHandle nHandle) {
      m_events.push_back(nHandle);
    }
    bool wait() {
      if (m_events.size() > MAXIMUM_WAIT_OBJECTS) {
        OdUInt32 nEvents = m_events.size();
        const EventHandle *pEvents = m_events.getPtr();
        for (OdUInt32 nEvent = 0; nEvent < nEvents; nEvent++)
          if (!waitEvent(pEvents[nEvent])) return false;
      } else if (m_events.isEmpty())
        return true;
      return ::WaitForMultipleObjects( (DWORD)m_events.size(), m_events.getPtr(), TRUE, INFINITE ) == WAIT_OBJECT_0;
    }
};
#endif

// Processors count detection

typedef BOOL (WINAPI *LPFN_GLPI)(
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION,
    PDWORD);

static int numCPUs(int &nPhysicalCores)
{
  LPFN_GLPI Glpi;
  Glpi = (LPFN_GLPI)::GetProcAddress(
    ::GetModuleHandleW(TEXT("kernel32")), "GetLogicalProcessorInformation"
    );
  if ( Glpi == 0 ) {
    return nPhysicalCores=1;
  }

  DWORD returnLength = 0;
  if ( !Glpi(0, &returnLength)) {
    if( ::GetLastError() == ERROR_INSUFFICIENT_BUFFER ) {
      OdUInt8Array buffer(returnLength);
      buffer.resize(returnLength);
      PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pLPI =
        (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)buffer.asArrayPtr();
      if ( Glpi(pLPI, &returnLength)) {
        DWORD byteOffset = 0;
        int nCPUs = 0;
        nPhysicalCores = 0;

        while ( byteOffset < returnLength ) {
          switch (pLPI->Relationship) {
            case RelationProcessorCore:
              ++nPhysicalCores;
              while(pLPI->ProcessorMask) {
                if(pLPI->ProcessorMask & 1)
                  ++nCPUs;
                pLPI->ProcessorMask >>= 1;
              }
              break;
            default:
              break;
          }
          byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
          ++pLPI;
        }
        return nCPUs;
      }
    }
  }
  return nPhysicalCores=1;
}

} // namespace OdTP

#else

// No threads library. This is no sense, but we still could build ThreadPool module.

namespace OdTP
{

// Events

// Define type of event handle
typedef unsigned int EventHandle;

// Create event function
inline void createEvent(EventHandle &handle, bool /*bSignaled*/ = true) {
  handle = 1;
}
// Close event function
inline void closeEvent(EventHandle /*hEvent*/) {
}
// Set event to signaled state function
inline bool setEvent(EventHandle /*hEvent*/) {
  return true;
}
// Set event to non-signaled state function
inline bool resetEvent(EventHandle /*hEvent*/) {
  return true;
}
// Wait for event completion function
inline bool waitEvent(EventHandle /*hEvent*/) {
  return true;
}
// Alterable wait for event completion function
inline void alterableWaitEvent(EventHandle /*hEvent*/) {
}

// Threads

// Define thread ID type
typedef unsigned int ThreadID;

// Define thread handle
typedef unsigned int ThreadHandle;

// Define return thread function type
typedef unsigned int ThreadFuncRVal;

// Define thread function argument type
typedef void *ThreadFuncArg;

// Thread func calling modifier
#define OD_TP_TFUNCMODIFIER

// Thread creation function
inline ThreadHandle createThread(ThreadFuncRVal (OD_TP_TFUNCMODIFIER *pThreadProc)(ThreadFuncArg),
                                 ThreadFuncArg pArg, ThreadID &id) {
  pThreadProc(pArg);
  return id = 1;
}
// Close thread function
inline void closeThread(ThreadHandle /*hThread*/) {
}
// Current thread ID function
inline ThreadID currentThreadId() {
  return 1;
}

#define OD_TP_INVALID_HANDLE_VALUE 0

#define OD_TP_ID_TO_UINT(id) id

// Processors count detection

static int numCPUs(int &nPhysicalCores)
{
  return nPhysicalCores = 1;
}

} // namespace OdTP

#endif

// Exceptions handler

#include <exception>
#if ((__cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1900)) && !defined(ANDROID)
namespace OdTP
{

class ExceptionHandler
{
  protected:
    std::exception_ptr m_pException;
  public:
    ExceptionHandler() {}
    void holdException(const OdError &odError) { m_pException = std::make_exception_ptr(odError); }
    void holdException(const std::exception & /*stdError*/) { holdException(); /* run through default holder, elsewhere we will lost exception inheritance. */ }
    void holdException() { m_pException = std::current_exception(); }
    bool hasException() const { return (bool)m_pException; }
    void processException(bool bReThrow = true, bool bClear = true)
    {
      if (m_pException)
      {
        if (bClear)
        {
          std::exception_ptr pException = m_pException;
          m_pException = nullptr;
          if (bReThrow)
            std::rethrow_exception(pException);
        }
        else if (bReThrow)
          std::rethrow_exception(m_pException);
      }
    }
};

} // namespace OdTP
#else
#include "OdErrorContext.h"
namespace OdTP
{

class ExceptionHandler
{
  protected:
    class StdErrorCtx : public OdErrorContext
    {
      protected:
        OdAnsiString m_errorData;
      public:
        StdErrorCtx() { }
        OdString description() const { return m_errorData; }
        OdResult code() const { return eExtendedError; }
        StdErrorCtx *setError(const char *pErrorDesc) { m_errorData = pErrorDesc; return this; }
    };
  protected:
    OdError m_exception;
  public:
    ExceptionHandler() : m_exception((OdErrorContext*)NULL) {}
    void holdException(const OdError &odError) { m_exception = odError; }
    void holdException(const std::exception & stdError) {
      m_exception.setContext(OdRxObjectImpl<StdErrorCtx>::createObject()->setError(stdError.what()));
    }
    void holdException() { m_exception = OdError(eExtendedError); }
    bool hasException() const { return !!m_exception.context(); }
    void processException(bool bReThrow = true, bool bClear = true)
    {
      if (m_exception.context())
      {
        if (bClear)
        {
          OdError error = m_exception;
          m_exception.setContext(NULL);
          if (bReThrow)
            throw error;
        }
        else if (bReThrow)
          throw m_exception;
      }
    }
};

} // namespace OdTP
#endif

// Thread function arguments conversion

namespace OdTP
{

union FcnCast {
  void *m_pVoid;
  OdApcEntryPointRxObjParam m_pFcnObjParam;
  OdApcEntryPointVoidParam m_pFcnVoidParam;

  static inline void *toVoid(OdApcEntryPointRxObjParam pFcn)
  {
    FcnCast fcnCast;
    fcnCast.m_pFcnObjParam = pFcn;
    return fcnCast.m_pVoid;
  }
  static inline void *toVoid(OdApcEntryPointVoidParam pFcn)
  {
    FcnCast fcnCast;
    fcnCast.m_pFcnVoidParam = pFcn;
    return fcnCast.m_pVoid;
  }
  static inline OdApcEntryPointRxObjParam toFcnObjParam(void *pVoid)
  {
    FcnCast fcnCast;
    fcnCast.m_pVoid = pVoid;
    return fcnCast.m_pFcnObjParam;
  }
  static inline OdApcEntryPointVoidParam toFcnVoidParam(void *pVoid)
  {
    FcnCast fcnCast;
    fcnCast.m_pVoid = pVoid;
    return fcnCast.m_pFcnVoidParam;
  }
};

} // namespace OdTP

class OdRxThreadPoolImpl : public OdRxThreadPoolService {
public:
  class OdApcEventImpl : public OdApcEvent {
    OdTP::EventHandle    m_event;
    OdApcEventImpl(const OdApcEventImpl&) {}
  public:
    OdApcEventImpl() {
      OdTP::createEvent(m_event);
    }

    ~OdApcEventImpl() {
      OdTP::closeEvent( m_event );
    }

    void set() {
      ODA_VERIFY( OdTP::setEvent( m_event ) );
    }

    void reset() {
      ODA_VERIFY( OdTP::resetEvent( m_event ) );
    }

    void wait() {
      ODA_VERIFY( OdTP::waitEvent( m_event ) );
    }

    void waitAndReset() {
      ODA_VERIFY( OdTP::waitEvent( m_event ) );
      ODA_VERIFY( OdTP::resetEvent( m_event ) );
    }

#ifdef OD_TP_MULTIWAIT
    OdTP::EventHandle getHandle() const { return m_event; }
#endif
  };

  typedef OdStaticRxObject<OdApcEventImpl> OdStaticApcEventImpl;

  struct QueueEntry {
    inline QueueEntry( OdApcAtom* atom, OdRxObject* rxPtrParam, OdApcParamType numberParam ) {
      this->atom = atom;
      this->objParam = rxPtrParam;
      this->numParam = numberParam;
      useRxObjParam = true;
    }

    inline QueueEntry( OdApcAtom* atom, OdApcParamType numParam ) {
      this->atom = atom;
      this->objParam = NULL;
      this->numParam = numParam;
      useRxObjParam = false;
    }

    inline QueueEntry( OdApcAtom* atom, OdRxObject* rxPtrParam ) {
      this->atom = atom;
      this->objParam = rxPtrParam;
      this->numParam = 0;
      useRxObjParam = true;
    }

    inline void asyncCall() {
      if( useRxObjParam )
        asyncCall( objParam );
      else
        asyncCall( numParam );
    }

    inline void asyncCall( OdRxObject* objParam ) {
      atom->apcEntryPoint( objParam );
    }

    inline void asyncCall( OdApcParamType numParam ) {
      atom->apcEntryPoint( numParam );
    }

    inline void clear() {
      atom.release();
      objParam.release();
    }

    inline bool clean() const {
      return atom.isNull();
    }

    OdApcAtomPtr  atom;
    OdRxObjectPtr objParam;
    OdApcParamType numParam;
    bool useRxObjParam;
  };

  class OdApcThreadImpl : public OdRxObjectImpl<OdApcThread> {
    OdRxThreadPoolImpl* frmwk;
    enum {
      kProcIsAtom = 1,
      kParamIsObject = 2
    };
    OdUInt32 flags;
    OdTP::ThreadID id;
    OdTP::ThreadHandle handle;
    OdTP::ExceptionHandler exception;
    OdStaticApcEventImpl ready;
    OdStaticApcEventImpl start;

    void* userProc;
    OdApcParamType userParam;

    static OdTP::ThreadFuncRVal OD_TP_TFUNCMODIFIER threadProc(OdTP::ThreadFuncArg pData) {
      reinterpret_cast<OdApcThreadImpl*>(pData)->threadMain();
      return 0;
    }

    inline OdApcAtom* procAsAtom() {
      ODA_ASSERT( userProc && GETBIT( flags, kProcIsAtom ) ); return reinterpret_cast<OdApcAtom*>( userProc );
    }

    inline OdRxObject* paramAsObject() {
      ODA_ASSERT( GETBIT( flags, kParamIsObject ) ); return reinterpret_cast<OdRxObject*>( userParam );
    }

    inline void asyncCall()
    {
      try
      {
        if (GETBIT(flags, kParamIsObject))
        {
          if (GETBIT(flags, kProcIsAtom))
            procAsAtom()->apcEntryPoint(paramAsObject());
          else
            OdTP::FcnCast::toFcnObjParam(userProc)(paramAsObject());
        }
        else
        {
          if (GETBIT(flags, kProcIsAtom))
            procAsAtom()->apcEntryPoint(userParam);
          else
            OdTP::FcnCast::toFcnVoidParam(userProc)(userParam);
        }
      }
      catch (...)
      {
        releaseAsyncCallObjects();
        throw;
      }
      releaseAsyncCallObjects();
    }
    inline void releaseAsyncCallObjects()
    {
      if (GETBIT(flags, kProcIsAtom))
        procAsAtom()->release();
      if (GETBIT(flags, kParamIsObject) && userParam)
        paramAsObject()->release();
      userProc = NULL;
      userParam = 0;
      release(); // unlock this thread object
    }

    void threadMain() {
      for(;;) {
        start.wait();
        start.reset();

        if( userProc==NULL )
          break;

        try { asyncCall(); }
        catch (const OdError &odError) { exception.holdException(odError); }
        catch (const std::exception &stdError) { exception.holdException(stdError); }
        catch (...) { exception.holdException(); }

        ready.set();
      }
      ready.set();
    }

    inline void asyncProcCall() {
      addRef(); // lock this thread object
      ready.reset();
      start.set();
    }

    OdApcThreadImpl() : userProc( NULL ) {
      flags = 0;
      handle = OD_TP_INVALID_HANDLE_VALUE;
      id = 0;
    }

  public:
    static OdSmartPtr<OdApcThreadImpl> createObject() {
      return OdSmartPtr<OdApcThreadImpl>( new OdApcThreadImpl, kOdRxObjAttach );
    }

    ~OdApcThreadImpl() {
      terminate();
    }

    void execute( OdRxThreadPoolImpl* frmwk ) {
      //ready.set(); // set in event constructor
      start.reset();
      this->frmwk = frmwk;
      handle = OdTP::createThread( threadProc, reinterpret_cast<OdTP::ThreadFuncArg>( this ), id );
    }

    void release() {
      if ( numRefs()==2 ) {
        frmwk->ready( this );
      }
      OdRxObjectImpl<OdApcThread>::release();
    }

    void asyncProcCall( OdApcAtom* atom, OdRxObject* rxPtrParam ) {
      ready.wait(); // Wait till previous operation is finalized completely
      ODA_ASSERT( userProc==NULL );

      userProc = atom;
      atom->addRef();
      userParam = reinterpret_cast<OdApcParamType>( rxPtrParam );
      SETBIT_1( flags, kProcIsAtom|kParamIsObject );

      asyncProcCall();
    }

    void asyncProcCall( OdApcAtom* atom, OdApcParamType numberParam ) {
      ready.wait(); // Wait till previous operation is finalized completely
      ODA_ASSERT( userProc==NULL );

      userProc = atom;
      atom->addRef();
      userParam = numberParam;
      SETBIT_1( flags, kProcIsAtom );
      SETBIT_0( flags, kParamIsObject );

      asyncProcCall();
    }

    void asyncProcCall( OdApcEntryPointRxObjParam ep, OdRxObject* rxPtrParam ) {
      ready.wait(); // Wait till previous operation is finalized completely
      ODA_ASSERT( userProc==NULL );

      SETBIT_0( flags, kProcIsAtom );
      userProc = OdTP::FcnCast::toVoid( ep );
      SETBIT_1( flags, kParamIsObject );
      userParam = reinterpret_cast<OdApcParamType>( rxPtrParam );
      if( rxPtrParam )
        rxPtrParam->addRef();

      asyncProcCall();
    }

    void asyncProcCall( OdApcEntryPointVoidParam ep, OdApcParamType numberParam ) {
      ready.wait(); // Wait till previous operation is finalized completely
      ODA_ASSERT( userProc==NULL );

      SETBIT_0( flags, kProcIsAtom|kParamIsObject );
      userProc = OdTP::FcnCast::toVoid( ep );
      userParam = numberParam;

      asyncProcCall();
    }

    unsigned int getId() const { return OD_TP_ID_TO_UINT(id); }

    void waitNoThrow() {
      ready.wait();
    }
    void wait(bool bNoThrow = false) {
      waitNoThrow();
      if (!bNoThrow)
        exception.processException();
    }

    bool hasException() const {
      return exception.hasException();
    }
    void processException(bool bReThrow = true, bool bClear = true) {
      return exception.processException(bReThrow, bClear);
    }

    void terminate( bool wait = true ) {
      if( handle != OD_TP_INVALID_HANDLE_VALUE ) {
        ready.wait();

        userProc = NULL;
        userParam = 0;
        ready.reset();
        start.set();

        ready.wait();
        OdTP::closeThread( handle );
        handle = OD_TP_INVALID_HANDLE_VALUE;
        id = 0;
      }
    }

#ifdef OD_TP_MULTIWAIT
    OdApcEventImpl *getReadyEvent() { return &ready; }
#endif
  };

  typedef OdSmartPtr<OdApcThreadImpl> OdApcThreadImplPtr;
  // SharedPtr used to prevent additional addRef/release calls which could cause returning thread too pool in some cases.
  typedef OdSharedPtr<OdApcThreadImplPtr> OdApcThreadImplPtrPtr;
  static OdApcThreadImpl *getPtr(OdApcThreadImplPtrPtr &pIn) { return (*pIn); }
  static const OdApcThreadImpl *getPtr(const OdApcThreadImplPtrPtr &pIn) { return (*pIn); }

  typedef OdVector<OdApcThreadImplPtrPtr> OdApcThreadImplPtrVector;
  typedef OdVector<OdApcThreadImpl*, OdMemoryAllocator<OdApcThreadImpl*> > OdApcThreadImplCPtrVector;

  OdApcThreadImplPtrVector  m_threads;
  OdApcThreadImplCPtrVector m_readyThreads;

  mutable int     m_nCPUs;
  mutable int     m_nPhysicalCores;
  mutable OdMutex m_ready;

  // #11743
  OdApcThreadImpl *finalizeThreading() {
    // Wait for all threads completion
    OdUInt32 nThreads = m_threads.size(), nThread;
    OdApcThreadImplPtrPtr *pThreads = m_threads.asArrayPtr();
#ifdef OD_TP_MULTIWAIT
    if (nThreads > 1)
    {
      OdTP::MultiWait mWait(nThreads);
      for (nThread = 0; nThread < nThreads; nThread++)
        mWait.add(getPtr(pThreads[nThread])->getReadyEvent()->getHandle());
      mWait.wait();
    }
    else
#endif
    {
      for (nThread = 0; nThread < nThreads; nThread++)
        getPtr(pThreads[nThread])->waitNoThrow();
    }
    // Handle exceptions
    OdApcThreadImpl *pExceptionThread = NULL;
    for (nThread = 0; nThread < nThreads; nThread++)
    {
      OdApcThreadImpl *pThread = getPtr(pThreads[nThread]);
      if (pThread->hasException())
      {
        if (!pExceptionThread)
          pExceptionThread = pThread;
        else
          pThread->processException(false);
      }
    }
    return pExceptionThread;
  }
public:
  void ready(OdApcThreadImpl* thread) {
    OdMutexAutoLock lock( m_ready );
    if (!m_readyThreads.contains( thread ))
    {
      m_readyThreads.append( thread );
      if (m_pTopLevelTask && m_pTopLevelTask->m_bWaitForFreeThread)
      {
        // If main thread waits for free thread we can here inform that we have it. Of course it is
        // could be grabbed by nested threads, but in this case we will return for waiting again.
        m_pTopLevelTask->m_bFreeThreadAvailable = true;
        m_pTopLevelTask->m_waitHandlerGate.set();
      }
    }
  }

  OdApcThreadImpl* readyThread( bool bRunNew = false ) {
    OdApcThreadImpl* pThread;
    {
      OdMutexAutoLock lock( m_ready );
      if( m_readyThreads.empty() ) {
        if( bRunNew ) {
          // runNewThread automatically adds populated thread into array of ready threads, this is means that
          // secondary call to readyThread will got this thread again. As result application will enter into
          // infinite loop since thread isn't ready actually.
          runNewThread();
          if ( m_readyThreads.empty() )
            return NULL; // Possible if system cannot spawn new thread
        } else
          return NULL;
      }
      pThread = m_readyThreads.last();
      m_readyThreads.removeLast();
    }
    return pThread;
  }

  class QueueImpl : public OdApcQueue, protected OdApcAtom {
  protected:
    OdRxThreadPoolImpl*  frmwrk;
    OdStaticApcEventImpl ready;

    QueueImpl() {
      frmwrk = 0;
    }

    ~QueueImpl() {
      if( frmwrk ) {
        frmwrk->release();
        frmwrk = 0;
      }
    }

  public:
    void init( OdRxThreadPoolImpl* frmwrk ) {
      frmwrk->addRef();
      this->frmwrk = frmwrk;
    }

    OdRxThreadPoolService& framework() {
      return *frmwrk;
    }

    void setAtomPoolRef( OdApcObjectPool* pAtomPool ) {
      throw OdError(eNotImplemented);
    }

    void executeMainThreadAction( MainThreadFunc mtFunc, void *pArg ) {
      frmwrk->executeMainThreadAction(mtFunc, pArg);
    }

    int numThreads() const {
      return 0;
    }
  };

  struct TopLevelTask : public OdRxObject
  {
    // Gate for main thread processing
    OdStaticApcEventImpl m_waitHandlerGate;
    // True if queue waits free thread for continue processing
    bool m_bWaitForFreeThread;
    // True if any thread was processing completed and thread freed for next processing
    bool m_bFreeThreadAvailable;
    // Stores function for processing in main thread
    MainThreadFunc m_mainThreadProc;
    // Stores function argument for processing in main thread
    void *m_mainThreadProcArg;
    // Gate for processing in main thread
    OdStaticApcEventImpl m_mainThreadProcGate;
    // Pointer to MTQueue::readyMutex
    OdMutex *m_pReadyMutex;
    OdMutex m_mainThreadJobEvent;
    // Information about main thread
    unsigned m_mainThreadId;
    bool     m_bMainThreadCounted;

    TopLevelTask()
      : m_bWaitForFreeThread(false)
      , m_bFreeThreadAvailable(false)
      , m_mainThreadProc(NULL)
      , m_mainThreadProcArg(NULL)
      , m_pReadyMutex(NULL)
      , m_mainThreadId(OD_TP_ID_TO_UINT(OdTP::currentThreadId()))
      , m_bMainThreadCounted(false)
    {
      // Wait handler gate is initially closed
      m_waitHandlerGate.reset();
      // Gate for processing in main thread is initially opened
      m_mainThreadProcGate.set();
    }
  } *m_pTopLevelTask;

  class MTQueue : public QueueImpl {
  protected:
    OdInt32 numEntries;
    OdMutex readyMutex;
    // Non-zero if this is top queue (we assuming that this job will be present until nested jobs doesn't accomplished)
    OdSmartPtr<TopLevelTask> m_pTopThreadJob;
    // Queue initialization parameters
    unsigned m_nThreadAttributes;  // Attributes to pass in increase/decrease
    enum MtQueueRuntimeFlags
    {
      // kMtQueueForceNewThreads // Force allocation of new threads if there is no exist
      // kMtQueueAllowExecByMain // Don't wait for free thread and execute by main even if this is a top-level task
      // kMtQueueForceTopLevel   // Make MtQueue top level even if other registered threads already run
      kFinalizeThreading = (kMtQueueLastFlag << 1), // True if top level task initiated, all threads must be finalized on complete
      kThreadsLocked     = (kMtQueueLastFlag << 2)  // True if threads locked by queue on initialization
    };
    OdUInt32 m_nFlags;             // Set of initial and runtime flags
    // Locked threads, requested by caller
    OdApcThreadImplPtrVector  m_queueThreads;
    OdApcThreadImplCPtrVector m_readyQueueThreads;
    // Saved Id's for completed threads
    typedef OdVector<unsigned, OdMemoryAllocator<unsigned> > OdUnsignedVector;
    OdUnsignedVector          m_completedThreads;
  public:
    MTQueue() {
      numEntries = 0;
      m_nThreadAttributes = ThreadsCounter::kNoAttributes;
      m_nFlags = 0;
    }

    ~MTQueue() {
      wait();
    }

    int numThreads() const {
      return (int)m_queueThreads.size();
    }

    void init( OdRxThreadPoolImpl* frmwrk, unsigned nThreadAttributes, int numThreads, OdUInt32 nFlags ) {
      QueueImpl::init( frmwrk );
      m_nThreadAttributes = nThreadAttributes;
      m_nFlags = nFlags;
      if (numThreads > 0)
      { // Collect requested threads
        do {
          OdApcThreadImpl *pThread = frmwrk->readyThread(GETBIT(nFlags, kMtQueueForceNewThreads));
          if (pThread)
          {
            m_queueThreads.push_back(new OdApcThreadImplPtr(pThread)); // SmartPtr will lock this thread for us, it newer will be returned to
                                                                       // thread pool until this array doesn't freed.
            m_readyQueueThreads.push_back(pThread);
          }
          else
            break;
        } while (--numThreads);
      }
      SETBIT(m_nFlags, kThreadsLocked, !m_queueThreads.isEmpty());
    }
    void setBusy() {
      OdMutexAutoLock lock( readyMutex );
      if ( !numEntries ) {
        ready.reset();
        // Top-level tasks require special handling, we assume that this is top-level task if no started threads registered
        if (GETBIT(m_nFlags, kMtQueueForceTopLevel) || !::odThreadsCounter())
        {
          // If no any threads started we can initiate main thread task
          if (!frmwrk->m_pTopLevelTask)
          { // Only if threads is available for run
            m_pTopThreadJob = OdRxObjectImpl<TopLevelTask>::createObject();
            m_pTopThreadJob->m_pReadyMutex = &readyMutex;
            // Service must know about top level task
            frmwrk->m_pTopLevelTask = m_pTopThreadJob;
          }
          // Threading must be finalized by this queue
          if (!::odThreadsCounter())
            SETBIT_1(m_nFlags, kFinalizeThreading);
        }
      }
      ++numEntries;
    }

    void setReady(bool bMainThreadCall = false) {
      OdMutexAutoLock lock( readyMutex );
      --numEntries;
      if (!bMainThreadCall)
      {
        /* Store completed threads in following form:
           Id, Id, ..., Id, 0, Id
           0, Id on endian means that top-level thread must be decreased also. */
        unsigned threadId = OD_TP_ID_TO_UINT(OdTP::currentThreadId());
        if (!GETBIT(m_nFlags, kThreadsLocked) || !m_completedThreads.contains(threadId))
          m_completedThreads.push_back(threadId);
        if (!numEntries && !m_pTopThreadJob.isNull())
        {
          m_completedThreads.push_back(0); // Marks as top job, since m_pTopThreadJob will be released
          m_completedThreads.push_back(m_pTopThreadJob->m_mainThreadId);
        }
        if (GETBIT(m_nFlags, kThreadsLocked))
        { // Return thread to list if ready queue threads if threads is locked by queue on init
          OdUInt32 nThreads = m_queueThreads.size();
          OdApcThreadImplPtrPtr *pThreads = m_queueThreads.asArrayPtr();
          for (OdUInt32 nThread = 0; nThread < nThreads; nThread++)
          {
            if (getPtr(pThreads[nThread])->getId() == threadId)
            {
              m_readyQueueThreads.push_back(getPtr(pThreads[nThread]));
              break;
            }
          }
          if (!numEntries)
          {
            // If we access last (probably single) thread in queue threads we must call decrease for all of them
            for (OdUInt32 nFreeThread = 0; nFreeThread < nThreads; nFreeThread++)
            {
              const unsigned thrdId = getPtr(pThreads[nFreeThread])->getId();
              if (!m_completedThreads.contains(thrdId))
                m_completedThreads.insertAt(0, thrdId);
            }
          }
          if (!m_pTopThreadJob.isNull() && m_pTopThreadJob->m_bWaitForFreeThread)
          {
            // If main thread waits for free thread we can here inform that we have it.
            m_pTopThreadJob->m_bFreeThreadAvailable = true;
            m_pTopThreadJob->m_waitHandlerGate.set();
          }
        }
      }
      if ( !numEntries ) {
        ready.set();
        // Complete top-level task
        if (!m_pTopThreadJob.isNull())
        {
          // All tasks completed, so we can open gate for finalize waiting
          m_pTopThreadJob->m_waitHandlerGate.set();
          // Object locked inside wait(), so we can free it is here
          m_pTopThreadJob.release();
          // Detach from main service. No more call's for m_pTopLevelTask possible.
          frmwrk->m_pTopLevelTask = NULL;
        }
      }
      else if (!GETBIT(m_nFlags, kThreadsLocked) && !m_pTopThreadJob.isNull())
      {
        // Top jobs maybe managed more effectively with decrease() calling, we can execute main thread
        // processing in any time.
        m_pTopThreadJob->m_waitHandlerGate.set();
      }
    }

    void wait() {
      {
        OdMutexAutoLock lock( readyMutex );
        if ( !numEntries ) {
          // If no active entries on this moment this is doesn't mean that no threads executed before starting of
          // wait() handler. In this case we also must check that any threads require decreasing.
          decreaseCompletedThreads();
          return;
        }
      }
      // Top-level task
      OdSmartPtr<TopLevelTask> pTopThreadJob(m_pTopThreadJob); // lock
      OdApcThreadImpl *pExceptionThread = NULL;
      if (!pTopThreadJob.isNull())
      {
        enum { kExitByFreeThread, kExitByComplete } breakCause = kExitByFreeThread;
        for (;;)
        {
          // Gate for: 1) wait free thread for top level entry
          //           2) execute operation in main thread
          //           3) stop waiting on threads completion
          pTopThreadJob->m_waitHandlerGate.waitAndReset();
          {
            {
              // Thread waiting flags can be modified under m_ready mutex
              OdMutexAutoLock lock2( frmwrk->m_ready );
              if ( pTopThreadJob->m_bWaitForFreeThread )
              {
                if ( pTopThreadJob->m_bFreeThreadAvailable )
                  break; // Open gate for using of next free thread
              }
            }
            OdMutexAutoLock lock( readyMutex );
            if ( pTopThreadJob->m_mainThreadProc )
            { // There is task for execution in main thread
              pTopThreadJob->m_mainThreadProc(pTopThreadJob->m_mainThreadProcArg);
              pTopThreadJob->m_mainThreadProc = NULL;
              pTopThreadJob->m_mainThreadProcArg = NULL;
              // Open gate for complete processing in main thread and wait for next work
              pTopThreadJob->m_mainThreadProcGate.set();
            }
            decreaseCompletedThreads(); // Top-level handler maybe forced by setReady() to decrease() free threads.
            if ( !numEntries )
            { // All threads completed
              breakCause = kExitByComplete;
              break;
            }
          }
        }
        // #11743
        if ((breakCause == kExitByComplete) && GETBIT(m_nFlags, kFinalizeThreading))
        {
          pExceptionThread = frmwrk->finalizeThreading();
          SETBIT_0(m_nFlags, kFinalizeThreading); // Clear finalization flag for next execution
        }
      }
      else
      {
        // For non top-level tasks we could simply wait completion of all threads
        ready.wait();
        // #11743 : This is possible if wait() started after m_pTopThreadJob released, in this case some threads
        //          can be still locked. Just look onto m_bFinalizeThreading flag to be sure that this is top-level queue.
        if (GETBIT(m_nFlags, kFinalizeThreading))
        {
          pExceptionThread = frmwrk->finalizeThreading();
          SETBIT_0(m_nFlags, kFinalizeThreading); // Clear finalization flag for next execution
        }
      }
      decreaseCompletedThreads(); // Call decrease() for all completed threads.
      // Generate exception, if exist.
      if (pExceptionThread)
        pExceptionThread->processException();
    }

    // OdApcAtom override for MTQueue
    void apcEntryPoint( OdApcParamType param ) {
      odThreadsCounter().startThread();
      QueueEntry* queueEntry = reinterpret_cast< QueueEntry* >( param );
      try { queueEntry->asyncCall(); }
      catch (...)
      {
        delete queueEntry;
        odThreadsCounter().stopThread();
        setReady();
        throw;
      }
      delete queueEntry;
      odThreadsCounter().stopThread();
      setReady();
    }

    void addEntryPoint( OdApcAtom* atom, OdRxObject* rxPtrParam ) {
      addEntryPoint( new QueueEntry( atom, rxPtrParam ) );
    }

    void addEntryPoint( OdApcAtom* atom, OdApcParamType numberParam ) {
      addEntryPoint( new QueueEntry( atom, numberParam ) );
    }

    // Process increase/decrease for all threads locked by this queue
    void callIncreaseDecrease(bool bInc, bool bTopJob, unsigned topJobId)
    {
      OdUnsignedVector threadIds;
      unsigned nTopJob = (!bTopJob) ? 0u : 1u;
      unsigned nThreads = m_queueThreads.size() + nTopJob;
      threadIds.resize(nThreads);
      unsigned *pOut = threadIds.asArrayPtr();
      if (nTopJob) *pOut = topJobId;
      const OdApcThreadImplPtrPtr *pIn = m_queueThreads.getPtr();
      for (unsigned nThread = 0; nThread < nThreads - nTopJob; nThread++)
        pOut[nThread + nTopJob] = getPtr(pIn[nThread])->getId();
      if (bInc)
        odThreadsCounter().increase(nThreads, pOut, m_nThreadAttributes);
      else
        odThreadsCounter().decrease(nThreads, pOut);
    }

    // Call decrease for all threads marked as completed
    void decreaseCompletedThreads()
    {
      OdMutexAutoLock lock( readyMutex );
      if (!m_completedThreads.isEmpty())
      {
        if (!GETBIT(m_nFlags, kThreadsLocked))
        {
          OdUInt32 nThreads = m_completedThreads.size();
          const unsigned *pThreads = m_completedThreads.getPtr();
          for (OdUInt32 nThread = 0; nThread < nThreads; nThread++)
          {
            const bool bTopJob = (nThread < nThreads - 1) && (pThreads[nThread + 1] == 0);
            if (bTopJob)
            {
              unsigned threadIds[2] = { pThreads[nThread + 2], pThreads[nThread] };
              odThreadsCounter().decrease(2, threadIds);
            }
            else
              odThreadsCounter().decrease(1, pThreads + nThread);
            for (OdUInt32 nReuse = 0; nReuse < m_queueThreads.size(); nReuse++)
            {
              if (getPtr(m_queueThreads[nReuse])->getId() == pThreads[nThread])
              {
                m_queueThreads.removeAt(nReuse);
                break;
              }
            }
            if (bTopJob)
              break;
          }
          m_completedThreads.clear();
        }
        else
        {
          OdUInt32 nThreads = m_completedThreads.size();
          bool bTopJob = (nThreads > 2) && (m_completedThreads[nThreads - 2] == 0);
          if (bTopJob) nThreads -= 2;
          if (nThreads == m_queueThreads.size())
          {
            callIncreaseDecrease(false, bTopJob, (bTopJob) ? m_completedThreads.last() : 0u);
            m_completedThreads.clear();
          }
        }
      }
    }

    OdApcThreadImpl* readyThread() {
      decreaseCompletedThreads(); // Force decreasing of threads which is previously started.
      if (!GETBIT(m_nFlags, kThreadsLocked))
      {
        OdApcThreadImpl *pThread = frmwrk->readyThread( GETBIT(m_nFlags, kMtQueueForceNewThreads) );
        if (pThread)
        {
          unsigned nId = pThread->getId();
          if (!m_pTopThreadJob.isNull() && !m_pTopThreadJob->m_bMainThreadCounted)
          {
            unsigned nIds[2] = { m_pTopThreadJob->m_mainThreadId, nId };
            odThreadsCounter().increase(2, nIds, m_nThreadAttributes);
            m_pTopThreadJob->m_bMainThreadCounted = true;
          }
          else
            odThreadsCounter().increase(1, &nId, m_nThreadAttributes);
          // We must lock thread by this queue until decrease() doesn't called, elsewhere it can become
          // available for other queues before decrease() call.
          OdMutexAutoLock lock( readyMutex );
          m_queueThreads.push_back(new OdApcThreadImplPtr(pThread));
        }
        return pThread;
      }
      else
      {
        OdMutexAutoLock lock( readyMutex );
        if (m_readyQueueThreads.isEmpty())
          return NULL;
        else {
          OdApcThreadImpl *pThread = m_readyQueueThreads.last();
          // If we access first thread in queue threads we can call increase for all of them
          if (m_readyQueueThreads.size() == m_queueThreads.size())
          {
            const bool bTopJob = !m_pTopThreadJob.isNull();
            callIncreaseDecrease(true, bTopJob, (bTopJob) ? m_pTopThreadJob->m_mainThreadId : 0u);
          }
          m_readyQueueThreads.removeLast();
          return pThread;
        }
      }
    }

    inline void addEntryPoint( QueueEntry* queueEntry ) {
      setBusy();
      // Grab free thread
      OdApcThreadImpl* thread = NULL;
      for (;;)
      {
        thread = readyThread();
        if (!m_pTopThreadJob.isNull() && !thread && !GETBIT(m_nFlags, kMtQueueAllowExecByMain))
        {
          // For top level handler we can't use main thread for processing - it need for waiting main thread
          // operations. So we wait for free thread and try to aquire free thread again.
          { OdMutexAutoLock _lock(frmwrk->m_ready);
            m_pTopThreadJob->m_bWaitForFreeThread = true;
            m_pTopThreadJob->m_bFreeThreadAvailable = false;
          }
          wait();
          { OdMutexAutoLock _lock(frmwrk->m_ready);
            m_pTopThreadJob->m_bWaitForFreeThread = false;
          }
        } else
          break;
      }
      // If thread available run through it, elsewhere run in main thread
      if( thread ) {
        // Pass to new thread
#ifndef __BORLANDC__
        thread->asyncProcCall( static_cast<OdApcAtom*> ( this ), reinterpret_cast< OdApcParamType >( queueEntry ) );
#else
        thread->asyncProcCall( dynamic_cast<OdApcAtom*> ( this ), reinterpret_cast< OdApcParamType >( queueEntry ) );
#endif
      }
      else {
        queueEntry->asyncCall();
        delete queueEntry;
        setReady(true);
      }
    }
  };

  // Execute action in main thread (doesn't depends from queues count)
  void executeMainThreadAction( MainThreadFunc mtFunc, void *pArg ) {
    if (odThreadsCounter())
    {
      // Check if we works with main thread from MTQueue
      if (m_pTopLevelTask)
      { // MT Loading task on top
        TD_AUTOLOCK(m_pTopLevelTask->m_mainThreadJobEvent); // Pass only one object per time
        // @@@TODO: something like jobs list could be used here to avoid time consuming locking
        //m_pTopLevelTask->m_mainThreadProcGate.waitAndReset(); // gate closed until previous operation doesn't processed
        // This is not bad if we reuse ready mutex here. It will prevent potential conflict with executions in main thread.
        { OdMutexAutoLock lock( *(m_pTopLevelTask->m_pReadyMutex) ); // Locked if something processed in main thread
          m_pTopLevelTask->m_mainThreadProc = mtFunc;
          m_pTopLevelTask->m_mainThreadProcArg = pArg;
          m_pTopLevelTask->m_mainThreadProcGate.reset();
        }
        // Open gate for handle in main thread
        m_pTopLevelTask->m_waitHandlerGate.set();
        // Wait for completion of main thread work
        m_pTopLevelTask->m_mainThreadProcGate.wait();
      }
      else
      { // Elsewhere external task on top level
        ODA_ASSERT(::odThreadsCounter().getMainThreadFunc());
        if (::odThreadsCounter().getMainThreadFunc())
          ::odThreadsCounter().getMainThreadFunc()(mtFunc, pArg);
        else
          mtFunc(pArg);
      }
    }
    else
      // ST
      mtFunc(pArg);
  }

  void registerExternalThreads( unsigned nThreads, const unsigned* aThreads, unsigned nThreadAttribs ) {
    // Simply redirects to thread counter currently
    odThreadsCounter().increase( nThreads, aThreads, nThreadAttribs );
  }
  void unregisterExternalThreads( unsigned nThreads, const unsigned* aThreads ) {
    // Simply redirects to thread counter currently
    odThreadsCounter().decrease( nThreads, aThreads );
  }
  void externalThreadStart() {
    // Simply redirects to thread counter currently
    odThreadsCounter().startThread();
  }
  void externalThreadStop() {
    // Simply redirects to thread counter currently
    odThreadsCounter().stopThread();
  }
  void setExternalMainThreadFunc( ExecuteMainThreadFunc execFunc ) {
    // Simply redirects to thread counter currently
    odThreadsCounter().setMainThreadFunc( execFunc );
  }
  ExecuteMainThreadFunc getExternalMainThreadFunc() const {
    // Simply redirects to thread counter currently
    return odThreadsCounter().getMainThreadFunc();
  }

#if 0 // Previous StQueue implementation, working independently from MtQueue.
  class STQueue : public QueueImpl {
  protected:
    OdMutex                m_mutex;
    OdApcThreadImpl*       m_thread;
    unsigned int           m_completedThread;
    std::queue<QueueEntry> m_queue;
    OdStaticApcEventImpl   m_ready;
    // Queue initialization parameters
    unsigned m_nThreadAttributes;  // Attributes to pass in increase/decrease
    enum StQueueRuntimeFlags
    {
      kFinalizeThreading = (kStQueueLastFlag << 1) // True if top level task initiated, all threads must be finalized on complete
    };
    OdUInt32 m_nFlags;             // Set of initial and runtime flags
    // Implementations
    void decreaseCompletedThread()
    {
      if (m_completedThread)
      {
        odThreadsCounter().decrease(1, &m_completedThread);
        m_completedThread = 0;
      }
    }
  public:
    STQueue() : m_thread(NULL), m_completedThread(0) {
    }

    ~STQueue() {
      wait();
    }

    int numThreads() const { return 1; }

    void init( OdRxThreadPoolImpl* frmwrk, unsigned nThreadAttributes, OdUInt32 nFlags ) {
      QueueImpl::init( frmwrk );
      m_nThreadAttributes = nThreadAttributes;
      m_nFlags = nFlags;
    }

    // OdApcAtom override for STQueue
    void apcEntryPoint( OdRxObject* /*pMessage*/ ) {
      QueueEntry entry(0,0,0);
      odThreadsCounter().startThread();
      for(;;) {
        {
          OdMutexAutoLock lock( m_mutex );
          if( m_queue.empty() ) {
            m_completedThread = m_thread->getId();
            m_thread = NULL;
            m_ready.set();
            break;
          }
          entry = m_queue.front();
          m_queue.pop();
        }
        entry.asyncCall();
      }
      odThreadsCounter().stopThread();
    }

    void addEntryPoint( OdApcAtom* atom, OdRxObject* rxPtrParam ) {
      if (GETBIT(m_nFlags, kStQueueExecByMain))
      { QueueEntry(atom, rxPtrParam).asyncCall();
        return; }
      OdMutexAutoLock lock( m_mutex );
      m_queue.push( QueueEntry( atom, rxPtrParam ) );
      addEntryPoint();
    }

    void addEntryPoint( OdApcAtom* atom, OdApcParamType numberParam ) {
      if (GETBIT(m_nFlags, kStQueueExecByMain))
      { QueueEntry(atom, numberParam).asyncCall();
        return; }
      OdMutexAutoLock lock( m_mutex );
      m_queue.push( QueueEntry( atom, numberParam ) );
      addEntryPoint();
    }
  private:
    inline void addEntryPoint() {
      decreaseCompletedThread();
      if( !m_thread ) {
        m_ready.reset();
        m_thread = frmwrk->readyThread( true );
        unsigned int nThreadId = m_thread->getId();
        odThreadsCounter().increase(1, &nThreadId);
#ifndef __BORLANDC__
        m_thread->asyncProcCall(static_cast<OdApcAtom*> ( this ), OdRxObjectPtr() );
#else
        m_thread->asyncProcCall(dynamic_cast<OdApcAtom*> ( this ), OdRxObjectPtr() );
#endif
      }
    }
  public:

    void wait() {
      m_ready.wait();
      decreaseCompletedThread();
    }
  };
#endif // #CORE-12045 : New StQueue implementation working through MtQueue (simplifies implementation and extends functionality).
  class STQueueSimplex : public QueueImpl
  {
    public:
      STQueueSimplex() : QueueImpl() {}
      ~STQueueSimplex() {}
      void addEntryPoint(OdApcAtom* atom, OdRxObject* rxPtrParam) {
        QueueEntry(atom, rxPtrParam).asyncCall();
      }
      void addEntryPoint(OdApcAtom* atom, OdApcParamType numberParam) {
        QueueEntry(atom, numberParam).asyncCall();
      }
      void wait() {}
  };
  class STQueue : public MTQueue {
  protected:
    OdMutex                m_queueMutex;
    std::queue<QueueEntry> m_queue;
    OdStaticApcEventImpl   m_queueReady;
    enum QueueCancel
    {
      kQueueOnHold = 0,
      kQueueInProgress,
      kQueueCanceled
    }                      m_queueState;
  public:
    STQueue() : MTQueue(), m_queueState(kQueueOnHold) {
    }

    ~STQueue() {
      wait();
    }

    void init( OdRxThreadPoolImpl* frmwrk, unsigned nThreadAttributes, OdUInt32 nFlags ) {
      if (GETBIT(nFlags, kStQueueForceTopLevel))
      {
        SETBIT_0(nFlags, kStQueueForceTopLevel);
        SETBIT_1(nFlags, kMtQueueForceTopLevel);
      }
      SETBIT_1(nFlags, kMtQueueForceNewThreads); // StQueue always require dedicated thread
      MTQueue::init(frmwrk, nThreadAttributes, 1, nFlags);
    }

    // OdApcAtom override for STQueue
    void apcEntryPoint( OdRxObject* /*pMessage*/ ) {
      QueueEntry entry(0,0,0);
      odThreadsCounter().startThread();
      for(;;) {
        m_queueReady.wait();
        { OdMutexAutoLock lock( m_queueMutex );
          if( m_queue.empty() ) {
            if (m_queueState == kQueueCanceled)
              break;
            else {
              m_queueReady.reset();
              continue;
            }
          }
          entry = m_queue.front();
          m_queue.pop();
        }
        try { entry.asyncCall(); }
        catch (...)
        {
          odThreadsCounter().stopThread();
          setReady();
          throw;
        }
      }
      odThreadsCounter().stopThread();
      setReady();
    }
    // OdApcAtom override for MTQueue
    void apcEntryPoint(OdApcParamType /*param*/) { apcEntryPoint((OdRxObject*)NULL); } // Simply redirect to StQueue implementation

    void addEntryPoint( OdApcAtom* atom, OdRxObject* rxPtrParam ) {
      OdMutexAutoLock lock( m_queueMutex );
      m_queue.push( QueueEntry( atom, rxPtrParam ) );
      addEntryPoint();
    }

    void addEntryPoint( OdApcAtom* atom, OdApcParamType numberParam ) {
      OdMutexAutoLock lock( m_queueMutex );
      m_queue.push( QueueEntry( atom, numberParam ) );
      addEntryPoint();
    }
  private:
    inline void addEntryPoint() {
      m_queueReady.set();
      if (m_queueState == kQueueOnHold)
      {
        m_queueState = kQueueInProgress;
        MTQueue::addEntryPoint(NULL);
      }
    }
  public:

    void wait() {
      { OdMutexAutoLock lock(m_queueMutex);
        if (m_queueState == kQueueInProgress)
        {
          m_queueState = kQueueCanceled;
          m_queueReady.set();
        }
        else // Nothing to wait
          return;
      }
      try { MTQueue::wait(); }
      catch (...)
      {
        m_queueState = kQueueOnHold; // Return to "on-hold" state for further processing.
        throw;
      }
      m_queueState = kQueueOnHold; // Return to "on-hold" state for further processing.
    }
  };

  OdApcThreadImpl* runNewThread() {
    OdApcThreadImplPtr thread = OdApcThreadImpl::createObject();
    {
      OdMutexAutoLock lock( m_ready );
      m_threads.append( new OdApcThreadImplPtr(thread) );
    }
    thread->execute( this );
    if (thread->getId() == 0)
    { // System cannot spawn new thread in some cases (not enough memory, etc.)
      OdMutexAutoLock lock( m_ready );
      // Eliminate thread from threads list
      ODA_FAIL(); // Assert in Debug mode
#if (_MSC_VER >= 1400) && defined(_DEBUG)
      int err;
      _get_errno(&err);
      typedef BOOL (WINAPI *AttachConsoleFuncPtr)(DWORD);
      AttachConsoleFuncPtr pAttachConsole = (AttachConsoleFuncPtr)::GetProcAddress(::LoadLibraryA("kernel32"), "AttachConsole");
      if (!pAttachConsole || (pAttachConsole(::GetCurrentProcessId()) == FALSE))
      {
        DWORD retCode = ::GetLastError();
        if (pAttachConsole && (retCode == ERROR_ACCESS_DENIED)) // True if process have console
        {
          printf("Can't create thread. Errno: %d\n", err);
        }
        else
        {
          ODA_TRACE(L"Can't create thread. Errno: %d\n", err);
        }
      }
#endif
      // Try to handle this case
      m_threads.removeLast(); // Thread is added in ready list here
      m_readyThreads.removeLast();
      return NULL;
    }
    return thread;
  }

  class DataReadWriteDispatcherImpl : public OdApcGateway {
    OdMutex           m_enterLock;
    OdMutex           m_readerCounterLock;
    OdTP::EventHandle m_hNoReaderEvent;
    OdUInt32          m_nReaders;
  public:

    DataReadWriteDispatcherImpl() {
      m_nReaders = 0;
      OdTP::createEvent(m_hNoReaderEvent);
    }

    ~DataReadWriteDispatcherImpl() {
      OdTP::closeEvent( m_hNoReaderEvent );
    }

    void on_enter() {
      OdMutexAutoLock lock( m_readerCounterLock );
      if( !m_nReaders ) {
        OdTP::resetEvent( m_hNoReaderEvent );
      }
      ++m_nReaders;
    }

    void on_leave() {
      OdMutexAutoLock lock( m_readerCounterLock );
      --m_nReaders;
      if(!m_nReaders) {
        OdTP::setEvent( m_hNoReaderEvent );
      }
    }

    void enter() {
      m_enterLock.lock();
      m_enterLock.unlock();
      on_enter();
    }

    void leave() {
      on_leave();
    }

    void lock() {
      m_enterLock.lock();
      OdTP::alterableWaitEvent( m_hNoReaderEvent );
    }

    void unlock() {
      m_enterLock.unlock();
    }

    void lockFromInside() {
      m_enterLock.lock();
      on_leave();
      OdTP::alterableWaitEvent( m_hNoReaderEvent );
    }

    void unlockFromInside() {
      on_enter();
      m_enterLock.unlock();
    }
  };

public:
  OdRxThreadPoolImpl()
    : m_nCPUs(0), m_nPhysicalCores(0), m_pTopLevelTask(NULL) {
  }

  void initApp() {
    reserve( numCPUs() );
    odThreadsCounter().setThreadPoolService(this);
  }

  void uninitApp() {
    odThreadsCounter().setThreadPoolService(NULL);
    while( m_threads.size() ) {
      getPtr(m_threads.last())->terminate();
      m_threads.removeLast();
    }
  }

  void reserve(int nThreads) {
    OdMutexAutoLock lock( m_ready );
    int nOldSize = m_threads.size();
    if(nOldSize < nThreads) {
      for(int i=nOldSize; i<nThreads; ++i) {
        runNewThread();
      }
    }
  }

  int numCPUs() const {
    if(m_nCPUs)
      return m_nCPUs;

    return m_nCPUs = OdTP::numCPUs(m_nPhysicalCores);
  }

  int numPhysicalCores() const {
    numCPUs();//to force system function call
    return m_nPhysicalCores;
  }

  int numThreads() const {
    OdMutexAutoLock lock( m_ready );
    return m_threads.size();
  }

  int numFreeThreads() const {
    OdMutexAutoLock lock( m_ready );
    return m_readyThreads.size();
  }

  OdApcQueuePtr newMTQueue(unsigned nThreadAttributes, int numThreads, OdUInt32 nFlags) {
    OdApcQueuePtr pRes = OdRxObjectImpl<MTQueue, OdApcQueue>::createObject();
    ((MTQueue*)pRes.get())->init( this, nThreadAttributes, numThreads, nFlags );
    return pRes;
  }

  OdApcQueuePtr newSTQueue(unsigned nThreadAttributes, OdUInt32 nFlags) {
    OdApcQueuePtr pRes;
    if (!GETBIT(nFlags, kStQueueExecByMain)) {
      pRes = OdRxObjectImpl<STQueue, OdApcQueue>::createObject();
      ((STQueue*)pRes.get())->init( this, nThreadAttributes, nFlags );
    } else {
      pRes = OdRxObjectImpl<STQueueSimplex, OdApcQueue>::createObject();
      ((STQueueSimplex*)pRes.get())->init( this );
    }
    return pRes;
  }

  /*
  OdApcDataReadWriteDispatcherPtr newDataReadWriteDispatcher() {
    OdApcDataReadWriteDispatcherPtr pRes = OdRxObjectImpl<DataReadWriteDispatcherImpl, OdApcDataReadWriteDispatcher>::createObject();
    //((OdApcGateway*)pRes.get())->init( this );
    return pRes;
  }
  */

  class OdApcObjectPoolImpl : public OdApcObjectPool {
    OdRxObjectPtrArray  m_pool;
    OdMutex             m_mutex;
    OdTP::EventHandle   m_hHasObjects;
  public:
    OdApcObjectPoolImpl() : m_pool( 8, -100 ) {
      OdTP::createEvent(m_hHasObjects);
    }

    ~OdApcObjectPoolImpl() {
      OdTP::closeEvent( m_hHasObjects );
    }

    void reserve( OdUInt32 n ) {
      OdMutexAutoLock lock( m_mutex );
      m_pool.reserve( n );
    }

    OdRxObjectPtr take() {
      m_mutex.lock();
      while( m_pool.empty() ) {
        m_mutex.unlock();
        OdTP::alterableWaitEvent( m_hHasObjects );
        m_mutex.lock();
      }
      OdRxObjectPtr pRes = m_pool.last();
      m_pool.removeLast();
      if( m_pool.empty() ) {
        OdTP::resetEvent( m_hHasObjects );
      }
      m_mutex.unlock();
      return pRes;
    }

    void put( OdRxObject* pObj ) {
      OdMutexAutoLock lock( m_mutex );
      m_pool.append( pObj );
      if( m_pool.size()==1 ) {
        OdTP::setEvent( m_hHasObjects );
      }
    }
  };

  OdApcObjectPoolPtr newObjectPool() {
    OdApcObjectPoolPtr pRes = OdRxObjectImpl<OdApcObjectPoolImpl>::createObject();
    pRes->reserve( numCPUs()*2 );
    return pRes;
  }

  class Gateway : public OdApcGateway {
    OdStaticApcEventImpl allThreadsAtCheckPointEvent;
    OdStaticApcEventImpl threadReleaseEvent;
#ifdef ODA_DIAGNOSTICS
    OdTP::ThreadID ctrlThreadId;
#endif
    OdMutex threadCounterMutex;
    OdUInt32 threadCounter;
  public:
    Gateway() : threadCounter(0)
    {
#ifdef _DEBUG
      ctrlThreadId = 0;
#endif
    }

    ~Gateway() {
    }

    void passBySecondary() {
      {
        OdMutexAutoLock lock( threadCounterMutex );
        ODA_ASSERT( ctrlThreadId!=0 );

        ODA_ASSERT( ctrlThreadId!=OdTP::currentThreadId() );
        ODA_ASSERT( threadCounter > 0 );

        if( --threadCounter == 0 )
          allThreadsAtCheckPointEvent.set();
      }
      threadReleaseEvent.wait();
    }

    void lockByMain( OdUInt32 numThreads ) {
#ifdef _DEBUG
      {
        OdMutexAutoLock lock( threadCounterMutex );
        ODA_ASSERT( threadCounter==0 && ctrlThreadId==0 );
        ctrlThreadId = OdTP::currentThreadId();
      }
#endif
      if( numThreads==0 ) {
        allThreadsAtCheckPointEvent.set();
        return;
      }
      threadCounter = numThreads;
      allThreadsAtCheckPointEvent.reset();
      threadReleaseEvent.reset();
    }

    void unlockByMain() {
#ifdef _DEBUG
      {
        OdMutexAutoLock lock( threadCounterMutex );
        //ODA_ASSERT( threadCounter==0 ); // all threads should be blocked yet
        ODA_ASSERT( ctrlThreadId==OdTP::currentThreadId() );
        ctrlThreadId = 0;
        threadCounter = 0;
      }
#endif
      allThreadsAtCheckPointEvent.reset();
      threadReleaseEvent.set();
    }

    void waitByMain() {
      ODA_ASSERT( ctrlThreadId==OdTP::currentThreadId() );
      allThreadsAtCheckPointEvent.wait();
    }
  };

  class LoopedGateway : public OdApcLoopedGateway {
    OdStaticRxObject<Gateway> sync0;
    OdStaticRxObject<Gateway> sync1;
    Gateway  *p0;
    Gateway  *p1;
    OdUInt32 numControlledThreads;
  public:
    LoopedGateway() {
      p0 = &sync0;
      p1 = &sync1;
    }

    virtual void init( OdUInt32 numThreads ) {
      ODA_ASSERT( numThreads );
      numControlledThreads = numThreads;
      --numControlledThreads;
      p0->Gateway::lockByMain( numControlledThreads );
    }

    virtual void passByMain() {
      p0->Gateway::waitByMain();
      std::swap( p0, p1 );
      p0->Gateway::lockByMain( numControlledThreads );
      p1->Gateway::unlockByMain();
    }

    virtual void waitByMain() {
      p0->Gateway::waitByMain();
    }
    virtual void passByMainNoWait() {
      std::swap( p0, p1 );
      p0->Gateway::lockByMain( numControlledThreads );
      p1->Gateway::unlockByMain();
    }

    virtual void passBySecondary() {
      p0->Gateway::passBySecondary();
    }
  };

  OdApcLoopedGatewayPtr newLoopedGateway() {
    OdSmartPtr<LoopedGateway> pRes = OdRxObjectImpl<LoopedGateway>::createObject();
    return pRes;
  }

  OdApcGatewayPtr newGateway() {
    OdSmartPtr<Gateway> pRes = OdRxObjectImpl<Gateway>::createObject();
    return pRes;
  }

  OdApcEventPtr newEvent() {
    OdApcEventPtr pRes = OdRxObjectImpl<OdApcEventImpl>::createObject();
    return pRes;
  }

  OdApcThreadPtr newThread() {
    return readyThread( true );
  }
};

ODRX_DEFINE_DYNAMIC_MODULE(OdRxThreadPoolImpl);

OdRxModule* odrxCreateModuleObject_For_OdRxThreadPoolService(const OdString& szModuleName)
{
  return OdRxStaticModule<OdRxThreadPoolImpl >::createModule(szModuleName);
}

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  switch ( dwReason )
  {
  case DLL_PROCESS_ATTACH:
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
    break;
  }
  return TRUE;
}

#endif //_TOOLKIT_IN_DLL_
