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
#include "DbAssocAction.h"
#include "DbAssocActionBody.h"
#include "DbAssocDependency.h"
#include "DbDictionary.h"
#include "DbAssocNetworkDeepCloneAux.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbImpAssocAction.h"
#include "DbAssocActionParam.h"
#include "DbEvalVariant.h"
#include "DbProxyObject.h"

#include "DbFiler.h"
#include "algorithm"
#include "DbAudit.h"
#include "DbHostAppServices.h"
#include <typeinfo>
class findValueParamByName
{
  const OdString& m_Name;
public:
  inline findValueParamByName(const OdString& Name)
    : m_Name(Name)
  {
  }

  inline bool operator()(const OdDbImpValueParamPtr &ValueParam) const
  {
    return ValueParam->name() == m_Name;
  }
};

OdDbImpAssocAction::OdDbImpAssocAction()
  : m_status(kIsUpToDateAssocStatus)
  , m_evalPriority(0)
  , m_Unk_UInt32_1(0)
  , m_actionIndex(0)
  , m_maxAssocDepIdx(0)
  , m_isBase(true)
  , m_EvaluationInProgress(false)
{}

OdDbImpAssocAction::~OdDbImpAssocAction()
{
}

OdDbImpAssocAction* OdDbImpAssocAction::getImpObject(OdDbAssocAction *pAction)
{
  return pAction->m_pImpObj;
}

OdDbObjectId OdDbImpAssocAction::actionBody() const
{
  return m_idActionBody;
}
    
OdResult OdDbImpAssocAction::setActionBody(OdDbObjectId& actionId, const OdDbObjectId& actionBodyId)
{
  m_idActionBody = actionBodyId;
  
  if ( !m_idActionBody.isNull() )
  {
    OdDbObjectPtr pObj = m_idActionBody.openObject(OdDb::kForWrite);
    pObj->setOwnerId(actionId);
  }
  return eOk;
}

OdDbAssocStatus OdDbImpAssocAction::status() const
{
  return m_status;
}


OdResult OdDbImpAssocAction::setStatus(OdDbAssocStatus newStatus,
                                    bool notifyOwningNetwork,
                                    bool setInOwnedActions)
{

  if (newStatus == m_status && !notifyOwningNetwork && newStatus != kChangedDirectlyAssocStatus)
    return eOk;

  if ( isEvaluationRequest(newStatus) )
  {
    if ( !isToBeSkipped(m_status) )
    {
      if ( isEvaluationRequest(m_status) )
      {
        if ( evaluationRequestSeverityLevel(newStatus) > evaluationRequestSeverityLevel(m_status) )
          m_status = newStatus;
      }
      else
      {
        m_status = newStatus;     
        notifyOwningNetwork = true;
      }
    }
  }
  else
  {
    m_status = newStatus;
  }

  if ( notifyOwningNetwork )
  {
    OdDbObjectId objId = owningNetwork();
    OdDbAssocActionPtr pAction = objId.openObject(OdDb::kForWrite);
    if ( !pAction.isNull() )
      pAction->setStatus(newStatus, notifyOwningNetwork, setInOwnedActions);
  }

  return eOk;
}


OdDbObjectId OdDbImpAssocAction::owningNetwork() const
{
  return m_idOwningNetwork;
}

OdResult OdDbImpAssocAction::setOwningNetwork(OdDbAssocAction *pAction, const OdDbObjectId& networkId, bool alsoSetAsDatabaseOwner)
{
  m_idOwningNetwork = networkId;
  if (alsoSetAsDatabaseOwner)
  {
    pAction->setOwnerId(networkId);
  }
  return eOk;
}
  
OdResult OdDbImpAssocAction::getDependencies(bool readDependenciesWanted,
                                             bool writeDependenciesWanted,
                                             OdDbObjectIdArray& dependencyIds) const
{
  ODA_ASSERT(readDependenciesWanted || writeDependenciesWanted);
  OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if (!pBody.isNull())
  {
    //return pBody->getDependenciesOverride(readDependenciesWanted, writeDependenciesWanted, dependencyIds);
  }

  OdDbAssocDependencyPtr pDep;
  for ( unsigned int i = 0; i < m_arrDependencies.length(); i++ )
  {
    pDep = m_arrDependencies[i].openObject();
    if ( pDep.isNull() )
      continue;

    if ( readDependenciesWanted && pDep->isReadDependency() )
    {
      dependencyIds.push_back( m_arrDependencies[i] );
      continue;
    }

    if ( writeDependenciesWanted && pDep->isWriteDependency() )
    {
      dependencyIds.push_back( m_arrDependencies[i] );
    }
  }

  return eOk;
}
OdResult OdDbImpAssocAction::addDependency(const OdDbObjectId& dependencyId, 
                       bool setThisActionAsOwningAction, OdDbAssocAction *pAction)
{
  int idx = findDependenciesArrayIndex(dependencyId);

  if ( idx >= 0 )
    return eOk;

  OdDbAssocDependencyPtr pDep = dependencyId.openObject(setThisActionAsOwningAction ?
                                                          OdDb::kForWrite : OdDb::kForRead);
  if ( pDep.isNull() )
    return eBadObjType;

  m_arrDependencies.append(dependencyId);
  if ( setThisActionAsOwningAction )
  {
    pDep->setOwningAction(pAction->objectId());
    pDep->setIsDelegatingToOwningAction(true);
  }
  
  return eOk;
}

