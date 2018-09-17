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




#ifndef GEOMENT_DBSOL3D_H
#define GEOMENT_DBSOL3D_H 

#include "TD_PackPush.h"
#include "ModelerDefs.h"
#include "DbRegion.h"
#include "DbSubentId.h"

class OdDbSubentId;
class OdDbCurve;
class OdBrBrep;
class OdModelerGeometry;
class OdDbLoftOptions;
class OdDbSweepOptions;
class OdDbRevolveOptions;
class OdDbSurface;
namespace GeMesh { class OdGeTrMesh; }
class wrTriangulationParams;

#include "DbEntity.h"
#include "OdArray.h"
#include "DwgDeclareMembers.h"



class OdDb3dSolid;

/** \details
This template class is a specialization of the OdSmartPtr class for OdDb3dSolid object pointers.
*/
typedef OdSmartPtr<OdDb3dSolid> OdDb3dSolidPtr;

/** \details
This template class is a specialization of the OdArray class for OdDb3dSolid object pointer arrays.
*/
typedef OdArray<OdDb3dSolidPtr> OdDb3dSolidPtrArray;


/** \details
    This class represents 3D Solid entities in an OdDbDatabase instance.

    \sa
    TD_Db
    
    \remarks
    An OdDb3dSolid entity is a wrapper for an ACIS model that represents
    the geometry of the OdDb3dSolid entity.
    
    <group OdDb_Classes> 

*/
class DBENT_EXPORT OdDb3dSolid : public OdDbEntity
{
public:

  DWGMAP_DECLARE_MEMBERS(OdDb3dSolid);
  
  OdDb3dSolid();
  
  /** \details
    Returns true if and only if there is no ACIS model associated with this entity. 
  */
  bool isNull() const;

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

    \param pStreamBuf [in] Pointer to the StreamBuf object from which the data is to be read.    
    \param pTypeVer   [in] Receives the type and version of the ACIS data read.
    
    \returns Returns eOk if successful or an appropriate error code if not. 
  */
  OdResult acisIn(
    OdStreamBuf* pStreamBuf, 
    AfTypeVer *pTypeVer = 0);

  /** \details
    Returns the boundary representation of the 3D solid entity.
    \param brep [out] Receives the boundary representation.
  */
  void brep(
    OdBrBrep& brep);

  bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const;

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

  /** \details
    Creates a WCS aligned solid box centered about the world origin. 
    
    \param xLen [in] Dimension along the WCS X-axis.
    \param yLen [in] Dimension along the WCS Y-axis. 
    \param zLen [in] Dimension along the WCS Z-axis. 

    \remarks
    The following constraints are applied:
    
* xLen >= 1e-6.
* yLen >= 1e-6.
* zLen >= 1e-6.

  */
  virtual void createBox(
    double xLen, 
    double yLen, 
    double zLen);
    
  /** \details
    Creates a WCS aligned frustum centered about the world origin. 
    
    \param height     [in] Dimension along the WCS Z-axis.
    \param xRadius    [in] Base radius along the WCS X-axis. 
    \param yRadius    [in] Base radius along the WCS Y-axis.
    \param topXRadius [in] Top radius along the WCS X-axis. 

    \remarks 
    The function can generate a frustum that is any one of the following:
    
* A circular cylinder (when xRadius is equal to topXRadius and xRadius and yRadius are the same)
* An elliptical cylinder (when xRadius is equal to topXRadius and xRadius and yRadius are not the same)
* A circular cone (when topXRadius is zero and xRadius and yRadius are the same)
* An elliptical cone (when topXRadius is zero and xRadius and yRadius are not the same)
* A truncated circular cone (when topXRadius is not zero but not equal to xRadius and xRadius and yRadius are the same)
* A truncated elliptical cone (when topXRadius is not zero  but not equal to xRadius and xRadius and yRadius are not the same)

    The following constraints are applied:
    
* height >= 1e-6.
* xRadius >= 1e-6.
* yRadius >= 1e-6.
* topXRadius >= 1e-6. 
  */
  virtual void createFrustum(
    double height, 
    double xRadius,
    double yRadius, 
    double topXRadius);
    
  /** \details
    Creates a sphere centered about the world origin with the specified radius. 
    
    \param radius [in] Radius for the sphere.

    \remarks
    The following constraints are applied:
    
* radius >= 1e-6.

  */
  virtual void createSphere(
    double radius);

  /** \details
    Creates a WCS aligned torus centered about the world origin
    and the Z-axis as its axis of rotation. 
    
    \param majorRadius [in] Radius of the torus
    \param minorRadius [in] Radius of the tube.

    \remarks
	majorRadius in contrast to minorRadius can't be equal to zero. 
    If majorRadius is smaller than zero, minorRadius must be larger than the absolute value of majorRadius. 
	
    The following constraints are applied:
    
* minorRadius >= 1e-6.
* |majorRadius| >= 1e-6
* if majorRadius < 0, then minorRadius > |majorRadius| + 1e-6
  */
  virtual void createTorus(
    double majorRadius, 
    double minorRadius);
    
