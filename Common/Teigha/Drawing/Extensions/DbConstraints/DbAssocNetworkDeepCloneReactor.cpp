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

#include "DbAssocNetworkDeepCloneReactor.h"
#include "DbAssocNetworkDeepCloneAux.h"
#include "DbAssocNetworkLongTransactionReactor.h"
#include "DbAssocVariable.h"
#include "DbAssocValueDependency.h"
#include "DbHostAppServices.h"
#include "ApLongTransactions.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocNetwork.h"
#include "DbBlockBegin.h"
#include "DbBlockEnd.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "DbCircle.h"
#include "DbDimension.h"
#include "DbIdMapping.h"
#include "DbDictionary.h"

#define STL_USING_ALGORITHM
#define STL_USING_LIST
#include "OdaSTL.h"



// Taken from DbCloneUtils which are not visible in extensions
template<class T> class CloneHelper
{
  OdDbObjectId m_owner;
  OdDbIdMapping& m_idMap;

public:
  CloneHelper(const OdDbObjectId& owner, OdDbIdMapping& idMap)
    : m_owner(owner)
    , m_idMap(idMap)
  {
  }
  void operator()(const OdDbObjectId& id)
  {
    OdDbObjectPtr pObj = id.openObject();
    if (pObj.get())
    {
      pObj = T::clone(pObj, m_idMap, m_owner);
      if (pObj.get())
      {
        pObj->setOwnerId(m_owner);
      }
      else
      {
        OdDbIdPair idPair(id);
        if (m_idMap.compute(idPair) && idPair.isCloned() && !idPair.isOwnerXlated())
        {
          idPair.value().openObject(OdDb::kForWrite)->setOwnerId(m_owner);
        }
      }
    }
  }
};

struct Deep
{
  static OdDbObjectPtr clone(const OdDbObject* pObj, OdDbIdMapping& idMap, const OdDbObjectId& owner)
  {
    return pObj->deepClone(idMap, owner.openObject());
  }
};

struct Wblock
{
  static OdDbObjectPtr clone(const OdDbObject* pObj, OdDbIdMapping& idMap, const OdDbObjectId& owner)
  {
    // Prevent from appearing assoc network in map
    OdDbObjectId searchOwnerId = pObj->ownerId();
    OdDbIdPair pair(searchOwnerId);
    bool ownerIsFound = idMap.compute(pair);
    if (!ownerIsFound || pair.value().isNull())
    {
      pair.setValue(owner);
      pair.setCloned(false);
      pair.setOwnerXlated(true);
      idMap.assign(pair);
    }
    OdDbObjectPtr pRes = pObj->wblockClone(idMap, owner.openObject());
    if (!ownerIsFound)
    {
      idMap.del(searchOwnerId);
    }
    return pRes;
  }
};

static void assocNetworkDeepCloneObjects(const OdDbObjectIdArray& objectIds, OdDbObjectId owner, OdDbIdMapping& idMap)
{
  std::for_each(objectIds.begin(), objectIds.end(), CloneHelper<Deep>(owner, idMap));
}
static void assocNetworkWblockCloneObjects(const OdDbObjectIdArray& objectIds, OdDbObjectId owner, OdDbIdMapping& idMap)
{
  std::for_each(objectIds.begin(), objectIds.end(), CloneHelper<Wblock>(owner, idMap));
}

OdDbAssocNetworkDeepCloneReactor::OdDbAssocNetworkDeepCloneReactor()
{
  m_Counter = 0;
}
OdDbAssocNetworkDeepCloneReactor::~OdDbAssocNetworkDeepCloneReactor()
{
  m_Counter = 0;
}

void OdDbAssocNetworkDeepCloneReactor::makeEmergencyClear(OdDbIdMapping& idMap)
{
  try
  {
    OdDbAssocNetworkDeepCloneReactorDataPtr pData = getData(idMap);
    ODA_ASSERT(pData.get());
    if (pData)
    {
      pData->m_dcMapping.clear(true);
    }
  }
  catch (...) {}
}

