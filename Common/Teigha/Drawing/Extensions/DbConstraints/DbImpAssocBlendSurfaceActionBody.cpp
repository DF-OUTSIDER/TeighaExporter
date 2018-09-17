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
#include "DbAssocBlendSurfaceActionBody.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "DbImpAssocBlendSurfaceActionBody.h"
#include "DbAssocNetwork.h"

OdDbImpAssocBlendSurfaceActionBody::OdDbImpAssocBlendSurfaceActionBody()
{
  m_unknownInt32 = 0;
}

OdDbImpAssocBlendSurfaceActionBody::~OdDbImpAssocBlendSurfaceActionBody()
{
}

OdResult OdDbImpAssocBlendSurfaceActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dwgInFields(pFiler);

  if (res != eOk)
    return res;

  m_unknownInt32 = pFiler->rdInt32();  // 0
  m_bFlag0 = pFiler->rdBool();
  m_bFlag1 = pFiler->rdBool();
  m_bFlag2 = pFiler->rdBool();
  m_unknownInt16_1 = pFiler->rdInt16();
  m_bFlag3 = pFiler->rdBool();
  m_bFlag4 = pFiler->rdBool();
  m_unknownInt16_2 = pFiler->rdInt16();
  return res;
}

void OdDbImpAssocBlendSurfaceActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocPathBasedSurfaceActionBody::dwgOutFields(pFiler);
  pFiler->wrInt32(m_unknownInt32);
  pFiler->wrBool(m_bFlag0);
  pFiler->wrBool(m_bFlag1);
  pFiler->wrBool(m_bFlag2);
  pFiler->wrInt16(m_unknownInt16_1);
  pFiler->wrBool(m_bFlag3);
  pFiler->wrBool(m_bFlag4);
  pFiler->wrInt16(m_unknownInt16_2);
}

OdResult OdDbImpAssocBlendSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocBlendSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_unknownInt32 = pFiler->rdUInt32();
  NEXT_CODE(290);
  m_bFlag0 = pFiler->rdBool();
  NEXT_CODE(291);
  m_bFlag1 = pFiler->rdBool();
  NEXT_CODE(292);
  m_bFlag2 = pFiler->rdBool();
  NEXT_CODE(72);
  m_unknownInt16_1 = pFiler->rdInt16();
  NEXT_CODE(293);
  m_bFlag3 = pFiler->rdBool();
  NEXT_CODE(294);
  m_bFlag4 = pFiler->rdBool();
  NEXT_CODE(73);
  m_unknownInt16_2 = pFiler->rdInt16();

  return eOk;
}

void OdDbImpAssocBlendSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocPathBasedSurfaceActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocBlendSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_unknownInt32);
  pFiler->wrBool(290, m_bFlag0);
  pFiler->wrBool(291, m_bFlag1);
  pFiler->wrBool(292, m_bFlag2);
  pFiler->wrUInt16(72, m_unknownInt16_1);
  pFiler->wrBool(293, m_bFlag3);
  pFiler->wrBool(294, m_bFlag4);
  pFiler->wrUInt16(73, m_unknownInt16_2);
}

void OdDbImpAssocBlendSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbObjectIdArray snapPointParams = pAction->ownedParams();

  OdDbAssocDependencyPtr pSurfaceAssocDep = m_surfaceObjectDepId.openObject(OdDb::kForWrite);

  OdDbLoftedSurfacePtr pSurface;

  if (!pSurfaceAssocDep.isNull())
  {
    OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

    if (bEntRef.isNull())
      return;

    pSurface = OdDbLoftedSurface::cast(bEntRef);

    ODA_ASSERT(!pSurface.isNull());
  }

  OdDbLoftOptions loftOptsTemp;
  pSurface->getLoftOptions(loftOptsTemp);

  OdDbBlendOptions blendOptsTemp;
  getBlendOptions(blendOptsTemp);

  OdDbObjectIdArray objDependentIDs;
  pAction->getDependencies(true, false, objDependentIDs);

  ODA_ASSERT(objDependentIDs.length() > 1);
  //OdDbObjectIdArray objIdForLoft;
  OdDbEntityPtrArray blendedEdges;
  OdDbEntityPtr pGuidesTemp;
  OdArray<OdDbHandle> pHandles;
  OdArray<OdDbEntityPtrArray> regions;
  OdResult convertToRegion(OdDbEntityPtrArray& regions);

  for (OdUInt32 i = 0; i < objDependentIDs.length(); i += 2 )
  {
    OdDbAssocDependencyPtr pDep = objDependentIDs.getAt(i).openObject(OdDb::kForRead);
    OdDbSurfacePtr pSurface = OdDbSurface::cast(pDep->dependentOnObject().openObject());
    OdDbHandle surfHandle = pSurface->handle();

    if (!pSurface.isNull())
    {
      //objIdForLoft.append(pDep->dependentOnObject());

      if (!pHandles.contains(surfHandle))// TODO ?
      {
        OdDbEntityPtrArray region;
        pSurface->convertToRegion(region);
        regions.append(region);
        blendedEdges.append(pSurface);
        pHandles.append(surfHandle);
      }
    }
  }

  pHandles.clear();

  OdDbLoftedSurfacePtr pLoftSurfTemp = OdDbLoftedSurface::createObject();
  pLoftSurfTemp->setUIsolineDensity(pSurface->uIsolineDensity());
  pLoftSurfTemp->setVIsolineDensity(pSurface->vIsolineDensity());

  //if (!pSurface.isNull())
  //{
  //	pSurface->getLoftOptions(loftOptsTemp);
  //}
  //for (OdUInt32 i = 0; i < objIdForLoft.length(); i++)
  //{
  //	OdDbEntityPtr pEnt = objIdForLoft.getAt(i).openObject(OdDb::kForRead);
  //	pEntsTemp.append(pEnt);
  //}
  //OdGePoint3d startPointEnt;// , startPointEntNext;
  //OdGePoint3d endPointEnt;// , endPointEntNext;
  //OdGePoint3dArray pts;
  //OdGePoint3dArray pts1;

  // OdArray<OdDbEntityPtr> pEntsTempFinalLoop;
  // OdArray<OdDbEntityPtr> pEntsTempStartside;
  // OdArray<OdDbEntityPtr> pEntsTempEndside;

  // OdDbCurvePtr pCurve1 = OdDbCurve::cast(pEntsTemp.getAt(0));
  // OdResult retGetPoint = pCurve1->getStartPoint(startPointEnt);
  // retGetPoint = pCurve1->getEndPoint(endPointEnt);

  // OdDbLinePtr 	pLineStart = OdDbLine::createObject(); 
  // pLineStart->setStartPoint(endPointEnt);
  // pLineStart->setEndPoint(startPointEnt);
  // pEntsTempStartside.append(pLineStart);

  //for (OdUInt32 i = 0; i < pEntsTemp.length() - 1; i++)
  //{
  //	pLineStart = OdDbLine::createObject();
  //   OdDbLinePtr pLineEnd = OdDbLine::createObject();
  //	pCurve1 = OdDbCurve::cast(pEntsTemp.getAt(i));
  //   OdDbCurvePtr pCurve2 = OdDbCurve::cast(pEntsTemp.getAt(i + 1));

  //	OdResult retGetPoint = pCurve1->getStartPoint(startPointEnt);
  //	retGetPoint = pCurve2->getStartPoint(endPointEnt);
  //   
  //   pLineStart->setStartPoint(startPointEnt);
  //   pLineStart->setEndPoint(endPointEnt);

  //   retGetPoint = pCurve1->getEndPoint(startPointEnt);
  //   retGetPoint = pCurve2->getEndPoint(endPointEnt);

  //   pLineEnd->setStartPoint(endPointEnt);
  //   pLineEnd->setEndPoint(startPointEnt);

  //   pEntsTempStartside.append(pLineStart);
  //   
  //   //pEntsTempEndside.append(pLineEnd);
  //   pEntsTempEndside.insertAt(pEntsTempEndside.length(), pLineEnd);
  //}

  // //pEntsTempFinalLoop.append(pEntsTemp.getAt(0));

  // for (OdUInt32 i = 0; i < pEntsTemp.length() - 1; i++)
  // {
  //   pEntsTempFinalLoop.append(pEntsTempStartside.getAt(i));
  // }

  // pEntsTempFinalLoop.append(pEntsTemp.getAt(pEntsTemp.length() - 1));

  // for (OdUInt32 i = 0; i < pEntsTemp.length() - 1; i++)
  // {
  //   pEntsTempFinalLoop.append(pEntsTempEndside.getAt(i));
  // }

  //for (OdUInt32 i = 0; i < pEntsTemp.length(); i++)
  //{
  //	OdDbCurvePtr curve1 = pEntsTemp.getAt(i);
  //	OdResult retGetPoint = curve1->getStartPoint(startPointEnt);

  //	retGetPoint = curve1->getEndPoint(endPointEnt);
  //	pts.append(startPointEnt);
  //	pts1.append(endPointEnt);
  //}

  //OdDbSplinePtr pLine = OdDbSpline::createObject();
  //pLine->setFitData(pts, 3, 0., OdGeVector3d::kIdentity, OdGeVector3d::kIdentity);
  OdDbEntityPtr pPathCurve;
  //OdString expression, evaluatorId;
  //setStartEdgeBulge(pAction, 0.111, expression, evaluatorId);
  //setEndEdgeBulge(pAction, 0.2222, expression, evaluatorId);
  //double d1 = startEdgeBulge(pAction, expression, evaluatorId);
  //expression.empty(), evaluatorId.empty();
  //double d2 = endEdgeBulge(pAction, expression, evaluatorId);
  //setStartEdgeContinuity(pAction, 3, expression, evaluatorId);
  //expression.empty(), evaluatorId.empty();
  //OdInt16  i1 = startEdgeContinuity(pAction, expression, evaluatorId);
  //expression.empty(), evaluatorId.empty();
  //OdInt16  i2 = endEdgeContinuity(pAction, expression, evaluatorId);
  //OdResult esTemp = pLoftSurfTemp->createBlendSurface(blendedEdges, blendOptsTemp);// , 0, true); // TODO
                                                                                                         //OdResult esTemp = pLoftSurfTemp->createLoftedSurface(pEntsTemp, pGuidesTemp, pPathCurve, loftOptsTemp);// , 0, true);

  //if (esTemp == eOk)
  //{
  //  //pLoftSurfTemp->setLoftOptions(loftOptsTemp);
  //  pSurface->handOverTo(pLoftSurfTemp);
  //}

  //pEntsTemp.clear();
}

