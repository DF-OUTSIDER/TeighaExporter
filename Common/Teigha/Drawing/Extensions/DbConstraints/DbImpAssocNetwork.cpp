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
#include "DbImpAssocNetwork.h"
#include "DbAssocNetwork.h"
#include "DbFiler.h"
#include "DbDictionary.h"
#include "DbImpAssocAction.h"
#include "DbAssocAction.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocNetworkDeepCloneAux.h"
#include "DbAssocVariable.h"
#include "DbHostAppServices.h"
#include "DbAudit.h"

OdDbImpAssocNetwork::OdDbImpAssocNetwork()
  : OdDbImpAssocAction()
  , m_maxChildActionIdx(0)
{
  m_isBase = false;
}

OdDbObjectIdArray OdDbImpAssocNetwork::getActions() const
{
  return m_idRefs;
}

OdResult OdDbImpAssocNetwork::addAction( const OdDbObjectId networkId, const OdDbObjectId& actionId, bool alsoSetAsDatabaseOwner )
{
  OdDbObjectPtr pObj = actionId.openObject(OdDb::kForWrite);
  if ( !pObj->isKindOf(OdDbAssocAction::desc()) )
    return eInvalidInput;

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(pObj);
  if (   networkId == pAction->owningNetwork() 
      && (!alsoSetAsDatabaseOwner || networkId ==  pAction->ownerId())
      && m_idRefs.contains(actionId))
  {
    ODA_FAIL_ONCE(); // temp test marker
    // #11465 fix audit problem via cloning (select + drag)
    return eInvalidInput; // already added
  }

  pAction->setOwningNetwork(networkId, alsoSetAsDatabaseOwner);

  m_idRefs.push_back(actionId);
  m_bIsHard.push_back(!pAction->isKindOf(OdDbAssocNetwork::desc()));

  m_maxChildActionIdx++;

  OdDbImpAssocAction *pImpAction = OdDbImpAssocAction::getImpObject(pAction);
  pImpAction->setActionIndex(m_maxChildActionIdx);

  return eOk;
}

OdResult OdDbImpAssocNetwork::removeAction(const OdDbObjectId& actionId, bool alsoEraseIt)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
  if (pAction.isNull())
    alsoEraseIt = false;
  else
    pAction->setOwningNetwork(OdDbObjectId::kNull, false);

  int i = m_idRefs.size();
  while (i-- > 0) {
    if (m_idRefs[i] == actionId) {
      m_idRefs.removeAt(i);
      m_bIsHard.removeAt(i);
      break;
    }
  }

  if ( alsoEraseIt )
  {
    pAction->erase();
    pAction->downgradeOpen();
  }
  return eOk;
}

OdResult OdDbImpAssocNetwork::postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  // Clear incomplete groups, this also removes some actions
  for (unsigned int i = 0; i < m_idRefs.size(); i++)
  {
    OdDbObjectPtr pObj = m_idRefs[i].openObject();

    if (!pObj.isNull() && pObj->isA() == OdDbAssoc2dConstraintGroup::desc())
    {
      pObj = 0;
      OdDbAssocNetworkDeepCloneAux::removeIncompleteConstraints(m_idRefs[i], idMap);
    }
  }

  int copyMode = idMap.origDb()->appServices()->getPARAMETERCOPYMODE();
  OdDbAssocNetworkDeepCloneAux::clearIncompleteExpressions(m_idRefs, copyMode, idMap);

  std::map<OdDbObjectId, OdDbObjectId> * pSrcToDestIdMap = OdDbAssocNetworkDeepCloneAux::srcToDestIdMap(idMap);
  ODA_ASSERT(pSrcToDestIdMap);

  OdDbObjectId srcBlockId = pAction->objectThatOwnsNetworkInstance();
  OdDbObjectId destBlockId = pSrcToDestIdMap ? (*pSrcToDestIdMap)[srcBlockId] : OdDbObjectId();

  // If network doesn't exist (block, wblock) don't create it here
  OdDbObjectId checkDestNetworkId = OdDbAssocNetwork::getInstanceFromObject(destBlockId, false);
  OdDbAssocNetworkDeepCloneAux::mergeVariables(m_idRefs, copyMode, idMap, checkDestNetworkId);

  // Create destination network
  OdDbObjectId destNetworkId = OdDbAssocNetwork::getInstanceFromObject(destBlockId, true, true);
  OdDbAssocNetworkPtr pDestNetwork = destNetworkId.openObject(OdDb::kForWrite);

  // Groups have to be treated after all the actions are postProcessed
  OdDbObjectIdArray aGroupIds;
  for (unsigned int i = 0; i < m_idRefs.size(); i++)
  {
    OdDbObjectId actionId = m_idRefs[i];
    OdDbAssocActionPtr pO = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
    if (pO.isNull())
      continue;
    if (pO->isKindOf(OdDbAssoc2dConstraintGroup::desc()))
    {
      aGroupIds.append(actionId);
      continue;
    }
    // Now give the action a chance to merge itself with some other
    if (pO->postProcessAfterDeepClone(idMap) == eIteratorDone)
      break;
  }

  unsigned int groupIdLength = aGroupIds.length();
  for (unsigned int i=0; i<groupIdLength; i++)
  {
    OdDbObjectId groupId = aGroupIds[i];
    OdDbAssocActionPtr pO = OdDbAssocAction::cast(groupId.openObject(OdDb::kForWrite));
    pO->postProcessAfterDeepClone(idMap);
  }

  return eOk;
}

