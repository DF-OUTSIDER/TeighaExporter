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

#ifndef _ODDBENTITY_INCLUDED_
#define _ODDBENTITY_INCLUDED_ /* { Secret} */

#include "TD_PackPush.h"

#include "DbObject.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeLine3d.h"
#include "Ge/GePlane.h"
#include "CmColor.h"
#include "IntArray.h"
#include "DbDatabase.h"
#include "DbSubentId.h"
#include "GsMarkerArray.h"
#include "DbGrip.h"
#include "OdaDefs.h"

class OdGePlane;
class OdGeMatrix3d;
class OdDbFullSubentPath;
class OdGePoint3d;
class OdGeVector3d;
class OdDbBlockTableRecord;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbEntity pointers.
*/
typedef OdSmartPtr<OdDbEntity> OdDbEntityPtr;

/** \details
  This template class is a specialization of the OdArray class for OdDbEntity object SmartPointers.
*/
typedef OdArray<OdDbEntityPtr> OdDbEntityPtrArray;

/** \details

    <group TD_Namespaces>
*/
namespace OdDb
{

  enum EntSaveAsType
  {
    kNoSave = 0,
    kSaveAsR12,
    kSaveAsR13,
    kSaveAsR14
  };

  enum Planarity
  {
    kNonPlanar = 0,
    kPlanar    = 1,
    kLinear    = 2
  };
}

