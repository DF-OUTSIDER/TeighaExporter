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

#ifndef _DBENTITYOVRRULE_H_INCLUDED_
#define _DBENTITYOVRRULE_H_INCLUDED_

#include "RxOverrule.h"
#include "DbEntity.h"

/** \details
  This class overrules a subset of osnap linked operations which an entity specifies. 
  It is used as a base class for classes derived from OdDbEntity to change their behavior. 
  Each default implementation calls the corresponding method of the target class.

  Corresponding C++ library: TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbOsnapOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDbOsnapOverrule);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbOsnapOverrule();

  /** \details
    Overrules the OdDbEntity::subGetOsnapPoints() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param osnapMode [in]  Osnap mode to be requested.
    \param gsSelectionMark [in]  GS marker of the subentity involved in the snap operation.
    \param pickPoint [in]  WCS coordinates of the point picked during the snap operation.
    \param lastPoint [in]  WCS coordinates of the point selected just before pickPoint.
    \param xWorldToEye [in]  A transformation matrix to transform from WCS to DCS.
    \param snapPoints [in]  An existing array to append osnap points.
  */
  virtual OdResult getOsnapPoints(
    const OdDbEntity* pSubject,
    OdDb::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints );

  /** \details
    Overrules the OdDbEntity::subGetOsnapPoints() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param osnapMode [in]  Osnap mode to be requested.
    \param gsSelectionMark [in]  GS marker of the subentity involved in the snap operation.
    \param pickPoint [in]  WCS coordinates of the point picked during the snap operation.
    \param lastPoint [in]  WCS coordinates of the point selected just before pickPoint.
    \param xWorldToEye [in]  A transformation matrix to transform from WCS to DCS.
    \param snapPoints [in]  An existing array to append osnap points.
    \param insertionMat [in]  A transformation matrix for block transformation.
  */
  virtual OdResult getOsnapPoints(
    const OdDbEntity* pSubject,
    OdDb::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints,
    const OdGeMatrix3d& insertionMat);

  /** \details
    Returns True when it is a block entity which requires investigating the block definition, or False otherwise.

    \remarks
    The application can override this function to inform the host application: don't investigate the compounded objects 
    that use a block definition for osnap points. This method is not called for objects that don't use a block definition 
    in the worldDraw() implementation.

    \param pSubject [in]  A raw pointer to an entity.
  */
  virtual bool isContentSnappable(const OdDbEntity* pSubject);
};

/** \details
  This class overrules a subset of transformation linked operations which an entity specifies. 
  It is used as a base class for classes derived from OdDbEntity to change their behavior. 
  Each default implementation calls the corresponding method of the target class.
  
  Corresponding C++ library:  TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbTransformOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDbTransformOverrule);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbTransformOverrule();

  /** \details
    Overrules OdDbEntity::subTransformBy() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param xform [in]  Transformation matrix.
  */
  virtual OdResult transformBy(OdDbEntity* pSubject, const OdGeMatrix3d& xform);

  /** \details
    Overrules OdDbEntity::subGetTransformedCopy() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param xform [in]  A transformation matrix.
    \param pEnt [in/out] A reference to an empty pointer in which a pointer to the transformed copy of the entity should be saved.
  */
  virtual OdResult getTransformedCopy(const OdDbEntity* pSubject, const OdGeMatrix3d& xform, OdDbEntityPtr& pEnt);

  /** \details
    Overrules OdDbEntity::subExplode() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param entitySet [in/out] An array of pointers to new entities; this array can contain the pointers from other entities; 
                    all entities which are a result of the exploding operation are appended to this array.
  */
  virtual OdResult explode(const OdDbEntity* pSubject, OdRxObjectPtrArray& entitySet);

  /** \details
    Overrules OdDbEntity::subCloneMeForDragging() method and calls it in the default implementation.
    This method returns True when a clone will be created for the dragging operation, or False otherwise.

    \param pSubject [in]  A raw pointer to an entity.
  */
  virtual bool cloneMeForDragging(OdDbEntity* pSubject);

  /** \details
    Overrules OdDbEntity::subHideMeForDragging() method and calls it in the default implementation.
    This method returns True when the original object should be hidden during the dragging operation, 
    or False otherwise.

    \param pSubject [in]  A raw pointer to an entity.
  */
  virtual bool hideMeForDragging(const OdDbEntity* pSubject);
};

