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
#if !defined WT_XAML_RENDITION
#define WT_XAML_RENDITION

#include "whiptk/typedefs_defines.h"
#include "whiptk/rendition.h"
#include "XAML/XamlColor.h"
#include "XAML/XamlColorMap.h"
#include "XAML/XamlDelineate.h"
#include "XAML/XamlFill.h"
#include "XAML/XamlView.h"
#include "XAML/XamlViewport.h"
#include "XAML/XamlVisible.h"
#include "XAML/XamlLineWeight.h"
#include "XAML/XamlProjection.h"
#include "XAML/XamlURL.h"
#include "XAML/XamlLayer.h"
#include "XAML/XamlObject_Node.h"
#include "XAML/XamlFont.h"
#include "XAML/XamlFontExtension.h"
#include "XAML/XamlCode_Page.h"
#include "XAML/XamlLinePattern.h"
#include "XAML/XamlDashPattern.h"
#include "XAML/XamlLineStyle.h"
#include "XAML/XamlFillPattern.h"
#include "XAML/XamlPenPattern.h"
#include "XAML/XamlMacro_Scale.h"
#include "XAML/XamlMacro_Index.h"
#include "XAML/XamlMerge_Control.h"
#include "XAML/XamlText_Background.h"
#include "XAML/XamlText_HAlign.h"
#include "XAML/XamlText_VAlign.h"
#include "XAML/XamlUserFillPattern.h"
#include "XAML/XamlUserHatchPattern.h"
#include "XAML/XamlContrastColor.h"
#include "XAML/XamlAttribute_Url.h"

#ifdef WHIP_ATTRIBUTE_LIST
#undef WHIP_ATTRIBUTE_LIST
#endif

#define WHIP_ATTRIBUTE_LIST \
    WHIP_ATTRIBUTE_MACRO(Attribute_URL,attribute_url)\
    WHIP_ATTRIBUTE_MACRO(Code_Page,code_page)\
    WHIP_ATTRIBUTE_MACRO(Color,color)\
    WHIP_ATTRIBUTE_MACRO(Color_Map,color_map)\
    WHIP_ATTRIBUTE_MACRO(Contrast_Color,contrast_color)\
    WHIP_ATTRIBUTE_MACRO(Dash_Pattern,dash_pattern)\
    WHIP_ATTRIBUTE_MACRO(Delineate,delineate)\
    WHIP_ATTRIBUTE_MACRO(Fill,fill)\
    WHIP_ATTRIBUTE_MACRO(Fill_Pattern,fill_pattern)\
    WHIP_ATTRIBUTE_MACRO(Font,font)\
    WHIP_ATTRIBUTE_MACRO(Font_Extension,font_extension)\
    WHIP_ATTRIBUTE_MACRO(Layer,layer)\
    WHIP_ATTRIBUTE_MACRO(Line_Pattern,line_pattern)\
    WHIP_ATTRIBUTE_MACRO(Line_Style,line_style)\
    WHIP_ATTRIBUTE_MACRO(Line_Weight,line_weight)\
    WHIP_ATTRIBUTE_MACRO(Macro_Scale,macro_scale)\
    WHIP_ATTRIBUTE_MACRO(Macro_Index,macro_index)\
    WHIP_ATTRIBUTE_MACRO(Merge_Control,merge_control)\
    WHIP_ATTRIBUTE_MACRO(Object_Node,object_node)\
    WHIP_ATTRIBUTE_MACRO(Pen_Pattern,pen_pattern)\
    WHIP_ATTRIBUTE_MACRO(Projection,projection)\
    WHIP_ATTRIBUTE_MACRO(User_Fill_Pattern,user_fill_pattern)\
    WHIP_ATTRIBUTE_MACRO(User_Hatch_Pattern,user_hatch_pattern)\
    WHIP_ATTRIBUTE_MACRO(Text_Background,text_background)\
    WHIP_ATTRIBUTE_MACRO(Text_HAlign,text_halign)\
    WHIP_ATTRIBUTE_MACRO(Text_VAlign,text_valign)\
    WHIP_ATTRIBUTE_MACRO(URL,url)\
    WHIP_ATTRIBUTE_MACRO(Viewport,viewport)\
    WHIP_ATTRIBUTE_MACRO(Visibility,visibility)

//NO    WHIP_ATTRIBUTE_MACRO(BlockRef,blockref)
//NO    WHIP_ATTRIBUTE_MACRO(Marker_Size,marker_size)
//NO    WHIP_ATTRIBUTE_MACRO(Marker_Symbol,marker_symbol)


class XAMLTK_API WT_XAML_Rendition :
    public WT_Rendition
{

public:
    WT_XAML_Rendition(void);
    WT_XAML_Rendition( WT_XAML_Rendition const &other) 
    : WT_Rendition( other )
    #define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
        , m_xaml_##class_lower (other.m_xaml_##class_lower)
        WHIP_ATTRIBUTE_LIST
    #undef WHIP_ATTRIBUTE_MACRO
    {}
    
    virtual ~WT_XAML_Rendition(void);

#define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
    WT_XAML_##class_name            m_xaml_##class_lower;
    WHIP_ATTRIBUTE_LIST
#undef WHIP_ATTRIBUTE_MACRO

    /// Assigns the contents of a WT_Rendition from another.
    WT_XAML_Rendition & operator= (WT_XAML_Rendition const & other)
    {
        WT_Rendition::operator = (other);
        
        #define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
            m_xaml_##class_lower = other.m_xaml_##class_lower;
            WHIP_ATTRIBUTE_LIST
        #undef WHIP_ATTRIBUTE_MACRO

        return *this;
    }

    #define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
        virtual WT_##class_name const & class_lower()    const { return m_xaml_##class_lower; }
        WHIP_ATTRIBUTE_LIST
    #undef WHIP_ATTRIBUTE_MACRO

    #define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
        virtual WT_##class_name & class_lower() { m_changed_flags |= class_name##_Bit; return m_xaml_##class_lower; }
        WHIP_ATTRIBUTE_LIST
    #undef WHIP_ATTRIBUTE_MACRO

	 virtual WT_Result sync(WT_File & file, WT_Integer32 required = 0);

     WT_Result sync_parts(WT_File & file, WT_Integer32 needed);
};



#endif //WT_XAML_RENDITION