static bool isAssocNetworkIsRegisteredInGlobal(OdDbAssocNetworkPtr pNetwork)
{
  if (!pNetwork.get())
    return false;
  OdDbDatabase * pDb = pNetwork->database();
  if (!pDb)
    return false;
  OdDbObjectId globalNetworkId = OdDbAssocNetwork::getInstanceFromDatabase(pDb, false);
  OdDbAssocNetworkPtr pGlobalNetwork = OdDbAssocNetwork::cast(globalNetworkId.openObject());
  if (!pGlobalNetwork.get())
    return false;
  OdDbObjectIdArray aAllAssocActions = pGlobalNetwork->getActions();
  return aAllAssocActions.contains(pNetwork->objectId());
}

// OdRxEventReactor methods
void OdDbAssocNetworkDeepCloneReactor::beginDeepCloneXlation(OdDbIdMapping& idMap)
{
  OdDbObjectIdArray aSrcObjectsSoFar;
  try
  {

    if (isInLongTransaction(idMap))
      return;

    OdDbAssocNetworkDeepCloneReactorDataPtr pData = getData(idMap);
    ODA_ASSERT(pData.get());
    if (pData)
    {
      pData->m_aSrcDimensionsCloned.clear();
      pData->m_aAllTheSrcAssocIds.clear();
      pData->m_dcMapping.init(idMap, true);
    }

    OdDbDatabasePtr pSrcDb(idMap.origDb());
    OdDbDatabasePtr pDestDb(idMap.destDb());

    std::map<OdDbObjectId, OdDbObjectId> * pSrcToDestIdMap = OdDbAssocNetworkDeepCloneAux::srcToDestIdMap(idMap);
    if (!pSrcToDestIdMap || pSrcToDestIdMap->empty())
      return;

    OdDbIdMappingIterPtr pIt = idMap.newIterator();
    for (; !pIt->done(); pIt->next())
    {
      OdDbIdPair idPair;
      pIt->getMap(idPair);
      aSrcObjectsSoFar.append(idPair.key());
    }
    pIt = 0;

    OdString tempAllActionDictName = OdDbAssocNetworkDeepCloneAux::getTempAssocDictionaryName();
    OdDbObjectId globalDictId = pDestDb->getNamedObjectsDictionaryId();
    OdDbDictionaryPtr pGlobalDict = OdDbDictionary::cast(globalDictId.openObject(OdDb::kForWrite));
    ODA_ASSERT(!pGlobalDict.isNull());
    if (pGlobalDict.isNull())
      return;

    OdDbObjectId tempAllActionDictId = pGlobalDict->getAt(tempAllActionDictName);
    ODA_ASSERT(tempAllActionDictId.isNull());
    if (!tempAllActionDictId.isNull())
    {
      pGlobalDict->remove(tempAllActionDictId);
    }

    OdDbObjectId BTId = pDestDb->getBlockTableId();
    // Open only for assert
    OdDbBlockTablePtr pBT = OdDbBlockTable::cast(BTId.openObject(OdDb::kForRead));
    ODA_ASSERT(!pBT->has(tempAllActionDictName));
    pBT = 0;

    std::map<OdDbObjectId, OdDbObjectId>::iterator it = pSrcToDestIdMap->begin();
    for (; it != pSrcToDestIdMap->end(); ++it)
    {
      OdDbObjectId srcBlockId = it->first;
      OdDbObjectId srcNetworkId = OdDbAssocNetwork::getInstanceFromObject(srcBlockId, false);
      OdDbAssocNetworkPtr pSrcNetwork = OdDbAssocNetwork::cast(srcNetworkId.openObject());
      if (pSrcNetwork.isNull())
        continue;

      if (!isAssocNetworkIsRegisteredInGlobal(pSrcNetwork)) // Detached assoc network say about the problem in source. We are better to skip it
        continue;
      // Clone all the actions in special temporary dictionary

      OdDbObjectId tempAllActionDictId = pGlobalDict->getAt(tempAllActionDictName);
      if (tempAllActionDictId.isNull())
      {
        OdDbDictionaryPtr pTempAllActionDict = OdDbDictionary::createObject();
        tempAllActionDictId = pGlobalDict->setAt(tempAllActionDictName, pTempAllActionDict);
      }

      OdDbObjectIdArray aSrcActionToCloneId = pSrcNetwork->getActions();
      pSrcNetwork = 0;

      int copyMode = idMap.origDb()->appServices()->getPARAMETERCOPYMODE();
      OdDbObjectIdArray aClearDepActionId = OdDbAssocNetworkDeepCloneAux::getUsedActions(aSrcActionToCloneId, copyMode);

      unsigned int actionLength;
      aSrcActionToCloneId = aClearDepActionId;

      // For dimensions repeat acad behaviuor - if we clone only entities on which dimensions are layed,
      // We have to deep clone OdDbDimension also
      // We have to clone ALL the dimensions because all the groups are cloned.
      // It means that cloned geom dependencies refer to dimensions: cloned, if they were cloned, original, if they were not cloned
      // We get big troubles of our cloned geom dependencies refer to original dimensions
      // So clone them and they will be cleared

      actionLength = aSrcActionToCloneId.length();
      OdDbObjectIdArray aDimensionsId;
      for (unsigned int i = 0; i < actionLength; i++)
      {

        OdDbObjectId actionId = aSrcActionToCloneId[i];
        OdDbAssoc2dConstraintGroupPtr pGroup = OdDbAssoc2dConstraintGroup::cast(actionId.openObject());
        if (pGroup.isNull())
          continue;
        OdArray<OdGeomConstraint*> apConstraints;
        if (eOk != pGroup->getConstraints(apConstraints))
        {
          continue;
        }
        pGroup = 0;

        unsigned int gcLength = apConstraints.length();
        for (unsigned int j = 0; j < gcLength; j++)
        {
          OdGeomConstraint * pGeomConstraint = apConstraints[j];
          // We need explicit constraint
          if (!pGeomConstraint->isKindOf(OdExplicitConstraint::desc()))
            continue;

          OdExplicitConstraint * pExplConstraint = (OdExplicitConstraint *)pGeomConstraint;
          OdDbObjectId dimDepId = pExplConstraint->dimDependencyId();
          OdDbAssocDependencyPtr pDimDep = OdDbAssocDependency::cast(dimDepId.openObject());
          if (pDimDep.isNull())
            continue;
          OdDbObjectId dimensionId = pDimDep->dependentOnObject();
          OdDbIdPair dimIdSearch(dimensionId);
          if ((!idMap.compute(dimIdSearch) || !dimIdSearch.isCloned()) && !aDimensionsId.contains(dimensionId))
          {
            if (pData)
            {
              pData->m_aAllTheSrcAssocIds.append(dimensionId);
              pData->m_aSrcDimensionsCloned.append(dimensionId);
            }
            aDimensionsId.append(dimensionId);
          }
        }
      }

      // Clone dimensions first
      unsigned int dimToCloneLength = aDimensionsId.length();
      if (dimToCloneLength)
      {

        OdDbObjectId BTId = pDestDb->getBlockTableId();
        OdDbBlockTablePtr pBT = OdDbBlockTable::cast(BTId.openObject());
        ODA_ASSERT(!pBT.isNull());
        if (pBT.isNull())
          return;

        OdDbObjectId tempAssocBTRId = pBT->getAt(tempAllActionDictName);
        if (tempAssocBTRId.isNull())
        {
          OdDbBlockTableRecordPtr pBTR = OdDbBlockTableRecord::createObject();
          pBTR->setName(tempAllActionDictName);
          pBT->upgradeOpen();
          tempAssocBTRId = pBT->add(pBTR);
          pBT->downgradeOpen();
          pBTR = 0;
        }
        pBT = 0;

        if (pSrcDb == pDestDb)
        {
          assocNetworkDeepCloneObjects(aDimensionsId, tempAssocBTRId, idMap);
        }
        else
        {
          assocNetworkWblockCloneObjects(aDimensionsId, tempAssocBTRId, idMap);
        }
      }

      if (pSrcDb == pDestDb)
      {
        assocNetworkDeepCloneObjects(aSrcActionToCloneId, tempAllActionDictId, idMap);
      }
      else
      {
        assocNetworkWblockCloneObjects(aSrcActionToCloneId, tempAllActionDictId, idMap);
      }

    }
    //ODA_TRACE_IDMAPPING(idMap, OD_T("1"));

    //addMoreObjectsToDeepClone
    //BEGIN
    std::list<OdDbObjectPtr> pActions;
    OdDbIdMappingIterPtr pMapIt = idMap.newIterator();
    for (pMapIt->start(); !pMapIt->done(); pMapIt->next())
    {
      OdDbIdPair idPair;
      pMapIt->getMap(idPair);

      OdDbObjectPtr pAction = idPair.key().openObject(OdDb::kForRead);
      if (pAction->isA() == OdDbAssocAction::desc())
      {
        pActions.push_back(pAction);
      }
    }

    std::list<OdDbObjectPtr>::iterator itActions;
    for (itActions = pActions.begin(); itActions != pActions.end(); itActions++)
    {
      OdDbObjectIdArray moreObj;

      OdDbAssocActionPtr pAction = OdDbAssocAction::cast(*itActions);
      pAction->addMoreObjectsToDeepClone(idMap, moreObj);

      OdDbObjectIdArray::iterator it;
      for (it = moreObj.begin(); it != moreObj.end(); it++)
      {
        OdDbObjectPtr pObj = it->openObject(OdDb::kForRead);
        pObj->deepClone(idMap, pObj->ownerId().openObject(OdDb::kForRead));
      }

      pMapIt = idMap.newIterator();
      for (pMapIt->start(); !pMapIt->done(); pMapIt->next())
      {
        OdDbIdPair idPair;
        pMapIt->getMap(idPair);

        OdDbObjectPtr pAction = idPair.key().openObject(OdDb::kForRead);
        if (pAction->isA() == OdDbAssocAction::desc()
          && std::find(pActions.begin(), pActions.end(), pAction) == pActions.end())
        {
          pActions.push_back(pAction);
        }
      }
    }
    //END

    //ODA_TRACE_IDMAPPING(idMap, OD_T("2"));
  }
  catch (...)
  {
    makeEmergencyClear(idMap);
    try
    {
      // Clean also idMap
      OdDbObjectIdArray aNewObjects;
      OdDbIdMappingIterPtr pIt = idMap.newIterator();
      for (; !pIt->done(); pIt->next())
      {
        OdDbIdPair idPair;
        pIt->getMap(idPair);
        if (!aSrcObjectsSoFar.contains(idPair.key()))
        {
          aNewObjects.append(idPair.key());
        }
      }
      pIt = 0;
      for (unsigned int i = 0; i < aNewObjects.length(); i++)
      {
        idMap.del(aNewObjects[i]);
      }
    }
    catch (...) {}
  }
}

