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




#ifndef OD_GEVEC2D_H
#define OD_GEVEC2D_H /*!DOM*/

#include "Ge/GeGbl.h"
#include "Ge/GeTol.h"

#include "TD_PackPush.h"

class OdGeMatrix2d;

/** \details
    This class represents vectors in 2D space. 

    \remarks
    OdGeVector2d may be viewed as an array[2] of doubles.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_BasicTypes.html, Working with Basic Geometry Types>
*/
class GE_TOOLKIT_EXPORT OdGeVector2d
{
public:
  /** \param source [in]  Object to be cloned.
    \param xx [in]  X-coordinate.
    \param yy [in]  Y-coordinate.
    
    \remarks
    If called with no arguments, constructs a zero-length vector.
  */
  OdGeVector2d () : x (0.0), y (0.0) {}
  OdGeVector2d (
    double xx, 
    double yy) : x (xx), y (yy) {}

  GE_STATIC_EXPORT static const OdGeVector2d kIdentity; // Additive identity vector.
  GE_STATIC_EXPORT static const OdGeVector2d kXAxis;    // X-Axis vector.
  GE_STATIC_EXPORT static const OdGeVector2d kYAxis;    // Y-Axis vector.

  friend GE_TOOLKIT_EXPORT OdGeVector2d operator* (
    const OdGeMatrix2d& xfm, 
    const OdGeVector2d& vect);
    

  /** \details
    Sets this vector to the product matrix * vect  scale * vect, and returns
    a reference to this vector. 
    
    \param matrix [in]  Any 2D matrix. 
    \param vect [in]  Any 2D vector 
  */
  OdGeVector2d& setToProduct (
    const OdGeMatrix2d& matrix, 
    const OdGeVector2d& vect)
  {
    *this = matrix * vect;
    return *this;
  }

  /** \details
    Sets this vector to the product matrix * vect  scale * vect, and returns
    a reference to this vector. 
    
    \param vect [in]  Any 2D vector. 
    \param scale [in]  Scale factor.
  */
  OdGeVector2d& setToProduct (
    const OdGeVector2d& vect, 
    double scale);
      
  /** \details
    Applies the 2D transformation matrix to this vector,
    and returns a reference to this vector.

    \param xfm [in]  2D transformation matrix.
  */
  OdGeVector2d& transformBy (
    const OdGeMatrix2d& xfm)  { return *this = xfm * *this;}
    
  /** \details
    Rotates this vector the specified angle, and returns
    a reference to this vector. 

    \param angle [in]  Rotation angle.
  */
  OdGeVector2d& rotateBy (
    double angle);
    
  /** \details
    Mirrors this vector about the specified 2D line
    and returns a reference to this vector.

    \param line [in]  Mirror Line.
  */
  OdGeVector2d& mirror (
    const OdGeVector2d& line);

  OdGeVector2d operator* (
    double scale) const;
  friend GE_TOOLKIT_EXPORT OdGeVector2d operator* (
    double scale, const OdGeVector2d& vector);

  OdGeVector2d& operator*= (
    double scale);
    
  OdGeVector2d  operator/ (
    double scale) const;
  OdGeVector2d& operator/= (double scale);

  OdGeVector2d operator+ (
    const OdGeVector2d& vect) const;
  OdGeVector2d& operator+= (
    const OdGeVector2d& vect);
  OdGeVector2d operator- (
    const OdGeVector2d& vect) const;
  OdGeVector2d& operator-= (
    const OdGeVector2d& vect);
    
  /** \details
    Sets this vector to vector1 + vector1, and returns a reference to this vector.
    
    \param vector1 [in]  Any 2D vector.
    \param vector2 [in]  Any 2D vector.
  */
    OdGeVector2d& setToSum (
    const OdGeVector2d& vector1, 
    const OdGeVector2d& vector2);
    
  OdGeVector2d operator- () const;
  
  /** \details
    Negates this vector (-x, -y), and returns a reference to this vector.
  */
  OdGeVector2d& negate ();

  /** \details
    Returns the vector perpendicular to this one (-y, x).
  */
  OdGeVector2d perpVector () const;

  /** \details
    Returns the angle between this vector and the X-axis
    
    \remarks
    Returns a value in the range [0.0 .. Oda2PI].
  */
  double angle () const;

  /** \details
    Returns the smallest angle to the specified vector.
    
    \param vect [in]  Any 2D vector.
    
    \remarks
    Returns a value in the range [0.0 .. Oda2PI].
    
    This function is commutative:
    
        a.angleTo (b) == b.angleTo (a)
  */
  double angleTo (
    const OdGeVector2d& vect) const;

  /** \details
    Returns the angle from this vector to the specified vector in the counterclockwise direction.
    
    \param vect [in]  Any 2D vector.
    
    \remarks
    Returns a value in the range [0.0 .. Oda2PI].
  */
  double angleToCCW (
    const OdGeVector2d& vect) const
  {
    return crossProduct (vect) > 0.0 ? angleTo(vect) : -angleTo(vect);
  }
    
  /** \details
    Returns the unit vector codirectional with this vector.
    
    \param tol [in]  Geometric tolerance.
        
    \remarks
    If the length() <= tol, this vector is returned.
  */
  OdGeVector2d normal (
    const OdGeTol& tol = OdGeContext::gTol) const;
    
