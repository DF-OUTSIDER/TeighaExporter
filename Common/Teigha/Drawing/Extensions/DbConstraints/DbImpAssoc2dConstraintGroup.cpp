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
#include "AssocActionPE.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbImpAssoc2dConstraintGroup.h"
#include "DbAssocGeomDependency.h"
#include "DbAssocValueDependency.h"
#include "DbAssocDimDependencyBodyBase.h"
#include "DbAssocNetworkDeepCloneAux.h"
#include "DbAssocVariable.h"
#include "DbAssocNetwork.h"
#include "OdConstraintGroupNodeImpl.h"
#include "OdCompositeConstraintImpl.h"
#include "OdDistanceConstraintImpl.h"
#include "OdImplicitConstr.h"
#include "OdExplicitConstr.h"
#include "DbFiler.h"

#include "DbIdMapping.h"
#include "OdConstrainedConstructionLineImpl.h"
#include "OdParallelConstraintImpl.h"
#include "OdDbAssocCurvePersSubentIdPE.h"

#include "DbSpline.h"
#include "StringArray.h"

#define STL_USING_ALGORITHM
#define STL_USING_SET
#include "OdaSTL.h"

#include "DbAudit.h"
#include "DbHostAppServices.h"

OdDbImpAssoc2dConstraintGroup::OdDbImpAssoc2dConstraintGroup()
  : OdDbImpAssocAction()
  , m_plane()
  , m_seedId(0)
  , m_Unk_Bool(false)
{
  m_isBase = false;
}

OdDbImpAssoc2dConstraintGroup::~OdDbImpAssoc2dConstraintGroup()
{
  m_mapNodes.clear();
}

OdResult OdDbImpAssoc2dConstraintGroup::setWorkPlane(const OdGePlane plane)
{
  m_plane = plane;
  return eOk;
}

const OdGePlane OdDbImpAssoc2dConstraintGroup::getWorkPlane() const
{
  return m_plane;
}


#if 0
OdResult OdDbImpAssoc2dConstraintGroup::transformActionBy(const OdGeMatrix3d& transform);
{
  throw OdError(eNotImplemented);
  return eNotImplemented;
}

int OdDbImpAssoc2dConstraintGroup::getDOF() const
{
  throw OdError(eNotImplemened);
  return 1;
}

SolutionStatus OdDbImpAssoc2dConstraintGroup::solutionStatus(bool bAlsoCheckForConstraints) const
{
  throw OdError(eNotImplemented);
  return eNotAvailable;
}

SolutionStatus OdDbImpAssoc2dConstraintGroup::geometryStatus(const OdConstrainedGeometry* pConsGeom) const
{
  throw OdError(eNotImplemented);
  return eNotAvailable;
}

SolutionStatus OdDbImpAssoc2dConstraintGroup::constraintStatus(const OdGeomConstraint* pConstraint) const
{
  throw OdError(eNotImplemented);
  return eNotAvailable;
}
#endif

void OdDbImpAssoc2dConstraintGroup::onAddConstrainedGeometry(const OdDbFullSubentPath& subentPath,
  OdConstrainedGeometry* pConsGeom)
{
  // check if edge of polyline was added
  OdDbObjectId objId = subentPath.objectIds().last();
  OdDbObjectPtr pObj = objId.openObject();
  if ( pObj->isKindOf(OdDbPolyline::desc()) && subentPath.subentId().type() == OdDb::kEdgeSubentType ) {
    // look for other adjacent edges of polyline and add coincidence constraint between corresponding vertexes
    // collect constrained implicit points of new created edge
    OdConstrainedCurve* pConsGeomCurve = OdConstrainedCurve::cast(pConsGeom);
    OdArray<OdConstrainedImplicitPoint*> aConsGeomPts;
    pConsGeomCurve->getConstrainedImplicitPoints(aConsGeomPts);
    // consider all constrained geometries in the constraint group
    OdArray<OdConstrainedGeometry*> aConstGeoms;
    getConstrainedGeometries(aConstGeoms);
    for ( unsigned int i = 0; i < aConstGeoms.length(); ++i ) {
      OdConstrainedCurve* pCurve = OdConstrainedCurve::cast(aConstGeoms[i]);
      if (!pCurve || pCurve == pConsGeomCurve)
        continue;
      OdDbFullSubentPathArray aFullSubentPaths;
      pCurve->getFullSubentPaths(aFullSubentPaths);
      if (aFullSubentPaths.empty())
        continue;
      OdDbFullSubentPath path = aFullSubentPaths[0];
      if (path.objectIds().last() == objId) {
        // edge from the same polyline was found, obtain its constrained implicit points
        OdArray<OdConstrainedImplicitPoint*> aPts;
        pCurve->getConstrainedImplicitPoints(aPts);
        // find constrained implicit points that have the same FullSubentPath
        // and create coincidence constraint between them
        for ( unsigned int j = 0; j < aPts.length(); ++j ) {
          aPts[j]->getFullSubentPaths(aFullSubentPaths);
          OdDbFullSubentPath ptPath = aFullSubentPaths[0];
          for ( unsigned int k = 0; k < aConsGeomPts.length(); ++k ) {
            aConsGeomPts[k]->getFullSubentPaths(aFullSubentPaths);
            if (aFullSubentPaths[0] == ptPath) {
              OdSmartPtr<OdPointCoincidenceConstraint> pCo = OdPointCoincidenceConstraint::createObject();
              tieNode(pCo, pCurve->owningConstraintGroupId());
              tieGeomsAndCnstr(aPts[j], aConsGeomPts[k], pCo);
            }
          }
        }
      }
    }
  }
}

