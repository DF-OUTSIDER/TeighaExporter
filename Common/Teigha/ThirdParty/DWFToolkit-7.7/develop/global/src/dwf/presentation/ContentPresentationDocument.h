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


#ifndef _DWFTK_CONTENTPRESENTATIONDOCUMENT_H
#define _DWFTK_CONTENTPRESENTATIONDOCUMENT_H

///
///\file        dwf/package/ContentPresentationDocument.h
///\brief       This file contains the DWFContentPresentationDocument class declaration.
///


#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/presentation/reader/ContentPresentationReader.h"
#include "dwf/presentation/utility/ContentPresentationContainer.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpresentation
///
///\class       DWFContentPresentationDocument   dwf/presentation/ContentPresentationDocument.h     "dwf/presentation/ContentPresentationDocument.h"
///\brief       This class represents the content presentation document
///\since       7.3
///
class DWFContentPresentationDocument : public DWFContentPresentationContainer
                                     , public DWFContentPresentationReader
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
    DWFContentPresentationDocument( DWFPackageReader*  pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationDocument()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationDocument()
        throw();

    ///
    ///         Gets an iterator to the presentations that were processed from the presentation document
    ///
    ///\return  A pointer to an iterator for enumerating the presentations (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Presentations returned from the iterator are owned by this class
    ///         and must not be deleted by the caller.
    ///
    virtual
    DWFContentPresentationNode::tList::Iterator* getNodes()
    {
        return _oNodes.iterator();
    }


    ///
    ///         Given an ID, search for a node with that ID.
    ///
    ///\param   zID      The ID of the node to search for.
    ///\return  The found node, or NULL.
    ///
    virtual
    DWFContentPresentationNode* getNode( const DWFString& zID )
    {
        DWFContentPresentationNode** ppNode = _oIDToNode.find( zID );
        if (ppNode)
        {
            return *ppNode;
        }
        else
        {
            return NULL;
        }
    }


    ///
    ///         Given an ID, search for a view with that ID.
    ///
    ///\param   zID      The ID of the view to search for.
    ///\return  The found view, or NULL.
    ///
    virtual
    DWFContentPresentationView* getView( const DWFString& zID )
    {
        DWFContentPresentationView** ppView = _oIDToView.find( zID );
        if (ppView)
        {
            return *ppView;
        }
        return NULL;
    }


    ///
    ///\copydoc DWFContentPresentationReader::provideVersion()
    ///
    _DWFTK_API
    virtual double provideVersion( double nVersion)
        throw();

    ///
    ///\copydoc DWFContentPresentationReader::providePresentation()
    ///
    _DWFTK_API
    virtual DWFContentPresentation* providePresentation( DWFContentPresentation* pPresentation )
        throw();

    ///
    ///\copydoc DWFContentPresentationReader::provideView()
    ///
    _DWFTK_API
    virtual DWFContentPresentationView* provideView( DWFContentPresentationView* pView )
        throw();

    ///
    ///\copydoc DWFContentPresentationReader::providePropertyReference()
    ///    
    _DWFTK_API
    virtual DWFPropertyReference* providePropertyReference( DWFPropertyReference* pPropertyReference )
        throw();

    ///
    ///\copydoc DWFContentPresentationReader::provideNode()
    ///
    _DWFTK_API
    virtual DWFContentPresentationNode* provideNode( DWFContentPresentationNode* pNode )
        throw();

    ///
    ///\copydoc DWFContentPresentationReader::provideReferenceNode()
    ///
    _DWFTK_API
    virtual DWFContentPresentationReferenceNode* provideReferenceNode( DWFContentPresentationReferenceNode* pReferenceNode )
        throw();

    ///
    ///\copydoc DWFContentPresentationReader::provideModelViewNode()
    ///
    _DWFTK_API
    virtual DWFContentPresentationModelViewNode* provideModelViewNode( DWFContentPresentationModelViewNode* pModelViewNode )
        throw();

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
            , _pDocument( NULL )
        {;}

        virtual ~_Serializer()
            throw()
        {;}

        void is( DWFContentPresentationDocument* pDocument )
        {
            _pDocument = pDocument;
        }

        //
        //
        //
        virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

    private:

        DWFContentPresentationDocument* _pDocument;
    };

private:

    friend class _Serializer;
    _Serializer _oSerializer;

#endif

private:

    DWFString   _zType;
    char*       _pBuffer;
    float       _nVersion;
    DWFString   _zHRef;

    unsigned char _nProviderFlags;

    DWFContentPresentationNode::tMap                    _oIDToNode;
    DWFContentPresentationNode::tList                   _oNodes;
    DWFContentPresentationView::tMap                    _oIDToView;

    DWFContentPresentationDocument( const DWFContentPresentationDocument& );
    DWFContentPresentationDocument& operator=( const DWFContentPresentationDocument& );
};

}

#endif

