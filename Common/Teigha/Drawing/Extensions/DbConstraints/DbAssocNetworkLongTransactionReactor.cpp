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

#include "DbAssocNetworkLongTransactionReactor.h"

#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocDependency.h"
#include "DbAssocGeomDependency.h"
#include "DbAssocNetwork.h"
#include "DbImpAssoc2dConstraintGroup.h"

#include "DbBlockTableRecord.h"
#include "DbDimension.h"
#include "DbIdMapping.h"
#include "DbLongTransaction.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"



ODRX_CONS_DEFINE_MEMBERS(OdDbAssocNetworkObjectModificationReactor, OdDbObjectReactor, RXIMPL_CONSTR);

OdDbAssocNetworkLongTransactionReactor::OdDbAssocNetworkLongTransactionReactor()
{
  m_Counter = 0;
}

OdDbAssocNetworkLongTransactionReactor::~OdDbAssocNetworkLongTransactionReactor()
{
  m_Counter = 0;
}

OdDbAssocNetworkObjectModificationReactor::~OdDbAssocNetworkObjectModificationReactor()
{
  unsigned int i, l = m_aWorksetObjReactorAttachedTo.length();
  for (i=0; i<l; i++)
  {
    OdDbObjectPtr pObj = m_aWorksetObjReactorAttachedTo[i].openObject(OdDb::kForWrite);
    if (!pObj.isNull())
      pObj->removeReactor(this);
  }
}

void OdDbAssocNetworkObjectModificationReactor::erased(const OdDbObject* pObject, bool erasing)
{
  if (!erasing || pObject->isUndoing())
    return;
  pObject->removeReactor(this);
  OdDbObjectId objId = pObject->objectId();
  m_aModifiedWorksetObj.append(objId);
  m_aWorksetObjReactorAttachedTo.remove(objId);
}

void OdDbAssocNetworkObjectModificationReactor::modified(const OdDbObject* pObject)
{
  if (pObject->isUndoing())
    return;
  m_aModifiedWorksetObj.append(pObject->objectId());
}

void OdDbAssocNetworkObjectModificationReactor::appendToWorksetObject(OdDbObjectId objId)
{
  OdDbObjectPtr pObj = objId.openObject(OdDb::kForWrite);
  ODA_ASSERT(!pObj.isNull());
  if (pObj.isNull())
    return;
  pObj->addReactor(this);
  m_aWorksetObjReactorAttachedTo.append(objId);
}

void OdDbAssocNetworkObjectModificationReactor::getModifiedWorksetObjects(OdDbObjectIdArray & aResArray)
{
  aResArray = m_aModifiedWorksetObj;
  std::sort(aResArray.begin(), aResArray.end());
  aResArray.erase(std::unique(aResArray.begin(), aResArray.end()), aResArray.end());
}

bool OdDbAssocNetworkObjectModificationReactor::isEmpty()
{
  return m_aWorksetObjReactorAttachedTo.empty();
}

void OdDbAssocNetworkLongTransactionReactor::beginCheckOut(OdDbLongTransaction& iTransaction, OdDbObjectIdArray& iOriginList)
{
  m_aOriginEntities = iOriginList;
}

void OdDbAssocNetworkLongTransactionReactor::showDimensions(OdDbObjectId blockId, bool show)
{
  OdDbBlockTableRecordPtr pBTR = OdDbBlockTableRecord::cast(blockId.openObject());
  ODA_ASSERT(!pBTR.isNull());
  if (pBTR.isNull())
    return;
  OdDbObjectIteratorPtr it = pBTR->newIterator();
  for (; !it->done(); it->step())
  {
    OdDbDimensionPtr pDim = OdDbDimension::cast(it->entity());
    if (pDim.isNull())
      continue;
    if (!pDim->isConstraintObject())
      continue;
    pDim->upgradeOpen();
    if (show)
      pDim->setVisibility(OdDb::kVisible);
    else
      pDim->setVisibility(OdDb::kInvisible);
    pDim->downgradeOpen();
  }
}

