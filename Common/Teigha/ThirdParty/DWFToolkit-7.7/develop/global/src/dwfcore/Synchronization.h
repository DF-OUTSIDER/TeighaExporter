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


#ifndef _DWFCORE_SYNCHRONIZATION_H
#define _DWFCORE_SYNCHRONIZATION_H


///
///\file        dwfcore/Synchronization.h
///\brief       This header contains the declaration for the DWFSynchronization interface.
///

///
///\page        notes           Notes
///\section     synch           Synchronization Objects
///             The synchronization objects provided by the core library all require platform-specific
///             interface definitions in addition to their implementations.  The DWFCore::DWFSynchronization
///             interface does define a platform-independent contract for locking functionality and should be 
///             used if possible in cross-platform code.
///             Also, the Synchronization.h header file will include the correct header files for the current
///             platform and included be used instead of the individual headers themselves.
///
///             The manner in which these classes were defined in this version of the library
///             prevents Doxygen from fully documenting their various implementations.  
///             So for this release only, the documentation will be taken from the Win32 versions.
///             Note that the actual public class interfaces and identical for all platforms.
///

///
///\example     Threads/Threads.cpp
///             This sample program illustrates basic thread usage and control using several
///             core synchronization classes and interfaces including:
///                 - DWFCore::DWFThreadMutex
///                 - DWFCore::DWFThreadPool
///                     - DWFCore::DWFThreadPool::Controller
///                 - DWFCore::DWFThreadWorker
///
///             Other concepts like exception handling and timestamping are shown using
///             the following classes:
///                 - DWFCore::DWFTimer
///                 - DWFCore::DWFException
///

#include "dwfcore/String.h"
#include "dwfcore/Exception.h"



namespace DWFCore
{


///
///\interface       DWFSynchronization   dwfcore/Synchronization.h  "dwfcore/Synchronization.h"
///\brief           This interface should be implemented by all synchronization objects.
///\since           1.0.1
///
class DWFSynchronization
{

public:

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFSynchronization()
        throw()
    {;}

    ///
    ///             Initialize the synchronization object.
    ///             A client must always invoke this method before using this object.
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    virtual void init()
        throw( DWFException ) = 0;

    ///
    ///             Destroy the synchronization object.
    ///             A client must always invoke this method after using this object.
    ///
    _DWFCORE_API 
    virtual void destroy()
        throw( DWFException ) = 0;

    ///
    ///             Blocking call to acquire ownership of the synchronization object.
    ///
    ///\throw       DWFException
    ///
    _DWFCORE_API 
    virtual void lock()
        throw( DWFException ) = 0;

    ///
    ///             Non-blocking call to acquire ownership of the synchronization object.
    ///             If the object cannot be locked, this method must return immediately.
    ///
    ///\return      \e true if the object lock was acquired, \e false if the call would block
    ///             waiting for the lock.
    ///\throw       DWFException
    ///
    _DWFCORE_API 
    virtual bool trylock()
        throw( DWFException ) = 0;

    ///
    ///             Relinquish ownership of the locked synchronization object.
    ///
    ///\throw       DWFException
    ///
    _DWFCORE_API 
    virtual void unlock()
        throw( DWFException ) = 0;

protected:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFSynchronization()
        throw()
    {;}

private:

    //
    // Not Implemented
    //
    DWFSynchronization( const DWFSynchronization& );
    DWFSynchronization& operator=( const DWFSynchronization& );
};

}


//
// include platform-specified headers
//

#ifdef  _DWFCORE_WIN32_SYSTEM
#include "dwfcore/win32/Mutex.h"
#include "dwfcore/win32/Signal.h"
#include "dwfcore/win32/Semaphore.h"
#include "dwfcore/win32/Thread.h"

#else
#include "dwfcore/ansi/Mutex.h"
#include "dwfcore/ansi/Signal.h"
#include "dwfcore/ansi/Semaphore.h"
#include "dwfcore/ansi/Thread.h"

#endif

#endif


