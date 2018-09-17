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
///\file        dwf/whiptk/logical_point.h
///

#if !defined LOGICAL_POINT_HEADER
#define LOGICAL_POINT_HEADER

class WT_Logical_Point_16;
class WT_Transform;

/// A 2D point using 32-bit integers for its x and y ordinals.
class WHIPTK_API WT_Logical_Point
{
	
public:
    WT_Integer32    m_x; /**< \brief Magnitude of the point along the X-axis. */
    WT_Integer32    m_y; /**< \brief Magnitude of the point along the Y-axis. */

    /// Constructs a WT_Logical_Point object (defaults to the origin.)
    WT_Logical_Point() : m_x(0), m_y(0) {}

    /// Constructs a WT_Logical_Point object from the given values.
    WT_Logical_Point(WT_Integer32 x, WT_Integer32 y)
        : m_x (x)
        , m_y (y)
    { }

    /// Destroys a WT_Logical_Point object.
    ~WT_Logical_Point() {}

    /// Returns WD_True if the given point is equal to this one.
    WT_Boolean    operator== (WT_Logical_Point const & lp) const
    {
        if (m_x == lp.m_x && m_y == lp.m_y)
            return WD_True;
        else
            return WD_False;
    }

    WT_Boolean    operator!= (WT_Logical_Point const & lp) const
    {
        if (operator==(lp))
            return WD_False;
        else
            return WD_True;
    }


    /// Returns WD_True if the given point is equal to this one.
    WT_Boolean  operator== (WT_Logical_Point_16 const & lp) const;

    /// Assigns the values from the given point to this one.
    WT_Logical_Point const & operator= (WT_Logical_Point_16 const & in);
    /// Applies the given transform to this point and returns the result.
    WT_Logical_Point const   operator* (WT_Transform const & trans) const;
    /// Applies the given transform to this point, sets it to this point, and returns the result.
    WT_Logical_Point const   operator*= (WT_Transform const & trans);
};

/// A 2D point using 16-bit integers for its x and y ordinals.
class WT_Logical_Point_16
{	
public:
    WT_Integer16    m_x; /**< \brief Magnitude of the point along the X-axis. */
    WT_Integer16    m_y; /**< \brief Magnitude of the point along the Y-axis. */


    // Constructors, Destructors

    /// Constructs a WT_Logical_Point_16 object (defaults to the origin.)
    WT_Logical_Point_16() : m_x(0), m_y(0) {}

    /// Constructs a WT_Logical_Point object from the given values.
    /** \note This is a convenience constructor.  Client code should ensure that the
     *  each of the values fit into 16-bits.
     */
    WT_Logical_Point_16(WT_Integer32 x, WT_Integer32 y)
    {
        m_x = (WT_Integer16)x;
        m_y = (WT_Integer16)y;
        WD_Assert(m_x == x);
        WD_Assert(m_y == y);
    }

    /// Constructs a WT_Logical_Point object from the given values.
    WT_Logical_Point_16(WT_Integer16 x, WT_Integer16 y)
        : m_x (x)
        , m_y (y)
    { }

    /// Destroys a WT_Logical_Point object.
    ~WT_Logical_Point_16() {}
    
    /// Returns WD_True if the given point is equal to this one.
    WT_Boolean    operator== (WT_Logical_Point const & lp) const
    {
        if ((WT_Integer32)m_x == lp.m_x &&
            (WT_Integer32)m_y == lp.m_y)
            return WD_True;
        else
            return WD_False;
    }

    /// Assignment operator.  Sets the value of the given point to this one.
    /** \note This is a convenience operator.  Client code should ensure that the
     *  each of the values fit into 16-bits.
     */
    WT_Logical_Point_16 const & operator= (WT_Logical_Point const & in)
    {
        m_x = (WT_Integer16) in.m_x;
        m_y = (WT_Integer16) in.m_y;
        WD_Assert (m_x == in.m_x); // Make sure that we don't truncate data
        WD_Assert (m_y == in.m_y);
        return *this;
    }
};

#endif // LOGICAL_POINT_HEADER
