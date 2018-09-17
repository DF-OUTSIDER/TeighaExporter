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
// UnivReadFiler.cpp
//

#include "OdaCommon.h"
#include "RxSystemServices.h"
#include "UnivReadFiler.h"
#include "Ge/GeMatrix3d.h"
#include "OdFileBuf.h"

#ifdef OD_TGS_ENABLED
#undef ODA_ASSERT_ONCE
#define ODA_ASSERT_ONCE(a)
#endif

OdUniversalReadFiler::OdUniversalReadFiler(const OdString& sFilePathName, 
                                           OdUInt32 nVersion, // = 100
                                           const char* pMagic) // = "TGS"
  : m_nVersion(100)
  , m_bBigEndian(false)
{
  ODA_ASSERT_ONCE(!sFilePathName.isEmpty());
  setBigEndian();

  ////odrxSystemServices() is undefined and generate exception in XmlGLES2View vis missing RTTI initializing
  ////OdStreamBufPtr pStream = odrxSystemServices()->createFile(sFilePathName, Oda::kFileRead, Oda::kShareDenyNo);
  OdRdFileBuf* pRdFileBuf = OdRdFileBuf::createObject().detach();
  OdStreamBufPtr pStream = (OdStreamBuf*) pRdFileBuf;
  pStream->release();
  pRdFileBuf->open(sFilePathName, Oda::kShareDenyWrite);

  ODA_ASSERT_ONCE(pStream.get());
  setStream(pStream, nVersion, pMagic);
}

OdUniversalReadFiler::OdUniversalReadFiler(OdStreamBuf* pStream, // = NULL
                                           OdUInt32 nVersion, // = 100
                                           const char* pMagic) // = "TGS"
  : m_nVersion(100)
  , m_bBigEndian(false)
{
  setBigEndian();

  setStream(pStream, nVersion, pMagic);
}

OdUniversalReadFiler::~OdUniversalReadFiler()
{
  m_pStream = NULL;
}

bool OdUniversalReadFiler::setStream(OdStreamBuf* pStream, 
                                     OdUInt32 nVersion, // = 100
                                     const char* pcszMagic) // = "TGS"
{
  if (nVersion)
    m_nVersion = nVersion;
  if (m_pStream.get() != pStream)
  {
    m_pStream = pStream;
    if (m_pStream.get())
    {
      if (pcszMagic)
      {
        OdUInt32 len = (OdUInt32)odStrLenA(pcszMagic);
        OdAnsiString asMagic;
        char* pBuf = asMagic.getBuffer(len + 1);
        rdRawData(pBuf, len);
        pBuf[len] = '\0';
        asMagic.releaseBuffer();
        ODA_ASSERT_ONCE(asMagic == pcszMagic);
        if (asMagic != pcszMagic)
          throw OdError(eUnsupportedFileFormat);
      }
      nVersion = rdUInt32();
      ODA_ASSERT_ONCE(!m_nVersion || nVersion <= m_nVersion);
      if (m_nVersion && nVersion > m_nVersion)
        throw OdError(eInvalidFileVersion);
    }
  }
  return true;
}

OdStreamBufPtr OdUniversalReadFiler::stream() const
{
  return m_pStream;
}

OdUInt32 OdUniversalReadFiler::version() const
{
  return m_nVersion;
}

bool OdUniversalReadFiler::isEof() const
{
#ifndef OD_TGS_ENABLED
  assertRd();
#endif
  return m_pStream->isEof();
}

void OdUniversalReadFiler::assertRd() const 
{ 
  if (m_pStream.isNull())
    throw OdError(eNotOpenForRead); 
}

bool OdUniversalReadFiler::isBigEndian() const
{
  return m_bBigEndian;
}

void OdUniversalReadFiler::setBigEndian()
{
  // TODO check result function after EMSCRIPTEN
  // https://ru.wikipedia.org/wiki/%D0%9F%D0%BE%D1%80%D1%8F%D0%B4%D0%BE%D0%BA_%D0%B1%D0%B0%D0%B9%D1%82%D0%BE%D0%B2
  unsigned short x = 1; // 0x0001
  setBigEndian(*((unsigned char *) &x) == 0);
}

void OdUniversalReadFiler::setBigEndian(bool bValue)
{
  m_bBigEndian = bValue;
}

void OdUniversalReadFiler::rdRawData(void* pData, OdUInt32 nDataSize) const
{
#ifndef OD_TGS_ENABLED
  assertRd();
#endif
  m_pStream->getBytes(pData, nDataSize);
}

//#ifdef ODA_BIGENDIAN
//inline void odSwap4Bytes(void* pBytes)
//{
//  odSwapBytes(((OdUInt8*)(pBytes))[0], ((OdUInt8*)(pBytes))[3]);
//  odSwapBytes(((OdUInt8*)(pBytes))[1], ((OdUInt8*)(pBytes))[2]);
//}
//#else
//#define odSwap4Bytes(bytes)
//#endif // ODA_BIGENDIAN

