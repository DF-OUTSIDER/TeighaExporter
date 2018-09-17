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
#include "DbAssocDependencyBody.h"
#include "DbFiler.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocDependencyBody, OdDbObject, L"AcDbAssocDependencyBody");


OdDbAssocDependencyBody::OdDbAssocDependencyBody():OdDbObject()
{
}

OdDbAssocDependencyBody::~OdDbAssocDependencyBody()
{}

OdResult OdDbAssocDependencyBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbObject::dwgInFields(pFiler);

  OdInt16 val = pFiler->rdInt16(); // ver
  ODA_ASSERT_ONCE(val == 1);
  if (val != 1)
    return eMakeMeProxy;;

  return res;
}

void OdDbAssocDependencyBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);

  pFiler->wrInt16(1); // ver
}

OdResult OdDbAssocDependencyBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  if (pFiler->nextItem() != 90 || pFiler->rdUInt32() != 1) // ver
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  return res;
} 

void OdDbAssocDependencyBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(desc()->name());
  pFiler->wrUInt32(90, 1); // ver
}

OdDbAssocStatus OdDbAssocDependencyBody::status() const
{
  assertReadEnabled();
  OdDbAssocDependencyPtr pParent = parentDependency().openObject();
  if (pParent.isNull())
    return kIsUpToDateAssocStatus;
  return pParent->status();
}

OdResult OdDbAssocDependencyBody::setStatus(OdDbAssocStatus newStatus, bool notifyOwningAction)
{
  assertWriteEnabled();
  OdDbAssocDependencyPtr pParent = parentDependency().openObject(OdDb::kForWrite);
  if (pParent.isNull())
    return eInvalidOwnerObject;
  return pParent->setStatus(newStatus, notifyOwningAction);
}

 OdDbObjectId OdDbAssocDependencyBody::owningAction() const
 {
  OdDbAssocDependencyPtr pParent = parentDependency().openObject();
  if (pParent.isNull())
    return OdDbObjectId::kNull;
  return pParent->owningAction();
 }

OdDbObjectId OdDbAssocDependencyBody::dependentOnObject() const
{
  assertReadEnabled();
  OdDbAssocDependencyPtr pParent = parentDependency().openObject();
  if (pParent.isNull())
    return OdDbObjectId::kNull;
  return pParent->dependentOnObject();
}

bool OdDbAssocDependencyBody::isActionEvaluationInProgress() const
{
  assertReadEnabled();
  OdDbAssocDependencyPtr pParent = parentDependency().openObject();
  if (pParent.isNull())
    return false;
  return pParent->isActionEvaluationInProgress();
}

OdDbAssocEvaluationCallback* OdDbAssocDependencyBody::currentEvaluationCallback() const
{
  assertReadEnabled();
  OdDbAssocDependencyPtr pParent = parentDependency().openObject();
  if (pParent.isNull())
    return NULL;
  return pParent->currentEvaluationCallback();
}
