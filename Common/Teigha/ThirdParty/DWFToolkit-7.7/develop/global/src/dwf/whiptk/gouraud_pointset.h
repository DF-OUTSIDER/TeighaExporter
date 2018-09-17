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

#if !defined GOURAUD_POINTSET_HEADER
#define GOURAUD_POINTSET_HEADER

///
///\file        dwf/whiptk/gouraud_pointset.h
///

#include "whiptk/pointset.h"

class WT_File;

/// A base class for a set of WT_Logical_Point objects, each with an assigned WT_RGBA32 color.
class WHIPTK_API WT_Gouraud_Point_Set_Data : public WT_Point_Set_Data
{
	friend class WT_W2D_Class_Factory;
protected:
    WT_RGBA32 *                 m_colors; /**< \brief The set of colors (one for each point in the set.) */
    WT_Integer32                m_colors_allocated; /**< \brief The number of allocated points for the colors.*/

public:
    /// Creates a WT_Gouraud_Point_Set object.
    WT_Gouraud_Point_Set_Data()
        : WT_Point_Set_Data ()
        , m_colors (WD_Null)
        , m_colors_allocated(0)
    { }

    /// Creates a WT_Gouraud_Point_Set object from the given data.
    WT_Gouraud_Point_Set_Data(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_RGBA32 const *           colors, /**< Set of colors (one for each point.) */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);

    /// Destroys a WT_Gouraud_Point_Set object.
    virtual ~WT_Gouraud_Point_Set_Data();

    /// Returns the array of colors (one per point in the set.)
    WT_RGBA32 * colors() const
    {
        return m_colors;
    }

private:

    WT_Gouraud_Point_Set_Data (WT_Gouraud_Point_Set_Data const &)
      : WT_Point_Set_Data ()
      , m_colors ()
      , m_colors_allocated(0)
    {
        WD_Complain ("cannot copy WT_Gouraud_Point_Set_Data");
    } // prohibited

    WT_Gouraud_Point_Set_Data & operator= (WT_Gouraud_Point_Set_Data const &)
    {
        WD_Complain ("cannot assign WT_Gouraud_Point_Set_Data");
        return *this;
    } // prohibited
};

/// A base class for a set of WT_Logical_Point objects, each with an assigned WT_RGBA32 color.
class WHIPTK_API WT_Gouraud_Point_Set : public WT_Gouraud_Point_Set_Data
{
	friend class WT_W2D_Class_Factory;

protected:
    /// Materialization type.
    enum {
        Materialize,
        Skip
    }                           m_read_mode;

    /// Materialization stage.
    enum {
        Getting_Count,
        Getting_Point,
        Getting_Color,
        Getting_Close_Paren
    }                           m_stage;

public:

    /// Creates a WT_Gouraud_Point_Set object.
    WT_Gouraud_Point_Set()
        : WT_Gouraud_Point_Set_Data ()
        , m_read_mode (Materialize)
        , m_stage (Getting_Count)
    {
        m_points_materialized  = 0; //inherited members cannot go in member initialization above
    }

    /// Creates a WT_Gouraud_Point_Set object from the given data.
    WT_Gouraud_Point_Set(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_RGBA32 const *           colors, /**< Set of colors (one for each point.) */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);

    /// Destroys a WT_Gouraud_Point_Set object.
    virtual ~WT_Gouraud_Point_Set()
    { }
    
    /// Causes the serialization of the object to the file.
    /** If this is a WT_Drawable derived object, this method may cause the object to be
     *  delayed (in case a coincident like-object follows) and/or merged (in case a coincident
     *  like-object preceeded) so as to optimize the output.
     *
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result   serialize(WT_File &  file, /**< The file being written. */
                                  WT_String  opcode_ascii, /**< The opcode to use if writing ASCII. */
                                  WT_Byte    opcode_32bit, /**< The opcode to use for a point set utilizing WT_Logical_Point32 objects. */
                                  WT_Byte    opcode_16bit /**< The opcode to use for a point set utilizing WT_Logical_Point16 objects. */
                                  ) const;

    // WT_Point_Set virtual methods
    WT_Result    materialize(WT_File & file);
    WT_Result    materialize_ascii(WT_File & file);
    WT_Result    materialize_16_bit(WT_File & file);
    WT_Result    skip_operand(WT_File & file);
    WT_Result    skip_operand_16_bit(WT_File & file);
    WT_Result    skip_operand_ascii(WT_File & file);

private:
    WT_Result    read_pointset(WT_File & file);
    WT_Result    read_pointset_16_bit(WT_File & file);
    WT_Result    read_pointset_ascii(WT_File & file);

    WT_Gouraud_Point_Set (WT_Gouraud_Point_Set const &)
      : WT_Gouraud_Point_Set_Data ()
      , m_read_mode (Materialize)
      , m_stage (Getting_Count)
    {
        WD_Complain ("cannot copy WT_Gouraud_Point_Set");
    } // prohibited

    WT_Gouraud_Point_Set & operator= (WT_Gouraud_Point_Set const &)
    {
        WD_Complain ("cannot assign WT_Gouraud_Point_Set");
        return *this;
    } // prohibited
};


#endif // GOURAUD_POINTSET_HEADER