/** \details
    This class is the base class for all graphical objects contained in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbEntity : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbEntity);

  OdDbEntity();

  /** \details
    Returns the Object ID of the OdDbBlockTableRecord that owns this entity.
  */
  OdDbObjectId blockId() const;

  /** \details
    Returns the color information of this entity as an OdCmColor instance.
  */
  OdCmColor color() const;

  /** \details
    Sets the color information of this entity from an OdCmColor instance.

    \param color [in]  OdCmColor object.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                  this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setColor(
    const OdCmColor &color,
    bool doSubents = true);

  /** \details
    Returns the color index of this entity (DXF 62).

    \returns
    Returns a value in the range [0 .. 256].

    * 0 indicates a color of BYBLOCK.
    * 256 indicates a color of BYLAYER.
  */
  OdUInt16 colorIndex() const;

  /** \details
    Returns the OdCmEntityColor settings of this object.
  */
  virtual OdCmEntityColor entityColor() const;

  /** \details
    Sets the color index of this entity (DXF 62).

    \param colorIndex [in]  Color index.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                  this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.

    colorIndex will be in the range [0 .. 256].
    * 0 indicates a color of BYBLOCK.
    * 256 indicates a color of BYLAYER.
  */
  virtual OdResult setColorIndex(
    OdUInt16 colorIndex,
    bool doSubents = true);

  /** \details
      Returns the Object ID of the OdDbColor object referenced by this entity.
  */
  OdDbObjectId colorId() const;

  /** \details
    Assigns the specified OdDbColor object to this entity.

    \param colorId [in]  Object ID of the OdDbColor object.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                  this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setColorId(
    OdDbObjectId colorId,
    bool doSubents = true);

  /** \details
    Returns the transparency setting of this entity.
  */
  OdCmTransparency transparency() const;

  /** \details
    Sets the transparency setting of this entity, and returns eOk if successful.

    \param transparency [in]  OdCmTransparency object be assigned.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                  this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.

    If you override this function, you must call OdDbEntity::setTransparency(), passing to it the calling arguments.
  */
  virtual OdResult setTransparency(
    const OdCmTransparency& transparency,
    bool doSubents = true);

  /** \details
    Returns the name of the plotStyleName string associated with this entity (DXF 390).
  */
  OdString plotStyleName() const;

  /** \details
    Returns the PlotStyleName of this entity.

    \param plotStyleNameId [out]  Receives the Object ID of the Plot Style Name of this entity.
  */
  OdDb::PlotStyleNameType getPlotStyleNameId(OdDbObjectId& plotStyleNameId) const;

  /** \details
    Sets the plot style of this entity.

    \param plotStyleName [in]  Name of the plot style.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setPlotStyleName(
    const OdString& plotStyleName,
    bool doSubents = true);

  /** \details
    Sets the plot style of this entity.
    
    \param plotStyleNameType [in]  Plot Style Name Type.
    \param plotStyleNameId [in]  Object ID of the plot style name.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by this entity.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.

    plotStyleNameId is used only when plotStyleNameType == kPlotStyleNameById.

    plotStyleNameType must be one of the following:

    <table>
    Name                                 Value
    OdDb::kPlotStyleNameByLayer          0
    OdDb::kPlotStyleNameByBlock          1
    OdDb::kPlotStyleNameIsDictDefault    2
    OdDb::kPlotStyleNameById             3
    </table>
  */
  virtual OdResult setPlotStyleName(
    OdDb::PlotStyleNameType plotStyleNameType,
    OdDbObjectId plotStyleNameId = OdDbObjectId::kNull,
    bool doSubents = true);

  /** \details
    Returns the name of the layer referenced by this entity (DXF 8).
  */
  OdString layer() const;

  /** \details
    Returns the Object ID of the OdDbLayerTableRecord referenced by this entity.
  */
  OdDbObjectId layerId() const;

  /** \details
    Sets the layer to be referenced by this entity (DXF 8).

    \param layerName [in]  Name of the layer.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setLayer(
    const OdString& layerName,
    bool doSubents = true,
    bool allowHiddenLayer = false);

  /** \details
    Sets the layer to be referenced by this entity (DXF 8).

    \param layerId [in]  Object ID of the LayerTableRecord.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setLayer(
    OdDbObjectId layerId,
     bool doSubents = true,
     bool allowHiddenLayer = false);


  /** \details
    Returns the name of the linetype referenced by this entity (DXF 6).
  */
  OdString linetype() const;

  /** \details
    Returns the Object ID of the linetype referenced by this entity (DXF 6).
  */
  OdDbObjectId linetypeId() const;

  /** \details
    Sets the linetype to be referenced by this entity (DXF 6).

    \param linetypeName [in]  Name of the linetype.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setLinetype(
    const OdString& linetypeName,
    bool doSubents = true);
  
  /** \details
    Sets the linetype to be referenced by this entity (DXF 6).

    \param linetypeID [in]  Object ID of the linetype.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setLinetype(
    OdDbObjectId linetypeID,
    bool doSubents = true);

  /** \details
    Returns the name of the material referenced by this entity (DXF 347).
  */
  OdString material() const;

  /** \details
    Returns the Object ID of the material referenced by this entity (DXF 347).
  */
  OdDbObjectId materialId() const;

  /** \details
    Sets the material to be referenced by this entity (DXF 347).

    \param materialName [in]  Name of the material.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setMaterial(
    const OdString& materialName,
    bool doSubents = true);
  
  /** \details
    Sets the material to be referenced by this entity (DXF 347).

    \param materialID [in]  Object ID of the material.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setMaterial(
    OdDbObjectId materialID,
    bool doSubents = true);


  enum VisualStyleType
  {
    kFullVisualStyle,
    kFaceVisualStyle,
    kEdgeVisualStyle
  };

  /** \details
    Returns the Object ID of the *visual style* used by this entity.
  */
  OdDbObjectId visualStyleId(
    VisualStyleType vstype = kFullVisualStyle) const;

  /** \details
    Sets the *visual style* to be used by this entity.

    \param visualStyleId [in]  Object ID of the *visual style*.
    \param vstype [in]  Type of the *visual style*.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setVisualStyle(
    OdDbObjectId visualStyleId,
    VisualStyleType vstype = kFullVisualStyle,
    bool doSubents = true);

  /** \details
    Returns OdGiMapper used to map a material to the entity.
    Returns null if the default mapper is used.
  */
  virtual const OdGiMapper* materialMapper() const;

  /** \details
    Sets the mapper used to map a material to the entity.

    \param mapper [in]  The mapper of a material.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                this entity.

    \remarks
    Use null to apply the default mapper.
  */
  virtual void setMaterialMapper(
    const OdGiMapper* mapper,
    bool doSubents = true);

  /** \details
    Returns the linetype scale of this entity (DXF 48).
  */
  double linetypeScale() const;

  /** \details
    Sets the Linetype scale of this entity (DXF 48).

    \param linetypeScale [in]  Linetype scale factor.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setLinetypeScale(
    double linetypeScale,
    bool doSubents = true);

  /** \details
    Returns the visibility status of this entity. It takes into account both permanent (DXF 60)
    and temporary visibility statuses: the entity is visible only if it's visible in both parts.

    \remarks
    visibility() returns one of the following:

    <table>
    Name                 Value
    OdDb::kInvisible     1
    OdDb::kVisible       0
    </table>
  */
  OdDb::Visibility visibility() const;

  /** \details
    Sets the visibility status of this entity (DXF 60).

    \param visibility [in]  Visibility status.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.

    visibility must be one of the following:

    <table>
    Name                 Value
    OdDb::kInvisible     1
    OdDb::kVisible       0
    </table>
  */
  ODRX_SEALED_VIRTUAL OdResult setVisibility(
    OdDb::Visibility visibility,
    bool doSubents = true) ODRX_SEALED;

  /** \details
    Returns the temporary (not saved) visibility status of this entity.

    \remarks
    tempVisibility() returns one of the following:

    <table>
    Name                 Value
    OdDb::kInvisible     1
    OdDb::kVisible       0
    </table>
  */
  OdDb::Visibility tempVisibility() const;

  /** \details
    Sets the temporary (not saved to file) visibility status of this entity.

    \param visibility [in]  Visibility status, must be one of the following:
    <table>
    Name                 Value
    OdDb::kInvisible     1
    OdDb::kVisible       0
    </table>
  */
  void setTempVisibility(OdDb::Visibility visibility);

  /** \details
    Returns the lineweight property of this entity (DXF 370).
  */
  OdDb::LineWeight lineWeight() const;

  /** \details
    Sets the lineweight property of this entity (DXF 370).

    \param lineWeight [in]  Lineweight.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setLineWeight(
    OdDb::LineWeight lineWeight,
    bool doSubents = true);

  /** \details
    Returns the CastShadows flag for this entity..
  */
  virtual bool                castShadows() const;

  /** \details
    Controls the CastShadows flag for this entity.
    \param castShadows [in]  CastShadows flag.
  */
  virtual void                setCastShadows(bool castShadows);

  /** \details
    Returns the ReceiveShadows flag for this entity..
  */
  virtual bool                receiveShadows() const;

  /** \details
    Controls the Receivehadows flag for this entity.
    \param receiveShadows [in]  ReceiveShadows flag.
  */
  virtual void                setReceiveShadows(bool receiveShadows);

  /** \details
    Returns the CollisionType for this entity.
    \remarks
    Subclasses may override this method to customize how the class interacts
    in collision-aware features.
  */
  virtual OdDb::CollisionType collisionType() const;

 /** \details
    Copies the properties from the specified entity to this entity.
    \param pSource [in]  Pointer to the source entity.
    \param doSubents [in]  If and only if true, applies the change to all sub-entities owned by
                this entity.
 */
  void setPropertiesFrom(
    const OdDbEntity* pSource,
    bool doSubents = true);

  /** \details
    Returns true if and only if this entity is planar.
  */
  virtual bool isPlanar() const;

  /** \details
    Returns the plane that contains this entity.

    \param plane [out]   Receives the plane that contains this entity.
    \param planarity [out]  Receives the planarity of this entity.

    \returns
    Returns eOk if successful, or an appropriate error code if not.

    planarity and plane return values as follows::

    <table>
    planarity     Value    Description    plane
    kNonPlanar    0        Non-planar     Not set
    kPlanar       1        Planar         Entity plane
    kLinear       2        Linear         Arrbitrary plane containing this entity
    </table>
  */
  virtual OdResult getPlane(
    OdGePlane& plane,
    OdDb::Planarity& planarity) const;

  /** \details
    Returns the WCS geometric extents of this entity.

    \param extents [out]  Receives the extents.

    \returns
    Returns eOk if successful, or eInvalidExtents if not.

    The extents are the WCS corner points of a box, aligned with the
    WCS axes, that encloses the 3D extents of this entity.
  */

  void subHandOverTo(
    OdDbObject* pNewObject);

  /** \details
    Applies the specified 3D transformation matrix to this entity.

    \param xfm [in]  3D transformation matrix.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  ODRX_SEALED_VIRTUAL OdResult transformBy(
    const OdGeMatrix3d& xfm) ODRX_SEALED;

  /** \details
    Creates a copy of this entity, and applies the supplied transformation
    to the newly created copy.

    \param xfm [in]  3D transformation matrix.
    \param pCopy [out]  Receives a SmartPointer to the newly created copy.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  ODRX_SEALED_VIRTUAL OdResult getTransformedCopy(
    const OdGeMatrix3d& xfm,
    OdDbEntityPtr& pCopy) const ODRX_SEALED;

  /** \details
    Explodes this entity into a set of simpler entities.

    \param entitySet [in/out] Receives an array of pointers to the new entities.

    \returns
    Returns eOk if successful, or an appropriate error code if not.

    \remarks
    Entities resulting from the explosion are appended to the specified array.
    The newly created entities are not database resident.
    The default implementation of this function returns eNotApplicable. This function can be
    overridden in custom classes.
  */
  ODRX_SEALED_VIRTUAL OdResult explode(
    OdRxObjectPtrArray& entitySet) const ODRX_SEALED;

  /** \details
    Explodes this entity into a set of simpler entities, and adds them to the specified block table record.

    \param pBlockRecord [in]  Pointer to the Block Table Record.
    \param ids [in/out] Receives an array of Object IDs of the new entities.

    \remarks
    Entities resulting from the explosion are appended to the specified array.

    The newly created entities are database resident or not depending on the block table record
    they are appended to. If block table record is database resident the entities are database
    resident as well. If block table record is not database resident newly created entities are not
    database resident.

    The default implementation of this function returns eNotApplicable. This function can be
    overridden in custom classes.
  */
  virtual OdResult explodeToBlock(
    OdDbBlockTableRecord *pBlockRecord,
    OdDbObjectIdArray *ids = 0);

  /** \details
    Explodes this entity into a set of simpler entities.

    \param entitySet [in/out] Receives an array of pointers to the new entities.

    \remarks
    The newly created entities will be not database resident. Entities resulting from the explosion are appended to the specified array.
    The default implementation of this function calls subWorldDraw() and makes
    entities from geometry generated by subWorldDraw(). This function can be
    overridden in custom classes.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult explodeGeometry(
    OdRxObjectPtrArray& entitySet) const;

  /** \details
    Explodes this entity into a set of simpler entities, and adds them to the specified block.

    \param pBlockRecord [in]  Pointer to the Block Table Record.
    \param ids [in/out] Receives array of Object IDs of the new entities.

    \remarks
    The newly created entities are database resident or not depending on the block table record
    they are appended to. If block table record is database resident the entities are database
    resident too. If block table record is not database resident newly created entities are not
    database resident.
    Entities resulting from the explosion are appended to the specified array.
    The default implementation of this function calls subWorldDraw() and makes
    entities from geometry generated by subWorldDraw(). This function can be
    overridden in custom classes.
  */
  virtual OdResult explodeGeometryToBlock(
    OdDbBlockTableRecord *pBlockRecord,
    OdDbObjectIdArray *ids = 0);

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;

  /** \details
    Applies the default properties of the specified database to this entity.

    \param pDb [in]  Pointer to the database whose default values are to be used.
    \param doSubents [in]  Set to true if subentities should be processed

    \remarks
    If pDb is null, the database containing this entity is used.

    The following properties are set
    * color
    * layer
    * linetype
    * linetype scale
    * lineweight
    * plotstyle
    * visibility
  */
  void setDatabaseDefaults(
    OdDbDatabase* pDb = 0,
    bool doSubents = false);

  /** \details
    Called by setDatabaseDefaults() after the values are set.

    \param pDb [in]  Pointer to the database whose default values are to be used.
    \param doSubents [in]  Set to true if subentities should be processed

    \remarks
    If pDb is null, the database containing this entity is used.

    This function allows custom classes to inspect and modify the values set by setDatabaseDefaults.

    The default implementation of this function returns eNotApplicable. This function can be
    overridden in custom classes.

    \remarks
    This function is not intended to be called by the user.
  */
  virtual void subSetDatabaseDefaults(
    OdDbDatabase* pDb,
    bool doSubents);

  virtual void applyPartialUndo(
    OdDbDwgFiler* pUndoFiler,
    OdRxClass* pClassObj);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  void appendToOwner(
    OdDbIdPair& idPair,
    OdDbObject* pOwnerObject,
    OdDbIdMapping& ownerIdMap);

  virtual OdResult dxfIn(
    OdDbDxfFiler* pFiler);

  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  virtual OdGiDrawable* drawable();

  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const;

    virtual void subList() const;
    virtual OdResult subGetClassID(void* pClsid) const;

  /** \details
    This function is called as the first operation of the swapIdWith() function.

    \param otherId [in]  Object ID of object with which to swap.
    \param swapXdata [in]  XData will be swaped if and only if swapXData is true.
    \param swapExtDict [in]  Extension dictionaries will be swapped if and only if swapExtDict is true.

    \remarks
    This function allows derived classes
    to implement custom behavior during the swapIdWith operation.

    The default implementation of this function does nothing. This function can be
    overridden in custom classes.



    \sa
    OdDbObject::swapIdWith
  */
  void subSwapIdWith(
    const OdDbObjectId& otherId,
    bool swapXdata = false,
    bool swapExtDict = false);

   virtual OdResult subErase( bool erasing) ODRX_OVERRIDE;

   virtual OdResult subOpen(OdDb::OpenMode mode) ODRX_OVERRIDE;

  /** \details
    Sets the bit flag indicating the entity's geometry is changed.

    \param graphicsModified [in]  New value.

    \remarks
    If true, assures that modifiedGraphics() will be called as the entity is being closed, even
    if the object was not opened for write.
  */
  void recordGraphicsModified(bool graphicsModified = true);

  virtual void copyFrom(const OdRxObject* pSource);

