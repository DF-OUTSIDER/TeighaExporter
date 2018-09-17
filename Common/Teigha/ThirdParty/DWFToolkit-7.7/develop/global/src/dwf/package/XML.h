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


#ifndef _DWFTK_XML_H
#define _DWFTK_XML_H


///
///\file        dwf/package/XML.h
///\brief       This file contains the declarations of several classes used for XML processing.
///

#include "dwfcore/XML.h"
#include "dwfcore/Core.h"
#include "dwfcore/OutputStream.h"
using namespace DWFCore;


#include "dwf/Version.h"
#include "dwf/Toolkit.h"

namespace DWFToolkit
{

//
// fwd decl
//
class DWFBookmark;
class DWFDefinedObject;
class DWFDefinedObjectInstance;
class DWFDependency;
class DWFResource;
class DWFResourceRelationship;
class DWFFontResource;
class DWFGraphicResource;
class DWFImageResource;
class DWFSectionContentResource;
class DWFContentPresentationResource;
class DWFSignatureResource;
class DWFInterface;
class DWFPaper;
class DWFProperty;
class DWFSource;
class DWFUnits;
class DWFPackageReader;

class DWFContentManager;
class DWFContent;
class DWFClass;
class DWFFeature;
class DWFEntity;
class DWFObject;
class DWFGroup;
class DWFInstance;
class DWFPropertySet;
class DWFContentPresentation;
class DWFContentPresentationView;
class DWFPropertyReference;
class DWFContentPresentationNode;
class DWFContentPresentationReferenceNode;
class DWFContentPresentationModelViewNode;
class DWFCoordinateSystem;
class DWFPackageContentPresentations;


///
///\ingroup     dwfpackage
///
///\class       DWFXMLElementBuilder    dwf/package/XML.h     "dwf/package/XML.h"
///\brief       This class builds DWFXMLBuildable objects for the parser.
///\since       7.0.1
///
class DWFXMLElementBuilder _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFXMLElementBuilder()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFXMLElementBuilder()
        throw();

