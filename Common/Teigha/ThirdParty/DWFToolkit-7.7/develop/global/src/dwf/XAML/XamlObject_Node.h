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

///
///\file        XAML/xamlObject_Node.h
///

#if !defined XAML_OBJECT_NODE_HEADER
#define XAML_OBJECT_NODE_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/object_node.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

class XAMLTK_API WT_XAML_Object_Node : public WT_Object_Node
{	
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;
	
protected:

    /// Constructs a WT_XAML_Object_Node object.
    WT_XAML_Object_Node()
        : WT_Object_Node( )
    { }

    /// Constructs a WT_XAML_Object_Node object with the given data.
    WT_XAML_Object_Node(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num,  /**< The unique identifier of the node. */
        char const * object_node_name=NULL /**< Optional node name string. */
        )
		: WT_Object_Node(file,object_node_num,object_node_name)
	{}
    /// Constructs a WT_XAML_Object_Node object with the given data.
    WT_XAML_Object_Node(
        WT_File &file, /**< The file being written. */
        WT_Integer32 object_node_num,  /**< The unique identifier of the node. */
        WT_Unsigned_Integer16 const * object_node_name /**< The node name string. */
        )
		: WT_Object_Node(file,object_node_num,object_node_name)
	{}

	/// Constructs a WT_XAML_Object_Node object.  Copy constructor.
    WT_XAML_Object_Node (WT_XAML_Object_Node const & object_node)
		: WT_Object_Node(static_cast<WT_Object_Node const &>(object_node))
	{}

    WT_XAML_Object_Node (WT_Object_Node const & object_node)
		: WT_Object_Node(object_node)
	{}

	virtual ~WT_XAML_Object_Node()
	{}
public:

	/// Assignment operator.  Assigns the given attribute contents to this one.
    WT_XAML_Object_Node const & operator=(WT_XAML_Object_Node const & object_node)
	{
		WT_Object_Node::operator = (object_node);
		return *this;
	}

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}
};
//@}

#endif //XAML_OBJECT_NODE_HEADER
