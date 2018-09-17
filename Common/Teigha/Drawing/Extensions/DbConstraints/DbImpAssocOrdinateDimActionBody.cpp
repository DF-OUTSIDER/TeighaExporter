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
#include "NextCodeDefs.h"
#include "DbAssocArrayModifyActionBody.h"
#include "DbImpAssocRotatedDimActionBody.h"
#include "DbBlockReference.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "DbAssocDependency.h"
#include "DbAssocGeomDependency.h"
#include "DbLinetypeTable.h"
#include "DbImpAssocOrdinateDimActionBody.h"
#include "DbAssocOrdinateDimActionBody.h"
#include "DbAssoc3PointAngularDimActionBody.h"
#include "DbObjectId.h"
#include "DbOrdinateDimension.h"

OdDbImpAssocOrdinateDimActionBody::OdDbImpAssocOrdinateDimActionBody()
{
//  m_ActionBodyType = kRotatedDim;
	m_paramBasedActionBody = new OdDbImpAssocParamBasedActionBody(true);
}

OdDbImpAssocOrdinateDimActionBody::~OdDbImpAssocOrdinateDimActionBody()
{
}

OdResult OdDbImpAssocOrdinateDimActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocAnnotationActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_undefined1 = pFiler->rdInt32();
  m_undefObjectID1 = pFiler->rdSoftPointerId();
  m_undefObjectID2 = pFiler->rdSoftPointerId();

  return res;
}

void OdDbImpAssocOrdinateDimActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocAnnotationActionBody::dwgOutFields(pFiler);

  pFiler->wrInt32(m_undefined1);
  pFiler->wrSoftPointerId(m_undefObjectID1);
  pFiler->wrSoftPointerId(m_undefObjectID2);
}

OdResult OdDbImpAssocOrdinateDimActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocAnnotationActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssoc3PointAngularDimActionBody::desc()->dxfName()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90);
  m_undefined1 = pFiler->rdUInt32();
  NEXT_CODE(330);
  m_undefObjectID1 = pFiler->rdObjectId();
  NEXT_CODE(330);
  m_undefObjectID2 = pFiler->rdObjectId();

  return eOk;
}

void OdDbImpAssocOrdinateDimActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocAnnotationActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssoc3PointAngularDimActionBody::desc()->dxfName());

  pFiler->wrUInt32(90, m_undefined1);
  pFiler->wrObjectId(330, m_undefObjectID1);
  pFiler->wrObjectId(330, m_undefObjectID1);
}

//const OdDbSmartCenterMarkParametersPtr OdDbImpAssocOrdinateDimActionBody::parameters() const
//{
//  return m_pParams;
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbImpAssocOrdinateDimActionBody::parameters()
//{
//  return m_pParams;
//}
//
//void OdDbImpAssocOrdinateDimActionBody::composeForLoad(OdDbObject *pObject,
//  OdDb::SaveType format,
//  OdDb::DwgVersion version,
//  OdDbAuditInfo* pAuditInfo)
//{
//  OdDbAssocOrdinateDimActionBody *Body = static_cast<OdDbAssocOrdinateDimActionBody *>(pObject);
//  //m_matrix = OdDbBlockReference::cast(getArrayEntity(Body->parentAction()).safeOpenObject())->blockTransform();
//}
//

// TODO AlexeyTyurin 06.05.20178 It's the preliminary version, based on ImpAssocRotatedDimActionBody
void OdDbImpAssocOrdinateDimActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbOrdinateDimensionPtr objDim = OdDbOrdinateDimension::cast(bEntRef);

  if (objDim.isNull())
    return;

  int const pointNum = 1;
  OdGePoint3d ptPoint[pointNum];
  OdGePoint3d ptDimPoint[pointNum];
  ODA_ASSERT(pAction->ownedParams().length() == pointNum);
  ptDimPoint[0] = objDim->definingPoint();

  evaluateOverrideAssocDim(parentActionId, ptPoint, ptDimPoint, pointNum);

	if (!objDim.isNull())
	{
		objDim->setDefiningPoint(ptPoint[0]);
	}
	else
	{
		ODA_ASSERT(false);
		return;
	}
}
