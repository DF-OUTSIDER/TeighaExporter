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

#ifndef _DWFCORE_UUID_WIN32_H
#define _DWFCORE_UUID_WIN32_H

///
///\file        dwfcore/win32/UUID.h
///\brief       This header contains the declaration of the DWFUUIDImpl_Win32 implementation class.
///


#include "dwfcore/UUID.h"

#ifdef  _DWFCORE_WIN32_SYSTEM

//
// UUID API provided by RPC library
//
#include <rpc.h>



namespace DWFCore
{

///
///\interface       DWFUUIDImpl_Win32   dwfcore/win32/UUID.h  "dwfcore/win32/UUID.h"
///\brief           Implementation class for generating uuids on Microsoft Windows platforms
///                 utilizing the Win32 RPC library.
///\since           1.0.1
///
///\note            This object is created internally by DWFUUID.
///
class DWFUUIDImpl_Win32 : virtual public DWFCoreMemory
                        , public DWFUUIDImpl
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFUUIDImpl_Win32()
        throw();

    ///
    ///             Copy Constructor
    ///
    ///\param       rUUID   The source from which to copy the uuid value.
    ///\throw       None
    ///
    DWFUUIDImpl_Win32( const DWFUUIDImpl_Win32& rUUID )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFUUIDImpl_Win32()
        throw();

    ///
    ///             Assignment Operator
    ///
    ///\param       rUUID   The source from which to copy the uuid value.
    ///\throw       None
    ///
    DWFUUIDImpl_Win32& operator=( const DWFUUIDImpl_Win32& rUUID )
        throw();

    ///
    ///\copydoc     DWFUUIDImpl::clone()
    ///
    DWFUUIDImpl* clone() const
        throw( DWFException );

    ///
    ///\copydoc     DWFUUIDImpl::uuid()
    ///
    const DWFString& uuid( bool bSquash )
        throw( DWFException );

    ///
    ///\copydoc     DWFUUIDImpl::next()
    ///
    const DWFString& next( bool bSquash )
        throw( DWFException );

private:

    UUID        _oUUID;
    DWFString   _zUUID;
};

}



#else
#error  This is a Win32 header file and is incompatible with your current system configuration
#endif 

#endif
