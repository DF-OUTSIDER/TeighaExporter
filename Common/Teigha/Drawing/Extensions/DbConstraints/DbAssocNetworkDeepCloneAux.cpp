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

#include "DbAssocNetworkDeepCloneAux.h"
#include "DbAssocNetworkDeepCloneReactor.h"
#include "DbAssocNetworkLongTransactionReactor.h"

#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocNetwork.h"
#include "DbAssocValueDependency.h"
#include "DbImpAssocDependency.h"
#include "OdExpression.h"
#include "OdUnitsFormatter.h"
#include "DbAssocVariable.h"
#include "DbImpAssoc2dConstraintGroup.h"
#include "DbDictionary.h"
#include "OdCharConverter.h"
#include "OdDbAssocPersSubentIdPE.h"
#include "OdImplicitConstr.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"


#define ACAD_ASSOC_TMP_BUCKET OdString(OD_T("ACAD_ASSOC_TMP_BUCKET"))
OdString OdDbAssocNetworkDeepCloneAux::getTempAssocDictionaryName()
{
  return ACAD_ASSOC_TMP_BUCKET;
}

void OdDbAssocNetworkDeepCloneAux::moveCompleteActionToDestNetwork(OdDbAssocAction *pAction, OdDbObjectId destBlockId,
  OdDb::DeepCloneType deepCloneType)
{
  OdDbAssocNetworkPtr pDestParentNetwork = OdDbAssocNetwork::getInstanceFromObject(destBlockId, false).openObject(OdDb::kForRead);
  ODA_ASSERT(!pDestParentNetwork.isNull());

  OdString tempAllActionDictName = OdDbAssocNetworkDeepCloneAux::getTempAssocDictionaryName();
  OdDbObjectId globalDictId = pAction->database()->getNamedObjectsDictionaryId();    
  OdDbDictionaryPtr pGlobalDict = OdDbDictionary::cast(globalDictId.openObject());
  ODA_ASSERT(!pGlobalDict.isNull());
  if (pGlobalDict.isNull())
    return;
  OdDbObjectId tempAllActionDictId = pGlobalDict->getAt(tempAllActionDictName);
  OdDbDictionaryPtr pTempAllActionDict = OdDbDictionary::cast(tempAllActionDictId.openObject(OdDb::kForWrite));
  if (!pTempAllActionDict.isNull())
  {
    pTempAllActionDict->remove(pAction->id());
    pTempAllActionDict.release();
  }
  if (pDestParentNetwork->getActions().contains(pAction->id()))
  {
    //ODA_ASSERT(deepCloneType == OdDb::kDcInsertCopy);
    return;
  }
  pDestParentNetwork->upgradeOpen();
  pDestParentNetwork->addAction(pAction->id(), true);
  pDestParentNetwork->setStatus(kChangedDirectlyAssocStatus);
}


class DimConstraintData : public OdRxObjectImpl<OdRxObject>
{
public:
  ODRX_DECLARE_MEMBERS(DimConstraintData);
  virtual ~DimConstraintData() {};
  virtual bool init(OdGeomConstraint* pConstraint) = 0;
  virtual bool fill(OdDbAssoc2dConstraintGroup * pDestGroup) = 0;
protected:
  bool init(OdExplicitConstraint* pConstraint, unsigned int reqGeomsLength)
  {
    OdArray<OdConstrainedGeometry*> apConsGeoms;
    pConstraint->getConnectedGeometries(apConsGeoms);
    unsigned int consGeomsLength = apConsGeoms.length();
    if (consGeomsLength != reqGeomsLength)
      return false;
    ODA_ASSERT(consGeomsLength <= 3);    
    m_valueDependencyId = pConstraint->valueDependencyId();
    m_dimDependencyId   = pConstraint->dimDependencyId();
    for (unsigned int i=0; i<consGeomsLength; i++)
    {
      OdResult res = apConsGeoms[i]->getFullSubentPaths(m_aSubentPath[i]);
      if (res != eOk)
        return false;
    }
    return true;
  }
  bool getGeometries(OdDbAssoc2dConstraintGroupPtr pGroup, OdConstrainedGeometry * apConsGeom[3], const unsigned int reqGeomsLength)
  {
    ODA_ASSERT(reqGeomsLength <= 3);
    bool bCreateArcLineMid = true;
    for (unsigned int i = 0; i<reqGeomsLength; i++)
    {
      apConsGeom[i] = 0;
      if (eOk != pGroup->getConstrainedGeometry(m_aSubentPath[i][0], apConsGeom[i], bCreateArcLineMid))
        return false;
    }
    return true;
  }
  OdDbObjectId m_valueDependencyId;
  OdDbObjectId m_dimDependencyId;
private:
  OdDbFullSubentPathArray m_aSubentPath[3];
};

ODRX_NO_CONS_DEFINE_MEMBERS(DimConstraintData, OdRxObject);

typedef OdSmartPtr<DimConstraintData> DimConstraintDataPtr;

