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
#include "DbAssocActionBody.h"
#include "DbImpAssocAction.h"

//begin proof of concept specific
#include "DbAssocNetwork.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocDependency.h"
#include "DbAssocGeomDependency.h"
#include "AssocActionPE.h"
//end proof of concept specific

#include "DbFiler.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocAction,     // ClassName
                      OdDbObject,           // ParentClass
                      DBOBJECT_CONSTR,      // DOCREATE
                      OdDb::kDHL_1021,      // DwgVer
                      OdDb::kMRelease6,     // MaintVer
                      1025,                 // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                      L"AcDbAssocAction",   // DWG class name
                      L"ACDBASSOCACTION",   // DxfName
                      L"ObjectDBX Classes", // AppName
                      OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocAction::OdDbAssocAction(bool createImpObj) 
  : m_pImpObj(NULL)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocAction();
}

OdDbAssocAction::~OdDbAssocAction()
{
  delete m_pImpObj;
}

OdDbObjectId OdDbAssocAction::actionBody() const
{
  assertReadEnabled();
  return m_pImpObj->actionBody();
}
    
OdResult OdDbAssocAction::setActionBody(const OdDbObjectId& actionBodyId)
{
  assertWriteEnabled();
  
  OdDbObjectId id = objectId();
  return m_pImpObj->setActionBody(id, actionBodyId);
}

OdDbAssocStatus OdDbAssocAction::status() const
{
  assertReadEnabled();
  return m_pImpObj->status();
}


OdResult OdDbAssocAction::setStatus(OdDbAssocStatus newStatus,
                                    bool notifyOwningNetwork,
                                    bool setInOwnedActions)
{

  assertWriteEnabled();
  
  return m_pImpObj->setStatus(newStatus, notifyOwningNetwork, setInOwnedActions);
}


OdDbObjectId OdDbAssocAction::owningNetwork() const
{
  assertReadEnabled();
  return m_pImpObj->owningNetwork();
}

OdResult OdDbAssocAction::setOwningNetwork(const OdDbObjectId& networkId, bool alsoSetAsDatabaseOwner)
{
  assertWriteEnabled();
  return m_pImpObj->setOwningNetwork(this, networkId, alsoSetAsDatabaseOwner);
}
  
OdResult OdDbAssocAction::getDependencies(bool readDependenciesWanted,
                                          bool writeDependenciesWanted,
                                          OdDbObjectIdArray& dependencyIds) const
{
  assertReadEnabled();
  return m_pImpObj->getDependencies(readDependenciesWanted, writeDependenciesWanted, dependencyIds);
}
OdResult OdDbAssocAction::addDependency(const OdDbObjectId& dependencyId, 
                       bool setThisActionAsOwningAction)
{
  assertWriteEnabled();
  return m_pImpObj->addDependency(dependencyId, setThisActionAsOwningAction, this);
}

OdResult OdDbAssocAction::removeDependency(const OdDbObjectId& dependencyId, 
                          bool alsoEraseIt)
{
  assertWriteEnabled();
  return m_pImpObj->removeDependency(dependencyId, alsoEraseIt);
}

OdResult OdDbAssocAction::removeAllDependencies(bool alsoEraseThem)
{
  assertWriteEnabled();
  return m_pImpObj->removeAllDependencies(alsoEraseThem);
}

OdResult OdDbAssocAction::getDependentObjects(bool readDependenciesWanted,
                                              bool writeDependenciesWanted,
                                              OdDbObjectIdArray& objectIds) const
{
  assertReadEnabled();
  return m_pImpObj->getDependentObjects(readDependenciesWanted, writeDependenciesWanted, objectIds);
}
/*
    bool OdDbAssocAction::isOwnedDependency(const OdDbAssocDependency* pDependency) const;

    bool OdDbAssocAction::isExternalDependency(const OdDbAssocDependency* pDependency) const;
*/

