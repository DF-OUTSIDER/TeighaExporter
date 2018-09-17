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
#include "DbAssoc3PointAngularDimActionBody.h"
#include "DbImpAssoc3PointAngularDimActionBody.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ODRX_DEFINE_MEMBERS_EX(OdDbAssoc3PointAngularDimActionBody,      // ClassName
  OdDbAssocAnnotationActionBody, // ParentClass
  DBOBJECT_CONSTR,               // DOCREATE
  OdDb::kDHL_1027,               // DwgVer
  OdDb::kMRelease0,              // MaintVer
  1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"ACDBAssoc3PointAngularDimActionBody",   // DxfName
  L"ACDBASSOC3POINTANGULARDIMACTIONBODY",   // DWG class name
  L"ObjectDBX Classes",         // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssoc3PointAngularDimActionBody::OdDbAssoc3PointAngularDimActionBody(bool bCreateImpl)
{
  if (bCreateImpl)
    m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpAssoc3PointAngularDimActionBody());
  else
    m_pImpObj = NULL;
}

OdDbAssoc3PointAngularDimActionBody::~OdDbAssoc3PointAngularDimActionBody()
{
  // via base class // delete m_pImpObj;
}

//const OdDbAssoc3PointAngularDimActionBodyPtr OdDbAssoc3PointAngularDimActionBody::parameters() const
//{
//  return static_cast<OdDbAssoc3PointAngularDimActionBody *>(m_pImpObj)->parameters();
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbCenterMarkActionBody::parameters()
//{
//  return static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->parameters();
//}

//void OdDbAssoc3PointAngularDimActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
//{
//  static_cast<OdDbImpAssoc3PointAngularDimActionBody *>(m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OdDbAssoc3PointAngularDimActionBody::modified(const OdDbObject* pObj)
{
	if (!pObj->isUndoing())
	{
		static_cast<OdDbImpAssocRotatedDimActionBody *>
			(m_pImpObj)->evaluateOverride(parentAction());
	}
}
