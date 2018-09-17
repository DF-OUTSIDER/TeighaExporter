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

#ifndef OD_GEAPLN3D_H
#define OD_GEAPLN3D_H  /*!DOM*/

#include "Ge/GePolyline3d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3dArray.h"
#include "Ge/GeKnotVector.h"

#include "TD_PackPush.h"

/** \details
    This class represents mathematical entities used to support various types of spline curves in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeAugPolyline3d : public OdGePolyline3d
{
public:


  /** \param curve [in]  Any 3D curve.
    \param fromParam [in]  Starting parameter value.
    \param toParam [in]  Ending parameter value.
    \param approxEps [in]  Approximate spacing along a curve.
    \param knots [in]  Knot vector.
    \param controlPoints [in]  Control point array.
    \param vecBundle [in]  Vector array. 
    \param source [in]  Object to be cloned.
  */
  OdGeAugPolyline3d();
  OdGeAugPolyline3d(
    const OdGeAugPolyline3d& apline);
  OdGeAugPolyline3d(
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& controlPoints,
    const OdGeVector3dArray& vecBundle);
  OdGeAugPolyline3d(
    const OdGePoint3dArray& controlPoints,
    const OdGeVector3dArray& vecBundle);
  OdGeAugPolyline3d(
    const OdGeCurve3d& curve,
    double fromParam, 
    double toParam, 
    double approxEps);

  OdGeAugPolyline3d& operator =(
    const OdGeAugPolyline3d& apline);

  /** \details
    Returns the control point at the specified index.

    \param cpIndex [in]  Control point index.

    \remarks
    Currently not implemented.
  */
   OdGePoint3d getPoint(
    int index) const;

  /** \details
    Sets the control point at the specified index, and returns a reference to this curve.

    \param controlpointIndex [in]  Control point index.
    \param point [in]  Any 3D point.
  */
   OdGeAugPolyline3d& setPoint(
    int controlpointIndex, 
    OdGePoint3d point);

  /** \details
    Returns the array of control points.

    \param controlPoints [out]  Receives an array of control points.
  */
   void getPoints(
    OdGePoint3dArray& controlPoints) const;

  /** \details
    Returns the vector at the specified index.

    \param vectorIndex [in]  Vector index.

    \remarks
    Currently not implemented.
  */
   OdGeVector3d getVector(
    int vectorIndex) const;

  /** \details
    Sets the vector at the specified index, and returns a reference to this curve.

    \param vectorIndex [in]  Vector index.
    \param vect [in]  Any 3D vector.

    \remarks
    Currently not implemented.
  */
   OdGeAugPolyline3d& setVector(
    int vectorIndex, 
    OdGeVector3d vect);

  /** \details
    Returns an array of the tangents (first derivative vectors) to the curve at each control point.

    \param tangents [out]  Receives an array of tangents.

    \remarks
    Currently not implemented.
  */
   void getD1Vectors(
    OdGeVector3dArray& tangents) const;

  /** \details
    Returns the second derivative vector at the specified index.

    \param vectorIndex [in]  Vector index.

    \remarks
    Currently not implemented.
  */
   OdGeVector3d getD2Vector(
    int vectorIndex) const;

  /** \details
    Sets the second derivative vector at the specified index, and returns a reference to this curve.

    \param vectorIndex [in]  Vector index.
    \param vect [in]  Second derivative vector.

    \remarks
    Currently not implemented.
  */
   OdGeAugPolyline3d& setD2Vector(
    int vectorIndex, 
    OdGeVector3d vect);

  /** \details
    Returns an array of the second derivative vectors to the curve at each control point.


    d2Vectors Returns an array of second derivative cectors.

    \remarks
    Currently not implemented.
  */
   void getD2Vectors(
    OdGeVector3dArray& d2Vectors) const;

  /** \details
    Returns the approximate tolerance that was used to construct the polyline.
  */
   double approxTol() const;

  /** \details
    Sets the approximate tolerance to be used to construct the polyline, and returns
    a reference to this polyline.

    \remarks
    This method recomputes the polyline.
  */
   OdGeAugPolyline3d& setApproxTol(
    double approxTol);
};

#include "TD_PackPop.h"

#endif // OD_GEAPLN3D_H

