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

#ifndef _DG_ELEMENTOVRRULE_H_INCLUDED_
#define _DG_ELEMENTOVRRULE_H_INCLUDED_

#include "RxOverrule.h"
#include "DgElement.h"
#include "DgGraphicsElement.h"

/** \details
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgElementOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgElementOverrule);
  OdDgElementOverrule();
  virtual OdResult open(OdDgElement* pSubject, OdDg::OpenMode mode);
  virtual OdResult close(OdDgElement* pSubject);
  virtual OdResult erase(OdDgElement* pSubject, bool erasing);
  virtual OdDgElementPtr deepClone(const OdDgElement* pSubject, OdDgIdMapping& idMap);
  virtual OdDgElementPtr wblockClone(const OdDgElement* pSubject, OdDgIdMapping& idMap);
};

/** \details
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgOsnapOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgOsnapOverrule);

  OdDgOsnapOverrule();

  virtual OdResult getOsnapPoints(
    const OdDgElement* pSubject,
    OdDgElement::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints );

  virtual OdResult getOsnapPoints(
    const OdDgElement* pSubject,
    OdDgElement::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints,
    const OdGeMatrix3d& insertionMat);

  virtual bool isContentSnappable(const OdDgElement* pSubject);
};

/** \details
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTransformOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTransformOverrule);
  OdDgTransformOverrule();
  virtual OdResult transformBy(OdDgElement* pSubject, const OdGeMatrix3d& xform);
  virtual OdResult getTransformedCopy(const OdDgElement* pSubject, const OdGeMatrix3d& xform, OdDgElementPtr& pEnt);
  virtual OdResult explode(const OdDgElement* pSubject, OdRxObjectPtrArray& entitySet);
  virtual bool cloneMeForDragging(OdDgElement* pSubject);
  virtual bool hideMeForDragging(const OdDgElement* pSubject);
};

/** \details
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgGripOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgGripOverrule);

  OdDgGripOverrule();
  virtual OdResult getGripPoints( const OdDgElement* pSubject, OdGePoint3dArray& gripPoints );

  virtual OdResult getGripPoints(const OdDgElement* pSubject, 
    OdDgGripDataPtrArray& gripsData,
    const double curViewUnitSize,
    const int gripSize,
    const OdGeVector3d& curViewDir,
    const int bitFlags );

  virtual OdResult moveGripPointsAt(OdDgElement* pSubject, 
    const OdIntArray& indices,
    const OdGeVector3d& offset );

  virtual OdResult moveGripPointsAt(OdDgElement* pSubject,
    const OdDgVoidPtrArray& grips,
    const OdGeVector3d& offset,
    int bitFlags );

  virtual OdResult getStretchPoints(const OdDgElement* pSubject,
    OdGePoint3dArray& stretchPoints);

  virtual OdResult moveStretchPointsAt(OdDgElement* pSubject,
    const OdIntArray & indices,
    const OdGeVector3d& offset);

  virtual void gripStatus(OdDgElement* pSubject, const OdDgElement::GripStat status);
};

/** \details
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgHighlightOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgHighlightOverrule);
  OdDgHighlightOverrule();
  virtual void highlight(const OdDgElement* pSubject, bool bDoIt = true, const OdDgFullSubentPath* pSubId = 0,
    bool highlightAll = false);
};

/** \details

    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgVisibilityOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgVisibilityOverrule);
  OdDgVisibilityOverrule();
  virtual bool getInvisibleFlag(const OdDgGraphicsElement* pSubject);
  virtual void setInvisibleFlag(OdDgGraphicsElement* pSubject, bool);
};

/** \details
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgGeometryOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgGeometryOverrule);

  OdDgGeometryOverrule();

  /** \details
    Overrules OdDgGraphicsElement::subIntersectWith() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pThisElm [in]  A raw pointer to an entity.
    \param pElm [in]  An entity with which this entity is intersected.
    \param intType [in]  Type of intersection.
    \param points [out]  An array of points in which the intersection points should be appended.
  */
  virtual OdResult intersectWith(const OdDgGraphicsElement* pThisElm,
    const OdDgGraphicsElement* pElm, OdDg::OdDgIntersectType intType,
    OdGePoint3dArray& points);

  /** \details
    Overrules OdDgGraphicsElement::subIntersectWith() method and calls it in the default implementation.
    This method returns eOk if successful.

    \param pThisElm [in]  A raw pointer to an entity.
    \param pElm [in]  An entity with which this entity is intersected.
    \param intType [in]  Type of intersection.
    \param projPlane [in]  A projection plane for the intersection of the two entities.
    \param points [out]  An array of points in which the intersection points should be appended.
  */
  virtual OdResult intersectWith(const OdDgGraphicsElement* pThisElm,
    const OdDgGraphicsElement* pElm, OdDg::OdDgIntersectType intType,
    const OdGePlane& projPlane, OdGePoint3dArray& points);

  virtual OdResult getGeomExtents(const OdDgElement* pSubject, OdGeExtents3d& extents);
  virtual OdResult getGeomExtents(const OdDgElement* pSubject, const OdDgElementId& idView, OdGeExtents3d& extents);
};

/** \details
    
    \sa
    TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgPropertiesOverrule : public OdRxOverrule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPropertiesOverrule);

  OdDgPropertiesOverrule();

  virtual void list(const OdDgGraphicsElement* pSubject);
};
#endif //_DG_ELEMENTOVRRULE_H_INCLUDED_
