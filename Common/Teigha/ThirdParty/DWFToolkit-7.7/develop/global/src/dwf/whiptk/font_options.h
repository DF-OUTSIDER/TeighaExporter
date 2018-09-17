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

#if !defined FONT_OPTIONS_HEADER
#define FONT_OPTIONS_HEADER

///
///\file        dwf/whiptk/font_options.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"
#include "whiptk/opcode.h"

static const int DEFAULT_CHAR_SPACING	=	1024;
static const int DEFAULT_WIDTH_SCALE	=	1024;

/** \addtogroup groupFontOptions Font options
 *  The font option objects assist the reader application in the font matching
 *  process to try to find a font that closely matches what the authoring application
 *  intended.
 *  \sa WT_Font.
 *  @{
 */


// ====================================================================================
/// Font option describing the font name.
class WHIPTK_API WT_Font_Option_Font_Name : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_String     m_font_name;

public:

    //@{ \name Construction
    /// Constructs a WT_Font_Option_Font_Name object.
    WT_Font_Option_Font_Name()
        : m_font_name ("Arial") // Shouldn't use hard coded constants like this.
    { }

    /// Constructs a WT_Font_Option_Font_Name object with the given name.
    WT_Font_Option_Font_Name(WT_String const &name)
        : m_font_name (name)
    { }
	virtual ~WT_Font_Option_Font_Name()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the font name.
    virtual WT_String const &   name() const                                            {   return m_font_name;   }
    /// Sets the font name.
    virtual void                set(WT_String const & name)                             {   m_font_name = name;   }
    /// Returns WD_True if the given font name is equal to this one.
    virtual WT_Boolean          operator==(WT_Font_Option_Font_Name const & nm) const   {   return (m_font_name == nm.m_font_name); }
    /// Returns WD_True if the given font name is NOT equal to this one.
    virtual WT_Boolean          operator!=(WT_Font_Option_Font_Name const & nm) const   {   return (m_font_name != nm.m_font_name); }
    /// Returns the font name.
    virtual operator WT_String const & () const                     {   return m_font_name;     }
    //@}

    //@{ \name WT_Option virtual methods
    virtual WT_ID            object_id() const;
    virtual WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    virtual WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

};

// ====================================================================================
/// Font option describing the font character set.
/** The charset option gives the file reading application a hint as to what type of
 *  characters will be needed by subsequent text rendering operations (which are
 *  normally specified in Unicode).  On systems with a wide array of Unicode based
 *  fonts, this value may be ignored. On systems with few Unicode fonts, the font
 *  matching process should give a high priority to finding a font that supports the
 *  selected character set.
 */
class WHIPTK_API WT_Font_Option_Charset : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Byte     m_charset;

public:

    /// Commmon character set values used by font matching algorithms.
    enum Font_Charset_Code
    {
         CHARSET_ANSI           = 0x00  // 0
        ,CHARSET_DEFAULT        = 0x01  // 1
        ,CHARSET_SYMBOL         = 0x02  // 2
        ,CHARSET_MAC            = 0x4D  // 77
        ,CHARSET_UNICODE        = 0x57  // 87
        ,CHARSET_SHIFTJIS       = 0X80  // 128
        ,CHARSET_HANGEUL        = 0X81  // 129
        ,CHARSET_JOHAB          = 0X82  // 130
        ,CHARSET_GB2312         = 0X86  // 134
        ,CHARSET_CHINESEBIG5    = 0X88  // 136
        ,CHARSET_GREEK          = 0XA1  // 161
        ,CHARSET_TURKISH        = 0XA2  // 162
        ,CHARSET_VIETNAMESE     = 0XA3  // 163
        ,CHARSET_HEBREW         = 0XB1  // 177
        ,CHARSET_ARABIC         = 0XB2  // 178
        ,CHARSET_BALTIC         = 0XBA  // 186
        ,CHARSET_RUSSIAN        = 0XCC  // 204
        ,CHARSET_THAI           = 0XDE  // 222
        ,CHARSET_EASTEUROPE     = 0XEE  // 238
        ,CHARSET_OEM            = 0XFF  // 255
    };