OdResult OdDbImpAssocAction::removeDependency(const OdDbObjectId& dependencyId, 
                          bool alsoEraseIt)
{
  int idx = findDependenciesArrayIndex(dependencyId);
  if ( idx == -1 )
    return eBadObjType;

  OdDbAssocDependencyPtr pDep = dependencyId.openObject(OdDb::kForWrite);
  /*
  if ( pDep->isAttachedToObject() )
    return eBadObjType;
  */
  pDep->setStatus(kErasedAssocStatus, false);
  pDep->detachFromObject();
  m_arrDependencies.removeAt(idx);
  pDep->setOwningAction(OdDbObjectId::kNull);
  if ( alsoEraseIt )
    pDep->erase(true);

  return eOk;
}

OdResult OdDbImpAssocAction::removeAllDependencies(bool alsoEraseThem)
{
  OdDbAssocDependencyPtr pDep;
  for ( unsigned int i = 0; i < m_arrDependencies.length(); i++ )
  {
    pDep = m_arrDependencies[i].openObject(OdDb::kForWrite);
    pDep->detachFromObject();
    pDep->setOwningAction(OdDbObjectId::kNull);
    if ( alsoEraseThem )
      pDep->erase(true);
  }

  m_arrDependencies.clear();
  return eOk;
}

OdResult OdDbImpAssocAction::getDependentObjects(bool readDependenciesWanted,
                                                 bool writeDependenciesWanted,
                                                 OdDbObjectIdArray& objectIds) const
{
  ODA_ASSERT(readDependenciesWanted || writeDependenciesWanted);
  OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if (!pBody.isNull())
  {
   // return pBody->getDependentObjectsOverride(readDependenciesWanted, writeDependenciesWanted, objectIds);
  }

  for (unsigned int i = 0, sz = m_arrDependencies.length(); i < sz; i++)
  {
    OdDbObjectId objId = m_arrDependencies[i];
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(objId.openObject());
    if ( pDep.isNull() )
      continue;

    if ( readDependenciesWanted && pDep->isReadDependency() )
    {
      objId = pDep->dependentOnObject();
    }
    else if ( writeDependenciesWanted && pDep->isWriteDependency() )
    {
      objId = pDep->dependentOnObject();
    }
    else
      continue;

    bool isExist = false;
    for (unsigned int j = 0, szJ = objectIds.length(); j < szJ; j++)
    {
      if ( objectIds[j] == objId )
      {
        isExist = true;
        break;
      }
    }

    if ( isExist )
      continue;
    else
      objectIds.push_back(objId);
  }
  
  return eOk;
}
/*

    bool OdDbImpAssocAction::is(const OdDbAssocDependency* pDependency) const;

    bool OdDbImpAssocAction::isExternalDependency(const OdDbAssocDependency* pDependency) const;
*/

bool OdDbImpAssocAction::isRelevantDependencyChange(const OdDbAssocDependency* pDependency) const
{
  OdDbObjectPtr pObj = m_idActionBody.openObject();
  if (!pObj.isNull() && pObj->isAProxy())
    return false;
  OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast(pObj);
  if (!pBody.isNull())
  {
    bool bRet;
    if (pBody->isRelevantDependencyChangeOverride(pDependency, bRet) == eOk)
      return bRet;
  }

  return true;
}
/*
    bool OdDbImpAssocAction::hasDependencyCachedValue(const OdDbAssocDependency* pDependency) const;

    bool OdDbImpAssocAction::areDependenciesOnTheSameThing(const OdDbAssocDependency* pDependency1, 
                                       const OdDbAssocDependency* pDependency2) const;

    bool OdDbImpAssocAction::areDependenciesEqual(const OdDbAssocDependency* pDependency1, 
                              const OdDbAssocDependency* pDependency2) const;

    OdResult OdDbImpAssocAction::getActionsDependentOnObject(const OdDbObject* pObject,
                                                bool readDependenciesWanted,
                                                bool writeDependenciesWanted,                                   
                                                OdDbObjectIdArray& actionIds);
*/
void OdDbImpAssocAction::dependentObjectCloned(const OdDbAssocDependency* pDependency, 
                           const OdDbObject*          pDbObj, 
                           const OdDbObject*          pNewObj)
{
  OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if (!pBody.isNull())
  {
    pBody->dependentObjectClonedOverride(pDependency, pDbObj, pNewObj);
  }
}


OdResult OdDbImpAssocAction::addMoreObjectsToDeepClone( OdDbIdMapping& idMap, 
                                                        OdDbObjectIdArray& additionalObjectsToClone ) const
{
  OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast( m_idActionBody.openObject() );
  if ( !pBody.isNull() )
  {
    return pBody->addMoreObjectsToDeepCloneOverride( idMap, additionalObjectsToClone );
  }

  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if (!pBody.isNull())
  {
    return pBody->postProcessAfterDeepCloneOverride(idMap);
  }

  // Map dependencies of destination action
  OdDbIdPair searchForDest(pAction->objectId());
  if (!idMap.compute(searchForDest) || !searchForDest.isCloned() || searchForDest.value().isErased())
  {
    return eOk;
  }

  // This action is kept in temporary dictionary
  OdDbAssocActionPtr pTempAction = OdDbAssocAction::cast(searchForDest.value().openObject(OdDb::kForWrite));

  std::map<OdDbObjectId, OdDbObjectId> * pSrcToDestIdMap = OdDbAssocNetworkDeepCloneAux::srcToDestIdMap(idMap);

  OdDbObjectId srcBlockId = pAction->objectThatOwnsNetworkInstance();
  OdDbObjectId destBlockId = pSrcToDestIdMap ? (*pSrcToDestIdMap)[srcBlockId] : OdDbObjectId();
  ODA_ASSERT(!destBlockId.isNull());

  OdDbAssocNetworkDeepCloneAux::moveCompleteActionToDestNetwork(pTempAction, destBlockId, idMap.deepCloneContext());

  return eOk;
}