class DistanceConstraintData : public DimConstraintData
{
public:
  ODRX_DECLARE_MEMBERS(DistanceConstraintData);
  ~DistanceConstraintData() {};
  bool init(OdGeomConstraint* pConstraint)
  {
    OdDistanceConstraint * pDC = (OdDistanceConstraint *) pConstraint;
    if (!DimConstraintData::init(pDC, 2))
      return false;
    m_directionType = pDC->directionType();
    m_fixedDirection = pDC->direction();
    return true;
  }
  bool fill(OdDbAssoc2dConstraintGroup * pDestGroup)
  {
    OdConstrainedGeometry * apConsGeom[2];
    if (!getGeometries(pDestGroup, apConsGeom, 2))
      return false;
    OdGeVector3d * pFixedDirection = (m_directionType == OdDistanceConstraint::kFixedDirection) ?
                                     &m_fixedDirection : NULL;
    OdConstrainedLine* pDirectionLine = NULL;
    OdDistanceConstraint * pConstr = 0;
    pDestGroup->addDistanceConstraint(apConsGeom[0], apConsGeom[1], m_directionType,
                                      m_valueDependencyId, m_dimDependencyId,
                                      pFixedDirection, pDirectionLine, &pConstr);
    return true;
  }
private:
  OdDistanceConstraint::DirectionType m_directionType;
  OdGeVector3d m_fixedDirection;
};

ODRX_CONS_DEFINE_MEMBERS(DistanceConstraintData, DimConstraintData, RXIMPL_CONSTR);

class AngleConstraintData : public DimConstraintData
{
public:
  ODRX_DECLARE_MEMBERS(AngleConstraintData);
  ~AngleConstraintData() {}
  bool init(OdGeomConstraint* pConstraint)
  {
    OdAngleConstraint * pAC = (OdAngleConstraint *) pConstraint;
    if (!DimConstraintData::init(pAC, 2))
      return false;
    m_sectorType = pAC->sectorType();
    return true;
  }
  bool fill(OdDbAssoc2dConstraintGroup * pDestGroup)
  {
    OdConstrainedGeometry * apConsGeom[2];
    if (!getGeometries(pDestGroup, apConsGeom, 2))
      return false;
    OdAngleConstraint * pConstr = 0;
    pDestGroup->addAngleConstraint((OdConstrainedLine*) apConsGeom[0], (OdConstrainedLine*) apConsGeom[1],
                                   m_sectorType,
                                   m_valueDependencyId, m_dimDependencyId,
                                   &pConstr);
    return true;
  }
private:
  OdAngleConstraint::SectorType m_sectorType;
};

ODRX_CONS_DEFINE_MEMBERS(AngleConstraintData, DimConstraintData, RXIMPL_CONSTR);

class ThreePointAngleConstraintData : public DimConstraintData
{
public:
  ODRX_DECLARE_MEMBERS(ThreePointAngleConstraintData);
  ~ThreePointAngleConstraintData() {}
  bool init(OdGeomConstraint* pConstraint)
  {
    Od3PointAngleConstraint * pAC = (Od3PointAngleConstraint *) pConstraint;
    if (!DimConstraintData::init(pAC, 3))
      return false;
    return true;
  }
  bool fill(OdDbAssoc2dConstraintGroup * pDestGroup)
  {
    OdConstrainedGeometry * apConsGeom[3];
    if (!getGeometries(pDestGroup, apConsGeom, 3))
      return false;
    Od3PointAngleConstraint * pConstr = 0;  
    pDestGroup->add3PointAngleConstraint((OdConstrainedPoint *) apConsGeom[0], (OdConstrainedPoint *) apConsGeom[1], (OdConstrainedPoint *) apConsGeom[2],
                                         m_sectorType, m_valueDependencyId, m_dimDependencyId,
                                         &pConstr);
    return true;
  }
private:
  OdAngleConstraint::SectorType m_sectorType;
};

ODRX_CONS_DEFINE_MEMBERS(ThreePointAngleConstraintData, DimConstraintData, RXIMPL_CONSTR);

class RadiusDiameterConstraintData : public DimConstraintData
{
public:
  ODRX_DECLARE_MEMBERS(RadiusDiameterConstraintData);
  ~RadiusDiameterConstraintData() {}
  bool init(OdGeomConstraint* pConstraint)
  {
    OdRadiusDiameterConstraint * pRC = (OdRadiusDiameterConstraint *) pConstraint;
    if (!DimConstraintData::init(pRC, 1))
      return false;
    m_rdType = pRC->constrType();
    return true;
  }
  bool fill(OdDbAssoc2dConstraintGroup * pDestGroup)
  {
    OdConstrainedGeometry * apConsGeom[1];
    if (!getGeometries(pDestGroup, apConsGeom, 1))
      return false;
    OdRadiusDiameterConstraint * pConstr = 0;
    pDestGroup->addRadiusDiameterConstraint(apConsGeom[0], m_rdType,
                                            m_valueDependencyId, m_dimDependencyId,
                                            &pConstr);
    return true;
  }
private:
  OdRadiusDiameterConstraint::RadiusDiameterConstrType m_rdType;
};

ODRX_CONS_DEFINE_MEMBERS(RadiusDiameterConstraintData, DimConstraintData, RXIMPL_CONSTR);

