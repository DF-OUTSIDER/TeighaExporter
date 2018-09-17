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




#ifndef OD_GESCL3D_H
#define OD_GESCL3D_H /*!DOM*/


class OdGeMatrix3d;
#include "Ge/GeGbl.h"

#include "TD_PackPush.h"

/** \details
    This class represents scaling transformations (scale vectors) in 3D space.
    
    \remarks
    OdGeScale3d may be viewed as an array[3] of doubles.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeScale3d
{
public:

 /** \param factor [in]  Uniform scale factor.
    \param xFactor [in]  The X scale factor.
    \param yFactor [in]  The Y scale factor.
    \param zFactor [in]  The Z scale factor.
    \param source [in]  Object to be cloned.
 */
  OdGeScale3d () 
    : sx (1.0), sy (1.0), sz (1.0) {}
    
  OdGeScale3d (
    double factor) 
    : sx (factor), sy (factor), sz (factor) {}
    
  OdGeScale3d (
    double xFactor, 
    double yFactor, double zFactor) 
    : sx (xFactor), sy (yFactor), sz (zFactor) {}

  GE_STATIC_EXPORT static const OdGeScale3d kIdentity; // Multplicitive identity scale.
  
  /** \param scaleVec [in]  Any 3D scale vector.
    \param factor [in]  Uniform scale factor.
  */
  OdGeScale3d operator * (
    const OdGeScale3d& scaleVec) const;
  OdGeScale3d operator * (
    double factor) const;

  friend OdGeScale3d operator * (
    double factor, 
    const OdGeScale3d& scaleVec);
    
  /** \param scaleVec [in]  Any 3D scale vector.
    \param factor [in]  Uniform scale factor.

    \remarks
    Multiplication of scale vectors is defined as follows:
    
              scl * [xs ys zs]              = [scl*xs scl*ys scl*zs]
              [xs1 ys1 zs1] * [xs2 ys2 zs2] = [xs1*xs2 ys1*ys2 zs1*zs2]
  */
  OdGeScale3d& operator *= (
    const OdGeScale3d& scaleVec);
  OdGeScale3d& operator *= (
    double factor);
    
  /** \details
    Sets this scale vector to the product leftSide * (this scale vector), and returns
    a reference to this scale vector.
    
    \param leftSide [in]  Any 3D scale vector.

    \remarks
    Scale muliplications is commutative. 
  */
  OdGeScale3d& preMultBy (
    const OdGeScale3d& leftSide);

  /** \details
    Sets this scale vector to the product (this scale vector) * (rightSide), and returns
    a reference to this scale vector.
    
    \param rightSide [in]  Any 3D scale vector.

    \remarks
    Scale muliplications is commutative. 
  */
   OdGeScale3d& postMultBy (
    const OdGeScale3d& rightSide);
 
  /** \details
    Sets this scale vector to the product scaleVec1 * scaleVec2 and returns
    a reference to this scale vector.
    
    \param scaleVec1 [in]  Any 2D scale vector.
    \param scaleVec2 [in]  Any 2D scale vector.
    
    \remarks
    Multiplication of scale vectors is defined as follows:
    
              scl * [xs ys zs]              = [scl*xs scl*ys scl*zs]
              [xs1 ys1 zs1] * [xs2 ys2 zs2] = [xs1*xs2 ys1*ys2 zs1*zs2]
  */
  OdGeScale3d& setToProduct (
    const OdGeScale3d& scaleVec1, 
    const OdGeScale3d& scaleVec2);

  /** \details
    Sets this scale vector to the product factor * scaleVec, and returns
    a reference to this scale vector.
    
    \param factor [in]  Uniform scale factor.
    \param scaleVec [in]  Any 2D scale vector.
    
    \remarks
    Multiplication of scale vectors is defined as follows:
    
              scl * [xs ys zs]              = [scl*xs scl*ys scl*zs]
              [xs1 ys1 zs1] * [xs2 ys2 zs2] = [xs1*xs2 ys1*ys2 zs1*zs2]
  */
  OdGeScale3d& setToProduct (
    const OdGeScale3d& scaleVec, double factor);
 
  /** \details
    Returns the inverse of this scale vector. 
  */
  OdGeScale3d inverse () const;

  /** \details
    Sets this scale vector to its inverse, and returns
    a reference to this scale vector. 
  */
  OdGeScale3d& invert ();
  
  /** \details
    Returns true if and only if the scaling matrix corresponding to this scale vector isUniScaledOrtho ().
    
    \param tol [in]  Geometric tolerance.
  */
  bool isProportional (
    const OdGeTol& tol = OdGeContext::gTol) const;
  
  bool operator == (
    const OdGeScale3d& scaleVec) const;
  bool operator != (
    const OdGeScale3d& scaleVec) const;

  /** \details
    Returns true if and only if scaleVec is identical to this one,
    within the specified tolerance.

    \param scaleVec [in]  Any 2D scale vector.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo (
    const OdGeScale3d& scaleVec, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \param i [in]  Index of component.
    
    \details
    Returns or references the ith component of the scale vector.

    * 0 returns or references the X scale factor.
    * 1 returns or references the Y scale factor.
    * 2 returns or references the Z scale factor.
  */
  double operator [] (
    unsigned int i) const { return * (&sx+i); }
  double& operator [] (
    unsigned int i) { return * (&sx+i); }
    
  /** \details
    Sets this scale vector to the specified X and Y factors, and 
    returns a reference to this vectors.

    \param xFactor [in]  The X scale factor.
    \param yFactor [in]  The Y scale factor.
    \param zFactor [in]  The Z scale factor.
  */
  OdGeScale3d& set (
    double xFactor, 
    double yFactor, 
    double zFactor) 
  { sx = xFactor; sy = yFactor; sz = zFactor; return *this; }
  
  /** \details
    Returns the equivalent 3D tranformation matrix.
  */
  operator OdGeMatrix3d () const;
  
  /** \details
    Returns the transformation matrix equivalent to this scale vector.
    
    \param xfm [out]  Receives the 3D transformation matrix.
    
  */
  void getMatrix (
    OdGeMatrix3d& xfm) const;
    
  /** \details
    Returns the scale vector corresponding to the
    lengths of the column vectors of the transformation matrix.  
    
    \param xfm [out]  Receives the 3D transformation matrix.
    
    \remarks
    xfm must be scaled ortho; i.e., xfm.isScaledOrtho () == true.   
  */
    OdGeScale3d& extractScale (
    const OdGeMatrix3d& xfm);
    
  /** \details
    Returns the scale vector corresponding to the
    lengths of the column vectors of the transformation matrix,
    and sets the scale factor of the matrix to 1 .  
    
    \param xfm [out]  Receives the 2D transformation matrix.
    
    \remarks
    xfm must be scaled ortho; i.e., xfm.isScaledOrtho () == true.   
  */
     OdGeScale3d& removeScale (
      OdGeMatrix3d& xfm, 
      bool negateX = false); // use true to get old Teigha implementation (default is ACAD way now)
  
  /** \details
    Returns true if this scale vector has nonzero coordinates or false if at least one coordinate is zero.   
  */
  bool isValid () const { return OdNonZero (sx) && OdNonZero (sy) && OdNonZero (sz); }

  double sx; // X scale *factor*.
  double sy; // Y scale *factor*.
  double sz; // Z scale *factor*.
};

#include "TD_PackPop.h"

#endif // OD_GESCL3D_H

