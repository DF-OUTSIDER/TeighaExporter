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
// BinaryGlesParser.cpp
//

#include "OdaCommon.h"
#include "BinaryGlesParser.h"
#include "XmlGlesLoader.h"
#include "RxDictionary.h"
#include "TrVisRendition.h"
#include "FlatMemStream.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#include "CloudTools.h"

#ifdef OD_TGS_ENABLED
#undef ODA_ASSERT_ONCE
#define ODA_ASSERT_ONCE(a)
#endif

///////////////////////////////////////////////////////////////////////////////

static bool func_Level(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk) // level 0 & 1
{
  bool isBegin = pFiler->rdBool();
  if (isBegin)
  {
    if (pThis->m_pLoader->DropBegin(pk))
      return true;
    pThis->m_sError = L"error inside OdXmlGlesLoader::DropBegin";
    return false;
  }
  if (pThis->m_pLoader->DropEnd(pk))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropEnd";
  return false;
}

static bool func_Id(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdAnsiString asValue;
  pFiler->rdAnsiString(asValue);
  if (pThis->m_pLoader->DropId(pk, asValue))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropId";
  return false;
}

static bool func_DbStub(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdAnsiString asHandle;
  pFiler->rdAnsiString(asHandle);
  if (pThis->m_pLoader->DropDbStub(pk, asHandle))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropDbStub";
  return false;
}

static bool func_Chars(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdAnsiString asValue;
  pFiler->rdAnsiString(asValue);
  if (pThis->m_pLoader->DropChars(pk, asValue))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropChars";
  return false;
}

