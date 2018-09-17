//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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


#ifndef _DWFTK_DEPENDENCY_H
#define _DWFTK_DEPENDENCY_H


///
///\file        dwf/package/Dependency.h
///\brief       This file contains the DWFDependency class declaration.
///



#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"


namespace DWFToolkit
{

///
///\class   DWFDependency   dwf/package/Dependency.h "dwf/package/Dependency.h"
///\since       7.0.1
///
class DWFDependency : public DWFXMLBuildable
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFDependency()
        throw()
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFDependency()
        throw(){;}

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    virtual void parseAttributeList( const char** /*ppAttributeList*/ )
        throw( DWFException )
    {;}

private:

};

}

#endif
