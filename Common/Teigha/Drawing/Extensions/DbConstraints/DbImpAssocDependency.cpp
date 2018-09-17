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
#include "DbImpAssocDependency.h"
#include "DbAssocDependency.h"
#include "DbFiler.h"
#include "DbAssocDependencyBody.h"
#include "DbAssocNetwork.h"
#include "DbImpAssocAction.h"
#include "OdDbAssocDependencyPE.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbCompoundObjectId.h"
//#include "DbCompoundObjectIdImpl.h"
#include <typeinfo>
////////////////////////////////////////////////////////////////////

OdDbImpAssocDependency::OdDbImpAssocDependency():
    m_nStatus(kIsUpToDateAssocStatus)
    ,m_nOrder(0)
    ,m_nIndex(0)
    ,m_bReadDep(true)
    ,m_bWriteDep(true)
    ,m_bObjectStateDependent(true)
    ,m_bDelegatingToOwningAction(false)
    ,m_refPath()
{
  m_prevDependencyOnObjectID = OdDbObjectId::kNull;
  m_nextDependencyOnObjectID = OdDbObjectId::kNull;
}

OdDbImpAssocDependency::~OdDbImpAssocDependency()
{
  m_refPath.release();
}

OdResult OdDbImpAssocDependency::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = eOk;
  OdUInt16 ver = pFiler->rdInt16();
  if (ver != 2)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_nStatus = (OdDbAssocStatus)pFiler->rdInt32();

  m_bReadDep = pFiler->rdBool();
  m_bWriteDep = pFiler->rdBool();
  m_bObjectStateDependent = pFiler->rdBool();
  m_bDelegatingToOwningAction = pFiler->rdBool();

  m_nOrder = pFiler->rdInt32();
  m_dependentOnObjectId = pFiler->rdSoftPointerId();

  res = m_refPath.dwgInFields(pFiler, 2);
  if ( res != eOk )
    return res;
  m_prevDependencyOnObjectID = pFiler->rdSoftPointerId();
  m_nextDependencyOnObjectID = pFiler->rdSoftPointerId();
  m_dependencyBodyId = pFiler->rdHardOwnershipId();
  m_nIndex = pFiler->rdInt32();
  return res;
}

void OdDbImpAssocDependency::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrInt16(2);   // Version?
  pFiler->wrInt32(m_nStatus);

  pFiler->wrBool(m_bReadDep);
  pFiler->wrBool(m_bWriteDep);
  pFiler->wrBool(m_bObjectStateDependent);
  pFiler->wrBool(m_bDelegatingToOwningAction);

  pFiler->wrInt32(m_nOrder);
  pFiler->wrSoftPointerId(m_dependentOnObjectId);
  m_refPath.dwgOutFields(pFiler, 0);
  pFiler->wrSoftPointerId(m_prevDependencyOnObjectID);
  pFiler->wrSoftPointerId(m_nextDependencyOnObjectID);
  pFiler->wrHardOwnershipId(m_dependencyBodyId);
  pFiler->wrInt32(m_nIndex);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdDbImpAssocDependency::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = eOk;

  // Check that we are at the correct subclass data
  if( !pFiler->atSubclassData(OdDbAssocDependency::desc()->name() ))
  {
    return eBadDxfSequence;
  }
  
  // Read version number (must be first)
  if( pFiler->nextItem() != 90 || pFiler->rdInt32() != 2)
    return eMakeMeProxy;

  NEXT_CODE(90)
  m_nStatus = (OdDbAssocStatus)pFiler->rdInt32();

  NEXT_CODE(290)
  m_bReadDep = pFiler->rdBool();
  NEXT_CODE(290)
  m_bWriteDep = pFiler->rdBool();
  NEXT_CODE(290)
  m_bObjectStateDependent = pFiler->rdBool();
  NEXT_CODE(290)
  m_bDelegatingToOwningAction = pFiler->rdBool();

  NEXT_CODE(90)
  m_nOrder = pFiler->rdInt32();
  NEXT_CODE(330)
  m_dependentOnObjectId = pFiler->rdObjectId();

  NEXT_CODE(290)
  res = m_refPath.dxfInFields(pFiler, 0, 0);
  if ( res != eOk )
    return res;
  NEXT_CODE(330)
  m_prevDependencyOnObjectID = pFiler->rdObjectId();
  NEXT_CODE(330)
  m_nextDependencyOnObjectID = pFiler->rdObjectId();
  NEXT_CODE(360)
  m_dependencyBodyId = pFiler->rdObjectId();
  NEXT_CODE(90)
  m_nIndex = pFiler->rdInt32();
  return res;
}
#undef NEXT_CODE


