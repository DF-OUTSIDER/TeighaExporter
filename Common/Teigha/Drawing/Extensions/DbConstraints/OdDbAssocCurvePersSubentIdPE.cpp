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
#include "OdDbAssocCurvePersSubentIdPE.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocCurvePersSubentIdPE, OdDbAssocPersSubentIdPE, L"AcDbAssocCurvePersSubentIdPE");

OdResult OdDbAssocCurvePersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity, 
                                                             const OdDbAssocPersSubentId* pPerSubentId,
                                                             OdArray<OdDbSubentId>& subents) const
{
  subents.append(OdDbSubentId(OdDb::kEdgeSubentType, 1));
  return eOk;
}

OdResult OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                            const OdDbSubentId&    edgeSubentId,
                                            OdDbSubentId&          startVertexSubentId,
                                            OdDbSubentId&          endVertexSubentId,
                                            OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  fillEndVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
  getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);

  return eOk;
}

void OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                           const OdDbSubentId&    edgeSubentId,
                                                           OdDbSubentId& startVertexSubentId)
  {
    startVertexSubentId.setType(OdDb::kVertexSubentType);
    startVertexSubentId.setIndex(kNotDefine);
  }

void OdDbAssocCurvePersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                                         const OdDbSubentId&    edgeSubentId,
                                                         OdDbSubentId& startVertexSubentId)
{
  startVertexSubentId.setType(OdDb::kVertexSubentType);
  startVertexSubentId.setIndex(kNotDefine);
}

void OdDbAssocCurvePersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                                           const OdDbSubentId&    edgeSubentId,
                                                           OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  otherVertexSubentIds.clear();
}

bool OdDbAssocCurvePersSubentIdPE::isTypeEqual(const OdDbEntity* pEntity, const OdRxClass * pTypeDesc) const
{
  if ( pEntity == NULL )
    return false;

  return pEntity->isA()->isEqualTo(pTypeDesc);
}
