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

#if !defined GOURAUD_POLYLINE_HEADER
#define GOURAUD_POLYLINE_HEADER

///
///\file        dwf/whiptk/gouraud_polyline.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/drawable.h"
#include "whiptk/gouraud_pointset.h"

/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing a gouraud-shaded polyline
/** This is a collection of contiguous line segments, with each vertex assigned a WT_RGBA32 color.
 *
 */
class WHIPTK_API WT_Gouraud_Polyline : public WT_Drawable, public WT_Gouraud_Point_Set
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

public:

    /// Constructs a WT_Gouraud_Polyline object.
    WT_Gouraud_Polyline ()
    {}

    /// Constructs a WT_Gouraud_Polyline object with the given data.
    WT_Gouraud_Polyline(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_RGBA32 const *           colors, /**< Set of colors (one for each point.) */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
        : WT_Gouraud_Point_Set(count, points, colors, copy)
    { }

	///Copy Constructor
	WT_Gouraud_Polyline(WT_Gouraud_Polyline const& pline) throw(WT_Result)
	{
		set(pline.count(),pline.points(),WD_True);
		m_colors = new WT_RGBA32[pline.count()];
        if (m_colors == NULL)
            throw WT_Result::Out_Of_Memory_Error;
        
        // set the allocated count.
        m_colors_allocated = pline.count();
        WD_COPY_MEMORY((void *)pline.colors(), pline.count() * sizeof(WT_RGBA32), m_colors);
	}

    /// Destroys a WT_Gouraud_Polyline object.
    virtual ~WT_Gouraud_Polyline()
    { }
public:
    // WT_Object virtual methods
    WT_ID            object_id() const;
    WT_Result        serialize(WT_File & file) const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    virtual void     update_bounds(WT_File * file);

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Gouraud_Polyline & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // GOURAUD_POLYLINE_HEADER
