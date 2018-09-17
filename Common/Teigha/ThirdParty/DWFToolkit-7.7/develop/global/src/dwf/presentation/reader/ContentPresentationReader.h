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


#ifndef _DWFTK_CONTENT_PRESENTATION_READER_H
#define _DWFTK_CONTENT_PRESENTATION_READER_H


///
///\file        dwf/presentation/reader/ContentPresentationReader.h
///\brief       This file contains the DWFContentPresentationReader class declaration.
///

#include "dwfcore/Core.h"
#include "dwfcore/STL.h"
#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/presentation/utility/ContentPresentationContainer.h"
#include "dwf/presentation/ContentPresentationModelViewNode.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFContentPresentationReader   dwf/package/reader/ContentPresentationReader.h     "dwf/package/reader/ContentPresentationReader.h"
///\brief       This class implements the required XML parsing handlers to
///             compose toolkit objects from the content presentation documents and
///             provide them via the typed callbacks.
///\since       7.2
///
///             This class contains the processing logic particular to the known versions 
///             of the content presentation documents.  The algorithm is tuned specifically for this
///             schema and is not intended to be generic.  
///
///\todo        Provide filtering for content presentation parsing.
///
class DWFContentPresentationReader : public DWFCore::DWFXMLCallback
                                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
public:
    ///
    ///         These enumeration flags alter the behavior of the parser
    ///         by restricting which elements are inflated into runtime
    ///         objects.  Generally this prevents unwanted object creation
    ///         (and thus memory allocations) but in some cases, additional
    ///         data processing can be avoided.
    ///
    typedef enum
    {
        ///
        ///     Parse the document only (no data objects will be created; no callbacks will be invoked.)
        ///
        eProvideNone                =   0x00,

        ///
        ///     Invoke the \a provideVersion() attribute callback.
        ///
        eProvideVersion             =   0x01,

        ///
        ///     Invoke the \a provideProperties() element callback.
        ///
        eProvidePresentations       =   0x02,
        ///
        ///     Invoke the \a provideView() element callback.
        ///
        eProvideViews               =   0x04,
        ///
        ///     Invoke the \a providePropertyReferences() element callback.
        ///
        eProvidePropertyReferences  =   0x08,
        ///
        ///     Invoke the \a provideNode() element callback.
        ///
        eProvideNodes               =   0x10,
        ///
        ///     Invoke the \a provideReferenceNode() element callback.
        ///
        eProvideRefereceNodes       =   0x20,
        ///
        ///     Invoke the \a provideModelViewNode() element callback.
        ///
        eProvideModelViewNodes      =   0x40,

        ///
        ///     Equivalent to \a eProvideVersion
        ///
        eProvideAttributes          =   0x01,
        ///
        ///     Equivalent to (eProvidePresentations | eProvideViews | eProvidePropertyReferences | eProvideNodes | eProvideRefereceNodes | eProvideModelViewNodes)
        ///
        eProvideElements            =   0x7E,

        ///
        ///     Equivalent to (eProvideAttributes | eProvideElements)
        ///
        eProvideAll                 =   0xFF

    } teProviderType;

    ///
    ///         Constructor
    ///
    ///\param   pPackageReader      Provides the manifest document stream.
    ///\param   nProviderFlags      A combination of \a teProviderType flags.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationReader( DWFPackageReader* pPackageReader = NULL,
                                  unsigned char     nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationReader()
        throw();

        //
        //
        // Provider callbacks for known attributes of the Manifest
        //
        //

    //
    //
    //
    _DWFTK_API
    DWFContentPresentationReader* filter() const
        throw();

    //
    //
    //
    _DWFTK_API
    void setFilter( DWFContentPresentationReader* pFilter )
        throw();

    ///
    ///         Accepts the object definition document version number attribute.
    ///
    ///\param   nVersion        The document version.
    ///\return  The filtered document version.
    ///\throw   None
    ///
    _DWFTK_API
    virtual double provideVersion( double nVersion)
        throw();

    ///
    ///         Accepts presentations.
    ///
    ///\param   pPresentation         The new presentation.
    ///                               The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered presentation.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentation* providePresentation( DWFContentPresentation* pPresentation )
        throw();

    ///
    ///         Accepts presentation views.
    ///
    ///\param   pView         The new presentation view.
    ///                       The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered presentation view.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentationView* provideView( DWFContentPresentationView* pView )
        throw();

    ///
    ///         Accepts property references.
    ///
    ///\param   pPropertyReference      The new property reference.
    ///                                 The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered property reference.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFPropertyReference* providePropertyReference( DWFPropertyReference* pPropertyReference )
        throw();

    ///
    ///         Accepts presentation nodes.
    ///
    ///\param   pNode         The new presentation node.
    ///                       The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered presentation node.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentationNode* provideNode( DWFContentPresentationNode* pNode )
        throw();

    ///
    ///         Accepts presentation reference nodes.
    ///
    ///\param   pReferenceNode  The new presentation reference node.
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered presentation reference node.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentationReferenceNode* provideReferenceNode( DWFContentPresentationReferenceNode* pReferenceNode )
        throw();

    ///
    ///         Accepts presentation model view nodes.
    ///
    ///\param   pModelViewNode  The new presentation model view node.
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered presentation model view node.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentationModelViewNode* provideModelViewNode( DWFContentPresentationModelViewNode* pModelViewNode )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartElement()
    ///
    _DWFTK_API
    void notifyStartElement( const char*   zName,
                             const char**  ppAttributeList )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndElement()
    ///
    _DWFTK_API
    void notifyEndElement( const char*     zName )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartNamespace()
    ///
    _DWFTK_API
    void notifyStartNamespace( const char* zPrefix,
                               const char* zURI )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace()
    ///
    _DWFTK_API
    void notifyEndNamespace( const char*   zPrefix )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData()
    ///
    _DWFTK_API
    void notifyCharacterData( const char* zCData, 
                              int         nLength ) 
        throw();

