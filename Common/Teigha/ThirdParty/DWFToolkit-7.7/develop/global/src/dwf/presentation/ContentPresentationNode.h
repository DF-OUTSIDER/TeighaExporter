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

//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/presentation/ContentPresentationNode.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $

#ifndef _DWFTK_CONTENT_PRESENTATION_NODE_H
#define _DWFTK_CONTENT_PRESENTATION_NODE_H


///
///\file        dwf/presentation/ContentPresentationNode.h
///\brief       This file contains the DWFPresentationNode class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Resource.h"
#include "dwf/presentation/utility/ContentPresentationNodeContainer.h"
namespace DWFToolkit
{

//
// fwd decl
//
class DWFContentPresentationReader;
class DWFImageResource;
class DWFSection;

///
///\ingroup     dwfpresentation
///
///\class       DWFContentPresentationNode     dwf/package/ContentPresentationNode.h      "dwf/package/ContentPresentationNode.h"
///\brief       The DWFContentPresentationNode class supports label-only
///             nodes in a presentation view.
///\since       7.2
///
///             The DWFContentPresentationNode class supports label-only
///             nodes in a presentation view. Icons can be assigned to be
///             displayed along with the nodes.
///

class DWFContentPresentationNode  : public DWFContentPresentationNodeContainer
                                  , public DWFXMLBuildable
#ifndef DWFTK_READ_ONLY
                                  , public DWFXMLSerializable
#endif
                                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
    friend class DWFContentPresentationNodeContainer;

public:
    ///
    ///\brief   This type defines a list of DWFContentPresentationNode pointers.
    ///
    typedef DWFOrderedVector<DWFContentPresentationNode*>            tList;
    ///
    ///\brief   This type defines a mapped collection of DWFContentPresentationNode pointers.
    ///
    typedef DWFStringKeySkipList<DWFContentPresentationNode*>         tMap;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFContentPresentationNode pointers.
    ///
    typedef DWFIterator<DWFContentPresentationNode*>                  tIterator;


public:

