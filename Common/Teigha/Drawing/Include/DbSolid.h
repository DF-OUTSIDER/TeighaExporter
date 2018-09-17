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




#ifndef _OD_DB_SOLID_
#define _OD_DB_SOLID_

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DwgDeclareMembers.h"

/** \details
  <group OdDb_Classes>

  This class implements the two-dimensional /solid-filled/ object (Planar Solid entity) in the database. 
  This class inherits the base functionality of entities. 
  
  \sa
  TD_Db

  \sa
  <link db_solid2d_sample.html, Example of Working with the Planar Solid Object>

  \sa
  <link db_solid2d.html, Working with Planar Solids>

  OdDbTrace, OdDbEntity classes
*/
class DBENT_EXPORT OdDbSolid : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbSolid);

  /** \details
    Builds an instance of the /2D solid-filled/ entity.
    
    \remarks
    Use the static pseudo-constructor instead it.
    See: <link db_solid2d_overview.html, Overview of Planar Solids>
  */
  OdDbSolid();

  /*  OdDbSolid(const OdGePoint3d& pt0,
        const OdGePoint3d& pt1,
        const OdGePoint3d& pt2,
        const OdGePoint3d& pt3);
      OdDbSolid(const OdGePoint3d& pt0,
        const OdGePoint3d& pt1,
        const OdGePoint3d& pt2);
  */

  /** \details
    Gets the WCS coordinates of the vertex belonged to the planar solid entity using the 
    specified index (DXF 10, 11, 12, or 13).

    \param pointIndex [in]  Vertex index 0, 1, 2, or 3.
    \param pointValue [out]  Reference to a variable in which this method must save the solid vertex as
                   three-dimensional point instance.

    \sa
    <link db_solid2d_sample.html, Example of Working with the Planar Solid Object>

    \sa
    <link db_solid2d_specific.html, Specific Planar Solid Properties>

    OdDbSolid::setPointAt() method
  */
  void getPointAt( int pointIndex, OdGePoint3d& pointValue ) const;

  
  /** \details
    Sets the WCS coordinates of the vertex belonged to the planar solid entity using the 
    specified index (DXF 10, 11, 12, or 13).

    \param pointIndex [in]  Vertex index 0, 1, 2, or 3.
    \param pointValue [in]  Three-dimensional point instance to set the vertex position.

    \sa
    <link db_solid2d_sample.html, Example of Working with the Planar Solid Object>

    \sa
    <link db_solid2d_specific.html, Specific Planar Solid Properties>

    OdDbSolid::getPointAt() method
  */
  void setPointAt( int pointIndex, const OdGePoint3d& pointValue );


  /** \details
    Returns the thickness of the planar solid entity (DXF 39) in drawing units. Thickness 
    is the extrusion length along the normal. A positive value defines the thickness to 
    be drawn along the normal direction. A negative value defines the thickness to be drawn 
    in the opposite direction from the normal. A zero value defines a planar solid without 
    thickness.

    \sa
    <link db_solid2d_sample.html, Example of Working with the Planar Solid Object>

    \sa
    <link db_solid2d_specific.html, Specific Planar Solid Properties>

    OdDbSolid::setThickness() method
  */
  double thickness() const;


  /** \details
    Sets the thickness of the planar solid entity (DXF 39) in drawing units. Thickness is 
    the extrusion length along the normal. A positive value extrudes along the normal. 
    A negative value extrudes opposite from the normal. A zero value defines a planar solid  
    without thickness. The initial value is zero by default.

    \param vThickness [in]  Thickness as a Double value.

    \sa
    <link db_solid2d_sample.html, Example of Working with the Planar Solid Object>

    \sa
    <link db_solid2d_specific.html, Specific Planar Solid Properties>

    OdDbSolid::thickness() method
  */
  void setThickness( double vThickness );


  /** \details
    Returns the WCS unit vector which is the normal to the plane of the planar solid entity 
    (DXF 210). The normal defines the orientation of the solid plane in world space.

    \sa
    <link db_solid2d_sample.html, Example of Working with the Planar Solid Object>

    \sa
    <link db_solid2d_specific.html, Specific Planar Solid Properties>

    OdDbSolid::setNormal() method
  */
  OdGeVector3d normal() const;

  
  /** \details
    Sets the WCS normal to the plane of the planar solid entity (DXF 210). The normal defines 
    the orientation of the solid plane in world space. The initial value is (0,0,1) by default.

    \param geVector [in]  Three-dimensional vector instance to set the normal.

    \remarks
    This method automatically converts the specified vector to the unit vector.

    \sa
    <link db_solid2d_sample.html, Example of Working with the Planar Solid Object>

    \sa
    <link db_solid2d_specific.html, Specific Planar Solid Properties>

    OdDbSolid::normal() method
  */
  void setNormal( const OdGeVector3d& geVector );


  /** \details
    Gets the solid plane. This method is inherited from the OdDbEntity class.

    \param gePlane [out]  Reference to a variable in which this method must save the solid plane
                parameters as a plane instance.
    \param gePlanarity [out] Reference to a variable in which this method must save the plane type
                    kNonPlanar - non-planar, kPlanar - planar entity, kLinear - arbitrary plane.

    \sa
    <link db_solid2d_sample.html, Example of Working with the Planar Solid Object>

    \sa
    <link db_solid2d.html, Working with Planar Solids>
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

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const;

  virtual OdResult subTransformBy(const OdGeMatrix3d& xfm);

  OdResult subGetSubentPathsAtGsMarker( OdDb::SubentType type,
                                        OdGsMarker gsMark, 
                                        const OdGePoint3d& ,
                                        const OdGeMatrix3d& , 
                                        OdDbFullSubentPathArray& subentPaths, 
                                        const OdDbObjectIdArray* pEntAndInsertStack ) const;

  OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
                                        OdGsMarkerArray& gsMarkers) const;

  OdGeMatrix3d getEcs( ) const;
};

/** \details
  The typified smart pointer for the two-dimensional /solid-filled/ object (Planar Solid entity).  
  This template class is specialization of the OdSmartPtr class for the OdDbSolid class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSolid> OdDbSolidPtr;

#include "TD_PackPop.h"

#endif
