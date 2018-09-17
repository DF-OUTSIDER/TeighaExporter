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

// OdOleSsItemHandlerImpl.cpp: implementation of the OdOleSsItemHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "Gi/GiCommonDraw.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiViewportDraw.h"
#include "Gs/Gs.h"
#include "OleSsItemHandlerModule.h"
#include "OdPlatformStreamer.h"
#include "FlatMemStream.h"
#include "MemoryStream.h"
#include "DynamicLinker.h"
#include "StaticRxObject.h"
#include "RxRasterServices.h" 
#include "OdSSRWIS.h"
#include "Gi/GiMetafile.h"
#include "Gs/GsBaseVectorizer.h"
#include "OleSsItemHandlerImpl.h"
#include "CompDocData.h"

//#if defined(_DEBUG) && defined(ODA_WINDOWS) && !defined(_WINRT)
//#define PATH_TO_SAVE_INTERNAL_DATA L"e:/_Oda/data/_ole"
//#include "../Kernel/Extensions/ExRender/GLES2/xml/IO/CloudTools.h"
//#endif
//
//#define DROP_PRES_STREAM

ODRX_NO_CONS_DEFINE_MEMBERS(OdOleSsItemHandlerImpl, OdOleItemHandlerBase);
ODRX_NO_CONS_DEFINE_MEMBERS(OdGiSelfGiDrawablePEImpl, OdGiSelfGiDrawablePE);

class OdOleSsCompObj
{
public:
  // It stores information on Application type including 3 strings and some unknown data probably also some flags - settings.
  //16 bytes – unknown. First 12 are constant.
  OdUInt8 m_unknown12[12];
  CLSID m_clsId;
  //Offset 16 – 4 bytes – first string length including Null terminator
  //First string (ASCIIZ)
  OdAnsiString m_asFirst; 
  //4 bytes – second string length
  //Second string (ASCIIZ)
  OdUInt32 m_lenSecond;
  OdUInt8 m_unknown4[4]; // is used if m_lenSecond == 0xffffffff
  OdAnsiString m_asSecond;
  //4 bytes – third string length
  OdAnsiString m_asThird;
  //Third string (ASCIIZ)
  //16 bytes unknown (some flags/settings) – constant
  OdUInt8 m_unknown16[16];

  OdOleSsCompObj()
  {
    static const OdUInt8 content[] = OD_COMP_DOC_COMP_OBJ_DATA;
    const OdUInt8* pCtx = content;
    ::memcpy(m_unknown12, pCtx, sizeof(m_unknown12));
    pCtx += sizeof(m_unknown12);
    ::memcpy(&m_clsId, pCtx, sizeof(m_clsId));
    pCtx += sizeof(m_clsId);
    m_asFirst = m_asSecond = m_asThird = "PBrush";
    m_lenSecond = m_asSecond.getLength() + 1;
    pCtx += 3 * (sizeof(OdUInt32) + m_lenSecond);
    ::memcpy(m_unknown16, pCtx, sizeof(m_unknown16));
    //pCtx += sizeof(m_unknown16);
  }

  virtual ~OdOleSsCompObj()
  {
  }

  void inFileds(OdStreamBufPtr pStream)
  {
    pStream->getBytes(m_unknown12, sizeof(m_unknown12));
    pStream->getBytes(&m_clsId, sizeof(m_clsId));
    OdUInt32 lenFirst = OdPlatformStreamer::rdInt32(*pStream);
    pStream->getBytes(m_asFirst.getBuffer(lenFirst), lenFirst); m_asFirst.releaseBuffer();
    m_lenSecond = OdPlatformStreamer::rdInt32(*pStream);
    if (m_lenSecond != 0xffffffff)
      pStream->getBytes(m_asSecond.getBuffer(m_lenSecond), m_lenSecond), m_asSecond.releaseBuffer();
    else
    {
      pStream->getBytes(m_unknown4, sizeof(m_unknown4));
      ODA_ASSERT_ONCE(m_unknown4[0] != 0 && !m_unknown4[1] && !m_unknown4[2] && !m_unknown4[3]);
    }
    OdUInt32 lenThird = OdPlatformStreamer::rdInt32(*pStream);
    pStream->getBytes(m_asThird.getBuffer(lenThird), lenThird); m_asThird.releaseBuffer();
    pStream->getBytes(m_unknown16, sizeof(m_unknown16));
  }

