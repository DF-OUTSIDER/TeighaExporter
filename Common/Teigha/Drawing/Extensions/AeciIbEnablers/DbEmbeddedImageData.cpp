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
#include "DbEmbeddedImageData.h"
#include "DbFiler.h"
#include "StaticRxObject.h"
#include "FlatMemStream.h"
#include "MemoryStream.h"
#include "OdPlatformStreamer.h"
#include "DbDatabase.h"
#include "GeometryFromProxy.h"
#include "RxRasterServices.h"
#include "DbDictionary.h"

ODRX_DEFINE_MEMBERS_EX(AECIDbEmbeddedImageData,                           // ClassName
                       OdDbObject,                                        // ParentClass
                       DBOBJECT_CONSTR,                                   // DOCREATE
                       OdDb::vAC21,                                       // DwgVer
                       OdDb::kMRelease1,                                  // MaintVer
                       0x0081,                                            // ProxyFlags
                       L"AeciDbEmbeddedImageData",                        // DWG class name
                       L"AECIDBEMBEDDEDIMAGEDATA",                        // DxfName
                       L"AECIIB|Autodesk Raster Design|www.autodesk.com", // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware)                                  // CustomFlags

const OdInt32 AECIDbEmbeddedImageData::kCurrentClassVersion = 1;

AECIDbEmbeddedImageData::AECIDbEmbeddedImageData()
  : m_originalSize(0)
  , m_originalCreatedTime(0)
  , m_originalModifiedTime(0)
  , m_unknown2(2)
  , m_unknown3(-1)
{
  m_createdTime = OdTimeStamp(OdTimeStamp::kInitUniversalTime).packedValue();
  m_modifiedTime = m_createdTime;
}

TOOLKIT_EXPORT void odDwgReadEndBits(OdDbDwgFiler* pFiler, OdBitBinaryData& data);
TOOLKIT_EXPORT void odDwgWriteEndBits(OdDbDwgFiler* pFiler, const OdBitBinaryData& data);

OdResult AECIDbEmbeddedImageData::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdDbObject::dwgInFields(pFiler);

  OdInt32 classVersion = pFiler->rdInt32();
  if (classVersion > kCurrentClassVersion)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  OdUInt8 buff[52];
  const OdUInt8* pBuff = buff;

  pFiler->rdBytes(buff, 16);
  m_createdTime = OD_INT64_FROM_BUFFPTR(pBuff);
  m_modifiedTime = OD_INT64_FROM_BUFFPTR(pBuff);

  OdTimeStamp testTime;
  testTime.setPackedValue(m_createdTime);
  OdInt16 month, day, year;
  testTime.getDate(month, day, year);

  m_originalPath = pFiler->rdString();
  m_originalFormat = pFiler->rdString();
  pFiler->rdBytes(buff, 16); pBuff = buff;
  m_originalCreatedTime = OD_INT64_FROM_BUFFPTR(pBuff);
  m_originalModifiedTime = OD_INT64_FROM_BUFFPTR(pBuff);
  m_originalSize = pFiler->rdInt32();

  m_unknown2 = pFiler->rdInt32();
  m_unknown3 = pFiler->rdInt32();

  pFiler->rdBytes(buff, 52);
  m_corrInfo.read(buff);

  bool hasImageData = pFiler->rdBool();
  if (hasImageData)
  {
    pFiler->rdBytes(buff, 4); pBuff = buff;
    m_imageUnknown = OD_INT32_FROM_BUFFPTR(pBuff); ODA_ASSERT_ONCE(m_imageUnknown==0x0aaaaaa);
    pFiler->rdBytes(buff, 20);
    m_imageProp.read(buff);
    if (pFiler->filerType() == OdDbFiler::kFileFiler)
    {
      odDwgReadEndBits(pFiler, m_imageData);
      if (m_imageData.size() == 0)
      {
        ODA_FAIL();
        return eMakeMeProxy;
      }
    }
    else
    {
      OdInt32 dataSize = pFiler->rdInt32();
      m_imageData.resize(dataSize);
      if (dataSize)
        pFiler->rdBytes(m_imageData.asArrayPtr(), dataSize);
    }
  }

//#if 0
//  setRasterImage(getRasterImage());
//#endif

  return eOk;
}

