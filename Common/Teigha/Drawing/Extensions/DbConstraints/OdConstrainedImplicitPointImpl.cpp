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
#include "OdConstrainedImplicitPointImpl.h"
#include "OdConstrainedCurveImpl.h"
#include "DbAssoc2dConstraintGroup.h"

#include "OdDbAssocCurvePersSubentIdPE.h"

#include "DbFiler.h"

OdConstrainedImplicitPointImpl::OdConstrainedImplicitPointImpl()
  : OdConstrainedPointImpl()
  , m_type(OdConstrainedImplicitPoint::kStartImplicit)
  , m_index(-1)
  , m_curveId(OdConstraintGroupNode::kNullGroupNodeId)
{
  m_implObjType = ImplicitPointImpl;
}

OdConstrainedImplicitPointImpl::~OdConstrainedImplicitPointImpl()
{

}

OdResult OdConstrainedImplicitPointImpl::init(OdConstraintGroupNodeId constrCurvId, OdConstrainedImplicitPoint::ImplicitPointType ptype, int index)
{
  m_type = ptype;
  m_index = index;
  m_curveId = constrCurvId;

  return eOk;
}

OdConstrainedImplicitPoint::ImplicitPointType OdConstrainedImplicitPointImpl::pointType()  const
{
  return (OdConstrainedImplicitPoint::ImplicitPointType)m_type;
}

OdInt32 OdConstrainedImplicitPointImpl::pointIndex() const
{
  return (OdConstrainedImplicitPoint::ImplicitPointType)m_index;
}

OdConstraintGroupNodeId OdConstrainedImplicitPointImpl::constrainedCurveId() const
{
  return m_curveId;
}

OdGePoint3d OdConstrainedImplicitPointImpl::point() const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  OdConstraintGroupNode* pCurve = pGr->getGroupNodePtr(constrainedCurveId());
  if (!pCurve)
    return OdGePoint3d();

  OdConstraintGroupNodeImpl *curveImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCurve);

  OdGePoint3d res;
  if ( ((OdConstrainedCurveImpl*)curveImpl)->point(this, res) == eOk )
    return res;

  return OdGePoint3d();
}

OdResult OdConstrainedImplicitPointImpl::setPoint(const OdGePoint3d& val)
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  OdConstraintGroupNode* pCurve = pGr->getGroupNodePtr(constrainedCurveId());

  OdConstraintGroupNodeImpl *curveImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCurve);
  return ((OdConstrainedCurveImpl*)curveImpl)->setPoint(this, val);
}

bool OdConstrainedImplicitPointImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  if ( getType(geom) != ImplicitPointImpl )
    return false;

  OdConstrainedImplicitPointImpl* sec = (OdConstrainedImplicitPointImpl*)&geom;

  return pointType() == sec->pointType() &&
         constrainedCurveId() == sec->constrainedCurveId() &&
         pointType() == OdConstrainedImplicitPoint::kDefineImplicit ? (pointIndex() == sec->pointIndex()) : true;
}

bool OdConstrainedImplicitPointImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}

void OdConstrainedImplicitPointImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedPointImpl::dwgOutFields(pFiler);
  pFiler->wrUInt8((OdUInt8)m_type);
  pFiler->wrInt32(m_index);
  pFiler->wrInt32(m_curveId);
}

OdResult OdConstrainedImplicitPointImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedPointImpl::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  m_type = static_cast<OdConstrainedImplicitPoint::ImplicitPointType>(pFiler->rdUInt8());
  m_index = pFiler->rdInt32();
  m_curveId = pFiler->rdInt32();
  return eOk;
}

void OdConstrainedImplicitPointImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedPointImpl::dxfOutFields(pFiler);
  pFiler->wrUInt8(280, (OdUInt8)m_type);
  pFiler->wrInt32(90, m_index);
  pFiler->wrUInt32(90, m_curveId);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedImplicitPointImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedPointImpl::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  NEXT_CODE(280)
  m_type = static_cast<OdConstrainedImplicitPoint::ImplicitPointType>(pFiler->rdUInt8());
  NEXT_CODE(90)
  m_index = pFiler->rdInt32();
  NEXT_CODE(90)
  m_curveId = pFiler->rdUInt32();
  return eOk;
}

#undef NEXT_CODE

OdResult OdConstrainedImplicitPointImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId, const OdGeMatrix3d& matrFromWrd)
{
  return eOk;
}

OdResult OdConstrainedImplicitPointImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  return eOk;
}

const OdGePoint3d OdConstrainedImplicitPointImpl::getOriginalPoint() const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  OdConstraintGroupNode* pCurve = pGr->getGroupNodePtr(constrainedCurveId());

  OdConstraintGroupNodeImpl *curveImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCurve);
  OdGePoint3d res;
  if ( ((OdConstrainedCurveImpl*)curveImpl)->getOriginalPoint(this, res) == eOk )
    return res;

  return OdGePoint3d();
}

OdResult OdConstrainedImplicitPointImpl::getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  OdConstraintGroupNode* pGrNode = pGr->getGroupNodePtr(constrainedCurveId());
  OdConstrainedCurve *pCurve = OdConstrainedCurve::cast(pGrNode);

  if ( !pCurve ) {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }
  OdResult res = pCurve->getFullSubentPaths(subentPaths);
  if ( res != eOk ) {
    ODA_FAIL_ONCE();
    return res;
  }

  OdDbFullSubentPath *path = subentPaths.asArrayPtr();

  OdDbObjectIdArray aObjId = path->objectIds();
  OdDbObjectPtr pObj = aObjId.last().openObject();
  path->subentId().setType(OdDb::kVertexSubentType);
  if ( !pObj->isKindOf(OdDbPolyline::desc()) )
  {
    switch ( m_type )
    {
      case OdConstrainedImplicitPoint::kStartImplicit:
        path->subentId().setIndex(OdDbAssocCurvePersSubentIdPE::kStart);
      break;
      case OdConstrainedImplicitPoint::kEndImplicit:
        path->subentId().setIndex(OdDbAssocCurvePersSubentIdPE::kEnd);
      break;
      case OdConstrainedImplicitPoint::kMidImplicit:
        path->subentId().setIndex(OdDbAssocCurvePersSubentIdPE::kMiddle);
      break;
      case OdConstrainedImplicitPoint::kCenterImplicit:
        path->subentId().setIndex(OdDbAssocCurvePersSubentIdPE::kCenter);
      break;
      case OdConstrainedImplicitPoint::kDefineImplicit:
        path->subentId().setIndex(pointIndex());
      break;
    }
  }
  else
  {
    OdDbPolylinePtr pPolyline = pObj;
    unsigned int index = (unsigned int) path->subentId().index();
    unsigned int nextIndex = (index >= pPolyline->numVerts()) ? 1 : index + 1;
    switch ( m_type )
    {
      case OdConstrainedImplicitPoint::kStartImplicit:
        path->subentId().setIndex(index);
      break;
      case OdConstrainedImplicitPoint::kEndImplicit:
        path->subentId().setIndex(nextIndex);
      break;
      case OdConstrainedImplicitPoint::kMidImplicit:
        path->subentId().setIndex(index + OdDbAssocCurvePersSubentIdPE::kMiddlePLinePrefix);
      break;
      case OdConstrainedImplicitPoint::kCenterImplicit:
        path->subentId().setIndex(index + OdDbAssocCurvePersSubentIdPE::kCenterPLinePrefix);
      break;
    }
  }

  return eOk;
}
