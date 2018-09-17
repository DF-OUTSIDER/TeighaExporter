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

#ifndef _DWFCORE_UUID_ANSI_H
#define _DWFCORE_UUID_ANSI_H


///
///\file        dwfcore/ansi/UUID.h
///\brief       This header contains the declaration of the DWFUUIDImpl_ANSI implementation class.
///


#include "dwfcore/UUID.h"

#ifdef  HAVE_CONFIG_H
#include "dwfcore/config.h"
#endif

    //
    // this system has libuuid
    //
#ifdef  HAVE_LIBUUID
#include <uuid/uuid.h>
#endif



namespace DWFCore
{

///
///\interface       DWFUUIDImpl_ANSI   dwfcore/ansi/UUID.h  "dwfcore/ansi/UUID.h"
///\brief           Implementation class for generating uuids on all platforms.
///\since           1.0.1
///
///                 If available, \e libuuid will be used to generate these values;
///                 otherwise, a generic algorithm will be used.
///
///\note            This object is created internally by DWFUUID.
///
class DWFUUIDImpl_ANSI  : public DWFUUIDImpl
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFUUIDImpl_ANSI()
        throw();

    ///
    ///             Copy Constructor
    ///
    ///\param       rUUID   The source from which to copy the uuid value.
    ///\throw       None
    ///
    DWFUUIDImpl_ANSI( const DWFUUIDImpl_ANSI& rUUID )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFUUIDImpl_ANSI()
        throw();

    ///
    ///             Assignment Operator
    ///
    ///\param       rUUID   The source from which to copy the uuid value.
    ///\throw       None
    ///
    DWFUUIDImpl_ANSI& operator=( const DWFUUIDImpl_ANSI& rUUID )
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

    void _generate()
        throw();

private:

    DWFString _zUUID;

    //
    // this system has libuuid
    //
#ifdef  HAVE_LIBUUID

    uuid_t      _tUUID;

#else

    typedef unsigned char   tUUID[16];

    tUUID     _tUUID;

#endif

};

}


#endif