public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Charset object.
    WT_Font_Option_Charset()
        : m_charset (CHARSET_DEFAULT)
    { }

    /// Constructs a WT_Font_Option_Charset object with the given character set code.
    WT_Font_Option_Charset(WT_Byte charset)
        : m_charset (charset)
    { }
	virtual ~WT_Font_Option_Charset()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the defined character set code (typically, but not necessarily one of \ref Font_Charset_Code "enum Font_Charset_Code".)
    WT_Byte     charset() const                                     {   return m_charset;   }
    /// Sets the character set code (typically, but not necessarily one of \ref Font_Charset_Code "enum Font_Charset_Code".)
    void        set(WT_Byte cs)                                     {   m_charset = cs;     }
    /// Returns WD_True if the given font character set option is equal to this one.
    WT_Boolean  operator==(WT_Font_Option_Charset const & cs) const {   return (m_charset == cs.m_charset); }
    /// Returns WD_True if the given font character set option is NOT equal to this one.
    WT_Boolean  operator!=(WT_Font_Option_Charset const & cs) const {   return (m_charset != cs.m_charset); }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}
};

// ====================================================================================
/// Font option describing the font pitch.
/** Describes how the characters of a font are positioned in a string. This option is
 *  used in the font matching process to resolve what the writing application intended
 *  against what the reading application has available.
 */
class WHIPTK_API WT_Font_Option_Pitch : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Byte            m_pitch;

public:

    /// Describes the character width style.
    enum Font_Pitch_Flags
    {
     PITCH_DEFAULT     = 0x00 /**< Font pitch is unspecified. */
    ,PITCH_FIXED       = 0x01 /**< Matching fonts should have characters that are all of the same width. */
    ,PITCH_VARIABLE    = 0x02 /**< Matching fonts should have characters that are proportionally spaced (variable width characters.) */
    ,PITCH_BITS        = ( PITCH_DEFAULT | PITCH_FIXED | PITCH_VARIABLE )
    };
public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Pitch object.
    WT_Font_Option_Pitch()
        : m_pitch (PITCH_VARIABLE)
    { }

    /// Constructs a WT_Font_Option_Pitch object with the given pitch value.
    WT_Font_Option_Pitch(
        WT_Byte pitch /**< Pitch value (one of PITCH_DEFAULT, PITCH_FIXED, or PITCH_VARIABLE.) */
        )
        : m_pitch (pitch)
    { }
	virtual ~WT_Font_Option_Pitch()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the pitch value (one of PITCH_DEFAULT, PITCH_FIXED, or PITCH_VARIABLE.)
    WT_Byte     pitch() const                                       {   return m_pitch;                     }
    /// Sets the pitch value (one of PITCH_DEFAULT, PITCH_FIXED, or PITCH_VARIABLE.)
    void        set(WT_Byte pitch)                                  {   m_pitch = pitch;                    }
    /// Returns WD_True if the given font pitch option is equal to this one.
    WT_Boolean  operator==(WT_Font_Option_Pitch const & pt) const   {   return (m_pitch == pt.m_pitch);     }
    /// Returns WD_True if the given font pitch option is NOT equal to this one.
    WT_Boolean  operator!=(WT_Font_Option_Pitch const & pt) const   {   return (m_pitch != pt.m_pitch);     }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}
};


// ====================================================================================
/// Font option describing the font family.
class WHIPTK_API WT_Font_Option_Family : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Byte            m_family;

public:

    /// Describes the overall style of a font.
    enum Font_Family_Flags
    {
         FAMILY_UNKNOWN    = 0x00
        ,FAMILY_ROMAN      = 0x10
        ,FAMILY_SWISS      = 0x20
        ,FAMILY_MODERN     = 0x30
        ,FAMILY_SCRIPT     = 0x40
        ,FAMILY_DECORATIVE = 0x50
        ,FAMILY_BITS       = (FAMILY_UNKNOWN | FAMILY_ROMAN | FAMILY_SWISS | FAMILY_MODERN | FAMILY_SCRIPT | FAMILY_DECORATIVE)
    };