void OdDbImpAssocDependency::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrSubclassMarker(OdDbAssocDependency::desc()->name());
  pFiler->wrInt32(90, 2);   // Version?
  pFiler->wrInt32(90, m_nStatus);

  pFiler->wrBool(290, m_bReadDep);
  pFiler->wrBool(290, m_bWriteDep);
  pFiler->wrBool(290, m_bObjectStateDependent);
  pFiler->wrBool(290, m_bDelegatingToOwningAction);

  pFiler->wrInt32(90, m_nOrder);
  pFiler->wrObjectId(330, m_dependentOnObjectId);

  m_refPath.dxfOutFields(pFiler, 0);

  pFiler->wrObjectId(330, m_prevDependencyOnObjectID);
  pFiler->wrObjectId(330, m_nextDependencyOnObjectID);
  pFiler->wrObjectId(360, m_dependencyBodyId);
  pFiler->wrInt32(90, m_nIndex);
}

OdDbObjectId OdDbImpAssocDependency::dependencyBody() const
{
  return m_dependencyBodyId;
}
    
OdResult OdDbImpAssocDependency::setDependencyBody(OdDbObjectId dependencyBodyId, OdDbObjectId dependencyId)
{
  m_dependencyBodyId = dependencyBodyId;
  OdDbObjectPtr pObj = m_dependencyBodyId.openObject(OdDb::kForWrite);
  pObj->setOwnerId(dependencyId);
  return eOk;
}

OdDbAssocStatus OdDbImpAssocDependency::status() const
{
  return (OdDbAssocStatus)m_nStatus;
}

OdResult OdDbImpAssocDependency::setStatus(OdDbAssocStatus newStatus, bool notifyOwningAction, OdDbAssocDependency *pDep)
{
  if ( newStatus == m_nStatus && !notifyOwningAction && newStatus != kChangedDirectlyAssocStatus)
    return eOk;

  if ( isEvaluationRequest(newStatus) )
  {
    if ( !isToBeSkipped(m_nStatus) )
    {
      if ( isEvaluationRequest(m_nStatus) )
      {
        if ( evaluationRequestSeverityLevel(newStatus) > evaluationRequestSeverityLevel(m_nStatus) )
          m_nStatus = newStatus;
      }
      else
      {
        m_nStatus = newStatus;     
        notifyOwningAction = true;
      }
    }
  }
  else
  {
    m_nStatus = newStatus;
  }

  if ( notifyOwningAction )
  {
    OdDbAssocActionPtr actionPtr = pDep->owningAction().openObject(OdDb::kForWrite);
    if (!actionPtr.isNull())
      actionPtr->setStatus(newStatus);
  }

  //TODO:restore previous status
  //kIsUpToDateAssocStatus;

  return eOk;
}

bool OdDbImpAssocDependency::isReadDependency() const
{
  return m_bReadDep;
}

bool OdDbImpAssocDependency::isWriteDependency() const
{
  return m_bWriteDep;
}

void OdDbImpAssocDependency::setIsReadDependency(bool bRead)
{
  m_bReadDep = bRead;
}

void OdDbImpAssocDependency::setIsWriteDependency(bool bWrite)
{
  m_bWriteDep = bWrite;
}

bool OdDbImpAssocDependency::isObjectStateDependent() const
{
  return m_bObjectStateDependent;
}

void OdDbImpAssocDependency::setIsObjectStateDependent(bool bDependent)
{
  m_bObjectStateDependent = bDependent;
}

int OdDbImpAssocDependency::order() const
{
  return m_nOrder;
}

void OdDbImpAssocDependency::setOrder(int newOrder)
{
  m_nOrder = (OdInt32)newOrder;
}


