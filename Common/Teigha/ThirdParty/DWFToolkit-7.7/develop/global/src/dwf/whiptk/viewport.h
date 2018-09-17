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

#if !defined VIEWPORT_HEADER
#define VIEWPORT_HEADER

///
///\file        dwf/whiptk/viewport.h
///

#include "whiptk/attribute.h"
#include "whiptk/pointset.h"
#include "whiptk/viewport_options.h"

class WT_Contour_Set;
class WT_File;

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute defining the current viewport receiving the subsequent graphics.
/** \sa WT_Rendition
 *
 */
class WHIPTK_API WT_Viewport   : public WT_Attribute
{
	friend class WT_Rendition;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

protected:

    enum WT_Materialize_Stage
    {
        Eating_Initial_Whitespace,
        Checking_For_Attribute_End,
        Getting_Viewport_Name,
        Getting_Viewport_Polygon,
        Materialize_Contour_Set,
        Getting_Viewport_Optioncode,
        Materializing_Viewport_Option,
        Eating_End_Whitespace
    } m_stage;

    WT_Contour_Set *        m_contour_set;
    WT_Point_Set *          m_temp_point_set;
    WT_Opcode *             m_temp_opcode;
    WT_Unsigned_Integer16   m_fields_defined;
    WT_Viewport_Optioncode  m_optioncode;
    WT_String               m_name;
    int                     m_incarnation;

    WT_Viewport_Option_Viewport_Units  m_viewport_units;

    enum WT_Viewport_Options
    {
        VIEWPORT_UNITS_BIT = 0x0001
    };

public:

    //@{ \name Construction / destruction
    /// Constructs a WT_Viewport object.
    WT_Viewport()
        : m_stage(Eating_Initial_Whitespace)
        , m_contour_set(WD_Null)
        , m_temp_point_set(WD_Null)
        , m_temp_opcode(WD_Null)
        , m_fields_defined(0)
        , m_incarnation(-1)
    {}
    /// Constructs a WT_Viewport object. Copy constructor.
    WT_Viewport( WT_Viewport const & vport) throw(WT_Result);
    /// Constructs a WT_Viewport object with the given data.
    WT_Viewport(
        WT_File &                   file, /**< The file being written. */
        char const *                name, /**< The viewport name. */
        int                         num_points, /**< The number of points in the viewport. */
        WT_Logical_Point const *    points, /**< An array of points defining the viewport. */
        WT_Boolean                  copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);
    /// Constructs a WT_Viewport object with the given data.
    WT_Viewport(
        WT_File &                   file, /**< The file being written. */
        WT_Unsigned_Integer16 const * name, /**< The viewport name. */
        int                         num_points, /**< The number of points in the viewport. */
        WT_Logical_Point const *    points, /**< An array of points defining the viewport. */
        WT_Boolean                  copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);
    /// Constructs a WT_Viewport object with the given data.
    WT_Viewport(
        WT_File &                   file, /**< The file being written. */
        WT_String const &           name, /**< The viewport name. */
        WT_Contour_Set const &      boundary, /**< A contour set defining the viewport. */
        WT_Boolean                  copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);
    /// Destroys a WT_Viewport object.
    virtual ~WT_Viewport();

    //@}
public:
    //@{ \name Data access members
    /// Returns the viewport boundary (as a contour set.)
    WT_Contour_Set const * contour() const {   return m_contour_set;   }
    /// Returns the viewport name
    WT_String const & name() const {   return m_name;   }
    /// Copies the settings from the given viewport to this one.
    WT_Result set(
        WT_Viewport const & vport, /**< Source viewport. */
        WT_Boolean copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        );

    /// Sets the given data.
    void set(
        WT_String const & name /**< the viewport name. */
        );

    /// Sets the given data.
    WT_Result set(
        WT_Contour_Set const * boundary, /**< A contour set defining the viewport. */
        WT_Boolean copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        );

    /// Returns a read-only reference to the viewport units option object.
    WT_Viewport_Option_Viewport_Units const &    viewport_units()    const { return m_viewport_units; }
    /// Returns a writable reference to the viewport units option object.
    WT_Viewport_Option_Viewport_Units &          viewport_units() { m_fields_defined |= VIEWPORT_UNITS_BIT; return m_viewport_units; }
    /// Assignment operator.  Copies the settings from the given viewport to this one.
    virtual WT_Viewport &  operator=(WT_Viewport const & vport);
    //@}

    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_ID              object_id() const;
    virtual WT_Result  materialize(WT_Opcode const & opcode, WT_File & file);
    virtual WT_Result  process(WT_File & file);
    virtual WT_Result  skip_operand(WT_Opcode const & opcode, WT_File & file);
    virtual WT_Result  serialize (WT_File & file) const;
    virtual WT_Result  sync (WT_File & file) const;
    virtual WT_Boolean operator== (WT_Attribute const & attrib) const;
    //@}

    /// Provides a default action handler for this attribute (e.g. sets the current rendition's corresponding attribute.)
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Viewport & item, /**< The attribute to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // VIEWPORT_HEADER
