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
#include "DbPolyline.h"
#include "DbSpline.h"
#include "DbAssocPlaneSurfaceActionBody.h"
#include "DbPlaneSurface.h"
#include "DbImpAssocPlaneSurfaceActionBody.h"
//////////////////////////////////////////////////////////////////////////
OdResult OdDbImpAssocPlaneSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocPlaneSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_unknownInt32 = pFiler->rdUInt32();

  return eOk;
}

void OdDbImpAssocPlaneSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocPathBasedSurfaceActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocPlaneSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_unknownInt32);
}

void OdDbImpAssocPlaneSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdRxObjectPtrArray regions;
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbObjectIdArray snapPointParams = pAction->ownedParams();

  OdDbAssocDependencyPtr pSurfaceAssocDep = m_surfaceObjectDepId.openObject(OdDb::kForWrite);

  if (pSurfaceAssocDep.isNull())
    return;

  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbPlaneSurfacePtr pSurface = OdDbPlaneSurface::cast(bEntRef);
  //OdDbPlaneSurfacePtr pSurface = OdDbPlaneSurface::cast(pSurfaceAssocDep->dependentOnObject().openObject(OdDb::kForWrite));

  ODA_ASSERT(!pSurface.isNull());
  //OdDbPlaneOptions revolveOptions;
  //pSurface->getPlaneOptions(revolveOptions);

  OdDbObjectIdArray objDependentIDs;
  pAction->getDependencies(true, true, objDependentIDs); //getDependentObjects(true, true, objDependentIDs);

  ODA_ASSERT(objDependentIDs.length() > 1);
  OdDbSurfacePtr pSurf;
  OdDbCurvePtr pEntTemp;
  OdRxObjectPtrArray curves;

  for (OdUInt32 i = 0; i < objDependentIDs.length(); i++)
  {
    OdDbAssocDependencyPtr pDep = objDependentIDs.getAt(i).openObject(OdDb::kForRead);
    pEntTemp = OdDbCurve::cast(pDep->dependentOnObject().openObject());

    if (!pEntTemp.isNull())
    {
      curves.push_back(pEntTemp.get());
    }
  }

  if (!pEntTemp.isNull() && OdDbRegion::createFromCurves(curves, regions) == eOk)
  {
    ODA_ASSERT(regions.length() == 1);
    OdDbPlaneSurfacePtr pPlaneSurfTemp = OdDbPlaneSurface::createObject();

    pPlaneSurfTemp->setUIsolineDensity(pSurface->uIsolineDensity());
    pPlaneSurfTemp->setVIsolineDensity(pSurface->vIsolineDensity());

    OdDbRegionPtr pRegion = OdDbRegion::cast(regions.getAt(0));
    ODA_ASSERT(!pRegion.isNull());
    OdResult esTemp = pPlaneSurfTemp->createFromRegion(pRegion);

    if (esTemp == eOk)
    {
      pSurface->handOverTo(pPlaneSurfTemp);
    }
  }

  curves.clear();
}
