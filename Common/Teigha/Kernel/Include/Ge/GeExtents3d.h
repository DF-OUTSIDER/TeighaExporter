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

#ifndef _ODGEEXTENTS3D_INCLUDED_
#define _ODGEEXTENTS3D_INCLUDED_ /*!DOM*/

#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"

#include "TD_PackPush.h"

class OdGeExtents2d;

#define INVALIDEXTENTS 1.0e20  /*!DOM*/

/** \details
    This class represents 3D bounding boxes as minimum and maximum 3d points.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeExtents3d
{
public:

  /** \param min [in]  Minimum point.
    \param max [in]  Maximum point.
  */
  OdGeExtents3d();

  OdGeExtents3d(
    const OdGePoint3d& min, 
    const OdGePoint3d& max);

  /** \details
    The invalid extents.
  */
  GE_STATIC_EXPORT static const OdGeExtents3d kInvalid;

  /** \details
    Returns the minimum point of this Extents object.
  */
  const OdGePoint3d& minPoint() const;

  /** \details
    Returns the maximum point of this Extents object.
  */
  const OdGePoint3d& maxPoint() const;

  /** \details
    Returns the vertex of the box depending on index.
    Minimum vertex is returned when iIndex = 0, maximum vertex otherwise.
  */
  const OdGePoint3d& operator[] (int iIndex) const {
    return iIndex == 0 ? m_min : m_max;
  }
  OdGePoint3d& operator[] (int iIndex) {
    return iIndex == 0 ? m_min : m_max;
  }

  /** \details
    Sets the minimum and maximum points for this Extents object.
    
    \param min [in]  Minimum point.
    \param max [in]  Maximum point.
  */
  void set(
    const OdGePoint3d& min, 
    const OdGePoint3d& max);

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
    const OdGePoint3d& pt1, 
    const OdGePoint3d& pt2);

  /** \details
    Updates the extents of this Extents object with the specified point.

    \param point [in]  Any 3D point.
  */
  OdGeExtents3d& addPoint(
    const OdGePoint3d& point);

  /** \details
    Updates the extents of this Extents object with the specified array of points.

    \param points [in]  Array of any 3D points.
  */
  OdGeExtents3d& addPoints(
    const OdGePoint3dArray& points);

  /** \details
    Updates the extents of this Extents object with the specified Extents object.
    
    \param extents [in]  Any 3D Extents object.
  */
  OdGeExtents3d& addExt(
    const OdGeExtents3d& extents);

  /** \details
    Returns true if and only if this Extents object contains valid extents.
    
    \remarks
    Extents are valid if and only if each member of the minimum extents 
    is less than or equal to the corresponding member of maximum extents.
  */
  inline bool isValidExtents() const
  {
    return ( (m_max.x >= m_min.x) && (m_max.y >= m_min.y) && (m_max.z >= m_min.z));
  }

  /** \details
    Updates the extents of this Extents object by the specified vector.
    \param vect [in]  Any 3D vector.
  */
    void expandBy(
    const OdGeVector3d& vect);

  /** \details
    Applies the 3D transformation matrix to the extents.

    \param xfm [in]  3D transformation matrix.
  */
  void transformBy(
    const OdGeMatrix3d& xfm);
    
  /** \details
    Returns true if and only if this Extents object contains the specified object.
    
    \param point [in]  Any 3D point.
    \param extents [in]  Any 3D Extents object.
  */
  bool contains(
    const OdGePoint3d& point, const OdGeTol& tol = OdGeContext::gTol) const;

  bool contains(
    const OdGeExtents3d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if specified Extents object
    does not intersect this one.

    \param extents [in]  Any 3D Extents object.
  */
  bool isDisjoint(
    const OdGeExtents3d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

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
    
    \param extents [in]  Any 3D Extents object.
    \param pResult [out]  Receives extents of the intersection.
    
    \remarks
    pResult object should be created by the caller. Possible return values are as follows.
    
    @untitled table
    kIntersectUnknown   Either or both extents boxes are invalid
    kIntersectNot       The extents boxes are NOT intersecting
    kIntersectOpIn      The specified Extents object is completely within this one
    kIntersectOpOut     This Extents object is completely within the specified one
    kIntersectOk        The extents boxes are intersecting, and a result is returned
    
  */
  IntersectionStatus intersectWith(const OdGeExtents3d& extents, OdGeExtents3d* pResult = 0) const;

  OdGePoint3d center() const
  {
    return m_min + (m_max - m_min) * 0.5;
  }

  enum Convert2dPlane
  {
    kConvert2dPlaneXY = 0x04,
    kConvert2dPlaneXZ = 0x0C,
    kConvert2dPlaneYX = 0x01,
    kConvert2dPlaneYZ = 0x0D,
    kConvert2dPlaneZX = 0x03,
    kConvert2dPlaneZY = 0x07
  };
  void convert2d(OdGeExtents2d &extents, Convert2dPlane plane = kConvert2dPlaneXY) const;
  void setFrom2d(const OdGeExtents2d &extents, Convert2dPlane plane = kConvert2dPlaneXY);

  bool isEqualTo(const OdGeExtents3d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

  bool operator ==(const OdGeExtents3d& extents) const
  {
    return isEqualTo(extents);
  }
  bool operator !=(const OdGeExtents3d& extents) const
  {
    return !isEqualTo(extents);
  }

protected:
  OdGePoint3d m_min;
  OdGePoint3d m_max;
};

// Inline implementations

inline OdGeExtents3d::OdGeExtents3d()
  : m_min(INVALIDEXTENTS,  INVALIDEXTENTS,  INVALIDEXTENTS)
  , m_max(-INVALIDEXTENTS, -INVALIDEXTENTS, -INVALIDEXTENTS)
{
}

inline OdGeExtents3d::OdGeExtents3d(const OdGePoint3d& min, const OdGePoint3d& max)
  : m_min(min)
  , m_max(max)
{
}

inline const OdGePoint3d& OdGeExtents3d::minPoint() const
{
  return m_min;
}

inline const OdGePoint3d& OdGeExtents3d::maxPoint() const
{
  return m_max;
}

inline void OdGeExtents3d::set(const OdGePoint3d& min, const OdGePoint3d& max)
{
  //ODA_ASSERT_ONCE(min.x <= max.x);
  //ODA_ASSERT_ONCE(min.y <= max.y);
  //ODA_ASSERT_ONCE(min.z <= max.z);
  m_min = min;
  m_max = max;
}

inline void OdGeExtents3d::comparingSet(const OdGePoint3d& pt1, const OdGePoint3d& pt2)
{
  if(pt1.x > pt2.x)
  {
    m_max.x = pt1.x;
    m_min.x = pt2.x;
  }
  else
  {
    m_min.x = pt1.x;
    m_max.x = pt2.x;
  }
  if(pt1.y > pt2.y)
  {
    m_max.y = pt1.y;
    m_min.y = pt2.y;
  }
  else
  {
    m_min.y = pt1.y;
    m_max.y = pt2.y;
  }
  if(pt1.z > pt2.z)
  {
    m_max.z = pt1.z;
    m_min.z = pt2.z;
  }
  else
  {
    m_min.z = pt1.z;
    m_max.z = pt2.z;
  }
}

#define ex_minmax(max, min, val) \
  if (max < val) max = val;\
  else if (min > val) min = val;

inline OdGeExtents3d& OdGeExtents3d::addPoint(const OdGePoint3d& point)
{
  if ((m_max.x < m_min.x) || (m_max.y < m_min.y) || (m_max.z < m_min.z))
  {
    m_max = m_min = point;
  }
  else
  {
    ex_minmax(m_max.x, m_min.x, point.x);
    ex_minmax(m_max.y, m_min.y, point.y);
    ex_minmax(m_max.z, m_min.z, point.z);
  }
  return *this;
}

inline OdGeExtents3d& OdGeExtents3d::addPoints(const OdGePoint3dArray& points)
{
  if (points.empty()) {
    return *this;
  }
  OdUInt32 i = 0, sz = points.size();
  const OdGePoint3d* pointsArr = points.asArrayPtr();
  if (!isValidExtents()) {
    m_max = m_min = pointsArr[0];
    i = 1;
  }
  for (; i < sz; ++i) {
    ex_minmax(m_max.x, m_min.x, pointsArr[i].x);
    ex_minmax(m_max.y, m_min.y, pointsArr[i].y);
    ex_minmax(m_max.z, m_min.z, pointsArr[i].z);
  }
  return *this;
}
#undef ex_minmax

inline OdGeExtents3d& OdGeExtents3d::addExt(const OdGeExtents3d& extents)
{
  ODA_ASSERT(extents.isValidExtents());

#if 0
  addPoint(extents.minPoint());
  addPoint(extents.maxPoint());
#else
  // Optimization
#define ex_minmax(max, min, valMax, valMin) \
  if (max < valMax) max = valMax; \
  if (min > valMin) min = valMin;

  if ((m_max.x < m_min.x) || (m_max.y < m_min.y) || (m_max.z < m_min.z))
  {
    set(extents.minPoint(), extents.maxPoint());
  }
  else
  {
    ex_minmax(m_max.x, m_min.x, extents.maxPoint().x, extents.minPoint().x);
    ex_minmax(m_max.y, m_min.y, extents.maxPoint().y, extents.minPoint().y);
    ex_minmax(m_max.z, m_min.z, extents.maxPoint().z, extents.minPoint().z);
  }

#undef ex_minmax
#endif
  return *this;
}

inline void OdGeExtents3d::expandBy(const OdGeVector3d& vect)
{
  ODA_ASSERT(isValidExtents());

  OdGePoint3d p1 = m_min, p2 = m_max;
  addPoint(p1 + vect);
  addPoint(p2 + vect);
}    

inline void OdGeExtents3d::transformBy(const OdGeMatrix3d& xfm)
{
  ODA_ASSERT(isValidExtents());

  OdGeVector3d d = m_max - m_min;
  
  m_max = m_min = (xfm * m_min);
  if(OdNonZero(d.x, 1.e-200))
    expandBy(xfm * (OdGeVector3d::kXAxis * d.x));
  if(OdNonZero(d.y, 1.e-200))
    expandBy(xfm * (OdGeVector3d::kYAxis * d.y));
  if(OdNonZero(d.z, 1.e-200))
    expandBy(xfm * (OdGeVector3d::kZAxis * d.z));
}    

inline bool OdGeExtents3d::contains(const OdGePoint3d& point, const OdGeTol& tol) const
{
  ODA_ASSERT(isValidExtents());

  return ( (point.x + tol.equalPoint()) >= m_min.x
    && (point.y + tol.equalPoint()) >= m_min.y
    && (point.z + tol.equalPoint()) >= m_min.z
    && (point.x - tol.equalPoint()) <= m_max.x
    && (point.y - tol.equalPoint()) <= m_max.y
    && (point.z - tol.equalPoint()) <= m_max.z );
}

inline bool OdGeExtents3d::contains(const OdGeExtents3d& extents, const OdGeTol& tol) const
{
  ODA_ASSERT(isValidExtents());

  return ((extents.m_min.x + tol.equalPoint()) >= m_min.x 
    && (extents.m_min.y + tol.equalPoint()) >= m_min.y 
    && (extents.m_min.z + tol.equalPoint()) >= m_min.z 
    && m_max.x >= (extents.m_max.x - tol.equalPoint()) 
    && m_max.y >= (extents.m_max.y - tol.equalPoint()) 
    && m_max.z >= (extents.m_max.z - tol.equalPoint()));
}    

inline bool OdGeExtents3d::isDisjoint(const OdGeExtents3d& extents, const OdGeTol& tol) const
{
  ODA_ASSERT(isValidExtents());

  return ((extents.m_min.x - tol.equalPoint()) > m_max.x 
    || (extents.m_min.y - tol.equalPoint()) > m_max.y 
    || (extents.m_min.z - tol.equalPoint()) > m_max.z
    || m_min.x > (extents.m_max.x + tol.equalPoint())
    || m_min.y > (extents.m_max.y + tol.equalPoint())
    || m_min.z > (extents.m_max.z + tol.equalPoint()));
}    

inline bool OdGeExtents3d::isEqualTo(const OdGeExtents3d& extents, const OdGeTol& tol) const
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

#undef INVALIDEXTENTS

#include "TD_PackPop.h"

#endif //_ODGEEXTENTS3D_INCLUDED_