  /** \details
    Creates a WCS aligned wedge centered about the world origin. 
    
    \param xLen [in] Dimension along the WCS X-axis.
    \param yLen [in] Dimension along the WCS Y-axis. 
    \param zLen [in] Dimension along the WCS Z-axis. 

    \returns Returns eOk if successful or an appropriate error code if not. 
    
    \remarks
    The inclined face is defined by the points 
    
              ( xLen/2, -yLen/2, -zLen/2)
              (-xLen/2, -yLen/2,  zLen/2)  
              ( xLen/2,  yLen/2, -zLen/2)
              (-xLen/2, +yLen/2,  zLen/2)  
    
    The following constraints are applied:
    
* xLen >= 1e-6.
* yLen >= 1e-6.
* zLen >= 1e-6.
  */
  virtual void createWedge(
    double xLen, 
    double yLen, 
    double zLen);

  /** \details
    Creates a solid by extruding the specified region to the specified height and with specified taper angle.
   
    \param pRegion      [in] Pointer to the region to be extruded.
    \param height       [in] Height of the extrusion.
    \param taperAngle   [in] Taper angle in radians.

    \returns Returns eOk if extrusion is successful or an appropriate error code if not. 

    \remarks
    The extrusion direction is along the normal of the region. The height value may be negative. pRegion should not have self-intersections.
    Creates a solid by extruding pRegion, a distance of height with a taper angle of taper. The extrusion direction is along the normal of the region if the height is positive. 
    
    The following constraints are applied:
    
* |height| >= 1e-6.
* |taperAngle| < OdaPI2 - 1e-6.
    
    Taper should be between half pi and -half pi. If the absolute value of taper < 1e-6, then the taper angle is set to 0. 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult extrude(
    const OdDbRegion* pRegion, 
    double height, 
    double taperAngle = 0.0);

  /** \details
    Creates a solid by revolving the specified region around the axis. Axis is specified with a point and vector. Revolution can be produced with an angle.
  
    \param pRegion            [in] Pointer to the region to be revolved.
    \param axisPoint          [in] Point on the axis of revolution.
    \param axisDir            [in] Vector defining the axis of revolution.
    \param angleOfRevolution  [in] Angle of revolution in radians.
    
    \returns Returns eOk if creating a solid by revolving is successful or an appropriate error code if not. 

    \remarks
    The axis of revolution is projected onto the plane of the region, parallel to its normal. 
    Revolution axis must be valid for a rotation of region: axisDir is not perpendicular to the plane of a region, axisDir does not pass through a region, etc. 
    
    The following constraints are applied:
* The projected axis of revolution cannot intersect the region. 
* angleOfRevolution >= 1e-6

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult revolve(
    const OdDbRegion* pRegion,
    const OdGePoint3d& axisPoint, 
    const OdGeVector3d& axisDir,
    double angleOfRevolution);

  /** \details
    Performs a Boolean operation between this solid and another solid object. 

    \param operation [in] Type of Boolean operation.	
    \param solid     [out] Pointer to other solid object which also will contain the result of the Boolean operation.
	
	  \returns Returns eOk if Boolean operation is successful or an appropriate error code if not. If solid is null, eInvalidInput is returned.
	
    \remarks 
    Possible operation types are: 
* kBoolUnite - unites two solids into one
* kBoolIntersect - returns the intersection of two solids
* kBoolSubtract - subtracts the second solid from the first one

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status 
    for other modelers. 
  */
    virtual OdResult booleanOper(OdDb::BoolOperType operation, OdDb3dSolid* solid);

  /** \details
    Creates a solid by extruding a region along a path curve. 
	
    \param region [in] A pointer to a region object for extruding from
    \param path   [in] A pointer to a curve object to extrude along. Must be one of the following: 
* Line
* OdDbArc
* OdDbCircle
* Ellipse
* OdDbSpline
* OdDb2dPolyline
* non-spline fit OdDb3dPolyline

    \param taperAngle [in] Extrusion taper angle in radians. Default value is 0.0. 
    Value must be between 0.5 pi and -0.5 pi. 
    If the path represented by the OdDbCurve object is closed, the taper angle parameter is ignored.

	  \returns Returns eOk if extruding operation is successful or an appropriate error code if not. If solid is null, eInvalidInput is returned.
	
    \remarks
    Entity path must not have self-intersections, and it is recommended that the path not have high curvature areas. 
	
    The following constraints are applied: |taper Angle| >= 1e-6

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult extrudeAlongPath(const OdDbRegion* region, const OdDbCurve* path, double taperAngle = 0.0);

  /** \details
    Intersects the given entity with the solid and imprints their intersection graph onto the solid. 

    \param pEntity [in] A Pointer to an entity to be imprinted.
	
	
    \returns Returns eOk if imprinting entity onto the solid is successful or an appropriate error code if not. 
  
    \remarks
    The given entity must be one of the following:
* Curve
* OdDb3dSolid
* Body
* Region
* Trace
* OdDbSolid
* Face
	  
    If the solid is null, or if the given entity is not one of the entities listed above, eInvalidInput is returned.
    
    If as a result of intersection a closed loop of new edges is created, a new face will be made. 
    If an open loop of edges is made, it can be added to an existing loop as a spur. 
    Or it can be added as a slit in the face. 
    
      This method is implemented only for Spatial modeler and returns eNotImplementedYet status
      for other modelers. 
  */
  virtual OdResult imprintEntity(const OdDbEntity *pEntity);

