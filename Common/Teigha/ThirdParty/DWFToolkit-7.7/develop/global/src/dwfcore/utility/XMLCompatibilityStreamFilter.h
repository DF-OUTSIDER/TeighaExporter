//
//  Copyright (c) 2004-2006 by Autodesk, Inc. 
// 
//  By using this code, you are agreeing to the terms and conditions of 
//  the License Agreement included in the documentation for this code. 
// 
//  AUTODESK MAKES NO WARRANTIES, EXPRESSED OR IMPLIED, 
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE 
//  WORKS WHICH INCORPORATE IT.  
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS 
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING 
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS, 
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE. 
// 
//  Use, duplication, or disclosure by the U.S. Government is subject to 
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software 
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical 
//  Data and Computer Software), as applicable. 
// 
#ifndef _DWFTK_XML_COMPATIBILITY_STREAM_FILTER_H
#define _DWFTK_XML_COMPATIBILITY_STREAM_FILTER_H

#include "dwfcore/BufferInputStream.h"
#include "dwfcore/String.h"

#include "dwfcore/utility/XMLCompatibilityPP.h"

///
///\file    dwfcore/utility/XMLCompatibilityStreamFilter.h
///\brief   The declaration of a class to preprocess input streams.
///

namespace DWFCore
{

///
///\class   DWFXMLCompatibilityStreamFilter dwfcore/utility/XMLCompatibilityStreamFilter.h
///\since   7.4
///\brief   Provide XML Compatibility preprocessing on an input stream
///
///         So that a client may access raw xml after it has been
///         preprocessed to conform to XML Compatibility specifications, 
///         this filter will chain to a given input stream.
///
class DWFXMLCompatibilityStreamFilter : public DWFCore::DWFBufferInputStream
{

public:

    ///
    ///\brief   Constructor
    ///\param   pInputStream    the stream providing the source XML
    ///\param   bOwnStream      specify wether this object will control the stream's lifetime
    ///\throw   none
    ///
    DWFXMLCompatibilityStreamFilter( DWFCore::DWFInputStream*   pInputStream,
                                     bool                       bOwnStream )
        throw();

    ///
    ///\brief   Destructor
    ///\throw   none
    ///
    virtual ~DWFXMLCompatibilityStreamFilter() throw();

    ///
    ///\copydoc DWFXMLCompatibilityPP::addSupportedNS()
    ///
    _DWFCORE_API
    void addSupportedNS( const DWFCore::DWFString& zUri,
                         const DWFXMLCompatibilityPP::tStringIt* pStrings = NULL )
        throw();

    ///
    ///\copydoc DWFXMLCompatibilityPP::addExtensionElementName()
    ///
    _DWFCORE_API
    void addExtensionElementName( const DWFCore::DWFString& zQualName )
        throw();

protected:

private:

};

}

#endif
