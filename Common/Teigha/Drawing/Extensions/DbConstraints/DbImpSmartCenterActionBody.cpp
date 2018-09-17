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
#include "DbImpSmartCenterActionBody.h"
#include "DbImpAssocActionBody.h"
#include "NextCodeDefs.h"
#include "DbAssocDependency.h"
#include "DbSmartCenterActionBody.h"
#include "DbLine.h"
#include "DbLinetypeTable.h"
#include "DbHostAppServices.h"
#include "DbBlockTable.h"
#include "DbLayerTableRecord.h"
#include "DbLinetypeTableRecord.h"
#include "DbIdMapping.h"
#include "DbAssocNetworkDeepCloneAux.h"
#include "DbDictionary.h"
#include "DbSymUtl.h"
#include "OdDbAssocPersSubentIdPE.h"
//////////////////////////////////////////////////////////////////////////

OdDbImpSmartCenterActionBodyBase::OdDbImpSmartCenterActionBodyBase()
{
}

OdDbImpSmartCenterActionBodyBase::~OdDbImpSmartCenterActionBodyBase()
{
}

OdResult OdDbImpSmartCenterActionBodyBase::dwgInFields(OdDbDwgFiler* pFiler)
{
	OdResult res = OdDbImpAssocActionBody::dwgInFields(pFiler);
	if (res != eOk)
		return res;

	return eOk;
}

void OdDbImpSmartCenterActionBodyBase::dwgOutFields(OdDbDwgFiler* pFiler) const
{
	OdDbImpAssocActionBody::dwgOutFields(pFiler);
}

OdResult OdDbImpSmartCenterActionBodyBase::dxfInFields(OdDbDxfFiler* pFiler)
{
	OdResult res = OdDbImpAssocActionBody::dxfInFields(pFiler);

	if (res != eOk)
		return res;

	return eOk;
}

void OdDbImpSmartCenterActionBodyBase::dxfOutFields(OdDbDxfFiler* pFiler) const
{
	OdDbImpAssocActionBody::dxfOutFields(pFiler);
}

void OdDbImpSmartCenterActionBodyBase::evaluateOverride(OdDbObjectId parentActionId)
{
	OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForWrite));
}

//////////////////////////////////////////////////////////////////////////

OdDbImpSmartCenterActionBody::OdDbImpSmartCenterActionBody()
{
  m_pParamActionBody = new OdDbImpAssocParamBasedActionBody(true);
}

OdDbImpSmartCenterActionBody::~OdDbImpSmartCenterActionBody()
{
  if (m_pParamActionBody != NULL)
    delete m_pParamActionBody;
}

OdResult OdDbImpSmartCenterActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
	OdResult res = OdDbImpSmartCenterActionBodyBase::dwgInFields(pFiler);
	if (res != eOk)
		return res;

	if (pFiler->dwgVersion() <= OdDb::vAC24)
	{
		res = m_pParamActionBody->dwgInFields(pFiler);

		if (res != eOk)
			return res;

		m_arrParams = m_pParamActionBody->m_arrParams;
		m_arrValueParams = m_pParamActionBody->m_arrValueParams;
	}

	//if (!pFiler->atSubclassData(OdDbSmartCenterActionBody::desc()->name()))
	//{
	//	ODA_FAIL_ONCE();
	//	return eMakeMeProxy;
	//}

  int ver = pFiler->rdInt32();
  ODA_ASSERT_ONCE(!ver); // TODO
  if (ver)
    return eMakeMeProxy;

	return eOk;
}

void OdDbImpSmartCenterActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
	OdDbImpSmartCenterActionBodyBase::dwgOutFields(pFiler);

  if (pFiler->dwgVersion() <= OdDb::vAC24)
  {
    m_pParamActionBody->m_arrParams = m_arrParams;
    m_pParamActionBody->m_arrValueParams = m_arrValueParams;
    m_pParamActionBody->dwgOutFields(pFiler);
  }

  int ver = 0;
  pFiler->wrInt32(ver);
}

OdResult OdDbImpSmartCenterActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	OdResult res = OdDbImpSmartCenterActionBodyBase::dxfInFields(pFiler);
	if (res != eOk)
		return res;

	//if (pFiler->atSubclassData(OdDbAssocParamBasedActionBody::desc()->name()))
	if (m_pParamActionBody != NULL)
	{
		res = m_pParamActionBody->dxfInFields(pFiler);

		if (res != eOk)
			return res;

		m_arrParams = m_pParamActionBody->m_arrParams;
		m_arrValueParams = m_pParamActionBody->m_arrValueParams;
	}

	if (!pFiler->atSubclassData(OdDbSmartCenterActionBody::desc()->name()))
	{
		ODA_FAIL_ONCE();
		return eMakeMeProxy;
	}

	NEXT_CODE(90)

  OdUInt32 ver = pFiler->rdUInt32();

  ODA_ASSERT_ONCE(!ver);

  if (ver)
    return eMakeMeProxy;

	return eOk;
}

void OdDbImpSmartCenterActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
	OdDbImpSmartCenterActionBodyBase::dxfOutFields(pFiler);

  if (pFiler->dwgVersion() <= OdDb::vAC24)
  {
    //pFiler->wrSubclassMarker(OdDbAssocParamBasedActionBody::desc()->name());
    m_pParamActionBody->m_arrParams = m_arrParams;
    m_pParamActionBody->m_arrValueParams = m_arrValueParams;
    m_pParamActionBody->dxfOutFields(pFiler);
  }

	pFiler->wrSubclassMarker(OdDbSmartCenterActionBody::desc()->name());

  OdUInt32 ver = 0;
  pFiler->wrUInt32(90, ver);
}

void OdDbImpSmartCenterActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.safeOpenObject(OdDb::kForWrite));
}

//void OdDbImpSmartCenterActionBody::startSettingEntities(OdDbBlockTableRecordPtr pRec)
//{
//  m_entityIter = pRec->newIterator();
//}

void OdDbImpSmartCenterActionBody::setAddedEntityPropertiesByBlock(OdDbEntityPtr pLine, OdDbAssocActionPtr pAction)//const double newScale, const OdDb::LineWeight newWeight, const OdCmColor newColor)
{
  if (!pLine.isNull())
  {
		OdDbDatabase *pDb = pAction->database();

		pLine->setLayer(pDb->getLayerZeroId());
		double scale = pDb->getCENTERLTSCALE();
		pLine->setLinetypeScale(scale);// TODO : byBlock needs
		pLine->setMaterial(pDb->byBlockMaterialId());
		pLine->setTransparency(OdCmTransparency::kByBlock);
		pLine->setLineWeight(OdDb::kLnWtByBlock);
		pLine->setColor(OdCmEntityColor::kByBlock);
		pLine->setPlotStyleName(OdDb::kPlotStyleNameByBlock);

		//pLine->setThickness(pDb->getTHICKNESS()); // TODO : byBlock needs

		// TODO: From setDatabaseDefaults, byBlock needs
		//if (!GETBIT(pImpl->m_entRtFlags, OdDbEntityImpl::kRtLineweightSet))
		//	pImpl->m_nLineweightIndex = OdDbUtils::lineWeightIndex(pDb->getCELWEIGHT());
		//if (!GETBIT(pImpl->m_entRtFlags, OdDbEntityImpl::kRtPlotStyleNameTypeSet))
		//	pImpl->setPlotStyleNameType(pDb->getCEPSNTYPE());
		//if (pImpl->m_PlotStyleNameId.isNull())
		//	pImpl->setPlotStyleId(pDb->getCEPSNID());
		//pImpl->setShadowsFlags(pDb->getCSHADOW());
  }
}

