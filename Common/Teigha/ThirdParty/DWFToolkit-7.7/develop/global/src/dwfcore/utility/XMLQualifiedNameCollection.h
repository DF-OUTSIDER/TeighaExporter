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
#ifndef _DWFTK_XML_QUALIFIED_NAME_COLLECTION_H
#define _DWFTK_XML_QUALIFIED_NAME_COLLECTION_H

#include "dwfcore/Exception.h"
#include "dwfcore/String.h"

///
///\file    dwfcore/utility/XMLQualifiedNameCollection.h
///\brief   The declaration of a class for collecting qualified names
///

namespace DWFCore
{

//
// forward declarations
//

class DWFXMLNamespaceCollection;

///
///\class   DWFXMLQualifiedNameCollection dwfcore/utility/XMLQualifiedNameCollection.h
///\since   7.4
///\brief   Store a collection of qualified names
///
///         This is a specialization of a collection that is
///         intended to simplify verification of the existance
///         of a qualified name.  This will take in to consideration
///         expansion of any prefixes and all possible prefixes for
///         a given namespace.
///
class DWFXMLQualifiedNameCollection
{

public:

    ///
    ///\brief   Constructor
    ///\param   rNamespaces a collection of namespaces that can be used
    ///         to expand prefixes.  No copy will be made, so in order
    ///         for the collection to be useful, it must have a lifetime
    ///         equal to or greater than that of this object
    ///\throws  none
    ///
    _DWFCORE_API
    DWFXMLQualifiedNameCollection( const DWFXMLNamespaceCollection& rNamespaces )
        throw();

    ///
    ///\brief   Destructor
    ///\throws  none
    ///
    _DWFCORE_API
    virtual ~DWFXMLQualifiedNameCollection()
        throw();


    ///
    ///\brief   Add a qualified name to the collection
    ///\param   zQualName   An xml name that is (optionally) qualified
    ///\throw   DWFCore::DWFException  If an error was encountered
    ///         while attempting to add the uri. Usually, insufficient memory.
    ///
    ///         This method should be able to determine if the name is qualified
    ///         or not and if it is expanded or not.  If the name is not 
    ///         qualified, it is assumed to belong to the global namespace.
    ///
    _DWFCORE_API
    void add( const DWFCore::DWFString& zQualName )
        throw( DWFCore::DWFException );

    ///
    ///\brief   Check for the existance of a name
    ///\param   zPfx    the namespace prefix that this name belongs to.
    ///                 an empty string indicates the default namespace.
    ///\param   zName   the name
    ///\throw   none
    ///
    ///         This method should be able to determine if the prefix is
    ///         is a full namespace uri or not.
    ///
    _DWFCORE_API
    bool has( const DWFCore::DWFString& zPfx,
              const DWFCore::DWFString& zName ) const
        throw();

    ///
    ///\brief   Check for the existance of a name
    ///\param   zQualName   the (optionally) qualified name to search for
    ///\throw   none
    ///
    ///         This method should be able to determine if a name is qualified
    ///         or not.
    ///
    _DWFCORE_API
    bool has( const DWFCore::DWFString& zQualName ) const
        throw();

protected:

private:

};

}

#endif
