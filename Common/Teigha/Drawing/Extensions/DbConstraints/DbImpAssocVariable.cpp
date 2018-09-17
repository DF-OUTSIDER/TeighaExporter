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
#include "DbImpAssocVariable.h"
#include "DbAssocNetwork.h"
#include "DbCompoundObjectId.h"
#include "DbAssocValueDependency.h"
#include "OdExpression.h"

#include "DbFiler.h"

OdDbAssocVariableCallback* OdDbImpAssocVariable::m_assocVariableCallback = NULL;

OdDbImpAssocVariable::OdDbImpAssocVariable():OdDbImpAssocAction(),
                                        m_varName(),
                                        m_expression(),
                                        m_description(),
                                        m_evaluatorId(L"AcDbCalc:1.0"),
                                        m_isMergeable(false),
                                        m_mustMerge(false),
                                        m_mergeableVariableName(),
                                        m_arrValueDependenciesIdx()
{
  m_isBase = false;
  m_value = OdDbEvalVariant::init(1.0);
}

OdDbImpAssocVariable::~OdDbImpAssocVariable()
{
}

const OdString OdDbImpAssocVariable::name() const
{
  return m_varName;
}

const OdString OdDbImpAssocVariable::expression() const
{
  return m_expression;
}

const OdDbEvalVariantPtr OdDbImpAssocVariable::value() const
{
  return m_value;
}

const OdString OdDbImpAssocVariable::description() const
{
  return m_description;
}

void OdDbImpAssocVariable::evaluate(OdDbAssocAction*, OdDbAssocEvaluationCallback*)
{
  if (m_arrDependencies.size() == 0)
    return;
  OdExpression::Parser p;
  OdDbEvalVariantPtr value = OdDbEvalVariant::createObject();
  for (unsigned int i = 0; i < m_arrDependencies.size(); i++) {
    OdDbAssocValueDependencyPtr pDep = m_arrDependencies[i].openObject();
    pDep->getDependentOnObjectValue(value);
    OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(pDep->dependentOnObject().openObject());
    if (!pVar.isNull())
      p.addVariable(pVar->name(), value);
  }
  if (p.setExpression(m_expression) != eOk)
    return;
  value = p.evaluate();
  if (value.isNull())
  {
    setStatus(kFailedToEvaluateAssocStatus, false);
    return;
  }
  m_value = value;
  setStatus(kIsUpToDateAssocStatus, false);
}

OdResult OdDbImpAssocVariable::setName(OdDbAssocVariable* pVariable, const OdString newName, bool updateReferencingExpressions)
{
  if ( m_varName.compare(newName) == 0 )
    return eOk;
  if ((newName.isEmpty() || newName[0] == OdChar('*')) && // anonymous variable
    !updateReferencingExpressions) //!todo implement check for ReferencingExpressions
  {
    m_varName = newName;
    return eOk;
  }
  OdDbObjectId objId = findObjectByName(newName, OdDbAssocVariable::desc());
  if ( !objId.isNull() )
    return eBadObjType;

  OdExpression::Parser p;
  OdResult res = p.setName(newName);
  if (res != eOk)
    return res;

  if (updateReferencingExpressions) {
    OdDbObjectIdArray aReactors = pVariable->getPersistentReactors();
    for (unsigned int i = 0; i < aReactors.size(); i++) {
      OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(aReactors[i].openObject());
      if (pDep.isNull())
        continue;
      OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(pDep->owningAction().openObject());
      if (!pVar.isNull()) {
        pVar->upgradeOpen();
        pVar->assertWriteEnabled();
        res = ((OdDbImpAssocVariable*)OdDbImpAssocAction::getImpObject(pVar))->renameVariable(m_varName, newName);
      }
      else {
        pDep->upgradeOpen();
        pDep->setStatus(kChangedDirectlyAssocStatus);
      }
    }
  }
  m_varName = newName;

  return eOk;
}

