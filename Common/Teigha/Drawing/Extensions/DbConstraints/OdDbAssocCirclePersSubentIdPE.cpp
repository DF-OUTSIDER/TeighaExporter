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
#include "DbCircle.h"
#include "OdDbAssocCirclePersSubentIdPE.h"
#include "Ge/GeCircArc3d.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocCirclePersSubentIdPE, OdDbAssocCurvePersSubentIdPE, L"AcDbAssocCirclePersSubentIdPE");

OdDbAssocPersSubentIdPtr OdDbAssocCirclePersSubentIdPE::createNewPersSubent(OdDbEntity* pEntity,
                                                                           const class OdDbCompoundObjectId& compId,
                                                                           const OdDbSubentId& subentId)
{
  OdDbAssocPersSubentIdPtr res;
  OdDbAssocPersSubentId* ptr = NULL;
  if ( !isTypeEqual( pEntity, OdDbCircle::desc() ) || !checkSubentId( subentId, subentId.type() ) )
    res.attach( const_cast<const OdDbAssocPersSubentId*>(ptr) );
  else
  {
    switch ( subentId.type() )
    {
      case OdDb::kVertexSubentType:
        res = OdDbAssocSimplePersSubentId::createObject();
        OdDbAssocSimplePersSubentId::cast( res )->init( subentId );
        break;

      case OdDb::kEdgeSubentType:
        res = OdDbAssocSingleEdgePersSubentId::createObject();
        break;
    }
  }
  return res;
}

OdResult OdDbAssocCirclePersSubentIdPE::getAllSubentities(const OdDbEntity* pEntity,
                                                         OdDb::SubentType subentType,
                                                         OdArray<OdDbSubentId>& allSubentIds)
{
  if ( !isTypeEqual(pEntity, OdDbCircle::desc()) )
    return eBadObjType;

  if ( subentType == OdDb::kEdgeSubentType )
  {
    
    allSubentIds.append(OdDbSubentId(OdDb::kEdgeSubentType, (OdGsMarker)1));
    return eOk;
  }
  else if ( subentType == OdDb::kVertexSubentType )
  {
    allSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, (OdGsMarker)kCenter));
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocCirclePersSubentIdPE::getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                                 const OdDbSubentId&    edgeSubentId,
                                                                 OdDbSubentId&          startVertexSubentId,
                                                                 OdDbSubentId&          endVertexSubentId,
                                                                 OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getEdgeVertexSubentities(pEntity, edgeSubentId, startVertexSubentId, endVertexSubentId, otherVertexSubentIds);
  return otherVertexSubentIds.length() == 0 ? eBadObjType : eOk;
}

OdResult OdDbAssocCirclePersSubentIdPE::getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                OdGeCurve3d*&       pEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbCircle::desc()) )
  {
    OdDbCirclePtr pCircle = OdDbCircle::cast(pEntity);
    pEdgeCurve = new OdGeCircArc3d(pCircle->center(), pCircle->normal(), pCircle->radius());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocCirclePersSubentIdPE::setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                                                const OdDbSubentId& edgeSubentId,
                                                                const OdGeCurve3d*  pNewEdgeCurve)
{
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType) && isTypeEqual(pEntity, OdDbCircle::desc())
        && pNewEdgeCurve->isKindOf(OdGe::kCircArc3d) )
  {
    OdGeCircArc3d* pGeCircle = (OdGeCircArc3d*)pNewEdgeCurve;
    OdDbCirclePtr pCircle = OdDbCircle::cast(pEntity);

    pCircle->setNormal(pGeCircle->normal());
    pCircle->setCenter(pGeCircle->center());
    pCircle->setRadius(pGeCircle->radius());
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocCirclePersSubentIdPE::getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  OdGePoint3d&        vertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbCircle::desc()) )
  {
    OdDbCirclePtr pCircle = OdDbCircle::cast(pEntity);
    vertexPosition = pCircle->center();
    return eOk;
  }
  else
    return eBadObjType;
}

OdResult OdDbAssocCirclePersSubentIdPE::setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                                                  const OdDbSubentId& vertexSubentId,
                                                                  const OdGePoint3d&  newVertexPosition)
{
  if ( checkSubentId(vertexSubentId, OdDb::kVertexSubentType) && isTypeEqual(pEntity, OdDbCircle::desc()) )
  {
    OdDbCirclePtr pCircle = OdDbCircle::cast(pEntity);
    pCircle->setCenter(newVertexPosition);
    return eOk;
  }
  else
    return eBadObjType;
}


OdResult OdDbAssocCirclePersSubentIdPE::setSubentityGeometry(OdDbEntity*         pEntity,
                                                            const OdDbSubentId& subentId,
                                                            const OdDbEntity*   pNewSubentityGeometry)
{
  if ( !isTypeEqual(pEntity, OdDbCircle::desc()) || !isTypeEqual(pNewSubentityGeometry, OdDbCircle::desc()) )
    return eBadObjType;

  OdDbCirclePtr pLineDst = OdDbCircle::cast(pEntity);
  OdDbCirclePtr pLineSrc = OdDbCircle::cast(pNewSubentityGeometry);

  if ( checkSubentId(subentId, OdDb::kEdgeSubentType) )
  {
    OdGeCurve3d* pCurve = NULL;
    if ( getEdgeSubentityGeometry(pNewSubentityGeometry, subentId, pCurve) != eOk )
    {
      delete pCurve;
      return eBadObjType;
    }
    else
    {
      OdResult res = setEdgeSubentityGeometry(pEntity, subentId, pCurve);
      delete pCurve;
      return res;
    }
  }
  else if ( checkSubentId(subentId, OdDb::kVertexSubentType) )
  {
    OdGePoint3d newPt;
    if ( getVertexSubentityGeometry(pNewSubentityGeometry, subentId, newPt) != eOk )
      return eBadObjType;

    setVertexSubentityGeometry(pEntity, subentId, newPt);
    return eOk;
  }
  else
    return eBadObjType;
}

void OdDbAssocCirclePersSubentIdPE::fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                                         const OdDbSubentId&    edgeSubentId,
                                                         OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillStartVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
}

void OdDbAssocCirclePersSubentIdPE::fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                                        const OdDbSubentId&    edgeSubentId,
                                                        OdDbSubentId& startVertexSubentId)
{
  OdDbAssocCurvePersSubentIdPE::fillEndVertexSubentId(pEntity, edgeSubentId, startVertexSubentId);
}

void OdDbAssocCirclePersSubentIdPE::getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                                          const OdDbSubentId&    edgeSubentId,
                                                          OdArray<OdDbSubentId>& otherVertexSubentIds)
{
  OdDbAssocCurvePersSubentIdPE::getOtherVertexSubentIds(pEntity, edgeSubentId, otherVertexSubentIds);
  if ( checkSubentId(edgeSubentId, OdDb::kEdgeSubentType ) )
  {
    otherVertexSubentIds.append(OdDbSubentId(OdDb::kVertexSubentType, kCenter));
  }
}

bool OdDbAssocCirclePersSubentIdPE::checkSubentId(const OdDbSubentId& subentId, const OdDb::SubentType type) const
{
  if ( type == OdDb::kEdgeSubentType )
  {
    return subentId.index() == 1;
  }
  else if ( type == OdDb::kVertexSubentType )
  {
    return subentId.index() == kCenter;
  }
  else
    return false;
}
