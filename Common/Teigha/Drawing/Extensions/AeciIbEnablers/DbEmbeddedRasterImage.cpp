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
#include "DbEmbeddedRasterImage.h"
#include "DbFiler.h"
#include "GeometryFromProxy.h"


#include "DbProxyEntity.h"

ODRX_DEFINE_MEMBERS_EX(AECIDbEmbeddedRasterImage,                               // ClassName
                       OdDbRasterImage,                                         // ParentClass
                       DBOBJECT_CONSTR,                                         // DOCREATE
                       OdDb::vAC21,                                             // DwgVer
                       OdDb::kMRelease1,                                        // MaintVer
                       0x087f,                                                  // ProxyFlags
                       L"AeciDbEmbeddedRasterImage",                            // DWG class name
                       L"AECIDBEMBEDDEDRASTERIMAGE",                            // DxfName
                       L"AECIIB|Autodesk Raster Design|www.autodesk.com",       // AppName
                       OdRx::kMTLoading | OdRx::kMTRender | OdRx::kMTRenderInBlock | OdRx::kHistoryAware) // CustomFlags

const OdInt32 AECIDbEmbeddedRasterImage::kCurrentClassVersion = 0;

AECIDbEmbeddedRasterImage::AECIDbEmbeddedRasterImage()
{
}

OdResult AECIDbEmbeddedRasterImage::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdResult res = OdDbRasterImage::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  OdInt32 classVersion = pFiler->rdInt32();
  if (classVersion > kCurrentClassVersion)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  return eOk;
}

void AECIDbEmbeddedRasterImage::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbRasterImage::dwgOutFields(pFiler);

  pFiler->wrInt32(kCurrentClassVersion);
}

void AECIDbEmbeddedRasterImage::dxfOut(OdDbDxfFiler* pFiler) const
{
  assertReadEnabled();
  if (!isDBRO())
  {
    OdDbRasterImage::dxfOut(pFiler);
    return;
  }

  OdDbEntityPtr pThisEnt(this);
  OdDbProxyEntityPtr pPrEnt = odEntityToProxy(*pThisEnt, OdDb::kDHL_CURRENT, OdDb::kMReleaseCurrent);
  database()->disableUndoRecording(true);
  pThisEnt->upgradeOpen();
  pThisEnt->handOverTo(pPrEnt);
  pPrEnt->downgradeOpen();
  pPrEnt->dxfOut(pFiler);
  pPrEnt->upgradeOpen();
  pPrEnt->handOverTo(pThisEnt);
  pThisEnt->downgradeOpen();
  database()->disableUndoRecording(false);
}
