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
#ifndef _DWFTK_XML_IDENTITY_WRITER_H
#define _DWFTK_XML_IDENTITY_WRITER_H

///
///\file    dwfcore/utility/XMLIdentityWriter.h
///\brief   The declaration of a class to write xml as seen.
///

#include "dwfcore/OutputStream.h"
#include "dwfcore/XML.h"

namespace DWFCore
{

///
///\class   DWFXMLIdentityWriter  dwfcore/utility/XMLIdentityWriter.h
///\since   7.4
///\brief   Convert xml element notifications to textual xml.
///
class DWFXMLIdentityWriter : public DWFXMLCallback
{

public:

    ///
    ///\brief   Constructor
    ///\param   rOut    a stream that will receive the xml
    ///\throw   none
    ///
    DWFXMLIdentityWriter( DWFCore::DWFOutputStream& rOut )
        throw();
    
    ///
    ///\brief   Destructor
    ///\throw   none
    ///
    virtual ~DWFXMLIdentityWriter() throw();

    ///
    ///\copydoc DWFXMLCallback::notifyStartElement()
    ///
    _DWFCORE_API
    void notifyStartElement( const char*   zName,
                             const char**  ppAttributeList )
        throw();

    ///
    ///\copydoc DWFXMLCallback::notifyEndElement()
    ///
    _DWFCORE_API
    void notifyEndElement( const char*     zName )
        throw();

    ///
    ///\copydoc DWFXMLCallback::notifyStartNamespace()
    ///
    _DWFCORE_API
    void notifyStartNamespace( const char* zPrefix,
                               const char* zURI )
        throw()
    {(void)zPrefix;(void)zURI;}

    ///
    ///\copydoc DWFXMLCallback::notifyEndNamespace()
    ///
    _DWFCORE_API
    void notifyEndNamespace( const char*   zPrefix )
        throw()
    {(void)zPrefix;}

    ///
    ///\copydoc DWFXMLCallback::notifyCharacterData()
    ///
    _DWFCORE_API
    void notifyCharacterData( const char* zCData,
                              int         nLength )
        throw()
    {(void)zCData; (void)nLength;}

protected:

private:

};

}

#endif