void OdDbAssocNetworkDeepCloneAux::moveDepFromSrcGroup(OdDbAssoc2dConstraintGroup * pSrcGroup, OdDbAssoc2dConstraintGroup * pDestGroup)
{
  // collect constraints
  OdArray<std::pair<OdGeomConstraint::GeomConstraintType, OdDbFullSubentPathArray> > aExistingConstraints;
  OdArray<DimConstraintDataPtr> aExistingDimConstraints;

  OdArray<OdGeomConstraint*> apSrcConstraints;
  pSrcGroup->getConstraints(apSrcConstraints);
  unsigned int constrLength = apSrcConstraints.length();
  for (unsigned int k=0; k<constrLength; k++) {

    OdGeomConstraint* pConstraint = apSrcConstraints[k];
    if (!pConstraint)
      continue;

    // Internal constraints are not to be explicitly created if they are not composite
    if (pConstraint->isInternal())
    {
      continue;
    }

    // Note that order constraints are checked is important due to some constraints derive from others!

    DimConstraintDataPtr pDimData;
    OdGeomConstraint::GeomConstraintType type;

    if (pConstraint->isKindOf(OdHorizontalConstraint::desc())) // Derived from OdParallelConstraint
      type = OdGeomConstraint::kHorizontal;
    else if (pConstraint->isKindOf(OdVerticalConstraint::desc()))
      type = OdGeomConstraint::kVertical;

    else if (pConstraint->isKindOf(OdG2SmoothConstraint::desc())) // Derived from OdCompositeConstraint
      type = OdGeomConstraint::kG2Smooth;

    else if (pConstraint->isKindOf(OdColinearConstraint::desc()))
      type = OdGeomConstraint::kColinear;
    else if (pConstraint->isKindOf(OdConcentricConstraint::desc()))
      type = OdGeomConstraint::kConcentric;
    else if (pConstraint->isKindOf(OdEqualLengthConstraint::desc()))
      type = OdGeomConstraint::kEqualLength;
    else if (pConstraint->isKindOf(OdEqualRadiusConstraint::desc()))
      type = OdGeomConstraint::kEqualRadius;
    else if (pConstraint->isKindOf(OdFixedConstraint::desc()))
      type = OdGeomConstraint::kFix;
    else if (pConstraint->isKindOf(OdNormalConstraint::desc()))
      type = OdGeomConstraint::kNormal;
    else if (pConstraint->isKindOf(OdParallelConstraint::desc())) // Derived from OdGeomConstraint
      type = OdGeomConstraint::kParallel;
    else if (pConstraint->isKindOf(OdPerpendicularConstraint::desc()))
      type = OdGeomConstraint::kPerpendicular;
    else if (pConstraint->isKindOf(OdPointCoincidenceConstraint::desc()))
      type = OdGeomConstraint::kCoincident;  
    else if (pConstraint->isKindOf(OdSymmetricConstraint::desc()))
      type = OdGeomConstraint::kSymmetric;
    else if (pConstraint->isKindOf(OdTangentConstraint::desc()))
      type = OdGeomConstraint::kTangent;    

    else if (pConstraint->isKindOf(Od3PointAngleConstraint::desc()))
      pDimData = ThreePointAngleConstraintData::createObject();
    else  if (pConstraint->isKindOf(OdAngleConstraint::desc()))
      pDimData = AngleConstraintData::createObject();
    else if (pConstraint->isKindOf(OdDistanceConstraint::desc())) // Derived from OdExplicitConstraint
      pDimData = DistanceConstraintData::createObject();
    else if (pConstraint->isKindOf(OdRadiusDiameterConstraint::desc()))
      pDimData = RadiusDiameterConstraintData::createObject();

    if (!pDimData.isNull())
    {
      if (!pDimData->init(pConstraint))
        continue;
      aExistingDimConstraints.append(pDimData);
      continue;
    }

    OdArray<OdConstrainedGeometry*> apConsGeoms;
    pConstraint->getConnectedGeometries(apConsGeoms);

    OdDbFullSubentPathArray subentPaths;
    unsigned int consGeomLength = apConsGeoms.length();
    if (consGeomLength)
    {
      for (unsigned int l=0; l<consGeomLength; l++) {
        OdDbFullSubentPathArray subentPath;
        OdConstrainedGeometry * pGeom = apConsGeoms[l];
        pGeom->getFullSubentPaths(subentPath);
        if (subentPath.length() > 0)
          subentPaths.append(subentPath[0]);    
      }
    }
    else
    {
      if (pConstraint->isKindOf(OdCompositeConstraint::desc()))
      {
        OdCompositeConstraint * pCG = OdCompositeConstraint::cast(pConstraint);
        OdArray<OdGeomConstraint*> apSubConstraints;
        pCG->getOwnedConstraints(apSubConstraints);
        for (unsigned int k=0; k<apSubConstraints.length(); k++)
        {
          OdGeomConstraint * pSubConstr = apSubConstraints[k];
          OdArray<OdConstrainedGeometry*> apSubCompConsGeoms;
          pSubConstr->getConnectedGeometries(apSubCompConsGeoms);

          bool metOnlyImplicitGeometry = false;
          for (unsigned int l=0; l<apSubCompConsGeoms.length(); l++)
          {
            OdConstrainedGeometry *pCG = apSubCompConsGeoms[l];
            if (!pCG->isKindOf(OdConstrainedImplicitPoint::desc()))
            {
              metOnlyImplicitGeometry = false;
              break;
            }
            metOnlyImplicitGeometry = true;
            OdDbFullSubentPathArray subentPath;
            pCG->getFullSubentPaths(subentPath);
            if (subentPath.length() > 0)
              subentPaths.append(subentPath[0]);
          }
          if (!metOnlyImplicitGeometry)
            subentPaths.clear();
          else if (subentPaths.length())
            break;
        }
      }
    }

    aExistingConstraints.append(std::pair<OdGeomConstraint::GeomConstraintType, OdDbFullSubentPathArray> (type, subentPaths));
  }

  // move dependencies
  unsigned int i;
  OdDbObjectIdArray aSrcDependencyIds;
  pSrcGroup->getDependencies(true, true, aSrcDependencyIds);
  unsigned int srcDepLength = aSrcDependencyIds.length();
  for (i=0; i<srcDepLength; i++) {

    OdDbObjectId depId = aSrcDependencyIds[i];
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(depId.openObject());
    ODA_ASSERT(!pDep.isNull());
    if (pDep.isNull())
      continue;

    OdDbCompoundObjectIdPtr compId = OdDbCompoundObjectId::createObject();
    pDep->getDependentOnCompoundObject(compId);
    OdDbObjectId objectId = compId->leafId();

    OdDbEntityPtr pObject = OdDbEntity::cast(objectId.openObject(OdDb::kForRead));
    OdSmartPtr<OdDbAssocPersSubentIdPE> persSubentIdPE = OdDbAssocPersSubentIdPE::cast(pObject);
    if (persSubentIdPE.isNull())
      continue;
#ifdef BS_CHANGES_ENABLED
    persSubentIdPE->setAssocGeomDependency(NULL);
#endif
    OdDbAssocGeomDependencyPtr pGeomDep = OdDbAssocGeomDependency::cast(pDep);
    if (!pGeomDep.isNull())
    {
      OdArray<OdDbSubentId> aSubentIds;
      persSubentIdPE->getTransientSubentIds(pObject, pGeomDep->persistentSubentId(), aSubentIds);
      if (aSubentIds.size() < 1)
      {
#ifdef BS_CHANGES_ENABLED
        const OdDb::SubentType subentType = (pGeomDep->persistentSubentId().isNull())
          ? OdDb::kEdgeSubentType
          : pGeomDep->persistentSubentId()->subentType(pObject);
        persSubentIdPE->getAllSubentities(pObject, subentType, aSubentIds);
#else
        persSubentIdPE->getAllSubentities(pObject, OdDb::kEdgeSubentType, aSubentIds);
#endif

        if (aSubentIds.size() < 1)
        {
          pSrcGroup->removeDependency(depId, true);
          continue;
        }
      }
      persSubentIdPE.release();

      OdDbObjectIdArray fullPath;
      compId->getFullPath(fullPath);

      OdDbFullSubentPath fsPath(fullPath, aSubentIds[0]); 

      pSrcGroup->removeDependency(depId, false);

      OdConstrainedGeometry* pGeom = 0;
      OdDbImpAssoc2dConstraintGroup * pDestImp = (OdDbImpAssoc2dConstraintGroup *) OdDbImpAssocAction::getImpObject(pDestGroup);
      pGeomDep->upgradeOpen();
      pDestGroup->assertWriteEnabled();
      pDestImp->moveConstrainedGeometry(fsPath, pGeom, pGeomDep, pDestGroup);

      continue;
    }

    // When we move from one group to another, dependency loses dependent on object
    OdDbObjectId depOnObject = pDep->dependentOnObject();

    pSrcGroup->removeDependency(depId, false);
    pDestGroup->addDependency(depId, true);

    OdDbObjectId depOnObjectAfterRemoveId = pDep->dependentOnObject();
    // Check that code follows current group implementation which erases dependent on object information
    ODA_ASSERT(depOnObjectAfterRemoveId.isNull());    

    OdDbCompoundObjectIdPtr dimCId = OdDbCompoundObjectId::createObject();
    dimCId->set(depOnObject, depOnObject.database());
    pDep->attachToObject(dimCId);

  }

  OdArray<std::pair<OdGeomConstraint::GeomConstraintType, OdDbFullSubentPathArray> >::iterator it = aExistingConstraints.begin();
  for (; it != aExistingConstraints.end(); ++it)
  {
    OdGeomConstraint* ppNewConstraint = 0;
#ifdef BS_CHANGES_ENABLED
    const OdResult addConstraintRes = pDestGroup->addGeometricalConstraint(it->first, it->second, &ppNewConstraint);
    ODA_ASSERT(addConstraintRes == eOk);
#else
    pDestGroup->addGeometricalConstraint(it->first, it->second, &ppNewConstraint);
#endif
  }

  OdArray<DimConstraintDataPtr>::iterator dimIt = aExistingDimConstraints.begin();
  bool res;
  for (; dimIt != aExistingDimConstraints.end(); ++dimIt)
  {
    res = (*dimIt)->fill(pDestGroup);
    ODA_ASSERT(res);
  }

}