    ///
    ///         Constructor
    ///
    ///\param   zLabel  An optional user-friendly label for this node.
    ///\param   zID     A unique string identifying this node. If not provided, one will be assigned.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationNode( const DWFString& zLabel = /*NOXLATE*/L"",
                                const DWFString& zID = /*NOXLATE*/L"" )
        throw();
    ///
    ///         Destructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationNode()
        throw();

    ///
    ///         Sets an id for this view.
    ///
    ///\param   zID             A string that specifies the id.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setID( const DWFString& zID )
        throw( DWFException );

    ///
    ///         Returns the ID for this node.
    ///
    ///\return  The id for this node
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& id() const
        throw();

    ///
    ///         Sets a user-friendly label for this node that 
    ///
    ///\param   zLabel       A string that specifies the label
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setLabel( const DWFString& zLabel )
        throw();

    ///
    ///         Returns the label for this node
    ///         If no label has been set, returns the null string
    ///
    ///\return  The label for this node
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& label() const
        throw();

    ///
    ///         Sets the icon to be used when this node is in its default state.
    ///
    ///\param   rImageResource      The image resource to be used as the icon.
    ///\param   rContainingSection  The section containing the resource.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setDefaultIconResource( DWFImageResource& rImageResource, DWFSection& rContainingSection )
        throw();

    ///
    ///         Sets the icon to be used when this node is in its default state.
    ///
    ///\param   rImageResource      The image resource to be used as the icon.
    ///                             The image resource is must be in the section containing
    ///                             the presentation.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setDefaultIconResource( DWFImageResource& rImageResource )
        throw();

    ///
    ///         Sets the URI of the default icon resource
    ///
    ///\param   zURI        The URI of the default icon resource
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setDefaultIconResourceURI( const DWFString& zURI )
        throw();

    ///
    ///         Returns the URI to the resource that contains the icon to be used when this
    ///         node is default. If no icon has been set, the string
    ///         will be a null string.
    ///
    ///\return  URI to the resource that contains the icon to be used when this
    ///         node is in its default state.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& defaultIconResourceURI() const
        throw();

    ///
    ///         Sets the icon to be used when this node is in an active state.
    ///
    ///\param   rImageResource      The image resource to be used as the icon.
    ///\param   rContainingSection  The section containing the resource.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setActiveIconResource( DWFImageResource& rImageResource, DWFSection& rContainingSection )
        throw();

    ///
    ///         Sets the icon to be used when this node is in an active state.
    ///
    ///\param   rImageResource      The image resource to be used as the icon.
    ///                             The image resource is must be in the section containing
    ///                             the presentation.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setActiveIconResource( DWFImageResource& rImageResource )
        throw();

    ///
    ///         Sets the URI of the active icon resource
    ///
    ///\param   zURI        The URI of the active icon resource
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setActiveIconResourceURI( const DWFString& zURI )
        throw();

    ///
    ///         Returns the URI to the resource that contains the icon to be used when this
    ///         node is active. If no icon has been set, the string
    ///         will be a null string.
    ///
    ///\return  URI to the resource that contains the icon to be used when this
    ///         node is in an active state.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& activeIconResourceURI() const
        throw();

    ///
    ///         Sets the icon to be used when this node is in a expanded state.
    ///         This is meaningful only for nodes that have children.
    ///
    ///\param   rImageResource      The image resource to be used as the icon.
    ///\param   rContainingSection  The section containing the resource.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setExpandedIconResource( DWFImageResource& rImageResource, DWFSection& rContainingSection )
        throw();

    ///
    ///         Sets the icon to be used when this node is in a expanded state.
    ///         This is meaningful only for nodes that have children.
    ///
    ///\param   rImageResource      The image resource to be used as the icon.
    ///                             The image resource is must be in the section containing
    ///                             the presentation.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setExpandedIconResource( DWFImageResource& rImageResource )
        throw();

    ///
    ///         Sets the URI of the expanded icon resource
    ///
    ///\param   zURI        The URI of the expanded icon resource
    ///\throw   None
    ///
    _DWFTK_API
    virtual void setExpandedIconResourceURI( const DWFString& zURI )
        throw();

    ///
    ///         Returns the URI to the resource that contains the icon to be used when this
    ///         node is in a expanded state. If no icon has been set, the string
    ///         will be a null string.
    ///
    ///\return  URI to the resource that contains the icon to be used when this
    ///         node is in a expanded state.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFString& expandedIconResourceURI() const
        throw();

    ///
    ///         Inserts a child node to this node.
    ///
    ///\param   pNode   The node to add as the child (must not be NULL).
    ///                 This  pointer is now owned by this node
    ///                 and will be released with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addChild( DWFContentPresentationNode* pNode )
        throw( DWFException );

    ///
    ///         Locates all children nodes in the container
    ///
    ///\return  A pointer to an iterator for enumerating the nodes (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Nodes returned from the iterator are owned by the container 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentationNode::tList::Iterator* getChildren( )
        throw();

    ///
    ///         Removes this node as a child
    ///
    ///\param   pNode           The node object to remove.
    ///\param   bDelete         If \e true the node will be deleted; otherwise, the node must be 
    ///                         deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void removeChild( DWFContentPresentationNode* pNode,
                              bool                        bDelete )
        throw( DWFException );

    ///
    ///         Specifies whether children of this node should be treated in a
    ///         mutually exclusive manner. This mostly applies to groups containing view
    ///         nodes, where the concept of undoing a node exists. When a group
    ///         if identified as exclusive only one node within group maybe
    ///         activated at a given time. This means, before applying a node
    ///         within the group, if there exists a previously applied node, it
    ///         should be "undone".
    ///
    ///\param   bExclusive      If \e true, children of this group should be
    ///                         treated in an exclusive manner. The default
    ///                         behavior is to not treat nodes as mutually
    ///                         exclusive.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setGroupExclusivity( bool bExclusive )
        throw();

    ///
    ///         The exclusivity of this group (when it has children)
    ///         
    ///
    ///\return  Returns whether the group of nodes contained within this node
    ///         must be treated as mutually exclusive.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual bool groupExclusivity() const
        throw();

    ///
    ///         Specifies whether this node is hidden or not.
    ///
    ///\param   bHidden       If \e true, this view will be marked as hidden.
    ///\throw   None
    ///
    virtual void setHidden( bool bHidden )
        throw()
    {
        _bHidden = bHidden;
    }

    ///
    ///         Returns whether this node is hidden or not.
    ///
    ///\return  Returns whether this node is hidden or not.
    ///\throw   None
    ///
    virtual bool hidden() const
        throw()
    {
        return _bHidden;
    }

    ///
    ///         Returns the parent of this node - the container which holds it.
    ///
    ///\return  The parent, or NULL if one has not yet been set.
    ///\throw   None
    ///
    virtual DWFContentPresentationNodeContainer* parent() const
        throw()
    {
        return _pParent;
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

#endif
protected:

    //
    //
    //
    DWFResource* containingResource();

#ifndef DWFTK_READ_ONLY
    //
    //
    //
    void generateResourceURI( DWFXMLSerializer& rSerializer, DWFResource* pResource, DWFSection* pSection, DWFString& zResourceURI );
    
#endif

private:

    DWFContentPresentationNode( const DWFContentPresentationNode& );
    DWFContentPresentationNode& operator=( const DWFContentPresentationNode& );

    void setParent( DWFContentPresentationNodeContainer* pParent )
    {
        _pParent = pParent;
    }

private:

    DWFString                               _zID;
    DWFString                               _zLabel;
    bool                                    _bExclusive;

    DWFString                               _zDefaultIconResourceURI;
    DWFString                               _zActiveIconResourceURI;
    DWFString                               _zExpandedIconResourceURI;

    DWFImageResource*                       _pDefaultIconResource;
    DWFSection*                             _pSectionContainingDefaultIconResource;

    DWFImageResource*                       _pActiveIconResource;
    DWFSection*                             _pSectionContainingActiveIconResource;

    DWFImageResource*                       _pExpandedIconResource;
    DWFSection*                             _pSectionContainingExpandedIconResource;

    bool                                    _bHidden;
    DWFContentPresentationNodeContainer*    _pParent;
};

}

#endif
