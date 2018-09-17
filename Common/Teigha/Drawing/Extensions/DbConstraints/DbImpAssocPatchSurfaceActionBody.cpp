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
#include "DbRevolvedSurface.h"
#include "DbAssocPatchSurfaceActionBody.h"
#include "DbImpAssocPatchSurfaceActionBody.h"
//////////////////////////////////////////////////////////////////////////
OdResult OdDbImpAssocPatchSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	const OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocPatchSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_unknownInt32 = pFiler->rdUInt32();

  return eOk;
}

void OdDbImpAssocPatchSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocPathBasedSurfaceActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocPatchSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_unknownInt32);
}

void OdDbImpAssocPatchSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbObjectIdArray snapPointParams = pAction->ownedParams();

  OdDbAssocDependencyPtr pSurfaceAssocDep = m_surfaceObjectDepId.openObject(OdDb::kForWrite);

	OdDbEntityPtr pSurfaceAsEnt;

  if (!pSurfaceAssocDep.isNull())
  {
    OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

    if (bEntRef.isNull())
      return;

    OdDbSurfacePtr pSurface = OdDbSurface::cast(bEntRef);

    //pSurface = OdDbSurface::cast(pSurfaceAssocDep->dependentOnObject().openObject(OdDb::kForWrite));
    pSurfaceAsEnt = OdDbEntity::cast(pSurfaceAssocDep->dependentOnObject().openObject(OdDb::kForWrite));
    ODA_ASSERT(!pSurface.isNull());
  }

  OdDbObjectIdArray objDependentIDs;
  pAction->getDependencies(true, true, objDependentIDs); //getDependentObjects(true, true, objDependentIDs);

  ODA_ASSERT(objDependentIDs.length() > 1);
	OdDbCurvePtr pEntTemp;
  OdDbEntityPtr pPathEnt;

  for (OdUInt32 i = 0; i < objDependentIDs.length(); i++)
  {
    OdDbAssocDependencyPtr pDep = objDependentIDs.getAt(i).openObject(OdDb::kForRead);

    OdDbSurfacePtr pSurf = OdDbSurface::cast(pDep->dependentOnObject().openObject());
	  const OdDbEntityPtr pEntTempAsEnt = OdDbEntity::cast(pDep->dependentOnObject().openObject());

    if (pSurfaceAsEnt != pEntTempAsEnt)
    {
      OdRxObjectPtrArray entitySet;
      pSurf->explode(entitySet);

      //for (OdUInt32 i1 = 0; i1 < entitySet.length(); i1++)
      //{
      //  pEntTemp = OdDbCurve::cast(entitySet.getAt(i1));

      //  if (!pEntTemp.isNull())
      //  {
      //    //pEntTemp->getOsnapPoints
      //    OdGePoint3dArray gpp;
      //    pEntTemp->getGripPoints(gpp);
      //  }
      //}
    }

    //if (!pEntTemp.isNull() && pPathEnt.isNull())
    //{
    //	pPathEnt = OdDbCurve::cast(pDep->dependentOnObject().openObject());
    //}
  }

  //OdDbObjectIdArray objIdForLoft;
  OdArray<OdDbEntityPtr> pEntsTemp;
  OdArray<OdDbEntityPtr> pGuidesTemp;
  for (OdUInt32 i = 0; i < snapPointParams.length(); i++)
  {
    OdDbAssocCompoundActionParamPtr pParam = snapPointParams.getAt(i).openObject(OdDb::kForRead);

    if (pParam->paramCount() > 0)
    {
      //OdString name = pParam->name();
      OdDbObjectId idCompoundActionParam = pParam->paramAtIndex(0);
      OdDbAssocEdgeActionParamPtr pEdgeParam = idCompoundActionParam.openObject(OdDb::kForRead);

      ODA_ASSERT(!pEdgeParam.isNull());
      if (pEdgeParam.isNull())
        continue;

      OdDbObjectIdArray dependencyIds;
      pEdgeParam->getDependencies(true, true, dependencyIds);

      ODA_ASSERT(dependencyIds.length() > 0);
      if (dependencyIds.length() < 1)
        continue;

      OdDbAssocDependencyPtr pDep = dependencyIds.getAt(0).openObject(OdDb::kForRead);

      ODA_ASSERT(!pDep.isNull());
      if (pDep.isNull())
        continue;

      OdDbEntityPtr pSpline = OdDbEntity::cast(pDep->dependentOnObject().openObject());

      ODA_ASSERT(!pSpline.isNull());
      if (pSpline.isNull())
        continue;

      //if (name == L"CrossSection")
      //{
      //	pEntsTemp.append(pSpline);
      //}

      //if (name == L"GuideCurve")
      //{
      //	pGuidesTemp.append(pSpline);
      //}
    }
  }

  OdDbSurfacePtr pLoftSurfTemp = OdDbSurface::createObject();

  //pLoftSurfTemp->setUIsolineDensity(pSurface->uIsolineDensity());
  //pLoftSurfTemp->setVIsolineDensity(pSurface->vIsolineDensity());

  OdDbEntityPtr pPathCurve;
  //OdResult esTemp;// = pLoftSurfTemp->createLoftedSurface(pEntsTemp, pGuidesTemp, pPathCurve, loftOptsTemp);// , 0, true);

  //if (esTemp == eOk)
  //{
  //	pSurface->handOverTo(pLoftSurfTemp);
  //}
}

int OdDbImpAssocPatchSurfaceActionBody::continuity(OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
	const int retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getInt32();
}

enum OdResult OdDbImpAssocPatchSurfaceActionBody::setContinuity(int distance, OdDbAssocActionPtr pAction, const OdString& expression,
  const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(distance);
  OdString emptyStr("");
  return pAction->setValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

double OdDbImpAssocPatchSurfaceActionBody::bulge(OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getDouble();
}

enum OdResult OdDbImpAssocPatchSurfaceActionBody::setBulge(double distance, OdDbAssocActionPtr pAction, const OdString& expression,
  const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(distance);
  OdString emptyStr("");
  return pAction->setValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

//////////////////////////////////////////////////////////////////////////
