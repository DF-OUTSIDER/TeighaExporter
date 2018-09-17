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

#if !defined POINTSET_HEADER
#define POINTSET_HEADER

///
///\file        dwf/whiptk/pointset.h
///

#include "whiptk/whipcore.h"

class WT_File;

/// A base class for a set of WT_Logical_Point objects.
class WHIPTK_API WT_Point_Set_Data
{
protected:
    WT_Integer32                m_count;  /**< \brief The number of points in the set.*/
    WT_Integer32                m_allocated; /**< \brief The number of allocated points.*/
    WT_Logical_Point *          m_points; /**< \brief The arrray of points.*/
    WT_Boolean                  m_transformed; /**< \brief WD_True if the points have been transformed.*/
    WT_Boolean                  m_relativized; /**< \brief WD_True if the points have been relativized.*/
    int                         m_points_materialized; /**< \brief The number of points materialized.*/

private:
    WT_Point_Set_Data (WT_Point_Set_Data const &)
      : m_count (0)
      , m_allocated (0)
      , m_points (WD_Null)
      , m_transformed(WD_False)
      , m_relativized(WD_False)
      , m_points_materialized (0)
    {
        WD_Complain ("cannot copy WT_Point_Set_Data");
    } // prohibited

    WT_Point_Set_Data & operator= (WT_Point_Set_Data const &)
    {
        WD_Complain ("cannot assign WT_Point_Set_Data");
        return *this;
    } // prohibited

public:

    //@{ \name Construction / destruction
    /// Constructs a WT_Point_Set_Data object.
    WT_Point_Set_Data()
        : m_count (0)
        , m_allocated (0)
        , m_points (WD_Null)
        , m_transformed(WD_False)
        , m_relativized(WD_False)
    { }

    /// Constructs a WT_Point_Set_Data object from a given array of points.
    /** \warning Be careful when using the \a copy parameter.  If the
     *  points are not copied, and they get deallocated, trouble will ensue.
     *  It's usually always safe to create copies of the points, but it may not
     *  always be a desirable choice due to performance.  To help provide a
     *  means to mitigate the performance hit taken due to allocation and
     *  deallocation, the option is available to refer to the points directly.
     */
    WT_Point_Set_Data(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);

    /// Destroys a WT_Point_Set_Data object, and deletes the points if they had been copied in the constructor.
    virtual ~WT_Point_Set_Data();

    //@}


    //@{ \name Get methods
    /// This method is intentionally non-virtual for performance reasons.
    /// Returns a pointer to the array of points.
    WT_Logical_Point * points() const
    {
        return m_points;
    }
    //@}

    //@{ \name Status methods
    /// This method is intentionally non-virtual for performance reasons.
    /// Returns the number of allocated points (zero if the points were directly referenced in the constructor.)
    int allocated() const
    {
        return m_allocated;
    }

    /// Clears the list of points, and deletes the points if they had been copied in the constructor.
    /// This method is intentionally non-virtual for performance reasons.
    void clear();

    /// Returns the number of points in the set.
    /// This method is intentionally non-virtual for performance reasons.
    int count() const
    {
        return m_count;
    }

    /// Returns WD_True if the first point in the set can be expressed using a WT_Logical_Point16 object.
    virtual WT_Boolean first_point_fits_in_16_bits() const;

    /// Returns WD_True if the remaining points in the set (beyond the first) can be expressed using a WT_Logical_Point16 object.
    virtual WT_Boolean remaining_points_fit_in_16_bits() const;

    /// Equality operator.  Returns WD_True if the given point set matches this one.
    virtual WT_Boolean operator== (WT_Point_Set_Data const & set) const;
    //@}


    //@{ \name Manipulation methods
    /// Returns the points to absolute values based on their relative position to last materialized point.
    virtual WT_Result de_relativize(WT_File & file);
    /// Relativizes the points in the set to the last serialized point.
    virtual WT_Result relativize(WT_File & file);
    /// Sets the current point set with other contents (deletes the current set if allocated.)
    virtual WT_Point_Set_Data const & set(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result);