OdResult OdDbImpAssocDependency::setOwningAction(OdDbObjectId actionId, OdDbAssocDependency *pDep)
{
  pDep->setOwnerId(actionId);
  if ( actionId.isNull() )
    return eOk;

  OdDbAssocActionPtr pAction = actionId.openObject();
  OdDbImpAssocAction *pImpActtion = OdDbImpAssocAction::getImpObject(pAction);
  
  m_nIndex = pImpActtion->getMaximumAssocDepIndex() + 1;
  pImpActtion->setMaximumAssocDepIndex(m_nIndex);

  return eOk;
}


OdDbObjectId OdDbImpAssocDependency::dependentOnObject() const
{
  return m_dependentOnObjectId;
}

bool OdDbImpAssocDependency::isDependentOnCompoundObject() const
{
  return !m_refPath.isEmpty();
}

OdDbObjectId OdDbImpAssocDependency::prevDependencyOnObject() const
{
  return m_prevDependencyOnObjectID;
}

 
OdDbObjectId OdDbImpAssocDependency::nextDependencyOnObject() const
{
  return m_nextDependencyOnObjectID;
}

OdResult OdDbImpAssocDependency::attachToObject(const OdDbCompoundObjectId* pCompoundId, OdDbAssocDependency *pDep)
{
  ODA_ASSERT(pDep != NULL);

  if (isAttachedToObject() || !isReadyToAttach())
    return eBadObjType;

  OdDbObjectIdArray arrObjId;
  if (pCompoundId->getFullPath(arrObjId) != eOk)
    return eBadObjType;

  //OdDbObjectPtr pObj = pCompoundId->leafId().openObject(); // AlexeyTyurin 11062017 : CORE-12281 If the length of path of pCompoundId > 1
  // the top usually is OdDbObjectRef () but the leaf usually is Entity (circle, line, etc.) that
  // hasn't dependencies
  OdDbObjectPtr pObj = pCompoundId->topId().openObject();
  ODA_ASSERT(!pObj.isNull());

  OdSmartPtr<OdDbAssocDependencyPE> pPE = pObj->isA()->getX(OdDbAssocDependencyPE::desc());
  if (!pPE.isNull() && !pPE->allowsDependencies(pObj, isNeedWriteDependency()))
  {
    return eVetoed; // according to ObjectARX man
  }

  if (arrObjId.length() > 1)
    m_refPath.set(*pCompoundId, pObj->database());

  const OdDbObjectId depId = pDep->objectId();
  OdDbObjectId lastDepId = OdDbAssocDependency::getFirstDependencyOnObject(pObj);
  OdDbAssocDependencyPtr pLastDep = OdDbAssocDependency::cast(lastDepId.openObject());

  for (unsigned int i = 0; i < arrObjId.length(); i++)
  {
    OdDbObjectPtr pObjTemp = arrObjId[i].openObject(OdDb::kForWrite);
    pObjTemp->addPersistentReactor(depId);
  }

  for (; !pLastDep.isNull() && !pLastDep->nextDependencyOnObject().isNull(); )
  {
    lastDepId = pLastDep->nextDependencyOnObject();
    pLastDep = OdDbAssocDependency::cast(lastDepId.openObject());
  }

  if (!pLastDep.isNull())
  {
    pLastDep->upgradeOpen();
    pLastDep->assertWriteEnabled();

    if (lastDepId != depId)
      pLastDep->m_pImpObj->m_nextDependencyOnObjectID = depId;
  }

  if (depId != lastDepId)
    m_prevDependencyOnObjectID = lastDepId;

  m_dependentOnObjectId = pObj->objectId();

  return eOk;
}

void OdDbImpAssocDependency::repairDependencies(OdDbObject* pObj)
{
  if (!pObj)
    return;
  OdDbObjectIdArray aReactors = pObj->getPersistentReactors();
  OdDbObjectIdArray aDeps;
  aDeps.push_back(OdDbObjectId());
  int i = aReactors.size();
  while (i-- > 0)
  {
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(aReactors[i].openObject());
    if (!pDep.isNull())
    {
      if (pDep->dependentOnObject() != pObj->objectId())
      {
        pObj->upgradeOpen();
        pObj->removePersistentReactor(aReactors[i]);
        aReactors.removeAt(i);
      }
      else
        aDeps.push_back(aReactors[i]);
    }
  }
  aDeps.push_back(OdDbObjectId());
  i = aDeps.size();
  while (i-- > 0)
  {
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(aDeps[i].openObject());
    if (!pDep.isNull())
    {
      if (pDep->m_pImpObj->m_prevDependencyOnObjectID != aDeps[i - 1] ||
        pDep->m_pImpObj->m_nextDependencyOnObjectID != aDeps[i + 1])
      {
        pDep->upgradeOpen();
        pDep->assertWriteEnabled();
        pDep->m_pImpObj->m_prevDependencyOnObjectID = aDeps[i - 1];
        pDep->m_pImpObj->m_nextDependencyOnObjectID = aDeps[i + 1];
      }
    }
  }
}

