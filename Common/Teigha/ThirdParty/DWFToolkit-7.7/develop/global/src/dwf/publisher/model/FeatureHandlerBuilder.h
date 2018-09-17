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

#ifndef _DWFTK_FEATURE_HANDLER_BUILDER_H
#define _DWFTK_FEATURE_HANDLER_BUILDER_H


///
///\file        dwf/publisher/model/FeatureHandlerBuilder.h
///\brief       This file contains the DWFFeatureHandlerBuilder class declaration.
///





#include "dwfcore/Exception.h"
#include "dwf/w3dtk/BOpcodeHandler.h"


namespace DWFToolkit
{

///
///\ingroup         dwfpublish3d
///
///\class           DWFFeatureHandlerBuilder     dwf/publisher/model/FeatureHandlerBuilder.h    "dwf/publisher/model/FeatureHandlerBuilder.h"
///\brief           Base implementation for a W3D scene feature op-code handler provider.
///\since           7.0.1
///
///                 This class should be specialized by classes that can provide W3D op-code handlers for scene features.
///
class DWFFeatureHandlerBuilder _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFFeatureHandlerBuilder()
        throw()
    {;}

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFFeatureHandlerBuilder()
        throw()
    {;}

    ///
    ///             Copy Constructor
    ///
    ///\throw       None
    ///
    DWFFeatureHandlerBuilder( const DWFFeatureHandlerBuilder& )
        throw()
    {;}

    ///
    ///             Assignment Operator
    ///
    ///\throw       None
    ///
    DWFFeatureHandlerBuilder& operator=( const DWFFeatureHandlerBuilder& )
        throw()
    {
        return *this;
    }

public:

    ///
    ///\ingroup     w3dtkhimage
    ///
    ///             Returns a handler for the texture feature.
    ///
    ///\return      A reference to a op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Texture& getTextureHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhimage
    ///
    ///             Returns a handler for the texture matrix feature.
    ///
    ///\return      A reference to a op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Matrix& getTextureMatrixHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }
};


}


#endif
