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


#ifndef _DWFCORE_SEMAPHORE_ANSI_H
#define _DWFCORE_SEMAPHORE_ANSI_H

///
///\file        dwfcore/ansi/Semaphore.h
///\brief       This header contains a DWFSemaphore class declaration.
///
///\note        No documentation will be generated from this file due to class name collision, 
///             such is the nature of Doyxgen.
///             Please refer to DWFCore::DWFSemaphore documented in \a dwfcore/win32/Semaphore.h as the class
///             declarations are identical.
///
///\warning     This file should never be included directly.  
///             Instead, include \a dwfcore/Synchronization.h and all 
///             related header files, appropriate with the build configuration,
///             will be included.
///


#include "dwfcore/Core.h"

#ifdef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY

#ifndef _DWFCORE_SYNCHRONIZATION_H
#error  This header should not be included directly - use "dwfcore/Synchronization.h" instead
#endif

#ifdef _DWFCORE_MAC_SYSTEM
#include <mach/mach_init.h>
#include <mach/task.h>
#include <mach/semaphore.h>
typedef semaphore_t sem_t;
#else
#include <semaphore.h>
#endif


namespace DWFCore
{

///
///\class           DWFSemaphore   dwfcore/ansi/Semaphore.h  "dwfcore/ansi/Semaphore.h"
///\brief           A synchronization object for counted, shared resources.
///\since           1.0.1
///
///\note            This class contains platform-specific data structures but yet still
///                 may be used in portable applications via the DWFSynchronization interface.
///
///\ingroup         CrossPlatformInterface
///
class DWFSemaphore : public DWFSynchronization
{

public:

    ///
    ///             Constructor
    ///
    ///\param       nCount  The maximum number of thread locks that can be obtained from the object.
    ///\throw       None
    ///
    _DWFCORE_API DWFSemaphore( unsigned int nCount )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    /// 
    _DWFCORE_API virtual ~DWFSemaphore()
        throw();

    //
    // Initialize the semaphore
    //
    _DWFCORE_API void init()
        throw( DWFException );

    //
    // Destroy the semaphore
    //
    _DWFCORE_API void destroy()
        throw( DWFException );

    //
    // Blocking call to acquire ownership of the semaphore
    //
    _DWFCORE_API void lock()
        throw( DWFException );

    //
    // Non-blocking call to acquire ownership of the semaphore
    //
    _DWFCORE_API bool trylock()
        throw( DWFException );

    //
    // Release ownership of the semaphore
    //
    _DWFCORE_API void unlock()
        throw( DWFException );

private:

    bool         _bInit;
    unsigned int _nCount;
    sem_t        _tSemaphore;

private:

    DWFSemaphore( const DWFSemaphore& );
    DWFSemaphore& operator=( const DWFSemaphore& );
};

}


#else
#error  This header file is incompatible with your current system configuration
#endif

#endif


