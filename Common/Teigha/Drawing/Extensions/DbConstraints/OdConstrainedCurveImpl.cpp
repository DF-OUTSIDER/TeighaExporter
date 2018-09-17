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
#include "OdConstrainedCurveImpl.h"

#include "DbAssocEdgePersSubentId.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbPolyline.h"

OdConstrainedCurveImpl::OdConstrainedCurveImpl():OdConstrainedGeometryImpl()
{
  
}

void OdConstrainedCurveImpl::getConstrainedImplicitPoints(OdArray<OdConstrainedImplicitPoint*>& apImplicitPoints) const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  OdArray<OdConstrainedGeometry*> apGeoms;
  if ( pGr->getConstrainedGeometries(apGeoms) != eOk )
    return;

  int sz = apGeoms.size();
  OdSmartPtr<OdConstrainedImplicitPoint> pImplPoint;
  for ( int i = 0; i < sz; i++ )
    if ( apGeoms[i]->isKindOf(OdConstrainedImplicitPoint::desc()) )
    {
      pImplPoint = OdConstrainedImplicitPoint::cast(apGeoms[i]);
      if (  !pImplPoint.isNull() && pImplPoint->constrainedCurveId() == nodeId() )
        apImplicitPoints.push_back(pImplPoint);
    }
}

OdResult OdConstrainedCurveImpl::getPolylineSegmentPtIdx(const OdDbObjectId geomDependencyId, const OdDbPolylinePtr& pLinePtr,
                                                     int& startIdx, int& endIdx) const
{
  const OdDbAssocPersSubentIdPtr persSubentIdPtr = geomDependency(geomDependencyId)->persistentSubentId();
    
  if ( !persSubentIdPtr->isKindOf(OdDbAssocEdgePersSubentId::desc()) )
  {
    ODA_FAIL();
    return eNotImplementedYet;
  }

  OdDbAssocEdgePersSubentIdPtr edgePersSubentIdPtr = OdDbAssocEdgePersSubentId::cast(persSubentIdPtr);
  int vIdentifier, stopFlag = 0;
  OdGePoint3d pt;
  for ( unsigned int i = 0; i < pLinePtr->numVerts(); i++ )
  {
    vIdentifier = pLinePtr->getVertexIdentifierAt(i);
    if ( vIdentifier == edgePersSubentIdPtr->index1() )
    {
      startIdx = i;
      stopFlag |= 1;
    }
    else if ( vIdentifier == edgePersSubentIdPtr->index2() )
    {
      endIdx = i;
      stopFlag |= 2;
    }
    
    if ( (stopFlag & 3) == 3 )
    {
      if (endIdx != ((startIdx + 1) % pLinePtr->numVerts()))
      {
        ODA_FAIL_ONCE();
        endIdx = (startIdx + 1) % pLinePtr->numVerts();
      }
      return eOk;
    }
  }

  if (pLinePtr->numVerts() == 2) // 2-point polyline without vertex identifiers
  {
    startIdx = 0;
    endIdx = 1;
    return eOk;
  }
  // check if only one vertex found
  if (stopFlag & 1)
  {
    ODA_FAIL_ONCE();
    endIdx = (startIdx + 1) % pLinePtr->numVerts();
    return eOk;
  }
  if (stopFlag & 2)
  {
    ODA_FAIL_ONCE();
    startIdx = (endIdx + pLinePtr->numVerts() - 1) % pLinePtr->numVerts();
    return eOk;
  }

  return eBadObjType;
}

OdResult OdConstrainedCurveImpl::getPolylineSegmentProp(const OdDbObjectId geomDependencyId, const OdDbPolylinePtr& pLinePtr,
                                                                          OdGePoint3d& wrdStart, OdGePoint3d& wrdEnd, double& bulge) const
{
  int startIdx, endIdx;
  if ( getPolylineSegmentPtIdx(geomDependencyId, pLinePtr, startIdx, endIdx) != eOk )
    return eBadObjType;

  pLinePtr->getPointAt(startIdx, wrdStart);
  bulge = pLinePtr->getBulgeAt(startIdx);

  pLinePtr->getPointAt(endIdx, wrdEnd);
  return eOk;
}/*getPolylineSegmentProp*/

OdResult OdConstrainedCurveImpl::getFullSubentPaths(OdDbFullSubentPathArray& subentPaths) const
{
  OdConstrainedGeometryImpl::getFullSubentPaths(subentPaths);
  OdDbFullSubentPath path;

  OdResult res = getFullSubentPath(path);
  if ( res != eOk )
    return res;

  subentPaths.push_back(path);
  return eOk;
 }