void OdDbAssocNetworkDeepCloneReactor::abortDeepClone(OdDbIdMapping& idMap)
{
  try
  {
    std::map<OdDbObjectId, OdDbObjectId> * pSrcToDestIdMap = OdDbAssocNetworkDeepCloneAux::srcToDestIdMap(idMap);
    if (!pSrcToDestIdMap || pSrcToDestIdMap->empty())
    {
      if (pSrcToDestIdMap)
        OdDbAssocNetworkDeepCloneAux::cleanData(idMap);
      return;
    }

    if (isInLongTransaction(idMap))
    {
      OdDbAssocNetworkDeepCloneAux::cleanData(idMap);
      return;
    }

    // delete of actions will be done by deep clone mechanism
    deleteTempDict(idMap.destDb(), false);

    std::map<OdDbObjectId, OdDbObjectId>::iterator it = pSrcToDestIdMap->begin();
    for (; it != pSrcToDestIdMap->end(); ++it)
    {
      OdDbObjectId srcBlockId = it->first;
      OdDbObjectId srcNetworkId = OdDbAssocNetwork::getInstanceFromObject(srcBlockId, false);
      OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::cast(srcNetworkId.openObject());
      if (pNetwork.isNull())
        continue;
      pNetwork->postProcessAfterDeepCloneCancel(idMap);
    }

    OdDbAssocNetworkDeepCloneReactorDataPtr pData = getData(idMap);
    ODA_ASSERT(pData.get());
    if (pData)
      pData->m_dcMapping.clear();
  }
  catch (...) {}
  OdDbAssocNetworkDeepCloneAux::cleanData(idMap);
}