OdDbLinetypeTableRecordPtr OdDbImpSmartCenterActionBody::getCenterLType(OdDbDatabase* pDb, OdString newType)
{
	OdDbObjectId idLineType;
  OdDbLinetypeTableRecordPtr lineTypeTableRec;

	if (!pDb)
		throw OdError(eNoDatabase);

	if (newType.isEmpty())
	{
		newType = pDb->getCENTERLTYPE();
	}

	if (newType == L".")
	{
		idLineType = pDb->getCELTYPE();
	}
	else
	{
		OdDbLinetypeTablePtr pTable = pDb->getLinetypeTableId().openObject();

		idLineType = pTable->getAt(newType);

		if (idLineType.isNull())
		{
			OdString  centerLinelineTypeFile = pDb->getCENTERLTYPEFILE();
			OdDbHostAppServices* pHost = pDb->appServices();
			OdString fileFound = pHost->findFile(centerLinelineTypeFile, pDb, OdDbHostAppServices::kCompiledShapeFile);

			if (!fileFound.isEmpty())
			{
				try
				{
					pDb->loadLineTypeFile(newType, fileFound);
					idLineType = pTable->getAt(newType);
				}
				catch (...)
				{
				}
			}
		}
		
		if (idLineType.isNull())
		{
			idLineType = pDb->getLinetypeByLayerId();
		}
	}

  lineTypeTableRec = idLineType.safeOpenObject();
	return lineTypeTableRec;
}

enum OdResult OdDbImpSmartCenterActionBody::setSubentLinetypeScale(double newOvershoot, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("SubentLinetypeScale"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

double OdDbImpSmartCenterActionBody::subentLinetypeScale(OdDbAssocActionPtr pAction) const
{
    double retD = -1.0;
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("SubentLinetypeScale"), *value.get(), expression, evaluatorId, 0) != eOk)
        return -1.0;

    return value->getDouble();
}

enum OdResult OdDbImpSmartCenterActionBody::setSubentLineWeight(double newOvershoot, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("SubentLineWeight"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

OdInt32 OdDbImpSmartCenterActionBody::subentLineWeight(OdDbAssocActionPtr pAction) const // the property subentLineWeight is double in the AcDbAssocAction page
{
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(double());
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("SubentLineWeight"), *value.get(), expression, evaluatorId, 0) != eOk)
        return -1;

    OdInt32 ret = value->getDouble();
    return ret;
}

enum OdResult OdDbImpSmartCenterActionBody::setSubentLineType(OdString newOvershoot, OdDbAssocActionPtr pAction)
{
    const OdDbEvalVariantPtr value = OdDbEvalVariant::init(newOvershoot);
    OdString emptyStr("");
    return pAction->setValueParam(OD_T("SubentLineType"), *value.get(), OD_T(""), OD_T(""), emptyStr, false, 0);
}

OdString OdDbImpSmartCenterActionBody::subentLineType(OdDbAssocActionPtr pAction) const
{
    OdString retD = OD_T("");
    OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);
    OdString expression(""), evaluatorId("");

    if (pAction->getValueParam(OD_T("SubentLineType"), *value.get(), expression, evaluatorId, 0) != eOk)
    {
        ODA_ASSERT(!"Line type not found");
        return OD_T("");
    }

    return value->getString();
}
//void OdDbTableImpl::finishSettingEntities(OdDbBlockTableRecordPtr pRec, OdGeMatrix3d matr)
//{
//  while (!m_entityIter->done())
//  {
//    OdDbEntityPtr obj = m_entityIter->entity(OdDb::kForWrite);
//    obj->erase();
//    m_entityIter->step();
//  }
//  if (OdNonZero(matr.scale() - 1.0))
//  {
//    m_entityIter = pRec->newIterator();
//    while (!m_entityIter->done())
//    {
//      OdDbEntityPtr pEntity = m_entityIter->entity(OdDb::kForWrite);
//      pEntity->transformBy(matr);
//      m_entityIter->step();
//    }
//  }
//}

OdDbObjectId OdDbImpSmartCenterActionBody::getCenterLayerId(OdDbDatabase* pDb)
{
	OdDbObjectId idLayer;
	OdString strLayer = pDb->getCENTERLAYER();
	if (strLayer == L".")
	{
		idLayer = pDb->getCLAYER();
	}
	else
	{
    idLayer = OdDbSymUtil::getLayerId(pDb, strLayer);
	}
	return idLayer;
}