    ///
    ///         Builds a generic or untyped resource.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   pPackageReader      The DWF package source data I/O object.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFResource* buildResource( const char**      ppAttributeList,
                                        DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a font resource.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   pPackageReader      The DWF package source data I/O object.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFFontResource* buildFontResource( const char**      ppAttributeList,
                                                DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a image resource.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   pPackageReader      The DWF package source data I/O object.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFImageResource* buildImageResource( const char**      ppAttributeList,
                                                  DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a graphic resource.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   pPackageReader      The DWF package source data I/O object.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFGraphicResource* buildGraphicResource( const char**      ppAttributeList,
                                                      DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a section content resource resource.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   pPackageReader      The DWF package source data I/O object.
    ///\throw   DWFException
    ///
    ///\since   7.3
    ///
    _DWFTK_API
    virtual DWFSectionContentResource* buildSectionContentResource( const char**      ppAttributeList,
                                                                    DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a content presentation resource.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   pPackageReader      The DWF package source data I/O object.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFContentPresentationResource* buildContentPresentationResource( const char**      ppAttributeList,
                                                                              DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a signature resource.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   pPackageReader      The DWF package source data I/O object.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFSignatureResource* buildSignatureResource( const char**      ppAttributeList,
                                                          DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a bookmark.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFBookmark* buildBookmark( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a defined object.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFDefinedObject* buildDefinedObject( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a defined object instance.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   nSequence           Describes the order in which the instance was encountered during parsing.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFDefinedObjectInstance* buildDefinedObjectInstance( const char**  ppAttributeList,
                                                                  unsigned long nSequence )
        throw( DWFException );

    ///
    ///         Builds a dependency.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFDependency* buildDependency( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds an interface.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFInterface* buildInterface( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a paper element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFPaper* buildPaper( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a property.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFProperty* buildProperty( const char** ppAttributeList, bool bIgnoreCustomizeAttribute = false )
        throw( DWFException );

    ///
    ///         Builds a source element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFSource* buildSource( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a units element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFUnits* buildUnits( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a content.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\param   pPackageReader      The DWF package source data I/O object.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFContent* buildContent( const char** ppAttributeList,
                                      DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a class content element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content element.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFClass* buildClass( const char** ppAttributeList, 
                                  DWFXMLBuildable::tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Builds a feature content element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content element.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFFeature* buildFeature( const char** ppAttributeList, 
                                      DWFXMLBuildable::tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Builds an entity content element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content element.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFEntity* buildEntity( const char** ppAttributeList, 
                                    DWFXMLBuildable::tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Builds an object content element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content element.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFObject* buildObject( const char** ppAttributeList, 
                                    DWFXMLBuildable::tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Builds a group content element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content element.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFGroup* buildGroup( const char** ppAttributeList, 
                                  DWFXMLBuildable::tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Builds a property set.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed property set.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFPropertySet* buildPropertySet( const char** ppAttributeList, 
                                              DWFXMLBuildable::tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Builds an instance rendering an object content element.
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content element.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFInstance* buildInstance( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a content presentation element
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content presentation.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFContentPresentation* buildContentPresentation( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a content presentation view
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content presentation view.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFContentPresentationView* buildContentPresentationView( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a property reference
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed property reference.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFPropertyReference* buildPropertyReference( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a content presentation node
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content presentation node.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFContentPresentationNode* buildContentPresentationNode( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a content presentation reference node
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content presentation reference node.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFContentPresentationReferenceNode* buildContentPresentationReferenceNode( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a content presentation model view node
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed content presentation model view node.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFContentPresentationModelViewNode* buildContentPresentationModelViewNode( const char** ppAttributeList )
        throw( DWFException );

	///
    ///         Builds a coordinate system object
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed coordinate system object
    ///\throw   DWFException
    ///
    ///\since   7.3
    ///
    _DWFTK_API
    virtual DWFCoordinateSystem* buildCoordinateSystem( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         Builds a package content presentation object
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed package content presentation object
    ///\throw   DWFException
    ///
    ///\since   7.3
    ///
    _DWFTK_API
    virtual DWFPackageContentPresentations* buildPackageContentPresentations( const char** ppAttributeList,
                                                                              DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a resource relationship object
    ///
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\return  A pointer to the constructed package content presentation object
    ///\throw   DWFException
    ///
    ///\since   7.3
    ///
    _DWFTK_API
    virtual DWFResourceRelationship* buildRelationship( const char** ppAttributeList,
                                                        DWFPackageReader* pPackageReader )
        throw( DWFException );

};


////////////



///
///\ingroup     dwfpackage
///
///\class       DWFXMLNamespace    dwf/package/XML.h     "dwf/package/XML.h"
///\brief       Encapsulates a namespace used to extend elements and/or attributes 
///             added to a DWF XML document.
///\since       7.1
///
class DWFXMLNamespace : public DWFXMLNamespaceBase
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFXMLNamespace()
        throw()
        : DWFXMLNamespaceBase()
    {;}

    ///
    ///         Constructor
    ///
    ///\param   zNamespace      The namespace prefix.  This parameter is required.
    ///                         This must be a non-DWF specified namespace
    ///\param   zXMLNS          The namespace reference (should include version).  This parameter is required.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFXMLNamespace( const DWFString& zNamespace, const DWFString& zXMLNS )
        throw( DWFException );

};

////////////

#ifndef DWFTK_READ_ONLY

///
///\ingroup     dwfpackage
///
///\class       DWFXMLSerializable    dwf/package/XML.h     "dwf/package/XML.h"
///\brief       This interface is used to indicate those classes that can be
///             written as XML into a stream.
///\since       7.0.1
///
class DWFXMLSerializable : public DWFXMLSerializableBase
                           // _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER - should be, but can't, as vc7.1 has a problem in r/w dll mode w/this.
{

public:

    ///
    ///         This method can be used to determine the namespace to use
    ///         during serialization.  
    ///
    ///\param   nFlags          These options should match those passed to \a serializeXML().
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString namespaceXML( unsigned int nFlags ) const
        throw();

};

#endif

////////////

}


#endif



