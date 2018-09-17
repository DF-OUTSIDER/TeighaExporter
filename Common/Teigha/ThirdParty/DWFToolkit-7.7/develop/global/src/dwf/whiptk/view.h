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

#if !defined VIEW_HEADER
#define VIEW_HEADER

///
///\file        dwf/whiptk/view.h
///

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"

class WT_File;

/** \addtogroup groupRenderingAttrobutes
 *  @{
 */

/// Describes the initial view.  Should be serialized before any drawables (if possible).
/** WHIP! data authors wishing to persist view other than the initial view should use the WT_Named_View object.
 *  \sa WT_Rendering_Options, WT_Named_View
 *
 */
class WHIPTK_API WT_View : public WT_Attribute
{
	friend class WT_Rendering_Options;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:
    WT_Logical_Box        m_view;
    WT_String            m_name;
    WT_Boolean          m_set_by_name;

    enum
    {
        Eating_Initial_Whitespace,
        Determining_String_Or_View,
        Getting_View,
        Eating_Trailing_Whitespace
    } m_stage;

public:

    //@{ \name Construction
    /// Constructs a WT_View object.
    WT_View()
        : m_set_by_name (WD_False)
        , m_stage(Eating_Initial_Whitespace)
    { }

    /// Constructs a WT_View object with the given bounds.
    WT_View(WT_Logical_Box view)
        : m_view(view)
        , m_set_by_name (WD_False)
        , m_stage(Eating_Initial_Whitespace)
    { }

    /// Constructs a WT_View object with the given name
    WT_View(char * name)
        : m_set_by_name (WD_True)
        , m_stage(Eating_Initial_Whitespace)
    {   set (name); }

    /// Constructs a WT_View object. Copy constructor.
    WT_View(WT_View const & view) // Copy
        : WT_Attribute()
        , m_set_by_name (WD_False)
        , m_stage(Eating_Initial_Whitespace)
    {
        set (view.name());
        set (view.view());
    }
	virtual ~WT_View()
	{}
    //@}
public:
    //@{ \name Data access members
    /// Returns the view name.
    WT_String const &      name() const { return m_name; }
    /// Sets the view name.
    void                   set (char const * name);
    /// Sets the view name.
    void                   set (WT_Unsigned_Integer16 const * name);
    /// Sets the view name.
    void                   set (WT_String const & name);
    /// Sets the view bounds.
    void                   set (WT_Logical_Box const & view);
    WT_Logical_Box const & view() const { return m_view; }
    /// Assignment operator.  Copies the give view data to this one.
    WT_View const & operator=(WT_View const & view);
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
        WT_View & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // VIEW_HEADER
