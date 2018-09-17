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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/reader/PackageManifestReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//


#ifndef _DWFTK_PACKAGE_MANIFEST_READER_H
#define _DWFTK_PACKAGE_MANIFEST_READER_H

///
///\file        dwf/package/reader/PackageManifestReader.h
///\brief       This file contains the DWFManifestReader class declaration.
///

#include "dwfcore/Core.h"
#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/package/XML.h"
#include "dwf/package/Section.h"
#include "dwf/package/Property.h"
#include "dwf/package/Interface.h"
#include "dwf/package/Dependency.h"
#include "dwf/package/reader/PackageReader.h"



namespace DWFToolkit
{

class DWFPackageContentPresentations;

///
///\ingroup     dwfpackage
///
///\class       DWFManifestReader   dwf/package/reader/PackageManifestReader.h     "dwf/package/reader/PackageManifestReader.h"
///\brief       This class implements the required XML parsing handlers to
///             compose toolkit objects from the manifest document and
///             provide them via the typed callbacks.
///\since       7.0.1
///
///             This class contains the processing logic particular to the known versions 
///             of the manifest.xml document.  The algorithm is tuned specifically for this
///             schema and is not intended to be generic.  
///
///             This reader (along with almost every other reader in the toolkit) is generally
///             used in two ways:
///             - <b>An object provider</b>:            Specialized classes implement some or all of the provider
///                                                     callback methods to receive objects from the parser as they
///                                                     are created.  The DWFManifest is an example of such an object.
///
///             - <b>A filter for another reader</b>:   Some objects that implement this interface (like DWFManifest),
///                                                     allow it to get hooked by the client.  This allows the client implementation
///                                                     of this interface to act a filter on the provider callback methods.
///                                                     The filter intercepts each callback and has the opporunity to examine,
///                                                     modify and even delete or replace the object before it is ultimately
///                                                     passed into the host object.
///             
///             The actual parsing logic can be constrained though the use of the \a DWFManifestReader::teProviderType flags.
///
class DWFManifestReader : public DWFCore::DWFXMLCallback
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
    typedef enum teProviderType
    {
        ///
        ///     Parse the document only (no data objects will be created; no callbacks will be invoked.)
        ///
        eProvideNone            =   0x00,

        ///
        ///     Invoke the \a provideVersion() attribute callback.
        ///
        eProvideVersion         =   0x01,
        ///
        ///     Invoke the \a provideObjectID() attribute callback.
        ///
        eProvideObjectID        =   0x02,

        ///
        ///     Invoke the \a provideDependency() element callback.
        ///
        eProvideDependencies    =   0x04,
        ///
        ///     Invoke the \a provideInterface() element callback.
        ///
        eProvideInterfaces      =   0x08,
        ///
        ///     Invoke the \a provideProperty() element callback.
        ///
        eProvideProperties      =   0x10,
        ///
        ///     Invoke the \a provideSection() element callback.
        ///
        eProvideSections        =   0x20,
        ///
        ///     Invoke the \a provideContent() element callback.
        ///
        eProvideContents  =   0x40,
        ///
        ///     Invoke the \a provideContentPresentations() element callback.
        ///
        eProvideContentPresentations  =   0x80,

        ///
        ///     Equivalent to (eProvideVersion | eProvideObjectID)
        ///
        eProvideAttributes      =   0x03,
        ///
        ///     Equivalent to (eProvideDependencies | eProvideInterfaces | eProvideProperties | eProvideSections | eProvideContents | eProvideContentPresentations)
        ///
        eProvideElements        =   0xFC,

        ///
        ///     Equivalent to (eProvideAttributes | eProvideElements)
        ///
        eProvideAll             =   0xFF

    } teProviderType;

public:

