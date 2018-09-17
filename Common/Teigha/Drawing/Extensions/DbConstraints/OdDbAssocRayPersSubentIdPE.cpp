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
#include "DbRay.h"
#include "OdDbAssocRayPersSubentIdPE.h"
#include "Ge/GeRay3d.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocRayPersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocRayPersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocRayPersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const class OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  OdDbAssocSingleEdgePersSubentIdPtr res;
  if ( !isTypeEqual(pEntity, OdDbRay::desc()) || !checkSubentId(subentId, OdDb::kEdgeSubentType) )
    res.attach((OdDbAssocSingleEdgePersSubentId*)NULL);
  else
  {
    res = OdDbAssocSingleEdgePersSubentId::createObject();
  }
  return res;
}

OdResult OdDbAssocRayPersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  if ( !isTypeEqual(pEntity, OdDbRay::desc()) )
    return eBadObjType;

  if ( subentType == OdDb::kEdgeSubentType )
  {
    
    allSubentIds.append(OdDbSubentId(OdDb::kEdgeSubentType, (OdGsMarker)1));
    return eOk;
  }
  else if ( subentType == OdDb::kVertexSubentType )
  {
    OdDbSubentId startId;
    allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kStart));
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocRayPersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                const OdDbSubentId&    edgeSubentId,
                                                                OdDbSubentId&          startVertexSubentId,
                                                                OdDbSubentId&          endVertexSubentId,
                                                                OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return startVertexSubentId.index() != kNotDefine ? eBadObjType : eOk;
}

OdResult OdDbAssocRayPersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                OdGeCurve3d*&       pEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbRay::desc()) )
  {
    OdDbRayPtr pRay = OdDbRay::cast(pEntity);
    pEdgeCurve = new OdGeRay3d(pRay->basePoint(), pRay->unitDir());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocRayPersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbRay::desc())
      && pNewEdgeCurve->isKindOf(OdGe::kRay3d) )
  {
    OdGeRay3d* pGeRay = (OdGeRay3d*)pNewEdgeCurve;
    OdDbRayPtr pRay = OdDbRay::cast(pEntity);
    pRay->setUnitDir(pGeRay->direction());
    pRay->setBasePoint(pGeRay->evalPoint(0.0));
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocRayPersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbRay::desc()) )
  {
    OdDbRayPtr pRay = OdDbRay::cast(pEntity);
    vertexPosition = pRay->basePoint();
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocRayPersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbRay::desc()) )
  {
    OdDbRayPtr pRay = OdDbRay::cast(pEntity);
    pRay->setBasePoint(newVertexPosition);
    return eOk;
  }
  else
    return eBadObjType;
}


OdResult OdDbAssocRayPersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity*   pNewSubentityGeometry)
{
  if ( !isTypeEqual(pEntity, OdDbRay::desc()) || !isTypeEqual(pNewSubentityGeometry, OdDbRay::desc()) )
    return eBadObjType;

  OdDbRayPtr pRayDst = OdDbRay::cast(pEntity);
  OdDbRayPtr pRaySrc = OdDbRay::cast(pNewSubentityGeometry);

  if ( checkSubentId(subentId, OdDb::kEdgeSubentType) )
  {
    pRayDst->setUnitDir(pRaySrc->unitDir());
    pRayDst->setBasePoint(pRaySrc->basePoint());

    return eOk;
  }
  else if ( checkSubentId(subentId, OdDb::kVertexSubentType) )
  {
    pRayDst->setBasePoint(pRaySrc->basePoint());

    return eOk;
  }
  else
    return eBadObjType;
}

void OdDbAssocRayPersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                         const OdDbSubentId&    edgeSubentId,
                                                         OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType ) )
    startVertexSubentId.setIndex(kStart);
}

bool OdDbAssocRayPersSubentIdPE::checkSubentId(const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if ( type == OdDb::kEdgeSubentType )
  {
    return subentId.index() == 1;
  }
  else if ( type == OdDb::kVertexSubentType )
  {
    return subentId.index() == kStart;
  }
  else
    return false;
}
