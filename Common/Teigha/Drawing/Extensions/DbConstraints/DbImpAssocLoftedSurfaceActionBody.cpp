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
#include "DbBlockTable.h"
#include "DbAssocDependency.h"
#include "DbImpAssocLoftedSurfaceActionBody.h"
#include "DbObjectId.h"
#include "DbAssocNetwork.h"
#include "DbLine.h"
#include "DbPolyline.h"
#include "DbAssocLoftedSurfaceActionBody.h"
#include "DbAssocPlaneSurfaceActionBody.h"
#include "DbLoftedSurface.h"
#include "DbAssocSurfacesParameters.h"
//////////////////////////////////////////////////////////////////////////
OdDbAssocLoftParametersPtr OdDbImpAssocLoftedSurfaceActionBody::parameters() const
{
  return m_pParams;
}

OdDbAssocLoftParametersPtr& OdDbImpAssocLoftedSurfaceActionBody::parameters()
{
  return m_pParams;
}

OdResult OdDbImpAssocLoftedSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	const OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dxfInFields(pFiler);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocLoftedSurfaceActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
    m_unknownInt32 = pFiler->rdUInt32();

  return eOk;
}

OdDbImpAssocLoftedSurfaceActionBody::OdDbImpAssocLoftedSurfaceActionBody(): m_continuity(0), m_bulge(0)
{
	m_unknownInt32 = 0;
}

OdDbImpAssocLoftedSurfaceActionBody::~OdDbImpAssocLoftedSurfaceActionBody()
{

}

void OdDbImpAssocLoftedSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocPathBasedSurfaceActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocLoftedSurfaceActionBody::desc()->name());
  pFiler->wrUInt32(90, m_unknownInt32);
}

void OdDbImpAssocLoftedSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
	OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
  OdDbObjectIdArray snapPointParams = pAction->ownedParams();
	OdDbObjectIdArray objDependentIDs;
  
	pAction->getDependencies(true, false, objDependentIDs); //getDependentObjects(true, true, objDependentIDs);
	ODA_ASSERT(objDependentIDs.length() > 1);
	
	if (m_surfaceObjectDepId.isNull())// 
		m_surfaceObjectDepId = objDependentIDs.getAt(0);

	// TODO
	//OdDbAssocDependencyPtr pSurfaceAssocDep = m_surfaceObjectDepId.openObject(OdDb::kForWrite);
	//if (pSurfaceAssocDep.isNull())
	//	return;
	OdDbEntityPtr bEntRef = checkAccompanying(parentActionId, pAction);

  if (bEntRef.isNull())
    return;

  OdDbLoftedSurfacePtr pSurface = OdDbLoftedSurface::cast(bEntRef);

  if (pSurface.isNull())
  {
    ODA_ASSERT(!"OdDbLoftedSurfacePtr is null");
    return;
  }

  OdDbLoftOptions loftOptsTemp;
  pSurface->getLoftOptions(loftOptsTemp);

  OdArray<OdDbEntityPtr> pEntsTemp;
  OdArray<OdDbEntityPtr> pGuidesTemp;
	OdDbEntityPtr pPathCurve;

	for (OdUInt32 i = 0; i < snapPointParams.length(); i++)
	{
		OdDbAssocCompoundActionParamPtr pParam = snapPointParams.getAt(i).openObject(OdDb::kForRead);

		if (pParam->paramCount() > 0)
		{
			const OdString name = pParam->name();
			OdDbObjectId idCompoundActionParam = pParam->paramAtIndex(0);
			OdDbAssocEdgeActionParamPtr pEdgeParam = idCompoundActionParam.openObject(OdDb::kForRead);

			ODA_ASSERT(!pEdgeParam.isNull());
			if (pEdgeParam.isNull())
				continue;

			OdDbObjectIdArray dependencyIds;
			pEdgeParam->getDependencies(true, false, dependencyIds);

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

			if (name == kLoftCrossSectionName)
			{
				pEntsTemp.append(pSpline);
			}

			if (name == kLoftGuideCurveName)
			{
				pGuidesTemp.append(pSpline);
			}

			if (name == kLoftPathCurveName)
			{
				pPathCurve = pSpline;
			}
			//pDep->setStatus(kIsUpToDateAssocStatus);
		}
	}
	
  OdDbLoftedSurfacePtr pLoftSurfTemp = OdDbLoftedSurface::createObject();

  pLoftSurfTemp->setUIsolineDensity(pSurface->uIsolineDensity());
  pLoftSurfTemp->setVIsolineDensity(pSurface->vIsolineDensity());

	const OdResult esTemp = pLoftSurfTemp->createLoftedSurface(pEntsTemp, pGuidesTemp, pPathCurve, loftOptsTemp);// , 0, true);

  if (esTemp == eOk)
  {
    pLoftSurfTemp->setLoftOptions(loftOptsTemp);
    pSurface->handOverTo(pLoftSurfTemp);
  }

	pEntsTemp.clear();
	pGuidesTemp.clear();
}

