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

#ifndef _DWFTK_STYLE_SEGMENT_H
#define _DWFTK_STYLE_SEGMENT_H

///
///\file        dwf/publisher/model/StyleSegment.h
///\brief       This file contains the DWFStyleSegment class declaration.
///


#ifndef DWFTK_READ_ONLY


#include "dwfcore/String.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/publisher/model/SegmentHandlerBuilder.h"
#include "dwf/publisher/model/AttributeHandlerBuilder.h"


namespace DWFToolkit
{

///
///\ingroup         dwfpublish3d
///
///\class           DWFStyleSegment     dwf/publisher/model/StyleSegment.h    "dwf/publisher/model/StyleSegment.h"
///\brief           Represents a special segment that contains one or more attributes that can be applied
///                 as a whole, to other scene segments.
///\since           7.0.1
///
class DWFStyleSegment : public DWFAttributeHandlerBuilder
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief       The local style library for the scene graph.
    ///
    static const char* const kz_Style_Library;
    ///
    ///\brief       The name of the predefined segment for stylizing feature edge segments.
    ///
    static const char* const kz_StyleSegment_PublishedEdges;

public:

    ///
    ///             Constructor
    ///
    ///\param       rSegmentBuilder     An interface for acquiring the segment-specific op-code handlers.
    ///\param       rAttributeBuilder   An interface for acquiring the attribute-specific op-code handlers.
    ///\param       nID                 A unique identifier for the segment.  Style segments are not explicitly named.
    ///\throw       None
    ///
    _DWFTK_API
    DWFStyleSegment( DWFSegmentHandlerBuilder&   rSegmentBuilder,
                     DWFAttributeHandlerBuilder& rAttributeBuilder,
                     unsigned int                nID )
        throw();

    ///
    ///             Copy Constructor
    ///
    ///\throw       None
    ///
    _DWFTK_API
    DWFStyleSegment( const DWFStyleSegment& )
        throw();

    ///
    ///             Assignment Operator
    ///
    ///\throw       None
    ///
    _DWFTK_API
    DWFStyleSegment& operator=( const DWFStyleSegment& )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFTK_API
    virtual ~DWFStyleSegment()
        throw();

    ///
    ///             Opens the segment for use.
    ///
    ///             The usage model for segments is:
    ///             -# Create or obtain a style segment object.
    ///             -# Open the segment.
    ///             -# Add attributes.
    ///             -# Close the segment.
    ///             -# Repeat. (The same segment object can be reused.)
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void open()
        throw( DWFException );

    ///
    ///\copydoc DWFSegment::close
    ///
    _DWFTK_API
    void close()
        throw( DWFException );

    ///
    ///             Returns the internal segment name.
    ///
    ///             This name is actually the path to the segment in the include library.
    ///
    ///\return      The internal segment name.
    ///\throw       None
    ///
    const DWFString& name() const
        throw()
    {
        return _zName;
    }

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getCameraHandler
    ///
    _DWFTK_API
    TK_Camera& getCameraHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getColorHandler
    ///
    _DWFTK_API
    TK_Color& getColorHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getColorMapHandler
    ///
    _DWFTK_API
    TK_Color_Map& getColorMapHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getColorRGBHandler
    ///
    _DWFTK_API
    TK_Color_RGB& getColorRGBHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getEdgePatternHandler
    ///
    _DWFTK_API
    TK_Named& getEdgePatternHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getEdgeWeightHandler
    ///
    _DWFTK_API
    TK_Size& getEdgeWeightHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getFacePatternHandler
    ///
    _DWFTK_API
    TK_Enumerated& getFacePatternHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getHandednessHandler
    ///
    _DWFTK_API
    TK_Enumerated& getHandednessHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getHeuristicsHandler
    ///
    _DWFTK_API
    TK_Heuristics& getHeuristicsHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getLinePatternHandler
    ///
    _DWFTK_API
    TK_Named& getLinePatternHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getLineStyleHandler
    ///
    _DWFTK_API
    TK_Line_Style& getLineStyleHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getLineWeightHandler
    ///
    _DWFTK_API
    TK_Size& getLineWeightHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getMarkerSizeHandler
    ///
    _DWFTK_API
    TK_Size& getMarkerSizeHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getMarkerSymbolHandler
    ///
    _DWFTK_API
    TK_Enumerated& getMarkerSymbolHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getModellingMatrixHandler
    ///
    _DWFTK_API
    TK_Matrix& getModellingMatrixHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getRenderingOptionsHandler
    ///
    _DWFTK_API
    TK_Rendering_Options& getRenderingOptionsHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getSelectabilityHandler
    ///
    _DWFTK_API
    TK_Selectability& getSelectabilityHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getTextAlignmentHandler
    ///
    _DWFTK_API
    TK_Enumerated& getTextAlignmentHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getTextFontHandler
    ///
    _DWFTK_API
    TK_Text_Font& getTextFontHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getTextPathHandler
    ///
    _DWFTK_API
    TK_Point& getTextPathHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getTextSpacingHandler
    ///
    _DWFTK_API
    TK_Size& getTextSpacingHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getUserOptionsHandler
    ///
    _DWFTK_API
    TK_User_Options& getUserOptionsHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getUnicodeOptionsHandler
    ///
    _DWFTK_API
    TK_Unicode_Options& getUnicodeOptionsHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getVisibilityHandler
    ///
    _DWFTK_API
    TK_Visibility& getVisibilityHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getWindowHandler
    ///
    _DWFTK_API
    TK_Window& getWindowHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getWindowHandler
    ///
    _DWFTK_API
    TK_Enumerated& getWindowFrameHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getWindowHandler
    ///
    _DWFTK_API
    TK_Enumerated& getWindowPatternHandler()
        throw( DWFException );

private:

    bool                        _bOpen;
    DWFString                   _zName;
    unsigned int                _nID;

    DWFSegmentHandlerBuilder&   _rSegmentBuilder;
    DWFAttributeHandlerBuilder& _rAttributeBuilder;

private:

    //
    // Not Implemented
    //

    DWFStyleSegment();
};


    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

_declspec(selectany) const char* const DWFStyleSegment::kz_Style_Library                = "?Style Library/";
_declspec(selectany) const char* const DWFStyleSegment::kz_StyleSegment_PublishedEdges  = "?Style Library/_dwfw3d_PublishedEdges";

#endif
#endif


}


#endif  
#endif
