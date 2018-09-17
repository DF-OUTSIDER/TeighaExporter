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

#if !defined XAML_GOURAUD_POLYLINE_HEADER
#define XAML_GOURAUD_POLYLINE_HEADER

///
///\file        XAML/XamlGouraud_Polyline.h
///

#include "XAML/XamlCore.h"

#include "whiptk/gouraud_polyline.h"
#include "XAML/XamlPath.h"
#include "XAML/XamlXML.h"

class XamlPolylineSegment;

/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing a gouraud-shaded polyline
/** This is a collection of contiguous line segments, with each vertex assigned a WT_RGBA32 color.
 *
 */
class XAMLTK_API WT_XAML_Gouraud_Polyline 
    : public WT_Gouraud_Polyline
    , public XamlDrawableAttributes::PathAttributeProvider
    , public XamlDrawableAttributes::StrokeDashArray::Provider
    , public XamlDrawableAttributes::StrokeDashOffset::Provider
    , public XamlDrawableAttributes::StrokeThickness::Provider
{
	friend class WT_XAML_Class_Factory;
    friend class XamlPath;

protected:

    /// Constructs a WT_XAML_Gouraud_Polyline object.
    WT_XAML_Gouraud_Polyline ()
    {}

    /// Constructs a WT_XAML_Gouraud_Polyline object with the given data.
    WT_XAML_Gouraud_Polyline(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_RGBA32 const *           colors, /**< Set of colors (one for each point.) */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
        : WT_Gouraud_Polyline(count, points, colors, copy)
    { }

	///Copy Constructor
	WT_XAML_Gouraud_Polyline(WT_Gouraud_Polyline const& pline) throw(WT_Result)
		:WT_Gouraud_Polyline(pline)
	{}
    /// Destroys a WT_XAML_Gouraud_Polyline object.
    virtual ~WT_XAML_Gouraud_Polyline()
    { }

public:
    // WT_Object virtual methods
    WT_Result        serialize(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    WT_Result        materializeSegment( const XamlPolylineSegment& rSegment, const WT_RGBA32& rStartColor, const WT_RGBA32& rEndColor, WT_XAML_File& rFile );

protected:
    virtual WT_Result providePathAttributes( XamlDrawableAttributes::PathAttributeConsumer* p, WT_XAML_File& r) const;
    virtual WT_Result provideStrokeDashArray( XamlDrawableAttributes::StrokeDashArray*& );
    virtual WT_Result provideStrokeDashOffset( XamlDrawableAttributes::StrokeDashOffset*& );
    virtual WT_Result provideStrokeThickness( XamlDrawableAttributes::StrokeThickness*& );
};

//@}

#endif // XAML_GOURAUD_POLYLINE_HEADER
