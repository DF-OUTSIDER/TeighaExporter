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
#include "DbImpAssocViewRepHatchActionParam.h"
#include "DbFiler.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocViewRepHatchActionParam,    // ClassName
                       OdDbAssocObjectActionParam,          // ParentClass
                       DBOBJECT_CONSTR,                     // DOCREATE
                       OdDb::kDHL_1027,                     // DwgVer
                       OdDb::kMRelease8,                    // MaintVer
                       1025,                                // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocViewRepHatchActionParam", // DWG class name
                       L"ACDBASSOCVIEWREPHATCHACTIONPARAM", // DxfName
                       L"AcSynergyObjDPW",                  // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocViewRepHatchActionParam::OdDbAssocViewRepHatchActionParam(bool createImpObj)
 : OdDbAssocObjectActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocViewRepHatchActionParam();
}

OdResult OdDbAssocViewRepHatchActionParam::dwgInFields(OdDbDwgFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  return m_pImpObj->dwgInFields(pFiler, objectId());
}

void OdDbAssocViewRepHatchActionParam::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  m_pImpObj->dwgOutFields(pFiler, objectId());
}

OdResult OdDbAssocViewRepHatchActionParam::dxfInFields(OdDbDxfFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  return m_pImpObj->dxfInFields(pFiler, objectId());
}

void OdDbAssocViewRepHatchActionParam::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  m_pImpObj->dxfOutFields(pFiler, objectId());
}


OdDbObjectId OdDbAssocViewRepHatchActionParam::hatchId() const
{
  assertReadEnabled();
  return ((OdDbImpAssocViewRepHatchActionParam*)m_pImpObj)->dependentOnObjectId();
}

OdUInt32 OdDbAssocViewRepHatchActionParam::hatchIndex() const
{
  assertReadEnabled();
  return ((OdDbImpAssocViewRepHatchActionParam *)m_pImpObj)->m_nHatchIndex;
}

void OdDbAssocViewRepHatchActionParam::setHatchIndex(OdUInt32 ind)
{
  assertWriteEnabled();
  ((OdDbImpAssocViewRepHatchActionParam *)m_pImpObj)->m_nHatchIndex = ind;
}

bool OdDbAssocViewRepHatchActionParam::isObsolete() const
{
  assertReadEnabled();
  return ((OdDbImpAssocViewRepHatchActionParam *)m_pImpObj)->m_bObsolete;
}

void OdDbAssocViewRepHatchActionParam::setObsolete(bool bObs)
{
  assertWriteEnabled();
  ((OdDbImpAssocViewRepHatchActionParam *)m_pImpObj)->m_bObsolete = bObs;
}

OdGeVector3d OdDbAssocViewRepHatchActionParam::normal() const
{
  assertReadEnabled();
  return ((OdDbImpAssocViewRepHatchActionParam *)m_pImpObj)->m_vNormal;
}

void OdDbAssocViewRepHatchActionParam::setNormal(OdGeVector3d const & vNorm)
{
  assertWriteEnabled();
  ((OdDbImpAssocViewRepHatchActionParam *)m_pImpObj)->m_vNormal = vNorm;
}

OdUInt32 OdDbAssocViewRepHatchActionParam::propertyOverrides() const
{
  assertReadEnabled();
  return ((OdDbImpAssocViewRepHatchActionParam *)m_pImpObj)->m_nPropertyOverrides;
}

void OdDbAssocViewRepHatchActionParam::setPropertyOverrides(OdUInt32 nOver)
{
  assertWriteEnabled();
  ((OdDbImpAssocViewRepHatchActionParam *)m_pImpObj)->m_nPropertyOverrides = nOver;
}