  /** \details
    Calculates the area of the solid.

    \param area [out] Reference for returning the area value.
	
    \returns Returns eOk if successful or an appropriate error code in other case. Area value is returned in square drawing units.
    
    \remarks
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult getArea(double& area) const;

  /** \details
    Checks whether the solid interferes with another solid object.

    \param otherSolid         [in] A pointer to another solid object for checking interference.
    \param createNewSolid     [in] A flag determining where a new solid object containing the interference must be created.
    \param solidsInterfere    [out] A reference to a boolean variable where the result of the interference checking will be stored and returned.
    \param commonVolumeSolid  [out] A reference to a pointer to a new solid containing the interference of the origin solid and otherSolid. 
	
    \returns Returns eOk if interference checking is successful or an appropriate error code in other case.
	
    \remarks
    The caller of this function is responsible for the memory used by the object pointed by commonVolumeSolid (resulting from intersection of this solid and otherSolid) when the function finishes. 
    In the case when commonVolumeSolid is added to a database, the database takes responsibility for the memory pointed by commonVolumeSolid. 
    In the case when commonVolumeSolid is not added to a database, the caller must delete memory when it is no longer needed to avoid a memory leak.

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult checkInterference(const OdDb3dSolid* otherSolid,
    bool createNewSolid, 
    bool& solidsInterfere,
    OdDb3dSolidPtr &commonVolumeSolid)
    const;

  /** \details
    Returns volume properties of the solid.

    \param volume         [out] A reference for returning the volume of the solid.
    \param centroid       [out] A reference to an OdGePoint3d object for returning the point, representing the centroid of the solid.
    \param momInertia     [out] A static array for returning X, Y, Z moments of inertia of the solid.
    \param prodInertia    [out] A static array for returning X, Y, Z products of inertia of the solid. 
    \param prinMoments    [out] A static array for returning X, Y, Z principal moments of the solid.
    \param prinAxes       [out] A static array for returning an OdGeVector3d object, determining principal axes of the solid.
    \param radiiGyration  [out] A static array for returning X, Y, Z radii of gyration of the solid.
    \param extents        [out] A reference to an OdGeExtents3d object for returning the bounding box of the solid.

    \returns Returns the result of getting volume properties of the solid: eOk if successful or an appropriate error code in other case. All properties are evaluated in the WCS.
    
    \remarks
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult getMassProp(double& volume,
    OdGePoint3d& centroid,
    double momInertia[3],
    double prodInertia[3],
    double prinMoments[3],
    OdGeVector3d prinAxes[3], 
    double radiiGyration[3],
    OdGeExtents3d& extents) const;

  /** \details
    Creates a DbRegion entity as a result of the intersection between the specified plane and the solid. 
	
    \param plane          [in] A reference to an OdGePlane object to use as the section cutting plane.
    \param sectionRegion  [out] A reference to a new region created as the result of the intersection between the specified plane and the solid.

    \returns Returns the result of the intersecting the solid with a plane: eOk if successful or an appropriate error code in other case. 
    Value eOk is also returned when a plane and the solid do not intersect each other or if the intersection region is not a valid region: 
    it intersects at a point, along a bounding face of the solid and so on. 
	
    \remarks
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult getSection(const OdGePlane& plane, OdDbRegionPtr &sectionRegion) const;

  /** \details
    Slices the solid with a plane and transforms the native solid to be the portion remaining on the positive side of the normal of the slicing plane.
	
    \param plane          [in] A reference to a slicing plane.
    \param bGetNegHalfToo [in] A flag determining whether the other (negative) part of the native solid should be created as a separate solid (when true).
    \param pNegHalfSolid  [out] A reference to a pointer to the created solid representing the negative half of the native solid. If bGetNegHalfToo is false, NULL value is returned.
	
    \returns Returns eOk if the native solid was successfully sliced or an appropriate error code in other case. 
    If the slicing plane does not intersect the solid, the solid is left unchanged, pNegHalfSolid is set to NULL, and eOk is returned. 
	
    \remarks
    The caller of this function is responsible for the memory used by the object pointed to by pNegHalfSolid (negative part of the sliced solid) when the function finishes. 
    In the case when pNegHalfSolid is added to a database, the database takes responsibility for the memory. 
    In the case when pNegHalfSolid is not added to a database, the caller must delete memory when it is no longer needed to avoid a memory leak.
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult getSlice(const OdGePlane& plane, bool bGetNegHalfToo, OdDb3dSolidPtr &pNegHalfSolid);

  /** \details
    Slices the solid with a surface and transforms the native solid to be the portion remaining on the positive side of the slicing surface normal.
	
    \param pSurface       [in] A pointer to a slicing surface.
    \param bGetNegHalfToo [in] A flag determining whether the other (negative) part of the native solid should be created as a separate solid (when true).
    \param pNegHalfSolid  [out] A reference to a pointer to the created solid representing the negative half of the native solid. If bGetNegHalfToo is false, NULL value is returned.
	
    \returns Returns eOk if the native solid was successfully sliced or an appropriate error code in other case. 
    If pSurface pointer is NULL or the solid is empty, returns eInvalidInput.
    
    \remarks
    The native solid will be transformed to a resulting body which is located in the negative side of the surface normal.	
    
    The caller of this function is responsible for the memory used by the object pointed to by pNegHalfSolid (negative part of the sliced solid) when the function finishes. 
    In the case when pNegHalfSolid is added to a database, the database takes responsibility for the memory. 
    In the case when pNegHalfSolid is not added to a database, the caller must delete memory when it is no longer needed to avoid a memory leak.
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult getSlice(OdDbSurface* pSurface,  bool bGetNegHalfToo, OdDb3dSolidPtr &pNegHalfSolid);

  /** \details
    Removes all edges and faces that are not needed for supporting the topology of the solid. 
	
    \returns Returns eOk if successful or an appropriate error code in other case.
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult cleanBody();

  /** \details
    Offsets all faces by the specified distance. 
	
    \param offsetDistance) [in] A distance for offset.

    \returns Returns eOk if all faces are successfully offset, or an appropriate error code in other case. 
    Returns eInvalidInput if the offsetDistance is less than -maxSide/2, 
    where maxSide is the size of the greater side of the solid's bounding box. 
    Also eInvalidInput is returned when the absolute value of offsetDistance is less than 1e-6.
    
    \remarks	
    If a face has surfaces that cannot be offset, it is removed. Therefore, the offset operation can cause the solid to become invalid. 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult offsetBody(double offsetDistance);

  /** \details
    Separates the solid into an array of solids representing the additional disjoint volumes. This solid is reduced to a solid with one volume. 

    \param newSolids [out] A reference to an array of pointers to OdDb3dSolid objects representing the additional disjoint volumes. 

    \returns Returns eOk if separating was successful or an appropriate error code in other case.
	
    \remarks
    The calling application is responsible for the resulting entities (either appending them to a database or deleting them when they are no longer needed). 
    If the new solids from the output array are not appended to the database, there will be a memory leak. 
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult separateBody(OdDb3dSolidPtrArray &newSolids);

  /** \details
    Creates a solid by sweeping a planar curve, region, or planar surface in the direction of the given vector using a distance equal to the length of the vector.

    \param pSweepEnt    [in] A pointer to the swept entity.
    \param directionVec [in] Vector determining the direction and length of extruding.
    \param sweepOptions [in] A set of sweeping options.

    \returns Returns eOk if extruding was successful or an appropriate error code in other case.
    
    \remarks	
    Sweeping parameters (for example, angle) are set through the sweepOptions object reference. 
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createExtrudedSolid ( OdDbEntity* pSweepEnt,
    const OdGeVector3d& directionVec,
    OdDbSweepOptions& sweepOptions );

  /** \details
    Creates a solid by extruding a face.

    \param pSweepEnt    [in] A pointer to the swept entity.
    \param faceSubentId [in] Object ID for the face that must be extruded.
    \param directionVec [in] Vector determining the direction and length of extruding.
    \param sweepOptions [in] A set of sweeping options.

    \returns Returns eOk if extruding was successful or an appropriate error code in other case.
    
    \remarks
		Sweeping parameters (for example, angle) are set through the sweepOptions object reference. 
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
    */
  virtual OdResult createExtrudedSolid ( OdDbEntity* pSweepEnt,
    const OdDbSubentId& faceSubentId,
    const OdGeVector3d& directionVec, 
    OdDbSweepOptions& sweepOptions );

