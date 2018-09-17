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




#ifndef GEOMENT_DBREGION_H
#define GEOMENT_DBREGION_H

#include "TD_PackPush.h"
#include "ModelerDefs.h"
#include "DbEntity.h"
#include "DbSubentId.h"
#include "DwgDeclareMembers.h"

class OdBrBrep;
class OdModelerGeometry;
namespace GeMesh { class OdGeTrMesh; }
class wrTriangulationParams;

/** \details
    This class represents Region entities in an OdDbDatabase instance.

    An OdDbRegion entity is a wrapper for an ACIS model that represents
    the geometry of the OdDbRegion entity.

    \sa
    TD_Db
    
   <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbRegion : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbRegion);

  OdDbRegion();

  /** \details
  Writes the ACIS data of this entity to the specified StreamBuf object.

  \param pStreamBuf [in] Pointer to the StreamBuf object to which the data is to be written.   
  \param typeVer    [in] Type and version of the ACIS data to write.
  
  \returns Returns eOk if successful or an appropriate error code if not. 
  */
  OdResult acisOut(
    OdStreamBuf* pStreamBuf, 
    AfTypeVer typeVer = kAfTypeVerAny);

  /** \details
  Reads the ACIS data for this entity from the specified StreamBuf object.

  \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data is to be read.   
  \param typeVer    [out] Receives the type and version of the ACIS data to return.

  \returns eOk if successful or an appropriate error code in the other case.
  
  \remarks 
  If typeVer is null, this function uses the type and version 
  of this Region entity.
  */
  OdResult acisIn(
    OdStreamBuf* pStreamBuf, 
    AfTypeVer *typeVer = 0);

  /** \details
  Returns the boundary representation of the 3D solid entity.
  
  \param brep [out]  Receives the boundary representation.
  */
  void brep(
    OdBrBrep& brep);

  bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) const;

  /** \details
  Returns true if and only if there is no ACIS model associated with this entity. 
  */
  bool isNull() const;

  /** \details
  Creates OdDbRegion entities from the closed loops defined by the specified curve segments.
  
  \param curveSegments  [in]  Array of curve segments. Each curve segment must be one of the following: OdDb3dPolyline, OdDbArc, OdDbCircle, OdDbEllipse, OdDbLine, OdDbPolyline, or OdDbSpline.
  \param regions        [out] Receives an array of pointers to the regions.

  \returns Returns eOk if successful, or an appropriate error code if not. 

  \remarks 
  The newly created regions are non-database residents. It is up to the caller to either add them
  to an OdDbDatabase or to delete them.
  */
  static OdResult createFromCurves( 
    const OdRxObjectPtrArray& curveSegments,
    OdRxObjectPtrArray& regions );

  /** \details
  Returns the WCS normal to the plane of this entity (DXF 210).

  \param normal [out]  Receives the normal.

  \returns Returns eOk if successful or an appropriate error code if not. 
  */
  virtual OdResult getNormal(
  OdGeVector3d& normal) const;

  /** \details
  Returns true if the region is planar or false in the other case.
  */
  virtual bool isPlanar() const;

  /** \details
  Returns the OdGePlane object that contains the region. If the region is Null, then the WCS X-Y plane is returned. 

  \param plane      [out] A reference to the OdGePlane object that stores the containing plane.
  \param planarity  [out] A reference to a variable containing information about planarity of the OdGePlane object.

  \returns Returns eOk if successful or an appropriate error code in other case.

  \remarks
  If typeVer is null, this function uses the type and version 
  of this region entity.
  */
  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;

  TD_USING(OdDbEntity::getPlane);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual OdDbObjectPtr decomposeForSave(
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData);

  /** \details
  Directly sets the contained modeler geometry of this entity.
  
  \param pGeometry [in]  Pointer to the modeler geometry.
  
  \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult setBody( const void* pGeometry );
  
  /** \details 
  Returns the current modeler geometry. 
  */
  virtual void *body( ) const;

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;
  
  virtual OdResult subTransformBy( 
    const OdGeMatrix3d& xfm );

  /** \details
  Creates a geometric representation of the region for saving proxy graphics or converting to previous formats.

  \param pWd [in] Pointer to the OdGiWorldDraw object.
  \param ver [in] Specifies the reason why the method is called.

  \remarks
  This method can be overridden in a custom classes.
  */
	  virtual void saveAs(
    OdGiWorldDraw* pWd,
    OdDb::DwgVersion ver ) const;

  /** \details
  Performs a Boolean operation between this and another region object. 
  
  Possible operation types are: 
* kBoolUnite - unites two regions into one.
* kBoolIntersect - returns the intersection of two regions.
* kBoolSubtract - subtracts the region from the first one.

  \param operation  [in] Type of Boolean operation. 	
  \param otherRegion      [out] Pointer to other region object which also will contain the result of the Boolean operation.

  \returns Returns eOk if the Boolean operation is successful or an appropriate error code if not. If the region is null, eInvalidInput is returned.

  \remarks
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  virtual OdResult booleanOper(OdDb::BoolOperType operation, OdDbRegion* otherRegion);

  /** \details
  Calculates the area of the region.
  Area value is returned in square drawing units.
  
  \param area [out] Reference for returning the area value.

  \returns Returns eOk if successful or an appropriate error code otherwise.
  
  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  virtual OdResult getArea(double& regionArea) const;

  /** \details
  Calculates the perimeter of the region.
  Perimeter value is returned in drawing units.
  
  \param area [out] Reference for returning the perimeter value.

  \returns Returns eOk if successful, or an appropriate error code otherwise.

  \remarks 
  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
    virtual OdResult getPerimeter(double&) const;

  /** \details
  Calculates the area properties of the region. 
  
  The method validates the origin, xAxis, and yAxis parameters to ensure that:
* axes are perpendicular to each other.
* axes and the origin lie in the same plane as the region.

  \param origin        [in]  Origin of the coordinate system to use for evaluation (WCS). 
  \param xAxis         [in]  X axis of the coordinate system to use for evaluation (WCS).
  \param yAxis         [in]  Y axis of the coordinate system to use for evaluation (WCS).
  \param perimeter     [out] Reference for returning the value of region's perimeter.
  \param area          [out] Reference for returning the value of region's area.
  \param centroid      [out] Reference for returning centroid of the region.
  \param momInertia    [out] Static array for returning moments of inertia for the region.
  \param prodInertia   [out] Reference for returning region's product of inertia.
  \param prinMoments   [out] Static array for returning principal moments for the region. 
  \param prinAxes      [out] Static array for returning principle axes of the region. 
  \param radiiGyration [out] Static array for returning radii of gyration of the region. 
  \param extentsLow    [out] Reference for returning region's minimum extents point. 
  \param extentsHigh   [out] Reference for returning region's maximum extents point.

  \returns Returns eOk if successful, or an appropriate error code otherwise.

  \remarks 
  Returned values are measured using the values of origin, xAxis, yAxis parameters. 
  and are represented in WCS coordinates.

  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  virtual OdResult getAreaProp( const OdGePoint3d& origin, const OdGeVector3d& xAxis,
    const OdGeVector3d& yAxis, double& perimeter,
    double& area, OdGePoint2d& centroid,
    double momInertia[2], double& prodInertia,
    double prinMoments[2], OdGeVector2d prinAxes[2],
    double radiiGyration[2], OdGePoint2d& extentsLow,
    OdGePoint2d& extentsHigh ) const;

  /** \details
	Finds the intersections of the specified entity with all the edges of the region's bounding box. 
	

  \param pEnt 		      [in]  Pointer to the entity which must be intersected with the region.
  \param intType        [in]  Intersection type.
  \param points 		    [out] Reference to the array of the appended points of intersection (point coordinates are represented in WCS).
  \param thisGsMarker   [in]  GS marker of the subentity of the region involved in the intersection operation.
  \param otherGsMarker  [in]  GS marker of the subentity of the entity represented by pEnt that is involved in the intersection operation.	
    

  \returns Returns eOk if successful or an appropriate error code in other case.

  \remarks 
	The intType parameter determines the extension of the region and the specified entity while calculating intersections:
* OdDb::kOnBothOperands - do not extend the region or the entity represented with pEnt bounding box edges. 
In this case the method calculates only where the bounding box lines actually intersect with the specified entity.
* OdDb::kExtendThis - extend the region's bounding box edges (if necessary) when calculating intersections without extending the specified entity. 
* OdDb::kExtendArg - extend the specified entity (if necessary) when calculating intersections without extending the region's bounding box edges.
* OdDb::kExtendBoth - extend both the region's bounding box and the specified entity (if necessary) when calculating intersections.
	
	Any found intersection points are appended to the points array. All points are represented in WCS coordinates. 

  The thisGsMarker and otherGsMarker parameters provide information for searching intersections between specified subentities. 
	The default value for these parameters is zero.
	
	This method is implemented only for Spatial modeler and returns eNotImplementedYet status 
	for other modelers. 
  */
  virtual OdResult subIntersectWith(const OdDbEntity* pEnt, OdDb::Intersect intType,
    OdGePoint3dArray& points, OdGsMarker thisGsMarker = 0,
    OdGsMarker otherGsMarker = 0) const;

  /** \details
  Projects the bounding box edges of the region and the specified entity onto the specified plane, finds the intersection points for the projections, 
  and then projects them back onto the bounding box edge of the region.   
   

  \param pEnt 			    [in]  Pointer to the entity which must be intersected with the region.
  \param intType 		    [in]  Intersection type.
  \param projPlane 	    [in]  Projection plane for the apparent intersection of the specified entity and the region.
  \param points 		    [out] Reference to the array of the appended points of intersection (points coordinates are represented in WCS).
  \param thisGsMarker   [in]  GS marker of the subentity of the region involved in the intersection operation.
  \param otherGsMarker  [in]  GS marker of the subentity of the entity represented by pEnt that is involved in the intersection operation.
  
  \returns Returns eOk if successful or an appropriate error code otherwise.
  
  \remarks 
  All the points appended to the points array will be on its bounding box edges. The projections are parallel to the normal vector of the plane referenced with projPlane. 
  Intersection type can be one of the following:
* OdDb::kOnBothOperands - do not extend the region or the entity represented with pEnt bounding box edges. 
  In this case the method calculates only where the bounding box lines actually intersect with the specified entity.
* OdDb::kExtendThis - extend the region's bounding box edges (if necessary) when calculating intersections without extending the specified entity.  
* OdDb::kExtendArg - extend the specified entity (if necessary) when calculating intersections without extending region's bounding box edges.
* OdDb::kExtendBoth  extend both the region's bounding box and the specified entity (if necessary) when calculating intersections.

  This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  virtual OdResult subIntersectWith(const OdDbEntity* pEnt, OdDb::Intersect intType,
    const OdGePlane& projPlane, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker = 0, OdGsMarker otherGsMarker = 0) const;

  virtual OdDbEntityPtr subSubentPtr(const OdDbFullSubentPath& id) const;

  virtual OdResult subGetGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath, OdGsMarkerArray& gsMarkers) const;

  virtual OdResult subGetSubentPathsAtGsMarker(OdDb::SubentType type,
                                               OdGsMarker gsMark, 
                                               const OdGePoint3d& pickPoint,
                                               const OdGeMatrix3d& viewXform, 
                                               OdDbFullSubentPathArray& subentPaths, 
                                               const OdDbObjectIdArray* pEntAndInsertStack = 0) const;

  /** \details
  Returns the number of changes occurred since the region was created.
  */
    virtual OdUInt32 numChanges() const;

  virtual OdDbSubentId internalSubentId(void* ent) const;
  virtual void* internalSubentPtr(const OdDbSubentId& id) const;

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual OdResult subGetGeomExtents( OdGeExtents3d& extents) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDbRegion object pointers.
*/
typedef OdSmartPtr<OdDbRegion> OdDbRegionPtr;

#include "TD_PackPop.h"

#endif

