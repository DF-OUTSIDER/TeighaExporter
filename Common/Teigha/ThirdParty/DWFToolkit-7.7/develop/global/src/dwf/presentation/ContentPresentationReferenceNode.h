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

//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/presentation/ContentPresentationReferenceNode.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $

#ifndef _DWFTK_CONTENT_PRESENTATION_REFERENCE_NODE_H
#define _DWFTK_CONTENT_PRESENTATION_REFERENCE_NODE_H


///
///\file        dwf/presentation/ContentPresentationReferenceNode.h
///\brief       This file contains the DWFPresentationReferenceNode class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/presentation/ContentPresentationNode.h"

namespace DWFToolkit
{

//
// fwd decl
//
class DWFContentElement;
class DWFResource;
class DWFPropertyReference;

///
///\ingroup     dwfpresentation
///
///\class       DWFContentPresentationReferenceNode     dwf/package/ContentPresentationReferenceNode.h      "dwf/package/ContentPresentationReferenceNode.h"
///\brief       The DWFContentPresentationReferenceNode class allows references to other dwf elements. 
///\since       7.2
///
///             The DWFContentPresentationReferenceNode class allows references to other dwf elements.
///             Specifically, content elements, resources and URIs can be referred to from reference nodes.
///             In addition, if the containing view has property references, URIs for properties that correspond
///             to this node can also be specified.
///

class DWFContentPresentationReferenceNode : public DWFContentPresentationNode
                                            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
    friend class DWFContentPresentationResource;
    friend class DWFContentPresentationReader;
public:

    ///
    ///         Constructor
    ///
    ///\param   zLabel  An optional user-friendly label for this node.
    ///\param   zID     A unique string identifying this node. If not provided, one will be assigned.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationReferenceNode( const DWFString& zLabel = /*NOXLATE*/L"",
                                         const DWFString& zID = /*NOXLATE*/L"" )
        throw();
    ///
    ///         Destructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationReferenceNode()
        throw();

    ///
    ///         Sets a reference to a content element
    ///
    ///\param   rContentElement       The element to which a reference is being set
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setContentElement( const DWFContentElement& rContentElement )
        throw();

    ///
    ///         Gets the id of the content element referenced by this node.
    ///         This can be the null string during publishing if the element
    ///         does not have an id assigned yet. During read, this can be
    ///         null if no content element reference has been set.
    ///
    ///\return  The ID of the content element
    ///\throw   None
    ///
    virtual const DWFString& contentElementID() const
        throw()
    {
        return _zContentElementID;
    }

    ///
    ///         Sets a reference to a DWF resource
    ///
    ///\param   rResource           The resource to which a reference is being set
    ///\param   rContainingSection  The section containing the resource.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setResource( DWFResource& rResource, DWFSection& rContainingSection )
        throw();

    ///
    ///         Sets a reference to a DWF resource
    ///
    ///\param   rResource       The resource to which a reference is being set.
    ///                         The resource is must be in the section containing
    ///                         the presentation.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setResource( DWFResource& rResource )
        throw();

    ///
    ///         Sets a reference to a DWF resource
    ///
    ///\param   rResourceURI    The uri to the resource to which a reference is being set.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setResource( const DWFString& rResourceURI )
        throw();

    ///
    ///         Gets the id of the resource referenced by this node.
    ///         This can be the null string during publishing if the resource
    ///         does not have an id assigned yet. During read, this can be
    ///         null if no resource reference has been set.
    ///
    ///\return  The ID of the resource
    ///\throw   None
    ///
    virtual const DWFString& resourceURI() const
        throw()
    {
        return _zResourceURI;
    }

    ///
    ///         Sets the URI for a property reference corresponding to this
    ///         node. The property reference must be contained by the
    ///         presentation view containing this node.
    ///
    ///\param   rPropertyReference      The property reference
    ///\param   zURI                    The string containing the URI
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setPropertyReferenceURI( DWFPropertyReference& rPropertyReference,
                                          const DWFString& zURI )
        throw(DWFException);

    ///
    ///         Sets the URI for a property reference corresponding to this
    ///         node. The property reference must be contained by the
    ///         presentation view containing this node.
    ///
    ///\param   zID                     The ID of the property reference
    ///\param   zURI                    The string containing the URI
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setPropertyReferenceURI( const DWFString& zID,
                                          const DWFString& zURI )
        throw(DWFException);

    ///
    ///         Gets the URI reference for the property reference corresponding
    ///         to this row. This can be the null string if no URI reference
    ///         has been set.
    ///
    ///\return  The string containing the URI
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& getPropertyReferenceURI( const DWFPropertyReference& rPropertyReference ) const
        throw();

    ///
    ///         Sets a reference using a URI 
    ///
    ///\param   zURI       The URI of the reference
    ///\throw   None
    ///
    virtual void setURI( const DWFString& zURI )
        throw()
    {
        _zURI = zURI;
    }

    ///
    ///         Gets the URI reference. This can be the null string if no URI
    ///         reference has been set.
    ///
    ///\return  The string containing the URI
    ///\throw   None
    ///
    virtual const DWFString& uri() const
        throw()
    {
        return _zURI;
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
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

private:
    void serializeAttributes( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

private:

    DWFResource* _containingResource();
    void _setContentElement( const DWFString& zID )
        throw()
    {
        _zContentElementID = zID;
    }


private:

    typedef DWFSkipList<DWFPropertyReference*, DWFString>       _tPropertyReferenceURIMap;
    typedef DWFStringKeySkipList<DWFString>                     _tPropertyReferenceIDURIMap;


    const DWFContentElement*                        _pContentElement;
    DWFString                                       _zContentElementID;

    DWFString                                       _zResourceURI;
    DWFResource*                                    _pResource;
    DWFSection*                                     _pSectionContainingResource;

    DWFString                                       _zURI;

    _tPropertyReferenceURIMap                       _oPRMap;
    _tPropertyReferenceIDURIMap                     _oPRIDMap;

    DWFString                                       _zEmptyString;
};

}

#endif