public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Family object.
    WT_Font_Option_Family()
        : m_family (FAMILY_ROMAN)
    { }

    /// Constructs a WT_Font_Option_Family object with the given font family value.
    WT_Font_Option_Family(
        WT_Byte family /**< One of one of FAMILY_UNKNOWN, FAMILY_ROMAN, FAMILY_SWISS, FAMILY_MODERN, FAMILY_SCRIPT or FAMILY_DECORATIVE. */
        )
        : m_family(family)
    { }
	virtual ~WT_Font_Option_Family()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the font family value.
    WT_Byte     family() const                                      {   return m_family;                    }
    /// Sets the font family value.
    void        set(
        WT_Byte family /**< One of one of FAMILY_UNKNOWN, FAMILY_ROMAN, FAMILY_SWISS, FAMILY_MODERN, FAMILY_SCRIPT or FAMILY_DECORATIVE. */
        )                                 {   m_family = family;                  }
    /// Returns WD_True if the given font family option is equal to this one.
    WT_Boolean  operator==(WT_Font_Option_Family const & fm) const  {   return (m_family == fm.m_family);   }
    /// Returns WD_True if the given font family option is NOT equal to this one.
    WT_Boolean  operator!=(WT_Font_Option_Family const & fm) const  {   return (m_family != fm.m_family);   }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}
};


// ====================================================================================
/// Font option describing the boldface, italics, and underline settings.
class WHIPTK_API WT_Font_Option_Style : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Boolean          m_bold;
    WT_Boolean          m_italic;
    WT_Boolean          m_underlined;

    enum
    {
        Starting = 1000,
        Getting_Next_Optioncode = 1001,
        Skipping_Optioncode = 1002
    };

    WT_Opcode       m_optioncode;

public:

    //@{ \name Construction
    /// Constructs a WT_Font_Option_Family object with the given settings (can be used as a default constructor.)
    WT_Font_Option_Style(
        WT_Boolean bold = WD_False,
        WT_Boolean italic = WD_False,
        WT_Boolean underlined = WD_False
        )
        : m_bold(bold)
        , m_italic(italic)
        , m_underlined(underlined)
    {
        m_stage = Starting;
    }
	virtual ~WT_Font_Option_Style()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the boldface setting.
    WT_Boolean      bold() const                                         {   return m_bold;       }
    /// Returns the italics setting.
    WT_Boolean      italic() const                                       {   return m_italic;     }
    /// Returns the underline setting.
    WT_Boolean      underlined() const                                   {   return m_underlined; }
    /// Sets the boldface setting.
    void            set_bold(WT_Boolean bold)                            {   m_bold = bold;       }
    /// Sets the italics setting.
    void            set_italic(WT_Boolean italic)                        {   m_italic = italic;   }
    /// Sets the underline setting.
    void            set_underlined(WT_Boolean underlined)                {   m_underlined = underlined;  }
    /// Returns WD_True if the given font style option is equal to this one.
    WT_Boolean      operator==(WT_Font_Option_Style  const & in) const
    {
        return (m_bold       == in.m_bold
             && m_italic     == in.m_italic
             && m_underlined == in.m_underlined);
    }
    /// Returns WD_True if the given font style option is NOT equal to this one.
    WT_Boolean      operator!=(WT_Font_Option_Style  const & in) const
    {
        return (m_bold      != in.m_bold
             || m_italic    != in.m_italic
             || m_underlined != in.m_underlined);
    }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}
};


// ====================================================================================
/// Font option describing the font height in drawing units.
class WHIPTK_API WT_Font_Option_Height : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Integer32        m_height;

public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Height object.
    WT_Font_Option_Height()
        : m_height (1000) // Pull a rabbit out of a hat! Shouldn't hard code data like this.
    { }
    /// Constructs a WT_Font_Option_Height object with the given height (in drawing units.)
    WT_Font_Option_Height(WT_Integer32 height)
        : m_height(height)
    { }
	virtual ~WT_Font_Option_Height()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the font height (in drawing units.)
    WT_Integer32    height() const                                      {   return m_height;     }
    /// Sets the font height (in drawing units.)
    void            set(WT_Integer32 height)                            {   m_height = height;   }
    /// Returns WD_True if the given font height option is equal to this one.
    WT_Boolean      operator==(WT_Font_Option_Height const & in) const  {   return (m_height == in.m_height);   }
    /// Returns WD_True if the given font height option is NOT equal to this one.
    WT_Boolean      operator!=(WT_Font_Option_Height const & in) const  {   return (m_height != in.m_height);   }
    /// Returns the font height.
    operator WT_Integer32() const                       {   return m_height;     }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}
};