// This method is still useful for WBLOCK scenario when we have to clean empty groups of result database
static void removeEmptyGroupsNetworkAndDictionaries(OdDbObjectId blockId)
{
  OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject(blockId, false);
  if (networkId.isNull())
    return;
  OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::cast(networkId.openObject());
  ODA_ASSERT(!pNetwork.isNull());
  if (pNetwork.isNull())
    return;
  const OdDbObjectIdArray & aActionsId = pNetwork->getActions();
  for (unsigned int i=0; i<aActionsId.length(); i++)
  {
    bool removeAction = false;
    OdDbObjectId actionId = aActionsId[i];
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject());
    if (pAction.isNull())
    {
      OdDbObjectPtr pObject = actionId.openObject();
      if (pObject.isNull())
      {
        removeAction = true;
        continue;
      }
      else if (!pObject->isAProxy())
      {
        removeAction = true;
      }
    }
    else if (pAction->isKindOf(OdDbAssoc2dConstraintGroup::desc()))
    {
      OdDbObjectIdArray aDepIds;
      pAction->getDependencies(true, true, aDepIds);
      if (!aDepIds.length())
      {
        removeAction = true;
      }
    }
    if (removeAction)
    {
      pAction = 0;
      pNetwork->upgradeOpen();
      pNetwork->removeAction(actionId, true);
      pNetwork->downgradeOpen();
    }
  }

  // Network was cleaned from empty groups
  unsigned int actionLength = pNetwork->getActions().length();
  pNetwork = 0;
  if (!actionLength)
  {
    OdDbAssocNetwork::removeInstanceFromObject(blockId, true);
  }
}

