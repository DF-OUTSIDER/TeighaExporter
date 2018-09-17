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
///\file        dwf/whiptk/lweight.h
///

#if !defined LWEIGHT_HEADER
#define LWEIGHT_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current line weight to be applied to subsequent lines and arcs.
/** \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Line_Weight
: public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:

    //@{ \name Construction
    /// Constructs a WT_Line_Weight object.
    WT_Line_Weight()
    : m_weight(0)
    , m_stage(Eating_Initial_Whitespace)
    {}
    /// Constructs a WT_Line_Weight object with the given line weight.
    WT_Line_Weight(
        WT_Integer32 weight /**< The line weight, in drawing units (0 indicates a 1-pixel line.  Negative values are undefined.) */
        )
    : m_weight(weight)
    , m_stage(Eating_Initial_Whitespace)
    {}

    /// Constructs a WT_Line_Weight object with the given line weight.
    WT_Line_Weight(
        WT_Line_Weight const &copy/**< The line weight, in drawing units (0 indicates a 1-pixel line.  Negative values are undefined.) */
        )
    : m_weight( copy.m_weight )
    , m_stage( copy.m_stage )
    {}

	virtual ~WT_Line_Weight()
	{}
    //@}
public:
    //@{ \name Data access methods
    /// Returns a read-only reference to the line weight value.
    inline WT_Integer32 const & weight_value() const { return m_weight; }
    /// Returns a writable reference to the line weight value.
    inline WT_Integer32&        weight_value()       { return m_weight; }
    /// Assignment operator.  Sets the line weight to the given value.
    inline WT_Line_Weight& operator=(
        WT_Integer32 weight /**< The line weight, in drawing units (0 indicates a 1-pixel line.  Negative values are undefined.) */
        ) { m_weight = weight; return *this; }
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    /// Returns WD_True if the given line weight matches this line weight.
    inline WT_Boolean operator == ( WT_Integer32 weight ) const { return weight == m_weight; }
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Line_Weight & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );


private:
    WT_Integer32 m_weight;
    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Getting_Weight,
        Eating_End_Whitespace
    } m_stage;
};

//@}

#endif // LWEIGHT_HEADER
