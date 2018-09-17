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
#include "DbBlockReference.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "DbAssocDependency.h"
#include "DbAssocGeomDependency.h"
#include "DbLinetypeTable.h"
#include "DbImpAssocAlignedDimActionBody.h"
#include "DbObjectId.h"
#include "DbAlignedDimension.h"

#include "Ge/GeLineSeg3d.h"
#include "DbAssocActionParam.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OdDbImpAssocAlignedDimActionBody::OdDbImpAssocAlignedDimActionBody()
{
  m_paramBasedActionBody = new OdDbImpAssocParamBasedActionBody(true);
//  m_ActionBodyType = kRotatedDim;
}

OdDbImpAssocAlignedDimActionBody::~OdDbImpAssocAlignedDimActionBody()
{
}

OdResult OdDbImpAssocAlignedDimActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocAnnotationActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_undefinedAnnotationActionBody = pFiler->rdInt16();
  m_ActionParamID1 = pFiler->rdSoftPointerId();
  m_ActionParamID2 = pFiler->rdSoftPointerId();

  return res;
}

void OdDbImpAssocAlignedDimActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocAnnotationActionBody::dwgOutFields(pFiler);

  pFiler->wrInt16(m_undefinedAnnotationActionBody);
  pFiler->wrSoftPointerId(m_ActionParamID1);
  pFiler->wrSoftPointerId(m_ActionParamID2);
}

OdResult OdDbImpAssocAlignedDimActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocAnnotationActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocAlignedDimActionBody::desc()->dxfName()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90);
  m_undefinedAnnotationActionBody = pFiler->rdUInt32();
  NEXT_CODE(330);
  m_ActionParamID1 = pFiler->rdObjectId();
  NEXT_CODE(330);
  m_ActionParamID2 = pFiler->rdObjectId();

  return eOk;
}

void OdDbImpAssocAlignedDimActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocAnnotationActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocAlignedDimActionBody::desc()->dxfName());
  pFiler->wrUInt32(90, m_undefinedAnnotationActionBody);
  pFiler->wrObjectId(330, m_ActionParamID1);
  pFiler->wrObjectId(330, m_ActionParamID2);
}

//const OdDbSmartCenterMarkParametersPtr OdDbImpAssocAlignedDimActionBody::parameters() const
//{
//  return m_pParams;
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbImpAssocAlignedDimActionBody::parameters()
//{
//  return m_pParams;
//}
//
void OdDbImpAssocAlignedDimActionBody::composeForLoad(OdDbObject *pObject,
  OdDb::SaveType format,
  OdDb::DwgVersion version,
  OdDbAuditInfo* pAuditInfo)
{
  OdDbAssocAlignedDimActionBody *Body = static_cast<OdDbAssocAlignedDimActionBody *>(pObject);
  //m_matrix = OdDbBlockReference::cast(getArrayEntity(Body->parentAction()).safeOpenObject())->blockTransform();
}


void OdDbImpAssocAlignedDimActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbAlignedDimensionPtr objDim = OdDbAlignedDimension::cast(bEntRef);

  if (objDim.isNull())
    return;

  int const pointNum = 2;
  OdGePoint3d ptPoint[pointNum];
  OdGePoint3d ptDimPoint[pointNum];
  ODA_ASSERT(pAction->ownedParams().length() == pointNum);
  ptDimPoint[0] = objDim->xLine1Point();
  ptDimPoint[1] = objDim->xLine2Point();

  evaluateOverrideAssocDim(parentActionId, ptPoint, ptDimPoint, pointNum);

  if (!objDim.isNull())
  {
    objDim->setXLine1Point(ptPoint[0]);

    if (pAction->ownedParams().length() == pointNum)
      objDim->setXLine2Point(ptPoint[1]);
  }
  else
  {
    ODA_ASSERT(false);
    return;
  }
}

OdResult OdDbImpAssocAlignedDimActionBody::createInstance(
	const OdDbFullSubentPathArray entPathArray,
	OdGePoint3dArray ptsSelected,
	OdDbObjectId& actionBodyId)
{
  OdString createAlignedDim = OD_T("Aligned");;
  return OdDbImpAssocRotatedDimActionBody::createInstance(entPathArray,
                                                          ptsSelected,
                                                          actionBodyId, createAlignedDim);
}

//OdResult OdDbImpAssocAlignedDimActionBody::createInstance(const OdDbObjectId bodyId1, const OdGePoint3d ptPoint1,
//  const OdDbObjectId bodyId2, const OdGePoint3d ptPoint2, const OdGePoint3d dimLinePt,
//  OdDbObjectId& arrayId,
//  OdDbObjectId& actionBodyId)
//{
//  OdString createAlignedDim = OD_T("Aligned");;
//  return OdDbImpAssocRotatedDimActionBody::createInstance(bodyId1, ptPoint1, bodyId2, ptPoint2, dimLinePt, arrayId, actionBodyId, createAlignedDim);
//}

