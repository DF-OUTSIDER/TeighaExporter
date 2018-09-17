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
#include "IdBlockReference.h"
#include "DbHostAppServices.h"

ODRX_DEFINE_MEMBERS_EX(OdIdBlockReference,                                                                           // ClassName
                       OdDbBlockReference,                                                                           // ParentClass
                       DBOBJECT_CONSTR,                                                                              // DOCREATE
                       OdDb::vAC21,                                                                                  // DwgVer
                       OdDb::kMRelease1,                                                                             // MaintVer
                       1,                                                                                            // NProxyFlags
                       L"AcIdBlockReference",                                                                        // DWG class name
                       L"ACIDBLOCKREFERENCE",                                                                        // DxfName
                       L"AcIdViewObj|Product: Inventor Drawing Enabler|Company: Autodesk|Website: www.autodesk.com", // AppName
                       OdRx::kMTLoading|OdRx::kMTRender|OdRx::kMTRenderInBlock | OdRx::kHistoryAware)                                      // CustomFlags

OdResult OdIdBlockReference::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdDbBlockReference::dwgInFields(pFiler);
  m_pVportId = pFiler->rdSoftPointerId();
  return eOk;
}

void OdIdBlockReference::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbBlockReference::dwgOutFields(pFiler);
  pFiler->wrSoftPointerId(m_pVportId);
}

bool OdIdBlockReference::subWorldDraw(OdGiWorldDraw* pWd) const
{
  assertReadEnabled();
  if (ownerId() == database()->getPaperSpaceId())
    return OdDbBlockReference::subWorldDraw(pWd);
  return false;
}

void OdIdBlockReference::subViewportDraw(OdGiViewportDraw* pViewportDraw) const
{
  assertReadEnabled();
  if (OdDbObjectId(pViewportDraw->viewportObjectId()) != m_pVportId)
    return;
  OdDbBlockTableRecordPtr pBR = blockTableRecord().openObject();
  if (!pBR.isNull())
  {
    OdGiGeometry& geom = pViewportDraw->geometry();
    OdGeMatrix3d matrix = blockTransform();
    matrix.validateZero(OdGeTol(1e-20));
    OdGiModelTransformSaver mt(geom, matrix);
    geom.draw(pBR);
  }
}

OdResult OdIdBlockReference::subExplode(OdRxObjectPtrArray& /*entitySet*/) const
{
  assertReadEnabled();
  return eOk;
}

#include "DbDimAssoc.h"
#include "DbViewport.h"

OdResult OdIdBlockReference::explodeGeometryToBlock(OdDbBlockTableRecord *pBlockRecord, OdDbObjectIdArray *ids)
{
  // Create "Normal" Block Reference
  OdDbBlockReferencePtr pRef = OdDbBlockReference::createObject();
  pRef->setPropertiesFrom(this);
  pRef->setBlockTableRecord(blockTableRecord());
  OdGeMatrix3d mTransform;
  OdDbObjectId idOwner = ownerId();
  // Smart pointer may be required for openObject()
  OdDbBlockTableRecordPtr pPS;

  OdDbViewportPtr pViewport = vportId().openObject();
  if (!pViewport.isNull())
  { // Add Viewport transform to own one
    mTransform = OdDbPointRef::mswcsToPswcs(pViewport) * blockTransform();
    if (!pBlockRecord)
    { // Default is Paper Space of Viewport
      pPS = pViewport->ownerId().openObject(OdDb::kForWrite);
      pBlockRecord = pPS;
    }
  }
  else
  { // Viewport Null - owner is PaperSpace
    mTransform = blockTransform();
    if (!pBlockRecord)
    { // Default is owner
      pPS = ownerId().openObject(OdDb::kForWrite);
      pBlockRecord = pPS;
    }
  }
  pRef->setBlockTransform(mTransform);
  OdDbObjectId idNew = pBlockRecord->appendOdDbEntity(pRef);
  if (ids)
  {
    ids->push_back(idNew);
  }
  return eOk;
}

OdDbObjectPtr OdIdBlockReference::decomposeForSave(
    OdDb::SaveType format, 
    OdDb::DwgVersion ver, 
    OdDbObjectId& replaceId, 
    bool& exchangeXData)
{
  if (format != OdDb::kDwg)
  {
    OdDbHostAppServices* pAppSvs = database()->appServices();
    pAppSvs->warning(pAppSvs->formatMessage(sidIdViewObjsDxf));
  }
  return OdDbBlockReference::decomposeForSave(format, ver, replaceId, exchangeXData);
}
