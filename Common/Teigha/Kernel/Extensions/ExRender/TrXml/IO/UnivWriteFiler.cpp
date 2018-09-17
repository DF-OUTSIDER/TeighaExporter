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
// UnivWriteFiler.cpp
//

#include "OdaCommon.h"
#include "RxSystemServices.h"
#include "UnivWriteFiler.h"
#include "Ge/GeMatrix3d.h"

OdUniversalWriteFiler::OdUniversalWriteFiler(const OdString& sFilePathName, 
                                             OdUInt32 nVersion, // = 100
                                             const char* pMagic) // = "TGS"
  : m_nVersion(100)
{
  ODA_ASSERT_ONCE(!sFilePathName.isEmpty());
  OdStreamBufPtr pStream = ::odrxSystemServices()->createFile(sFilePathName, Oda::kFileWrite, 
                                                              Oda::kShareDenyWrite, Oda::kCreateAlways);
  ODA_ASSERT_ONCE(pStream.get());
  setStream(pStream, nVersion, pMagic);
}

OdUniversalWriteFiler::OdUniversalWriteFiler(OdStreamBuf* pStream, // = NULL
                                             OdUInt32 nVersion, // = 100
                                             const char* pMagic) // = "TGS"
  : m_nVersion(100)
{
  setStream(pStream, nVersion, pMagic);
}

OdUniversalWriteFiler::~OdUniversalWriteFiler()
{
}

bool OdUniversalWriteFiler::setStream(OdStreamBuf* pStream, 
                                      OdUInt32 nVersion, // = 100
                                      const char* pMagic) // = "TGS"
{
  if (nVersion)
    m_nVersion = nVersion;
  if (m_pStream.get() != pStream)
  {
    m_pStream = pStream;
    if (pMagic)
      wrRawData(pMagic, (OdUInt32)odStrLenA(pMagic));
    wrUInt32(m_nVersion);
  }
  return true;
}

OdStreamBufPtr OdUniversalWriteFiler::stream() const
{
  return m_pStream;
}

OdUInt32 OdUniversalWriteFiler::version() const
{
  return m_nVersion;
}

void OdUniversalWriteFiler::assertWr() const 
{ 
  if (m_pStream.isNull())
    throw OdError(eNotOpenForWrite); 
}

// Process raw data

void OdUniversalWriteFiler::wrRawData(const void* pData, OdUInt32 nDataSize)
{
  assertWr();
  m_pStream->putBytes(pData, nDataSize);
}

void OdUniversalWriteFiler::wr2BytesNumber(void* pData)
{
  odSwap2BytesNumber(*(OdInt16*) pData);
  wrRawData(pData, 2); 
}

void OdUniversalWriteFiler::wr4BytesNumber(void* pData)
{
  odSwap4BytesNumber(*(OdUInt32*) pData);
  wrRawData(pData, 4); 
}

void OdUniversalWriteFiler::wr4Bytes(void* pData)
{
  odSwap4Bytes(pData);
  wrRawData(pData, 4);
}

void OdUniversalWriteFiler::wr8Bytes(void* pData)
{
  odSwap8Bytes(pData);
  wrRawData(pData, 8); 
}

// Implement data primitives

void OdUniversalWriteFiler::wrBool(bool bVal)
{ 
  wrUInt8(bVal ? 1 : 0);
}

void OdUniversalWriteFiler::wrInt(int val) 
{ 
  wrInt32(val);
}

void OdUniversalWriteFiler::wrUInt8(OdUInt8 val) 
{ 
  wrRawData(&val, sizeof(OdUInt8)); 
}

void OdUniversalWriteFiler::wrInt16(OdInt16 val) 
{ 
  wr2BytesNumber(&val);
}

void OdUniversalWriteFiler::wrUInt16(OdUInt16 val) 
{ 
  wr2BytesNumber(&val); 
}

void OdUniversalWriteFiler::wrInt32(OdInt32 val) 
{ 
  wr4BytesNumber(&val); 
}

void OdUniversalWriteFiler::wrUInt32(OdUInt32 val) 
{ 
  wr4BytesNumber(&val); 
}

void OdUniversalWriteFiler::wrInt64(OdInt64 val) 
{ 
  wr8Bytes(&val);
}

void OdUniversalWriteFiler::wrUInt64(OdUInt64 val) 
{ 
  wr8Bytes(&val);
}

void OdUniversalWriteFiler::wrFloat(float val) 
{ 
  wr4Bytes(&val); 
}

void OdUniversalWriteFiler::wrDouble(double val)
{
  wr8Bytes(&val);
}

void OdUniversalWriteFiler::wrMatrix3d(const OdGeMatrix3d& data) 
{ 
  //wrRawData(&data, sizeof(OdGeMatrix3d)); 
  wrFloat(data.entry[0][0]); wrFloat(data.entry[0][1]); wrFloat(data.entry[0][2]); wrFloat(data.entry[0][3]); 
  wrFloat(data.entry[1][0]); wrFloat(data.entry[1][1]); wrFloat(data.entry[1][2]); wrFloat(data.entry[1][3]); 
  wrFloat(data.entry[2][0]); wrFloat(data.entry[2][1]); wrFloat(data.entry[2][2]); wrFloat(data.entry[2][3]); 
  wrFloat(data.entry[3][0]); wrFloat(data.entry[3][1]); wrFloat(data.entry[3][2]); wrFloat(data.entry[3][3]);
}

void OdUniversalWriteFiler::wrAnsiString(const OdAnsiString &str)
{
  int nLen = str.getLength();
  wrInt(nLen);
  if (nLen)
    wrRawData(str.c_str(), OdUInt32(sizeof(char) * nLen));
}

