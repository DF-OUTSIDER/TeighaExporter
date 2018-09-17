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
#ifndef __DG_BSPLINESURFACE_H__
#define __DG_BSPLINESURFACE_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgElementIterator.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeNurbSurface.h"
#include "Ge/GeSurfacePtrArray.h"

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineSurface : public OdDgGraphicsElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgBSplineSurface );
public:

  /** \details
     get/set surface display mode flag.
  */
  bool getSurfaceDisplayFlag() const;
  void setSurfaceDisplayFlag( bool value );

/** \details
     get/set surface control net display mode flag.
  */
  bool getControlNetDisplayFlag() const;
  void setControlNetDisplayFlag( bool value );

/** \details
     get/set inside boundaries flag. If this flag is true, than first boundary describe hole into
     b-spline surface. Otherwise first boundary describe border of b-spline surface.

    \remarks
    It's the same as get/setHBitFlag(...).
  */
  bool getFirstBoundaryIsHoleFlag() const;
  void setFirstBoundaryIsHoleFlag( bool bSet );

   /** \details
  Gets all NURB data.
  
  \param uOredrU 			      [out] Order in the u direction. 
  \param uOrverV 			      [out] Order in the v direction.
  \param bRational 		      [out] Determines whether the surface is rational.
  \param bClosedInU         [out] Determines whether the surface is closed in the u direction. 
  \param bClosedInV         [out] Determines whether the surface is closed in the v direction.  
  \param iUNumControlPoints [out] The quantity of control points in the u direction.
  \param iVNumControlPoints [out] The quantity of control points in the v direction.
  \param ctrlPtsArr 		    [out] An array of control points (in WCS coordinates).
  \param weights 			      [out] An array of the weights for each control point.
  \param uKnots 			      [out] An array of the knot values in the u direction.
  \param vKnots 			      [out] An array of the knot values in the v direction.

  \returns Returns eOk if successful or an appropriate error code otherwise.

  \remarks
  The control points are represented with an OdGePoint3d array. 
  The column indexes are for the v direction, and row indexes are for the u direction. 
  For example, if the surface has n control points in the u direction and m control points in the v direction, the array looks like [0,0], [0,1], ...[0,n-1], ..., [m-1,0], [m-1,1], ...[m-1,n-1]. 
  If the surface is rational, the weights array will contain the same number of entries as the ctrlPts array. The values in weights and ctrlPts arrays have the same sequential order as the control points array.
  If the surface is not closed in the u direction, the size of the uKnots vector is greater than the iUNumControlPoints value by the degree + 1. 
  If the surface is closed in the u direction, the size of the uKnots vector is greater than the iUNumControlPoints value by 1. 
  Similarly, if the surface is not closed in the v direction, the size of the vKnots vector is greater than the iUNumControlPoints by the degree + 1. 
  If the surface is closed in the v direction, the size of the vKnots vector is greater than the iUNumControlPoints by 1. 
  */
  OdResult get( OdUInt8& uOrderU, OdUInt8& uOrderV, bool& bRational, bool& bClosedInU, bool& bClosedInV,
                int& iUNumControlPoints, int& iVNumControlPoints, OdGePoint3dArray& ctrlPtsArr, 
                OdGeDoubleArray& weights, OdGeKnotVector& uKnots, OdGeKnotVector& vKnots) const;

  /** \details
  Set new data for the existing b-spline surface.
  
  \param uOredrU 			      [in] A new value for order in the u direction.
  \param uOredrV 			      [in] A new value for order in the v direction. 
  \param bRational 			    [in] Determines whether the surface is rational (new value).
  \param bClosedInU         [in] A new value closed state in the u direction. 
  \param bClosedInV         [in] A new value closed state in the v direction. 
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
  
  If the surface is not closed in the u direction, the size of the uKnots vector must be equal to iUNumControlPoints + uOrderU. 
  If the surface is closed in the u direction, the size of the uKnots vector must be iUNumControlPoints + 1. 
  Similarly, if the surface is not closed in the v direction, the size of the vKnots vector must be equal to iVNumControlPoints + uOrderV. 
  If the surface is closed in the v direction, the size of the vKnots vector must be iVNumControlPoints + 1. 
  Each knot must have a value greater than or equal to the value of its predecessor in the array. 
  
  If any of the passed-in parameters are not valid, the surface will not be created.
  */
  OdResult set (OdUInt8 uOrderU, OdUInt8 uOrderV, bool bRational, bool bClosedInU, bool bClosedInV,
                int iUNumControlPoints, int iVNumControlPoints, const OdGePoint3dArray& ctrlPtsArr, 
                const OdGeDoubleArray& weights, const OdGeKnotVector& uKnots, const OdGeKnotVector& vKnots); 

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
  OdResult getControlPoints(OdUInt32& iUCount, OdUInt32& iVCount, OdGePoint3dArray& points) const;
  
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
  OdResult setControlPoints(OdUInt32 iUCount, OdUInt32 iVCount, const OdGePoint3dArray& points);

  /** \details
  Returns the control point (in WCS coordinates) at the specified position in the control points vector.

  \param iUIndex 	[in]  An index of a control point in the u direction. Index starts from the 0 value.
  \param iVIndex  [in]  An index of a control point in the v direction. Index starts from the 0 value.
  \param point 	  [out] The control point (in WCS coordinates) at specified index.
  
  \returns Returns eOk if a control point was returned successfully. If the surface doesn't contain the control point information, the method returns eFail. 
  Returns eOutOfRange if the uIndex or vIndex are not valid.
  */
  OdResult getControlPointAt(OdUInt32 iUIndex, OdUInt32 iVIndex, OdGePoint3d& point) const;

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
  OdResult setControlPointAt(OdUInt32 iUIndex, OdUInt32 iVIndex, const OdGePoint3d& point);

  /** \details
  Returns the quantity of control points in the u direction.
  
  \param iCount [out] A passed-in parameter for returning the quantity of control points in the u direction.
  
  \returns Returns eOk if successful or eFail if the surface doesn't contain control point information.
  */
  OdResult getNumberOfControlPointsInU(OdUInt32& iCount) const;
  
  /** \details
  Returns the quantity of control points in the v direction.
  
  \param iCount [out] A passed-in parameter for returning the quantity of control points in the v direction.
  
  \returns Returns eOk if successful or eFail if the surface doesn't contain control point information.
  */
  OdResult getNumberOfControlPointsInV(OdUInt32& iCount) const;

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
  OdResult getNumberOfKnotsInU(OdUInt32& iCount) const;
  
  /** \details
  Returns the quantity of knots in the v direction.
  
  \param iCount [out] A passed-in parameter for returning the quantity of knots in the v direction.

  \returns Returns eOk if successful or eFail if the surface doesn't contain knot information.
  */
  OdResult getNumberOfKnotsInV(OdUInt32& iCount) const;

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
  OdResult getWeight(OdUInt32 iUIndex, OdUInt32 iVIndex, double& weight ) const;
  
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
  OdResult setWeight(OdUInt32 iUIndex, OdUInt32 iVIndex, double weight );

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
  OdResult getOrderInU(OdUInt8& uOrderU) const;
  
  /** \details
  Returns the degree of NURBS surface in the v direction.

  \param iDegree [out] A passed-in parameter for returning the degree in the v direction.

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult getOrderInV(OdUInt8& uOrderV) const;

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
  Determines whether the surface is rational.
  If the surface is rational, the method returns true, otherwise the method returns false.
  
  \param bIsRational [out] A passed-in parameter for returning the indicator of whether the surface is rational.

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult isRational(bool& bIsRational) const;

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
  OdResult getNumberOfSpansInU(OdUInt32& span) const;
  OdResult setNumberOfSpansInU(OdUInt32 span);
  
  /** \details
  Returns the simple patches in the v direction.

  \param span [out]  A passed-in parameter for returning the number of simple patches in the v direction.

  \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  OdResult getNumberOfSpansInV(OdUInt32& span) const;
  OdResult setNumberOfSpansInV(OdUInt32 span);

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
  Returns number of boundaries.
  */
  OdUInt32 getBoundariesCount() const;

  /** \details
  Add new boundary to b-spline surface.

  \param arrBoundaryPts [in] Array of OdGePoint2d( uParam, vParam ), that define boundary contour for surface.
  \returns Returns eOk if successful.
  */
  OdResult addBoundary(const OdGePoint2dArray& arrBoundaryPts );

  /** \details
  Return boundary contour points of b-spline surface.

  \param uIndex         [in]  Index of boundary contour.
  \param arrBoundaryPts [out] Array of OdGePoint2d( uParam, vParam ), that define boundary contour for surface.
  \returns Returns eOk if successful.
  */
  OdResult getBoundary( OdUInt32 uIndex, OdGePoint2dArray& arrBoundaryPts ) const;

  /** \details
  Set boundary contour points of b-spline surface.

  \param uIndex         [in]  Index of boundary contour.
  \param arrBoundaryPts [in] Array of OdGePoint2d( uParam, vParam ), that define boundary contour for surface.
  \returns Returns eOk if successful.
  */
  OdResult setBoundary( OdUInt32 uIndex, const OdGePoint2dArray& arrBoundaryPts );

  /** \details
  Delete boundary contour of b-spline surface.

  \param uIndex         [in]  Index of boundary contour.
  \returns Returns eOk if successful.
  */
  OdResult deleteBoundary( OdUInt32 uIndex );

  // OdGiDrawable overridden
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual OdRxObjectPtr clone() const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;

  //generates Ge-analogue of this object;
  //could generate eInvalidInput exception if the object can not be converted
  void generateGeSurface( OdGeNurbSurface& destination ) const;

  // OdDgBSplineSurface
  /** \details
    The flag indicates whether the element is a solid or a hole for closed element types:
      * false is Solid
      * true is Hole
  */
  virtual bool getHoleFlag() const;
  virtual void setHoleFlag(bool flag);
};

typedef OdSmartPtr< OdDgBSplineSurface > OdDgBSplineSurfacePtr;


#include "TD_PackPop.h"

#endif // __DG_BSPLINESURFACE_H__
