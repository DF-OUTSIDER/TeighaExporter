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


#ifndef OD_GEPNT3D_H
#define OD_GEPNT3D_H /*!DOM*/

#include "Ge/GePoint2d.h"
#include "OdArray.h"

class OdGeMatrix3d;
class OdGeLinearEnt3d;
class OdGePlane;
class OdGePlanarEnt;
class OdGeVector3d;
class OdGePoint2d;

#include "TD_PackPush.h"

/** \details
    This class represents points (locations) in 3D space. 

    \remarks
    OdGePoint3d may be viewed as an array[3] of doubles.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_BasicTypes.html, Working with Basic Geometry Types>
*/
class GE_TOOLKIT_EXPORT OdGePoint3d
{
public:
  /** \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
    \param zz [in]  The Z-coordinate of this point.
    \param plane [in]  Any plane.
    \param pnt2d [in]  Any 2D point.
    \param source [in]  Object to be cloned.
    
    \remarks
    plane and pnt2d define a point as follows:
            
            p + uAxis * pnt2d.x + vAxis * pnt2d.y,
            
    where p, uAxis, and vAxis are returned by the function call plane.get (p, uAxis, vAxis);
    
    With no arguments, creates a point at the origin.
  */
  OdGePoint3d() 
    : x (0.0), y (0.0), z (0.0)
  {
  }
  OdGePoint3d(
    double xx, 
    double yy, 
    double zz)
    : x (xx), y (yy), z (zz)
  {
  }
  
  OdGePoint3d(
    const OdGePlanarEnt& plane, 
    const OdGePoint2d& pnt2d);

  GE_STATIC_EXPORT static const OdGePoint3d kOrigin; // Origin (0,0,0).

  friend GE_TOOLKIT_EXPORT OdGePoint3d operator *(
    const OdGeMatrix3d& matrix, 
    const OdGePoint3d& point);
    
  friend GE_TOOLKIT_EXPORT OdGePoint3d operator *(
    double scale, 
    const OdGePoint3d& point)
  {
    return OdGePoint3d(point.x * scale, point.y * scale, point.z * scale);
  }

  /** \details
    Sets this point to the product of xfm * point, and returns a reference to this point.
    
    \param xfm [in]  A 3D matrix. 
    \param point [in]  Any 3D point. 
  */
  OdGePoint3d& setToProduct(
    const OdGeMatrix3d& xfm, 
    const OdGePoint3d& point);

  /** \details
    Sets this point to the result of the matrix multiplication xfm  this point*.
    Returns a reference to this point.

    \param xfm [in]  A 3D matrix.
  */
  OdGePoint3d& transformBy(
    const OdGeMatrix3d& xfm);
 
  /** \details
    Rotates this point about the specified basePoint and axis of rotation by the specified angle. 

    \param angle [in]  Rotation angle.
    \param basePoint [in]  Basepoint.
    \param vect [in]  Axis of rotation.
  */
  OdGePoint3d& rotateBy(
    double angle, 
    const OdGeVector3d& vect, 
    const OdGePoint3d& basePoint = OdGePoint3d::kOrigin);

  /** \details
    Mirrors this point about the specified plane.

    \param line [in]  Mirror Line.
  */
  OdGePoint3d& mirror(
    const OdGePlane& plane);

  /** \details
    Scales this point by the scale factor about the basepoint.

    \param scaleFactor [in]  Scale Factor. The scale factor must be greater than zero.
    \param basePoint [in]  Basepoint.
  */
  OdGePoint3d& scaleBy(
    double scaleFactor, 
    const OdGePoint3d& basePoint = OdGePoint3d::kOrigin);

  /** \details
    Projects this point onto the specified plane.
    
    \param plane [in]  Any plane.
    
    Remarks,
    If no plane is specified, this point is projected into the XY plane.
  */
  OdGePoint2d convert2d (
    const OdGePlanarEnt& plane) const;
   
  OdGePoint2d convert2d() const
  {
    return OdGePoint2d(x, y);
  }

  OdGePoint3d operator *(
    double scale) const
  {
     return OdGePoint3d (x * scale, y * scale, z * scale);
  }
  
  OdGePoint3d& operator *=(
    double scale)
  {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
  }
      
  OdGePoint3d operator /(
    double scale) const
  {
    return OdGePoint3d (x / scale, y / scale, z / scale);
  }
    
  OdGePoint3d& operator /=(
    double scale)
  {
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
  }
  
  OdGePoint3d operator +(
    const OdGeVector3d& vect) const;
    
  OdGePoint3d& operator +=(
    const OdGeVector3d& vect);
      
  OdGePoint3d operator -(
    const OdGeVector3d& vect) const;
      
  OdGePoint3d& operator -=(
    const OdGeVector3d& vect);
      
  /** \details
    Sets this point to point + vect, and returns a reference to this point.
    
    \param point [in]  Any 3D point.
    \param vect [in]  Any 3D vector.
  */
  OdGePoint3d& setToSum(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);

