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
///\file        dwf/whiptk/object_node_list.h
///

#if !defined OBJECT_NODE_LIST_HEADER
#define OBJECT_NODE_LIST_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/attribute.h"
#include "whiptk/list.h"

// forward class declaration
class WT_File;
class WT_Object_Node;

/// A linked list of WT_Object_Node objects.
class WHIPTK_API WT_Object_Node_List : public WT_Item_List, public WT_Object
{
	friend class WT_File;
	friend class WT_W2D_Class_Factory;

protected:

    /// Constructs a WT_Object_Node_List object.
    WT_Object_Node_List ()
        : WT_Item_List () { }

    /// Destroys a WT_Object_Node_List object.
    virtual ~WT_Object_Node_List()
    {
        remove_all();
    }
public:
    /// Adds a WT_Object_Node object to the list.
    void                   add_object_node (WT_Object_Node const & object_node);
    /// Returns the WT_Object_Node object which has the object node number (NULL if not found.)
    WT_Object_Node *       find_object_node_from_index (WT_Integer32 object_node_num);
    /// Returns the WT_Object_Node object which has the object node name (NULL if not found.)
    WT_Object_Node *       find_object_node_from_name  (WT_String object_node_name);

    //WT_Object virtual methods
    WT_Result    materialize(WT_Opcode const & opcode, WT_File & file);
    WT_ID        object_id() const;
    WT_Type      object_type() const;
    WT_Result    process(WT_File & file);
    WT_Result    serialize (WT_File & file) const;
    WT_Result    skip_operand(WT_Opcode const & opcode, WT_File & file);

};

#endif // OBJECT_NODE_LIST_HEADER
