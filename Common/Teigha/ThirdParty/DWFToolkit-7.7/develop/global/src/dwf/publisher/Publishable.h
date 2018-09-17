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

#ifndef _DWFTK_PUBLISHABLE_H
#define _DWFTK_PUBLISHABLE_H


///
///\file        dwf/publisher/Publishable.h
///\brief       This file contains the DWFPublishable class declaration.
///

#ifndef DWFTK_READ_ONLY



#include "dwfcore/STL.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/Exception.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"

namespace DWFToolkit
{

///
///\ingroup         dwfpublish
///
///\interface   DWFPublishable   dwf/publisher/Publisher.h    "dwf/publisher/Publisher.h"
///\brief       Base class for interaction with the DWFPublisher.
///\since       7.0.1
///
///             This class defines the most basic requirements for publishing content.
///
class DWFPublishable
    // _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE - intentionally omitted, as this is just an interface
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFPublishable() {;}

    ///
    ///         Returns the MIME type of the publishable data.
    ///
    ///\return  The MIME type.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFString& getMIMEType()
        throw( DWFException ) = 0;

    ///
    ///         Returns a stream from which the publishable data can be read.
    ///
    ///\return  The data stream.
    ///         The caller must release the stream with \b DWFCORE_FREE_OBJECT.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFInputStream* getInputStream()
        throw( DWFException ) = 0;

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFPublishable() {;}
};

}

#endif  

#endif