void AECIDbEmbeddedImageData::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbObject::dwgOutFields(pFiler);

  pFiler->wrInt32(kCurrentClassVersion);

  OdUInt8 buff[52];
  OdUInt8* pBuff = buff;
  OD_INT64_TO_BUFFPTR(pBuff, m_createdTime);
  OD_INT64_TO_BUFFPTR(pBuff, m_modifiedTime);
  pFiler->wrBytes(buff, 16);
  pFiler->wrString(m_originalPath);
  pFiler->wrString(m_originalFormat);
  pBuff = buff;
  OD_INT64_TO_BUFFPTR(pBuff, m_originalCreatedTime);
  OD_INT64_TO_BUFFPTR(pBuff, m_originalModifiedTime);
  pFiler->wrBytes(buff, 16);
  pFiler->wrInt32(m_originalSize);
  pFiler->wrInt32(m_unknown2);
  pFiler->wrInt32(m_unknown3);
  m_corrInfo.write(buff);
  pFiler->wrBytes(buff, 52);
  bool hasImageData = m_imageData.size() > 0;
  pFiler->wrBool(hasImageData);
  if (hasImageData)
  {
    pBuff = buff;
    OD_INT32_TO_BUFFPTR(pBuff, m_imageUnknown);
    pFiler->wrBytes(buff, 4);
    m_imageProp.write(buff);
    pFiler->wrBytes(buff, 20);
    if (pFiler->filerType() == OdDbFiler::kFileFiler)
    {
      odDwgWriteEndBits(pFiler, m_imageData);
    }
    else
    {
      OdInt32 dataSize = m_imageData.size();
      pFiler->wrInt32(dataSize);
      if (dataSize)
        pFiler->wrBytes(m_imageData.asArrayPtr(), dataSize);
    }
  }
}

