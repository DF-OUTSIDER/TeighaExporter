//
//  Copyright (c) 2006 by Autodesk, Inc.
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

#if !defined XAML_POINTSET_HEADER
#define XAML_POINTSET_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/pointset.h"

class WT_File;

/// A base class for a set of WT_Point2D objects.
class XAMLTK_API WT_XAML_Point_Set_Data
{
protected:
    WT_Integer32                m_count;  /**< \brief The number of points in the set.*/
    WT_Integer32                m_allocated; /**< \brief The number of allocated points.*/
    WT_Point2D *                m_points; /**< \brief The arrray of points.*/
    bool                        m_relativized; /**< \brief WD_True if the points have been relativized.*/

private:
    WT_XAML_Point_Set_Data (WT_XAML_Point_Set_Data const &)
      : m_count (0)
      , m_allocated (0)
      , m_points (NULL)
      , m_relativized(false)
    {
        WD_Complain ("cannot copy WT_XAML_Point_Set_Data");
    } // prohibited

    WT_XAML_Point_Set_Data & operator= (WT_XAML_Point_Set_Data const &)
    {
        WD_Complain ("cannot assign WT_XAML_Point_Set_Data");
        return *this;
    } // prohibited

public:

    //@{ \name Construction / destruction
    /// Constructs a WT_XAML_Point_Set_Data object.
    WT_XAML_Point_Set_Data()
        : m_count (0)
        , m_allocated (0)
        , m_points (NULL)
        , m_relativized(false)
    { }

    /// Constructs a WT_XAML_Point_Set_Data object from a given array of logical points.
    WT_XAML_Point_Set_Data(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points
        );

    /// Constructs a WT_XAML_Point_Set_Data object from a given array of points.
    /** \warning Be careful when using the \a copy parameter.  If the
     *  points are not copied, and they get deallocated, trouble will ensue.
     *  It's usually always safe to create copies of the points, but it may not
     *  always be a desirable choice due to performance.  To help provide a
     *  means to mitigate the performance hit taken due to allocation and
     *  deallocation, the option is available to refer to the points directly.
     */
    WT_XAML_Point_Set_Data(
        int                         count, /**< The number of points in the array. */
        WT_Point2D const *          points, /**< Pointer to the array of points. */
        bool                        copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        );

    /// Constructrs a WT_XAML_Point_Set_Data from a vector of points
    WT_XAML_Point_Set_Data(
        vector<WT_Point2D>&         points
        ) throw (WT_Result);

    /// Destroys a WT_XAML_Point_Set_Data object, and deletes the points if they had been copied in the constructor.
    virtual ~WT_XAML_Point_Set_Data();

    //@}


    //@{ \name Get methods
    /// Returns a pointer to the array of points.
    virtual WT_Point2D * points() const
    {
        return m_points;
    }
    //@}

    //@{ \name Status methods
    /// Returns the number of allocated points (zero if the points were directly referenced in the constructor.)
    virtual int allocated() const
    {
        return m_allocated;
    }

    /// Clears the list of points, and deletes the points if they had been copied in the constructor.
    virtual void clear();

    /// Returns the number of points in the set.
    virtual int count() const
    {
        return m_count;
    }

    /// Equality operator.  Returns WD_True if the given point set matches this one.
    virtual bool operator== (WT_XAML_Point_Set_Data const & set) const;
    //@}

    //@{ \name Manipulation methods
    /// Returns the points to absolute values based on their relative position to last materialized point.
    virtual void de_relativize(WT_File & file);
    /// Relativizes the points in the set to the last serialized point.
    virtual void relativize(WT_File & file);
    /// Sets the current point set with other contents (deletes the current set if allocated.)
    virtual WT_XAML_Point_Set_Data const & set(
        int                         count, /**< The number of points in the array. */
        WT_Logical_Point const *    points /**< Pointer to the array of points. */
        );

    /// Sets the current point set with other contents (deletes the current set if allocated.)
    virtual WT_XAML_Point_Set_Data const & set(
        int                         count, /**< The number of points in the array. */
        WT_Point2D const *          points, /**< Pointer to the array of points. */
        bool                        copy /**< Whether the points should be copied or if their addresses should be used directly from the array. */
        );
};

#endif // XAML_POINTSET_HEADER
