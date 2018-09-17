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

#if !defined UNITS_HEADER
#define UNITS_HEADER

///
///\file        dwf/whiptk/units.h
///

#include "whiptk/whipcore.h"
#include "whiptk/matrix.h"
#include "whiptk/object.h"

class WT_File;

/** \addtogroup groupSupportObjects
 *  @{
 */
/// Stores the transform between source (authoring tool) coordinate space and WHIP! drawing coordinate space.
class WHIPTK_API WT_Units : public WT_Object
{
	friend class WT_Drawing_Info;
	friend class WT_W2D_Class_Factory;
	friend class WT_Opcode;

private:
    WT_Matrix      m_transform;
    WT_Matrix      m_adjoint;
    WT_String           m_units;
    WT_Boolean          m_adjoint_valid;

    enum
    {
        Eating_Initial_Whitespace,
        Getting_Units_String,
        Getting_Transform,
        Getting_Close_Paren
    }                   m_stage;

public:

    //@{ \name Construction
    /// Constructs a WT_Units object.
    WT_Units()
        : m_adjoint_valid(WD_False)
        , m_stage(Eating_Initial_Whitespace)
    { }

    /// Constructs a WT_Units object with the given data.
    WT_Units(
        WT_Matrix const & xform, /**< Transform from source (authoring tool) WHIP! coordinates. */
        WT_String units /**< A string describing the source unit of measure. */
        )
        : m_transform(xform)
        , m_units(units)
        , m_adjoint_valid(WD_False)
        , m_stage(Eating_Initial_Whitespace)
    { }

	///Copy Constructor
	WT_Units(WT_Units const& units)
	{
		*this = units;
	}

	virtual ~WT_Units()
	{}
public:
    //@{ \name Data access methods
    /// Returns a transform which converts from source (authoring tool) coordinates to WHIP! coordinates.
    WT_Matrix const &  application_to_dwf_transform() const    {   return m_transform;    }
    /// Returns a transform which converts from WHIP! coordinates to source (authoring tool) coordinates.
    WT_Matrix const &  dwf_to_application_adjoint_transform();
    /// Sets the transform which converts from source (authoring tool) coordinates to WHIP! coordinates.
    void               set_application_to_dwf_transform(WT_Matrix const & xform);
    /// Sets the string describing the source unit of measure.
    /** Although optional, if applicable the units description should follow convention and be one of the
     *  following (non-localized) strings:
     *  \li \c millimeters
     *  \li \c centimeters
     *  \li \c meters
     *  \li \c kilometers
     *  \li \c inches
     *  \li \c feet
     *  \li \c feet \c and \c inches
     *  \li \c yards
     *  \li \c miles
     */
    void               set_units(WT_String const & units)      {   m_units = units;       }
    /// Sets the string describing the source unit of measure from the given string buffer.
    /** Although optional, if applicable the units description should follow convention and be one of the
     *  following (non-localized) strings:
     *  \li \c millimeters
     *  \li \c centimeters
     *  \li \c meters
     *  \li \c kilometers
     *  \li \c inches
     *  \li \c feet
     *  \li \c feet \c and \c inches
     *  \li \c yards
     *  \li \c miles
     */
    void               set_units(int length, WT_Unsigned_Integer16 const * units);
    /// Returns the string describing the source unit of measure.
    WT_String const &  units() const                           {   return m_units;        }
    //@}

    //@{ \name Utility methods
    /// Transforms the given point from WHIP! coordinates to source (authoring tool) coordinates.
    WT_Point3D         transform(WT_Logical_Point const & in_pt);
    /// Transforms the given point from source (authoring tool) coordinates to WHIP! coordinates.
    WT_Logical_Point   transform(WT_Point3D const & in_pt) const;
    /// Transforms the given point from WHIP! coordinates to source (authoring tool) coordinates.
    WT_Point3D         transform_from_DWF_to_application(WT_Point3D const & in_pt);
    /// Transforms the given point from source (authoring tool) coordinates to WHIP! coordinates.
    WT_Point3D         transform_from_application_to_DWF(WT_Point3D const & in_pt) const;
    //@}

    /// Returns WD_True if the unit objects are equal.
    WT_Boolean                      operator==(WT_Units const &) const;
    /// Returns WD_True if the unit objects are NOT equal.
    WT_Boolean                      operator!=(WT_Units const & u) const    {   return !((*this)==u);  }

    //@{ \name WT_Object virtual methods
    WT_Result          materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Object::WT_ID   object_id() const;
    WT_Object::WT_Type object_type() const;
    WT_Result          process(WT_File & file);
    WT_Result          serialize(WT_File & file) const;
    WT_Result          skip_operand(WT_Opcode const & opcode, WT_File & file);
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Units & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};
/** @}
 */
#endif // UNITS_HEADER
