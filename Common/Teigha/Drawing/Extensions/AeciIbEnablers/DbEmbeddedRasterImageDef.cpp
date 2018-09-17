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
#include "DbObject.h"
#include "DbEmbeddedRasterImageDef.h"
#include "DbEmbeddedImageData.h"
#include "DbFiler.h"
#include "DbDictionary.h"
#include "DbDatabase.h"
#include "RxRasterServices.h"
#include "GeometryFromProxy.h"

ODRX_DEFINE_MEMBERS_EX(AECIDbEmbeddedRasterImageDef,                      // ClassName
                       OdDbRasterImageDef,                                // ParentClass
                       DBOBJECT_CONSTR,                                   // DOCREATE
                       OdDb::vAC21,                                       // DwgVer
                       OdDb::kMRelease1,                                  // MaintVer
                       0x087f,                                            // ProxyFlags
                       L"AeciDbEmbeddedRasterImageDef",                   // DWG class name
                       L"AECIDBEMBEDDEDRASTERIMAGEDEF",                   // DxfName
                       L"AECIIB|Autodesk Raster Design|www.autodesk.com", // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware)            // CustomFlags

const OdInt32 AECIDbEmbeddedRasterImageDef::kCurrentClassVersion = 0;

AECIDbEmbeddedRasterImageDef::AECIDbEmbeddedRasterImageDef()
  : m_isLoaded(true)
{
}

OdResult AECIDbEmbeddedRasterImageDef::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdResult res = OdDbRasterImageDef::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  OdInt32 classVersion = pFiler->rdInt32();
  if (classVersion > kCurrentClassVersion)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }
  m_objectName = pFiler->rdString();
  m_isLoaded = pFiler->rdBool();

  return eOk;
}

void AECIDbEmbeddedRasterImageDef::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbRasterImageDef::dwgOutFields(pFiler);

  pFiler->wrInt32(kCurrentClassVersion);
  pFiler->wrString(m_objectName);
  pFiler->wrBool(m_isLoaded);
}

void AECIDbEmbeddedRasterImageDef::dxfOut(OdDbDxfFiler* pFiler) const
{
  assertReadEnabled();
  if (!isDBRO())
  {
    OdDbRasterImageDef::dxfOut(pFiler);
    return;
  }

  OdDbObjectPtr pThisObj(this);
  OdDbProxyObjectPtr pPrObj = odObjectToProxy(*pThisObj, OdDb::kDHL_CURRENT, OdDb::kMReleaseCurrent);
  database()->disableUndoRecording(true);
  pThisObj->upgradeOpen();
  pThisObj->handOverTo(pPrObj);
  pPrObj->downgradeOpen();
  pPrObj->dxfOut(pFiler);
  pPrObj->upgradeOpen();
  pPrObj->handOverTo(pThisObj);
  pThisObj->downgradeOpen();
  database()->disableUndoRecording(false);
}

static inline
AECIDbEmbeddedImageDataPtr getImageData(OdDbDatabase* pDb, const OdString& objName)
{
  if (!pDb)
    return AECIDbEmbeddedImageDataPtr();

  OdDbDictionaryPtr pDic = pDb->getNamedObjectsDictionaryId().safeOpenObject();
  OdDbDictionaryPtr pRasterDic = pDic->getAt(L"AECI_EMBED_IMAGE_DATA_DICT").openObject();
  if (pRasterDic.isNull())
    return AECIDbEmbeddedImageDataPtr();

  return AECIDbEmbeddedImageData::cast(pRasterDic->getAt(objName).openObject());
}

OdStreamBufPtr AECIDbEmbeddedRasterImageDef::getRasterImage() const
{
  assertReadEnabled();
  AECIDbEmbeddedImageDataPtr pData = getImageData(database(), m_objectName);
  if (pData.get())
  {
    return pData->getRasterImage();
  }
  return OdStreamBufPtr();
}

OdResult AECIDbEmbeddedRasterImageDef::load(bool modifyDatabase/* = true*/)
{
  if (!image(false).isNull()) // assertReadEnabled()
    return eOk; // lazy loading as declared in the interface

  if (modifyDatabase)
    assertWriteEnabled();

  OdGiRasterImagePtr pImage;
  OdStreamBufPtr pImageBuf = getRasterImage();
  if (pImageBuf.get())
  {
    try
    {
      OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
      if (!pRasSvcs.isNull())  // catch below is not enough to prevent an access violation on Solaris
      {
        OdGiRasterImagePtr pImage = pRasSvcs->loadRasterImage(pImageBuf);
        if (!pImage.isNull())
          pImage = pImage->changeImageSource(OdGiRasterImage::kFromDwg);
        OdDbRasterImageDef::setImage(pImage, modifyDatabase);
      }
    }
    catch(const OdError& e)
    {
      return e.code();
    }
  }

  return pImage.isNull() ? eInvalidInput : eOk;
}

bool AECIDbEmbeddedRasterImageDef::isLoaded() const
{
  assertReadEnabled();
  return m_isLoaded;
}

OdString AECIDbEmbeddedRasterImageDef::getEmbeddedObjectName() const
{
  assertReadEnabled();
  return m_objectName;
}

void AECIDbEmbeddedRasterImageDef::setEmbeddedObjectName(const OdString& objName)
{
  assertWriteEnabled();
  m_objectName = objName;
  //AECIDbEmbeddedImageDataPtr pData = getImageData(database(), m_objectName);
  //if (pData.get())
  //{
  //  m_isLoaded = pData->imageRawData().size() != 0;
  //}
}
