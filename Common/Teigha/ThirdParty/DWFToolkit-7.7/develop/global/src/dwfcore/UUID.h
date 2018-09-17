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

#ifndef _DWFCORE_UUID_H
#define _DWFCORE_UUID_H

///
///\file        dwfcore/UUID.h
///\brief       This header contains the declaration for the DWFUUID class
///             the the non-final implementation interface it uses.
///

///
///\example     UUID/UUID.cpp
///             This sample program demonstates uuid generation with the DWFCore::DWFUUID class
///             and was used as a smoke test for algorithm implementation and duplicate detection.  
///
///             Also shown in the sample:
///                 - DWFCore::DWFString
///                     -  DWFCore::DWFString::EncodeBase64()
///                 - DWFCore::DWFStringKeySkipList
///


#include "dwfcore/Core.h"
#include "dwfcore/Exception.h"
#include "dwfcore/String.h"



namespace DWFCore
{

///
///\interface       DWFUUIDImpl   dwfcore/UUID.h  "dwfcore/UUID.h"
///\brief           This interface is used by DWFUUID to support
///                 different uuid generation algorithms.
///\since           1.0.1
///
class DWFUUIDImpl
{

public:

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFUUIDImpl()
        throw()
    {;}

    ///
    ///             Create a copy of the generator object.
    ///
    ///\return      A pointer to a new object.  This pointer must be deleted
    ///             with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw       DWFException
    ///
    virtual DWFUUIDImpl* clone() const
        throw( DWFException ) = 0;

    ///
    ///             Returns a string representation of the current uuid.
    /// 
    ///\param       bSquash     if \e true, the 128-bit uuid will be Base64 encoded
    ///                         into a short form string representation, otherwise the 
    ///                         standard form string representation will be returned.
    ///                         The encoding used for the short form string will be
    ///                         non RFC 1521 compliant as detailed in \a DWFString::EncodeBase64().
    ///
    ///\return      A string representation of the uuid.
    ///\throw       DWFException
    ///
    virtual const DWFString& uuid( bool bSquash )
        throw( DWFException ) = 0;

    ///
    ///             Modifies the uuid by incrementing the timestamp portion
    ///             of the current value.  This method allows for the creation
    ///             of sequential uuids that are statistically only slightly less
    ///             unique that the original.
    ///
    ///\note        The process will only increment the 32-bit timestamp portion of the 
    ///             128-bit uuid.  As a rule, once this value reaches the \b 0xffffffee
    ///             limit, a brand new uuid value will be generated instead.
    /// 
    ///\param       bSquash     if \e true, the 128-bit uuid will be Base64 encoded
    ///                         into a short form string representation, otherwise the 
    ///                         standard form string representation will be returned.
    ///                         The encoding used for the short form string will be
    ///                         non RFC 1521 compliant as detailed in \a DWFString::EncodeBase64().
    ///
    ///\return      A string representation of the uuid.
    ///\throw       DWFException
    ///
    virtual const DWFString& next( bool bSquash )
        throw( DWFException ) = 0;

protected:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFUUIDImpl()
        throw()
    {;}
};

///
///\class           DWFUUID   dwfcore/UUID.h  "dwfcore/UUID.h"
///\brief           This class represents a 128-bit uuid value.
///\since           1.0.1
///
class DWFUUID : virtual public DWFCoreMemory
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFUUID()
        throw();

    ///
    ///             Copy Constructor
    ///
    ///\param       rUUID   Contains the uuid value to copy.
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFUUID( const DWFUUID& rUUID )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    ~DWFUUID()
        throw();

    ///
    ///             Assignment Operator
    ///
    ///\param       rUUID   Contains the uuid value to copy.
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFUUID& operator=( const DWFUUID& rUUID )
        throw();

    ///
    ///\copydoc     DWFUUIDImpl::uuid()
    ///
    _DWFCORE_API 
    const DWFString& uuid( bool bSquash )
        throw( DWFException );

    ///
    ///\copydoc     DWFUUIDImpl::next()
    ///
    _DWFCORE_API 
    const DWFString& next( bool bSquash )
        throw( DWFException );

    ///
    ///             Generates a new uuid by incrementing the timestamp portion
    ///             of the current value.  This method allows for the creation
    ///             of sequential uuids that are statistically only slightly less
    ///             unique that the original.
    ///
    ///\note        This method does \b not alter the object.  Instead it generates
    ///             a new one.
    ///
    ///\note        The process will only increment the 32-bit timestamp portion of the 
    ///             128-bit uuid.  As a rule, once this value reaches the \b 0xffffffee
    ///             limit, a brand new uuid value will be generated.
    /// 
    ///\return      A new object containing the next uuid in the sequence.
    ///\throw       DWFException
    ///
    _DWFCORE_API 
    DWFUUID next()
        throw( DWFException );

private:

    DWFUUIDImpl*    _pImpl;
};

}

#endif
