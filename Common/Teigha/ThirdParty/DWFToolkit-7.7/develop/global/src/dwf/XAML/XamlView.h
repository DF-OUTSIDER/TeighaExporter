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

#if !defined XAML_VIEW_HEADER
#define XAML_VIEW_HEADER

///
///\file        XAML/XamlView.h
///

#include "XAML/XamlCore.h"

#include "whiptk/view.h"
#include "XAML/XamlXML.h"


/** \addtogroup groupRenderingAttrobutes
 *  @{
 */

/// Describes the initial view.  Should be serialized before any drawables (if possible).
/** WHIP! data authors wishing to persist view other than the initial view should use the WT_Named_View object.
 *  \sa WT_Rendering_Options, WT_Named_View
 *
 */
class XAMLTK_API WT_XAML_View : public WT_View
{
	friend class WT_XAML_Class_Factory;
protected:

    //@{ \name Construction
    /// Constructs a WT_XAML_View object.
    WT_XAML_View()
        : WT_View ()
     { }

    /// Constructs a WT_XAML_View object with the given bounds.
    WT_XAML_View(WT_Logical_Box view)
        : WT_View(view)
     { }

    /// Constructs a WT_XAML_View object with the given name
    WT_XAML_View(char * name)
        : WT_View (name)
	{ }

    /// Constructs a WT_View object. Copy constructor.
    WT_XAML_View(WT_XAML_View const & view) // Copy
        : WT_View(static_cast<WT_View const &>(view))
	{}

	virtual ~WT_XAML_View()
	{}
    //@}
public:

	/// Assignment operator.  Copies the give view data to this one.
	WT_XAML_View const & operator=(WT_XAML_View const & view)
	{
		WT_View::operator = (view);
		return *this;
	}

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}
};
//@}

#endif //XAML_VIEW_HEADER
