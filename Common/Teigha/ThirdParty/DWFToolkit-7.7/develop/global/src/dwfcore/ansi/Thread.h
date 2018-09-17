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

#ifndef _DWFCORE_THREAD_H
#define _DWFCORE_THREAD_H


///
///\file        dwfcore/ansi/Thread.h
///\brief       This header contains the POSIX-based DWFThread class declaration.
///
///\note        No documentation will be generated from this file due to class name collision, 
///             such is the nature of Doyxgen.
///             Please refer to DWFCore::DWFThread documented in \a dwfcore/win32/Thread.h as the class
///             declarations are identical.
///
///\warning     This file should never be included directly.  
///             Instead, include \a dwfcore/Synchronization.h and all 
///             related header files, appropriate with the build configuration,
///             will be included.
///
///\todo        Move from dwfcore/ansi/Thread.h to dwfcore/posix/Thread.h
///


#include "dwfcore/Core.h"

#ifdef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY

#ifndef _DWFCORE_SYNCHRONIZATION_H
#error  This header should not be included directly - use "dwfcore/Synchronization.h" instead
#endif


namespace DWFCore
{

//
// fwd decl
//
class DWFThreadWorker;
class DWFThreadPool;


///\class           DWFThread   dwfcore/ansi/Thread.h  "dwfcore/ansi/Thread.h"
///\brief           A pooled worker thread implementation for the POSIX thread library.
///\since           1.0.1
///
///\ingroup         CrossPlatformInterface
///
class DWFThread
{

public:

    ///
    ///\enum    teState
    ///\brief   Enumeration values that define the thread state.
    ///
    ///         The thread is created in the \a eNone state and 
    ///         switches to \a eRun when \a run() is invoked;
    ///         the final state is \a eEnd.
    ///
    ///         When a worker is being executed, the state is changed
    ///         to \a eWork; it will be reset to \eRun once the
    ///         thread is returned to the pool.
    ///
    typedef enum
    {
        ///
        ///     Indicates the thread is not executing in the thread pool;
        ///     the thread has not yet entered the \a run() method.
        ///
        eNone,

        ///
        ///     Indicates the thread is executing in the thread pool;
        ///     the thread is inside of the \a run() method and is
        ///     waiting for work.
        ///
        eRun,

        ///
        ///     Indicates the thread is no longer executing the thread pool;
        ///     the thread has exited the \a run() method.
        ///
        eEnd,

        ///
        ///     Indicates the thread is executing in thread pool and is
        ////    currently running a worker inside of the \a run() method.
        ///
        eWork,

        ///
        ///     Currently only used internally for state change requests to forcibly terminate execution.
        ///
        eKill,

        ///
        ///     Indicates the thread executing in the thread pool has been suspended;
        ///     the thread is inside of the \a run() method.
        ///     This value is also used internally for state change requests.
        ///
        eSuspend

    } teState;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    ~DWFThread()
        throw();

    ///
    ///         Runs the main thread logic.  This method is invoked
    ///         within the thread function by the thread pool.
    ///
    ///\throw   None
    ///
    void run()
        throw();

    ///
    ///         Requests the thread end normally.
    ///
    ///\throw   DWFException
    ///
    void end()
        throw( DWFException );

    ///
    ///         Request the thread terminate immediately.
    ///
    ///\throw   DWFException
    ///
    void kill()
        throw( DWFException );

    ///
    ///         Request the thread suspended itself.
    ///
    ///\throw   DWFException
    ///
    void suspend()
        throw( DWFException );

    ///
    ///         Request the suspended thread resume itself.
    ///
    ///\throw   DWFException
    ///
    void resume()
        throw( DWFException );

private:
    friend class DWFThreadPool;

    //
    // Constructor
    //
    DWFThread( DWFThreadPool& rPool )
        throw();

    //
    // Begins the thread
    //
    void _begin()
        throw( DWFException );

    //
    // Joins the thread
    //
    void _join()
        throw( DWFException );

    //
    // Runs a worker in the thread
    //
    void _work( DWFThreadWorker& rWorker )
        throw( DWFException );

    //
    // returns the current run state
    //
    teState _state()
        throw( DWFException );

    //
    // sets the current run state
    //
    void _setstate( teState eState )
        throw( DWFException );

    //
    // returns the current state change request
    //
    teState _requeststate()
        throw( DWFException );

    //
    // requests a state change
    //
    void _setrequest( teState eState )
        throw( DWFException );

private:

    DWFThreadPool&      _rPool;

    pthread_t           _tThread;
    pthread_attr_t      _tThreadAttributes;

    pthread_mutex_t     _tStateMutex;

    DWFSignal           _oWorkSignal;
    DWFThreadWorker*    _pWorker;

    volatile teState    _eRunState;
    volatile teState    _eRequestState;

private:

    //
    // Not Implemented
    //
    DWFThread( const DWFThread& );
    DWFThread& operator=( const DWFThread& );
};

}


#else
#error  This header file is incompatible with your current system configuration
#endif

#endif
