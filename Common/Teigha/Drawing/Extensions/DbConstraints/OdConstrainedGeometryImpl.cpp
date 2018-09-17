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
#include "OdConstrainedGeometryImpl.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbFiler.h"

#include "OdConstrainedRigidSetImpl.h"

#include "DbBlockReference.h"

OdConstrainedGeometryImpl::OdConstrainedGeometryImpl():OdConstraintGroupNodeImpl()
                              , m_rigidSetIdx(OdConstraintGroupNode::kNullGroupNodeId)
                              , m_origGeomSet(false)
{
  m_idDependency = OdDbObjectId::kNull;
  m_implObjType = ConstrainedGeometryUnknown;

  m_Unk_UInt8 = 1;
}
OdConstrainedGeometryImpl::~OdConstrainedGeometryImpl()
{
}

OdResult OdConstrainedGeometryImpl::init(const OdDbObjectId depId)
{
  OdDbObjectPtr pObj = depId.openObject();
  
  if ( !pObj->isKindOf(OdDbAssocGeomDependency::desc()) )
    return eBadObjType;

  OdDbAssocGeomDependencyPtr pGeomDep = OdDbAssocGeomDependency::cast(pObj);
  if ( !pGeomDep->isDelegatingToOwningAction() )
    return eBadObjType;


  m_idDependency = depId;
  m_groupObjectId = pGeomDep->owningAction();

  return eOk;
}

bool OdConstrainedGeometryImpl::isReadOnly() const
{
  return m_Unk_UInt8 == 0;
}

OdDbObjectId OdConstrainedGeometryImpl::geomDependencyId() const
{
  return m_idDependency;
}

OdConstrainedRigidSet* OdConstrainedGeometryImpl::getOwningRigidSet() const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  if ( pGr.isNull() )
    return NULL;
  OdConstraintGroupNode *pGrNode = pGr->getGroupNodePtr(m_rigidSetIdx);
  
  OdSmartPtr<OdConstrainedRigidSet> pRigidSet = OdConstrainedRigidSet::cast(pGrNode);
  return pRigidSet;
}

OdResult OdConstrainedGeometryImpl::getConnectedConstraints(OdArray<OdGeomConstraint*>& apConstraints) const
{
  OdResult res = eOk;
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  if (pGr.isNull())
    return eNullObjectId;
  for (OdGroupNodeIdArray::const_iterator i = m_connections.begin(); i != m_connections.end(); i++)
  {
    if (OdConstraintGroupNode* n = pGr->getGroupNodePtr(*i))
    {
      if (n->isKindOf(OdGeomConstraint::desc()))
      {
        apConstraints.append((OdGeomConstraint*)n);
      }
    }
  }

	return eOk;
}

OdResult OdConstrainedGeometryImpl::getCommonConstraints(OdConstrainedGeometry* pOtherConsGeom,
                                              OdArray<OdGeomConstraint*>& apConstraints) const
{
  OdResult res = eOk;

  OdArray<OdGeomConstraint*> connToThisConstr;
  if ( (res = getConnectedConstraints(connToThisConstr)) != eOk )
  {
    return res;
  }

  OdArray<OdGeomConstraint*> connToOtherConstr;
  if ( ( res = pOtherConsGeom->getConnectedConstraints(connToOtherConstr)) != eOk )
  {
    return res;
  }
  
  if ( connToThisConstr.size() == 0 || connToOtherConstr.size() == 0 )
  {
    return res;
  }

  for ( unsigned int i = 0; i < connToThisConstr.size(); i++ )
  {
    if ( connToOtherConstr.contains(connToThisConstr[i]) )
    {
      apConstraints.append(connToThisConstr[i]);
    }
  }

	return res;
}


OdResult OdConstrainedGeometryImpl::getConnectedGeometries(OdArray<OdConstrainedGeometry*>& apConsGeoms) const
{
  OdResult res = eOk;
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());

  OdArray<OdGeomConstraint*> apConstraints;
  if ( ( res = getConnectedConstraints(apConstraints)) != eOk )
  {
    return res;
  }

  OdArray<OdConstrainedGeometry*> apConstConsGeom;
  for ( unsigned int i = 0; i < apConstraints.size(); i++ )
  {
    if ( (res = apConstraints[i]->getConnectedGeometries(apConstConsGeom)) != eOk )
    {
      return res;
    }

    for ( unsigned int j = 0; j < apConstConsGeom.size(); j++ )
    {
      if ( apConstConsGeom[j]->nodeId() == nodeId() )
        continue;
      else if ( apConsGeoms.contains(apConstConsGeom[j]) )
        continue;
      else
        apConsGeoms.append(apConstConsGeom[j]);

    }

    apConstConsGeom.clear();
  }

	return res;
}

OdResult OdConstrainedGeometryImpl::getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const
{
  subentPaths.clear();

  return eNotImplementedYet;
}

void OdConstrainedGeometryImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstraintGroupNodeImpl::dwgOutFields(pFiler);
  pFiler->wrSoftPointerId(m_idDependency);
  pFiler->wrInt32(m_rigidSetIdx);
}

OdResult OdConstrainedGeometryImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstraintGroupNodeImpl::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  m_idDependency = pFiler->rdSoftPointerId();
  m_rigidSetIdx = pFiler->rdInt32();

  return eOk;
}

void OdConstrainedGeometryImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstraintGroupNodeImpl::dxfOutFields(pFiler);
  pFiler->wrObjectId(330, m_idDependency);
  pFiler->wrUInt32(90, m_rigidSetIdx);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedGeometryImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstraintGroupNodeImpl::dxfInFields(pFiler);
  if (res != eOk)
    return res;
  NEXT_CODE(330)
  m_idDependency = pFiler->rdObjectId();
  NEXT_CODE(90)
  m_rigidSetIdx = pFiler->rdUInt32();
  return eOk;
}

#undef NEXT_CODE

bool OdConstrainedGeometryImpl::operator==(const OdConstrainedGeometryImpl& geom) const
{
  return this == &geom;
}

bool OdConstrainedGeometryImpl::operator!=(const OdConstrainedGeometryImpl& geom) const
{
  return !operator==(geom);
}

OdResult OdConstrainedGeometryImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId,
                                           const OdGeMatrix3d& matrFromWrd)
{
  OdConstrainedRigidSet* pRS = getOwningRigidSet();
  if ( pRS != NULL )
    pRS->setDataFromGeomDependency(geomDependencyId, matrFromWrd);

  return eOk;
}

OdResult OdConstrainedGeometryImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  OdConstrainedRigidSet* pRS = getOwningRigidSet();
  if ( pRS != NULL )
  {
    OdConstrainedRigidSetImpl *pRsImp = (OdConstrainedRigidSetImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pRS);
    pRsImp->updateDbObject(pObj, pDep, matrToWrd);
    return eOk;
  }

  return eBadObjType;
}

bool OdConstrainedGeometryImpl::isModified() const
{
  return m_origGeomSet;
}

OdDbAssocGeomDependencyPtr OdConstrainedGeometryImpl::geomDependency(const OdDbObjectId geomDepId) const
{
  return OdDbAssocGeomDependency::cast(geomDepId.openObject());
}

OdDbAssocGeomDependencyPtr OdConstrainedGeometryImpl::geomDependency(const OdDbObjectId geomDepId,
                                                      OdDb::OpenMode mode)
{
  return OdDbAssocGeomDependency::cast(geomDepId.openObject(mode));
}

OdDbObjectPtr OdConstrainedGeometryImpl::geomObject(const OdDbObjectId geomDepId, OdDb::OpenMode mode)
{
  OdDbAssocGeomDependencyPtr pGeomDep = geomDependency(geomDepId, OdDb::kForRead);
  if ( pGeomDep.isNull() )
    return OdDbObjectPtr();

/*
  OdDbCompoundObjectIdPtr compoundId;
  pGeomDep->getDependentOnCompoundObject(compoundId);

  OdDbObjectIdArray aObjId;
  compoundId->getFullPath(aObjId);
  
  OdDbObjectId resID = aObjId[aObjId.length() - 1];
*/
  
  OdDbObjectId resID = pGeomDep->dependentOnObject();
  return resID.openObject(mode);

}

void OdConstrainedGeometryImpl::matrToWrdCoord(const OdDbObjectId geomDepId, OdGeMatrix3d &matr)
{
  matr.setToIdentity();

  OdDbAssocGeomDependencyPtr pGeomDep = geomDependency(geomDepId, OdDb::kForRead);
  if ( pGeomDep.isNull() || !pGeomDep->isDependentOnCompoundObject() )
    return;

  OdDbCompoundObjectIdPtr compoundId;
  pGeomDep->getDependentOnCompoundObject(compoundId);
  OdDbObjectIdArray aObjId;

  compoundId->getPath(aObjId);
  OdDbBlockReferencePtr pBlock;
  for ( unsigned int i = 0; i < aObjId.length(); i++ )
  {
    pBlock = aObjId[i].openObject();
    matr.postMultBy(pBlock->blockTransform());
  }
}

OdResult OdConstrainedGeometryImpl::getFullSubentPath(OdDbFullSubentPath& path) const
{
  OdDbAssocGeomDependencyPtr pGeomDep = geomDependency(geomDependencyId());
  if (pGeomDep.isNull())
    return eBadObjType;

  OdDbCompoundObjectIdPtr compoundId;
  pGeomDep->getDependentOnCompoundObject(compoundId);
  OdDbObjectIdArray aObjId;
  compoundId->getFullPath(aObjId);

  OdDbSubentId subentId(pGeomDep->subentType(), 1);
  path = OdDbFullSubentPath(aObjId, subentId);

  return eOk;
}
