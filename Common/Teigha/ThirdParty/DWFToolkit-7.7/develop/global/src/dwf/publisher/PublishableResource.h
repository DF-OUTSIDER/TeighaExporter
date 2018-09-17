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

#ifndef _DWFTK_PUBLISHABLERESOURCE_H
#define _DWFTK_PUBLISHABLERESOURCE_H


///
///\file        dwf/publisher/PublishableResource.h
///\brief       This file contains the DWFPublishableResource class declaration.
///

#ifndef DWFTK_READ_ONLY



#include "dwfcore/STL.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/Exception.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/publisher/Publishable.h"

namespace DWFToolkit
{

///
///\ingroup         dwfpublish
///
///\interface   DWFPublishableResource   dwf/publisher/PublishableResource.h    "dwf/publisher/PublishableResource.h"
///\brief       Content that can be added to/associated with a section.
///\since       7.0.1
///
class DWFPublishableResource : public DWFPublishable
                               _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
                             
{

public:

    ///
    ///\brief   This type defines a list of DWFPublishableResource pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFPublishableResource*)  tList;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFPublishableResource() {;}

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFPublishableResource() {;}
};

}

#endif  

#endif

