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
#include "DbAssocDependency.h"
#include "DbImpAssocDependency.h"
#include "DbFiler.h"
#include "DbAssocDependencyBody.h"
#include "DbAssocNetwork.h"

////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocDependency,    // ClassName
                       OdDbObject,             // ParentClass
                       DBOBJECT_CONSTR,        // DOCREATE
                       OdDb::kDHL_1021,        // DwgVer
                       OdDb::kMRelease6,       // MaintVer
                       1025,                   // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocDependency", // DWG class name
                       L"ACDBASSOCDEPENDENCY", // DxfName
                       L"ObjectDBX Classes",   // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocDependency::OdDbAssocDependency(bool createImp) : OdDbObject()
{
  if ( createImp )
    m_pImpObj = new OdDbImpAssocDependency();
  else
    m_pImpObj = NULL;
}

OdDbAssocDependency::~OdDbAssocDependency()
{
  delete m_pImpObj;
}


OdResult OdDbAssocDependency::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbObject::dwgInFields(pFiler);
  if ( res != eOk)
    return res;

  return m_pImpObj->dwgInFields(pFiler);
}



void OdDbAssocDependency::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  m_pImpObj->dwgOutFields(pFiler);
}

OdResult OdDbAssocDependency::dxfInFields(OdDbDxfFiler* pFiler)
{
	OdResult res;
	if( eOk != (res = OdDbObject::dxfInFields( pFiler )) )
    return res;

  return m_pImpObj->dxfInFields(pFiler);
}

void OdDbAssocDependency::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  m_pImpObj->dxfOutFields(pFiler);
}

OdDbObjectId OdDbAssocDependency::dependencyBody() const
{
  assertReadEnabled();
  return m_pImpObj->dependencyBody();
}
    
OdResult OdDbAssocDependency::setDependencyBody(OdDbObjectId dependencyBodyId)
{
  assertWriteEnabled();
  return m_pImpObj->setDependencyBody(dependencyBodyId, objectId());
}

OdDbAssocStatus OdDbAssocDependency::status() const
{
  assertReadEnabled();
  return m_pImpObj->status();
}

OdResult OdDbAssocDependency::setStatus(OdDbAssocStatus newStatus, bool notifyOwningAction)
{
  assertWriteEnabled();
  
  return m_pImpObj->setStatus(newStatus, notifyOwningAction, this);
}

bool OdDbAssocDependency::isReadDependency() const
{
  assertReadEnabled();
  return m_pImpObj->isReadDependency();
}

bool OdDbAssocDependency::isWriteDependency() const
{
  assertReadEnabled();
  return m_pImpObj->isWriteDependency();
}

void OdDbAssocDependency::setIsReadDependency(bool bRead)
{
  assertWriteEnabled();
  m_pImpObj->setIsReadDependency(bRead);
}

void OdDbAssocDependency::setIsWriteDependency(bool bWrite)
{
  assertWriteEnabled();
  m_pImpObj->setIsWriteDependency(bWrite);
}

bool OdDbAssocDependency::isObjectStateDependent() const
{
  assertReadEnabled();
  return m_pImpObj->isObjectStateDependent();
}

void OdDbAssocDependency::setIsObjectStateDependent(bool bDependent)
{
  assertWriteEnabled();
  m_pImpObj->setIsObjectStateDependent(bDependent);
}

int OdDbAssocDependency::order() const
{
  assertReadEnabled();
  return m_pImpObj->order();
}

void OdDbAssocDependency::setOrder(int newOrder)
{
  assertWriteEnabled();
  m_pImpObj->setOrder(newOrder);
}


OdResult OdDbAssocDependency::setOwningAction(OdDbObjectId actionId)
{
  assertWriteEnabled();
  return m_pImpObj->setOwningAction(actionId, this);
}


OdDbObjectId OdDbAssocDependency::dependentOnObject() const
{
  assertReadEnabled();
  return m_pImpObj->dependentOnObject();
}

bool OdDbAssocDependency::isDependentOnCompoundObject() const
{
  assertReadEnabled();
  return m_pImpObj->isDependentOnCompoundObject();
}

OdDbObjectId OdDbAssocDependency::prevDependencyOnObject() const
{
  assertReadEnabled();
  return m_pImpObj->prevDependencyOnObject();
}

 
OdDbObjectId OdDbAssocDependency::nextDependencyOnObject() const
{
  assertReadEnabled();
  return m_pImpObj->nextDependencyOnObject();
}

OdResult OdDbAssocDependency::attachToObject(const OdDbCompoundObjectId* compoundId)
{
  assertWriteEnabled();
  return m_pImpObj->attachToObject(compoundId, this);
}

OdResult OdDbAssocDependency::dependentOnObjectStatus() const
{
  assertReadEnabled();
  return m_pImpObj->dependentOnObjectStatus();
}

OdResult OdDbAssocDependency::detachFromObject()
{
  assertWriteEnabled();
  return m_pImpObj->detachFromObject(this);
}

OdResult OdDbAssocDependency::updateDependentOnObject()
{
  assertReadEnabled();
  return m_pImpObj->updateDependentOnObject(this);
}

