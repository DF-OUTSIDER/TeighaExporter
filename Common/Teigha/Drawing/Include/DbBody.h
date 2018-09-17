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




#ifndef GEOMENT_DBBODY_H
#define GEOMENT_DBBODY_H

#include "TD_PackPush.h"
#include "ModelerDefs.h"
#include "DbEntity.h"
#include "DbSubentId.h"
#include "DwgDeclareMembers.h"

class OdModelerGeometry;
class OdStreamBuf;
class OdBrBrep;
class OdBrEntity;
class OdBrMesh2dFilter;
class OdBrMesh2d;
namespace GeMesh { class OdGeTrMesh; }
class wrTriangulationParams;

/** \details
  This class represents 3D body entities in an OdDbDatabase instance.

  \sa
  TD_Db
  
  \remarks
  An OdDbBody entity is a wrapper for an ACIS model and cannot be
  represented as an OdDb3dSolid entity or an OdDbRegion entity.
  
  <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbBody : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbBody);

  /** \details
    Default constructor.
  */
  OdDbBody();
  
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
    Writes the specified ACIS solids to the specified file.

    \param filename   [in] Filename to write.
    \param solids     [in] Array of smart pointers to ACIS solids.
    \param typeVer    [in] Type and version of the ACIS data to write.
   
    \returns Returns eOk if successful or an appropriate error code if not. 
  */
  static OdResult acisOut(
    const OdString& filename, 
    const OdDbEntityPtrArray& solids, 
    AfTypeVer typeVer = kAfVer700|kAfTypeASCII);

  /** \details
    Reads the ACIS data for this entity from the specified StreamBuf object.

    \param pStreamBuf [in] Pointer to the StreamBuf object from which the data is to be read.   
    \param pTypeVer   [in] Receives the type and version of the ACIS data read.

    \returns Returns eOk if successful or an appropriate error code if not. 
    
    \remarks
    If pTypeVer is not null, this function returns the version of the ACIS data
    of this 3D solid entity.
  */
  OdResult acisIn(
    OdStreamBuf* pStreamBuf, 
    AfTypeVer *pTypeVer = 0);
    
  /** \details
    Reads an array of ACIS solids from the specified file.

    \param filename   [in] Filename to read.
    \param solids     [out] Receives the array of smart pointers to the ACIS solids.

    \returns Returns eOk if successful or an appropriate error code if not. 
    
    \remarks
    If pTypeVer is not null, this function returns the version of the ACIS data
    of this 3D solid entity.
  */
  static OdResult acisIn(
    const OdString& filename, 
    OdDbEntityPtrArray& solids);

  /** \details
    Returns the boundary representation of the 3D solid entity.
    
    \param brep [out] Receives the boundary representation.
  */
  void brep(
    OdBrBrep& brep);

  bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  /** \details
    Returns true if the entity has no view-dependent graphics. In other case returns false.
    
    \param pWd [in] Pointer to the OdGiWorldDraw object.
  */
  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  /** \details
    Computes the entity to produce the right appearance given each viewport's possibly 
    different viewpoint of the entity. Draws an entity in layout viewports. 
    
    \param pVd [in] Pointer to the OdGiViewportDraw object. 
  */
  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const;

  /** \details
    Determines the behavior for custom objects when saving to a .dwg or .dxf file. 
    
    \param ver [in] Drawing version to save as.
    \param replaceId [out] Object ID of the object replacing this object.
    \param exchangeXData [out] Set to true if and only if this function did not add XData to the replacement object.

    \remarks
    This function either
    * Returns an OdDbObjectPtr for a non-database-resident (NDBRO) replacement object, setting replaceId to OdDbObjectId::kNull.
    * Returns NULL, setting replaceId for a database-resident (DBRO) replacement object.
    
    Custom objects can decompose themselves into other objects, adding additional XData as required. Teigha 
    transfers XData from this object to the replacement object if and only if exchangeXData is true.
    
    The default implementation returns NULL and sets replaceId to OdDbObjectId::kNull. This function can be
    overridden in custom classes.

    The method is not intended to be called by client code. For processing, owned objects use the *convertForSave()*
    method.
  */
  virtual OdDbObjectPtr decomposeForSave(
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData);
  
  
  /** \details
  Directly sets the contained modeler geometry of this entity.
  
  \param pGeometry [in] Pointer to the modeler geometry.
  
  \returns Returns eOk if new model geometry was successfully set or an appropriate error in the other case.
  */
  virtual OdResult setBody( const void* pGeometry );
  
  /** \details 
    Returns the current modeler geometry. 
  */
  virtual void *body( ) const;

  /** \details
    Explodes this entity into a set of simpler entities.  

    \param entitySet [in/out] Receives an array of pointers to the new entities.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.

    \remarks
    Entities resulting from the explosion are appended to the specified array.
    
    The newly created entities are not database residents.
    
    The default implementation of this function returns eNotApplicable. This function can be
    overridden in custom classes.
  */
  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  /** \details
    Applies the specified 3D transformation matrix to this entity.

    \param xfm [in] 3D transformation matrix.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult subTransformBy( 
    const OdGeMatrix3d& xfn );

  /** \details
    Creates a geometric representation of the entity for saving proxy graphics or converting to previous formats.

    \param pWd [in] Pointer to the OdGiWorldDraw object.
    \param ver [in] Specifies the reason why the method is called.

    \remarks
    This method can be overridden in custom classes.
  */
  virtual void saveAs(
    OdGiWorldDraw* pWd,
    OdDb::DwgVersion ver ) const;

    
  /** \details
    Creates a copy of the subentity. 
    
    \param id [in] Subentity that should be copied.
    
    \returns If successful, returns a pointer to the resulting copy.  Otherwise returns null.
  */
  virtual OdDbEntityPtr subSubentPtr(const OdDbFullSubentPath& id) const;

  /** \details
    Returns the array of the graphics system marker(s) (GS marker(s)) associated
    with primitive(s) that is used for drawing the subentity. 

    \param subPath [in] Path to the subentity (or subentities) for which the GS marker(s) is requested.
    \param gsMarkers [out] Array of the GS marker(s).   

    \returns
    Returns eOk if successful, or an appropriate error code if not.  
  */
  virtual OdResult subGetGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath, OdGsMarkerArray& gsMarkers) const;

  /** \details
    Returns the array of OdDbFullSubentPath object(s) by the specified GS marker.
    Each OdDbFullSubentPath object represents a subentity.

    \param type [in] Subentity type.
    \param gsMark [in] GS marker representing a subentity.   
    \param pickPoint [in] Point (in WCS coordinates) picked during the entity selection.   
    \param viewXform [in] Transformation matrix to transform from WCS to DCS. 
    \param subentPaths [out] Array of OdDbFullSubentPath object(s) generated from the GS marker. 
    \param pEntAndInsertStack [in] Array of objectIds.

    \returns
    Returns eOk if successful, or an appropriate error code if not.  
  */
  virtual OdResult subGetSubentPathsAtGsMarker(OdDb::SubentType type,
                                               OdGsMarker gsMark, 
                                               const OdGePoint3d& pickPoint,
                                               const OdGeMatrix3d& viewXform, 
                                               OdDbFullSubentPathArray& subentPaths, 
                                               const OdDbObjectIdArray* pEntAndInsertStack ) const;
  /** \details
    Returns the number of changes occurred since the body's creation.
  */
  virtual OdUInt32 numChanges() const;

  /** \details
    For internal use only.

    \param ent [in] ACIS object pointer. 
  */
  virtual OdDbSubentId internalSubentId(void* ent) const;
  
  /** \details
    For internal use only.

    \param id [out] Subentity ID in the ACIS object. 
  */
  virtual void* internalSubentPtr(const OdDbSubentId& id) const;
  
  /** \details
    Sets the values of this object's subentity traits, and returns with the calling object's subentity traits.

    \param pTraits [in] Pointer to the OdGiDrawableTraits object to be set.

    \remarks
    When overriding subSetAttributes(), you must OR (|) the return value of ::subSetAttributes(pTraits) 
    with any flags you add. A derived class may not remove flags for any reason. The default 
    implementation does nothing but return kDrawableNone. This function can be overridden in custom classes.
  */
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  /** \details
    Returns WCS coordinates of box's corner points. The box encloses the 3D extents of the entity.

    \param extents [out] Entity's extents.

    \returns
    Returns eOk if successful, or an appropriate error code if not.  
  */
  virtual OdResult subGetGeomExtents( OdGeExtents3d& extents) const;

  /** \details
    For internal usage only.
  */
  static OdResult setSubentPath(OdBrEntity &ent, OdDbFullSubentPath& subpath);

  /** \details
    Creates the 2D mesh control, associated with a topology object.

    \param pEnt [in] Topology object.
    \param filter [in] Filter for controlling the mesh generation.
    \param mesh2d [out] 2D mesh control object.

    \returns
    Returns eOk if successful, or an appropriate error code if not.  
  */
  static OdResult generateMesh(const OdDbEntity *pEnt, const OdBrMesh2dFilter & filter, OdBrMesh2d &mesh2d);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbBody object pointers.
*/
typedef OdSmartPtr<OdDbBody> OdDbBodyPtr;

#include "TD_PackPop.h"

#endif

