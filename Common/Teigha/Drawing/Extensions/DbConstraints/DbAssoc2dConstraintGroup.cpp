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
#include "DbAssoc2dConstraintGroup.h"
#include "DbImpAssoc2dConstraintGroup.h"
#include "DbAssocGeomDependency.h"
#include "OdConstraintGroupNodeImpl.h"
#include "DbAssocNetwork.h"
#include "OdExplicitConstr.h"
#include "DbFiler.h"
#include "DbIdMapping.h"
#include "DbAudit.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssoc2dConstraintGroup,    // ClassName
                       OdDbAssocAction,               // ParentClass
                       DBOBJECT_CONSTR,               // DOCREATE
                       OdDb::kDHL_1021,               // DwgVer
                       OdDb::kMRelease6,              // MaintVer
                       1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssoc2dConstraintGroup", // DWG class name
                       L"ACDBASSOC2DCONSTRAINTGROUP", // DxfName
                       L"ObjectDBX Classes",          // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssoc2dConstraintGroup::OdDbAssoc2dConstraintGroup():OdDbAssocAction(false)
{
  m_pImpObj = new OdDbImpAssoc2dConstraintGroup();
}

void OdDbAssoc2dConstraintGroup::audit(OdDbAuditInfo* pAuditInfo)
{
  OdDbAssocAction::audit(pAuditInfo);
  
  if (pAuditInfo->fixErrors())
    assertWriteEnabled();
  else
    assertReadEnabled();

  ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->audit(this, pAuditInfo);
}

const OdGePlane OdDbAssoc2dConstraintGroup::getWorkPlane() const
{
  assertReadEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->getWorkPlane();
}

OdResult OdDbAssoc2dConstraintGroup::setWorkPlane(const OdGePlane plane)
{
  assertWriteEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->setWorkPlane(plane);
}

#if 0
OdResult OdDbAssoc2dConstraintGroup::transformActionBy(const OdGeMatrix3d& transform);
{
  assertWriteEnabled();
  throw OdError(eNotImplemented);
  return eNotImplemented;
}

int OdDbAssoc2dConstraintGroup::getDOF() const
{
  assertReadEnabled();
  throw OdError(eNotImplemened);
  return 1;
}

SolutionStatus OdDbAssoc2dConstraintGroup::solutionStatus(bool bAlsoCheckForConstraints) const
{
  assertReadEnabled();
  throw OdError(eNotImplemented);
  return eNotAvailable;
}

SolutionStatus OdDbAssoc2dConstraintGroup::geometryStatus(const OdConstrainedGeometry* pConsGeom) const
{
  assertReadEnabled();
  throw OdError(eNotImplemented);
  return eNotAvailable;
}

SolutionStatus OdDbAssoc2dConstraintGroup::constraintStatus(const OdGeomConstraint* pConstraint) const
{
  assertReadEnabled();
  throw OdError(eNotImplemented);
  return eNotAvailable;
}
#endif

OdResult OdDbAssoc2dConstraintGroup::addConstrainedGeometry(const OdDbFullSubentPath& subentPath, OdConstrainedGeometry*& pConsGeom)
{
  assertWriteEnabled();
  
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->addConstrainedGeometry(subentPath, pConsGeom, this);
}

