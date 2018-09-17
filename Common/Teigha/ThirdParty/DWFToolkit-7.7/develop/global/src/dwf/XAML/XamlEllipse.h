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

#if !defined XAML_ELLIPSE_HEADER
#define XAML_ELLIPSE_HEADER

///
///\file        XAML/XamlEllipse.h
///

#include "XAML/XamlCore.h"

#include "whiptk/object.h"
#include "whiptk/ellipse.h"
#include "XAML/XamlXML.h"


/// A drawable describing a filled ellipse, or elliptical wedge.
/** \image html CircleStartEnd.png "Circular arc (major==minor) start/end angle details"
 *  \image html EllipseStartEnd.png "Elliptical arc (major!=minor) start/end angle details"
 */
class XAMLTK_API WT_XAML_Filled_Ellipse : public WT_Filled_Ellipse
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_File;
		
protected:

    /// Constructs a WT_XAML_Filled_Ellipse object.
    WT_XAML_Filled_Ellipse()
		:WT_Filled_Ellipse()
    { }

    /// Constructs a WT_XAML_Filled_Ellipse object with the given data.
    WT_XAML_Filled_Ellipse (
        WT_Integer32            x,  /**< Horizontal component of the center point of ellipse. */
        WT_Integer32            y,  /**< Vertical component of the center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : WT_Filled_Ellipse (x, y, major, minor, start, end, tilt)
    { }

    /// Constructs a WT_Filled_Ellipse object with the given data.
    WT_XAML_Filled_Ellipse(
        WT_Logical_Point const &  pos,  /**< Center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : WT_Filled_Ellipse (pos, major, minor, start, end, tilt)
    { }

	WT_XAML_Filled_Ellipse(const WT_Filled_Ellipse& ellipse)
		: WT_Filled_Ellipse(ellipse)
	{}

	virtual ~WT_XAML_Filled_Ellipse()
	{}
public:

	//@{ \name WT_Object virtual methods
    WT_Result        serialize(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    
};

/// A drawable describing an unfilled ellipse, or elliptal arc.
/** \image html CircleStartEnd.png "Circular arc (major==minor) start/end angle details"
 *  \image html EllipseStartEnd.png "Elliptical arc (major!=minor) start/end angle details"
 */
class XAMLTK_API WT_XAML_Outline_Ellipse : public WT_Outline_Ellipse
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_File;
	
protected:

    /// Constructs a WT_XAML_Outline_Ellipse object.
    WT_XAML_Outline_Ellipse ()
		:WT_Outline_Ellipse()
    { }

    /// Constructs a WT_XAML_Outline_Ellipse object with the given data.
    WT_XAML_Outline_Ellipse (
        WT_Integer32            x,  /**< Horizontal component of the center point of ellipse. */
        WT_Integer32            y,  /**< Vertical component of the center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : WT_Outline_Ellipse (x, y, major, minor, start, end, tilt)
    { }

    /// Constructs a WT_XAML_Outline_Ellipse object with the given data.
    WT_XAML_Outline_Ellipse(
        WT_Logical_Point const &  pos,  /**< Center point of ellipse. */
        WT_Integer32            major, /**< Major axis radius. */
        WT_Integer32            minor, /**< Minor axis radius. */
        WT_Unsigned_Integer16   start   = 0, /**< Optional start angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   end     = 0, /**< End angle (in 360/65,536ths of a degree.) */
        WT_Unsigned_Integer16   tilt    = 0 /**< Rotation angle (in 360/65,536ths of a degree.) */
        )
        : WT_Outline_Ellipse (pos, major, minor, start, end, tilt)
    { }

	WT_XAML_Outline_Ellipse(const WT_Outline_Ellipse& ellipse)
		: WT_Outline_Ellipse(ellipse)
	{}

	virtual ~WT_XAML_Outline_Ellipse()
	{}

public:
    virtual WT_Result   serialize(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
};

#endif //XAML_ELLIPSE_HEADER