  /** \details
    Creates a solid by extruding a face.

    \param pSweepEnt    [in] A pointer to the swept entity.
    \param faceSubentId [in] Object ID for the face that must be extruded.
    \param height       [in] Multiplier for the direction vector.
    \param sweepOptions [in] A set of sweeping options.

    \returns Returns eOk if extruding was successful or an appropriate error code in other case.
    
    \remarks
		Sweeping parameters (for example, angle) are set through the sweepOptions object reference. 
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createExtrudedSolid ( OdDbEntity* pSweepEnt,
    const OdDbSubentId& faceSubentId,
    double height, 
    OdDbSweepOptions& sweepOptions );

  /** \details
    Creates a lofted solid from the given curves.
    	
    \param crossSectionCurves [in] An array of curve entities to be used as cross sections for the lofting operation.
    \param guideCurves        [in] Array of guide curves.
    \param pPathCurve        [in] Path curve.
    \param loftOptions        [in] Options for lofting.
	
    \returns Returns eOk if successful or an appropriate error code in other case.

    \remarks
    Lofting parameters can be passed with the loftOptions parameter. 
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createLoftedSolid (
    OdDbEntityPtrArray& crossSectionCurves,
    OdDbEntityPtrArray& guideCurves,
    OdDbEntity* pPathCurve, OdDbLoftOptions& loftOptions );

  /** \details
    Creates a pyramid. World origin is centered about the base of the pyramid and the pyramid is positioned at half of the specified height above the world origin.

    \param height     [in] Value of the pyramid's height.
    \param sides      [in] Quantity of sides for the pyramid.
    \param radius     [in] Radius for circle inscribed into the bottom polygon.
    \param topRadius  [in] Radius for circle inscribed into the top polygon. 
	
    \returns Returns eOk if successful or an appropriate error code in other case. 
    
    \remarks 
    The pyramid's base is located in the X-Y plane of the WCS. The pyramid's direction (from the base to the top) coincides with the positive direction of the Z axis.
    When radius and topRadius values are equal, a prism is created. 
	
    The following constraints are applied:
* |height| >= 1e-6;
* |radius| >= 1e-6;
* |topRadius| >= 1e-6;
* 3 <= sides <= 64.
  */
	  virtual OdResult createPyramid(double height, int sides, 
    double radius, double topRadius = 0.0 );

