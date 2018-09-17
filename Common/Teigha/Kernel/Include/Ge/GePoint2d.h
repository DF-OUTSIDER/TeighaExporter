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


#ifndef OD_GEPNT2D_H
#define OD_GEPNT2D_H /*!DOM*/

#include "Ge/GeVector2d.h"
#include "OdArray.h"

#ifdef OD_HAVE_MATH_FILE
#include <math.h>
#endif

#ifdef OD_HAVE_COMPLEX_FILE
#include <complex>
#endif

class OdGeMatrix2d;
class OdGeVector2d;
class OdGeLinearEnt2d;
class OdGeLine2d;

#include "TD_PackPush.h"

/** \details
    This class represents points (locations) in 2D space. 

    \remarks
    OdGePoint2d may be viewed as an array[2] of doubles.
    
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>
    
    \sa
    <link ge_BasicTypes.html, Working with Basic Geometry Types>
*/
class GE_TOOLKIT_EXPORT OdGePoint2d
{
public:
  /** \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
    \param source [in]  Object to be cloned.

    \remarks
    The default constructor constructs a point at the origin.
  */
  OdGePoint2d()
    : x (0.0), y (0.0)
  {
  }
  OdGePoint2d(
    double xx, 
    double yy) 
    : x (xx), y (yy)
  {
  }

  GE_STATIC_EXPORT static const OdGePoint2d kOrigin; // Origin (0,0).

  /** \remarks
    Sets this point to the result of the matrix multiplication matrix * point,
    or the point scaled by a factor of scale. 

    \param matrix [in]  Any 2D matrix.
    \param scale [in]  Scale factor.
    \param point [in]  The point to be multiplied.
  */
  friend GE_TOOLKIT_EXPORT OdGePoint2d operator *(
    const OdGeMatrix2d& matrix, 
    const OdGePoint2d& point);

  friend GE_TOOLKIT_EXPORT OdGePoint2d operator *(
    double scale, 
    const OdGePoint2d& point)
  {
    return OdGePoint2d(point.x * scale, point.y * scale);
  }

  /** \details
    Sets this point to the result of the matrix multiplication (matrix * point).
    Returns a reference to this point.

    \param matrix [in]  A 2D matrix.
    \param point [in]  The point to be multiplied.
  */
  OdGePoint2d& setToProduct(
    const OdGeMatrix2d& matrix, 
    const OdGePoint2d& point);

  /** \details
    Sets this point to the result of the matrix multiplication xfm  this point*.
    Returns a reference to this point.

    \param xfm [in]  A 2D matrix.
  */
  OdGePoint2d& transformBy(
    const OdGeMatrix2d& xfm);

  /** \details
    Rotates this point about the specified basePoint by the specified angle. 

    \param angle [in]  Rotation angle.
    \param basePoint [in]  Basepoint.
  */
  OdGePoint2d& rotateBy(
    double angle, 
    const OdGePoint2d& basePoint = kOrigin);

  /** \details
    Mirrors this point about the specified 2D line.

    \param line [in]  Mirror Line.
  */
  OdGePoint2d& mirror(
    const OdGeLine2d& line);

  /** \details
    Scales this point by the scale factor about the basepoint.

    \param scaleFactor [in]  Scale Factor. The scale factor must be greater than zero.
    \param basePoint [in]  Basepoint.
  */
  OdGePoint2d& scaleBy(
    double scaleFactor, 
    const OdGePoint2d& basePoint = kOrigin);

  OdGePoint2d operator *(
    double scale) const
  {
    return OdGePoint2d (x*scale, y*scale);
  }

  OdGePoint2d& operator *=(
    double scale)
  {
    x *= scale;
    y *= scale;
    return *this;
  }

  OdGePoint2d operator /(
    double scale) const
  {
    return OdGePoint2d (x/scale, y/scale);
  }

  OdGePoint2d& operator /=(
    double scale)
  {
    x /= scale;
    y /= scale;
    return *this;
  }

  OdGePoint2d operator +(
    const OdGeVector2d& vect) const
  {
    return OdGePoint2d (x + vect.x, y + vect.y);
  }

  OdGePoint2d& operator +=(
    const OdGeVector2d& vect)
  {
    x += vect.x;
    y += vect.y;
    return *this;
  }

  OdGePoint2d operator -(
    const OdGeVector2d& vect) const
  {
    return OdGePoint2d (x - vect.x, y - vect.y);
  }

  OdGePoint2d& operator -=(
    const OdGeVector2d& vect)
  {
    x -= vect.x;
    y -= vect.y;
    return *this;
  }

  /** \details
    Sets this point to point + vect, and returns a reference to this point.

    \param point [in]  Any 2D point.
    \param vect [in]  A 2D vector.
  */
  OdGePoint2d& setToSum(
    const OdGePoint2d& point, 
    const OdGeVector2d& vect)
  {
    x = point.x + vect.x;
    y = point.y + vect.y;
    return *this;
  }

  OdGeVector2d operator -(
    const OdGePoint2d& point) const
  {
    return OdGeVector2d (x - point.x, y - point.y);
  }

  OdGeVector2d operator +(
    const OdGePoint2d& point) const
  {
    return OdGeVector2d(x + point.x, y + point.y);
  }

  /** \details
    Returns this point as a vector.
  */
  const OdGeVector2d& asVector() const
  {
    return *reinterpret_cast<const OdGeVector2d*>(this);
  }

  /** \details
    Returns the distance from this point to the specified point.

    \param point [in]  Any 2D point.
  */
  double distanceTo(
    const OdGePoint2d& point) const
  {
    return sqrt ( (x-point.x)* (x-point.x)+ (y-point.y)* (y-point.y));
  }

  bool operator ==(
    const OdGePoint2d& point) const
  {
    return isEqualTo (point);
  }
  bool operator !=(
    const OdGePoint2d& point) const
  {
    return !isEqualTo (point);
  }

  /** \details
    Returns true if and only if the distance from this point to point is within the 
    specified tolerance.

    \param point [in]  Any 2D point.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo(
    const OdGePoint2d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \param i [in]  Index of coordinate.

    \remarks
    Returns or references the ith coordinate of this point.
    
    * 0 returns or references the X-coordinate.
    * 1 returns or references the Y-coordinate.
  */
  double& operator [](
    unsigned int i)
  {
    return * (&x+i);
  }
  double operator [](
    unsigned int i) const
  {
    return * (&x+i);
  }

  /** \details
    Sets this point to the specified X and Y coordinates. 
    Returns a reference to this point.

    \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
  */
  OdGePoint2d& set(
    double xx, 
    double yy)
  {
    x = xx;
    y = yy;
    return *this;
  }

  double x; // X-coordinate
  double y; // Y-coordinate
};

#include "TD_PackPop.h"

#endif

