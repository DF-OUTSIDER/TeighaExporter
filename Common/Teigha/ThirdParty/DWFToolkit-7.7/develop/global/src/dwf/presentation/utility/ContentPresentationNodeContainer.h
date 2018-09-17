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

//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/presentation/utility/ContentPresentationNodeContainer.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $

#ifndef _DWFTK_CONTENT_PRESENTATION_NODE_CONTAINER_H
#define _DWFTK_CONTENT_PRESENTATION_NODE_CONTAINER_H


///
///\file        dwf/presentation/utility/ContentPresentationNodeContainer.h
///\brief       This file contains the DWFContentPresentationNodeContainer class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/package/XML.h"
#include "dwf/Toolkit.h"

namespace DWFToolkit
{

//
// fwd decl
//
class DWFContentPresentationNode;

///
///\ingroup     dwfpackage
///
///\class       DWFContentPresentationNodeContainer   dwf/package/utility/ContentPresentationNodeContainer.h     "dwf/package/utility/ContentPresentationNodeContainer.h"
///\brief       An collection utility for DWFContentPresentationNode objects.
///\since       7.2
///
///             This class is a content presentation view owner.
///
class DWFContentPresentationNodeContainer _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

public:

    ///
    ///         Constructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationNodeContainer()
        throw();

    ///
    ///         Destructor
    ///
    ///         This method will delete all of the container views
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationNodeContainer()
        throw();

    ///
    ///         Inserts a node to the container.
    ///
    ///\param   pNode   The node to add to the container (must not be NULL).
    ///                 This  pointer is now owned by the container
    ///                 and will be released with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addNode( DWFContentPresentationNode* pNode )
        throw( DWFException );

    ///
    ///         Insert a node at the given index. If the given index is greater
    ///         than the size of the vector, an exception is thrown. 
    ///
    ///\param   pNode   The node to add to the container (must not be NULL).
    ///                 This pointer is now owned by the container
    ///                 and will be released with the \b DWFCORE_FREE_OBJECT macro.
    ///\param   index   The index of the location to insert it.
    ///\throw   A DWFUnexpectedException is thrown if the index is greater than the size.
    ///
    _DWFTK_API
    virtual void insertAt( DWFContentPresentationNode* pNode, size_t index )
	    throw( DWFException );

    ///
    ///         Locates all nodes in the container
    ///
    ///\return  A pointer to an iterator for enumerating the nodes (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Nodes returned from the iterator are owned by the container 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFOrderedVector<DWFContentPresentationNode*>::Iterator* getNodes( )
        throw();

    ///
    ///         Clears the node from the container.
    ///
    ///\param   pNode           The node object to remove from this container.
    ///\param   bDelete         If \e true the node will be deleted; otherwise, the node must be 
    ///                         deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void removeNode( DWFContentPresentationNode* pNode,
                             bool                        bDelete )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///         Returns an interface with which the container can be
    ///         serialized into an XML document.
    ///
    ///\return  A reference to a serialization inteface.
    ///\throw   None
    ///
    DWFXMLSerializable& getSerializable() const
        throw()
    {
        return (DWFXMLSerializable&)_oSerializer;
    }
#endif

#ifndef DWFTK_READ_ONLY

private:

    //
    // in order to avoid bizarre diamond patterns and
    // excessively overload base implementations,
    // all container classes must define and implementation this
    // internal [PRIVATE] class utility for serializing themselves into XML.
    //
    class _Serializer : public DWFXMLSerializable
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        _Serializer()
            throw()
            : DWFXMLSerializable()
            , _pContainer( NULL )
        {;}

        virtual ~_Serializer()
            throw()
        {;}

        void is( DWFContentPresentationNodeContainer* pContainer )
        {
            _pContainer = pContainer;
        }

        //
        //
        //
        virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

    private:

        DWFContentPresentationNodeContainer* _pContainer;
    };

private:

    _Serializer _oSerializer;

#endif

protected:
#if defined(__BCPLUSPLUS__)
    public:
#endif

    ///
    ///\brief       The main collection of nodes
    ///
    DWFOrderedVector<DWFContentPresentationNode*>      _oNodes;

	typedef DWFStringKeySkipList<size_t>     IDToPostionMap;
	IDToPostionMap						  posMap;

private:

    DWFContentPresentationNodeContainer( const DWFContentPresentationNodeContainer& );
    DWFContentPresentationNodeContainer& operator=( const DWFContentPresentationNodeContainer& );
};

}

#endif
