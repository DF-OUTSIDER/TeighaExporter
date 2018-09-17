/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#ifndef _ODGEEXTENTS2D_INCLUDED_
#define _ODGEEXTENTS2D_INCLUDED_ /*!DOM*/

#include "Ge/GePoint2d.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeVector2d.h"
#include "Ge/GeMatrix2d.h"

#include "TD_PackPush.h"

#define INVALIDEXTENTS 1.0e20 /*!DOM*/

/** \details
    This class represents 2D bounding boxes as minimum and maximum 2d points.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeExtents2d
{
public:

  /** \param min [in]  Minimum point.
    \param max [in]  Maximum point.
  */
  OdGeExtents2d ()
    : m_min (INVALIDEXTENTS,  INVALIDEXTENTS)
    , m_max (-INVALIDEXTENTS, -INVALIDEXTENTS)
  {}

  OdGeExtents2d (
    const OdGePoint2d& min, 
    const OdGePoint2d& max) 
  { set (min, max); }

  /** \details
    The invalid extents.
  */
  GE_STATIC_EXPORT static const OdGeExtents2d kInvalid;

  /** \details
    Returns the minimum point of this Extents object.
  */
  const OdGePoint2d& minPoint () const 
  { return m_min; }

  /** \details
    Returns the maximum point of this Extents object.
  */
  const OdGePoint2d& maxPoint () const 
  { return m_max; }

  /** \details
    Sets the minimum and maximum points for this Extents object.
    
    \param min [in]  Minimum point.
    \param max [in]  Maximum point.
  */
  void set ( const OdGePoint2d& min, const OdGePoint2d& max) 
  { 
    //ODA_ASSERT_ONCE(min.x <= max.x);
    //ODA_ASSERT_ONCE(min.y <= max.y);
    m_min = min; m_max = max; 
  }

  /** \details
    Sets the minimum and maximum points for this Extents object to
    that of the box defined by pt1 and pt2.

    \param pt1 [in]  First point.
    \param pt2 [in]  Second point.

    \remarks
    pt1 and pt2 need only define a box. They need not be the minimum
    and maximum points of the box.
  */
  void comparingSet(
    const OdGePoint2d& pt1,
    const OdGePoint2d& pt2);

  /** \details
    Updates the extents of this Extents object with the specified point.

    \param point [in]  Any 2D point.
  */
  OdGeExtents2d& addPoint (
    const OdGePoint2d& point)
  {
    if ( !isValidExtents() )
    {
      m_max = m_min = point;
    }
    else
    {
      m_max.x = odmax (point.x, m_max.x);
      m_max.y = odmax (point.y, m_max.y);
      m_min.x = odmin (point.x, m_min.x);
      m_min.y = odmin (point.y, m_min.y);
    }
    return *this;
  }

  /** \details
    Updates the extents of this Extents object with the specified array of points.

    \param points [in]  Array of any 2D points.
  */
  OdGeExtents2d& addPoints(const OdGePoint2dArray& points)
  {
    if (points.empty()) {
      return *this;
    }
    OdUInt32 i = 0, sz = points.size();
    const OdGePoint2d* pointsArr = points.asArrayPtr();
    if (!isValidExtents()) {
      m_max = m_min = pointsArr[0];
      i = 1;
    }
#define ex_minmax(max, min, val) if (max < val) max = val; else if (min > val) min = val;
    for (; i < sz; ++i) {
      ex_minmax(m_max.x, m_min.x, pointsArr[i].x);
      ex_minmax(m_max.y, m_min.y, pointsArr[i].y);
    }
#undef ex_minmax
    return *this;
  }

  /** \details
    Updates the extents of this Extents object with the specified Extents object.
    
    \param extents [in]  Any 2D Extents object.
  */
  OdGeExtents2d& addExt (
    const OdGeExtents2d& extents)
  {
    if (extents.isValidExtents ())
    {
      addPoint (extents.minPoint ());
      addPoint (extents.maxPoint ());
    }
    return *this;
  }

  /** \details
    Returns true if and only if this Extents object contains valid extents.
    
    \remarks
    Extents are valid if and only if each member of the minimum extents 
    is less than or equal to the corresponding member of maximum extents.
  */
  bool isValidExtents () const
  {
    return ( (m_max.x >= m_min.x) && (m_max.y >= m_min.y) );
  }

  /** \details
    Updates the extents of this Extents object by the specified vector.
    \param vect [in]  Any 2D vector.
  */
    void expandBy (
    const OdGeVector2d& vect)
  {
    if (isValidExtents ())
    {
      OdGePoint2d p1 = m_min, p2 = m_max;
      p1 += vect;
      p2 += vect;
      addPoint (p1);
      addPoint (p2);
    }
  }

  /** \details
    Applies the 2D transformation matrix to the extents.

    \param xfm [in]  2D transformation matrix.
  */
  void transformBy (
    const OdGeMatrix2d& xfm)
  {
    OdGeVector2d vecX (OdGeVector2d::kXAxis * (m_max.x - m_min.x)),
      vecY (OdGeVector2d::kYAxis * (m_max.y - m_min.y));

    if (isValidExtents ())
    {
      m_max = m_min = (xfm * m_min);
      expandBy (xfm * vecX);
      expandBy (xfm * vecY);
    }
  }
    
  void translate (
    const OdGeVector2d &iShift)
  {
    m_min += iShift;
    m_max += iShift;
  }

  /** \details
    Returns true if and only if this Extents object contains the specified object.
    
    \param point [in]  Any 2D point.
    \param extents [in]  Any 2D Extents object.
  */
  bool contains (
    const OdGePoint2d& point, const OdGeTol& tol = OdGeContext::gTol) const;

  bool contains (
    const OdGeExtents2d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if specified Extents object
    does not intersect this one.

    \param extents [in]  Any 2D Extents object.
  */
  bool isDisjoint (
    const OdGeExtents2d& extents, const OdGeTol& tol = OdGeContext::gTol) const;  

  enum IntersectionStatus
  {
    kIntersectUnknown,// Either or both extents are invalid
    kIntersectNot,    // Extents are NOT intersecting
    kIntersectOpIn,   // Operand is completely within this extents
    kIntersectOpOut,  // This extents is completely within operand
    kIntersectOk      // Extents are intersecting, result is returned
  };

  /** \details
    Determines the intersection of the specified Extents object with this one,
    and returns the resulting intersection box.
    
    \param extents [in]  Any 2D Extents object.
    \param pResult [out]  Receives extents of the intersection.
    
    \remarks
    pResult object should be created by the caller. Possible return values are as follows.
    
    @untitled table
    kIntersectUnknown   Either or both Extents objects are invalid
    kIntersectNot       The Extents objects are NOT intersecting
    kIntersectOpIn      The specified Extents object is completely within this one
    kIntersectOpOut     This Extents object is completely within the specified one
    kIntersectOk        The Extents objectes are intersecting, and a result is returned
    
  */
  IntersectionStatus intersectWith (
    const OdGeExtents2d& extents, 
    OdGeExtents2d* pResult = 0) const;

  OdGePoint2d center() const
  {
    return m_min + (m_max - m_min) * 0.5;
  }

  bool isEqualTo(const OdGeExtents2d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

  bool operator ==(const OdGeExtents2d& extents) const
  {
    return isEqualTo(extents);
  }
  bool operator !=(const OdGeExtents2d& extents) const
  {
    return !isEqualTo(extents);
  }

protected:
  OdGePoint2d m_min;
  OdGePoint2d m_max;
};

// Inlines section

inline void OdGeExtents2d::comparingSet(const OdGePoint2d& pt1, const OdGePoint2d& pt2)
{
  if (pt1.x > pt2.x)
  {
    m_max.x = pt1.x;
    m_min.x = pt2.x;
  }
  else
  {
    m_min.x = pt1.x;
    m_max.x = pt2.x;
  }
  if (pt1.y > pt2.y)
  {
    m_max.y = pt1.y;
    m_min.y = pt2.y;
  }
  else
  {
    m_min.y = pt1.y;
    m_max.y = pt2.y;
  }
}

inline bool OdGeExtents2d::isEqualTo(const OdGeExtents2d& extents, const OdGeTol& tol) const
{
  const OdUInt8 bValid = ((isValidExtents()) ? 1 : 0) | ((extents.isValidExtents()) ? 2 : 0);
  switch (bValid)
  {
    // Both invalid
    case 0: return true;
    // Both valid
    case 3: return m_min.isEqualTo(extents.m_min, tol) && m_max.isEqualTo(extents.m_max, tol);
  }
  return false;
}

inline bool OdGeExtents2d::contains(const OdGePoint2d& point, const OdGeTol& tol) const
{
  ODA_ASSERT(isValidExtents());

  return ( (point.x + tol.equalPoint()) >= m_min.x
    && (point.y + tol.equalPoint()) >= m_min.y
    && (point.x - tol.equalPoint()) <= m_max.x
    && (point.y - tol.equalPoint()) <= m_max.y );
}

inline bool OdGeExtents2d::contains(const OdGeExtents2d& extents, const OdGeTol& tol) const
{
  ODA_ASSERT(isValidExtents());

  return ((extents.m_min.x + tol.equalPoint()) >= m_min.x 
    && (extents.m_min.y + tol.equalPoint()) >= m_min.y 
    && m_max.x >= (extents.m_max.x - tol.equalPoint()) 
    && m_max.y >= (extents.m_max.y - tol.equalPoint()));
}

inline bool OdGeExtents2d::isDisjoint(const OdGeExtents2d& extents, const OdGeTol& tol) const
{
  ODA_ASSERT(isValidExtents());

  return ((extents.m_min.x - tol.equalPoint()) > m_max.x 
    || (extents.m_min.y - tol.equalPoint()) > m_max.y 
    || m_min.x > (extents.m_max.x + tol.equalPoint())
    || m_min.y > (extents.m_max.y + tol.equalPoint()));
} 

#undef INVALIDEXTENTS

#include "TD_PackPop.h"

#endif //_ODGEEXTENTS2D_INCLUDED_
