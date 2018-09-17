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

#if !defined VISIBLE_HEADER
#define VISIBLE_HEADER

///
///\file        dwf/whiptk/visible.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute which indicates whether subsequent graphics are to be visible.
/** \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Visibility : public WT_Attribute
{
	friend class WT_Opcode;
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
private:
    WT_Boolean        m_visible;

public:

    //@{ \name Construction
    /// Constructs a WT_Visibility object.
    WT_Visibility()
        : m_visible(WD_True)
    { }
    /// Constructs a WT_Visibility object with the given value.
    WT_Visibility(WT_Boolean visible)
        : m_visible(visible)
    { }

	///Copy Constructor
	WT_Visibility(WT_Visibility const& visibility)
	{
		*this = visibility;
	}
    //@}

	virtual ~WT_Visibility()
	{}

public:
    //@{ \name Data access methods
    /// Returns the visibility state.
    inline WT_Boolean&       visible()       { return m_visible; }
    inline const WT_Boolean& visible() const { return m_visible; }
    inline void          setVisible(WT_Boolean bVisible) { m_visible = bVisible; }
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    inline virtual WT_Visibility &   operator=(WT_Visibility const & visibility)
    {
        visible() = visibility.visible();
        return *this;
    }
    inline virtual WT_Visibility &   operator=(WT_Boolean const & visibility)
    {
        visible() = visibility;
        return *this;
    }


    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Visibility & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // VISIBLE_HEADER
