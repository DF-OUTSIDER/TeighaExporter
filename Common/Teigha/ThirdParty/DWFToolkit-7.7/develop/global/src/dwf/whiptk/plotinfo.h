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

///
///\file        dwf/whiptk/plotinfo.h
///

#if !defined PLOTINFO_HEADER
#define PLOTINFO_HEADER

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// Contains information relating the WHIP! drawing coordinate space to plotting (paper) space.
/** \deprecated The information contained in the WT_Plot_Info object should be specified instead
 *  in the %DWF 06.00 (and greater) DWFEPlot::EPlotPaper and DWFEPlot::EPlotGraphicResource objects.
 *  This remains in the toolkit only for backward compatability with previous %DWF versions.
 *
 *  \note The serialization of this opcode should be deferred until just prior to the first drawable.
 */
class WHIPTK_API WT_Plot_Info
: public WT_Object
{
public:
    /// Indicates the units in which the paper (or virtual paper) is described.
    enum WT_Paper_Units
    {
        Millimeters,
        Inches,

        // Not units
        Count, /**< Not a unit.  Should be the number of unit types + 1. */
        Undefined = -1 /**< Not a unit. */
    };

protected:
    double          m_height;
    double          m_width;
    WT_Point2D      m_ll;   // lower left
    WT_Point2D      m_ur;   // upper right
    WT_Matrix2D m_dwf2paper;
    WT_Paper_Units  m_units;
    WT_Boolean      m_show;
    WT_String       m_temp_string;
    WT_Integer32    m_rotation;

    enum WT_Materialize_Stage
    {   Eating_Initial_Whitespace,
        Getting_Show_Boolean,
        Getting_Landscape_To_Portrait_Flip,
        Getting_Units_String,
        Getting_Paper_Width,
        Getting_Paper_Height,
        Getting_Plot_Area_Left,
        Getting_Plot_Area_Bottom,
        Getting_Plot_Area_Right,
        Getting_Plot_Area_Top,
        Getting_To_Paper_Transform,
        Eating_End_Whitespace
    } m_stage;

public:

    //@{ \name Construction
    /// Constructs a WT_Plot_Info object.
    WT_Plot_Info()
    : m_height(11.0)
    , m_width(8.5)
    , m_ll(0.25,0.25)
    , m_ur(8.25,10.75)
    , m_units(Inches)
    , m_show(WD_False)
    , m_rotation(0)
    , m_stage(Eating_Initial_Whitespace)
    {}
    //@}

    virtual ~WT_Plot_Info()
    {}

    //@{ \name Data acces methods
    /// Returns a writable accessor to the flag specifying if the paper graphic should be shown by a viewing application.
    inline WT_Boolean&           for_display()     { return m_show; }
    /// Returns WD_True if the paper graphic should be shown by a viewing application.
    inline const WT_Boolean&     for_display()     const { return m_show; }
    /// Returns a writable accessor to the vector, in in paper units, from the lower-left corner of the paper to the lower-left boundary of the printable area.
    inline WT_Point2D&           lower_left()      { return m_ll; }
    /// Returns a vector, in in paper units, from the lower-left corner of the paper to the lower-left boundary of the printable area.
    inline const WT_Point2D&     lower_left()      const { return m_ll; }
    /// Returns a writable accessor to the paper height (in paper units.)
    inline double&               paper_height()        { return m_height; }
    /// Returns the paper height (in paper units.)
    inline const double&         paper_height()        const { return m_height; }
    /// Returns a writable accessor to the paper units (one of \ref WT_Paper_Units "enum WT_Paper_Units".)
    inline WT_Paper_Units&       paper_units() { return m_units; }
    /// Returns the paper units (one of \ref WT_Paper_Units "enum WT_Paper_Units".)
    inline const WT_Paper_Units& paper_units() const { return m_units; }
    /// Returns a writable accessor to the paper width (in paper units.)
    inline double&               paper_width()         { return m_width; }
    /// Returns the paper width (in paper units.)
    inline const double&         paper_width()         const { return m_width; }
    /// Returns a writable accessor to the rotation angle (in degrees, one of 0, 90, 180, 270.)
    inline const WT_Integer32&           rotation()              const { return m_rotation; }
    /// Returns a the pre-applied rotation angle (in degrees, one of 0, 90, 180, 270, typically portrait==0 or landscape==90.)
    inline WT_Integer32&                 rotation()        { return m_rotation; }
    /// Returns a writable accessor to the 3x3 matrix which facilitates transforming DWF coordinate space to paper coordinate space.
    inline WT_Matrix2D&          to_paper()       { return m_dwf2paper; }
    /// Returns the 3x3 matrix which facilitates transforming DWF coordinate space to paper coordinate space.
    inline const WT_Matrix2D&    to_paper()       const { return m_dwf2paper; }
    /// Returns a writable accessor to the vector, in in paper units, from the upper-right corner of the paper to the upper-right boundary of the printable area.
    inline WT_Point2D&           upper_right()     { return m_ur; }
    /// Returns the vector, in in paper units, from the upper-right corner of the paper to the upper-right boundary of the printable area.
    inline const WT_Point2D&     upper_right()     const { return m_ur; }    //@}

    //@{ \name WT_Object virtrual methods
    WT_Result materialize( const WT_Opcode& opcode, WT_File& file);
    WT_ID     object_id()   const { return WT_Object::Plot_Info_ID; }
    WT_Type   object_type() const { return WT_Object::Definition; }
    WT_Result process( WT_File& file);
    virtual WT_Result serialize( WT_File& file ) const;
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Plot_Info  & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // PLOTINFO_HEADER
