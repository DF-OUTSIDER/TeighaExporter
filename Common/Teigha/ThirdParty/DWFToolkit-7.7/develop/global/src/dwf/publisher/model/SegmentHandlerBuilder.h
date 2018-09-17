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

#ifndef _DWFTK_SEGMENT_HANDLER_BUILDER_H
#define _DWFTK_SEGMENT_HANDLER_BUILDER_H

///
///\file        dwf/publisher/model/SegmentHandlerBuilder.h
///\brief       This file contains the DWFSegmentHandlerBuilder class declaration.
///

#ifndef DWFTK_READ_ONLY


#include "dwf/w3dtk/BOpcodeHandler.h"


namespace DWFToolkit
{


///
///\ingroup         dwfpublish3d
///
///\class           DWFSegmentHandlerBuilder     dwf/publisher/model/SegmentHandlerBuilder.h    "dwf/publisher/model/SegmentHandlerBuilder.h"
///\brief           Base implementation for a W3D scene segment op-code handler provider.
///\since           7.0.1
///
///                 This class should be specialized by classes that can provide W3D op-code handlers for scene segments.
///
class DWFSegmentHandlerBuilder _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFSegmentHandlerBuilder()
        throw()
    {;}

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFSegmentHandlerBuilder()
        throw()
    {;}

    ///
    ///             Copy Constructor
    ///
    ///\throw       None
    ///
    DWFSegmentHandlerBuilder( const DWFSegmentHandlerBuilder& )
        throw()
    {;}

    ///
    ///             Assignment Operator
    ///
    ///\throw       None
    ///
    DWFSegmentHandlerBuilder& operator=( const DWFSegmentHandlerBuilder& )
        throw()
    {
        return *this;
    }

    //
    // Segment Opcode Handler Builders
    //
public:

    ///
    ///             Returns a handler for closing a segment.
    ///
    ///\return      A reference to a op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Close_Segment& getCloseSegmentHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for opening a segment.
    ///
    ///\return      A reference to a op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Open_Segment& getOpenSegmentHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for including one segment within another.
    ///
    ///\return      A reference to a op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Referenced_Segment& getIncludeSegmentHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for referencing one segment from another.
    ///
    ///\return      A reference to a op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Referenced_Segment& getStyleSegmentHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }
};


}


#endif  
#endif

