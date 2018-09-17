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
#include "DbEntity.h"
#include "DbPoint.h"
#include "OdDbAssocPointPersSubentIdPE.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocPointBasedPersSubentIdPE, OdDbAssocPersSubentIdPE, L"AcDbAssocPointBasedPersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocPointBasedPersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const class OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  OdDbAssocSimplePersSubentIdPtr res;
  if ( !isTypeEqual(pEntity, OdDbPoint::desc()) || !checkSubentId(subentId) )
    res.attach((OdDbAssocSimplePersSubentId*)NULL);
  else
  {
    res = OdDbAssocSimplePersSubentId::createObject();
    res->init(subentId);
  }
  return res;
}

OdResult OdDbAssocPointBasedPersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity, 
                                                             const OdDbAssocPersSubentId* pPerSubentId,
                                                             OdArray<OdDbSubentId>& subents) const
{
  subents.append(OdDbSubentId(OdDb::kVertexSubentType, -1));
  return eOk;
}

OdResult OdDbAssocPointBasedPersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  if ( subentType == OdDb::kVertexSubentType && isTypeEqual(pEntity, OdDbPoint::desc()) )
  {
    
    allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)-1));
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocPointBasedPersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                const OdDbSubentId&    edgeSubentId,
                                                                OdDbSubentId&          startVertexSubentId,
                                                                OdDbSubentId&          endVertexSubentId,
                                                                OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  fillEndVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);

  return eNotImplementedYet;
}

void OdDbAssocPointBasedPersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
  const OdDbSubentId&    edgeSubentId,
  OdDbSubentId& startVertexSubentId)
{
  startVertexSubentId.setType(OdDb::kVertexSubentType);
  startVertexSubentId.setIndex(kNotDefine);
}

void OdDbAssocPointBasedPersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
  const OdDbSubentId&    edgeSubentId,
  OdDbSubentId& startVertexSubentId)
{
  startVertexSubentId.setType(OdDb::kVertexSubentType);
  startVertexSubentId.setIndex(kNotDefine);
}

void OdDbAssocPointBasedPersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
  const OdDbSubentId&    edgeSubentId,
  OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  otherVertexSubentIds.clear();
}

OdResult OdDbAssocPointBasedPersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(vertexSubentId) && isTypeEqual(pEntity, OdDbPoint::desc()) )
  {
    OdDbPointPtr pPt = OdDbPoint::cast(pEntity);
    vertexPosition = pPt->position();
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocPointBasedPersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(vertexSubentId) && isTypeEqual(pEntity, OdDbPoint::desc()) )
  {
    OdDbPointPtr pPt = OdDbPoint::cast(pEntity);
    pPt->setPosition(newVertexPosition);
    return eOk;
  }
  else
    return eBadObjType;
}


OdResult OdDbAssocPointBasedPersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity*   pNewSubentityGeometry)
{
  if ( checkSubentId(subentId) && isTypeEqual(pEntity, OdDbPoint::desc()) &&
                  isTypeEqual(pNewSubentityGeometry, OdDbPoint::desc()) )
  {
    OdDbPointPtr pPtDst = OdDbPoint::cast(pEntity);
    OdDbPointPtr pPtSrc = OdDbPoint::cast(pNewSubentityGeometry);
    pPtDst->setPosition(pPtSrc->position());
    return eOk;
  }
  else
    return eBadObjType;
}

bool OdDbAssocPointBasedPersSubentIdPE::checkSubentId(const OdDbSubentId& subentId) const
{
  return subentId.type() == OdDb::kVertexSubentType && subentId.index() == -1;
}

bool OdDbAssocPointBasedPersSubentIdPE::isTypeEqual(const OdDbEntity* pEntity, const OdRxClass * pTypeDesc) const
{
  if (pEntity == NULL)
    return false;

  return pEntity->isA()->isEqualTo(pTypeDesc);
}
