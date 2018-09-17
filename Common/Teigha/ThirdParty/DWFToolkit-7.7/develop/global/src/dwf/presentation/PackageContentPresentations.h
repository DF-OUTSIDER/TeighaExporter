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


#ifndef _DWFTK_PACKAGECONTENTPRESENTATIONS_H
#define _DWFTK_PACKAGECONTENTPRESENTATIONS_H

///
///\file        dwf/package/PackageContentPresentations.h
///\brief       This file contains the DWFPackageContentPresentations class declaration.
///

#include "dwfcore/XMLParser.h"

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/presentation/ContentPresentationDocument.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpresentation
///
///\class       DWFPackageContentPresentations   dwf/presentation/PackageContentPresentations.h     "dwf/presentation/PackageContentPresentations.h"
///\brief       This class contains the package level content presentations.
///\since       7.3
///
class DWFPackageContentPresentations : public DWFXMLBuildable
                                     , public DWFContentPresentationDocument
#ifndef DWFTK_READ_ONLY
                                     , public DWFXMLSerializable
#endif
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
    DWFPackageContentPresentations( DWFPackageReader*  pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFPackageContentPresentations()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPackageContentPresentations()
        throw();

    ///
    ///         Sets the href of the content.
    ///
    ///\param   zHRef   The href of the content
    ///\return  None
    ///\throw   None
    ///
    void setHRef( const DWFString& zHRef)
        throw()
    {
        _zHRef = zHRef;
    }

    ///
    ///         Returns the location of the package presentation document.
    ///
    ///\return  The HREF.
    ///\throw   None
    ///
    const DWFString& href() const
    {
        return _zHRef;
    }

    ///
    ///         The presentation input stream.
    ///
    ///\return  The input stream for the presentation.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFInputStream* getInputStream()
        throw( DWFException );

    ///
    ///         This parses the package content presentation document using the default reader
    ///         If a filter is provided, it will be set on the default reader.
    ///         If the content has already been loaded, this will do nothing
    ///
    ///\param   pReaderFilter   If provided, this will be set on the default reader.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void load( DWFContentPresentationReader* pReaderFilter = NULL )
        throw( DWFException );

    ///
    ///         This parses the package content presentation document using the provided reader.
    ///         This allows the user to directly participate in the parsing process.
    ///         No in-memory representation of the content will be created or cached.
    ///
    ///\param   rCustomReader   The custom reader.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void load( DWFContentPresentationReader& rCustomReader )
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** /*ppAttributeList*/ )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

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

    _DWFTK_API
    void _parseDocument( DWFInputStream& rDocumentStream,
                         DWFXMLCallback& rDocumentReader )
        throw( DWFException );

    ///
    ///         Sets the container HREF.
    ///
    ///\return  The HREF.
    ///\throw   None
    ///
    void SetHRef( const DWFString& zID )
        throw()
    {
        _zHRef.assign( zID );
        _zHRef.append( L".presentations.xml" );
    }

private:
    DWFString           _zHRef;
    DWFPackageReader*   _pPackageReader;
    bool                _bLoaded;

    DWFPackageContentPresentations( const DWFPackageContentPresentations& );
    DWFPackageContentPresentations& operator=( const DWFPackageContentPresentations& );
};

}

#endif
