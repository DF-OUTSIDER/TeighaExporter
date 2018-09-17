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
#include "DbAssocOrdinateDimActionBody.h"
#include "DbImpAssocOrdinateDimActionBody.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ODRX_DEFINE_MEMBERS_EX(OdDbAssocOrdinateDimActionBody,      // ClassName
  OdDbAssocAnnotationActionBody, // ParentClass
  DBOBJECT_CONSTR,               // DOCREATE
  OdDb::kDHL_1027,               // DwgVer
  OdDb::kMRelease0,              // MaintVer
  1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"AcDbAssocOrdinateDimActionBody",   // DWG class name
  L"ACDBASSOCORDINATEDIMACTIONBODY",   // DxfName
  L"ObjectDBX Classes",         // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocOrdinateDimActionBody::OdDbAssocOrdinateDimActionBody(bool bCreateImpl)
{
  if (bCreateImpl)
    m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpAssocOrdinateDimActionBody());
  else
    m_pImpObj = NULL;
}

OdDbAssocOrdinateDimActionBody::~OdDbAssocOrdinateDimActionBody()
{
  // via base class // delete m_pImpObj;
}

//const OdDbAssocOrdinateDimActionBodyPtr OdDbAssocOrdinateDimActionBody::parameters() const
//{
//  return static_cast<OdDbAssocOrdinateDimActionBody *>(m_pImpObj)->parameters();
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbCenterMarkActionBody::parameters()
//{
//  return static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->parameters();
//}

//void OdDbAssocOrdinateDimActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
//{
//  static_cast<OdDbImpAssocOrdinateDimActionBody *>(m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