OdResult OdDbImpAssoc2dConstraintGroup::moveConstrainedGeometry(
  const OdDbFullSubentPath& subentPath, OdConstrainedGeometry*& pConsGeom,
  OdDbAssocGeomDependency * pExistingDep, OdDbAssoc2dConstraintGroup *pGroup)
{
  if ( getConstrainedGeometry(subentPath, pConsGeom, true) == eOk )
    return eOk;

  OdDbCompoundObjectIdPtr pCmpObj = OdDbCompoundObjectId::createObject();
  pCmpObj->setFullPath(subentPath.objectIds());

  pExistingDep->setOrder(10000);
  if (pExistingDep->attachToObject(pCmpObj) != eOk)
  {
    return eBadObjType;
  }
  
  if (pExistingDep->setSubentity(subentPath.subentId()) != eOk)
  {
    return eBadObjType;
  }

  if (pExistingDep->isWriteEnabled())
    pExistingDep->downgradeOpen();
  addDependency(pExistingDep->objectId(), true, pGroup);

  pConsGeom = addConstrainedGeometry(pExistingDep);

  onAddConstrainedGeometry(subentPath, pConsGeom);

  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::addConstrainedGeometry(const OdDbFullSubentPath& subentPath, OdConstrainedGeometry*& pConsGeom,
                                                               OdDbAssoc2dConstraintGroup *pGroup)
{
  if ( getConstrainedGeometry(subentPath, pConsGeom, true) == eOk )
    return eOk;

  OdDbCompoundObjectIdPtr pCmpObj = OdDbCompoundObjectId::createObject();
  pCmpObj->setFullPath(subentPath.objectIds());

  OdDbAssocGeomDependencyPtr pGeomDep = OdDbAssocGeomDependency::createObject();
  pGeomDep->setOrder(10000);
  
  OdDbObjectId geomDepId = pGroup->database()->addOdDbObject(pGeomDep);

  if ( pGeomDep->attachToObject(pCmpObj) != eOk )
  {
    pGeomDep->erase();
    return eBadObjType;
  }
  
  if ( pGeomDep->setSubentity(subentPath.subentId()) != eOk )
  {
    pGeomDep->erase();
    return eBadObjType;
  }

  if ( pGeomDep->isWriteEnabled() )
    pGeomDep->downgradeOpen();
  addDependency(geomDepId, true, pGroup);
  pConsGeom = addConstrainedGeometry(pGeomDep);
  onAddConstrainedGeometry(subentPath, pConsGeom);

  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteConstrainedGeometry(const OdDbObjectIdArray& aDestDepToRemove)
{
  std::map<OdDbObjectId, OdConstrainedGeometry*> aConstGeom;
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    if (pNode->second->isKindOf(OdConstrainedGeometry::desc()))
      aConstGeom[((OdConstrainedGeometry*)pNode->second.get())->geomDependencyId()] = (OdConstrainedGeometry*)(pNode->second.get());
  }
  OdArray<OdGeomConstraint*> aConstraints;
  OdArray<OdConstrainedGeometry*> aGeometries;
  for (OdDbObjectIdArray::const_iterator i = aDestDepToRemove.begin(); i != aDestDepToRemove.end(); ++i)
  {
    std::map<OdDbObjectId, OdConstrainedGeometry*>::iterator pConstrainedGeometry = aConstGeom.find(*i);
    if (pConstrainedGeometry != aConstGeom.end())
      aGeometries.append(pConstrainedGeometry->second);
  }
  return deleteNodes(aConstraints, aGeometries);
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteConstrainedGeometry(const OdDbObjectId& geomDependencyId)
{
  OdDbAssocGeomDependencyPtr geomDependency = OdDbAssocGeomDependency::cast(geomDependencyId.openObject());
  OdConstrainedGeometry* pConstrainedGeometry = NULL;

  if (!geomDependency.isNull())
    getConstrainedGeometry(geomDependency, pConstrainedGeometry);

  geomDependency.release();

  if (pConstrainedGeometry == NULL)
    return eNullObjectPointer;

  OdArray<OdGeomConstraint*> aConstraints;
  OdArray<OdConstrainedGeometry*> aGeometries;
  aGeometries.append(pConstrainedGeometry);

  return deleteNodes(aConstraints, aGeometries);
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstrainedGeometry(const OdDbFullSubentPath& fullSubentPath,
                                  OdConstrainedGeometry*&   pConsGeom,
                                  bool bCreateArcLineMid)
{
  OdDbObjectIdArray arrObjId = fullSubentPath.objectIds();

  OdDbObjectId obj = arrObjId[arrObjId.length() - 1];
  OdDbObjectPtr pObj = obj.openObject();
  OdDbObjectIdArray arrDepId;
  OdDbAssocDependency::getDependenciesOnObject(pObj, true, true, arrDepId);

  if ( arrDepId.length() == 0 )
    return eBadObjType;

  OdDbObjectPtr pDepObj;
  OdDbAssocGeomDependencyPtr pGeomDep;
  OdConstrainedGeometry* pConsGeomTmp;
  OdDbFullSubentPathArray arrFullSubentPath;
  OdConstrainedImplicitPoint::ImplicitPointType* pPtType = NULL;
  OdConstrainedImplicitPoint::ImplicitPointType ptType;

  const OdDbSubentId subentId = fullSubentPath.subentId();

  for ( unsigned int i = 0; i < arrDepId.length(); i++ )
  {
    pDepObj = arrDepId[i].openObject();
    if ( !pDepObj->isKindOf(OdDbAssocGeomDependency::desc()) )
      continue;

    pGeomDep = OdDbAssocGeomDependency::cast(pDepObj);
    if ( getConstrainedGeometry(pGeomDep, pConsGeomTmp) != eOk )
      continue;

    arrFullSubentPath.clear();
    if ( pConsGeomTmp->getFullSubentPaths(arrFullSubentPath) != eOk )
      continue;

    if ( fullSubentPath == arrFullSubentPath[0] )
    {
      pConsGeom = pConsGeomTmp;
      return eOk;
    }

    if ( subentId.type() == OdDb::kVertexSubentType )//find point
    {
      if ( subentId.index() < 0 && !pObj->isKindOf(OdDbSpline::desc()) ) // simple circle, line, ellipse, arc
      {
        if ( subentId.index() == OdDbAssocCurvePersSubentIdPE::kMiddle )
        {
          ptType = OdConstrainedImplicitPoint::kMidImplicit;
        }
        else if ( subentId.index() == OdDbAssocCurvePersSubentIdPE::kCenter )
        {
          ptType = OdConstrainedImplicitPoint::kCenterImplicit;
        }
        else if ( subentId.index() == OdDbAssocCurvePersSubentIdPE::kEnd )
        {
          ptType = OdConstrainedImplicitPoint::kEndImplicit;
        }
        else if ( subentId.index() == OdDbAssocCurvePersSubentIdPE::kStart )
        {
          ptType = OdConstrainedImplicitPoint::kStartImplicit;
        }
        else
          ODA_FAIL();

        pPtType = &ptType;

        if ( getConstrainedGeometry(pGeomDep, pConsGeom, pPtType, -1, bCreateArcLineMid) == eOk )
          return eOk;
      }
      else//polyline points or define points
      {
        if ( pObj->isKindOf(OdDbSpline::desc()) )
        {
          int idx = (int)subentId.index();
          switch (idx) {
          case OdDbAssocCurvePersSubentIdPE::kStart:
            idx = 0;
            break;
          case OdDbAssocCurvePersSubentIdPE::kEnd:
            idx = OdDbSpline::cast(pObj)->numControlPoints() - 1;
            break;
          }
          ptType = OdConstrainedImplicitPoint::kDefineImplicit;
          pPtType = &ptType;
          if ( getConstrainedGeometry(pGeomDep,
                                pConsGeom,
                                pPtType,
                                idx,
                                bCreateArcLineMid) == eOk )
            return eOk;
        }
        else //polyline
        {
          if ( subentId.index() < 1000000 )//start, end
          {
            ptType = OdConstrainedImplicitPoint::kStartImplicit;
            pPtType = &ptType;
            if ( getConstrainedGeometry(pGeomDep, pConsGeom, pPtType, -1, bCreateArcLineMid) == eOk )
            {
              if ( pConsGeom->getFullSubentPaths(arrFullSubentPath) == eOk )
                if ( fullSubentPath == arrFullSubentPath[0] )
                  return eOk;

            }

           ptType = OdConstrainedImplicitPoint::kEndImplicit;
           pPtType = &ptType;
           if ( getConstrainedGeometry(pGeomDep, pConsGeom, pPtType, -1, bCreateArcLineMid) == eOk )
           {
             if ( pConsGeom->getFullSubentPaths(arrFullSubentPath) == eOk )
                if ( fullSubentPath == arrFullSubentPath[0] )
                  return eOk;
           }
           continue;
         }
          
          if ( subentId.index() > OdDbAssocCurvePersSubentIdPE::kMiddlePLinePrefix ) //middle point
          {
            ptType = OdConstrainedImplicitPoint::kMidImplicit;
            pPtType = &ptType;
          }
          else if ( subentId.index() > OdDbAssocCurvePersSubentIdPE::kCenterPLinePrefix )//center
          {
            ptType = OdConstrainedImplicitPoint::kCenterImplicit;
            pPtType = &ptType;
          }
          
          if ( getConstrainedGeometry(pGeomDep, pConsGeom, pPtType, -1, bCreateArcLineMid) == eOk )
            return eOk;
        }
      }
    }
  }
  
  pConsGeom = NULL;
  return eBadObjType;
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstrainedGeometry(const OdDbAssocGeomDependency* pGeomDependency,
                                  OdConstrainedGeometry*&   pConsGeom,
                                  OdConstrainedImplicitPoint::ImplicitPointType* pPtType,
                                  int defPtIndex,
                                  bool bCreateArcLineMid)
{
  const OdDbObjectId searchId = pGeomDependency->objectId();
  OdResult res = eBadObjType;
  if (searchId.isNull())
  {
    pConsGeom = NULL;
    return res;
  }
  for (OdConstraintGroupNodeMap::const_iterator i = m_mapNodes.begin(); i != m_mapNodes.end(); ++i)
  {
    if (i->second->isKindOf(OdConstrainedGeometry::desc()))
    {
      if (((OdConstrainedGeometry*)(i->second.get()))->geomDependencyId() == searchId)
      {
        pConsGeom = (OdConstrainedGeometry*)(i->second.get());
        res = eOk;
        break;
      }
    }
  }

  if ( res == eOk )
  {
    if ( pPtType != NULL && pConsGeom->isKindOf(OdConstrainedCurve::desc()) )
    {
      res = eBadObjType;
      OdConstrainedCurve* pCurve = OdConstrainedCurve::cast(pConsGeom);
      OdArray<OdConstrainedImplicitPoint*> aPoints;
      pCurve->getConstrainedImplicitPoints(aPoints);
      for ( unsigned int i = 0; i < aPoints.length(); i++ )
      {
        if ( aPoints[i]->pointType() == *pPtType )
        {
          if ( defPtIndex != -1 )
          {
            if ( aPoints[i]->pointIndex() == defPtIndex )
            {
              pConsGeom = aPoints[i];
              res = eOk;
              break;
            }
          }
          else
          {
            pConsGeom = aPoints[i];
            res = eOk;
            break;
          }
        }
      }
      
      if ( res != eOk && *pPtType == OdConstrainedImplicitPoint::kMidImplicit && bCreateArcLineMid )
      {
        pConsGeom = addImplicitPoint(pCurve, OdConstrainedImplicitPoint::kMidImplicit);
        res = eOk;
      }
    }
    else if ( pPtType != NULL )
    {
      pConsGeom = NULL;
      res = eBadObjType;
    }
  }
  else
    pConsGeom = NULL;

  return res;
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstrainedGeometries(OdArray<OdConstrainedGeometry*>& aConstGeom) const
{
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    if (pNode->second.get()->isKindOf(OdConstrainedGeometry::desc()) )
      aConstGeom.append((OdConstrainedGeometry*)(pNode->second.get()));
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::addGeometricalConstraint(OdDbObjectId actionId, 
                                                                 OdGeomConstraint::GeomConstraintType type, 
                                                                 const OdDbFullSubentPathArray& aPaths,
                                                                 OdGeomConstraint** ppNewConstraint)
{
  OdArray<OdConstrainedGeometry*> apConsGeoms;
  OdArray<OdConstrainedGeometry*> apAllConsGeoms;
  if ( getConstrainedGeometries(apAllConsGeoms) != eOk )
    return eBadObjType;

  OdDbFullSubentPathArray arrGeomPath;
  unsigned int i, j;
  for ( j = 0; j < aPaths.length(); j++ ) {
    for ( i = 0; i < apAllConsGeoms.length(); i++ ) {
      arrGeomPath.clear();
      if ( apAllConsGeoms[i]->getFullSubentPaths(arrGeomPath) != eOk || arrGeomPath.length() == 0 )
        continue;
      if ( aPaths[j] == arrGeomPath[0] )
      {
        apConsGeoms.append(apAllConsGeoms[i]);
        break;
      }
    }
  }

  return addGeometricalConstraint(actionId, type, apConsGeoms, ppNewConstraint);
}

OdResult OdDbImpAssoc2dConstraintGroup::addGeometricalConstraint(OdDbObjectId actionId,
                                                                 OdGeomConstraint::GeomConstraintType type,
                                                                 const OdArray<OdConstrainedGeometry*>& apConsGeoms,
                                                                 OdGeomConstraint** ppNewConstraint)
{
  *ppNewConstraint = NULL;

  switch ( type )
  {
    case OdGeomConstraint::kHorizontal:
      if ( apConsGeoms.length() == 1 )
        *ppNewConstraint = addHorizontalConstraint(apConsGeoms[0], actionId);
      else if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addHorizontalConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kVertical:
      if ( apConsGeoms.length() == 1 )
        *ppNewConstraint = addVerticalConstraint(apConsGeoms[0], actionId);
      else if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addVerticalConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kParallel:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addParallelConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kPerpendicular:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addPerpendicularConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kNormal:
        if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addNormalConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kColinear:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addColinearConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kCoincident:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addCoincidentConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kConcentric:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addConcentricConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kTangent:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addTangentConstraint(apConsGeoms[0], apConsGeoms[1], NULL, NULL, actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kEqualRadius:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addEqualRadiusConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kEqualLength:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addEqualLengthConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kSymmetric:
      if ( apConsGeoms.length() == 3 )
        *ppNewConstraint = addSymmetricConstraint(apConsGeoms[0], apConsGeoms[1], apConsGeoms[2], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kG2Smooth:
      if ( apConsGeoms.length() == 2 )
        *ppNewConstraint = addG2SmoothConstraint(apConsGeoms[0], apConsGeoms[1], actionId);
      else
        *ppNewConstraint = NULL;
    break;
    case OdGeomConstraint::kFix:
      if ( apConsGeoms.length() == 1 )
        *ppNewConstraint = addFixConstraint(apConsGeoms[0], actionId);
      else
        *ppNewConstraint = NULL;
    break;
  }

  if ( *ppNewConstraint != NULL ) {
    OdGeomConstraintImpl *pGeomCnstrImpl = (OdGeomConstraintImpl*)getConstraintGroupNodeImpl(*ppNewConstraint);
    pGeomCnstrImpl->setImplied(false);
    return eOk;
  }
  else
    return eBadObjType;
}

bool OdDbImpAssoc2dConstraintGroup::isEmpty() const
{
  return m_mapNodes.empty();
}

OdResult OdDbImpAssoc2dConstraintGroup::addDistanceConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                                              OdConstrainedGeometry* pConsGeom1,
                                                              OdConstrainedGeometry* pConsGeom2,
                                                              OdDistanceConstraint::DirectionType directionType,
                                                              OdDbObjectId valueDependencyId,
                                                              OdDbObjectId dimDependencyId, // May be null
                                                              const OdGeVector3d* pFixedDirection/* = NULL*/,
                                                              const OdConstrainedLine* pDirectionLine/* = NULL*/,
                                                              OdDistanceConstraint** ppNewDisConstraint/* = NULL*/)
{
  OdDistanceConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pConsGeom1->getCommonConstraints(pConsGeom2, aConstr);
  OdGeomConstraint* tmp;
  if (isConstraintOfThisTypeExist(aConstr, OdDistanceConstraint::desc(), &tmp))
    return eBadObjType;

  OdSmartPtr<OdDistanceConstraint> spDistCnstr = ::odrxCreateObject(OdDistanceConstraint::desc()->name());
  tieNode(spDistCnstr, pGroup->objectId());

  res = spDistCnstr.get();

  tieDimAndCnstr(res, valueDependencyId, dimDependencyId, pGroup);
  tieGeomsAndCnstr3(res, pConsGeom1, pConsGeom2);

  if (pFixedDirection)
    res->setDirection(*pFixedDirection);
  else if (pDirectionLine)
    res->setDirection(pDirectionLine->nodeId(), directionType);

  *ppNewDisConstraint = res;
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::addAngleConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                                           OdConstrainedLine* pConsLine1,
                                                           OdConstrainedLine* pConsLine2,
                                                           OdAngleConstraint::SectorType sectorType,
                                                           OdDbObjectId valueDependencyId,
                                                           OdDbObjectId dimDependencyId, // May be null
                                                           OdAngleConstraint** ppNewAngConstraint/* = NULL*/)
{
  OdAngleConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pConsLine1->getCommonConstraints(pConsLine2, aConstr);
  OdGeomConstraint* tmp;
  if (isConstraintOfThisTypeExist(aConstr, OdAngleConstraint::desc(), &tmp))
    return eBadObjType;

  OdSmartPtr<OdAngleConstraint> spAngCnstr = ::odrxCreateObject(OdAngleConstraint::desc()->name());
  tieNode(spAngCnstr, pGroup->objectId());

  res = spAngCnstr.get();

  tieDimAndCnstr(res, valueDependencyId, dimDependencyId, pGroup);
  tieGeomsAndCnstr3(res, pConsLine1, pConsLine2);

  res->setSectorType(sectorType);

  *ppNewAngConstraint = res;
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::add3PointAngleConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                                                 OdConstrainedPoint* pConsPoint1,
                                                                 OdConstrainedPoint* pConsPoint2,
                                                                 OdConstrainedPoint* pConsPoint3,
                                                                 OdAngleConstraint::SectorType sectorType,
                                                                 OdDbObjectId valueDependencyId,
                                                                 OdDbObjectId dimDependencyId, // May be null
                                                                 Od3PointAngleConstraint** ppNewAngConstraint/* = NULL*/)
{
  Od3PointAngleConstraint* res = NULL;

  OdSmartPtr<Od3PointAngleConstraint> sp3PtAngCnstr = ::odrxCreateObject(Od3PointAngleConstraint::desc()->name());
  tieNode(sp3PtAngCnstr, pGroup->objectId());

  res = sp3PtAngCnstr.get();

  tieDimAndCnstr(res, valueDependencyId, dimDependencyId, pGroup);
  tieGeomsAndCnstr3(res, pConsPoint1, pConsPoint2, pConsPoint3);

  res->setSectorType(sectorType);

  *ppNewAngConstraint = res;
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::addRadiusDiameterConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                                                    OdConstrainedGeometry* pConsGeom,
                                                                    OdRadiusDiameterConstraint::RadiusDiameterConstrType type,
                                                                    OdDbObjectId valueDependencyId,
                                                                    OdDbObjectId dimDependencyId, // May be null
                                                                    OdRadiusDiameterConstraint** ppNewRadDiaConstraint/* = NULL*/)
{
  OdRadiusDiameterConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pConsGeom->getConnectedConstraints(aConstr);
  OdGeomConstraint* tmp;
  if (isConstraintOfThisTypeExist(aConstr, OdRadiusDiameterConstraint::desc(), &tmp))
    return eBadObjType;

  OdSmartPtr<OdRadiusDiameterConstraint> spRadDiaCnstr = ::odrxCreateObject(OdRadiusDiameterConstraint::desc()->name());
  tieNode(spRadDiaCnstr, pGroup->objectId());

  res = spRadDiaCnstr.get();

  tieDimAndCnstr(res, valueDependencyId, dimDependencyId, pGroup);
  tieGeomsAndCnstr3(res, pConsGeom, NULL);

  res->setConstrType(type);

  *ppNewRadDiaConstraint = res;
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteNodes(OdArray<OdGeomConstraint*> &iaConstraints,
                                                    OdArray<OdConstrainedGeometry*> &iaGeometries)
{
  OdResult res = eOk;
  unsigned int i, j;
  OdConstrainedGeometry* pConstrainedGeometry = 0;

  if (iaConstraints.size() > 0) {
    if ((res = deleteConstraints(iaConstraints, iaGeometries)) != eOk)
      return res;
  } else if (iaGeometries.size() == 1) {
    pConstrainedGeometry = iaGeometries[0];
    pConstrainedGeometry->getConnectedConstraints(iaConstraints);
    for (i = 0; i < iaConstraints.size(); i++) {
      OdGeomConstraintPtr geomConstraint = iaConstraints[i];
      if (!geomConstraint->isImplied())
        continue;
      OdArray<OdConstrainedGeometry*> aConnectedGeometries;
      ((OdGeomConstraintImpl*)geomConstraint->m_pImpl)->getConnectedGeometries(aConnectedGeometries);
      for (j = 0; j < aConnectedGeometries.size(); j++) {
        OdConstrainedGeometry* connectedGeometry = aConnectedGeometries[j];
        bool isImplicit = connectedGeometry->isKindOf(OdConstrainedImplicitPoint::desc());
        if (isImplicit && !iaGeometries.contains(connectedGeometry))
          iaGeometries.append(connectedGeometry);
      }
    }
  }

  do {
    for (i = 0; i < iaGeometries.size(); i++) {
      pConstrainedGeometry = iaGeometries[i];
      iaConstraints.clear();
      pConstrainedGeometry->getConnectedConstraints(iaConstraints);
      if ((res = deleteConstraints(iaConstraints, iaGeometries)) != eOk)
        return res;
    }
    if ((res = deleteGeometries(iaGeometries)) != eOk)
      return res;
    if (!iaGeometries.isEmpty())
      continue;

    std::set<OdConstraintGroupNodeId> aIsExternal;
    for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode){
      OdGeomConstraint* geomConstraint = OdGeomConstraint::cast(pNode->second);
      if (geomConstraint && (!geomConstraint->isInternal() || geomConstraint->getOwningCompositeConstraint())) {
        OdArray<OdConstrainedGeometry*> aConnectedGeometries;
        ((OdGeomConstraintImpl*)geomConstraint->m_pImpl)->getConnectedGeometries(aConnectedGeometries);
        for (j = 0; j < aConnectedGeometries.size(); j++) {
            aIsExternal.insert(aConnectedGeometries[j]->nodeId());
        }
      }
    }
    for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode){
      OdConstrainedImplicitPoint* pPoint = OdConstrainedImplicitPoint::cast(pNode->second);
      if (pPoint && aIsExternal.find(pPoint->nodeId()) != aIsExternal.end())
        aIsExternal.insert(pPoint->constrainedCurveId());
    }
    for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode){
      OdConstrainedGeometry* pConstrainedGeometry = OdConstrainedGeometry::cast(pNode->second);
      if (!pConstrainedGeometry || pConstrainedGeometry->geomDependencyId().isNull() || aIsExternal.find(pConstrainedGeometry->nodeId()) != aIsExternal.end())
        continue;
      OdConstrainedImplicitPoint* point = OdConstrainedImplicitPoint::cast(pConstrainedGeometry);
      if (point && aIsExternal.find(point->constrainedCurveId()) != aIsExternal.end())
        continue;
      OdArray<OdConstrainedGeometry*> aConnectedGeometries;
      ((OdConstrainedGeometryImpl*)pConstrainedGeometry->m_pImpl)->getConnectedGeometries(aConnectedGeometries);
      bool hasNoExternal = true;
      for (j = 0; j < aConnectedGeometries.size() && hasNoExternal; j++) {
        hasNoExternal = aIsExternal.find(aConnectedGeometries[j]->nodeId()) == aIsExternal.end();
      }
      if (hasNoExternal) {
        iaGeometries.append(pConstrainedGeometry);
        break;
      }
    }
  } while (!iaGeometries.isEmpty());
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteGeometries(OdArray<OdConstrainedGeometry*> &ioaGeometries)
{
  int i = ioaGeometries.size();
  while (i-- > 0) {
    OdConstrainedGeometry *pConstrainedGeometry = ioaGeometries[i];
    OdDbObjectId depId = pConstrainedGeometry->geomDependencyId();
    if (!depId.isNull())
      removeDependency(depId, true);
    removeNode(pConstrainedGeometry);
  }
  ioaGeometries.clear();

  for (OdConstraintGroupNodeMap::reverse_iterator ri = m_mapNodes.rbegin(); ri != m_mapNodes.rend(); ++ri) {
    OdConstraintGroupNodePtr node = ri->second;
    if (!node->isKindOf(OdConstrainedGeometry::desc()))
      continue;
    OdConstrainedGeometry *pConstrainedGeometry = OdConstrainedGeometry::cast(node);
    if (node->isKindOf(OdConstrainedImplicitPoint::desc())) {
      OdSmartPtr<OdConstrainedImplicitPoint> point = OdConstrainedImplicitPoint::cast(node);
      bool isOrhpan = getGroupNodePtr(point->constrainedCurveId()) == NULL;
      if (isOrhpan && !ioaGeometries.contains(pConstrainedGeometry))
        ioaGeometries.append(pConstrainedGeometry);
    } else if (node->isKindOf(OdConstrainedDatumLine::desc())) {
      bool found = false;
      for (OdConstraintGroupNodeMap::reverse_iterator rj = m_mapNodes.rbegin(); rj != m_mapNodes.rend() && !found; ++rj) {
        OdConstraintGroupNodePtr constr = rj->second;
        if (constr->isKindOf(OdParallelConstraint::desc())) {
          OdSmartPtr<OdParallelConstraint> parallelConstraint = OdParallelConstraint::cast(constr);
          OdConstraintGroupNodeImpl *pImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(parallelConstraint);
          found = (((OdParallelConstraintImpl*)pImpl)->getDatumLineIdx() == node->nodeId());
        }
      }
      if (!found && !ioaGeometries.contains(pConstrainedGeometry))
        ioaGeometries.append(pConstrainedGeometry);
    }
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteHelpParameters(OdArray<OdHelpParameter*> &ioaHelpParameters,
                                                             OdConstraintGroupNodeId id)
{
  int i = ioaHelpParameters.size();
  while (i-- > 0) {
    OdHelpParameter* helpParameter = ioaHelpParameters[i];
    bool isNoConnection = false;
    OdConstraintGroupNodeImpl *pImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(helpParameter);
    pImpl->removeConnection(id, isNoConnection);
    if (isNoConnection) {
      removeNode(helpParameter);
      ioaHelpParameters.remove(helpParameter);
    }
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteConstraints(OdArray<OdGeomConstraint*> &iaConstraints,
                                                          OdArray<OdConstrainedGeometry*> &ioaGeometries)
{
  OdResult res = eOk;
  for (unsigned i = 0; i < iaConstraints.size(); i++) {
    OdGeomConstraintPtr geomConstraint = iaConstraints[i];

    OdCompositeConstraint *pComposite = geomConstraint->getOwningCompositeConstraint();
    if (pComposite != NULL) {
      if (iaConstraints.contains(pComposite)) {
        iaConstraints[i] = NULL;
        continue;
      } else {
        iaConstraints[i] = pComposite;
        geomConstraint = pComposite;
      }
    }

    if (geomConstraint->isKindOf(OdExplicitConstraint::desc())) {
      OdSmartPtr<OdExplicitConstraint> explicitConstraint = OdExplicitConstraint::cast(geomConstraint);
      OdDbObjectId dimDepId = explicitConstraint->dimDependencyId();
      if (!dimDepId.isNull()) {
        if (OdDbAssocDimDependencyBodyBase::getEraseDimensionIfDependencyIsErased()) {
          OdDbAssocDependencyPtr dimDep = dimDepId.openObject();
          if (!dimDep.isNull()) {
            OdDbObjectId dimId = dimDep->dependentOnObject();
            OdDbObjectPtr dimension = dimId.openObject(OdDb::kForWrite);
            if (!dimension.isNull()) {
              dimension->removePersistentReactor(dimDepId);
              dimension->erase(true);
            }
          }
        }
        removeDependency(dimDepId, true);
      }
      OdDbObjectId valDepId = explicitConstraint->valueDependencyId();
      OdDbAssocValueDependencyPtr valDep = valDepId.openObject();
      if (!valDep.isNull()) {
        OdDbObjectId varId = valDep->dependentOnObject();
        valDep = 0;
        removeDependency(valDepId, true);
        OdDbAssocNetworkPtr pNetwork = owningNetwork().openObject(OdDb::kForWrite);
        if (!pNetwork.isNull())
          pNetwork->removeAction(varId, true);
      }
    }

    if (geomConstraint->isKindOf(OdCompositeConstraint::desc())) {
      OdSmartPtr<OdCompositeConstraint> compositeConstraint = OdCompositeConstraint::cast(geomConstraint);
      OdArray<OdGeomConstraint*> aConstr;
      compositeConstraint->getOwnedConstraints(aConstr);
      for (unsigned j = 0; j < aConstr.size(); j++) {
        OdGeomConstraintImpl* pImpl = (OdGeomConstraintImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(aConstr[j]);
        pImpl->setOwningCompositeConstraintId(OdConstraintGroupNode::kNullGroupNodeId);
        if (!iaConstraints.contains(aConstr[j]))
          iaConstraints.append(aConstr[j]);
      }
    }

    OdArray<OdHelpParameter*> aHelpParameters;
    if ((res = geomConstraint->getConnectedHelpParameters(aHelpParameters)) != eOk)
      return res;

    OdArray<OdConstrainedGeometry*> aConnectedGeometries;
    if ((res = ((OdGeomConstraintImpl*)geomConstraint->m_pImpl)->getConnectedGeometries(aConnectedGeometries)) != eOk)
      return res;

    for (unsigned j = 0; j < aConnectedGeometries.size(); j++) {
      OdConstrainedGeometry* connectedGeometry = aConnectedGeometries[j];
      OdConstrainedImplicitPoint* implicitPoint = OdConstrainedImplicitPoint::cast(connectedGeometry);
      bool isNoConnection = false,
           isImplicit = geomConstraint->isImplied() &&
                        connectedGeometry->isKindOf(OdConstrainedConstructionLine::desc()),
           isDefinePt = (implicitPoint && implicitPoint->constrainedCurveId() != OdConstraintGroupNode::kNullGroupNodeId);

      OdConstraintGroupNodeImpl *pImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(connectedGeometry);
      pImpl->removeConnection(geomConstraint->nodeId(), isNoConnection);

      for (unsigned k = 0; k < aHelpParameters.size(); k++) {
        pImpl->removeConnection(aHelpParameters[k]->nodeId(), isNoConnection);
      }

      if ((res = deleteHelpParameters(aHelpParameters, connectedGeometry->nodeId())) != eOk)
        return res;

      if ((isNoConnection || isImplicit) && !isDefinePt && !ioaGeometries.contains(connectedGeometry))
        ioaGeometries.append(connectedGeometry);
    }

    if ((res = deleteHelpParameters(aHelpParameters, geomConstraint->nodeId())) != eOk)
      return res;

    removeNode(geomConstraint);
  }
  return res;
}

OdResult OdDbImpAssoc2dConstraintGroup::deleteConstraint(OdGeomConstraint* pGeomConst)
{
  OdResult res = eInvalidInput;

  if (!pGeomConst || pGeomConst->isInternal())
    return res;

  OdArray<OdGeomConstraint*> aConstraints;
  OdArray<OdConstrainedGeometry*> aGeometries;

  aConstraints.append(pGeomConst);

  return deleteNodes(aConstraints, aGeometries);
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstraints(OdArray<OdGeomConstraint*>& apConstraints) const
{
  for (OdConstraintGroupNodeMap::const_iterator i = m_mapNodes.begin(); i != m_mapNodes.end() ; ++i) {
    if (i->second->isKindOf(OdGeomConstraint::desc()) )
      apConstraints.append(const_cast<OdGeomConstraint*>(static_cast<const OdGeomConstraint*>(i->second.get())));
  }
  return eOk;
}

#if 0
OdResult OdDbImpAssoc2dConstraintGroup::autoConstrain(const OdDbFullSubentPathArray& aPaths,
                         const OdGeTol& tol,
                         OdAutoConstrainEvaluationCallback* pCallback)
#endif
OdResult OdDbImpAssoc2dConstraintGroup::getAllConnectedGeomDependencies(const OdDbObjectIdArray& srcGeomDependencyIds,
                                           OdDbObjectIdArray& allConnectedGeomDependencyIds) const
{

  return eOk;
}

OdConstraintGroupNode* OdDbImpAssoc2dConstraintGroup::getGroupNodePtr(const OdConstraintGroupNodeId nodeId) const
{
  OdConstraintGroupNodeMap::const_iterator it = m_mapNodes.find(nodeId);
  if (it != m_mapNodes.end())
    return const_cast<OdConstraintGroupNode*>(it->second.get());
  return 0;
}

#if 0
void OdDbImpAssoc2dConstraintGroup::regenDimensionSystem()

OdResult OdDbImpAssoc2dConstraintGroup::geometryMirrored(OdDbAssocGeomDependency* pGeomDependency)
  
void OdDbImpAssoc2dConstraintGroup::addGlobalCallback(class OdDbAssoc2dConstraintCallback* pCallback)

void OdDbImpAssoc2dConstraintGroup::removeGlobalCallback(class OdDbAssoc2dConstraintCallback* pCallback)

class OdDbAssoc2dConstraintCallback* OdDbImpAssoc2dConstraintGroup::globalCallback()

#endif

void OdDbImpAssoc2dConstraintGroup::evaluate(OdDbAssocAction *pAction, OdDbAssocEvaluationCallback* pEvaluationCallback)
{
  ODA_ASSERT(isEvaluationRequest(status()));

  OdDbObjectId bodyId = actionBody();
  if ( !bodyId.isNull() )
  {
    OdDbObjectPtr pObj = bodyId.openObject(OdDb::kForWrite);
    OdDbAssocActionBodyPtr pBody = OdDbAssocActionBody::cast(pObj);
    pBody->evaluateOverride();
    return;
  }
  
  OdSmartPtr<AssocActionPE> pPE = pAction->isA()->getX(AssocActionPE::desc());
  if ( !pPE.isNull() )
  {
    if ( !pAction->isWriteEnabled() )
      pAction->upgradeOpen();
    pPE->evaluate(pAction);
    doPostEvaluateJob(pAction);
  }

}

OdResult OdDbImpAssoc2dConstraintGroup::postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  // map dependencies of destination group
  OdDbIdPair searchForDest(pAction->objectId());
  if (!idMap.compute(searchForDest) || !searchForDest.isCloned() || searchForDest.value().isErased())
  {
    return eOk;
  }

  OdDbAssocActionPtr pTempAction = OdDbAssocAction::cast(searchForDest.value().openObject(OdDb::kForWrite));
  ODA_ASSERT(pTempAction->isKindOf(OdDbAssoc2dConstraintGroup::desc()));

  std::map<OdDbObjectId, OdDbObjectId> * pSrcToDestIdMap = OdDbAssocNetworkDeepCloneAux::srcToDestIdMap(idMap);

  OdDbObjectId srcBlockId = pAction->objectThatOwnsNetworkInstance();
  OdDbObjectId destBlockId = pSrcToDestIdMap ? (*pSrcToDestIdMap)[srcBlockId] : OdDbObjectId();
  ODA_ASSERT(!destBlockId.isNull());

  pTempAction = 0;

  // This group is kept in temporary dictionary
  OdDbAssoc2dConstraintGroupPtr pTempConstraintGroup = OdDbAssoc2dConstraintGroup::cast(searchForDest.value().openObject(OdDb::kForWrite));

  // If after clearing temp group has no dependencies we are no longer interested in it
  // It will be erased in deep clone reactor when temp dictionary is erased
  OdDbObjectIdArray aTempDependencyIds;
  pTempConstraintGroup->getDependencies(true, true, aTempDependencyIds);
  if (aTempDependencyIds.length())
  {

    // After this operation continue check whether destination network
    // Already has group of same kind
    // Then actions have to be reassigned and newly created group has to be removed

    OdDbAssocNetworkPtr pDestParentNetwork = OdDbAssocNetwork::getInstanceFromObject(destBlockId, false).openObject(OdDb::kForRead);
    ODA_ASSERT(!pDestParentNetwork.isNull());

    OdDbAssoc2dConstraintGroupPtr pAlreadyExistingGroup; // Refers to the group with the same plane which was not cloned
    OdGePlane destWorkPlane = pTempConstraintGroup->getWorkPlane();

    const OdDbObjectIdArray & aDestActionIds = pDestParentNetwork->getActions();
    unsigned int i, destActionLength = aDestActionIds.length();
    for (i=0; i<destActionLength; i++)
    {
      OdDbAssoc2dConstraintGroupPtr pCheckedGroup = OdDbAssoc2dConstraintGroup::cast(aDestActionIds[i].openObject());
      if (pCheckedGroup.isNull())
        continue;
      OdGePlane checkedWorkPlane = pCheckedGroup->getWorkPlane();
      if (destWorkPlane != checkedWorkPlane)
        continue;
      pAlreadyExistingGroup = pCheckedGroup;
      break;
    }

    if (pAlreadyExistingGroup.isNull())
    {
      OdDbAssocNetworkDeepCloneAux::moveCompleteActionToDestNetwork(pTempConstraintGroup, destBlockId, idMap.deepCloneContext());
    }
    else
    {
      pAlreadyExistingGroup->upgradeOpen();
      OdDbAssocNetworkDeepCloneAux::moveDepFromSrcGroup(pTempConstraintGroup.get(), pAlreadyExistingGroup.get());
    }
  }

  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::postProcessAfterDeepCloneCancel(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dwgInFields(pFiler, objectId);
  if ( res != eOk )
    return res;
  OdUInt32 ver = pFiler->rdInt16();
  ODA_ASSERT(!ver || ver == 2);

  m_Unk_Bool = pFiler->rdBool();
  ODA_ASSERT_ONCE(!m_Unk_Bool);
  OdGePoint3d ptOrigin = pFiler->rdPoint3d();
  OdGeVector3d vU = pFiler->rdVector3d();
  OdGeVector3d vV = pFiler->rdVector3d();
  m_plane.set(ptOrigin, vU, vV);
  m_Unk_ObjId = pFiler->rdHardOwnershipId();
  OdUInt32 nCnt = pFiler->rdInt32();
  m_arrDependencies.resize(nCnt);
  OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
  while (nCnt--)
  {
    *pId++ = pFiler->rdHardOwnershipId();
  }
  m_seedId = pFiler->rdInt32();

  if (pFiler->dwgVersion() <= OdDb::vAC24)
    res = dwgInNodes2010(pFiler, objectId, ver);
  else
    res = dwgInNodes2013(pFiler, objectId);
  if ( res != eOk )
    return res;

  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dwgInNodes2010(OdDbDwgFiler* pFiler, 
                                                       OdDbObjectId objectId,
                                                       OdUInt32 ver)
{
  ODA_ASSERT(!ver || ver == 2);
  OdUInt32 sizeNodes = pFiler->rdInt32();
  m_mapNodes.clear();
  for (OdUInt32 idx = 0; idx < sizeNodes; idx++)
  {
    OdString strName = pFiler->rdString();
    OdConstraintGroupNodePtr pNode = ::odrxCreateObject(strName);
    if (pNode.isNull())
    {
      ODA_FAIL(); // Class not registered
      throw OdError(eDwgObjectImproperlyRead);
    }
    if (!ver)
    {
      // intermediate R24 version (thre is no way to save data in this format)
      // dataFiles/dwg/ACADSamples/2010/architectural_example-imperial.dwg
      //
      // set marker for OdConstraintGroupNodeImpl::dwgInFields
      OdConstraintGroupNodeId id = pFiler->rdInt32();
      if (!id || !pNode->m_pImpl)
      {
        ODA_FAIL_ONCE(); // TODO
        return eMakeMeProxy;
      }
      pNode->m_pImpl->setNodeId(id);
    }

    OdResult res = pNode->dwgInFields(pFiler);
    if (res != eOk)
      return res;
    if (!m_mapNodes.insert(std::make_pair(pNode->m_pImpl->nodeId(), pNode)).second)
    {
      ODA_FAIL(); // nodeId duplicated
      m_mapNodes[UINT_MAX] = 0;
    }
    pNode->m_pImpl->setOwnerAction(objectId);
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dwgInNodes2013(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  if (pFiler->rdInt16())
  { // TODO AC2013
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  if (pFiler->rdInt32())
  { // TODO AC2013
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  if (pFiler->rdBool())
  { // TODO AC2013
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  OdUInt32 sizeDictClassNames  = pFiler->rdInt32();
  OdStringArray asDictClassNames;
  asDictClassNames.resize(sizeDictClassNames);
  OdUInt32 index = 0;
  for (; index < sizeDictClassNames; index++)
  {
    OdString strName = pFiler->rdString();
    ODA_ASSERT_ONCE(!strName.isEmpty());

    asDictClassNames[index] = strName;
  }

  OdUInt32 sizeNodes = pFiler->rdInt32();
  OdArray<OdConstraintGroupNodePtr> arrNodes;
  arrNodes.resize(sizeNodes);
  for (index = 0; index < sizeNodes; index++)
  {
    if (pFiler->rdBool())
    {
      ODA_FAIL_ONCE(); // TODO
      return eMakeMeProxy;
    }

    OdUInt32 idx = pFiler->rdInt32();
    idx--;
    ODA_ASSERT_ONCE(idx < sizeDictClassNames);

    OdString strName = asDictClassNames[idx];
    ODA_ASSERT_ONCE(!strName.isEmpty());

    OdConstraintGroupNodePtr pNode = ::odrxCreateObject(strName);
    if (pNode.isNull())
    {
      ODA_FAIL(); // Class not registered
      throw OdError(eDwgObjectImproperlyRead);
    }
    ODA_ASSERT_ONCE(strName == pNode->isA()->name());

    OdConstraintGroupNodeId idNode = pFiler->rdInt32();
    ODA_ASSERT_ONCE(idNode && idNode <= m_seedId); // test

    OdConstraintGroupNodeImpl* pImpl = pNode->m_pImpl;
    pImpl->setNodeId(idNode);
    arrNodes[index] = pNode;
  }
  m_mapNodes.clear();
  for (index = 0; index < arrNodes.size(); index++)
  {
    OdConstraintGroupNodePtr pNode = arrNodes[index];
    OdConstraintGroupNodeImpl* pImpl = pNode->m_pImpl;
    ODA_ASSERT_ONCE(pImpl->nodeId());
    OdResult res = pNode->dwgInFields(pFiler);
    if (res != eOk)
      return res;
    if (!m_mapNodes.insert(std::make_pair(pImpl->nodeId(), pNode)).second)
    {
      ODA_FAIL(); // nodeId duplicated
      m_mapNodes[UINT_MAX] = 0;
    }

    pImpl->setOwnerAction(objectId);
  }
  return eOk;
}

void OdDbImpAssoc2dConstraintGroup::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dwgOutFields(pFiler, objectId);

  pFiler->wrInt16(2);   // Version?
  ODA_ASSERT_ONCE(!m_Unk_Bool);
  pFiler->wrBool(m_Unk_Bool);

  OdGePoint3d ptOrigin;
  OdGeVector3d vU, vV;
  m_plane.get(ptOrigin, vU, vV);
  pFiler->wrPoint3d(ptOrigin);
  pFiler->wrVector3d(vU);
  pFiler->wrVector3d(vV);

  pFiler->wrHardOwnershipId(m_Unk_ObjId);
  OdUInt32 nCnt = m_arrDependencies.size();
  pFiler->wrInt32(nCnt);
  const OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
  while (nCnt--)
  {
    pFiler->wrHardOwnershipId(*pId++);
  }
  pFiler->wrInt32(m_seedId);

  if (pFiler->dwgVersion() <= OdDb::vAC24)
    dwgOutNodes2010(pFiler, objectId);
  else
    dwgOutNodes2013(pFiler, objectId);
}

void OdDbImpAssoc2dConstraintGroup::dwgOutNodes2010(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrInt32(OdInt32(m_mapNodes.size()));
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    pFiler->wrString(pNode->second->isA()->name());
    pNode->second->dwgOutFields(pFiler);
  }
}

void OdDbImpAssoc2dConstraintGroup::dwgOutNodes2013(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrInt16(0);
  pFiler->wrInt32(0);
  pFiler->wrBool(false);

  // collect dictionary of class names
  OdStringArray asDictClassNames;
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    OdString strName = pNode->second->isA()->name();
    if (asDictClassNames.contains(strName))
      continue;
    asDictClassNames.push_back(strName);
  }

  // write dictionary
  OdUInt32 sizeDictClassNames = asDictClassNames.size();
  pFiler->wrInt32(sizeDictClassNames);
  for (OdUInt32 index = 0; index < sizeDictClassNames; index++)
  {
    pFiler->wrString(asDictClassNames[index]);
  }

  // write section to create node instances (with indexes to dictionary)
  pFiler->wrInt32(OdInt32(m_mapNodes.size()));
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    pFiler->wrBool(false);

    OdString strName = pNode->second->isA()->name();
    unsigned int idx = (unsigned int)(-1);
    if (!asDictClassNames.find(strName, idx))
    {
      ODA_FAIL_ONCE();
      throw OdError(eFileInternalErr);
    }
    idx++; // 1 base index
    ODA_ASSERT_ONCE(idx && idx <= sizeDictClassNames);

    pFiler->wrInt32(idx);

    const OdConstraintGroupNodeImpl* pImpl = pNode->second->m_pImpl;
    OdConstraintGroupNodeId idNode = pImpl->nodeId();
    ODA_ASSERT_ONCE(idNode && idNode <= m_seedId);

    pFiler->wrInt32(idNode);
  }

  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    pNode->second->dwgOutFields(pFiler);
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}


OdResult OdDbImpAssoc2dConstraintGroup::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dxfInFields(pFiler, objectId);
  if ( res != eOk )
    return res;

  NEXT_CODE(90)
#ifdef ODA_DIAGNOSTICS
  OdUInt32 nUnk = 
#endif // ODA_DIAGNOSTICS
  pFiler->rdUInt32();
  ODA_ASSERT(nUnk == 2);  // Version?

  NEXT_CODE(70)
  m_Unk_Bool = (pFiler->rdInt16() != 0);
  ODA_ASSERT_ONCE(!m_Unk_Bool);
  NEXT_CODE(10)
  OdGePoint3d ptOrigin;
  pFiler->rdPoint3d(ptOrigin);
  NEXT_CODE(10)
  OdGeVector3d vU;
  pFiler->rdVector3d(vU);
  NEXT_CODE(10)
  OdGeVector3d vV;
  pFiler->rdVector3d(vV);
  m_plane.set(ptOrigin, vU, vV);
  NEXT_CODE(360)
  m_Unk_ObjId = pFiler->rdObjectId();
  NEXT_CODE(90)
  OdUInt32 nCnt = pFiler->rdUInt32();
  m_arrDependencies.resize(nCnt);
  OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
  while (nCnt--)
  {
    NEXT_CODE(360)
    *pId++ = pFiler->rdObjectId();
  }
  NEXT_CODE(90)
  m_seedId = pFiler->rdUInt32();

  if (pFiler->dwgVersion() <= OdDb::vAC24)
    res = dxfInNodes2010(pFiler, objectId);
  else
    res = dxfInNodes2013(pFiler, objectId);
  if (res != eOk)
    return res;

  ODA_ASSERT_ONCE(pFiler->atEOF());  // TODO
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dxfInNodes2010(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res;
  NEXT_CODE(90)
  unsigned int nCnt = pFiler->rdUInt32();
  m_mapNodes.clear();
  while (nCnt--)
  {
    NEXT_CODE(1)
    OdString strName = pFiler->rdString();
    OdConstraintGroupNodePtr pNode = ::odrxCreateObject(strName);
    if (pNode.isNull())
    {
      ODA_FAIL(); // Class not registered
      throw OdError(eDwgObjectImproperlyRead);
    }
    res = pNode->dxfInFields(pFiler);
    if (res != eOk)
      return res;
    pNode->m_pImpl->setOwnerAction(objectId);
    if (pNode->nodeId() == 0)
      return eBadDxfSequence;
    if (!m_mapNodes.insert(std::make_pair(pNode->nodeId(), pNode)).second)
    {
      ODA_FAIL(); // nodeId duplicated
      m_mapNodes[UINT_MAX] = 0;
    }
  }
  return eOk;
}

OdResult OdDbImpAssoc2dConstraintGroup::dxfInNodes2013(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res;
  NEXT_CODE(90)
  unsigned int nCnt = pFiler->rdUInt32();
  if (nCnt > 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  NEXT_CODE(90)
  nCnt = pFiler->rdUInt32();
  if (nCnt > 0)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  NEXT_CODE(290)
  if (pFiler->rdBool())
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  nCnt = pFiler->rdUInt32();
  OdArray<OdConstraintGroupNodePtr> arrNodes;
  unsigned int nCnt1 = nCnt;
  while (nCnt--)
  {
    NEXT_CODE(1)
    OdString strName = pFiler->rdString();
    OdConstraintGroupNodePtr pNode = ::odrxCreateObject(strName);
    if (pNode.isNull())
    {
      ODA_FAIL(); // Class not registered
      throw OdError(eDwgObjectImproperlyRead);
    }
    OdConstraintGroupNodeImpl *pImpl = pNode->m_pImpl;

    NEXT_CODE(90)
    pImpl->setNodeId(pFiler->rdUInt32());
    arrNodes.push_back(pNode);
  }
  m_mapNodes.clear();
  for (unsigned int index = 0; index < arrNodes.size(); index++)
  {
    OdConstraintGroupNodePtr pNode = arrNodes[index];
    res = pNode->dxfInFields(pFiler);
    if (res != eOk)
      return res;
    OdConstraintGroupNodeImpl *pImpl = pNode->m_pImpl;
    if (!m_mapNodes.insert(std::make_pair(pImpl->nodeId(), pNode)).second)
    {
      ODA_FAIL(); // nodeId duplicated
      m_mapNodes[UINT_MAX] = 0;
    }
    pImpl->setOwnerAction(objectId);
  }

  return eOk;
}

void OdDbImpAssoc2dConstraintGroup::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dxfOutFields(pFiler, objectId);
  pFiler->wrUInt32(90, 2);  // Version?

  ODA_ASSERT_ONCE(!m_Unk_Bool);
  pFiler->wrInt16(70, m_Unk_Bool ? 1 : 0);

  OdGePoint3d ptOrigin;
  OdGeVector3d vU, vV;
  m_plane.get(ptOrigin, vU, vV);
  pFiler->wrPoint3d(10, ptOrigin);
  pFiler->wrVector3d(10, vU);
  pFiler->wrVector3d(10, vV);

  pFiler->wrObjectId(360, m_Unk_ObjId);
  OdUInt32 nCnt = m_arrDependencies.size();
  pFiler->wrUInt32(90, nCnt);
  const OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
  while (nCnt--)
  {
    pFiler->wrObjectId(360, *pId++);
  }
  pFiler->wrUInt32(90, m_seedId);

  if (pFiler->dwgVersion() <= OdDb::vAC24)
    dxfOutNodes2010(pFiler, objectId);
  else
    dxfOutNodes2013(pFiler, objectId);
}

void OdDbImpAssoc2dConstraintGroup::dxfOutNodes2010(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrUInt32(90, OdUInt32(m_mapNodes.size()));
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    OdString strName = pNode->second->isA()->name();
    pFiler->wrString(1, strName);
    pNode->second->dxfOutFields(pFiler);
  }
}

void OdDbImpAssoc2dConstraintGroup::dxfOutNodes2013(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrUInt32(90, 0);
  pFiler->wrUInt32(90, 0);
  pFiler->wrBool(290, false);
  pFiler->wrUInt32(90, OdUInt32(m_mapNodes.size()));
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    pFiler->wrString(1, pNode->second->isA()->name());
    pFiler->wrUInt32(90, pNode->first);
  }
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
    pNode->second->dxfOutFields(pFiler);
}

//add constraints functions
bool OdDbImpAssoc2dConstraintGroup::isConstraintOfThisTypeExist(const OdArray<OdGeomConstraint*> aConstr,
                                                                const OdRxClass* pObjType,
                                                                OdGeomConstraint** ppConstraint)
{
  unsigned int size = aConstr.size();

  for ( unsigned int i = 0; i < size; i++ )
  {
    if ( aConstr[i]->isA() == pObjType )
    {
      *ppConstraint = aConstr[i];
      return true;
    }
  }

  return false;
}

OdResult OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeObjects(OdArray<OdConstraintGroupNodePtr>& aResObj, OdRxClass* pObjType)
{
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    if (pNode->second->isA() == pObjType)
      aResObj.push_back(pNode->second);
  }
  if ( aResObj.length() > 0 )
    return eOk;
  else
    return eBadObjType;
}

bool OdDbImpAssoc2dConstraintGroup::findDatumLine(OdConstrainedDatumLine** ppDatumLine, const OdGeVector3d& dir)
{

  OdArray<OdConstraintGroupNodePtr> aDatumObj;
  OdResult res = getConstraintGroupNodeObjects(aDatumObj, OdConstrainedDatumLine::desc());
  if ( res != eOk )
  {
    *ppDatumLine = NULL;
    return false;
  }

  unsigned int size = aDatumObj.size();
  OdConstrainedDatumLine * pLine;
  OdGeVector3d curDir;
  for ( unsigned int i = 0; i < size; i++ )
  {
    pLine = OdConstrainedDatumLine::cast(aDatumObj[i]);
    curDir = curDir = pLine->direction();
    if ( curDir == dir )
    {
      *ppDatumLine = pLine;
      return true;
    }
  }

  *ppDatumLine = NULL;
  return false;
}

OdResult OdDbImpAssoc2dConstraintGroup::tieNode(OdConstraintGroupNodePtr node, OdDbObjectId actionId)
{
  OdConstraintGroupNodeImpl* nodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(node);
  nodeImpl->setNodeId(++m_seedId);
  if (!actionId.isNull())
    nodeImpl->setOwnerAction(actionId);
  if (!m_mapNodes.insert(std::make_pair(node->nodeId(), node)).second)
  {
    ODA_FAIL_ONCE();
    throw OdError(eDuplicateKey);
  }
  return eOk;
}

void OdDbImpAssoc2dConstraintGroup::tieGeomsAndCnstr3(OdGeomConstraint *pCnstr,
                                                      OdConstrainedGeometry* pGeomF,
                                                      OdConstrainedGeometry* pGeomS,
                                                      OdConstrainedGeometry* pGeomT)
{
  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);
  pGrNodeImpl->addConnection(pCnstr->nodeId());

  if (pGeomS) {
    pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomS);
    pGrNodeImpl->addConnection(pCnstr->nodeId());
  }

  if (pGeomT) {
    pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomT);
    pGrNodeImpl->addConnection(pCnstr->nodeId());
  }

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCnstr);
  pGrNodeImpl->addConnection(pGeomF->nodeId());
  if (pGeomS)
    pGrNodeImpl->addConnection(pGeomS->nodeId());
  if (pGeomT)
    pGrNodeImpl->addConnection(pGeomT->nodeId());
}

void OdDbImpAssoc2dConstraintGroup::tieDimAndCnstr(OdExplicitConstraint *pCnstr,
                                                   OdDbObjectId valueDependencyId,
                                                   OdDbObjectId dimDependencyId,
                                                   OdDbAssoc2dConstraintGroup *pGroup)
{
  addDependency(valueDependencyId, true, pGroup);
  pCnstr->setValueDependencyId(valueDependencyId);
  if (!dimDependencyId.isNull()) {
    addDependency(dimDependencyId, true, pGroup);
    pCnstr->setDimDependencyId(dimDependencyId);
  }
}

void OdDbImpAssoc2dConstraintGroup::tieGeomsAndCnstr(OdConstrainedGeometry* pGeomF,
                                                     OdConstrainedGeometry* pGeomS,
                                                     OdGeomConstraint *pCnstr)
{
  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);
  pGrNodeImpl->addConnection(pCnstr->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomS);
  pGrNodeImpl->addConnection(pCnstr->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCnstr);
  pGrNodeImpl->addConnection(pGeomF->nodeId());
  pGrNodeImpl->addConnection(pGeomS->nodeId());
}

void OdDbImpAssoc2dConstraintGroup::tieGeomsAndCnstrWithHeplPrm(OdConstrainedGeometry* pGeomF,
                                                                OdConstrainedGeometry* pGeomS,
                                                                OdGeomConstraint *pCnstr,
                                                                OdHelpParameter *pHelpPrmF,
                                                                OdHelpParameter *pHelpPrmS)
{
  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);
  pGrNodeImpl->addConnection(pCnstr->nodeId());
  if ( pHelpPrmF != NULL )
    pGrNodeImpl->addConnection(pHelpPrmF->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomS);
  pGrNodeImpl->addConnection(pCnstr->nodeId());
  if ( pHelpPrmS != NULL )
    pGrNodeImpl->addConnection(pHelpPrmS->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pCnstr);
  pGrNodeImpl->addConnection(pGeomF->nodeId());
  pGrNodeImpl->addConnection(pGeomS->nodeId());
  if ( pHelpPrmF != NULL )
    pGrNodeImpl->addConnection(pHelpPrmF->nodeId());

  if ( pHelpPrmS != NULL )
    pGrNodeImpl->addConnection(pHelpPrmS->nodeId());

  if ( pHelpPrmF != NULL )
  {
    pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pHelpPrmF);
    pGrNodeImpl->addConnection(pGeomF->nodeId());
    pGrNodeImpl->addConnection(pCnstr->nodeId());
  }

  if ( pHelpPrmS != NULL )
  {
    pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pHelpPrmS);
    pGrNodeImpl->addConnection(pGeomS->nodeId());
    pGrNodeImpl->addConnection(pCnstr->nodeId());
  }
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addHorizontalConstraint(OdConstrainedGeometry* pGeomF,
                                                                         OdDbObjectId actionId)
{
  return addVertHorzConstraint(pGeomF, actionId, OdHorizontalConstraint::desc(),
                                  OdGeVector3d::kXAxis, OdHorizontalConstraint::desc()->name());
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addHorizontalConstraint(OdConstrainedGeometry* pGeomF,
                                                                         OdConstrainedGeometry* pGeomS,
                                                                         OdDbObjectId actionId)
{
  return addVertHorzConstraint(pGeomF, pGeomS, actionId, OdHorizontalConstraint::desc(),
                                  OdGeVector3d::kXAxis, OdHorizontalConstraint::desc()->name());
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addVerticalConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdDbObjectId actionId)
{
  return addVertHorzConstraint(pGeomF, actionId, OdVerticalConstraint::desc(),
                                      OdGeVector3d::kYAxis, OdVerticalConstraint::desc()->name());
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addVerticalConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId)
{
  return addVertHorzConstraint(pGeomF, pGeomS, actionId, OdVerticalConstraint::desc(),
                                      OdGeVector3d::kYAxis, OdVerticalConstraint::desc()->name());
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addParallelConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedLine::desc()) || !pGeomS->isKindOf(OdConstrainedLine::desc()) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdParallelConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdParallelConstraint> spPrlCnstr = ::odrxCreateObject(OdParallelConstraint::desc()->name());
  tieNode(spPrlCnstr , actionId);

  res = spPrlCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addPerpendicularConstraint(OdConstrainedGeometry* pGeomF,
                                                                            OdConstrainedGeometry* pGeomS,
                                                                            OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedLine::desc()) || !pGeomS->isKindOf(OdConstrainedLine::desc()) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdPerpendicularConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdPerpendicularConstraint> spPrpCnstr = ::odrxCreateObject(OdPerpendicularConstraint::desc()->name());
  tieNode(spPrpCnstr, actionId);

  res = spPrpCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addNormalConstraint(OdConstrainedGeometry* pGeomF,
                                                                     OdConstrainedGeometry* pGeomS,
                                                                     OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( (!pGeomF->isKindOf(OdConstrainedLine::desc()) || !pGeomS->isKindOf(OdConstrainedCircle::desc())) &&
       (!pGeomF->isKindOf(OdConstrainedCircle::desc()) || !pGeomS->isKindOf(OdConstrainedLine::desc())) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdNormalConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdNormalConstraint> spNrmCnstr = ::odrxCreateObject(OdNormalConstraint::desc()->name());
  tieNode(spNrmCnstr, actionId);

  res = spNrmCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addColinearConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedLine::desc()) || !pGeomS->isKindOf(OdConstrainedLine::desc()) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdColinearConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdColinearConstraint> spClnCnstr = ::odrxCreateObject(OdColinearConstraint::desc()->name());
  tieNode(spClnCnstr, actionId);

  res = spClnCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addCoincidentConstraint(OdConstrainedGeometry* pGeomF,
                                                                         OdConstrainedGeometry* pGeomS,
                                                                         OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !(pGeomF->isKindOf(OdConstrainedPoint::desc()) || pGeomS->isKindOf(OdConstrainedPoint::desc())) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  if ( pGeomF->isKindOf(OdConstrainedImplicitPoint::desc()) && pGeomS->isKindOf(OdConstrainedImplicitPoint::desc()) )
  {
    OdConstrainedImplicitPoint *pImplPtF = OdConstrainedImplicitPoint::cast(pGeomF);
    OdConstrainedImplicitPoint *pImplPtS = OdConstrainedImplicitPoint::cast(pGeomS);
    if ( pImplPtF->constrainedCurveId() == pImplPtS->constrainedCurveId() )
      return res;
  }

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdPointCoincidenceConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdPointCoincidenceConstraint> spCoiCnstr = ::odrxCreateObject(OdPointCoincidenceConstraint::desc()->name());
  tieNode(spCoiCnstr, actionId);

  res = spCoiCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}


OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addConcentricConstraint(OdConstrainedGeometry* pGeomF,
                                                                         OdConstrainedGeometry* pGeomS,
                                                                         OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( (!pGeomF->isKindOf(OdConstrainedCircle::desc()) && !pGeomF->isKindOf(OdConstrainedEllipse::desc())) ||
       (!pGeomS->isKindOf(OdConstrainedCircle::desc()) && !pGeomS->isKindOf(OdConstrainedEllipse::desc())) )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdConcentricConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdConcentricConstraint> spClnCnstr = ::odrxCreateObject(OdConcentricConstraint::desc()->name());
  tieNode(spClnCnstr, actionId);

  res = spClnCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addTangentConstraint(OdConstrainedGeometry* pGeomF,
                                                                      OdConstrainedGeometry* pGeomS,
                                                                      OdHelpParameter* pHelpParamF,
                                                                      OdHelpParameter* pHelpParamS,
                                                                      OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( pHelpParamF == NULL && pHelpParamS == NULL )
  {
    bool bLineF = pGeomF->isKindOf(OdConstrainedLine::desc());
    if ( bLineF )
    {
      if ( !pGeomS->isKindOf(OdConstrainedCircle::desc()) && !pGeomS->isKindOf(OdConstrainedEllipse::desc()) )
        return res;

    }
    else if ( !pGeomF->isKindOf(OdConstrainedCircle::desc()) && !pGeomF->isKindOf(OdConstrainedEllipse::desc()) )
      return res;
    else if ( !pGeomS->isKindOf(OdConstrainedCircle::desc()) && !pGeomS->isKindOf(OdConstrainedEllipse::desc()) && 
              !pGeomS->isKindOf(OdConstrainedLine::desc()))
      return res;
  }
  else
  {
    if ( pHelpParamF != NULL && pHelpParamS != NULL )
    {
      if ( !pGeomF->isKindOf(OdConstrainedSpline::desc()) && !pGeomS->isKindOf(OdConstrainedSpline::desc()) )
        return res;
    }
    else 
		{
    OdConstrainedGeometry *pGeom = pHelpParamF == NULL ? pGeomF : pGeomS;
    if ( !pGeom->isKindOf(OdConstrainedArc::desc()) && !pGeom->isKindOf(OdConstrainedBoundedEllipse::desc()) &&
          !pGeom->isKindOf(OdConstrainedBoundedLine::desc()))
          return res;
		}
  }

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  OdSmartPtr<OdTangentConstraint> spTanCnstr = ::odrxCreateObject(OdTangentConstraint::desc()->name());
  tieNode(spTanCnstr, actionId);

  res = spTanCnstr.get();
  tieGeomsAndCnstrWithHeplPrm(pGeomF, pGeomS, res, pHelpParamF, pHelpParamS);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addEqualRadiusConstraint(OdConstrainedGeometry* pGeomF,
                                                                          OdConstrainedGeometry* pGeomS,
                                                                          OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedCircle::desc()) || !pGeomS->isKindOf(OdConstrainedCircle::desc()))
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdEqualRadiusConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdEqualRadiusConstraint> spEqrCnstr = ::odrxCreateObject(OdEqualRadiusConstraint::desc()->name());
  tieNode(spEqrCnstr, actionId);

  res = spEqrCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addEqualLengthConstraint(OdConstrainedGeometry* pGeomF,
                                                                          OdConstrainedGeometry* pGeomS,
                                                                          OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedBoundedLine::desc()) || !pGeomS->isKindOf(OdConstrainedBoundedLine::desc()))
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdEqualLengthConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdEqualLengthConstraint> spEqlCnstr = ::odrxCreateObject(OdEqualLengthConstraint::desc()->name());
  tieNode(spEqlCnstr, actionId);

  res = spEqlCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addSymmetricConstraint(OdConstrainedGeometry* pGeomF,
                                                                        OdConstrainedGeometry* pGeomS,
                                                                        OdConstrainedGeometry* pGeomLine,
                                                                        OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomLine->isKindOf(OdConstrainedLine::desc()) )
    return res;

  if ( pGeomF->isA() != pGeomS->isA() )
  {
    
    if ( !pGeomF->isKindOf(OdConstrainedPoint::desc()) || !pGeomS->isKindOf(OdConstrainedPoint::desc()) )
      return res;
  }

  if ( pGeomF->isKindOf(OdConstrainedSpline::desc()) || pGeomS->isKindOf(OdConstrainedSpline::desc()) )
    return res;

  OdArray<OdGeomConstraint*> aConstr0;
  pGeomF->getCommonConstraints(pGeomS, aConstr0);

  OdArray<OdGeomConstraint*> aConstr1;
  pGeomF->getCommonConstraints(pGeomLine, aConstr1);

  unsigned int i, j, size0 = aConstr0.length(), size1 = aConstr1.length();

  for ( i = 0; i < size0; i++ )
  {
    for ( j = 0; j < size1; j++ )
    {
      if ( aConstr0[i]->nodeId() == aConstr1[j]->nodeId() )
      {
        if ( aConstr0[i]->isA() == OdSymmetricConstraint::desc() )
          return res;
      }
    }
  }

  OdSmartPtr<OdSymmetricConstraint> spSymCnstr = ::odrxCreateObject(OdSymmetricConstraint::desc()->name());
  tieNode(spSymCnstr, actionId);

  res = spSymCnstr.get();
  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomLine);
  pGrNodeImpl->addConnection(res ->nodeId());

  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(res);
  pGrNodeImpl->addConnection(pGeomLine->nodeId());

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addG2SmoothConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);
  OdGeomConstraint *pCoiCnstr = NULL;
  if ( !isConstraintOfThisTypeExist(aConstr, OdPointCoincidenceConstraint::desc(), &pCoiCnstr) )
  {
    pCoiCnstr = addCoincidentConstraint(pGeomF, pGeomS, actionId);
    if (!pCoiCnstr)
      return res;
	}

  if ( pCoiCnstr->getOwningCompositeConstraint() != NULL )
    return res;

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdConstrainedImplicitPoint *pPointF;
  OdConstrainedImplicitPoint *pPointS;

  if ( pGeomF->isKindOf(OdConstrainedImplicitPoint::desc()) )
    pPointF = OdConstrainedImplicitPoint::cast(pGeomF);
  else
    return res;

  if ( pGeomS->isKindOf(OdConstrainedImplicitPoint::desc()) )
    pPointS = OdConstrainedImplicitPoint::cast(pGeomS);
  else
    return res;

  OdConstrainedGeometry *pCnstrGeomF = OdConstrainedGeometry::cast(getGroupNodePtr(pPointF->constrainedCurveId()));
  OdConstrainedGeometry *pCnstrGeomS = OdConstrainedGeometry::cast(getGroupNodePtr(pPointS->constrainedCurveId()));

  //add tangency
  OdHelpParameter* pHlpPrmF = OdDbImpAssoc2dConstraintGroup::createHelpParameter(pPointF, actionId);
  OdHelpParameter* pHlpPrmS = OdDbImpAssoc2dConstraintGroup::createHelpParameter(pPointS, actionId);

  if ( pHlpPrmF == NULL && pHlpPrmS == NULL )
    return res;

  OdGeomConstraint *pTngCnstr = addTangentConstraint(pCnstrGeomF, pCnstrGeomS, pHlpPrmF, pHlpPrmS, actionId);
  if ( pTngCnstr == NULL )
    return res;

  //add EqualCurvature
  pHlpPrmF = OdDbImpAssoc2dConstraintGroup::createHelpParameter(pPointF, actionId);
  pHlpPrmS = OdDbImpAssoc2dConstraintGroup::createHelpParameter(pPointS, actionId);

  OdGeomConstraint *pEqCurvCnstr = addEqualCurvatureConstraint(pCnstrGeomF, pCnstrGeomS, pHlpPrmF, pHlpPrmS, actionId);
  if ( pEqCurvCnstr == NULL )
    return res;

  OdSmartPtr<OdG2SmoothConstraint> spG2Cnstr = ::odrxCreateObject(OdG2SmoothConstraint::desc()->name());
  tieNode(spG2Cnstr, actionId);

  res = spG2Cnstr.get();
  OdCompositeConstraintImpl* nodeImpl = (OdCompositeConstraintImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(res);
  nodeImpl->addOwnedConstraint(pCoiCnstr);
  nodeImpl->addOwnedConstraint(pTngCnstr);
  nodeImpl->addOwnedConstraint(pEqCurvCnstr);

  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addFixConstraint(OdConstrainedGeometry* pGeom, OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  OdArray<OdGeomConstraint*> aConstr;
  pGeom->getConnectedConstraints(aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, OdFixedConstraint::desc(), &res) )
    return NULL;

  OdSmartPtr<OdFixedConstraint> spFixCnstr = ::odrxCreateObject(OdFixedConstraint::desc()->name());
  tieNode(spFixCnstr, actionId);

  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeom);
  pGrNodeImpl->addConnection(spFixCnstr->nodeId());

  res = spFixCnstr.get();
  pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(res);
  pGrNodeImpl->addConnection(pGeom->nodeId());
  
  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addEqualCurvatureConstraint(OdConstrainedGeometry* pGeomF,
                                                                             OdConstrainedGeometry* pGeomS,
                                                                             OdHelpParameter* pHelpParamF,
                                                                             OdHelpParameter* pHelpParamS,
                                                                             OdDbObjectId actionId)
{
  OdGeomConstraint* res = NULL;

  if ( pHelpParamF != NULL && pHelpParamS != NULL )
  {
    if ( !pGeomF->isKindOf(OdConstrainedSpline::desc()) && !pGeomS->isKindOf(OdConstrainedSpline::desc()) )
      return res;
  }
  else
  {
    OdConstrainedGeometry *pGeom = pHelpParamF == NULL ? pGeomF : pGeomS;
	  if ( !pGeom->isKindOf(OdConstrainedArc::desc()) && !pGeom->isKindOf(OdConstrainedBoundedEllipse::desc()) &&
        !pGeom->isKindOf(OdConstrainedBoundedLine::desc()))
        return res;
	}

  if ( pGeomF->nodeId() == pGeomS->nodeId() )
    return res;

  OdSmartPtr<OdEqualCurvatureConstraint> spTanCnstr = ::odrxCreateObject(OdEqualCurvatureConstraint::desc()->name());
  tieNode(spTanCnstr, actionId);

  res = spTanCnstr.get();
  tieGeomsAndCnstrWithHeplPrm(pGeomF, pGeomS, res, pHelpParamF, pHelpParamS);

  return res;
}

OdHelpParameter* OdDbImpAssoc2dConstraintGroup::createHelpParameter(OdConstrainedImplicitPoint *pPoint, OdDbObjectId actionId)
{
  if ( pPoint->pointType() != OdConstrainedImplicitPoint::kDefineImplicit )
    return NULL;

  OdConstraintGroupNode *node = getGroupNodePtr(pPoint->constrainedCurveId());
  if ( node == NULL )
    return NULL;

  if ( !node->isKindOf(OdConstrainedSpline::desc()) )
    return NULL;

  OdConstrainedSpline *spline = OdConstrainedSpline::cast(node);
  OdGeNurbCurve3d nurb = spline->nurbSpline();

  double val = pPoint->pointIndex() == 0 ? nurb.startParam() : nurb.endParam();
  
  OdSmartPtr<OdHelpParameter> spHelpParam = ::odrxCreateObject(OdHelpParameter::desc()->name());
  spHelpParam->setValue(val);

  tieNode(spHelpParam, actionId);
  return spHelpParam.get();
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addVertHorzConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdDbObjectId actionId,
                                                                       const OdRxClass *desc,
                                                                       const OdGeVector3d dir,
                                                                       const OdString name)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedLine::desc()) )
    return res;
  
  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getConnectedConstraints(aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, desc, &res) )
    return NULL;

  OdConstrainedDatumLine *pDatumLine = NULL;
  if ( !findDatumLine(&pDatumLine, dir) )
  {
    OdSmartPtr<OdConstrainedDatumLine> spDatumLine =
        ::odrxCreateObject(OdConstrainedDatumLine::desc()->name());

    tieNode(spDatumLine, actionId);
    pDatumLine = spDatumLine.get();

    OdConstrainedDatumLineImpl *pDatumImpl = (OdConstrainedDatumLineImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pDatumLine);

    pDatumImpl->init(OdGePoint3d::kOrigin, dir);
  }

  OdSmartPtr<OdParallelConstraint> spVHCnstr = ::odrxCreateObject(name);
  tieNode(spVHCnstr, actionId);

  OdConstraintGroupNodeImpl *pGrNodeImpl = OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pGeomF);
  pGrNodeImpl->addConnection(spVHCnstr->nodeId());

  OdParallelConstraintImpl *pConstrImpl = (OdParallelConstraintImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(spVHCnstr);
  pConstrImpl->addConnection(pGrNodeImpl->nodeId());
  pConstrImpl->setDatumLineIdx(pDatumLine->nodeId());

  res = spVHCnstr.get();
  return res;
}

