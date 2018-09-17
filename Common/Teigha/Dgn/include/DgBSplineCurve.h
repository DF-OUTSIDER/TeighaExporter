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
#ifndef __DG_BSPLINECURVE_H__
#define __DG_BSPLINECURVE_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgCurveElement2d.h"
#include "DgCurveElement3d.h"
#include "DgElementIterator.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeNurbCurve3d.h"


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineCurve2d : public OdDgCurveElement2d
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgBSplineCurve2d );
public:

 /** \details
     get/set curve display mode flag.
  */
  bool getCurveDisplayFlag() const;
  void setCurveDisplayFlag( bool value );
/** \details
     get/set curve control polygon display mode flag.
  */
  bool getPolygonDisplayFlag() const;
  void setPolygonDisplayFlag( bool value );

  
  /** \details
    The flag indicates whether the element is a solid or a hole for closed element types:
      * false is Solid
      * true is Hole
  */
  virtual bool getHoleFlag() const;
  virtual void setHoleFlag( bool bSet );

  /** \details
    Returns true if and only if this Curve object is closed.
  */
  virtual bool     isClosed() const;
  virtual bool     getClosedFlag() const;
  virtual OdResult setClosedFlag( bool bSet );

  /** \details
    Returns true if and only if this Curve object is periodic.
  */
  virtual bool isPeriodic() const;

 /** \details
     Returns true if and only if this Spline entity is rational.
  */
  bool isRational() const;
  
  /** \details
    Returns the order of this Spline entity.
    \remarks
    degree is in the range [1..25].  
  */
  OdUInt32 getOrder() const;
    
  /** \details
      Increased the order of this spline to the specified value.
  */
  OdResult elevateOrder(OdUInt32 uNewOrder );

  /** \details
    Returns the number of control points in this curve entity.
  */
  OdUInt32 numControlPoints() const;
  
  /** \details
    Returns the specified control point of this curve entity.

    \param controlPointIndex [in]  Control point index.
    \param point [out]  Receives the control point.
  */
  OdResult getControlPointAt( OdUInt32 controlPointIndex, OdGePoint2d& point) const;
  
  /** \details
      Sets the specified control point of this curve entity.

      \param controlPointIndex [in]  Control point index.
      \param point [in]  Control point.
  */
  OdResult setControlPointAt( OdUInt32 controlPointIndex, const OdGePoint2d& point);
  
  /** \details
    Returns the number of fit points in this Spline entity.
  */
  OdUInt32 numFitPoints() const;
  
  /** \details
    Returns the specified fit point of this Spline entity.

    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point.
  */
  OdResult getFitPointAt( OdUInt32 fitPointIndex, OdGePoint2d& point) const;
  
  /** \details
    Sets the specfied fit point of this Spline entity.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point.
  */
  OdResult setFitPointAt( OdUInt32 fitPointIndex, const OdGePoint2d& point);
  
  /** \details
    Inserts a fit point into this curve entity after the specified index.
      
    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point.
    
    \remarks
    If fitPointIndex < 0, point is inserted at before the first fit point. 
    
    If fitPointIndex >= numFitPoints(), point is appended to the spline. 
  */
  OdResult insertFitPointAt( OdInt32 fitPointIndex, const OdGePoint2d& point);
  
  /** \details
    Removes the specified fit point from this Spline entity.

    \param fitPointIndex [in]  Fit point index.
  */
  OdResult removeFitPointAt( OdUInt32 fitPointIndex);
  
  /** \details
    Returns the start point and end point WCS fit tangents for this curve entity.
    \param startTangent [out]  Receives the start point tangent.
    \param endTangent [out]  Receives the end point tangent.
  */
  OdResult getFitTangents( OdGeVector2d& startTangent, OdGeVector2d& endTangent) const;
  
  /** \details
    Sets the start point and end point WCS fit tangents for this curve entity.
    \param startTangent [out]  Start point tangent.
    \param endTangent [out]  End point tangent.
  */
  OdResult setFitTangents( const OdGeVector2d& startTangent, const OdGeVector2d& endTangent);

  /** \details
    Control mode of start and end tangent calculation. If true than used natural end tangents formula,
    otherwise used Bessel end tangent formula.
  */
  bool getNaturalTangentsFlag() const;
  void setNaturalTangentsFlag( bool value );

  /** \details
    Control scale of start and end tangents. If true than tangent scaled by 1/3 of the chord length,
    defined by first two points or last two points. Otherwise tangents are scaled by Bessel length tangent 
    formula.
  */
  bool getChordLenTangentsFlag() const;
  void setChordLenTangentsFlag( bool value );

  /** \details
    Only for closed curves. If true than b-spline curve will have a geometrically smooth joint at the end point,
    otherwise there is no such enforcement applied.
  */
  bool getColinearTangentsFlag() const;
  void setColinearTangentsFlag( bool value );
  
  /** \details
    Returns true if and only if this Spline entity is constructed using fit points.
  */
  bool hasFitData() const;
  
  /** \details
    Returns the fit data for this Spline entity.

    \param fitPoints [out]  Receives the fit points.
    \param uOrder [out]  Receives the order.
    \param tangentsExist [out]  Receives true if and only if start and end tangents are used.
    \param startTangent [out]  Receives the start point tangent.
    \param endTangent [out]  Receives the end point tangent.
	  \param bUniformKnots [out]  Receives the knot parametrization type (uniform or chordLength).
  */
  OdResult getFitData( 
    OdGePoint2dArray& fitPoints, 
    OdUInt32& uOrder, 
    bool& tangentsExist, 
    OdGeVector2d& startTangent, 
    OdGeVector2d& endTangent ) const;
  
  OdResult getFitData(
    OdGePoint2dArray& fitPoints, 
    OdUInt32& uOrder,  
    bool& tangentsExist, 
    OdGeVector2d& startTangent, 
    OdGeVector2d& endTangent,
    bool& bUniformKnots ) const;
  
  /** \details
    Sets the fit data for this Spline entity.
    \param fitPoints [in]  Fit points.
    \param startTangent [in]  Start point tangent.
    \param endTangent [in]  End point tangent.
	  \param endTangent [in]  Knot parametrization.
  */
  OdResult setFitData(
    const OdGePoint2dArray& fitPoints, 
    const OdGeVector2d& startTangent, 
    const OdGeVector2d& endTangent,
    bool  bClosed );

  OdResult setFitData(
    const OdGePoint2dArray& fitPoints, 
    const OdGeVector2d& startTangent, 
    const OdGeVector2d& endTangent,
    bool  bClosed,
    bool  bUniformKnots );
  
  /** \details
    Purges the fit data for this curve entity.
  */
  void purgeFitData();
  
  /** \details
    Returns the NURBS data for this Spline entity.
    \param order [out]  Receives the degree.
    \param rational [out]  Receives true if and only if this Spline entity is rational.
    \param closed [out]  Receives true if and only if this Spline entity is closed.
    \param controlPoints [out]  Receives an array of WCS control points.
    \param knots [out]  Receives the knot vector.
    \param weights [out]  Receives an array of weights.
  */
  OdResult getNurbsData(
    OdUInt32& order, 
    bool& rational, 
    bool& closed,
    OdGePoint2dArray& controlPoints, 
    OdGeKnotVector& knots, 
    OdGeDoubleArray& weights) const;
  
  /** \details
    Sets the NURBS data for this Spline entity.
    \param order [in]  Order.
    \param rational [in]  Controls if this curve entity is rational.
    \param closed [in]  Controls if this curve entity closed.
    \param controlPoints [in]  Array of WCS control points.
    \param knots [in]  Knot vector.
    \param weights [in]  Array of weights.
    
    \remarks
    * degree is in the range of [1..25]
    * If rational is true, controlPoints.length() must equal weights.length()
    * For closed curve knots.size() == controlPoints.size() + 1 
      ( For example (0.0, 0.1, 0.5, 0.7, 1.0) for 4 control points )
      For opened curve knots.size() == controlPoints.size() + order 
      ( For example (0.0, 0.0, 0.0, 0.1, 0.5, 0.7, 1.0, 1.0, 1.0 ) for 6 control points, and order = 3 )
      if knots.size() == 0 or something wrong with knot values, than uniform 
      knots will be generated.
  */

  OdResult setNurbsData( 
    OdUInt32 order, 
    bool rational, 
    bool closed, 
    const OdGePoint2dArray& controlPoints, 
    const OdGeKnotVector& knots, 
    const OdGeDoubleArray& weights);
  
  /** \details
    Returns the specified weight (DXF 41).
    \param weightIndex [in]  Weight index.
  */
  double weightAt(
    OdUInt32 weightIndex) const;
  
  /** \details
    Sets the specified weight (DXF 41).
    \param weightIndex [in]  Weight index.
    \param weight [in]  Weight.
  */
  OdResult setWeightAt(
    OdUInt32 weightIndex, 
    double weight);

  /** \details
    Returns the knots of this curve entity.
  */
  OdGeKnotVector getKnots();
  
  /** \details
      Inserts a knot value into this spline.
  */
  OdResult insertKnot(double param);
  
  /** \details
      OdDgCurveElement2d methods
  */

  /** \details
     Reverses this Spline entity.
     
     \remarks
     The start point becomes the end point, and vice versa.
  */

  virtual OdResult reverseCurve();

  virtual OdResult getPointAtParam(
    double param, 
    OdGePoint2d& pointOnCurve) const;

  virtual OdResult getParamAtPoint(
    const OdGePoint2d& pointOnCurve, 
    double& param) const;

  virtual OdResult getStartParam(
    double& startParam) const;

  virtual OdResult getEndParam (
    double& endParam) const;

  virtual OdResult getStartPoint(
    OdGePoint2d& startPoint) const;

  virtual OdResult getEndPoint(
    OdGePoint2d& endPoint) const;

  virtual OdResult getDistAtParam( double param, double& dist ) const;

  virtual OdResult getParamAtDist( double dist, double& param ) const;

  virtual OdResult getFirstDeriv (
    double param,
    OdGeVector2d& firstDeriv) const;

  virtual OdResult getSecondDeriv (
    double param,
    OdGeVector2d& secondDeriv) const;

  virtual OdResult getArea(
    double& area) const;

  TD_USING(OdDgCurveElement2d::getFirstDeriv);
  TD_USING(OdDgCurveElement2d::getSecondDeriv);

  /** \details
    Returns pointers to the curves that result from splitting this curve at the points corresponding 
    to params.

    \param params [in]  The parameter values corresponding to the split points.
    \param entitySet [out]  Receives an array of pointers to the split curves.
    
    \remarks
    * The first split curve will be from the start of this curve to params[0].
    * The second split curve will be from params[0] to params[1].
    * The last split curve will be from params[n-1] to the end of his curve.
  */
        virtual OdResult getSplitCurves(
    const OdGeDoubleArray& params, 
    OdRxObjectPtrArray& entitySet) const;

  TD_USING(OdDgCurveElement2d::getSplitCurves);

  /** \details
    Inserts a control point at the specified knot.
    
    \param knotParam [in]  The knot parameter to add the control point at.
    \param ctrlPt [in]  Coordinates of the control point to be added.
    \param weight [in] The weight to be assigned to the control point.
    
    \remarks
    When called for a non-rational spline, weight is ignored.
	
	  \returns
    eOk if successful, or an appropriate error code if not.
   */
  OdResult insertControlPointAt(double knotParam, const OdGePoint2d& ctrlPt, double weight = 1.0);

  /** \details
    Deletes a control point at the specified index.
    
    \param index [in]  The index of the control point to be deleted.
	
	  \returns
    eOk if successful, or an appropriate error code if not.
  */
  OdResult removeControlPointAt(int index);