OdResult  OdDbImpAssocBlendSurfaceActionBody::setBlendOptions(const OdDbBlendOptions& blendOptions)
{
  //assertWriteEnabled();
  //OdDbLoftedSurfaceImpl*  pImpl = OdDbLoftedSurfaceImpl::getImpl(this);

  //TODO: Call options check method first (when implemented)

  //OdResult res = pImpl->OdDbSurfaceImpl::createLoftedObject(
  //  pImpl->m_pCrossSections,
  //  pImpl->m_pGuides,
  //  pImpl->m_pPath,
  //  (OdDbLoftOptions&)loftOptions,
  //  false);

  //if (res == eOk)
  {
    m_pOptions = blendOptions;
  }

  return eOk;
}

OdInt16  OdDbImpAssocBlendSurfaceActionBody::startEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
	const OdInt16 retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return -1;
  }

  return value->getInt16();
}

OdResult  OdDbImpAssocBlendSurfaceActionBody::setStartEdgeContinuity(OdDbAssocActionPtr pAction, OdInt16 intValue, const OdString& expression, const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(intValue);
  OdString emptyStr("");
  return pAction->setValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

OdInt16  OdDbImpAssocBlendSurfaceActionBody::endEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
	const OdInt16 retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  //if (pAction->getValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, 1) != eOk)
  if (pAction->getValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return -1;
  }
  value = value->next();
  ODA_ASSERT(!value.isNull());

  if (value.isNull())
    return -1;

  return value->getInt16();
}

