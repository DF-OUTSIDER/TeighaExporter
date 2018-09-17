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




#ifndef _OD_DB_POINT_
#define _OD_DB_POINT_

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DwgDeclareMembers.h"

/** \details
  <group OdDb_Classes>

  This class implements the point object (Point entity) in the database. 
  This class inherits the base functionality of entities. 
  
  \sa
  TD_Db

  \sa
  <link db_point_sample.html, Example of Working with the Point Object>

  \sa
  <link db_point.html, Working with Points>

  <link db_point_appearance.html, Appearance and Size of Points>
*/
class DBDIM_EXPORT OdDbPoint : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbPoint);

  /** \details
    Builds an instance of the point entity.
    
    \param gePoint [in]  WCS position for the point entity.

    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_point_overview.html, Overview of Points>
  */
  OdDbPoint();
  OdDbPoint( const OdGePoint3d& gePoint );


  /** \details
    Returns the WCS position of the point entity (DXF 10) as three-dimensional point
    instance.

    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point_specific.html, Specific Point Properties>

    OdDbPoint::setPosition() method
  */
  OdGePoint3d position() const;


  /** \details
    Sets the WCS position of the point entity (DXF 10). The initial value is (0,0,0)
    by default.

    \param gePoint [in]  Three-dimensional point instance to set the point position.

    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point_specific.html, Specific Point Properties>

    OdDbPoint::position() method
  */
  void setPosition( const OdGePoint3d& gePoint );


  /** \details
    Returns the thickness of the point entity (DXF 39) in drawing units. Thickness is 
    the extrusion length along the normal. A positive value defines the thickness to 
    be drawn along the normal direction. A negative value defines the thickness to be 
    drawn in the opposite direction from the normal. A zero value defines a trace without 
    thickness.
    
    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point_specific.html, Specific Point Properties>

    OdDbPoint::setThickness() method
  */
  double thickness() const;


  /** \details
    Sets the thickness of the point entity (DXF 39) in drawing units. Thickness is the 
    extrusion length along the normal. A positive value extrudes along the normal. 
    A negative value extrudes opposite from the normal. A zero value defines a trace  
    without thickness. The initial value is zero by default.

    \param vThickness [in]  Thickness value as a Double value.
    
    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point_specific.html, Specific Point Properties>

    OdDbPoint::thickness() method
  */
  void setThickness( double vThickness );


  /** \details
    Returns the WCS unit vector which is the normal to the plane of the trace entity (DXF 210).
    The normal defines the orientation of the trace plane in world space.

    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point_specific.html, Specific Point Properties>

    OdDbPoint::setNormal() method
  */
  OdGeVector3d normal() const;


  /** \details
    Sets the WCS normal to the plane of the trace entity (DXF 210). The normal defines 
    the orientation of the trace plane in world space. The initial value is (0,0,1) by default.

    \param geVector [in]  Three-dimensional vector instance to set the normal.

    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point_specific.html, Specific Point Properties>

    OdDbPoint::normal() method
  */
  void setNormal( const OdGeVector3d& geVector );


  /** \details
    Returns the ECS Rotation which defines the angle in radians between the OCS X-axis for  
    the point based on the normal vector and the X-axis that will be used for displaying the  
    point entity (DXF 50). A positive angle value is measured clockwise. A negative angle 
    value is measured counterclockwise.

    \remarks
    The ECS Rotation is used when PDMODE is non-zero.

    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point_specific.html, Specific Point Properties>

    OdDbPoint::setEcsRotation() method
  */
  double ecsRotation() const;


  /** \details
    Sets the ECS Rotation which defines the angle in the range 0 to 2PI radians between the  
    OCS X-axis for the point based on the normal vector and the X-axis that will be used for  
    displaying the point entity (DXF 50). A positive angle rotates clockwise. A negative angle  
    rotates counterclockwise. The initial value is zero by default.
    
    \param vAngle [in]  Rotation angle as a Double value.
    
    \remarks
    The ECS Rotation is used when PDMODE is non-zero. This method does not convert to an  
    equivalent angle and sets the specified angle value.

    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point_specific.html, Specific Point Properties>

    OdDbPoint::ecsRotation() method
  */
  void setEcsRotation( double vAngle );


  /** \details
    Determines whether the point entity is planar as a Boolean value and always returns True.

    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point.html, Working with Points>

    OdDbPoint::getPlane(), OdDbEntity::isPlanar() method
  */
  virtual bool isPlanar() const { return true; }


  /** \details
    Gets an arbitrary plane passed through the point entity. This method is inherited from the 
    OdDbEntity class.

    \param gePlane [out]  Reference to a variable in which this method must save the plane parameters as 
                a plane instance.
    \param gePlanarity [out] Reference to a variable in which this method must save the plane type 
                    (returns kLinear - arbitrary plane).

    \sa
    <link db_point_sample.html, Example of Working with the Point Object>

    \sa
    <link db_point.html, Working with Points>

    OdDbPoint::isPlanar(), OdDbEntity::getPlane() methods
  */
  virtual OdResult getPlane( OdGePlane& gePlane, OdDb::Planarity& gePlanarity ) const;


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

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const;

  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits *pTraits) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;
};

/** \details
  The typified smart pointer for the point object (Point entity). This template class 
  is specialization of the OdSmartPtr class for the OdDbPoint class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbPoint> OdDbPointPtr;

#include "TD_PackPop.h"

#endif