bool OdDbAssocAction::isRelevantDependencyChange(const OdDbAssocDependency* pDependency) const
{
  assertReadEnabled();

  return m_pImpObj->isRelevantDependencyChange(pDependency);
}
/*
    bool OdDbAssocAction::hasDependencyCachedValue(const OdDbAssocDependency* pDependency) const;

    bool OdDbAssocAction::areDependenciesOnTheSameThing(const OdDbAssocDependency* pDependency1, 
                                       const OdDbAssocDependency* pDependency2) const;

    bool OdDbAssocAction::areDependenciesEqual(const OdDbAssocDependency* pDependency1, 
                              const OdDbAssocDependency* pDependency2) const;

    OdResult OdDbAssocAction::getActionsDependentOnObject(const OdDbObject* pObject,
                                                bool readDependenciesWanted,
                                                bool writeDependenciesWanted,                                   
                                                OdDbObjectIdArray& actionIds);
*/
void OdDbAssocAction::dependentObjectCloned(const OdDbAssocDependency* pDependency, 
                           const OdDbObject*          pDbObj, 
                           const OdDbObject*          pNewObj)
{
  assertReadEnabled();

  m_pImpObj->dependentObjectCloned(pDependency, pDbObj, pNewObj);
}

OdResult OdDbAssocAction::addMoreObjectsToDeepClone(OdDbIdMapping& idMap, 
                                       OdDbObjectIdArray& additionalObjectsToClone) const
{
  assertReadEnabled();
  return m_pImpObj->addMoreObjectsToDeepClone( idMap, additionalObjectsToClone );
}

OdResult OdDbAssocAction::postProcessAfterDeepClone(OdDbIdMapping& idMap)
{
  assertReadEnabled();
  return m_pImpObj->postProcessAfterDeepClone(this, idMap);
}

OdResult OdDbAssocAction::postProcessAfterDeepCloneCancel(OdDbIdMapping& idMap)
{
  assertReadEnabled();
  return m_pImpObj->postProcessAfterDeepCloneCancel(this, idMap);
}

bool OdDbAssocAction::isActionEvaluationInProgress() const
{
  assertReadEnabled();

  return m_pImpObj->isActionEvaluationInProgress();
}

OdDbAssocEvaluationCallback* OdDbAssocAction::currentEvaluationCallback() const
{
  assertReadEnabled();

  return m_pImpObj->currentEvaluationCallback();
}

OdResult OdDbAssocAction::evaluateDependencies()
{
  assertReadEnabled();

  return m_pImpObj->evaluateDependencies();
}

void OdDbAssocAction::evaluateDependency(OdDbAssocDependency* pDependency)
{
  assertWriteEnabled();
  m_pImpObj->evaluateDependency(pDependency);
}

OdResult OdDbAssocAction::ownedDependencyStatusChanged(OdDbAssocDependency* pOwnedDependency, 
                                      OdDbAssocStatus      previousStatus)
{
  assertReadEnabled();
  return m_pImpObj->ownedDependencyStatusChanged(pOwnedDependency, previousStatus);
}
/*
    OdResult OdDbAssocAction::transformActionBy(const OdGeMatrix3d& transform);

    bool OdDbAssocAction::isEqualTo(const OdDbAssocAction* pOtherAction) const;

    OdDbAssocEvaluationPriority OdDbAssocAction::evaluationPriority() const;
*/
void OdDbAssocAction::getDependentActionsToEvaluate(OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const
{
  assertReadEnabled();
  m_pImpObj->getDependentActionsToEvaluate(pActionsToEvaluateCallback);
}

void OdDbAssocAction::evaluate(OdDbAssocEvaluationCallback* pEvaluationCallback)
{
  assertWriteEnabled();

  m_pImpObj->evaluate(this, pEvaluationCallback);
}

OdDbObjectId OdDbAssocAction::objectThatOwnsNetworkInstance() const
{
  assertReadEnabled();
  return m_pImpObj->objectThatOwnsNetworkInstance(this);
}

//void OdDbAssocAction::dragStatus(const OdDb::DragStat status);

OdResult OdDbAssocAction::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  return m_pImpObj->dwgInFields(pFiler, objectId());
}

