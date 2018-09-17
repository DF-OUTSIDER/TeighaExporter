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
#include "DbImpAssoc3PointAngularDimActionBody.h"
#include "DbAssoc3PointAngularDimActionBody.h"
#include "DbObjectId.h"
OdDbImpAssoc3PointAngularDimActionBody::OdDbImpAssoc3PointAngularDimActionBody()
{
  //m_ActionBodyType = kRotatedDim;
    m_paramBasedActionBody = new OdDbImpAssocParamBasedActionBody(true);
}

OdDbImpAssoc3PointAngularDimActionBody::~OdDbImpAssoc3PointAngularDimActionBody()
{
}

OdResult OdDbImpAssoc3PointAngularDimActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocAnnotationActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_undefinedAnnotationActionBody = pFiler->rdInt16();
  m_ActionParamID1 = pFiler->rdSoftPointerId();
  m_ActionParamID2 = pFiler->rdSoftPointerId();
  m_ActionParamID3 = pFiler->rdSoftPointerId();

  return res;
}

void OdDbImpAssoc3PointAngularDimActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocAnnotationActionBody::dwgOutFields(pFiler);
  pFiler->wrInt16(m_undefinedAnnotationActionBody);
  pFiler->wrSoftPointerId(m_ActionParamID1);
  pFiler->wrSoftPointerId(m_ActionParamID2);
  pFiler->wrSoftPointerId(m_ActionParamID3);
}

OdResult OdDbImpAssoc3PointAngularDimActionBody::dxfInFields(OdDbDxfFiler* pFiler)
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
  m_undefinedAnnotationActionBody = pFiler->rdUInt32();
  NEXT_CODE(330);
  m_ActionParamID1 = pFiler->rdObjectId();
  NEXT_CODE(330);
  m_ActionParamID2 = pFiler->rdObjectId();
  NEXT_CODE(330);
  m_ActionParamID3= pFiler->rdObjectId();
  return eOk;
}

void OdDbImpAssoc3PointAngularDimActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocAnnotationActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssoc3PointAngularDimActionBody::desc()->dxfName());

  pFiler->wrUInt32(90, m_undefinedAnnotationActionBody);
  pFiler->wrObjectId(330, m_ActionParamID1);
  pFiler->wrObjectId(330, m_ActionParamID2);
  pFiler->wrObjectId(330, m_ActionParamID3);
}

//const OdDbSmartCenterMarkParametersPtr OdDbImpAssoc3PointAngularDimActionBody::parameters() const
//{
//  return m_pParams;
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbImpAssoc3PointAngularDimActionBody::parameters()
//{
//  return m_pParams;
//}
//
//void OdDbImpAssoc3PointAngularDimActionBody::composeForLoad(OdDbObject *pObject,
//  OdDb::SaveType format,
//  OdDb::DwgVersion version,
//  OdDbAuditInfo* pAuditInfo)
//{
//  OdDbAssoc3PointAngularDimActionBody *Body = static_cast<OdDbAssoc3PointAngularDimActionBody *>(pObject);
//  //m_matrix = OdDbBlockReference::cast(getArrayEntity(Body->parentAction()).safeOpenObject())->blockTransform();
//}
//

void OdDbImpAssoc3PointAngularDimActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
}
