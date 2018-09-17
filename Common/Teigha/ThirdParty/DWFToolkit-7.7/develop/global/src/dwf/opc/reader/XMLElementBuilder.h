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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/reader/XMLElementBuilder.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCXMLBUILDER_H
#define _DWFTK_OPCXMLBUILDER_H


///
///\file        dwf/opc/reader/XMLElementBuilder.h
///\brief       This file contains the declarations of the OPCXMLElementBuilder class using for XML processing.
///

#include "dwfcore/Core.h"
#include "dwfcore/String.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"

namespace DWFToolkit
{

///
///\ingroup     opc
///
///\class       OPCXMLElementBuilder    dwf/opc/reader/XMLElementBuilder.h     "dwf/opc/reader/XMLElementBuilder.h"
///\brief       This class builds DWFXMLBuildable objects for the parser.
///\since       7.4.0
///
class OPCXMLElementBuilder _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    OPCXMLElementBuilder()
        throw()
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~OPCXMLElementBuilder()
        throw()
    {;}

    ///
    ///         Build an object of the the given type. This will create an object
    ///         of the given type and ask it to parse the provided list of attributes.
    ///         The pointer will be set to the new object.
    ///
    ///\param   pT                  An uninitialized pointer of the type that
    ///                             needs to be created. The method will assign the
    ///                             pointer the address of the object.
    ///\param   ppAttributeList     The attribute list for class member data.
    ///\throw   DWFException
    ///
    template<class T>
    void build( T*& pT, const char** ppAttributeList )
    throw( DWFException )
    {
        pT = DWFCORE_ALLOC_OBJECT( T );

        if (pT == NULL)
        {
            _DWFCORE_THROW( DWFMemoryException, /*NOXLATE*/L"Failed to allocate OPCXMLBuildable object" );
        }

        pT->parseAttributeList( ppAttributeList );
    }

};

}

#endif

