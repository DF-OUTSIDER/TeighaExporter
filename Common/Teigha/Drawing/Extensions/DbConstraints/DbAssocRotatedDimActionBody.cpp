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
#include "DbAssocRotatedDimActionBody.h"
#include "DbImpAssocRotatedDimActionBody.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocAnnotationActionBody, OdDbAssocActionBody, L"AcDbAssocAnnotationActionBody")

OdDbAssocAnnotationActionBody::OdDbAssocAnnotationActionBody()
{
}

void OdDbAssocAnnotationActionBody::evaluateOverride()
{
  static_cast<OdDbImpAssocAnnotationActionBody *>(m_pImpObj)->evaluateOverride(parentAction());
}

void OdDbAssocAnnotationActionBody::modified(const OdDbObject* pObj)
{
	if (!pObj->isUndoing())
	{
		static_cast<OdDbImpAssocAnnotationActionBody *>
			(m_pImpObj)->evaluateOverride(parentAction());
	}
}

void OdDbAssocAnnotationActionBody::setAssocDependencyID(OdDbObjectId assocDependencyID)
{
  static_cast<OdDbImpAssocAnnotationActionBody *>(m_pImpObj)->setAssocDependencyID(assocDependencyID);
}

void OdDbAssocAnnotationActionBody::setActionParamID1(OdDbObjectId actionParamID)
{
  static_cast<OdDbImpAssocAnnotationActionBody *>(m_pImpObj)->setActionParamID1(actionParamID);
}

void OdDbAssocAnnotationActionBody::setActionParamID2(OdDbObjectId actionParamID)
{
  static_cast<OdDbImpAssocAnnotationActionBody *>(m_pImpObj)->setActionParamID2(actionParamID);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ODRX_DEFINE_MEMBERS_EX(OdDbAssocRotatedDimActionBody,      // ClassName
  OdDbAssocAnnotationActionBody, // ParentClass
  DBOBJECT_CONSTR,               // DOCREATE
  OdDb::kDHL_1027,               // DwgVer
  OdDb::kMRelease0,              // MaintVer
  1025,                          // nProxyFlags 
  L"AcDbAssocRotatedDimActionBody",   // DWG class name
  L"ACDBASSOCROTATEDDIMACTIONBODY",   // DxfName
  L"ObjectDBX Classes",         // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocRotatedDimActionBody::OdDbAssocRotatedDimActionBody(bool bCreateImpl)
{
  if (bCreateImpl)
    m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpAssocRotatedDimActionBody());
  else
    m_pImpObj = NULL;
}

OdDbAssocRotatedDimActionBody::~OdDbAssocRotatedDimActionBody()
{
  // via base class // delete m_pImpObj;
}

//const OdDbAssocRotatedDimActionBodyPtr OdDbAssocRotatedDimActionBody::parameters() const
//{
//  return static_cast<OdDbAssocRotatedDimActionBody *>(m_pImpObj)->parameters();
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbCenterMarkActionBody::parameters()
//{
//  return static_cast<OdDbImpCenterMarkActionBody *>(m_pImpObj)->parameters();
//}

void OdDbAssocRotatedDimActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  static_cast<OdDbImpAssocRotatedDimActionBody *>(m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
}

OdResult OdDbAssocRotatedDimActionBody::createInstance(const OdDbFullSubentPathArray entPathArray,
                                                       OdGePoint3dArray ptsSelected,
                                                       OdDbObjectId& actionBodyId)
{
  return OdDbImpAssocRotatedDimActionBody::createInstance
    (entPathArray,
     ptsSelected,
     actionBodyId);
}

