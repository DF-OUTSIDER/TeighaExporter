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


#ifndef _DWFCORE_SIGNAL_WIN32_H
#define _DWFCORE_SIGNAL_WIN32_H


///
///\file        dwfcore/win32/Signal.h
///\brief       This header contains the DWFSignal class declaration for Microsoft Windows platforms.
///
///\warning     This file should never be included directly.  
///             Instead, include \a dwfcore/Synchronization.h and all 
///             related header files, appropriate with the build configuration,
///             will be included.
///
///\todo        Change to a non-final implementation pattern where DWFCore::DWFSignal is declared in a common header file.  
///             These generic classes will then use a platform-specific
///             implementation object to deliver the required functional behavior.  
///


#include "dwfcore/Core.h"

#ifdef  _DWFCORE_WIN32_SYSTEM

#include "dwfcore/Exception.h"



namespace DWFCore
{

///
///\class           DWFSignal   dwfcore/win32/Signal.h  "dwfcore/win32/Signal.h"
///\brief           An inter-thread notification synchronization object for Microsoft Windows platforms.
///\since           1.0.1
///
///                 This implementation does not provide any sort of manual state control or reset.
///                 Any number of threads my block in \a wait(), they will all be release immediately
///                 by \a raise().  Any subsequent \a wait() call will block again until the signal is re-raised.
///
///                 Example:
///                 \code
///                     (thread 1)                              (thread 2)
///                     for (i = 0; i < 10; i++)
///                     {
///                         cout << "waiting..." << endl;
///                         rSignal.wait();                     cout << "raising signal..." << endl;
///                         cout << "signalled!" << endl;       oSignal.raise();
///                     }                                       cout << "raised." << endl;
///                     cout << "thread 1 done." << endl;       cout << "thread 2 done." << endl;
///
///                     Output:
///                     
///                         waiting...
///                         raising signal...
///                         signalled!
///                         waiting...              <== thread 1 blocked again
///                         raised.
///                         thread 2 done.
///                 \endcode
///                         
///\ingroup         CrossPlatformInterface
///
class DWFSignal : virtual public DWFCoreMemory
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFSignal()
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    virtual ~DWFSignal()
        throw();

    ///
    ///             Initialize the signal.
    ///             A client must always invoke this method before using this object.
    ///
    ///\throw       DWFException
    ///
    _DWFCORE_API 
    void init()
        throw( DWFException );

    ///
    ///             Destroy the signal.
    ///             A client must always invoke this method after using this object.
    ///
    ///\throw       DWFException
    ///
    _DWFCORE_API 
    void destroy()
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
    _DWFCORE_API 
    bool wait( unsigned long nMilliseconds = 0 )
        throw( DWFException );

    ///
    ///             Raises the signal releasing any threads blocking on wait().
    ///
    ///\throw       DWFException
    ///
    _DWFCORE_API 
    void raise()
        throw( DWFException );

private:

    bool    _bInit;
    HANDLE  _hEvent;

private:

    //
    // Not Implemented
    //
    DWFSignal( const DWFSignal& );
    DWFSignal& operator=( const DWFSignal& );
};


}


#else
#error  This is a Win32 header file and is incompatible with your current system configuration
#endif 

#endif


