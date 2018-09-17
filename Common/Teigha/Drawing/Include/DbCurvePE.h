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

#ifndef _OD_DB_CURVE_PE_
#define _OD_DB_CURVE_PE_


#include "DbCurve.h"

/** \details
    This class defines the interface for the Curve Protocol Extension classes.
    
    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbCurvePE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbCurvePE);

  /** \details
    Creates a curve by projecting the specified Curve object onto the specified plane
    along a normal to the place.
    
    \param pCurve [in]  Pointer to the curve to project.
    \param projPlane [in]  Projection plane.
    \param pProjCurve [out]  Receives a SmartPointer to the projected curve.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getOrthoProjectedCurve(const OdDbCurve* pCurve, 
    const OdGePlane& projPlane, 
    OdDbCurvePtr* pProjCurve) const = 0;

 /** \details
    Creates a curve by projecting the specified Curve object onto the specified plane
    along the specified direction.
    
    \param pCurve [in]  Pointer to the curve to project.
    \param projPlane [in]  Projection plane.
    \param projDirection [in]  Projection direction.
    \param pProjCurve [out]  Receives a SmartPointer to the projected curve.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
 */
  virtual OdResult getProjectedCurve(const OdDbCurve* pCurve, 
    const OdGePlane& projPlane,
    const OdGeVector3d& projDirection, 
    OdDbCurvePtr* pProjCurve) const = 0;

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
  virtual OdResult getOffsetCurves(const OdDbCurve* pCurve, 
    double offsetDistance,  
    OdRxObjectPtrArray& offsetCurves) const = 0;

  /** \details
    Creates a set of curves representing the offsetting of the specified Curve object by the specified distance in a
    plane with the specified normal.
    \param pCurve [in]  Pointer to the curve to offset.
    \param offsetDistance [in]  Offset distance.
    \param offsetCurves [out]  Receives an array of SmartPointers to the offset curves.
    \param normal [in]  Normal of the plane.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Pointers to all new curves are appended to the offsetCurves array.
  */
  virtual OdResult getOffsetCurvesGivenPlaneNormal(const OdDbCurve* pCurve, 
    const OdGeVector3d& normal, 
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
  virtual OdResult getSplitCurves (const OdDbCurve* pCurve, 
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
  virtual OdResult getSplitCurves (const OdDbCurve* pCurve, 
    const OdGePoint3dArray& points, 
    OdRxObjectPtrArray& curveSegments) const = 0;

  /** \details
    Extends the specified Curve object to the specified WCS point.
    
    \param pCurve [in]  Pointer to the curve to extend.
    \param param [in]  Parameter specifying point.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult extend(OdDbCurve* pCurve, 
    double param) = 0;

  /** \details
    Extends the specified Curve object to the specified WCS point.
    
    \param pCurve [in]  Pointer to the curve to extend.
    \param toPoint [in]  Point to which to extend.
    \param extendStart [in]  True to extend start of curve, false to extend end of curve.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult extend(OdDbCurve* pCurve, bool extendStart,
    const OdGePoint3d& toPoint) = 0;


  /** \details
    Returns an OdDbSpline approximation of the specified Curve object.
    
    \param pCurve [in]  Pointer to the curve.
    \param spline [out]  Receives a SmartPointer to the OdDbSpline.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getSpline (const OdDbCurve* pCurve, 
    OdDbSplinePtr* spline) const = 0;


  /** \details
    Returns the point on the specified Curve object closest to the given point.
    
    \param pCurve [in]  Pointer to the curve.
    \param givenPoint [in]  Given point.
    \param pointOnCurve [out]  Receives the closed point on this Curve object.
    \param extend [in]  Extends this Curve object if and only if true.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getClosestPointTo(const OdDbCurve* pCurve, 
    const OdGePoint3d& givenPoint,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const = 0;
  
  /** \details
    Returns the point on the specified Curve object closest to the given point.
    
    \param pCurve [in]  Pointer to the curve.
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
  virtual OdResult getClosestPointTo(const OdDbCurve* pCurve, 
    const OdGePoint3d& givenPoint,
    const OdGeVector3d& normal,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbCurvePE object pointers.
*/
typedef OdSmartPtr<OdDbCurvePE> OdDbCurvePEPtr;

#endif //_OD_DB_CURVE_PE_
