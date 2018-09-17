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

#if !defined RENDITION_HEADER
#define RENDITION_HEADER

///
///\file        dwf/whiptk/rendition.h
///

#include "whiptk/whipcore.h"
#include "whiptk/rendopts.h"
#include "whiptk/dwginfo.h"
#include "whiptk/color.h"
#include "whiptk/colormap.h"
#include "whiptk/backgrnd.h"
#include "whiptk/fill.h"
#include "whiptk/view.h"
#include "whiptk/viewport.h"
#include "whiptk/visible.h"
#include "whiptk/lweight.h"
#include "whiptk/marksize.h"
#include "whiptk/marksymb.h"
#include "whiptk/projection.h"
#include "whiptk/url.h"
#include "whiptk/layer.h"
#include "whiptk/object_node.h"
#include "whiptk/font.h"
#include "whiptk/code_page.h"
#include "whiptk/linepat.h"
#include "whiptk/dashpat.h"
#include "whiptk/linestyle.h"
#include "whiptk/plotinfo.h"
#include "whiptk/merge_control.h"
#include "whiptk/font_extension.h"
#include "whiptk/plot_optimized.h"
#include "whiptk/fillpat.h"
#include "whiptk/blockref.h"
#include "whiptk/penpat.h"
#include "whiptk/delineate.h"
#include "whiptk/macro_scale.h"
#include "whiptk/macro_index.h"
#include "whiptk/text_background.h"
#include "whiptk/text_halign.h"
#include "whiptk/text_valign.h"
#include "whiptk/usrfillpat.h"
#include "whiptk/usrhatchpat.h"
#include "whiptk/contrastcolor.h"
#include "whiptk/attribute_url.h"

// IMP. Attributes Marker_Size and Marker_Symbol have been deprecated and are only supported for
//      W2D files lower then version 601

#define WHIP_ATTRIBUTE_LIST \
    WHIP_ATTRIBUTE_MACRO(Attribute_URL,attribute_url)\
    WHIP_ATTRIBUTE_MACRO(BlockRef,blockref)\
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
    WHIP_ATTRIBUTE_MACRO(Marker_Size,marker_size)\
    WHIP_ATTRIBUTE_MACRO(Marker_Symbol,marker_symbol)\
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


/** \addtogroup groupRenditionAttributes Rendition attributes
 *  Contains objects which describe attributes to be applied to geometry.
 *  \sa WT_Rendition, WT_Rendering_Options
 *  @{
 *  @}
 */


/** \addtogroup groupFileIO
 *  @{
 */

/// Contains attributes to be applied to geometry.
/** The WT_File::desired_rendition() should be used when setting rendition attributes
 *  during file writing.  When reading a file, WT_File::rendition() should be used.
 *  \sa WT_File, \ref groupDrawable "Drawable objects", \ref groupRenditionAttributes "Rendition attributes", WT_Rendering_Options
 */
class WHIPTK_API WT_Rendition
{
protected:
    WT_Integer32            m_changed_flags;    

protected:
    WT_Rendering_Options    m_rendering_options;
    WT_Drawing_Info         m_drawing_info;
    WT_Plot_Info            m_plot_info;
    WT_URL_List             m_url_lookup_list;

    WT_Result sync_parts(WT_File & file, WT_Integer32 needed);

#define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
    WT_##class_name            m_##class_lower;
    WHIP_ATTRIBUTE_LIST
#undef WHIP_ATTRIBUTE_MACRO

public:
    /// Used in identifying which rendition attributes are applicable to a given object and/or which have changed and need to be serialized if needed.
    enum WT_Attribute_Bits
    {
        // For speed's sake, these should be ordered from most oftenly changed
        // to least oftenly changed...
        Color_Bit              = 0x00000001,
        Fill_Bit               = 0x00000002,
        Visibility_Bit         = 0x00000004,
        URL_Bit                = 0x00000008,
        Line_Weight_Bit        = 0x00000010,
        Line_Style_Bit         = 0x00000020,
        Macro_Scale_Bit        = 0x00000040,    // IMP. This Bit is shared by WT_Marker_Size ( version < 601) and WT_Macro_Size ( version => 601)
        Marker_Size_Bit        = 0x00000040,
        Macro_Index_Bit        = 0x00000080,    // IMP. This Bit is shared by WT_Marker_Symbol ( version < 601) and WT_Macro_Index ( version => 601)    
        Marker_Symbol_Bit      = 0x00000080,
        Layer_Bit              = 0x00000100,    
        Color_Map_Bit          = 0x00000200,
        Line_Pattern_Bit       = 0x00000400,
        Projection_Bit         = 0x00000800,
        Font_Bit               = 0x00001000,
        Viewport_Bit           = 0x00002000,
        Code_Page_Bit          = 0x00004000,
        Dash_Pattern_Bit       = 0x00008000,
        Fill_Pattern_Bit       = 0x00010000,
        Merge_Control_Bit      = 0x00020000,
        BlockRef_Bit           = 0x00040000,
        Pen_Pattern_Bit        = 0x00080000,
        Font_Extension_Bit     = 0x00100000,
        Object_Node_Bit        = 0x00200000,
        Text_Background_Bit    = 0x00400000,
        Text_HAlign_Bit        = 0x00800000,
        Text_VAlign_Bit        = 0x01000000,
        Delineate_Bit          = 0x02000000,
        User_Fill_Pattern_Bit  = 0x04000000,
        User_Hatch_Pattern_Bit = 0x08000000,
        Contrast_Color_Bit     = 0x10000000,
        Attribute_URL_Bit      = 0x20000000
    };
public:
    /// Constructs a WT_Rendition object.
    WT_Rendition()
        : m_changed_flags(0)
        , m_color_map() // Normally we need to specify which version of the default colormap we want, but we don't yet know what version file we'll be reading.
    { }