/* No implementation
  Cause this entity, and any other entity who's draw bit is set, to be be drawn.
    void draw();
  */

  /** \details
    This method is triggered by the standard LIST command and is to be display
    the dxf format contents of the entity to the display.
  */
  ODRX_SEALED_VIRTUAL void list() const ODRX_SEALED;

  virtual OdResult getGeomExtents(OdGeExtents3d& extents) const ODRX_SEALED;

  ODRX_SEALED_VIRTUAL void highlight(bool bDoIt = true, const OdDbFullSubentPath* pSubId = 0,
    bool highlightAll = false) const ODRX_SEALED;

  /** \details
    Returns all appropriate object snap points of this entity.

    \param osnapMode [in]  The object snap mode being queried.
    \param gsSelectionMark [in]  The GS marker of the subentity being queried.
    \param pickPoint [in]  The WCS point being queried.
    \param lastPoint [in]  The WCS point picked before pickPoint.
    \param xWorldToEye [in]  The WCS->DCS transformation matrix.
    \param snapPoints [in/out] Receives an array of UCS object snap points.

    \remarks
    Object snap points are appended to the specified array.

    osnapMode must be one of the following:

    <table>
    Name                      Value   Description
    OdDb::kOsModeEnd          1       Endpoint
    OdDb::kOsModeMid          2       Midpoint
    OdDb::kOsModeCen          3       Center
    OdDb::kOsModeNode         4       Node
    OdDb::kOsModeQuad         5       Quadrant
    OdDb::kOsModeIntersec     6       Intersection
    OdDb::kOsModeIns          7       Insertion point
    OdDb::kOsModePerp         8       Perpendicular
    OdDb::kOsModeTan          9       Tangent
    OdDb::kOsModeNear         10      Nearest
    OdDb::kOsModeApint        11      Apparent intersection
    OdDb::kOsModePar          12      Parallel
    OdDb::kOsModeStart        13      Unknown
    </table>
  */
  ODRX_SEALED_VIRTUAL OdResult getOsnapPoints(
    OdDb::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints ) const ODRX_SEALED;

  /** \details
    Returns all appropriate object snap points of this entity.

    \param osnapMode [in]  The object snap mode being queried.
    \param gsSelectionMark [in]  The GS marker of the subentity being queried.
    \param pickPoint [in]  The WCS point being queried.
    \param lastPoint [in]  The WCS point picked before pickPoint.
    \param xWorldToEye [in]  The WCS->DCS transformation matrix.
    \param snapPoints [in/out] Receives an array of UCS object snap points.

    \remarks
    Object snap points are appended to the specified array.

    osnapMode must be one of the following:

    <table>
    Name                      Value   Description
    OdDb::kOsModeEnd          1       Endpoint
    OdDb::kOsModeMid          2       Midpoint
    OdDb::kOsModeCen          3       Center
    OdDb::kOsModeNode         4       Node
    OdDb::kOsModeQuad         5       Quadrant
    OdDb::kOsModeIntersec     6       Intersection
    OdDb::kOsModeIns          7       Insertion point
    OdDb::kOsModePerp         8       Perpendicular
    OdDb::kOsModeTan          9       Tangent
    OdDb::kOsModeNear         10      Nearest
    OdDb::kOsModeApint        11      Apparent intersection
    OdDb::kOsModePar          12      Parallel
    OdDb::kOsModeStart        13      Unknown
    </table>
  */
  ODRX_SEALED_VIRTUAL OdResult getOsnapPoints(
    OdDb::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints,
    const OdGeMatrix3d& insertionMat) const ODRX_SEALED;
  
  ODRX_SEALED_VIRTUAL bool isContentSnappable() const ODRX_SEALED;

  /** \details
    Returns all grip points of this entity.

    \param gripPoints [in/out] Receives an array of WCS grip points.

    \remarks
    Grip points are appended to the specified array.
  */
  ODRX_SEALED_VIRTUAL OdResult getGripPoints( OdGePoint3dArray& gripPoints ) const ODRX_SEALED;

  /** \details
    Moves the specified grip points of this entity.

    \param indices [in]  Array of indicies.
    \param offset [in] The direction and magnitude of the grip points offset (WCS).

    \remarks
    Each element in gripPoints has a corresponding entry in indices, which specifies the index of
    the grip point as returned by getGripPoints.
  */
  ODRX_SEALED_VIRTUAL OdResult moveGripPointsAt(
    const OdIntArray& indices,
    const OdGeVector3d& offset ) ODRX_SEALED;

  /** \details
    Returns OdDbGripData objects for grip points of this entity.

    \param grips [in/out] Receives an array of OdDbGripData objects.

    \remarks
    Grip points are appended to the specified array.
  */
  ODRX_SEALED_VIRTUAL OdResult getGripPoints(
    OdDbGripDataPtrArray& grips,
    const double curViewUnitSize,
    const int gripSize,
    const OdGeVector3d& curViewDir,
    const int bitFlags ) const ODRX_SEALED;

  /** \details
    Moves the specified grip points of this entity.

    \param gripAppData [in]  Array of appData pointers from the OdDbGripData objects (one for each grip point to be moved).
    \param offset [in] The direction and magnitude of the grip points offset (WCS).
  */
  ODRX_SEALED_VIRTUAL OdResult moveGripPointsAt(
    const OdDbVoidPtrArray& grips,
    const OdGeVector3d& offset,
    int bitFlags ) ODRX_SEALED;

  /** \details
    Returns all stretch points of this entity.

    \param stretchPoints [in/out] Receives an array of WCS stretch points.

    \remarks
    Stretch points are appended to the specified array.
  */
  ODRX_SEALED_VIRTUAL OdResult getStretchPoints( OdGePoint3dArray& stretchPoints ) const ODRX_SEALED;

  /** \details
    Moves the specified stretch points of this entity.

    \param stretchPoints [in]  Array of moved grip points.
    \param indices [in]  Array of indicies.
    \param offset [in] The direction and magnitude of the stretch points offset (WCS).

    \remarks
    Each element in stretchPoints has a corresponding entry in indices, which specifies the index of
    the stretch point as returned by getStretchPoints.
  */
  ODRX_SEALED_VIRTUAL OdResult moveStretchPointsAt(
    const OdIntArray& indices,
    const OdGeVector3d& offset ) ODRX_SEALED;

  /** \details
    Notification function called when this entity is involved in a drag operation.

    \param status [in]  Status of the drag operation.
  */
  virtual void dragStatus( const OdDb::DragStat status );

  /** \details
    Notification function called when this entity is involved in a grip operation.

    \param status [in]  Status of the grip operation.
  */
  ODRX_SEALED_VIRTUAL void gripStatus( const OdDb::GripStat status ) ODRX_SEALED;

  /** \details
    Returns the cloneMeForDragging flag for this entity.
  */
  ODRX_SEALED_VIRTUAL bool cloneMeForDragging() ODRX_SEALED;

  /** \details
    Returns the hideMeForDragging flag for this entity.
  */
  ODRX_SEALED_VIRTUAL bool hideMeForDragging() const ODRX_SEALED;

  /** \details
    Creates a geometric representation of this entity for saving proxy graphics or converting to previous formats.

    \param pWd [in]  Pointer to the OdGiWorldDraw object.
    \param ver [in]  Specifies the reason why the method is called.

    \remarks
    The default implementation of this function calls worldDraw().
    This function can be overridden in custom classes.
  */
  virtual void saveAs(
    OdGiWorldDraw* pWd,
    OdDb::DwgVersion ver ) const;



  /** \details
    Is applicable for compound objects only which return the kDrawableIsCompoundObject flag
    from subSetAttributes(). The matrix returned is the one that
    is passed to pushModelTransform before rendering nested objects.
    Default implementation returns OdDb::eNotApplicable and the identity matrix in xM.
  */
  ODRX_SEALED_VIRTUAL OdResult getCompoundObjectTransform(OdGeMatrix3d & xM) const ODRX_SEALED;

  /** \details
    Intersects this entity with other entity if possible.

    \param pEnt   [in] Pointer to other entity
    \param intType [in] Determines type of intersection to be calculated
  */
  ODRX_SEALED_VIRTUAL OdResult intersectWith(const OdDbEntity* pEnt, OdDb::Intersect intType,
                             OdGePoint3dArray& points, OdGsMarker thisGsMarker = 0,
                             OdGsMarker otherGsMarker = 0) const ODRX_SEALED;

  /** \details
    Intersects this entity with other entity if possible.

    \param pEnt   [in] Pointer to other entity
    \param intType [in] Determines type of intersection to be calculated
  */
  ODRX_SEALED_VIRTUAL OdResult intersectWith(const OdDbEntity* pEnt, OdDb::Intersect intType,
                             const OdGePlane& projPlane, OdGePoint3dArray& points,
                             OdGsMarker thisGsMarker = 0, OdGsMarker otherGsMarker = 0) const ODRX_SEALED;

  /** \details
    Uses the bounding box of this object to determine an intersection array of points.
  */
  OdResult boundingBoxIntersectWith(const OdDbEntity* pEnt, OdDb::Intersect intType,
                                OdGePoint3dArray& points, OdGsMarker thisGsMarker, OdGsMarker otherGsMarker) const;

  /** \details
    Uses the bounding box of this object to determine an intersection array of points.
  */
  OdResult boundingBoxIntersectWith(const OdDbEntity* pEnt, OdDb::Intersect intType,
                                const OdGePlane& projPlane, OdGePoint3dArray& points,
                                OdGsMarker thisGsMarker, OdGsMarker otherGsMarker) const;

  /** \details
	Returns all subentity paths by graphic system marker for complex entities, that has
	the ability to associate an integer identifier (GS marker) with each graphic primitive or 
	with groups of primitives during draw themself.

	\param type [in]  The subentity type being queried.
	\param gsMarker [in]  The GS marker of the subentity being queried.
	\param pickPoint [in]  The WCS point being queried.
	\param xrm [in]  The WCS->DCS transformation matrix.
	\param subentPaths [out] The array of OdDbFullSubentPath objects.
	\param pEntAndInsertStack [in] The array of objectIds of the objects that are the nested containers of the subentity.

	\remarks
	Subentity type must be one of the following:

	<table>
    Name                      Value   Description
    OdDb::kFaceSubentType     1		  Face subentity
    OdDb::kEdgeSubentType     2		  Edge subentity
    OdDb::kVertexSubentType   3		  Vertex subentity
    OdDb::kMlineSubentCache   4		  Not used yet
	OdDb::kClassSubentType    5		  MultiType subentity
	</table>
  */

  ODRX_SEALED_VIRTUAL OdResult getSubentPathsAtGsMarker(OdDb::SubentType type, OdGsMarker gsMark, 
										const OdGePoint3d& pickPoint,const OdGeMatrix3d& xfm, 
										OdDbFullSubentPathArray& subentPaths, 
										const OdDbObjectIdArray* pEntAndInsertStack = 0) const ODRX_SEALED;

  /** \details
	Returns all graphic system marker by subentity paths for complex entities, that has
	the ability to associate an integer identifier (GS marker) with each graphic primitive or 
	with groups of primitives during draw themself.

	\param subPath [in]  The OdDbFullSubentPath to the subentity.
	\param gsMarkers [in/out] The array of integer identifiers (GS markers).
  */

  ODRX_SEALED_VIRTUAL OdResult getGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
    OdGsMarkerArray& gsMarkers) const ODRX_SEALED;

  /** \details
	Gets the grip points and supporting information about the grips for a subentity.

	\param path [in]  The OdDbFullSubentPath to the subentity.
	\param grips [in/out] Receives an array of pointers of OdDbGripData objects, one object for each grip point in the subentity.
	\param curViewUnitSize [in] The size (in pixels) of one drawing unit in the current viewport.
	\param gripSize [in] The current grip size (in pixels).
	\param curViewDir [in] The view direction in the current viewport.
	\param bitflags [in] The bitmap of one or more flags specified by the GetGripPointsFlags enumeration.
  */

  ODRX_SEALED_VIRTUAL OdResult getGripPointsAtSubentPath( const OdDbFullSubentPath& path, OdDbGripDataPtrArray& grips,
											  const double curViewUnitSize, const int gripSize,
                        const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const ODRX_SEALED;

  /** \details
	Applies a vector offset to one or more grip points exposed by subentities on the object.

	\param paths [in]  The array of OdDbFullSubentPath objects.
	\param gripAppData [in]  The array of pointers of OdDbGripData objects.
	\param offset [in]  The vector (in WCS coordinates) indicating the direction and magnitude of the translation.
	\param bitflags [in]  The bitmap of one or more flags specified by the GetGripPointsFlags enumeration.
  */

  ODRX_SEALED_VIRTUAL OdResult moveGripPointsAtSubentPaths( const OdDbFullSubentPathArray& paths,
    const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, const OdUInt32 bitflags) ODRX_SEALED;

  /** \details
	Deletes one or more subentities from an entity.

	\param paths [in]  The array of OdDbFullSubentPath objects.
  */

  ODRX_SEALED_VIRTUAL OdResult deleteSubentPaths(const OdDbFullSubentPathArray& paths) ODRX_SEALED;

  /** \details
  Add one or more subentities to an entity.

  \param paths [in]  The array of OdDbFullSubentPath objects.
  */

  ODRX_SEALED_VIRTUAL OdResult addSubentPaths(const OdDbFullSubentPathArray& paths) ODRX_SEALED;

  /** \details
	Create an object by OdDbFullSubentPath to the subentity.

	\param path [in]  The OdDbFullSubentPath to the subentity.
  */

  ODRX_SEALED_VIRTUAL OdDbEntityPtr subentPtr(const OdDbFullSubentPath& path) const ODRX_SEALED;

  /** \details
	Applies a geometric transformation to one or more subentities on an object. 

	\param paths [in]  The array of OdDbFullSubentPath objects.
	\param xform [in]  The WCS transformation to apply to each of the supplied subentities.
  */

  ODRX_SEALED_VIRTUAL OdResult transformSubentPathsBy(const OdDbFullSubentPathArray &paths, const OdGeMatrix3d & xform) ODRX_SEALED;

  /** \details
	Obtains the CLSID of the wrapper coclass for the subentity.

	\param path [in]  The OdDbFullSubentPath to the subentity.
	\param clsId [in/out] The pointer to a CLSID structure whose contents are set to the CLSID of the wrapper for the subentity.
  */

  ODRX_SEALED_VIRTUAL OdResult getSubentClassId( const OdDbFullSubentPath& path, void* clsId ) ODRX_SEALED;

  /** \details
	Returns the geometric extents for a subentity.

	\param path [in]  The OdDbFullSubentPath to the subentity.
	\param extents [in/out] The geometric extents, expressed in world coordinates, of the subentity.
  */

  ODRX_SEALED_VIRTUAL OdResult getSubentPathGeomExtents( const OdDbFullSubentPath& path, OdGeExtents3d& extents ) ODRX_SEALED;

  ODRX_SEALED_VIRTUAL  void  subentGripStatus(OdDb::GripStat status, const OdDbFullSubentPath& subentity) ODRX_SEALED;
  
  /** \details
  Returns the entity coordinate system matrix.

  \remarks
  Default implementation returns Identity matrix.
  */
  virtual OdGeMatrix3d getEcs() const;

protected:
  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;
  virtual OdDbObjectPtr subDeepClone(OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);
  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& xfm,
    OdDbEntityPtr& pCopy) const;
  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;
  virtual OdResult subGetCompoundObjectTransform(OdGeMatrix3d & xM) const;

  virtual bool subCloneMeForDragging();
  virtual bool subHideMeForDragging() const;

  virtual void subGripStatus( const OdDb::GripStat status );

  virtual OdResult subGetOsnapPoints(
    OdDb::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints ) const;
  
  virtual OdResult subGetOsnapPoints(
    OdDb::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints,
    const OdGeMatrix3d& insertionMat) const;
  
  virtual bool subIsContentSnappable() const;

  virtual OdResult subGetGripPoints( OdGePoint3dArray& gripPoints ) const;
  virtual OdResult subMoveGripPointsAt(const OdIntArray& indices,const OdGeVector3d& offset );
  virtual OdResult subGetGripPoints(
    OdDbGripDataPtrArray& grips,
    const double curViewUnitSize,
    const int gripSize,
    const OdGeVector3d& curViewDir,
    const int bitFlags ) const;
  virtual OdResult subMoveGripPointsAt(
    const OdDbVoidPtrArray& grips,
    const OdGeVector3d& offset,
    int bitFlags );
  virtual OdResult subGetStretchPoints( OdGePoint3dArray& stretchPoints ) const;
  virtual OdResult subMoveStretchPointsAt(
    const OdIntArray& indices,
    const OdGeVector3d& offset );

  virtual OdResult subIntersectWith(const OdDbEntity* pEnt, OdDb::Intersect intType,
    OdGePoint3dArray& points, OdGsMarker thisGsMarker = 0,
    OdGsMarker otherGsMarker = 0) const;

  virtual OdResult subIntersectWith(const OdDbEntity* pEnt, OdDb::Intersect intType,
    const OdGePlane& projPlane, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker = 0, OdGsMarker otherGsMarker = 0) const;

  virtual void subHighlight(bool bDoIt = true, const OdDbFullSubentPath* pSubId = 0,
    bool highlightAll = false) const;

  virtual OdDb::Visibility subVisibility() const;
  virtual OdResult subSetVisibility(OdDb::Visibility visibility, bool doSubents = true);

  virtual OdResult subGetGeomExtents(
      OdGeExtents3d& extents) const;
  virtual OdResult subDeleteSubentPaths(const OdDbFullSubentPathArray& paths);
  virtual OdResult subAddSubentPaths(const OdDbFullSubentPathArray& paths);

  virtual OdResult subMoveGripPointsAtSubentPaths( const OdDbFullSubentPathArray& paths, 
    const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, const OdUInt32 bitflags);
  virtual OdResult subGetGripPointsAtSubentPath( const OdDbFullSubentPath& path, OdDbGripDataPtrArray& grips,
    const double curViewUnitSize, const int gripSize,
    const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const;
  virtual OdResult subGetSubentPathsAtGsMarker(OdDb::SubentType type, OdGsMarker gsMark, 
    const OdGePoint3d& pickPoint,const OdGeMatrix3d& xfm, 
    OdDbFullSubentPathArray& subentPaths, 
    const OdDbObjectIdArray* pEntAndInsertStack = 0) const;
  virtual OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
    OdGsMarkerArray& gsMarkers) const;
  virtual OdDbEntityPtr subSubentPtr(const OdDbFullSubentPath& path) const;
  virtual OdResult subTransformSubentPathsBy(const OdDbFullSubentPathArray &paths, const OdGeMatrix3d & xform);
  virtual OdResult subGetSubentClassId( const OdDbFullSubentPath& path, void* clsId ) const;
  virtual OdResult subGetSubentPathGeomExtents( const OdDbFullSubentPath& path, OdGeExtents3d& extents ) const;
  virtual void subSubentGripStatus(OdDb::GripStat status, const OdDbFullSubentPath& subentity);

  friend class OdDbPropertiesOverrule;
  friend class OdDbOsnapOverrule;
  friend class OdDbTransformOverrule;
  friend class OdDbGripOverrule;
  friend class OdDbSubentityOverrule;
  friend class OdDbHighlightOverrule;
  friend class OdDbVisibilityOverrule;
  friend class OdDbGeometryOverrule;
};

#include "TD_PackPop.h"

#endif /* _ODDBENTITY_INCLUDED_ */

