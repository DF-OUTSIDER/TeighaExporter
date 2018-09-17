//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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


#ifndef _DWFTK_SOURCE_H
#define _DWFTK_SOURCE_H

///
///\file        dwf/package/Source.h
///\brief       This file contains the DWFSource class declaration.
///



#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/writer/PackageWriter.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFSource   dwf/package/Source.h     "dwf/package/Source.h"
///\brief       This class is used to identify the origin of the section content.
///\since       7.0.1
///
class DWFSource : public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
                , public DWFXMLSerializable
#endif

                _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFSource()
        throw();

    ///
    ///         Constructor
    ///
    ///\param   zHRef       A locator for more information about (or the actual location of) the original document.
    ///\param   zProvider   The design or creator application or data source.
    ///\param   zObjectID   A provider-specific identifier for the source document.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSource( const DWFString& zHRef,
               const DWFString& zProvider,
               const DWFString& zObjectID )
        throw();

    ///
    ///         Copy Constructor
    ///
    ///\param   rSource     The object from which to copy data.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSource( const DWFSource& rSource )
        throw();

    ///
    ///         Assignment Operator
    ///
    ///\param   rSource     The object from which to copy data.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSource& operator=( const DWFSource& rSource )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSource()
        throw();

    ///
    ///         Returns the source HREF.
    ///
    ///\return  The HREF.
    ///\throw   None
    ///
    const DWFString& href() const
        throw()
    {
        return _zHRef;
    }

    ///
    ///         Returns the source provider.
    ///
    ///\return  The provider.
    ///\throw   None
    ///
    const DWFString& provider() const
        throw()
    {
        return _zProvider;
    }

    ///
    ///         Returns the source object ID.
    ///
    ///\return  The object ID.
    ///\throw   None
    ///
    const DWFString& objectID() const
        throw()
    {
        return _zObjectID;
    }

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

    DWFString _zHRef;
    DWFString _zProvider;
    DWFString _zObjectID;
};

}

#endif
