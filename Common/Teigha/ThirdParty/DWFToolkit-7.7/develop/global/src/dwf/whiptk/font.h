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

#if !defined FONT_HEADER
#define FONT_HEADER

///
///\file        dwf/whiptk/font.h
///

#include "whiptk/font_options.h"

#define WD_FONT_STYLE_BOLD        0x01
#define WD_FONT_STYLE_ITALIC      0x02
#define WD_FONT_STYLE_UNDERLINED  0x04


class WT_File;


/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current text font.
/** \sa WT_Rendition \sa WT_Text \sa \ref groupFontOptions "Font options"
 *
 *
 *  \sa See the embedded fonts example provided in the sample app \em W2DTest accompanying the WHIP!
 *  Toolkit source distribution.
 */
class WHIPTK_API WT_Font : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Text;
	friend class WT_Opcode;
	friend class WT_DWF_Header;

protected:
    WT_Font_Option_Font_Name    m_option_font_name;
    WT_Font_Option_Charset      m_option_charset;
    WT_Font_Option_Pitch        m_option_pitch;
    WT_Font_Option_Family       m_option_family;
    WT_Font_Option_Style        m_option_style;
    WT_Font_Option_Height       m_option_height;
    WT_Font_Option_Rotation     m_option_rotation;
    WT_Font_Option_Width_Scale  m_option_width_scale;
    WT_Font_Option_Spacing      m_option_spacing;
    WT_Font_Option_Oblique      m_option_oblique;
    WT_Font_Option_Flags        m_option_flags;

    WT_Integer32            m_name_length;  // Obsolete, but needed to read older files
    WT_Boolean              m_rotation_checked; // A hack to get landscape_to_portrait() working.

    enum
    {
        Getting_Name_Length,
        Getting_Fields,
        Getting_Name,
        Getting_Charset,
        Getting_Pitch,
        Getting_Family,
        Getting_Style,
        Getting_Height,
        Getting_Rotation,
        Getting_Width_Scale,
        Getting_Spacing,
        Getting_Oblique,
        Getting_Flags,
        Getting_Next_Optioncode,
        Materializing_Option,
        Getting_Bold_Italic,
        Getting_Pitch_and_Family,
        Skipping_Last_Paren,
        Completed
    }               m_stage;

    WT_Font_Optioncode      m_optioncode;

    WT_Unsigned_Integer16   m_fields_defined;

public:

    /// Bit mask describing which font options have been defined by this font.
    enum Font_Options
    {
        FONT_NO_FIELDS          = 0x0000,
        FONT_NAME_BIT           = 0x0001,
        FONT_CHARSET_BIT        = 0x0002,
        FONT_PITCH_BIT          = 0x0004,
        FONT_FAMILY_BIT         = 0x0008,
        FONT_STYLE_BIT          = 0x0010,
        FONT_HEIGHT_BIT         = 0x0020,
        FONT_ROTATION_BIT       = 0x0040,
        FONT_WIDTH_SCALE_BIT    = 0x0080,
        FONT_SPACING_BIT        = 0x0100,
        FONT_OBLIQUE_BIT        = 0x0200,
        FONT_FLAGS_BIT          = 0x0400,
        FONT_ALL_FIELDS         = 0xFFFF
    };
public:
    //@{ \name Construction
    /// Constructs a WT_Font object.
    WT_Font()
        : m_name_length (0) // Obsolete, but needed to read older files
        , m_rotation_checked(WD_False)
        , m_stage (Getting_Name_Length)
        , m_fields_defined(FONT_NO_FIELDS)
    { }

    /// Constructs a WT_Font object from the given data.
    WT_Font (
        WT_String             name, /**< Font name. */
        WT_Boolean            bold, /**< Boldface option. */
        WT_Boolean            italic, /**< Italics option. */
        WT_Boolean            underline, /**< Underline option. */
        WT_Byte               charset, /**< Character set code as defined in WT_Font_Option_Charset::Font_Charset_Code. */
        WT_Byte               pitch, /**< Pitch bitmask as defined in WT_Font_Option_Pitch::Font_Pitch_Flags. */
        WT_Byte               family, /**< Font family as defined in WT_Font_Option_Pitch::Font_Family_Flags. */
        WT_Integer32          height, /**< Font height in drawing units. */
        WT_Unsigned_Integer16 rotation, /**< Font rotation in 360/65636ths of a degree. */
        /** The desired width scaling (multiplied by 1024) to be applied to the font.  A value of 1024 indicates
         *  no scaling, whereas 2048 indicates a font twice as wide as normal, and 512 indicates a font 1/2 as
         *  wide as normal. May range from a value of one (1/1024 of normal) to 65535 (64 times normal). */
        WT_Unsigned_Integer16 width_scale,
        /** The desired inter-character spacing scale (multiplied by 1024) to be applied to the font.  A value
         *  of 1024 indicates normal inter-character spacing, whereas 2048 indicates spacing that is twice as
         *  wide as normal, and 512 indicates spacing that is 1/2 as wide as normal. May range from a value of
         *  one (1/1024 of normal) to 65535 (64 times normal). */
        WT_Unsigned_Integer16 spacing,
        /** The desired rotation of the individual characters within the font in 360/65636ths of a degree.  Note
         *  that \a rotation describes how the entire string is rotated, whereas \a oblique rotates the characters
         *  in place. */
        WT_Unsigned_Integer16 oblique,
        WT_Integer32          flags = 0 /**< \em RESERVED.  The font flags are rarely used. */
        );

	/// Copy Constructor
	WT_Font(WT_Font const& font)
        : m_name_length (0) // Obsolete, but needed to read older files
        , m_rotation_checked(WD_False)
        , m_stage (Getting_Name_Length)
        , m_fields_defined(FONT_NO_FIELDS)
	{
		*this = font;
	}

	virtual ~WT_Font()
	{}
    //@}