void OdDbAssocNetworkDeepCloneReactor::endDeepClone(OdDbIdMapping& idMap)
{
  try
  {
    std::map<OdDbObjectId, OdDbObjectId> * pSrcToDestIdMap = OdDbAssocNetworkDeepCloneAux::srcToDestIdMap(idMap);
    if (!pSrcToDestIdMap)
      return;
    OdDbAssocNetworkDeepCloneReactorDataPtr pData = getData(idMap);
    ODA_ASSERT(pData.get());
    if (pData && pData->m_dcMapping.isFailure())
    {
      deleteTempDict(idMap.destDb(), true);
      OdDbAssocNetworkDeepCloneAux::cleanData(idMap);
      return;
    }
    if (pSrcToDestIdMap->empty() || isInLongTransaction(idMap))
    {
      OdDbAssocNetworkDeepCloneAux::cleanData(idMap);
      return;
    }

    //Already init
    ODA_ASSERT(pData.get());
    if (pData)
      pData->m_dcMapping.init(idMap, false);

    // Move dimensions to proper blocks
    unsigned int i, srcDimensionsClonedLength = pData ? pData->m_aSrcDimensionsCloned.length() : 0;
    if (srcDimensionsClonedLength)
    {
      std::map<OdDbObjectId, OdDbObjectIdArray> blockDimMap;
      for (i = 0; i < srcDimensionsClonedLength; i++)
      {
        OdDbObjectId srcDepId = pData->m_aSrcDimensionsCloned[i];
        OdDbIdPair srcDimIdSearch(srcDepId);
        if (!idMap.compute(srcDimIdSearch) || !srcDimIdSearch.isCloned())
          continue;

        OdDbObjectId destBlockId = (*pSrcToDestIdMap)[srcDepId.openObject()->ownerId()];
        if (blockDimMap.find(destBlockId) == blockDimMap.end())
        {
          blockDimMap.insert(std::pair<OdDbObjectId, OdDbObjectIdArray>(destBlockId, OdDbObjectIdArray()));
        }
        blockDimMap[destBlockId].append(srcDimIdSearch.value());
      }

      std::map<OdDbObjectId, OdDbObjectIdArray>::iterator blockDepIt = blockDimMap.begin();
      for (; blockDepIt != blockDimMap.end(); ++blockDepIt)
      {
        OdDbBlockTableRecordPtr pDestBlock = OdDbBlockTableRecord::cast(blockDepIt->first.openObject(OdDb::kForWrite));
        ODA_ASSERT(!pDestBlock.isNull());
        if (pDestBlock.isNull())
          continue;
        pDestBlock->assumeOwnershipOf(blockDepIt->second);
      }

      ODA_ASSERT(pData);
      if (pData)
        pData->m_aSrcDimensionsCloned.clear();
    }

    std::map<OdDbObjectId, OdDbObjectId>::iterator it = pSrcToDestIdMap->begin();
    for (; it != pSrcToDestIdMap->end(); ++it)
    {
      OdDbObjectId srcBlockId = it->first;
      OdDbObjectId srcNetworkId = OdDbAssocNetwork::getInstanceFromObject(srcBlockId, false);
      if (srcNetworkId.isNull())
        continue;
      collectSrcActionsAndDependencies(srcNetworkId, idMap);
    }

    it = pSrcToDestIdMap->begin();

    for (; it != pSrcToDestIdMap->end(); ++it)
    {
      OdDbObjectId srcBlockId = it->first;
      OdDbObjectId srcNetworkId = OdDbAssocNetwork::getInstanceFromObject(srcBlockId, false);
      OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::cast(srcNetworkId.openObject());
      if (pNetwork.isNull())
        continue;
      pNetwork->postProcessAfterDeepClone(idMap);
    }

    deleteTempDict(idMap.destDb(), true);

    it = pSrcToDestIdMap->begin();
    for (; it != pSrcToDestIdMap->end(); ++it)
    {
      removeEmptyGroupsNetworkAndDictionaries(it->second);
    }

    clearErasedActionsAndDependenciesFromMap(idMap);

    if (pData)
      pData->m_dcMapping.clear();

    // For WBLOCK call evaluate else network of destination database will not be evaluated
    OdDbDatabasePtr pSrcDb(idMap.origDb());
    OdDbDatabasePtr pDestDb(idMap.destDb());
    if (pSrcDb != pDestDb)
    {
      OdDbObjectId globalNetworkId = OdDbAssocNetwork::getInstanceFromDatabase(pDestDb, false);
      OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::cast(globalNetworkId.openObject(OdDb::kForWrite));
      if (!pNetwork.isNull())
      {
        pNetwork->evaluate(NULL);
        pNetwork->setStatus(kIsUpToDateAssocStatus, false, true);
      }
    }
  }
  catch (...)
  {
    makeEmergencyClear(idMap);
  }
  OdDbAssocNetworkDeepCloneAux::cleanData(idMap);
}