void OdDbAssocDependency::setDependentOnObject(OdDbCompoundObjectId* compoundId)
{
  assertWriteEnabled();
  m_pImpObj->setDependentOnObject(compoundId, this);
}

OdDbObjectId OdDbAssocDependency::getFirstDependencyOnObject(OdDbObjectPtr pObject)
{
  if ( pObject.isNull() )
    return OdDbObjectId::kNull;

  OdDbObjectIdArray arrPersistentReactors = pObject->getPersistentReactors();
  OdDbObjectPtr pObj;
  OdDbAssocDependencyPtr pDep;
  for ( unsigned int i = 0; i < arrPersistentReactors.length(); i++ )
  {
    pObj = arrPersistentReactors[i].openObject();
    if ( !pObj.isNull() && pObj->isKindOf(OdDbAssocDependency::desc()) )
    {
      pDep = OdDbAssocDependency::cast(pObj);
      if ( pDep->prevDependencyOnObject() == OdDbObjectId::kNull )
        return arrPersistentReactors[i];
    }
  }

  return OdDbObjectId::kNull;
}

void OdDbAssocDependency::getDependenciesOnObject(OdDbObjectPtr pObject, 
                                        bool readDependenciesWanted, 
                                        bool writeDependenciesWanted,
                                        OdDbObjectIdArray& dependencyIds)
{
  OdDbObjectId depId = OdDbAssocDependency::getFirstDependencyOnObject(pObject);

  if ( depId == OdDbObjectId::kNull )
    return;

  bool add;
  OdDbAssocDependencyPtr pDep;
  do
  {
    add = true;

    pDep = depId.openObject();
    if ( readDependenciesWanted )
      add = pDep->isReadDependency();
    
    if ( writeDependenciesWanted )
      add &= pDep->isWriteDependency();

    if ( add )
      dependencyIds.push_back(depId);
  } 
  while( (depId = pDep->nextDependencyOnObject()) != OdDbObjectId::kNull );
}

OdResult OdDbAssocDependency::notifyDependenciesOnObject(OdDbObjectPtr pObject, OdDbAssocStatus newStatus)
{
  return eNotImplementedYet;
}

bool OdDbAssocDependency::isDelegatingToOwningAction() const
{
  assertReadEnabled();
  return m_pImpObj->isDelegatingToOwningAction();
}


void OdDbAssocDependency::setIsDelegatingToOwningAction(bool bDelegating)
{
  assertWriteEnabled();
  m_pImpObj->setIsDelegatingToOwningAction(bDelegating);
}

/*
bool OdDbAssocDependency::hasCachedValue() const;
*/
bool OdDbAssocDependency::isRelevantChange() const
{
  assertReadEnabled();

  return m_pImpObj->isRelevantChange(this);
}
/*
    bool OdDbAssocDependency::isDependentOnTheSameThingAs(const OdDbAssocDependency* pOtherDependency) const;

    bool OdDbAssocDependency::isDependentOnObjectReadOnly() const;

    bool OdDbAssocDependency::isEqualTo(const OdDbAssocDependency* pOtherDependency) const;
*/
bool OdDbAssocDependency::isActionEvaluationInProgress() const
{
  assertReadEnabled();

  return m_pImpObj->isActionEvaluationInProgress( this );
}

OdDbAssocEvaluationCallback* OdDbAssocDependency::currentEvaluationCallback() const
{
  assertReadEnabled();
  return m_pImpObj->currentEvaluationCallback();
}

void OdDbAssocDependency::evaluate()
{
  assertWriteEnabled();
  m_pImpObj->evaluate();
}


void OdDbAssocDependency::erased(const OdDbObject* dbObj, bool isErasing)
{
  assertNotifyEnabled();
  if (!dbObj->isUndoing())
  {
    ODA_ASSERT(isWriteEnabled());
    assertWriteEnabled();
    m_pImpObj->erased(dbObj, this, isErasing);
  }
}

void OdDbAssocDependency::modified(const OdDbObject* dbObj)
{
  assertNotifyEnabled();
  if (!dbObj->isUndoing())
  {
    ODA_ASSERT(isWriteEnabled());
    assertWriteEnabled();
    m_pImpObj->modified(dbObj, this);
  }
}

void OdDbAssocDependency::copied(const OdDbObject* pDbObj, const OdDbObject* pNewObj)
{
  assertNotifyEnabled();
  m_pImpObj->copied(pDbObj, pNewObj, this);
}

OdResult OdDbAssocDependency::getDependentOnCompoundObject(OdDbCompoundObjectIdPtr& res) const
{
  assertReadEnabled();

  return m_pImpObj->getDependentOnCompoundObject(res);
}

OdStringArray OdDbAssocDependency::compareWith(const OdDbObjectId depIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
{
	OdStringArray retArray;
	OdDbAssocDependencyPtr comparedDepPtr = OdDbAssocDependency::cast(depIDToCompare.openObject());

	if (comparedDepPtr.isNull())
	{
		retArray.append(CS_OBJECT_ERROR);
		return retArray;
	}

	OdDbImpAssocDependency* pImpObjCompared = static_cast<OdDbImpAssocDependency *>(comparedDepPtr->m_pImpObj);

	return static_cast<OdDbImpAssocDependency *>
		(m_pImpObj)->compareWith(pImpObjCompared);
}
