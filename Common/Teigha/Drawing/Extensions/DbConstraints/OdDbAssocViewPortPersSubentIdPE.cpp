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
#include "DbViewport.h"
#include "OdDbAssocViewPortPersSubentIdPE.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocViewPortPersSubentIdPE, OdDbAssocPersSubentIdPE, L"AcUndefined");

////}

OdResult OdDbAssocViewPortPersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
  const OdDbSubentId& edgeSubentId,
  OdGeCurve3d*&   pEdgeCurve)
{
  OdDbViewportPtr pViewport = OdDbViewport::cast(pEntity);

  if (!pViewport.isNull())
  {
    return OdDbAssocPersSubentIdPE::getEdgeSubentityGeometry(pEntity, edgeSubentId, pEdgeCurve);
  //  OdDbCompoundObjectIdPtr resConpoundObj;
  //  OdDbObjectIdArray fullPath;

  //  m_assocGeomDependency->getDependentOnCompoundObject(resConpoundObj);
  //  resConpoundObj->getFullPath(fullPath);
  //  OdDbObjectId pPathID = 0;
  //  OdDbEntityPtr pSubEnt;
  //  OdResult res = eBadObjType;

  //  if (fullPath.length() > 1)
  //  {
  //    pPathID = fullPath[1];
  //    pSubEnt = OdDbEntity::cast(pPathID.openObject());
  //  }

  //  if (pSubEnt.isNull())
  //    return res;

  //  OdDbAssocPersSubentIdPEPtr pPE = pSubEnt->isA()->getX(OdDbAssocPersSubentIdPE::desc());
  //  pPE->setAssocGeomDependency(m_assocGeomDependency);
  //  return pPE->getEdgeSubentityGeometry(pSubEnt, edgeSubentId, pEdgeCurve);
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocViewPortPersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
  const OdDbSubentId& edgeSubentId,
  const OdDbImpAssocGeomDependency* assocVPGeomDependency,
  OdGeCurve3d*&   pEdgeCurve)
{
  OdDbViewportPtr pViewport = OdDbViewport::cast(pEntity);

  if (!pViewport.isNull())
  {
    OdDbCompoundObjectIdPtr resConpoundObj;
    OdDbObjectIdArray fullPath;

    if (assocVPGeomDependency == NULL)
      return OdDbAssocPersSubentIdPE::getEdgeSubentityGeometry(pEntity, edgeSubentId, pEdgeCurve);

    assocVPGeomDependency->getDependentOnCompoundObject(resConpoundObj);
    resConpoundObj->getFullPath(fullPath);
    OdDbObjectId pPathID = 0;
    OdDbEntityPtr pSubEnt;
    OdResult res = eBadObjType;

    if (fullPath.length() > 1)
    {
      pPathID = fullPath[1];
      pSubEnt = OdDbEntity::cast(pPathID.openObject());
    }

    if (pSubEnt.isNull())
      return res;

    OdDbAssocPersSubentIdPEPtr pPE = pSubEnt->isA()->getX(OdDbAssocPersSubentIdPE::desc());
    //pPE->setAssocGeomDependency(m_assocGeomDependency);
    return pPE->getEdgeSubentityGeometry(pSubEnt, edgeSubentId, assocVPGeomDependency, pEdgeCurve);
  }
  else
    return eBadObjType;

}

OdResult OdDbAssocViewPortPersSubentIdPE::getTransientSubentIds(const OdDbEntity* pEntity,
  const OdDbAssocPersSubentId* pPerSubentId,
  OdArray<OdDbSubentId>& subents) const
{
  subents.append(OdDbSubentId(OdDb::kEdgeSubentType, 1));
  return eOk;
}
