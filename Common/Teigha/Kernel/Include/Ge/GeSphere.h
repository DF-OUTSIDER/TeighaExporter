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

#ifndef OD_GESPHERE_H
#define OD_GESPHERE_H /*!DOM*/

#include "Ge/GeSurface.h"
class OdGeCircArc3d;

#include "TD_PackPush.h"

/** \details
    This class represents spheres.  

    \remarks
    A sphere is defined by its
    * radius
    * center
    * northAxis
    * refAxis

    northAxis defines the direction from the center to the north pole. 

    refAxis, a vector orthogonal to northAxis, the prime meridian.

    Latitude is defined by the U parameter, which defaults to 
    [-OdaPI/2, OdaPI/2] for closed spheres.  The lower bound maps to the south
    pole, zero maps to the equator, and the upper bound maps to the north pole.

    Longitude is defined by the V parameter, which defaults to 
    [-OdaPI, OdaPI) for closed spheres.  Zero corresponds to the meridian defined by
    the refAxis of this sphere.

    The sphere is periodic in V with a period of Oda2PI.

    [umin, umax] by [vmin, vmax] defines a spherical patch with 4 sides
    bounded by 2 longitudinal arcs and 2 latitudinal arcs.  

    The following constraints apply when defining a patch.

    * umin < umax and |umin - umax| <= Oda2PI.
    * vmin < vmax and |vmin - vmax| <= OdaPI.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeSphere.html, Working with Spheress>
*/
class GE_TOOLKIT_EXPORT OdGeSphere : public OdGeSurface
{
public:


  /**
    \param radius [in] The *radius* of this sphere.
    \param center [in]  The origin of the this sphere.
    \param northAxis [in]  the *direction* to the north pole.
    \param refAxis [in] the *direction* to the prime meridian.
    \param startAngleU [in] Starting longitude.
    \param endAngleU [in] Ending longitude.
    \param startAngleV [in] Starting latitude.
    \param endAngleV [in] Ending latitude.
  */
  OdGeSphere();
  OdGeSphere(
    double radius, 
    const OdGePoint3d& center);
  OdGeSphere(
    double radius, 
    const OdGePoint3d& center,
    const OdGeVector3d& northAxis, 
    const OdGeVector3d& refAxis,
    double startAngleU, 
    double endAngleU, 
    double startAngleV, 
    double endAngleV);
  OdGeSphere(const OdGeSphere& sphere);

  /** \details
    Returns the radius of this sphere.
  */
   double radius() const;

  /** \details
    Returns the center of this sphere.
  */
   OdGePoint3d center() const;

  /** \details
    Returns the start and end longitude.

    \param startAngleU [out]  Receives the start longitude.
    \param endAngleU [out]  Receives the end longitude.
  */
   void getAnglesInU(
    double& startAngleU, 
    double& endAngleU) const;

  /** \details
    Returns the start and end latitude.

    \param startAngleV [out]  Receives the start latitude.
    \param endAngleV [out]  Receives the end latitude.
  */
   void getAnglesInV(
    double& startAngleV, 
    double& endAngleV) const;

  /** \details
    Returns the direction to the north pole.

    \param northAxis [out]  Receives the direction to the north pole.
  */
   OdGeVector3d northAxis() const;

  /** \details
    Returns the direction to the north pole.

    \param refAxis [out]  Receives the direction to the prime meridian.
  */
   OdGeVector3d refAxis() const;

  /** \details
    Returns the location of the north pole.
  */
   OdGePoint3d northPole() const;

  /** \details
    Returns the location of the south pole.
  */
   OdGePoint3d southPole() const;

  /** \details
    Returns true if and only if the normal to this surface
    is pointing outward.
  */
   bool isOuterNormal() const;

  /** \details
    Returns true if and only if the equator is full circle.

    \param tol [in]  Geometric tolerance.
  */
     bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Sets the radius of this sphere.

    \param radius [in]  The radius of this sphere.
  */
   OdGeSphere& setRadius(
    double radius);

  /** \details
    Sets the startint and ending longitudes.

    \param startAngleU [in]  Starting longitude.
    \param endAngleU [in]  Ending longitude.
  */
   OdGeSphere& setAnglesInU(
    double startAngleU, 
    double endAngleU);

  /** \details
    Sets the starting and ending latitudes.

    \param startAngleV [in]  Starting latitude.
    \param endAngleV [in]  Ending latitude.
  */
   OdGeSphere& setAnglesInV(
    double startAngleV, 
    double endAngleV);

  /** \details
    Sets the parameters for this sphere according to the arguments.

    \param radius [in]  The radius of this sphere.
    \param center [in]  The origin of the this sphere.

    \returns
    Returns a reference to this sphere.
  */
   OdGeSphere& set(
    double radius, 
    const OdGePoint3d& center);

  /** \details
    Sets the parameters for this sphere according to the arguments.

    \param radius [in]  The radius of this sphere.
    \param center [in]  The origin of the this sphere.
    \param northAxis [in]  the direction to the north pole.
    \param refAxis [in]  the direction to the prime meridian.
    \param startAngleU [in]  Starting longitude.
    \param endAngleU [in]  Ending longitude.
    \param startAngleV [in]  Starting latitude.
    \param endAngleV [in]  Ending latitude.

    \returns
    Returns a reference to this sphere.
  */
  OdGeSphere& set(
    double radius, 
    const OdGePoint3d& center,
    const OdGeVector3d& northAxis,
    const OdGeVector3d& refAxis,
    double startAngleU,
    double endAngleU,
    double startAngleV,
    double endAngleV);

  OdGeSphere& operator =(const OdGeSphere& sphere);

  /** \details
    Returns true if and only if this sphere intersects with
    a line entity, and returns the number of intersections and the
    points of intersection.

    \param lineEnt [in]  Any 3D line entity.
    \param numInt [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point.
    \param p2 [out]  Receives the second intersection point.
    \param tol [in]  Geometric tolerance.

    \remarks
    * p1 is valid if and only if numInt >= 1.
    * p2 is valid if and only if numInt = 2.
  */
   bool intersectWith(
    const OdGeLinearEnt3d& lineEnt, 
    int& numInt,
    OdGePoint3d& p1, 
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns false if and only if clockwise direction for
    constant u-parameter lines are circles around northAxis,
    but counterclockwise if this is true.
  */
   bool isReverseV() const;

  /** \details
    Sets ReverseV according to the parameter.

    \param isReverseV [in]  Sets ReverseV.
  */
   void setReverseV(
     bool isReverseV);

  //////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif // OD_GESPHERE_H

