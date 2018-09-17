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
#include "DbAssocBlendSurfaceActionBody.h"
#include "DbImpAssocBlendSurfaceActionBody.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ODRX_DEFINE_MEMBERS_EX(OdDbAssocBlendSurfaceActionBody,      // ClassName
	OdDbAssocPathBasedSurfaceActionBody, // ParentClass
	DBOBJECT_CONSTR,               // DOCREATE
	OdDb::kDHL_1024,               // DwgVer
								   //OdDb::kMRelease125,              // MaintVer
	OdDb::kMRelease25,              // MaintVer
	1025,                          // nProxyFlags 
	L"AcDbAssocBlendSurfaceActionBody",   // DWG class name
	L"ACDbASSOCBLENDSURFACEACTIONBODY",   // DxfName
	L"ObjectDBX Classes",         // AppName
	OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocBlendSurfaceActionBody::OdDbAssocBlendSurfaceActionBody(bool bCreateImpl)
{
	if (bCreateImpl)
		m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpAssocBlendSurfaceActionBody());
	else
		m_pImpObj = NULL;
}

OdDbAssocBlendSurfaceActionBody::~OdDbAssocBlendSurfaceActionBody()
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
OdResult OdDbAssocBlendSurfaceActionBody::createInstance(
  const OdDbFullSubentPathArray entPathArray,
  OdGePoint3dArray ptsSelected,
  OdDbObjectId& centerLineId,
  OdDbObjectId& actionBodyId)
{
  return OdDbImpAssocBlendSurfaceActionBody::createInstance(entPathArray, ptsSelected, centerLineId, actionBodyId);
}

//OdResult OdDbCenterLineActionBody::getCenterLineGeometry(OdGeLineSeg3d & segm3d, bool boolCondition)
//{
//  return static_cast<OdDbImpCenterLineActionBody *>
//    (m_pImpObj)->getCenterLineGeometry(segm3d, boolCondition, parentAction());
//}

OdInt16  OdDbAssocBlendSurfaceActionBody::startEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->startEdgeContinuity(pAction, expression, evaluatorId);
}

OdResult  OdDbAssocBlendSurfaceActionBody::setStartEdgeContinuity(OdDbAssocActionPtr pAction, OdInt16 intValue, const OdString& expression, const OdString& evaluatorId)
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->setStartEdgeContinuity(pAction, intValue, expression, evaluatorId);
}

OdInt16  OdDbAssocBlendSurfaceActionBody::endEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->endEdgeContinuity(pAction, expression, evaluatorId);
}

OdArray<OdInt16>  OdDbAssocBlendSurfaceActionBody::arrayEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->arrayEdgeContinuity(pAction, expression, evaluatorId);
}

OdResult  OdDbAssocBlendSurfaceActionBody::setArrayEdgeContinuity(const OdDbAssocActionPtr pAction, OdArray<OdInt16>& values, OdString& expression, OdString& evaluatorId)
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->setArrayEdgeContinuity(pAction, values, expression, evaluatorId);
}

OdResult  OdDbAssocBlendSurfaceActionBody::setEndEdgeContinuity(OdDbAssocActionPtr pAction, OdInt16 intValue, const OdString& expression, const OdString& evaluatorId)
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->setEndEdgeContinuity(pAction, intValue, expression, evaluatorId);
}

double  OdDbAssocBlendSurfaceActionBody::startEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->startEdgeBulge(pAction, expression, evaluatorId);
}

OdArray<double>  OdDbAssocBlendSurfaceActionBody::arrayEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->arrayEdgeBulge(pAction, expression, evaluatorId);
}

OdResult  OdDbAssocBlendSurfaceActionBody::setArrayEdgeBulge(const OdDbAssocActionPtr pAction, OdArray<double>& values, OdString& expression, OdString& evaluatorId)
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->setArrayEdgeBulge(pAction, values, expression, evaluatorId);
}
  
OdResult  OdDbAssocBlendSurfaceActionBody::setStartEdgeBulge(OdDbAssocActionPtr pAction, double dValue, const OdString& expression, const OdString& evaluatorId)
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->setStartEdgeBulge(pAction, dValue, expression, evaluatorId);
}

double  OdDbAssocBlendSurfaceActionBody::endEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->endEdgeBulge(pAction, expression, evaluatorId);
}

OdResult  OdDbAssocBlendSurfaceActionBody::setEndEdgeBulge(OdDbAssocActionPtr pAction, double dValue, const OdString& expression, const OdString& evaluatorId)
{
  return static_cast<OdDbImpAssocBlendSurfaceActionBody *>
    (m_pImpObj)->setEndEdgeBulge(pAction, dValue, expression, evaluatorId);
}

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
