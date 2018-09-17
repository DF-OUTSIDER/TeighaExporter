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

#ifndef OD_GECINT3D_H
#define OD_GECINT3D_H /*!DOM*/

#include "Ge/GeEntity3d.h"
#include "Ge/GeInterval.h"

#include "TD_PackPush.h"

class OdGeCurve3d;
class OdGeInterval;
class OdGePointOnCurve3d;

/** \details
    This class represents intersections of 3D curves.
    Currently not implemented.

    \remarks
    The intersection object links to 3D curve objects but does not contain
	them. Calculation of the intersection is performed by a calling function, 
	which owns the intersection output.
	
	If one of the curve objects is deleted, the intersection must be recalculated.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeCurveCurveInt3d : public OdGeEntity3d
{
public:


  /** \param curve1 [in]  First 3D curve.
    \param curve2 [in]  Second 3D curve.
    \param planeNormal [in]  Specifies the normal to the plane for getIntConfigs(). 
    \param range1 [in]  Range of first curve.
    \param range2 [in]  Range of second curve.
    \param tol [in]  Geometric tolerance.
    \param source [in]  Object to be cloned.

    \remarks
    The planeNormal argument is used only with getIntConfigs(). 
    You must supply this argument if and only if you are using getIntConfigs().

    \remarks
    Currently not implemented.
  */
  OdGeCurveCurveInt3d();
  OdGeCurveCurveInt3d(
    const OdGeCurve3d& curve1, 
    const OdGeCurve3d& curve2,
    const OdGeVector3d& planeNormal = OdGeVector3d::kIdentity,
    const OdGeTol& tol = OdGeContext::gTol);
  OdGeCurveCurveInt3d(
    const OdGeCurve3d& curve1, 
    const OdGeCurve3d& curve2,
    const OdGeInterval& range1, 
    const OdGeInterval& range2,
    const OdGeVector3d& planeNormal = OdGeVector3d::kIdentity,
    const OdGeTol& tol = OdGeContext::gTol);
  OdGeCurveCurveInt3d(
    const OdGeCurveCurveInt3d& source);

  /** \details
    Returns the first curve.

    \remarks
    Currently not implemented.
  */
   const OdGeCurve3d *curve1() const;

  /** \details
    Returns the second curve.

    \remarks
    Currently not implemented.
  */
   const OdGeCurve3d  *curve2() const;

  /** \details
    Returns the ranges of the two curves.

    \param range1 [out]  Receives the range of the first curve.
    \param range2 [out]  Receives the range of the second curve.

    \remarks
    Currently not implemented.
  */
   void getIntRanges(
    OdGeInterval& range1,
    OdGeInterval& range2) const;

  /** \details
    Returns the normal to the plane for getIntConfigs()

    \remarks
    Currently not implemented.
  */
   OdGeVector3d planeNormal() const;

  /** \details
    Returns the tolerance for determining intersections.

    \remarks
    Currently not implemented.
  */
   OdGeTol tolerance() const;

  /** \details
    Returns the number of intersections between the curves within the specified ranges.

    \remarks
    Currently not implemented.
  */
   int numIntPoints() const;

  /** \details
    Returns the specified intersection point.

    \param intNum [in]  The zero-based index of the intersection point to return.

    \remarks
    Currently not implemented.
  */
   OdGePoint3d intPoint(
    int intNum) const;

  /** \details
    Returns the curve parameters at the specified intersection point.

    \param intNum [in]  The zero-based index of the intersection point to return.
    \param param1 [out]  Receives the parameter of the first curve at the intersection point.
    \param param2 [out]  Receives the parameter of the second curve at the intersection point.

    \remarks
    Currently not implemented.
  */
   void getIntParams(
    int intNum,
    double& param1, 
    double& param2) const;

  /** \details
    Returns the specified intersection point as an OdGePointOnCurve3d on the first curve.

    \param intNum [in]  The zero-based index of the intersection point to return.
    \param intPnt [out]  Receives the specified intersection point on the first curve.

    \remarks
    Currently not implemented.
  */
   void getPointOnCurve1(
    int intNum, 
    OdGePointOnCurve3d& intPnt) const;

  /** \details
    Returns the specified intersection point as an OdGePointOnCurve3d on the second curve.

    \param intNum [in]  The zero-based index of the intersection point to return.
    \param intPnt [out]  Receives the specified intersection point on the second curve.

    \remarks
    Currently not implemented.
  */
   void getPointOnCurve2(
    int intNum, 
    OdGePointOnCurve3d& intPnt) const;

  /** \details
    Returns the configuration of the intersecion point of
    the first curve with respect to the second, and vice versa.

    \param intNum [in]  The zero-based index of the intersection point to query.
    \param config1wrt2 [out]  Receives the configuration of the first curve with respect to the second.
    \param config2wrt1 [out]  Receives the configuration of the second curve with respect to the first.

    \remarks
    The curves must be coplanar, and are with respect to the planeNormal.
    
    The possible values for config1wrt2 and config2wrt1 are as follows:

    @untitled table
    kNotDefined
    kUnknown
    kLeftRight
    kRightLeft
    kLeftLeft
    kRightRight
    kPointLeft
    kPointRight
    kLeftOverlap
    kOverlapLeft
    kRightOverlap
    kOverlapRight
    kOverlapStart
    kOverlapEnd,
    kOverlapOverlap

    \remarks
    Currently not implemented.
  */
   void getIntConfigs(
    int intNum, 
    OdGe::OdGeXConfig& config1wrt2,
    OdGe::OdGeXConfig& config2wrt1) const;

  /** \details
    Returns true if and only if the curves are tangential at specified intersection point 

    \param intNum [in]  The zero-based index of the intersection point to query.

    \remarks
    Currently not implemented.
  */
   bool isTangential(
    int intNum) const;

  /** \details
    Returns true if and only if the curves are transversal (cross) at specified intersection point 

    \param intNum [in]  The zero-based index of the intersection point to query.

    \remarks
    Currently not implemented.
  */
   bool isTransversal(
    int intNum) const;

  /** \details
    Returns the tolerance used to compute the specified intersection point. 

    \param intNum [in]  The zero-based index of the intersection point to query.

    \remarks
    Currently not implemented.
  */
   double intPointTol(
    int intNum) const;

  /** \details
    Returns the number of intervals of overlap for the two curves. 

    \remarks
    Currently not implemented.
  */
   int overlapCount() const;

  /** \details
    Returns true if and only if the curves are oriented in the same direction
    where they overlap. 

    \remarks
    This value has meaning only if overlapCount() > 0

    \remarks
    Currently not implemented.
  */
   bool overlapDirection() const;

  /** \details
    Returns the ranges for each curve a the specified overlap. 

    \param overlapNum [in]  The zero-based index of the overlap range to query.
    \param range1 [out]  Receives the range of the first curve for the specified overlap.
    \param range2 [out]  Receives the range of the second curve for the specified overlap.

    \remarks
    Currently not implemented.
  */
   void getOverlapRanges(
    int overlapNum,
    OdGeInterval& range1,
    OdGeInterval& range2) const;

  /** \details
    Swaps the first and the second curve.

    \remarks
    Currently not implemented.
  */
   void changeCurveOrder();

  /** \details
    Orders the intersection points so they correspond to
    increasing parameter values of the first curve.

    \remarks
    Currently not implemented.
  */
   OdGeCurveCurveInt3d& orderWrt1();

  /** \details
    Orders the intersection points so they correspond to
    increasing parameter values of the second curve.

    \remarks
    Currently not implemented.
  */
   OdGeCurveCurveInt3d& orderWrt2();

  /** \details
    Sets the curves, plane normals, and tolerance for which to
    determine intersections

    \param curve1 [in]  First 3D curve.
    \param curve2 [in]  Second 3D curve.
    \param planeNormal [in]  Specifies the normal to the plane for getIntConfigs(). 
    \param tol [in]  Geometric tolerance.

    \remarks
    The planeNormal argument is used only with getIntConfigs(). 
    You must supply this argument only if you are using getIntConfigs().

    \remarks
    Currently not implemented.
  */
   OdGeCurveCurveInt3d& set(
    const OdGeCurve3d& curve1,
    const OdGeCurve3d& curve2,
    const OdGeVector3d& planeNormal = OdGeVector3d::kIdentity,
    const OdGeTol& tol = OdGeContext::gTol);
  
  /** \details
    Sets the curves, ranges, plane normals, and tolerance for which to
    determine intersections

    \param curve1 [in]  First 3D curve.
    \param curve2 [in]  Second 3D curve.
    \param planeNormal [in]  Specifies the normal to the plane for getIntConfigs(). 
    \param range1 [in]  Range of first curve.
    \param range2 [in]  Range of second curve.
    \param tol [in]  Geometric tolerance.

    \remarks
    The planeNormal argument is used only with getIntConfigs(). 
    You must supply this argument only if you are using getIntConfigs().

    \remarks
    Currently not implemented.
  */
  OdGeCurveCurveInt3d& set(
    const OdGeCurve3d& curve1,
    const OdGeCurve3d& curve2,
    const OdGeInterval& range1,
    const OdGeInterval& range2,
    const OdGeVector3d& planeNormal = OdGeVector3d::kIdentity,
    const OdGeTol& tol = OdGeContext::gTol);

  OdGeCurveCurveInt3d& operator =(
    const OdGeCurveCurveInt3d& crvCrvInt);

  //////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif // OD_GECINT3D_H