OdResult OdDbImpAssocDependency::dependentOnObjectStatus() const
{
  if ( isAttachedToObject() )
    return eOk;
  else
    return eBadObjType;
}

OdResult OdDbImpAssocDependency::detachFromObject(OdDbAssocDependency *pDep)
{
  if ( !isAttachedToObject() )
    return eBadObjType;

  OdDbObjectIdArray arrObjId;
  if ( m_refPath.getFullPath(arrObjId) != eOk )
    return eBadObjType;

  OdDbObjectPtr pObj;
  OdDbObjectId depId = pDep->objectId();
  for ( unsigned int i = 0; i < arrObjId.length(); i++ )
  {
    pObj = arrObjId[i].openObject(OdDb::kForWrite);
    if (!pObj.isNull())
      pObj->removePersistentReactor(depId);
  }

  pObj = m_dependentOnObjectId.openObject(OdDb::kForWrite);
  if (!pObj.isNull())
    pObj->removePersistentReactor(depId);
  if (!m_prevDependencyOnObjectID.isNull()) {
    OdDbAssocDependencyPtr pPrevDep = m_prevDependencyOnObjectID.openObject(OdDb::kForWrite);
    if (!pPrevDep.isNull()) {
      pPrevDep->assertWriteEnabled();
      pPrevDep->m_pImpObj->m_nextDependencyOnObjectID = m_nextDependencyOnObjectID;
    }
  }
  if (!m_nextDependencyOnObjectID.isNull()) {
    OdDbAssocDependencyPtr pNextDep = m_nextDependencyOnObjectID.openObject(OdDb::kForWrite);
    if (!pNextDep.isNull()) {
      pNextDep->assertWriteEnabled();
      pNextDep->m_pImpObj->m_prevDependencyOnObjectID = m_prevDependencyOnObjectID;
    }
  }
  m_prevDependencyOnObjectID.setNull();
  m_nextDependencyOnObjectID.setNull();

  m_dependentOnObjectId = OdDbObjectId::kNull;

  return eOk;
}

OdResult OdDbImpAssocDependency::updateDependentOnObject(OdDbAssocDependency *pDep)
{
  if ( !dependencyBody().isNull() )
  {
    OdDbAssocDependencyBodyPtr body = OdDbAssocDependencyBody::cast(m_dependencyBodyId.openObject(OdDb::kForWrite));
    return body->updateDependentOnObjectOverride();
  }

  if ( m_nStatus == kIsUpToDateAssocStatus )
    return eOk;
  else
    return eBadObjType;
}

void OdDbImpAssocDependency::setDependentOnObject(OdDbCompoundObjectId* pCompoundId, OdDbAssocDependency *pDep)
{
  m_refPath.set(*pCompoundId, pDep->database());
  m_dependentOnObjectId = m_refPath.leafId();
}

bool OdDbImpAssocDependency::isDelegatingToOwningAction() const
{
  return m_bDelegatingToOwningAction;
}


void OdDbImpAssocDependency::setIsDelegatingToOwningAction(bool bDelegating)
{
  m_bDelegatingToOwningAction = bDelegating;
}

/*
bool OdDbImpAssocDependency::hasCachedValue() const;
*/
bool OdDbImpAssocDependency::isRelevantChange(const OdDbAssocDependency *pDep) const
{
  if ( isDelegatingToOwningAction() )
  {
    OdDbAssocActionPtr actionPtr = OdDbAssocAction::cast(pDep->owningAction().openObject());

    if ( actionPtr.isNull() )
      return true;

    return actionPtr->isRelevantDependencyChange(pDep);
  }
  else
    return false;
}
/*
    bool OdDbImpAssocDependency::isDependentOnTheSameThingAs(const OdDbAssocDependency* pOtherDependency) const;

    bool OdDbImpAssocDependency::isDependentOnObjectReadOnly() const;

    bool OdDbImpAssocDependency::isEqualTo(const OdDbAssocDependency* pOtherDependency) const;
*/
bool OdDbImpAssocDependency::isActionEvaluationInProgress( const OdDbAssocDependency *pDependency ) const
{
   OdDbAssocActionPtr pAction = OdDbAssocAction::cast( pDependency->owningAction().openObject( OdDb::kForRead ) );
   if ( pAction.isNull() )
   {
     return false;
   }

   return pAction->isActionEvaluationInProgress();
}

