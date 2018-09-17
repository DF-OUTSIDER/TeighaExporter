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
//
// ExEmbedRaster.cpp: implementation to embed raster as OLE item for Windows
//

#include "OdaCommon.h"
#include "OleItemHandler.h"
#include "DynamicLinker.h"
#include "RxRasterServices.h"
#include "MemoryStream.h"
#include "OdBinaryData.h"
#include "ExEmbedRaster.h"
#include "CompDocData.h"

#include "comdef.h"

//#if defined(_DEBUG) && defined(ODA_WINDOWS) && !defined(_WINRT)
//#define PATH_TO_SAVE_INTERNAL_DATA L"e:/_Oda/data/_ole"
//#include "../Kernel/Extensions/ExRender/GLES2/xml/IO/CloudTools.h"
//#endif

bool exEmbedRaster(OdOleItemHandler* pHandler, OdGiRasterImagePtr pImage)
{
  OdRxRasterServicesPtr pRasSvcs = ::odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  ODA_ASSERT_ONCE(pRasSvcs.get() && pImage.get());
  if (pRasSvcs.isNull() || pImage.isNull())
    return false;

  OdString sStorageDir, sStoragePathName;
#ifdef PATH_TO_SAVE_INTERNAL_DATA
  if (OdFilePathName(PATH_TO_SAVE_INTERNAL_DATA).existsDir())
  {
    sStorageDir = PATH_TO_SAVE_INTERNAL_DATA L"/";
    for (int idx = 0; OdFilePathName(sStoragePathName.format(L"%lsinsertraster_%04d.storage", 
                                                             sStorageDir.c_str(), idx)).exists();)
      idx++;
  }
  else
#endif
  {
    ODA_ASSERT_VAR(DWORD ln = )
      ::GetTempPath(MAX_PATH, sStorageDir.getBuffer(MAX_PATH + 1));
    ODA_ASSERT_ONCE(ln && ln < MAX_PATH);
    sStorageDir.releaseBuffer();
    ODA_ASSERT_VAR(ln = )
      ::GetTempFileName(sStorageDir.c_str(), L"ir_", 0, sStoragePathName.getBuffer(MAX_PATH + 1));
    sStoragePathName.releaseBuffer();
    sStoragePathName.makeLower();
    ::DeleteFile(sStoragePathName);
    sStoragePathName.replace(L".tmp",L".storage");
  }

  IStoragePtr pStorage;
  if (StgCreateDocfile(sStoragePathName.c_str(), STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE, 0, &pStorage) != S_OK)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  //HKEY_CLASSES_ROOT\PBrush\CLSID = {0003000A-0000-0000-C000-000000000046} 
  const CLSID clsid = OD_PBRUSH_CLSID;
  if (pStorage->SetClass(clsid) != S_OK)
  {
    ODA_FAIL_ONCE();
    return false;
  }

  IStreamPtr pStream;
  ULONG len;

  if (pStorage->CreateStream(L"\01Ole10Native", STGM_WRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream) != S_OK)
  {
    ODA_FAIL_ONCE();
    return false;
  }
  OdMemoryStreamPtr pMemStreamBuf = OdMemoryStream::createNew();
  if (!pRasSvcs->convertRasterImage(pImage,  OdRxRasterServices::kBMP, pMemStreamBuf))
  {
    ODA_FAIL_ONCE();
    return false;
  }
  pMemStreamBuf->rewind();
  OdUInt32 nSize = (OdUInt32) pMemStreamBuf->length();
  ODA_ASSERT_ONCE(nSize);
  if (!nSize)
    return false;
  OdBinaryData data;
  data.resize(sizeof(nSize) + nSize + 10);
  OdUInt8* pBuf = data.asArrayPtr();
  OdUInt32 nSizeEx = nSize + 10;
  ::memcpy(pBuf, &nSizeEx, sizeof(nSizeEx));
  pMemStreamBuf->getBytes(pBuf + sizeof(nSize), nSize);
  ::memset(pBuf + sizeof(nSize) + nSize, 0, 10);
  pMemStreamBuf = NULL;
  len = 0;
  if (pStream->Write(pBuf, data.size(), &len) != S_OK || len != data.size())
  {
    ODA_FAIL_ONCE();
    return false;
  }
  pStream = NULL;

  if (pStorage->CreateStream(L"\01CompObj", STGM_WRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream) != S_OK)
  {
    ODA_FAIL_ONCE();
    return false;
  }
  static const OdUInt8 dataCompObj[] = OD_COMP_DOC_COMP_OBJ_DATA;
  len = 0;
  if (pStream->Write(dataCompObj, sizeof(dataCompObj), &len) != S_OK || len != sizeof(dataCompObj))
  {
    ODA_FAIL_ONCE();
    return false;
  }
  pStream = NULL;

  if (pStorage->CreateStream(L"\01Ole", STGM_WRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &pStream) != S_OK)
  {
    ODA_FAIL_ONCE();
    return false;
  }
  static const OdUInt8 dataOle[] = OD_COMP_DOC_OLE_DATA;
  len = 0;
  if (pStream->Write(dataOle, sizeof(dataOle), &len) != S_OK || len != sizeof(dataOle))
  {
    ODA_FAIL_ONCE();
    return false;
  }
  pStream = NULL;
  pStorage = NULL;

  OdStreamBufPtr pStreamBuf = ::odrxSystemServices()->createFile(sStoragePathName);
  ODA_ASSERT_ONCE(pStreamBuf.get());
  nSize = (OdUInt32) pStreamBuf->length();
  pHandler->setCompoundDocument(nSize, *pStreamBuf.get());
//#if _DEBUG // test for redesigned getCompoundDocument & getCompoundDocumentDataSize of OleDwgItem of OdaMfcApp
//  ODA_ASSERT_ONCE(nSize == pHandler->getCompoundDocumentDataSize());
//  OdMemoryStreamPtr pTestBuf = OdMemoryStream::createNew();
//  pHandler->getCompoundDocument(*pTestBuf.get());
//  ODA_ASSERT_ONCE(nSize == (OdUInt32) pTestBuf->length());
//#endif
  pHandler->setDrawAspect(OdOleItemHandler::kContent);
  pStreamBuf = NULL;

#ifndef PATH_TO_SAVE_INTERNAL_DATA
  ::DeleteFile(sStoragePathName);
#endif
  return true;
}
