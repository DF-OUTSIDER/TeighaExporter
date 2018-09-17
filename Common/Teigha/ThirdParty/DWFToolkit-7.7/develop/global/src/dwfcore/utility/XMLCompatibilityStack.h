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
#ifndef _DWFTK_XML_COMPATIBILITY_STACK
#define _DWFTK_XML_COMPATIBILITY_STACK

#include "dwfcore/Exception.h"

///
///\file    dwfcore/utility/XMLCompatibilityStack.h
///\brief   Declaration of a class for tracking context information
///
namespace DWFCore
{

//
// forward declarations
//

class DWFXMLCompatibilityContext;

///
///\class   DWFXMLCompatibilityStack dwfcore/utility/XMLCompatibilityStack.h
///\brief   Track the state of preprocessing as elements are encountered.
///\since   7.4
///
class DWFXMLCompatibilityStack
{

public:

    ///
    ///\brief   Add a context to the stack
    ///\param   pCtx    a new context to apply to the processing.  This 
    ///         collection will take ownership of the context.
    ///\return  the frame number on to which the context is pushed.
    ///\throw   DWFCore::DWFException if there is an error adding
    ///         the 
    ///
    int push( const DWFXMLCompatibilityContext* pCtx )
        throw( DWFCore::DWFException );

    ///
    ///\brief   Remove contexts from the stack
    ///\param   nFrame the frame number at which to stop popping contexts
    ///\throw   none
    ///
    ///         Contexts will be removed from the stack until the first one
    ///         associated with nFrame is encountered.
    ///
    void popTo( int nFrame )
        throw();

protected:

private:

};

}

#endif
