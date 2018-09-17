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




#ifndef _OD_DB_CIRCLE_
#define _OD_DB_CIRCLE_

#include "TD_PackPush.h"

#include "DbCurve.h"
#include "DwgDeclareMembers.h"

class OdDbCircleImpl;

/** \details
  <group OdDb_Classes>

  This class implements the circle object (Circle entity) in the database. This 
  class inherits the base functionality of entities. A circle is a circular closed 
  figure in world space.

  \sa
  TD_Db

  \sa
  <link db_circle_sample.html, Example of Working with the Circle Object>

  \sa
  <link db_circle.html, Working with Circles>

  OdDbArc, OdDbEllipse classes
*/
class DBENT_EXPORT OdDbCircle : public OdDbCurve
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbCircle);

  /** \details
    Builds an instance of the circle object.
    
    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_circle_overview.html, Overview of Circles>
  */
  OdDbCircle();


  /** \details
    Returns the WCS coordinates of the circle center (DXF 10) as three-dimensional 
    point instance.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_specific.html, Specific Circle Properties>

    OdDbCircle::setCenter() method
  */
  OdGePoint3d center() const;


  /** \details
    Sets the center of the circle entity (DXF 10) in WCS. The initial value is (0,0,0) 
    by default.
    
    \param center [in]  Three-dimensional point instance to set the center.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_specific.html, Specific Circle Properties>

    OdDbCircle::center() method
  */
  void setCenter( const OdGePoint3d& center );


  /** \details
    Returns the radius of the circle entity (DXF 40).

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_specific.html, Specific Circle Properties>

    OdDbCircle::setRadius() method
  */
  double radius() const;


  /** \details
    Sets the radius of the circle entity (DXF 40). The initial value is 0.0 by default.

    \param radius [in]  Radius as a Double value.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_specific.html, Specific Circle Properties>

    OdDbCircle::radius() method
  */
  void setRadius( double radius );


  /** \details
    Returns the thickness of the circle entity (DXF 39) in drawing units. Thickness is 
    the extrusion length along the normal. A positive value defines the thickness to 
    be drawn along the normal direction. A negative value defines the thickness to be 
    drawn in the opposite direction from the normal. A zero value defines a circle without 
    thickness.
    
    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_specific.html, Specific Circle Properties>

    OdDbCircle::setThickness() method
  */
  double thickness() const;


  /** \details
    Sets the thickness of the circle entity (DXF 39) in drawing units. Thickness is the 
    extrusion length along the normal. A positive value extrudes along the normal. 
    A negative value extrudes opposite from the normal. A zero value defines a circle  
    without thickness. The initial value is zero by default.

    \param vThickness [in]  Thickness as a Double value.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_specific.html, Specific Circle Properties>

    OdDbCircle::thickness() method
  */
  void setThickness( double vThickness );


  /** \details
    Returns the WCS unit vector which is the normal to the plane of the circle entity 
    (DXF 210). The normal defines the orientation of the circle plane in world space.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_specific.html, Specific Circle Properties>

    OdDbCircle::setNormal() method
  */
  OdGeVector3d normal() const;


  /** \details
    Sets the WCS normal to the plane of the circle entity (DXF 210). The normal defines 
    the orientation of the circle plane in world space. The initial value is (0,0,1) by default.

    \param geVector [in]  Three-dimensional vector instance to set the normal.

    \remarks
    This method automatically converts the specified vector to the unit vector.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_specific.html, Specific Circle Properties>

    OdDbCircle::normal() method
  */
  void setNormal( const OdGeVector3d& geVector );


  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& xfm, 
    OdDbEntityPtr& pCopy) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual bool isClosed() const;

  virtual bool isPeriodic() const;


  /** \details
    Determines whether the circle entity is planar as a Boolean value and returns True.
    This method is inherited from the OdDbEntity class.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle.html, Working with Circles>

    OdDbCircle::getPlane(), OdDbEntity::isPlanar() methods
  */
  virtual bool isPlanar() const;


  /** \details
    Gets a plane in which the circle entity places. This method is inherited from the  
    OdDbEntity class.

    \param gePlane [out]  Reference to a variable in which this method must save the plane parameters as 
                a plane instance.
    \param gePlanarity [out] Reference to a variable in which this method must save the plane type
                    (returns kPlanar - planar entity).

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle.html, Working with Circles>

    OdDbCircle::isPlanar(), OdDbEntity::getPlane() methods
  */
  virtual OdResult getPlane( OdGePlane& gePlane, OdDb::Planarity& gePlanarity ) const;


  /** \details
    Gets the start parameter value. The start parameter equals zero for circles.
    Parameter is an angle measured counterclockwise from the OCS X-axis in the 
    range 0 to 2PI radians.

    \param startParam [out]  Reference to a variable in which this method must save the start    
                   parameter value as a Double value in radians.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getStartParam( double& startParam ) const;


  /** \details
    Gets the end parameter value. The end parameter equals the 2PI. Parameter is an 
    angle measured counterclockwise from the OCS X-axis in the range 0 to 2PI radians.

    \param endParam [out]  Reference to a variable in which this method must save the end    
                 parameter value as a Double value in radians.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getEndParam( double& endParam ) const;


  /** \details
    Computes the WCS coordinates of the start point on the circle for which the parameter 
    equals zero. The start point coincides with the end point.

    \param startPoint [out]  Reference to a variable in which this method must save the start 
                   point as three-dimensional point instance.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getStartPoint( OdGePoint3d& startPoint ) const;


  /** \details
    Computes the WCS coordinates of the end point on the circle for which the parameter 
    equals the circumference. The end point coincides with the start point.

    \param endPoint [out]  Reference to a variable in which this method must save the end  
                 point as three-dimensional point instance.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getEndPoint( OdGePoint3d& endPoint ) const;


  /** \details
    Computes the WCS coordinates of the point on the circle at the specified parameter 
    value. Parameter is an angle measured counterclockwise from the OCS X-axis in the 
    range 0 to 2PI radians.

    \param param [in]  Parameter value specifying the point on the circle as a Double value 
              in radians. 
    \param pointOnCurve [out] Reference to a variable in which this method must save the 
                     three-dimensional point instance corresponding to the parameter.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getPointAtParam( double param, OdGePoint3d& pointOnCurve ) const;


  /** \details
    Computes the parameter at the point placed on the circle. Parameter is an angle
    measured counterclockwise from the OCS X-axis in the range 0 to 2PI radians.

    \param pointOnCurve [in]  Three-dimensional point instance which specifies the point on 
                     the circle. 
    \param param [out] Reference to a variable in which this method must save the parameter   
              value as a Double value in radians.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getParamAtPoint( const OdGePoint3d& pointOnCurve, double& param ) const;


  /** \details
    Computes the distance along the circle measured from the OCS X-axis to the point on 
    the circle specified the parameter.

    \param param [in]  Parameter value specifying the point on the circle as a Double value 
              in radians. 
    \param dist  [out] Reference to a variable in which this method must save the distance value 
              as a Double value in drawing units.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getDistAtParam( double param, double& dist ) const;


  /** \details
    Computes the parameter at the distance along the circle. Parameter is an angle
    measured counterclockwise from the OCS X-axis in the range 0 to 2PI radians. 

    \param dist [in]   Distance value along the circle curve as a Double value in drawing units. 
    \param param [out]  Reference to a variable in which this method must save the parameter value  
              as a Double value in radians.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getParamAtDist( double dist, double& param ) const;


  /** \details
    Computes the first derivative for the specified parameter of the circle entity and 
    returns the three-dimensional vector instance.

    \param param [in]  Parameter value specifying the point on the circle as a Double value 
              in radians. 
    \param firstDeriv [out] Reference to a variable in which this method must save the first   
                   derivative as three-dimensional vector instance.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getFirstDeriv( double param, OdGeVector3d& firstDeriv ) const;


  /** \details
    Computes the second derivative for the specified parameter of the circle entity and  
    returns the three-dimensional vector instance.

    \param param [in]  Parameter value specifying the point on the circle as a Double value
              in radians. 
    \param secondDeriv [out] Reference to a variable in which this method must save the second   
                    derivative as three-dimensional vector instance.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getSecondDeriv( double param, OdGeVector3d& secondDeriv ) const;


  /** \details
    Returns the area of the circle calculated using the classic formula (PIRR).

    \param vArea [out]  Reference to a variable in which this method must save the area
              as a Double value.

    \sa
    <link db_circle_sample.html, Example of Working with the Circle Object>

    \sa
    <link db_circle_compute.html, Computed Circle Properties>
  */
  virtual OdResult getArea( double& vArea ) const;

  /** \details
  Returns the entity coordinate system matrix.
  */
  OdGeMatrix3d getEcs( ) const;
  

  TD_USING(OdDbCurve::getFirstDeriv);
  TD_USING(OdDbCurve::getSecondDeriv);

  virtual OdResult getOdGeCurve(OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol);
};

/** \details
  The typified smart pointer for the circle object (Circle entity). This template class 
  is specialization of the OdSmartPtr class for the OdDbCircle class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbCircle> OdDbCirclePtr;

#include "TD_PackPop.h"

#endif