OdResult OdDbImpAssocNetwork::postProcessAfterDeepCloneCancel(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  return eOk;
}

OdResult OdDbImpAssocNetwork::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dwgInFields(pFiler, objectId);
  if ( res != eOk)
    return res;

#ifdef _DEBUG
  int val = pFiler->rdInt16();
  ODA_ASSERT(val == 0);    // Version?
#else
  pFiler->rdInt16();
#endif // _DEBUG

  m_maxChildActionIdx = pFiler->rdInt32();
  OdUInt32 nRefs = pFiler->rdInt32();
  m_idRefs.resize(nRefs);
  m_bIsHard.resize(nRefs);
  OdDbObjectId* pId = m_idRefs.asArrayPtr();
  bool* pHard = m_bIsHard.asArrayPtr();
  while (nRefs--)
  {
    *pHard = pFiler->rdBool();
    if (*pHard++)
    {
      *pId++ = pFiler->rdHardOwnershipId();
    }
    else
    {
      *pId++ = pFiler->rdSoftPointerId();
    }
  }

  // Some more references
  nRefs = pFiler->rdInt32();
  m_idRefs1.resize(nRefs);
  pId = m_idRefs1.asArrayPtr();
  while (nRefs--)
  {
    *pId++ = pFiler->rdSoftPointerId();
  }
  return eOk;
}


void OdDbImpAssocNetwork::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dwgOutFields(pFiler, objectId);
  pFiler->wrInt16(0);   // Version?
  pFiler->wrInt32(m_maxChildActionIdx);

  OdUInt32 nRefs = m_idRefs.size();
  pFiler->wrInt32(nRefs);
  const OdDbObjectId* pId = m_idRefs.asArrayPtr();
  const bool* pHard = m_bIsHard.asArrayPtr();
  while (nRefs--)
  {
    pFiler->wrBool(*pHard);
    if (*pHard++)
    {
      pFiler->wrHardOwnershipId(*pId++);
    }
    else
    {
      pFiler->wrSoftPointerId(*pId++);
    }
  }

  //Some more references
  nRefs = m_idRefs1.size();
  pFiler->wrInt32(nRefs);
  pId = m_idRefs1.asArrayPtr();
  while (nRefs--)
  {
    pFiler->wrSoftPointerId(*pId++);
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdDbImpAssocNetwork::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  // Check that we are at the correct subclass data
  if( !pFiler->atSubclassData( OdDbAssocNetwork::desc()->name() ))
  {
    return eBadDxfSequence;
  }

  NEXT_CODE(90)
#ifdef ODA_DIAGNOSTICS
  int val = 
#endif // ODA_DIAGNOSTICS
  pFiler->rdInt32();
  ODA_ASSERT(val == 0);  // Version?

  NEXT_CODE(90)
  m_maxChildActionIdx = pFiler->rdInt32();
  NEXT_CODE(90)
  OdUInt32 nRefs = pFiler->rdInt32();
  m_idRefs.resize(nRefs);
  m_bIsHard.resize(nRefs);
  OdDbObjectId* pId = m_idRefs.asArrayPtr();
  bool* pHard = m_bIsHard.asArrayPtr();
  while (nRefs--)
  {
    switch(pFiler->nextItem())

    {
    case 330:
      *pHard++ = false;
      break;
    case 360:
      *pHard++ = true;
      break;
    default:
      ODA_FAIL();
      throw OdError(eBadDxfSequence);
    }
    *pId++ = pFiler->rdObjectId();
  }

  // More references
  NEXT_CODE(90)
  nRefs = pFiler->rdInt32();
  m_idRefs1.resize(nRefs);
  pId = m_idRefs1.asArrayPtr();
  while (nRefs--)
  {
    NEXT_CODE(330)
    *pId++ = pFiler->rdObjectId();
  }

  return eOk;
}

