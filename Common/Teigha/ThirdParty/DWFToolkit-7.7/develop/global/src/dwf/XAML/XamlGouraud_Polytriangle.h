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
#if !defined XAML_GOURAUD_POLYTRI_HEADER
#define XAML_GOURAUD_POLYTRI_HEADER

///
///\file        dwf/XAML/XamlGouraud_Polytriangle.h
///

#include "XAML/XamlCore.h"

#include "whiptk/gouraud_polytri.h"
#include "XAML/XamlPath.h"
#include "XAML/XamlXML.h"


/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing a gouraud-shaded polytriangle.
/** This is a collection of contiguous triangles, with each vertex assigned a WT_RGBA32 color.
 *  \image html Polytriangle.png "Polytriangle point ordering details"
 *
 */
class XAMLTK_API WT_XAML_Gouraud_Polytriangle : public WT_Gouraud_Polytriangle
{
	friend class WT_XAML_Class_Factory;
	friend class XamlPath;

private:
    enum
    {
        FirstPoint,
        SecondPoint,
        ThirdPoint
    } _eParseState;

protected:
    /// Constructs a WT_XAML_Gouraud_Polytriangle object.
    WT_XAML_Gouraud_Polytriangle () 
        : _eParseState(FirstPoint)
    { }

    /// Constructs a WT_XAML_Gouraud_Polytriangle object with the given data.
    WT_XAML_Gouraud_Polytriangle(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_RGBA32 const *           colors, /**< Set of colors (one for each point.) */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
        : WT_Gouraud_Polytriangle(count, points, colors, copy)
        , _eParseState(FirstPoint)
    { }

	///Copy Constructor
	WT_XAML_Gouraud_Polytriangle(WT_Gouraud_Polytriangle const& polytri) throw(WT_Result)
		: WT_Gouraud_Polytriangle(polytri)
        , _eParseState(FirstPoint)
	{}

    /// Destroys a WT_XAML_Gouraud_Polytriangle object.
    virtual ~WT_XAML_Gouraud_Polytriangle()
    { }

public:
    // Mandatory WT_Object virtual methods
    WT_Result        serialize(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    /// materialize a point into the point set
    WT_Result        materializePoint( const WT_Point2D& pt, const WT_RGBA32& color, WT_XAML_File &rFile );

};
//@}

#endif // XAML_GOURAUD_POLYTRI_HEADER
