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




#ifndef ODDB_DBSPLINE_H
#define ODDB_DBSPLINE_H

#include "TD_PackPush.h"

#include "DbCurve.h"
#include "DwgDeclareMembers.h"

class OdGeKnotVector;

/** \details
    This class represents Spline entities in an OdDbDatabase instance.

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbSpline : public OdDbCurve
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbSpline);

  OdDbSpline();
  
  /** \details
     Returns true if and only if this Spline entity is rational (DXF 70, bit 0x04).
  */
  bool isRational() const;
  
  /** \details
    Returns the degree of this Spline entity (DXF 71).
    \remarks
    degree is in the range [1..25].  
  */
  int degree() const;
  
  /** \details
      Increased the degree of this spline to the specified value.
  */
  void elevateDegree(int newDegree);
    
  /** \details
    Returns the number of control points in this Spline entity (DXF 73).
  */
  int numControlPoints() const;
  
  /** \details
    Returns the specified control point of this Spline entity (DXF 10).

    \param controlPointIndex [in]  Control point index.
    \param point [out]  Receives the control point.
  */
  void getControlPointAt(
    int controlPointIndex, 
    OdGePoint3d& point) const;
  
  /** \details
      Sets the specified control point of this Spline entity (DXF 10).

      \param controlPointIndex [in]  Control point index.
      \param point [in]  Control point.
  */
  void setControlPointAt(
    int controlPointIndex, 
    const OdGePoint3d& point);
  
  /** \details
    Returns the number of fit points in this Spline entity (DXF 74).
  */
  int numFitPoints() const;
  
  /** \details
    Returns the specified fit point of this Spline entity (DXF 11).

    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point.
  */
  OdResult getFitPointAt(
    int fitPointIndex, 
    OdGePoint3d& point) const;
  
  /** \details
    Sets the specfied fit point of this Spline entity (DXF 11).

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point.
  */
  void setFitPointAt(
    int fitPointIndex, 
    const OdGePoint3d& point);
  
  /** \details
    Inserts a fit point into this Spline entity after the specified index.
      
    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point.
    
    \remarks
    If fitPointIndex < 0, point is inserted at before the first fit point. 
    
    If fitPointIndex >= numFitPoints(), point is appended to the spline. 
  */
  void insertFitPointAt(
    int fitPointIndex, 
    const OdGePoint3d& point);
  
  /** \details
    Removes the specified fit point from this Spline entity.

    \param fitPointIndex [in]  Fit point index.
  */
  void removeFitPointAt(
    int fitPointIndex);
  
  /** \details
    Returns the curve fitting tolerance for this Spline entity (DXF 44).
    
    \remarks
    This is the maximum drawing unit distance by which the Spline curve can deviate
    from a fit points. 
  */
  double fitTolerance() const;
  
  /** \details
    Sets the curve fitting tolerance for this Spline entity (DXF 44).
    
    \remarks
    This is the maximum drawing unit distance by which the Spline curve can deviate
    from a fit points. 
    
    \param fitTolerance [in]  Geometric tolerance.
  */
  void setFitTol(
    double fitTolerance);
  
  /** \details
    Returns the start point and end point WCS fit tangents for this Spline entity (DXF 12, 13).
    \param startTangent [out]  Receives the start point tangent.
    \param endTangent [out]  Receives the end point tangent.
  */
  OdResult getFitTangents(
    OdGeVector3d& startTangent, 
    OdGeVector3d& endTangent) const;
  
  /** \details
    Sets the start point and end point WCS fit tangents for this Spline entity (DXF 12, 13).
    \param startTangent [out]  Start point tangent.
    \param endTangent [out]  End point tangent.
  */
  void setFitTangents(
    const OdGeVector3d& startTangent, 
    const OdGeVector3d& endTangent);
  
  /** \details
    Returns true if and only if this Spline entity is constructed using fit points.
  */
  bool hasFitData() const;
  
  /** \details
    Returns the fit data for this Spline entity.

    \param fitPoints [out]  Receives the fit points.
    \param degree [out]  Receives the degree.
    \param fitTolerance [out]  Receives the fit tolerance.
    \param tangentsExist [out]  Receives true if and only if start and end tangents are used.
    \param startTangent [out]  Receives the start point tangent.
    \param endTangent [out]  Receives the end point tangent.
  */
  OdResult getFitData(
    OdGePoint3dArray& fitPoints, 
    int& degree, 
    double& fitTolerance, 
    bool& tangentsExist, 
    OdGeVector3d& startTangent, 
    OdGeVector3d& endTangent ) const;
  
  /** \details
    Returns the fit data for this Spline entity.

    \param fitPoints [out]  Receives the fit points.
    \param degree [out]  Receives the degree.
    \param fitTolerance [out]  Receives the fit tolerance.
    \param tangentsExist [out]  Receives true if and only if start and end tangents are used.
    \param startTangent [out]  Receives the start point tangent.
    \param endTangent [out]  Receives the end point tangent.
	\param knotParam [out]  Receives the knot parametrization.
  */
  OdResult getFitData(
    OdGePoint3dArray& fitPoints, 
    int& degree, 
    double& fitTolerance, 
    bool& tangentsExist, 
    OdGeVector3d& startTangent, 
    OdGeVector3d& endTangent,
    OdGe::OdGeKnotParameterization& knotParam) const;
  
  /** \details
    Sets the fit data for this Spline entity.
    \param fitPoints [in]  Fit points.
    \param degree [in]  Degree.
    \param fitTolerance [in]  Fit tolerance.
    \param startTangent [in]  Start point tangent.
    \param endTangent [in]  End point tangent.
  */
  void setFitData(
    const OdGePoint3dArray& fitPoints, 
    int degree, 
    double fitTolerance, 
    const OdGeVector3d& startTangent, 
    const OdGeVector3d& endTangent );
  
  /** \details
    Sets the fit data for this Spline entity.
    \param fitPoints [in]  Fit points.
    \param degree [in]  Degree.
    \param fitTolerance [in]  Fit tolerance.
    \param startTangent [in]  Start point tangent.
    \param endTangent [in]  End point tangent.
    \param knotParam [in]  Knot parametrization.
  */
  void setFitData(
    const OdGePoint3dArray& fitPoints, 
    int degree, 
    double fitTolerance, 
    const OdGeVector3d& startTangent, 
    const OdGeVector3d& endTangent,
    OdGe::OdGeKnotParameterization knotParam);
  
  /** \details
    Purges the fit data for this Spline entity.
  */
  void purgeFitData();
  
  /** \details
      TBC.
      void updateFitData();
  */
  
  /** \details
    Returns the NURBS data for this Spline entity.
    \param degree [out]  Receives the degree.
    \param rational [out]  Receives true if and only if this Spline entity is rational.
    \param closed [out]  Receives true if and only if this Spline entity is closed.
    \param periodic [out]  Receives true if and only if this Spline entity is periodic.
    \param controlPoints [out]  Receives an array of WCS control points.
    \param knots [out]  Receives the knot vector.
    \param weights [out]  Receives an array of weights.
    \param controlPtTol [out]  Receives the control point tolerance.
    \param knotTol [out]  Receives the knot tolerance.
  */
  void getNurbsData(
    int& degree, 
    bool& rational, 
    bool& closed, 
    bool& periodic,
    OdGePoint3dArray& controlPoints, 
    OdGeDoubleArray& knots, 
    OdGeDoubleArray& weights, 
    double& controlPtTol,
    double& knotTol) const;

  /** \details
    Returns the NURBS data for this Spline entity.
    \param degree [out]  Receives the degree.
    \param rational [out]  Receives true if and only if this Spline entity is rational.
    \param closed [out]  Receives true if and only if this Spline entity is closed.
    \param periodic [out]  Receives true if and only if this Spline entity is periodic.
    \param controlPoints [out]  Receives an array of WCS control points.
    \param knots [out]  Receives the knot vector.
    \param weights [out]  Receives an array of weights.
    \param controlPtTol [out]  Receives the control point tolerance.
  */
  void getNurbsData(
    int& degree, 
    bool& rational, 
    bool& closed, 
    bool& periodic,
    OdGePoint3dArray& controlPoints, 
    OdGeKnotVector& knots, 
    OdGeDoubleArray& weights, 
    double& controlPtTol) const;
  
  /** \details
    Sets the NURBS data for this Spline entity.
    \param degree [in]  Degree.
    \param rational [in]  Controls if this Spline entity is rational.
    \param closed [in]  Controls if this Spline entity closed.
    \param periodic [in]  Controls if this Spline entity is periodic.
    \param controlPoints [in]  Array of WCS control points.
    \param knots [in]  Knot vector.
    \param weights [in]  Array of weights.
    \param controlPtTol [in]  Control point tolerance.
    \param knotTol [in]  Knot tolerance.
    
    \remarks
    * degree is in the range of [1..25]
    * If rational is true, controlPoints.length() must equal weights.length()
    * If periodic is false, then knots.length() must equal controlPoints.length() + degree + 1
    * If periodic is true, then knots.length() must equal controlPoints.length(), 
      the first and last controlPoints must be equal, and the first and last weights (if provided) must be equal.
    * If two control points are within controlPtTol, they are treated as the same control point.
    * If two knots are within knotTol, they are treated as the same knot.
  */
  void setNurbsData(int degree, 
    bool rational, 
    bool closed, 
    bool periodic,
    const OdGePoint3dArray& controlPoints, 
    const OdGeDoubleArray& knots, 
    const OdGeDoubleArray& weights,
    double controlPtTol, 
    double knotTol );
 
  /** \details
    Sets the NURBS data for this Spline entity.
    \param degree [in]  Degree.
    \param rational [in]  Controls if this Spline entity is rational.
    \param closed [in]  Controls if this Spline entity closed.
    \param periodic [in]  Controls if this Spline entity is periodic.
    \param controlPoints [in]  Array of WCS control points.
    \param knots [in]  Knot vector.
    \param weights [in]  Array of weights.
    \param controlPtTol [in]  Control point tolerance.
    
    \remarks
    * degree is in the range of [1..25]
    * If rational is true, controlPoints.length() must equal weights.length()
    * If periodic is false, then knots.length() must equal controlPoints.length() + degree + 1
    * If periodic is true, then knots.length() must equal controlPoints.length(), 
      the first and last controlPoints must be equal, and the first and last weights (if provided) must be equal.
    * If two control points are within controlPtTol, they are treated as the same control point.
  */ 
  void setNurbsData(
    int degree, 
    bool rational, 
    bool closed, 
    bool periodic,
    const OdGePoint3dArray& controlPoints, 
    const OdGeKnotVector& knots, 
    const OdGeDoubleArray& weights,
    double controlPtTol);
  
  /** \details
    Returns the specified weight (DXF 41).
    \param weightIndex [in]  Weight index.
  */
  double weightAt(
    int weightIndex) const;
  
  /** \details
    Sets the specified weight (DXF 41).
    \param weightIndex [in]  Weight index.
    \param weight [in]  Weight.
  */
  void setWeightAt(
    int weightIndex, 
    double weight);
  
  /** \details
      Inserts a knot value into this spline.
  */
  void insertKnot(double param);
  
  /** \details
     Reverses this Spline entity.
     
     \remarks
     The start point becomes the end point, and vice versa.
  */
  virtual OdResult reverseCurve();
  
  
  virtual OdResult subGetClassID(
    void* pClsid) const;
  
  /*
     void getOffsetCurvesGivenPlaneNormal(const OdGeVector3d& normal,  //Replace OdRxObjectPtrArray
                                          double offsetDist, 
                                          OdRxObjectPtrArray& offsetCurves ) const;
  */
    
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);
  
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;
  
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);
  
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  virtual bool isPlanar() const;

  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;


  /** \details
      OdDbCurve methods
  */

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

  TD_USING(OdDbCurve::getFirstDeriv);
  TD_USING(OdDbCurve::getSecondDeriv);

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

  TD_USING(OdDbCurve::getSplitCurves);
  
  OdDbObjectPtr decomposeForSave(
    OdDb::DwgVersion ver,
    OdDbObjectId& replaceId,
    bool& exchangeXData);

  /** \details
    Defines the spline method (either by fit points or by control points).
  */
    enum SplineType 
  {
    kFitPoints = 0,     // By fit points.
    kControlPoints = 1  // By control points.
  };

  /** \details
    Returns the spline method (either by fit points or by control points).
  */
    SplineType type() const;

  /** \details
    Sets the spline method (either by fit points or by control points).

    \param type [in]  spline type to be set.

    \returns
    Returns the result of the operation: eOk if successful, or an appropriate error code if not.

    \remarks
    Changing type from kControlPoints to kFitPoints recalculates fit points from control points.
  */
 
  OdResult setType(SplineType type);

  /** \details
    Returns true if Control Vertices frame is visible.
  */
        bool cvFrame() const;

  /** \details
    Sets visibility of Control Vertices frame for this Spline entity.
    \param bVisible [in]  Control vertices frame visiblity.
  */
        void setCvFrame(bool bVisible);

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

  /** \details
    Modifies the position and tangent of the point with a specified parameter.

    \param param [in]  Parameter value of the point.
    \param point [in]  New position of the point.
    \param deriv [in]  New value of the tangent vector at the point.
           
    \returns
    Returns true if and only if the point has successfully been adjusted.
  */
  bool modifyPositionAndTangent(double param, const OdGePoint3d& point, const OdGeVector3d* deriv);

  virtual OdResult getOdGeCurve(OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Explodes this entity into a set of simpler entities.  

    \param entitySet [in/out] Receives an array of pointers to the new entities.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.

    \remarks
    Entities resulting from the explosion are appended to the specified array.
    
    The newly created entities are not database residents.
    
    The default implementation of this function returns eNotApplicable. This function can be
    overridden in custom classes.
  */
  virtual OdResult subExplode(OdRxObjectPtrArray& entitySet) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSpline object pointers.
*/
typedef OdSmartPtr<OdDbSpline> OdDbSplinePtr;

/** \details
    This class represents Spline entities in an OdDbDatabase instance.

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbSplinePE : public OdRxObject
{
public:
    ODRX_DECLARE_MEMBERS(OdDbSplinePE);

  virtual OdResult getParamAtPoint(
    const OdDbCurve* pCurve,
    const OdGePoint3d& pointOnCurve,
    double& param) const = 0;

  virtual OdResult getArea(
    const OdDbCurve* pCurve,
    double& area) const = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDbSplinePE object pointers.
*/
typedef OdSmartPtr<OdDbSplinePE> OdDbSplinePEPtr;

#include "TD_PackPop.h"

#endif