OdArray<OdInt16>  OdDbImpAssocBlendSurfaceActionBody::arrayEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  OdArray<OdDbEvalVariantPtr> values;
  OdStringArray expressions, evaluatorIds;
  pAction->getValueParamArray(kContinuityParamName, values, expressions, evaluatorIds);

  OdArray<OdInt16> retArray;

  for (OdUInt16 i = 0; i < values.length(); i++)
  {
    OdDbEvalVariantPtr value = values[i];
    retArray.append(value->getInt16());
  }
  
  return retArray;
}

OdArray<double>  OdDbImpAssocBlendSurfaceActionBody::arrayEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  OdArray<OdDbEvalVariantPtr> values;
  OdStringArray expressions, evaluatorIds;
  pAction->getValueParamArray(kBulgeParamName, values, expressions, evaluatorIds);

  OdArray<double> retArray;

  for (OdUInt16 i = 0; i < values.length(); i++)
  {
    OdDbEvalVariantPtr value = values[i];
    retArray.append(value->getDouble());
  }

  return retArray;
}

OdResult  OdDbImpAssocBlendSurfaceActionBody::setArrayEdgeContinuity(OdDbAssocActionPtr pAction, OdArray<OdInt16>& values, OdString& expression, OdString& evaluatorId)
{
  OdStringArray errorMessages;
  OdArray<OdDbEvalVariantPtr> valuesEvalVariant;
  OdStringArray expressions, evaluatorIds;

  for (OdUInt16 i = 0; i < values.length(); i++)
  {
    const OdDbEvalVariantPtr additionEvalVariant = OdDbEvalVariant::init(values[i]);
    valuesEvalVariant.append(additionEvalVariant);
  }

  return pAction->setValueParamArray(kContinuityParamName, valuesEvalVariant, expressions, evaluatorIds, errorMessages);
}

OdResult  OdDbImpAssocBlendSurfaceActionBody::setArrayEdgeBulge(OdDbAssocActionPtr pAction, OdArray<double>& values, OdString& expression, OdString& evaluatorId)
{
  OdStringArray errorMessages;
  OdArray<OdDbEvalVariantPtr> valuesEvalVariant;
  OdStringArray expressions, evaluatorIds;

  for (OdUInt16 i = 0; i < values.length(); i++)
  {
    const OdDbEvalVariantPtr additionEvalVariant = OdDbEvalVariant::init(values[i]);
    valuesEvalVariant.append(additionEvalVariant);
  }

  return pAction->setValueParamArray(kBulgeParamName, valuesEvalVariant, expressions, evaluatorIds, errorMessages);
}

OdResult  OdDbImpAssocBlendSurfaceActionBody::setEndEdgeContinuity(OdDbAssocActionPtr pAction, OdInt16 intValue, const OdString& expression, const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(intValue);
  OdString emptyStr("");
  return pAction->setValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, emptyStr, false, 1);
}

double  OdDbImpAssocBlendSurfaceActionBody::startEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
	const double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  return value->getDouble();
}

OdResult  OdDbImpAssocBlendSurfaceActionBody::setStartEdgeBulge(OdDbAssocActionPtr pAction, double dValue, const OdString& expression, const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(dValue);
  OdString emptyStr("");
  return pAction->setValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

double  OdDbImpAssocBlendSurfaceActionBody::endEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
  double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  //if (pAction->getValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, 1) != eOk)
  //  return -1.0;

  if (pAction->getValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, 0) != eOk)
    return -1.0;

  value = value->next();
  ODA_ASSERT(!value.isNull());

  if (value.isNull())
    return -1.0;

  return value->getDouble();
}

