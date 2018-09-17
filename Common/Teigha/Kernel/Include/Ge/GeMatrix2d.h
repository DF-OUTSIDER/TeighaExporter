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




#ifndef OD_GE_MATRIX_2D_H
#define OD_GE_MATRIX_2D_H /*!DOM*/

#include "Ge/GeGbl.h"
#include "Ge/GePoint2d.h"

class OdGeVector2d;
class OdGeLine2d;
class OdGeTol;

#include "TD_PackPush.h"

/** \details
    This class represents 2D transformation matrices that define affine 
    ( translation, rotation, and/or scaling ) transformations.

    \remarks
    OdGeMatrix2d may be viewed as an array[3][3] of doubles.
    
    An OdGeMatrix2d, M, can be expressed as a 3  3 matrix*, in the form
    
              a00   a01   t0
              a10   a11   t1
                0     0    1
                
    The linear part of M is the matrix
                
              a00   a01
              a10   a11
                
    The translational part of M is the column
    
              t0
              t1
              
    The origin of the coordinate system of M is (t0, t1).
    

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeMatrix2d
{
public:
  OdGeMatrix2d();
  //OdGeMatrix2d(const OdGeMatrix2d& src);
  //OdGeMatrix2d& operator =(const OdGeMatrix2d& src);

  /** \details
      The identity matrix.
  */
  GE_STATIC_EXPORT static const OdGeMatrix2d kIdentity;

  /** \details
    Sets this matrix to the identity matrix, and returns a reference to this matrix.
  */
  OdGeMatrix2d& setToIdentity();

  /** \details
    Returns the product (this matrix) * matrix. 
  */
  OdGeMatrix2d operator* (
    const OdGeMatrix2d& matrix) const;
    
  /** \remarks
    Sets this matrix to the product (this matrix) * matrix, and returns
    a reference to this matrix. 
  */
  OdGeMatrix2d& operator*= (
    const OdGeMatrix2d& matrix);

  /** \details
    Sets this matrix to the product leftSide * (this matrix), and returns
    a reference to this matrix.
    
    \param leftSide [in]  Any 2D matrix 
  */
  OdGeMatrix2d& preMultBy(
    const OdGeMatrix2d& leftSide);

  /** \details
    Sets this matrix to the product (this matrix) * rightSide, and returns
    a reference to this matrix. 
    
    \param rightSide [in]  Any 2D matrix 
  */
  OdGeMatrix2d& postMultBy(
    const OdGeMatrix2d& rightSide);

  /** \details
    Sets this matrix to the product matrix1 * matrix2, and returns
    a reference to this matrix.
     
    \param matrix1 [in]  Any 2D matrix 
    \param matrix2 [in]  Any 2D matrix 
  */
  OdGeMatrix2d& setToProduct(
    const OdGeMatrix2d& matrix1, 
    const OdGeMatrix2d& matrix2);

  // Multiplicative inverse.
  //

  /** \details
    Sets this matrix to its inverse, and returns
    a reference to this matrix. 
  */
  OdGeMatrix2d& invert();

  /** \details
    Returns the inverse of this matrix. 
  */
  OdGeMatrix2d inverse() const;

  /** \details
    Returns true if and only if this matrix is singular.
    
    \remarks
    * A matrix is singular if and only if its determinant == 0.
    * A singular matrix cannot be inverted.
  */
  bool isSingular(const OdGeTol& tol = OdGeContext::gTol) const;
 
  /** \details
    Sets this matrix to its transpose, and returns
    a reference to this matrix. 
  */
  OdGeMatrix2d& transposeIt();

  /** \details
    Returns the transpose of this matrix. 
  */
  OdGeMatrix2d transpose() const;

  /** \details
    Equality operator. 
  */
  bool operator ==(
    const OdGeMatrix2d& matrix) const;

  /** \details
    Inequality operator. 
  */
  bool operator !=(
    const OdGeMatrix2d& matrix) const;

  /** \details
    Returns true if and only if matrix is identical to this one,
    within the specified tolerance.

    \param matrix [in]  Matrix to be compared.
    \param tol [in]  Geomentric tolerance.
  */
  bool isEqualTo(
    const OdGeMatrix2d& matrix,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only the columns vectors of the linear part of
    this matrix are of equal length and perpendicular
    to each other  within the
    specified tolerance. 

    \param tol [in]  Geomentric tolerance.
  */
  bool isUniScaledOrtho(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only the column vectors of the linear part of
    this matrix are perpendicular
    to each other within the specified tolerance.

    \param tol [in]  Geomentric tolerance.
  */
  bool isScaledOrtho(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the scale factor of this matrix.
    
    \remarks
    The scale factor is the square root of the longest column vector
    of the linear part of this matrix.
  */
  double scale() const;

  // Determinant
  //
  
  /** \details
    Returns the determinant of this matrix.
  */
  double det() const;

  /** \details
    Sets the translation part of the matrix to the specified vector.
    
    \param vect [in]  Translation vector.
  */
  OdGeMatrix2d& setTranslation(
    const OdGeVector2d& vect);

  /** \details
      Returns the translation vector of this matrix.
  */
  OdGeVector2d translation() const;

  /** \details
    Returns the matrix of the translation by vector.
    
    \param vector [in]  Translation vector.
  */
  static OdGeMatrix2d translation(
    const OdGeVector2d& vector);

  // Retrieve scaling, rotation, mirror components
  //
  
  /** \details
    Returns true if an only if this matrix is conformal (isUniScaledOrtho()),
    and returns the scale factor, angle of rotation,
    the presence of a mirror component to the matrix,
    and the direction of reflection.
          
    \param scale [out]  Receives the scale factor.
    \param angle [out]  Receives the angle of rotation.
    \param isMirror [out]  Receives true if andn only if the matrix has a mirror component.
    \param reflex [in]  Direction of reflection.
    
    \remarks
    reflex is valid if and only if isMirror is true.
  */
  bool isConformal(
    double& scale, 
    double& angle,
    bool& isMirror, 
    OdGeVector2d& reflex) const;


  // Set/get coordinate system
  //
  
  /** \details
    Sets this matrix to the matrix which maps
    the WCS to the coordinate system 
    defined by origin, X-axis, and Y-axis, and returns a reference
    to this matrix.
    
    \param origin [in]  Origin of the coordinate system.
    \param xAxis [in]  X-axis of the coordinate system.
    \param yAxis [in]  Y-axis of the coordinate system.
  */
  OdGeMatrix2d& setCoordSystem(
    const OdGePoint2d& origin, 
    const OdGeVector2d& xAxis,
    const OdGeVector2d& yAxis);
  /** \details
    Returns the origin, X-axis, and Y-axis of the coordinate system 
    to which this matrix maps the WCS.
    
    \param origin [out]  Receives the origin of the coordinate system.
    \param xAxis [out]  Receives the X-axis of the coordinate system.
    \param yAxis [out]  Receives the Y-axis of the coordinate system.
  */
  void getCoordSystem(
    OdGePoint2d& origin, 
    OdGeVector2d& xAxis, 
    OdGeVector2d& yAxis) const;
 
  /** \details
    Sets this matrix to the matrix which translates
    by vect, and returns a reference
    to this matrix.
    
    \param vect [in]  Translation vector.
  */
  OdGeMatrix2d& setToTranslation(
    const OdGeVector2d& vect);
    
    
  /** \details
    Sets this matrix to the matrix which rotates
    by angle about center, and returns a reference
    to this matrix.
    
    \param angle [in]  Rotation angle.
    \param center [in]  Center of rotation.
  */
  OdGeMatrix2d& setToRotation(
    double angle, 
    const OdGePoint2d& center = OdGePoint2d::kOrigin);
    
  /** \details
    Sets this matrix to the matrix which scales
    by scale about tcenter, and returns a reference
    to this matrix.
    
    \param scale [in]  Scale factor.
    \param center [in]  Center of scaling.
  */
  OdGeMatrix2d& setToScaling(
    double scale,
    const OdGePoint2d& center = OdGePoint2d::kOrigin);

  /** \details
    Sets this matrix to the matrix which mirrors
    about the specified object, and returns a reference
    to this matrix.
    
    \param mirrorPoint [in]  Mirror point.
  */
  OdGeMatrix2d& setToMirroring(
    const OdGePoint2d& mirrorPoint);

  /** \details
    Sets this matrix to the matrix which mirrors
    about the specified object, and returns a reference
    to this matrix.

    \param mirrorLine [in]  Mirror line entity.
  */
  OdGeMatrix2d& setToMirroring(
    const OdGeLine2d& mirrorLine);
    
  /** \details
    Sets this matrix to the matrix which maps
    the coordinate system defined by fromOrigin, fromXAxis, and fromYAxis,
    to the coordinate system 
    defined by toOrigin, toXAxis, and toYAxis, and returns a reference
    to this matrix.
    
    \param fromOrigin [in]  Origin of the initial coordinate system.
    \param fromXAxis [in]  X-axis of the initial coordinate system.
    \param fromYAxis [in]  Y-axis of the initial coordinate system.
    \param toOrigin [in]  Origin of the initial coordinate system.
    \param toXAxis [in]  X-axis of the initial coordinate system.
    \param toYAxis [in]  Y-axis of the initial coordinate system.
  */
  OdGeMatrix2d& setToAlignCoordSys(
    const OdGePoint2d& fromOrigin,
    const OdGeVector2d& fromXAxis,
    const OdGeVector2d& fromYAxis,
    const OdGePoint2d& toOrigin,
    const OdGeVector2d& toXAxis,
    const OdGeVector2d& toYAxis);
 
//  static OdGeMatrix2d translation (
//     const OdGeVector2d& vect);
    
  /** \details
    Returns the matrix which rotates
    by angle about center.
    
    \param angle [in]  Rotation angle.
    \param center [in]  Center of rotation.
  */
  static OdGeMatrix2d rotation(
    double angle, 
    const OdGePoint2d& center = OdGePoint2d::kOrigin);
    
  /** \details
    Returns the matrix which scales
    by scale about center.
    
    \param scale [in]  Scale factor.
    \param center [in]  Center of scaling.
  */
  static OdGeMatrix2d scaling(
    double scale, 
    const OdGePoint2d& center = OdGePoint2d::kOrigin);

  /** \details
    Returns the matrix which mirrors
    about the specified object.
   
    \param mirrorPoint [in]  Mirror point.
  */
  static OdGeMatrix2d mirroring(
    const OdGePoint2d& mirrorPoint);
  
  /** \details
    Returns the matrix which mirrors
    about the specified object.
   
    \param mirrorLine [in]  Mirror line entity.
  */
  static OdGeMatrix2d mirroring(
    const OdGeLine2d& mirrorLine);

  /** \details
    Returns the matrix which maps
    the  coordinate system defined by fromOrigin, fromXAxis, and fromYAxis,
    to the coordinate system 
    defined by toOrigin, toXAxis, and toYAxis.
    
    \param fromOrigin [in]  Origin of the initial coordinate system.
    \param fromXAxis [in]  X-axis of the initial coordinate system.
    \param fromYAxis [in]  Y-axis of the initial coordinate system.
    \param toOrigin [in]  Origin of the initial coordinate system.
    \param toXAxis [in]  X-axis of the initial coordinate system.
    \param toYAxis [in]  Y-axis of the initial coordinate system.
  */
  static OdGeMatrix2d alignCoordSys(
    const OdGePoint2d& fromOrigin,
    const OdGeVector2d& fromXAxis,
    const OdGeVector2d& fromYAxis,
    const OdGePoint2d& toOrigin,
    const OdGeVector2d& toXAxis,
    const OdGeVector2d& toYAxis);

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

  // [row][column]
  double entry[3][3];
};

// these operations really decrease rendering performance in non-inline case :

inline const double* OdGeMatrix2d::operator [](int row) const 
{ 
  return entry[row]; 
}

inline double* OdGeMatrix2d::operator [](int row) 
{ 
  return entry[row]; 
}

inline double OdGeMatrix2d::operator ()(int row, int column) const 
{ 
  return entry[row][column]; 
}

inline double& OdGeMatrix2d::operator ()(int row, int column) 
{ 
  return entry[row][column]; 
}

#include "TD_PackPop.h"

#endif // OD_GE_MATRIX_2D_H
