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

#ifndef _DWFTK_ATTRIBUTE_HANDLER_BUILDER_H
#define _DWFTK_ATTRIBUTE_HANDLER_BUILDER_H


///
///\file        dwf/publisher/model/AttributeHandlerBuilder.h
///\brief       This file contains the DWFAttributeHandlerBuilder class declaration.
///



#include "dwfcore/Exception.h"
#include "dwf/w3dtk/BOpcodeHandler.h"


namespace DWFToolkit
{


///
///\ingroup         dwfpublish3d
///
///\class           DWFAttributeHandlerBuilder     dwf/publisher/model/AttributeHandlerBuilder.h    "dwf/publisher/model/AttributeHandlerBuilder.h"
///\brief           Base implementation for a W3D scene attribute op-code handler provider.
///\since           7.0.1
///
///                 This class should be specialized by classes that can provide W3D op-code handlers for scene attributes.
///
class DWFAttributeHandlerBuilder _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFAttributeHandlerBuilder()
        throw()
    {;}

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFAttributeHandlerBuilder()
        throw()
    {;}

    ///
    ///             Copy Constructor
    ///
    ///\throw       None
    ///
    DWFAttributeHandlerBuilder( const DWFAttributeHandlerBuilder& )
        throw()
    {;}

    ///
    ///             Assignment Operator
    ///
    ///\throw       None
    ///
    DWFAttributeHandlerBuilder& operator=( const DWFAttributeHandlerBuilder& )
        throw()
    {
        return *this;
    }

public:

    ///
    ///             Returns a handler for the camera attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Camera& getCameraHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the color attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Color& getColorHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the color map attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Color_Map& getColorMapHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the RGB color attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Color_RGB& getColorRGBHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhpandw
    ///
    ///             Returns a handler for the edge pattern attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Named& getEdgePatternHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhpandw
    ///
    ///             Returns a handler for the edge weight attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Size& getEdgeWeightHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhpandw
    ///
    ///             Returns a handler for the face pattern attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Enumerated& getFacePatternHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the polygon handedness attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Enumerated& getHandednessHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the scene heuristics attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Heuristics& getHeuristicsHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhpandw
    ///
    ///             Returns a handler for the line pattern attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Named& getLinePatternHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhpandw
    ///
    ///             Returns a handler for the line style attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Line_Style& getLineStyleHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhpandw
    ///
    ///             Returns a handler for the line weight attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Size& getLineWeightHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the marker size attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Size& getMarkerSizeHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the marker symbol attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Enumerated& getMarkerSymbolHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the modelling matrix (transform) attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Matrix& getModellingMatrixHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the rendering options attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Rendering_Options& getRenderingOptionsHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the object selectability attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Selectability& getSelectabilityHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhtextf
    ///
    ///             Returns a handler for the text alignment attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Enumerated& getTextAlignmentHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhtextf
    ///
    ///             Returns a handler for the text font attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Text_Font& getTextFontHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhtextf
    ///
    ///             Returns a handler for the text path attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Point& getTextPathHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhtextf
    ///
    ///             Returns a handler for the text spacing attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Size& getTextSpacingHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the user options attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_User_Options& getUserOptionsHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhtextf
    ///
    ///             Returns a handler for the Unicode options attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Unicode_Options& getUnicodeOptionsHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the visibility attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Visibility& getVisibilityHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for the window attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Window& getWindowHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }


    ///
    ///             Returns a handler for the window attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Enumerated& getWindowFrameHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }


    ///
    ///             Returns a handler for the window attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Enumerated& getWindowPatternHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }


    ///
    /// Opens the most recent geometry for attribute changes.
    ///
    ///\throw       DWFException
    ///
    virtual void openLocalLightAttributes()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    /// Closes the ability to do geometry attribute change that was initiated by a call to openLocalLightAttributes.
    ///
    ///\throw       DWFException
    ///
    virtual void closeLocalLightAttributes()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }



protected:
    ///
    ///             Returns a handler for the view (camera) attribute.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Camera& getViewHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }
};


}




#endif

