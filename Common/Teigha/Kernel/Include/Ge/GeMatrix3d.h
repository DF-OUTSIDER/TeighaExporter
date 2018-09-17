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


#ifndef OD_GE_MATRIX_3D_H
#define OD_GE_MATRIX_3D_H /*!DOM*/

#include "Ge/GeGbl.h"
#include "Ge/GeMatrix2d.h"
#include "Ge/GePoint3d.h"

class OdGeLine3d;
class OdGeVector3d;
class OdGePlane;
class OdGeTol;
class OdGeScale3d;

#include "TD_PackPush.h"

/** \details
    This class represents 3D transformation matrices that define affine 
    ( translation, rotation, and/or scaling ) transformations.
    
    \remarks
    OdGeMatrix3d may be viewed as an array[4][4] of doubles.
    
    An OdGeMatrix3d, M, can be expressed as a 4  4 matrix*, in the form
    
              a00   a01   a02   t0
              a10   a11   a12   t1
              a20   a21   a22   t2
                0     0     0    1
                
    The linear part of M is the matrix
                
              a00   a01   a02
              a10   a11   a12
              a20   a21   a22
                              
    The translational part of M is the column
    
              t0
              t1
              t2
              
    The origin of the coordinate system of M is (t0, t1, t2).

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeMatrix3d
{
public:
  OdGeMatrix3d();
  //OdGeMatrix3d(const OdGeMatrix3d& src);
  //OdGeMatrix3d& operator =(const OdGeMatrix3d& src);

  GE_STATIC_EXPORT static const OdGeMatrix3d kIdentity; // Multiplicative identity matrix.

  // Reset matrix.
  //

  /** \details
    Sets this matrix to the identity matrix, and returns a reference to this matrix.
  */
  OdGeMatrix3d& setToIdentity();
  
  /** \details
    Sets any values within tol of 0 to 0.
  */
  void validateZero(
    const OdGeTol& tol = OdGeContext::gTol); // for correcting in case that Zero is corrupted

  // Multiplication.
  //

  /** \details
    Returns the product (this matrix) * matrix. 
  */
  OdGeMatrix3d operator *(
    const OdGeMatrix3d& matrix) const;

  /** \details
    Sets this matrix to the product (this matrix) * matrix, and returns
    a reference to this matrix. 
  */
  OdGeMatrix3d& operator *=(
    const OdGeMatrix3d& matrix);
  
  /** \details
    Sets this matrix to the product leftSide * (this matrix), and returns
    a reference to this matrix. 
    
    \param leftSide [in]  Any 2D matrix 
  */
  OdGeMatrix3d& preMultBy(
    const OdGeMatrix3d& leftSide);

  /** \details
    Sets this matrix to the product (this matrix) * rightSide, and returns
    a reference to this matrix. 
    
    \param rightSide [in]  Any 2D matrix 
  */
  OdGeMatrix3d& postMultBy(
    const OdGeMatrix3d& rightSide);

  /** \details
    Sets this matrix to the product matrix1 * matrix2, and returns
    a reference to this matrix. 
    
    \param matrix1 [in]  Any 2D matrix 
    \param matrix2 [in]  Any 2D matrix 
  */
  OdGeMatrix3d& setToProduct(
    const OdGeMatrix3d& matrix1, 
    const OdGeMatrix3d& matrix2);

  // Multiplicative inverse.
  //

  /** \details
    Sets this matrix to its inverse, and returns
    a reference to this matrix. 
  */
  OdGeMatrix3d& invert();


  OdGeMatrix3d inverse() const;
  OdGeMatrix3d inverse(
    const OdGeTol& tol) const;

  /** \details
    Returns true if and only if this matrix has an inverse,
    and the inverse of this matrix.
    
    \param inverseMatrix [out]  Receives the inverse matrix.
    \param tol [in]  Geometric tolerance. 
  */
  bool inverse(
    OdGeMatrix3d& inverseMatrix, 
    double tol) const;

  /** \details
    Returns true if and only if this matrix is singular.
    
    \remarks
    * A matrix is singular if and only if its determinant == 0.
    * A singular matrix cannot be inverted.
  */
  bool isSingular(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Sets this matrix to its transpose, and returns
    a reference to this matrix. 
  */
  OdGeMatrix3d& transposeIt();

  /** \details
    Returns the transpose of this matrix. 
  */
  OdGeMatrix3d transpose() const;

  /** \details
    Equality operator. 
  */
  bool operator ==(
    const OdGeMatrix3d& matrix) const;

  /** \details
    Inequality operator. 
  */
  bool operator !=(
    const OdGeMatrix3d& matrix) const;

  /** \details
    Returns true if and only if matrix is identical to this one,
    within the specified tolerance.

    \param matrix [in]  Matrix to be compared.
    \param tol [in]  Geomentric tolerance.
  */
  bool isEqualTo(
    const OdGeMatrix3d& matrix, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only the columns vectors of the linear part of
    this matrix are of equal length and mutually perpendicular within the
    specified tolerance. 

    \param tol [in]  Geomentric tolerance.
  */
  bool isUniScaledOrtho(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only the column vectors of the linear part of
    this matrix are mutually perpendicular within the
    specified tolerance.
    
    \param tol [in]  Geomentric tolerance.
  */
  bool isScaledOrtho(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /*!DOM*/
  bool isPerspective(
    const OdGeTol& tol = OdGeContext::gTol) const;
 
  /** \details
    Returns the determinant of this matrix.
  */
  double det() const;

  /** \details
    Sets the translation part of the matrix to the specified vector.
    
    \param vect [in]  Translation vector.
  */
  OdGeMatrix3d& setTranslation(
    const OdGeVector3d& vect);
    
  /** \details
    Sets this matrix to the matrix which maps
    the WCS to the coordinate system 
    defined by origin, X-axis, Y-axis, and Z-axis, and returns a reference
    to this matrix.
    
    \param origin [in]  Origin of the coordinate system.
    \param xAxis [in]  X-axis of the coordinate system.
    \param yAxis [in]  Y-axis of the coordinate system.
    \param zAxis [in]  Z-axis of the coordinate system.
  */
  OdGeMatrix3d& setCoordSystem(
    const OdGePoint3d& origin, 
    const OdGeVector3d& xAxis,
    const OdGeVector3d& yAxis, 
    const OdGeVector3d& zAxis);

  /** \details
    Returns the origin, X-axis, Y-axis, and Z-axis of the coordinate system 
    to which this matrix maps the WCS.
    
    \param origin [out]  Receives the origin of the coordinate system.
    \param xAxis [out]  Receives the X-axis of the coordinate system.
    \param yAxis [out]  Receives the Y-axis of the coordinate system.
    \param zAxis [out]  Receives the Z-axis of the coordinate system.
  */
  void getCoordSystem(
    OdGePoint3d& origin, 
    OdGeVector3d& xAxis, 
    OdGeVector3d& yAxis, 
    OdGeVector3d& zAxis) const;
  
  /** \details
    Returns the origin of the coordinate system 
    to which this matrix maps the WCS.
  */
    OdGePoint3d getCsOrigin() const;

  /** \details
    Returns the X-axis of the coordinate system 
    to which this matrix maps the WCS.
  */
    OdGeVector3d getCsXAxis() const;

  /** \details
    Returns the Y-axis of the coordinate system 
    to which this matrix maps the WCS.
  */
    OdGeVector3d getCsYAxis() const;
  /** \details
    Returns the Z-axis of the coordinate system 
    to which this matrix maps the WCS.
  */
    OdGeVector3d getCsZAxis() const;
 
  /** \details
    Sets this matrix to the matrix which translates
    by vect, and returns a reference
    to this matrix.
    
    \param vect [in]  Translation vector.
  */
  OdGeMatrix3d& setToTranslation(
    const OdGeVector3d& vect);

  /** \details
    Sets this matrix to the matrix which rotates
    by angle about an axis passing through center, and returns a reference
    to this matrix.
    
    \param angle [in]  Rotation angle.
    \param axis [in]  Rotation axis.
    \param center [in]  Center of rotation.
  */
  OdGeMatrix3d& setToRotation(
    double angle, 
    const OdGeVector3d& axis,
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Sets this matrix to the matrix which scales
    by scale about center, and returns a reference
    to this matrix.
    
    \param scale [in]  Scale factor.
    \param center [in]  Center of scaling.
  */
  OdGeMatrix3d& setToScaling(
    double scale, 
    const OdGePoint3d& center = OdGePoint3d::kOrigin);
  
  /** \details
    Sets this matrix to the matrix which scales
    by scale about center, and returns a reference
    to this matrix.
    
    \param scale [in]  Scale factor.
    \param center [in]  Center of scaling.
  */ 
 OdGeMatrix3d& setToScaling(
    const OdGeScale3d& scale, 
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Sets this matrix to the matrix which mirrors
    about the specified object, and returns a reference
    to this matrix.
    
    \param mirrorPlane [in]  Mirror plane.
  */
  OdGeMatrix3d& setToMirroring(
    const OdGePlane& mirrorPlane);

  /** \details
    Sets this matrix to the matrix which mirrors
    about the specified object, and returns a reference
    to this matrix.

    \param mirrorPoint [in]  Mirror point.
  */
  OdGeMatrix3d& setToMirroring(
    const OdGePoint3d& mirrorPoint);

  /** \details
    Sets this matrix to the matrix which mirrors
    about the specified object, and returns a reference
    to this matrix.
    
    \param mirrorLine [in]  Mirror line entity.
  */
  OdGeMatrix3d& setToMirroring(
    const OdGeLine3d& mirrorLine);
    
  /** \details
    Sets this matrix to the matrix which parallel
    projects onto the specified plane in the 
    specified directionm and returns a reference
    to this matrix.
   
    \param projectionPlane [in]  Projection plane.
    \param projectDir [in]  Projection direction.
  */
  OdGeMatrix3d& setToProjection(
    const OdGePlane& projectionPlane,
    const OdGeVector3d& projectDir);
    
  /** \details
    Sets this matrix to the matrix which maps
    the coordinate system defined by fromOrigin, fromXAxis, fromYAxis, and fromZAxis,
    to the coordinate system 
    defined by toOrigin, toXAxis, toYAxis, and toZAxis, and returns a reference
    to this matrix.
    
    \param fromOrigin [in]  Origin of the initial coordinate system.
    \param fromXAxis [in]  X-axis of the initial coordinate system.
    \param fromYAxis [in]  Y-axis of the initial coordinate system.
    \param fromZAxis [in]  Z-axis of the initial coordinate system.
    \param toOrigin [in]  Origin of the initial coordinate system.
    \param toXAxis [in]  X-axis of the initial coordinate system.
    \param toYAxis [in]  Y-axis of the initial coordinate system.
    \param toZAxis [in]  Z-axis of the initial coordinate system.
  */
  OdGeMatrix3d& setToAlignCoordSys(
    const OdGePoint3d& fromOrigin,
    const OdGeVector3d& fromXAxis,
    const OdGeVector3d& fromYAxis,
    const OdGeVector3d& fromZAxis,
    const OdGePoint3d& toOrigin,
    const OdGeVector3d& toXAxis,
    const OdGeVector3d& toYAxis,
    const OdGeVector3d& toZAxis);


  /** \details
    Sets this matrix to the matrix which maps the WCS to
    the plane coordinate system, and returns a reference to this matrix.
    
    \param normal [in]  The normal to the plane.    
    
    \remarks
    If normal is specified, the origin of the plane is (0,0,0).    
  */
      OdGeMatrix3d& setToWorldToPlane(
    const OdGeVector3d& normal);

  /** \details
    Sets this matrix to the matrix which maps the WCS to
    the plane coordinate system, and returns a reference to this matrix.
    
    \param plane [in]  The plane.
  */
  OdGeMatrix3d& setToWorldToPlane(
    const OdGePlane& plane);
    
  /** \details
    Sets this matrix to the matrix which maps
    the plane coordinate system to the WCS, and returns a reference to this matrix.
    
    \param normal [in]  The normal to the plane.    
    
    \remarks
    If normal is specified, the origin of the plane is (0,0,0).    
  */
      OdGeMatrix3d& setToPlaneToWorld(
    const OdGeVector3d& normal);
  
  /** \details
    Sets this matrix to the matrix which maps
    the plane coordinate system to the WCS, and returns a reference to this matrix.
    
    \param plane [in]  The plane. 
  */
  OdGeMatrix3d& setToPlaneToWorld(
    const OdGePlane& plane);

  /** \details
    Returns the matrix of the translation by vect.
    
    \param vect [in]  Translation vector.
  */
  static OdGeMatrix3d translation(
    const OdGeVector3d& vect);
 
  /** \details
    Returns the translation vector of this matrix.
  */
  OdGeVector3d translation() const;
  
  /** \details
    Returns the matrix which rotates
    by angle about an axis passing through center, and returns a reference
    to this matrix.
    
    \param angle [in]  Rotation angle.
    \param axis [in]  Rotation axis.
    \param center [in]  Center of rotation.
  */
  static OdGeMatrix3d rotation(
    double angle, 
    const OdGeVector3d& axis,
    const OdGePoint3d& center = OdGePoint3d::kOrigin);
    
  /** \details
    Returns the matrix which scales
    by scale about center, and returns a reference
    to this matrix.
    
    \param scale [in]  Scale factor.
    \param center [in]  Center of scaling.
  */
  static OdGeMatrix3d scaling(
    double scale, 
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Returns the matrix which scales
    by scale about center, and returns a reference
    to this matrix.
    
    \param scale [in]  Scale factor.
    \param center [in]  Center of scaling.
  */
  static OdGeMatrix3d scaling(
    const OdGeScale3d& scale, 
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Sets this matrix to the matrix which mirrors
    about the specified object, and returns a reference
    to this matrix.
    
    \param mirrorPlane [in]  Mirror plane.
  */
  static OdGeMatrix3d mirroring(
    const OdGePlane& mirrorPlane);

  /** \details
    Sets this matrix to the matrix which mirrors
    about the specified object, and returns a reference
    to this matrix.
    
    \param mirrorPoint [in]  Mirror point.
  */  
  static OdGeMatrix3d mirroring(
    const OdGePoint3d& mirrorPoint);

  /** \details
    Sets this matrix to the matrix which mirrors
    about the specified object, and returns a reference
    to this matrix.

    \param mirrorLine [in]  Mirror line entity.
  */
  static OdGeMatrix3d mirroring(
    const OdGeLine3d& mirrorLine);

  /** \details
    Returns the matrix which parallel
    projects onto the specified plane in the 
    specified direction.
   
    \param projectionPlane [in]  Projection plane.
    \param projectDir [in]  Projection direction.
  */
  static OdGeMatrix3d projection(
    const OdGePlane& projectionPlane,
    const OdGeVector3d& projectDir);

  /** \details
    Returns the matrix which maps
    the coordinate system defined by fromOrigin, fromXAxis, fromYAxis, and fromZAxis,
    to the coordinate system 
    defined by toOrigin, toXAxis, toYAxis, and toZAxis.
    
    \param fromOrigin [in]  Origin of the initial coordinate system.
    \param fromXAxis [in]  X-axis of the initial coordinate system.
    \param fromYAxis [in]  Y-axis of the initial coordinate system.
    \param fromZAxis [in]  Z-axis of the initial coordinate system.
    \param toOrigin [in]  Origin of the initial coordinate system.
    \param toXAxis [in]  X-axis of the initial coordinate system.
    \param toYAxis [in]  Y-axis of the initial coordinate system.
    \param toZAxis [in]  Z-axis of the initial coordinate system.
  */
  static OdGeMatrix3d alignCoordSys(
    const OdGePoint3d& fromOrigin,
    const OdGeVector3d& fromXAxis,
    const OdGeVector3d& fromYAxis,
    const OdGeVector3d& fromZAxis,
    const OdGePoint3d& toOrigin,
    const OdGeVector3d& toXAxis,
    const OdGeVector3d& toYAxis,
    const OdGeVector3d& toZAxis);

  /** \details
    Returns the matrix which maps the WCS to
    the plane coordinate system.
    
    \param normal [in]  The normal to the plane.    
    
    \remarks
    If normal is specified, the origin of the plane is (0,0,0).   
  */
      static OdGeMatrix3d worldToPlane(
    const OdGeVector3d& normal);
  
  /** \details
    Returns the matrix which maps the WCS to
    the plane coordinate system.
    
    \param plane [in]  The plane.   
  */
  static OdGeMatrix3d worldToPlane(
    const OdGePlane&);
    
  /** \details
    Returns the matrix which maps
    the plane coordinate system to the WCS.
    
    \param normal [in]  The normal to the plane.    
    
    \remarks
    If normal is specified, the origin of the plane is (0,0,0)    
  */
      static OdGeMatrix3d planeToWorld(
    const OdGeVector3d& normal);

  /** \details
    Returns the matrix which maps
    the plane coordinate system to the WCS.
      
    \param plane [in]  The plane.   
  */
  static OdGeMatrix3d planeToWorld(
    const OdGePlane&);

  /** \details
    Returns the scale factor of this matrix.
    
    \remarks
    The scale factor is the square root of the longest column vector
    of the linear part of this matrix.
  */
  double scale() const;

  /** \details
    Returns the largest absolute value of the linear part of this matrix.
  */
  double norm() const;
  
  /** \details
    Transforms the plane specified by a normal and an elevation. 
    \remarks
    Returns the normal and elevation of the transformed plane, as well as 
    a 2D transformation matrix for transforming points in the
    original plane coordinates to the transformed plane coordinates.

    \param normal [in/out] Normal to the plane.
    \param elevation [in/out] Elevation of the plane.
  */
  OdGeMatrix2d convertToLocal(
    OdGeVector3d& normal, 
    double& elevation) const;

  // For convenient access to the data.
  //
  
  /** \details
    Returns or references entry[row] as matrix[row].

    \param row [in]  Row.
  */
  const double* operator [](
    int row) const;
  double* operator [](
    int row);

  /** \details
    Returns or references entry[row][column] as matrix(row,column).

    \param row [in]  Row.
    \param column [in]  Column.
  */
  double operator ()(
    int row, 
    int column) const;
  double& operator ()(
    int row, 
    int column);

  double entry[4][4]; 
};

// these operations really decrease rendering performance in non-inline case

inline const double* OdGeMatrix3d::operator [](int row) const 
{ 
  return entry[row]; 
}

inline double* OdGeMatrix3d::operator [](int row) 
{ 
  return entry[row]; 
}

inline double OdGeMatrix3d::operator ()(int row, int column) const 
{ 
  return entry[row][column]; 
}

inline double& OdGeMatrix3d::operator ()(int row, int column) 
{ 
  return entry[row][column]; 
}

#include "TD_PackPop.h"

#endif // OD_GE_MATRIX_3D_H