void OdDbAssocNetworkLongTransactionReactor::endCheckOut(OdDbLongTransaction& iTransaction)
{
  try
  {
    unsigned int origLength = m_aOriginEntities.length();
  if (!origLength)
    return;

  // One more cheap check - if src block has no assoc network, skip it
  OdDbObjectId originBlock = iTransaction.originBlock();
  OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromObject(originBlock, false);
  
  if (networkId.isNull())
    return;

  m_OriginalObjectReactorsMap.clear();
  m_WorksetObjectToOriginalMap.clear();
  m_aModifiedWorksetEntities.clear();

  showDimensions(originBlock, false);

  OdDbIdMapping * pMapping = iTransaction.activeIdMap();
  ODA_ASSERT(pMapping);

  m_pModificationReactor = 0;

  m_pModificationReactor = OdDbAssocNetworkObjectModificationReactor::createObject();
  for (unsigned int i=0; i<origLength; i++)
  {
    OdDbObjectId originId = m_aOriginEntities[i];
    OdDbIdPair origSearch(originId);
    if (!pMapping->compute(origSearch))
      continue;

    OdDbObjectPtr pOrigObj = originId.openObject();
    ODA_ASSERT(!pOrigObj.isNull());
    if (pOrigObj.isNull())
      continue;
    OdDbObjectIdArray aOrigPersReactors = pOrigObj->getPersistentReactors();
    unsigned int j, origPersLength = aOrigPersReactors.length();
    for (j=0; j<origPersLength; j++)
    {
      OdDbObjectId persReactorId = aOrigPersReactors[j];
      OdDbObjectPtr pReactor = persReactorId.openObject();
      if (pReactor.get() && pReactor->isKindOf(OdDbAssocDependency::desc()))
      {
        m_OriginalObjectReactorsMap[originId].append(persReactorId);
        pOrigObj->upgradeOpen();
        pOrigObj->removePersistentReactor(persReactorId);
        pOrigObj->downgradeOpen();
      }
    }
    pOrigObj = 0;

    OdDbObjectId worksetObjectId = origSearch.value();
    m_WorksetObjectToOriginalMap[worksetObjectId] = originId;
    OdDbObjectPtr pWorkSetObject = worksetObjectId.openObject();
    ODA_ASSERT(!pWorkSetObject.isNull());
    if (pWorkSetObject.isNull())
      continue;

    // Append only to objects which have dependency
    OdDbObjectIdArray aPersReactors = pWorkSetObject->getPersistentReactors();
    unsigned int persLength = aPersReactors.length();
    for (j=0; j<persLength; j++)
    {
      OdDbObjectId persReactorId = aPersReactors[j];
      OdDbObjectPtr pReactor = persReactorId.openObject();
      if (pReactor.get())
      {
        if (pReactor->isKindOf(OdDbAssocDependency::desc()))
        {
          pWorkSetObject->upgradeOpen();
          pWorkSetObject->removePersistentReactor(persReactorId);
          pWorkSetObject->downgradeOpen();
        }
      }
    }
    OdDbAssocOriginalObjectReactorsMap::iterator findIt = m_OriginalObjectReactorsMap.find(originId);
    if ((findIt != m_OriginalObjectReactorsMap.end()) && !findIt->second.isEmpty())
      m_pModificationReactor->appendToWorksetObject(origSearch.value());
  }
  if (m_pModificationReactor->isEmpty())
  {
    m_pModificationReactor = 0;
  }
  }
  catch (OdError &)
  {
    m_OriginalObjectReactorsMap.clear();
    m_WorksetObjectToOriginalMap.clear();
    m_aModifiedWorksetEntities.clear();
  }
}

void OdDbAssocNetworkLongTransactionReactor::beginCheckIn(OdDbLongTransaction& iTransaction)
{
  // NULL if no reactor created. This may happen if origin block has no assoc network or entities with dependencies
  if (!m_pModificationReactor.get())
    return;
  try
  {
    showDimensions(iTransaction.originBlock(), true);

    m_pModificationReactor->getModifiedWorksetObjects(m_aModifiedWorksetEntities);

    m_pModificationReactor = 0;

    unsigned int i, l = m_aModifiedWorksetEntities.length();
    for (i = 0; i < l; i++)
    {
      OdDbObjectId workSetObjectId = m_aModifiedWorksetEntities[i];
      if (!workSetObjectId.isErased())
      {
        continue;
      }
      OdDbObjectId originalObjectId = m_WorksetObjectToOriginalMap[workSetObjectId];
      OdDbObjectPtr pObj = originalObjectId.openObject(OdDb::kForWrite);
      ODA_ASSERT(!pObj.isNull());
      if (pObj.isNull())
        continue;
      const OdDbObjectIdArray & aPersReactorId = m_OriginalObjectReactorsMap[originalObjectId];
      for (unsigned int j = 0; j < aPersReactorId.length(); j++)
        pObj->addPersistentReactor(aPersReactorId[j]);
    }
  }
  catch (OdError &)
  {
    m_OriginalObjectReactorsMap.clear();
    m_WorksetObjectToOriginalMap.clear();
    m_aModifiedWorksetEntities.clear();
  }
}

