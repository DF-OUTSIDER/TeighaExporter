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




#ifndef OD_DBRAY_H
#define OD_DBRAY_H

#include "TD_PackPush.h"

#include "DbCurve.h"
#include "DwgDeclareMembers.h"

class OdGePoint3d;
class OdGeVector3d;

/** \details
  <group OdDb_Classes>

  This class implements the ray object (Ray entity) in the database. 
  This class inherits the base functionality of entities. A ray is a semi-infinite line.

  \sa
  TD_Db

  \sa
  <link db_ray_sample.html, Example of Working with the Ray Object>

  \sa
  <link db_ray.html, Working with Rays>

  OdDbXline, OdDbLine classes
*/
class DBENT_EXPORT OdDbRay : public OdDbCurve
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbRay);

  /** \details
    Builds an instance of the ray object.
    
    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_ray_overview.html, Overview of Rays>
  */
  OdDbRay();
  
  /*
    void getOffsetCurvesGivenPlaneNormal(
    const OdGeVector3d& normal, double offsetDist,
    OdRxObjectPtrArray& offsetCurves) const; //Replace OdRxObjectPtrArray
  */


  /** \details
    Returns the WCS coordinates of the base point from which the ray outs 
    (DXF 10) as three-dimensional point instance.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_specific.html, Specific Ray Properties>

    OdDbRay::setBasePoint() method
  */
  OdGePoint3d basePoint() const;


  /** \details
    Sets the base point of the from which the ray outs (DXF 10). The initial value is 
    (0,0,0) by default.

    \param geBasePoint [in]  Three-dimensional point instance to set the base point.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_specific.html, Specific Ray Properties>

    OdDbRay::basePoint() method
  */
  void setBasePoint( const OdGePoint3d& geBasePoint ); 


  /** \details
    Returns the WCS unit direction vector of the ray (DXF 11) as three-dimensional 
    vector instance. This vector orients the ray in would space.
    
    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_specific.html, Specific Ray Properties>

    OdDbRay::setUnitDir() method
  */
  OdGeVector3d unitDir() const;


  /** \details
    Sets the direction vector of the ray (DXF 11) in WCS. The initial value is (0,0,1) 
    by default.

    \param geDirVector [in]  Three-dimensional vector instance to set the ray direction.

    \remarks
    The method automatically converts the specified coordinates to a unit vector.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_specific.html, Specific Ray Properties>

    OdDbRay::unitDir() method
  */
  void setUnitDir( const OdGeVector3d& geDirVector );


  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  virtual OdResult subGetClassID(
    void* pClsid) const;

  bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  /** \remarks
    Always returns eInvalidExtents.
  */
  OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const;

  virtual bool isClosed() const;

  virtual bool isPeriodic() const;


  /** \details
    Determines whether the ray entity is planar as a Boolean value and returns True.
    This method is inherited from the OdDbEntity class.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray.html, Working with Rays>

    OdDbRay::getPlane(), OdDbEntity::isPlanar() methods
  */
  virtual bool isPlanar() const;


  /** \details
    Gets an arbitrary plane passed through the ray entity. This method is inherited from the  
    OdDbEntity class.

    \param gePlane [out]  Reference to a variable in which this method must save the plane parameters as 
                a plane instance.
    \param gePlanarity [out] Reference to a variable in which this method must save the plane type 
                    (returns kLinear - arbitrary plane).

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray.html, Working with Rays>

    OdDbRay::isPlanar(), OdDbEntity::getPlane() methods
  */
  virtual OdResult getPlane( OdGePlane& gePlane, OdDb::Planarity& gePlanarity ) const;


  /** \details
    Gets the start parameter value. The start parameter equals a zero for rays.

    \param startParam [out]  Reference to a variable in which this method must save the start    
                   parameter value as a Double value.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_compute.html, Computed Ray Properties>
  */
  virtual OdResult getStartParam( double& startParam ) const;


  /** \details
    This method is not applicable for the ray entity.

    \param endParam [out]  Reference to a variable of a Double type.
  */
  virtual OdResult getEndParam( double& endParam ) const;


  /** \details
    Computes the WCS coordinates of the start point of the ray for which the parameter 
    equals zero. The start point coincides with the base point.

    \param startPoint [out]  Reference to a variable in which this method must save the start 
                   point as three-dimensional point instance.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_compute.html, Computed Ray Properties>
  */
  virtual OdResult getStartPoint( OdGePoint3d& startPoint ) const;


  /** \details
    This method is not applicable for the ray entity.

    \param endPoint [out]  Reference to a variable which is three-dimensional point instance.
  */
  virtual OdResult getEndPoint( OdGePoint3d& endPoint ) const;


  /** \details
    Computes the WCS coordinates of the point on the ray at the specified parameter value.

    \param param [in]  Parameter value specifying the point on the ray as a Double value. 
    \param pointOnCurve [out]  Reference to a variable in which this method must save the 
                     three-dimensional point instance corresponding to the parameter.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_compute.html, Computed Ray Properties>
  */
  virtual OdResult getPointAtParam( double param, OdGePoint3d& pointOnCurve ) const;


  /** \details
    Computes the parameter at the point placed on the ray.

    \param pointOnCurve [in]  Three-dimensional point instance which specifies the point on 
                     the ray. 
    \param param [out] Reference to a variable in which this method must save the parameter   
              value as a Double value.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_compute.html, Computed Ray Properties>
  */
  virtual OdResult getParamAtPoint( const OdGePoint3d& pointOnCurve, double& param ) const;


  /** \details
    Computes the distance along the ray measured from the base point in drawing units   
    at the specified parameter.

    \param param [in]  Parameter value specifying the point on the ray as a Double value. 
    \param dist  [out] Reference to a variable in which this method must save the distance value 
              as a Double value.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_compute.html, Computed Ray Properties>
  */
  virtual OdResult getDistAtParam( double param, double& dist ) const;


  /** \details
    Computes the parameter at the distance along the ray measured from the base point.

    \param dist [in]   Distance value along the ray as a Double value in drawing units. 
    \param param [out]  Reference to a variable in which this method must save the parameter value  
              as a Double value.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_compute.html, Computed Ray Properties>
  */
  virtual OdResult getParamAtDist( double dist, double& param ) const;


  /** \details
    Computes the first derivative for the specified parameter of the ray entity and returns
    the three-dimensional vector instance. The first derivative is constant for any point of 
    the ray.

    \param param [in]  Parameter value specifying the point on the ray as a Double value. 
    \param firstDeriv [out]  Reference to a variable in which this method must save the first derivative  
                   as three-dimensional vector instance.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_compute.html, Computed Ray Properties>
  */
  virtual OdResult getFirstDeriv( double param, OdGeVector3d& firstDeriv ) const;


  /** \details
    Computes the second derivative for the specified parameter of the ray entity and returns 
    the three-dimensional vector instance. The second derivative has (0,0,0) coordinates for 
    any point of the ray.

    \param param [in]  Parameter value specifying the point on the ray as a Double value. 
    \param secondDeriv [out]  Reference to a variable in which this method must save the second derivative  
                    as three-dimensional vector instance.

    \sa
    <link db_ray_sample.html, Example of Working with the Ray Object>

    \sa
    <link db_ray_compute.html, Computed Ray Properties>
  */
  virtual OdResult getSecondDeriv( double param, OdGeVector3d& secondDeriv ) const;


  /** \details
    This method is not applicable for the ray entity.
  */
  virtual OdResult getArea( double& area ) const;

  TD_USING(OdDbCurve::getFirstDeriv);
  TD_USING(OdDbCurve::getSecondDeriv);

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  virtual OdResult getOdGeCurve(OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol);
};

/** \details
  The typified smart pointer for the ray object (Ray entity). This template class 
  is specialization of the OdSmartPtr class for the OdDbRay class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbRay> OdDbRayPtr;

#include "TD_PackPop.h"

#endif

