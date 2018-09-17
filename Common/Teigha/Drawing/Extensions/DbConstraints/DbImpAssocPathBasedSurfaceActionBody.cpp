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
#include "DbBlockTableRecord.h"
#include "DbAssocDependency.h"
#include "DbObjectId.h"
#include "Ge/GeCircArc3d.h"
#include "DbPolyline.h"
#include "DbImpAssocPathBasedSurfaceActionBody.h"
#include "DbLoftedSurface.h"

#include <typeinfo>

OdDbImpAssocSurfaceActionBody::OdDbImpAssocSurfaceActionBody()
{
  m_udef1 = 0;
  m_udef2 = 0;
  m_udefInt16_1 = 0;
  m_udefInt16_2 = 0;
  m_boolUndef1 = false;
  m_boolUndef2 = false;
  //m_paramBasedActionBody = NULL;
}

OdDbImpAssocSurfaceActionBody::~OdDbImpAssocSurfaceActionBody()
{
  //if (m_paramBasedActionBody != NULL)
  //	delete m_paramBasedActionBody;
}

OdResult OdDbImpAssocSurfaceActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocActionBody::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  res = OdDbImpAssocParamBasedActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_udef1 = pFiler->rdInt32();
  m_surfaceObjectDepId = pFiler->rdSoftPointerId();
  m_boolUndef1 = pFiler->rdBool();
  m_udef2 = pFiler->rdInt32();
  m_boolUndef2 = pFiler->rdBool();
  m_udefInt16_1 = pFiler->rdInt16();

  return res;
}

void OdDbImpAssocSurfaceActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocActionBody::dwgOutFields(pFiler);
  OdDbImpAssocParamBasedActionBody::dwgOutFields(pFiler);

  pFiler->wrInt32(m_udef1);
  pFiler->wrSoftPointerId(m_surfaceObjectDepId);
  pFiler->wrBool(m_boolUndef1);
  pFiler->wrInt32(m_udef2);
  pFiler->wrBool(m_boolUndef2);
  pFiler->wrInt16(m_udefInt16_1);
}

OdResult OdDbImpAssocSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocActionBody::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  res = OdDbImpAssocParamBasedActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_udef1 = pFiler->rdUInt32();
  NEXT_CODE(330)
    m_surfaceObjectDepId = pFiler->rdObjectId();
  NEXT_CODE(290);
  m_boolUndef1 = pFiler->rdBool();
  NEXT_CODE(90)
    m_udef2 = pFiler->rdUInt32();
  NEXT_CODE(290);
  m_boolUndef2 = pFiler->rdBool();
  NEXT_CODE(70);
  m_udefInt16_1 = pFiler->rdInt16();

  ODA_ASSERT_ONCE(pFiler->atEOF());

  return eOk;
}

void OdDbImpAssocSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocActionBody::dxfOutFields(pFiler);
  OdDbImpAssocParamBasedActionBody::dxfOutFields(pFiler);
  //if (pFiler->filerType() == OdDbFiler::kBagFiler)

  pFiler->wrSubclassMarker(OdDbAssocSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_udef1);
  pFiler->wrObjectId(330, m_surfaceObjectDepId);
  pFiler->wrBool(290, m_boolUndef1);
  pFiler->wrUInt32(90, m_udef2);
  pFiler->wrBool(290, m_boolUndef2);
  pFiler->wrUInt16(70, m_udefInt16_1);
}

OdStringArray  OdDbImpAssocSurfaceActionBody::compareWith(OdDbImpAssocActionBody *bodyIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
{
  OdStringArray retArr;
  OdDbImpAssocSurfaceActionBody* comparedBody = static_cast<OdDbImpAssocSurfaceActionBody *>(bodyIDToCompare);
  ODA_ASSERT(comparedBody != NULL);

  if (comparedBody != NULL)
  {
    retArr = OdDbImpAssocActionBody::compareWith(bodyIDToCompare, filerVersion /*OdDb::kDHL_CURRENT*/);
    OdStringArray retArr1 = OdDbImpAssocParamBasedActionBody::compareWith(bodyIDToCompare, filerVersion /*OdDb::kDHL_CURRENT*/);
    retArr.append(retArr1);

    if (m_udef1 != comparedBody->m_udef1)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

    if (typeid(*(m_surfaceObjectDepId.openObject())) != typeid(*(comparedBody->m_surfaceObjectDepId.openObject())))
      retArr.append(CS_DIFFERENT_OBJECTS);

    if (m_boolUndef1 != comparedBody->m_boolUndef1)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

    if (m_udef2 != comparedBody->m_udef2)
      retArr.append(CS_NOT_CRITICAL);

    if (m_boolUndef2 != comparedBody->m_boolUndef2)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

    if (m_udefInt16_1 != comparedBody->m_udefInt16_1)
      retArr.append(CS_NOT_CRITICAL);
  }
  return retArr;
}

///////////////////////////////////////////////////////////////////////////////////
OdResult OdDbImpAssocPathBasedSurfaceActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
	const OdResult res = OdDbImpAssocSurfaceActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_udefPathBased = pFiler->rdInt32();

  return res;
}

void OdDbImpAssocPathBasedSurfaceActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocSurfaceActionBody::dwgOutFields(pFiler);
  pFiler->wrInt32(m_udefPathBased);
}

OdResult OdDbImpAssocPathBasedSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocPathBasedSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_udefPathBased = pFiler->rdUInt32();

  ODA_ASSERT_ONCE(pFiler->atEOF());

  return eOk;
}

void OdDbImpAssocPathBasedSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocSurfaceActionBody::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(OdDbAssocPathBasedSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_udefPathBased);
}

OdStringArray  OdDbImpAssocPathBasedSurfaceActionBody::compareWith(OdDbImpAssocActionBody *bodyIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
{
  OdStringArray retArr;
  OdDbImpAssocPathBasedSurfaceActionBody* comparedBody = static_cast<OdDbImpAssocPathBasedSurfaceActionBody *>(bodyIDToCompare);
  ODA_ASSERT(comparedBody != NULL);

  if (comparedBody != NULL)
  {
    retArr = OdDbImpAssocSurfaceActionBody::compareWith(bodyIDToCompare, filerVersion /*OdDb::kDHL_CURRENT*/);

    if (m_udefPathBased != comparedBody->m_udefPathBased)
      retArr.append(CS_NOT_CRITICAL);
  }

  return retArr;
}

OdDbObjectId OdDbImpAssocPathBasedSurfaceActionBody::createOdDdAssocPathActionParam(const OdDbObjectId actionId, const OdDbObjectId source, const OdDbAssocDependencyPtr pGeomDependency, OdString paramName, int paramIndex)
{

  OdDbObjectPtr ptObj;
  OdDbSmartCenterActionBodyPtr smartObj;
  OdDbObjectId anchorId;

  if (!source.isNull())
  {
    ptObj = source.openObject();
    smartObj = OdDbSmartCenterActionBody::cast(ptObj);
  }

  OdDbObjectId paramId;
  OdRxClass* pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("ACDBASSOCPATHACTIONPARAM")).get();
  OdDbAssocPathActionParamPtr pPathActionParam;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));

  switch (pAction->addParam(paramName, pClass, paramId, paramIndex))
  {
  case eOk:
    pPathActionParam = OdDbAssocPathActionParam::cast(paramId.openObject(OdDb::kForWrite));
    break;
  case eAlreadyInDb:
    pPathActionParam = OdDbAssocPathActionParam::cast(paramId.openObject(OdDb::kForRead));
    break;
  default:
    ODA_FAIL_ONCE();
  }

  if (!pPathActionParam.isNull() && !source.isNull())
  {
    OdDbAssocEdgeActionParamPtr pAssocActionParam;
    int paramIndex = 0;
    OdDbObjectId paramId;
    OdString paramNameChild = OD_T(" ");

    pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbAssocEdgeActionParam")).get();

    switch (pPathActionParam->addChildParam(actionId, paramNameChild, pClass, anchorId, paramId, paramIndex))
    {
    case eOk:
    {
      pAssocActionParam = OdDbAssocEdgeActionParam::cast(paramId.openObject(OdDb::kForWrite));
    }
    break;
    case eAlreadyInDb:
      pAssocActionParam = OdDbAssocEdgeActionParam::cast(paramId.openObject(OdDb::kForRead));
      break;
    default:
      ODA_FAIL_ONCE();
    }

    if (!pAssocActionParam.isNull())
    {
      OdGeCurve3d* pGeCurve;
      OdDbCurvePtr refCurve = OdDbCurve::cast(source.safeOpenObject());

      if (!refCurve.isNull())
      {
        refCurve->getOdGeCurve(pGeCurve);
        pAssocActionParam->setEdgeSubentityGeometry(pGeCurve);
        delete pGeCurve;
      }

      OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
      pCompoundObjectId->set(pGeomDependency->objectId());

      OdResult res = pAssocActionParam->setObjectOnly(
        pCompoundObjectId,
        true,
        false);
    }

    //anchorId = pOSnapActionParam->objectId();
  }

  return paramId;
}