// ====================================================================================
/// Font option describing the font rotation.
class WHIPTK_API WT_Font_Option_Rotation : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Unsigned_Integer16        m_rotation;

public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Rotation object.
    WT_Font_Option_Rotation()
        : m_rotation (0)
    { }
    /// Constructs a WT_Font_Option_Rotation object with the given rotation angle.
    WT_Font_Option_Rotation(
        WT_Unsigned_Integer16 rotation /**< Font rotation in 360/65636ths of a degree. */
        )
        : m_rotation(rotation)
    { }
	virtual ~WT_Font_Option_Rotation()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the font rotation in 360/65636ths of a degree.
    WT_Unsigned_Integer16   rotation() const                                        {   return m_rotation;     }
    /// Sets the font rotation in 360/65636ths of a degree.
    void                    set(WT_Unsigned_Integer16 rotation)                     {   m_rotation = rotation; }
    /// Returns WD_True if the given font rotation option is equal to this one.
    WT_Boolean              operator==(WT_Font_Option_Rotation const & in) const    {   return (m_rotation == in.m_rotation);   }
    /// Returns WD_True if the given font rotation option is NOT equal to this one.
    WT_Boolean              operator!=(WT_Font_Option_Rotation const & in) const    {   return (m_rotation != in.m_rotation);   }
    /// Returns the font rotation in 360/65636ths of a degree.
    operator WT_Unsigned_Integer16() const                  {   return m_rotation;     }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

};


// ====================================================================================
/// Font option describing the width scale.
/** This is the desired width scaling (multiplied by 1024) to be applied to the font.
 *  A value of 1024 indicates no scaling, whereas 2048 indicates a font twice as wide
 *  as normal, and 512 indicates a font 1/2 as wide as normal. May range from a value
 *  of one (1/1024 of normal) to 65535 (64 times normal).
 */
class WHIPTK_API WT_Font_Option_Width_Scale : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Unsigned_Integer16        m_width_scale;

public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Width_Scale object.
    WT_Font_Option_Width_Scale()
        : m_width_scale (DEFAULT_WIDTH_SCALE)
    { }
    /// Constructs a WT_Font_Option_Width_Scale object with the given width scale.
    WT_Font_Option_Width_Scale( WT_Unsigned_Integer16 width_scale)
        : m_width_scale(width_scale)
    { }
	virtual ~WT_Font_Option_Width_Scale()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the width scale.
    WT_Unsigned_Integer16   width_scale() const                     {   return m_width_scale;        }
    /// Sets the width scale.
    void                    set(WT_Unsigned_Integer16 width_scale)  {   m_width_scale = width_scale; }
    /// Returns WD_True if the given width scale option is equal to this one.
    WT_Boolean              operator==(WT_Font_Option_Width_Scale const & in) const  {   return (m_width_scale == in.m_width_scale);   }
    /// Returns WD_True if the given width scale option is NOT equal to this one.
    WT_Boolean              operator!=(WT_Font_Option_Width_Scale const & in) const  {   return (m_width_scale != in.m_width_scale);   }
    /// Returns the width scale.
    operator WT_Unsigned_Integer16() const                   {   return m_width_scale;     }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

};


// ====================================================================================
/// Font option describing the inter-character spacing.
/** This is the desired inter-character spacing scale (multiplied by 1024) to be applied
 *  to the font.  A value of 1024 indicates normal inter-character spacing, whereas 2048
 *  indicates spacing that is twice as wide as normal, and 512 indicates spacing that is
 *  1/2 as wide as normal. May range from a value of one (1/1024 of normal) to 65535
 *  (64 times normal).
 */
class WHIPTK_API WT_Font_Option_Spacing : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Unsigned_Integer16        m_spacing;

public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Spacing object.
    WT_Font_Option_Spacing()
		: m_spacing (DEFAULT_CHAR_SPACING)
    { }
    /// Constructs a WT_Font_Option_Spacing object with the given spacing.
    WT_Font_Option_Spacing(WT_Unsigned_Integer16 spacing)
        : m_spacing(spacing)
    { }
	virtual ~WT_Font_Option_Spacing()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the inter-character spacing.
    WT_Unsigned_Integer16   spacing() const                                      {   return m_spacing;        }
    /// Sets the inter-character spacing.
    void                    set(WT_Unsigned_Integer16 spacing)                   {   m_spacing = spacing;     }
    /// Returns WD_True if the given spacing option is equal to this one.
    WT_Boolean              operator==(WT_Font_Option_Spacing const & in) const  {   return (m_spacing == in.m_spacing);   }
    /// Returns WD_True if the given spacing option is NOT equal to this one.
    WT_Boolean              operator!=(WT_Font_Option_Spacing const & in) const  {   return (m_spacing != in.m_spacing);   }
    /// Returns the inter-character spacing.
    operator WT_Unsigned_Integer16() const               {   return m_spacing;     }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

};

