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
#include "DbLoftedSurface.h"
#include "DbBlockTableRecord.h"
#include "DbAssocFilletSurfaceActionBody.h"
#include "DbImpAssocFilletSurfaceActionBody.h"
#include "DbAssocNetwork.h"

//////////////////////////////////////////////////////////////////////////

OdDbImpAssocFilletSurfaceActionBody::OdDbImpAssocFilletSurfaceActionBody()
{
  m_unknownInt16_0 = 0;
}

OdDbImpAssocFilletSurfaceActionBody::~OdDbImpAssocFilletSurfaceActionBody()
{
}

OdResult OdDbImpAssocFilletSurfaceActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocSurfaceActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_unknownInt16_0 = pFiler->rdInt32();  // 0
  m_unknownInt16 = pFiler->rdInt16();
  m_point1 = pFiler->rdPoint2d();
  m_point2 = pFiler->rdPoint2d();
  return res;
}

void OdDbImpAssocFilletSurfaceActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocSurfaceActionBody::dwgOutFields(pFiler);
  pFiler->wrInt32(m_unknownInt16_0);
  pFiler->wrInt16(m_unknownInt16);
  pFiler->wrPoint2d(m_point1);
  pFiler->wrPoint2d(m_point2);
}

OdResult OdDbImpAssocFilletSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocFilletSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_unknownInt16_0 = pFiler->rdUInt32();
  NEXT_CODE(70);
  m_unknownInt16 = pFiler->rdInt16();
  NEXT_CODE(10);
  pFiler->rdPoint2d(m_point1);
  NEXT_CODE(10);
  pFiler->rdPoint2d(m_point2);

  return eOk;
}

void OdDbImpAssocFilletSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocSurfaceActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocFilletSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_unknownInt16_0);
  pFiler->wrUInt16(70, m_unknownInt16);
  pFiler->wrPoint2d(10, m_point1);
  pFiler->wrPoint2d(10, m_point2);
}

void OdDbImpAssocFilletSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbObjectIdArray objDependentIDs;
  pAction->getDependencies(true, false, objDependentIDs); 

  ODA_ASSERT(objDependentIDs.length() == 2);


  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbSurfacePtr pSurface = OdDbSurface::cast(bEntRef);

  if (pSurface.isNull())
  {
    ODA_ASSERT(!"OdDbSurfacePtr is null");
    return;
  }

  OdArray<OdDbEntityPtr> pEntsTemp;

  for (OdUInt32 i = 0; i < objDependentIDs.length(); i++)
  {
    OdDbAssocDependencyPtr pDep = objDependentIDs.getAt(i).openObject(OdDb::kForWrite);
    OdDbSurfacePtr pSurface = OdDbSurface::cast(pDep->dependentOnObject().openObject());

    if (!pSurface.isNull())
    {
      pEntsTemp.append(pSurface);
    }

    pDep->setStatus(kIsUpToDateAssocStatus);
  }

  ODA_ASSERT(pEntsTemp.length() == 2);
  OdDbSurfacePtr pSurfTemp = OdDbSurface::createObject();
  OdDbSurface* fSur;

  pSurfTemp->setUIsolineDensity(pSurface->uIsolineDensity());
  pSurfTemp->setVIsolineDensity(pSurface->vIsolineDensity());
  OdString expression, evaluatorId;
  OdDouble rad = radius(pAction, expression, evaluatorId);
  OdGePoint3d pickPt1(m_point1.x, m_point1.y, 0.0), pickPt2(m_point2.x, m_point2.y, 0.0);
  OdGeVector3d  projDir;
  OdDb::FilletTrimMode trimMode = OdDb::kTrimNone;

  OdResult esTemp = pSurfTemp->createFilletSurface(pEntsTemp[0]->objectId(), pickPt1,
    pEntsTemp[1]->objectId(), pickPt2,
    rad, trimMode, projDir, fSur);

  if (esTemp == eOk)
  {
    //pLoftSurfTemp->setLoftOptions(loftOptsTemp);
    pSurface->handOverTo(pSurfTemp);
  }
}

OdDouble  OdDbImpAssocFilletSurfaceActionBody::radius(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  OdDouble retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kFilletRadiusParamName, *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return -1;
  }

  return value->getDouble();
}

OdResult OdDbImpAssocFilletSurfaceActionBody::setRadius(OdDbAssocActionPtr pAction, OdDouble dValue, const OdString& expression, const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(dValue);
  OdString emptyStr("");
  return pAction->setValueParam(kFilletRadiusParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}