OdResult  OdDbImpAssocBlendSurfaceActionBody::setEndEdgeBulge(OdDbAssocActionPtr pAction, double dValue, const OdString& expression, const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(dValue);
  OdString emptyStr("");
  return pAction->setValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, emptyStr, false, 1);
}

OdResult OdDbImpAssocBlendSurfaceActionBody::createInstance(
  const OdDbFullSubentPathArray entPathArray,
  OdGePoint3dArray ptsSelected,
  OdDbObjectId& centerLineId,
  OdDbObjectId& actionBodyId)
{
  OdDbObjectIdArray sourceEntites;
  //OdDbSmartCenterLineParametersPtr pParameters = OdDbSmartCenterLineParameters::createObject();
	const OdResult res = eOk;
  //OdGeLineSeg3d segLine[2];

  OdDbObjectIdArray pathArray = entPathArray[0].objectIds();

  if (pathArray.length() < 1)
    return eInvalidInput;

  sourceEntites.append(pathArray[pathArray.length() - 1]);
  OdDbDatabase *pDb = pathArray[0].database();

  OdDbBlockTablePtr pMainBT = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectId ownerBTRId = pathArray[0].openObject()->ownerId();
  OdDbBlockTableRecordPtr pOwnerSpace = ownerBTRId.safeOpenObject(OdDb::kForWrite);
	const OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject(pOwnerSpace->objectId(), true);

  //create loft reference
  OdDbLoftedSurfacePtr pLoftedSurface = OdDbLoftedSurface::createObject();
  pDb->addOdDbObject(pLoftedSurface, ownerBTRId);
  //create action and action body
  //BEGIN
  OdDbObjectId actionId;
	OdRxClass * pClass = OdDbAssocBlendSurfaceActionBody::desc();

  if (eOk != createActionAndActionBodyAndPostToDatabase(pClass, networkId, actionId, actionBodyId))
  {
    return eInvalidInput;
  }
  //END
  OdDbAssocBlendSurfaceActionBodyPtr pActionBody = OdDbAssocActionBody::cast(actionBodyId.openObject(OdDb::kForWrite));
  //append and  loft ref
  pOwnerSpace->appendOdDbEntity(pLoftedSurface);
  //arrayId = pArrayBlock->objectId();

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
  pAction->setActionBody(actionBodyId);

 // OdString expression, evaluatorId;
 // pActionBody->setStartEdgeBulge(pAction, 0.111, expression, evaluatorId);

 // pActionBody->setEndEdgeBulge(pAction, 0.2222, expression, evaluatorId);
 // double d1 = pActionBody->startEdgeBulge(pAction, expression, evaluatorId);
 // expression.empty(), evaluatorId.empty();
 // double d2 = pActionBody->endEdgeBulge(pAction, expression, evaluatorId);

 // pActionBody->setEndEdgeContinuity(pAction, 4, expression, evaluatorId);
 // pActionBody->setStartEdgeContinuity(pAction, 3, expression, evaluatorId);
 // expression.empty(), evaluatorId.empty();
 // OdInt16  i1 = pActionBody->startEdgeContinuity(pAction, expression, evaluatorId);
 // expression.empty(), evaluatorId.empty();
 // OdInt16  i2 = pActionBody->endEdgeContinuity(pAction, expression, evaluatorId);
 // //OdResult esTemp = pLoftSurfTemp->createBlendSurface(pEntsTemp, pGuidesTemp, pPathCurve, blendOptsTemp);// , 0, true);
 // OdArray<OdInt16>  yyy = pActionBody->arrayEdgeContinuity(pAction, expression, evaluatorId);// const
 // OdArray<OdInt16> valuesInt16;
 // valuesInt16.append(1);
 // valuesInt16.append(2);
 // valuesInt16.append(3);
 // pActionBody->setArrayEdgeContinuity(pAction, valuesInt16, expression, evaluatorId);
 // yyy = pActionBody->arrayEdgeContinuity(pAction, expression, evaluatorId);// const
 // valuesInt16.clear();
 // valuesInt16.append(2);
 // valuesInt16.append(1);
 // pActionBody->setArrayEdgeContinuity(pAction, valuesInt16, expression, evaluatorId);
 // yyy = pActionBody->arrayEdgeContinuity(pAction, expression, evaluatorId);// const
 // valuesInt16.clear();
 // valuesInt16.append(1);
 // valuesInt16.append(2);
 // valuesInt16.append(3);
 // valuesInt16.append(4);
 // pActionBody->setArrayEdgeContinuity(pAction, valuesInt16, expression, evaluatorId);
 // yyy = pActionBody->arrayEdgeContinuity(pAction, expression, evaluatorId);// const

 // OdArray<double> valuesD16;
 // valuesD16.append(1);
 // valuesD16.append(2);
 // valuesD16.append(3);
 // pActionBody->setArrayEdgeBulge(pAction, valuesD16, expression, evaluatorId);
 // OdArray<double> xxx = pActionBody->arrayEdgeBulge(pAction, expression, evaluatorId);// const
 // valuesD16.clear();
 // valuesD16.append(2);
 // valuesD16.append(1);
 // pActionBody->setArrayEdgeBulge(pAction, valuesD16, expression, evaluatorId);
 //xxx = pActionBody->arrayEdgeBulge(pAction, expression, evaluatorId);// const
 // valuesD16.clear();
 // valuesD16.append(1);
 // valuesD16.append(2);
 // valuesD16.append(3);
 // valuesD16.append(4);
 // pActionBody->setArrayEdgeBulge(pAction, valuesD16, expression, evaluatorId);
 // xxx = pActionBody->arrayEdgeBulge(pAction, expression, evaluatorId);// const

  ////create destination BTR
  //OdDbBlockTableRecordPtr pDestinationBTR = OdDbBlockTableRecord::createObject();
  //OdString destinationBTRname = OD_T("*U");

  //pDestinationBTR->setName(destinationBTRname);
  //pDb->addOdDbObject(pDestinationBTR, pMainBT->objectId());

  ////create center line block reference
  //OdDbBlockReferencePtr pCenterLineBlock = OdDbBlockReference::createObject();
  ////From 11941: For properties of Block Reference :
  ////While creating new : setDatabaseDefaults() plus
  ////  CENTERLAYER, CENTERLTSCALE, CENTERLTYPE
  //pCenterLineBlock->setDatabaseDefaults(pDb);

  //OdDbLinetypeTableRecordPtr centerLTypetableRec = OdDbImpSmartCenterActionBody::getCenterLType(pDb);
  //OdDbObjectId layer = getCenterLayerId(pDb);
  //double scale = pDb->getCENTERLTSCALE();

  //pCenterLineBlock->setLinetype(centerLTypetableRec->objectId());
  //pCenterLineBlock->setLayer(layer);
  //pCenterLineBlock->setLinetypeScale(scale);
  ////End first part of "From 11941" //////////////////////////////////////////////////////////////////////
  //pCenterLineBlock->setBlockTableRecord(pDestinationBTR->objectId());
  //pDb->addOdDbObject(pCenterLineBlock, ownerBTRId);

  ////create action and action body
  ////BEGIN
  //OdRxClass* pClass;
  //pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbCenterLineActionBody")).get();

  //OdDbObjectId actionId;

  //if (eOk != createActionAndActionBodyAndPostToDatabase(pClass, networkId, actionId, actionBodyId))
  //{
  //  return eInvalidInput;
  //}

  ////append BTR and block ref
  //pMainBT->add(pDestinationBTR);
  //pOwnerSpace->appendOdDbEntity(pCenterLineBlock);

  //OdDbCenterLineActionBodyPtr pActionBody =
  //  OdDbAssocActionBody::cast(actionBodyId.openObject(OdDb::kForWrite));

  //OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
  //pAction->setActionBody(actionBodyId);
  ////END
  ////END

  ////create dependency
  ////BEGIN
  ////geometry reference
  //OdDbAssocGeomDependencyPtr pGeomDependency[2];
  //pGeomDependency[0] = OdDbAssocGeomDependency::createObject();
  //pDb->addOdDbObject(pGeomDependency[0]);
  //pGeomDependency[0]->setIsReadDependency(true);
  //pGeomDependency[0]->setIsWriteDependency(false);

  //OdDbObjectPtr assocOb = sourceEntites[0].openObject(OdDb::kForWrite);
  //OdDbFullSubentPath entPath = entPathArray[0];

  //if (entPath.subentId().type() != OdDb::kNullSubentType)
  //{
  //  OdUInt32 segInd = entPath.subentId().index() - 1;
  //  pGeomDependency[0]->createNewEdgePersSubent(assocOb, segInd);//, false);
  //}
  //else
  //  pGeomDependency[0]->createNewSingleEdgePersSubent(assocOb->objectId());

  ////pDependency->setOrder(0 - 2147483648u);
  //OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  //pCompoundObjectId->setEmpty();
  //pCompoundObjectId->setFullPath(entPath.objectIds());//, OdDbDatabase* pHostDatabase = NULL)
  //pGeomDependency[0]->attachToObject(pCompoundObjectId);
  //pAction->addDependency(pGeomDependency[0]->objectId(), true);

  ////the next geometry reference
  //pGeomDependency[1] = OdDbAssocGeomDependency::createObject();
  //pDb->addOdDbObject(pGeomDependency[1]);
  //pGeomDependency[1]->setIsReadDependency(true);
  //pGeomDependency[1]->setIsWriteDependency(false);
  //assocOb = sourceEntites[1].openObject(OdDb::kForWrite);
  //entPath = entPathArray[1];

  //if (entPath.subentId().type() != OdDb::kNullSubentType)
  //{
  //  OdUInt32 segInd = entPath.subentId().index() - 1;
  //  pGeomDependency[1]->createNewEdgePersSubent(assocOb, segInd);//, false);
  //}
  //else
  //  pGeomDependency[1]->createNewSingleEdgePersSubent(assocOb->objectId());
  ////pDependency->setOrder(0 - 2147483648u);
  //pCompoundObjectId = OdDbCompoundObjectId::createObject();
  //pCompoundObjectId->setEmpty();
  //pCompoundObjectId->setFullPath(entPath.objectIds());//, OdDbDatabase* pHostDatabase = NULL)
  //pGeomDependency[1]->attachToObject(pCompoundObjectId);
  //pAction->addDependency(pGeomDependency[1]->objectId(), true);

  ////center line block reference
  //OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();
  //pDb->addOdDbObject(pDependency);
  //pDependency->setIsReadDependency(true);
  //pDependency->setIsWriteDependency(true);
  ////pDependency->setOrder(0 - 2147483648u);
  //pCompoundObjectId = OdDbCompoundObjectId::createObject();
  //pCompoundObjectId->setEmpty();
  //pCompoundObjectId->set(pCenterLineBlock->objectId());
  //pDependency->attachToObject(pCompoundObjectId);
  //pAction->addDependency(pDependency->objectId(), true);

  //if (pActionBody->getCenterLineGeometry(segLine[0], 0) != eOk)
  //{
  //  return eInvalidInput;
  //}

  //if (pActionBody->getCenterLineGeometry(segLine[1], 1) != eOk)
  //{
  //  return eInvalidInput;
  //}
  //// EdgeAction references
  //for (int i = 0; i < 2; i++)
  //{
  //  int paramIndex = i;
  //  OdDbObjectId paramId;
  //  OdString paramName = OD_T("InputEdge");
  //  pClass = (OdRxClass*)odrxClassDictionary()->getAt(OD_T("AcDbAssocEdgeActionParam")).get();
  //  OdDbAssocEdgeActionParamPtr pEdgeActionParam;
  //  OdDbVertexRefPtr pNormalVertexRef;
  //  switch (pAction->addParam(paramName, pClass, paramId, paramIndex))
  //  {
  //  case eOk:
  //    pEdgeActionParam = OdDbAssocEdgeActionParam::cast(paramId.openObject(OdDb::kForWrite));
  //    break;
  //  case eAlreadyInDb:
  //    pEdgeActionParam = OdDbAssocEdgeActionParam::cast(paramId.openObject(OdDb::kForRead));
  //    break;
  //  default:
  //    ODA_FAIL_ONCE();
  //  }

  //  if (!pEdgeActionParam.isNull())
  //  {
  //    pCompoundObjectId = OdDbCompoundObjectId::createObject();
  //    pCompoundObjectId->setEmpty();
  //    pCompoundObjectId->set(pGeomDependency[i]->objectId());

  //    OdResult res = pEdgeActionParam->setObject(//actionId,
  //      pCompoundObjectId,
  //      true,
  //      true);

  //    OdDbCurvePtr refCurve = OdDbCurve::cast(sourceEntites[i].safeOpenObject());

  //    if (!refCurve.isNull())
  //    {
  //      pEdgeActionParam->setEdgeSubentityGeometry(&(segLine[i]));
  //    }
  //  }
  //}

  //OdString expression, evaluatorId, pErrorMessage;
  //OdGePoint3d intersectPoint;
  //OdGeLineSeg3d ptLineStartIntersect;
  //OdGeLineSeg3d ptLineEndIntersect;
  //OdArray<int> inputArr;
  //OdGePointOnCurve3d closestPointN;

  ////Calculate intersection of two assotiated lines 
  //bool ResOfIntersect = segLine[1].intersectWith(segLine[0], intersectPoint);

  //ptLineStartIntersect.set(segLine[0].startPoint(), intersectPoint);
  //ptLineEndIntersect.set(segLine[0].endPoint(), intersectPoint);

  //if (ptLineEndIntersect.length() > ptLineStartIntersect.length())
  //{
  //  if (ptLineEndIntersect.getNormalPoint(ptsSelected[0], closestPointN))
  //  {
  //    inputArr.append(1);
  //  }
  //  else if (ptLineStartIntersect.getNormalPoint(ptsSelected[0], closestPointN))
  //  {
  //    inputArr.append(0);
  //  }
  //}
  //else
  //{
  //  if (ptLineStartIntersect.getNormalPoint(ptsSelected[0], closestPointN))
  //  {
  //    inputArr.append(0);
  //  }
  //  else if (ptLineEndIntersect.getNormalPoint(ptsSelected[0], closestPointN))
  //  {
  //    inputArr.append(1);
  //  }
  //}

  //ptLineStartIntersect.set(segLine[1].startPoint(), intersectPoint);
  //ptLineEndIntersect.set(segLine[1].endPoint(), intersectPoint);

  //if (ptLineEndIntersect.length() > ptLineStartIntersect.length())
  //{
  //  if (ptLineEndIntersect.getNormalPoint(ptsSelected[1], closestPointN))
  //  {
  //    inputArr.append(1);
  //  }
  //  else if (ptLineStartIntersect.getNormalPoint(ptsSelected[1], closestPointN))
  //  {
  //    inputArr.append(0);
  //  }
  //}
  //else
  //{
  //  if (ptLineStartIntersect.getNormalPoint(ptsSelected[1], closestPointN))
  //  {
  //    inputArr.append(0);
  //  }
  //  else if (ptLineEndIntersect.getNormalPoint(ptsSelected[1], closestPointN))
  //  {
  //    inputArr.append(1);
  //  }
  //}
  //////
  //ODA_ASSERT(inputArr.length() == 2);
  //res = eSubSelectionSetEmpty;

  return res;
}
//const OdDbSmartCenterMarkParametersPtr OdDbImpAssocExtrudedSurfaceActionBody::parameters() const
//{
//  return m_pParams;
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbImpAssocExtrudedSurfaceActionBody::parameters()
//{
//  return m_pParams;
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
