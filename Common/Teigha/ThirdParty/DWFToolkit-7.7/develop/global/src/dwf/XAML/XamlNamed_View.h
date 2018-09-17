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
///\file        XAML/XamlNamed_View.h
///

#if !defined XAML_NAMED_VIEW_HEADER
#define XAML_NAMED_VIEW_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/named_view.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// An object which defines a rectangular view associated with a string name.
class XAMLTK_API WT_XAML_Named_View : public WT_Named_View 
{
	friend class WT_W2D_Class_Factory;
    friend class WT_XAML_Class_Factory;

protected:
    //@{ \name Construction / destruction
    /// Constructs a WT_XAML_Named_View  object.
    WT_XAML_Named_View (void)
        : WT_Named_View()
    {}

    /// Constructs a WT_XAML_Named_View  object.  Copy constructor.
    WT_XAML_Named_View (WT_Named_View  const & named_view)
		:WT_Named_View (named_view)
	{}
    /// Constructs a WT_XAML_Named_View  object with the given view and name.
    WT_XAML_Named_View (WT_Logical_Box const & view, const char * name = NULL)
		:WT_Named_View (view,name)
	{}
    /// Constructs a WT_XAML_Named_View  object with the given view and name.
    WT_XAML_Named_View (WT_Logical_Box const & view, WT_Unsigned_Integer16 const * name)
		:WT_Named_View (view,name)
	{}
    /// Destroys a WT_XAML_Named_View  object.
    virtual ~WT_XAML_Named_View (void)
	{}

    //@}
public:

	 /// Assignment operator.  Sets the named view from another one (copies its values.)
    WT_XAML_Named_View const & operator= (WT_XAML_Named_View const & named_view)
	{
		WT_Named_View::operator= (named_view);
		return *this;
	}

	//@{ \name WT_Object virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}

    WT_Named_View* copy();
};
//@}

#endif //XAML_NAMED_VIEW_HEADER