/** \details
    Returns true if the spline doesn't contain control points.
*/
  bool isNull() const;

  virtual OdResult getOdGeCurve(OdGeCurve2d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve2d& geCurve, const OdGeTol& tol = OdGeContext::gTol);

  // OdGiDrawable overridden
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pDrwTraits) const;

  virtual OdRxObjectPtr clone() const;

  //generates Ge-analogue of this object;
  //could generate eInvalidInput exception if the object can not be converted
  void generateGeCurve( OdGeNurbCurve3d& destination ) const;
};

typedef OdSmartPtr< OdDgBSplineCurve2d > OdDgBSplineCurve2dPtr;

/** \details
    This class represents b-spline curve 2d entities in an OdDgDatabase instance.

    \sa
    TG_Db
    <group OdDb_Classes> 
*/
class TG_EXPORT OdDgBSplineCurve2dPE : public OdRxObject
{
public:
    ODRX_DECLARE_MEMBERS(OdDgBSplineCurve2dPE);

  virtual OdResult getParamAtPoint(
    const OdDgBSplineCurve2d* pCurve,
    const OdGePoint2d& pointOnCurve,
    double& param) const = 0;

  virtual OdResult getArea(
    const OdDgBSplineCurve2d* pCurve,
    double& area) const = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgBSplineCurve2dPE object pointers.
*/
typedef OdSmartPtr<OdDgBSplineCurve2dPE> OdDgBSplineCurve2dPEPtr;


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineCurve3d : public OdDgCurveElement3d
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgBSplineCurve3d );
public:
/** \details
     get/set curve display mode flag.
  */
  bool getCurveDisplayFlag() const;
  void setCurveDisplayFlag( bool value );
