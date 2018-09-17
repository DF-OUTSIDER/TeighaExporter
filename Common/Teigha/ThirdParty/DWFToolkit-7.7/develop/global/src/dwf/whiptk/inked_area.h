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

#if !defined INKED_AREA_HEADER
#define INKED_AREA_HEADER

///
///\file        dwf/whiptk/inked_area.h
///

/** \addtogroup groupRenderingAttrobutes
 *  @{
 */

// ====================================================================================
/// Describes the drawing extents of the WHIP! data.
/** \deprecated This information is now contained in the DWFEPlot::EPlotGraphicResource "extents" rectangle.
 *   This remains in the toolkit only for backward compatability with previous versions of %DWF.
 *  \sa WT_Rendering_Options
 */
class WHIPTK_API WT_Inked_Area : public WT_Attribute
{
private:
    WT_Logical_Point *       m_bounds;
    int                      m_materialization_counter;

    enum
    {
        Getting_Operand,
        Getting_Bounds,
        Getting_Close_Paren,
        Completed
    } m_stage;

public:

    //@{ \name Contstruction / destruction
    /// Constructs a WT_Inked_Area object.
    WT_Inked_Area()
        : m_bounds (WD_Null), m_stage(Getting_Operand)
    {}
    /// Constructs a WT_Inked_Area object with the given bounds (lower-left, lower-right, upper-right, upper-left.)
    WT_Inked_Area(WT_Logical_Point const * pos) throw(WT_Result);
    /// Constructs a WT_Inked_Area object. Copy constructor
    WT_Inked_Area(WT_Inked_Area const & bounds) throw(WT_Result);
    /// Destroys a WT_Inked_Area object.
    virtual ~WT_Inked_Area();
    //@}

    //@{ \name Data access methods
    /// Returns the array of points representing the bounds (lower-left, lower-right, upper-right, upper-left.)
    WT_Logical_Point const * bounds() const  { return m_bounds; }
    /// Returns the bounding box points to absolute values based on their relative position to last materialized point.
    void        de_relativize(WT_File & file);
    /// Alias for bounds() - Returns the array of points representing the bounds (lower-left, lower-right, upper-right, upper-left.)
    WT_Logical_Point const * get_inked_area() const;
    /// Relativizes the bounding box points to the last serialized point.
    void        relativize(WT_File & file);
    /// Sets the array of points representing the bounds (lower-left, lower-right, upper-right, upper-left.)
    WT_Result set(WT_Logical_Point const * bounds);
    /// Alias for set() - Sets the array of points representing the bounds (lower-left, lower-right, upper-right, upper-left.)
    WT_Result set_inked_area(WT_Logical_Point const * bounds);
    /// Applies the given transform to the bounding box points.
    void        transform(WT_Transform const & transform);
    /// Assignment operator.  Assigns the given inked area settings to this one.
    virtual WT_Inked_Area const &   operator=(WT_Inked_Area const & inked_area) throw(WT_Result);
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID            object_id() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    WT_Result        serialize (WT_File & file) const;
    WT_Result        sync (WT_File & file) const;
    WT_Boolean       operator== (WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Inked_Area & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // INKED_AREA_HEADER
