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




#ifndef _OD_DB_LINE_
#define _OD_DB_LINE_

#include "TD_PackPush.h"

#include "DbCurve.h"
#include "DwgDeclareMembers.h"

/** \details
  <group OdDb_Classes>

  This class implements the line object (Line entity) in the database. This class 
  inherits the base functionality of entities. A line is a straight segment in world space.

  \sa
  TD_Db

  \sa
  <link db_line_sample.html, Example of Working with the Line Object>

  \sa
  <link db_line.html, Working with Lines>

  OdDbXline, OdDbRay, OdDbMline classes
*/
class DBENT_EXPORT OdDbLine : public OdDbCurve
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbLine);

  /** \details
    Builds an instance of the line object.
    
    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_line_overview.html, Overview of Lines>
  */
  OdDbLine();


  /** \details
    Returns the WCS coordinates of the start point of the line (DXF 10) as 
    three-dimensional point instance.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_specific.html, Specific Line Properties>

    OdDbLine::setStartPoint(), OdDbLine::endPoint() method
  */
  virtual OdGePoint3d startPoint() const;


  /** \details
    Sets the start point of the line (DXF 10) in WCS. The initial value is (0,0,0) by default.
    
    \param geStartPoint [in]  Three-dimensional point instance to set the start point.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_specific.html, Specific Line Properties>

    OdDbLine::startPoint(), OdDbLine::setEndPoint() methods
  */
  void setStartPoint( const OdGePoint3d& geStartPoint );


  /** \details
    Returns the WCS coordinates of the end point of the line (DXF 11) as 
    three-dimensional point instance.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_specific.html, Specific Line Properties>

    OdDbLine::setEndPoint(), OdDbLine::startPoint() methods
  */
  virtual OdGePoint3d endPoint() const;


  /** \details
    Sets the end point of the line (DXF 11) in WCS. The initial value is (0,0,0) by default.
    
    \param geEndPoint [in]  Three-dimensional point instance to set the end point.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_specific.html, Specific Line Properties>

    OdDbLine::endPoint(), OdDbLine::setStartPoint() methods
  */
  void setEndPoint( const OdGePoint3d& geEndPoint );


  /** \details
    Returns the thickness of the line entity (DXF 39) in drawing units. Thickness is the extrusion 
    length along the normal. A positive value defines the thickness to be drawn along the normal 
    direction. A negative value defines the thickness to be drawn in the opposite direction from  
    the normal. A zero value defines a line without thickness.
    
    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_specific.html, Specific Line Properties>

    OdDbLine::setThickness() method
  */
  virtual double thickness() const;


  /** \details
    Sets the thickness of the line entity (DXF 39) in drawing units. Thickness is the 
    extrusion length along the normal. A positive value extrudes along the normal. 
    A negative value extrudes opposite from the normal. A zero value defines a line  
    without thickness. The initial value is zero by default.

    \param vThickness [in]  Thickness as a Double value.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_specific.html, Specific Line Properties>

    OdDbLine::thickness() method
  */
  void setThickness( double vThickness );


  /** \details
    Returns the WCS unit vector which is the normal to the plane of the line entity (DXF 210).
    The normal defines the orientation of the line plane in world space.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_specific.html, Specific Line Properties>

    OdDbLine::setNormal() method
  */
  virtual OdGeVector3d normal() const;


  /** \details
    Sets the WCS normal to the plane of the line entity (DXF 210). The normal defines 
    the orientation of the line plane in world space. The initial value is (0,0,1) by default.

    \param geVector [in]  Three-dimensional vector instance to set the normal.

    \remarks
    This method automatically converts the specified vector to the unit vector.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_specific.html, Specific Line Properties>

    OdDbLine::normal() method
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

  virtual OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;
  
  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  virtual bool isClosed() const;

  virtual bool isPeriodic() const;


  /** \details
    Determines whether the line entity is planar as a Boolean value and returns True.
    This method is inherited from the OdDbEntity class.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line.html, Working with Lines>

    OdDbLine::getPlane(), OdDbEntity::isPlanar() methods
  */
  virtual bool isPlanar() const;


  /** \details
    Gets an arbitrary plane passed through the line entity. This method is inherited from the  
    OdDbEntity class.

    \param gePlane [out]  Reference to a variable in which this method must save the plane parameters as 
                a plane instance.
    \param gePlanarity [out] Reference to a variable in which this method must save the plane type 
                    (returns kLinear - arbitrary plane).

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line.html, Working with Lines>

    OdDbLine::isPlanar(), OdDbEntity::getPlane() methods
  */
  virtual OdResult getPlane( OdGePlane& gePlane, OdDb::Planarity& gePlanarity ) const;


  /** \details
    Gets the start parameter value. The start parameter equals a zero for lines.
    Parameter is a distance measured from the start point in drawing units.

    \param startParam [out]  Reference to a variable in which this method must save the start    
                   parameter value as a Double value.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getStartParam( double& startParam ) const;


  /** \details
    Gets the end parameter value. The end parameter equals the line length.
    Parameter is a distance measured from the start point in drawing units.

    \param endParam [out]  Reference to a variable in which this method must save the end    
                 parameter value as a Double value.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getEndParam( double& endParam ) const;


  /** \details
    Computes the WCS coordinates of the start point on the line for which the parameter 
    equals zero.

    \param startPoint [out]  Reference to a variable in which this method must save the start 
                   point as three-dimensional point instance.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getStartPoint( OdGePoint3d& startPoint ) const;


  /** \details
    Computes the WCS coordinates of the end point on the line for which the parameter 
    equals the line length.

    \param endPoint [out]  Reference to a variable in which this method must save the end  
                 point as three-dimensional point instance.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getEndPoint( OdGePoint3d& endPoint ) const;


  /** \details
    Computes the WCS coordinates of the point on the line at the specified parameter value.
    Parameter is a distance measured from the start point in drawing units.
    
    \param param [in]  Parameter value specifying the point on the line as a Double value. 
    \param pointOnCurve [out]  Reference to a variable in which this method must save the 
                     three-dimensional point instance corresponding to the parameter.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getPointAtParam( double param, OdGePoint3d& pointOnCurve ) const;


  /** \details
    Computes the parameter at the point placed on the line. Parameter is a distance
    measured from the start point in drawing units.

    \param pointOnCurve [in]  Three-dimensional point instance which specifies the point on 
                     the line. 
    \param param [out] Reference to a variable in which this method must save the parameter   
              value as a Double value.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getParamAtPoint( const OdGePoint3d& pointOnCurve, double& param ) const;


  /** \details
    Computes the distance along the line measured from the start point in drawing units   
    at the specified parameter.

    \param param [in]  Parameter value specifying the point on the line as a Double value. 
    \param dist  [out] Reference to a variable in which this method must save the distance value 
              as a Double value.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getDistAtParam( double param, double& dist ) const;


  /** \details
    Computes the parameter at the distance along the line. Parameter is a distance 
    measured from the start point in drawing units.

    \param dist [in]   Distance value along the line as a Double value in drawing units. 
    \param param [out]  Reference to a variable in which this method must save the parameter value  
              as a Double value.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getParamAtDist( double dist, double& param ) const;


  /** \details
    Computes the first derivative for the specified parameter of the line entity and returns
    the three-dimensional vector instance. The first derivative is constant for any point of 
    the line.

    \param param [in]  Parameter value specifying the point on the line as a Double value. 
    \param firstDeriv [out]  Reference to a variable in which this method must save the first   
                   derivative as three-dimensional vector instance.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getFirstDeriv( double param, OdGeVector3d& firstDeriv ) const;


  /** \details
    Computes the second derivative for the specified parameter of the line entity and returns 
    the three-dimensional vector instance. The second derivative is constant for any point 
    of the line.

    \param param [in]  Parameter value specifying the point on the line as a Double value. 
    \param secondDeriv [out]  Reference to a variable in which this method must save the second  
                    derivative as three-dimensional vector instance.

    \sa
    <link db_line_sample.html, Example of Working with the Line Object>

    \sa
    <link db_line_compute.html, Computed Line Properties>
  */
  virtual OdResult getSecondDeriv( double param, OdGeVector3d& secondDeriv ) const;


  /** \details
    Returns a zero for the line entity.
  */
  virtual OdResult getArea( double& area ) const;


  TD_USING(OdDbCurve::getFirstDeriv);
  TD_USING(OdDbCurve::getSecondDeriv);

  /*
     void getOffsetCurvesGivenPlaneNormal(
       const OdGeVector3d& normal, double offsetDist,
       OdRxObjectPtrArray& offsetCurves) const;
  */

  OdGeMatrix3d getEcs( ) const;

  virtual OdResult getOdGeCurve(OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol);
  virtual OdResult reverseCurve();
};

/** \details
  The typified smart pointer for the line object (Line entity). This template class 
  is specialization of the OdSmartPtr class for the OdDbLine class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbLine> OdDbLinePtr;

#include "TD_PackPop.h"

#endif