struct ReactorKeeper
{
  OdDbAssocNetworkDeepCloneReactor m_assocNetworkDeepCloneReactor;
  OdDbAssocNetworkLongTransactionReactor m_assocNetworkLongTransactionReactor;
};

ReactorKeeper * gpReactorKeeper = 0;
void OdDbAssocNetworkDeepCloneAux::createAssocNetworkCloneReactors()
{
  DimConstraintData::rxInit();
  DistanceConstraintData::rxInit();
  AngleConstraintData::rxInit();
  ThreePointAngleConstraintData::rxInit();
  RadiusDiameterConstraintData::rxInit();
  OdDbAssocNetworkObjectModificationReactor::rxInit();
  gpReactorKeeper = new ReactorKeeper;
  gpReactorKeeper->m_assocNetworkDeepCloneReactor.addRefReactor();
  gpReactorKeeper->m_assocNetworkLongTransactionReactor.addRefReactor();
}

void OdDbAssocNetworkDeepCloneAux::deleteAssocNetworkCloneReactors()
{
  gpReactorKeeper->m_assocNetworkDeepCloneReactor.releaseReactor();
  gpReactorKeeper->m_assocNetworkLongTransactionReactor.releaseReactor();
  delete gpReactorKeeper;
  gpReactorKeeper = 0;
  OdDbAssocNetworkObjectModificationReactor::rxUninit();
  DistanceConstraintData::rxUninit();
  AngleConstraintData::rxUninit();
  ThreePointAngleConstraintData::rxUninit();
  RadiusDiameterConstraintData::rxUninit();
  DimConstraintData::rxUninit();
}

