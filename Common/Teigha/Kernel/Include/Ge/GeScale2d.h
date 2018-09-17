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




#ifndef OD_GESCL2D_H
#define OD_GESCL2D_H /*!DOM*/


class OdGeMatrix2d;
class OdGeScale3d;
#include "Ge/GeGbl.h"
#include "TD_PackPush.h"

/** \details
    This class represents scaling transformations (scale vectors) in 2D space.

    \remarks
    OdGeScale2d may be viewed as an array[2] of doubles.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>
*/
class GE_TOOLKIT_EXPORT OdGeScale2d
{
public:
  /** \param factor [in]  Uniform scale factor.
    \param xFactor [in]  The X scale factor.
    \param yFactor [in]  The Y scale factor.
    \param source [in]  Object to be cloned.
  */
  OdGeScale2d();
  OdGeScale2d(
    double factor);
  OdGeScale2d(
    double xFactor,
    double yFactor);

  GE_STATIC_EXPORT static const   OdGeScale2d kIdentity; // Multplicitive identity vector.

  /** \param scaleVec [in]  Any 2D scale vector.
    \param factor [in]  Uniform scale factor.
  */
  OdGeScale2d operator * (
    const OdGeScale2d& scaleVec) const;

  friend OdGeScale2d operator * (
    double factor,
    const OdGeScale2d& scaleVec);

  /** \param scaleVec [in]  Any 2D scale vector.
    \param factor [in]  Uniform scale factor.

    \remarks
    Multiplication of scale vectors is defined as follows:

              scl * [xs ys]          = [scl*xs scl*ys]
              [xs1 ys1] * [xs2 ys2]  = [xs1*xs2 ys1*ys2]
  */
  OdGeScale2d& operator *= (
    const OdGeScale2d& scaleVec);

  /** \details
    Sets this scale vector to the product leftSide * (this scale vector), and returns
    a reference to this scale vector.

    \param leftSide [in]  Any 2D scale vector.

    \remarks
    Scale muliplications is commutative.
  */
  OdGeScale2d& preMultBy (
    const OdGeScale2d& leftSide);

  /** \details
    Sets this scale vector to the product (this scale vector) * (rightSide), and returns
    a reference to this scale vector.

    \param rightSide [in]  Any 2D scale vector.

    \remarks
    Scale muliplications is commutative.
  */
  OdGeScale2d& postMultBy (
    const OdGeScale2d& rightSide);


  /** \details
    Sets this scale vector to the product factor * scaleVec, and returns
    a reference to this scale vector.

    \param factor [in]  Uniform scale factor.
    \param scaleVec [in]  Any 2D scale vector.

    \remarks
    Multiplication of scale vectors is defined as follows:

              scl * [xs1 ys1]        = [scl*xs1 scl*ys1]
              [xs1 ys1 ] * [xs2 ys2] = [xs1*xs2 ys1*ys2]
  */
  OdGeScale2d& setToProduct(
    const OdGeScale2d& scaleVec, double factor);

  /** \details
    Sets this scale vector to the product scaleVec1 * scaleVec2 and returns
    a reference to this scale vector.

    \param scaleVec1 [in]  Any 2D scale vector.
    \param scaleVec2 [in]  Any 2D scale vector.

    \remarks
    Multiplication of scale vectors is defined as follows:

              scl * [xs1 ys1]        = [scl*xs1 scl*ys1]
              [xs1 ys1 ] * [xs2 ys2] = [xs1*xs2 ys1*ys2]
  */
  OdGeScale2d& setToProduct(
    const OdGeScale2d& scaleVec1,
    const OdGeScale2d& scaleVec2);

  /** \param factor [in]  Uniform scale factor.
    \remarks
    Multiplication of scale vectors is defined as follows:

              scl * [xs1 ys1]        = [scl*xs1 scl*ys1]
              [xs1 ys1 ] * [xs2 ys2] = [xs1*xs2 ys1*ys2]
  */
  OdGeScale2d operator * (
    double factor) const;


  /** \param factor [in]  Uniform scale factor.
  */
  OdGeScale2d& operator *= (
    double factor);



  /** \details
    Returns the inverse of this scale vector.
  */
  OdGeScale2d inverse () const;

  /** \details
    Sets this scale vector to its inverse, and returns
    a reference to this scale vector.
  */
  OdGeScale2d& invert ();

  /** \details
    Returns true if and only if the scaling matrix corresponding to this scale vector isUniScaledOrtho().

    \param tol [in]  Geometric tolerance.
  */
  bool isProportional(
    const OdGeTol& tol = OdGeContext::gTol) const;

  bool operator == (
    const OdGeScale2d& scaleVec) const;
  bool operator != (
    const OdGeScale2d& scaleVec) const;

  /** \details
    Returns true if and only if scaleVec is identical to this one,
    within the specified tolerance.

    \param scaleVec [in]  Any 2D scale vector.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo (
    const OdGeScale2d& scaleVec,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns or references the ith component of the scale vector.
    
    \param i [in]  Index of component.
    
    \remarks
    * 0 returns or references the X scale factor.
    * 1 returns or references the Y scale factor.
  */
  double& operator [] (
    unsigned int i)
  {
    return * (&sx + i);
  }
  double operator [] (
    unsigned int i) const
  {
    return * (&sx + i);
  }

  /** \details
    Sets this scale vector to the specified X and Y factors, 
    and returns a reference to this vector.

    \param xFactor [in]  The X scale factor.
    \param yFactor [in]  The Y scale factor.
  */
  OdGeScale2d& set (
    double xFactor, 
    double yFactor);


  /** \details
    Returns the transformation matrix equivalent to this scale vector.
    
    \param xfm [out]  Receives the 2D transformation matrix.
    
  */
  void getMatrix (
    OdGeMatrix2d& xfm) const;
    
  /** \details
    Returns the scale vector corresponding to the
    lengths of the column vectors of the transformation matrix.  
    
    \param xfm [out]  Receives the 2D transformation matrix.
    
    \remarks
    xfm must be scaled ortho; i.e., xfm.isScaledOrtho() == true.   
  */
    OdGeScale2d& extractScale (
    const OdGeMatrix2d& xfm);
    
  /** \details
    Returns the scale vector corresponding to the
    lengths of the column vectors of the transformation matrix,
    and sets the scale factor of the matrix to 1 .  
    
    \param xfm [out]  Receives the 2D transformation matrix.
    
    \remarks
    xfm must be scaled ortho; i.e., xfm.isScaledOrtho() == true.   
  */
    OdGeScale2d& removeScale (
    OdGeMatrix2d& xfm);

  /** \details
    Returns the equivalent 2D tranformation matrix.
    or a 3D scale vector [sx sy 1].
  */
  operator OdGeMatrix2d () const;
  operator OdGeScale3d () const;

  double sx; // X scale *factor*.
  double sy; // Y scale *factor*.
};

#include "TD_PackPop.h"

#endif // OD_GESCL2D_H