OdResult OdDbImpAssocVariable::fixVariable(const OdString& name, OdDbEvalVariantPtr value)
{
  OdExpression::Parser p;
  p.fixVariable(name, value);
  OdResult res = p.setExpression(m_expression);
  if (res == eOk)
    m_expression = (p.isConstExpression()) ? OdExpression::createExpression(m_value) : p.getTranslated();
  return res;
}

OdResult OdDbImpAssocVariable::renameVariable(const OdString& oldName, const OdString& newName)
{
  OdExpression::Parser p;
  p.renameVariable(oldName, newName);
  OdResult res = p.setExpression(m_expression);
  if (res == eOk)
    m_expression = p.getTranslated();
  return res;
}

OdResult OdDbImpAssocVariable::checkCyclicDependencies(OdArray<OdString>& names, OdDbObjectIdArray& reactors)
{
  OdResult res = eOk;
  for (unsigned int i = 0; i < reactors.size() && res == eOk; i++) {
    OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(reactors[i].openObject());
    if (!pDep.isNull()) {
      OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(pDep->owningAction().openObject());
      if (!pVar.isNull()) {
        OdString name = OdString(pVar->name()).makeLower();
        for (unsigned int j = 0; j < names.size(); j++) {
          if (name == names[j])
            return eDuplicateKey;
        }
        OdDbObjectIdArray aSubReactors = pVar->getPersistentReactors();
        res = ((OdDbImpAssocVariable*)OdDbImpAssocAction::getImpObject(pVar))->checkCyclicDependencies(names, aSubReactors);
      }
    }
  }
  return res;
}

OdDbObjectId OdDbImpAssocVariable::findObjectByName(const OdString  objectName, 
                              const OdRxClass* pObjectClass) const
{
  OdDbObjectId networkId = owningNetwork();
  if ( networkId.isNull() )
    return OdDbObjectId::kNull;

  OdDbAssocNetworkPtr pNetwork = networkId.openObject();
  OdDbObjectIdArray arrObj = pNetwork->getActions();
  OdDbObjectPtr pObj;
  OdDbAssocVariablePtr pVar;
  for ( unsigned int i = 0; i < arrObj.length(); i++ )
  {
    pObj = arrObj[i].openObject();
    if ( pObj->isKindOf(pObjectClass) )
    {
      //Currently the only named objects are OdDbImpAssocVariables
      if ( pObj->isKindOf(OdDbAssocVariable::desc()) )
      {
        pVar = OdDbAssocVariable::cast(pObj);
        if ( pVar->name().iCompare(objectName) == 0 )
          return arrObj[i];
      }
    } 
  }

  return OdDbObjectId::kNull;
}