  void outFileds(OdStreamBufPtr pStream)
  {
    pStream->putBytes(m_unknown12, sizeof(m_unknown12));
    pStream->putBytes(&m_clsId, sizeof(m_clsId));
    OdUInt32 lenFirst = m_asFirst.getLength() + 1;
    OdPlatformStreamer::wrInt32(*pStream, lenFirst);
    pStream->putBytes(m_asFirst.c_str(), lenFirst);
    OdPlatformStreamer::wrInt32(*pStream, m_lenSecond);
    if (m_lenSecond != 0xffffffff)
    {
      ODA_ASSERT_ONCE(m_lenSecond == (m_asSecond.getLength() + 1));
      pStream->putBytes(m_asSecond.c_str(), m_lenSecond);
    }
    else
    {
      ODA_ASSERT_ONCE(m_unknown4[0] != 0 && !m_unknown4[1] && !m_unknown4[2] && !m_unknown4[3]);
      pStream->putBytes(m_unknown4, sizeof(m_unknown4));
    }
    OdUInt32 lenThird = m_asThird.getLength() + 1;
    OdPlatformStreamer::wrInt32(*pStream, lenThird);
    pStream->putBytes(m_asThird.c_str(), lenThird);
    pStream->putBytes(m_unknown16, sizeof(m_unknown16));
  }
};

class OdOleSsNative
{
public:
  //First 4 bytes – stream data length
  //In case of PBrush application bitmap starts here.
  //In other cases probably format may be different but for Package (JPG) it is:
  //Int32 bytes – stream length
  //Int16 – unknown – equal to “2”
  //Null terminated label – “WriteEx.jpg”
  OdAnsiString m_asLabel0; 
  //Some string  null terminated - “C:\WINDOWS\system32\SHELL32.dll”
  OdAnsiString m_asLabel1;
  //Int32 - unknown 
  OdUInt32 m_unknown32;
  //Int32 – length of following string including Null terminator
  OdUInt32 m_lenSource;
  OdAnsiString m_asSource; 
  //Source file name string null terminated – “D:\ODA\#CURTR~1\exe\VC8DLL~1/ODWRIT~1.JPG”
  //Int32 – Length of data
  //Data follows - .JPG file contents.
  bool m_isNativePBrush;
  OdBinaryData m_data;
  OdUInt8 m_unknown10[10]; // at end

  OdOleSsNative(bool isNativePBrush = true)
    : m_isNativePBrush(isNativePBrush)
  {
    ::memset(m_unknown10, 0, sizeof(m_unknown10));
  }

  OdOleSsNative(OdGiRasterImagePtr pImage)
    : m_isNativePBrush(true)
  {
    ODA_ASSERT_ONCE(pImage.get());

    OdRxRasterServicesPtr pRasSvcs = ::odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    OdMemoryStreamPtr pStreamBuf = OdMemoryStream::createNew();
    if (pRasSvcs.isNull() || !pRasSvcs->convertRasterImage(pImage,  OdRxRasterServices::kBMP, pStreamBuf))
    {
      ODA_FAIL_ONCE();
      return;
    }
    pStreamBuf->rewind();
    OdUInt32 nSize = (OdUInt32) pStreamBuf->length();
    ODA_ASSERT_ONCE(nSize);
    m_data.resize(nSize);
    if (nSize)
      pStreamBuf->getBytes(m_data.asArrayPtr(), m_data.size());
    ::memset(m_unknown10, 0, sizeof(m_unknown10));
  }