public:
    /** \name Font option read-only accessor methods
     *  The following methods provide read-only access to the font options.
     *  @{
     */
    WT_Font_Option_Charset const & charset() const         { return m_option_charset; }
    WT_Font_Option_Family const & family() const           { return m_option_family; }
    WT_Font_Option_Flags const & flags() const             { return m_option_flags; }
    WT_Font_Option_Font_Name const & font_name() const     { return m_option_font_name; }
    WT_Font_Option_Height const & height() const           { return m_option_height; }
    WT_Font_Option_Oblique const & oblique() const         { return m_option_oblique; }
    WT_Font_Option_Pitch const & pitch() const             { return m_option_pitch; }
    WT_Font_Option_Rotation const & rotation() const       { return m_option_rotation; }
    WT_Font_Option_Spacing const & spacing() const         { return m_option_spacing; }
    WT_Font_Option_Style const & style() const             { return m_option_style; }
    WT_Font_Option_Width_Scale const & width_scale() const { return m_option_width_scale;}
    //@}

    /** \name Font option accessor methods
     *  The following methods provide writable access to the font options.
     *  @{
     */
    WT_Font_Option_Charset & charset()         { m_fields_defined |= FONT_CHARSET_BIT; return m_option_charset; }
    WT_Font_Option_Family & family()           { m_fields_defined |= FONT_FAMILY_BIT; return m_option_family; }
    WT_Font_Option_Flags & flags()             { m_fields_defined |= FONT_FLAGS_BIT; return m_option_flags; }
    WT_Font_Option_Font_Name & font_name()     { m_fields_defined |= FONT_NAME_BIT; return m_option_font_name; }
    WT_Font_Option_Height & height()           { m_fields_defined |= FONT_HEIGHT_BIT; return m_option_height; }
    WT_Font_Option_Oblique & oblique()         { m_fields_defined |= FONT_OBLIQUE_BIT; return m_option_oblique; }
    WT_Font_Option_Pitch & pitch()             { m_fields_defined |= FONT_PITCH_BIT; return m_option_pitch; }
    WT_Font_Option_Rotation & rotation()       { m_fields_defined |= FONT_ROTATION_BIT; return m_option_rotation; }
    WT_Font_Option_Spacing & spacing()         { m_fields_defined |= FONT_SPACING_BIT; return m_option_spacing; }
    WT_Font_Option_Style & style()             { m_fields_defined |= FONT_STYLE_BIT; return m_option_style; }
    WT_Font_Option_Width_Scale & width_scale() { m_fields_defined |= FONT_WIDTH_SCALE_BIT; return m_option_width_scale;}
    //@}

    //@{ \name Data access methods
    /// Returns the bitmask indicating which font options are defined (one or more of \ref Font_Options "enum Font_Options".
    WT_Unsigned_Integer16              fields_defined() const                           {   return m_fields_defined;    }
    /// Sets the bitmask indicating which font options are defined (one or more of \ref Font_Options "enum Font_Options".
    void                               set_fields_defined(WT_Unsigned_Integer16 fields) {   m_fields_defined = fields;  }
    /// Assignment operator.  Copies the given font settings to this one.
    WT_Font const & operator=  (WT_Font const & font);
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    WT_Boolean       operator!= (WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Font & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // FONT_HEADER
