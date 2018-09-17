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
#ifndef _DWFTK_XML_COMPATIBILITY_PREPROCESSOR_H
#define  _DWFTK_XML_COMPATIBILITY_PREPROCESSOR_H

///
///\file	dwfcore/utility/XMLCompatibilityPP.h
///\brief	This file contians the primary interface for the XML Markup 
///			Compatibility preprocessor.
///

#include "dwfcore/Iterator.h"
#include "dwfcore/String.h"
#include "dwfcore/utility/XMLCompatibilityExceptions.h"
#include "dwfcore/XML.h"

//
//
//
namespace DWFCore
{

///
///\ingroup     dwfpackage
///
///\class       DWFXMLCompatibilityPP dwfcore/utility/XMLCompatibilityPP.h
///\brief       The primary interface implementing an XML preprocessor of
///             the Open XML Markup Compatibility specification.
///\since       7.4
///
///\todo        Write full description for DWFXMLCompatibilityPP
///
class DWFXMLCompatibilityPP : public DWFXMLCallback
{

public:

    ///
    ///\brief   A list of strings
    ///
    ///         Used for identifying collections of uris and possibly
    ///         XML names, qualified or otherwise.
    ///
    typedef DWFCore::DWFIterator< DWFCore::DWFString > tStringIt;

    ///
    ///\brief   Constructor
    ///
    ///\param   rTarget the callback to which XML notifications will be forwarded
    ///                 after this preprocessor has completed.
    ///\throw   none
    _DWFCORE_API
    DWFXMLCompatibilityPP( DWFXMLCallback& rTarget ) throw();

    ///
    ///\brief   Destructor
    ///\throw   none
    ///
    _DWFCORE_API
    virtual ~DWFXMLCompatibilityPP() throw();

    ///
    ///\brief   Report errors encountered durig preprocessing
    ///\see     dwfcore/utility/XMLCompatibilityExceptions.h
    ///\throw   DWFXMLCompatibilityException  Any of a class of exceptions that
    ///         detail what went wrong during processing.
    ///
    ///         Since Expat is used to process the XML file, and it is
    ///         C software, it is not possible for our handlers to
    ///         throw exceptions.  Therefore, errors during processing
    ///         are recorded, and it is the client's responsibility to
    ///         periodically check for errors using this function.  It is
    ///         possible to wait until all source markup bytes have been
    ///         provided to the processor, but it is recommended to check
    ///         as frequently as is possible (while maintaining a decent
    ///         buffer size).
    ///
    _DWFCORE_API
    void signalError() throw( DWFXMLCompatibilityException );

    ///
    ///\brief   Define a namespace recognized by the target
    ///\throw   none
    ///\param   zUri    the uri of an xml namespace to recognize as
    ///                 supported.
    ///\param   pColl   an optional collection of namespaces that are
    ///                 subsumed by the current namespace.  The class
    ///                 does not retain access to the string list after
    ///                 this call so the caller is free to do anything
    ///                 with it.
    ///
    ///         The target, described in DWFXMLCompatibilityPP(), will
    ///         understand elements and attributes from some set of
    ///         namespaces.  This allows a way to specify what
    ///         those namespaces are.
    ///
    ///\note    The target need not be the object calling this.
    ///
    _DWFCORE_API
    void addSupportedNS( const DWFCore::DWFString& zUri,
                         const tStringIt* pStrings = NULL )
        throw();

    ///
    ///\brief   Define elements for which preprocessing is disabled
    ///
    ///\param   zQualName   the fully qualified, expanded, name of the element
    ///\throw   none
    ///
    ///         The "Markup Compatibility" spec allows the content consumer
    ///         to recognize elements for which compatibility processing
    ///         is disabled and therfore passed verbatim to the target.
    ///         The name must be qualified and expanded because a content
    ///         generator could choose any namespace prefix.
    ///
    _DWFCORE_API
    void addExtensionElementName( const DWFCore::DWFString& zQualName )
        throw();

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
    void notifyStartNamespace( const char* /*zPrefix*/,
                               const char* /*zURI*/ )
        throw()
    {;}

    ///
    ///\copydoc DWFXMLCallback::notifyEndNamespace()
    ///
    _DWFCORE_API
    void notifyEndNamespace( const char*   /*zPrefix*/ )
        throw()
    {;}

    ///
    ///\copydoc DWFXMLCallback::notifyCharacterData()
    ///
    _DWFCORE_API
    void notifyCharacterData( const char* /*zCData*/,
                              int         /*nLength*/ )
        throw()
    {;}

protected:

private:

    //
    // Not Implemented
    //
    DWFXMLCompatibilityPP( const DWFXMLCompatibilityPP& );
    DWFXMLCompatibilityPP& operator=( const DWFXMLCompatibilityPP& );
};

}

#endif