/** \details
     get/set curve control polygon display mode flag.
  */
  bool getPolygonDisplayFlag() const;
  void setPolygonDisplayFlag( bool value );

  
  /** \details
    The flag indicates whether the element is a solid or a hole for closed element types:
      * false is Solid
      * true is Hole
  */
  virtual bool getHoleFlag() const;
  virtual void setHoleFlag( bool bSet );

  /** \details
    Returns true if and only if this Curve object is closed.
  */
  virtual bool isClosed() const;
  virtual bool getClosedFlag() const;
  virtual OdResult setClosedFlag( bool bSet );

  /** \details
    Returns true if and only if this Curve object is periodic.
  */
  virtual bool isPeriodic() const;

 /** \details
     Returns true if and only if this Spline entity is rational.
  */
  bool isRational() const;
  
  /** \details
    Returns the order of this Spline entity.
    \remarks
    degree is in the range [1..25].  
  */
  OdUInt32 getOrder() const;
  
  /** \details
      Increased the order of this spline to the specified value.
  */
  OdResult elevateOrder(OdUInt32 uNewOrder );
    
  /** \details
    Returns the number of control points in this curve entity.
  */
  OdUInt32 numControlPoints() const;
  
  /** \details
    Returns the specified control point of this curve entity.

    \param controlPointIndex [in]  Control point index.
    \param point [out]  Receives the control point.
  */
  OdResult getControlPointAt( OdUInt32 controlPointIndex, OdGePoint3d& point) const;
  
  /** \details
      Sets the specified control point of this curve entity.

      \param controlPointIndex [in]  Control point index.
      \param point [in]  Control point.
  */
  OdResult setControlPointAt( OdUInt32 controlPointIndex, const OdGePoint3d& point);
  
  /** \details
    Returns the number of fit points in this Spline entity.
  */
  OdUInt32 numFitPoints() const;
  
  /** \details
    Returns the specified fit point of this Spline entity.

    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point.
  */
  OdResult getFitPointAt( OdUInt32 fitPointIndex, OdGePoint3d& point) const;
  
  /** \details
    Sets the specfied fit point of this Spline entity.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point.
  */
  OdResult setFitPointAt( OdUInt32 fitPointIndex, const OdGePoint3d& point);
  
  /** \details
    Inserts a fit point into this curve entity after the specified index.
      
    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point.
    
    \remarks
    If fitPointIndex < 0, point is inserted at before the first fit point. 
    
    If fitPointIndex >= numFitPoints(), point is appended to the spline. 
  */
  OdResult insertFitPointAt( OdInt32 fitPointIndex, const OdGePoint3d& point);
  
  /** \details
    Removes the specified fit point from this Spline entity.

    \param fitPointIndex [in]  Fit point index.
  */
  OdResult removeFitPointAt( OdUInt32 fitPointIndex);
  
  /** \details
    Returns the start point and end point WCS fit tangents for this curve entity.
    \param startTangent [out]  Receives the start point tangent.
    \param endTangent [out]  Receives the end point tangent.
  */
  OdResult getFitTangents( OdGeVector3d& startTangent, OdGeVector3d& endTangent) const;
  
  /** \details
    Sets the start point and end point WCS fit tangents for this curve entity.
    \param startTangent [out]  Start point tangent.
    \param endTangent [out]  End point tangent.
  */
  OdResult setFitTangents( const OdGeVector3d& startTangent, const OdGeVector3d& endTangent);

  /** \details
    Control mode of start and end tangent calculation. If true than used natural end tangents formula,
    otherwise used Bessel end tangent formula.
  */
  bool getNaturalTangentsFlag() const;
  void setNaturalTangentsFlag( bool value );

  /** \details
    Control scale of start and end tangents. If true than tangent scaled by 1/3 of the chord length,
    defined by first two points or last two points. Otherwise tangents are scaled by Bessel length tangent 
    formula.
  */
  bool getChordLenTangentsFlag() const;
  void setChordLenTangentsFlag( bool value );

  /** \details
    Only for closed curves. If true than b-spline curve will have a geometrically smooth joint at the end point,
    otherwise there is no such enforcement applied.
  */
  bool getColinearTangentsFlag() const;
  void setColinearTangentsFlag( bool value );
  
  /** \details
    Returns true if and only if this Spline entity is constructed using fit points.
  */
  bool hasFitData() const;
  
  /** \details
    Returns the fit data for this Spline entity.

    \param fitPoints [out]  Receives the fit points.
    \param uOrder [out]  Receives the order.
    \param tangentsExist [out]  Receives true if and only if start and end tangents are used.
    \param startTangent [out]  Receives the start point tangent.
    \param endTangent [out]  Receives the end point tangent.
  */
  OdResult getFitData( 
    OdGePoint3dArray& fitPoints, 
    OdUInt32& uOrder, 
    bool& tangentsExist, 
    OdGeVector3d& startTangent, 
    OdGeVector3d& endTangent ) const;
  
  /** \details
    Returns the fit data for this Spline entity.

    \param fitPoints [out]  Receives the fit points.
    \param uOrder [out]  Receives the order.
    \param tangentsExist [out]  Receives true if and only if start and end tangents are used.
    \param startTangent [out]  Receives the start point tangent.
    \param endTangent [out]  Receives the end point tangent.
	  \param bUniformKnots [out]  Receives the knot parametrization type (uniform or chordLength).
  */
  OdResult getFitData(
    OdGePoint3dArray& fitPoints, 
    OdUInt32& uOrder,  
    bool& tangentsExist, 
    OdGeVector3d& startTangent, 
    OdGeVector3d& endTangent,
    bool& bUniformKnots ) const;
  
  /** \details
    Sets the fit data for this Spline entity.
    \param fitPoints [in]  Fit points.
    \param startTangent [in]  Start point tangent.
    \param endTangent [in]  End point tangent.
	  \param bUniformKnots [in]  Knot parametrization.
  */
  OdResult setFitData(
    const OdGePoint3dArray& fitPoints, 
    const OdGeVector3d& startTangent, 
    const OdGeVector3d& endTangent,
    bool  bClosed );

  /** \details
    Sets the fit data for this Spline entity.
    \param fitPoints [in]  Fit points.
    \param startTangent [in]  Start point tangent.
    \param endTangent [in]  End point tangent.
	  \param bUniformKnots [in]  Knot parametrization.
  */
  OdResult setFitData(
    const OdGePoint3dArray& fitPoints, 
    const OdGeVector3d& startTangent, 
    const OdGeVector3d& endTangent,
    bool  bClosed,
    bool  bUniformKnots );
  
  /** \details
    Purges the fit data for this curve entity.
  */
  void purgeFitData();
  
  /** \details
    Returns the NURBS data for this Spline entity.
    \param order [out]  Receives the degree.
    \param rational [out]  Receives true if and only if this Spline entity is rational.
    \param closed [out]  Receives true if and only if this Spline entity is closed.
    \param controlPoints [out]  Receives an array of WCS control points.
    \param knots [out]  Receives the knot vector.
    \param weights [out]  Receives an array of weights.
  */
  OdResult getNurbsData(
    OdUInt32& order, 
    bool& rational, 
    bool& closed,
    OdGePoint3dArray& controlPoints, 
    OdGeKnotVector& knots, 
    OdGeDoubleArray& weights) const;
  
  /** \details
    Sets the NURBS data for this Spline entity.
    \param order [in]  Order.
    \param rational [in]  Controls if this curve entity is rational.
    \param closed [in]  Controls if this curve entity closed.
    \param controlPoints [in]  Array of WCS control points.
    \param knots [in]  Knot vector.
    \param weights [in]  Array of weights.
    
    \remarks
    * degree is in the range of [1..25]
    * If rational is true, controlPoints.length() must equal weights.length()
    * For closed curve knots.size() == controlPoints.size() + 1 
      ( For example (0.0, 0.1, 0.5, 0.7, 1.0) for 4 control points )
      For opened curve knots.size() == controlPoints.size() + order 
      ( For example (0.0, 0.0, 0.0, 0.1, 0.5, 0.7, 1.0, 1.0, 1.0 ) for 6 control points, and order = 3 )
      if knots.size() == 0 or something wrong with knot values, than uniform 
      knots will be generated.
  */

  OdResult setNurbsData(
    OdUInt32 order, 
    bool rational, 
    bool closed, 
    const OdGePoint3dArray& controlPoints, 
    const OdGeKnotVector& knots, 
    const OdGeDoubleArray& weights);
  
  /** \details
    Returns the specified weight (DXF 41).
    \param weightIndex [in]  Weight index.
  */
  double weightAt(
    OdUInt32 weightIndex) const;
  
  /** \details
    Sets the specified weight (DXF 41).
    \param weightIndex [in]  Weight index.
    \param weight [in]  Weight.
  */
  OdResult setWeightAt(
    OdUInt32 weightIndex, 
    double weight);
  
  /** \details
    Returns the knots of this curve entity.
  */
  OdGeKnotVector getKnots();

  /** \details
      Inserts a knot value into this spline.
  */
  OdResult insertKnot(double param);
  
  /** \details
      OdDgCurveElement2d methods
  */

  /** \details
     Reverses this Spline entity.
     
     \remarks
     The start point becomes the end point, and vice versa.
  */

  virtual OdResult reverseCurve();

  virtual OdResult getPointAtParam(
    double param, 
    OdGePoint3d& pointOnCurve) const;

  virtual OdResult getParamAtPoint(
    const OdGePoint3d& pointOnCurve, 
    double& param) const;

  virtual OdResult getStartParam(
    double& startParam) const;

  virtual OdResult getEndParam (
    double& endParam) const;

  virtual OdResult getStartPoint(
    OdGePoint3d& startPoint) const;

  virtual OdResult getEndPoint(
    OdGePoint3d& endPoint) const;

  virtual OdResult getDistAtParam( double param, double& dist ) const;

  virtual OdResult getParamAtDist( double dist, double& param ) const;

  virtual OdResult getFirstDeriv (
    double param,
    OdGeVector3d& firstDeriv) const;

  virtual OdResult getSecondDeriv (
    double param,
    OdGeVector3d& secondDeriv) const;

  virtual OdResult getArea(
    double& area) const;

  TD_USING(OdDgCurveElement3d::getFirstDeriv);
  TD_USING(OdDgCurveElement3d::getSecondDeriv);

  /** \details
    Returns pointers to the curves that result from splitting this curve at the points corresponding 
    to params.

    \param params [in]  The parameter values corresponding to the split points.
    \param entitySet [out]  Receives an array of pointers to the split curves.
    
    \remarks
    * The first split curve will be from the start of this curve to params[0].
    * The second split curve will be from params[0] to params[1].
    * The last split curve will be from params[n-1] to the end of his curve.
  */
        virtual OdResult getSplitCurves(
    const OdGeDoubleArray& params, 
    OdRxObjectPtrArray& entitySet) const;

  TD_USING(OdDgCurveElement3d::getSplitCurves);

  /** \details
    Inserts a control point at the specified knot.
    
    \param knotParam [in]  The knot parameter to add the control point at.
    \param ctrlPt [in]  Coordinates of the control point to be added.
    \param weight [in] The weight to be assigned to the control point.
    
    \remarks
    When called for a non-rational spline, weight is ignored.
	
	  \returns
    eOk if successful, or an appropriate error code if not.
   */
  OdResult insertControlPointAt(double knotParam, const OdGePoint3d& ctrlPt, double weight = 1.0);

  /** \details
    Deletes a control point at the specified index.
    
    \param index [in]  The index of the control point to be deleted.
	
	  \returns
    eOk if successful, or an appropriate error code if not.
  */
  OdResult removeControlPointAt(int index);

