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




#ifndef OD_DBCURVE_H
#define OD_DBCURVE_H

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DbVertex.h"

class OdDbSpline;
typedef OdSmartPtr<OdDbSpline> OdDbSplinePtr;

class OdDbCurveImpl;
class OdDbCurve;

/** \details
This template class is a specialization of the OdSmartPtr class for OdDbCurve object pointers.
*/
typedef OdSmartPtr<OdDbCurve> OdDbCurvePtr;
/** \details
    The class is the base class for all OdDb curves.

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbCurve : public OdDbEntity
{
public:
  ODDB_DECLARE_MEMBERS(OdDbCurve);

  OdDbCurve();

  /** \details
    Returns true if and only if this Curve object is closed.
  */
  virtual bool isClosed() const;

  /** \details
    Returns true if and only if this Curve object is periodic.
  */
  virtual bool isPeriodic() const;

  /** \details
    Returns the parameter corresponding to the start point of this Curve object.

    \param startParam [out]  Receives the start parameter.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getStartParam(
    double& startParam) const = 0;

  /** \details
    Returns the parameter corresponding to the end point of this Curve object.

    \param endParam [out]  Receives the end parameter.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getEndParam(
    double& endParam) const = 0;

  /** \details
    Returns the WCS start point of this Curve object.

    \param startPoint [out]  Receives the start point.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getStartPoint(
    OdGePoint3d& startPoint) const = 0;

  /** \details
    Returns the WCS end point of this Curve object.

    \param endPoint [out]  Receives the end point.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getEndPoint(
    OdGePoint3d& endPoint) const = 0;

  /** \details
    Returns the WCS point on this Curve object corresponding to the specified parameter.
    
    \param param [in]  Parameter specifying point.
    \param pointOnCurve [out]  Receives the point on this Curve object.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
      virtual OdResult getPointAtParam(
    double param, 
    OdGePoint3d& pointOnCurve) const = 0;

  /** \details
    Returns the parameter corresponding to the specified WCS point on this Curve object.

    \param param [out]  Receives the parameter corresponding to pointOnCurve.
    \param pointOnCurve [in]  Point to be evaluated.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
      virtual OdResult getParamAtPoint(
    const OdGePoint3d& pointOnCurve, 
    double& param) const = 0;

  /** \details
    Returns the distance along this Curve object from its start point to the point specified by param.
    
    \param param [in]  Parameter specifying point.
    \param dist [out]  Receives the distance. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getDistAtParam (
    double param, 
    double& dist) const = 0;

  /** \details
    Returns the parameter corresponding to the point a specified distance 
    along this Curve object from its start point.
    
    \param param [out]  Receives the parameter.
    \param dist [in]  Distance along the curve. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getParamAtDist (
    double dist, 
    double& param) const = 0;

  /** \details
    Returns the distance along this Curve object from its start point to the specified WCS point.
    
    \param pointOnCurve [in]  Point on the curve.
    \param dist [out]  Receives the distance. 
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getDistAtPoint (
    const OdGePoint3d& pointOnCurve, 
    double& dist)const;

  /** \details
    Returns the WCS point a specified distance along this Curve object from its start point.
    
    \param pointOnCurve [out]  Receives the point.
    \param dist [in]  Distance along the curve. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getPointAtDist (
    double dist, 
    OdGePoint3d& pointOnCurve) const;
  
  /** \details
    Returns the first derivative of this Curve object at the specified WCS point.
    
    \param param [in]  Parameter specifying point.
    \param firstDeriv [out]  Receives the first derivative. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getFirstDeriv (
    double param,
    OdGeVector3d& firstDeriv) const = 0;
  
  /** \details
    Returns the first derivative of this Curve object at the specified WCS point.
    
    \param pointOnCurve [in]  Point on the curve.
    \param firstDeriv [out]  Receives the first derivative. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getFirstDeriv (
    const OdGePoint3d& pointOnCurve,
    OdGeVector3d& firstDeriv) const;

  /** \details
    Returns the second derivative of this Curve object at the specified point.
    
    \param param [in]  Parameter specifying point.
    \param secondDeriv [out]  Receives the first derivative. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getSecondDeriv (
    double param,
    OdGeVector3d& secondDeriv) const;
  
  /** \details
    Returns the second derivative of this Curve object at the specified point.
    
    \param pointOnCurve [in]  Point on the curve.
    \param secondDeriv [out]  Receives the first derivative. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getSecondDeriv (
    const OdGePoint3d& pointOnCurve,
    OdGeVector3d& secondDeriv) const;

  /** \details
    Returns the point on this Curve object closest to the given point.
    
    \param givenPoint [in]  Given point.
    \param pointOnCurve [out]  Receives the closed point on this Curve object.
    \param extend [in]  Extends this Curve object if and only if true.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getClosestPointTo(
    const OdGePoint3d& givenPoint,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const;

  /** \details
    Returns the point on this Curve object closest to the given point.
    
    \param givenPoint [in]  Given point.
    \param pointOnCurve [out]  Receives the closed point on this Curve object.
    \param extend [in]  Extends this Curve object if and only if true.
    \param normal [in]  Normal to the projection plane.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    
    If normal is specified, this Curve object is projected onto the plane
    defined by givenPoint and normal, finds the closest point on
    the projected curve to givenPoint, and projects said closest
    point back onto the plane of this Curve object. It is this point
    that is returned as pointOnCurve. 
  */
  virtual OdResult getClosestPointTo(
    const OdGePoint3d& givenPoint,
    const OdGeVector3d& normal,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const;
  

  /** \details
    Returns OdDbSpline approximation of this Curve object.    
    \param spline [out]  Receives a SmartPointer to the OdDbSpline.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getSpline (
    OdDbSplinePtr* spline) const;

  /** \details
    Extends this Curve object to the specified WCS point.
    
    \param param [in]  Parameter specifying point.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult extend(
    double param);
  
  /** \details
    Extends this Curve object to the specified WCS point.
    
    \param toPoint [in]  Point to which to extend.
    \param extendStart [in]  True to extend start of curve, false to extend end of curve.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult 
    extend(bool extendStart,
    const OdGePoint3d& toPoint);

  /** \details
    Returns the area of this Curve object.

    \param area [out]  Receives the area.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getArea(
    double& area) const;
  
  /** \details
    Creates a curve by projecting this Curve object onto the specified plane
    along its normal.
    
    \param projPlane [in]  Projection plane.
    \param pProjCurve [out]  Receives a SmartPointer to the projected curve.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getOrthoProjectedCurve(const OdGePlane& projPlane,
    OdDbCurvePtr* pProjCurve) const;

  /** \details
    Creates a curve by projecting this Curve object onto the specified plane
    along the specified direction.
    
    \param projPlane [in]  Projection plane.
    \param projDirection [in]  Projection direction.
    \param pProjCurve [out]  Receives a SmartPointer to the projected curve.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getProjectedCurve(const OdGePlane& projPlane,
    const OdGeVector3d& projDirection, OdDbCurvePtr* pProjCurve) const;

  /** \details
    Creates a set of curves representing the offsetting of this Curve object by the specified distance.
    \param offsetDistance [in]  Offset distance.
    \param offsetCurves [out]  Receives an array of SmartPointers to the offset curves.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the offsetCurves array.
  */
  virtual OdResult getOffsetCurves(double offsetDistance,  
    OdRxObjectPtrArray& offsetCurves) const;

  /** \details
    Creates a set of curves representing the offsetting of this Curve object by the specified distance in a
    plane with the specified normal.
    \param offsetDistance [in]  Offset distance.
    \param offsetCurves [out]  Receives an array of SmartPointers to the offset curves.
    \param normal [in]  Normal of the plane.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the offsetCurves array.
  */
  virtual OdResult getOffsetCurvesGivenPlaneNormal(
    const OdGeVector3d& normal, double offsetDistance,
    OdRxObjectPtrArray& offsetCurves) const;          

  /** \details
    Creates a set of curves representing this splitting of this Curve object at the specified (parametric) points. 
    \param params [in]  Array of curve parameters defining the segments.
    \param curveSegments [out]  Receives an array of SmartPointers to the curves segments.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the curveSegments array.
  */
  virtual OdResult getSplitCurves (const OdGeDoubleArray& params,
    OdRxObjectPtrArray& curveSegments) const;

  /** \details
    Creates a set of curves representing this splitting of this Curve object at the specified (parametric) points. 

    \param points [in]  Array of points defining the segments.
    \param curveSegments [out]  Receives an array of SmartPointers to the curves segments.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the curveSegments array.
  */
  virtual OdResult getSplitCurves (const OdGePoint3dArray& points,
    OdRxObjectPtrArray& curveSegments) const;

  virtual OdResult reverseCurve();

  /** \details
    Returns an OdGeCurve3d that is geometrically identical to this OdDbCurve.
    \param pGeCurve [out]  Receives a pointer to an OdGeCurve3d object that is geometrically identical to this OdDbCurve.
    The caller of this function is responsible for deleting this OdGeCurve3d object.
    \param tol [in] Optional tolerance.
    \returns
    Returns eOk if successful.
  */
  virtual OdResult  getOdGeCurve ( OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol ) const;


  /** \details
    Takes an OdGeCurve3d and returns an OdDbCurve that is geometrically identical to the OdGeCurve3d.
    \param geCurve [in]  Reference to an OdGeCurve3d.
    \param pDbCurve [out]  Receives a pointer to an OdDbCurve object that is geometrically identical to geCurve.
    The caller of this function is responsible for deleting this OdDbCurve object or adding it to the database.
    \param normal [in] Optional normal vector. If this parameter is supplied then it must be a valid vector that 
    is perpendicular to the input geCurve, and this vector will become the normal vector of the output pDbCurve.
    If this parameter is not supplied, then this function will compute the normal vector itself.
    \param tol [in] Optional tolerance
    \returns
    Returns eOk if successful.
  */
  static OdResult  createFromOdGeCurve ( const OdGeCurve3d& geCurve, OdDbCurve*& pDbCurve,
                                                               OdGeVector3d *normal = NULL,
                                                               const OdGeTol& tol = OdGeContext::gTol );

  /** \details
    Takes an OdGeCurve3d and sets this OdDbCurve to be geometrically identical to the OdGeCurve3d.
    \param geCurve [in]  reference to an OdGeCurve3d.
    \param normal [in]  Optional normal vector. If this parameter is supplied then it must be a valid vector that 
    is perpendicular to the input geCurve, and this vector will become the normal vector of the output pDbCurve.
    If this parameter is not supplied, then this function will compute the normal vector itself.
    \param tol [in] Optional tolerance.
    \returns
    Returns eOk if successful.
  */
  virtual OdResult  setFromOdGeCurve ( const OdGeCurve3d& geCurve,
                                                            OdGeVector3d *normal = NULL,
                                                            const OdGeTol& tol = OdGeContext::gTol );


  virtual OdInt32 findVertexIndex(const OdDbVertex* vert) const { return 0; }
};

#include "TD_PackPop.h"

#endif