protected:

    ///
    ///         DWF package content source.
    ///
    DWFPackageReader* _pPackageReader;

    ///
    ///         Default building behavior and element object allocator for readers.
    ///
    ///
    DWFXMLElementBuilder        _oDefaultElementBuilder;

    ///
    ///         This will point to the default builder initially.
    ///         But implementation classes can alter this pointer as necessary
    ///         but they should all use only this pointer to do the actual work.
    ///
    DWFXMLElementBuilder*       _pElementBuilder;

private:

    unsigned char _nProviderFlags;
    unsigned char _nCurrentCollectionProvider;

    DWFContentPresentationReader* _pReaderFilter;

    DWFContentPresentation*                             _pCurrentPresentation;

    DWFContentPresentationView*                         _pCurrentPresentationView;

    DWFPropertyReference*                               _pCurrentPropertyReference;

    DWFContentPresentationNodeContainer*                _pCurrentNodeContainer;
    _DWFTK_STD_VECTOR(DWFContentPresentationNodeContainer*) _oNodeContainerStack;

    DWFContentPresentationNode*                         _pCurrentNode;

    int                                                 _nCurrentCuttingPlanesCount;
    float *                                             _pCurrentCuttingPlanes;
    float *                                             _pCurrentCuttingPlanesInteriorPointer;

    DWFModelSceneChangeHandler*                         _pCurrentHandler;
private:

    void _provideVersion( double nVersion)
        throw();

    void _providePresentation( DWFContentPresentation* pPresentation )
        throw();

    DWFContentPresentationView* _provideView( DWFContentPresentationView* pView )
        throw();

    DWFPropertyReference* _providePropertyReference( DWFPropertyReference* pPropertyReference )
        throw();

    DWFContentPresentationNode* _provideNode( DWFContentPresentationNode* pNode )
        throw();

    DWFContentPresentationReferenceNode* _provideReferenceNode( DWFContentPresentationReferenceNode* pReferenceNode )
        throw();

    DWFContentPresentationModelViewNode* _provideModelViewNode( DWFContentPresentationModelViewNode* pModelViewNode )
        throw();

    void _getAttribute( const char**  ppAttributeList, const char * pAttrName, const char * &rpAttrValue );

    //
    // Not implemented
    //

    DWFContentPresentationReader( const DWFContentPresentationReader& );
    DWFContentPresentationReader& operator=( const DWFContentPresentationReader& );
};

}

#endif
