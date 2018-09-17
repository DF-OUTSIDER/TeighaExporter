//
//  Copyright (c) 2005-2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/SectionContentResource.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//


#ifndef _DWFTK_SECTION_CONTENT_RESOURCE_H
#define _DWFTK_SECTION_CONTENT_RESOURCE_H

///
///\file        dwf/package/SectionContentResource.h
///\brief       This file contains the DWFSectionContentResource class declaration.
///


#include "dwf/Toolkit.h"
#include "dwf/package/Resource.h"

namespace DWFToolkit
{

//
// fwd declarations
//
class DWFContent;


///
///\ingroup     dwfpackage
///
///\class       DWFSectionContentResource   dwf/package/SectionContentResource.h     "dwf/package/SectionContentResource.h"
///\brief       This class is used to publish and read the DWFInstance data related to a section.
///\since       7.2.0
///
///             This resource allows the section to publish and parse the resource-related
///             instance data stored in the associated content library.
///
class DWFSectionContentResource : public DWFResource
                                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\param   pContent    The content library in which the resource instances are
    ///                     stored.
    ///\throw   DWFException is thrown if pContent is NULL.
    ///
    _DWFTK_API
    DWFSectionContentResource( DWFContent* pContent )
        throw( DWFException );

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
    DWFSectionContentResource( DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSectionContentResource()
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList(const char** ppAttributeList)
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         A section content resource should only have one content associated
    ///         with it. This method provides direct access to the one and only
    ///         content ID in this resource
    ///
    _DWFTK_API
    DWFString contentID() const
        throw()
    {
        return _oContentID[0];
    }

    ///
    ///\copydoc DWFResource::setObjectID(const DWFString&)
    ///
    _DWFTK_API
    virtual void setObjectID( const DWFString& zObjectID )
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///         Provides a stream for reading the resource data.
    ///
    ///         This method implementation differs from the base class
    ///         in that an in-memory representation of the entire
    ///         document is generated on the fly from the current
    ///         state of the section specific content information.
    ///
    ///\param   bCache  This parameter is ignored.
    ///
    ///\return  A pointer to a data stream.
    ///         The caller is responsible for releasing the pointer
    ///         with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFInputStream* getInputStream( bool bCache = false )
        throw( DWFException );
	
    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

    ///
    ///         Access associated content.
    ///
    ///\return  A pointer to the assocaited content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContent* getContent() const
        throw();
protected:

    //DWFOwner
    _DWFTK_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

private:

    //
    // Pointer to the content that the resource is associated with.
    //
    DWFContent*     _pContent;

    //
    // Version of the section content XML getting written out
    //
    DWFString       _zVersion;

    //
    // Buffer to which the XML is written for serialization of this content.
    //
    char*           _pBuffer;
	
	//
	// Variable to determine if the data is already serialized to XML
	//
	bool			_bSerialized;

};

}

#endif
