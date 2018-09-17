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
#ifndef _DWFTK_XML_COMPATIBILITY_CONTEXT_H
#define _DWFTK_XML_COMPATIBILITY_CONTEXT_H

///
///\file    dwfcore/utility/XMLCompatibilityContext.h
///\brief   Contains the declaration of a class tracking the state of a stack frame.
///

#include "dwfcore/utility/XMLCompatibilityInterfaces.h"
#include "dwfcore/utility/XMLNamespaceCollection.h"
#include "dwfcore/utility/XMLQualifiedNameCollection.h"

namespace DWFCore
{

///
///\class   DWFXMLCompatibilityContext  dwfcore/utility/XMLCompatibilityContext.h
///\since   7.4
///\brief   Simple stack member to track process state.
///
///         This class basically remembers the various configuration setting that can change
///         as elements are encountered when processing an XML Markup Compatibility document.
///
class DWFXMLCompatibilityContext
{

public:

    ///
    ///\brief   Constructor
    ///\param   rProcess    The process controlling processing at this point
    ///\throw   none
    ///
    DWFXMLCompatibilityContext( DWFXMLCompatibilityProcessTransferable& rProcess )
        throw();

    ///
    ///\brief   Constructor
    ///\param   rProcess    The process controlling processing at this point
    ///\param   rIgnoredNs  A list of namespaces that can be ignored
    ///\param   rProcessContentent  a list of elements whose contents should be processed
    ///                             even if the element itself is ignored.
    ///\throw   none
    ///
    DWFXMLCompatibilityContext( DWFXMLCompatibilityProcessTransferable& rProcess,
                                DWFXMLNamespaceCollection& rIgnoredNs,
                                DWFXMLQualifiedNameCollection& rProcessContent )
        throw();

    ///
    ///\brief Destructor
    ///\throw none
    ///
    virtual ~DWFXMLCompatibilityContext();

    ///
    ///\brief   Get the process tracked by this context.
    ///\return  the process
    ///\throw   none
    ///
    DWFXMLCompatibilityProcessTransferable& process()
        throw();

    ///
    ///\brief   Get the list of namespaces ignored in this context
    ///\return  the list of namespaces
    ///\throw   none
    ///
    DWFXMLNamespaceCollection&  ignoredNamespaces()
        throw();

    ///
    ///\brief   Get the list of elements that should have contents processed
    ///\return  the list of elements
    ///\throw   none
    ///
    DWFXMLQualifiedNameCollection& processedElements()
        throw();

protected:

private:

    static DWFXMLNamespaceCollection koNullNamespaces;
    static DWFXMLQualifiedNameCollection koNullNames;

    DWFXMLCompatibilityProcessTransferable& _rProcess;
    DWFXMLNamespaceCollection& _rIgnoredNs;
    DWFXMLQualifiedNameCollection& _rProcessContent;

    //
    // Not Implemented
    //
    DWFXMLCompatibilityContext(const DWFXMLCompatibilityContext&);
    DWFXMLCompatibilityContext& operator=(const DWFXMLCompatibilityContext&);

};

}

#endif
