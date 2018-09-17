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

#ifndef __GSDEFS_H_INCLUDED_
#define __GSDEFS_H_INCLUDED_

#include "TD_PackPush.h"
#include "OdRound.h"

//****************************************************************************
// Helper classes
//****************************************************************************

// copied(with modification) from limits.h to avoid extra #includes

#define SCALAR_MIN    (-2147483647 - 1)  // minimum(signed) int32 value
#define SCALAR_MAX      2147483647      // maximum(signed) int32 value

/** \details
    This class represents 2D Device Coordinate points.
    Corresponding C++ library: TD_Gs

    \remarks
    Device coordinates are measured in pixels from the lower-left corner
    of the device.
    
    \sa
    Coordinate Systems.
    
    <group OdGs_Classes> 
*/
class OdGsDCPoint
{
public:
  enum MaxFlag { Maximum };
  enum MinFlag { Minimum };
  
  /** \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
    
    \remarks
    Builds an instance of the object
    
    OdGsDCPoint(Maximum) sets x = SCALAR_MAX, y = SCALAR_MAX
    
    OdGsDCPoint(Minimum) sets x = SCALAR_MIN, y = SCALAR_MIN
  */
  OdGsDCPoint() { }
  OdGsDCPoint(long xx, long yy) : x(xx), y(yy) { }
  
  
  OdGsDCPoint(MaxFlag) { x = SCALAR_MAX; y = SCALAR_MAX; }
  OdGsDCPoint(MinFlag) { x = SCALAR_MIN; y = SCALAR_MIN; }
  
  /** \remarks
    Sets this object to the value of the object on the right.

    Maximum sets x = SCALAR_MAX, y = SCALAR_MAX
    
    Minimum sets x = SCALAR_MIN, y = SCALAR_MIN
  */
  void operator=(MaxFlag) { x = SCALAR_MAX; y = SCALAR_MAX; }
  void operator=(MinFlag) { x = SCALAR_MIN; y = SCALAR_MIN; }
  
  void operator=(const OdGsDCPoint& dcPoint) { x = dcPoint.x; y = dcPoint.y; }
  bool operator==(const OdGsDCPoint& dcPoint) const { return x == dcPoint.x && y == dcPoint.y; }
  bool operator!=(const OdGsDCPoint& dcPoint) const { return x != dcPoint.x || y != dcPoint.y; }
  inline long operator[](unsigned int i) const { return *(&x + i); }

  long x;  // X-coordinate.
  long y;  // Y-coordinate.
};

/** \details
    This class represents 2D device coordinate rectangles.
    Corresponding C++ library: TD_Gs

    \remarks
    Device coordinates are measured in pixels from the lower-left corner
    of the device.
    
    \sa
    Coordinate Systems.
    
    <group OdGs_Classes>
*/
class OdGsDCRect
{
public:
  enum NullFlag { Null };
  /** \param minPoint [in]  Lower-left corner.
    \param maxPoint [in]  Upper-right corner.
    \param xMin [in]  Minimum X-coordinate.
    \param xMax [in]  Maximum X-coordinate.
    \param yMin [in]  Minimum Y-coordinate.
    \param yMax [in]  Maximum Y-coordinate.
  */
  OdGsDCRect() { }
  OdGsDCRect(const OdGsDCPoint& minPoint, const OdGsDCPoint& maxPoint) : m_min(minPoint), m_max(maxPoint) { }
  OdGsDCRect(long xMin, long xMax, long yMin, long yMax) : m_min(xMin,yMin), m_max(xMax,yMax) { }
  OdGsDCRect(NullFlag) { set_null(); }
  
  OdGsDCRect & operator=(const OdGsDCRect& dcRect)
  {
    m_min = dcRect.m_min;
    m_max = dcRect.m_max;
    return*this;
  }
  void operator|=(const OdGsDCRect& dcRect)
  {
    if(m_min.x > dcRect.m_min.x) 
      m_min.x = dcRect.m_min.x; 
    if(m_max.x < dcRect.m_max.x) 
      m_max.x = dcRect.m_max.x;
    
    if(m_min.y > dcRect.m_min.y)
      m_min.y = dcRect.m_min.y;
    if(m_max.y < dcRect.m_max.y)
      m_max.y = dcRect.m_max.y;
  }
  void operator&=(const OdGsDCRect& dcRect)
  {
    intersectWith(dcRect);
  }
  bool operator==(const OdGsDCRect& dcRect) const
  {
    return m_min == dcRect.m_min && m_max == dcRect.m_max; 
  }
  bool operator!=(const OdGsDCRect& dcRect) const
  {
    return !(*this == dcRect);
  }
  /** \details
    Sets this rectangle object to the null rectangle.
  */
  void set_null()
  { 
    m_min = OdGsDCPoint::Maximum; 
    m_max = OdGsDCPoint::Minimum; 
  } 
  
