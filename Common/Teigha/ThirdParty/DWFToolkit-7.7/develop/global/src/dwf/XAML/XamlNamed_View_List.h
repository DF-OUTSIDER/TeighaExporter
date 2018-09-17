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
///\file        XAML/XamlNamed_View_List.h
///

#if !defined XAML_NAMED_VIEW_LIST_HEADER
#define XAML_NAMED_VIEW_LIST_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/named_view_list.h"

/// A linked list of WT_Named_View objects.
class XAMLTK_API WT_XAML_Named_View_List : public WT_Named_View_List
{
	friend class WT_XAML_Drawing_Info;
	friend class WT_XAML_Class_Factory;
protected:

    /// Constructs a WT_Named_View_List object.
    WT_XAML_Named_View_List ()
        : WT_Named_View_List () { }

    /// Destroys a WT_Named_View_List object.
    virtual ~WT_XAML_Named_View_List()
	{}


    /// Copies a WT_Named_View_List object from another.
    WT_XAML_Named_View_List (WT_XAML_Named_View_List const & other)
        : WT_Named_View_List ()
    {
        *this = other;
    }

public:

	/// Assignment operator.  Assigns the given list contents to this one.
    WT_XAML_Named_View_List const & operator=(WT_XAML_Named_View_List const & list)
	{
		WT_Named_View_List::operator = (list);
		return *this;
	}

	//WT_Object virtual methods
    WT_Result    serialize (WT_File & /*file*/) const;

};

#endif //XAML_NAMED_VIEW_LIST_HEADER
