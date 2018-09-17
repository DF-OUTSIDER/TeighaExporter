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

#ifndef OD_GESURF_H
#define OD_GESURF_H /*!DOM*/

#include "Ge/GeEntity3d.h"
#include "Ge/GeVector3dArray.h"
#include "Ge/GePoint2d.h"
#include "Ge/GeUvBox.h"

class OdGePoint2d;
class OdGeCurve3d;
class OdGePointOnCurve3d;
class OdGePointOnSurface;
class OdGePointOnSurfaceData;
class OdGeInterval;
class OdGeExtents3d;

#include "TD_PackPush.h"

/** \details
    This class is the base class for all OdGe parametric surfaces.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSurface : public OdGeEntity3d
{
public:


  /** \details
    Returns the 2D pair of parameter values of a point on this surface.

    \param point [in]  Point to be evaluated.
    \param tol [in]  Geometric tolerance.

    \remarks
    The returned parameters specify a point within tol of point.
    If point is not on this surface, the results are unpredictable.
    If you are not sure the point is on this surface, use 
    isOn() instead of this function.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   OdGePoint2d paramOf(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  TD_USING(OdGeEntity3d::isOn);
  /** \param ParamPoint [out]  Receives the 2D pair of parameter values at the point. 
   
    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   bool isOn( 
    const OdGePoint3d& point, 
    OdGePoint2d& paramPoint,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this surface closest to the specified point.

    \param point [in]  Any 3D point.
    \param tol [in]  Geometric tolerance.
  */
   OdGePoint3d closestPointTo(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this surface closest to the specified point.

    \param point [in]  Any 3D point.
    \param pntOnSurface [out]  Receives the closest point on surface to specified point. 
    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   void getClosestPointTo(
    const OdGePoint3d& point, 
    OdGePointOnSurface& pntOnSurface,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the distance to the point on this curve closest to the specified point.

    \param point [in]  Any 3D point.
    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   double distanceTo(
    const OdGePoint3d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the normal of this surface has been reversed an odd
    number of times.
  */
   bool isNormalReversed() const;

  /** \details
    Reverses the normal of this surface and returns a reference to this surface.
  */
   OdGeSurface& reverseNormal();

  /** \details
    Returns the minimum rectangle in parameter space that contains the parameter
    domain of this surface.

    \param intrvlU [out]  Receives the u interval.
    \param intrvlV [out]  Receives the v interval.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   void getEnvelope(
    OdGeInterval& intrvlU, 
    OdGeInterval& intrvlV) const;

  /** \details
    Returns the minimum rectangle in parameter space that contains the parameter
    domain of this surface.

    \param uvbox [out]  Receives the uv-box

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   void getEnvelope(
    OdGeUvBox& uvbox) const;

  /** \details
    Returns true if and only if this surface is closed in the U direction.

    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
     bool isClosedInU(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if this surface is closed in the V direction.

    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
     bool isClosedInV(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point corresponding to the parameter pair, as well as the
    derivatives and the normal at that point.

    \param param [in]  The parameter pair to be evaluated.
    
    \remarks
    Derivatives are ordered as follows: du, dv, dudu, dvdv, dudv.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   OdGePoint3d evalPoint(
    const OdGePoint2d& param) const;
  
  /** \details
    Returns the point corresponding to the parameter pair, as well as the
    derivatives and the normal at that point.

    \param param [in]  The parameter pair to be evaluated.
    \param numDeriv [in]  The number of derivatives to be computed.
    \param derivatives [out]  Receives an array of derivatives at the point corresponding to param.
    
    \remarks
    Derivatives are ordered as follows: du, dv, dudu, dvdv, dudv

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGePoint3d evalPoint(
    const OdGePoint2d& param, 
    int numDeriv,
    OdGeVector3dArray& derivatives) const;
  
  /** \details
    Returns the point corresponding to the parameter pair, as well as the
    derivatives and the normal at that point.

    \param param [in]  The parameter pair to be evaluated.
    \param numDeriv [in]  The number of derivatives to be computed.
    \param derivatives [out]  Receives an array of derivatives at the point corresponding to param.
    \param normal [out]  Receives the normal at the point corresponding to param.
    
    \remarks
    Derivatives are ordered as follows: du, dv, dudu, dvdv, dudv

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
    OdGePoint3d evalPoint(
    const OdGePoint2d& param,
    int numDeriv,
    OdGeVector3dArray& derivatives, 
    OdGeVector3d& normal) const;

  OdGeSurface& operator=(
    const OdGeSurface& surf);

  /** \details
    Returns bounding box of the whole surface or a part of it.
    The box is axis-aligned in the specified local coordinate system.
    It is guaranteed to enclose the surface, but not guaranteed to be tight/minimal.

    \param range [in]  The part of the surface to be bounded.
    \param coordSystem [in]  The local coordinate system in which box extents are computed.

    \remarks
    If range is unbounded (e.g. by default), then bounding box of the whole surface (which range can be
    obtained by getEnvelope method) is computed. Otherwise bounding box is computed only for the set of
    points with parameter in the specified range.
    If coordSystem is NULL (e.g. by default), then bounding box is aligned with global coordinate axes.
    Otherwise, the box will be axis-aligned in the local coordinate system specified by coordSystem matrix.
  */
  OdGeExtents3d getGeomExtents(
    const OdGeUvBox& range = OdGeUvBox(),
    const OdGeMatrix3d* coordSystem = NULL) const;

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
  Returns projP and true,
  if and only if there is a point on this surface, projP, where
  a normal to this surface passes through the point p.

  \param p [in]  Any 3D point.
  \param projP [out]  Receives the point on this surface. 
  */
   bool project(const OdGePoint3d& p, OdGePoint3d& projP, const OdGeTol& tol = OdGeContext::gTol) const;

   /** \details
    Returns true if and only if this surface has reparameterization,
    and returns the swap flag value and u- and v-coefficients.

    \param swapUV [out]  Receives the current uv parameter swap flag value. If returns true, uv parameters were swapped; returns false in the other case.
    \param uCoeffA [out]  Receives the current value of the uCoeffA coefficient.
    \param vCoeffA [out]  Receives the current value of the vCoeffA coefficient.
    \param uCoeffB [out]  Receives the current value of the uCoeffB coefficient.
    \param vCoeffB [out]  Receives the current value of the vCoeffB coefficient.
  */
   bool getReparameterization(
     bool& swapUV,
     double& uCoeffA,
     double& vCoeffA,
     double& uCoeffB,
     double& vCoeffB) const;

   /** \details
    Sets a new swap flag value, and u- and v-coefficients to new values.

    \param swapUV [in]  A new value of the swap flag to be set. Default value is false.
    \param uCoeffA [in]  A new uCoeffA value. Default value is 1.0.
    \param vCoeffA [in]  A new vCoeffA value. Default value is 1.0.
    \param uCoeffB [in]  A new uCoeffB value. Default value is 0.0.
    \param vCoeffB [in]  A new vCoeffB value. Default value is 0.0.
  */
  void setReparameterization(
    bool swapUV = false,
    double uCoeffA = 1.,
    double vCoeffA = 1.,
    double uCoeffB = 0.,
    double vCoeffB = 0.);

  /** \details
    Returns the minimum rectangle in parameter space that contains the implicit parameter domain of this surface.

    \param implicitIntrvlU [out]  Receives the implicit u interval.
    \param implicitIntrvlV [out]  Receives the implicit v interval.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getImplicitEnvelope(
    OdGeInterval& implicitIntrvlU,
    OdGeInterval& implicitIntrvlV) const;

  /** \details
    Set the rectangle in parameter space that defines the parameter
    domain of this surface.
    \returns
    Returns true if and only the new parameter domain was successfully set
    for this surface.

    \param realIntrvlU [in] A reference to a new real u interval to be set.
    \param realIntrvlV [in] A reference to a new real v interval to be set.
  */
  bool setEnvelope(
    const OdGeInterval& realIntrvlU,
    const OdGeInterval& realIntrvlV);

protected:
  OdGeSurface();
  OdGeSurface(
    const OdGeSurface& surf);

};

#include "TD_PackPop.h"

#endif // OD_GESURF_H