    /// Copies a WT_Rendition object from another.
    WT_Rendition (WT_Rendition const & other)
        : m_changed_flags     (other.m_changed_flags)
        , m_rendering_options (other.m_rendering_options)
        , m_drawing_info      (other.m_drawing_info)
        , m_plot_info         (other.m_plot_info)
        , m_url_lookup_list   (other.m_url_lookup_list)
    #define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
        , m_##class_lower (other.m_##class_lower)
        WHIP_ATTRIBUTE_LIST
    #undef WHIP_ATTRIBUTE_MACRO
    {
        ;
    }

	/// Destroys a WT_Rendition object.
    virtual ~WT_Rendition()
    { }

    /// Assigns the contents of a WT_Rendition from another.
    WT_Rendition & operator= (WT_Rendition const & other)
    {
        m_changed_flags     = other.m_changed_flags; //
        m_rendering_options = other.m_rendering_options;
        m_drawing_info      = other.m_drawing_info;
        m_plot_info         = other.m_plot_info;
        m_url_lookup_list   = other.m_url_lookup_list;

        #define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
            m_##class_lower = other.m_##class_lower;
            WHIP_ATTRIBUTE_LIST
        #undef WHIP_ATTRIBUTE_MACRO

        return *this;
    }

    

    //@{ \name Sub object access methods
    /// Returns a writable reference to the local WT_Drawing_Info object for this rendition.
    virtual WT_Drawing_Info & drawing_info()
    {   return m_drawing_info; }

    /// Returns a writable reference to the local WT_Rendering_Options object for this rendition.
    virtual WT_Rendering_Options & rendering_options()
    {   return m_rendering_options; }

    /// Returns a writable reference to the local WT_Plot_Info object for this rendition.
    virtual WT_Plot_Info & plot_info()
    {   return m_plot_info; }

    /// Returns a writable reference to the local WT_URL_List object for this rendition.
    virtual WT_URL_List  & url_lookup_list()
    {   return m_url_lookup_list; }
    //@}

    /** \name Rendition attribute read-only accessor methods
     *  The following methods provide read-only access to the rendition attributes.
     *  \sa \ref groupRenditionAttributes "Rendition attributes"
     *  @{
     */
    #define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
        virtual WT_##class_name const & class_lower()    const { return m_##class_lower; }
        WHIP_ATTRIBUTE_LIST
    #undef WHIP_ATTRIBUTE_MACRO
    //@}

    /** \name Rendition attribute accessor methods
     *  The following methods provide writable access to the rendition attributes.
     *  \sa \ref groupRenditionAttributes "Rendition attributes"
     *  @{
     */
    #define WHIP_ATTRIBUTE_MACRO(class_name, class_lower) \
        virtual WT_##class_name & class_lower() { m_changed_flags |= class_name##_Bit; return m_##class_lower; }
        WHIP_ATTRIBUTE_LIST
    #undef WHIP_ATTRIBUTE_MACRO
    //@}

    /// Serializes pending changed rendition settings to the file, if necessary.
    /** Only outputs relevant desired rendition changes to the file.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result sync(WT_File & file, WT_Integer32 required = 0)
    {
        WT_Integer32   needed = required & m_changed_flags;
        WD_CHECK(m_rendering_options.sync(file));
        if (needed)
        {
            m_changed_flags &= ~needed;
            return sync_parts(file, needed);
        }
        return WT_Result::Success;
    }

};

//@}

#endif // RENDITION_HEADER
