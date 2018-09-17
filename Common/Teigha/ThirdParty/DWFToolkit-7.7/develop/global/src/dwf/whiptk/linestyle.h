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

#if !defined LINESTYLE_HEADER
#define LINESTYLE_HEADER

///
///\file        dwf/whiptk/linestyle.h
///

#include "whiptk/pattern_scale.h"


#define WHIP_LINE_STYLE_LIST\
    WHIP_LINE_STYLE_OPTION(Adapt_Patterns,adapt_patterns,ADAPT_PATTERNS,WT_Boolean,WD_False,0x0001, A WT_Line_Style option which specifies that a rendering application should adjust a line pattern length so that each line contains one or more complete pattern., The adaptive pattern setting also applies to user-defined dash patterns. \sa WT_Line_Style \sa WT_Dash_Pattern \note Do not use an adaptive line type when drawing circles; arcs or the edges of wedges or polygons. The rendering application will likely attempt to draw the complete pattern in every chord as there are typically 72 chords in a circle using the default chord angle.)\
    WHIP_LINE_STYLE_OPTION(Dash_End_Cap,dash_end_cap,DASH_END_CAP,WT_Capstyle_ID,Butt_Cap,0x0002, A WT_Line_Style option which describes the type of line caps for the ending points of interior pattern segments., This attribute applies to lines and ellipses / arcs. \sa WT_Line_Style \sa WT_Line_Style::WT_Line_End_Cap \image html StartAndEndCaps.png "Line and dash start and end caps details")\
    WHIP_LINE_STYLE_OPTION(Dash_Start_Cap,dash_start_cap,DASH_START_CAP,WT_Capstyle_ID,Butt_Cap,0x0004, A WT_Line_Style option which describes the type of line caps for the starting points of interior pattern segments., This attribute applies to lines and ellipses / arcs. \sa WT_Line_Style \sa WT_Line_Style::WT_Line_Start_Cap \image html StartAndEndCaps.png "Line and dash start and end caps details")\
    WHIP_LINE_STYLE_OPTION(Line_End_Cap,line_end_cap,LINE_END_CAP,WT_Capstyle_ID,Butt_Cap,0x0008, A WT_Line_Style option which describes the type of line caps for the ending points of lines., This attribute applies lines and ellipses / arcs.  \sa WT_Line_Style \sa WT_Line_Style::WT_Dash_End_Cap \image html CapsAndJoins.png "Line style caps and joins details")\
    WHIP_LINE_STYLE_OPTION(Line_Join,line_join,LINE_JOIN,WT_Joinstyle_ID,Miter_Join,0x0010, A WT_Line_Style option which describes how the joints of subsequent thickened multi-segment polylines are rendered., \sa WT_Line_Style \image html CapsAndJoins.png "Line style caps and joins details")\
    WHIP_LINE_STYLE_OPTION(Line_Start_Cap,line_start_cap,LINE_START_CAP,WT_Capstyle_ID,Butt_Cap,0x0020, A WT_Line_Style option which describes the type of line caps for the starting points of lines., The attribute applies to lines and ellipses / arcs. \sa WT_Line_Style \sa WT_Line_Style::WT_Dash_Start_Cap \image html CapsAndJoins.png "Line style caps and joins details")\
    WHIP_LINE_STYLE_OPTION(Miter_Angle,miter_angle,MITER_ANGLE,WT_Unsigned_Integer16,10,0x0040, A WT_Line_Style option which describes an inside angle formed by extending two intersecting line segments past the join., \sa WT_Line_Style \sa WT_Line_Style::WT_Line_Join )\
    WHIP_LINE_STYLE_OPTION(Miter_Length,miter_length,MITER_LENGTH,WT_Unsigned_Integer16,6,0x0080, A WT_Line_Style option which describes a distance from the intersection of two line segments to the furtherest point of the boundary formed by the extended edges of these line segments., \sa WT_Line_Style \sa WT_Line_Style::WT_Line_Join )

/** \addtogroup groupLineStyleOptions Line style options
*  The line style option objects assist the reader application in rendering thick line
*  and dash end and start caps, as well as line join styles.
*  \sa WT_Line_Style.
*
*  @{
*/

//@}