OdGeomConstraint* OdDbImpAssoc2dConstraintGroup::addVertHorzConstraint(OdConstrainedGeometry* pGeomF,
                                                                       OdConstrainedGeometry* pGeomS,
                                                                       OdDbObjectId actionId,
                                                                       const OdRxClass *desc,
                                                                       const OdGeVector3d dir,
                                                                       const OdString name)
{
  OdGeomConstraint* res = NULL;

  if ( !pGeomF->isKindOf(OdConstrainedPoint::desc()) || !pGeomS->isKindOf(OdConstrainedPoint::desc()) )
    return res;

  OdArray<OdGeomConstraint*> aConstr;
  pGeomF->getCommonConstraints(pGeomS, aConstr);

  if ( isConstraintOfThisTypeExist(aConstr, desc, &res) )
    return NULL;

  OdConstrainedDatumLine *pDatumLine = NULL;
  if ( !findDatumLine(&pDatumLine, dir) )
  {
    OdSmartPtr<OdConstrainedDatumLine> spDatumLine = ::odrxCreateObject(OdConstrainedDatumLine::desc()->name());
    tieNode(spDatumLine, actionId);
    pDatumLine = spDatumLine.get();

    OdConstrainedDatumLineImpl *pDatumImpl = (OdConstrainedDatumLineImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(pDatumLine);
    pDatumImpl->init(OdGePoint3d::kOrigin, dir);
  }

  OdConstrainedLine *p2PointsLine = NULL;
  if ( pGeomF->isKindOf(OdConstrainedImplicitPoint::desc()) && pGeomS->isKindOf(OdConstrainedImplicitPoint::desc()) )
  {
    OdConstrainedImplicitPoint *pImplPointF = OdConstrainedImplicitPoint::cast(pGeomF);
    OdConstrainedImplicitPoint *pImplPointS = OdConstrainedImplicitPoint::cast(pGeomS);
  
    if ( pImplPointF->constrainedCurveId() == pImplPointS->constrainedCurveId() )
    {
      OdConstraintGroupNode *pCurve = getGroupNodePtr(pImplPointF->constrainedCurveId());
      ODA_ASSERT_ONCE(pCurve);
      if (pCurve && pCurve->isKindOf(OdConstrainedBoundedLine::desc()))
        p2PointsLine = OdConstrainedBoundedLine::cast(pCurve);
    }
  }

  if ( p2PointsLine == NULL )
  {
    OdSmartPtr<OdConstrained2PointsConstructionLine> sp2PointsLine =
      ::odrxCreateObject(OdConstrained2PointsConstructionLine::desc()->name());

    tieNode(sp2PointsLine, actionId);

    OdSmartPtr<OdPointCurveConstraint> spPointCurveCnstrF =
      ::odrxCreateObject(OdPointCurveConstraint::desc()->name());

    tieNode(spPointCurveCnstrF, actionId);

    tieGeomsAndCnstr(pGeomF, sp2PointsLine, spPointCurveCnstrF);

    OdSmartPtr<OdPointCurveConstraint> spPointCurveCnstrS =
      ::odrxCreateObject(OdPointCurveConstraint::desc()->name());

    tieNode(spPointCurveCnstrS, actionId);

    tieGeomsAndCnstr(pGeomS, sp2PointsLine, spPointCurveCnstrS);
  }

  OdSmartPtr<OdParallelConstraint> spVHCnstr = ::odrxCreateObject(name);
  tieNode(spVHCnstr, actionId);

  res = spVHCnstr.get();

  OdParallelConstraintImpl *pConstrImpl = (OdParallelConstraintImpl*)OdDbImpAssoc2dConstraintGroup::getConstraintGroupNodeImpl(res);
  pConstrImpl->setDatumLineIdx(pDatumLine->nodeId());

  tieGeomsAndCnstr(pGeomF, pGeomS, res);

  return res;
}

void OdDbImpAssoc2dConstraintGroup::postReadWork()
{
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    if ( pNode->second->isKindOf(OdDistanceConstraint::desc()) )
    {
      OdDistanceConstraintImpl *pDstCnstrImpl = (OdDistanceConstraintImpl*)getConstraintGroupNodeImpl(pNode->second);
      pDstCnstrImpl->fillCurrentStateObj();
    }
  }
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addConstrainedGeometry(OdDbAssocGeomDependencyPtr pGeomDep)
{
  OdConstrainedGeometry *res = NULL;
  if ( pGeomDep->subentType() == OdDb::kVertexSubentType )
  {

    res = addPoint(pGeomDep->objectId());
  }
  else if ( pGeomDep->subentType() == OdDb::kEdgeSubentType )
  {
    OdArray<OdGeCurve3d*> arrCurve;
    if ( pGeomDep->getEdgeSubentityGeometry(arrCurve) != eOk )
      return NULL;

    if ( arrCurve[0]->isKindOf(OdGe::kCircArc3d) )
      if ( arrCurve[0]->isClosed() )
        res = addCircle(pGeomDep->objectId());
      else
        res = addArc(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kLineSeg3d) )
      res = addBoundedLine(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kRay3d) )
      res = addRay(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kLine3d) )
      res = addXLine(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kEllipArc3d) )
      if ( arrCurve[0]->isClosed() )
        res = addEllipse(pGeomDep->objectId());
      else
        res = addEllipseArc(pGeomDep->objectId());
    else if ( arrCurve[0]->isKindOf(OdGe::kNurbCurve3d) )
        res = addSpline(pGeomDep->objectId(), *(OdGeNurbCurve3d*)arrCurve[0]);

    delete arrCurve[0];
    arrCurve.clear();
  }

  return res;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addPoint(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedPoint> pPt = OdConstrainedPoint::createObject();
  pPt->init(geomDepId);
  tieNode(pPt, OdDbObjectId::kNull);
  return pPt;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addCircle(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedCircle> pCircle = OdConstrainedCircle::createObject();
  pCircle->init(geomDepId);
  tieNode(pCircle, OdDbObjectId::kNull);

  addImplicitPoint(pCircle, OdConstrainedImplicitPoint::kCenterImplicit, -1);

  return pCircle;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addArc(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedArc> pArc = OdConstrainedArc::createObject();
  pArc->init(geomDepId);
  tieNode(pArc, OdDbObjectId::kNull);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kCenterImplicit, -1);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kStartImplicit, -1);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kEndImplicit, -1);

  return pArc;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addBoundedLine(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedBoundedLine> pLine = OdConstrainedBoundedLine::createObject();
  pLine->init(geomDepId, false);
  tieNode(pLine, OdDbObjectId::kNull);

  addImplicitPoint(pLine, OdConstrainedImplicitPoint::kStartImplicit, -1);

  addImplicitPoint(pLine, OdConstrainedImplicitPoint::kEndImplicit, -1);

  return pLine;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addRay(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedBoundedLine> pLine = OdConstrainedBoundedLine::createObject();
  pLine->init(geomDepId, true);
  tieNode(pLine, OdDbObjectId::kNull);

  addImplicitPoint(pLine, OdConstrainedImplicitPoint::kStartImplicit, -1);

  return pLine;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addXLine(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedLine> pLine = OdConstrainedLine::createObject();
  pLine->init(geomDepId);
  tieNode(pLine, OdDbObjectId::kNull);

  return pLine;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addEllipse(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedEllipse> pEllipse = OdConstrainedEllipse::createObject();
  pEllipse->init(geomDepId);
  tieNode(pEllipse, OdDbObjectId::kNull);

  addImplicitPoint(pEllipse, OdConstrainedImplicitPoint::kCenterImplicit, -1);

  return pEllipse;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addEllipseArc(OdDbObjectId geomDepId)
{
  OdSmartPtr<OdConstrainedBoundedEllipse> pArc = OdConstrainedBoundedEllipse::createObject();
  pArc->init(geomDepId);
  tieNode(pArc, OdDbObjectId::kNull);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kCenterImplicit, -1);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kStartImplicit, -1);

  addImplicitPoint(pArc, OdConstrainedImplicitPoint::kEndImplicit, -1);

  return pArc;
}

OdConstrainedGeometry* OdDbImpAssoc2dConstraintGroup::addSpline(OdDbObjectId geomDepId, const OdGeNurbCurve3d& spline)
{
  OdSmartPtr<OdConstrainedSpline> pSpline = OdConstrainedSpline::createObject();
  pSpline->init(geomDepId, spline);
  tieNode(pSpline, OdDbObjectId::kNull);

  int ptNum = spline.numControlPoints();
  for ( int i = 0; i < ptNum; i++ )
    addImplicitPoint(pSpline, OdConstrainedImplicitPoint::kDefineImplicit, i);

  return pSpline;
}

OdConstrainedImplicitPoint* OdDbImpAssoc2dConstraintGroup::addImplicitPoint(OdConstrainedCurve *pCurve, OdConstrainedImplicitPoint::ImplicitPointType type, int index)
{
  OdSmartPtr<OdConstrainedImplicitPoint> pPoint = OdConstrainedImplicitPoint::createObject();
  pPoint->init(pCurve->nodeId(), type, index);
  tieNode(pPoint, pCurve->owningConstraintGroupId());

  if ( type == OdConstrainedImplicitPoint::kDefineImplicit )
    return pPoint;

  OdSmartPtr<OdGeomConstraint> pCnstr;
  if ( type == OdConstrainedImplicitPoint::kCenterImplicit )
    pCnstr = OdCenterPointConstraint::createObject();
  else if ( type == OdConstrainedImplicitPoint::kMidImplicit)
    pCnstr = OdMidPointConstraint::createObject();
  else
    pCnstr = OdPointCurveConstraint::createObject();

  tieNode(pCnstr, pCurve->owningConstraintGroupId());

  if (type == OdConstrainedImplicitPoint::kMidImplicit) {
    tieGeomsAndCnstr(pPoint, pCurve, pCnstr);
    //additional p-c constraint to keep point on line
    pCnstr = OdPointCurveConstraint::createObject();
    tieNode(pCnstr, pCurve->owningConstraintGroupId());
  }

  tieGeomsAndCnstr(pCurve, pPoint, pCnstr);

  return pPoint;
}

void OdDbImpAssoc2dConstraintGroup::doPostEvaluateJob(OdDbAssocAction *pAction)
{
  OdDbDatabase* pDB = pAction->database();
  pDB->startTransaction();

  /*
  OdGeMatrix3d matrToWrd = OdGeMatrix3d::planeToWorld(m_plane);
  int size = m_arrNodes.size();
  for ( int i = 0; i < size; i++ )
  {
    if ( m_arrNodes[i]->isKindOf(OdConstrainedGeometry::desc()) )
      (OdConstrainedGeometry::cast(m_arrNodes[i]))->updateDbObject(matrToWrd);
    else if ( m_arrNodes[i]->isKindOf(OdExplicitConstraint::desc()) )
    {
      (OdExplicitConstraint::cast(m_arrNodes[i]))->updateDimDependency(matrToWrd);
    }
  }
  */

  OdDbAssoc2dConstraintGroupPtr pCnstrGr = OdDbAssoc2dConstraintGroup::cast(pAction);

  OdConstrainedGeometry *pGeomObj;
  unsigned int i;
  for ( i = 0; i < m_arrDependencies.length(); i++ )
  {
    OdDbObjectPtr pObj = m_arrDependencies[i].openObject();
    if ( pObj->isKindOf(OdDbAssocGeomDependency::desc()) )
    {
      OdDbAssocGeomDependencyPtr pGeomDep = OdDbAssocGeomDependency::cast(pObj);
      if ( pCnstrGr->getConstrainedGeometry(pGeomDep, pGeomObj, NULL, -1, false) != eOk )
        continue;
      
      if ( ((OdConstrainedGeometryImpl*)getConstraintGroupNodeImpl(pGeomObj))->isModified() )
      {
        pGeomDep->upgradeOpen();
        pGeomDep->setStatus(kChangedTransitivelyAssocStatus, false);
      }
    }
  }
  //geomDepPtr->setStatus(kIsUpToDateAssocStatus, false);
  
  for ( i = 0; i < m_arrDependencies.length(); i++ )
  {
    OdDbObjectPtr pObj = m_arrDependencies[i].openObject();
    if ( pObj->isKindOf(OdDbAssocGeomDependency::desc()) )
    {
      OdDbAssocGeomDependencyPtr pGeomDep = OdDbAssocGeomDependency::cast(pObj);
      if ( pGeomDep->status() == kIsUpToDateAssocStatus )
        continue;

      pGeomDep->updateDependentOnObject();
    }
    else if ( pObj->isKindOf(OdDbAssocValueDependency::desc()) )
    {
      OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(pObj);
      pDep->upgradeOpen();
      pDep->setStatus(kIsUpToDateAssocStatus, false);
    }
    else if ( pObj->isKindOf(OdDbAssocDependency::desc()) )
    {
      OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(pObj);
      pDep->updateDependentOnObject();
    }
  }
  pDB->endTransaction();
}

void OdDbImpAssoc2dConstraintGroup::composeForLoad( OdDbObject* pObj, 
                                         OdDb::SaveType format,
                                         OdDb::DwgVersion version,
                                         OdDbAuditInfo* pAuditInfo )
{
  OdDbImpAssocAction::composeForLoad(pObj, format, version, pAuditInfo );
  if (m_mapNodes.find(UINT_MAX) != m_mapNodes.end())
  {
    ODA_FAIL_ONCE(); // nodeId were duplicated
    OdDbHostAppServices* pHostApp = pObj->database()->appServices();
    pHostApp->warning(pHostApp->formatMessage(sidRecvObjInvalidData, odDbGetObjectName(pObj).c_str()));
    OdDbAssocAction* pAction = (OdDbAssocAction*)pObj;
    pAction->removeAllDependencies(false);
    m_mapNodes.clear();
    OdDbAssocNetworkPtr pOwner = OdDbAssocNetwork::cast(pAction->owningNetwork().openObject(OdDb::kForWrite));
    if (!pOwner.isNull())
      pOwner->removeAction(pObj->objectId(), false);
    return;
  }
  postReadWork();
}

OdResult OdDbImpAssoc2dConstraintGroup::removeNode(OdConstraintGroupNode* pNode)
{
  OdConstraintGroupNodeMap::iterator it = m_mapNodes.find(pNode->nodeId());
  if (it != m_mapNodes.end())
  {
    m_mapNodes.erase(it);
    return eOk;
  }
  else
    return eInvalidKey;
}

void OdDbImpAssoc2dConstraintGroup::audit(OdDbAssoc2dConstraintGroup* pGroup, OdDbAuditInfo* pAuditInfo)
{
  int numErrorsFound = pAuditInfo->numErrors();
  for (OdConstraintGroupNodeMap::const_iterator pNode = m_mapNodes.begin(); pNode != m_mapNodes.end(); ++pNode)
  {
    OdSmartPtr<OdConstrainedCurve> pConsCurve = OdConstrainedCurve::cast(pNode->second);
    if (pConsCurve.isNull())
      continue;
    
    dynamic_cast<OdConstrainedCurveImpl*>(pConsCurve->m_pImpl)->audit(pConsCurve.get(), pAuditInfo);
  }

  if (numErrorsFound != pAuditInfo->numErrors())
  {
    OdDbHostAppServices* pHostApp = pGroup->database()->appServices();
    pAuditInfo->printError(pGroup,
                           pHostApp->formatMessage(sidInvalidConstraintGeometry),
                           pHostApp->formatMessage(sidVarValidInvalid),
                           pHostApp->formatMessage(sidVarDefRepair));
  }
}
