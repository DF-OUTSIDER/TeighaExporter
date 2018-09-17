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
#if !defined LOGICAL_BOX_HEADER
#define LOGICAL_BOX_HEADER

///
///\file        dwf/whiptk/logical_box.h
///

/// A rectangle defined by two points (lower left, upper right).
class WHIPTK_API WT_Logical_Box
{
	
public:
    WT_Logical_Point    m_min; /**< \brief Lower-left point of rectangle. */
    WT_Logical_Point    m_max; /**< \brief Upper-right point of rectangle. */

    /// Constructs a WT_Logical_Box object.
    WT_Logical_Box()
    { }

    /// Constructs a WT_Logical_Box object with the given points.
    WT_Logical_Box(WT_Logical_Point min, WT_Logical_Point max)
        : m_min (min)
        , m_max (max)
    { }

    /// Constructs a WT_Logical_Box object with the given point values.
    WT_Logical_Box(WT_Integer32 minx, WT_Integer32 miny, WT_Integer32 maxx, WT_Integer32 maxy)
    {
        m_min.m_x = minx;
        m_min.m_y = miny;
        m_max.m_x = maxx;
        m_max.m_y = maxy;
    }

    /// Destroys a WT_Logical_Box object.
    virtual ~WT_Logical_Box()
    { }
public:
    /// Returns the upper-right point.
    WT_Logical_Point & maxpt()
    {   return m_max;   }

    /// Returns the lower-left point.
    WT_Logical_Point & minpt()
    {   return m_min;   }

    /// Returns WD_True if the given box is equivalent to this one.
    WT_Boolean    operator== (WT_Logical_Box const & lb) const
    {
    if (m_min == lb.m_min && m_max == lb.m_max)
        return WD_True;
    else
        return WD_False;
    }

    // Operator += is used grow the Logical_Box 
    WT_Logical_Box& operator+= ( const WT_Logical_Box& rhlb ) 
    {
        if( rhlb.m_min.m_x < m_min.m_x )
        {
            m_min.m_x = rhlb.m_min.m_x;
        }
        if( rhlb.m_min.m_y < m_min.m_y )
        {
            m_min.m_y = rhlb.m_min.m_y;
        }
        if( rhlb.m_max.m_x > m_max.m_x )
        {
            m_max.m_x = rhlb.m_max.m_x;
        }
        if( rhlb.m_max.m_y > m_max.m_y )
        {
            m_max.m_y = rhlb.m_max.m_y;
        }
        return *this;
    }
};

#endif // LOGICAL_BOX_HEADER
