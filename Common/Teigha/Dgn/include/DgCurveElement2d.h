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




#ifndef OD_DGCURVEELEMENT2D_H
#define OD_DGCURVEELEMENT2D_H

#include "DgGraphicsElement.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GeCurve2d.h"
#include "Ge/GeNurbCurve2d.h"

class OdDgCurveElement2d;
class OdDgBSplineCurve2d;
typedef OdSmartPtr< OdDgBSplineCurve2d > OdDgBSplineCurve2dPtr;

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgCurveElement2d object pointers.
*/
typedef OdSmartPtr<OdDgCurveElement2d> OdDgCurveElement2dPtr;
/** \details
    The class is the base class for all OdDg 2d curves.

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgCurveElement2d : public OdDgGraphicsElement
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgCurveElement2d);

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
    double& startParam) const;

  /** \details
    Returns the parameter corresponding to the end point of this Curve object.

    \param endParam [out]  Receives the end parameter.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getEndParam(
    double& endParam) const;

  /** \details
    Returns the WCS start point of this Curve object.

    \param startPoint [out]  Receives the start point.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getStartPoint(
    OdGePoint2d& startPoint) const;

  /** \details
    Returns the WCS end point of this Curve object.

    \param endPoint [out]  Receives the end point.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getEndPoint(
    OdGePoint2d& endPoint) const;

  /** \details
    Returns the WCS point on this Curve object corresponding to the specified parameter.
    
    \param param [in]  Parameter specifying point.
    \param pointOnCurve [out]  Receives the point on this Curve object.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
      virtual OdResult getPointAtParam(
    double param, 
    OdGePoint2d& pointOnCurve) const;

  /** \details
    Returns the parameter corresponding to the specified WCS point on this Curve object.

    \param param [out]  Receives the parameter corresponding to pointOnCurve.
    \param pointOnCurve [in]  Point to be evaluated.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
      virtual OdResult getParamAtPoint(
    const OdGePoint2d& pointOnCurve, 
    double& param) const;

  /** \details
    Returns the distance along this Curve object from its start point to the point specified by param.
    
    \param param [in]  Parameter specifying point.
    \param dist [out]  Receives the distance. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getDistAtParam (
    double param, 
    double& dist) const;

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
    double& param) const;

  /** \details
    Returns the distance along this Curve object from its start point to the specified WCS point.
    
    \param pointOnCurve [in]  Point on the curve.
    \param dist [out]  Receives the distance. 
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getDistAtPoint (
    const OdGePoint2d& pointOnCurve, 
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
    OdGePoint2d& pointOnCurve) const;
  
  /** \details
    Returns the first derivative of this Curve object at the specified WCS point.
    
    \param param [in]  Parameter specifying point.
    \param pointOnCurve [in]  Point on the curve.
    \param firstDeriv [out]  Receives the first derivative. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getFirstDeriv (
    double param,
    OdGeVector2d& firstDeriv) const;

  virtual OdResult getFirstDeriv (
    const OdGePoint2d& pointOnCurve,
    OdGeVector2d& firstDeriv) const;

  /** \details
    Returns the second derivative of this Curve object at the specified point.
    
    \param param [in]  Parameter specifying point.
    \param pointOnCurve [in]  Point on the curve.
    \param secondDeriv [out]  Receives the first derivative. 

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getSecondDeriv (
    double param,
    OdGeVector2d& secondDeriv) const;

  virtual OdResult getSecondDeriv (
    const OdGePoint2d& pointOnCurve,
    OdGeVector2d& secondDeriv) const;

  /** \details
    Returns the point on this Curve object closest to the given point.
    
    \param givenPoint [in]  Given point.
    \param pointOnCurve [out]  Receives the closed point on this Curve object.
    \param extend [in]  Extends this Curve object if and only if true.
    \param normal [in]  Normal to the projection plane.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    If normal is specified, this Curve object is projected onto the plane
    defined by givenPoint and normal, finds the closest point on
    the projected curve to givenPoint, and projects said closest
    point back onto the plane of this Curve object. It is this point
    that is returned as pointOnCurve. 
  */
  virtual OdResult getClosestPointTo(
    const OdGePoint2d& givenPoint,
    OdGePoint2d& pointOnCurve, 
    bool extend = false) const;

  /** \details
    Returns OdDgBSplineCurve2d approximation of this Curve object.    
    \param spline [out]  Receives a pointer to the OdDgBSplineCurve2d.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getSpline (
    OdDgBSplineCurve2dPtr& spline) const;

  /** \details
    Extends this Curve object to the specified WCS point.
    
    \param param [in]  Parameter specifying point.
    \param toPoint [in]  Point to which to extend.
    \param extendStart [in]  True to extend start of curve, false to extend end of curve.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult extend(
    double param);
  
  virtual OdResult 
    extend(bool extendStart,
    const OdGePoint2d& toPoint);

  /** \details
    Returns the area of this Curve object.

    \param area [out]  Receives the area.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getArea(
    double& area) const;

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
    Creates a set of curves representing this splitting of this Curve object at the specified (parametric) points. 
    \param params [in]  Array of curve parameters defining the segments.
    \param points [in]  Array of points defining the segments.
    \param curveSegments [out]  Receives an array of SmartPointers to the curves segments.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the curveSegments array.
  */
  virtual OdResult getSplitCurves (const OdGeDoubleArray& params,
    OdRxObjectPtrArray& curveSegments) const;

  virtual OdResult getSplitCurves (const OdGePoint2dArray& points,
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
  virtual OdResult  getOdGeCurve ( OdGeCurve2d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol ) const;


  /** \details
    Takes an OdGeCurve3d and returns an OdDbCurve that is geometrically identical to the OdGeCurve3d.
    \param geCurve [in]  Reference to an OdGeCurve2d.
    \param pDgCurve [out]  Receives a pointer to an OdDgCurveElement2d object that is geometrically identical to geCurve.
    The caller of this function is responsible for deleting this OdDbCurve object or adding it to the database.
    \param tol [in] Optional tolerance
    \returns
    Returns eOk if successful.
  */
  static OdResult  createFromOdGeCurve ( const OdGeCurve2d& geCurve, OdDgCurveElement2d*& pDgCurve,
                                         const OdGeTol& tol = OdGeContext::gTol );

  /** \details
    Takes an OdGeCurve3d and sets this OdDbCurve to be geometrically identical to the OdGeCurve3d.
    \param geCurve [in]  reference to an OdGeCurve2d.
    \param tol [in] Optional tolerance.
    \returns
    Returns eOk if successful.
  */
  virtual OdResult  setFromOdGeCurve ( const OdGeCurve2d& geCurve,
                                       const OdGeTol& tol = OdGeContext::gTol );
};