    /// Applies the given transform to the point set.
    virtual WT_Result transform(WT_Transform const & transform);
    //@}

};

/// A derived class of WT_Point_Set_Data which has I/O methods
class WHIPTK_API WT_Point_Set : public WT_Point_Set_Data
{
    friend class WT_W2D_Class_Factory;
    friend class WT_Viewport;
    
protected:
    int                         m_points_materialized; /**< \brief The number of points materialized.*/

private:
    WT_Point_Set (WT_Point_Set const &)
    {
        WD_Complain ("cannot copy WT_Point_Set");
    } // prohibited

    WT_Point_Set & operator= (WT_Point_Set const &)
    {
        WD_Complain ("cannot assign WT_Point_Set");
        return *this;
    } // prohibited

public:

    //@{ \name Construction / destruction
    /// Constructs a WT_Point_Set object.
    WT_Point_Set()
        : m_points_materialized (0)
    { }

    /// Constructs a WT_Point_Set object from a given array of points.
    /** \warning Be careful when using the \a copy parameter.  If the
     *  points are not copied, and they get deallocated, trouble will ensue.
     *  It's usually always safe to create copies of the points, but it may not
     *  always be a desirable choice due to performance.  To help provide a
     *  means to mitigate the performance hit taken due to allocation and
     *  deallocation, the option is available to refer to the points directly.
     */
    WT_Point_Set(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points, /**< Pointer to the array of points. */
        WT_Boolean                  copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        ) throw(WT_Result)
    : WT_Point_Set_Data( count, points, copy)
    { }

public:

    /// Destroys a WT_Point_Set object, and deletes the points if they had been copied in the constructor.
    virtual ~WT_Point_Set();
    //@}

    /// Clears the list of points, and deletes the points if they had been copied in the constructor.
    virtual void clear();

    //@{ \name Materialization / serialization methods
    /// Materializes the point set from the file as a set of WT_Logical_Point32 objects.
    /** Reads in the object data and causes the file reading to proceed to the end of this object.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Out_Of_Memory_Error Could not allocate the points.
     */
    virtual WT_Result   materialize(WT_File & file);

    /// Materializes the point set from the file as ASCII-serialized points.
    /** Reads in the object data and causes the file reading to proceed to the end of this object.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Corrupt_File_Error No points in the set.
     *  \retval WT_Result::Out_Of_Memory_Error Could not allocate the points.
     */
    virtual WT_Result   materialize_ascii(WT_File & file);

    /// Materializes the point set from the file as a set of WT_Logical_Point16 objects.
    /** Reads in the object data and causes the file reading to proceed to the end of this object.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Out_Of_Memory_Error Could not allocate the points.
     */
    virtual WT_Result   materialize_16_bit(WT_File & file);

    /// Causes the serialization of the object to the file.
    /** If this is a WT_Drawable derived object, this method may cause the object to be
     *  delayed (in case a coincident like-object follows) and/or merged (in case a coincident
     *  like-object preceeded) so as to optimize the output.
     *
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result serialize(WT_File &  file, /**< The file being written. */
                                  WT_Byte    opcode_ascii, /**< The opcode to use if writing ASCII. */
                                  WT_Byte    opcode_32bit, /**< The opcode to use for a point set utilizing WT_Logical_Point32 objects. */
                                  WT_Byte    opcode_16bit /**< The opcode to use for a point set utilizing WT_Logical_Point16 objects. */
                                  ) const;

    /// Causes the file reading to proceed to the end of this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    virtual WT_Result   skip_operand(WT_File & file);

    /// Causes the file reading to proceed to the end of this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    virtual WT_Result   skip_operand_ascii(WT_File & file);

    /// Causes the file reading to proceed to the end of this object.
    /** \warning This is used by the framework and should not be called by client code.
     */
    virtual WT_Result   skip_operand_16_bit(WT_File & file);
    //@}

};


#endif // POINTSET_HEADER
