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
#ifndef _DWFTK_XML_NAMESPACE_COLLECTION_H
#define _DWFTK_XML_NAMESPACE_COLLECTION_H

///
///\file    dwfcore/utility/XMLNamespaceCollection.h
///\brief   Contains the class declaration of a collection of namespaces
///

#include "dwfcore/Exception.h"
#include "dwfcore/STL.h"
#include "dwfcore/String.h"

namespace DWFCore
{

///
///\class   DWFXMLNamespaceCollection dwfcore/utility/XMLNamespaceCollection.h
///\brief   Store a collection of namespaces
///\since   7.4
///
///         This specialization of a collection of strings which
///         is intended to simplify verification of the existance
///         of a given namespace.
///
class DWFXMLNamespaceCollection
{

public:

    ///
    ///\brief   Constructor
    ///\throw   none
    ///
    _DWFCORE_API
    DWFXMLNamespaceCollection() throw();

    ///
    ///\brief   Destructor
    ///\throw   none
    ///
    _DWFCORE_API
    virtual ~DWFXMLNamespaceCollection() throw();

    ///
    ///\brief   Add a namespace prefixless namespace.
    ///\throw   DWFCore::DWFException  If an error was encountered
    ///         while attempting to add the uri. Usually, insufficient memory.
    ///\param   zUri    The uri identifying the namespace.
    ///
    ///         This helps in situations where namespaces are referrenced
    ///         in multiple ways.
    ///
    _DWFCORE_API
    void add( const DWFCore::DWFString& zUri )
        throw( DWFCore::DWFException );

    ///
    ///\brief   Add a namespace and it's prefix to the collection.
    ///\throw   DWFCore::DWFException  If an error was encountered
    ///         while attempting to add the uri. Usually, insufficient memory.
    ///\param   zPfx    The abbreviation prefix for the namespace.
    ///\param   zUri    The uri unambiguously identifying the namespace
    ///
    _DWFCORE_API
    void add( const DWFCore::DWFString& zPfx,
              const DWFCore::DWFString& zUri)
        throw( DWFCore::DWFException);

    ///
    ///\brief   Test for the presence of a namespace in this collection
    ///\throw   none
    ///\param   zUri    the full uri to namespace to check for.
    ///
    ///         This should return true if the namespace is in the collection
    ///         regardless of what prefix(es) are registered for the namespace.
    ///
    _DWFCORE_API
    bool has( const DWFCore::DWFString& zUri ) const
        throw();

    ///
    ///\brief   Test for the presence of a namespace in this collection
    ///\throw   none
    ///\param   zPfx    the prefix of the namespace to search for.
    ///
    ///         This should return true if the namespace prefix is registered
    ///         in the collection.
    ///
    _DWFCORE_API
    bool hasByPrefix( const DWFCore::DWFStringException& zPfx ) const
        throw();

protected:

private:

    //
    // Not Implemented
    //

    DWFXMLNamespaceCollection( const DWFXMLNamespaceCollection& );
    DWFXMLNamespaceCollection& operator=( const DWFXMLNamespaceCollection& );

};

}

#endif