std::map<OdDbObjectId, OdDbObjectId> * OdDbAssocNetworkDeepCloneAux::srcToDestIdMap(OdDbIdMapping& idMap)
{
  OdDbAssocNetworkDeepCloneReactorDataPtr pData = gpReactorKeeper->m_assocNetworkDeepCloneReactor.getData(idMap);
  return pData ? &pData->m_dcMapping.m_deepCloneMapping : (std::map<OdDbObjectId, OdDbObjectId> *) 0;
}

void OdDbAssocNetworkDeepCloneAux::cleanData(OdDbIdMapping& idMap)
{
  gpReactorKeeper->m_assocNetworkDeepCloneReactor.deleteData(idMap);
}

void OdDbAssocNetworkDeepCloneAux::removeIncompleteConstraints(
  OdDbObjectId srcGroupId, OdDbIdMapping& idMap)
{
  OdDbIdPair destGroupIdSearch(srcGroupId);
  if (!idMap.compute(destGroupIdSearch) || !destGroupIdSearch.isCloned() || destGroupIdSearch.value().isErased())
    return;

  OdDbAssoc2dConstraintGroupPtr pSrcGroup = OdDbAssoc2dConstraintGroup::cast(srcGroupId.openObject());

  OdDbAssoc2dConstraintGroupPtr pDestGroup = OdDbAssoc2dConstraintGroup::cast(destGroupIdSearch.value().openObject());
  // We check constraint of src group and choose the ones which were not mapped completely
  // There is index 1-1 correspondence between source and destination constraints

  // If src group has no constraints, do nothing
  OdArray<OdGeomConstraint*> apSrcConstraints;
  if (eOk != pSrcGroup->getConstraints(apSrcConstraints))
  {
    return;
  }

  bool removeFixConstraints = false;
  const OdDb::DeepCloneType dcType = idMap.deepCloneContext();
  if ((OdDb::kDcCopy    == dcType) ||
      (OdDb::kDcExplode == dcType)   )
  {
    removeFixConstraints = true;
  }

  if (removeFixConstraints)
  {
    OdArray<OdGeomConstraint*> apDestConstraints;
    if (eOk != pDestGroup->getConstraints(apDestConstraints))
    {
      return;
    }
    pDestGroup->upgradeOpen();
    unsigned int i, gcLength = apDestConstraints.length();
    for (i=0; i<gcLength; i++)
    {
      OdGeomConstraint * pConstraint = apDestConstraints[i];
      if (pConstraint->isKindOf(OdFixedConstraint::desc()))
      {
        pDestGroup->deleteConstraint(apDestConstraints[i]);
      }
    }
    if (pDestGroup->isWriteEnabled())
    {
      pDestGroup->downgradeOpen();
    }
  }

  OdDbObjectIdArray aSrcDepIds;
  pSrcGroup->getDependencies(true, true, aSrcDepIds);

  OdDbObjectIdArray aDestDepToRemove;
  unsigned int i, srcDepLength = aSrcDepIds.length();
  for (i=0; i<srcDepLength; i++)
  {
    OdDbObjectId srcDepId = aSrcDepIds[i];
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(srcDepId.openObject());
    // Null dependency is for internal objects
    if (pDep.isNull())
    {
      continue;
    }
    OdDbObjectId depOnObjectId = pDep->dependentOnObject();
    pDep = 0;

    bool foundObjectWhichIsNotCloned = false;
    OdDbIdPair depOnObjectIdSearch(depOnObjectId);
    if (!idMap.compute(depOnObjectIdSearch) || !depOnObjectIdSearch.isCloned())
    {
      foundObjectWhichIsNotCloned = true;
    }
    OdDbObjectPtr pObject = depOnObjectIdSearch.value().openObject();
    if (pObject.isNull())
    {
      foundObjectWhichIsNotCloned = true;
    }
    pObject = 0;

    OdDbIdPair depIdSearch(srcDepId);
    if (!idMap.compute(depIdSearch) || !depIdSearch.isCloned())
    {
      continue;
    }

    if (foundObjectWhichIsNotCloned)
    {
      aDestDepToRemove.append(depIdSearch.value());
      continue;
    }

    OdDbAssocGeomDependencyPtr pDestDep = OdDbAssocGeomDependency::cast(depIdSearch.value().openObject());
    if (pDestDep.isNull())
      continue;
    OdConstrainedGeometry * pConsGeom = 0;
    pDestGroup->getConstrainedGeometry(pDestDep, pConsGeom);
    pDestDep = 0;
    if (!pConsGeom)
    {
      aDestDepToRemove.append(depIdSearch.value());
      continue;
    }
  }

  aDestDepToRemove.erase(std::unique(aDestDepToRemove.begin(), aDestDepToRemove.end()), aDestDepToRemove.end());

  pDestGroup->upgradeOpen();
  OdDbImpAssoc2dConstraintGroup* pGroupImp = (OdDbImpAssoc2dConstraintGroup*)OdDbImpAssoc2dConstraintGroup::getImpObject(pDestGroup);
  pDestGroup->assertWriteEnabled();
  pGroupImp->deleteConstrainedGeometry(aDestDepToRemove);
  if (pGroupImp->isEmpty())
  {
    OdDbAssocNetworkPtr net = pGroupImp->owningNetwork().openObject(OdDb::kForWrite);
    if (!net.isNull()) {
      net->removeAction(pDestGroup->objectId(), false);
      net->setStatus(kChangedDirectlyAssocStatus);
    }
    pDestGroup->erase();
  }
  pDestGroup = 0;
}

