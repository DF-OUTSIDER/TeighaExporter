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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/reader/DataSectionDescriptorReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//



#ifndef _DWFTK_DATA_SECTION_DESCRIPTOR_READER_H
#define _DWFTK_DATA_SECTION_DESCRIPTOR_READER_H

///
///\file        dwf/package/reader/DataSectionDescriptorReader.h
///\brief       This file contains the DWFDataSectionDescriptorReader class declaration.
///


#include "dwfcore/Core.h"
using namespace DWFCore;

#include "dwf/package/Units.h"
#include "dwf/package/Property.h"
#include "dwf/package/FontResource.h"
#include "dwf/package/GraphicResource.h"
#include "dwf/presentation/ContentPresentationResource.h"
#include "dwf/package/reader/SectionDescriptorReader.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFDataSectionDescriptorReader   dwf/package/reader/DataSectionDescriptorReader.h     "dwf/package/reader/DataSectionDescriptorReader.h"
///\brief       This class implements the required XML parsing handlers to
///             compose toolkit objects from data section descriptor documents and
///             provide them via the typed callbacks.
///\since       7.2
///
///             This class contains the processing logic particular to the known versions 
///             of the data section descriptor.xml documents.  The algorithm is tuned specifically for these
///             schema and is not intended to be generic.  
///
///             This reader (along with almost every other reader in the toolkit) is generally
///             used in two ways:
///             - <b>An object provider</b>:            Specialized classes implement some or all of the provider
///                                                     callback methods to receive objects from the parser as they
///                                                     are created.  The DWFDataSection and DWFDataGlobalSection
///                                                     are examples of such objects.
///
///             - <b>A filter for another reader</b>:   Some objects that implement this interface
///                                                     allow it to hooked by client.  This allows the client implementation
///                                                     of this interface to act a filter on the provider callback methods.
///                                                     The filter intercepts each callback and has the opporunity to examine,
///                                                     modify and even delete or replace the object before it is ultimately
///                                                     passed into the host object.
///             
///             The actual parsing logic can be constained though the use of the \a DWFDataSectionDescriptorReader::teProviderType flags.
///
class DWFDataSectionDescriptorReader : public DWFSectionDescriptorReader
                                       _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         These enumeration flags alter the behavior of the parser
    ///         by restricting which elements are inflated into runtime
    ///         objects.  Generally these only prevent unwanted object creation
    ///         (and thus memory allocations) but in some cases, additional
    ///         data processing can be avoided.
    ///
    typedef enum
    {
        ///
        ///     Parse the document only (no data objects will be created; no callbacks will be invoked.)
        ///
        eProvideNone                        =   0x000,

        ///
        ///     Invoke the \a provideName() attribute callback.
        ///
        eProvideName                        =   0x001,
        ///
        ///     Invoke the \a provideVersion() attribute callback.
        ///
        eProvideVersion                     =   0x002,
        ///
        ///     Invoke the \a providePlotOrder() attribute callback.
        ///
        eProvidePlotOrder                   =   0x004,
        ///
        ///     Invoke the \a provideObjectID() attribute callback.
        ///
        eProvideObjectID                    =   0x008,

        ///
        ///     Invoke the \a provideProperty() element callback.
        ///
        eProvideProperties                  =   0x010,
        ///
        ///     Indicates that resource elements should be processed.
        ///
        eProvideResources                   =   0x020,

        ///
        ///     Invoke the \a provideResource() element callback.
        ///
        eProvideUntypedResource             =   0x040,
        ///
        ///     Invoke the \a provideFontResource() element callback.
        ///
        eProvideFontResource                =   0x080,
        ///
        ///     Invoke the \a provideImageResource() element callback.
        ///
        eProvideImageResource               =   0x100,
        ///
        ///     Invoke the \a provideContentPresentationResource() element callback.
        ///
        eProvideContentPresentationResource =   0x200,

        ///
        ///     Equivalent to (eProvideName | eProvideVersion | eProvidePlotOrder | eProvideColor | eProvideObjectID)
        ///
        eProvideAttributes                  =   0x00F,
        
        ///
        ///     Equivalent to (eProvideProperties | eProvideResources | eProvideUntypedResource | eProvideFontResource | eProvideGraphicResource | eProvideImageResource | eProvideContentResource | eProvideContentPresentationResource)
        ///
        eProvideElements                    =   0xFF0,

        ///
        ///     Equivalent to (eProvideAttributes | eProvideElements)
        ///
        eProvideAll                         =   0xFFF

    } teProviderType;

