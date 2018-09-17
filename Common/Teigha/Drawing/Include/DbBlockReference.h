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

#ifndef _OD_BLOCK_REFERENCE_
#define _OD_BLOCK_REFERENCE_

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DbDatabase.h"
#include "Ge/GeMatrix3d.h"
#include "DbObjectIterator.h"
#include "DwgDeclareMembers.h"

class OdGeScale3d;
class OdDbAttribute;
class OdDbSequenceEnd;
class OdDbBlockReferenceImpl;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbAttribute object pointers.
*/
typedef OdSmartPtr<OdDbAttribute> OdDbAttributePtr;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSequenceEnd object pointers.
*/
typedef OdSmartPtr<OdDbSequenceEnd> OdDbSequenceEndPtr;

/** \details
    This class represents block references (Inserts) in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbBlockReference : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbBlockReference);

  OdDbBlockReference();
  
  /** \details
    Returns the Object ID of the block table record associated with this OdDbBlockReference (DXF 2).
  */
  OdDbObjectId blockTableRecord() const;

  /** \details
    Sets the Object ID of the block table record associated with this OdDbBlockReference (DXF 2).
    
    \param objectId [in]  Object ID of the block.
  */
  virtual void setBlockTableRecord(
    OdDbObjectId objectId);

  /** \details
    Returns the insertion point of this block reference (WCS equivalent of DXF 10).
  */
  virtual OdGePoint3d position() const;

  /** \details
    Sets the insertion point of this block reference. (WCS equivalent of DXF 10.)

    \param position [in]  Any 3D point.
  */
  virtual void setPosition(
    const OdGePoint3d& position);

  /** \details
    Returns the scale factors applied to this block reference (DXF 41, 42, 43).
  */
  OdGeScale3d scaleFactors() const;

  /** \details
    Sets the scale factors to be applied to this block reference (DXF 41, 42, 43).

    \param scale [in]  Any 3D scale factor.
    
    Throws:
    <table>
    Exception             Cause
    eInvalidInput         One or more scale factors is 0
    </table>
  */
  virtual void setScaleFactors(
    const OdGeScale3d& scale);

  /** \details
    Returns the rotation angle applied to this block reference (DXF 50).

    \remarks
    Rotation is about the Z axis, relative the X-axis, in the coordinate system parallel to
    this object's OCS, but with its origin at this object's insertion point.

    \remarks
    All angles are expressed in radians.
  */
  double rotation() const;

  /** \details
    Sets the rotation angle to be applied to this block reference, in radians (DXF 50).

    \remarks
    Rotation is about the Z axis, relative the X-axis, in the coordinate system parallel to
    this object's OCS, but with its origin at this object's insertion point.

    \param angle [in]  Rotation angle.
    
    \remarks
    All angles are expressed in radians.
  */
    virtual void setRotation(
    double angle);

  /** \details
    Returns the WCS normal to the plane of this entity (DXF 210).
  */
  OdGeVector3d normal() const;
  
  /** \details
    Sets the WCS normal to the plane of this entity (DXF 210).
    
    \param normal [in]  Normal.
  */
  virtual void setNormal(
    const OdGeVector3d& normal);

  /** \details
    Returns true if and only if there is a plane that contains the entity.
    In another case this method returns false. 
  */
  virtual bool isPlanar() const { return true; }
  
  /** \details
    Returns the plane that contains this entity.
    
    \param plane [out]  Receives the plane that contains this entity.
    \param planarity [out]  Receives the planarity of this entity.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    planarity and plane return values as follows:
    <table>
    planarity   Value   Description     Plane
    kNonPlanar  0       Non-planar      Not set
    kPlanar     1       Planar          Entity plane
    kLinear     2       Linear          Arbitrary plane containing this entity
    </table>
  */
  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;
  
  /** \details
    Returns the transformation matrix mapping a point in the MCS
    to the WCS.
    
    \remarks
    The Microspace Coordinate System (MCS) is the WCS within the block definition.
    Applying this matrix to the center of an arc within the block definition
    returns the center of the arc as it appears in the block reference.
  */
  OdGeMatrix3d blockTransform() const;

  /** \details
    Sets the transformation matrix mapping a point in the MCS
    to the WCS.
    
    \param xfm [in]  Any 3D transformation matrix.
    
    \remarks
    The Microspace Coordinate System (MCS) is the WCS within the block definition.
    Applying this matrix to the center of an arc within the block definition
    returns the center of the arc as it appears in the block reference.
  */
  virtual OdResult setBlockTransform(
    const OdGeMatrix3d& xfm);

  /** \details
    Appends the specified OdDbAttribute to the attribute list of this block reference.

    \param pAttr [in]  Pointer to the attribute to be added.

    \returns
    Returns the Object ID of the newly appended attribute.

    \remarks
    This block reference becomes the owner of the passed in attribute, and the attribute
    is added to the database to which this block reference belongs.  
    This block reference must be added to a database before calling this function.
    The object's attribute list should not be added by the client application.
  */
  OdDbObjectId appendAttribute(
    OdDbAttribute* pAttr);

  /** \details
    Opens an attribute owned by this block reference.

    \param objId [in]  Object ID of the attribute to be opened.
    \param mode [in]  Mode in which the attribute is to be opened.
    \param openErasedOne [in]  If and only if true, erased objects will be opened.

    \returns
    Returns a SmartPointer to the opened object if successful, otherwise a null SmartPointer.
  */
  OdDbAttributePtr openAttribute(
    OdDbObjectId ObjId,
    OdDb::OpenMode mode, 
    bool openErasedOne = false);

  /** \details
    Opens the OdDbSequenceEnd entity for this block reference.

    \param mode [in]  Mode in which to open the OdDbSequenceEnd entity.

    \returns
    Returns a SmartPointer to the newly opened OdDbSequenceEnd, or a null SmartPointer.

    \remarks
    This method is provided solely for applications that store XData on
    OdDbSequenceEnd entities; this is not recommended. 
  */
  OdDbSequenceEndPtr openSequenceEnd(
    OdDb::OpenMode mode);

  /** \details
    Returns an AttributeIterator for this block reference.
  */
  OdDbObjectIteratorPtr attributeIterator() const;

  /** \details
    Explodes this entity into a set of simpler entities, and adds them to the specified block 
    table record.

    \param BlockRecord [out]  Pointer to the Block Table Record.
    \param ids [in/out]  Receives an array of Object IDs of the new Block References.

    \returns
    Entities resulting from the explosion are appended to the specified array.
    The newly created entities are database resident or not depending on the block table 
    record they are appended to. If block table record is database resident the entities 
    are database resident as well. If block table record is not database resident newly 
    created entities are not database resident.
    The default implementation of this function returns eNotApplicable. This function can 
    be overridden in custom classes.
  */
  virtual OdResult explodeToBlock(
    OdDbBlockTableRecord *BlockRecord, 
    OdDbObjectIdArray *ids = 0);

  /** \details
    Sets the values of this object's subentity traits, and returns with the calling 
    object's subentity traits.
    
    \param pTraits [in] Pointer to the OdGiDrawableTraits object to be set.

    \remarks
    When overriding subSetAttributes(), you must OR (|) the return value of 
    <base class>::subSetAttributes(pTraits) with any flags you add.
    A derived class may not remove flags for any reason.
    The default implementation does nothing but returns kDrawableNone. 
    This function can be overridden in custom classes.
  */
  OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const;

  /** \details
    Returns true if the entity has no view-dependent graphics. In another case
    returns false.
    
    \param pWd [in] Pointer to the OdGiWorldDraw object.
  */
  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;
  
  /** \details
    Computes the entity to produce the right appearance given each viewport's 
    possibly different viewpoint of the entity.Draws an entity in layout viewports.
    
    \param pVd [in]  Pointer to the OdGiViewportDraw object.
  */
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;

  /** \details
    Reads the .dwg file data of this object.
    
    \param pFiler [in]  Filer object from which data are read.
    
    \remarks 
    Returns the filer status. 
    This function is called by dwgIn() to allow the object to read its data. 
    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dwgInFields(pFiler). 
    3) If it returns eOK, continue; otherwise return whatever the parent's dwgInFields(pFiler) 
    returned. 
    4) Call the OdDbDwgFiler(pFiler) methods to read each of the object's data items in the 
    order they were written. 
    5) Return pFiler->filerStatus().
  */
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object.
    
    \param pFiler [in]  Pointer to the filer to which data are written.
    
    \remarks 
    This function is called by dwgIn() to allow the object to write its data. 
    When overriding this function: 
    1) Call assertReadEnabled(). 2) Call the parent class's dwgOutFields(pFiler). 
    3) Call the OdDbDwgFiler(pFiler) methods to write each of the object's data items in the 
    order they were written.
  */
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the DXF data of this object. 
    
    \param pFiler [in]  Pointer to the filer from which data are read. 
    
    \remarks 
    Returns the filer status. 
    This function is called by dxfIn() to allow the object to read its data. 
    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dwgInFields(pFiler).
    3) If it returns eOK, continue; otherwise return whatever the parent's dxfInFields(pFiler) 
    returned. 
    4) Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the 
    order they were written. 
    5) Return pFiler->filerStatus().
  */
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  /** \details
    Reads the DXF data of this object. 
    
    \param pFiler [in]  Pointer to the filer from which data are read. 
    
    \remarks 
    Returns the filer status. 
    This function is called by dxfIn() to allow the object to read its data. 
    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dwgInFields(pFiler).
    3) If it returns eOK, continue; otherwise return whatever the parent's dxfInFields(pFiler) 
    returned. 
    4) Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the 
    order they were written. 
    5) Return pFiler->filerStatus().
  */
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Reads the DXF R12 format data of this object.  
    
    \param pFiler [in] Pointer to the filer from which data are to be read.    
    
    \remarks
    Returns the filer status. 
    This function is called by dxfIn() to allow the object to read its data. 
    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dxfInFields_R12(pFiler). 
    3) If it returns eOK, continue; otherwise return whatever the parent's dxfOutFields_R12(pFiler) 
    returned. 
    4) Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the 
    order they were written. 
    5) Return pFiler->filerStatus().
 */
  virtual OdResult dxfInFields_R12(OdDbDxfFiler* pFiler);

  /** \details
    Writes the DXF data of this object.   
    
    \param pFiler [in] Pointer to the filer to which data are to be written.     
    
    \remarks
    Returns the filer status. 
    This function is called by dxfOut() to allow the object to write its data. 
    When overriding this function: 
    1) Call assertReadEnabled(). 2) Call the parent class's dxfOutFields(pFiler). 
    3) Use pFiler to call the OdDbDxfFiler methods to write each of the object's data items in 
    the order they were written.
  */
  virtual void dxfOutFields_R12(
  OdDbDxfFiler* pFiler) const;

  /** \details
    Reads the DXF format data of this object from the specified filer.    
    
    \param pFiler [in] Pointer to the filer from which the data are to be read.      
    
    \remarks
    Returns the filer status. 
    This function calls this object's dxfInFields(pFiler), then loads any Xdata associated 
    with this object.
  */
  virtual OdResult dxfIn(
    OdDbDxfFiler* pFiler);

  /** \details
    Writes the DXF format data of this object to the specified filer.   
    
    \param pFiler [in] Pointer to the filer to which the data are to be written.     
    
    \remarks
    This function calls this object's dxfOutFields(pFiler) function, writes any Xdata 
    associated with this object.
  */
  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Called as the first operation as this object is being closed, for database-resident objects only.   
    
    \remarks
    This function is notified just before the current open operation is to be closed, 
    giving this function the ability to perform necessary operations. 
    When overriding this function: 
    1) If the OdDbObject's state is incorrect, throw exception. 
    2) Call parent class's subClose(). 
    3) If other actions are required before close, do them. 
    The default implementation of this function does nothing. This function can be overridden 
    in custom classes.
  */ 
  void subClose();

  /** \details
    Overridables
  */
  virtual OdResult subGetClassID(
    void* pClsid) const;

  /** \details
    Applies the specified 3D transformation matrix to this entity.
    
    \param xfm [in] 3D transformation matrix.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */    
  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  /** \details
    This is subGetTransformedCopy, a member of class OdDbBlockReference.
    
    \param xfm [in] Input matrix for transforming the copy of the entity.
    \param pCopy [out] Pointer to the transformed copy of the entity. 
  */ 
  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& xfm, 
    OdDbEntityPtr& pCopy) const ODRX_OVERRIDE;

 /* Replace OdRxObjectPtrArray */
  /** \details
    Explodes this entity into a set of simpler entities. 
    
    \param entitySet [in] Receives an array of pointers to the new entities.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */ 
  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE;

  /** \details
    Called as the first operation of the handOverTo function.    
    
    \param pNewObject [in] Pointer to the object with which to replace this object in the database. 
    
    \remarks
    This function allows custom classes to populate the new object. 
    Overriding this function in a child class allows a child instance 
    to be notified each time an object is handed over. 
    This function is notified just before an object is to be handed over; giving this 
    function the ability to cancel the handover. 
    When overriding this function: 
    1) If the OdDbObject's state is incorrect, throw exception. 
    2) Call parent class's subHandover(). 
    3) If other actions are required before handover, do them. 
    The default implementation of this function does nothing. This function can be overridden 
    in custom classes.
  */
  void subHandOverTo(
    OdDbObject* pNewObject);

  /** \details
    This is subGetGeomExtents, a member of class OdDbBlockReference.
    
    \param extents [out] An output extents.
  */
  OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const;

  /** \details
    Explodes this entity into a set of simpler entities. 
     
    \param entitySet [in/out] Receives an array of pointers to the new entities.
    
    \remarks
    The newly created entities will be not database resident.

    \returns  
    Returns eOk if successful, or an appropriate error code if not.
    
    \remarks
    Entities resulting from the explosion are appended to the specified array. 
    The default implementation of this function calls subWorldDraw()
    and makes entities from geometry generated by subWorldDraw(). 
    This function can be overridden in custom classes.
  */
  OdResult explodeGeometry(OdRxObjectPtrArray& entitySet) const;
  
  /** \details
    This is treatAsOdDbBlockRefForExplode, a member of class OdDbBlockReference.
    Determines how the entity should be exploded.  
  */
  virtual bool treatAsOdDbBlockRefForExplode() const;

  /** \details
    This method gets copies of all entities in the referenced block table record,
    applies parentXform to them and after this calculates extents on the
    resulting object collection.
    
    \param extents [out] WCS extents of the block reference.
    \param parentXform [in] Transformation matrix for block reference's geometry.
  */
  OdResult geomExtentsBestFit(OdGeExtents3d& extents,
                              const OdGeMatrix3d& parentXform = OdGeMatrix3d::kIdentity) const;

  /** \details
    Explodes the current object and appends resulting entities to the block table record
    which owns the current OdDbBlockReference object that calls this method. 
    
    \remarks
    The block reference entity must be in a database and must be uniformly scaled.  
  */
  virtual OdResult explodeToOwnerSpace() const;

  /** \details
    Copies the contents of the specified object into this object.
    
    \param pSrc [in] Pointer to the source object.

    \remarks
    The default implementation of this function does nothing but throw an eNotApplicable exception.
    The source object and this object need not be of the same type. 
    This function is meaningful only when implemented by custom classes.
  */  
  virtual void copyFrom(const OdRxObject* pSrc) ODRX_OVERRIDE;

  /** \details
    Returns the transformation matrix of compound object.
    
    \param xM [out] Transformation matrix.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  ODRX_SEALED_VIRTUAL OdResult subGetCompoundObjectTransform(OdGeMatrix3d & xM) const ODRX_OVERRIDE;

  /** \details
    This is subGetSubentPathsAtGsMarker, a member of class OdDbBlockReference.
    
    \param type [in] Subentity type.
    \param gsMark [in] Gs marker representing a subentity.
    \param pickPoint [in] WCS point picked during selection of the entity.
    \param viewXform [in] Transformation matrix to transform from WCS to DCS.
    \param subentPaths [out] Array of OdDbFullSubentPath objects.
    \param pEntAndInsertStack [in] Array of object IDs of the objects which are the subcontainers of the subentity.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not. 
 */
  ODRX_SEALED_VIRTUAL OdResult subGetSubentPathsAtGsMarker(OdDb::SubentType type,
                                    OdGsMarker gsMark, 
                                    const OdGePoint3d& pickPoint,
                                    const OdGeMatrix3d& viewXform, 
                                    OdDbFullSubentPathArray& subentPaths, 
                                    const OdDbObjectIdArray* pEntAndInsertStack) const ODRX_OVERRIDE;
  
  /** \details
    This is subGetGsMarkersAtSubentPath, a member of class OdDbBlockReference.
    
    \param subPath [in] Information needed to identify the subentity (or subentities).
    \param gsMarkers [out] Array of Gs markers for the subentity or subentities.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not. 
  */ 
  ODRX_SEALED_VIRTUAL OdResult subGetGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath, 
                                    OdGsMarkerArray& gsMarkers) const ODRX_OVERRIDE;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBlockReference object pointers.
*/
typedef OdSmartPtr<OdDbBlockReference> OdDbBlockReferencePtr;

TOOLKIT_EXPORT void decompTransform(
    const OdGeMatrix3d& mTrans,
    OdGePoint3d& pOrigin, 
    OdGeScale3d& sScale,
    double & dRotation, 
    OdGeVector3d& vNormal );
TOOLKIT_EXPORT void decompTransform(
    OdGeVector3d& normal, 
    OdGePoint3d& position, 
    OdGeScale3d& scale,
    double& rotationAngle,
    const OdGeMatrix3d& transformMat,
    const OdDbBlockTableRecordPtr pBlockTableRecord);

#include "TD_PackPop.h"

#endif