  /** \details
    Returns true if and only if this rectangle object is the null rectangle.
  */
  bool is_null() const
  {
    ODA_ASSERT((m_min.x <= m_max.x && m_min.y <= m_max.y) ||
     (m_min == OdGsDCPoint::Maximum && m_max == OdGsDCPoint::Minimum));
    return m_min.x > m_max.x;
  }
  
  /** \details
    Returns true if and only if this rectangle object is inside the specified rectangle object.
    \param dcRect [in]  Any display coordinate rectangle.
    
    \remarks
    A coincident edge is considered inside.
  */
  bool within(const OdGsDCRect& dcRect) const
  {
    ODA_ASSERT(!is_null()); 
    return m_min.x >= dcRect.m_min.x && 
      m_min.y >= dcRect.m_min.y && 
      m_max.x <= dcRect.m_max.x &&
      m_max.y <= dcRect.m_max.y;
  }

  void offset(long x, long y)
  {
    m_min.x += x;
    m_max.x += x;
    m_min.y += y;
    m_max.y += y;
  }

  void intersectWith(const OdGsDCRect& dcRect, bool bValidate = true)
  {
    if(m_min.x < dcRect.m_min.x)
      m_min.x = dcRect.m_min.x; 
    if(m_max.x > dcRect.m_max.x) 
      m_max.x = dcRect.m_max.x;

    if(m_min.y < dcRect.m_min.y)
      m_min.y = dcRect.m_min.y;
    if(m_max.y > dcRect.m_max.y)
      m_max.y = dcRect.m_max.y;

    if(bValidate && (m_min.x > m_max.x || m_min.y > m_max.y))
      *this = Null;
  }

  void normalize()
  {
    long tmp;
    if (m_max.x < m_min.x)
      tmp = m_max.x, m_max.x = m_min.x, m_min.x = tmp;
    if (m_max.y < m_min.y)
      tmp = m_max.y, m_max.y = m_min.y, m_min.y = tmp;
  }

  OdGsDCPoint m_min;
  OdGsDCPoint m_max;
};

/** \details
    This class represents 2D device coordinate rectangles.
    Corresponding C++ library: TD_Gs

    \remarks
    Device coordinates are measured in pixels from the lower-left corner
    of the device.
    
    \sa
    Coordinate Systems.
    
    <group OdGs_Classes>
*/
class OdGsDCRectDouble
{
public:
  /** \param minPoint [in]  Lower-left corner.
    \param maxPoint [in]  Upper-right corner.
    \param xMin [in]  Minimum X-coordinate.
    \param xMax [in]  Maximum X-coordinate.
    \param yMin [in]  Minimum Y-coordinate.
    \param yMax [in]  Maximum Y-coordinate.
  */
  OdGsDCRectDouble() { }

  OdGsDCRectDouble(const OdGePoint2d& minPoint, const OdGePoint2d& maxPoint)
    : m_min(minPoint), m_max(maxPoint) { }

  OdGsDCRectDouble(double xMin, double xMax, double yMin, double yMax)
    : m_min(xMin,yMin), m_max(xMax,yMax) { }

  explicit OdGsDCRectDouble(const OdGsDCRect& rc)
    : m_min(rc.m_min.x, rc.m_min.y), m_max(rc.m_max.x,rc.m_max.y) { }

  OdGsDCRectDouble& operator=(const OdGsDCRect& dcRect)
  {
    m_min.x = dcRect.m_min.x;
    m_min.y = dcRect.m_min.y;
    m_max.x = dcRect.m_max.x;
    m_max.y = dcRect.m_max.y;
    return *this;
  }
  bool operator==(const OdGsDCRectDouble& dcRect) const
  {
    return m_min == dcRect.m_min && m_max == dcRect.m_max; 
  }
  bool operator!=(const OdGsDCRectDouble& dcRect) const
  {
    return !(*this == dcRect);
  }

  OdGsDCRect round() const
  {
    return OdGsDCRect(OdGsDCRect(
        OdTruncateToLong(m_min.x),
        OdTruncateToLong(m_max.x),
        OdTruncateToLong(m_min.y),
        OdTruncateToLong(m_max.y)));
  }

  OdGePoint2d m_min;
  OdGePoint2d m_max;
};

typedef void* OdGsWindowingSystemID; // i.e. -- HWND

enum EMetafilePlayMode
{
  kMfDisplay, //play for visualization. Default, valid for all metafile types.
  //Following modes are valid only if metafile can be played as OdGiConveyorGeometry
  //( if useMetafileAsGeometry() returns true).
  kMfSelect, //play as geometry for selection
  kMfNested, //play data of nested metafiles only
  kMfExtents //play as geometry for extents calculation
};

// Negative GETBIT
#ifndef GETBITNEG
#define GETBITNEG(flags, bit) (((flags) & (bit)) != (bit))
#endif

#include "TD_PackPop.h"

#endif // __GSDEFS_H_INCLUDED_
