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

#if !defined XAML_CONTOUR_SET_HEADER
#define XAML_CONTOUR_SET_HEADER

///
///\file        XAML/XamlContour_Set.h
///

#include "XAML/XamlCore.h"

#include "whiptk/contour_set.h"

/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing a set of contours (containing positive and negative filled regions.)
/** Clockwise-wound contours indicate positive (filled) spaces.
 *  Counterclockwise-wound contours indicate negative spaces (holes).
 *  \image html ContourSet.png "Contour set positive/negative windings details"
 *
 */
class XAMLTK_API WT_XAML_Contour_Set : public WT_Contour_Set 
{
	friend class WT_XAML_Class_Factory;
protected:

    /// Constructs a WT_XAML_Contour_Set object.
    WT_XAML_Contour_Set ()
        : WT_Contour_Set()        
    { }

    /// Constructs a WT_XAML_Contour_Set object with the given data.
    WT_XAML_Contour_Set(
        WT_File &                   file, /**< The current file (used for its incarnation value.) */
        WT_Integer32                contours, /**< The number of contours in the set (size of the \a counts array.) */
        WT_Integer32 const *        counts, /**< The array of counts (each array item indicates the number of points in the respective contour.) */
        WT_Integer32                point_count, /**< The total number of points in the contour set (should equal the sum of the counts.) */
        WT_Logical_Point const *    points, /**< The array of points used by the contours. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
		:WT_Contour_Set(file,contours,counts,point_count,points,copy)
	{}

    /// Constructs a WT_XAML_Contour_Set object with the given data for one countour (i.e. a polygon).  Copies the points locally.
    WT_XAML_Contour_Set(
        WT_File &                   file, /**< The current file (used for its incarnation value.) */
        WT_Integer32                point_count, /**< The total number of points in the contour set (should equal the sum of the counts.) */
        WT_Logical_Point const *    points /**< The array of points used by the contours. */
        ) throw(WT_Result)
		:WT_Contour_Set(file,point_count,points)
	{}

    /// Constructs a WT_XAML_Contour_Set object with the given data.  Can be used as a copy constructor.
    WT_XAML_Contour_Set(
        WT_Contour_Set const &  source, /**< The contour set from which to copy. */
        WT_Boolean              copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
		:WT_Contour_Set(source,copy)
	{}

    /// Virtual abstract copy.  Copies a WT_Contour_Set object. 
    virtual WT_Contour_Set* copy(
        WT_Boolean              copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
    )
    {
        return new WT_XAML_Contour_Set( *this, copy );
    }

    /// Destroys a WT_XAML_Contour_Set object.
    virtual ~WT_XAML_Contour_Set()
	{}
public:

	 /// Assignment operator.  Assigns the given list contents to this one.
    WT_XAML_Contour_Set const & operator= (WT_XAML_Contour_Set const & source)
	{
		WT_Contour_Set::operator = (source);
		return *this;
	}

	//@{ \name WT_Object and WT_Drawable virtual methods
	WT_Result        serialize(WT_File & /*file*/) const;
    /// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
};
//@}

#endif //XAML_CONTOUR_SET_HEADER
