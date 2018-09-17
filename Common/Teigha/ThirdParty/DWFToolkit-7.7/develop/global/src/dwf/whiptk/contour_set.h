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

#if !defined CONTOUR_SET_HEADER
#define CONTOUR_SET_HEADER

///
///\file        dwf/whiptk/contour_set.h
///

#include "whiptk/whipcore.h"
#include "whiptk/file.h"
#include "whiptk/drawable.h"
#include "whiptk/pointset.h"

/** \addtogroup groupDrawable
 *  @{
 */

/// A drawable describing a set of contours (containing positive and negative filled regions.)
/** Clockwise-wound contours indicate positive (filled) spaces.
 *  Counterclockwise-wound contours indicate negative spaces (holes).
 *  \image html ContourSet.png "Contour set positive/negative windings details"
 *
 */
class WHIPTK_API WT_Contour_Set : public WT_Drawable
{
	friend class WT_W2D_Class_Factory;
	friend class WT_Viewport;
	friend class WT_Opcode;

protected:

    WT_Unsigned_Integer32   m_incarnation; /**< \brief Rendition incarnation. */
    WT_Integer32            m_num_contours; /**< \brief The number of contours in the set (size of counts_list.) */
    WT_Integer32 *          m_counts_list; /**< \brief The array of counts (each array item indicates the number of points in the respective contour.) */
    WT_Integer32            m_total_point_count; /**< \brief The total number of points in the contour set. */
    WT_Boolean              m_local_copy; /**< \brief WD_True if the points in the set were allocated locally (instead of being referenced directly from an external data pool.) */
    WT_Boolean              m_local_copy_counts; /**< \brief WD_True if the m_counts_list in the set were allocated locally (instead of being referenced directly from an external data pool.) */
    WT_Logical_Point *      m_points; /**< \brief The array of points used by the contours. */
    WT_Boolean              m_transformed; /**< \brief WD_True if the points have been transformed. */
    WT_Boolean              m_relativized; /**< \brief WD_True if the points have been relativized. */

    /// Materialization stage.
    enum WT_Materialize_Stage
    {
        Getting_Num_Contours,
        Getting_Count_List,
        Getting_Points,
        Skipping_Close_Paren
    }                       m_stage;

    WT_Integer32            m_stage_item_counter; /**< \brief Used during materialization as a contour counter. */


public:

    /// Constructs a WT_Contour_Set object.
    WT_Contour_Set ()
        : m_incarnation(0xFFFFFFFFU)
        , m_num_contours(0)
        , m_counts_list (WD_Null)
        , m_total_point_count(0)
        , m_local_copy(WD_False)
        , m_local_copy_counts(WD_False)
        , m_points(WD_Null)
        , m_transformed(WD_False)
        , m_relativized(WD_False)
        , m_stage(Getting_Num_Contours)
        , m_stage_item_counter(0)
    { }

    /// Constructs a WT_Contour_Set object with the given data.
    WT_Contour_Set(
        WT_File &                   file, /**< The current file (used for its incarnation value.) */
        WT_Integer32                contours, /**< The number of contours in the set (size of the \a counts array.) */
        WT_Integer32 const *        counts, /**< The array of counts (each array item indicates the number of points in the respective contour.) */
        WT_Integer32                point_count, /**< The total number of points in the contour set (should equal the sum of the counts.) */
        WT_Logical_Point const *    points, /**< The array of points used by the contours. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);

    /// Constructs a WT_Contour_Set object with the given data for one countour (i.e. a polygon).  Copies the points locally.
    WT_Contour_Set(
        WT_File &                   file, /**< The current file (used for its incarnation value.) */
        WT_Integer32                point_count, /**< The total number of points in the contour set (should equal the sum of the counts.) */
        WT_Logical_Point const *    points /**< The array of points used by the contours. */
        ) throw(WT_Result);

    /// Constructs a WT_Contour_Set object with the given data.  Can be used as a copy constructor.
    WT_Contour_Set(
        WT_Contour_Set const &  source, /**< The contour set from which to copy. */
        WT_Boolean              copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);

    /// Destroys a WT_Contour_Set object.
    virtual ~WT_Contour_Set();

