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
#include "DbAssocSweptSurfaceActionBody.h"
#include "DbBlockTableRecord.h"
#include "DbImpAssocSweptSurfaceActionBody.h"
#include "DbAssocNetwork.h"
#include "DbSweptSurface.h"
//////////////////////////////////////////////////////////////////////////
OdResult OdDbImpAssocSweptSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocSweptSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_unknownInt32 = pFiler->rdUInt32();

  return eOk;
}

void OdDbImpAssocSweptSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocPathBasedSurfaceActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocSweptSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_unknownInt32);
}

void OdDbImpAssocSweptSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbObjectIdArray snapPointParams = pAction->ownedParams();

  OdDbAssocDependencyPtr pSurfaceAssocDep = m_surfaceObjectDepId.openObject(OdDb::kForWrite);

  if (pSurfaceAssocDep.isNull())
    return;

  OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbSweptSurfacePtr pSurface = OdDbSweptSurface::cast(bEntRef);

  ODA_ASSERT(!pSurface.isNull());
  OdDbSweepOptions sweepOptions;
  OdString expression, evaluatorId;
  pSurface->getSweepOptions(sweepOptions);
  sweepOptions.setAlignAngle(alignAngle(pAction, expression, evaluatorId));
  sweepOptions.setTwistAngle(twistAngle(pAction, expression, evaluatorId));
  sweepOptions.setScaleFactor(scaleFactor(pAction, expression, evaluatorId));

  OdDbObjectIdArray objDependentIDs;
  pAction->getDependencies(true, true, objDependentIDs); //getDependentObjects(true, true, objDependentIDs);

  ODA_ASSERT(objDependentIDs.length() > 1);
  OdDbSurfacePtr pSurf;
  OdDbCurvePtr pEntTemp;
  OdDbEntityPtr pPathEnt;

  for (OdUInt32 i = 0; i < objDependentIDs.length(); i++)
  {
    OdDbAssocDependencyPtr pDep = objDependentIDs.getAt(i).openObject(OdDb::kForRead);

    if (pEntTemp.isNull() && pPathEnt.isNull())
    {
      pEntTemp = OdDbCurve::cast(pDep->dependentOnObject().openObject());
      continue;
    }

    if (!pEntTemp.isNull() && pPathEnt.isNull())
    {
      pPathEnt = OdDbCurve::cast(pDep->dependentOnObject().openObject());
    }
  }

  OdResult esTemp = eNotInitializedYet;

  if (!pEntTemp.isNull())
  {
    OdDbSweptSurfacePtr pSweptSurfTemp = OdDbSweptSurface::createObject();
    OdDbSweepOptions sweepOptsTemp;

    pSweptSurfTemp->setUIsolineDensity(pSurface->uIsolineDensity());
    pSweptSurfTemp->setVIsolineDensity(pSurface->vIsolineDensity());
    esTemp = pSweptSurfTemp->createSweptSurface(pEntTemp, pPathEnt, sweepOptions);// , 0, true);

    if (esTemp == eOk)
    {
      pSurface->handOverTo(pSweptSurfTemp);
    }
  }
  //delete[] objIdForLoft;
}

OdResult OdDbImpAssocSweptSurfaceActionBody::setScaleFactor(OdDbAssocActionPtr pAction, OdDouble dValue, const OdString& expression, const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(dValue);
  OdString emptyStr("");
  return pAction->setValueParam(kScaleFactorParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

OdDouble  OdDbImpAssocSweptSurfaceActionBody::scaleFactor(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  OdDouble retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kScaleFactorParamName, *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return -1;
  }

  return value->getDouble();
}

OdResult OdDbImpAssocSweptSurfaceActionBody::setAlignAngle(OdDbAssocActionPtr pAction, OdDouble dValue, const OdString& expression, const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(dValue);
  OdString emptyStr("");
  return pAction->setValueParam(kRotationAngleParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

OdDouble  OdDbImpAssocSweptSurfaceActionBody::alignAngle(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  OdDouble retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kRotationAngleParamName, *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return -1;
  }

  return value->getDouble();
}

OdDouble  OdDbImpAssocSweptSurfaceActionBody::twistAngle(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  OdDouble retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kTwistAngleParamName, *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return -1;
  }

  return value->getDouble();
}

OdResult OdDbImpAssocSweptSurfaceActionBody::setTwistAngle(OdDbAssocActionPtr pAction, OdDouble dValue, const OdString& expression, const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(dValue);
  OdString emptyStr("");
  return pAction->setValueParam(kTwistAngleParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}