class OdDbObjectIdList
{
public:
  OdDbObjectIdArray& ids() { return m_array; }
  bool contains(OdDbObjectId val) { return m_set.find(val) != m_set.end(); }
  void append(OdDbObjectId val) { if (!contains(val)) { m_set.insert(val); m_array.push_back(val); } }
private:
  OdDbObjectIdArray m_array;
  std::set<OdDbObjectId> m_set;
};

static void collectVariables(OdDbAssocAction* pAction, OdDbObjectIdList& varsQueue)
{
  if (!pAction)
    return;

  OdDbObjectIdArray deps;
  pAction->getDependencies(true, true, deps);

  for (unsigned int j = 0; j < deps.size(); j++)
  {
    OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(deps[j].openObject());

    if (!pDep.isNull())
      varsQueue.append(pDep->dependentOnObject());
  }
}

void OdDbAssocNetworkDeepCloneAux::clearIncompleteExpressions(const OdDbObjectIdArray& aSrcActions,
  int copyMode,
  OdDbIdMapping& idMap)
{
  for (unsigned int i = 0; i < aSrcActions.size(); i++)
  {
    OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(aSrcActions[i].openObject());

    if (pVar.isNull())
      continue;

    OdDbObjectIdList usedVars;
    collectVariables(pVar, usedVars);
    const OdDbObjectIdArray& aUsedVars = usedVars.ids();
    bool convertToConst = (copyMode < 2 && !aUsedVars.isEmpty());
    bool lostParams = convertToConst;
    int varCount = aUsedVars.size();
    OdExpression::Parser p;

    if (copyMode > 1)
    {
      for (unsigned int j = 0; j < aUsedVars.size(); j++)
      {
        OdDbAssocVariablePtr pParam = OdDbAssocVariable::cast(aUsedVars[j].openObject());

        if (pParam.isNull())
          continue;

        OdDbIdPair varPair(pParam->objectId());

        if (!idMap.compute(varPair) || !varPair.isCloned() || varPair.value().isNull() || varPair.value().isErased())
        {
          varCount--;
          lostParams = true;

          if (p.fixVariable(pParam->name(), pParam->value()) != eOk)
          {
            convertToConst = true;
            break;
          }
        }
      }
    }

    if (varCount == 0)
      convertToConst = true;

    if (lostParams)
    {
      if (!convertToConst && p.setExpression(pVar->expression()) != eOk)
        convertToConst = true;

      OdDbIdPair varPair(aSrcActions[i]);

      if (idMap.compute(varPair) && varPair.isCloned() && !varPair.value().isErased())
      {
        OdDbAssocVariablePtr pDestVar = OdDbAssocVariable::cast(varPair.value().openObject(OdDb::kForWrite));

        if (pDestVar.isNull())
          continue;

        if (convertToConst)
        {
          pDestVar->removeAllDependencies(true);
          pDestVar->setExpression(OdExpression::createExpression(pVar->value()), OdString::kEmpty, false, false);
        }
        else
        {
          OdDbObjectIdArray deps;
          pDestVar->getDependencies(true, true, deps);

          for (unsigned int j = 0; j < deps.size(); j++)
          {
            OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(deps[j].openObject());

            if (!pDep.isNull() && pDep->dependentOnObject().isErased())
            {
              pDep = 0;
              pDestVar->removeDependency(deps[j], true);
            }
          }

          pDestVar->setExpression(p.getTranslated(), OdString::kEmpty, false, false);
        }
      }
    }
  }
}

