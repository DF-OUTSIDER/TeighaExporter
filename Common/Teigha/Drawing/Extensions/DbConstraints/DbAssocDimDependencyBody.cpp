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
#include "DbAssocDimDependencyBody.h"
#include "DbAssoc2dConstraintGroup.h"
#include "OdExplicitConstr.h"
#include "DbDimension.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocDimDependencyBody,     // ClassName
                       OdDbAssocDimDependencyBodyBase, // ParentClass
                       DBOBJECT_CONSTR,                // DOCREATE
                       OdDb::kDHL_1021,                // DwgVer
                       OdDb::kMRelease6,               // MaintVer
                       1025,                           // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocDimDependencyBody",  // DWG class name
                       L"ASSOCDIMDEPENDENCYBODY",      // DxfName
                       L"ObjectDBX Classes",           // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocDimDependencyBody::OdDbAssocDimDependencyBody():OdDbAssocDimDependencyBodyBase()
{
}

OdDbAssocDimDependencyBody::~OdDbAssocDimDependencyBody()
{
}

OdResult OdDbAssocDimDependencyBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbAssocDimDependencyBodyBase::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  int rdVal = pFiler->rdInt16();
  ODA_ASSERT(rdVal == 1);

  return res;
}

void OdDbAssocDimDependencyBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbAssocDimDependencyBodyBase::dwgOutFields(pFiler);
  
  pFiler->wrInt16(1);
}

OdResult OdDbAssocDimDependencyBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbAssocDimDependencyBodyBase::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  if( !pFiler->atSubclassData(desc()->name()))
    return eBadDxfSequence;

  if( pFiler->nextItem() != 90 || pFiler->rdUInt32() != 1)
    return eMakeMeProxy;

  return res;
}

void OdDbAssocDimDependencyBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbAssocDimDependencyBodyBase::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(desc()->name());
  pFiler->wrUInt32(90, 1);
}

OdString OdDbAssocDimDependencyBody::getEntityTextOverride() const
{
  assertReadEnabled();
  OdDbObjectId dimId = dependentOnObject();
  if ( dimId.isNull() )
    return OdString::kEmpty;

  OdDbDimensionPtr pDim = dimId.openObject();
  if ( pDim.isNull() )
    return OdString::kEmpty;

  return pDim->dimensionText();
}

OdResult OdDbAssocDimDependencyBody::setEntityTextOverride(const OdString& newText)
{
  assertReadEnabled();
  OdDbObjectId dimId = dependentOnObject();
  if ( dimId.isNull() )
    return eBadObjType;

  OdDbDimensionPtr pDim = dimId.openObject(OdDb::kForWrite);
  if ( pDim.isNull() )
    return eBadObjType;

  pDim->setDimensionText(newText);

  return eOk;
}

double OdDbAssocDimDependencyBody::getEntityMeasurementOverride() const
{
  assertReadEnabled();
  OdDbObjectId dimId = dependentOnObject();
  if ( dimId.isNull() )
    return eBadObjType;

  OdDbDimensionPtr pDim = dimId.openObject();
  if ( pDim.isNull() )
    return eBadObjType;

  return pDim->getMeasurement();
}

bool OdDbAssocDimDependencyBody::isEntityAttachmentChangedOverride() const
{
  assertReadEnabled();
  return true;
}

OdResult OdDbAssocDimDependencyBody::updateDependentOnObjectOverride()
{
  assertWriteEnabled();
  OdDbObjectId actionId = owningAction();
  if ( actionId.isNull() )
    return eBadObjType;

  OdDbAssocActionPtr pAction = actionId.openObject();
  if ( !pAction->isKindOf(OdDbAssoc2dConstraintGroup::desc()) )
    return eBadObjType;

  OdDbAssoc2dConstraintGroupPtr pCnstrGr = OdDbAssoc2dConstraintGroup::cast(pAction);

  OdExplicitConstraint* pCnstr = constraint();
  if (pCnstr)
    pCnstr->updateDimDependency(OdGeMatrix3d::planeToWorld(pCnstrGr->getWorkPlane()));
  OdString name, expression, value;
  getVariableNameAndExpression(name, expression, value);
  upgradeOpen();
  setEntityNameAndExpression(name, expression, value);
  setStatus(kIsUpToDateAssocStatus, false);

  return eOk;
}

OdResult /*static*/ OdDbAssocDimDependencyBody::createAndPostToDatabase(const OdDbObjectId& dimId,
                                    OdDbObjectId&       dimDepId,
                                    OdDbObjectId&       dimDepBodyId)
{
  if ( dimId.isNull() )
  {
    return eBadObjType;
  }

  OdDbAssocDependencyPtr pDep = OdDbAssocDependency::createObject();
  OdDbAssocDimDependencyBodyPtr pDepBody = OdDbAssocDimDependencyBody::createObject();

  if ( pDepBody.isNull() )
    return eBadObjType;

  if ( pDep.isNull() )
    return eBadObjType;
  
  OdDbDatabase *pDB = dimId.database();

  dimDepId = pDB->addOdDbObject(pDep);
  dimDepBodyId = pDB->addOdDbObject(pDepBody);

  if ( dimDepId.isNull() || dimDepBodyId.isNull() )
    return eBadObjType;

  pDep->setDependencyBody(dimDepBodyId);

  OdDbCompoundObjectIdPtr dimCId = OdDbCompoundObjectId::createObject();
  dimCId->set(dimId, pDB);
  pDep->attachToObject(dimCId);
  pDep->setIsReadDependency(false);
  pDep->setIsWriteDependency(true);

  return eOk;
}