OdDbAssocGeomDependencyPtr  getAssocGeomDependencyPtr(OdDbAssocActionPtr pAction, OdDb::OpenMode mode, int geomDepNumber)
{
  OdDbAssocGeomDependencyPtr pGeomDep;
  OdDbObjectIdArray idDependencies;
  OdResult res;

  res = pAction->getDependencies(true, false, idDependencies);

  if (res != eOk)
    return pGeomDep;

  int counter = 0;
  unsigned int i = 0;

  for (i = 0; i < idDependencies.length(); i++)
  {
    pGeomDep = OdDbAssocGeomDependency::cast(idDependencies[i].openObject(mode));
    if (!pGeomDep.isNull())
    {
      if (geomDepNumber == counter)
        break;

      counter++;
    }
  }

  if (i > idDependencies.length())
    return OdDbAssocGeomDependencyPtr();

  return pGeomDep;
}

OdResult OdDbImpSmartCenterActionBody::addMoreObjectsToDeepCloneOverride(const OdDbSmartCenterActionBody *pBody,
  OdDbIdMapping& idMap,
  OdDbObjectIdArray& additionalObjectsToClone) const
{
  // AlxeyTyurin 04182018 I had not OdDbBlockTableRecord as DependentObject for CenterObjects. At least for this time 

  //OdDbAssocActionPtr pAction = pBody->parentAction().safeOpenObject(OdDb::kForRead);

  //OdDbObjectIdArray pDepObjId;

  //pAction->getDependencies(true, true, pDepObjId);
  //OdDbIdPair IdPair(OdDbAssocDependency::cast(getIdDepArrayEntity(pDepObjId).openObject())
  //  ->dependentOnObject());
  //if (!idMap.compute(IdPair))
  //{
  //  return eOk;
  //}

  //pDepObjId.clear();
  //pAction->getDependentObjects(true, true, pDepObjId);

  //OdDbObjectIdArray::iterator it;

  //for (it = pDepObjId.begin(); it != pDepObjId.end(); it++)
  //{
  //  OdDbObjectPtr ttt = it->openObject(OdDb::kForRead);

  //  OdDbBlockTableRecordPtr pBTR = OdDbBlockTableRecord::cast(it->openObject(OdDb::kForRead));
  //  if (!pBTR.isNull())
  //  {
  //    additionalObjectsToClone.append(*it);
  //  }
  //}

  return eOk;
}