  /** \details
    Creates a solid by revolving a specified curve, region, or planar surface around the specified axis of revolution.

    \param pRevEnt        [in] Pointer to the planar curve, region, or planar surface that must be revolved. 
    \param axisPnt        [in] Point on the axis of revolution.
    \param axisDir        [in] Direction of the revolution axis.
    \param revAngle       [in] Revolve angle in radians.
    \param startAngle     [in] Start angle of rotation. If 0, then rotation will start from current position of pRevEnt.
    \param revolveOptions [in] Revolve options.

    \returns Returns eOk if successful or an appropriate error code in other case. 
    
    \remarks
    Axis of revolution is defined by a point (axisPnt) and vector (axisDir). 
    pRevEnt will be revolved according to the start and revolve angles counter-clockwise about the axisDir vector. 
    
    Options for revolving can be specified with the revolveOptions object reference.

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createRevolvedSolid ( OdDbEntity* pRevEnt,
    const OdGePoint3d& axisPnt,
    const OdGeVector3d& axisDir,
    double revAngle, double startAngle,
    OdDbRevolveOptions& revolveOptions );

  /** \details
    Creates a solid by revolving a face about the axis of revolution.
	
    \param pRevEnt        [in] Pointer to entity containing face for revolving.
    \param faceSubentId   [in] Face's subentity ID.
    \param axisPnt        [in] Point on the axis of revolution.
    \param axisDir        [in] Direction of revolution axis.
    \param revAngle       [in] Revolve angle in radians.
    \param startAngle     [in] Start angle of rotation. If 0, then rotation will start from current position of pRevEnt.
    \param revolveOptions [in] Revolve options.
	
    \returns Returns eOk if successful, or an appropriate error code in other case.
	
    \remarks
    Entity for revolving must be a valid planar curve, region, or planar surface. 
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createRevolvedSolid ( OdDbEntity* pRevEnt,
    const OdDbSubentId& faceSubentId,
    const OdGePoint3d& axisPnt,
    const OdGeVector3d& axisDir,
    double revAngle, double startAngle,
    OdDbRevolveOptions& revolveOptions );

  /** \details
    Creates a swept solid by sweeping a face along the specified path.

    \param pSweepEnt    [in] Pointer to face entity for sweeping.
    \param faceSubentId [in] Face's subentity ID.
    \param pPathEnt     [in] Pointer to the curve entity that determines the path to sweep along.
    \param sweepOptions [in] Sweep options.

    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks
    To set optional sweep parameters (for example, draft angle) use the sweepOptions parameter.
    The default value for the alignment option of OdDbSweepOptions (returned by align() method) is kNoAlignment. 
    Set it to kAlignSweepEntityToPath before calling this method.

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createSweptSolid ( OdDbEntity *pSweepEnt,
    const OdDbSubentId& faceSubentId,
    OdDbEntity* pPathEnt,
    OdDbSweepOptions& sweepOptions );

  /** \details
    Creates a swept solid by sweeping a curve, region, or planar surface along the specified path.
	
    \param pSweepEnt    [in] Pointer to face entity for sweeping.
    \param pPathEnt     [in] Pointer to the curve entity that determines the path to sweep along.
    \param sweepOptions [in] Sweep options.
	
    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks	
    To set optional sweep parameters (for example, draft angle) use the sweepOptions parameter. 	
    The default value for the alignment option of OdDbSweepOptions (returned by align() method) is kNoAlignment. 
    Set it to kAlignSweepEntityToPath before calling this method.
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createSweptSolid ( OdDbEntity* pSweepEnt,
    OdDbEntity* pPathEnt,
    OdDbSweepOptions& sweepOptions );

  /** \details
    Writes out a Stereo Lithography (STL) representation of the solid to a file with the specified name.
	
    \param filename    [in] Full file name.
    \param asciiFormat [in] A flag determining the format of the output file (if true, file has ASCII text format, or in other case file has binary format).
	
    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks
    If the specified file already exists, it will be overwritten.	
    
    This method is not implemented. 
  */

