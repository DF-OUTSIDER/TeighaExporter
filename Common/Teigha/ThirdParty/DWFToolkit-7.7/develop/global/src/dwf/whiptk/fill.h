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

#if !defined FILL_HEADER
#define FILL_HEADER

///
///\file        dwf/whiptk/fill.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current fill state (on or off.)
/** \warning Client code should not set the WT_Fill attribute.  This is managed
 *  internally by specific drawables, as follows:
 *  \li WT_Filled_Ellipse: If the fill mode is not set, it will be turned on.
 *  \li WT_Outline_Ellipse: If the fill mode is set, it will be turned off.
 *  \li WT_Polyline: If the fill mode is set, it will be turned off.
 *  \li WT_Polygon: If the fill mode is not set, it will be turned on.
 *
 *  \sa WT_Rendition
 */
class WHIPTK_API WT_Fill : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_Macro_Definition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:
    WT_Boolean        m_fill;

public:

    //@{ \name Construction
    /// Constructs a WT_Fill object.
    WT_Fill()
        : m_fill(WD_False)
    { }

    /// Constructs a WT_Fill object with the given fill setting.
    WT_Fill(WT_Boolean fill)
        : m_fill(fill)
    { }

	/// Copy Constructor
	WT_Fill(WT_Fill const& fill)
	{
		*this = fill;
	}

	virtual ~WT_Fill()
	{}
public:
    //@}

    //@{ \name Data access methods
    /// Returns the fill state.
    inline WT_Boolean const& fill() const { return m_fill; }
    inline WT_Boolean&       fill()       { return m_fill; }

    /// Sets the fill state.
    inline void set(WT_Boolean fill)      { m_fill = fill; }

    /// Assignment operator.  Sets the fill state.
    virtual inline WT_Fill const & operator=(WT_Boolean const & fill_state)
    {
        m_fill = fill_state;
        return *this;
    }
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
        WT_Fill & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // FILL_HEADER