OdResult OdDbImpAssocVariable::validateNameAndExpression(const OdString nameToValidate, 
                                                      const OdString expressionToValidate,
                                                      OdString       errorMessage) const
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocVariable::setExpression(OdDbAssocVariable* pVariable,
                                             const OdString newExpression,
                                          const OdString evaluatorId, 
                                          bool checkForCyclicalDependencies,
                                          bool updateDependenciesOnReferencedSymbols,
                                          OdString errorMessage,
                                          bool silentMode)
{
  if (!evaluatorId.isEmpty())
    m_evaluatorId = evaluatorId;
  if (newExpression.isEmpty()) {
    m_expression.empty();
    int i = m_arrDependencies.size();
    while (i-- > 0) {
      removeDependency(m_arrDependencies[i], true);
    }
    m_arrValueDependenciesIdx.clear();
    return eOk;
  }
  OdExpression::Parser p;
  OdResult res;
  if ((res = p.setExpression(newExpression)) != eOk)
    return res;
  OdArray<OdString> usedVars;
  p.getVariables(usedVars);
  int i = usedVars.size();
  while (i-- > 0) {
    if (m_varName.iCompare(usedVars[i]) == 0)
      return eDuplicateKey;
    OdDbAssocVariablePtr var = findObjectByName(usedVars[i], OdDbAssocVariable::desc()).openObject();
    if (var.isNull())
      return eNullObjectPointer;
    p.addVariable(var->name(), var->value());
  }
  if (checkForCyclicalDependencies && usedVars.size() > 0) {
    OdDbObjectIdArray aReactors = pVariable->getPersistentReactors();
    if ((res = checkCyclicDependencies(usedVars, aReactors)) != eOk)
      return res;
  }
  OdDbEvalVariantPtr val = p.evaluate();
  if (val.isNull())
    return eInvalidResBuf;
  if (updateDependenciesOnReferencedSymbols) {
    setExpression(pVariable, OdString::kEmpty, OdString::kEmpty, false, false, OdString::kEmpty, true);
    OdInt16 c = 0;
    int i = usedVars.size();
    while (i-- > 0) {
      OdDbAssocVariablePtr var = findObjectByName(usedVars[i], OdDbAssocVariable::desc()).safeOpenObject();
      OdDbCompoundObjectIdPtr varCId = OdDbCompoundObjectId::createObject();
      varCId->set(var->objectId());
      OdDbAssocValueDependencyPtr valDep = OdDbAssocValueDependency::createObject();
      pVariable->addDependency(var->database()->addOdDbObject(valDep));
      valDep->setIsWriteDependency(false);
      valDep->attachToObject(varCId);
      valDep->setDependentOnObjectValue(var->value());
      m_arrValueDependenciesIdx.append(c++); //!TODO check proper behavior
    }
  }
  m_value = val;
  m_expression = newExpression;
  OdDbObjectIdArray aReactors = pVariable->getPersistentReactors();
  for (unsigned int i = 0; i < aReactors.size(); i++) {
    OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(aReactors[i].openObject());
      if (pDep.isNull())
        continue;
      OdDbObjectPtr pAction = pDep->owningAction().openObject();
      ODA_ASSERT_ONCE(!pAction.isNull());
      if (!pAction.isNull() && pAction->isA() != OdDbAssocVariable::desc()) {
        pAction = 0;
        pDep->upgradeOpen();
        pDep->setStatus(kChangedDirectlyAssocStatus);
      }
  }
  
  return eOk;
}

const OdString OdDbImpAssocVariable::evaluatorId() const
{
  return m_evaluatorId;
}

OdResult OdDbImpAssocVariable::setEvaluatorId(const OdString evalId)
{
  m_evaluatorId = evalId;
  return eOk;
}


OdResult OdDbImpAssocVariable::setValue(const OdDbEvalVariant& newValue)
{
  if ( m_arrDependencies.length() > 0 )
    return eBadObjType;

  m_expression.empty();

  *m_value.get() = newValue;
  setStatus(kChangedDirectlyAssocStatus);

  return eOk;
}

OdResult OdDbImpAssocVariable::setDescription(const OdString newDescription)
{
  m_description = newDescription;
  return eOk;
}


bool OdDbImpAssocVariable::isMergeable() const
{
  return m_isMergeable;
}

bool OdDbImpAssocVariable::mustMerge() const
{
  return m_mustMerge;
}


OdString OdDbImpAssocVariable::mergeableVariableName() const
{
  return m_mergeableVariableName;
}

void OdDbImpAssocVariable::setIsMergeable(bool isMerg, bool mustMerg, const OdString mergeableVariableName)
{
  m_isMergeable = isMerg;
  m_mustMerge = mustMerg;
  m_mergeableVariableName = mergeableVariableName;
}

OdResult OdDbImpAssocVariable::evaluateExpression(OdDbEvalVariant& evaluatedExpressionValue,
                                     OdString        errorMessage) const
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocVariable::evaluateExpression(OdDbObjectIdArray&        objectIds,    // in/out argument
                                               OdArray<OdDbEvalVariant>& objectValues, // in/out argument
                                               OdDbEvalVariant&          evaluatedExpressionValue,
                                               OdString                 errorMessage) const
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocVariable::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dwgInFields(pFiler, objectId);
  if (res != eOk) 
    return res;
  if (pFiler->rdInt16() != 2)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_varName = pFiler->rdString();
  m_expression = pFiler->rdString();
  m_evaluatorId = pFiler->rdString();
  m_description = pFiler->rdString();

  res = m_value->dwgInFields(pFiler);
  if ( res != eOk ) 
    return res;

  m_isMergeable = pFiler->rdBool();
  m_mustMerge = pFiler->rdBool();

  OdInt16 qty = pFiler->rdInt16();
  if ( qty > 0 )
  {
    m_arrDependencies.resize(qty);
    m_arrValueDependenciesIdx.resize(qty);

    OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
    OdInt16 *pIdx = m_arrValueDependenciesIdx.asArrayPtr();
    while (qty--)
    {
      *pId++ = pFiler->rdHardOwnershipId();
      *pIdx++ = pFiler->rdInt16();
    }
  }
  return eOk;
}

