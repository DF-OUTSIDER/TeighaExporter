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


#ifndef _DBSURFACE_H_INCLUDED_
#define _DBSURFACE_H_INCLUDED_
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2016 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 

#include "TD_PackPush.h"
#include "ModelerDefs.h"
#include "DbEntity.h"
#include "Db3dSolid.h"

class OdModelerGeometry;
class OdStreamBuf;
class OdBrBrep;

class OdDbSurface;
class OdDbNurbSurface;

/** \details
  This template class is a specialization of the OdArray class for OdDbSurface object pointer arrays.
*/
typedef OdArray<OdDbNurbSurface*, OdMemoryAllocator<OdDbNurbSurface*> > OdDbNurbSurfaceArray;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSurfacePtr object pointers.
*/
typedef OdSmartPtr<OdDbSurface> OdDbSurfacePtr;

/** \details
  Determines surface's edge extension types.
*/
enum EdgeExtensionType 
{
  /** kExtendEdge (0x00) - Edges are extended.*/
  kExtendEdge  = 0x00,
  /** kStretchEdge (0x01) - Edges are stretched.*/
  kStretchEdge = 0x01,
};

/** \details
    This class implements a three-dimensional surface entity.
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSurface : public OdDbEntity
{
public:
  OdDbSurface();
  ODDB_DECLARE_MEMBERS(OdDbSurface);

  /** \details
     Returns the number of isolines in the U direction of the surface, displayed in wireframe mode for visualization.
  */
  virtual OdUInt16 uIsolineDensity() const;

  /** \details
     Sets the number of isolines in the U direction of the surface, displayed in wireframe mode for visualization.
     
     \param numIsolines [in]  The number of isolines in the U direction of the surface, displayed in wireframe mode for visualization. 
  */
  virtual void setUIsolineDensity(OdUInt16 numIsolines);

  /** \details
     Returns the number of isolines in the V direction of the surface, displayed in wireframe mode for visualization.
  */
  virtual OdUInt16 vIsolineDensity() const;

    /** \details
     Sets the number of isolines in the V direction of the surface, displayed in wireframe mode for visualization.
     
     \param numIsolines [in]  The number of isolines in the V direction of the surface, displayed in wireframe mode for visualization. 
    */
  virtual void setVIsolineDensity(OdUInt16 numIsolines);

  /** \details
    Creates an extruded surface from a 2D entity with thickness
    or a planar surface from a closed curve or region.
    
    \param pFromEntity [in] Input surface
    \param pNewSurface [out] Resulting surface

    \returns Returns eOk if an extruded surface was created successfully or an appropriate error code in the other case. 
    
    \remarks
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  static OdResult createFrom ( const OdDbEntity* pFromEntity, OdDbSurfacePtr& pNewSurface );

  /** \details
    Creates region (OdDbRegion class) entities for each face of the surface. 

    \param regions [out]  Reference to an array of the OdDbRegion entities created from the face of the surface.
  
    \returns Returns eOk if successful, or an appropriate error code otherwise.
    
    \remarks
    The surface must be planar. 
    Regions created by the method are not included in the database, so the caller is responsible for managing memory of the created OdDbRegion objects.
  
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult convertToRegion ( OdDbEntityPtrArray& regions );

  /** \details
    Creates a 3D solid object by thickening the surface by a specified amount. 
    
    \param thickness  [in] Amount to thicken the surface.
    \param bBothSides [in] Flag indicating whether the thickening should be applied to both sides of the surface.
    \param pSolid 	  [out] Reference to an OdDb3dSolid object pointer created during the thickening. If thickening fails, this parameter is NULL.
    
    \returns Returns eOk if successful, or an appropriate error code otherwise.
    
    \remarks
    A solid created by this method is not included in the database, so the caller is responsible for managing memory allocated for the solid.
    If thickness is less than zero, the surface will be thickened in the opposite direction.
    
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult thicken(double thickness, bool bBothSides, OdDb3dSolidPtr& pSolid) const;

  /** \details
    Calculates the total surface area.
    
    \param area [out]  A reference for returning the calculated surface area value.
    
    \returns Returns eOk if successful, or an appropriate error code otherwise.
    
    \remarks 
    Method calculates the area of a single side of the surface.
    
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult getArea(double& area) const;

  /** \details
    Returns true if and only if there is no ACIS model associated with this entity. 
  */
  bool isNull() const;

  /** \details
    Writes the ACIS data of this entity to the specified StreamBuf object, 
    or writes the specified ACIS solids to the specified file.

    \param pStreamBuf [in] Pointer to the StreamBuf object to which the data is to be written. 
    \param typeVer    [in] Type and version of the ACIS data to write.

    \returns Returns eOk if successful or an appropriate error code otherwise. 
  */
  OdResult acisOut(
    OdStreamBuf* pStreamBuf, 
    AfTypeVer typeVer = kAfTypeVerAny);

  /** \details
    Reads the ACIS data for this entity from the specified StreamBuf object,
    or reads an array of ACIS solids from the specified file.

    \param pStreamBuf [in] Pointer to the StreamBuf object from which the data is to be read.   
    \param pTypeVer [in] Receives the type and version of the ACIS data read.

    \returns Returns eOk if successful or an appropriate error code otherwise. 
    If pTypeVer is not null, this function returns the version of the ACIS data
    of this 3D solid entity.
  */
  OdResult acisIn(
    OdStreamBuf* pStreamBuf, 
    AfTypeVer *pTypeVer = 0);

  /** \details
    Returns the boundary representation of the 3D solid entity.
    \param brep [out]  Receives the boundary representation.
  */
  void brep( OdBrBrep& brep );

  bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) const;

  virtual OdDbObjectPtr decomposeForSave(
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData);

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm );

  /** \details
    Directly sets the contained modeler geometry of this entity.
    
    \param pGeometry [in]  Pointer to the modeler geometry.
    
    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setBody( const void* pGeometry );
  
  /** \details 
    Returns the current modeler geometry. 
  */
  virtual void *body( ) const;

  // OdDbObject methods
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

  /** This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  virtual OdDbSubentId internalSubentId(void* ent) const;
  /** This method is implemented only for Spatial modeler and returns eNotImplementedYet status
  for other modelers. 
  */
  virtual void* internalSubentPtr(const OdDbSubentId& id) const;

  virtual OdDbEntityPtr subSubentPtr(const OdDbFullSubentPath& id) const;
  /** \details
    Creates new entities that represent the result of interference between the surface and specified entity. Created entities are appended to the specified array.
    
    \param interferenceObjects [out]  Output array of interference entities, which can contain OdDbSurface or curve entities.
    \param pEntity             [in]   Pointer to an entity the surface must be interfered with. pEntity can point to the following entity types: OdDbSurface, OdDb3dSolid, OdDbRegion, or OdDbBody.
    \param flags               [in]   Interference flags; equal to kDefault (0x00).
    
    \returns Returns eOk if interference entities are successfully created or an appropriate error code otherwise. 
    For example, if interference entities are not created, eNoIntersection error code is returned.
    eInvalidInput is returned if an unsupported entity or NULL pointer is provided as a value of pEntity.  
    
    \remarks 
    Created entities are not included in the database, so caller is responsible for the correct handling of them. 
    This means that caller must include created entities in the database or delete them when they are no longer needed.
    
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createInterferenceObjects(OdDbEntityPtrArray& interferenceObjects,
                                             OdDbEntityPtr pEntity, 
                                             unsigned int flags ) const;

  /** \details
    Performs a Boolean union operation with two surfaces: this one and another specified surface. Creates a new surface entity as a result. 
    
    \param pSurface 	  [in] Pointer to the other surface to be united with this surface.
    \param pNewSurface  [out] Pointer to a new surface that represents the union operation result.
    
    \returns Returns eOk if the union operation is successfully performed or an appropriate error code otherwise. If the union operation fails, NULL is returned as a value of pNewSurface.
    
    \remarks 
    Resulting surface is not included in the database, so caller is responsible for the correct handling of the memory pointed to with pNewSurface. 
    
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult booleanUnion(const OdDbSurfacePtr pSurface, OdDbSurfacePtr& pNewSurface);
  
  /** \details
    Performs a Boolean subtraction operation with two surfaces: this one and another specified surface. Creates a new surface entity as a result of subtracting the specified surface from this surface. 

    \param pSurface 	  [in] Pointer to the other surface to be subtracted from this surface.
    \param pNewSurface  [out] Pointer to a new surface that represents the subtraction operation result.
    
    \returns Returns eOk if the subtraction operation is successfully performed, or an appropriate error code otherwise. 
    If the subtraction operation fails, NULL is returned as a value of pNewSurface.
    
    \remarks 
    Resulting surface is not included in the database, so caller is responsible for the correct handling of the memory pointed to with pNewSurface. 
    
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult booleanSubtract(const OdDbSurfacePtr pSurface, OdDbSurfacePtr& pNewSurface);
  
  /** \details
    Performs a Boolean subtraction operation with a specified solid object and the surface. Creates a new surface entity as a result of subtracting the specified solid from this surface. 
   
    \param pSolid	      [in] Pointer to the specified solid to be subtracted from the surface.
    \param pNewSurface  [out] Pointer to a new surface that represents the subtraction operation result.
    
    \returns Returns eOk if the subtraction operation is successfully performed or an appropriate error code otherwise. 
    If the subtraction operation fails, NULL is returned as a value of pNewSurface.
    
    \remarks 
    Resulting surface is not included in the database, so caller is responsible for the correct handling of the memory pointed to with pNewSurface. 
    
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult booleanSubtract(const OdDb3dSolidPtr pSolid, OdDbSurfacePtr& pNewSurface);
  
  /** \details
    Performs a Boolean intersection operation for two surfaces: this one and another specified surface. 
    Entities representing the intersections between two entities are stored in an array. 
    
    \param pSurface2 		        [in] Pointer to the other surface to be intersected with the surface.
    \param intersectionEntities [out] A reference to an entity array for storing the results of the intersection operation. 
	The resulting array can contain the following entity types: OdDbSurface, OdDbLine, OdDbCircle, OdDbArc, OdDbEllipse, and OdDbSpline.
    
    \returns Returns eOk if the intersection operation is successfully performed or an appropriate error code otherwise. 

    \remarks 
    Resulting entities are not included in the database, so the caller is responsible for the correct handling of the memory pointed to with the resulting array's items. 
	
	This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult booleanIntersect(const OdDbSurfacePtr pSurface2, OdDbEntityPtrArray& intersectionEntities);

  /** \details
    Performs a Boolean intersection operation between this surface and the specified solid object. 
    Entities representing the intersections between the two entities are stored in an array. 
    
    \param pSolid			          [in]  Pointer to the solid object to be intersected with the surface.
    \param intersectionEntities [out] A reference to an entity array for storing the results of the intersection operation between the solid and surface.
	The resulting array can contain the following types of entities: OdDbSurface, OdDbLine, OdDbCircle, OdDbArc, OdDbEllipse, and OdDbSpline.

    \returns Returns eOk if the intersection operation is successfully performed, or an appropriate error code otherwise. 

    \remarks 
    Resulting entities are not included in the database, so the caller is responsible for the correct handling of the memory pointed to with the result array's items. 
	
	This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult booleanIntersect(const OdDb3dSolidPtr pSolid, OdDbEntityPtrArray& intersectionEntities);

  /** \details
    Intersects the specified entity with the surface and imprints the intersection graph onto the surface. 
    
    In the case of creating a closed loop of edges, a new face is created. 
    In the case of creating an open loop of edges, it can be added as a spur to an existing loop on a face or as a slit in the face. 

    \param pEntity [in]  Pointer to the entity to be imprinted. 
	The specified entity must one of the following types: OdDbCurve, OdDb3dSolid, OdDbSurface, OdDbBody, OdDbRegion, OdDbTrace, OdDbSolid, or OdDbFace.

    \returns Returns eOk if the intersection operation is successfully performed or an appropriate error code otherwise. 

    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
*/
  virtual OdResult imprintEntity(const OdDbEntityPtr pEntity);
  
  /** \details
    Intersects the specified plane with the surface and creates the intersection in a resulting array. 
 
    \param sectionPlane   [in]  Pointer to a plane object used for the intersection (section cutting plane).
    \param sectionObjects [out] A reference to an entity array for representing resulting sections.
    
    \returns 
    Returns eOk if the intersection operation is successfully performed, or an appropriate error code otherwise. 

    \remarks 
    Resulting entities are not included in the database, so the caller is responsible for the correct handling of the memory pointed to with the result array's items. 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult createSectionObjects(const OdGePlane& sectionPlane, OdDbEntityPtrArray& sectionObjects) const;

  /** \details
    Slices the surface with the specified plane and sets the surface to be the portion on the positive normal side plane. 
    
    \param slicePlane      [in]   Reference to a plane object to be used for slicing the surface.
    \param pNegHalfSurface [out]  Pointer to a new surface representing the negative side of the surface that is sliced by the specified plane.
    \param pNewSurface     [out]  Pointer to a new surface representing the positive half of the surface that is sliced by the specified plane.
    
    \returns Returns eOk if the slicing operation is successfully performed or an appropriate error code otherwise. 
    
    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult sliceByPlane(const OdGePlane& slicePlane,
                                OdDbSurfacePtr& pNegHalfSurface, 
                                OdDbSurfacePtr& pNewSurface);
  /** \details
    Slices the surface with the specified surface. 

    \param pSlicingSurface [in]  Pointer to the surface to be used to slice this surface. 
    \param pNegHalfSurface [out] Pointer to a new surface object representing the other (negative) side of the sliced surface.
    \param pNewSurface     [out] Pointer to a new surface object representing the positive side of the sliced surface.
    
    \returns Returns eOk if the slicing operation is successfully performed or an appropriate error code otherwise. 
    
    \remarks 
    If the specified surface does not intersect this surface and this surface is left unchanged, pNegHalfSurface is set to NULL.
    
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult sliceBySurface(const OdDbSurfacePtr pSlicingSurface, 
                                  OdDbSurfacePtr& pNegHalfSurface,
                                  OdDbSurfacePtr& pNewSurface);

  /** \details
    Creates associative chamfers at the specified edges of the surface. 
    
    \param edgeSubentIds    [in] Object IDs of edges at which the chamfer will be applied. 
    \param baseFaceSubentId [in] Object ID of the base face where the chamfer will be applied.
    \param baseDist         [in] Chamfer distance on the base face.  
    \param otherDist        [in] Chamfer distance on the other faces.  
    
    \returns Returns eOk if creating the chamfer is successful or an appropriate error code otherwise. 
    
    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult chamferEdges(const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdDbSubentId& baseFaceSubentId, 
                                double baseDist, 
                                double otherDist);

  /** \details
    Creates rounded fillets along the specified edges of the surface.
    
    \param edgeSubentIds [in] Object IDs of the edges where the fillet will be applied. 
    \param radius        [in] Array of radius values for the corresponding edges. 
    \param startSetback  [in] Array of starting setback values for the corresponding edges. 
    \param endSetback    [in] Array of ending setback values for the corresponding edge.
    
    \returns Returns eOk if creating the fillet is successful or an appropriate error code otherwise. 
    
    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult filletEdges( const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdGeDoubleArray& radius, 
                                const OdGeDoubleArray& startSetback, 
                                const OdGeDoubleArray& endSetback);

  virtual OdResult setSubentColor(const OdDbSubentId& subentId, const OdCmColor& color);
  virtual OdResult getSubentColor(const OdDbSubentId& subentId, OdCmColor& color) const;
  virtual OdResult setSubentMaterial(const OdDbSubentId& subentId, const OdDbObjectId& matId);
  virtual OdResult getSubentMaterial(const OdDbSubentId& subentId, OdDbObjectId& matId) const;
  virtual OdResult setSubentMaterialMapper(const OdDbSubentId& subentId, const OdGiMapper& mapper);
  virtual OdResult getSubentMaterialMapper(const OdDbSubentId& subentId, OdGiMapper& mapper) const;

  virtual OdResult subGetGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath, 
                                               OdGsMarkerArray& gsMarkers) const;

  virtual OdResult subGetSubentPathsAtGsMarker( OdDb::SubentType type,
                                                OdGsMarker gsMark, 
                                                const OdGePoint3d& pickPoint,
                                                const OdGeMatrix3d& viewXform, 
                                                OdDbFullSubentPathArray& subentPaths, 
                                                const OdDbObjectIdArray* pEntAndInsertStack = 0) const;

  virtual OdResult subGetTransformedCopy(const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;

  /** \details
    Converts the surface to nurb surfaces.
    
    \param nurbSurfaceArray [in] Array of generated nurb surfaces.
    
    \returns Returns eOk if the converting operation was successfully finished or an appropriate error code otherwise. 
        
    \remarks 
    Created nurb surfaces are not included in the database, so the caller must manage the memory occupied by nurb surface objects. 

    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  virtual OdResult convertToNurbSurface(OdDbNurbSurfaceArray& nurbSurfaceArray);
  
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  /** \details
    Trims a surface with specified cutting surfaces (curves). 
    A surface can be trimmed by other surfaces or curves. When curves are used, the actual trimming boundary is formed by projecting 
    the curves onto the surface, so the additional information of project direction needs to be provided.    

    \param blankSurfaceId     [in] Reference to a surface to be trimmed.
    \param toolIds            [in] Reference to an array of the cutting entity's IDs. Bodies of specified entities are used to trim the specified surface.
    \param toolCurveIds       [in] Reference to an array of the cutting curve's IDs. The specified surface will be trimmed by projecting curves to it.
    \param projVectors        [in] Reference to an array of projection direction for each cutting curve.
    \param pickPoint          [in] Pick point that specifies which area of the specified surface must be trimmed.
    \param viewVector         [in] Vector representing the view direction for creating a ray starting from the pick point.
    \param bAutoExtend        [in] If this option is set, then when a tool body consists of a single face with analytic geometry, the underlying geometry will be extended as much as possible to make sure the surface is trimmed. The tool body supplied will not be modified. 
    \param bAssociativeEnabled [in] Specifies whether the surface trimming operation should be associative.  
    
    \returns Returns eOk if the trimming operation was successfully finished or an appropriate error code otherwise. 
    
    \remarks 
    This method is not implemented. 
  */
  static OdResult trimSurface(const OdDbObjectId&      blankSurfaceId, 
                              const OdDbObjectIdArray& toolIds,
                              const OdDbObjectIdArray& toolCurveIds, 
                              const OdGeVector3dArray& projVectors,
                              const OdGePoint3d&       pickPoint, 
                              const OdGeVector3d&      viewVector,
                              bool                     bAutoExtend,
                              bool                     bAssociativeEnabled);

  /** \details
    Creates entities by projecting the specified entity along the projection direction to the surface.
    
    \param pEntityToProject 		[in] Pointer to the entity to be projected. If the entity to project is represented by an OdDbPoint object, an OdDbPoint object, representing the nearest point on the surface, is returned. 
    \param projectionDirection 	[in] Vector specifying the direction of the projection.
    \param projectedEntities 	  [out] Array of pointers to entities created by the projection.
    
    \returns Returns eOk if projecting operation was successfully finished or an appropriate error code otherwise. 
    
    \remarks 
    All entities created as a result of projection are not included in the database, so the caller is responsible for managing the memory occupied with these entities.
	
	This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  OdResult projectOnToSurface(const OdDbEntity*   pEntityToProject,
                              const OdGeVector3d& projectionDirection,
                              OdDbEntityPtrArray& projectedEntities) const;

  /** \details 
    Returns the current perimeter value of the surface.
    
    \param dRegionPerimeter [in] A reference to a double value to store the perimeter. 

    \returns Returns eOk if the perimeter was successfully returned to the calling subroutine or an appropriate error code in the other case.
    
    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  OdResult getPerimeter(double&) const;

  /** \details 
    Creates a ray which starts from a specified point and has a specified direction.

    \param rayBasePoint [in]  A base point to start a ray from.
    \param rayDir,      [in]  A ray direction.
    \param rayRadius,   [in]  A ray radius. It defines the tolerance used for intersection checking.
    \param subEntIds,   [out] A reference to a subentities array to return. Returned subentities can be one of the following: faces, edges, or vertices.
    \param parameters   [out] An array of parameters to return. Parameters shows the intersection between a ray and subentities.

    \returns Returns eOk if a ray was successfully created or an appropriate error code in the other case.

    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */
  OdResult rayTest( const OdGePoint3d&      rayBasePoint, 
                    const OdGeVector3d&     rayDir,
                    double                  rayRadius, 
                    OdArray<OdDbSubentId> & subEntIds, 
                    OdGeDoubleArray&        parameters) const;

  /** \details 
    Creates an offset surface. 
    
    \param pInputSurface    [in]  A pointer to a surface to be offset.
    \param dOffsetDistance  [in]  An offset distance.
    \param offsetSurface    [out] An offset surface to return.
    
    \returns Returns eOk if the offset surface was successfully created or an appropriate error code in the other case.
  
    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */

  static OdResult createOffsetSurface ( OdDbEntity* pInputSurface,
                                        double dOffsetDistance,
                                        OdDbEntityPtr& offsetSurface);

  /** \details 
    Creates an offset surface and includes it in the current database. 
    
    \param pInputSurface        [in]  A pointer to a surface to be offset.
    \param dOffsetDistance      [in]  An offset distance.
    \param bAssociativeEnabled  [in]  An associativity flag value. The associativity flag indicates whether the patch surface associativity is enabled (if true) or not (if false).
    \param offsetSurfaceId      [out] An offset surface identifier to return.
    
    \returns Returns eOk if the offset surface was successfully created or an appropriate error code in the other case.
  
    \remarks 
    This method is implemented only for Spatial modeler and returns eNotImplementedYet status
    for other modelers. 
  */

  static OdResult createOffsetSurface ( OdDbEntity* pInputSurface,
                                        double dOffsetDistance,
                                        bool  bAssociativeEnabled,
                                        OdDbObjectId& offsetSurfaceId);

  /** \details  
    This method is not implemented. 
  */
  static OdResult createFilletSurface ( const OdDbObjectId&  surfId1,
                                        const OdGePoint3d&   pickPt1,
                                        const OdDbObjectId&  surfId2,
                                        const OdGePoint3d&   pickPt2,
                                        double               dRadius,
                                        OdDb::FilletTrimMode trimMode,
                                        const OdGeVector3d&  projDir,
                                        OdDbSurface*&        filletSurface);

  /** \details
    This method is not implemented. 
  */
  static OdResult createFilletSurface ( const OdDbObjectId&  surfId1,
                                        const OdGePoint3d&   pickPt1,
                                        const OdDbObjectId&  surfId2,
                                        const OdGePoint3d&   pickPt2,
                                        double               dRadius,
                                        OdDb::FilletTrimMode trimMode,
                                        const OdGeVector3d&  projDir,
                                        bool                 bAssociativeEnabled,
                                        OdDbObjectId&        filletSurfaceId);

  /**  \details   
    This method is not implemented. 
  */
  static OdResult createExtendSurface ( OdDbObjectId                 sourceSurface, 
                                        const OdArray<OdDbSubentId>& edgesId,  
                                        double                       dExtDist, 
                                        EdgeExtensionType            extOption, 
                                        bool                         bAssociativeEnabled, 
                                        OdDbObjectId&                newExtendSurfaceId );

  /** \details
    This method is not implemented. 
  */
  OdResult extendEdges( OdDbFullSubentPathArray& edgesId,
                        double                   dExtDist, 
                        EdgeExtensionType        extOption, 
                        bool                     bAssociativeEnabled );

  virtual OdResult subGetGeomExtents( OdGeExtents3d& extents) const;

  /** \details
  Called as the first operation of the handOverTo function.

  \param pNewObject [in]  Pointer to the object with which to replace this object in the database.

  \remarks
  This function allows custom classes to populate the new object.

  \remarks
  Overriding this function in a child class allows a child instance to be notified each time an
  object is handed over.

  This function is notified just before an object is to be handed over; giving this function
  the ability to cancel the handover.

  When overriding this function:

  1)  If the OdDbObject's state is incorrect, throw exception.

  2)  Call parent class's subHandover().

  3)  If other actions are required before handover, do them.

  The default implementation of this function does nothing.  This function can be
  overridden in custom classes.
  */
  //For surfces(this custom class) this function restore previous lost attributes
  virtual void subHandOverTo(OdDbObject* pNewObject);
};

#include "TD_PackPop.h"
#endif //_DBSURFACE_H_INCLUDED_