/** \details
    This class defines the interface for the 2d Curve Protocol Extension classes.
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCurveElement2dPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCurveElement2dPE);

  /** \details
    Creates a set of curves representing the offsetting the specified Curve object by the specified distance.
    \param pCurve [in]  Pointer to the curve to offset.
    \param offsetDistance [in]  Offset distance.
    \param offsetCurves [out]  Receives an array of SmartPointers to the offset curves.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the offsetCurves array.
  */
  virtual OdResult getOffsetCurves(const OdDgCurveElement2d* pCurve, 
    double offsetDistance,  
    OdRxObjectPtrArray& offsetCurves) const = 0;  

  /** \details
    Creates a set of curves representing this splitting of the specified Curve object at the specified (parametric) points. 
    \param pCurve [in]  Pointer to the curve to offset.
    \param params [in]  Array of curve parameters defining the segments.
    \param curveSegments [out]  Receives an array of SmartPointers to the curves segments.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the curveSegments array.
  */
  virtual OdResult getSplitCurves (const OdDgCurveElement2d* pCurve, 
    const OdGeDoubleArray& params, 
    OdRxObjectPtrArray& curveSegments) const = 0;

  /** \details
    Creates a set of curves representing this splitting of the specified Curve object at the specified (parametric) points. 
    \param pCurve [in]  Pointer to the curve to offset.
    \param points [in]  Array of points defining the segments.
    \param curveSegments [out]  Receives an array of SmartPointers to the curves segments.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the curveSegments array.
  */
  virtual OdResult getSplitCurves (const OdDgCurveElement2d* pCurve, 
    const OdGePoint2dArray& points, 
    OdRxObjectPtrArray& curveSegments) const = 0;

  /** \details
    Extends the specified Curve object to the specified WCS point.
    
    \param pCurve [in]  Pointer to the curve to extend.
    \param param [in]  Parameter specifying point.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult extend(OdDgCurveElement2d* pCurve, 
    double param) = 0;

  /** \details
    Extends the specified Curve object to the specified WCS point.
    
    \param pCurve [in]  Pointer to the curve to extend.
    \param toPoint [in]  Point to which to extend.
    \param extendStart [in]  True to extend start of curve, false to extend end of curve.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult extend(OdDgCurveElement2d* pCurve, bool extendStart,
    const OdGePoint2d& toPoint) = 0;


  /** \details
    Returns an OdDbSpline approximation of the specified Curve object.
    
    \param pCurve [in]  Pointer to the curve.
    \param spline [out]  Receives a SmartPointer to the OdDbSpline.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getSpline (const OdDgCurveElement2d* pCurve, 
    OdDgBSplineCurve2dPtr& spline) const = 0;


  /** \details
    Returns the point on the specified Curve object closest to the given point.
    
    \param pCurve [in]  Pointer to the curve.
    \param givenPoint [in]  Given point.
    \param pointOnCurve [out]  Receives the closed point on this Curve object.
    \param extend [in]  Extends this Curve object if and only if true.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getClosestPointTo(const OdDgCurveElement2d* pCurve, 
    const OdGePoint2d& givenPoint,
    OdGePoint2d& pointOnCurve, 
    bool extend = false) const = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbCurvePE object pointers.
*/
typedef OdSmartPtr<OdDgCurveElement2dPE> OdDgCurveElement2dPEPtr;

#endif