void OdDbImpAssocVariable::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dwgOutFields(pFiler, objectId);

  pFiler->wrInt16(2);

  pFiler->wrString(m_varName);
  pFiler->wrString(m_expression);
  pFiler->wrString(m_evaluatorId);
  pFiler->wrString(m_description);

  m_value->dwgOutFields(pFiler);

  pFiler->wrBool(m_isMergeable );
  pFiler->wrBool(m_mustMerge);

  unsigned int qty = m_arrDependencies.size();
  pFiler->wrInt16((OdInt16)qty);
  if ( qty > 0 )
  {
    const OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
    const OdInt16 *pIdx = m_arrValueDependenciesIdx.asArrayPtr();
    while (qty--)
    {
      pFiler->wrHardOwnershipId(*pId++);
      pFiler->wrInt16(*pIdx++);
    }
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{ \
  ODA_FAIL(); \
  return eMakeMeProxy; \
}

OdResult OdDbImpAssocVariable::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dxfInFields(pFiler, objectId);
  if ( res != eOk ) 
    return res;

  if( !pFiler->atSubclassData( OdDbAssocVariable::desc()->name() ))
  {
    return eBadDxfSequence;
  }

  NEXT_CODE(90)
  if (pFiler->rdUInt32() != 2)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(1)
  pFiler->rdString(m_varName);

  NEXT_CODE(1)
  pFiler->rdString(m_expression);

  NEXT_CODE(1)
  pFiler->rdString(m_evaluatorId);

  NEXT_CODE(1)
  pFiler->rdString(m_description);

  res = m_value->dxfInFields(pFiler);
  if (res != eOk) 
    return res;

  NEXT_CODE(290)
  m_isMergeable = pFiler->rdBool();
  NEXT_CODE(290)
  m_mustMerge = pFiler->rdBool();

  NEXT_CODE(90)
  OdUInt32 qty = pFiler->rdInt32();
  if ( qty > 0 )
  { 
    m_arrDependencies.resize(qty);
    m_arrValueDependenciesIdx.resize(qty);
    OdDbObjectId *pId = m_arrDependencies.asArrayPtr();
    OdInt16 *pIdx = m_arrValueDependenciesIdx.asArrayPtr();
    while (qty--)
    {
      NEXT_CODE(360)
      *pId++ = pFiler->rdObjectId();

      NEXT_CODE(90)
      *pIdx++ = (OdInt16)pFiler->rdInt32();
    }
  }

  return res;
}

void OdDbImpAssocVariable::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker( OdDbAssocVariable::desc()->name() );
  pFiler->wrInt32(90, 2);
  pFiler->wrString(1, m_varName);
  pFiler->wrString(1, m_expression);
  pFiler->wrString(1, m_evaluatorId);
  pFiler->wrString(1, m_description);

  m_value->dxfOutFields(pFiler);

  pFiler->wrBool(290, m_isMergeable);
  pFiler->wrBool(290, m_mustMerge);

  int qty = m_arrDependencies.length();
  pFiler->wrInt32(90, qty);
  if ( qty > 0 )
  {
    const OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
    const OdInt16* pIdx = m_arrValueDependenciesIdx.asArrayPtr();
    while ( qty-- )
    {
      pFiler->wrObjectId(360, *pId++);
      pFiler->wrInt32(90, *pIdx++);
    }
  }
}