OdDbObjectIdArray OdDbAssocNetworkDeepCloneAux::getUsedActions(const OdDbObjectIdArray& aSrcActions, int copyMode)
{
  OdDbObjectIdArray res;
  OdDbObjectIdList varsQueue;

  for (unsigned int i = 0; i < aSrcActions.size(); i++)
  {
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast(aSrcActions[i].openObject());

    if (pAction.isNull() || pAction->isA() == OdDbAssocVariable::desc())
      continue;
#ifdef BS_CHANGES_ENABLED //vladimirv, reported: OdaBugZilla:21998
    // ignore empty/invalid actions without any references to objects.
    OdDbObjectIdArray depIds;
    if (pAction->getDependentObjects(true, true, depIds) == eOk)
    {
      bool hasDepObj = false;
      for (unsigned int j = 0; j < depIds.size(); j++)
      {
        if (!depIds[j].isNull())
        {
          hasDepObj = true;
          break;
        }
      }
      if (!hasDepObj)
        continue;
    }
#endif

#ifdef BS_CHANGES_ENABLED // #8283-eugener
    if (pAction->isA() == OdDbAssocAction::desc() && pAction->actionBody())
      collectVariables(pAction, varsQueue);
    else
#endif
    if (pAction->isA() == OdDbAssoc2dConstraintGroup::desc())
    {
      if (copyMode == 0)
        continue;

      collectVariables(pAction, varsQueue);
    }

    res.push_back(aSrcActions[i]);
  }

  if (copyMode > 1)
  {
    for (unsigned int i = 0; i < varsQueue.ids().size(); i++)
    {
      collectVariables(OdDbAssocAction::cast(varsQueue.ids()[i].openObject()), varsQueue);
    }
  }

  res.append(varsQueue.ids());
  return res;
}

static void collectNames(const OdDbObjectIdArray& aActions, std::map<OdString, OdDbObjectId>& varNames, bool useNullId)
{
  for (unsigned int i = 0; i < aActions.size(); i++)
  {
    OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(aActions[i].openObject());

    if (pVar.isNull())
      continue;

    OdString nameKey = pVar->name();
    nameKey.makeUpper();

    if (!useNullId)
      varNames[nameKey] = pVar->objectId();
    else if (varNames.find(nameKey) == varNames.end())
      varNames[nameKey] = OdDbObjectId::kNull;
  }
}

