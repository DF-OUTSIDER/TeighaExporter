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

#ifndef OD_GE_CURVE_2D_H
#define OD_GE_CURVE_2D_H /*!DOM*/

class OdGePointOnCurve2d;
class OdGeInterval;
class OdGeMatrix2d;
class OdGeLine2d;
class OdGePointOnCurve2dData;
class OdGeBoundBlock2d;
class OdGeExtents2d;

#include "Ge/GeEntity2d.h"
#include "Ge/GeIntArray.h"
#include "Ge/GeInterval.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeVector2d.h"
#include "Ge/GeVector2dArray.h"
#include "Ge/GeCurve2dPtrArray.h"
#include "Ge/GeDoubleArray.h"

#include "Ge/GeCurve3dPtrArray.h"

#include "TD_PackPush.h"

/** \details
    This class is the base class for all OdGe 2D curves.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeCurve2d : public OdGeEntity2d
{
public:
  
  /** \details
    Returns pointer to 3d variant of the curve (z-coordinate equal to zero).

    \remarks
    For OdGeOffsetCurve a base curve will be created that should be MANUALLY deleted
    Please, see documentation about OdGeOffsetCurve
  */
  OdGeCurve3d* convertTo3d() const;

  /** \details
    Returns the parametric interval of the curve.

    \param interval [out]  Receives the parametric interval of the curve.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getInterval(
    OdGeInterval& interval) const;

  /** \details
    Returns the parametric interval of the curve.

    \param interval [out]  Receives the parametric interval of the curve.
    \param start [out]  Receives the start point of interval.
    \param end [out]  Receives the end point of interval. 

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getInterval(
    OdGeInterval& interval, 
    OdGePoint2d& start,
    OdGePoint2d& end) const;

  /** \details
    Reverses the parameter direction this curve.   

    \remarks
    The point set of this curve is unchanged.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGeCurve2d& reverseParam();
  
  /** \details
    Sets to natural domain of the curve

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGeCurve2d& setInterval();

  /** \details
    Sets the parametric interval of this curve.

    \param interval [in]  Parametric interval of this curve

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool setInterval(
    const OdGeInterval& interval);

  /** \details
    Returns the distance between the input point and the closest 
    point on this curve.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  double distanceTo(
    const OdGePoint2d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the distance between the closest points between this 
    curve and the input curve.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  double distanceTo(
    const OdGeCurve2d& otherCur,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the closest point on this curve to the input point. 

    \remarks
    This function is the same as getClosestPointTo().
  */
  OdGePoint2d closestPointTo(
    const OdGePoint2d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;
  
  /** \details
    Finds the closest points between this curve and the input curve.

    \remarks
    This function is the same as getClosestPointTo().
  */
  OdGePoint2d closestPointTo(
    const OdGeCurve2d& curve2d,
    OdGePoint2d& pntOnOtherCrv,
    const OdGeTol& tol= OdGeContext::gTol) const;

  /** \details
    Finds the closest point on this curve to the input point. 

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getClosestPointTo(
    const OdGePoint2d& point,
    OdGePointOnCurve2d& pntOnCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the closest points between this curve and the input curve.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getClosestPointTo(
    const OdGeCurve2d& curve2d,
    OdGePointOnCurve2d& pntOnThisCrv,
    OdGePointOnCurve2d& pntOnOtherCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the point on the input curve whose normal passes through the input point. 

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool getNormalPoint(
    const OdGePoint2d& point,
    OdGePointOnCurve2d& pntOnCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  TD_USING(OdGeEntity2d::isOn);

  /** \details
    Determines if pnt lies on curve. 
    The param parameter is only set to a meaningful value 
    if this function returns true.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool isOn(
    const OdGePoint2d& point, 
    double& param,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if input parameter value is within valid parametric range of curve.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool isOn(
    double param,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the parameter value of a point.

    \param point [in]  Input point on the curve.
    \param tol [in]  Geometric tolerance.

    \remarks
    The returned parameters specify a point within tol of point.
    If point is not on the curve, the results are unpredictable.
    If you are not sure the point is on the curve, use 
    isOn () instead of this function.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  double paramOf(
    const OdGePoint2d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Return the offset of the curve.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getTrimmedOffset(
    double distance,
    OdGeCurve2dPtrArray& offsetCurveList,
    OdGe::OffsetCrvExtType extensionType = OdGe::kFillet,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the curve is closed within the specified tolerance.

    \param tol [in]  Geometric tolerance.
  */
  bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if the curve is periodic. The curve is periodic if and 
    only if it is closed and the two points evalPoint(t) and 
    evalPoint(t+period) are always the same point for all values of t. 

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool isPeriodic(
    double& period) const;

  /** \details
    Returns true, and a linear entity coincident with this curve,
    if and only if this curve is linear.

    \param line [out]  Receives the line coincident with this curve.
    \param tol [in]  Geometric tolerance.

    \remarks
    An infinite line is returned, even if this curve is bounded.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool isLinear(
    OdGeLine2d& line, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the arc length of the curve between the two parameter values.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  double length(
    double fromParam, 
    double toParam,
    double tol
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    ) const;


  /** \details
    Returns the arc length of the curve between the two parameter values.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */    
  double length(
    double tol 
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    ) const;

  /** \details
    Returns the parameter value of the point whose arc length distance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  double paramAtLength(
    double datumParam, 
    double length,
    bool posParamDir 
#ifndef SWIG
    = true
#endif
    , double tol 
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    ) const;

  /** \details
    Returns the area of the interval of the curve defined by 
    startParam and endParam. 
  */
  bool area(
    double startParam, double endParam,
    double& value, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if the curve is degenerate and if so 
    returns type of entity that the curve degenerates. 

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool isDegenerate(
    OdGe::EntityId& degenerateType,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if the curve is degenerate and if so returns 
    the entity that the curve degenerates into.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool isDegenerate(
    OdGeEntity2d*& pConvertedEntity,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns two segments that are obtained by splitting the curve 
    at the input parameter value. If the specified parameter value
    does not lie within the parametric interval of the curve or if
    it corresponds to the start or endpoint of the curve, then piece1
    and piece2 are set to null.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getSplitCurves(
    double param, 
    OdGeCurve2d*& piece1,
    OdGeCurve2d*& piece2) const;

  /** \details
    Explode curve into its component sub-curves.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool explode(
    OdGeCurve2dPtrArray& explodedCurves,
    OdGeIntArray& newExplodedCurve,
    const OdGeInterval* interval = NULL) const;

  /** \details
    Local closest points.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getLocalClosestPoints(
    const OdGePoint2d& point,
    OdGePointOnCurve2d& approxPnt,
    const OdGeInterval* nbhd = NULL,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Local closest points.
    
    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  void getLocalClosestPoints(
    const OdGeCurve2d& otherCurve,
    OdGePointOnCurve2d& approxPntOnThisCrv,
    OdGePointOnCurve2d& approxPntOnOtherCrv,
    const OdGeInterval* nbhd1 = NULL,
    const OdGeInterval* nbhd2 = NULL,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Oriented bounding box of curve.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGeBoundBlock2d boundBlock() const;

  /** \details
    Oriented bounding box of curve.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGeBoundBlock2d boundBlock(
    const OdGeInterval& range) const;

  /** \details
    Bounding box whose sides are parallel to coordinate axes.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGeBoundBlock2d orthoBoundBlock() const;
  
  /** \details
    Bounding box whose sides are parallel to coordinate axes.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGeBoundBlock2d orthoBoundBlock(
    const OdGeInterval& range) const;

  /** \details
    Returns bounding box of the whole curve or a part of it.
    The box is axis-aligned in the specified local coordinate system.
    It is guaranteed to enclose the curve, but not guaranteed to be tight/minimal.

    \param range [in]  The part of the curve to be bounded.
    \param coordSystem [in]  The local coordinate system in which box extents are computed.

    \remarks
    If range is unbounded (e.g. by default), then bounding box of the whole curve (which range can be
    obtained by getInterval method) is computed. Otherwise bounding box is computed only for the set of
    points with parameter in the specified range.
    If coordSystem is NULL (e.g. by default), then bounding box is aligned with global coordinate axes.
    Otherwise, the box will be axis-aligned in the local coordinate system specified by coordSystem matrix.
  */
  OdGeExtents2d getGeomExtents(
    const OdGeInterval& range = OdGeInterval(),
    const OdGeMatrix2d* coordSystem = NULL) const;

  /** \details
    Returns true, and the start point, if and only if the parametric interval of the curve has a lower bound.

    \param startPoint [out]  Receives the start point of the interval.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool hasStartPoint(
    OdGePoint2d& startPoint) const;

  /** \details
    Returns true, and the end point, if and only if the parametric interval of the curve has an upper bound.

    \param endPoint [out]  Receives the end point of the interval.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  bool hasEndPoint(
    OdGePoint2d& endPoint) const;

  /** \details
    Returns the point on the curve corresponding to the specified parameter value.

    \param param [in]  Parameter to be evaluated.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGePoint2d evalPoint(
    double param) const;
  
  /** \details
    Returns the point on the curve corresponding to the specified parameter value, and the derviatives at that point.

    \param param [in]  Parameter to be evaluated.
    \param numDeriv [in]  The number of derivatives to be computed 
                 (should not be set larger than 2). 
    \param derivatives [out] Receives an array of derivatives at the point corresponding to param.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGePoint2d evalPoint(
    double param, 
    int numDeriv,
    OdGeVector2dArray& derivatives) const;

  /** \details
    Returns sample points along this curve and their parameter values in the specified arrays.

    \param fromParam [in]  Starting parameter value.
    \param toParam [in]  Ending parameter value.
    \param approxEps [in]  Approximate spacing along a curve.
    \param pointArray [out] Returns an array of sample points.
    \param paramArray [out] Returns an array of parameters at each point.
  */
  void getSamplePoints(
    double fromParam, 
    double toParam, 
    double approxEps, 
    OdGePoint2dArray& pointArray, 
    OdGeDoubleArray& paramArray) const;
  
  /** \details
    Returns sample points along this curve and their parameter values in the specified arrays.

    \param fromParam [in]  Starting parameter value.
    \param toParam [in]  Ending parameter value.
    \param approxEps [in]  Approximate spacing along a curve.
    pointArray Returns an array of sample points.
    paramArray Returns an array of parameters at each point.
  */
  void getSamplePoints(
    int numSample, 
    OdGePoint2dArray& pointArray) const;

  OdGeCurve2d& operator =(const OdGeCurve2d& curve);

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  void getSamplePoints(
    double fromParam, 
    double toParam, 
    double approxEps, 
    OdGePoint2dArray& pointArray, 
    OdGeDoubleArray* pParamArray = NULL) const;

  /** \details
    Appends sample points along this curve and their parameter values to the specified arrays.

    \param fromParam [in]  Starting parameter value.
    \param toParam [in]  Ending parameter value.
    \param approxEps [in]  Approximate spacing along a curve.
    \param pointArray [out]  Receives an array of sample points.
    \param pParamArray [in]  Pointer to an array to receive the parameters at each point.
  */
  void appendSamplePoints(
    double fromParam, 
    double toParam, 
    double approxEps, 
    OdGePoint2dArray& pointArray, 
    OdGeDoubleArray* pParamArray = NULL) const;
 
  /** \details
    Appends sample points along this curve and their parameter values to the specified arrays.

    \param numSample [in]  Number of samples.
    \param pointArray [out]  Receives an array of sample points.
  */
  void appendSamplePoints(
    int numSample, 
    OdGePoint2dArray& pointArray) const;

  /** \details
    Restore 2d parametric curve on surface from 3d curve.

    \param numSample [in]  Number of samples.
    \param pointArray [out]  Receives an array of sample points.
  */

  static OdGeCurve2d* restoreUvCurve(
    const OdGeCurve3d* curve, 
    const OdGeSurface* surface, 
    const OdGeTol& tol = OdGeContext::gTol);

protected:
  /** \param source [in]  Object to be cloned.
  */
  OdGeCurve2d();
  OdGeCurve2d(
    const OdGeCurve2d& source);
};

#include "TD_PackPop.h"

#endif // OD_GE_CURVE_2D_H