  OdGeVector3d operator -(
    const OdGePoint3d& point) const;
    
  /** \details
    Returns this point as a vector.
  */
  const OdGeVector3d& asVector() const;

  /** \details
  Returns this point as a vector.
  */
  OdGeVector3d& asVector();

  /** \details
    Returns the distance from this point to the specified point.

    \param point [in]  Any 3D point.
  */
  double distanceTo(
    const OdGePoint3d& point) const;

  /** \details
    Projects this point onto the specified plane along the specified vector.

    \param plane [in]  Plane on which this point is to be projected
    \param vect [in]  Vector defining the projection direction.
  */
  OdGePoint3d project(
    const OdGePlane& plane, 
    const OdGeVector3d& vect) const;
    
  /** \details
    Projects this point onto the specified plane along the normal to the plane.

    \param plane [in]  Plane on which this point is to be projected
  */
    OdGePoint3d orthoProject(
    const OdGePlane& plane) const;

  bool operator ==(
    const OdGePoint3d& point) const
  {
    return isEqualTo (point);
  }
    
  bool operator !=(
    const OdGePoint3d& point) const
  {
    return !isEqualTo (point);
  }
    
  /** \details
    Returns true if and only if the distance from this point to point is within the 
    specified tolerance.

    \param point [in]  Any 3D point.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \param i [in]  Index of coordinate.
    
    \details
    Returns or references the ith coordinate of this point.
\remarks
    * 0 returns or references the X-coordinate.
    * 1 returns or references the Y-coordinate.
    * 2 returns or references the Z-coordinate.
  */
  double operator [](
    unsigned int i) const
  {
    return * (&x+i);
  }
      
  double& operator [](
    unsigned int i)
  {
    return * (&x+i);
  }
  
  /** \details
    Sets the parameters for this point according to the arguments
    
    \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
    \param zz [in]  The Z-coordinate of this point.
    
    \remarks
    Returns a reference to this point.
\remarks
    plane and pnt2d define a point as follows:
            
            p + uAxis * point.x + vAxis * point.y,
            
    where p, uAxis, and vAxis are returned by the function call plane.get (p, uAxis, vAxis);
  */
  OdGePoint3d& set(
    double xx, 
    double yy, 
    double zz)
  {
    x = xx;
    y = yy;
    z = zz;
    return *this;
  }

  OdGePoint3d& set(
    const OdGePoint2d &xy, 
    double zz = 0.0)
  {
    x = xy.x;
    y = xy.y;
    z = zz;
    return *this;
  }

 /** \details
    Sets the parameters for this point according to the arguments
    
    \param plane [in]  Any plane.
    \param point [in]  Any 2D point.
    
    \remarks
    Returns a reference to this point.
\remarks
    plane and pnt2d define a point as follows:
            
            p + uAxis * point.x + vAxis * point.y,
            
    where p, uAxis, and vAxis are returned by the function call plane.get (p, uAxis, vAxis);
  */  
  OdGePoint3d& set(
    const OdGePlanarEnt& plane, 
    const OdGePoint2d& point);

  double x; // X-coordinate
  double y; // Y-coordinate
  double z; // Z-coordinate

  OdGePoint3d& swapWithPoint(
    OdGePoint3d& point);
};

#include "TD_PackPop.h"


#include "Ge/GeVector3d.h"

inline OdGePoint3d OdGePoint3d::operator +(
  const OdGeVector3d& vect) const
{
  return OdGePoint3d(x + vect.x, y + vect.y, z + vect.z);
}

inline OdGePoint3d& OdGePoint3d::operator +=(
  const OdGeVector3d& vect)
{
  x += vect.x;
  y += vect.y;
  z += vect.z;
  return *this;
}

inline OdGePoint3d OdGePoint3d::operator -(
  const OdGeVector3d& vect) const
{
  return OdGePoint3d(x - vect.x, y - vect.y, z - vect.z);
}

inline OdGePoint3d& OdGePoint3d::operator -=(
  const OdGeVector3d& vect)
{
  x -= vect.x;
  y -= vect.y;
  z -= vect.z;
  return *this;
}

inline OdGeVector3d OdGePoint3d::operator -(
  const OdGePoint3d& point) const
{
  return OdGeVector3d(x - point.x, y - point.y, z - point.z);
}

inline const OdGeVector3d& OdGePoint3d::asVector() const
{
  ODA_ASSUME(sizeof(OdGePoint3d) == 3 * sizeof(double));
  ODA_ASSUME(sizeof(OdGeVector3d) == 3 * sizeof(double));
  return *reinterpret_cast<const OdGeVector3d*>(this);
}

inline OdGeVector3d& OdGePoint3d::asVector()
{
  ODA_ASSUME(sizeof(OdGePoint3d) == 3 * sizeof(double));
  ODA_ASSUME(sizeof(OdGeVector3d) == 3 * sizeof(double));
  return *reinterpret_cast<OdGeVector3d*>(this);
}

#endif