// ====================================================================================
/// Font option describing the obliquing angle to be applied to characters in a string displayed with the parent font.
class WHIPTK_API WT_Font_Option_Oblique : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Unsigned_Integer16        m_oblique;

public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Oblique object.
    WT_Font_Option_Oblique()
        : m_oblique (0)
    { }
    /// Constructs a WT_Font_Option_Oblique object with the given oblique angle.
    WT_Font_Option_Oblique(
        WT_Unsigned_Integer16 oblique /**< Character rotation in 360/65636ths of a degree. */
        )
        : m_oblique(oblique)
    { }
	virtual ~WT_Font_Option_Oblique()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the oblique angle in 360/65636ths of a degree.
    WT_Unsigned_Integer16   oblique() const                                      {   return m_oblique;        }
    /// Sets the oblique angle in 360/65636ths of a degree.
    void                    set(WT_Unsigned_Integer16 oblique)                   {   m_oblique = oblique;     }
    /// Returns WD_True if the given oblique option is equal to this one.
    WT_Boolean              operator==(WT_Font_Option_Oblique const & in) const  {   return (m_oblique == in.m_oblique);   }
    /// Returns WD_True if the given oblique option is NOT equal to this one.
    WT_Boolean              operator!=(WT_Font_Option_Oblique const & in) const  {   return (m_oblique != in.m_oblique);   }
    /// Returns the oblique angle in 360/65636ths of a degree.
    operator WT_Unsigned_Integer16() const               {   return m_oblique;     }

    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

};


// ====================================================================================
/// Font option representing a set of option flags.  \em RESERVED.
class WHIPTK_API WT_Font_Option_Flags : public WT_Option
{
	friend class WT_Font;
	friend class WT_W2D_Class_Factory;

private:
    WT_Integer32        m_flags;

public:
    //@{ \name Construction
    /// Constructs a WT_Font_Option_Flags object.
    WT_Font_Option_Flags()
        : m_flags (0)
    { }
    /// Constructs a WT_Font_Option_Flags object with the given flags.
    WT_Font_Option_Flags(WT_Integer32 flags)
        : m_flags(flags)
    { }
	virtual ~WT_Font_Option_Flags()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the flags.
    WT_Integer32    flags() const                                       {   return m_flags;     }
    /// Sets the flags.
    void            set(WT_Integer32 flags)                             {   m_flags = flags;    }
    /// Returns WD_True if the given flags option is equal to this one.
    WT_Boolean      operator==(WT_Font_Option_Flags const & in) const   {   return (m_flags == in.m_flags);   }
    /// Returns WD_True if the given flags option is NOT equal to this one.
    WT_Boolean      operator!=(WT_Font_Option_Flags const & in) const   {   return (m_flags != in.m_flags);   }
    /// Returns the flags.
    operator WT_Integer32() const                       {   return m_flags;     }
    //@}

    //@{ \name WT_Option virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize  (WT_Object const & parent, WT_File & file) const;
    WT_Result        materialize(WT_Object       & parent, WT_Optioncode const & optioncode, WT_File & file);
    //@}

};


// ====================================================================================

/// Provides an ID for font option objects.
class WHIPTK_API WT_Font_Optioncode : public WT_Optioncode
{
public:

    /// Font option identifiers.
    enum
    {
        Unknown_Option  = 0,
        Font_Name_Option,
        Charset_Option,
        Pitch_Option,
        Family_Option,
        Style_Option,
        Height_Option,
        Rotation_Option,
        Width_Scale_Option,
        Spacing_Option,
        Oblique_Option,
        Flags_Option
    };

    /// Returns the option ID of the font option.
    virtual int option_id_from_optioncode();
};

//@}

#endif // FONT_OPTIONS_HEADER
