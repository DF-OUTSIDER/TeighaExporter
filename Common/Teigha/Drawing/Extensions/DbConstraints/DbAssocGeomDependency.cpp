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
#include "DbAssocGeomDependency.h"
#include "DbImpAssocGeomDependency.h"
#include "DbFiler.h"
#include "DbEntity.h"
#include "DbAssocActionBody.h"
//#include "DbDatabase.h"
//#include "../Source/database/DbSystemInternals.h"
#ifdef _DEBUG
#include "DbAssocManager.h"
#endif

ODRX_DEFINE_MEMBERS_EX(OdDbAssocGeomDependency,    // ClassName
                       OdDbAssocDependency,        // ParentClass
                       DBOBJECT_CONSTR,            // DOCREATE
                       OdDb::kDHL_1021,            // DwgVer
                       OdDb::kMRelease6,           // MaintVer
                       1025,                       // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocGeomDependency", // DWG class name
                       L"ACDBASSOCGEOMDEPENDENCY", // DxfName
                       L"ObjectDBX Classes",       // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocGeomDependency::OdDbAssocGeomDependency(bool createImp) : OdDbAssocDependency(false)
{
  if ( createImp )
    m_pImpObj = new OdDbImpAssocGeomDependency();
}

OdDbAssocPersSubentIdPtr OdDbAssocGeomDependency::persistentSubentId() const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->persistentSubentId();
}

OdResult OdDbAssocGeomDependency::getTransientSubentIds(OdArray<OdDbSubentId>& transientSubentIds) const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->getTransientSubentIds(transientSubentIds);
}

OdResult OdDbAssocGeomDependency::setSubentity(const OdDbSubentId& transientSubentId)
{
  assertWriteEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->setSubentity(transientSubentId);
}

OdDb::SubentType OdDbAssocGeomDependency::subentType() const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->subentType();
}

int OdDbAssocGeomDependency::transientSubentCount() const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->transientSubentCount();
}

bool OdDbAssocGeomDependency::isCachingSubentityGeometry() const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->isCachingSubentityGeometry();
}

void OdDbAssocGeomDependency::setCachingSubentityGeometry(bool bCaching)
{
  assertWriteEnabled();
  ((OdDbImpAssocGeomDependency*)m_pImpObj)->setCachingSubentityGeometry(bCaching);
}

OdResult OdDbAssocGeomDependency::getVertexSubentityGeometry(OdGePoint3dArray& vertexPositions) const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->getVertexSubentityGeometry(vertexPositions);
}

OdResult OdDbAssocGeomDependency::getVertexSubentityGeometryFromPersSubentiy(OdGeCurve3d* & geometry, OdGePoint3dArray& vertexPositions) const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->getVertexSubentityGeometryFromPersSubentiy(geometry, vertexPositions);
}

OdResult OdDbAssocGeomDependency::getEdgeSubentityGeometry(OdArray<OdGeCurve3d*>& edgeCurves) const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->getEdgeSubentityGeometry(edgeCurves);
}

OdResult OdDbAssocGeomDependency::getFaceSubentityGeometry(OdArray<OdGeSurface*>& faceSurfaces) const
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->getFaceSubentityGeometry(faceSurfaces);
}

OdResult OdDbAssocGeomDependency::setVertexSubentityGeometry(const OdGePoint3dArray& newVertexPositions)
{
  assertWriteEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->setVertexSubentityGeometry(newVertexPositions);
}

OdResult OdDbAssocGeomDependency::setEdgeSubentityGeometry(const OdArray<const OdGeCurve3d*>& newEdgeCurves)
{
  assertWriteEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->setEdgeSubentityGeometry(newEdgeCurves);
}

OdResult OdDbAssocGeomDependency::setFaceSubentityGeometry(const OdArray<const OdGeSurface*>& newFaceSurfaces)
{
  assertWriteEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->setFaceSubentityGeometry(newFaceSurfaces);
}

OdResult OdDbAssocGeomDependency::dependentOnObjectMirrored()
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->dependentOnObjectMirrored();
}

OdResult OdDbAssocGeomDependency::redirectToAnotherSubentity(const OdDbObjectId& oldObjectId,
                                                             const OdDbSubentId& oldSubentId,
                                                             const OdDbObjectId& newObjectId,
                                                             const OdDbSubentId& newSubentId)
{
  ODA_FAIL();
  //throw OdError(eNotImplementedYet);
  return eNotImplementedYet;
}

OdSmartPtr<OdDbAssocGeomDependency> OdDbAssocGeomDependency::createObject( OdDbObjectId objId, OdDbSubentId subId )
{
    OdDbAssocGeomDependencyPtr pDep = OdDbAssocGeomDependency::createObject();

    objId.database()->addOdDbObject( pDep );
    pDep->setIsReadDependency( false );
    pDep->setIsWriteDependency( true );
    OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
    pCompoundObjectId->setEmpty();
    pCompoundObjectId->set( objId );
    pDep->attachToObject( pCompoundObjectId );

    static_cast<OdDbImpAssocGeomDependency *>(pDep->m_pImpObj)->setSubentity( subId );

    return pDep;
}

OdResult OdDbAssocGeomDependency::createNewSingleEdgePersSubent(const OdDbObjectId lineID)
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->createNewSingleEdgePersSubent(lineID);
}

OdResult OdDbAssocGeomDependency::createNewAssocSimplePersSubent(OdDbObjectId lineID, OdDb::SubentType type, PointPositionOnBlockLine isStartPoint)
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->createNewAssocSimplePersSubent(lineID, type, isStartPoint);
}

OdResult OdDbAssocGeomDependency::createNewEdgePersSubent(OdDbEntityPtr pEntity, OdDbAssocActionPtr pAction, OdUInt32& subentIndex)//, bool bPolylineGeometry)
{
  assertReadEnabled();
  return ((OdDbImpAssocGeomDependency*)m_pImpObj)->createNewEdgePersSubent(pEntity, pAction, subentIndex);// , bPolylineGeometry);
}

//void OdDbAssocGeomDependency::getViewPortOrBlRefFromFullSubentPath(OdDbViewportPtr& pViewport, OdDbBlockReferencePtr& pBlockRef)
//{
//  OdDbCompoundObjectIdPtr compoundObjectId;
//  OdDbObjectIdArray path;
//  
//  getDependentOnCompoundObject(compoundObjectId);
//  compoundObjectId->getPath(path);
//
//  int len = path.length();
//
//  for (int i = 0; i < len; i++)
//  {
//    OdDbObjectId tempObjId = path.getAt(i);
//
//    pViewport = OdDbViewport::cast(tempObjId.safeOpenObject(OdDb::kForWrite));
//
//    if (!pViewport.isNull())
//    {
//      break;
//    }
//
//    pBlockRef = OdDbBlockReference::cast(tempObjId.safeOpenObject());
//
//    if (!pBlockRef.isNull())
//    {
//      break;
//    }
//  }
//}
//void OdDbAssocGeomDependency::modifiedGraphics(const OdDbObject* pObj)
//{
//  assertNotifyEnabled();
//  OdDbAssocActionPtr pAction = owningAction().openObject(OdDb::kForWrite);
//
//  if (!pAction.isNull())
//  {
//    OdDbAssocActionBodyPtr pActionBody = OdDbAssocActionBody::cast(pAction->actionBody().openObject());
//
//    //if (!pActionBody.isNull())
//    //{
//    //  pActionBody->modified(pObj);
//    //  pAction->setStatus(kChangedDirectlyAssocStatus);
//    //}
//    
//    //OdDbObjectImpl::modified(this, pObj);
//  }
//}