OdResult OdDbImpSmartCenterActionBody::postProcessAfterDeepCloneOverride(const OdDbSmartCenterActionBody *pBody, OdDbIdMapping& idMap)//(const OdDbSmartCenterActionBody *pBody, OdDbIdMapping& idMap)
{
  // Map dependencies of destination action
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(pBody->parentAction().openObject(OdDb::kForRead));

  OdDbIdPair searchForDest(pAction->objectId());
  if (!idMap.compute(searchForDest) || !searchForDest.isCloned() || searchForDest.value().isErased())
  {
    return eOk;
  }

  //// This action is kept in temporary dictionary
  OdDbAssocActionPtr pTempAction = OdDbAssocAction::cast(searchForDest.value().openObject(OdDb::kForWrite));

  OdDbSmartCenterActionBodyPtr pTempBody = pTempAction->actionBody().openObject(OdDb::kForWrite);
  pTempBody->parameters()->setOwner(pTempBody);

  ////TODO who and why added them (comment from ArrayActionBody method )
  OdDbObjectIdArray tempActionPersReactIDs = pTempAction->getPersistentReactors();
  if (!tempActionPersReactIDs.isEmpty())
  {
    OdDbObjectIdArray::iterator it;
    for (it = tempActionPersReactIDs.begin();
    it != tempActionPersReactIDs.end();
      it++)
    {
      pTempAction->removePersistentReactor(*it);
    }
  }

  OdDbObjectIdArray DepIDs;
  OdDbIdPair pairId;

  OdDbGeomRefPtr rowProfile;
  OdDbIdPair pathEntityIdPair;
  OdDbObjectId pathDependencyId;
  ////check src arrayEntity presence, remove from temp dictionary if absent
  pAction->getDependencies(true, true, DepIDs);

  OdDbAssocDependencyPtr pDepArrayEntity = getIdDepArrayEntity(DepIDs).openObject(OdDb::kForRead);

  if (!pDepArrayEntity.isNull())
    pairId.setKey(pDepArrayEntity->dependentOnObject());
  
  if (!idMap.compute(pairId))
  {
    if (!pathEntityIdPair.value().isNull())
    {
      OdDbEntityPtr pPath = OdDbEntity::cast(pathEntityIdPair.value().safeOpenObject(OdDb::kForWrite));

      pairId.setKey(pathDependencyId);
      if (idMap.compute(pairId))
        pPath->removePersistentReactor(pairId.value());
    }

    OdString tempAllActionDictName = OdDbAssocNetworkDeepCloneAux::getTempAssocDictionaryName();
    OdDbObjectId globalDictId = pTempAction->database()->getNamedObjectsDictionaryId();
    OdDbDictionaryPtr pGlobalDict = globalDictId.safeOpenObject(OdDb::kForWrite);
    ODA_ASSERT(!pGlobalDict.isNull());
    OdDbObjectId tempAllActionDictId = pGlobalDict->getAt(tempAllActionDictName);
    OdDbDictionaryPtr pTempAllActionDict = tempAllActionDictId.safeOpenObject(OdDb::kForWrite);
    pTempAllActionDict->remove(pTempAction->id());
    pTempAllActionDict = 0;
    DepIDs.clear();
    pTempAction->getDependencies(true, true, DepIDs);
    pDepArrayEntity = getIdDepArrayEntity(DepIDs).openObject(OdDb::kForRead);
    // copy Center Object without object to assotiate
    if (!pDepArrayEntity.isNull())
      pTempAction->removeDependency(pDepArrayEntity->objectId(), true);
  }

  std::map<OdDbObjectId, OdDbObjectId> * pSrcToDestIdMap = OdDbAssocNetworkDeepCloneAux::srcToDestIdMap(idMap);

  OdDbObjectId srcBlockId = pAction->objectThatOwnsNetworkInstance();
  OdDbObjectId destBlockId = pSrcToDestIdMap ? (*pSrcToDestIdMap)[srcBlockId] : OdDbObjectId();
  ODA_ASSERT(!destBlockId.isNull());

  OdDbAssocNetworkDeepCloneAux::moveCompleteActionToDestNetwork(pTempAction, destBlockId, idMap.deepCloneContext());
  
  return eIteratorDone;
}

const OdDbSmartCenterCommonParametersPtr OdDbImpSmartCenterActionBody::parameters() const
{
  return m_pParams;
}

OdDbSmartCenterCommonParametersPtr& OdDbImpSmartCenterActionBody::parameters()
{
  return m_pParams;
}

OdDbViewportPtr OdDbImpSmartCenterActionBody::getViewPortFromCompoundObject(OdDbAssocGeomDependencyPtr pGeomDep)
{
  OdDbViewportPtr viewportV;
  OdDbCompoundObjectIdPtr compobjId;
  OdResult res = pGeomDep->getDependentOnCompoundObject(compobjId);
  OdDbObjectIdArray pathArray;

  if (!compobjId.isNull())
  {
    compobjId->getPath(pathArray);

    if (pathArray.length() > 0)
      viewportV = OdDbViewport::cast(pathArray.getAt(0).safeOpenObject());
  }

  return viewportV;
}

OdResult OdDbImpSmartCenterActionBody::addSubentitySpecialSettings(OdDbEntityPtr ent, const OdDbSubentId subentId, OdUInt32 idx, OdDbSmartCenterActionBodyPtr pActionBody)
{ 
  OdDbAssocPersSubentIdPEPtr pPE = ent->isA()->getX(OdDbAssocPersSubentIdPE::desc());

  return pPE->addSubentitySpecialSettings(ent, subentId, idx, pActionBody);
}
