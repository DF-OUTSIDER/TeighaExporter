//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

#ifndef _DWFCORE_THREAD_POOL_H
#define _DWFCORE_THREAD_POOL_H


///
///\file        dwfcore/ThreadPool.h
///\brief       This header contains the declaration of the DWFThreadPool class
///             and the DWFThreadWorker interface that it uses.
///


#include "dwfcore/Core.h"
#include "dwfcore/STL.h"
#include "dwfcore/Exception.h"
#include "dwfcore/Synchronization.h"



namespace DWFCore
{

///
///\interface       DWFThreadWorker   dwfcore/ThreadPool.h  "dwfcore/ThreadPool.h"
///\brief           This interface must be implemented by those classes that
///                 will be run in a worker thread in a DWFThreadPool.
///\since           1.0.1
///
///                 A thread worker (a specialization of this class) encapsulates
///                 a unit of work to be executed in a thread context.  The worker
///                 or it's dependents will never have access to the thread itself
///                 nor directly control it's behavior or define it's code path.
///                 (Although it is possible to make certain requests on the worker
///                 thread via the DWFThreadPool::Controller interface.)
///                 A thread worker is run in a sandbox within a pooled thread,
///                 it does not own the thread.  Likewise, if the same worker is 
///                 run again in the same thread pool, there is no guarantee that it
///                 will execute in the same thread context as any previous run.
///
class DWFThreadWorker
{

public:

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    virtual ~DWFThreadWorker()
        throw()
    {;}

    ///
    ///             This method is invoked by a worker thread to execute
    ///             specialized code in the thread.  This may seem like an obvious
    ///             point to make but... the worker thread is done with this interface
    ///             when this function returns.  Since the class specialization is aware
    ///             of this condition (it contains the code), there is no state code or
    ///             similar set, returned or maintained by this method or interface.
    ///
    ///\throw       None    A thread worker may \b never throw exceptions from this method.
    ///
    _DWFCORE_API
    virtual void begin()
        throw() = 0;

    ///
    ///             Request that the worker suspend itself.
    ///
    ///             This method will \b not call any resumption/suspension mechanism in the underlying
    ///             thread library.  Rather, it is provided for worker specializations to
    ///             implement as appropriate in conjunction with the thread routine
    ///             scoped by the \a begin() method.  Additionally, no return value or
    ///             arbitrary state code is returned here as the details and requirements
    ///             and behaviors of this method are left entirely to the class specialization
    ///             and therefore no restrictions or assumptions are placed on its use or definition.
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    virtual void suspend()
        throw()
    {;}

    ///
    ///             Request that the suspended worker resume itself.
    ///
    ///             This method will \b not call any resumption/suspension mechanism in the underlying
    ///             thread library.  Rather, it is provided for worker specializations to
    ///             implement as appropriate in conjunction with the thread routine
    ///             scoped by the \a begin() method.  Additionally, no return value or
    ///             arbitrary state code is returned here as the details and requirements
    ///             and behaviors of this method are left entirely to the class specialization
    ///             and therefore no restrictions or assumptions are placed on its use or definition.
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    virtual void resume()
        throw()
    {;}

protected:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    DWFThreadWorker()
        throw()
    {;}
};

///
///\class           DWFThreadPool   dwfcore/ThreadPool.h  "dwfcore/ThreadPool.h"
///\brief           This class manages worker threads and executes DWFThreadWorker
///                 objects within them.
///\since           1.0.1
///
///                 The thread pool has two basic parts: a producer-consumer queue
///                 of worker threads and an internal queue monitor.  The queue contains
///                 DWFThread objects that operate in a well-defined algorithm and 
///                 allow the execution of DWFThreadWorker objects within.  When the
///                 thread pool is asked to run a thread worker, it will block the requesting
///                 thread a pooled thread becomes available (i.e. returned to the queue.)  The monitor
///                 is itself a thread worker that mediates requests among the control interfaces,
///                 the pool and the worker threads.  The monitor is never exposed publicly.
///
///\todo            Internal requests are made to the monitor in a non-blocking fashion and timeout
///                 after 1 second.  Review this logic and/or make these timeouts adjustable.
///
class DWFThreadPool : virtual public DWFCoreMemory
{

public:

    ///
    ///\class       Controller   dwfcore/ThreadPool.h  "dwfcore/ThreadPool.h"
    ///\brief       This interface returned by thread pool for controlling a running pooled worker thread.
    ///\since       1.0.1
    ///
    ///             This interface is intended to provide access to the low-level
    ///             thread library control functions.  Thus, some methods may not
    ///             be supported; so the returned state enumeration should always
    ///             be evaluted to determine the result of the call.
    ///             Further, no interaction is made directly with the DWFThreadWorker
    ///             object - those requirements and behaviors may be filled in by the class specialization.
    ///
    class Controller : virtual public DWFCoreMemory
    {

    public:

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFCORE_API 
        ~Controller()
            throw();