    /// Virtual abstract copy.  Copies a WT_Contour_Set object. 
    virtual WT_Contour_Set* copy(
        WT_Boolean              copy = WD_True /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) const;

public:
    //@{ \name Data access methods
    /// Returns the number of contours in the set.
    WT_Integer32 const&        contours() const    {   return m_num_contours;  }
    WT_Integer32&              contours()          {   return m_num_contours;  }
    /// The array of counts (each array item indicates the number of points in the respective contour.)
    WT_Integer32 *              counts()   const    {   return m_counts_list;   }
    /// The array of points used by the contours.
    WT_Logical_Point const *    points()   const    {   return m_points;        }
    /// The total number of points in the contour set.
    WT_Integer32 const&         total_points() const{   return m_total_point_count; }
    WT_Integer32&               total_points()      {   return m_total_point_count; }
    //@}

    //@{ \name Manipulation methods
    /// Returns the points to absolute values based on their relative position to last materialized point.
    virtual WT_Result             de_relativize(WT_File & file);
    /// Relativizes the points in the set to the last serialized point.
    virtual WT_Result             relativize(WT_File & file);
    /// Replaces the contents of the contour set with the given data.
    WT_Result set(
        WT_Unsigned_Integer32       incarnation, /**< Rendition incarnation. */
        WT_Integer32                contours, /**< The number of contours in the set (size of the \a counts array.) */
        WT_Integer32 const *        counts, /**< The array of counts (each array item indicates the number of points in the respective contour.) */
        WT_Integer32                point_count, /**< The total number of points in the contour set (should equal the sum of the counts.) */
        WT_Logical_Point const *    points, /**< The array of points used by the contours. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        );
    /// Applies the given transform to the point set.
    WT_Result             transform(WT_Transform const & transform);
    /// Assignment operator.  Assigns the given list contents to this one.
    WT_Contour_Set const & operator=(WT_Contour_Set const & source) throw(WT_Result);
    //@}

    //@{ \name Status methods
    /// Returns WD_True if the first point in the set can be expressed using a WT_Logical_Point16 object.
    WT_Boolean       first_point_fits_in_16_bits() const;
    /// Returns WD_True if the remaining points in the set (beyond the first) can be expressed using a WT_Logical_Point16 object.
    WT_Boolean       remaining_points_fit_in_16_bits() const;
    /// Equality operator.  Returns WD_True if the given list's contents is equal to this one.
    WT_Boolean  operator==(WT_Contour_Set const & other) const;
    //@}

    //@{ \name WT_Object and WT_Drawable virtual methods
    WT_Result        materialize(WT_Opcode const & opcode, WT_File & file);
    /// Materializes the point set from the file as a set of WT_Logical_Point16 objects.
    /** Reads in the object data and causes the file reading to proceed to the end of this object.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Out_Of_Memory_Error Could not allocate the points.
     */
    WT_Result        materialize_16_bit(WT_File & file);
    /// Materializes the point set from the file as a set of WT_Logical_Point32 objects.
    /** Reads in the object data and causes the file reading to proceed to the end of this object.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Out_Of_Memory_Error Could not allocate the points.
     */
    WT_Result        materialize_32_bit(WT_File & file);
    WT_ID            object_id() const;
    WT_Result        process(WT_File & file);
    WT_Result        serialize(WT_File & file) const
    {   return serialize(file, WD_False);   }
    /// Causes the serialization of the object to the file.
    /** If this is a WT_Drawable derived object, this method may cause the object to be
     *  delayed (in case a coincident like-object follows) and/or merged (in case a coincident
     *  like-object preceeded) so as to optimize the output.
     *
     *  \retval WT_Result::Success The operation was successful.
     */
    WT_Result        serialize(
        WT_File & file, /**< The file being written. */
        WT_Boolean embeded_in_another_opcode /**< WD_True if this object is used as a sub-object of another object. */
        ) const;
    /// Causes the file reading to proceed to the end of this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    WT_Result        skip_operand(WT_File & file);
    WT_Result        skip_operand(WT_Opcode const & opcode, WT_File & file);
    /// Causes the file reading to proceed to the end of this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    WT_Result        skip_operand_16_bit(WT_File & file);
    virtual void     update_bounds(WT_File * file);
    //@}


    /// Provides a default action handler for this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    static WT_Result default_process(
        WT_Contour_Set & item, /**< The object to process. */
        WT_File & file /**< The file being read. */
        );

};

//@}

#endif // CONTOUR_SET_HEADER
