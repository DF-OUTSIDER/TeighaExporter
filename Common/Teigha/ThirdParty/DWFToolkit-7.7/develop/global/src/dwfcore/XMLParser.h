//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
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
#ifndef _DWFCORE_XML_READER_H
#define _DWFCORE_XML_READER_H

///
///\file    dwfcore/XMLReader.h
///\brief   Declaration of XML utility class
///

#include "dwfcore/InputStream.h"
#include "dwfcore/XML.h"
#include "dwfcore/Pointer.h"
#include "dwfcore/String.h"


#if     defined(DWFCORE_SYSTEM_EXPAT) || defined(DWF_USE_SYSTEM_EXPAT)
#include <expat.h>
#else
#include "dwfcore/expat/expat.h"
#endif

namespace DWFCore
{

///
///\class   DWFXMLParser dwfcore/XMLParser.h
///\since   7.4
///\brief   Provide logic to run an XML document through expat.
///
class DWFXMLParser
{

public:
    
    ///
    ///         Constructor
    ///
    ///\param   pCallback A pointer to a required implementation of DWFXMLCallback
    ///
    ///\throw   DWFNullPointerException if the pCallback param is NULL
    ///\throw   DWFMemoryException if an internal parser buffer could not be allocated
    ///
    _DWFCORE_API
	DWFXMLParser(DWFXMLCallback* pCallback) throw(DWFException);

    ///
    ///         Destructor
    ///
    _DWFCORE_API
    virtual ~DWFXMLParser();

    ///
    ///         Creates the internal expat parser and gets it ready for work
    ///
    _DWFCORE_API
    void initializeParser();

    ///
    ///         Releases the internal expat parser.  initializeParser must be
    ///         called again if parsing is to restart with this same parser.
    ///
    _DWFCORE_API
    void releaseParser();

    ///
    ///         Stops parsing.  Can be continued later.
    ///
    /// \throw  DWFIOException if stopping generates an error
    ///
    _DWFCORE_API
    void stopParser() throw(DWFException);

    ///
    ///         Returns true if parsing has reached the end of the document.
    ///
    ///\return  bool true if parsing has reached the end of the document.
    ///
    ///\throw   None
    ///
    bool isParsingFinished() { return _bIsParsingFinished; }
    
    ///
    ///         Expat callback handler.
    ///
    ///         This method acts only as a router for \a pCallback
    ///         which is required to implement DWFXMLCallback::notifyStartElement().
    ///
    ///\param   pCallback           An implementation of DWFXMLCallback.
    ///\param   zName               The name of the element (includes any namespace prefix)
    ///\param   ppAttributeList     An array of null-terminated atribute name, value string pairs.
    ///                             The array itself is zero-terminated.
    ///\throw   None
    ///
    _DWFCORE_API
    static void XMLCALL StartElementHandler( void*             pCallback,
                                             const char*       zName,
                                             const char**      ppAttributeList )
        throw();

    ///
    ///         Expat callback handler.
    ///
    ///         This method acts only as a router for \a pCallback
    ///         which is required to implement DWFXMLCallback::notifyEndElement().
    ///
    ///\param   pCallback           An implementation of DWFXMLCallback.
    ///\param   zName               The name of the element (includes any namespace prefix)
    ///\throw   None
    ///
    _DWFCORE_API
    static void XMLCALL EndElementHandler( void*               pCallback,
                                           const char*         zName )
        throw();

    ///
    ///         Expat callback handler.
    ///
    ///         This method acts only as a router for \a pCallback
    ///         which is required to implement DWFXMLCallback::notifyStartNamespace().
    ///
    ///\param   pCallback           An implementation of DWFXMLCallback.
    ///\param   zPrefix             The namespace prefix.
    ///\param   zURI                The namespace URI (may be NULL).
    ///\throw   None
    ///
    _DWFCORE_API
    static void XMLCALL StartNamespaceDeclHandler( void*       pCallback,
                                                   const char* zPrefix,
                                                   const char* zURI )
        throw();

    ///
    ///         Expat callback handler.
    ///
    ///         This method acts only as a router for \a pCallback
    ///         which is required to implement DWFXMLCallback::notifyEndNamespace().
    ///
    ///\param   pCallback           An implementation of DWFXMLCallback.
    ///\param   zPrefix             The namespace prefix.
    ///\throw   None
    ///
    _DWFCORE_API
    static void XMLCALL EndNamespaceDeclHandler( void*         pCallback,
                                                 const char*   zPrefix )
        throw();

    ///
    ///         Expat callback handler.
    ///
    ///         This method acts only as a router for \a pCallback
    ///         which is required to implement DWFXMLCallback::notifyCharacterData().
    ///
    ///\param   pCallback           An implementation of DWFXMLCallback.
    ///\param   zCData              A \b non-null-terminated string of CDATA.
    ///\param   nLength             The number of characters in \a zCData.
    ///\throw   None
    ///
    _DWFCORE_API
    static void XMLCALL CharacterDataHandler( void*        pCallback,
                                              const char*  zCData,
                                              int          nLength )
        throw();


    ///
    ///         Call this function to process an xml stream and 
    ///         receive notice of various components of the stream.
    ///
    ///\param   rDocumentStream     a stream providing the xml
    ///\param   rDocumentReader     an object which receives and does something
    ///                             with notice of xml components
    ///
    _DWFCORE_API
    virtual void parseDocument( DWFInputStream& rDocumentStream ) 
        throw( DWFException );

private:
	DWFPointer<char>		   _apParseBuffer;
	XML_Parser				   _pXMLParser;
    DWFXMLCallback*            _pCallBack;
	//Flags
    bool	                   _bIsParsingFinished;
};

}

#endif

