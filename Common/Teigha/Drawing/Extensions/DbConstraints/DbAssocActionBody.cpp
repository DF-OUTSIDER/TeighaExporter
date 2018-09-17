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
#include "DbImpAssocActionBody.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocActionBody, OdDbObject, L"AcDbAssocActionBody");

OdDbAssocActionBody::OdDbAssocActionBody()
  : OdDbObject()
  , m_pImpObj(NULL)
{
}

OdDbAssocActionBody::~OdDbAssocActionBody()
{
  if (m_pImpObj)
  {
    delete m_pImpObj;
    m_pImpObj = NULL;
  }
}

OdDbAssocStatus OdDbAssocActionBody::status() const
{
  assertReadEnabled();

  OdDbAssocActionPtr pAction = parentAction().openObject();
  return pAction->status();
}

OdResult OdDbAssocActionBody::setStatus(OdDbAssocStatus newStatus, 
                                        bool notifyOwningNetwork,
                                        bool setInOwnedActions)
{
  assertWriteEnabled();
  OdDbAssocActionPtr pAction = parentAction().openObject(OdDb::kForWrite);
  return pAction ->setStatus(newStatus, notifyOwningNetwork, setInOwnedActions);
}

OdDbObjectId OdDbAssocActionBody::owningNetwork() const
{
  assertReadEnabled();
  OdDbAssocActionPtr pAction = parentAction().openObject();
  return pAction->owningNetwork();
}

OdResult OdDbAssocActionBody::getDependencies(bool readDependenciesWanted, 
                                              bool writeDependenciesWanted,
                                              OdDbObjectIdArray& dependencyIds) const
{
  assertReadEnabled();
  OdDbAssocActionPtr pAction = parentAction().openObject();
  return pAction->getDependencies(readDependenciesWanted, writeDependenciesWanted, dependencyIds);
}

OdResult OdDbAssocActionBody::addDependency(const OdDbObjectId& dependencyId, 
                                            bool setThisActionAsOwningAction)
{
  assertWriteEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForWrite );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }

  return pAction->addDependency( dependencyId, setThisActionAsOwningAction );
}

OdResult OdDbAssocActionBody::removeDependency(const OdDbObjectId& dependencyId, 
                                               bool alsoEraseIt)
{
  assertWriteEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForWrite );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }

  return pAction->removeDependency( dependencyId, alsoEraseIt );
}

OdResult OdDbAssocActionBody::removeAllDependencies(bool alsoEraseThem)
{
  assertWriteEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForWrite );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }

  return pAction->removeAllDependencies( alsoEraseThem );
}

OdResult OdDbAssocActionBody::evaluateDependencies()
{
  assertWriteEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForWrite );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }

  return pAction->evaluateDependencies();
}

bool OdDbAssocActionBody::isActionEvaluationInProgress() const
{
  assertReadEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForRead );
  return pAction->isActionEvaluationInProgress();
}

OdDbAssocEvaluationCallback* OdDbAssocActionBody::currentEvaluationCallback() const
{
  assertReadEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForWrite );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return NULL;
  }

  return pAction->currentEvaluationCallback();
}

OdResult OdDbAssocActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;
  
  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  return m_pImpObj->dwgInFields(pFiler);
}

void OdDbAssocActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    throw OdError(eNotImplementedYet);

  OdDbObject::dwgOutFields(pFiler);
  m_pImpObj->dwgOutFields(pFiler);
}

OdResult OdDbAssocActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  return m_pImpObj->dxfInFields(pFiler);
}

void OdDbAssocActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    throw OdError(eNotImplementedYet);

  OdDbObject::dxfOutFields(pFiler);
  m_pImpObj->dxfOutFields(pFiler);
}

OdResult OdDbAssocActionBody::setValueParam( const OdString& paramName, const OdDbEvalVariant& value, const OdString& expression, const OdString& evaluatorId, OdString& errorMessage, bool silentMode, int valueIndex )
{
  assertWriteEnabled();
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForWrite );
  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }
  return pAction->setValueParam( paramName, value, expression, evaluatorId, errorMessage, silentMode, valueIndex);
}

OdResult OdDbAssocActionBody::getValueParam( const OdString& paramName, OdDbEvalVariant& value, OdString& expression, OdString& evaluatorId, int valueIndex ) const
{
  assertReadEnabled();
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForRead );
  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }

  return pAction->getValueParam(paramName, value, expression, evaluatorId, valueIndex);
}

OdDbObjectId OdDbAssocActionBody::paramAtName( const OdString& paramName, int index /*= 0*/ ) const
{
  assertReadEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForRead );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return OdDbObjectId();
  }

  return pAction->paramAtName( paramName, index );
}

OdValue::UnitType OdDbAssocActionBody::valueParamUnitType( const OdString& paramName ) const
{
  assertReadEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForRead );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return OdValue::kUnitless;
  }

  return pAction->valueParamUnitType( paramName );
}

OdResult OdDbAssocActionBody::setValueParamUnitType( const OdString& paramName, OdValue::UnitType unitType )
{
  assertWriteEnabled();
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForWrite );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }

  return pAction->setValueParamUnitType( paramName, unitType );
}

OdResult OdDbAssocActionBody::addParam( const OdString& paramName, OdRxClass* pParamClass, OdDbObjectId& paramId, int& paramIndex )
{
  assertWriteEnabled(); 
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForWrite );

  if (pAction.isNull())
  {
    ODA_FAIL_ONCE();
    return eNullPtr;
  }

  return pAction->addParam( paramName, pParamClass, paramId, paramIndex );
}

void OdDbAssocActionBody::ownedValueParamNames( OdStringArray& paramNames ) const
{
  assertReadEnabled();
  OdDbAssocActionPtr pAction = parentAction().openObject( OdDb::kForRead );

  pAction->ownedValueParamNames( paramNames );
}

OdResult OdDbAssocActionBody::createActionAndActionBodyAndPostToDatabase( OdRxClass* pActionBodyClass, const OdDbObjectId& objectId, OdDbObjectId& createdActionId, OdDbObjectId& createdActionBodyId )
{
  return OdDbImpAssocActionBody::createActionAndActionBodyAndPostToDatabase(pActionBodyClass, objectId, createdActionId, createdActionBodyId);
}

OdStringArray OdDbAssocActionBody::compareWith(const OdDbObjectId bodyIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
{
	OdStringArray retArray;
	OdDbAssocActionBodyPtr comparedBodyPtr = OdDbAssocActionBody::cast(bodyIDToCompare.openObject());

	if (comparedBodyPtr.isNull())
	{
		retArray.append(CS_OBJECT_ERROR);
		return retArray;
	}

	OdDbImpAssocActionBody* pImpObjCompared = static_cast<OdDbImpAssocActionBody *>(comparedBodyPtr->m_pImpObj);

	return static_cast<OdDbImpAssocActionBody *>
		(m_pImpObj)->compareWith(pImpObjCompared);
}