/** \details
  This class overrules a subset of grip linked operations which an entity specifies. 
  It is used as a base class for classes derived from OdDbEntity to change their behavior. 
  Each default implementation calls the corresponding method of the target class.

  Corresponding C++ library:  TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGripOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGripOverrule);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbGripOverrule();

  /** \details
    Overrules OdDbEntity::subGetGripPoints() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param gripPoints [in/out] An existing array to which the grip points should be appended.
  */
  virtual OdResult getGripPoints( const OdDbEntity* pSubject, OdGePoint3dArray& gripPoints );

  /** \details
    Overrules OdDbEntity::subGetGripPoints() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param gripsData [in] An array of pointers to dynamically allocated grip data objects. 
    \param curViewUnitSize [in] A size of one drawing unit in the current viewport (in pixels).
    \param gripSize [in] A size of current grip point (in pixels).
    \param curViewDir [in] A vector that specifies the view direction in the current viewport.
    \param bitFlags [in] Mask of one or more flags specified by the OdDbGripOperations::GetGripPointsFlags enumeration.
  */
  virtual OdResult getGripPoints(const OdDbEntity* pSubject, 
    OdDbGripDataPtrArray& gripsData,
    const double curViewUnitSize,
    const int gripSize,
    const OdGeVector3d& curViewDir,
    const int bitFlags );

  /** \details
    Overrules OdDbEntity::subMoveGripPointsAt() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param indices [in]  An array containing the indices corresponding to grip points obtained from 
                the getGripPoints() method that indicate which grip points are now "hot."
    \param offset [in] A vector that indicates the direction and magnitude which the grip points have been translated.
  */
  virtual OdResult moveGripPointsAt(OdDbEntity* pSubject, 
    const OdIntArray& indices,
    const OdGeVector3d& offset );

  /** \details
    Overrules OdDbEntity::subMoveGripPointsAt() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param grips [in] An array of pointers referring to each grip point to be moved.
    \param offset [in] A vector that indicates the direction and magnitude which the grip points have been translated.
    \param bitFlags [in] Mask of one or more flags specified by the OdDbGripOperations::GetGripPointsFlags enumeration.
  */
  virtual OdResult moveGripPointsAt(OdDbEntity* pSubject,
    const OdDbVoidPtrArray& grips,
    const OdGeVector3d& offset,
    int bitFlags );

  /** \details
    Overrules OdDbEntity::subGetStretchPoints() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param stretchPoints [in/out] An existing array to which the stretch points should be appended.
  */
  virtual OdResult getStretchPoints(const OdDbEntity* pSubject,
    OdGePoint3dArray& stretchPoints);

  /** \details
    Overrules OdDbEntity::subMoveStretchPointsAt() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param indices [in]  An array containing the indices that indicate which stretch points are being translated.
  */
  virtual OdResult moveStretchPointsAt(OdDbEntity* pSubject,
    const OdIntArray & indices,
    const OdGeVector3d& offset);

  /** \details
    Overrules OdDbEntity::subGripStatus() method and calls it in the default implementation.

    \param pSubject [in]  A raw pointer to an entity.
    \param status [in]  The status of the grip operation specified by the OdDbGripOperations::GripStatus enumeration.
  */
   virtual void gripStatus(OdDbEntity* pSubject, const OdDb::GripStat status);
};