void OdDbAssocNetworkDeepCloneReactor::collectSrcActionsAndDependencies(OdDbObjectId srcActionId, OdDbIdMapping& idMap)
{
  OdDbAssocNetworkDeepCloneReactorDataPtr pData = getData(idMap);
  ODA_ASSERT(pData.get());
  if (pData)
    pData->m_aAllTheSrcAssocIds.append(srcActionId);

  OdDbObjectPtr pSrcObject = srcActionId.openObject();
  if (pSrcObject.get())
    if (pSrcObject->isAProxy())
      return;
  pSrcObject = 0;

  OdDbAssocActionPtr pSrcAction = OdDbAssocAction::cast(srcActionId.openObject());
  ODA_ASSERT(!pSrcAction.isNull());
  if (pSrcAction.isNull())
    return;

  OdDbObjectIdArray aDepIdToCheck;
  pSrcAction->getDependencies(true, true, aDepIdToCheck);
  if (pData)
    pData->m_aAllTheSrcAssocIds.append(aDepIdToCheck);
  unsigned int i, length = aDepIdToCheck.length();
  for (i=0; i<length; i++)
  {
    OdDbAssocDependencyPtr pSrcDep = OdDbAssocDependency::cast(aDepIdToCheck[i].openObject());
    if (pSrcDep.isNull())
      continue;
    OdDbObjectId srcDepBodyId = pSrcDep->dependencyBody();
    if (pData && !srcDepBodyId.isNull())
      pData->m_aAllTheSrcAssocIds.append(srcDepBodyId);
  }

  OdDbAssocNetworkPtr pSrcNetwork = OdDbAssocNetwork::cast(pSrcAction);
  if (!pSrcNetwork.isNull())
  {
    const OdDbObjectIdArray & aActionIds = pSrcNetwork->getActions();
    unsigned int actionLength = aActionIds.length();
    for (unsigned int i=0; i<actionLength; i++)
    {
      collectSrcActionsAndDependencies(aActionIds[i], idMap);
    }
  }
}

