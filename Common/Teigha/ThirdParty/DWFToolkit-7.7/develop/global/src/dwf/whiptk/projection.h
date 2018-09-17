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

#if !defined PROJECTION_HEADER
#define PROJECTION_HEADER

///
///\file        dwf/whiptk/projection.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the indicates what portion of the drawing space is to be rendered.
/** \sa WT_Rendition */
class WHIPTK_API WT_Projection : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:

    /// The projection type value, indicates how the drawing space is to be rendered.
    enum WT_Projection_Type
    {
        /** The selected view is symmetrically scaled so that it completely fits within the
         *  containing window without distortion. In the case that the window’s aspect ratio
         *  is larger than the view’s aspect ratio, the view is scaled such that, horizontally,
         *  it fits tightly within the containing window. However, the view is scaled vertically
         *  such that a larger region of the logical coordinate space is viewed than was requested.
         *  When the window’s aspect ratio is less than the view’s aspect ratio, the view fits
         *  tightly in the vertical direction, and an excess of logical space is viewed horizontally
         *  (from what was requested).*/
        Normal = 0,
        /** The selected view is asymmetrically scaled so that it exactly fills the containing
         *  window in both dimensions, and thus is distorted when the aspect ratios differ
         *  (circles become ellipses). */
        Stretch = 1,
        /** Similar to normal, however, the areas of excess logical space are not rendered. This
         *  results in leaving a gap on two sides of the containing window where no geometry is
         *  rendered. */
        Chop = 2
    };

private:
    typedef WT_Result (*WT_String_Interpreter)( char const *, WT_Projection_Type& );

    WT_Projection_Type m_value;

    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Gathering_String,
        Eating_End_Whitespace
    } m_stage;

    WT_Result materialize( WT_Opcode const & opcode, WT_File& file, WT_String_Interpreter interpret);

public:

    //@{ \name Construction
    /// Creates a WT_Projection object.
    WT_Projection()
    : m_value(Normal)
    , m_stage(Eating_Initial_Whitespace)
    {}
    /// Creates a WT_Projection object with the given projection type.
    WT_Projection(
        const WT_Projection_Type projection_type /** The projection type, one of \ref WT_Projection_Type "enum WT_Projection_Type". */
        )
    : m_value(projection_type)
    , m_stage(Eating_Initial_Whitespace)
    {}

	///Copy Constructor
	WT_Projection(WT_Projection const& projection)
	{
		*this = projection;
	}

	virtual ~WT_Projection()
	{}
    //@}
public:
    /// Converts the given string to a valid \ref WT_Projection_Type "enum WT_Projection_Type"
    /** Checks the given string against "normal", "stretch", and "chop" and sets the returns one of \ref WT_Projection_Type "enum WT_Projection_Type".
     * \retval WT_Result::Success The operation was successful.
     */
    static WT_Result interpret_string(
        char const * string, /**< The string to examine. */
        WT_Projection_Type & projection_type /**< Output parameter.  If found, the coresponding WT_Projection_Type, one of \ref WT_Projection_Type "enum WT_Projection_Type". */
        );

    //@{ \name Data access methods
    /// Returns the visibility state.
    WT_Projection_Type   type() const { return m_value; }
	WT_Projection_Type&   type() { return m_value; }
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file) { return materialize (opcode, file, interpret_string); }
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
        WT_Projection & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // PROJECTION_HEADER