static bool func_BinaryStream(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt32 nData = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(nData);
  OdBinaryData data;
  data.resize(nData);
  unsigned char* pData = (unsigned char*) data.asArrayPtr();
  pFiler->rdRawData(pData, nData);
  if (pThis->m_pLoader->DropBinaryStream(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropChars";
  return false;
}

static bool func_Boolean(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  bool bVal = pFiler->rdBool();
  if (pThis->m_pLoader->DropBoolean(pk, bVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropBoolean";
  return false;
}

static bool func_UInt8RGB(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdBinaryData data;
  data.resize(4);
  for (unsigned int idx = 0; idx < data.size(); idx++)
  {
    OdUInt8& iVal = data[idx];
    iVal = pFiler->rdUInt8();
  }
  if (pThis->m_pLoader->DropUInt8RGB(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt8RGB";
  return false;
}

static bool func_UInt8(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt8 iVal = pFiler->rdUInt8();
  if (pThis->m_pLoader->DropUInt8(pk, iVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt8";
  return false;
}

static bool func_UInt16(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt8 iVal = pFiler->rdUInt16();
  if (pThis->m_pLoader->DropUInt16(pk, iVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt16";
  return false;
}

static bool func_UInt32(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt32 iVal = pFiler->rdUInt32();
  if (pThis->m_pLoader->DropUInt32(pk, iVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt32";
  return false;
}

static bool func_UInts16(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt32 nData = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(nData);
  OdArray<OdUInt16, OdMemoryAllocator<OdUInt16> > data;
  data.resize(nData);
  for (OdUInt32 idx = 0; idx < nData; idx++)
  {
    OdUInt16& iVal = data[idx]; 
    iVal = pFiler->rdUInt16();
  }
  if (pThis->m_pLoader->DropUInts16(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInts16";
  return false;
}

static bool func_UInt64(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt64 val = pFiler->rdUInt64();
  if (pThis->m_pLoader->DropUInt64(pk, val))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt64";
  return false;
}

static bool func_Int16(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdInt16 iVal = pFiler->rdInt16(); 
  if (pThis->m_pLoader->DropInt16(pk, iVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropInt16";
  return false;
}

static bool func_Int32(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdInt32 iVal = pFiler->rdInt32();
  if (pThis->m_pLoader->DropInt32(pk, iVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropInt32";
  return false;
}

static bool func_Ints(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt32 nData = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(nData);
  OdIntArray data;
  data.resize(nData);
  for (OdUInt32 idx = 0; idx < nData; idx++)
  {
    int& iVal = data[idx]; 
    iVal = (int) pFiler->rdUInt16();
  }
  if (pThis->m_pLoader->DropInts(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropInts";
  return false;
}

static bool func_Float(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  float fVal = pFiler->rdFloat();
  if (pThis->m_pLoader->DropFloat(pk, fVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropFloat";
  return false;
}

static bool func_Floats(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt32 nData = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(nData);
  OdArray<float, OdMemoryAllocator<float> > data;
  data.resize(nData);
  for (OdUInt32 idx = 0; idx < nData; idx++)
  {
    float& fVal = data[idx]; 
    fVal = pFiler->rdFloat();
  }
  if (pThis->m_pLoader->DropFloats(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropFloats";
  return false;
}

static bool func_Double(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  double dVal = pFiler->rdDouble();
  if (pThis->m_pLoader->DropDouble(pk, dVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropDouble";
  return false;
}

static bool func_Doubles(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt32 nData = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(nData);
  OdArray<double, OdMemoryAllocator<double> > data;
  data.resize(nData);
  for (OdUInt32 idx = 0; idx < nData; idx++)
  {
    double& dVal = data[idx]; 
    dVal = pFiler->rdDouble();
  }
  if (pThis->m_pLoader->DropDoubles(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropDoubles";
  return false;
}

static bool func_Matrix(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdGeMatrix3d data;
  pFiler->rdMatrix3d(data);
  if (pThis->m_pLoader->DropMatrix(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropFloats";
  return false;
}

static bool func_UInt32ArrayType(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  OdUInt32 iVal = pFiler->rdUInt32(); 
  if (!pThis->m_pLoader->DropUInt32(pk, iVal))
  {
    pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt32";
    return false;
  }
  switch ((enum OdTrVisArrayWrapper::Type) iVal)
  {
  case OdTrVisArrayWrapper::Type_Vertex:
  case OdTrVisArrayWrapper::Type_Normal:
  case OdTrVisArrayWrapper::Type_Color:
  case OdTrVisArrayWrapper::Type_TexCoord:
    pThis->m_funcArray = func_Floats; break;
  case OdTrVisArrayWrapper::Type_Index:
    pThis->m_funcArray = func_UInts16; break;
  case OdTrVisArrayWrapper::Type_Marker:
    pThis->m_funcArray = func_Level; break; // func_UInts64 is unused now
  default:
    pThis->m_sError = L"func_UInt32ArrayType error: unsupported array type";
    return false;
  }
  return true;
}

static bool func_Array(OdBinaryGlesParser* pThis, OdUniversalReadFiler* pFiler, OdEnPathKey pk)
{
  if (pThis->m_funcArray)
    return (*pThis->m_funcArray)(pThis, pFiler, pk);

  pThis->m_sError = L"func_Array error: m_funcArray is undefined";
  return false;
}

///////////////////////////////////////////////////////////////////////////////

OdBinaryGlesParser::OdBinaryGlesParser( OdXmlGlesLoaderIdMapper* pIdMapper )
  : m_funcArray(NULL)
{
  m_pIdMapper = pIdMapper;
}

#if defined(_DEBUG) && !defined(OD_TGS_ENABLED)

static std::map<OdUInt16, OdAnsiString> s_mapKeyPaths;
static const char* keyPath(OdUInt16 key) // return NULL if missing
{
  if (!s_mapKeyPaths.size())
  {
    OdUInt16 key_ = 1;
#   define PATH_ENTRY(path, enKey, func) \
      s_mapKeyPaths[key_++] = path;
#   include "PathEntryDefs.h"
    ODA_ASSERT_ONCE(key_ > 1);
  }

  std::map<OdUInt16, OdAnsiString>::iterator itr = s_mapKeyPaths.find(key);
  if (itr == s_mapKeyPaths.end())
    return NULL;
  const char* pcszPath = itr->second.c_str();
  ODA_ASSERT_ONCE(pcszPath && *pcszPath);
  return pcszPath;
}
#endif

//static TBinaryGlesParserFunc keyFunc(OdUInt16 key) // return NULL if missing
//{
//  static std::map<OdUInt16, TBinaryGlesParserFunc> s_mapFuncs;
//  if (!s_mapFuncs.size())
//  {
//    OdUInt16 key_ = 1;
//#   define PATH_ENTRY(path, enKey, func) \
//      s_mapFuncs[key_++] = func;
//#   include "PathEntryDefs.h"
//    ODA_ASSERT_ONCE(key_ > 1);
//  }
//
//  TBinaryGlesParserFunc funcPath = NULL;
//  std::map<OdUInt16, TBinaryGlesParserFunc>::iterator itr = s_mapFuncs.find(key);
//  if (itr == s_mapFuncs.end())
//    return funcPath;
//  funcPath = itr->second;
//  ODA_ASSERT_ONCE(funcPath);
//  return funcPath;
//}

bool OdBinaryGlesParser::parse(OdUniversalReadFiler* pFiler)
{
  try
  {
    for (OdUInt16 key = pFiler->rdUInt16(); key; key = pFiler->rdUInt16())
    {
      OdEnPathKey pk = OdEnPathKey(key);
#if defined(_DEBUG) && !defined(OD_TGS_ENABLED)
      static OdAnsiString s_asPrevPath;
      const char* pcszPath = keyPath(key);
      ODA_ASSERT_ONCE(pcszPath && *pcszPath);
      s_asPrevPath = pcszPath;
#endif
      //TBinaryGlesParserFunc func = keyFunc(key);
      //if (!func) // || !pcszPath || !*pcszPath)
      //{
      //  m_sError = L"Internal binary format error.";
      //  return false; // continue;
      //}
      //bool bRes = (*func)(this, pFiler, pk);

      bool bRes = false;
#   define PATH_ENTRY(path, enKey, func) \
        case pk_##enKey: bRes = func(this, pFiler, pk); break;
      switch (pk)
      {
#     include "PathEntryDefs.h"
      }
      ODA_ASSERT_ONCE(bRes);
      if (!bRes)
        return false;
    }
  }
  catch( const OdError& )
  {
    return false;
  }

  return true;
}

bool OdBinaryGlesParser::parse(const OdString& sFilePathName, 
                               OdTrVisRendition* pRendition)
{
  m_pLoader = new OdXmlGlesLoader(pRendition, m_pIdMapper);
  m_sError.empty();

  OdUInt32 nVersion = (OdUInt32) pk_Count;
  //if (!nVersion)
  //{
  //  keyPath(0); // fill map
  //  nVersion = s_mapKeyPaths.size();
  //}
  OdUniversalReadFiler filer(sFilePathName, nVersion, "TGS");

  return parse(&filer);
}

// EMSCRIPTEN way
bool OdBinaryGlesParser::parse(OdStreamBuf* pStream, // OdUInt8* buffer, OdUInt32 numBytes,
                               OdTrVisRendition* pRendition,
                               TCodingType type) // = enAuto
{
  m_pLoader = new OdXmlGlesLoader(pRendition,m_pIdMapper);
  m_sError.empty();

  OdUInt32 nVersion = (OdUInt32) pk_Count;
  //if (!nVersion)
  //{
  //  keyPath(0); // fill map
  //  nVersion = s_mapKeyPaths.size();
  //}
  OdUniversalReadFiler filer(NULL, nVersion, "TGS");
  switch (type)
  {
  case enBigEndian:
    filer.setBigEndian(true); break;
  case enLittleEndian:
    filer.setBigEndian(false); break;
  }
  //OdStreamBufPtr pStream = OdFlatMemStream::createNew(buffer, OdUInt64(numBytes));
  //if (!filer.setStream(pStream.get(), nVersion, "TGS"))
  if (!filer.setStream(pStream, nVersion, "TGS"))
    return false;
  return parse(&filer);
}

const OdString& OdBinaryGlesParser::errorMessage() const
{
  return m_sError;
}