OdDbAssocEvaluationCallback* OdDbImpAssocDependency::currentEvaluationCallback() const
{
  ODA_FAIL_ONCE(); // TODO : Implement me!
  return NULL;
}

void OdDbImpAssocDependency::evaluate()
{
  
}


void OdDbImpAssocDependency::erased(const OdDbObject* dbObj, OdDbAssocDependency *pDep, bool isErasing)
{
  if (isErasing) {
    if (dbObj->objectId() == dependentOnObject()) {
      OdDbAssoc2dConstraintGroupPtr pGroup = OdDbAssoc2dConstraintGroup::cast(pDep->owningAction().openObject());
      if (!pGroup.isNull()) {
        OdArray<OdGeomConstraint*> apConstraints;
        pGroup->getConstraints(apConstraints);
        unsigned int i;
        for (i = 0; i < apConstraints.size(); i++) {
          OdSmartPtr<OdExplicitConstraint> pDim = OdExplicitConstraint::cast(apConstraints[i]);
          if (!pDim.isNull() && (pDim->dimDependencyId() == pDep->objectId() ||
            pDim->valueDependencyId() == pDep->objectId()))
          {
            pGroup->upgradeOpen();
            pGroup->deleteConstraint(pDim);
            break;
          }
        }
      }
    }
    
    if ( !dbObj->isUndoing() )
    {
      //detachFromObject(pDep);
      setStatus( kChangedDirectlyAssocStatus, true, pDep );
      setStatus( kErasedAssocStatus, false, pDep );
    }
  }
  else
  {
    setStatus( kIsUpToDateAssocStatus, false, pDep );
  }
}

void OdDbImpAssocDependency::modified(const OdDbObject* dbObj, OdDbAssocDependency *pDep)
{
  if ( pDep->isUndoing() || dbObj->isUndoing() || pDep->objectId().isEffectivelyErased() 
    || dbObj->database()->isDatabaseConverting() || dbObj->database()->isDatabaseLoading())
    return;

  if ( pDep->isActionEvaluationInProgress() )
    return;

  if (!m_dependencyBodyId.isNull()) {
    OdDbAssocDependencyBodyPtr body = OdDbAssocDependencyBody::cast(m_dependencyBodyId.openObject());
    body->modifiedOverride(dbObj);
    return;
  }

  bool isDependentOnObjectFound = dbObj->objectId() == m_dependentOnObjectId;

  if (!isDependentOnObjectFound)
  {
    OdDbObjectIdArray path;
    m_refPath.getFullPath(path);
    int len = path.length();
    for (int i = 0; i < len; i++)
    {
      OdDbObjectId tempObjId = path.getAt(i);

      if (dbObj->objectId() == tempObjId)
      {
          isDependentOnObjectFound = true;
          break;
      }
    }
  }

  if (m_nStatus == kErasedAssocStatus || !isDependentOnObjectFound)
    return;
  try 
  {
    if ( isRelevantChange(pDep) )
    {
      setStatus(kChangedDirectlyAssocStatus, true, pDep); 
    }
  }
  catch (const OdError&) 
  {
    erased(dbObj, pDep);
  }
}

void OdDbImpAssocDependency::copied(const OdDbObject* pDbObj, const OdDbObject* pNewObj, OdDbAssocDependency *pDep)
{
  OdDbAssocDependencyBodyPtr body = OdDbAssocDependencyBody::cast(m_dependencyBodyId.openObject(OdDb::kForWrite));
  if (!body.isNull())
    body->clonedOverride(pDbObj, pNewObj);
  OdDbAssocActionPtr actionPtr = pDep->owningAction().openObject(OdDb::kForWrite);
  if (!actionPtr.isNull())
    actionPtr->dependentObjectCloned(pDep, pDbObj, pNewObj);
  //!TMP
  if (pNewObj && pDep) {
    OdDbObjectPtr newObj = pNewObj;
    newObj->upgradeOpen();
    // do not keep reactor as constraints copy functionality is not implemented yet
    newObj->removePersistentReactor(pDep->objectId());
  }
}

