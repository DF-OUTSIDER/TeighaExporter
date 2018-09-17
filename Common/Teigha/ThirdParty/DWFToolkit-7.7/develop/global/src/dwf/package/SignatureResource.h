//
//  Copyright (c) 1996-2005 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#ifndef _DWFTK_SIGNATURE_RESOURCE_H
#define _DWFTK_SIGNATURE_RESOURCE_H

///
///\file        dwf/package/SignatureResource.h
///\brief       This file contains the DWFSignatureResource class declaration.
///


#include "dwf/Toolkit.h"
#include "dwf/package/Resource.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFSignatureResource   dwf/package/SignatureResource.h     "dwf/package/SignatureResource.h"
///\brief       This class represents a digital signature resource.
///\since       7.3
///
///             Digital signatures are ...
///
class DWFSignatureResource : public DWFResource
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:


public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read resource content
    ///         data from the DWF package.
    ///
    ///\param   pPackageReader  Provides access to resource content in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSignatureResource( DWFPackageReader*  pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read resource content
    ///         data from the DWF package.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFSignatureResource()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSignatureResource()
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif


private:
        

private:

    DWFSignatureResource( const DWFSignatureResource& );
    DWFSignatureResource& operator=( const DWFSignatureResource& );
};


}

#endif
