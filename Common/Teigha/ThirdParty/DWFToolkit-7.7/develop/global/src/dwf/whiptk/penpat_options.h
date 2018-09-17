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

///
///\file        dwf/whiptk/penpat_options.h
///

#if !defined PENPAT_OPTIONS_HEADER
#define PENPAT_OPTIONS_HEADER

/** \addtogroup groupRenderingAttrobutes
 *  @{
 */

/// Options for use with WT_Pen_Pattern object.
/** \sa WT_Rendering_Options */
class WHIPTK_API WT_PenPat_Options : public WT_Attribute
{
	friend class WT_Rendering_Options;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

protected:
    WT_Boolean               m_scale_pen_width;
    WT_Boolean               m_map_colors_to_gray_scale;
    WT_Boolean               m_use_alternate_fill_rule;
    WT_Boolean               m_use_error_diffusion_for_DWF_Rasters;

    enum
    {
        Getting_Operand,
        Getting_Scale_Pen_Width,
        Getting_Map_Colors_To_Gray_Scale,
        Getting_Use_Alternate_Fill_Rule,
        Getting_Use_Error_Diffusion_For_DWF_Rasters,
        Getting_Close_Paren,
        Completed
    } m_stage;

public:

    //@{ \name Construction
    /// Constructs a WT_PenPat_Options object.
    WT_PenPat_Options()
        : m_stage(Getting_Operand)
    {
        m_scale_pen_width = WD_True;
        m_map_colors_to_gray_scale = WD_True;
        m_use_alternate_fill_rule = WD_True;
        m_use_error_diffusion_for_DWF_Rasters = WD_False;
    }

    /// Constructs a WT_PenPat_Options object with the given data.
    WT_PenPat_Options(
        WT_Boolean const scale_pen_width,
        WT_Boolean const map_colors_to_gray_scale,
        WT_Boolean const use_alternate_fill_rule,
        WT_Boolean const use_error_diffusion_for_DWF_Rasters)
    {
        m_scale_pen_width = scale_pen_width;
        m_map_colors_to_gray_scale = map_colors_to_gray_scale;
        m_use_alternate_fill_rule = use_alternate_fill_rule;
        m_use_error_diffusion_for_DWF_Rasters = use_error_diffusion_for_DWF_Rasters;
    }

    /// Constructs a WT_PenPat_Options object.  Copy constructor.
    WT_PenPat_Options(WT_PenPat_Options const & other)
        : WT_Attribute()
    {
        *this = other;
    }

	virtual ~WT_PenPat_Options()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the state of the \em map_colors_to_gray_scale setting.
    WT_Boolean const get_map_colors_to_gray_scale() const { return m_map_colors_to_gray_scale; }
    /// Returns the state of the \em scale_pen_width setting.
    WT_Boolean const get_scale_pen_width() const { return m_scale_pen_width; }
    /// Returns the state of the \em use_alternate_fill_rule setting.
    WT_Boolean const get_use_alternate_fill_rule() const { return m_use_alternate_fill_rule; }
    /// Returns the state of the \em use_error_diffusion_for_DWF_Rasters setting.
    WT_Boolean const get_use_error_diffusion_for_DWF_Rasters() const { return m_use_error_diffusion_for_DWF_Rasters; }
    /// Sets the \em map_colors_to_gray_scale setting.
    /** If true, this will cause colors to be rendered as gray scale, using the configured screening patterns. */
    void             set_map_colors_to_gray_scale(WT_Boolean map_colors_to_gray_scale) { m_map_colors_to_gray_scale = map_colors_to_gray_scale; }
    /// Sets the \em scale_pen_width setting.
    /** If true, and if the WHIP! data is plotted on a paper size other than that for which it was originally
     *  created for, then pen widths will be enlarged or reduced in proportion to the two paper sizes. */
    void             set_scale_pen_width(WT_Boolean scale_pen_width) { m_scale_pen_width = scale_pen_width; }
    /// Sets the \em use_alternate_fill_rule setting.
    void             set_use_alternate_fill_rule(WT_Boolean use_alternate_fill_rule) { m_use_alternate_fill_rule = use_alternate_fill_rule; }
    /// Sets the \em use_error_diffusion_for_DWF_Rasters setting.
    /** If true, error diffusion will be used instead of ordered dithering (if dithering is necessary to
     *  render any rasters embedded in the WHIP! data.) */
    void             use_error_diffusion_for_DWF_Rasters(WT_Boolean use_error_diffusion_for_DWF_Rasters) { m_use_error_diffusion_for_DWF_Rasters = use_error_diffusion_for_DWF_Rasters; }
    /// Assignment operator.  Assigns the given options object's contents to this one.
    virtual WT_PenPat_Options const &   operator=(WT_PenPat_Options const & options);
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_PenPat_Options & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );


};

//@}

#endif // PENPAT_OPTIONS_HEADER
