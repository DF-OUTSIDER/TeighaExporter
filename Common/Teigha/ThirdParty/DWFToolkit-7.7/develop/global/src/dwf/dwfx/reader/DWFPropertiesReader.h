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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/reader/DWFPropertiesReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXDWFPROPERTIESREADER_H
#define _DWFTK_DWFXDWFPROPERTIESREADER_H

///
///\file        dwf/dwfx/reader/DWFPropertiesReader.h
///\brief       This file contains the DWFXDWFPropertiesReader class declaration.
///

#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/opc/reader/XMLElementBuilder.h"

namespace DWFToolkit
{

///
///\ingroup     dwfx
///
///\class       DWFXDWFPropertiesReader   dwf/dwfx/reader/DWFPropertiesReader.h     "dwf/dwfx/reader/DWFPropertiesReader.h"
///\brief       This class implements the required XML parsing handlers to compose dwfx core properties.
///\since       7.4.0
///
///             This class contains the processing logic particular to the known versions
///             of the dwfx dwf properties.  The algorithm is tuned specifically for this
///             schema and is not intended to be generic.
///
class DWFXDWFPropertiesReader : public DWFCore::DWFXMLCallback
                                _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         These enumeration flags alter the behavior of the parser by restricting which
    ///         elements are inflated into runtime objects.  Generally these only prevent unwanted
    ///         object creation (and thus memory allocations) but in some cases, additional data
    ///         processing can be avoided.
    ///
    typedef enum teProviderType
    {
        ///
        ///     Parse the document only (no data objects will be created; no callbacks will be invoked.)
        ///
        eProvideNone                =   0x00,

        ///
        ///     Invoke the \a provideProperty() callback.
        ///
        eProvideProperties          =   0x01,

        ///
        ///     Equivalent to (eProvideProperties)
        ///
        eProvideElements            =   0xF0,

        ///
        ///     Equivalent to (eProvideElements)
        ///
        eProvideAll                 =   0xFF

    } teProviderType;

public:

    ///
    ///         Constructor
    ///
    ///\param   nProviderFlags      A combination of \a teProviderType flags.
    ///\throw   None
    ///
    _DWFTK_API
    DWFXDWFPropertiesReader( unsigned int nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFXDWFPropertiesReader()
        throw();


    ///
    ///         Returns the parser filter, if applicable.
    ///
    ///\return  The external filter to be applied to this parser.
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    _DWFTK_API
    DWFXDWFPropertiesReader* filter() const
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
    void setFilter( DWFXDWFPropertiesReader* pFilter )
        throw();


    ///
    ///         Accept the property value for source product vendor.
    ///
    ///\param   zValue  The property value.
    ///\return  The filtered value.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString provideSourceProductVendor( const DWFString& zValue )
        throw();

    ///
    ///         Accept the property value for source product name.
    ///
    ///\param   zValue  The property value.
    ///\return  The filtered value.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString provideSourceProductName( const DWFString& zValue )
        throw();

    ///
    ///         Accept the property value for source product version.
    ///
    ///\param   zValue  The property value.
    ///\return  The filtered value.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString provideSourceProductVersion( const DWFString& zValue )
        throw();

    ///
    ///         Accept the property value for DWF product vendor.
    ///
    ///\param   zValue  The property value.
    ///\return  The filtered value.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString provideDWFProductVendor( const DWFString& zValue )
        throw();

    ///
    ///         Accept the property value for DWF product version.
    ///
    ///\param   zValue  The property value.
    ///\return  The filtered value.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString provideDWFProductVersion( const DWFString& zValue )
        throw();

    ///
    ///         Accept the property value for DWF Toolkit version
    ///
    ///\param   zValue  The property value.
    ///\return  The filtered value.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString provideDWFToolkitVersion( const DWFString& zValue )
        throw();

    ///
    ///         Accept the property value for DWF Format version
    ///
    ///\param   zValue  The property value.
    ///\return  The filtered value.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString provideDWFFormatVersion( const DWFString& zValue )
        throw();

    ///
    ///         Accept the property value for Password Encrypted Document
    ///
	///\param   zValue  The property value: "true" or "false"
    ///\return  The filtered value.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString providePasswordEncrypted( const DWFString& zValue )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartElement(const char* zName, const char**  ppAttributeList)
    ///
    _DWFTK_API
    void notifyStartElement( const char*   zName,
                             const char**  ppAttributeList )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndElement(const char* zName)
    ///
    _DWFTK_API
    void notifyEndElement( const char*     zName )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartNamespace(const char* zPrefix, const char* zURI )
    ///
    _DWFTK_API
    void notifyStartNamespace( const char* zPrefix,
                               const char* zURI )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace(const char*   zPrefix)
    ///
    _DWFTK_API
    void notifyEndNamespace( const char*   zPrefix )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData(const char* zCData, int nLength)
    ///
    _DWFTK_API
    void notifyCharacterData( const char* zCData, 
                              int         nLength ) 
        throw();

private:

    void _provideProperty( const char** ppAttributeList )
        throw();

private:

    unsigned int                    _nProviderFlags;
    DWFXDWFPropertiesReader*        _pFilter;

private:

    //
    // Not implemented
    //

    DWFXDWFPropertiesReader( const DWFXDWFPropertiesReader& );
    DWFXDWFPropertiesReader& operator=( const DWFXDWFPropertiesReader& );

};

}

#endif