void OdDbAssocAction::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  m_pImpObj->dwgOutFields(pFiler, objectId());
}

OdResult OdDbAssocAction::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  return m_pImpObj->dxfInFields(pFiler, objectId());
}

void OdDbAssocAction::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  m_pImpObj->dxfOutFields(pFiler, objectId());
}

OdStringArray OdDbAssocAction::compareWith(const OdDbObjectId actionIDToCompare, OdDb::DwgVersion filerVersion) const
{
	OdStringArray retArray;
	OdDbAssocActionPtr comparedActionPtr = OdDbAssocAction::cast(actionIDToCompare.openObject());

	if (comparedActionPtr.isNull())
	{
		retArray.append(CS_OBJECT_ERROR);
		return retArray;
	}

	OdDbImpAssocAction* pImpObjCompared = static_cast<OdDbImpAssocAction *>(comparedActionPtr->m_pImpObj);

	return static_cast<OdDbImpAssocAction*>
		(m_pImpObj)->compareWith(pImpObjCompared);
}

void OdDbAssocAction::composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  OdDbObject::composeForLoad(format, version, pAuditInfo );
  m_pImpObj->composeForLoad( this, format, version, pAuditInfo );
}

OdDbObjectPtr OdDbAssocAction::decomposeForSave(OdDb::DwgVersion ver, OdDbObjectId& replaceId, bool& exchangeXData)
{
  replaceId = NULL;
  exchangeXData = false;
  m_pImpObj->decomposeForSave(this, OdDb::kUnknown, ver);
  return (OdDbObject*)NULL;
}

void OdDbAssocAction:: audit(OdDbAuditInfo* pAuditInfo)
{
  OdDbObject::audit(pAuditInfo);
  m_pImpObj->audit(this, pAuditInfo);
}

// AC2013
// OdDbAssocActionParams stuff
OdResult OdDbAssocAction::removeAllParams(bool bEraseThem)
{
  assertWriteEnabled();
  return m_pImpObj->removeAllParams(bEraseThem);
}


int OdDbAssocAction::paramCount() const
{
  assertReadEnabled();
  return m_pImpObj->paramCount();
}


const OdDbObjectIdArray& OdDbAssocAction::ownedParams() const
{
  assertReadEnabled();
  return m_pImpObj->ownedParams();
}


OdResult OdDbAssocAction::addParam(const OdDbObjectId& paramId, int& paramIndex)
{
  assertWriteEnabled();
  return m_pImpObj->addParam(paramId, paramIndex);
}


OdResult OdDbAssocAction::removeParam(const OdDbObjectId& paramId, bool alsoEraseIt)
{
  assertWriteEnabled();
  return m_pImpObj->removeParam(paramId, alsoEraseIt);
}

OdDbObjectId OdDbAssocAction::paramAtIndex(int paramIndex) const
{
  assertReadEnabled();
  return m_pImpObj->paramAtIndex(paramIndex);
}

OdResult OdDbAssocAction::addParam(const OdString& paramName, 
                                   OdRxClass* pParamClass, 
                                   OdDbObjectId& paramId, 
                                   int& paramIndex)
{
  assertWriteEnabled();
  return m_pImpObj->addParam(paramName, pParamClass, paramId, paramIndex, this );
}

OdDbObjectIdArray OdDbAssocAction::paramsAtName(const OdString& paramName) const
{
  assertReadEnabled();
  return m_pImpObj->paramsAtName(paramName);
}

OdDbObjectId OdDbAssocAction::paramAtName(const OdString& paramName, 
                                          int index) const // = 0
{
  assertReadEnabled();
  return m_pImpObj->paramAtName(paramName, index);
}

void OdDbAssocAction::ownedValueParamNames(OdStringArray& paramNames) const
{
  assertReadEnabled();
  m_pImpObj->ownedValueParamNames(paramNames);
}

