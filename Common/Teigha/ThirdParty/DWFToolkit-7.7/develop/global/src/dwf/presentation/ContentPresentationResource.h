//
//  Copyright (c) 2006 by Autodesk, Inc.
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


#ifndef _DWFTK_CONTENT_PRESENTATION_RESOURCE_H
#define _DWFTK_CONTENT_PRESENTATION_RESOURCE_H


///
///\file        dwf/presentation/ContentPresentationResource.h
///\brief       This file contains the DWFContentPresentationResource class declaration.
///


#include "dwf/Toolkit.h"
#include "dwf/package/Resource.h"
#include "dwf/presentation/ContentPresentationDocument.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpresentation
///
///\class       DWFContentPresentationResource   dwf/package/ContentPresentationResource.h     "dwf/package/ContentPresentationResource.h"
///\brief       This class is used to create and publish presentations for a section.
///\since       7.2
///
///             This class is used to create and publish presentations for a section. This class also has the default implementation
///             for the content presentation document.
///
class DWFContentPresentationResource : public DWFResource
                                     , public DWFContentPresentationDocument
                                     _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

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
    DWFContentPresentationResource( DWFPackageReader*  pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is meant for publishers intending to include
    ///         this resource in the DWF package.
    ///
    ///\param   zRole   Describes the role of the document.
    ///                 Roles known (but not restricted) to the toolkit currently are:
    ///                 - DWFXML::kzRole_ContentPresentation
    ///                 - DWFXML::kzRole_MarkupContentPresentation
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationResource( const DWFString& zRole )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationResource()
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

    typedef std::map<DWFString, DWFXMLNamespace>    _tNamespaceMap;
    _tNamespaceMap                                  _oAddedNamespaces;

#endif

private:

    DWFString   _zType;
    char*       _pBuffer;
	bool		_bSerialized;

private:

    DWFContentPresentationResource( const DWFContentPresentationResource& );
    DWFContentPresentationResource& operator=( const DWFContentPresentationResource& );
};


}

#endif
