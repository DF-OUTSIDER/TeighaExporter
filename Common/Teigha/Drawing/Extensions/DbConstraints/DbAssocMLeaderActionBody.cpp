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
#include "DbAssocMLeaderActionBody.h"
#include "DbImpAssocMLeaderActionBody.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ODRX_DEFINE_MEMBERS_EX(OdDbAssocMLeaderActionBody,      // ClassName
  OdDbAssocAnnotationActionBody, // ParentClass
  DBOBJECT_CONSTR,               // DOCREATE
  OdDb::kDHL_1027,               // DwgVer
  OdDb::kMRelease0,              // MaintVer
  1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"AcDbAssocMLeaderActionBody",   // DWG class name
  L"ACDBASSOCMLEADERACTIONBODY",   // DxfName
  L"ObjectDBX Classes",         // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocMLeaderActionBody::OdDbAssocMLeaderActionBody(bool bCreateImpl)
{
  if (bCreateImpl)
    m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpAssocMLeaderActionBody());
  else
    m_pImpObj = NULL;
}

OdDbAssocMLeaderActionBody::~OdDbAssocMLeaderActionBody()
{
  // via base class // delete m_pImpObj;
}

//const OdDbAssocMLeaderActionBodyPtr OdDbAssocMLeaderActionBody::parameters() const
//{
//  return static_cast<OdDbAssocMLeaderActionBody *>(m_pImpObj)->parameters();
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbCenterMarkActionBody::parameters()
//{
//  return static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->parameters();
//}

//void OdDbAssocMLeaderActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
//{
//  static_cast<OdDbImpAssocMLeaderActionBody *>(m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
//}

OdResult OdDbAssocMLeaderActionBody::createInstance(const OdDbObjectId bodyId1,
                                                    OdGePoint3dArray ptsSelected, const OdString mText,
                                                    OdDbObjectId& actionBodyId)
{
  return OdDbImpAssocMLeaderActionBody::createInstance(bodyId1,
                                                       ptsSelected, mText, actionBodyId);
}