OdResult OdDbAssocAction::getValueParamArray(const OdString& paramName,
                                             OdArray<OdDbEvalVariantPtr>& values,
                                             OdStringArray& expressions,
                                             OdStringArray& evaluatorIds) const
{
  assertReadEnabled();
  return m_pImpObj->getValueParamArray(paramName, values, expressions, evaluatorIds);
}

OdResult OdDbAssocAction::getValueParam(const OdString& paramName,
                                        OdDbEvalVariant& value,
                                        OdString& expression,
                                        OdString& evaluatorId,
                                        int valueIndex) const // = 0
{
  assertReadEnabled();
  return m_pImpObj->getValueParam(paramName, value, expression, evaluatorId, valueIndex);
}

OdResult OdDbAssocAction::setValueParamArray(const OdString& paramName,
                                             const OdArray<OdDbEvalVariantPtr>& values,
                                             const OdStringArray& expressions,
                                             const OdStringArray& evaluatorIds,
                                             OdStringArray& errorMessages,
                                             bool silentMode)
{
  assertWriteEnabled();
  return m_pImpObj->setValueParamArray(paramName, values, expressions, evaluatorIds, 
                                       errorMessages, silentMode);
}

OdResult OdDbAssocAction::setValueParam(const OdString& paramName,
                                        const OdDbEvalVariant& value,
                                        const OdString& expression,
                                        const OdString& evaluatorId,
                                        OdString& errorMessage,
                                        bool silentMode,
                                        int valueIndex) // = 0
{
  assertWriteEnabled();
  return m_pImpObj->setValueParam(paramName, value, expression, evaluatorId,
                                  errorMessage, silentMode, valueIndex);
}

OdValue::UnitType OdDbAssocAction::valueParamUnitType(const OdString& paramName) const
{
  assertReadEnabled();
  return m_pImpObj->valueParamUnitType(paramName);
}

OdResult OdDbAssocAction::setValueParamUnitType(const OdString& paramName, 
                                                OdValue::UnitType unitType)
{
  assertWriteEnabled();
  return m_pImpObj->setValueParamUnitType(paramName, unitType);
}

OdResult OdDbAssocAction::removeValueParam(const OdString& paramName)
{
  assertWriteEnabled();
  return m_pImpObj->removeValueParam(paramName);
}

OdResult OdDbAssocAction::valueParamInputVariables(const OdString& paramName, 
                                                   OdDbObjectIdArray& variableIds) const
{
  assertReadEnabled();
  return m_pImpObj->valueParamInputVariables(paramName, variableIds);
}

OdResult OdDbAssocAction::setValueParamControlledObjectDep(const OdString& paramName, 
                                                           const OdDbObjectId& controlledObjectDepId)
{
  assertWriteEnabled();
  return m_pImpObj->setValueParamControlledObjectDep(paramName, controlledObjectDepId);
}

OdResult OdDbAssocAction::updateValueParamControlledObject(const OdString& paramName) const
{
  //sassertWriteEnabled();
  return m_pImpObj->updateValueParamControlledObject(paramName);
}

OdResult OdDbAssocAction::updateValueParamFromControlledObject(const OdString& paramName)
{
  assertWriteEnabled();
  return m_pImpObj->updateValueParamFromControlledObject(paramName);
}

OdResult OdDbAssocAction::updateAllObjectsControlledByValueParams() const
{
  //assertWriteEnabled();
  return m_pImpObj->updateAllObjectsControlledByValueParams();
}

OdResult OdDbAssocAction::transformAllConstantGeometryParams(const OdGeMatrix3d& transform)
{
  assertWriteEnabled();
  return m_pImpObj->transformAllConstantGeometryParams(transform);
}

OdResult OdDbAssocAction::scaleAllDistanceValueParams(double scaleFactor)
{
  assertWriteEnabled();
  return m_pImpObj->scaleAllDistanceValueParams(scaleFactor);
}
