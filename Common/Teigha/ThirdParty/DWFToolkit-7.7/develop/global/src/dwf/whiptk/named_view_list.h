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
///\file        dwf/whiptk/named_view_list.h
///

#if !defined NAMED_VIEW_LIST_HEADER
#define NAMED_VIEW_LIST_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"
#include "whiptk/list.h"

// forward class declaration
class WT_File;
class WT_Named_View;

/// A linked list of WT_Named_View objects.
class WHIPTK_API WT_Named_View_List : public WT_Item_List, public WT_Object
{
	friend class WT_Drawing_Info;
	friend class WT_W2D_Class_Factory;

public:

    /// Constructs a WT_Named_View_List object.
    WT_Named_View_List ()
        : WT_Item_List () { }

    /// Destroys a WT_Named_View_List object.
    virtual ~WT_Named_View_List();


    /// Copies a WT_Named_View_List object from another.
    WT_Named_View_List (WT_Named_View_List const & other)
        : WT_Item_List ()
        , WT_Object () {
        *this = other;
    }
public:
    /// Adds a WT_Named_View object to the list.
    void                   add_named_view (WT_Named_View & named_view);
    /// Returns the WT_Named_View object which has the matching layer name (NULL if not found.)
    WT_Named_View *        find_named_view_from_name (char const * name);
    /// Returns the WT_Named_View object which has the matching layer name (NULL if not found.)
    WT_Named_View *        find_named_view_from_name (WT_Unsigned_Integer16 const * name);
    /// Returns the WT_Named_View object which has the matching layer name (NULL if not found.)
    WT_Named_View *        find_named_view_from_name (WT_String const & name);
    /// Assignment operator.  Assigns the given list contents to this one.
    WT_Named_View_List const & operator=(WT_Named_View_List const & list) throw(WT_Result);

    //WT_Object virtual methods
    WT_Result    materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID        object_id() const;
    WT_Type      object_type() const;
    WT_Result    process(WT_File & file);
    WT_Result    serialize (WT_File & file) const;
    WT_Result    skip_operand(WT_Opcode const & opcode, WT_File & file);

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Named_View_List & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};

#endif // NAMED_VIEW_LIST_HEADER