OdResult OdDbImpAssocAction::postProcessAfterDeepCloneCancel(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if (!pBody.isNull())
  {
    return pBody->postProcessAfterDeepCloneCancelOverride(idMap);
  }
  return eOk;
}

/*
    OdResult OdDbImpAssocAction::addMoreObjectsToDeepClone(OdDbIdMapping& idMap, 
                                       OdDbObjectIdArray& additionalObjectsToClone) const;

    OdResult OdDbImpAssocAction::postProcessAfterDeepClone(OdDbIdMapping& idMap); 

    OdResult OdDbImpAssocAction::postProcessAfterDeepCloneCancel(OdDbIdMapping& idMap); 
*/
bool OdDbImpAssocAction::isActionEvaluationInProgress() const
{
  return m_EvaluationInProgress;
}

OdDbAssocEvaluationCallback* OdDbImpAssocAction::currentEvaluationCallback() const
{
  ODA_FAIL();
  //throw OdError(eNotImplemented);
  return NULL;
}

OdResult OdDbImpAssocAction::evaluateDependencies()
{
  ODA_FAIL();
  //throw OdError(eNotImplemented);
  return eNotImplemented;
}

void OdDbImpAssocAction::evaluateDependency(OdDbAssocDependency* pDependency)
{
  pDependency->setStatus(kIsUpToDateAssocStatus, false);
}

OdResult OdDbImpAssocAction::ownedDependencyStatusChanged(OdDbAssocDependency* pOwnedDependency, 
                                      OdDbAssocStatus      previousStatus)
{
  ODA_FAIL();
  //throw OdError(eNotImplemented);
  return eNotImplemented;
}
/*
    OdResult OdDbImpAssocAction::transformActionBy(const OdGeMatrix3d& transform);

    bool OdDbImpAssocAction::isEqualTo(const OdDbImpAssocAction* pOtherAction) const;

    OdDbAssocEvaluationPriority OdDbImpAssocAction::evaluationPriority() const;
*/
void OdDbImpAssocAction::getDependentActionsToEvaluate(OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const
{
}

void OdDbImpAssocAction::evaluate(OdDbAssocAction *pAction, OdDbAssocEvaluationCallback* pEvaluationCallback)
{
  m_EvaluationInProgress = true;
  //ODA_ASSERT(isEvaluationRequest(status()));

  OdDbObjectId bodyId = actionBody();
  if ( !bodyId.isNull() )
  {
    OdDbObjectPtr pObj = bodyId.openObject(OdDb::kForWrite);
    OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast(pObj);
    if (!pBody.isNull())  // It may be proxy
    {
      pBody->evaluateOverride();
    }
    m_EvaluationInProgress = false;
    return;
  }
  
//   OdSmartPtr<AssocActionPE> pPE = pAction->isA()->getX(AssocActionPE::desc());
//   if ( !pPE.isNull() )
//   {
//     pPE->evaluate(pAction);
//   }
// 
//   doPostEvaluateJob();
  m_EvaluationInProgress = false;
}

OdDbObjectId OdDbImpAssocAction::objectThatOwnsNetworkInstance(const OdDbObject *pObj) const
{
  OdDbObjectId ownerId;
  OdDbObjectPtr pChildObj = pObj;
  OdDbObjectPtr pOwnedObj;

  bool cont = true;
  bool isDictionary = false;
  while ( cont )
  {
    ownerId = pChildObj->ownerId();
    if ( ownerId.isNull() )
      return OdDbObjectId::kNull;
    OdDbObjectPtr pOwnedObj = ownerId.openObject();
    if ( isDictionary )
    {
      if ( !pOwnedObj->isKindOf(OdDbDictionary::desc()) )
        return ownerId;

    }
    else if ( pOwnedObj->isKindOf(OdDbDictionary::desc()) )
      isDictionary = true;

    pChildObj = pOwnedObj;
  }

  return OdDbObjectId::kNull;
}

//void OdDbImpAssocAction::dragStatus(const OdDb::DragStat status);

OdResult OdDbImpAssocAction::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  int ver = pFiler->rdInt16();
  if (!ver || ver > 2)
  {
    ODA_FAIL(); // TODO
    return eMakeMeProxy;
  }

  m_arrDependencies.clear();
  m_arrParams.clear();
  m_arrValueParams.clear();

  m_Unk_UInt32_1 = pFiler->rdInt32();
  ODA_ASSERT_ONCE(m_Unk_UInt32_1 == 0 || m_Unk_UInt32_1 == 1);
  //m_Unk_UInt32_1 = 0; // value of 1 is ignored while reading (cf. file attached to #6257) TODO

  m_idOwningNetwork = pFiler->rdSoftPointerId();
  m_idActionBody = pFiler->rdHardOwnershipId();

  m_actionIndex = pFiler->rdInt32();

  m_maxAssocDepIdx = pFiler->rdInt32();

  OdInt32 n = pFiler->rdInt32();
  if (n > 0) {
    while (n-- > 0) {
      bool isHard = pFiler->rdBool();
      if (isHard)
        m_arrDependencies.push_back(pFiler->rdHardOwnershipId());
      else
      {
        return eMakeMeProxy;
      }
    }
  }

  if (ver > 1) 
  {
    // AC27
    if (pFiler->rdInt16())
    { // TODO AC2013
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }

    // Param Ids
    n = pFiler->rdInt32();
    while (n-- > 0) {
      m_arrParams.push_back(pFiler->rdHardOwnershipId());
    }
    
    if (pFiler->rdInt16())
    { // TODO AC2013
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }

    n = pFiler->rdInt32();
    while (n-- > 0) 
    {
      OdDbImpValueParamPtr pValueParam = new OdDbImpValueParam(); 
      OdResult res = pValueParam->dwgInFields(pFiler);
      if (res != eOk)
        return res;
      m_arrValueParams.push_back(pValueParam);
    }
  }
  return eOk;
}