/** \details
  This class overrules a subset of subentity linked operations which an entity specifies. 
  It is used as a base class for classes derived from OdDbEntity to change their behavior. 
  Each default implementation calls the corresponding method of the target class.
    
  Corresponding C++ library:  TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbSubentityOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDbSubentityOverrule);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbSubentityOverrule();

  /** \details
    Overrules OdDbEntity::subDeleteSubentPaths() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param paths [in]  An array of full subentity path objects identifying the subentities to be deleted from the entity.
  */
  virtual OdResult deleteSubentPaths(OdDbEntity* pSubject, const OdDbFullSubentPathArray& paths);

  /** \details
    Overrules OdDbEntity::subAddSubentPaths() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param paths [in]  An array of full subentity path objects identifying the subentities to be added to the entity.
  */
  virtual OdResult addSubentPaths(OdDbEntity* pSubject, const OdDbFullSubentPathArray& paths);

  /** \details
    Overrules OdDbEntity::subTransformSubentPathsBy() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param paths [in]  An array of full subentity path objects identifying the subentities to be transformed.
    \param xform [in]  A transformation matrix to be applied to each subentity.
  */
  virtual OdResult transformSubentPathsBy(OdDbEntity* pSubject,
    const OdDbFullSubentPathArray& paths,
    const OdGeMatrix3d&            xform);

  /** \details
    Overrules OdDbEntity::subGetGripPointsAtSubentPath() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param path [in]  A full subentity path object identifying the subentity.
    \param grips [in]  An array of pointers to dynamically allocated grip data objects.
    \param curViewUnitSize [in]  A size of one drawing unit in the current viewport (in pixels).
    \param gripSize [in]  A size of current grip point (in pixels).
    \param curViewDir [in]  A vector that specifies the view direction in the current viewport.
    \param bitFlags [in]  Mask of one or more flags specified by the OdDbGripOperations::GetGripPointsFlags enumeration.
  */
  virtual OdResult getGripPointsAtSubentPath(
    const OdDbEntity*              pSubject,
    const OdDbFullSubentPath&      path,
    OdDbGripDataPtrArray&          grips,
    const double                   curViewUnitSize,
    const int                      gripSize,
    const OdGeVector3d&            curViewDir,
    const OdUInt32                 bitflags);

  /** \details
    Overrules OdDbEntity::subMoveGripPointsAtSubentPaths() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param paths [in]  An array of full subentity path objects identifying the subentities to be moved.
    \param gripAppData [in]  An array of pointers referring to each grip point to be moved.
    \param offset [in]  A vector that indicates the direction and magnitude of the translation to be applied to the grips.
    \param bitFlags [in]  Mask of one or more flags specified by the OdDbGripOperations::GetGripPointsFlags enumeration.
  */
  virtual OdResult moveGripPointsAtSubentPaths(
    OdDbEntity*                    pSubject,
    const OdDbFullSubentPathArray& paths,
    const OdDbVoidPtrArray&        gripAppData,
    const OdGeVector3d&            offset,
    const OdUInt32                 bitflags);

  /** \details
    Overrules the OdDbEntity::subGetSubentPathsAtGsMarker() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param type [in]  A subentity type specified by the OdDb::SubentType enumeration.
    \param gsMark [in]  GS marker representing a specific subentity.
    \param pickPoint [in]  WCS coordinates of the point picked during the selection of the entity.
    \param viewXform [in]  A transformation matrix to transform from WCS to DCS.
    \param subentPaths [out]  An array of full subentity path objects identifying the subentities generated from the GS marker.
    \param pEntAndInsertStack [in]  An array of object IDs that are the nested containers of the subentity.
  */
  virtual  OdResult getSubentPathsAtGsMarker(
    const OdDbEntity*      pSubject,
    OdDb::SubentType       type,
    OdGsMarker             gsMark, 
    const OdGePoint3d&     pickPoint,
    const OdGeMatrix3d&    viewXform, 
    OdDbFullSubentPathArray&   subentPaths, 
    const OdDbObjectIdArray* pEntAndInsertStack = 0);

  /** \details
    Overrules the OdDbEntity::subGetGsMarkersAtSubentPath() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param subPath [in]  A full subentity path object identifying the subentity.
    \param gsMarkers [out]   An array with GSMarkers for the subentities.
  */
  virtual  OdResult getGsMarkersAtSubentPath(
    const OdDbEntity*         pSubject,
    const OdDbFullSubentPath& subPath, 
    OdGsMarkerArray& gsMarkers);

  /** \details
    Overrules the OdDbEntity::subentPtr() method and calls it in the default implementation.
    This method returns a smart pointer to the subentity if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param id [in]  A full subentity path object identifying the subentity.
  */
  virtual OdDbEntityPtr subentPtr(const OdDbEntity* pSubject, const OdDbFullSubentPath& id);

  /** \details
    Overrules the OdDbEntity::subGetCompoundObjectTransform() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param xMat [out]  A transformation matrix to be filled for nested objects.
  */
  virtual OdResult getCompoundObjectTransform(const OdDbEntity* pSubject, OdGeMatrix3d & xMat);

  /** \details
    Overrules OdDbEntity::subGetSubentPathGeomExtents() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param path [in]  A full subentity path object identifying the subentity.
    \param extents [out]  An extents object in which subentity extents should be saved.
  */
  virtual OdResult getSubentPathGeomExtents(const OdDbEntity* pSubject,
    const OdDbFullSubentPath& path, 
    OdGeExtents3d& extents);

  /** \details
    Overrules OdDbEntity::subentGripStatus() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param status [in]  A status of the grip operation specified by the OdDb::GripStat enumeration.
    \param path [in]  A full subentity path object identifying the subentity whose grip status is to be changed.
  */
  virtual void subentGripStatus(OdDbEntity* pSubject, 
    OdDb::GripStat status,
    const OdDbFullSubentPath& subentity);

  /** \details
    Overrules OdDbEntity::subGetSubentClassId() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param path [in]  A full subentity path object identifying the subentity.
    \param clsId [out]  A pointer to the class description structure whose contents are set for the subentity.
  */
  virtual OdResult getSubentClassId(const OdDbEntity* pSubject,
    const OdDbFullSubentPath& path,
    void*                    clsId);
};

