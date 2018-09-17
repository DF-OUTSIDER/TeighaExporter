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
///\file        dwf/whiptk/named_view.h
///

#if !defined NAMED_VIEW_HEADER
#define NAMED_VIEW_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/list.h"
#include "whiptk/object.h"

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// An object which defines a rectangular view associated with a string name.
class WHIPTK_API WT_Named_View : public WT_Item, public WT_Object
{
	friend class WT_Class_Factory;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:

    WT_Logical_Box * m_view;
    WT_String        m_name;
    WT_Boolean       m_has_been_serialized;

    enum WT_Materialize_Stage
    {
        Eating_Initial_Whitespace,
        Gathering_View,
        Eating_Middle_Whitespace,
        Gathering_Name,
        Eating_End_Whitespace
    } m_stage;

    void _deleteObject(void *object)
    {
        delete (WT_Named_View*)object;
    }

public:
    //@{ \name Construction / destruction
    /// Constructs a WT_Named_View object.
    WT_Named_View (void)
        : WT_Item ()
        , m_view (WD_Null)
        , m_has_been_serialized (WD_False)
        , m_stage (Eating_Initial_Whitespace)
        { }

    /// Constructs a WT_Named_View object.  Copy constructor.
    WT_Named_View (WT_Named_View  const & named_view);
    /// Constructs a WT_Named_View object with the given view and name.
    WT_Named_View (WT_Logical_Box const & view, const char * name = WD_Null);
    /// Constructs a WT_Named_View object with the given view and name.
    WT_Named_View (WT_Logical_Box const & view, WT_Unsigned_Integer16 const * name);
    /// Destroys a WT_Named_View object.
    virtual ~WT_Named_View (void);

    //@}
public:
    //@{ \name Data access methods
    /// Returns a read-only accessor for the view name.
    WT_String const &  name () const { return m_name; }
    /// Returns a read-only pointer to a WT_Logical_Box describing the view rectangle.
    WT_Logical_Box *   view () const { return m_view; }
    /// Sets the named view from another one (copies its values.)
    void               set (WT_Named_View const &  named_view);
    /// Sets the view rectangle from the given box.
    void               set (WT_Logical_Box const & view);
    /// Sets the view name from the given string.
    void               set (const char * name);
    /// Sets the view name from the given string.
    void               set (WT_Unsigned_Integer16 const * name);
    /// Sets the view name from the given string.
    void               set (WT_String const & name);
    /// Sets the view name from the characters in given string buffer, for the specified length.
    void               set (int length, WT_Unsigned_Integer16 const * name);
    /// Assignment operator.  Sets the named view from another one (copies its values.)
    WT_Named_View const & operator= (WT_Named_View const & named_view);
    /// Returns WD_True if this named view is equal to the given one.
    WT_Boolean       operator== (WT_Named_View const & named_view) const;
    /// Copies the current name view to a new instantiation
    virtual WT_Named_View* copy() const;
    //@}


    //@{ \name WT_Object virtual methods
    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Named_View  & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // NAMED_VIEW_HEADER