void OdDbImpAssocAction::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  int ver = (pFiler->dwgVersion() >= OdDb::vAC27) ? 2 : 1;
  pFiler->wrInt16(ver);

  pFiler->wrInt32(m_Unk_UInt32_1);

  pFiler->wrSoftPointerId(m_idOwningNetwork);
  pFiler->wrHardOwnershipId(m_idActionBody);

  pFiler->wrInt32(m_actionIndex);
  pFiler->wrInt32(m_maxAssocDepIdx);
  OdInt32 n = (m_isBase) ? m_arrDependencies.size() : 0;
  pFiler->wrInt32(n);
  OdInt32 index = 0;
  for (; index < n; index++) 
  {
    bool isHard = true;
    pFiler->wrBool(isHard);
    if (isHard)
      pFiler->wrHardOwnershipId(m_arrDependencies[index]);
    //else
    //  pFiler->wrSoftOwnershipId(m_arrDependencies[index]);
  }

  if (ver > 1) 
  {
    // AC27
    pFiler->wrInt16(0);
  
    // Param Ids
    n = m_arrParams.size();
    pFiler->wrInt32(n); // Param Ids
    for (index = 0; index < n; index++)
      pFiler->wrHardOwnershipId(m_arrParams[index]);

    pFiler->wrInt16(0);

    n = m_arrValueParams.size();
    pFiler->wrInt32(n);
    for (index = 0; index < n; index++) 
    {
      OdDbImpValueParamPtr pValueParam = m_arrValueParams[index]; 
      pValueParam->dwgOutFields(pFiler);
    }
  }
}

#define NEXT_CODE(code)         \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL_ONCE();              \
  return eMakeMeProxy;          \
}

OdResult OdDbImpAssocAction::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  // Check that we are at the correct subclass data
  if (!pFiler->atSubclassData(OdDbAssocAction::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_arrDependencies.clear();
  m_arrParams.clear();
  m_arrValueParams.clear();

  // Read version number (must be first)
  NEXT_CODE(90)
  OdUInt32 nVersion = pFiler->rdUInt32();
  if (nVersion > 2)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_Unk_UInt32_1 = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(   m_Unk_UInt32_1 == 0 || m_Unk_UInt32_1 == 1 
                  || m_Unk_UInt32_1 == 5); // attachment of #11465 -> oda : sel + drag -> audited in ACAD + save as dxf
  //m_Unk_UInt32_1 = 0; // is ignored while reading (it's look like a some non-critical index)
  NEXT_CODE(330)
  m_idOwningNetwork = pFiler->rdObjectId();
  NEXT_CODE(360)
  m_idActionBody = pFiler->rdObjectId();

  NEXT_CODE(90)
  m_actionIndex = pFiler->rdUInt32();

  NEXT_CODE(90)
  m_maxAssocDepIdx = pFiler->rdUInt32();

  NEXT_CODE(90)
  OdUInt32 n = pFiler->rdUInt32();
  while (n-- > 0) {
    NEXT_CODE(360)
    m_arrDependencies.push_back(pFiler->rdObjectId());
  }
  if (nVersion > 1)
  {
    NEXT_CODE(90)
    if (pFiler->rdUInt32() > 0)
    {  // TODO AC2013
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }

    NEXT_CODE(90)
    // Param Ids
    n = pFiler->rdUInt32();
    ODA_ASSERT_ONCE(!n || !m_idActionBody.isNull());
    m_arrParams.reserve(n);
    while (n-- > 0) {
      NEXT_CODE(360)
      m_arrParams.append(pFiler->rdObjectId());
    }

    NEXT_CODE(90)
    if (pFiler->rdUInt32() > 0)
    {  // TODO AC2013
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }

    NEXT_CODE(90)
    n = pFiler->rdUInt32();
    ODA_ASSERT_ONCE(!n || !m_idActionBody.isNull());
    m_arrValueParams.clear();
    // see OdDbImpAssocParamBasedActionBody::dxfInFields R24 :

    while (n-- > 0) 
    {
      OdDbImpValueParamPtr pValueParam = new OdDbImpValueParam(); 
      OdResult res = pValueParam->dxfInFields(pFiler);
      if (res != eOk)
        return res;
      m_arrValueParams.push_back(pValueParam);
    }
    ODA_ASSERT_ONCE(   pFiler->atEOF()  // TODO
                       // OdDbImpAssoc2dConstraintGroup::dxfInFields does not contain subclass name reading
                    || OdDbAssoc2dConstraintGroup::cast(objectId.openObject().get()).get());
  }

  return eOk;
}

void OdDbImpAssocAction::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrSubclassMarker(OdDbAssocAction::desc()->name());

  int nVersion = (pFiler->dwgVersion() <= OdDb::vAC24) ? 1 : 2;
  pFiler->wrInt32(90, nVersion);

  pFiler->wrUInt32(90, m_Unk_UInt32_1);

  pFiler->wrObjectId(330, m_idOwningNetwork);
  pFiler->wrObjectId(360, m_idActionBody);

  pFiler->wrUInt32(90, m_actionIndex);
  pFiler->wrUInt32(90, m_maxAssocDepIdx);
  OdUInt32 n = (m_isBase) ? m_arrDependencies.size() : 0;
  pFiler->wrUInt32(90, n);
  OdUInt32 i = 0;
  for (; i < n; i++)
    pFiler->wrObjectId(360, m_arrDependencies[i]);

  if (nVersion > 1)
  {
    // AC2013
    pFiler->wrUInt32(90, 0);  

    n = m_arrParams.size();
    ODA_ASSERT_ONCE(!n || !m_idActionBody.isNull());
    pFiler->wrUInt32(90, n);
    for (i = 0; i < n; i++)
      pFiler->wrObjectId(360, m_arrParams[i]);
    
    pFiler->wrUInt32(90, 0);

    n = m_arrValueParams.size();
    ODA_ASSERT_ONCE(!n || !m_idActionBody.isNull());
    pFiler->wrUInt32(90, n);
    for (i = 0; i < n; i++)
      m_arrValueParams[i]->dxfOutFields(pFiler);
  }
}