  /** \details
    Sets this vector to the unit vector codirectional with this vector,
    and returns a reference to this vector
    
    \param tol [in]  Geometric tolerance.

    \remarks
    If this.length() <= tol, this vector is unchanged.
  */
  OdGeVector2d& normalize (
    const OdGeTol& tol = OdGeContext::gTol);
  
  /** \details
    Sets this vector to the unit vector codirectional with this vector,
    and returns a reference to this vector
    
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of normalization.

    \remarks
    If this.length() <= tol, this vector is unchanged, and kThis is returned in status.

    Possible values for status are as follows:

    @untitled table
    kOk 
    k0This
  */
  OdGeVector2d& normalize (
    const OdGeTol& tol, 
    OdGeError& status);
    
  /** \details
    Sets this vector to the unit vector codirectional with this vector,
    and returns the length prior to normalization.

    \param tol [in]  Geometric tolerance.

    \remarks
    If this.length() <= tol, this vector is unchanged and zero length is returned.
  */
   double normalizeGetLength (double tol = 1.e-300);

  /** \details
    Returns the length of this vector.
  */
        double length () const;
  
  /** \details
    Returns the square of the length of this vector.
  */
        double lengthSqrd () const;
  
  /** \details
    Returns true if and only if the length of this vector is 1.0 within the specified tolerance.

    \param tol [in]  Geometric tolerance.
  */
        bool isUnitLength (
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the length of this vector is 0.0 within the specified tolerance.

    \param tol [in]  Geometric tolerance.
  */
        bool isZeroLength (
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified vector is parallel to this vector within the specified tolerance.

    \param vect [in]  Any 2D vector.
    \param tol [in]  Geometric tolerance.
  */
        bool isParallelTo (
    const OdGeVector2d& vect,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified vector is parallel to this vector within the specified tolerance.

    \param vect [in]  Any 2D vector.
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of test.

    \remarks
    If the length of either vector is < tol, kThis is returned in status.

    Possible values for status are as follows:

    @untitled table
    kOk
    k0This
    k0Arg1
  */    
  bool isParallelTo (
    const OdGeVector2d& vect, 
    const OdGeTol& tol,
    OdGeError& status) const;
    
  /** \details
    Returns true if and only if the specified vector is codirectional to this vector within the specified tolerance.

    \param vect [in]  Any 2D vector.
    \param tol [in]  Geometric tolerance.   
  */
        bool isCodirectionalTo (
    const OdGeVector2d& vect,
    const OdGeTol& tol=OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified vector is codirectional to this vector within the specified tolerance.

    \param vect [in]  Any 2D vector.
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of test.

    \remarks
    If the length of either vector is < tol, kThis is returned in status.

    Possible values for status are as follows:

    @untitled table
    kOk       
    k0This    
    k0Arg1    
  */
  bool isCodirectionalTo (
    const OdGeVector2d& vect, 
    const OdGeTol& tol,
    OdGeError& status) const;
    
  /** \details
    Returns true if and only if the specified vector is perpendicular to this vector within the specified tolerance.

    \param vect [in]  Any 2D vector.
    \param tol [in]  Geometric tolerance.
  */
        bool isPerpendicularTo (
    const OdGeVector2d& vect,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified vector is perpendicular to this vector within the specified tolerance.

    \param vect [in]  Any 2D vector.
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of test.

    \remarks
    If the length of either vector is < tol, kThis is returned in status.

    Possible values for status are as follows:

    @untitled table
    kOk       
    k0This    
    k0Arg1    
  */
  bool isPerpendicularTo (
    const OdGeVector2d& vect, 
    const OdGeTol& tol,
    OdGeError& status) const;
    
  /** \details
    Returns the dot product of this vector and the specified vector.
    
    \param vect [in]  Any 2D vector.
  */
  double dotProduct (
    const OdGeVector2d& vect) const;

  /** \details
    Returns the z value of the cross product of this vector with
    the specified vector.
    
    \param vect [in]  Any 2D vector.
  */
  double  crossProduct (
    const OdGeVector2d& vect) const
  {
    return x * vect.y - y * vect.x;
  }

  bool operator== (
    const OdGeVector2d& vect) const;
  bool operator!= (
    const OdGeVector2d& vect) const;
    
  /** \details
    Returns true if and only if vector is identical to this vector,
    within the specified tolerance.

    \param vect [in]  Any 2D vector.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo (
    const OdGeVector2d& vect,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \param i [in]  Index of coordinate.

    \details
    Returns or references the ith coordinate of this vector.

    * 0 returns or references the X-coordinate.
    * 1 returns or references the Y-coordinate.
  */
  double operator[] (
    unsigned int i) const;
  double& operator[] (
    unsigned int i) ;
  
  /** \details
    Sets this vector to the specified X and Y coordinates, 
    and returns a reference to this vector.

    \param xx [in]  X-coordinate.
    \param yy [in]  Y-coordinate.
  */
  OdGeVector2d& set (
    double xx, 
    double yy)
  {
    x = xx;
    y = yy;
    return *this;
  }    

  /** \details
    Returns the equivalent 2D tranformation matrix.
  */
  operator OdGeMatrix2d () const;

  double x; // X-coordinate.
  double y; // Y-coordinate.
};


#include "TD_PackPop.h"

#endif