void OdDbImpAssocNetwork::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker( OdDbAssocNetwork::desc()->name() );

  pFiler->wrInt32(90, 0);   // Version ?

  pFiler->wrInt32(90, m_maxChildActionIdx);

  OdUInt32 nRefs = m_idRefs.size();
  pFiler->wrInt32(90, nRefs);
  const OdDbObjectId* pId = m_idRefs.asArrayPtr();
  const bool* pHard = m_bIsHard.asArrayPtr();
  while (nRefs--)
  {
    if (*pHard++)
    {
      pFiler->wrObjectId(360, *pId++);
    }
    else
    {
      pFiler->wrObjectId(330, *pId++);
    }
  }

  // More references
  nRefs = m_idRefs1.size();
  pFiler->wrInt32(90, nRefs);
  pId = m_idRefs1.asArrayPtr();
  while (nRefs--)
  {
    pFiler->wrObjectId(330, *pId++);
  }
}

OdResult OdDbImpAssocNetwork::setStatus(OdDbAssocStatus newStatus,  bool notifyOwningNetwork, bool setInOwnedActions)
{
  OdResult res = OdDbImpAssocAction::setStatus(newStatus,  notifyOwningNetwork, setInOwnedActions);
  if ( res != eOk )
    return res;

  if ( setInOwnedActions )
  {
    OdDbObjectIdArray attachedActions = getActions();

    for (unsigned int i = 0; i < attachedActions.size(); i++) {
      OdDbAssocActionPtr curAction = OdDbAssocAction::cast(attachedActions[i].openObject(OdDb::kForWrite));
      if (!curAction.isNull())
        curAction->setStatus(newStatus, false, setInOwnedActions);
    }
  }

  return eOk;
}

void OdDbImpAssocNetwork::evaluate(OdDbAssocAction *pAction, OdDbAssocEvaluationCallback* pEvaluationCallback)
{
  OdDbObjectIdArray attachedActions = getActions();
  for (unsigned int i = 0; i < attachedActions.size(); i++)
  {
    OdDbAssocActionPtr pChildAction = OdDbAssocAction::cast(attachedActions[i].openObject(OdDb::kForWrite));
    if (pChildAction.isNull())
      continue;
    OdDbAssocStatus st = pChildAction->status();
    if (st != kIsUpToDateAssocStatus)
    {
      pChildAction->evaluate(pEvaluationCallback);
      if (kErasedAssocStatus == pChildAction->status())
      {
        removeAction(attachedActions[i], true);
      }
      else if (kRemovedFromNetwork != pChildAction->status())
        pChildAction->setStatus(kIsUpToDateAssocStatus, false, false);
    }
  }
  if (m_idRefs.length() == 0) {
    OdDbAssocNetworkPtr net = OdDbAssocNetwork::cast(owningNetwork().openObject(OdDb::kForWrite));
    if (!net.isNull())
      net->removeAction(pAction->objectId(), false);
    OdDbDictionaryPtr dict = OdDbDictionary::cast(pAction->ownerId().openObject(OdDb::kForWrite));
    if (!dict.isNull())
    {
      OdDbDictionaryPtr dictTOP = OdDbDictionary::cast( dict->ownerId().openObject(OdDb::kForWrite) );
      dict->erase(true);
      dict->downgradeOpen();
      if ( !dictTOP.isNull() && !dictTOP->numEntries() )
      {
        dictTOP->erase();
      }
    }

    pAction->erase();
  }
}

void OdDbImpAssocNetwork::audit(OdDbObject* pObj, OdDbAuditInfo* pAuditInfo)
{
  OdDb::OpenMode mode = pAuditInfo->fixErrors() ? OdDb::kForWrite : OdDb::kForRead;
  int i = m_idRefs.size();
  while (i-- > 0)
  {
    OdDbObjectPtr pObj = m_idRefs[i].openObject(mode);
    bool bRemove;
    if (pObj.isNull())
    {
      bRemove = true;
    }
    else
    {
      pObj->audit(pAuditInfo);
      // For permanently erased object with Null objectId pObj->isErased() returns false
      bRemove = pObj->objectId().isErased();
    }
    if (bRemove)
    {
      m_idRefs.removeAt(i);
      m_bIsHard.removeAt(i);
    }
  }
}

