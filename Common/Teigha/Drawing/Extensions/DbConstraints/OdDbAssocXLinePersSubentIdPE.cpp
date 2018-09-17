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
#include "DbXline.h"
#include "OdDbAssocXLinePersSubentIdPE.h"
#include "Ge/GeLine3d.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocXLinePersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocXLinePersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocXLinePersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const class OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  OdDbAssocSingleEdgePersSubentIdPtr res;
  if ( !isTypeEqual(pEntity, OdDbXline::desc()) || !checkSubentId(subentId) )
    res.attach((OdDbAssocSingleEdgePersSubentId*)NULL);
  else
  {
    res = OdDbAssocSingleEdgePersSubentId::createObject();
  }
  return res;
}

OdResult OdDbAssocXLinePersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  if ( subentType == OdDb::kEdgeSubentType && isTypeEqual(pEntity, OdDbXline::desc()) )
  {
    
    allSubentIds.append(OdDbSubentId(OdDb::kEdgeSubentType, (OdGsMarker)1));
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocXLinePersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                const OdDbSubentId&    edgeSubentId,
                                                                OdDbSubentId&          startVertexSubentId,
                                                                OdDbSubentId&          endVertexSubentId,
                                                                OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return eNotImplementedYet;
}

OdResult OdDbAssocXLinePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                OdGeCurve3d*&       pEdgeCurve)
{
  if ( checkSubentId(edgeSubentId) && isTypeEqual(pEntity, OdDbXline::desc()) )
  {
    OdDbXlinePtr pLine = OdDbXline::cast(pEntity);
    pEdgeCurve = new OdGeLine3d(pLine->basePoint(), pLine->unitDir());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocXLinePersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(edgeSubentId) && isTypeEqual(pEntity, OdDbXline::desc()) && pNewEdgeCurve->isKindOf(OdGe::kLine3d) )
  {
    OdGeLine3d* pGeLine = (OdGeLine3d*)pNewEdgeCurve;
    OdDbXlinePtr pLine = OdDbXline::cast(pEntity);
    pLine->setUnitDir(pGeLine->direction());
    pLine->setBasePoint(pGeLine->pointOnLine());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocXLinePersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d&        vertexPosition)
{
  return eBadObjType;
}

OdResult OdDbAssocXLinePersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d&  newVertexPosition)
{
  return eBadObjType;
}


OdResult OdDbAssocXLinePersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity*   pNewSubentityGeometry)
{
  if ( checkSubentId(subentId) && isTypeEqual(pEntity, OdDbXline::desc()) &&
                  isTypeEqual(pNewSubentityGeometry, OdDbXline::desc()) )
  {
    OdDbXlinePtr pLineDst = OdDbXline::cast(pEntity);
    OdDbXlinePtr pLineSrc = OdDbXline::cast(pNewSubentityGeometry);
    pLineDst->setUnitDir(pLineSrc->unitDir());
    pLineDst->setBasePoint(pLineSrc->basePoint());

    return eOk;
  }
  else
    return eBadObjType;
}

bool OdDbAssocXLinePersSubentIdPE::checkSubentId(const OdDbSubentId& subentId) const
{
  return subentId.type() == OdDb::kEdgeSubentType && subentId.index() == 1;
}