void OdDbAssocNetworkDeepCloneAux::mergeVariables(const OdDbObjectIdArray& aSrcActions,
  int copyMode,
  OdDbIdMapping& idMap,
  OdDbObjectId destNetworkId)
{
  std::map<OdString, OdDbObjectId> varNames;
  OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::cast(destNetworkId.openObject());
  bool isFinalDest = (!pNetwork.isNull() || idMap.deepCloneContext() == OdDb::kDcCopy);

  if (!pNetwork.isNull())
    collectNames(pNetwork->getActions(), varNames, false);

  pNetwork = 0;
  collectNames(aSrcActions, varNames, true);
  OdDbObjectIdList aIdsToDelete;
  OdDbObjectIdList aActionsToDelete;

  for (unsigned int i = 0; i < aSrcActions.size(); i++)
  {
    OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(aSrcActions[i].openObject());

    if (pVar.isNull())
      continue;

    OdString name = pVar->name();
    pVar = 0;
    OdDbIdPair varPair(aSrcActions[i]);

    if (!idMap.compute(varPair) || !varPair.isCloned())
      continue;

    OdDbAssocVariablePtr pDestVar = OdDbAssocVariable::cast(varPair.value().openObject());

    if (pDestVar.isNull())
      continue;

    OdDbObjectId sameVarId;
    bool isAnonimous = (name.isEmpty() || name[0] == OdChar('*'));

    OdString nameKey = name;
    if (!isAnonimous)
    {
      std::map<OdString, OdDbObjectId>::iterator it = varNames.find(nameKey.makeUpper());
      if (it != varNames.end())
        sameVarId = it->second;
    }

    if (pDestVar->objectId() == sameVarId)
#ifdef BS_CHANGES_ENABLED //egore, reported: OdaBugZilla:22946
      continue;
#else
    {
      ODA_ASSERT(idMap.deepCloneContext() == OdDb::kDcInsertCopy);
      continue;
    }
#endif
    bool isNameCollision = !sameVarId.isNull();
    bool isDifferentValue = false;

    if (isNameCollision && copyMode == 4)
    {
      OdDbAssocVariablePtr pSameVar = OdDbAssocVariable::cast(sameVarId.openObject());
      isDifferentValue = (!pSameVar.isNull() && *(pSameVar->value().get()) != *(pDestVar->value().get()));
    }

    bool isOnlyExpr = true;
    bool hasDependency = false;
    OdDbImpAssocDependency::repairDependencies(pDestVar.get());
    OdDbObjectIdArray reactors = pDestVar->getPersistentReactors();
    pDestVar = 0;

    for (unsigned int j = 0; isOnlyExpr && j < reactors.size(); j++)
    {
      OdDbObjectPtr pObj = reactors[j].openObject();

      if (pObj.isNull() || pObj->isA() == OdDbDictionary::desc())
        continue;

      OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(pObj);

      if (!pDep.isNull())
      {
        if (aIdsToDelete.contains(pDep->owningAction()))
          continue;
#ifdef BS_CHANGES_ENABLED // #8412-eugener
        if (pDep->owningAction().isErased())
          continue;

#endif
        hasDependency = true;
        OdDbObjectPtr pVar = pDep->owningAction().openObject();

        if (!pVar.isNull() && pVar->isA() == OdDbAssocVariable::desc())
          continue;
      }

      isOnlyExpr = false;
    }

    bool isSameVarOnlyExpr = (isNameCollision && !isDifferentValue && !isOnlyExpr);
    OdDbObjectIdArray sameVarReactors;
    if (isSameVarOnlyExpr)
    {
      sameVarReactors = sameVarId.openObject()->getPersistentReactors();
      for (unsigned int j = 0; j < sameVarReactors.size(); j++)
      {
        OdDbObjectPtr pObj = sameVarReactors[j].openObject();
        if (pObj.isNull() || pObj->isA() == OdDbDictionary::desc())
          continue;
        OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(pObj);
        if (!pDep.isNull() && !OdDbAssocVariable::cast(pDep->owningAction().openObject()).isNull())
          continue;
        isSameVarOnlyExpr = false;
        break;
      }
    }

    bool doRename = (isDifferentValue || (isNameCollision && !isOnlyExpr && !isSameVarOnlyExpr));
    bool doErase = (isOnlyExpr && ((!isNameCollision && copyMode == 2 && isFinalDest) || !hasDependency));
    bool doReplace = (isOnlyExpr && isNameCollision && !isDifferentValue);
    bool doSwap = (isSameVarOnlyExpr && isNameCollision && !isDifferentValue);

    if (doRename)
    {
      OdString prefix = name;
      prefix.trimRight(OD_T("0123456789"));
      OdString suffix = name.mid(prefix.getLength());
      int i = (varNames.size() > 99) ? int(varNames.size()) : 0;

      if (suffix.getLength() > 7)
        prefix = name;
      else if (i == 0 && !suffix.isEmpty())
        i = OdUnitsFormatterTool::toInt(suffix);

      do
      {
        nameKey = name.format(OD_T("%ls%d"), prefix.c_str(), ++i);
      } while (varNames.find(nameKey.makeUpper()) != varNames.end());

      OdDbAssocVariablePtr pDestVar = OdDbAssocVariable::cast(varPair.value().openObject(OdDb::kForWrite));

      if (!pDestVar.isNull() && (pDestVar->setName(name, true) == eOk))
        varNames[nameKey] = varPair.value();
      else
      {
        ODA_FAIL();
      }
    }
    else if (doSwap)
    {
      for (unsigned int j = 0; j < sameVarReactors.size(); j++)
      {
        OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(sameVarReactors[j].openObject(OdDb::kForWrite));

        if (!pDep.isNull())
        {
          pDep->detachFromObject();
          OdDbCompoundObjectIdPtr varCId = OdDbCompoundObjectId::createObject();
          varCId->set(varPair.value());
          pDep->attachToObject(varCId);
        }
      }

      aActionsToDelete.append(sameVarId);
    }
    else if (doErase || doReplace)
    {
      if (doReplace)
      {
        for (unsigned int j = 0; j < reactors.size(); j++)
        {
          OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(reactors[j].openObject(OdDb::kForWrite));

          if (!pDep.isNull())
          {
            pDep->detachFromObject();
            OdDbCompoundObjectIdPtr varCId = OdDbCompoundObjectId::createObject();
            varCId->set(sameVarId);
            pDep->attachToObject(varCId);
          }
        }
      }

      aIdsToDelete.append(varPair.value());
    }
  }

  for (unsigned int i = 0; i < aIdsToDelete.ids().size(); i++)
  {
    OdDbObjectPtr pObj = aIdsToDelete.ids()[i].openObject(OdDb::kForWrite);

    if (!pObj.isNull())
      pObj->erase();
  }

  for (unsigned int i = 0; i < aActionsToDelete.ids().size(); i++)
  {
    if (pNetwork.isNull())
      pNetwork = OdDbAssocNetwork::cast(destNetworkId.openObject(OdDb::kForWrite));
    ODA_ASSERT(!pNetwork.isNull());
    if (!pNetwork.isNull())
      pNetwork->removeAction(aActionsToDelete.ids()[i], true);
  }
}

#ifdef BS_CHANGES_ENABLED

void OdDbAssocNetworkDeepCloneUtils::setNotModified2dConstraints(OdDbDatabase* pDb)
{
  if (!pDb)
    return;
  OdDbObjectId netId = OdDbAssocNetwork::getInstanceFromObject(pDb->getModelSpaceId(), false);
  OdDbAssocNetworkPtr pNet = OdDbAssocNetwork::cast(netId.openObject());
  if (pNet.isNull())
    return;
  OdDbObjectIdArray aActions = pNet->getActions();
  for (unsigned int i = 0; i < aActions.size(); i++)
  {
    OdDbAssoc2dConstraintGroupPtr pGroup = OdDbAssoc2dConstraintGroup::cast(aActions[i].openObject());
    if (!pGroup.isNull())
      ((OdDbImpAssoc2dConstraintGroup*)OdDbImpAssocAction::getImpObject(pGroup))->setNotModified();
  }
}

static OdDbObjectId s_explodedBlockRefId;

void OdDbAssocNetworkDeepCloneUtils::setExplodedBlockRefId(const OdDbObjectId& blockRefId)
{
  s_explodedBlockRefId = blockRefId;
}

OdDbObjectId OdDbAssocNetworkDeepCloneUtils::getExplodedBlockRefId()
{
  return s_explodedBlockRefId;
}
#endif

