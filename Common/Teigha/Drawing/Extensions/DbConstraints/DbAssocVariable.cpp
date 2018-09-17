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
#include "DbAssocVariable.h"
#include "DbImpAssocVariable.h"
#include "DbAssocNetwork.h"

#include "DbFiler.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocVariable,    // ClassName
                       OdDbAssocAction,      // ParentClass
                       DBOBJECT_CONSTR,      // DOCREATE
                       OdDb::kDHL_1021,      // DwgVer
                       OdDb::kMRelease6,     // MaintVer
                       1025,                 // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocVariable", // DWG class name
                       L"ACDBASSOCVARIABLE", // DxfName
                       L"ObjectDBX Classes", // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocVariableValueProviderPE, OdRxObject, L"AcDbAssocVariableValueProviderPE");

OdDbAssocVariable::OdDbAssocVariable():OdDbAssocAction(false)
{
  m_pImpObj = new OdDbImpAssocVariable();
}

const OdString OdDbAssocVariable::name() const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->name();
}

const OdString OdDbAssocVariable::expression() const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->expression();
}

const OdDbEvalVariantPtr OdDbAssocVariable::value() const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->value();
}

const OdString OdDbAssocVariable::description() const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->description();
}

OdResult OdDbAssocVariable::setName(const OdString newName, bool updateReferencingExpressions)
{
  assertWriteEnabled();

  return ((OdDbImpAssocVariable*)m_pImpObj)->setName(this, newName, updateReferencingExpressions);
}

OdDbObjectId OdDbAssocVariable::findObjectByName(const OdString  objectName, 
                              const OdRxClass* pObjectClass) const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->findObjectByName(objectName, pObjectClass);
}

OdResult OdDbAssocVariable::validateNameAndExpression(const OdString nameToValidate, 
                                                      const OdString expressionToValidate,
                                                      OdString       errorMessage) const
{
  assertReadEnabled();
  OdDbImpAssocVariable* impObj = (OdDbImpAssocVariable*)m_pImpObj;
  if ( OdDbImpAssocVariable::m_assocVariableCallback != NULL )
    return OdDbImpAssocVariable::m_assocVariableCallback->validateNameAndExpression(this,
                                                                             nameToValidate, 
                                                                             expressionToValidate,
                                                                             errorMessage);
  else
    return (impObj)->validateNameAndExpression(nameToValidate, 
                                               expressionToValidate,
                                               errorMessage);
}

OdResult OdDbAssocVariable::setExpression(const OdString newExpression, 
                                          const OdString evaluatorId, 
                                          bool checkForCyclicalDependencies,
                                          bool updateDependenciesOnReferencedSymbols,
                                          OdString errorMessage,
                                          bool silentMode)
{
  assertWriteEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->setExpression(this,
                                                           newExpression,
                                                           evaluatorId, 
                                                           checkForCyclicalDependencies,
                                                           updateDependenciesOnReferencedSymbols,
                                                           errorMessage,
                                                           silentMode);
}

const OdString OdDbAssocVariable::evaluatorId() const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->evaluatorId();
}

OdResult OdDbAssocVariable::setEvaluatorId(const OdString evalId)
{
  assertWriteEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->setEvaluatorId(evalId);
}

OdResult OdDbAssocVariable::setValue(const OdDbEvalVariant& newValue)
{
  assertWriteEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->setValue(newValue);
}

OdResult OdDbAssocVariable::setDescription(const OdString newDescription)
{
  assertWriteEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->setDescription(newDescription);
}


bool OdDbAssocVariable::isMergeable() const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->isMergeable();
}

bool OdDbAssocVariable::mustMerge() const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->mustMerge();
}


OdString OdDbAssocVariable::mergeableVariableName() const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->mergeableVariableName();
}

void OdDbAssocVariable::setIsMergeable(bool isMerg, bool mustMerg, const OdString mergeableVariableName)
{
  assertWriteEnabled();
  ((OdDbImpAssocVariable*)m_pImpObj)->setIsMergeable(isMerg, mustMerg, mergeableVariableName);
}

OdResult OdDbAssocVariable::evaluateExpression(OdDbEvalVariant& evaluatedExpressionValue,
                                     OdString        errorMessage) const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->evaluateExpression(evaluatedExpressionValue, errorMessage);
}

OdResult OdDbAssocVariable::evaluateExpression(OdDbObjectIdArray&        objectIds,    // in/out argument
                                               OdArray<OdDbEvalVariant>& objectValues, // in/out argument
                                               OdDbEvalVariant&          evaluatedExpressionValue,
                                               OdString                 errorMessage) const
{
  assertReadEnabled();
  return ((OdDbImpAssocVariable*)m_pImpObj)->evaluateExpression(objectIds,
                                                                objectValues,
                                                                evaluatedExpressionValue,
                                                                errorMessage);
}

void OdDbAssocVariable::addGlobalCallback(OdDbAssocVariableCallback* pCallback)
{
}

void OdDbAssocVariable::removeGlobalCallback(OdDbAssocVariableCallback* pCallback)
{
}

OdDbAssocVariableCallback* OdDbAssocVariable::globalCallback()
{
  return NULL;
}
