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

#if !defined TRANSFORM_HEADER
#define TRANSFORM_HEADER

///
///\file        dwf/whiptk/transform.h
///

/// A simple transform providing translation, scaling, and simple 90, 180, 270 degree rotations.
/** \exception WT_Result::Toolkit_Usage_Error The rotation was not one of 0, 90, 180, 270.
 */
class WHIPTK_API WT_Transform
{
public:
    WT_Logical_Point    m_translate; /**< \brief Translation vector. */
    double              m_x_scale; /**< \brief X scale factor. */
    double              m_y_scale; /**< \brief Y scale factor. */
    OdInt32             m_rotation; /**< \brief Rotation (in degrees, one of 0, 90, 180, 270). */

    /// Constructs a WT_Transform object.
    WT_Transform()
        : m_translate(0,0)
        , m_x_scale(1.0)
        , m_y_scale(1.0)
        , m_rotation(0)
    { }

    /// Constructs a WT_Transform object with the given data.
    WT_Transform (
        WT_Logical_Point trans, /**< Translation vector. */
        double xval, /**< X scale factor. */
        double yval, /**< Y scale factor. */
        double rotation = 0.0 /**< Optional rotation (in degrees, one of 0, 90, 180, 270). */
        )
        : m_translate(trans)
        , m_x_scale(xval)
        , m_y_scale(yval)
    {
        set_rotation(rotation);
    }

	virtual ~WT_Transform()
	{}

    /// Sets the rotation value.
    /** \exception WT_Result::Toolkit_Usage_Error The rotation was not one of 0, 90, 180, 270.
     */
    void set_rotation(
        double rotation /**< Rotation (in degrees, one of 0, 90, 180, 270). */
        )
    {
        switch ((OdInt32)rotation)
        {
            case 0:
            case 90:
            case 180:
            case 270:
                m_rotation = (OdInt32)rotation;
                break;
            default:
                m_rotation = 0;
                throw WT_Result::Toolkit_Usage_Error;
        }
    }

    /// Returns the rotation value (in degrees, one of 0, 90, 180, 270).
    OdInt32 rotation() const
    {   return m_rotation;    }
};

#endif // TRANSFORM_HEADER