OdStringArray OdDbImpAssocAction::compareWith(const OdDbImpAssocAction *actionIDToCompare, OdDb::DwgVersion filerVersion) const
{ 
	OdStringArray retArr;  
	
	if (m_Unk_UInt32_1 != actionIDToCompare->m_Unk_UInt32_1)
		retArr.append(CS_NOT_CRITICAL);

	if (!m_idOwningNetwork.isNull() && !actionIDToCompare->m_idOwningNetwork.isNull())
	{
		if (typeid(*(m_idOwningNetwork).openObject()) != typeid(*(actionIDToCompare->m_idOwningNetwork.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (!m_idActionBody.isNull() && !actionIDToCompare->m_idActionBody.isNull())
	{
		if (typeid(*(m_idActionBody).openObject()) != typeid(*(actionIDToCompare->m_idActionBody.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (m_actionIndex != actionIDToCompare->m_actionIndex)
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

	if (m_maxAssocDepIdx != actionIDToCompare->m_maxAssocDepIdx)
    retArr.append(CS_NOT_CRITICAL);
		//retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

	if (m_arrDependencies.size() != actionIDToCompare->m_arrDependencies.size())
	{
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
		return retArr;
	}
	
	OdUInt32 n = m_arrDependencies.size();
	
	OdUInt32 i = 0;
	
	for (i = 0; i < n; i++)
	{
		if (!m_arrDependencies[i].isNull() && !actionIDToCompare->m_arrDependencies[i].isNull())
		{
			if (typeid(*(m_arrDependencies[i]).openObject()) != typeid(*(actionIDToCompare->m_arrDependencies[i].openObject())))
				retArr.append(CS_DIFFERENT_OBJECTS);
		}
	}
	
	if (m_arrParams.size() != actionIDToCompare->m_arrParams.size())
	{
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
		return retArr;
	}

	n = m_arrParams.size();

	for (i = 0; i < n; i++)
	{
		if (!m_arrParams[i].isNull() && !actionIDToCompare->m_arrParams[i].isNull())
		{
			if (typeid(*(m_arrParams[i]).openObject()) != typeid(*(actionIDToCompare->m_arrParams[i].openObject())))
				retArr.append(CS_DIFFERENT_OBJECTS);
		}
	}

	if (m_arrValueParams.size() != actionIDToCompare->m_arrValueParams.size())
	{
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
		return retArr;
	}

	n = m_arrValueParams.size();

	for (i = 0; i < n; i++)
	{
		OdStringArray retArrV = m_arrValueParams[i]->compareWith(actionIDToCompare->m_arrValueParams[i], filerVersion);
		retArr.append(retArrV);
	}


	return retArr;
}

void OdDbImpAssocAction::composeForLoad( OdDbObject* pObj, 
                                         OdDb::SaveType format,
                                         OdDb::DwgVersion version,
                                         OdDbAuditInfo* pAuditInfo )
{
  OdDbAssocActionBodyPtr pActionBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if ( !pActionBody.isNull() && (format != OdDb::kDwg || (pObj->database()->multiThreadedMode() != OdDb::kSTMode )))
  {
    pActionBody->composeForLoad( format, version, pAuditInfo );
  }

  if (version > OdDb::vAC24 || m_idActionBody.isNull() || pActionBody.isNull())
    return;

  //OdDbImpAssocParamBasedActionBody* pParamBaseImpl = static_cast<OdDbImpAssocParamBasedActionBody*>(pActionBody->m_pImpObj);
  OdDbImpAssocActionBody* pParamBaseImpl = static_cast<OdDbImpAssocActionBody*>(pActionBody->m_pImpObj);

  bool bUpdateOwner = (m_arrParams.isEmpty());

  m_arrParams = pParamBaseImpl->m_arrParams;
  if (bUpdateOwner)
  {
    // fix owner of child to fix audit problem (for soubor_2010.dxf etc)
    OdUInt32 cnt = m_arrParams.size();
    for (OdUInt32 idx = 0; idx < cnt; idx++)
    {
      OdDbObjectId idChild = m_arrParams[idx];
      OdDbObjectPtr pChild = idChild.openObject(OdDb::kForWrite);
      ODA_ASSERT_ONCE(pChild.get() && pObj);
      OdDbObjectId idOwner = pChild->ownerId(),
                   id = pObj->objectId();
      if (idOwner == id)
        continue;
      ODA_ASSERT_ONCE(!idOwner.isNull() && id == idOwner.openObject()->ownerId());
      
      pChild->setOwnerId(id);
    }
  }

  m_arrValueParams = pParamBaseImpl->m_arrValueParams;
}

void OdDbImpAssocAction::decomposeForSave(OdDbObject* pObj,
                                          OdDb::SaveType format, 
                                          OdDb::DwgVersion version)
{
  if (version > OdDb::vAC24 || m_idActionBody.isNull())
    return;
  OdDbAssocActionBodyPtr pActionBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if (pActionBody.isNull() || !pActionBody->m_pImpObj)
  {
    //ODA_ASSERT_ONCE(OdDbProxyObject::cast(m_idActionBody.openObject()).get());
    return;
  }

  OdDbImpAssocParamBasedActionBody* pParamBaseImpl = static_cast<OdDbImpAssocParamBasedActionBody*>(pActionBody->m_pImpObj);

  pParamBaseImpl->m_arrParams = m_arrParams;
  pParamBaseImpl->m_arrValueParams = m_arrValueParams;
}

OdResult OdDbImpAssocAction::setActionIndex(int newIndex)
{
  if ( newIndex <= 0 )
    return eInvalidInput;

  m_actionIndex = newIndex;
  return eOk;
}

int OdDbImpAssocAction::getMaximumAssocDepIndex()
{
  return m_maxAssocDepIdx;
}

void OdDbImpAssocAction::setMaximumAssocDepIndex(int index)
{
  m_maxAssocDepIdx = index;
}

int OdDbImpAssocAction::findDependenciesArrayIndex(OdDbObjectId objectId)
{
  for ( unsigned int i = 0; i < m_arrDependencies.length(); i ++ )
    if ( m_arrDependencies[i] == objectId )
      return i;

  return -1;
}

void OdDbImpAssocAction::doPostEvaluateJob(OdDbAssocAction *pAction)
{
}

// AC2013
// OdDbAssocActionParams stuff
OdResult OdDbImpAssocAction::removeAllParams(bool bEraseThem)
{
  if (bEraseThem)
  {
    for (int i = m_arrParams.size(); i > 0;)
    {
      OdDbObjectPtr pParam = m_arrParams[--i].openObject(OdDb::kForWrite);
      if (!pParam.isNull())
        pParam->erase();
    }
  }
  m_arrParams.clear();
  return eOk;
}


int OdDbImpAssocAction::paramCount() const
{
  return m_arrParams.size();
}


const OdDbObjectIdArray& OdDbImpAssocAction::ownedParams() const
{
  return m_arrParams;
}


OdResult OdDbImpAssocAction::addParam(const OdDbObjectId& paramId, int& paramIndex)
{
  for (int i = m_arrParams.size(); i > 0;)
  {
    if (m_arrParams[--i] == paramId)
    {
      paramIndex = i;
      return eOk;
    }
  }
  paramIndex = m_arrParams.append(paramId);
  return eOk;
}

OdResult OdDbImpAssocAction::removeParam(const OdDbObjectId& paramId, bool alsoEraseIt)
{
  for (int i = m_arrParams.size(); i > 0;)
  {
    if (m_arrParams[--i] == paramId)
    {
      if (alsoEraseIt)
      {
        OdDbObjectPtr pParam = m_arrParams[i].openObject(OdDb::kForWrite);
        if (!pParam.isNull())
          pParam->erase();
      }
      m_arrParams.removeAt(i);
      i--;
      return eOk;
    }
  }
  return eInvalidInput;
}

OdDbObjectId OdDbImpAssocAction::paramAtIndex(int paramIndex) const
{
  return m_arrParams[paramIndex];
}

//#if defined(DB_ASSOC_ACTION_PARAM_METHOGS_DEFINED)

OdResult OdDbImpAssocAction::addParam(const OdString& paramName, 
                                      OdRxClass* pParamClass, 
                                      OdDbObjectId& paramId, 
                                      int& paramIndex,
                                      OdDbAssocAction *AssocAction)
{
  ODA_ASSERT_ONCE(!paramName.isEmpty() && pParamClass);
  OdDbObjectIdArray idPars = paramsAtName(paramName);
  if (paramIndex < (int)idPars.size())
  {
    paramId = idPars[paramIndex];
    return eAlreadyInDb;
  }

  OdDbObjectPtr pObj = OdDbObject::cast(pParamClass->create());
  if (pObj.isNull())
    return eBadObjType;

  OdDbAssocActionParamPtr pParam = OdDbAssocActionParam::cast(pObj.get()); 
  ODA_ASSERT_ONCE(!pParam.isNull());
  if (pParam.isNull())
    return eBadObjType;
  
  AssocAction->database()->addOdDbObject( pParam );

  pParam->setOwnerId( AssocAction->objectId() );
  pParam->setName(paramName);

  paramId = pParam->objectId();


  m_arrParams.push_back(paramId);

  return eOk;
}

OdDbObjectIdArray OdDbImpAssocAction::paramsAtName(const OdString& paramName) const
{
  ODA_ASSERT_ONCE(!paramName.isEmpty());

  OdDbObjectIdArray idsRes;
  OdUInt32 sz = m_arrParams.size(),
           idx = 0;
  for (; idx < sz; idx++)
  {
    OdDbObjectPtr pObj = m_arrParams[idx].openObject();
    ODA_ASSERT_ONCE(!pObj.isNull());
    if (pObj.isNull())
      continue;

    OdDbAssocActionParamPtr pParam = OdDbAssocActionParam::cast(pObj.get()); 
    ODA_ASSERT_ONCE(!pParam.isNull());
    if (pParam.isNull())
      continue;
    
    OdString sName = pParam->name();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (paramName != sName)
      continue;

    idsRes.push_back(m_arrParams[idx]);
  }

  return idsRes;
}
  
OdDbObjectId OdDbImpAssocAction::paramAtName(const OdString& paramName,
                                             int index) const // = 0
{
  OdDbObjectIdArray ids = paramsAtName(paramName);
  ODA_ASSERT_ONCE(index >= 0);
  if (index < 0 || index >= (int) ids.size())
    return OdDbObjectId();
  return ids[index];
}

void OdDbImpAssocAction::ownedValueParamNames(OdStringArray& paramNames) const
{
  paramNames.clear();

  OdArray<const OdDbImpValueParamPtr>::iterator it; 

  for (it = m_arrValueParams.begin(); it != m_arrValueParams.end(); it++)
  {
    paramNames.push_back( (*it)->name() );
  }
}

OdResult OdDbImpAssocAction::getValueParamArray(const OdString& paramName,
                                                OdArray<OdDbEvalVariantPtr>& values,
                                                OdStringArray& expressions,
                                                OdStringArray& evaluatorIds) const
{
  values.clear();
  expressions.clear();
  evaluatorIds.clear();

  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(), m_arrValueParams.end(), findValueParamByName(paramName));

  if (m_arrValueParams.end() == it)
    return eNotInGroup;

  OdDbObjectIdArray inputVariablesIds = (*it)->inputVariablesIds();
  int nEvalVars = inputVariablesIds.length();

  OdDbEvalVariantPtr pEvalVar = (*it)->value();

  for (int i = 0; i < nEvalVars; ++i)
  {
    values.append(pEvalVar);
    expressions.append(OdString());//TODO
    evaluatorIds.append(OdString());
    pEvalVar = pEvalVar->next();
  }

  return eOk;
}

OdResult OdDbImpAssocAction::getValueParam(const OdString& paramName,
                                           OdDbEvalVariant& value,
                                           OdString& expression,
                                           OdString& evaluatorId,
                                           int valueIndex) const // = 0
{
  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(), m_arrValueParams.end(), findValueParamByName(paramName));

  if (m_arrValueParams.end() == it)
    return eNotInGroup;

  if (0 == valueIndex)
  {
    value = *(*it)->value();
    return eOk;
  }

  OdDbObjectIdArray inputVariablesIds = (*it)->inputVariablesIds();
  int nEvalVars = inputVariablesIds.length();

  ODA_ASSERT(valueIndex < nEvalVars);

  if (valueIndex >= nEvalVars)
  {
    return eNotInGroup;
  }
  
  OdDbEvalVariantPtr pEvalVar = (*it)->value();

  int counter = 0;
  for (int i = 0; i < nEvalVars; ++i)
  {
    if (valueIndex == counter)
    {
      break;
    }

    pEvalVar = pEvalVar->next();
    counter++;
  }

  value = *pEvalVar;
  return eOk;
}

OdResult OdDbImpAssocAction::setValueParamArray(const OdString& paramName,
                                                const OdArray<OdDbEvalVariantPtr>& values,
                                                const OdStringArray& expressions,
                                                const OdStringArray& evaluatorIds,
                                                OdStringArray& errorMessages,
                                                bool silentMode)
{
  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(), m_arrValueParams.end(), findValueParamByName(paramName));
  OdDbImpValueParamPtr pValueParam;

  if (m_arrValueParams.end() == it)
  {
    pValueParam = new OdDbImpValueParam;
    pValueParam->setName(paramName);
    m_arrValueParams.push_back(pValueParam);
  }
  else
  {
    pValueParam = (*it);
  }

  OdDbObjectIdArray inputVariablesIds = pValueParam->inputVariablesIds();
  OdUInt16 nEvalVarsStart = inputVariablesIds.length();

  if (values.length() != nEvalVarsStart)
  {
    inputVariablesIds.resize(values.length());
    pValueParam->setInputVariablesIds(inputVariablesIds);
  }

  OdUInt16 nEvalVars = inputVariablesIds.length();

  pValueParam->unitType();
  OdDbEvalVariantPtr pLast = pValueParam->value();
  // add EvalVariants
  for (int i = 0; i < nEvalVars; ++i)
  {
    if (i < nEvalVarsStart - 1)
    {
      pLast = pLast->next();
    }

    if (i >= nEvalVarsStart)
    {
      OdDbEvalVariantPtr pNewEvVar = OdDbEvalVariant::createObject();
      pLast->setNext(pNewEvVar);
      pLast = pNewEvVar;
    }
  }

  OdDbEvalVariantPtr pEvalVar = pValueParam->value( );
  OdDbEvalVariantPtr pEvalVarPrev = pValueParam->value();
///////////////////////////////////////////////////////////
  // delete all spare EvalVariants
  if (values.length() < nEvalVarsStart)
  {
    for (OdUInt16 i = 0; i < nEvalVarsStart; i++)
    {
      pEvalVar = pEvalVarPrev->next();

      OdDbEvalVariantPtr  pEvalVarPrev1 = pEvalVarPrev->next();
      if (values.length() - 1 == i)
      {
        pEvalVarPrev->setNext(NULL);
      }

      if (i >= values.length())
      {
        pEvalVarPrev = NULL;//TODO
      }

      pEvalVarPrev = pEvalVar;
    }
  }
  ////////////////////////////////////////////////////
  pEvalVar = pValueParam->value();
  for (int i = 0; i < nEvalVars; i++)
  {
    *pEvalVar = *(values[i]);
    pEvalVar = pEvalVar->next();
  }

  return eOk;
}


OdResult OdDbImpAssocAction::setValueParam(const OdString& paramName,
                                           const OdDbEvalVariant& value,
                                           const OdString& expression,
                                           const OdString& evaluatorId,
                                           OdString& errorMessage,
                                           bool silentMode,
                                           int valueIndex) // = 0
{
  OdArray<OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(),
    m_arrValueParams.end(),
    findValueParamByName(paramName));

  OdDbImpValueParamPtr pValueParam;// = new OdDbImpValueParam;

  if (m_arrValueParams.end() == it)
  {
    pValueParam = new OdDbImpValueParam;
    pValueParam->setName(paramName);
    m_arrValueParams.push_back(pValueParam);
  }
  else
  {
    pValueParam = (*it);
  }

  OdDbObjectIdArray inputVariablesIds = pValueParam->inputVariablesIds();
  int nEvalVarsStart = inputVariablesIds.length();

  if (0 == valueIndex)
  {
    OdDbEvalVariantPtr pEvalVar = pValueParam->value();
    OdDbEvalVariantPtr pEvalVarNext = pEvalVar->next();

    pValueParam->setValue(value);

    if (nEvalVarsStart > 1)
    {
      pEvalVar = pValueParam->value();
      pEvalVar->setNext(pEvalVarNext);
      inputVariablesIds.resize(nEvalVarsStart);
      pValueParam->setInputVariablesIds(inputVariablesIds);
    }

    return eOk;
  }

  if (valueIndex >= nEvalVarsStart)
  {
    inputVariablesIds.resize(valueIndex + 1);
    pValueParam->setInputVariablesIds(inputVariablesIds);
  }

  inputVariablesIds = pValueParam->inputVariablesIds();
  int nEvalVars = inputVariablesIds.length();

  OdDbEvalVariantPtr pLast = pValueParam->value();

  for (int i = 0; i < nEvalVars; ++i)
  {
    if (i < nEvalVarsStart - 1)
    {
      pLast = pLast->next();
    }
    
    if (i >= nEvalVarsStart)
    {
      OdDbEvalVariantPtr pNewEvVar = OdDbEvalVariant::createObject();
      pLast->setNext(pNewEvVar);
      pLast = pNewEvVar;
    }
  }

  OdDbEvalVariantPtr pEvalVar = pValueParam->value();

  int counter = 0;
  for (int i = 0; i < nEvalVars; ++i)
  {
    if (valueIndex == counter)
    {
      break;
    }

    pEvalVar = pEvalVar->next();
    counter++;
  }

  *pEvalVar = value;
  return eOk;
}

OdValue::UnitType OdDbImpAssocAction::valueParamUnitType(const OdString& paramName) const
{
  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                                   m_arrValueParams.end(), 
                                                                   findValueParamByName(paramName) );

  if ( m_arrValueParams.end() == it )
  {
    return OdValue::kUnitless;
  }
  else
  {
    return (*it)->unitType();
  }
}

OdResult OdDbImpAssocAction::setValueParamUnitType(const OdString& paramName, 
                                                   OdValue::UnitType unitType)
{
  OdArray<OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                             m_arrValueParams.end(), 
                                                             findValueParamByName(paramName) );

  if ( m_arrValueParams.end() == it )
  {
    return eNotInGroup;
  }

  (*it)->setUnitType (unitType);
  return eOk;
}

OdResult OdDbImpAssocAction::removeValueParam(const OdString& paramName)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::valueParamInputVariables(const OdString& paramName,
                                                      OdDbObjectIdArray& variableIds) const
{
  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                                   m_arrValueParams.end(), 
                                                                   findValueParamByName(paramName) );
  if ( m_arrValueParams.end() == it )
  {
    return eInvalidInput;
  }
  else
  {
    variableIds = (*it)->inputVariablesIds();
  }
  return eOk;
}

OdResult OdDbImpAssocAction::setValueParamControlledObjectDep(const OdString& paramName, 
                                                              const OdDbObjectId& controlledObjectDepId)
{
  OdArray<OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                                   m_arrValueParams.end(), 
                                                                   findValueParamByName(paramName) );

  if ( m_arrValueParams.end() == it )
  {
    return eInvalidInput;
  }
  else
  {
    (*it)->setControlledObjectDepId(controlledObjectDepId);
  }
  return eOk;
}

