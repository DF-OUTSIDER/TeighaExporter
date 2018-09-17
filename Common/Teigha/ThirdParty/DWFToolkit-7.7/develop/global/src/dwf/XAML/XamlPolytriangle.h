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

#if !defined XAML_POLYTRI_HEADER
#define XAML_POLYTRI_HEADER

///
///\file        XAML/XamlPolytriangle.h
///

#include "XAML/XamlCore.h"

#include "whiptk/polytri.h"

/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing a polytriangle (a collection of contiguous triangles).
/** \image html Polytriangle.png "Polytriangle point ordering details"
 *
 */
class XAMLTK_API WT_XAML_Polytriangle : public WT_Polytriangle 
{
	friend class WT_XAML_Class_Factory;
protected:
/// Constructs a WT_XAML_Polytriangle object.
    WT_XAML_Polytriangle ()
		: WT_Polytriangle ()
    { }

    /// Constructs a WT_XAML_Polytriangle object from the given data.
    WT_XAML_Polytriangle(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        )
        : WT_Polytriangle (count, points, copy)
    { }

	///Copy Constructor
	WT_XAML_Polytriangle(WT_Polytriangle const& polytriangle)
		:WT_Polytriangle(polytriangle)
	{}
    /// Destroys a WT_XAML_Polytriangle object.
    virtual ~WT_XAML_Polytriangle()
    { }
public:
	//@{ \name WT_Object and WT_Drawable virtual methods
    virtual WT_Result   delay(WT_File & /*file*/) const
	{
		return WT_Result::Success;
	}
    virtual WT_Result   dump(WT_File & /*file*/) const
	{
		return WT_Result::Success;
	}
    WT_Result        serialize(WT_File & /*file*/) const;
    //@}

};
//@}

#endif //XAML_POLYTRI_HEADER
