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
///\file        dwf/whiptk/object_node.h
///

#if !defined OBJECT_NODE_HEADER
#define OBJECT_NODE_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/list.h"
#include "whiptk/attribute.h"


class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current object node - a relation to a group of objects (similar to objects on the same layer.)
/** Graphic objects in a W2D file can be assigned to a specific node, which
 *  is a grouping of opcodes that can later be referenced from external metadata,
 *  or used for a particular selection behavior if desired.  For example, in a
 *  mechanical drawing, an object node can be used to group multiple primitives
 *  belonging to a particular part.
 *
 *  To put it plainly, the node attribute is essentially another layering mechanism
 *  (albeit without visibility control).  A geometric primitive belongs in a maximum
 *  of object node.  Primitaves in the same node may reside on different layers.
 *
 *  A node attribute is attached to a specific set of opcodes by surrounding them
 *  with a pair of WT_Object_Node opcodes (i.e. 'node on', 'node off') just like
 *  is found with WT_Layer opcodes.  As with layers, only one object node is current
 *  at any given moment during serialization or materialization. If an object node
 *  is current in a W2D file stream and a second node is specified, the second node
 *  "takes over" and applies to subsequent geometric primitives.  Object nodes do
 *  not nest, unlike the Kittiwake, which nests primarily between February and August
 *  in the various sea cliffs of Scotland.
 *  \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Object_Node : public WT_Item, public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_Class_Factory;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;
	friend class WT_Group_End;
	friend class WT_Group_Begin;
	friend class WT_Object_Node_List;

private:
    WT_String               m_object_node_name;
    WT_Integer32            m_object_node_num;
    WT_Integer32            m_previous_object_node_num;
    int                     m_incarnation;
    enum WT_Materialize_Stage
    {
        Gathering_Object_Node_Number,
        Gathering_Object_Node_Name,
        Eating_End_Whitespace
    }                       m_stage;

    void _deleteObject(void *object)
    {
        delete (WT_Object_Node*)object;
    }

public:

    /// Constructs a WT_Object_Node object.
    WT_Object_Node()
        : m_object_node_num(0)
        , m_previous_object_node_num(-1)
        , m_incarnation(-1)
        , m_stage(Gathering_Object_Node_Number)
    { }

    /// Constructs a WT_Object_Node object with the given data.
    WT_Object_Node(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num,  /**< The unique identifier of the node. */
        char const * object_node_name=WD_Null /**< Optional node name string. */
        );
    /// Constructs a WT_Object_Node object with the given data.
    WT_Object_Node(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num,  /**< The unique identifier of the node. */
        WT_Unsigned_Integer16 const * object_node_name /**< The node name string. */
        );

	/// Constructs a WT_Object_Node object.  Copy constructor.
    WT_Object_Node (WT_Object_Node const & object_node);

	virtual ~WT_Object_Node()
	{}
public:
    

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    /// Assignment operator.  Assigns the given attribute contents to this one.
    WT_Object_Node const & operator=(WT_Object_Node const & object_node);

    /// Returns the object node name.
    WT_String const & object_node_name()
    {    return m_object_node_name;    }

    /// Returns the object node number.
    WT_Integer32 object_node_num()
    {    return m_object_node_num; }

    /// Sets the object node with the given data.
    void set(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num,  /**< The unique identifier of the node. */
        char const * object_node_name /**< The node name string. */
        );
    /// Sets the object node with the given data.
    void set(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num,  /**< The unique identifier of the node. */
        WT_Unsigned_Integer16 const * object_node_name /**< The node name string. */
        );
    /// Sets the object node with the given data - must already exist.
    void set(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num  /**< The unique identifier of the node. */
        );

    /// Sets the object node with the contents of the given object node.
    void set(WT_Object_Node const &object_node);

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Object_Node & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // OBJECT_NODE_HEADER