    ///
    ///         Constructor
    ///
    ///\param   pPackageReader      Provides the manifest document stream.
    ///\param   nProviderFlags      A combination of \a teProviderType flags.
    ///\throw   None
    ///
    _DWFTK_API
    DWFManifestReader( DWFPackageReader* pPackageReader = NULL,
                       unsigned char     nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFManifestReader()
        throw();

    ///
    ///         Returns the parser filter, if applicable.
    ///
    ///\return  The external filter to be applied to this parser.
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    _DWFTK_API
    DWFManifestReader* filter() const
        throw();

    ///
    ///         Inserts a parser filter.
    ///
    ///\param   pFilter     The filter to apply to this parser.
    ///                     This pointer is \b not ever deleted by this object.
    ///                     This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    _DWFTK_API
    void setFilter( DWFManifestReader* pFilter )
        throw();

    ///
    ///         Accepts the manifest document version number attribute.
    ///
    ///\param   nVersion        The document version.
    ///\return  The filtered document version.
    ///\throw   None
    ///
    _DWFTK_API
    virtual double provideVersion( double nVersion )
        throw( DWFException );

    ///
    ///         Accepts the unique manifest document identifier attribute.
    ///
    ///\param   zObjectID       The document ID.
    ///\return  The filtered document ID.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const char* provideObjectID( const char* zObjectID )
        throw( DWFException );

    ///
    ///         Accepts dependency element objects.
    ///
    ///\param   pDependency     The new dependency. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered dependency object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFDependency* provideDependency( DWFDependency* pDependency )
        throw( DWFException );

    ///
    ///         Accepts interface element objects.
    ///
    ///\param   pInterface      The new interface. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered interface object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFInterface* provideInterface( DWFInterface* pInterface )
        throw( DWFException );

    ///
    ///         Accepts property element objects.
    ///
    ///\param   pProperty       The new property. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered property object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFProperty* provideProperty( DWFProperty* pProperty )
        throw( DWFException );

    ///
    ///         Accepts section element objects.
    ///
    ///\param   pSection        The new section. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered section object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFSection* provideSection( DWFSection* pSection )
        throw( DWFException );

    ///
    ///         Accepts content element objects.
    ///
    ///\param   pContent        The new content. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered content manager object.
    ///\throw   None
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFContent* provideContent( DWFContent* pContent )
        throw( DWFException );

    ///
    ///         Accepts package level content content presentation
    ///
    ///\param   pPackageContentPresentations        The new package content presentations.
    ///                                             The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered package content presentation object.
    ///\throw   None
    ///
    ///\since   7.3
    ///
    _DWFTK_API
    virtual DWFPackageContentPresentations* provideContentPresentations( DWFPackageContentPresentations* pPackageContentPresentations )
        throw( DWFException );

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
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData
    ///
    _DWFTK_API
    void notifyCharacterData( const char* zCData, 
                              int         nLength ) 
        throw();

protected:

    ///
    ///         DWF package content source.
    ///
    DWFPackageReader*       _pPackageReader;

    ///
    ///         Default building behavior and element object allocator for readers.
    ///
    ///
    DWFXMLElementBuilder    _oDefaultElementBuilder;

    ///
    ///         This will point to the default builder initially.
    ///         But implementation classes can alter this pointer as necessary
    ///         but they should all use only this pointer to do the actual work.
    ///
    DWFXMLElementBuilder*   _pElementBuilder;

private:

    void _provideVersion( double nVersion )
        throw();

    void _provideObjectID( const char* zObjectID )
        throw();

    void _provideDependency( DWFDependency* pDependency )
        throw();

    void _provideInterface( DWFInterface* pInterface )
        throw();

    void _provideProperty( DWFProperty* pProperty )
        throw();

    void _provideSection( DWFSection* pSection )
        throw();

    void _provideContent( DWFContent* pContent )
        throw();

    void _provideContentPresentations( DWFPackageContentPresentations* pPackageContentPresentations )
        throw();

private:

    unsigned char _nProviderFlags;
    unsigned char _nCurrentCollectionProvider;

    DWFXMLBuildable*    _pCurrentElement;
    DWFManifestReader*  _pFilter;

private:

    //
    // Not implemented
    //

    DWFManifestReader( const DWFManifestReader& );
    DWFManifestReader& operator=( const DWFManifestReader& );
};

}

#endif