OdResult OdDbImpAssocDependency::getDependentOnCompoundObject(OdDbCompoundObjectIdPtr& res) const
{
  if (res.isNull())
    res = OdDbCompoundObjectId::createObject();
  if ( isDependentOnCompoundObject() )
    *res = m_refPath;
  else
  {
    res->setEmpty();
    res->set(dependentOnObject());
  }

  return eOk;
}

OdStringArray OdDbImpAssocDependency::compareWith(const OdDbImpAssocDependency *depIDToCompare, OdDb::DwgVersion filerVersion) const
{ 
	OdStringArray retArr;

	if (m_nStatus != depIDToCompare->m_nStatus)
		retArr.append(CS_UNDEFINED_ANNOTATION_ACTIONBODY);

	if (m_bReadDep != depIDToCompare->m_bReadDep)
		retArr.append(CS_READ_DEP);

	if (m_bWriteDep != depIDToCompare->m_bWriteDep)
		retArr.append(CS_WRITE_DEP);

	if (m_bObjectStateDependent != depIDToCompare->m_bObjectStateDependent)
		retArr.append(CS_OBJECT_STATE_DEPENDENT);

	if (m_bDelegatingToOwningAction != depIDToCompare->m_bDelegatingToOwningAction)
		retArr.append(CS_DELEGATING_TO_OWNING_ACTION);

	if (m_nOrder != depIDToCompare->m_nOrder)
		retArr.append(CS_ORDER);

	if ((!m_dependentOnObjectId.isNull() && depIDToCompare->m_dependentOnObjectId.isNull()) ||
		(m_dependentOnObjectId.isNull() && !depIDToCompare->m_dependentOnObjectId.isNull()))
	{
		retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (!m_dependentOnObjectId.isNull() && !depIDToCompare->m_dependentOnObjectId.isNull())
	{
		if (typeid(*(m_dependentOnObjectId).openObject()) != typeid(*(depIDToCompare->m_dependentOnObjectId.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}
	
	//m_refPath.dxfOutFields(pFiler, 0);
	if ((!m_prevDependencyOnObjectID.isNull() && depIDToCompare->m_prevDependencyOnObjectID.isNull()) ||
		(m_prevDependencyOnObjectID.isNull() && !depIDToCompare->m_prevDependencyOnObjectID.isNull()))
	{
		retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (!m_prevDependencyOnObjectID.isNull() && !depIDToCompare->m_prevDependencyOnObjectID.isNull())
	{
		if (typeid(*(m_prevDependencyOnObjectID).openObject()) != typeid(*(depIDToCompare->m_prevDependencyOnObjectID.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if ((!m_nextDependencyOnObjectID.isNull() && depIDToCompare->m_nextDependencyOnObjectID.isNull()) ||
		 (m_nextDependencyOnObjectID.isNull() && !depIDToCompare->m_nextDependencyOnObjectID.isNull()))
	{
		retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (!m_nextDependencyOnObjectID.isNull() && !depIDToCompare->m_nextDependencyOnObjectID.isNull())
	{
		if (typeid(*(m_nextDependencyOnObjectID).openObject()) != typeid(*(depIDToCompare->m_nextDependencyOnObjectID.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}
	
	if ((!m_dependencyBodyId.isNull() && !depIDToCompare->m_dependencyBodyId.isNull()) ||
		(!m_dependencyBodyId.isNull() && !depIDToCompare->m_dependencyBodyId.isNull()))
	{
		retArr.append(CS_DIFFERENT_OBJECTS);
	}
	if (!m_dependencyBodyId.isNull() && !depIDToCompare->m_dependencyBodyId.isNull())
	{
		if (typeid(*(m_dependencyBodyId).openObject()) != typeid(*(depIDToCompare->m_dependencyBodyId.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (m_nIndex != depIDToCompare->m_nIndex)
		retArr.append(CS_INDEX);

	return retArr;
}
