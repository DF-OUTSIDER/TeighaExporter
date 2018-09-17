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
#include "DbAssocSurfaceActionBody.h"
#include "DbAssocEdgeFilletActionBody.h"
#include "DbImpAssocExtrudedSurfaceActionBody.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ODRX_DEFINE_MEMBERS_EX(OdDbAssocEdgeFilletActionBody,      // ClassName
	OdDbAssocPathBasedSurfaceActionBody, // ParentClass
	DBOBJECT_CONSTR,               // DOCREATE
	OdDb::kDHL_1024,               // DwgVer
	OdDb::kMRelease25,              // MaintVer
	1025,                          // nProxyFlags 
	L"AcDbAssocEdgeFilletActionBody",   // DWG class name
	L"ACDbASSOCEDGEFILLETACTIONBODY",   // DxfName
	L"ObjectDBX Classes",         // AppName
	OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocEdgeFilletActionBody::OdDbAssocEdgeFilletActionBody(bool bCreateImpl)
{
	//if (bCreateImpl)
	//	m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpAssocRotatedDimActionBody());
	//else
	m_pImpObj = NULL;
}

OdDbAssocEdgeFilletActionBody::~OdDbAssocEdgeFilletActionBody()
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

//void OdDbAssocRotatedDimActionBody::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
//{
//	static_cast<OdDbImpAssocRotatedDimActionBody *>(m_pImpObj)->composeForLoad(this, format, version, pAuditInfo);
//}
//
//void OdDbAssocRotatedDimActionBody::modified(const OdDbObject* pObj)
//{
//	if (!pObj->isUndoing())
//	{
//		static_cast<OdDbImpAssocRotatedDimActionBody *>
//			(m_pImpObj)->evaluateOverride(parentAction());
//	}
//}
//
//OdResult OdDbAssocRotatedDimActionBody::createInstance(const OdDbFullSubentPathArray entPathArray,
//	const OdDbObjectIdArray& sourceEntites,
//	OdGePoint3dArray ptsSelected,
//	OdDbObjectId& arrayId, OdDbObjectId& actionBodyId)
//{
//	return OdDbImpAssocRotatedDimActionBody::createInstance
//		(entPathArray,
//			sourceEntites,
//			ptsSelected,
//			arrayId, actionBodyId);
//}
//
//OdStringArray OdDbAssocRotatedDimActionBody::compareWith(const OdDbObjectId bodyIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
//{
//	OdStringArray retArray;
//	OdDbAssocRotatedDimActionBodyPtr comparedBodyPtr = OdDbAssocRotatedDimActionBody::cast(bodyIDToCompare.openObject());
//
//	if (comparedBodyPtr.isNull())
//	{
//		retArray.append(CS_OBJECT_ERROR);
//		return retArray;
//	}
//
//	OdDbImpAssocRotatedDimActionBody* pImpObjCompared = static_cast<OdDbImpAssocRotatedDimActionBody *>(comparedBodyPtr->m_pImpObj);
//
//	return static_cast<OdDbImpAssocRotatedDimActionBody *>
//		(m_pImpObj)->compareWith(pImpObjCompared);
//}