        ///
        ///         Attempts to end the thread in the most graceful manner possible,
        ///         but will, as a last resort, forcibly terminate the thread
        ///         potentially leaking resources and leaving the worker object
        ///         in an unknown state.
        ///
        ///\note    This request is made on the thread itself, not the worker object.
        ///
        ///\throw   DWFException
        ///
        _DWFCORE_API
        void end()
            throw( DWFException );

        ///
        ///         Request the running worker thread to suspend itself.
        ///         It will be possible to make a request that is not honored.
        ///
        ///\return  Returns the run state of the worker thread after attempting the suspend operation.
        ///\throw   DWFException
        ///
        _DWFCORE_API
        DWFThread::teState suspend()
            throw( DWFException );

        ///
        ///         Request the suspended worker thread to resume running.
        ///         It will be possible to make a request that is not honored
        ///
        ///\return  Returns the run state of the worker thread after attempting the resume operation.
        ///\throw   DWFException
        ///
        _DWFCORE_API
        DWFThread::teState resume()
            throw( DWFException );

    private:
        friend class DWFThreadPool;

        //
        // Constructor
        //
        _DWFCORE_API
        Controller( DWFThread&     rThread,
                    DWFThreadPool& rPool )
            throw();

        //
        // Not Implemented
        //
        Controller( const Controller& );
        Controller& operator=( const Controller& );

    private:

        DWFThread&     _rThread;
        DWFThreadPool& _rPool;
    };

    friend class Controller;

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    DWFThreadPool()
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    ~DWFThreadPool()
        throw();

    ///
    ///             Initializes the pool by creating and starting the worker threads and the monitor.
    ///
    ///\param       nThreads    The number of threads that will be available in the pool.
    ///                         This number must be greater than zero.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    void init( unsigned int nThreads )
        throw( DWFException );

    ///
    ///             Runs the worker in a pooled thread.
    ///             This call will block until a pooled thread is available.
    ///
    ///\param       rWorker     The worker to run in a pooled thread.
    ///\return      A pointer to a thread control interface.
    ///             The caller is reponsible for releasing this pointer with the \b DWFCORE_FREE_OBJECT macro.
    ///
    _DWFCORE_API
    DWFThreadPool::Controller* run( DWFThreadWorker& rWorker )
        throw( DWFException );

private:
    friend class DWFThread;

    //
    //
    //
    DWFThread* _acquireThread()
        throw( DWFException );

    //
    //
    //
    void _returnThread( DWFThread* pThread )
        throw( DWFException );

    //
    // Internal API for Controllers
    //
private:

    //
    // Request the running worker to end
    // Returns the run state of the worker after processing the call
    // It will be possible to make a request that is not honored
    //
    void _end( DWFThread* pThread )
        throw( DWFException );

    //
    // Request the running worker to suspend itself
    // Returns the run state of the worker after processing the call
    // It will be possible to make a request that is not honored
    //
    DWFThread::teState _suspend( DWFThread& rThread )
        throw();

    //
    // Request the suspended worker to resume running
    // Returns the run state of the worker after processing the call
    // It will be possible to make a request that is not honored
    //
    DWFThread::teState _resume( DWFThread& rThread )
        throw();

private:

    class _Monitor : public DWFThreadWorker
                   , virtual public DWFCoreMemory
    {
    public:

        _Monitor()
            throw()
            : _bRun( true )
            , _oRequestSignal()
            , _oResponseSignal()
            , _oRequestMutex()
            , _pThread( NULL )
            , _eRequest( DWFThread::eNone )
        {
            _oRequestMutex.init();
            _oRequestSignal.init();
            _oResponseSignal.init();
        }

        virtual ~_Monitor()
            throw()
        {
            _oRequestMutex.destroy();
            _oRequestSignal.destroy();
            _oResponseSignal.destroy();
        }

        void begin()
            throw();

        void finish()
            throw();

        bool request( DWFThread&         rThread,
                      DWFThread::teState eRequest,
                      unsigned int       nMilliseconds )
            throw();

    private:

        bool               _bRun;
        DWFSignal          _oRequestSignal;
        DWFSignal          _oResponseSignal;
        DWFThreadMutex     _oRequestMutex;

        DWFThread*         _pThread;
        DWFThread::teState _eRequest;

    private:

        _Monitor( const _Monitor& );
        _Monitor& operator=( const _Monitor& );
    };

private:

    bool                _bInit;

    vector<DWFThread*>  _oThreads;
    queue<DWFThread*>   _oThreadQueue;
    DWFThreadMutex*     _pQueueMutex;
    DWFSemaphore*       _pQueueSemaphore;

    _Monitor            _oMonitor;
    DWFThread*          _pMonitorThread;

private:

    //
    // Not Implemented
    //
    DWFThreadPool( const DWFThreadPool& );
    DWFThreadPool& operator=( const DWFThreadPool& );
};


}

#endif