void OdDbAssocNetworkDeepCloneReactor::clearErasedActionsAndDependenciesFromMap(OdDbIdMapping& idMap)
{
  OdDbAssocNetworkDeepCloneReactorDataPtr pData = getData(idMap);
  ODA_ASSERT(pData.get());
  unsigned int length = pData ? pData->m_aAllTheSrcAssocIds.length() : 0;
  for (unsigned int i = 0; i<length; i++)
  {
    OdDbObjectId srcObjectId = pData->m_aAllTheSrcAssocIds[i];
    OdDbIdPair objectIdSearch(srcObjectId);
    if (idMap.compute(objectIdSearch))
    {
      if (objectIdSearch.key().isErased() || objectIdSearch.value().isErased())
      {
        idMap.del(srcObjectId);
      }
    }
  }
}

void OdDbAssocNetworkDeepCloneReactor::deleteTempDict(OdDbDatabasePtr pDb, const bool bDeleteActions)
{
  OdString tempAllActionDictName = OdDbAssocNetworkDeepCloneAux::getTempAssocDictionaryName();

  OdDbObjectId BTId = pDb->getBlockTableId();
  OdDbBlockTablePtr pBT = OdDbBlockTable::cast(BTId.openObject());
  ODA_ASSERT(!pBT.isNull());
  if (pBT.isNull())
    return;
  if (pBT->has(tempAllActionDictName))
  {
    OdDbBlockTableRecordPtr pBTR = pBT->getAt(tempAllActionDictName, OdDb::kForWrite);
    pBT = 0;
    pBTR->erase();
    pBTR = 0;
  }
  pBT = 0;

  OdDbObjectId globalDictId = pDb->getNamedObjectsDictionaryId();  
  OdDbDictionaryPtr pGlobalDict = OdDbDictionary::cast(globalDictId.openObject(OdDb::kForWrite));
  ODA_ASSERT(!pGlobalDict.isNull());
  if (pGlobalDict.isNull())
    return;
  OdDbObjectId tempAllActionDictId = pGlobalDict->getAt(tempAllActionDictName);
  OdDbDictionaryPtr pTempAllActionDict = OdDbDictionary::cast(tempAllActionDictId.openObject(OdDb::kForWrite));
  if (!pTempAllActionDict.isNull())
  {
    if (bDeleteActions)
    {
      OdDbDictionaryIteratorPtr it = pTempAllActionDict->newIterator();
      for (; !it->done(); it->next())
      {
        OdDbObjectPtr pObj = it->getObject(OdDb::kForWrite);
        pObj->erase();
      }
    }
    pGlobalDict->remove(tempAllActionDictName);
    pTempAllActionDict->erase();
  }
}

bool OdDbAssocNetworkDeepCloneReactor::isInLongTransaction(OdDbIdMapping & idMap)
{
  OdApLongTransactionManager * pTM = odapLongTransactionManager();
  if (pTM)
  {
    OdDbObjectId origTransId = pTM->currentLongTransactionFor(idMap.origDb());
    OdDbObjectId destTransId = pTM->currentLongTransactionFor(idMap.destDb());
    if (!origTransId.isNull() || !destTransId.isNull())
      return true;
  }
  return false;
}

static OdMutex s_dataMutex;
struct DataMutexProxy
{
  DataMutexProxy()
  {
    s_dataMutex.lock();
  }
  ~DataMutexProxy()
  {
    s_dataMutex.unlock();
  }
};

OdDbAssocNetworkDeepCloneReactorDataPtr OdDbAssocNetworkDeepCloneReactor::getData(OdDbIdMapping & idMap)
{
  DataMutexProxy mutexProxy;
  void * key = &idMap;
  IdMappingToInternalDataMap::iterator it = m_idMapToData.find(key);
  if (it == m_idMapToData.end())
  {
    OdDbAssocNetworkDeepCloneReactorDataPtr pNewData(new OdDbAssocNetworkDeepCloneReactorData());
    m_idMapToData.insert(std::make_pair(key, pNewData));
    return pNewData;
  }
  return it->second;
}

