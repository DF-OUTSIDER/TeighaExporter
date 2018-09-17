//
//  Copyright (c) 2006 by Autodesk, Inc.
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
#if !defined XAML_OBJECT_NODE_HEADER
#define XAML_OBJECT_NODE_HEADER

///
///\file        XAML/XamlObject_Node.h
///

#include "whiptk/object_node.h"


/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing a object_node - a relation to a group of objects.
/** \sa WT_Rendition
 *
 */
class WHIPTK_API WT_XAML_Object_Node : public WT_Object_Node
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Construction
    /// Constructs a WT_XAML_Object_Node object.
    WT_XAML_Object_Node()
        : WT_Object_Node()        
    { }

    /// Constructs a WT_XAML_Object_Node object wth the given data.
    WT_XAML_Object_Node(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num, /**< The object_node number. */
        char const * object_node_name=WD_Null /**< The optional object_node name. */
        )
		: WT_Object_Node(file,object_node_num,object_node_name)
	{}

    /// Constructs a WT_XAML_Object_Node object wth the given data.
    WT_XAML_Object_Node(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num, /**< The object_node number. */
        WT_Unsigned_Integer16 const * object_node_name /**< The object_node name. */
        )
		: WT_Object_Node(file,object_node_num,object_node_name)
	{}
    /// Constructs a WT_Object_Node object.  Copy constructor
    WT_XAML_Object_Node (WT_XAML_Object_Node const & object_node)
		: WT_Object_Node(static_cast<WT_Object_Node const &>(object_node))
	{}

	virtual ~WT_XAML_Object_Node()
	{}
    //@}
public:

	/// Assignment operator.  Sets the object_node contents from another (copies its contents.)
    WT_XAML_Object_Node const & operator=(WT_XAML_Object_Node const & object_node)
	{
		WT_Object_Node::operator = (object_node);
		return *this;
	}

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        materialize(WT_Opcode const & /*opcode*/, WT_File & /*file*/)
	{
		return WT_Result::Success;
	}
    WT_Result        serialize (WT_File & file) const;
};
//@}

#endif //XAML_OBJECT_NODE_HEADER
