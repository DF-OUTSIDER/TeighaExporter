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


#ifndef _DbNurbSurface_h_Included_
#define _DbNurbSurface_h_Included_

#include "TD_PackPush.h"
#include "DbSurface.h"
#include "DbCurve.h"
#include "Ge/GeNurbSurface.h"

class OdGeKnotVector;

/** \details
  This template class is a specialization of the OdArray class for OdDbCurve object pointer arrays.
*/
typedef OdArray<OdDbCurvePtr> OdDbCurvePtrArray;

/** \details
    This class implements a three-dimensional NURB surface entity.
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbNurbSurface : public OdDbSurface
{
public:
  /** \details 
    Creates an OdDbNurbSurface object.
  */
  OdDbNurbSurface ();
  //virtual ~OdDbNurbSurface();
  ODDB_DECLARE_MEMBERS(OdDbNurbSurface);

  /** \details
  Gets all NURB data.
  
  \param iUDegree 			    [out] Degree in the u direction. 
  \param iVDegree 			    [out] Degree in the v direction.
  \param bRational 		      [out] Determines whether the surface is rational.
  \param iUNumControlPoints [out] The quantity of control points in the u direction.
  \param iVNumControlPoints [out] The quantity of control points in the v direction.
  \param ctrlPtsArr 		    [out] An array of control points (in WCS coordinates).
  \param weights 			      [out] An array of the weights for each control point.
  \param uKnots 			      [out] An array of the knot values in the u direction.
  \param vKnots 			      [out] An array of the knot values in the v direction.

  \returns Returns eOk if successful or an appropriate error code otherwise.

  \remarks
  The control points are represented with an AcGePoint3d array. 
  The column indexes are for the v direction, and row indexes are for the u direction. 
  For example, if the surface has n control points in the u direction and m control points in the v direction, the array looks like [0,0], [0,1], ...[0,n-1], ..., [m-1,0], [m-1,1], ...[m-1,n-1]. 
  If the surface is rational, the weights array will contain the same number of entries as the ctrlPts array. The values in weights and ctrlPts arrays have the same sequential order as the control points array.
  If the surface is not periodic in the u direction, the size of the uKnots vector is greater than the iUNumControlPoints value by the degree + 1. 
  If the surface is periodic in the u direction, the size of the uKnots vector is greater than the iUNumControlPoints value by 1. 
  Similarly, if the surface is not periodic in the v direction, the size of the vKnots vector is greater than the iUNumControlPoints by the degree + 1. 
  If the surface is periodic in the v direction, the size of the vKnots vector is greater than the iUNumControlPoints by 1. 
  */
  OdResult get (int& iUDegree, int& iVDegree, bool& bRational, int& iUNumControlPoints, int& iVNumControlPoints,
                OdGePoint3dArray& ctrlPtsArr, OdGeDoubleArray& weights, OdGeKnotVector& uKnots, OdGeKnotVector& vKnots) const;

  /** \details
  Set new data for the existing NURBS surface.
  
  \param iUDegree 			    [in] A new value for degrees in the u direction.
  \param iVDegree 			    [in] A new value for degrees in the v direction. 
  \param bRational 			    [in] Determines whether the surface is rational (new value).
  \param IUNumControlPoints [in] A new value of the quantity of control points in the u direction.
  \param iVNumControlPoints [in] A new value of the quantity of control points in the v direction. 
  \param ctrlPtsArr 			  [in] A new array of control points (in WCS coordinates).
  \param weights 			      [in] A new array of the weights for each control point.
  \param uKnots 			      [in] A new array of the knot values in the u direction.
  \param vKnots 			      [in] A new array of the knot values in the v direction.

  \returns Returns eOk if successful or an appropriate error code otherwise.
  
  \remarks 
  The column indexes is for the v direction, and row indexes are for the u direction. 
  For example, if the surface has n control points in the u direction and m control points in the v direction, 
  the array looks like [0,0], [0,1], ...[0,n-1], ..., [m-1,0], [m-1,1], ...[m-1,n-1]. 

  If the surface is rational (bRational == true), the weights array must contain the same number of entries as the ctrlPts array. All weight values must be greater than 0.0. 
  The values in weights and ctrlPts arrays must have the same sequential order as the control points array.
  If the surface is not rational (bRational == false), the weight array must be empty.
  
  If the surface is not periodic in the u direction, the size of the uKnots vector must be equal to iUNumControlPoints + iUDegree + 1. 
  If the surface is periodic in the u direction, the size of the uKnots vector must be iUNumControlPoints + 1. 
  Similarly, if the surface is not periodic in the v direction, the size of the vKnots vector must be equal to iVNumControlPoints + iVDegree + 1. 
  If the surface is periodic in the v direction, the size of the vKnots vector must be iVNumControlPoints + 1. 
  Each knot must have a value greater than or equal to the value of its predecessor in the array. 
  
  If any of the passed-in parameters are not valid, the surface will not be created.

  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult set (int iUDegree, int iVDegree, bool bRational, int iUNumControlPoints, int iVNumControlPoints,
                const OdGePoint3dArray& ctrlPtsArr, const OdGeDoubleArray& weights, const OdGeKnotVector& uKnots, const OdGeKnotVector& vKnots); 

  //OdDbObject methods
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  //virtual OdResult subGetClassID(void* pClsid) const;

  /** \details
  Returns the control points and the quantity of control points in both u and v directions.

  \param iUCount  [out] The quantity of control points in u direction.
  \param iVCount  [out] The quantity of control points in v direction.
  \param points   [out] Array of control points (in WCS coordinates).

  \returns Returns eOk if control point were successfully returned, or an appropriate error code in other case.

  \remarks
  The control points will be allocated in the passed-in OdGePoint3d array. 
  The column indexes are for the v direction, and row indexes are for the u direction. 
  For example, if the surface has n control points in the u direction and m control points in the v direction, 
  the array looks like [0,0], [0,1], ...[0,vCount-1], ..., [uCount-1,0], [uCount-1,1], ...[uCount-1,vCount-1]. 
  */
  OdResult getControlPoints(int& iUCount, int& iVCount, OdGePoint3dArray& points) const;
  
  /** \details
  Sets the control points. 

  \param iUCount [in] The new value of the quantity of control points in the u direction.
  \param iVCount [in] The new value of the quantity of control points in the v direction.
  \param points  [in] New array of control points (in WCS coordinates).
  
  \returns Returns eOk if control points were successfully set or an appropriate error code in other case. 
  Returns eFail if the surface doesn't contain the control point information.
  Returns eInvalidInput if the uCount and vCount are not the same as the current numbers.

  \remarks
  The new quantity of control points in the u and v direction must be equal to the current values. 
  New control points are allocated in an AcGePoint3d array. 
  The column indexes are for the v direction, and row indexes are for the u direction. 
  For example, if the surface has n control points in the u direction and m control points in the v direction, the array looks like [0,0], [0,1], ...[0,vCount-1], ..., [uCount-1,0], [uCount-1,1], ...[uCount-1,vCount-1].
  */
  OdResult setControlPoints(int iUCount, int iVCount, const OdGePoint3dArray& points);

  /** \details
  Returns the control point (in WCS coordinates) at the specified position in the control points vector.

  \param iUIndex 	[in]  An index of a control point in the u direction. Index starts from the 0 value.
  \param iVIndex  [in]  An index of a control point in the v direction. Index starts from the 0 value.
  \param point 	  [out] The control point (in WCS coordinates) at specified index.
  
  \returns Returns eOk if a control point was returned successfully. If the surface doesn't contain the control point information, the method returns eFail. 
  Returns eOutOfRange if the uIndex or vIndex are not valid.
  */
  OdResult getControlPointAt(int iUIndex, int iVIndex, OdGePoint3d& point) const;

  /** \details
  Sets a new control point (in WCS coordinates) at a specified position in the control points vector.

  \param iUIndex  [in] An index of a control point in the u direction. Index starts from the 0 value.
  \param iVIndex  [in] An index of a control point in the v direction. Index starts from the 0 value.
  \param point    [in] A new control point (in WCS coordinates) to be set at the specified position.

  \returns Returns eOk if a control point was returned successfully. If the surface doesn't contain the control point information, the method returns eFail. 
  Returns eOutOfRange if the uIndex or vIndex are not valid.
  
  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult setControlPointAt(int iUIndex, int iVIndex, const OdGePoint3d& point);

  /** \details
  Returns the quantity of control points in the u direction.
  
  \param iCount [out] A passed-in parameter for returning the quantity of control points in the u direction.
  
  \returns Returns eOk if successful or eFail if the surface doesn't contain control point information.
  */
  OdResult getNumberOfControlPointsInU(int& iCount) const;
  
  /** \details
  Returns the quantity of control points in the v direction.
  
  \param iCount [out] A passed-in parameter for returning the quantity of control points in the v direction.
  
  \returns Returns eOk if successful or eFail if the surface doesn't contain control point information.
  */
  OdResult getNumberOfControlPointsInV(int& iCount) const;

  /** \details
  Returns the knot vector in the u direction.

  \param knots [out] A passed-in parameter for returning the knot vector in the u direction  

  \returns Returns eOk if successful or eFail if the surface doesn't contain knot information.
  */
  OdResult getUKnots(OdGeKnotVector& knots) const;

  /** \details
  Returns the knot vector in the v direction.

  \param knots [out] A passed-in parameter for returning the knot vector in the v direction  

  \returns Returns eOk if successful or eFail if the surface doesn't contain knot information.
  */
  OdResult getVKnots(OdGeKnotVector& knots) const;

  /** \details
  Returns the quantity of knots in the u direction.
  
  \param iCount [out] A passed-in parameter for returning the quantity of knots in the u direction.

  \returns Returns eOk if successful or eFail if the surface doesn't contain knot information.
  */
  OdResult getNumberOfKnotsInU(int& iCount) const;
  
  /** \details
  Returns the quantity of knots in the v direction.
  
  \param iCount [out] A passed-in parameter for returning the quantity of knots in the v direction.

  \returns Returns eOk if successful or eFail if the surface doesn't contain knot information.
  */
  OdResult getNumberOfKnotsInV(int& iCount) const;

  /** \details
  Returns the weight at a specified position (uIndex, vIndex) in the weights array. 
  Returns valid weight value only when the surface is rational.
  
  \param iUIndex  [in]  An index of weight value in the u direction. Index starts from the 0 value.
  \param iVIndex  [in]  An index of weight value in the v direction. Index starts from the 0 value.
  \param weight   [out] A passed-in parameter for returning the weight value at the specified position.

  \returns Returns eOk if the weight value was successfully returned.  
  If the surface doesn't contain the weight information (weight array is empty or is NULL), returns eFail. 
  If the uIndex or vIndex values are invalid, returns eOutOfRange.
  */
  OdResult getWeight(int iUIndex, int iVIndex, double& weight ) const;
  
  /** \details
  Sets the weight value at a specified position in the weights array.

  If the surface is not rational, the method does nothing.
  
  \param iUIndex  [in] An index of weight value in the u direction. Index starts from the 0 value.
  \param iVIndex  [in] An index of weight value in the v direction. Index starts from the 0 value.
  \param weight   [in] A new weight value to be set at a specified position.

  \returns Returns eOk if a new weight value was successfully set. 
  If the surface doesn't contain the weight information (weight array is empty or is NULL), returns eFail. 
  If the uIndex or vIndex values are invalid, returns eOutOfRange.
  
  \remarks   
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult setWeight(int iUIndex, int iVIndex, double weight );

  /** \details
  Evaluates the position on the NURBS surface at the specified parameter value (u, v).
  
  \param dU   [in]  u direction parameter. 
  \param dV   [in]  v direction parameter.
  \param pos  [out] A passed-in parameter for returning the evaluated position on the surface.
  
  \returns Returns eOk if evaluation is successfully done. 
  If the surface doesn't contain any NURBS information, returns eFail. 
  If the u or v value is out of range, returns eOutOfRange.
  
  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult evaluate(double dU, double dV, OdGePoint3d& pos) const;
  
  /** \details
  Evaluates the position and first derivatives on the NURBS surface at the specified parameter value (u, v).

  \param dU     [in]  u direction parameter. 
  \param dV     [in]  v direction parameter.
  \param pos    [out] A passed-in parameter for returning the evaluated position on the surface.
  \param uDeriv [out] A passed-in parameter for returning the first derivative with respect to u.
  \param vDeriv [out] A passed-in parameter for returning the first derivative with respect to v.

  \returns 
  Returns eOk if evaluation is successfully done. 
  If the surface doesn't contain any NURBS information, returns eFail. 
  If the u or v value is out of range, returns eOutOfRange.
 
  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult evaluate(double dU, double dV, OdGePoint3d& pos, OdGeVector3d& uDeriv, OdGeVector3d& vDeriv) const;

  /** \details
  Evaluates the position and first and second derivatives on the NURBS surface at the specified parameter value (u, v).
  
  \param dU       [in]  u direction parameter. 
  \param dV       [in]  v direction parameter.
  \param pos      [out] A passed-in parameter for returning the evaluated position on the surface.
  \param uDeriv   [out] A passed-in parameter for returning the first derivative with respect to u.
  \param vDeriv   [out] A passed-in parameter for returning the first derivative with respect to v.
  \param uuDeriv  [out] A passed-in parameter for returning the second derivative with respect to u twice.
  \param uvDeriv  [out] A passed-in parameter for returning the second derivative with respect to u and v. 
  \param vvDeriv  [out] A passed-in parameter for returning the second derivative with respect to v twice. 

  \returns Returns eOk if evaluation is successfully done. 
  If the surface doesn't contain any NURBS information, returns eFail. 
  If the u or v value is out of range, returns eOutOfRange.

  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult evaluate(double dU, double dV, OdGePoint3d& pos, OdGeVector3d& uDeriv, OdGeVector3d& vDeriv,
                    OdGeVector3d& uuDeriv, OdGeVector3d& uvDeriv, OdGeVector3d& vvDeriv) const;
  
  /** \details
  Evaluates the position and an arbitrary number of derivatives of the surface at the specified parameter value (u, v).

  \param dU           [in]  u direction parameter. 
  \param dV           [in]  v direction parameter.
  \param iDerivDegree [in]  The degree of derivatives to evaluate.
  \param point        [out] A passed-in parameter for returning the evaluated position on the surface.
  \param derivatives  [out] A passed-in parameter for returning derivatives array of vectors, in the order of uDeriv, vDeriv, uuDeriv, uvDeriv, vvDeriv.

  \returns Returns eOk if evaluation is successfully done. 
  If the surface doesn't contain any NURBS information, returns eFail. 
  If the u or v value is out of range, returns eOutOfRange.

  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult evaluate(double dU, double dV, int iDerivDegree, OdGePoint3d& point, OdGeVector3dArray& derivatives) const;

  /** \details
  Returns the degree of NURBS surface in the u direction.

  \param iDegree [out] A passed-in parameter for returning the degree in the u direction.

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult getDegreeInU(int& iDegree) const;
  
  /** \details
  Returns the degree of NURBS surface in the v direction.

  \param iDegree [out] A passed-in parameter for returning the degree in the v direction.

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult getDegreeInV(int& iDegree) const;
  
  /** \details
  Determines whether the surface is closed in the u direction.
  
  \param bIsClosed [out] A passed-in parameter for returning the indicator of whether the surface is closed in the u direction. 
  bIsClosed is equal to true if the surface is closed in the u direction, otherwise it is equal to false.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult isClosedInU(bool& bIsClosed) const;
  
  /** \details
  Determines whether the surface is closed in the v direction.
  
  \param bIsClosed [out] A passed-in parameter for returning the indicator of whether the surface is closed in the v direction.
  bIsClosed is equal to true if the surface is closed in the v direction, otherwise it is equal to false.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult isClosedInV(bool& bIsClosed) const;

  /** \details
  Determines whether the surface is periodic in the u direction.
  
  \param bIsPeriodic [out] A passed-in parameter for returning the indicator of whether the surface is periodic in the u direction. 
  bIsPeriodic is equal to true if the surface is periodic in the u direction, otherwise it is equal to false.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult isPeriodicInU(bool& bIsPeriodic) const;
  
  /** \details
  Determines whether the surface is periodic in the v direction.
  
  \param bIsPeriodic [out] A passed-in parameter for returning the indicator of whether the surface is periodic in the v direction.
  bIsPeriodic is equal to true if the surface is periodic in the v direction, otherwise it is equal to false.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult isPeriodicInV(bool& bIsPeriodic) const;

  /** \details
  Returns the period value in the u direction for the surface.
  Returned value is valid only if the surface is periodic in the u direction.

  \param dPeriod [out] A passed-in parameter for returning the period value in the u direction.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 

  */
  OdResult getPeriodInU(double& dPeriod) const;
  
  /** \details
  Returns the period value in the v direction for the surface.
  Returned value is valid only if the surface is periodic in the v direction.
  
  \param dPeriod [out] A passed-in parameter for returning the period value in the v direction.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult getPeriodInV(double& dPeriod) const;

  /** \details
  Determines whether the surface is rational.
  If the surface is rational, the method returns true, otherwise the method returns false.
  
  \param bIsRational [out] A passed-in parameter for returning the indicator of whether the surface is rational.

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult isRational(bool& bIsRational) const;

  /** \details
  Determines whether the surface is planar. If the surface is planar, also returns the plane information and normal.
  If the surface is planar, the method returns true via the bIsPlanar parameter, otherwise the method returns false.
  
  \param bIsPlanar    [out] A passed-in parameter for returning the indicator of whether the surface is planar.
  \param ptOnSurface  [out] A passed-in parameter for returning the point on the surface (if the surface is planar).
  \param normal       [out] A passed-in parameter for returning the plane normal (if the surface is planar).

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult isPlanar(bool& bIsPlanar, OdGePoint3d& ptOnSurface, OdGeVector3d& normal) const;
  using OdDbEntity::isPlanar;

  /** \details
  Determines whether a specified point is on the surface.
  If specified point is on the surface, the method returns true via bOnSurface, otherwise the method returns false.
  
  \param point      [in] A point.
  \param bOnSurface [out] A passed-in parameter for returning the indicator of whether the specified point is on the surface.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult isPointOnSurface(const OdGePoint3d& point, bool& bOnSurface) const;

  /** \details
  Returns the normal vector at the specified parameter location ([u, v]).
  
  \param dU     [in]  u parameter.
  \param dV     [in]  v parameter.
  \param normal [out] A passed-in parameter for returning the normal vector.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. If the dU or dV values are invalid, returns eOutOfRange. 
  */
  OdResult getNormal(double dU, double dV, OdGeVector3d& normal) const;

  /** \details
  Returns the simple patches in the u direction.

  \param span [out] A passed-in parameter for returning the number of simple patches in the u direction.

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult getNumberOfSpansInU(int& span) const;
  
  /** \details
  Returns the simple patches in the v direction.

  \param span [out]  A passed-in parameter for returning the number of simple patches in the v direction.

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult getNumberOfSpansInV(int& span) const;

  /** \details
  Returns the isolines at a specified u value.
  The isolines are created in the v direction. 
  If there are holes in the surface, there could be a number of isoline segments at a specified u value.

  \param dU           [in]  u parameter.
  \param lineSegments [out] A passed-in parameter for returning the array of isoline segments.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult getIsolineAtU(double dU, OdDbCurvePtrArray& lineSegments) const;
  
  /** \details
  Returns the isolines at a specified v value.
  The isolines are created in the u direction. 
  If there are holes in the surface, there could be a number of isoline segments at a specified v value.

  \param dV           [in]  v parameter.
  \param lineSegments [out] A passed-in parameter for returning the array of isoline segments.
  
  \returns 
  Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  

  */
  OdResult getIsolineAtV(double dV, OdDbCurvePtrArray& lineSegments) const;

  /** \details
  Inserts a knot at the specified u parameter.
  The knot insertion doesn't modify the shape of the surface. 
  It will add a new row of control points in the v direction and adjust the local control point's location.

  \param dU [in]  u parameter.
  
  \returns Returns eOk if successful.
  If the surface doesn't contain any NURBS information, returns eFail. 
  If the surface's knot multiplicity at the u parameter is bigger than the degree in the u direction, returns eInvalidInput.
  
  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult InsertKnotAtU(double dU);
  
/** \details
  Inserts a knot at the specified v parameter.
  The knot insertion doesn't modify the shape of the surface. 
  It will add a new row of control points in the u direction and adjust the local control point's location.
  
  \param dV [in]  v parameter.
  
  \returns Returns eOk if successful. 
  If the surface doesn't contain any NURBS information, returns eFail. 
  If the surface's knot multiplicity at the v parameter is bigger than the degree in the v direction, returns eInvalidInput.

  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
*/
  OdResult InsertKnotAtV(double dV);

  /** \details
  Inserts a row of control points at the given u knot parameter.

  \param dU [in] u parameter.
  \param vCtrlPts  [in] An array of control points to be added.
  \param vWeights  [in] Array of weight values to add if the surface is rational.
  
  \returns Returns eOk if successful. 
  If the surface doesn't contain any NURBS information, returns eFail. 
  If the dU parameter is out of range, returns eOutOfRange.

  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult InsertControlPointsAtU(double dU, const OdGePoint3dArray& vCtrlPts, const OdGeDoubleArray& vWeights);
  
  /** \details
  Inserts a row of control points at the given v knot parameter.
  
  \param dV       [in] v parameter.
  \param uCtrlPts [in] An array of control points to be added.
  \param vWeights [in] Array of weight values to add if the surface is rational.
  
  \returns Returns eOk if successful. 
  If the surface doesn't contain any NURBS information, returns eFail. 
  If the dV parameter is out of range, returns eOutOfRange.
  
  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult InsertControlPointsAtV(double dV, const OdGePoint3dArray& uCtrlPts, const OdGeDoubleArray& uWeights);
  
  /** \details
  Removes a row of control points at the specified position in the control points array.
  The method modifies the shape of the surface.
  
  \param iUDegree [in] An index of the control point to remove from the array.
  
  \returns Returns eOk if successful. 
  If the surface doesn't contain any NURBS information, returns eFail. 
  
  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult RemoveControlPointsAtU(int iUDegree);
  
  /** \details
  Removes a row of control points at the specified position in the control points array.
  The method modifies the shape of the surface.
  
  \param iVDegree [in] An index of the control point to remove from the array.
  
  \returns Returns eOk if successful. 
  If the surface doesn't contain any NURBS information, returns eFail. 

  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult RemoveControlPointsAtV(int iVDegree);

  /** \details
  Rebuilds the NURBS surface with a specified new degree and quantity of control points in the u and v directions.
  This operation modifies the shape of the surface.
  
  \param iUDegree     [in] The new degree value in the u direction.
  \param iVDegree     [in] The new degree value in the v direction.
  \param iNumUCtrlPts [in] The new quantity of control point in the u direction.
  \param iNumVCtrlPts [in] The new quantity of control point in the v direction.
  \param bRestore     [in] Restore flag.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  
  \remarks 
  This method is not implemented. 
  */
  OdResult rebuild(int iUDegree, int iVDegree, int iNumUCtrlPts, int iNumVCtrlPts, bool bRestore = false);

  /** \details
  Adjusts the location and tangent of a point on the surface with the specified parameter ([u, v]). 
  The local control points are adjusted accordingly. 
  If the pointer uDeriv and vDeriv are equal to NULL, then only the location of the point is adjusted.
  
  \param dU     [in] u parameter.
  \param dV     [in] v parameter.
  \param point  [in] A new location of the point on the surface.
  \param uDeriv [in] The tangent vector in the u direction.
  \param vDeriv [in] The tangent vector in the v direction.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  
  \remarks 
  This method is not implemented. 
  */
  OdResult modifyPositionAndTangent(double dU, double dV, const OdGePoint3d& point, const OdGeVector3d* uDeriv = NULL, const OdGeVector3d* vDeriv = NULL);

  /** \details
  Returns the u and v parameter for a point on the surface.
  
  \param point [in] A point on the surface.
  \param dU    [in] A passed-in parameter for returning the value of the u parameter.
  \param dV    [in] A passed-in parameter for returning the value of the v parameter.
  
  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  
  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  OdResult getParameterOfPoint(const OdGePoint3d& point, double& dU, double& dV) const;

  /** \details
  Creates a surface from a specified surface with conversion.
  
  \param pSource    [in] A pointer to the source surface.
  \param version    [in] .dwg file version of the source surface.
  \param transferId [in] Indicates whether the Id of the source surface is transferred to the surface.
  
  \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  OdResult convertFrom(OdDbSurface* pSource, OdDb::DwgVersion version, bool transferId = true);
  // Overridden methods from AcDbEntity
  //void dragStatus(const AcDb::DragStat status);
  
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbNurbSurface object pointers.
*/  
typedef OdSmartPtr<OdDbNurbSurface> OdDbNurbSurfacePtr;

#include "TD_PackPop.h"
#endif //_DbNurbSurface_h_Included_
