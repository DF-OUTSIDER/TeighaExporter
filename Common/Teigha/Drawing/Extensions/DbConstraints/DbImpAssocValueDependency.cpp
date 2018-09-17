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
#include "DbAssocValueDependency.h"
#include "DbImpAssocValueDependency.h"
#include "DbAssocVariable.h"
#include "DbImpAssocVariable.h"
#include "DbAssocNetwork.h"

#include "DbFiler.h"

OdDbImpAssocValueDependency::OdDbImpAssocValueDependency() : OdDbImpAssocDependency(),
                                                       m_valueName()
{
  m_cashedValue = OdDbEvalVariant::createObject();
}

const OdString& OdDbImpAssocValueDependency::valueName() const
{
  return m_valueName;
}

OdResult OdDbImpAssocValueDependency::setValueName(const OdString& newValueName)
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocValueDependency::getDependentOnObjectValue(OdDbEvalVariantPtr& objectValue) const
{
  objectValue->copyFrom(m_cashedValue);
  return eOk;
}

OdResult OdDbImpAssocValueDependency::setDependentOnObjectValue(const OdDbEvalVariantPtr& newObjectValue)
{
  m_cashedValue->copyFrom(newObjectValue);
  return eOk;
}

void OdDbImpAssocValueDependency::erased(const OdDbObject* dbObj, OdDbAssocDependency *pDep, bool isErasing)
{
  if (dbObj->isUndoing())
    return;
  OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(dbObj);
  if (!isErasing || pVar.isNull())
    return;
  OdDbAssocVariablePtr pAction = OdDbAssocVariable::cast(pDep->owningAction().openObject());
  if (pAction.isNull()) {
    OdDbImpAssocDependency::erased(dbObj, pDep, isErasing);
    return;
  }
  else {
    pAction->upgradeOpen();
    pAction->removeDependency(pDep->objectId(), true);
    ((OdDbImpAssocVariable*)OdDbImpAssocAction::getImpObject(pAction))->fixVariable(pVar->name(), m_cashedValue);
  }
  OdDbAssocNetworkPtr net = pVar->owningNetwork().openObject(OdDb::kForWrite);
  if (!net.isNull())
    net->removeAction(pVar->objectId(), true);
}

void OdDbImpAssocValueDependency::modified(const OdDbObject* dbObj, OdDbAssocDependency *pDep)
{
  if ( dbObj->isKindOf(OdDbAssocVariable::desc()) )
  {
    OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(dbObj);
    if ( *pVar->value() == *m_cashedValue )
      return;

    setDependentOnObjectValue(pVar->value());
    OdDbAssocVariablePtr pAction = OdDbAssocVariable::cast(pDep->owningAction().openObject(OdDb::kForWrite));
    if (!pAction.isNull())
      pAction->evaluate(NULL);
    else
	    setStatus(kChangedDirectlyAssocStatus, true, pDep);

    setStatus(kIsUpToDateAssocStatus, false, pDep);
  }
}

void OdDbImpAssocValueDependency::copied(const OdDbObject* pDbObj, const OdDbObject* pNewObj, OdDbAssocDependency *pDep)
{
}

OdResult OdDbImpAssocValueDependency::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocDependency::dwgInFields(pFiler);
  if ( res != eOk )   
    return res;

  if ( pFiler->rdInt16() != 0 )
    return eMakeMeProxy;

  m_valueName = pFiler->rdString();
  res = m_cashedValue->dwgInFields(pFiler);
  return res;
}

void OdDbImpAssocValueDependency::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocDependency::dwgOutFields(pFiler);

  pFiler->wrInt16(0);

  pFiler->wrString(m_valueName);

  m_cashedValue->dwgOutFields(pFiler);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdDbImpAssocValueDependency::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocDependency::dxfInFields(pFiler);
  if ( res != eOk )   
    return res;

  if( !pFiler->atSubclassData( OdDbAssocValueDependency::desc()->name() ))
  {
    return eBadDxfSequence;
  }

  if ( pFiler->nextItem() != 90 || pFiler->rdUInt32() != 0)
    return eMakeMeProxy;

  NEXT_CODE(1)
  pFiler->rdString(m_valueName);

  res = m_cashedValue->dxfInFields(pFiler);
  if ( res != eOk )   
    return res;

  return res;
}

void OdDbImpAssocValueDependency::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocDependency::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocValueDependency::desc()->name());
  pFiler->wrInt32(90, 0);
  pFiler->wrString(1, m_valueName);
  m_cashedValue->dxfOutFields(pFiler);
}

bool OdDbImpAssocValueDependency::isRelevantChange(const OdDbAssocDependency *pDep) const
{
    return true;
}

OdResult OdDbImpAssocValueDependency::updateDependentOnObject(OdDbAssocDependency * pDep)
{
  return eOk;
}