void OdDbAssocNetworkLongTransactionReactor::endCheckIn(OdDbLongTransaction& iTransaction)
{
  if (m_OriginalObjectReactorsMap.empty())
    return;

  try
  {
    OdDbAssocOriginalObjectReactorsMap::iterator it = m_OriginalObjectReactorsMap.begin();
    for (; it != m_OriginalObjectReactorsMap.end(); ++it)
    {
      // Dependency can be erased in such situation:
      // Assume entities A and B have dep DA and DB and common constraint, no other constraints
      //   WorksetEntity of A is erased during refedit
      //   Then its dependency is already attached in beginCheckIn
      //   Then DA and corresponding constraints will be erased
      //   Then DB will be also erased because group has no constraints
      for (unsigned int i = 0; i < it->second.length(); i++)
      {
        OdDbObjectId persReactorId = it->second[i];
        if (persReactorId.isErased())
          continue;

        OdDbObjectId originObjectId = it->first;
        // Means we removed object from workset
        if (originObjectId.isErased())
          continue;

        OdDbObjectPtr pObj = originObjectId.openObject(OdDb::kForWrite);
        ODA_ASSERT(!pObj.isNull());
        if (pObj.isNull())
          continue;
        pObj->addPersistentReactor(persReactorId);
      }
    }

    unsigned int i, length = m_aModifiedWorksetEntities.length();
    for (i = 0; i < length; i++)
    {
      OdDbObjectId worksetObjId = m_aModifiedWorksetEntities[i];
      OdDbObjectId originalObjectId = m_WorksetObjectToOriginalMap[worksetObjId];
      const OdDbObjectIdArray & aPersReactors = m_OriginalObjectReactorsMap[originalObjectId];
      for (unsigned int j = 0; j < aPersReactors.length(); j++)
      {
        OdDbObjectId reactorDestObjectId = aPersReactors[j];
        OdDbAssocGeomDependencyPtr pDep = OdDbAssocGeomDependency::cast(reactorDestObjectId.openObject());
        if (pDep.isNull())
          continue;

        OdDbAssocStatus depStatus = pDep->status();
        if (kChangedDirectlyAssocStatus != depStatus)
          continue;

        OdDbAssoc2dConstraintGroupPtr pOwningGroup = OdDbAssoc2dConstraintGroup::cast(pDep->owningAction().openObject(OdDb::kForWrite));
        if (pOwningGroup.get())
          pOwningGroup->deleteConstrainedGeometry(reactorDestObjectId);
        pOwningGroup = 0;
      }
    }
    m_OriginalObjectReactorsMap.clear();

    OdDbObjectId globalNetworkId = OdDbAssocNetwork::getInstanceFromDatabase(iTransaction.originBlock().database(), false);
    OdDbAssocNetworkPtr pNetwork = globalNetworkId.openObject(OdDb::kForWrite);
    if (!pNetwork.isNull())
    {
      pNetwork->evaluate(NULL);
      pNetwork->setStatus(kIsUpToDateAssocStatus, false, true);
    }
  }
  catch (OdError &)
  {
    m_OriginalObjectReactorsMap.clear();
    m_WorksetObjectToOriginalMap.clear();
    m_aModifiedWorksetEntities.clear();
  }
}

void OdDbAssocNetworkLongTransactionReactor::abortLongTransaction(OdDbLongTransaction& iTransaction)
{
  if (!m_pModificationReactor.get())
    return;
  try
  {
    m_pModificationReactor = 0;

    showDimensions(iTransaction.originBlock(), true);

    // Restore reactors for block entities
    OdDbAssocOriginalObjectReactorsMap::iterator it = m_OriginalObjectReactorsMap.begin();
    for (; it != m_OriginalObjectReactorsMap.end(); ++it)
    {
      OdDbObjectId originObjectId = it->first;
      OdDbObjectPtr pObj = originObjectId.openObject(OdDb::kForWrite);
      ODA_ASSERT(!pObj.isNull());
      if (pObj.isNull())
        continue;
      const OdDbObjectIdArray & aPersReactors = it->second;
      for (unsigned int j = 0; j < aPersReactors.length(); j++)
      {
        pObj->addPersistentReactor(aPersReactors[j]);
      }
    }
    m_OriginalObjectReactorsMap.clear();
  }
  catch (OdError &)
  {
    m_OriginalObjectReactorsMap.clear();
    m_WorksetObjectToOriginalMap.clear();
    m_aModifiedWorksetEntities.clear();
  }
}

#ifdef DISABLE_NEW_CLONING_CODE
void OdDbAssocNetworkLongTransactionReactor::addRefReactor()
{
}

void OdDbAssocNetworkLongTransactionReactor::releaseReactor()
{
}
#else
void OdDbAssocNetworkLongTransactionReactor::addRefReactor()
{
  if (m_Counter++ == 0)
  {
    OdApLongTransactionManager * pLongTransactionManager = odapLongTransactionManager();
    if (pLongTransactionManager)
    {
      pLongTransactionManager->addReactor(this);
    }
  }
}

void OdDbAssocNetworkLongTransactionReactor::releaseReactor()
{
  if (--m_Counter == 0)
  {
    OdApLongTransactionManager * pLongTransactionManager = odapLongTransactionManager();
    if (pLongTransactionManager)
    {
      pLongTransactionManager->removeReactor(this);
    }
  }
}
#endif
