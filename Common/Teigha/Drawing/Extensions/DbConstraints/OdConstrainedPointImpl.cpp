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

#include "OdaCommon.h"
#include "OdConstrainedPointImpl.h"
#include "OdConstrainedGeometry.h"
#include "DbFiler.h"

#include "DbPoint.h"
#include "DbBlockReference.h"

OdConstrainedPointImpl::OdConstrainedPointImpl():OdConstrainedGeometryImpl(),
                                         m_point(),
                                         m_pointOrig()
{
  m_implObjType = PointImpl;
}

OdGePoint3d OdConstrainedPointImpl::point() const
{
  return m_point;
}

OdResult OdConstrainedPointImpl::init(const OdDbObjectId depId)
{
  OdResult res = OdConstrainedGeometryImpl::init(depId);
  if ( res != eOk )
    return res;

  OdDbAssoc2dConstraintGroupPtr pGr = m_groupObjectId.openObject();
  OdGeMatrix3d matrFromWrd = OdGeMatrix3d::worldToPlane(pGr->getWorkPlane());
  setDataFromGeomDependency(depId, matrFromWrd);

  m_origGeomSet = false;
  return eOk;
}

void OdConstrainedPointImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedGeometryImpl::dwgOutFields(pFiler);
  if ( !geomDependencyId().isNull() )
    pFiler->wrPoint3d(m_point);
}

OdResult OdConstrainedPointImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedGeometryImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  if ( !geomDependencyId().isNull() )
    m_point = pFiler->rdPoint3d();
  return res;
}

void OdConstrainedPointImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedGeometryImpl::dxfOutFields(pFiler);
  if ( !geomDependencyId().isNull() )
    pFiler->wrPoint3d(10, m_point);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedPointImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedGeometryImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  if ( !geomDependencyId().isNull() )
  {
    NEXT_CODE(10)
    pFiler->rdPoint3d(m_point);
  }

  return res;
}

#undef NEXT_CODE

bool OdConstrainedPointImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  if ( getType(geom) != m_implObjType )
    return false;

  OdConstrainedPointImpl* sec = (OdConstrainedPointImpl*)&geom;

  return point().isEqualTo(sec->point());
}

bool OdConstrainedPointImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}

OdResult OdConstrainedPointImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  OdConstrainedGeometryImpl::setDataFromGeomDependency(geomDependencyId, matrFromWrd);

  OdGeMatrix3d blockToWrd;
  matrToWrdCoord(geomDependencyId, blockToWrd);
  OdDbObjectPtr objPtr = geomObject(geomDependencyId);

  if ( objPtr->isKindOf(OdDbPoint::desc()) )
  {
    OdDbPointPtr pointPtr = OdDbPoint::cast(objPtr);
    OdGePoint3d point(pointPtr->position());

    point.transformBy(blockToWrd);
    point.transformBy(matrFromWrd);

    m_pointOrig.set(point.x, point.y, point.z);
    m_origGeomSet = true;

    setPoint(point);
  }
  else if ( objPtr->isKindOf(OdDbBlockReference::desc()) )
  {
    OdDbBlockReferencePtr blockPtr = OdDbBlockReference::cast(objPtr);
    OdGePoint3d point(blockPtr->position());

    point.transformBy(blockToWrd);
    point.transformBy(matrFromWrd);

    m_pointOrig.set(point.x, point.y, point.z);
    m_origGeomSet = true;

    setPoint(point);
  }
  
  m_idDependency = geomDependencyId;

  return eOk;
}

OdResult OdConstrainedPointImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( OdConstrainedGeometryImpl::updateDbObject(pObj, pDep, matrToWrd) == eOk )
    return eOk;

  m_origGeomSet = false;

  if ( pObj->isKindOf(OdDbPoint::desc()) )
  {
    OdGePoint3d pointWrd(point());
    pointWrd.transformBy(matrToWrd);

    OdDbPointPtr pPoint = OdDbPoint::cast(pObj);
    pPoint->setPosition(pointWrd);

    return eOk;
  }
  else if ( pObj->isKindOf(OdDbBlockReference::desc()) )
  {
    OdGePoint3d pointWrd(point());
    pointWrd.transformBy(matrToWrd);

    OdDbBlockReferencePtr blockPtr = OdDbBlockReference::cast(pObj);
    blockPtr->setPosition(pointWrd);

    return eOk;
  }
   
  
  return eOk;
}

OdResult OdConstrainedPointImpl::setPoint(const OdGePoint3d& val)
{
  if ( val == m_point )
    return eOk;

  if ( !m_origGeomSet )
  {
    m_pointOrig.set(m_point.x, m_point.y, m_point.z);
    m_origGeomSet = true;
  }

  m_point.set(val.x, val.y, val.z);

  return eOk;
}

const OdGePoint3d OdConstrainedPointImpl::getOriginalPoint() const
{
  if ( m_origGeomSet )
    return m_pointOrig;
  else
    return m_point;
}

OdResult OdConstrainedPointImpl::getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const
{
  OdConstrainedGeometryImpl::getFullSubentPaths(subentPaths);
  OdDbFullSubentPath path;

  OdResult res = getFullSubentPath(path);
  if ( res  != eOk )
    return res;

  OdDbAssocGeomDependencyPtr pGeomDep = geomDependency(geomDependencyId());
  const OdDbAssocPersSubentId *pPersSubentId = pGeomDep->persistentSubentId();

  if ( pPersSubentId ->isKindOf(OdDbAssocSimplePersSubentId::desc()) )
  {
    const OdDbObjectIdArray aObj = path.objectIds();
    OdDbAssocSimplePersSubentId* pSimplePersSubentId = OdDbAssocSimplePersSubentId::cast(pPersSubentId);
    OdDbEntityPtr pEnt = aObj[aObj.length() - 1].openObject();
    OdDbSubentId subentId = pSimplePersSubentId->subentId(pEnt);

    path.subentId().setType(subentId.type());
    path.subentId().setIndex(subentId.index());

    subentPaths.push_back(path);
    return eOk;
  }
  return eNotImplementedYet;
}
