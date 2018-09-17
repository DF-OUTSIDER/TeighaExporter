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
#include "DbBlockReference.h"
#include "OdDbAssocBlockReferncePersSubentIdPE.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocBlockReferncePersSubentIdPE, OdDbAssocPointBasedPersSubentIdPE, L"AcDbAssocBlockReferncePersSubentIdPE");

//OdResult OdDbAssocBlockReferncePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
//  const OdDbSubentId& edgeSubentId,
//  const OdDbImpAssocGeomDependency* assocBlockGeomDependency,
//  OdGeCurve3d*&   pEdgeCurve,
//  OdDbObjectIdArray fullPath,
//  OdUInt32 currentPathIndex 
//  )
//{
//  if (checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType))
//  {
//    OdDbCompoundObjectIdPtr resConpoundObj;
//
//    OdDbObjectId pPathID = 0;
//    OdDbEntityPtr pSubEnt;
//    OdResult res = eBadObjType;
//
//    if (currentPathIndex >= fullPath.length())
//      return eInvalidIndex;
//
//    if (fullPath.length() > 1)
//    //if (fullPath.length() > 0)
//    {
//      pPathID = fullPath[currentPathIndex];
//      pSubEnt = OdDbEntity::cast(pPathID.openObject());
//
//      OdDbViewportPtr pViewport = OdDbViewport::cast(pSubEnt);
//
//      if (!pViewport.isNull())
//      {
//        currentPathIndex++;
//        pPathID = fullPath[currentPathIndex];
//        pSubEnt = OdDbEntity::cast(pPathID.openObject());
//      }
//    }
//
//    if (pSubEnt.isNull())
//      return res;
//
//    OdDbAssocPersSubentIdPEPtr pPE = pSubEnt->isA()->getX(OdDbAssocPersSubentIdPE::desc());
//    if (pPE.isNull())
//    {
//      ODA_ASSERT_ONCE(!"OdDbAssocPersSubentIdPE is null");
//      return eNullObjectPointer;
//    }
//
//    //pPE->setAssocGeomDependency(m_assocGeomDependency);
//    OdArray<OdDbSubentId> arrIds;
//
//    res = pPE->getTransientSubentIds(pSubEnt, (assocBlockGeomDependency == NULL) ? NULL : assocBlockGeomDependency->persistentSubentId().get()
//      , arrIds);
//
//    if (res == eOk)
//    {
//      for (unsigned int i = 0; i < arrIds.length(); ++i)
//      {
//        OdDbAssocBlockReferncePersSubentIdPEPtr pPEBlRef = OdDbAssocBlockReferncePersSubentIdPE::cast(pPE);
//
//        if (pPEBlRef.isNull())
//        {
//          res = pPE->getEdgeSubentityGeometry(pSubEnt, arrIds[i], pEdgeCurve);
//        }
//        else
//        {
//          res = pPEBlRef->getEdgeSubentityGeometry(pSubEnt, edgeSubentId, assocBlockGeomDependency, pEdgeCurve, fullPath, currentPathIndex + 1);
//        }
//
//        if (res != eOk)
//          return res;
//        
//        //OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast(pSubEnt);
//        //
//        //if (!pBlockRef.isNull())
//        //{
//        //  OdGeMatrix3d matrix = pBlockRef->blockTransform();
//        //  pEdgeCurve->transformBy(matrix);
//        //}
//      }
//    }
//
//    ODA_ASSERT(pEdgeCurve != NULL);
//
//    //pPE->setAssocGeomDependency(NULL); //TODO ABT 09222017
//
//    return res;
//  }
//  else
//    return eBadObjType;
//}
//
//OdResult OdDbAssocBlockReferncePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
//  const OdDbSubentId& edgeSubentId,
//  const OdDbImpAssocGeomDependency* assocBlockGeomDependency,
//  OdGeCurve3d*&       pEdgeCurve)
//{
//  if (assocBlockGeomDependency == NULL)
//    return OdDbAssocPointBasedPersSubentIdPE::getEdgeSubentityGeometry(pEntity, edgeSubentId, pEdgeCurve);
//
//  OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast(pEntity);
//
//  if (checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType) && !pBlockRef.isNull())
//  {
//    OdDbCompoundObjectIdPtr resConpoundObj;
//    OdDbObjectIdArray fullPath;
//
//    assocBlockGeomDependency->getDependentOnCompoundObject(resConpoundObj);
//    resConpoundObj->getFullPath(fullPath);
//
//    return getEdgeSubentityGeometry(pEntity, edgeSubentId, assocBlockGeomDependency, pEdgeCurve, fullPath, 1);
//    //return getEdgeSubentityGeometry(pEntity, edgeSubentId, assocBlockGeomDependency, pEdgeCurve, fullPath, 0);
//  }
//  else
//    return eBadObjType;
//}
//
//OdResult OdDbAssocBlockReferncePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
//  const OdDbSubentId& edgeSubentId,
//  OdGeCurve3d*&   pEdgeCurve)
//{
//  return OdDbAssocPointBasedPersSubentIdPE::getEdgeSubentityGeometry(pEntity, edgeSubentId, pEdgeCurve);
//  
//  //OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast(pEntity);
//  //
//  //if (checkSubentId(pEntity, edgeSubentId, OdDb::kEdgeSubentType) && !pBlockRef.isNull())
//  //{
//  //  OdDbCompoundObjectIdPtr resConpoundObj;
//  //  OdDbObjectIdArray fullPath;
//
//  //  m_assocGeomDependency->getDependentOnCompoundObject(resConpoundObj);
//  //  resConpoundObj->getFullPath(fullPath);
//
//  //  return getEdgeSubentityGeometry(pEntity, edgeSubentId, pEdgeCurve, fullPath, 1);
//  //}
//  //else
//  //  return eBadObjType;
//
//}
//
//bool OdDbAssocBlockReferncePersSubentIdPE::checkSubentId(const OdDbEntity* pEntity, const OdDbSubentId& subentId, const OdDb::SubentType type) const
//{
//    return true; //TODO
//}

OdResult OdDbAssocBlockReferncePersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity,
  const OdDbAssocPersSubentId* pPerSubentId,
  OdArray<OdDbSubentId>& subents) const
{
  //m_pPersSubent = pPerSubentId;
  return OdDbAssocPointBasedPersSubentIdPE::getTransientSubentIds(pEntity, pPerSubentId, subents);
}