  virtual ~OdOleSsNative()
  {
  }

  void inFileds(OdStreamBufPtr pStream)
  {
    OdUInt64 lenStr = pStream->length();
    OdInt32 len = OdPlatformStreamer::rdInt32(*pStream);
    if (!m_isNativePBrush)
    {
      OdInt16 inknown0 = OdPlatformStreamer::rdInt16(*pStream);
      ODA_ASSERT_ONCE(inknown0 == 2);
      char* pBuf = m_asLabel0.getBuffer(1024);
      while ((*pBuf = (char) pStream->getByte()) != 0)
        pBuf++;
      m_asLabel0.releaseBuffer();
      ODA_ASSERT_ONCE(m_asLabel0.getLength() < 1024);
      pBuf = m_asLabel1.getBuffer(1024);
      while ((*pBuf = (char) pStream->getByte()) != 0)
        pBuf++;
      m_asLabel1.releaseBuffer();
      ODA_ASSERT_ONCE(m_asLabel1.getLength() < 1024);
      m_unknown32 = OdPlatformStreamer::rdInt32(*pStream);
      ODA_ASSERT_ONCE(m_unknown32 == 0x30000);
      m_lenSource = OdPlatformStreamer::rdInt32(*pStream);
      pStream->getBytes(m_asSource.getBuffer(m_lenSource), m_lenSource); m_asSource.releaseBuffer();
      //ODA_ASSERT_ONCE(pStream->tell() >= 0x66); // ole.dwg 
      len = OdPlatformStreamer::rdInt32(*pStream);
    }
    if (m_isNativePBrush)
      m_data.resize(len - sizeof(m_unknown10));
    else
      m_data.resize(len);
    pStream->getBytes(m_data.asArrayPtr(), m_data.size());
    if (m_isNativePBrush)
      pStream->getBytes(m_unknown10, sizeof(m_unknown10));
  }

  void outFileds(OdStreamBufPtr pStream)
  {
    ODA_ASSERT_ONCE(m_isNativePBrush && m_data.size());
    OdInt32 len = m_data.size();
    if (m_isNativePBrush)
      len += sizeof(m_unknown10);
    OdPlatformStreamer::wrInt32(*pStream, len);
    if (len)
      pStream->putBytes(m_data.asArrayPtr(), m_data.size());
    if (m_isNativePBrush)
      pStream->putBytes(m_unknown10, sizeof(m_unknown10));
  }
};

class OdOleSsOle
{
public:
  OdUInt8 m_unknown20[20];

  OdOleSsOle()
  {
    static const OdUInt8 content[] = OD_COMP_DOC_OLE_DATA; 
    ::memcpy(m_unknown20, content, sizeof(content));
  }

  virtual ~OdOleSsOle()
  {
  }

  void inFileds(OdStreamBufPtr pStream)
  {
    pStream->getBytes(m_unknown20, 20);
  }

  void outFileds(OdStreamBufPtr pStream)
  {
    pStream->putBytes(m_unknown20, 20);
  }
};

OdOleSsItemHandlerImpl::OdOleSsItemHandlerImpl()
  : m_type(kUnknown) // to take via ole2frame
{
  // since this object is not DB object and can't be turned to proxy,
  // we should lock its module to prevent unloading.
  OdOleSsItemHandlerModuleImpl::internalLockModule();
}

OdOleSsItemHandlerImpl::~OdOleSsItemHandlerImpl()
{
  OdOleSsItemHandlerModuleImpl::internalUnlockModule(); // unlock module
}

