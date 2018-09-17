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


#ifndef _DWFCORE_SIGNAL_ANSI_H
#define _DWFCORE_SIGNAL_ANSI_H


///
///\file        dwfcore/ansi/Signal.h
///\brief       This header contains a DWFSignal class declaration.
///
///\note        No documentation will be generated from this file due to class name collision, 
///             such is the nature of Doyxgen.
///             Please refer to DWFCore::DWFSemaphore documented in \a dwfcore/win32/Signal.h as the class
///             declarations are identical.
///
///\warning     This file should never be included directly.  
///             Instead, include \a dwfcore/Synchronization.h and all 
///             related header files, appropriate with the build configuration,
///             will be included.
///
///\todo        Move from dwfcore/ansi/Signal.h to dwfcore/posix/Signal.h
///

#include "dwfcore/Core.h"

#ifdef  _DWFCORE_FOR_ANSI_SYSTEM_ONLY

#include "dwfcore/Exception.h"

//
// POSIX threads 
//
#include <pthread.h>

//
// Platform independent core library
//
namespace DWFCore
{

///
///\class           DWFSignal   dwfcore/ansi/Signal.h  "dwfcore/ansi/Signal.h"
///\brief           An POSIX-based inter-thread notification synchronization object.
///\since           1.0.1
///
class DWFSignal
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API DWFSignal()
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API virtual ~DWFSignal()
        throw();

    ///
    ///             Initialize the signal.
    ///             A client must always invoke this method before using this object.
    ///
    ///\throw       DWFException
    ///
    _DWFCORE_API void init()
        throw( DWFException );

    ///
    ///             Destroy the signal.
    ///             A client must always invoke this method after using this object.
    ///
    ///\throw       DWFException
    ///
    _DWFCORE_API void destroy()
        throw( DWFException );

    ///
    ///             Blocking call to wait for the signal to be raised.
    ///             All blocked threads will be released on \a raise().
    ///
    ///\param       nMilliseconds   The amount of time to wait for the signal
    ///                             to be raised. Specify \b 0 milliseconds to wait indefinitely.
    ///\return      \e true if the signal was raised, \e false if the wait timed out.
    ///\throw       DWFException
    ///
    _DWFCORE_API bool wait( unsigned long nMilliseconds = 0 )
        throw( DWFException );

    ///
    ///             Raises the signal releasing any threads blocking on wait().
    ///
    ///\throw       DWFException
    ///
    _DWFCORE_API void raise()
        throw( DWFException );

private:

    bool               _bInit;
    pthread_cond_t     _tCondition;
    pthread_mutex_t    _tMutex;

private:

    //
    // Not Implemented
    //
    DWFSignal( const DWFSignal& );
    DWFSignal& operator=( const DWFSignal& );
};


}


#else
#error  This header file is incompatible with your current system configuration
#endif

#endif