public:

    ///
    ///         Constructor
    ///
    ///\param   pPackageReader      Provides the section descriptor document stream.
    ///\param   nProviderFlags      A combination of \a teProviderType flags.
    ///\throw   None
    ///
    _DWFTK_API
    DWFDataSectionDescriptorReader( DWFPackageReader*  pPackageReader = NULL,
                                    unsigned int       nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFDataSectionDescriptorReader()
        throw();

    ///
    ///\copydoc DWFSectionDescriptorReader::filter()
    ///
    _DWFTK_API
    DWFDataSectionDescriptorReader* filter() const
        throw();

    ///
    ///\copydoc DWFSectionDescriptorReader::setFilter()
    ///
    _DWFTK_API
    void setFilter( DWFDataSectionDescriptorReader* pFilter )
        throw();

    ///
    ///         Accepts the section name attribute.
    ///
    ///\param   zName       The section name.
    ///\return  The filtered section name.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const char* provideName( const char* zName )
        throw();

    ///
    ///         Accepts the section identifier attribute.
    ///
    ///\param   zObjectID   The section ID.
    ///\return  The filtered section ID.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const char* provideObjectID( const char* zObjectID )
        throw();

    ///
    ///         Accepts the descriptor document version attribute.
    ///
    ///\param   nVersion        The document version.
    ///\return  The filtered document version.
    ///\throw   None
    ///
    _DWFTK_API
    virtual double provideVersion( double nVersion )
        throw();

    ///
    ///         Accepts the section plot order attribute.
    ///
    ///\param   nPlotOrder      The plot order.
    ///\return  The filtered plot order.
    ///\throw   None
    ///
    _DWFTK_API
    virtual double providePlotOrder( double nPlotOrder )
        throw();

    ///
    ///         Accepts property element objects.
    ///
    ///\param   pProperty   The new property. 
    ///                     The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered property object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFProperty* provideProperty( DWFProperty* pProperty )
        throw();

    ///
    ///         Accepts resource element objects.
    ///
    ///\param   pResource   The new resource. 
    ///                     The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered resource object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFResource* provideResource( DWFResource* pResource )
        throw();

    ///
    ///         Accepts font resource element objects.
    ///
    ///\param   pResource   The new resource. 
    ///                     The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered resource object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFFontResource* provideFontResource( DWFFontResource* pResource )
        throw();

    ///
    ///         Accepts image resource element objects.
    ///
    ///\param   pResource   The new resource. 
    ///                     The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered resource object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFImageResource* provideImageResource( DWFImageResource* pResource )
        throw();

    ///
    ///         Accepts content presentation resource element objects
    ///
    ///\param   pResource   The new resource. 
    ///                     The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered resource object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentationResource* provideContentPresentationResource( DWFContentPresentationResource* pResource )
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

protected:

    ///
    ///\copydoc DWFSectionDescriptorReader::_provideAttributes()
    ///
    void _provideAttributes( const char**   ppAttributeList )
        throw();

private:

    void _provideName( const char* zName )
        throw();

    void _provideObjectID( const char* zObjectID )
        throw();

    void _provideVersion( double nVersion )
        throw();

    void _providePlotOrder( double nPlotOrder )
        throw();

    void _provideProperty( DWFProperty* pProperty )
        throw();

    void _provideResource( DWFResource* pResource )
        throw();

    void _provideFontResource( DWFFontResource* pResource )
        throw();

    void _provideImageResource( DWFImageResource* pResource )
        throw();

    void _provideContentPresentationResource( DWFContentPresentationResource* pResource )
        throw();

private:

    unsigned int                    _nProviderFlags;
    unsigned int                    _nCurrentCollectionProvider;

    DWFResource*                    _pCurrentResource;
    DWFDataSectionDescriptorReader* _pFilter;

private:

    //
    // Not implemented
    //

    DWFDataSectionDescriptorReader( const DWFDataSectionDescriptorReader& );
    DWFDataSectionDescriptorReader& operator=( const DWFDataSectionDescriptorReader& );
};

}

#endif
