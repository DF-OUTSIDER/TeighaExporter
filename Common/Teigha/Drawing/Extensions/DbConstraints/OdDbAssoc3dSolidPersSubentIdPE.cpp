///////////////////////////////////////////////////////////////////////////////// 
//// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
//// All rights reserved. 
//// 
//// This software and its documentation and related materials are owned by 
//// the Alliance. The software may only be incorporated into application 
//// programs owned by members of the Alliance, subject to a signed 
//// Membership Agreement and Supplemental Software License Agreement with the
//// Alliance. The structure and organization of this software are the valuable  
//// trade secrets of the Alliance and its suppliers. The software is also 
//// protected by copyright law and international treaty provisions. Application  
//// programs incorporating this software must include the following statement 
//// with their copyright notices:
////   
////   This application incorporates Teigha(R) software pursuant to a license 
////   agreement with Open Design Alliance.
////   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
////   All rights reserved.
////
//// By use of this software, its documentation or related materials, you 
//// acknowledge and accept the above terms.
/////////////////////////////////////////////////////////////////////////////////
#include "OdaCommon.h"
#include "DbEntity.h"
#include "Db3dSolid.h"
#include "OdDbAssoc3dSolidPersSubentIdPE.h"
#include <ModelerGeometry.h>
#include "DbAssocEdgePersSubentId.h"
#include "ShHistory/DbAssocPersSubentManager.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssoc3dSolidPersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssoc3dSolidPersSubentIdPE");

//OdDbAssocPersSubentIdPtr OdDbAssoc3dSolidPersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
//  const class OdDbCompoundObjectId& compId,
//  const OdDbSubentId& subentId)
//{
//  OdDbAssocEdgePersSubentIdPtr res;
//  if (!isTypeEqual(pEntity, OdDbPolyline::desc()) || !checkSubentId(pEntity, subentId, OdDb::kEdgeSubentType))
//    res.attach((OdDbAssocEdgePersSubentId*)NULL);
//  else
//  {
//    OdDbPolylinePtr pPolyLine = OdDbPolyline::cast(pEntity);
//    int ptQty = pPolyLine->numVerts();
//    int maxIdentifier = 0;
//    for (int i = 0; i < ptQty; i++)
//    {
//      if (pPolyLine->getVertexIdentifierAt(i) > maxIdentifier)
//        maxIdentifier = pPolyLine->getVertexIdentifierAt(i);
//    }
//
//    int startVertexIdx = (int)subentId.index() - 1;
//    int endVertexIdx = startVertexIdx + 1;
//    if (endVertexIdx >= ptQty)
//      endVertexIdx = 0;
//
//    if (pPolyLine->getVertexIdentifierAt(startVertexIdx) == 0)
//    {
//      pPolyLine->setVertexIdentifierAt(startVertexIdx, ++maxIdentifier);
//    }
//
//    if (pPolyLine->getVertexIdentifierAt(endVertexIdx) == 0)
//    {
//      pPolyLine->setVertexIdentifierAt(endVertexIdx, ++maxIdentifier);
//    }
//
//    res = OdDbAssocEdgePersSubentId::createObject();
//    res->init(pPolyLine->getVertexIdentifierAt(startVertexIdx), pPolyLine->getVertexIdentifierAt(endVertexIdx));
//  }
//
//  return res;
//}

OdResult OdDbAssoc3dSolidPersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity,
  const OdDbAssocPersSubentId* pPerSubentId,
  OdArray<OdDbSubentId>& subents) const
{
  const OdDb3dSolidPtr pSolid = OdDb3dSolid::cast(pEntity);
  const OdDbAssocAsmBasedEntityPersSubentIdPtr pAsmPerSubentId = OdDbAssocAsmBasedEntityPersSubentId::cast(pPerSubentId);
  const OdDbAssocIndexPersSubentIdPtr pIndexSubentId = OdDbAssocIndexPersSubentId::cast(pPerSubentId);

  if (pSolid.isNull())
    return eWrongObjectType;
  
  OdDbSubentId currentSubentId;

  if (!pAsmPerSubentId.isNull())
  {
    currentSubentId = pAsmPerSubentId->subentId(NULL);
  }

  if (!pIndexSubentId.isNull())
  {
    currentSubentId = pIndexSubentId->subentId(NULL);
  }

  subents.clear();

  if ( checkSubentId(pEntity, currentSubentId, pPerSubentId->subentType(pEntity)))
  {
    unsigned int internalIdx = currentSubentId.index();
    subents.append(OdDbSubentId(OdDb::kEdgeSubentType, internalIdx));
  }

  return (subents.isEmpty()) ? eInvalidInput : eOk;
}

OdResult OdDbAssoc3dSolidPersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity* pEntity,
  const OdDbSubentId& edgeSubentId,
  OdGeCurve3d*&  pEdgeCurve)
{
  OdResult res = eInvalidFaceVertexIndex;
  const OdDbDatabase* pDb = pEntity->database();
  OdDbObjectId objId = OdDbAssocPersSubentManager::getInstanceFromDatabase(pDb);
  OdDbAssocPersSubentManagerPtr pAssocPSManager = objId.openObject(/*OdDb::kForWrite*/);

  if (checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType) && !pAssocPSManager.isNull())// && isTypeEqual(pEntity, OdDbCircle::desc()))
  {
    OdUInt32 idx = (unsigned int)edgeSubentId.index();
    OdArray<OdUInt32> indexesOfEdgeCalculation = pAssocPSManager->infoAcisDataIndexes(idx);

    OdDb3dSolidPtr pSolid(pEntity); // more types here

    OdModelerGeometryPtr pModeler = getModelerGeometry(pSolid);

    if (pModeler.isNull())
      return eGeneralModelingFailure;

    pSolid->assertWriteEnabled();

    res = pModeler->getEdgeSubentityGeometry(indexesOfEdgeCalculation, pEdgeCurve);
  }

  return res;
}

bool OdDbAssoc3dSolidPersSubentIdPE::checkSubentId(const OdDbEntity* pEntity, const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if (!isTypeEqual(pEntity, OdDb3dSolid::desc()) || type != OdDb::kEdgeSubentType)
    return false;

  const OdDb3dSolidPtr pSolid = OdDb3dSolid::cast(pEntity);
  unsigned int idx = (unsigned int)subentId.index();

  return idx > 1 && !pSolid.isNull();
}
