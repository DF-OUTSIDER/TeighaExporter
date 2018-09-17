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
#include "DbAssocOffsetSurfaceActionBody.h"
#include "DbImpAssocOffsetSurfaceActionBody.h"
#include "DbAssocNetwork.h"
//////////////////////////////////////////////////////////////////////////

OdDbImpAssocOffsetSurfaceActionBody::OdDbImpAssocOffsetSurfaceActionBody()
{
  m_undefined = 0;
}

OdDbImpAssocOffsetSurfaceActionBody::~OdDbImpAssocOffsetSurfaceActionBody()
{
}

OdResult OdDbImpAssocOffsetSurfaceActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocSurfaceActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_undefined = pFiler->rdInt32();  // 0
  m_unknownBool = pFiler->rdBool();
  return res;
}

void OdDbImpAssocOffsetSurfaceActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocSurfaceActionBody::dwgOutFields(pFiler);
  pFiler->wrInt32(m_undefined);
  pFiler->wrBool(m_unknownBool);
}

OdResult OdDbImpAssocOffsetSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	const OdResult res = OdDbImpAssocSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocOffsetSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_undefined = pFiler->rdUInt32();
  NEXT_CODE(290);
  m_unknownBool = pFiler->rdBool();

  return eOk;
}

void OdDbImpAssocOffsetSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocSurfaceActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocOffsetSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_undefined);
  pFiler->wrBool(290, m_unknownBool);
}

void OdDbImpAssocOffsetSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbObjectIdArray snapPointParams = pAction->ownedParams();

  OdDbAssocDependencyPtr pSurfaceAssocDep = m_surfaceObjectDepId.openObject(OdDb::kForWrite);

  if (pSurfaceAssocDep.isNull())
    return;

  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbSurfacePtr pSurface = OdDbSurface::cast(bEntRef);
  //OdDbSurfacePtr pSurface = OdDbSurface::cast(pSurfaceAssocDep->dependentOnObject().openObject(OdDb::kForWrite));

  ODA_ASSERT(!pSurface.isNull());

  //pSurface->ge
  OdDbObjectIdArray objDependentIDs;
  pAction->getDependencies(true, true, objDependentIDs); //getDependentObjects(true, true, objDependentIDs);

  ODA_ASSERT(objDependentIDs.length() > 1);
  OdDbSurfacePtr pEntTemp;
  OdDbSurfacePtr pEntOrig;

  for (OdUInt32 i = 0; i < objDependentIDs.length(); i++)
  {
    OdDbAssocDependencyPtr pDep = objDependentIDs.getAt(i).openObject(OdDb::kForRead);
    pEntTemp = OdDbSurface::cast(pDep->dependentOnObject().openObject());

    if (pEntTemp == pSurface)
    {
      int yyy = 7;
    }
    else
      pEntOrig = pEntTemp;
  }

  OdResult esTemp;

  if (!pEntTemp.isNull())
  {
    OdDbEntityPtr pSurfTemp;// = OdDbSurface::createObject();
                            //pLoftSurfTemp->setUIsolineDensity(pSurface->uIsolineDensity());
                            //pLoftSurfTemp->setVIsolineDensity(pSurface->vIsolineDensity());
    OdString emptyStr("");
    esTemp = OdDbSurface::createOffsetSurface(pEntOrig, distance(pAction, emptyStr, emptyStr), pSurfTemp);// , 0, true);

    if (esTemp == eOk)
    {
      pSurface->handOverTo(pSurfTemp);
    }
  }
}

double OdDbImpAssocOffsetSurfaceActionBody::distance(OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
	const double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kOffsetDistanceParamName, *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getDouble();
}

enum OdResult OdDbImpAssocOffsetSurfaceActionBody::setDistance(double distance, OdDbAssocActionPtr pAction, const OdString& expression,
  const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(distance);
  OdString emptyStr("");
  return pAction->setValueParam(kOffsetDistanceParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}