OdResult OdDbImpAssocAction::updateValueParamControlledObject(const OdString& paramName) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::updateValueParamFromControlledObject(const OdString& paramName)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::updateAllObjectsControlledByValueParams() const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::transformAllConstantGeometryParams(const OdGeMatrix3d& transform)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::scaleAllDistanceValueParams(double scaleFactor)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
//#endif


void OdDbImpAssocAction::audit(OdDbObject* pObj, OdDbAuditInfo* pAuditInfo)
{
  if (pObj->isA() == OdDbAssocAction::desc())
  { // Pure OdDbAssocAction must have body
    OdDbObjectPtr pActionBody = actionBody().openObject();
    if (pActionBody.isNull()
      || (!pActionBody->isKindOf(OdDbAssocActionBody::desc()) && pActionBody->isA() != OdDbProxyObject::desc()))
    {
      OdDbHostAppServices * pHostApp = pObj->database()->appServices();
      pAuditInfo->errorsFound(1);
      pAuditInfo->printError( pObj,
        pHostApp->formatMessage( sidAssocActionBodyIsInvalid ),
        pHostApp->formatMessage( sidVarValidInvalid ),
        pHostApp->formatMessage( sidVarDefRemove )
        );

      if (pAuditInfo->fixErrors())
      {
        pObj->erase();
        pAuditInfo->errorsFixed(1);
      }
    }
  }
}

