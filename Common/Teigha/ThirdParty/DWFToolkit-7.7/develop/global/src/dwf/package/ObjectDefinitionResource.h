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


#ifndef _DWFTK_OBJECT_DEFINITION_RESOURCE_H
#define _DWFTK_OBJECT_DEFINITION_RESOURCE_H


///
///\file        dwf/package/ObjectDefinitionResource.h
///\brief       This file contains the DWFObjectDefinitionResource class declaration.
///


#include "dwf/Toolkit.h"
#include "dwf/package/Resource.h"
#include "dwf/package/utility/DefinedObjectContainer.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFObjectDefinitionResource   dwf/package/ObjectDefinitionResource.h     "dwf/package/ObjectDefinitionResource.h"
///\brief       This class is used to create and publish an object definition document.
///\since       7.0.1
///
///             This resource class is slightly different from the others in the toolkit
///             in that it is only ever created by the publishing application.
///             This allows the publisher to associate different object definitions
///             with other resources in the same section; specifically, in most cases,
///             tying object instances to graphic nodes.
///
///\todo        While resources of this type are not created by the parsing process due
///             to efficiency issues (on-demand object/property resolution), it might
///             be more consistent to merge this object and the DWFObjectDefinition together.
///             
///
class DWFObjectDefinitionResource : public DWFResource
                                  , public DWFDefinedObjectContainer
                                  , public DWFDefinedObjectInstanceContainer
                                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\param   zType   The root element defining the document type.
    ///                 Document types known (but not restricted) to the toolkit currently are:
    ///                 - DWFXML::kzElement_ObjectDefinition
    ///                 - DWFXML::kzElement_PageObjectDefinition
    ///                 - DWFXML::kzElement_SpaceObjectDefinition
    ///                 - DWFXML::kzElement_GlobalObjectDefinition
    ///
    ///\param   zRole   Describes the role of the document.
    ///                 Roles known (but not restricted) to the toolkit currently are:
    ///                 - DWFXML::kzRole_ObjectDefinition
    ///                 - DWFXML::kzRole_MarkupObjectDefinition
    ///\throw   None
    ///
    _DWFTK_API
    DWFObjectDefinitionResource( const DWFString& zType,
                                 const DWFString& zRole )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFObjectDefinitionResource()
        throw();


#ifndef DWFTK_READ_ONLY

    //
    // Returns a stream for reading the resource data
    // The caller is responsible for releasing the pointer
    // with the \b DWFCORE_FREE_OBJECT macro.
    // 
    // This resource object is a bit special since it's created
    // on-demand when a section with defined objects is serialized.
    // This method is overridden in order to provide a real byte stream
    // for the package writer to use to create the object defintion document
    //
    // bCache is ignored

    ///
    ///         Provides a stream for reading the resource data.
    ///
    ///         This method implementation differs from the base class
    ///         in that an in-memory representation of the entire
    ///         document is generated on the fly from the current
    ///         state of the contained objects and instances.
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
    ///\copydoc DWFXMLSerializable::addNamespace()
    ///
    _DWFTK_API
    const DWFXMLNamespace& addNamespace( const DWFString& zNamespace, 
                                         const DWFString& zXMLNS )
        throw( DWFException );

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

private:

    typedef std::map<DWFString, DWFXMLNamespace> _tNamespaceMap;
    _tNamespaceMap                               _oAddedNamespaces;

#endif

private:

    DWFString   _zType;
    char*       _pBuffer;

private:

    DWFObjectDefinitionResource( const DWFObjectDefinitionResource& );
    DWFObjectDefinitionResource& operator=( const DWFObjectDefinitionResource& );
};


}

#endif