enum OdResult OdDbImpAssocLoftedSurfaceActionBody::getContinuity(OdDbImpAssocLoftParameters::ProfileType type, int &continuity, OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
	const int retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, 0) != eOk)
    return eInvalidIndex;

  continuity = value->getInt32();

  return eOk;
}

enum OdResult OdDbImpAssocLoftedSurfaceActionBody::setContinuity(OdDbImpAssocLoftParameters::ProfileType type, int distance, OdDbAssocActionPtr pAction, const OdString& expression,
  const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(distance);
  OdString emptyStr("");
  return pAction->setValueParam(kContinuityParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

enum OdResult  OdDbImpAssocLoftedSurfaceActionBody::getBulge(OdDbImpAssocLoftParameters::ProfileType type, double &bulge, OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const
{
	const double retD = -1.0;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, 0) != eOk)
    return eInvalidIndex;

  bulge = value->getDouble();
  return eOk;
}

enum OdResult OdDbImpAssocLoftedSurfaceActionBody::setBulge(OdDbImpAssocLoftParameters::ProfileType type, double distance, OdDbAssocActionPtr pAction, const OdString& expression,
  const OdString& evaluatorId)
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(distance);
  OdString emptyStr("");
  return pAction->setValueParam(kBulgeParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

OdResult OdDbImpAssocLoftedSurfaceActionBody::createInstance(
	const OdDbFullSubentPathArray entPathArray,
	const OdDbFullSubentPathArray guideCurves, // TODO
	const OdDbFullSubentPath pathCurve,        // TODO
	const OdDbLoftOptions& loftOptions,        // TODO
	OdDbObjectId& actionBodyId)
{
  OdDbObjectIdArray sourceEntites;
	OdDbObjectIdArray guideEntites;
	OdDbObjectIdArray pathEntites;

	if (entPathArray.length() < 2)
    return eInvalidInput;
	OdDbObjectIdArray pathArray;

	for (OdUInt32 i = 0; i < entPathArray.size(); i++)
	{
		pathArray = entPathArray[i].objectIds();

		//pathArray = i.objectIds();
    //OdDbViewportPtr viewportV = OdDbViewport::cast(pathArray[0].openObject());

    //if (!viewportV.isNull())
    //{
    //  pathArray.removeFirst();
    //}
    //OdDbObjectPtr d = pathArray[pathArray.length() - 1].openObject();
    sourceEntites.append(pathArray[pathArray.length() - 1]);
  }
  
  OdDbDatabase *pDb = pathArray[0].database();

	for (OdUInt32 i = 0; i < guideCurves.size(); i++)
	{
		pathArray = guideCurves[i].objectIds();
		guideEntites.append(pathArray[pathArray.length() - 1]);
	}

  OdDbAssocLoftParametersPtr pParameters = OdDbAssocLoftParameters::createObject();
  OdResult res = eInvalidInput;

  if (sourceEntites.isEmpty())
    return eInvalidInput;

  OdDbBlockTablePtr pMainBT = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectId ownerBTRId = sourceEntites.first().openObject()->ownerId();
  OdDbBlockTableRecordPtr pOwnerSpace = ownerBTRId.safeOpenObject(OdDb::kForWrite);
	const OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject(pOwnerSpace->objectId(), true);

  //create loft reference
  OdDbLoftedSurfacePtr pLoftedSurface = OdDbLoftedSurface::createObject();

  pDb->addOdDbObject(pLoftedSurface, ownerBTRId);

	OdRxClass * pClass = OdDbAssocLoftedSurfaceActionBody::desc();
  OdDbObjectId actionId;

  if (eOk != createActionAndActionBodyAndPostToDatabase(pClass, networkId, actionId, actionBodyId))
  {
    return eInvalidInput;
  }
  //END
  OdDbAssocLoftedSurfaceActionBodyPtr pActionBody = OdDbAssocActionBody::cast(actionBodyId.openObject(OdDb::kForWrite));
  //append and  loft ref
  pOwnerSpace->appendOdDbEntity(pLoftedSurface);
  //arrayId = pArrayBlock->objectId();

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
  pAction->setActionBody(actionBodyId);

  OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();
  pDb->addOdDbObject(pDependency);
  pDependency->setIsReadDependency(false);
  pDependency->setIsWriteDependency(true);
  pDependency->setOrder(0 - 2147483648u);
  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty();
  pCompoundObjectId->set(pLoftedSurface->objectId());
  pDependency->attachToObject(pCompoundObjectId);
  pAction->addDependency(pDependency->objectId(), true);

  OdDbObjectIdArray pathActionParams;
  int paramIndex = 0;

  for (unsigned int i = 0; i < sourceEntites.length(); i++)
  {
    OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();

    pDb->addOdDbObject(pDependency);
    pDependency->setIsReadDependency(true);
    pDependency->setIsWriteDependency(false);
    pCompoundObjectId = OdDbCompoundObjectId::createObject();

    OdDbObjectPtr assocObPtr = sourceEntites.getAt(i).openObject();

    pCompoundObjectId->set(assocObPtr->objectId());
    pDependency->attachToObject(pCompoundObjectId);
    pAction->addDependency(pDependency->objectId(), true);

    OdDbObjectId sourceId;
    OdDbAssocGeomDependencyPtr pGeomDependency;
	  const OdDbObjectId pathParam = createOdDdAssocPathActionParam(actionId, assocObPtr->objectId(), pDependency, kLoftCrossSectionName, paramIndex);
    pathActionParams.append(pathParam);
    paramIndex++;
  }

	for (unsigned int i = 0; i < guideEntites.length(); i++)
	{
		OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();

		pDb->addOdDbObject(pDependency);
		pDependency->setIsReadDependency(true);
		pDependency->setIsWriteDependency(false);
		pCompoundObjectId = OdDbCompoundObjectId::createObject();

		OdDbObjectPtr assocObPtr = guideEntites.getAt(i).openObject();

		pCompoundObjectId->set(assocObPtr->objectId());
		pDependency->attachToObject(pCompoundObjectId);
		pAction->addDependency(pDependency->objectId(), true);

		OdDbObjectId sourceId;
		OdDbAssocGeomDependencyPtr pGeomDependency;
		const OdDbObjectId pathParam = createOdDdAssocPathActionParam(actionId, assocObPtr->objectId(), pDependency, kLoftGuideCurveName, paramIndex);
		pathActionParams.append(pathParam);
		paramIndex++;
	}

	const short continuity = 1;
	const double bulge = 0.5;
  pParameters->setInputVariables(sourceEntites, continuity, bulge);
  pActionBody->parameters() = pParameters;
  const_cast<OdDbAssocLoftParametersPtr &>(pParameters)->setOwner(pActionBody);

	OdDbObjectId pathId = NULL;

	if (pathCurve.objectIds().length() > 0)
	{
		pathArray = pathCurve.objectIds();
		pCompoundObjectId = OdDbCompoundObjectId::createObject();

		OdDbObjectPtr assocObPtr = pathArray[pathArray.length() - 1].openObject();

		pCompoundObjectId->set(assocObPtr->objectId());
		pDependency->attachToObject(pCompoundObjectId);
		pAction->addDependency(pDependency->objectId(), true);
		pathId = assocObPtr->objectId();
  }

	const OdDbObjectId pathParam = createOdDdAssocPathActionParam(actionId, pathId, pDependency, kLoftPathCurveName);
	pathActionParams.append(pathParam);
  // See https://jira.opendesign.com/browse/CORE-12899  even if undo is fixed to not throw restoring empty object
  // this is optimization
  pDb->disableUndoRecording(true);
  pActionBody->evaluateOverride();
  pDb->disableUndoRecording(false);
  return eOk;
}

OdStringArray  OdDbImpAssocLoftedSurfaceActionBody::compareWith(OdDbImpAssocActionBody *bodyIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
{
  OdStringArray retArr;
  OdDbImpAssocLoftedSurfaceActionBody* comparedBody = static_cast<OdDbImpAssocLoftedSurfaceActionBody *>(bodyIDToCompare);
  ODA_ASSERT(comparedBody != NULL);

  if (comparedBody != NULL)
  {
    retArr = OdDbImpAssocPathBasedSurfaceActionBody::compareWith(bodyIDToCompare, filerVersion /*OdDb::kDHL_CURRENT*/);

    if (m_unknownInt32 != comparedBody->m_unknownInt32)
      retArr.append(CS_NOT_CRITICAL);
  }

  return retArr;
}
