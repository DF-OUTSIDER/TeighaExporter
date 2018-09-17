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


#ifndef _DWFCORE_MUTEX_ANSI_H
#define _DWFCORE_MUTEX_ANSI_H

///
///\file        dwfcore/ansi/Mutex.h
///\brief       This header contains a POSIX-based DWFThreadMutex class declaration.
///
///\note        No documentation will be generated from this file due to class name collision, 
///             such is the nature of Doyxgen.
///             Please refer to DWFCore::DWFThreadMutex documented in \a dwfcore/win32/Mutex.h as the class
///             declarations are identical.
///\note        There is no POSIX-based or otherwise ANSI-compliant DWFCore::DWFProcessMutex in the library.
///
///\warning     This file should never be included directly.  
///             Instead, include \a dwfcore/Synchronization.h and all 
///             related header files, appropriate with the build configuration,
///             will be included.
///
///\todo        Move from dwfcore/ansi/Mutex.h to dwfcore/posix/Mutex.h
///\todo        Implement DWFCore::DWFProcessMutex for all platforms (POSIX currently does not support this behavior.)
///

#include "dwfcore/Core.h"

#ifdef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY

#ifndef _DWFCORE_SYNCHRONIZATION_H
#error  This header should not be included directly - use "dwfcore/Synchronization.h" instead
#endif

//
// POSIX threads 
//
#include <pthread.h>



namespace DWFCore
{

///
///\class           DWFThreadMutex   dwfcore/ansi/Mutex.h  "dwfcore/ansi/Mutex.h"
///\brief           A light weight inter-process synchronization object using the POSIX thread library.
///\since           1.0.1
///
///                 This mutex is required only to provide synchronicity across threads within a single process.
///                 This particular implementation uses the POSIX mutex which currently only able to act in this capacity.
///
///\note            This class contains platform-specific data structures but yet still
///                 may be used in portable applications via the DWFSynchronization interface.
///
///\ingroup         CrossPlatformInterface
///
class DWFThreadMutex : public DWFSynchronization
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    DWFThreadMutex()
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    virtual ~DWFThreadMutex()
        throw();

    //
    // Initialize the mutex
    //
    _DWFCORE_API void init()
        throw( DWFException );

    //
    // Destroy the synchronization object
    //
    _DWFCORE_API void destroy()
        throw( DWFException );

    //
    // Blocking call to acquire ownership of the mutex
    //
    _DWFCORE_API void lock()
        throw( DWFException );

    //
    // Non-blocking call to acquire ownership of the mutex
    //
    _DWFCORE_API bool trylock()
        throw( DWFException );

    //
    // Release ownership of the mutex
    //
    _DWFCORE_API void unlock()
        throw( DWFException );

private:

    bool                   _bInit;
    pthread_mutex_t        _tMutex;

private:

    //
    // Not Implemented
    //
    DWFThreadMutex( const DWFThreadMutex& );
    DWFThreadMutex& operator=( const DWFThreadMutex& );
};


//
// no implementation for cross-process mutex yet
//

}


#else
#error  This header file is incompatible with your current system configuration
#endif

#endif