static OdString getExtByContent(OdBinaryData dataImage)
{
  OdString sExt;
  if (!dataImage.size())
    return sExt;
  OdUInt8 frst = dataImage.getAt(0);
  switch (frst)
  {
  case 1: sExt = L".wmf"; break;
  case 0x42: sExt = L".bmp"; break;
  case 0x47: sExt = L".gif"; break;
  case 0x49: sExt = L".tif"; break;
  case 0x89: sExt = L".png"; break;
  case 0xFF: sExt = L".jpg"; break;
  default: sExt = L".unknown"; break;
  }

#ifdef PATH_TO_SAVE_INTERNAL_DATA
  if (OdFilePathName(PATH_TO_SAVE_INTERNAL_DATA).existsDir())
  {
    OdString path;
    for (int idx = 0; OdFilePathName(path.format(L"%ls/preview_%04d%ls", PATH_TO_SAVE_INTERNAL_DATA, idx, sExt.c_str())).exists();)
      idx++;
    OdStreamBufPtr io = ::odrxSystemServices()->createFile(path, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
    if (io.get())
      io->putBytes(dataImage.asArrayPtr(), dataImage.size());
  }
#endif
  return sExt;
}

void OdOleSsItemHandlerImpl::setCompoundDocument(OdUInt32 nSize, OdStreamBuf& stream)
{
  if (nSize == 0)
  {
    m_pImage = NULL;
    m_wmfData.clear();
  }
  else
  {
    m_buff.resize(nSize);
    stream.getBytes(m_buff.asArrayPtr(), nSize);
#ifdef PATH_TO_SAVE_INTERNAL_DATA
    if (OdFilePathName(PATH_TO_SAVE_INTERNAL_DATA).existsDir())
    {
      OdString path;
      for (int idx = 0; OdFilePathName(path.format(L"%ls/storage.%04d", PATH_TO_SAVE_INTERNAL_DATA, idx)).exists();)
        idx++;
      OdStreamBufPtr io = ::odrxSystemServices()->createFile(path, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
      if (io.get())
        io->putBytes(m_buff.asArrayPtr(), nSize);
    }
#endif
#ifndef DROP_PRES_STREAM
    OdStreamBufPtr pMemStream = OdFlatMemStream::createNew(m_buff.asArrayPtr(), m_buff.size());
    struct _RootStorage* pRootStorage = openFileStorage(pMemStream.get());
#else
    OdStreamBufPtr pMemStream = OdMemoryStream::createNew();
    pMemStream->putBytes(m_buff.asArrayPtr(), m_buff.size()); pMemStream->rewind();
    struct _RootStorage* pRootStorage = openFileStorage(pMemStream.get(), STG_RW);
#endif

    OdBinaryData dataImageNative, dataImagePress, dataImageContent;
    Storage* pStorage = NULL;
    if (pRootStorage && (pStorage = getStorage(pRootStorage)) != NULL)
    {
      Stream* pStrm = NULL;
      unsigned long nSize = 0,
                    nOffsetInContents = 22; // for wmf
      bool isNativePBrush = true;
      unsigned int cnt = 0;
      OdOleSsCompObj compObj;
      OdOleSsNative native;

      if ((pStrm = openStorageStream(pStorage, L"\01CompObj")) != NULL)
      {
        OdBinaryData data = readStorageStream(&pStrm);
        OdStreamBufPtr pStream = OdFlatMemStream::createNew(data.asArrayPtr(), data.size());
        compObj.inFileds(pStream);
        if (compObj.m_asThird == "OLE Package" || compObj.m_asThird	== "Package")
        {
          ODA_ASSERT_ONCE(compObj.m_asFirst == "OLE Package" || compObj.m_asFirst	== "Package"); // &&  compObj.m_asSecond.isEmpty());
          isNativePBrush = false;
          m_type = kEmbedded;
        }
        if (compObj.m_asThird == "PBrush")
          m_sUserType = L"Paintbrush Picture";
        else if (compObj.m_asThird.find("Static") >= 0)
          m_type = kStatic;
        if (compObj.m_asThird == "StaticDib") // compObj.m_asFirst == "Picture (Device Independent Bitmap)"
          nOffsetInContents = 0;
      }

      if ((pStrm = openStorageStream(pStorage, L"\01Ole10Native")) != NULL) // ole_past_bmp.dwg 
      {
        native.m_isNativePBrush = isNativePBrush;
        m_type = isNativePBrush ? kEmbedded : kLink;
        OdBinaryData data = readStorageStream(&pStrm);
        OdStreamBufPtr pStream = OdFlatMemStream::createNew(data.asArrayPtr(), data.size());
        native.inFileds(pStream);
        dataImageNative = native.m_data;
      }
      if ((pStrm = openStorageStream(pStorage, L"\02OlePres000")) != NULL)
        dataImagePress = readStorageStream(&pStrm, 0x28);
      if ((pStrm = openStorageStream(pStorage, "CONTENTS")) != NULL) // ole_past_wmf.dwg 
        dataImageContent = readStorageStream(&pStrm, nOffsetInContents);

#ifdef DROP_PRES_STREAM
      if (dataImagePress.size())
      {
        if (destroy(pStorage, L"\02OlePres000") != SSTG_OK)
        {
          ODA_FAIL_ONCE();
        }
      }
#endif

      closeStorage(&pStorage);
    }
    if (pRootStorage)
      closeStructuredStorage(&pRootStorage);
#ifdef DROP_PRES_STREAM
    pMemStream->rewind();
    OdUInt32 nSizeOut = (OdUInt32) pMemStream->length();
    m_buff.resize(nSizeOut);
    pMemStream->getBytes(m_buff.asArrayPtr(), m_buff.size());
#endif

    if (dataImageNative.size() || dataImagePress.size() || dataImageContent.size())
    {
      OdRxRasterServicesPtr pRasSvcs = ::odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
      if (pRasSvcs.get())
      {
        if (dataImageNative.size())
          m_pImage = pRasSvcs->loadRasterImage(OdFlatMemStream::createNew(dataImageNative.asArrayPtr(), 
                                               dataImageNative.size()));
        if (m_pImage.isNull() && dataImageContent.size())
          m_pImage = pRasSvcs->loadRasterImage(OdFlatMemStream::createNew(dataImageContent.asArrayPtr(), 
                                               dataImageContent.size()));
        if (m_pImage.isNull() && dataImagePress.size())
          m_pImage = pRasSvcs->loadRasterImage(OdFlatMemStream::createNew(dataImagePress.asArrayPtr(), 
                                               dataImagePress.size()));
      }

      if (getExtByContent(dataImageNative) == L".wmf")
        m_wmfData = dataImageNative;
      else if (getExtByContent(dataImagePress) == L".wmf")
        m_wmfData = dataImagePress;
      else if (getExtByContent(dataImageContent) == L".wmf")
        m_wmfData = dataImageContent;

      ODA_ASSERT_ONCE(m_pImage.get() || m_wmfData.size());
    }
  }
}

OdBinaryData createCompoundDocument(OdGiRasterImagePtr pImage)
{
  OdBinaryData buff;
  ODA_ASSERT_ONCE(pImage.get());
  if (pImage.isNull())
    return buff;
  OdRxRasterServicesPtr pRasSvcs = ::odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  ODA_ASSERT_ONCE(pRasSvcs.get());
  if (pRasSvcs.isNull())
    return buff;

  OdMemoryStreamPtr pMemStream = OdMemoryStream::createNew();

  struct _RootStorage* pRootStorage = NULL;
//#if 0 //def PATH_TO_SAVE_INTERNAL_DATA // test
//  OdFilePathName info(PATH_TO_SAVE_INTERNAL_DATA + L"/dummy.storage");
//  if (info.exists())
//  {
//    OdStreamBufPtr io = ::odrxSystemServices()->createFile(info.pathName());
//    OdBinaryData dummyData;
//    dummyData.resize((OdUInt32) io->length());
//    io->getBytes(dummyData.asArrayPtr(), dummyData.size());
//    pMemStream->putBytes(dummyData.asArrayPtr(), dummyData.size());
//    pMemStream->rewind();
//    pRootStorage = openFileStorage(pMemStream.get(), STG_RW);
//  }
//  else
//#endif   
  pRootStorage = createFileStorage(pMemStream.get()); // createStructuredStorageInSource (

  Storage* pStorage = NULL;
  if (pRootStorage && (pStorage = getStorage(pRootStorage)) != NULL)
  {
    //StgInfo* pInfo = NULL;
    //if (getStorageInfo(pStorage, &pInfo) == SSTG_OK)
    //{
    //  OdString sName = (const wchar_t*) pInfo->sName;
    //  ODA_ASSERT_ONCE(sName == "Root Entry");
    //}
    SSRW_CLSID clsid = OD_PBRUSH_CLSID; 

    if (storageSetClass(pStorage, &clsid) != SSTG_OK)
    {
      ODA_FAIL_ONCE();
    }

    OdBinaryData data;
    OdUInt32 len = 0;
    OdMemoryStreamPtr pStream;
    Stream* pStrm = NULL;

    pStream = OdMemoryStream::createNew();
    OdOleSsCompObj compObj; compObj.outFileds(pStream);
    len = (OdUInt32) pStream->length(); data.resize(len); pStream->rewind();
    pStream->getBytes(data.asArrayPtr(), data.size());
    if ((pStrm = createStorageStream(pStorage, L"\01CompObj")) != NULL)
      writeStorageStream(&pStrm, data);

    pStream = OdMemoryStream::createNew();
    OdOleSsNative native(pImage); native.outFileds(pStream);
    len = (OdUInt32) pStream->length(); data.resize(len); pStream->rewind();
    pStream->getBytes(data.asArrayPtr(), data.size());
    if ((pStrm = createStorageStream(pStorage, L"\01Ole10Native")) != NULL)
      writeStorageStream(&pStrm, data);
    
    pStream = OdMemoryStream::createNew();
    OdOleSsOle ole; ole.outFileds(pStream);
    len = (OdUInt32) pStream->length(); data.resize(len); pStream->rewind();
    pStream->getBytes(data.asArrayPtr(), data.size());
    if ((pStrm = createStorageStream(pStorage, L"\01Ole")) != NULL)
      writeStorageStream(&pStrm, data);

    closeStorage(&pStorage);
    closeStructuredStorage(&pRootStorage);

    len = (OdUInt32) pMemStream->length();
    buff.resize(len);
    pMemStream->rewind();
    pMemStream->getBytes(buff.asArrayPtr(), buff.size());
  }

  return buff;
}

OdUInt32 OdOleSsItemHandlerImpl::getCompoundDocumentDataSize() const
{
  ODA_ASSERT_ONCE(m_pImage.isNull() || m_buff.size()); // m_buff  should be created via createCompoundDocument
  return m_buff.size();
}

void OdOleSsItemHandlerImpl::getCompoundDocument(OdStreamBuf& stream) const
{
  if (m_buff.size())
    stream.putBytes(m_buff.asArrayPtr(), m_buff.size());
}

class OdGiMetafileImpl : public OdGiMetafile
{
  OdUInt8* m_pBytes;
  OdUInt32 m_size;
public:
  OdGiMetafileImpl()
    : m_pBytes(NULL)
    , m_size(0)
  {
  }
  void init(OdUInt8* pBytes, OdUInt32 size)
  {
    ODA_FAIL_ONCE(); // TODO
    //m_pBytes = pBytes;
    //m_size = size;
  }
  virtual OdUInt32 dataSize() const
  {
    return m_size;
  }
  virtual void bitsData(OdUInt8* pBytes) const
  {
    pBytes = m_pBytes;
  }
  virtual const OdUInt8* bitsData() const
  {
    return m_pBytes;
  }
};

bool OdOleSsItemHandlerImpl::draw(const OdGiCommonDraw&, // drawObj 
                                  void*, // hdc
                                  const OdGsDCRect&) const // rect
{
  // TODO draw m_pImage on HDC for Windows //see CPreviewGiRasterImage::OnPaint()
  return false;
}

bool OdOleSsItemHandlerImpl::draw(OdGiConveyorGeometry* pDestGeom,
                                  const OdGePoint3d& origin,
                                  const OdGeVector3d& u, const OdGeVector3d& v, 
                                  const OdGsDCRect&, // rect
                                  bool isExport) // = false
{
  if (   !u.isZeroLength() && !v.isZeroLength()
      && (m_pImage.get() || m_wmfData.size()))
  {
    if (m_pImage.get())
      pDestGeom->rasterImageProc(
         origin, 
         u / (double) m_pImage->pixelWidth(), 
         v / (double) m_pImage->pixelHeight(),
         m_pImage, 0, 0, !isExport);
    //else if (m_wmfData.size())
    //{
      // TODO
      //OdStaticRxObject<OdGiMetafileImpl> wmfMetafile;
      //wmfMetafile.init(const_cast<OdUInt8*>(m_wmfData.asArrayPtr()), m_wmfData.size());
      //pDestGeom->metafileProc(
      //  origin,
      //  u, // / fabs((double) rect.m_max.x - rect.m_min.x),
      //  v, // / fabs((double) rect.m_max.y - rect.m_min.y),
      //  &wmfMetafile);
    //}
  }
  return true;
}

bool OdGiSelfGiDrawablePEImpl::draw(OdRxObject* pRxObject, 
                                    OdGiConveyorGeometry* pDestGeom,
                                    const OdGePoint3d& origin,
                                    const OdGeVector3d& u, const OdGeVector3d& v,
                                    const OdGsDCRect& rect,
                                    bool isExport) const // = false
{
  OdOleSsItemHandlerImplPtr pItem = OdOleSsItemHandlerImpl::cast(pRxObject);
  ODA_ASSERT_ONCE(pItem.get());
  if (pItem.get())
    return pItem->draw(pDestGeom, origin, u, v, rect, isExport);
  return true;
}

OdOleItemHandler::Type OdOleSsItemHandlerImpl::type() const
{
  return m_type;
}

OdOleItemHandler::DvAspect OdOleSsItemHandlerImpl::drawAspect() const
{
  return OdOleItemHandler::DvAspect(m_drawAspect);
}

OdString OdOleSsItemHandlerImpl::userType() const
{
  return m_sUserType;
}

OdString OdOleSsItemHandlerImpl::linkName() const
{
  return linkPath();
}

OdString OdOleSsItemHandlerImpl::linkPath() const
{
  return m_sLinkPath;
}

bool OdOleSsItemHandlerImpl::embedRaster(OdGiRasterImagePtr pImage,
                                         OdRxObject*) // pRxDb = NULL
{
  m_pImage = pImage;
  m_type = kEmbedded;
  m_sUserType = L"Paintbrush Picture";
  m_sLinkPath = OdString::kEmpty;
  ODA_ASSERT_ONCE(m_pImage.get());
  m_buff = createCompoundDocument(m_pImage);
  setDrawAspect(kContent);
  return !m_pImage.isNull();
}

//bool OdOleSsItemHandlerImpl::embedRasterFromFile(const OdString& path)
//{
//  OdRxRasterServicesPtr pRasSvcs = ::odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
//  ODA_ASSERT_ONCE(pRasSvcs.get());
//  if (pRasSvcs.isNull())
//    return false;
//  return embedRaster(pRasSvcs->loadRasterImage(path));
//}

OdGiRasterImagePtr OdOleSsItemHandlerImpl::getRaster(bool) const // bDisplayedOnly = true
{
  return m_pImage;
}

bool OdOleSsItemHandlerImpl::getWmfData(OdBinaryData& data) const
{
  data = m_wmfData;
  return m_wmfData.size() != 0;
}
