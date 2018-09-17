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


#ifndef _DWFCORE_MUTEX_WIN32_H
#define _DWFCORE_MUTEX_WIN32_H


///
///\file        dwfcore/win32/Mutex.h
///\brief       This header contains the DWFThreadMutex and DWFProcessMutex
///             class declarations for Microsoft Windows platforms.
///
///\warning     This file should never be included directly.  
///             Instead, include \a dwfcore/Synchronization.h and all 
///             related header files, appropriate with the build configuration,
///             will be included.
///
///\todo        Change to a non-final implementation pattern where DWFCore::DWFThreadMutex
///             and DWFCore::DWFProcessMutex are declared in a common header file.  
///             These generic classes will then use a platform-specific
///             implementation object (Win32, POSIX, etc) to deliver the
///             required functional behavior.  
///


#include "dwfcore/Core.h"

#ifdef  _DWFCORE_WIN32_SYSTEM

#ifndef _DWFCORE_SYNCHRONIZATION_H
#error  This header should not be included directly - use "core/Synchronization.h" instead
#endif


namespace DWFCore
{

///
///\class           DWFThreadMutex   dwfcore/win32/Mutex.h  "dwfcore/win32/Mutex.h"
///\brief           A light weight inter-process synchronization object for Microsoft Windows platforms.
///\since           1.0.1
///
///                 This mutex is required only to provide synchronicity across threads within a single process.
///                 This particular implementation uses the Win32 CRITICAL_SECTION which is far more efficient
///                 than the kernel mutex.
///
///\note            This class contains platform-specific data structures but yet still
///                 may be used in portable applications via the DWFSynchronization interface.
///
///\ingroup         CrossPlatformInterface
///
class DWFThreadMutex : virtual public DWFCoreMemory
                     , public DWFSynchronization
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
    _DWFCORE_API 
    void init()
        throw( DWFException );

    //
    // Destroy the mutex
    //
    _DWFCORE_API 
    void destroy()
        throw( DWFException );

    //
    // Blocking call to acquire ownership of the mutex
    //
    _DWFCORE_API 
    void lock()
        throw( DWFException );

    //
    // Non-blocking call to acquire ownership of the mutex
    //
    _DWFCORE_API 
    bool trylock()
        throw( DWFException );

    //
    // Release ownership of the mutex
    //
    _DWFCORE_API 
    void unlock()
        throw( DWFException );

private:

    bool                _bInit;
    CRITICAL_SECTION    _tSection;

private:

    DWFThreadMutex( const DWFThreadMutex& );
    DWFThreadMutex& operator=( const DWFThreadMutex& );
};


///
///\class           DWFProcessMutex   dwfcore/win32/Mutex.h  "dwfcore/win32/Mutex.h"
///\brief           A intra-process synchronization object for Microsoft Windows platforms.
///\since           1.0.1
///
///                 This mutex is required to provide synchronicity across processes. 
///                 This particular implementation uses the Win32 kernel mutex.  A cross
///                 process mutex must be uniquely named so it may be acquired with out
///                 relying on any form of inter-process communication.
///
///\note            This class contains platform-specific data structures but yet still
///                 may be used in portable applications via the DWFSynchronization interface.
///
///\ingroup         CrossPlatformInterface
///
class DWFProcessMutex : virtual public DWFCoreMemory
                      , public DWFSynchronization
{

public:

    ///
    ///             Constructor
    ///
    ///\param       zName   The globally available, unique name of this mutex.
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFProcessMutex( const DWFString& zName )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    virtual ~DWFProcessMutex()
        throw();

    //
    // Initialize the mutex
    //
    _DWFCORE_API 
    void init()
        throw( DWFException );

    //
    // Destroy the mutex
    //
    _DWFCORE_API 
    void destroy()
        throw( DWFException );

    //
    // Blocking call to acquire ownership of the mutex
    //
    _DWFCORE_API 
    void lock()
        throw( DWFException );

    //
    // Non-blocking call to acquire ownership of the mutex
    //
    _DWFCORE_API 
    bool trylock()
        throw( DWFException );

    //
    // Release ownership of the mutex
    //
    _DWFCORE_API 
    void unlock()
        throw( DWFException );

    ///
    ///             Returns the name of the mutex.
    ///
    ///\return      The name of the mutex.
    ///\throw       None
    ///
    _DWFCORE_API 
    const DWFString& name() const
        throw();

private:

    bool        _bInit;
    HANDLE      _hMutex;
    DWFString   _zName;

private:

    //
    // Not Implemented
    //

    DWFProcessMutex( const DWFProcessMutex& );
    DWFProcessMutex& operator=( const DWFProcessMutex& );
};

}


#else
#error  This is a Win32 header file and is incompatible with your current system configuration
#endif 

#endif


