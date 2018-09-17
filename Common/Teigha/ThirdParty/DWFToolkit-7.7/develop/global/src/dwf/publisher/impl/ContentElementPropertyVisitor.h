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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/publisher/impl/ContentElementPropertyVisitor.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//

#ifndef _DWFTK_CONTENT_ELEMENT_PROPERTY_VISITOR_H
#define _DWFTK_CONTENT_ELEMENT_PROPERTY_VISITOR_H

///
///\file        dwf/publisher/impl/ContentElementPropertyVisitor.h
///\brief       This file contains the DWFContentElementPropertyVisitor class declaration.
///

#ifndef DWFTK_READ_ONLY

#include "dwf/publisher/PropertyVisitor.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpublish
///
///\class       DWFContentElementPropertyVisitor   dwf/publisher/impl/ContentElementPropertyVisitor.h    "dwf/publisher/impl/ContentElementPropertyVisitor.h"
///\brief       An implementation for objects (visitors) that publishers can use with publishable sources to extract their property data. This 
///             implementation is used for working with content elements available since DWF toolkit version 7.2
///\since       7.2.0
///

class DWFContentElementPropertyVisitor : public DWFPropertyVisitor
                                         _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
};

}

#endif
#endif