#define WHIP_LINE_STYLE_CLASS( class_type, class_lower, class_upper, data_type, default_value, hex_bit, brief_description, full_description ) \
    /** \brief brief_description */ \
    /** full_description \ingroup groupLineStyleOptions */ \
    class WHIPTK_API WT_##class_type : public WT_Option \
    {\
    public:\
        friend class WT_Line_Style; \
        /** \brief Constructs a WT_##class_type object */ WT_##class_type() \
        : m_stage( Eating_Initial_Whitespace ) \
        , m_value( ( data_type ) default_value ) {} \
        /** \brief Type cast operator, returns the data_type style option value. */ inline operator data_type () const { return m_value; } \
        /** \brief Assignment operator, assigns the value to this style option. */ inline WT_##class_type & operator= ( data_type value ) { m_value = value; return *this; } \
        /** \brief Returns WD_True if the given option is equal to this one. */ inline WT_Boolean operator== ( WT_##class_type const & option ) const { return m_value == option.m_value; } \
        /** \brief Returns WD_True if the given option is NOT equal to this one. */ inline WT_Boolean operator!= ( WT_##class_type const & option ) const { return m_value != option.m_value; } \
        /** \brief Returns WD_True if the given option value is equal to this one. */ inline WT_Boolean operator== ( data_type const & value ) const { return m_value == value; } \
        /** \brief Returns WD_True if the given option value is NOT equal to this one. */ inline WT_Boolean operator!= ( data_type const & value ) const { return m_value != value; } \
        WT_Result serialize( WT_Object const & parent, WT_File& file) const; \
        WT_Result materialize( WT_Object& parent, WT_Optioncode const & optioncode, WT_File & file); \
    private: \
        WT_ID     object_id() const { return Option_ID; } \
        enum WT_Materialize_Stage \
        {   Eating_Initial_Whitespace, \
            Getting_Value, \
            Eating_End_Whitespace \
        } m_stage; \
        data_type m_value; \
    };

/// Describes the attributes which affect rendered lines, polylines, and arcs.
/** \ingroup groupRenditionAttributes
 *  \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Line_Style
: public WT_Attribute
{
    friend class WT_Pattern_Scale;
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;


    #define WHIP_LINE_STYLE_OPTION( class_type, __A, __B, __C, __D, __E, __F, __G) \
        friend class WT_##class_type;
        WHIP_LINE_STYLE_LIST
    #undef WHIP_LINE_STYLE_OPTION
public:

    /// An enumeration of cap styles used by WT_Line_End_Cap, WT_Line_Start_Cap, WT_Dash_End_Cap, and WT_Dash_Start_Cap.
    enum WT_Capstyle_ID
    {
        Butt_Cap,
        Square_Cap,
        Round_Cap,
        Diamond_Cap,
        // Not Styles:
        Capstyle_Count,
        Undefined_Capstyle = -1
    };

    /// An enumeration of styles used when joining two thick line segments.
    enum WT_Joinstyle_ID
    {
        Miter_Join,
        Bevel_Join,
        Round_Join,
        Diamond_Join,
        // Not Styles:
        Joinstyle_Count,
        Undefined_Joinstyle = -1
    };

    #define WHIP_LINE_STYLE_OPTION( class_type, class_lower, class_upper, data_type, default_value, hex_bit, brief_description, full_description ) \
        WHIP_LINE_STYLE_CLASS( class_type, class_lower, class_upper, data_type, default_value, hex_bit, brief_description, full_description )
    // Define the option classes
    WHIP_LINE_STYLE_LIST

    /// Provides an ID for line style option objects.
    /** \ingroup groupLineStyleOptions */
    class WHIPTK_API WT_Line_Style_Option_Code
    : public WT_Optioncode
    {
    public:
        /// Line style option identifiers.
        enum WT_Line_Style_Option_ID
        {   Unknown_Option,
            Adapt_Patterns_Option,
            Pattern_Scale_Option,
            Line_Join_Option,
            Dash_Start_Cap_Option,
            Dash_End_Cap_Option,
            Line_Start_Cap_Option,
            Line_End_Cap_Option,
            Miter_Angle_Option,
            Miter_Length_Option
        };
        /// Returns the option ID of the line style option.
        int option_id_from_optioncode();
    };
    #undef WHIP_LINE_STYLE_OPTION
public:
    //@{ \name Construction
    /// Constructs a WT_Line_Style object.
    WT_Line_Style()
    : m_stage(Eating_Initial_Whitespace)
    , m_fields_defined(0)
    {}
    
    WT_Line_Style( WT_Line_Style const & copy )
    {
        operator=( copy );
    }

	virtual ~WT_Line_Style()
	{}
    //@}
public:
    /// Copies the given style's option settings to this one.
    virtual WT_Line_Style &operator= ( WT_Line_Style const &copy )
    {
        m_stage = copy.m_stage;
        m_fields_defined = copy.m_fields_defined;
        m_pattern_scale = copy.m_pattern_scale;
        #define WHIP_LINE_STYLE_OPTION( class_type, class_lower, class_upper, __A, __B, __C, __D, __E ) \
            m_##class_lower = copy.m_##class_lower;
            WHIP_LINE_STYLE_LIST
        #undef WHIP_LINE_STYLE_OPTION
        return *this;
    }
    /// Merges the given style's option settings to this one.
    void merge( WT_Line_Style const &style );
    //@{ \name Returns read-only references to the line style options.
    #define WHIP_LINE_STYLE_OPTION( class_type, class_lower, __A, __B, __C, __D, __E, __F) \
        WT_##class_type const & class_lower() const {  return m_##class_lower; }
        WHIP_LINE_STYLE_LIST
    #undef WHIP_LINE_STYLE_OPTION
    WT_Pattern_Scale const & pattern_scale() const {  return m_pattern_scale; }
    //@}

    //@{ \name Returns writable references to the line style options.
    #define WHIP_LINE_STYLE_OPTION( class_type, class_lower, class_upper, __A, __B, __C, __D, __E ) \
        WT_##class_type & class_lower() {  m_fields_defined |= class_upper##_BIT; return m_##class_lower; }
        WHIP_LINE_STYLE_LIST
    #undef WHIP_LINE_STYLE_OPTION
    WT_Pattern_Scale& pattern_scale() {  m_fields_defined |= PATTERN_SCALE_BIT;  return m_pattern_scale; }
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const { return Line_Style_ID; }
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
        WT_Line_Style & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

    #ifndef DOXYGEN_IGNORE
    static WT_Result interpret_joinstyle( const char *, WT_Joinstyle_ID& );
    static WT_Result interpret_capstyle( const char *, WT_Capstyle_ID& );
    static WT_Result serialize_capstyle_ascii( WT_Capstyle_ID, WT_File& );

    /// Bit settings to identify which line style options are defined.
    enum
    {
        #define WHIP_LINE_STYLE_OPTION( __A, __B, class_upper, __C, __D, hex_bit, __E, __F ) \
            class_upper##_BIT = hex_bit,
            WHIP_LINE_STYLE_LIST
        #undef WHIP_LINE_STYLE_OPTION
        PATTERN_SCALE_BIT = 0x0100
    };
    #endif

protected:

    WT_Line_Style_Option_Code m_optioncode;
    WT_Pattern_Scale m_pattern_scale;
    #define WHIP_LINE_STYLE_OPTION( class_type, class_lower, __A, __B, __C, __D, __E, __F) \
        protected: WT_##class_type m_##class_lower;
        WHIP_LINE_STYLE_LIST
    #undef WHIP_LINE_STYLE_OPTION


    WT_Result materialize_ascii( WT_Opcode const&, WT_File& );

    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Getting_Pattern_Number,
        Checking_For_Attribute_End,
        Getting_Optioncode,
        Materializing_Option,
        // extended binary :
        Getting_Adapt_Patterns,
        Getting_Pattern_Scale,
        Getting_Line_Join,
        Getting_Dash_Start_Cap,
        Getting_Dash_End_Cap,
        Getting_Line_Start_Cap,
        Getting_Line_End_Cap,
        Getting_Miter_Length,
        Getting_Miter_Angle,
        Eating_End_Whitespace
    } m_stage;

public:
    WT_Unsigned_Integer16 m_fields_defined;
};

#endif // LINESTYLE_HEADER
