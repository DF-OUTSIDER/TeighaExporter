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


#ifndef _DWFTK_SECTION_DESCRIPTOR_READER_H
#define _DWFTK_SECTION_DESCRIPTOR_READER_H

///
///\file        dwf/package/reader/SectionDescriptorReader.h
///\brief       This file contains the DWFSectionDescriptorReader class declaration.
///

#include "dwfcore/Core.h"
#include "dwfcore/XML.h"
using namespace DWFCore;


#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"



namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFSectionDescriptorReader   dwf/package/reader/SectionDescriptorReader.h     "dwf/package/reader/SectionDescriptorReader.h"
///\brief       This class implements the required XML parsing handlers to
///             compose toolkit objects from the section descriptor documents and
///             provide them via the typed callbacks.
///\since       7.0.1
///
///             This class contains some shared processing logic particular to the known versions 
///             of the descriptor.xml section documents.  The algorithm is tuned specifically for these
///             schema and is not intended to be generic.  
///
///             This reader (along with almost every other reader in the toolkit) is generally
///             used in two ways:
///             - <b>An object provider</b>:            Specialized classes implement some or all of the provider
///                                                     callback methods to receive objects from the parser as they
///                                                     are created.  The DWFEPlotSection and DWFEPlotGlobalSection
///                                                     are examples of such objects.
///
///             - <b>A filter for another reader</b>:   Some objects that implement this interface
///                                                     allow it to get hooked by the client. This allows the client implementation
///                                                     of this interface to act a filter on the provider callback methods.
///                                                     The filter intercepts each callback and has the opporunity to examine,
///                                                     modify and even delete or replace the object before it is ultimately
///                                                     passed into the host object.
///             
///             The actual parsing logic can be constrained though the use of the \a DWFSectionDescriptorReader::teProviderType flags.
///
class DWFSectionDescriptorReader : public DWFCore::DWFXMLCallback

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
        eProvideNone            =   0x00,

        ///
        ///     Invoke the \a _provideAttributes() callback.
        ///
        eProvideAttributes      =   0x01,
        ///
        ///     Invoke the \a provideType() attribute callback.
        ///
       eProvideType            =   0x02,

        ///
        ///     Equivalent to (eProvideAttributes | eProvideType)
        ///
        eProvideAll             =   0xFF

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
    DWFSectionDescriptorReader( DWFPackageReader*   pPackageReader = NULL,
                                unsigned char       nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSectionDescriptorReader()
        throw();

    ///
    ///         Returns the parser filter, if applicable.
    ///
    ///\return  The external filter to be applied to this parser.
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSectionDescriptorReader* filter() const
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
    void setFilter( DWFSectionDescriptorReader* pFilter )
        throw();

    ///
    ///         Accepts the section type attribute.
    ///
    ///\param   zType       The section type.
    ///\return  If \e true, provide the type to the filter.
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool provideType( const char*   zType )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartElement()
    ///
    _DWFTK_API
    virtual void notifyStartElement( const char*    zName,
                                     const char**   ppAttributeList )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndElement()
    ///
    _DWFTK_API
    virtual void notifyEndElement( const char*      zName )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartNamespace()
    ///
    _DWFTK_API
    virtual void notifyStartNamespace( const char*  zPrefix,
                                       const char*  zURI )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace()
    ///
    _DWFTK_API
    virtual void notifyEndNamespace( const char*    zPrefix )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData()
    ///
    _DWFTK_API
    virtual void notifyCharacterData( const char* zCData, 
                                      int         nLength ) 
        throw();

    //
    //
    //
    _DWFTK_API
    virtual void _provideAttributes( const char**   ppAttributeList )
        throw();

protected:

    _DWFTK_API
    DWFString _findAttributeValue( const char*  zAttributeName,
                                   const char** ppAttributeList );

private:

    void _provideType( const char* zType )
        throw();

protected:

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

    ///
    ///         DWF package content source.
    ///
    DWFPackageReader*           _pPackageReader;

private:

    unsigned char               _nProviderFlags;
    DWFSectionDescriptorReader* _pFilter;

private:

    //
    // Not implemented
    //

    DWFSectionDescriptorReader( const DWFSectionDescriptorReader& );
    DWFSectionDescriptorReader& operator=( const DWFSectionDescriptorReader& );
};



}

#endif
