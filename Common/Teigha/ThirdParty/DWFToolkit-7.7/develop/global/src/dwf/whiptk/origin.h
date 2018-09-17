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
///\file        dwf/whiptk/origin.h
///

#if !defined ORIGIN_HEADER
#define ORIGIN_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_File;

/** \addtogroup groupSupportObjects Support objects
 *
 * @{
 */

/// An absolute point in drawing coordinates used to reset the relative coordinate processing.
/** This is typically added to the WHIP! data automatically as needed by the framework
 *  during serialization. */
class WHIPTK_API WT_Origin : public WT_Object
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Point_Set;
	friend class WT_Opcode;
	friend class WT_Gouraud_Point_Set;
	friend class WT_Contour_Set;

private:
    WT_Logical_Point        m_origin;

protected:

    //@{ \name Construction
    /// Constructs a WT_Origin object.
    WT_Origin()
        : m_origin(0, 0)
    { }
    /// Constructs a WT_Origin object with the given point.
    WT_Origin(WT_Logical_Point const & point)
        : m_origin(point)
    { }
	virtual ~WT_Origin()
	{}
    //@}
public:
    //@{ \name Data access methods
    WT_Logical_Point const &    origin() const
    {    return m_origin;    }
    //@}

    //@{ \name WT_Object virtual methods
    WT_ID            object_id() const;
    WT_Type          object_type() const;
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    WT_Result        process(WT_File & file);
    WT_Result        serialize (WT_File & file) const { return serialize(file, WD_True); }
    /// Causes the serialization of the object to the file.
    /** \warning This is used by drawables which manually update the file object's current point.
     *  Client code should not use this serialization method.
     *  \retval WT_Result::Success The operation was successful.
     */
    WT_Result        serialize (WT_File & file, WT_Boolean update_current_point) const;
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    //@}

    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Origin & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );
};

//@}

#endif // ORIGIN_HEADER