void OdDbAssocNetworkDeepCloneReactor::deleteData(OdDbIdMapping & idMap)
{
  DataMutexProxy mutexProxy;

  void * key = &idMap;
  IdMappingToInternalDataMap::iterator it = m_idMapToData.find(key);
  ODA_ASSERT(it != m_idMapToData.end());
  if (it != m_idMapToData.end())
  {
    m_idMapToData.erase(it);
  }
}

OdDbAssocNetworkDeepCloneReactorData::OdDbAssocNetworkDeepCloneMapping::OdDbAssocNetworkDeepCloneMapping()
  : m_failure(false)
{
}

void OdDbAssocNetworkDeepCloneReactorData::OdDbAssocNetworkDeepCloneMapping::clear(bool setFailure)
{
  m_failure = setFailure;
  m_deepCloneMapping.erase(m_deepCloneMapping.begin(), m_deepCloneMapping.end());
}

void OdDbAssocNetworkDeepCloneReactorData::OdDbAssocNetworkDeepCloneMapping::init(OdDbIdMapping& idMap, bool iInitOnlySrc)
{
  m_failure = false;
  if (iInitOnlySrc)
  {
    clear();

    // First of all check the need of cloning
    if (OdDbAssocNetwork::getInstanceFromDatabase(idMap.origDb(), false).isNull())
      return;

    OdDbIdMappingIterPtr pMapIter = idMap.newIterator();
    if (pMapIter.isNull())
      return;
   
    for (; !pMapIter->done(); pMapIter->next())
    {
      OdDbIdPair curPair;
      pMapIter->getMap(curPair);
      OdDbEntityPtr pSrcEntity = OdDbEntity::cast(curPair.key().openObject());
      if (pSrcEntity.isNull())
        continue;

      if ((pSrcEntity->isA() == OdDbBlockBegin::desc()) ||
          (pSrcEntity->isA() == OdDbBlockEnd::desc()) ||
          (pSrcEntity->isKindOf(OdDbDimension::desc())))
        continue;

      OdDbObjectId ownerId = pSrcEntity->ownerId();
      OdDbObjectPtr pOwner = ownerId.openObject();
      if (!pOwner.get() || (pOwner->isA() != OdDbBlockTableRecord::desc()))
        continue;
      pOwner = 0;

      // Check whether block has assoc network else we are not interested in it
      OdDbObjectId assocNetworkId = OdDbAssocNetwork::getInstanceFromObject(ownerId, false);
      if (assocNetworkId.isNull())
        continue;

      m_deepCloneMapping[ownerId] = OdDbObjectId();
    }
  }
  else
  {
    std::map<OdDbObjectId, OdDbObjectId>::iterator it = m_deepCloneMapping.begin();
    for (; it!=m_deepCloneMapping.end(); ++it)
    {
      // Take any entity
      OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::cast(it->first.openObject());
      ODA_ASSERT(pBlock.get());
      OdDbObjectIteratorPtr pBlockIter = pBlock->newIterator();
      pBlock = 0;

      bool clonedEntityWasFound = false;
      for (; !pBlockIter->done(); pBlockIter->step())
      {
        OdDbEntityPtr pSrcEnt = pBlockIter->entity();
        if (pSrcEnt->isKindOf(OdDbDimension::desc()))
          continue;
        OdDbIdPair srcEntSearch(pSrcEnt->id());
        pSrcEnt = 0;
        if (idMap.compute(srcEntSearch) && srcEntSearch.isCloned())
        {
          it->second = srcEntSearch.value().openObject()->ownerId();
          clonedEntityWasFound = true;
          break;
        }
      }

      // We must find some dest entity, since some entity of the src block was cloned, since it appeared in idmap
      ODA_ASSERT(clonedEntityWasFound);
    }
  }
}

#ifdef DISABLE_NEW_CLONING_CODE
void OdDbAssocNetworkDeepCloneReactor::addRefReactor()
{
}

void OdDbAssocNetworkDeepCloneReactor::releaseReactor()
{
}
#else
void OdDbAssocNetworkDeepCloneReactor::addRefReactor()
{
  if (m_Counter++ == 0)
  {
    odrxEvent()->addReactor(this);
  }
}

void OdDbAssocNetworkDeepCloneReactor::releaseReactor()
{
  if (--m_Counter == 0)
  {
    odrxEvent()->removeReactor(this);
  }
}
#endif
