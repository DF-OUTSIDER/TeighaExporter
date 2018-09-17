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


#if !defined WT_XAML_XML_HEADER
#define WT_XAML_XML_HEADER

#include "dwfcore/SkipList.h"

class XamlXML
{
public:
// String defines for each WhipTk class, overkill but we'll need most of them
    #define CALLBACK_MACRO(class_name, class_lower) \
        static const char* const kpz##class_name##_Element;
    CALLBACK_LIST
    #undef CALLBACK_MACRO

    typedef DWFCore::DWFCharKeySkipList<const char*> tAttributeMap;

    static void populateAttributeMap (const char** ppAttributeList, tAttributeMap& rMap);

    static const char* const kpzAbsolute_Value; //LinearGradientBrush
    static const char* const kpzAdapt_Patterns_Attribute; 
    static const char* const kpzAddress_Attribute;
    static const char* const kpzAngle_Attribute; //User_Hatch_Pattern->Angle
    static const char* const kpzArea_Attribute;
    static const char* const kpzBevel_Value; //StrokeLineJoin
    static const char* const kpzBidiLevel_Attribute;
    static const char* const kpzBoldItalicSimulation_Value; //StyleSimulations (Glyphs)
    static const char* const kpzBold_Attribute; //Font
    static const char* const kpzBoldSimulation_Value; //StyleSimulations (Glyphs)
    static const char* const kpzBounds_Attribute; //Text
    static const char* const kpzCannonical_Attribute; //Font_Extension
    static const char* const kpzCanvas_Element;
    static const char* const kpzCaretStops_Attribute;
    static const char* const kpzCenterX_Attribute;
    static const char* const kpzCenterY_Attribute;
    static const char* const kpzCharset_Attribute; //Font
    static const char* const kpzClip_Attribute;
    static const char* const kpzColorMap_Flag_Attribute;
    static const char* const kpzColor_Attribute; //this is for the SolidColorBrush, LinearGradientBrush
    static const char* const kpzColumns_Attribute; //User_Fill_Pattern columns
    static const char* const kpzComplete_Attribute;
    static const char* const kpzContours_Attribute;
    static const char* const kpzCount_Attribute; //W2XGouraud_Polytriangle_Element, total point count for Contour_Sets
    static const char* const kpzDash_Start_Cap_Attribute;
    static const char* const kpzDash_End_Cap_Attribute;
    static const char* const kpzData_Attribute;
    static const char* const kpzData_Size_Attribute;
    static const char* const kpzDelineate_Attribute;
    static const char* const kpzDescription_Attribute; //Embed
    static const char* const kpzDeviceFontName_Attribute;
    static const char* const kpzDpi_Attribute;  //Image's scanned resoultion
    static const char* const kpzEnd_Attribute; //Ellipse
    static const char* const kpzEndPoint_Attribute; //LinearGradientBrush
    static const char* const kpzExtent_Attribute;
    static const char* const kpzFalse_Value;
    static const char* const kpzFamily_Attribute; //Font
    static const char* const kpzFilename_Attribute; //Embed
    static const char* const kpzFill_Attribute;
    static const char* const kpzFlags_Attribute; //Font
    static const char* const kpzFlat_Value; //StrokeDashCap
    static const char* const kpzFontRenderingEmSize_Attribute;
    static const char* const kpzFontUri_Attribute;
    static const char* const kpzFormat_Attribute;
    static const char* const kpzFriendly_Name_Attribute;
    static const char* const kpzGlyphs_Element;
    static const char* const kpzGradientStop_Element; //LinearGradientBrush
    static const char* const kpzGradientStops_Element; //LinearGradientBrush
    static const char* const kpzHeight_Attribute; //Font
    static const char* const kpzHatch_Pattern_Element; //User_Hatch_Pattern->pattern
    static const char* const kpzId_Attribute;
    static const char* const kpzImageBrush_Element;
    static const char* const kpzImageSource_Attribute;
    static const char* const kpzIndex_Attribute; //W2XColor_Element, W2XMacro_Index
    static const char* const kpzIndices_Attribute;
    static const char* const kpzIsSideways_Attribute;
    static const char* const kpzItalic_Attribute; //Font
    static const char* const kpzItalicSimulation_Value; //StyleSimulations (Glyphs)
    static const char* const kpzLabel_Attribute;
    static const char* const kpzLanguage_Attribute;
    static const char* const kpzLine_End_Cap_Attribute;
    static const char* const kpzLine_Join_Attribute;
    static const char* const kpzLine_Start_Cap_Attribute;
    static const char* const kpzLinearGradientBrush_Element;
    static const char* const kpzLogfont_Attribute; //Font_Extension
    static const char* const kpzMIME_Attribute; //Embed
    static const char* const kpzMappingMode_Attribute; //LinearGradientBrush
    static const char* const kpzMatrixTransform_Element; //this is for the RenderTransform
    static const char* const kpzMatrix_Attribute; //this is for the RenderTransform
    static const char* const kpzMerge_Value; //MergeControl
    static const char* const kpzMiter_Angle_Attribute; 
    static const char* const kpzMiter_Length_Attribute;
    static const char* const kpzMiter_Value; //StrokeLineJoin
    static const char* const kpzMode_Attribute;
    static const char* const kpzName_Attribute;
    static const char* const kpzNamePrefix_Attribute;
    static const char* const kpzNavigateUri_Attribute;
    static const char* const kpzNumber_Attribute;
    static const char* const kpzObjectStream_Attribute; //Macro Definition
    static const char* const kpzOblique_Attribute; //Font
    static const char* const kpzOffset_Attribute; //LinearGradientBrush
    static const char* const kpzOpacityMask_Attribute;
    static const char* const kpzOpacity_Attribute;
    static const char* const kpzOpaque_Value; //MergeControl
    static const char* const kpzOriginX_Attribute;
    static const char* const kpzOriginY_Attribute;
    static const char* const kpzOverscore_Attribute; //Text
    static const char* const kpzPNG_Attribute;
    static const char* const kpzPath_Element;
    static const char* const kpzPitch_Attribute; //Font
    static const char* const kpzPoints_Attribute; //Macro_Draw
    static const char* const kpzPrivilege_Attribute;
    static const char* const kpzRadialGradientBrush_Element;
    static const char* const kpzRefName_Attribute;
    static const char* const kpzRef_Attribute;
    static const char* const kpzRenderTransform_Attribute;
    static const char* const kpzRender_Attribute;
    static const char* const kpzRenditionSync_Element;
    static const char* const kpzRequest_Attribute;
    static const char* const kpzResources_Element;
    static const char* const kpzResourceDictionary_Element;
    static const char* const kpzRows_Attribute; ////User_Fill_Pattern rows
    static const char* const kpzSkew_Attribute; //user_Hatch_Pattern->skew
    static const char* const kpzSource_Attribute;
    static const char* const kpzRotation_Attribute; //Font
    static const char* const kpzRound_Value; //StrokeDashCap, EndCap, StartCap, LineJoin
    static const char* const kpzScale_Attribute; 
    static const char* const kpzScreening_Percentage_Attribute;
    static const char* const kpzSolidColorBrush_Element;
    static const char* const kpzSpacing_Attribute; //Font,User_Hatch_Pattern->spacing
    static const char* const kpzSquare_Value; //StrokeDashCap
    static const char* const kpzStart_Attribute; //Ellipse
    static const char* const kpzStartPoint_Attribute; //LinearGradientBrush
    static const char* const kpzStaticResource_Declaration;
    static const char* const kpzStaticResource_DeclarationEnd;
    static const char* const kpzStrokeDashArray_Attribute;
    static const char* const kpzStrokeDashCap_Attribute;
    static const char* const kpzStrokeDashOffset_Attribute;
    static const char* const kpzStrokeEndLineCap_Attribute;
    static const char* const kpzStrokeLineJoin_Attribute;
    static const char* const kpzStrokeMiterLimit_Attribute;
    static const char* const kpzStrokeStartLineCap_Attribute;
    static const char* const kpzStrokeThickness_Attribute;
    static const char* const kpzStroke_Attribute;
    static const char* const kpzStyleSimulations_Attribute;
    static const char* const kpzStyle_Attribute; //MergeControl
    static const char* const kpzTileMode_Attribute;
    static const char* const kpzTile_Value; //VisualBrush
    static const char* const kpzTransform_Attribute;
    static const char* const kpzTransparent_Value; //MergeControl
    static const char* const kpzTriangle_Value; //StrokeDashCap
    static const char* const kpzTrue_Value;
    static const char* const kpzType_Attribute;
    static const char* const kpzURL_Attribute; //Embed
    static const char* const kpzURL_Item_Element;
    static const char* const kpzUnderline_Attribute; //Font
    static const char* const kpzUnderscore_Attribute; //Text
    static const char* const kpzUnicodeString_Attribute;
    static const char* const kpzValue_Attribute; //TextVAlign, TextHAlign
    static const char* const kpzVersion_Major_Attribute; //W2X
    static const char* const kpzVersion_Minor_Attribute; //W2X
    static const char* const kpzViewboxUnits_Attribute; 
    static const char* const kpzViewbox_Attribute;
    static const char* const kpzViewportUnits_Attribute;
    static const char* const kpzViewport_Attribute;
    static const char* const kpzVisibility_Attribute;
    static const char* const kpzVisualBrush_Element;
    static const char* const kpzVisual_Element;
    static const char* const kpzW2X_Element;
    static const char* const kpzWeight_Attribute;
    static const char* const kpzWidthScale_Attribute; //Font
    static const char* const kpzWidth_Attribute;
    static const char* const kpzX_Namespace;
    static const char* const kpzXaml_Namespace;
    static const char* const kpzXkey_Attribute;    
    static const char* const kpzXmlns_Attribute;
    static const char* const kpzXmlnsX_Attribute;
    static const char* const kpzX_Attribute; //User_Hatch_Pattern->x
    static const char* const kpzY_Attribute; //User_Hatch_Pattern->y
};
#endif // WT_XAML_FILE_HEADER
