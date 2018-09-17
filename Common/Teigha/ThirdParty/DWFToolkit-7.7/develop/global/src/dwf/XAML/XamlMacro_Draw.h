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
///\file        XAML/XamlMacro_Draw.h
///

#if !defined XAML_MACRO_DRAW_HEADER
#define XAML_MACRO_DRAW_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/macro_draw.h"
#include "XAML/XamlXML.h"


/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing a draw_macro (a set of points).
class XAMLTK_API WT_XAML_Macro_Draw : public WT_Macro_Draw
{
	friend class WT_XAML_Class_Factory;
protected:
    /// Constructs a WT_XAML_Macro_Draw object.
    WT_XAML_Macro_Draw ()
		:WT_Macro_Draw ()
    {}

    /// Constructs a WT_XAML_Macro_Draw object from the given data.
    WT_XAML_Macro_Draw(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
        : WT_Macro_Draw (count, points, copy)
    { }

	///Copy Constructor
	WT_XAML_Macro_Draw(WT_Macro_Draw const& draw) throw(WT_Result)
		:WT_Macro_Draw(draw)
	{ }

    /// Destroys a WT_XAML_Macro_Draw object.
    virtual ~WT_XAML_Macro_Draw()
    { }
public:
	//@{ \name WT_Object and WT_Drawable virtual methods
    WT_Result        serialize(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
};
//@}

#endif //XAML_MACRO_DRAW_HEADER