void AECIDbEmbeddedImageData::dxfOut(OdDbDxfFiler* pFiler) const
{
  assertReadEnabled();
  if (!isDBRO())
  {
    OdDbObject::dxfOut(pFiler);
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

OdDbObjectId AECIDbEmbeddedImageData::imageDataDictionary(OdDbDatabase* pDb, bool createIfNotFound)
{
  if(!pDb)
    throw OdError(eNoDatabase);

  OdDbDictionaryPtr pRootDic = pDb->getNamedObjectsDictionaryId().safeOpenObject();
  OdDbObjectId dicId = pRootDic->getAt(L"AECI_EMBED_IMAGE_DATA_DICT");
  if (dicId.isErased() && createIfNotFound)
  {
    OdDbObjectPtr pDic = OdDbDictionary::createObject();
    pRootDic->upgradeOpen();
    dicId = pRootDic->setAt(L"AECI_EMBED_IMAGE_DATA_DICT", pDic);
  }
  return dicId;
}

class AECIEmbeddedTiffComposer
{
public:
  enum TiffDataType
  {
    kTiffNotype = 0,      // placeholder
    kTiffByte = 1,        // 8-bit unsigned integer
    kTiffAscii = 2,       // 8-bit bytes w/ last byte null
    kTiffShort = 3,       // 16-bit unsigned integer
    kTiffLong = 4,        // 32-bit unsigned integer
    kTiffRational = 5,    // 64-bit unsigned fraction (the ratio of two 32-bit integer values)
    kTiffSbyte = 6,       // 8-bit signed integer
    kTiffUndefined = 7,   // 8-bit untyped data
    kTiffSshort = 8,      // 16-bit signed integer
    kTiffSlong = 9,       // 32-bit signed integer
    kTiffSRational = 10,  // 64-bit signed fraction (the ratio of two 32-bit integer values)
    kTiffFloat = 11,      // 32-bit IEEE floating point
    kTiffDouble = 12,     // 64-bit IEEE floating point
    kTiffIfd = 13         // 32-bit unsigned integer (offset)
  };

  enum  TiffDataTagDefinition
  {
    kTiffImageWidth = 256,
    kTiffImageLength = 257,
    kTiffBitsPerSample = 258,   // bits per channel (sample)
    kTiffCompression = 259,     // data compression technique
    kTiffPhotometric = 262,     // photometric interpretation
    kTiffSamplesPerPixel = 277, // samples per pixel
    kTiffRowsPerStrip = 278,    // rows per strip of data
    kTiffXResolution = 282,     // pixels/resolution in x
    kTiffYResolution = 283,     // pixels/resolution in y
    kTiffPlanarConfig = 284,    // storage organization
    kTiffResolutionUnit = 296,  // units of resolutions
    kTiffTileWidth = 322,       // tile width in pixels
    kTiffTileLength = 323,      // tile height in pixels
    kTiffTileOffsets = 324,     // offsets to data tiles
    kTiffTileByteCounts = 325,  // byte counts for tiles
    kTiffSampleFormat = 339     // data sample format
  };

  AECIEmbeddedTiffComposer()
  {
  }

  AECIEmbeddedTiffComposer(const AECIDbEmbeddedImageData& img)
    : m_corrInfo(img.sourceCorrelation())
    , m_imageProp(img.imageProperties())
    , m_imageData(img.imageRawData())
  {
  }

  const AECISourceCorrelationInfo& correlationInfo() { return m_corrInfo; }

  const AECIImageProperties& imageProperties() { return m_imageProp; }

  const OdBitBinaryData& imageRawData() { return m_imageData; }

  void wrRational(OdStreamBuf& strm, double v, bool writeSigned = false) const
  {
    int sign = 1;
    if (v < 0)
    {
      v = -v;
      ODA_ASSERT_ONCE(writeSigned); // Information lost writing signed value as unsigned rational
      if (writeSigned)
      {
        sign = -1;
      }
    }
    OdUInt32 den = 1L;
    if (v > 0)
    {
      while (v < 1L << (31-3) && den < 1L << (31-3))
        v *= 1 << 3, den *= 1L << 3;
    }
    OdUInt32 num = (OdUInt32) (sign * (v + 0.5));

    OdPlatformStreamer::wrInt32(strm, num); // numerator
    OdPlatformStreamer::wrInt32(strm, den); // denominator
  }

  void wrIfdEntry(OdStreamBuf& strm, int tag, int type, int value) const
  {
    OdPlatformStreamer::wrInt16(strm, (OdInt16)tag);
    OdPlatformStreamer::wrInt16(strm, (OdInt16)type);
    OdPlatformStreamer::wrInt32(strm, 1); // count
    OdPlatformStreamer::wrInt32(strm, value);
  }

  OdUInt16 rdIfdEntry(OdStreamBuf& strm, OdInt32& value) const
  {
    OdUInt16 tag = OdPlatformStreamer::rdInt16(strm);
    OdPlatformStreamer::rdInt16(strm); // type
    OdPlatformStreamer::rdInt32(strm); // count
    value = OdPlatformStreamer::rdInt32(strm);
    return tag;
  }

  void getTileSize(OdUInt32& tw, OdUInt32& tl) const
  {
    tw = m_imageProp.m_width;
    tl = m_imageProp.m_height;
  }

  OdStreamBufPtr composeTiff() const
  {
    unsigned rawDataSize = m_imageData.size();
    if (rawDataSize == 0)
      return OdStreamBufPtr();

    OdMemoryStreamPtr ps = OdMemoryStream::createNew();

    const OdUInt16 ifdNumEntries = 16;
    const unsigned ifdEntrySize = 12; // Tag (short) + Type (short) + Count (long) + Value (long)
    const unsigned ifdSize = sizeof(OdInt16) + ifdNumEntries * ifdEntrySize + sizeof(OdInt32);
    const unsigned ifdOffset = rawDataSize + 8;

    // tiff header
    OdPlatformStreamer::wrInt32(*ps.get(), 0x002a4949); // signature
    OdPlatformStreamer::wrInt32(*ps.get(), ifdOffset);  // IFD offset
    // tiff tiled data
    ps->putBytes(m_imageData.asArrayPtr(), rawDataSize);
    // tiff IFD
    OdPlatformStreamer::wrInt16(*ps.get(), ifdNumEntries); // number of entries
    wrIfdEntry(*ps.get(), kTiffImageWidth, kTiffShort, m_imageProp.m_width);
    wrIfdEntry(*ps.get(), kTiffImageLength, kTiffShort, m_imageProp.m_height);
    wrIfdEntry(*ps.get(), kTiffBitsPerSample, kTiffShort, 1);
    wrIfdEntry(*ps.get(), kTiffCompression, kTiffShort, 4); // CCITT Group 4 fax encoding
    wrIfdEntry(*ps.get(), kTiffPhotometric, kTiffShort, 0); // min value is white
    wrIfdEntry(*ps.get(), kTiffSamplesPerPixel, kTiffShort, 1);
    wrIfdEntry(*ps.get(), kTiffRowsPerStrip, kTiffShort, m_imageProp.m_height);
    wrIfdEntry(*ps.get(), kTiffXResolution, kTiffRational, ifdOffset + ifdSize);
    wrIfdEntry(*ps.get(), kTiffYResolution, kTiffRational, ifdOffset + ifdSize + 8);
    wrIfdEntry(*ps.get(), kTiffPlanarConfig, kTiffShort, 1); // contig
    wrIfdEntry(*ps.get(), kTiffResolutionUnit, kTiffShort, 3); // 1 - no meaningful units, 2 - english, 3 - metric
    OdUInt32 tw = 0, tl = 0;
    getTileSize(tw, tl);
    wrIfdEntry(*ps.get(), kTiffTileWidth, kTiffShort, tw);
    wrIfdEntry(*ps.get(), kTiffTileLength, kTiffShort, tl);
    wrIfdEntry(*ps.get(), kTiffTileOffsets, kTiffLong, 8);
    wrIfdEntry(*ps.get(), kTiffTileByteCounts, kTiffLong, rawDataSize);
    wrIfdEntry(*ps.get(), kTiffSampleFormat, kTiffShort, 1); // unsigned integer data
    OdPlatformStreamer::wrInt32(*ps.get(), 0); // next IFD offset
    ODA_ASSERT(ps->tell() == (ifdOffset + ifdSize));

    // tiff IFD referenced data (rational XResolution, YResolution)
    wrRational(*ps.get(), m_imageProp.m_width * m_corrInfo.m_density);
    wrRational(*ps.get(), m_imageProp.m_height * m_corrInfo.m_density);

    ps->seek(0, OdDb::kSeekFromStart);
    return ps;
  }

  void decomposeTiff(OdStreamBufPtr ps)
  {
    // tiff header
    OdUInt32 signature = OdPlatformStreamer::rdInt32(*ps.get());
    if (signature != 0x002a4949)
      throw OdError(eNotApplicable);
    OdUInt32 ifdOffset = OdPlatformStreamer::rdInt32(*ps.get());
    ps->seek(ifdOffset, OdDb::kSeekFromStart);

    // tiff IFD
    OdUInt16 ifdNumEntries = OdPlatformStreamer::rdInt16(*ps.get());
    OdUInt32 tileOffsets = 0;
    OdUInt32 tileByteCounts = 0;
    OdUInt32 tileWidth = 0;
    OdUInt32 tileLength = 0;
    for (OdUInt16 i = 0; i < ifdNumEntries; ++i)
    {
      OdInt32 value = 0;
      switch (rdIfdEntry(*ps.get(), value))
      {
      case kTiffImageWidth: m_imageProp.m_width = value; break;
      case kTiffImageLength: m_imageProp.m_height = value; break;
      case kTiffTileWidth: tileWidth = value; break;
      case kTiffTileLength: tileLength = value; break;
      case kTiffTileOffsets: tileOffsets = value; break;
      case kTiffTileByteCounts: tileByteCounts = value; break;
      default:
        break;
      }
    }
    if (tileWidth != m_imageProp.m_width || tileLength != m_imageProp.m_height)
    {
      throw OdError(eNotApplicable);
    }

    // tiff tiled data
    if (tileOffsets == 0 || tileByteCounts == 0)
    {
      throw OdError(eNotApplicable);
    }
    ps->seek(tileOffsets, OdDb::kSeekFromStart);
    m_imageData.resize(tileByteCounts + 1);
    ps->getBytes(m_imageData.asArrayPtr(), tileByteCounts);

    // Append Acad terminate symbol
    if (m_imageData[tileByteCounts - 1] != 0x7a)
      m_imageData[tileByteCounts] = 0x7a;
    else
      m_imageData.resize(tileByteCounts);
    m_imageData.updateBitSize();
  }
private:
  AECISourceCorrelationInfo m_corrInfo;
  AECIImageProperties m_imageProp;
  OdBitBinaryData m_imageData;
};

OdStreamBufPtr AECIDbEmbeddedImageData::getRasterImage() const
{
  assertReadEnabled();
  OdStreamBufPtr pStream;
  try
  {
    pStream = AECIEmbeddedTiffComposer(*this).composeTiff();

#if 0
    OdStreamBufPtr pFile = odrxSystemServices()->createFile(database()->getFilename() + L".tif", Oda::kFileWrite, Oda::kShareDenyReadWrite, Oda::kCreateAlways);
    pStream->copyDataTo(pFile);
    pStream->seek(0, OdDb::kSeekFromStart);
    pFile = 0;
#endif
  }
  catch (OdError&)
  {
  }
  return pStream;
}

void AECIDbEmbeddedImageData::setRasterImage(OdStreamBufPtr pRasterBuf)
{
  assertWriteEnabled();

  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME, false);

  OdStreamBufPtr pTiffBuff = OdMemoryStream::createNew();

  const OdUInt32 dstFlagsChain[3] = { OdRxRasterServices::kTiffCompression, OdRxRasterServices::kTiffCompressionEmbedded, 0 };
  if (!pRasSvcs->convertRasterImage(pRasterBuf.get(), pTiffBuff.get(), OdRxRasterServices::kTIFF, NULL, dstFlagsChain))
    throw OdError(eNotApplicable);

  AECIEmbeddedTiffComposer composer;
  composer.decomposeTiff(pTiffBuff);

  m_imageProp = composer.imageProperties();
  m_imageData = composer.imageRawData();

  m_modifiedTime = OdTimeStamp(OdTimeStamp::kInitUniversalTime).packedValue();

#if 0
  pTiffBuff->seek(0, OdDb::kSeekFromStart);
  OdStreamBufPtr pFile = odrxSystemServices()->createFile(database()->getFilename() + L"-converted.tif", Oda::kFileWrite, Oda::kShareDenyReadWrite, Oda::kCreateAlways);
  pTiffBuff->copyDataTo(pFile);
  pFile = 0;
#endif
}

OdTimeStamp AECIDbEmbeddedImageData::dataCreatedTime() const
{
  assertReadEnabled();
  OdTimeStamp t;
  t.setPackedValue(m_createdTime);
  return t;
}

OdTimeStamp AECIDbEmbeddedImageData::dataModifiedTime() const
{
  assertReadEnabled();
  OdTimeStamp t;
  t.setPackedValue(m_modifiedTime);
  return t;
}

OdUInt32 AECIDbEmbeddedImageData::imageRawDataSize() const
{
  assertReadEnabled();
  return m_imageData.size();
}

OdString AECIDbEmbeddedImageData::originalFilePath() const
{
  assertReadEnabled();
  return m_originalPath;
}

void AECIDbEmbeddedImageData::setOriginalFilePath(const OdString& path)
{
  assertWriteEnabled();
  m_originalPath = path;
}

OdString AECIDbEmbeddedImageData::originalFormat() const
{
  assertReadEnabled();
  return m_originalFormat;
}

void AECIDbEmbeddedImageData::setOriginalFormat(const OdString& format)
{
  assertWriteEnabled();
  m_originalFormat = format;
}

void AECIDbEmbeddedImageData::originalFileDetails(OdTimeStamp& origCreatedTime, OdTimeStamp& origModifiedTime, OdUInt32& origSize) const
{
  assertReadEnabled();
  origCreatedTime.setPackedValue(m_originalCreatedTime);
  origModifiedTime.setPackedValue(m_originalModifiedTime);
  origSize = m_originalSize;
}

void AECIDbEmbeddedImageData::setOriginalFileDetails(OdTimeStamp origCreatedTime, OdTimeStamp origModifiedTime, OdUInt32 origSize)
{
  assertWriteEnabled();
  m_originalCreatedTime = origCreatedTime.packedValue();
  m_originalModifiedTime = origModifiedTime.packedValue();
  m_originalSize = origSize;
}

const AECIImageProperties& AECIDbEmbeddedImageData::imageProperties() const
{
  assertReadEnabled();
  return m_imageProp;
}

const OdBitBinaryData& AECIDbEmbeddedImageData::imageRawData() const
{
  assertReadEnabled();
  return m_imageData;
}

const AECISourceCorrelationInfo& AECIDbEmbeddedImageData::sourceCorrelation() const
{
  assertReadEnabled();
  return m_corrInfo;
}

void AECIDbEmbeddedImageData::setSourceCorrelation(const AECISourceCorrelationInfo& src)
{
  assertReadEnabled();
  m_corrInfo = src;
}

void AECISourceCorrelationInfo::read(const OdUInt8* pd)
{
  OdUInt8* p = (OdUInt8*)pd;
  m_units = OD_INT32_FROM_BUFFPTR(p);
  m_scale = OD_DOUBLE_FROM_BUFFPTR(p);
  m_rotation = OD_DOUBLE_FROM_BUFFPTR(p);
  m_density = OD_DOUBLE_FROM_BUFFPTR(p);
  m_origin.x = OD_DOUBLE_FROM_BUFFPTR(p);
  m_origin.y = OD_DOUBLE_FROM_BUFFPTR(p);
  m_origin.z = OD_DOUBLE_FROM_BUFFPTR(p);
}

void AECISourceCorrelationInfo::write(OdUInt8* p) const
{
  OD_INT32_TO_BUFFPTR(p, m_units);
  OD_DOUBLE_TO_BUFFPTR(p, m_scale);
  OD_DOUBLE_TO_BUFFPTR(p, m_rotation);
  OD_DOUBLE_TO_BUFFPTR(p, m_density);
  OD_DOUBLE_TO_BUFFPTR(p, m_origin.x);
  OD_DOUBLE_TO_BUFFPTR(p, m_origin.y);
  OD_DOUBLE_TO_BUFFPTR(p, m_origin.z);
}

void AECIImageProperties::read(const OdUInt8* p)
{
   m_width = OD_INT32_FROM_BUFFPTR(p);
   m_height = OD_INT32_FROM_BUFFPTR(p);
   m_value3 = OD_INT32_FROM_BUFFPTR(p);
   m_value4 = OD_INT32_FROM_BUFFPTR(p);
   m_value5 = OD_INT32_FROM_BUFFPTR(p);
}

void AECIImageProperties::write(OdUInt8* p) const
{
  OD_INT32_TO_BUFFPTR(p, m_width );
  OD_INT32_TO_BUFFPTR(p, m_height);
  OD_INT32_TO_BUFFPTR(p, m_value3);
  OD_INT32_TO_BUFFPTR(p, m_value4);
  OD_INT32_TO_BUFFPTR(p, m_value5);
}