/** \details
  This class overrules a subset of highlight linked operations which an entity specifies. 
  It is used as a base class for classes derived from OdDbEntity to change their behavior. 
  Each default implementation calls the corresponding method of the target class.
    
  Corresponding C++ library:  TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbHighlightOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDbHighlightOverrule);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbHighlightOverrule();

  /** \details
    Overrules OdDbEntity::subHighlight() method and calls it in the default implementation.

    \param pSubject [in]  A raw pointer to an entity.
    \param bDoIt [in]  Indicates whether to highlight.
    \param pSubId [in]  A full subentity path that identifies the subentity to be highlighted.
    \param highlightAll [in]  Indicates whether to highlight in all viewports.
  */
  virtual void highlight(const OdDbEntity* pSubject, bool bDoIt = true, const OdDbFullSubentPath* pSubId = 0,
    bool highlightAll = false);
};

/** \details
  This class overrules a subset of visibility linked operations which an entity specifies. 
  It is used as a base class for classes derived from OdDbEntity to change their behavior. 
  Each default implementation calls the corresponding method of the target class.

  Corresponding C++ library:  TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbVisibilityOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDbVisibilityOverrule);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbVisibilityOverrule();

  /** \details
    Overrules OdDbEntity::subVisibility() method and calls it in the default implementation.
    This method returns the visibility state of the object as the OdDb::Visibility enumeration.

    \param pSubject [in]  A raw pointer to an entity.
  */
  virtual OdDb::Visibility visibility(const OdDbEntity* pSubject);

  /** \details
    Overrules OdDbEntity::subSetVisibility() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param visibility [in]  A visibility state as the OdDb::Visibility enumeration.
    \param doSubents [in]  Indicates whether the new value should be applied to the subentitites.
  */
  virtual OdResult setVisibility(OdDbEntity* pSubject, OdDb::Visibility visibility, bool doSubents = true);
};

/** \details
  This class overrules a subset of geometry linked operations which an entity specifies. 
  It is used as a base class for classes derived from OdDbEntity to change their behavior. 
  Each default implementation calls the corresponding method of the target class.
    
  Corresponding C++ library:  TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeometryOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeometryOverrule);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbGeometryOverrule();

  /** \details
    Overrules OdDbEntity::subIntersectWith() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pThisEnt [in]  A raw pointer to an entity.
    \param pEnt [in]  An entity with which this entity is intersected.
    \param intType [in]  Type of intersection.
    \param points [out]  An array of points in which the intersection points should be appended.
    \param thisGsMarker [in]  GS marker of the subentity of this entity which is involved in the intersection.
    \param otherGsMarker [in]  GS marker of the subentity of the entity pointed to by pEnt which is involved in the intersection.
  */
  virtual OdResult intersectWith(const OdDbEntity* pThisEnt,
    const OdDbEntity* pEnt, OdDb::Intersect intType,
    OdGePoint3dArray& points, OdGsMarker thisGsMarker = 0,
    OdGsMarker otherGsMarker = 0);
  /** \details
    Overrules OdDbEntity::subIntersectWith() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pThisEnt [in]  A raw pointer to an entity.
    \param pEnt [in]  An entity with which this entity is intersected.
    \param intType [in]  Type of intersection.
    \param projPlane [in]  A projection plane for the intersection of the two entities.
    \param points [out]  An array of points in which the intersection points should be appended.
    \param thisGsMarker [in]  GS marker of the subentity of this entity which is involved in the intersection.
    \param otherGsMarker [in]  GS marker of the subentity of the entity pointed to by pEnt which is involved in the intersection.
  */
  virtual OdResult intersectWith(const OdDbEntity* pThisEnt,
    const OdDbEntity* pEnt, OdDb::Intersect intType,
    const OdGePlane& projPlane, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker = 0, OdGsMarker otherGsMarker = 0);

  /** \details
    Overrules OdDbEntity::subGetGeomExtents() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pSubject [in]  A raw pointer to an entity.
    \param extents [out]  An extents object in which entity extents should be saved.
  */
  virtual OdResult getGeomExtents(const OdDbEntity* pSubject, OdGeExtents3d& extents);
};

#endif //_DBENTITYOVRRULE_H_INCLUDED_