  virtual void stlOut(const OdChar* filename, bool asciiFormat) const;

  virtual OdDbObjectPtr decomposeForSave(
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData);

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm );
  
    /** \details
    Saves 3D solid entity to a .dwg file.
	
    \param pWd [in] Pointer to an OdGiWorldDraw object instance used for saving the solid.
	\param ver [in] Version of saved .dwg file.
    */
    
  /** \details
    Creates a geometric representation of the entity for saving proxy graphics or converting to previous formats.

    \param pWd [in] Pointer to the OdGiWorldDraw object.
    \param ver [in] Specifies the reason why the method is called.

    \remarks
    This method can be overridden in a custom classes.
  */
  virtual void saveAs(
    OdGiWorldDraw* pWd,
    OdDb::DwgVersion ver ) const;

  /** \details
    Directly sets the contained modeler geometry of this entity.
	 
    \param pGeometry [in] Pointer to the modeler geometry.

    \returns Returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult setBody( const void* pGeometry );
  
  /** \details
    Returns contained modeler geometry of this entity.
  */
  virtual void *body( ) const;

  /** \details
    Copies the specified edge as a separate entity (instance of OdDbLine, OdDbCircle, OdDbArc, OdDbEllipse, OdDbSpline class).
	
    \param subentId   [in] Edge's subentity ID in the solid.
    \param newEntity  [in] Reference to a new entity for the specified edge.
	
    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks
    The properties of the resulting entity (color, layer, line type, lineweight) are inherited from the corresponding current document.
    The calling application is responsible for the memory located with the resulting entity. 
    If the resulting entity is not appended to a database, it can cause a memory leak (if it is not deleted when it isn't needed anymore). 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
    */
  virtual OdResult copyEdge(const OdDbSubentId &subentId, OdDbEntityPtr &newEntity);
  
  /** \details
    Copies the specified face as a separate entity (OdDbRegion or otherwise will be OdDbBody).

    \param subentId   [in] Face's subentity ID in the solid.
    \param newEntity  [in] Reference to a new entity containing a copy of the specified face.
    
    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks
    If face is planar, an OdDbRegion entity is created. Otherwise an OdDbBody entity is created. 
    The properties of the resulting entity (color, layer, line type, lineweight) are inherited from the corresponding current document.
    
    The calling application is responsible for the memory located with the resulting entity. 
    If the resulting entity is not appended to a database, it can cause a memory leak (if it is not deleted when it isn't needed anymore). 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult copyFace(const OdDbSubentId &subentId, OdDbEntityPtr &newEntity);

  virtual OdResult subGetSubentPathsAtGsMarker(OdDb::SubentType type,
                                               OdGsMarker gsMark, 
                                               const OdGePoint3d& pickPoint,
                                               const OdGeMatrix3d& viewXform, 
	                                             OdDbFullSubentPathArray& subentPaths, 
	                                             const OdDbObjectIdArray* pEntAndInsertStack = 0) const;

	
  virtual OdDbSubentId internalSubentId(void* ent) const;
  
  virtual void* internalSubentPtr(const OdDbSubentId& id) const;

  virtual OdResult subGetGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath, 
                                               OdGsMarkerArray& gsMarkers) const;

  virtual OdDbEntityPtr subSubentPtr(const OdDbFullSubentPath& id) const;

  /** \details
    Extrudes faces of the solid with specified distance and taper angle.

    \param faceSubentIds  [in] Face's subentity ID in the solid.
    \param height         [in] Distance of extrusion.
    \param taper          [in] Taper angle.
  
    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks  
    The distance of extrusion is determined by the height parameter and the taper angle (the taper parameter). 
  
    The extrusion direction can be along or opposite the normal of each extruded face depending on the height value: 
* If height is positive, direction is along the normal (extruding faces outward from the solid).
* If height is negative, direction is opposite the normal (extruding faces into the solid).
  
    If the absolute value of taper is less than 1e-6, taper angle is assumed to be equal to 0.0.

    If taper value does not equal 0.0, each face should have edges that join together smoothly (i.e. tangents at the points of connection must be equal). 
  
    The following constraints are applied:
* number of specified faces (in faceSubentIds parameter) must be more than zero
* |height| >= 1e-6
* |taper| >= (PI/2) - 1e-6
  
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult extrudeFaces(const OdArray<OdDbSubentId *> &faceSubentIds, 
    double height, double taper);

  /** \details
    Extrudes specified faces along the specified path determined by a curve entity. 

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be extruded.
    \param path          [in] Pointer to a curve object to extrude along (extrusion path). Can not be NULL.

    \returns Returns eOk if successful or an appropriate error code in other case.

    Faces for extrusion must be planar.
    
    Curve entity must be one of the following:
* OdDbLine
* OdDbArc
* OdDbEllipse
* OdDbSpline
* OdDb2dPolyline
* Non-spline fit OdDb3dPolyline

    Extrusion path must not have self-intersections. Also it is recommended that the path doesn't have high curvature areas. 
    If one of the end points of the path is not located on the region's plane, the path will move the point on the plane. 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult extrudeFacesAlongPath(const OdArray<OdDbSubentId *> &faceSubentIds, 
    const OdDbCurve* path);

  /** \details
    Tapers the specified faces about the specified base point and draft vector for a specified draft angle.   

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be tapered.
    \param basePoint     [in] Reference to the base point of the draft plane.
    \param draftVector   [in] Reference to the draft direction vector.
    \param draftAngle    [in] Draft angle in radians.

    \returns Returns eOk if successful or an appropriate error code in other case.

    \remarks    
    The base point and draft vector define a draft plane about which faces are tapered. 
    If a face is lying in the draft plane, it will not be modified. 

    Tapering faces can cause the solid to become invalid in some cases. 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult taperFaces(const OdArray<OdDbSubentId *> &faceSubentIds, 
    const OdGePoint3d &basePoint, 
    const OdGeVector3d &draftVector,
    double draftAngle);

  /** \details
    Transforms the specified faces of a solid by applying the transformation matrix for rotation and (or) moving the faces. 

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be transformed.
    \param matrix         [in] Transformation matrix to be applied to the specified faces.

    \returns Returns eOk if successful or an appropriate error code in other case.	

    \remarks
    Transformation faces can cause the solid to become invalid in some cases. 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult transformFaces(const OdArray<OdDbSubentId *> &faceSubentIds,  const OdGeMatrix3d &matrix);
  
  /** \details
    Removes the specified faces from the solid.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be removed.

    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks
    After removing specified faces, the adjacent faces will enlarge to fill the gaps. 
    This method implements gap filling only if there is no other solution using adjacent faces. 
    If adjacent faces can't be used, the method leaves the solid unmodified. 

    Removing faces can cause the solid to become invalid in some cases. 
    
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult removeFaces(const OdArray<OdDbSubentId *> &faceSubentIds);
  
  /** \details
    Offsets the specified faces of the solid for the specified distance. 	

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be offset.
    \param offsetDistance [in] Offset distance.

    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks
    Faces with surfaces that cannot be offset are removed and the solid is healed, if possible. 

    The following constraints are applied:
* number of specified faces (in faceSubentIds parameter) must be more than zero
* offsetDistance >= minus half of the max side of the solid's bounding box
* |offsetDistance| >= 1e-6

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
    */
  virtual OdResult offsetFaces(const OdArray<OdDbSubentId *> &faceSubentIds, double offsetDistance);
  
  /** \details
    Transforms the solid into a thin-walled solid shell by offsetting faces. 

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be removed from the shell.
    \param offsetDistance [in] Offset distance for each face.

    \returns Returns eOk if successful or an appropriate error code in other case.

    \remarks 
    Offset can have an outside or inside direction related to the solid, depending on the sign of the offsetDistance parameter:
* positive value means outside direction
* negative value means inside direction

    If a face has surfaces that cannot be offset by the thickness, it will be removed from the solid. Removal wound is healed with the surrounding face surfaces. 
    Edges and vertices on the specified faces will be merged if possible. 
    Isolated edges in tweaked faces and neighboring faces may degenerate to a point and will be removed. 
    However, when removing loops or faces, insertion of edges or other topology changes are prohibited.
    
    Operation can cause the solid to become invalid in some cases.

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult shellBody(const OdArray<OdDbSubentId *> &faceSubentIds, double offsetDistance);

  /** \details
    Returns the number of changes occurred since the solid's creation.
  */
  virtual OdUInt32 numChanges() const;

  /** \details
    Sets the color of the specified face or edge subentity. 

    \param subentId [in] ID of subentity (face or edge) to be colored.
    \param color    [in] Color value to set.

    \returns Returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult setSubentColor(const OdDbSubentId &subentId, const OdCmColor &color);
  
  /** \details
    Returns the color of the specified subentity.

    \param subentId [in] ID of subentity.
    \param color    [in] A reference to an OdCmColor object to store the current color of the specified subentity.

    \returns Returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult getSubentColor(const OdDbSubentId &subentId, OdCmColor &color) const;
  
  /** \details
    Sets a material for the specified subentity.

    \param subentId [in] Subentity ID 
    \param matId    [in] Object ID for the material.

    \returns Returns eOk if successful or an appropriate error code in other case
    
    \remarks
    To remove the currently assigned material, set matId parameter value to NULL.    
  */
  virtual OdResult setSubentMaterial(const OdDbSubentId &subentId, OdDbObjectId matId);

  /** \details
    Returns the material of the specified subentity.

    \param subentId [in] Subentity ID.
    \param matId    [in] Reference to an OdDbObjectId for returning the material object ID.
    
    \returns Returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult getSubentMaterial(const OdDbSubentId &subentId, OdDbObjectId& matId) const;

  /** \details
    Sets an explicit material mapper for the specified subentity.

    \param subentId [in] Subentity ID 
    \param mapper   [in] Pointer to an OdGiMapper object.

    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks 
    To remove the current material mapper assignment, set map parameter value to NULL.
  */
  virtual OdResult setSubentMaterialMapper(const OdDbSubentId &subentId, const OdGiMapper& mapper);
  
  /** \details
    Returns current material mapper for the specified subentity.

    \param subentId [in] Subentity ID.
    \param mapper   [out] Reference to an OdGiMapper object containing the current subentity material mapper.
	
    \returns Returns eOk if successful or an appropriate error code in other case.
  */
    virtual OdResult getSubentMaterialMapper(const OdDbSubentId &subentId, OdGiMapper& mapper) const;
  
  /** \details
    Creates chamfers at the edges of a solid.

    \param edgeSubentIds    [in] Array of edge object IDs. The chamfer will be applied at the specified edges.
    \param baseFaceSubentId [in] Base face object IDs. Chamfer will be applied on the specified face.
    \param baseDist         [in] Chamfer distance for the base face.
    \param otherDist        [in] Chamfer distance for the other faces.

    \returns Returns eOk if successful or an appropriate error code in other case.

    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult chamferEdges(const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdDbSubentId& baseFaceSubentId, 
                                double baseDist, 
                                double otherDist);

  /** \details
    Creates rounded fillets on the edges of a solid.

    \param edgeSubentIds [in] Array of edges' object IDs. Fillet will be applied on the specified edges.
    \param radius        [in] Array of fillet radius values for the corresponding edges.
    \param startSetback  [in] Array of start setback values for the corresponding edges.
    \param endSetback    [in] Array of end setback values for the corresponding edges.

    \returns Returns eOk if successful or an appropriate error code in other case

    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult filletEdges( const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdGeDoubleArray& radius, 
                                const OdGeDoubleArray& startSetback, 
                                const OdGeDoubleArray& endSetback);

  /** \details
    Creates a 3D solid from the specified entity.

    \param pFromEntity [in] Pointer to an entity from which the solid must be created. 
	
    \returns Returns eOk if conversion is successful or an appropriate error code in other case.

    \remarks     
    An entity to create from must enclose a volume and can be one of the following:
* OdDbSurface
* OdDbSubDMesh
* closed curve with thickness
* thick OdDbSolid
* thick OdDbTrace
* polyline with constant width and thickness

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createFrom(const OdDbEntity* pFromEntity);

  /** \details
    Returns whether the solid records its operations history.

    \remarks
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual bool recordHistory() const;
  
  /** \details
    Returns whether to draw items from the solid's operation history.
	
    \remarks
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
    virtual bool showHistory() const;

  /** \details
    Sets a new value for the flag that determines whether the solid records its operation history.
	
    \param bRecord [in] Flag determining whether the solid records operation history.
	
    \returns Returns eOk if successful or an appropriate error code in other case
  */
    virtual OdResult setRecordHistory(bool bRecord);
  
  /** \details
    Sets a new value for the flag that determines whether to draw items from the solid's operation history.
	
    \param bShow [in] Flag determining whether the solid records operation history.
	
    \remarks Returns eOk if successful or an appropriate error code in other case.
  */
      virtual OdResult setShowHistory(bool bShow);

  /** \details
    Converts individual history items from a parameterized type to a simple boundary-representation without its parameter or type information.

    \param paths [in] Array of references to history items.

    \returns Returns eOk if successful or an appropriate error code in other case.
	
    \remarks 
    This method is not implemented. 
  */
  virtual OdResult convertToBrepAtSubentPaths(const OdDbFullSubentPathArray& /*paths*/);

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  /** \details
    Creates non-database resident entities by projecting the specified entity along the projection direction to the solid.
    	

    \param pEntityToProject    [in] Pointer to the entity to be projected.
    \param projectionDirection [in] Vector determining the projection direction.
    \param projectedEntities   [out] Array of pointers to non-database resident entities created by the projection.
	
    \returns Returns eOk if successful or an appropriate error code in other case.
    
    \remarks	
    Specified pointer to the entity must not be equal to NULL and the entity must be of a supported type.
	
    When an OdDbPoint is specified as the entity to be projected, the result of the projection will be an AcDbPoint representing the nearest point on the solid.
	
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  OdResult projectOnToSolid( const OdDbEntity*   pEntityToProject,
                             const OdGeVector3d& projectionDirection,
                             OdDbEntityPtrArray& projectedEntities) const;

  /** \details
    Creates a solid by trimming and (or) extending the specified surfaces and 3D solids, forming an enclosed volume.

    \param limitingBodies [in] Reference to the entity array (representing surface and solids) that are used to create a solid.
    \params limitingFlags [in] Reserved for internal use.  

    \returns Returns eOk if successful or an appropriate error code in other case.

    \remarks 
    This method is not implemented. 
  */
  OdResult createSculptedSolid(OdDbEntityPtrArray& limitingBodies,
                               const OdGeIntArray& limitingFlags);

  /** \details
    This method is not implemented. 
  */
  virtual void subClose();

  virtual OdResult subGetGeomExtents( OdGeExtents3d& extents) const;
};

#include "TD_PackPop.h"

#endif

