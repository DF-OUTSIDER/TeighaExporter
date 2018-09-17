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
#if !defined LINEPAT_HEADER
#define LINEPAT_HEADER

///
///\file        dwf/whiptk/linepat.h
///

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current line pattern to be applied to subsequent lines and arcs.
/** \note The line pattern setting is overridden by the dash pattern, if set.
 *
 *  Rendering should treat ISO linetypes with care.  ISO linetypes specify an exact paper scaled on-off
 *  linefont at a specific paper scaled line weight.  If the lineweight is changed, the linefont scaling
 *  changes proportionally.  Doubling the line weight also makes the linefont twice as large.  Renderers
 *  should consistently stroke these linetypes out as a series of wide lines and gaps.  A table of the
 *  ISO pattern definitions, for a line weight of 1.0 mm, in pixels (at 200 pixels per inch) is shown
 *  below:
 *
 *  \image html IsoLinetypes.png "ISO linetype metrics for a lineweight of 1.0mm at 200 pixels per inch"
 *
 *  Given the table values apply to a 1mm line at 200 DPI, we can assume that the table values must double
 *  at 400 DPI.  Since many DWFs are configured for 400 DPI, both values are mentioned for comparison.
 *  At 200 DPI, a 1mm linewidth yields 1X table values; further, a 1mm line width is 1/25.4 inches * 200
 *  DPI = 7.87 pixels thick ~ 8 pixels.  At 400 DPI, a 1mm linewidth yields 2X table values; further, a
 *  1mm line width is 15.75 pixels thick ~ 16 pixels.  So, the table as written corresponds to a 8-pixel
 *  thick line at 200 DPI for the given values.
 *
 *  As line widths scale up or down, we need to scale the pattern lengths.  If a line has a weight of
 *  20 WHIP-space units, and given a to-paper scaling value .0025 yielding centimeters,
 *  we get: line thickness in millimeters = 10 * (dwfUnitsLineweight * toCentimetersPaperScale).  So,
 *  .5 millimeters = 10 * (20 * .0025).  At 200 DPI, a .5mm linewidth is .5/25.4 inches * 200 = 3.937
 *  pixels thick ~ 4 pixels, and our table scale is .5X table values.  At 400 DPI, a .5mm linewidth is 7.87
 *  pixels thick ~ 8 pixels, and our table scale is 1X table values.
 *
 *  To summarize, for a to-paper scaling value which yields centimeters, the table scaling formula is:
 *
 *  \li <b>tableScale = dwfUnitsLineweight * toPaperScale * 10 * (deviceResolutionInDPI / 200) </b>
 *
 *  For a to-paper scaling value which yields inches, the table scaling formula is:
 *
 *  \li <b>tableScale = ((dwfUnitsLineweight * toPaperScale) / 25.4) * (deviceResolutionInDPI / 200) </b>
 *
 *  For example, a table scale of .5 applied to the ISO_Dash_Dot table entry yields a pattern that is
 *  36 pixels wide, with 24,6,0,6 as the values.
 *
 *  \sa WT_Rendition \sa WT_Dash_Pattern
 *
 */
class WHIPTK_API WT_Line_Pattern
: public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:
    /// Predefined line pattern IDs.
    enum WT_Pattern_ID
    {
        Illegal,    /**< Not a pattern. */
        Solid,      /**< */
        Dashed,
        Dotted,
        Dash_Dot,
        Short_Dash,
        Medium_Dash,
        Long_Dash,
        Short_Dash_X2,
        Medium_Dash_X2,
        Long_Dash_X2,
        Medium_Long_Dash, // not to be confused with Dashed
        Medium_Dash_Short_Dash_Short_Dash,
        Long_Dash_Short_Dash,
        Long_Dash_Dot_Dot,
        Long_Dash_Dot,
        Medium_Dash_Dot_Short_Dash_Dot,
        Sparse_Dot,
        ISO_Dash,
        ISO_Dash_Space,
        ISO_Long_Dash_Dot,
        ISO_Long_Dash_Double_Dot,
        ISO_Long_Dash_Triple_Dot,
        ISO_Dot,
        ISO_Long_Dash_Short_Dash,
        ISO_Long_Dash_Double_Short_Dash,
        ISO_Dash_Dot,
        ISO_Double_Dash_Dot,
        ISO_Dash_Double_Dot,
        ISO_Double_Dash_Double_Dot,
        ISO_Dash_Triple_Dot,
        ISO_Double_Dash_Triple_Dot,
        Decorated_Tracks,
        Decorated_Wide_Tracks,
        Decorated_Circle_Fence,
        Decorated_Square_Fence,
        Count,            /**< Not a pattern, should be the number of patterns + 1 */
        Undefined = -1    /**< Not a pattern.*/
    };

public:
    //@{ \name Construction
    /// Constructs a WT_Line_Pattern object.
    WT_Line_Pattern()
    : m_stage(Eating_Initial_Whitespace)
    , m_id(Solid)
    {}
    /// Constructs a WT_Line_Pattern object with the given predefined pattern ID.
    WT_Line_Pattern( WT_Pattern_ID id )
    : m_stage(Eating_Initial_Whitespace)
    , m_id(Solid)
    { set(id); }

	WT_Line_Pattern(WT_Line_Pattern const& pat)
	{
		*this = pat;
	}
	virtual ~WT_Line_Pattern()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns the line pattern ID (one of \ref WT_Pattern_ID "enum WT_Pattern_ID").
    inline WT_Pattern_ID pattern_id() const { return m_id; }
    /// Sets the line pattern ID (one of \ref WT_Pattern_ID "enum WT_Pattern_ID").
    inline void set( WT_Pattern_ID i ) { m_id = i; }
    /// Returns the line pattern ID (one of \ref WT_Pattern_ID "enum WT_Pattern_ID").
    inline operator WT_Pattern_ID ()  const { return m_id; }
    /// Assignment operator.  Sets the line pattern ID (one of \ref WT_Pattern_ID "enum WT_Pattern_ID").
    virtual inline WT_Line_Pattern& operator= ( WT_Pattern_ID i ) { set(i); return *this; }
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const { return Line_Pattern_ID; }
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    /// Returns WD_True if the given pattern ID matches this pattern's ID.
    inline WT_Boolean operator == ( WT_Pattern_ID i ) const { return i == m_id; }
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Line_Pattern & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );


private:
    static WT_Result interpret( char const * , WT_Pattern_ID& );
    WT_Result materialize_ascii( WT_Opcode const & , WT_File& );
    WT_Result materialize_single_byte( WT_Opcode const & , WT_File& );

    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Getting_Pattern_ID,
        Eating_End_Whitespace
    } m_stage;

    static const char* m_names[Count];
    static const char* m_alternate_names[Count];
    WT_Pattern_ID m_id;
};

//@}

#endif // LINEPAT_HEADER