/** \details
    Returns true if the spline doesn't contain control points.
*/
  bool isNull() const;

  virtual OdResult getOdGeCurve(OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol);

  // OdGiDrawable overridden
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pDrwTraits) const;

  virtual OdRxObjectPtr clone() const;

  //generates Ge-analogue of this object;
  //could generate eInvalidInput exception if the object can not be converted
  void generateGeCurve( OdGeNurbCurve3d& destination ) const;
};

typedef OdSmartPtr< OdDgBSplineCurve3d > OdDgBSplineCurve3dPtr;

/** \details
    This class represents b-spline curve 2d entities in an OdDgDatabase instance.

    \sa
    TG_Db
    <group OdDb_Classes> 
*/
class TG_EXPORT OdDgBSplineCurve3dPE : public OdRxObject
{
public:
    ODRX_DECLARE_MEMBERS(OdDgBSplineCurve3dPE);

  virtual OdResult getParamAtPoint(
    const OdDgBSplineCurve3d* pCurve,
    const OdGePoint3d& pointOnCurve,
    double& param) const = 0;

  virtual OdResult getArea(
    const OdDgBSplineCurve3d* pCurve,
    double& area) const = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgBSplineCurve2dPE object pointers.
*/
typedef OdSmartPtr<OdDgBSplineCurve3dPE> OdDgBSplineCurve3dPEPtr;


#include "TD_PackPop.h"

#endif // __DG_BSPLINECURVE_H__