inline void odSwap2BytesNumberEx(OdInt16& W) 
{ 
  odSwapBytes(((OdUInt8*)&(W))[0], ((OdUInt8*)&(W))[1]); 
}
inline void odSwap4BytesNumberEx(OdUInt32& DW)
{
  odSwapWords(((OdUInt16*)&(DW))[0], ((OdUInt16*)&(DW))[1]);
  odSwapBytes(((OdUInt8*)&(DW))[0], ((OdUInt8*)&(DW))[1]);
  odSwapBytes(((OdUInt8*)&(DW))[2], ((OdUInt8*)&(DW))[3]);
}
inline void odSwap4BytesEx(void* pBytes)
{
  odSwapBytes(((OdUInt8*)(pBytes))[0], ((OdUInt8*)(pBytes))[3]);
  odSwapBytes(((OdUInt8*)(pBytes))[1], ((OdUInt8*)(pBytes))[2]);
}
inline void odSwap8BytesEx(void* pBytes)
{
  odSwapBytes(((OdUInt8*)(pBytes))[0], ((OdUInt8*)(pBytes))[7]);
  odSwapBytes(((OdUInt8*)(pBytes))[1], ((OdUInt8*)(pBytes))[6]);
  odSwapBytes(((OdUInt8*)(pBytes))[2], ((OdUInt8*)(pBytes))[5]);
  odSwapBytes(((OdUInt8*)(pBytes))[3], ((OdUInt8*)(pBytes))[4]);
}

void OdUniversalReadFiler::rd2BytesNumber(void* pData) const
{
  rdRawData(pData, 2); 
  //odSwap2BytesNumber(*(OdInt16*) pData);
  if (isBigEndian())
    odSwap2BytesNumberEx(*(OdInt16*) pData);
}

void OdUniversalReadFiler::rd4BytesNumber(void* pData) const
{
  rdRawData(pData, 4); 
  //odSwap4BytesNumber(*(OdUInt32*) pData);
  if (isBigEndian())
    odSwap4BytesNumberEx(*(OdUInt32*) pData);
}

void OdUniversalReadFiler::rd4Bytes(void *pData) const
{
  rdRawData(pData, 4); 
  //odSwap4Bytes(pData);
  if (isBigEndian())
    odSwap4BytesEx(pData);
}

void OdUniversalReadFiler::rd8Bytes(void *pData) const
{
  rdRawData(pData, 8); 
  //odSwap8Bytes(pData);
  if (isBigEndian())
    odSwap8BytesEx(pData);
}

// Implement data primitives

bool OdUniversalReadFiler::rdBool() const 
{ 
  return rdUInt8() != 0; 
}

int OdUniversalReadFiler::rdInt() const 
{ 
  return (int) rdInt32();
}

OdUInt8 OdUniversalReadFiler::rdUInt8() const 
{ 
  OdUInt8 val(0); rdRawData(&val, sizeof(OdUInt8)); return val; 
}

OdInt16 OdUniversalReadFiler::rdInt16() const 
{ 
  OdInt16 val(0); rd2BytesNumber(&val); return val; 
}

OdUInt16 OdUniversalReadFiler::rdUInt16() const 
{ 
  OdUInt16 val(0);
  if (!isEof())
  //try
  //{
    rd2BytesNumber(&val); 
  //}
  //catch (const OdError_FileException& e) // WebAdapt generate files without terminate zero
  //{
  //  ODA_ASSERT_ONCE(e.code() == eEndOfFile && m_pStream->isEof());
  //  val = 0;
  //}
  return val; 
}

OdInt32 OdUniversalReadFiler::rdInt32() const 
{ 
  OdInt32 val(0); rd4BytesNumber(&val); return val; 
}

OdUInt32 OdUniversalReadFiler::rdUInt32() const 
{ 
  OdUInt32 val(0); rd4BytesNumber(&val); return val; 
}

OdInt64 OdUniversalReadFiler::rdInt64() const 
{ 
  OdInt64 val(0); rd8Bytes(&val); return val; 
}

OdUInt64 OdUniversalReadFiler::rdUInt64() const 
{ 
  OdUInt64 val(0); rd8Bytes(&val); return val; 
}

float OdUniversalReadFiler::rdFloat() const 
{ 
  float val(0.0f); rd4Bytes(&val); return val; 
}

double OdUniversalReadFiler::rdDouble() const
{
  double val(0.0); rd8Bytes(&val); return val; 
}

void OdUniversalReadFiler::rdMatrix3d(OdGeMatrix3d& data) const 
{ 
  //rdRawData(&data, sizeof(OdGeMatrix3d)); 
  data.entry[0][0] = rdFloat(); data.entry[0][1] = rdFloat(); data.entry[0][2] = rdFloat(); data.entry[0][3] = rdFloat();
  data.entry[1][0] = rdFloat(); data.entry[1][1] = rdFloat(); data.entry[1][2] = rdFloat(); data.entry[1][3] = rdFloat();
  data.entry[2][0] = rdFloat(); data.entry[2][1] = rdFloat(); data.entry[2][2] = rdFloat(); data.entry[2][3] = rdFloat();
  data.entry[3][0] = rdFloat(); data.entry[3][1] = rdFloat(); data.entry[3][2] = rdFloat(); data.entry[3][3] = rdFloat();
}

void OdUniversalReadFiler::rdAnsiString(OdAnsiString &str) const
{
  int nLen = rdInt();
  if (!nLen)
    str.empty();
  else
  {
    char* pData = str.getBufferSetLength(nLen);
    rdRawData(pData, OdUInt32(sizeof(char) * nLen));
    str.releaseBuffer(nLen);
  }
}
