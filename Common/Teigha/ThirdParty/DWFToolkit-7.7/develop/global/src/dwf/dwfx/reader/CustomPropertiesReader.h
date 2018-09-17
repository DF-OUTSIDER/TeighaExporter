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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/reader/CustomPropertiesReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXCUSTOMPROPERTIESREADER_H
#define _DWFTK_DWFXCUSTOMPROPERTIESREADER_H

///
///\file        dwf/dwfx/reader/CustomPropertiesReader.h
///\brief       This file contains the DWFXCustomPropertiesReader class declaration.
///

#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"

namespace DWFToolkit
{

//
// fwd declaration
//
class DWFProperty;

///
///\ingroup     dwfx
///
///\class       DWFXCustomPropertiesReader   dwf/dwfx/reader/DWFPropertiesReader.h     "dwf/dwfx/reader/DWFPropertiesReader.h"
///\brief       This class implements the required XML parsing handlers to compose dwfx core properties.
///\since       7.4.0
///
///             This class contains the processing logic particular to the known versions
///             of the custom properties.  The algorithm is tuned specifically for this
///             schema and is not intended to be generic.
///
class DWFXCustomPropertiesReader : public DWFCore::DWFXMLCallback
                                   _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFXCustomPropertiesReader()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFXCustomPropertiesReader()
        throw();


    ///
    ///         Returns the parser filter, if applicable.
    ///
    ///\return  The external filter to be applied to this parser.
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    _DWFTK_API
    DWFXCustomPropertiesReader* filter() const
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
    void setFilter( DWFXCustomPropertiesReader* pFilter )
        throw();

    ///
    ///         Accept the property.
    ///
    ///\param   pProperty       The new property.
    ///\return  The filtered property.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFProperty* provideProperty( DWFProperty* pProperty )
        throw();


    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartElement(const char* zName, const char** ppAttributeList )
    ///
    _DWFTK_API
    void notifyStartElement( const char*   zName,
                             const char**  ppAttributeList )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndElement(const char* zName )
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
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace(const char* zPrefix)
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

protected:

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

    DWFXCustomPropertiesReader*     _pFilter;

    DWFProperty*                    _pCurrentProperty;

private:

    //
    // Not implemented
    //

    DWFXCustomPropertiesReader( const DWFXCustomPropertiesReader& );
    DWFXCustomPropertiesReader& operator=( const DWFXCustomPropertiesReader& );

};

}

#endif
