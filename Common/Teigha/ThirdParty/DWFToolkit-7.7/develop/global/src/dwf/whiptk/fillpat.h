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

#if !defined FILLPAT_HEADER
#define FILLPAT_HEADER

///
///\file        dwf/whiptk/fillpat.h
///

#include "whiptk/pattern_scale.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current fill pattern.
/** \sa WT_Rendition
 *  \image html FillPatterns.png "Suggested rendering of fill patterns"
 *
 */
class WHIPTK_API WT_Fill_Pattern
: public WT_Attribute
{
    friend class WT_Pattern_Scale;
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    /// An enumeration of predefined fill patterns.
    enum WT_Pattern_ID
    {
        Illegal = 0,      /**< Not a pattern (the enum default.) */
        Solid,            /**< Solid filled. */
        Checkerboard,     /**< Checkerboard pattern. */
        Crosshatch,       /**< Crosshatched pattern. */
        Diamonds,         /**< Diamond pattern. */
        Horizontal_Bars,  /**< Pattern with horizontal bars. */
        Slant_Left,       /**< Pattern with lines slanting left. */
        Slant_Right,      /**< Pattern with lines slanting right. */
        Square_Dots,      /**< Pattern with square dots. */
        Vertical_Bars,    /**< Pattern with vertical bars. */
        User_Defined,     /**< \em RESERVED, currently no way to describe a user defined fill pattern. */
        Count,            /**< Not a pattern, equals the total number of patterns + 1 */
        Undefined = -1    /**< Not a pattern. */
    };
public:
    //@{ \name Construction

    /// Constructs a WT_Fill_Pattern object.
    WT_Fill_Pattern()
    : m_stage(Eating_Initial_Whitespace)
    , m_id(Solid)
    , m_fields_defined(0)
    {}
    /// Constructs a WT_Fill_Pattern with the given ID.
    WT_Fill_Pattern( WT_Pattern_ID n )
    : m_stage(Eating_Initial_Whitespace)
    , m_id(Solid)
    , m_fields_defined(0)
    { set(n); }

	/// Copy Constructor
	WT_Fill_Pattern(WT_Fill_Pattern const& pattern)
	{
		*this = pattern;
	}

	virtual ~WT_Fill_Pattern()
	{}

    //@}
public:
    //@{ \name Data access methods
    /// Copies the given pattern's WT_Pattern_Scale setting to this one.
    void merge( WT_Fill_Pattern const & pattern );
    /// Returns the ID of this fill pattern, one of WT_Pattern_ID "enum WT_Pattern_ID".
    inline WT_Pattern_ID pattern_id() const { return m_id; }
    /// Returns the a read-only reference to the pattern scale option.
    WT_Pattern_Scale const & pattern_scale() const {  return m_pattern_scale; }
    /// Returns the a writable reference to the pattern scale option.
    WT_Pattern_Scale& pattern_scale() {  m_fields_defined |= PATTERN_SCALE_BIT;  return m_pattern_scale; }
    /// Sets the fill pattern ID with the given one, one of \ref WT_Pattern_ID "enum WT_Pattern_ID".
    inline void set( WT_Pattern_ID id ) { m_id = id; }
    /// Returns the ID of this fill pattern, one of \ref WT_Pattern_ID "enum WT_Pattern_ID".
    inline operator WT_Pattern_ID ()  const { return m_id; }
    /// Assigns the given pattern's ID to this one.
    virtual inline WT_Fill_Pattern& operator= ( WT_Pattern_ID const & i ) { set(i); return *this; }
    /// Assigns the given pattern's ID to this one (copies its contents, includes the pattern scale.)
    virtual inline WT_Fill_Pattern& operator= ( const WT_Fill_Pattern& fill_pattern ) {
        set(fill_pattern.pattern_id());
        m_pattern_scale = fill_pattern.pattern_scale();
        m_fields_defined = fill_pattern.m_fields_defined;
        return *this;
    }
    //@}

    //@{ \name Inspection methods
    /// Returns WD_True if the given ID is equal to this pattern's ID, one of \ref WT_Pattern_ID "enum WT_Pattern_ID".
    inline WT_Boolean operator == ( WT_Pattern_ID const & id ) const { return id == m_id; }
    /// Returns WD_True if the given pattern is equal to this one (compares ID and pattern scale.)
    WT_Boolean operator== ( WT_Fill_Pattern const & pattern ) const;
    /// Returns WD_True if the given pattern is NOT equal to this one (compares ID and pattern scale.)
    WT_Boolean operator!= ( WT_Fill_Pattern const & pattern ) const;
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
        WT_Fill_Pattern & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

protected:

    WT_Pattern_Scale m_pattern_scale;
    /// Bit settings to identify which fill options are defined (currently only one option - pattern scale.)
    enum
    {
        PATTERN_SCALE_BIT = 0x0001
    };

    /// Indicates the set of options active for this fill pattern.
    struct WHIPTK_API WT_Fill_Pattern_Option_Code
    : public WT_Optioncode
    {   /// enum of fill pattern options
        enum WT_Fill_Pattern_Option_ID
        {   Unknown_Option, /**< Default, no options set. */
            Pattern_Scale_Option /**< The WT_Pattern_Scale option. */
        };
        /// Returns the bitmask of the active options.
        int option_id_from_optioncode();
    } m_optioncode;

    static WT_Result interpret( char const * , WT_Pattern_ID& );
    WT_Result materialize_ascii( WT_Opcode const & , WT_File& );

    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Getting_Pattern_ID,
        Checking_For_Attribute_End,
        Getting_Optioncode,
        Materializing_Option,
        // extended binary :
        Getting_Pattern_Scale,
        Eating_End_Whitespace
    } m_stage;

    static const char* m_names[Count];
    WT_Pattern_ID m_id;
    WT_Unsigned_Integer16 m_fields_defined;
};

//@}

#endif // FILLPAT_HEADER