OdResult OdDbAssoc2dConstraintGroup::deleteConstrainedGeometry(const OdDbObjectId& geomDependencyId)
{
  assertWriteEnabled();

  OdResult res = ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->deleteConstrainedGeometry(geomDependencyId);
  if (((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->isEmpty()) {
    OdDbAssocNetworkPtr net = owningNetwork().openObject(OdDb::kForWrite);
    if (!net.isNull()) {
      net->removeAction(objectId(), false);
      net->setStatus(kChangedDirectlyAssocStatus);
    }
    erase();
  }
  return res;
}

OdResult OdDbAssoc2dConstraintGroup::getConstrainedGeometry(const OdDbFullSubentPath& fullSubentPath,
                                  OdConstrainedGeometry*&   pConsGeom,
                                  bool bCreateArcLineMid)
{
  assertReadEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->getConstrainedGeometry(fullSubentPath,
                                                                           pConsGeom,
                                                                           bCreateArcLineMid);
}

//TODO: add code for ImplicitPointType
OdResult OdDbAssoc2dConstraintGroup::getConstrainedGeometry(const OdDbAssocGeomDependency* pGeomDependency,
                                  OdConstrainedGeometry*&   pConsGeom,
                                  OdConstrainedImplicitPoint::ImplicitPointType* pPtType,
                                  int defPtIndex,
                                  bool bCreateArcLineMid)
{
  assertReadEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->getConstrainedGeometry(pGeomDependency,
                                                                           pConsGeom,
                                                                           pPtType,
                                                                           defPtIndex,
                                                                           bCreateArcLineMid);
}

OdResult OdDbAssoc2dConstraintGroup::getConstrainedGeometries(OdArray<OdConstrainedGeometry*>& aConstGeom) const
{
  assertReadEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->getConstrainedGeometries(aConstGeom);
}

OdResult OdDbAssoc2dConstraintGroup::addGeometricalConstraint(OdGeomConstraint::GeomConstraintType type, 
                                    const OdDbFullSubentPathArray& aPaths,
                                    OdGeomConstraint** ppNewConstraint)
{
  assertWriteEnabled();
  OdResult res =  ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->addGeometricalConstraint(objectId(), 
                                                                                        type, 
                                                                                        aPaths,
                                                                                        ppNewConstraint);
  if ( res == eOk )
  {
    setStatus(kChangedDirectlyAssocStatus);
    downgradeOpen();
    OdDbObjectId networkId = owningNetwork();
    OdDbAssocNetworkPtr pNet = networkId.openObject(OdDb::kForWrite);
    pNet->evaluate(NULL);
  }

  return res;
}

OdResult OdDbAssoc2dConstraintGroup::addGeometricalConstraint(OdGeomConstraint::GeomConstraintType type,
                                    const OdArray<OdConstrainedGeometry*>& apConsGeoms,
                                    OdGeomConstraint** ppNewConstraint)
{
  assertWriteEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->addGeometricalConstraint(objectId(),
                                                                               type,
                                                                               apConsGeoms,
                                                                               ppNewConstraint);
}

OdResult OdDbAssoc2dConstraintGroup::addDistanceConstraint(
    OdConstrainedGeometry* pConsGeom1,
    OdConstrainedGeometry* pConsGeom2,
    OdDistanceConstraint::DirectionType directionType,
    OdDbObjectId valueDependencyId,
    OdDbObjectId dimDependencyId, // May be null
    const OdGeVector3d* pFixedDirection/* = NULL*/,
    const OdConstrainedLine* pDirectionLine/* = NULL*/,
    OdDistanceConstraint** ppNewDisConstraint/* = NULL*/)
{
  assertWriteEnabled();
  OdResult res =
      ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->addDistanceConstraint(this,
                                                                         pConsGeom1,
                                                                         pConsGeom2,
                                                                         directionType,
                                                                         valueDependencyId,
                                                                         dimDependencyId,
                                                                         pFixedDirection,
                                                                         pDirectionLine,
                                                                         ppNewDisConstraint);
  if (res == eOk) {
    setStatus(kChangedDirectlyAssocStatus);
    downgradeOpen();
    OdDbObjectId networkId = owningNetwork();
    OdDbAssocNetworkPtr pNet = networkId.openObject(OdDb::kForWrite);
    pNet->evaluate(NULL);
  }
  return res;
}

OdResult OdDbAssoc2dConstraintGroup::addAngleConstraint(
    OdConstrainedLine* pConsLine1,
    OdConstrainedLine* pConsLine2,
    OdAngleConstraint::SectorType sectorType,
    OdDbObjectId valueDependencyId,
    OdDbObjectId dimDependencyId, // May be null
    OdAngleConstraint** ppNewAngConstraint/* = NULL*/)
{
  assertWriteEnabled();
  OdResult res =
      ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->addAngleConstraint(this,
                                                                      pConsLine1,
                                                                      pConsLine2,
                                                                      sectorType,
                                                                      valueDependencyId,
                                                                      dimDependencyId,
                                                                      ppNewAngConstraint);
  if (res == eOk) {
    setStatus(kChangedDirectlyAssocStatus);
    downgradeOpen();
    OdDbObjectId networkId = owningNetwork();
    OdDbAssocNetworkPtr pNet = networkId.openObject(OdDb::kForWrite);
    pNet->evaluate(NULL);
  }
  return res;
}

OdResult OdDbAssoc2dConstraintGroup::add3PointAngleConstraint(
    OdConstrainedPoint* pConsPoint1,
    OdConstrainedPoint* pConsPoint2,
    OdConstrainedPoint* pConsPoint3,
    OdAngleConstraint::SectorType sectorType,
    OdDbObjectId valueDependencyId,
    OdDbObjectId dimDependencyId, // May be null
    Od3PointAngleConstraint** ppNewAngConstraint/* = NULL*/)
{
  assertWriteEnabled();
  OdResult res =
      ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->add3PointAngleConstraint(this,
                                                                            pConsPoint1,
                                                                            pConsPoint2,
                                                                            pConsPoint3,
                                                                            sectorType,
                                                                            valueDependencyId,
                                                                            dimDependencyId,
                                                                            ppNewAngConstraint);
  if (res == eOk) {
    setStatus(kChangedDirectlyAssocStatus);
    downgradeOpen();
    OdDbObjectId networkId = owningNetwork();
    OdDbAssocNetworkPtr pNet = networkId.openObject(OdDb::kForWrite);
    pNet->evaluate(NULL);
  }
  return res;
}

OdResult OdDbAssoc2dConstraintGroup::addRadiusDiameterConstraint(
    OdConstrainedGeometry* pConsGeom,
    OdRadiusDiameterConstraint::RadiusDiameterConstrType type,
    OdDbObjectId valueDependencyId,
    OdDbObjectId dimDependencyId, // May be null
    OdRadiusDiameterConstraint** ppNewRadDiaConstraint/* = NULL*/)
{
  assertWriteEnabled();
  OdResult res =
      ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->addRadiusDiameterConstraint(this,
                                                                               pConsGeom,
                                                                               type,
                                                                               valueDependencyId,
                                                                               dimDependencyId,
                                                                               ppNewRadDiaConstraint);
  if (res == eOk) {
    setStatus(kChangedDirectlyAssocStatus);
    downgradeOpen();
    OdDbObjectId networkId = owningNetwork();
    OdDbAssocNetworkPtr pNet = networkId.openObject(OdDb::kForWrite);
    pNet->evaluate(NULL);
  }
  return res;
}
OdResult OdDbAssoc2dConstraintGroup::deleteConstraint(OdGeomConstraint* pGeomConst)
{
  assertWriteEnabled();
  OdResult res = ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->deleteConstraint(pGeomConst);
  if (((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->isEmpty()) {
    OdDbAssocNetworkPtr net = owningNetwork().openObject(OdDb::kForWrite);
    if (!net.isNull()) {
      net->removeAction(objectId(), false);
      net->setStatus(kChangedDirectlyAssocStatus);
    }
    erase();
  }
  return res;
}


OdResult OdDbAssoc2dConstraintGroup::getConstraints(OdArray<OdGeomConstraint*>& apConstraints) const
{
  assertReadEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->getConstraints(apConstraints);
}

#if 0
OdResult OdDbAssoc2dConstraintGroup::autoConstrain(const OdDbFullSubentPathArray& aPaths,
                         const OdGeTol& tol,
                         OdAutoConstrainEvaluationCallback* pCallback)
#endif
OdResult OdDbAssoc2dConstraintGroup::getAllConnectedGeomDependencies(const OdDbObjectIdArray& srcGeomDependencyIds,
                                           OdDbObjectIdArray& allConnectedGeomDependencyIds) const
{

  assertReadEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->getAllConnectedGeomDependencies(srcGeomDependencyIds,
                                                                                    allConnectedGeomDependencyIds);
}

OdConstraintGroupNode* OdDbAssoc2dConstraintGroup::getGroupNodePtr(const OdConstraintGroupNodeId nodeId) const
{
  assertReadEnabled();
  return ((OdDbImpAssoc2dConstraintGroup*)m_pImpObj)->getGroupNodePtr(nodeId);
}

#if 0
void OdDbAssoc2dConstraintGroup::regenDimensionSystem()

OdResult OdDbAssoc2dConstraintGroup::geometryMirrored(OdDbAssocGeomDependency* pGeomDependency)
  
void OdDbAssoc2dConstraintGroup::addGlobalCallback(class OdDbAssoc2dConstraintCallback* pCallback)

void OdDbAssoc2dConstraintGroup::removeGlobalCallback(class OdDbAssoc2dConstraintCallback* pCallback)

class OdDbAssoc2dConstraintCallback* OdDbAssoc2dConstraintGroup::globalCallback()

#endif
