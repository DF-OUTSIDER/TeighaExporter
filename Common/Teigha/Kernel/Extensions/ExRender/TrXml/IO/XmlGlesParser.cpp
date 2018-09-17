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
// XmlGlesParser.cpp

#include "OdaCommon.h"
#include "XmlGlesParser.h"
#include "XmlGlesLoader.h"
#include "RxDictionary.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#include "tinyxml.h"
#include "CloudTools.h"

///////////////////////////////////////////////////////////////////////////////

static bool func_Level(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath) // level 0 & 1
{
  pThis->m_pLoader->DropBegin(pk);
  if (!pThis->parse(elem, asPath))
    return false;
  pThis->m_pLoader->DropEnd(pk);
  return true;
}

static bool func_Id(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdAnsiString asValue = elem->GetText();
  if (pThis->m_pLoader->DropId(pk, asValue))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropId";
  return false;
}

static bool func_DbStub(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdAnsiString asHandle = elem->GetText();
  if (pThis->m_pLoader->DropDbStub(pk, asHandle))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropDbStub";
  return false;
}

static bool func_Chars(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdAnsiString asValue = elem->GetText();
  asValue.replace("&#x0A;", "\n");
  asValue.replace("&gt;", ">");
  asValue.replace("&lt;", "<");
  if (pThis->m_pLoader->DropChars(pk, asValue))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropChars";
  return false;
}

static bool func_BinaryStream(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdAnsiString asValue = elem->GetText();
  OdBinaryData data;
  bool isBase64 = (asValue.findOneOf("GHIJKLMNOPQRSTUVWXYZghijklmnopqrstuvwxyz0123456789+/") >= 0);
  if (isBase64)
    decodeBase64(asValue, data);
  else
  {
    //decodeHex(asValue, data);
    OdUInt32 len = asValue.getLength();
    ODA_ASSERT_ONCE(!(len % 2));
    len = len / 2;
    const char* pHex = asValue.c_str();
//#ifndef EMCC
    OdChar buf[3];
//#else
//    char buf[3];
//#endif
    buf[2] = 0;
    OdUInt32 val;
    data.resize(len);
    for (OdUInt32 idx = 0; idx < len; idx++)
    {
      buf[0] = *pHex++;
      buf[1] = *pHex++;
//#ifndef EMCC
      odSScanf(buf, L"%02X", &val);
//#else
//      sscanf(buf, "%02X", &val);
//#endif
      ODA_ASSERT_ONCE(!(val & 0xFF));
      data[idx] = OdUInt8(val);
    }
  }
  if (pThis->m_pLoader->DropBinaryStream(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropChars";
  return false;
}

static bool func_Boolean(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdAnsiString asValue = elem->GetText();
  ODA_ASSERT_ONCE(asValue == "0" || asValue == "1");
  bool bVal = (asValue != "0"); 
  if (pThis->m_pLoader->DropBoolean(pk, bVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropBoolean";
  return false;
}

static bool func_UInt8RGB(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdBinaryData data;
  OdString sValue = elem->GetText();
  OdStringArray lst = OdString_split(sValue, L',');
  for (int idx = 0; idx < (int) lst.size(); idx++)
  {
    int iVal = odStrToInt((sValue = lst.getAt(idx)).c_str()); 
    ODA_ASSERT_ONCE(iVal >= 0 && iVal <= 255);
    data.push_back(OdUInt8(iVal));
  }
  if (pThis->m_pLoader->DropUInt8RGB(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt8RGB";
  return false;
}

static bool func_UInt8(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  int iVal = odStrToInt(sValue.c_str()); 
  ODA_ASSERT_ONCE(iVal >= 0 && iVal <= 255);
  if (pThis->m_pLoader->DropUInt8(pk, OdUInt8(iVal)))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt8";
  return false;
}

static bool func_UInt16(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  int iVal = odStrToInt(sValue.c_str()); 
  ODA_ASSERT_ONCE(iVal >= 0 && iVal <= 0xffff);
  if (pThis->m_pLoader->DropUInt16(pk, OdUInt8(iVal)))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt16";
  return false;
}

static bool func_UInt32(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  OdUInt32 iVal = odStrToUInt(sValue.c_str()); 
  if (pThis->m_pLoader->DropUInt32(pk, iVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt32";
  return false;
}

static bool func_UInts16(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdArray<OdUInt16, OdMemoryAllocator<OdUInt16> > data;
  OdAnsiString asValue = elem->GetText();
  char* pszBuf = const_cast<char*>(asValue.c_str());
  const char* pszVal = NULL;
  while ((pszVal = CharBufSplit_next(pszBuf)) != NULL)
  {
    OdUInt32 iVal = odStrToInt(OdString(pszVal).c_str()); 
    data.push_back(iVal);
  }
  if (pThis->m_pLoader->DropUInts16(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInts16";
  return false;
}

static bool func_UInt64(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdUInt64 val = 0;
//#ifndef EMCC
  OdString sValue = elem->GetText();
  odSScanf(sValue, PRIu64W, &val);
//#else
//  const char* pcszValue = elem->GetText();
//  sscanf(pcszValue, PRIu64, &val);
//#endif
  if (pThis->m_pLoader->DropUInt64(pk, val))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropUInt64";
  return false;
}

static bool func_Int16(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  OdInt32 iVal = odStrToInt(sValue.c_str()); 
  ODA_ASSERT_ONCE(SHRT_MIN <= iVal && iVal <= SHRT_MAX);
  if (pThis->m_pLoader->DropInt16(pk, (OdInt16) iVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropInt16";
  return false;
}

static bool func_Int32(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  OdInt32 iVal = odStrToInt(sValue.c_str()); 
  if (pThis->m_pLoader->DropInt32(pk, iVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropInt32";
  return false;
}

static bool func_Ints(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdIntArray data;
  OdAnsiString asValue = elem->GetText();
  char* pszBuf = const_cast<char*>(asValue.c_str());
  const char* pszVal = NULL;
  while ((pszVal = CharBufSplit_next(pszBuf)) != NULL)
  {
    OdUInt32 iVal = odStrToInt(OdString(pszVal).c_str()); 
    data.push_back(iVal);
  }
  if (pThis->m_pLoader->DropInts(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropInts";
  return false;
}

static bool func_Float(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  double dVal = odStrToD(sValue.c_str()); 
  ODA_ASSERT_ONCE(dVal >= -FLT_MAX && dVal <= FLT_MAX);
  if (pThis->m_pLoader->DropFloat(pk, (float) dVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropFloat";
  return false;
}

static bool func_Floats(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdArray<float, OdMemoryAllocator<float> > data;
  OdAnsiString asValue = elem->GetText();
  char* pszBuf = const_cast<char*>(asValue.c_str());
  const char* pszVal = NULL;
  while ((pszVal = CharBufSplit_next(pszBuf)) != NULL)
  {
    double dVal = odStrToD(pszVal); 
    ODA_ASSERT_ONCE(dVal >= -FLT_MAX && dVal <= FLT_MAX);
    data.push_back(float(dVal));
  }
  if (pThis->m_pLoader->DropFloats(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropFloats";
  return false;
}

static bool func_Double(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  double dVal = odStrToD(sValue.c_str()); 
  ODA_ASSERT_ONCE(dVal >= -FLT_MAX && dVal <= FLT_MAX);
  if (pThis->m_pLoader->DropDouble(pk, dVal))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropDouble";
  return false;
}

static bool func_Doubles(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdArray<double, OdMemoryAllocator<double> > data;
  OdAnsiString asValue = elem->GetText();
  char* pszBuf = const_cast<char*>(asValue.c_str());
  const char* pszVal = NULL;
  while ((pszVal = CharBufSplit_next(pszBuf)) != NULL)
  {
    double dVal = odStrToD(pszVal); 
    ODA_ASSERT_ONCE(dVal >= -FLT_MAX && dVal <= FLT_MAX);
    data.push_back(dVal);
  }
  if (pThis->m_pLoader->DropDoubles(pk, data))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropDoubles";
  return false;
}

static bool func_Matrix(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  OdStringArray lst = OdString_split(sValue, L',');
  OdGeMatrix3d data;
  OdArray<float, OdMemoryAllocator<float> > dataFloats;
  int idx = 0;
  for (; idx < (int) lst.size(); idx++)
  {
    double dVal = odStrToD((sValue = lst.getAt(idx)).c_str()); 
    ODA_ASSERT_ONCE(dVal >= -FLT_MAX && dVal <= FLT_MAX);
    data.entry[idx / 4][idx % 4] = dVal;
    dataFloats.push_back((float) dVal);
  }
  if (idx != 16)
  {
    ODA_FAIL_ONCE();
    pThis->m_sError = L"invalid enty count of matrix data";
    return false;
  }
  if (pThis->m_pLoader->DropMatrix(pk, data, &dataFloats))
    return true;
  pThis->m_sError = L"error inside OdXmlGlesLoader::DropMatrix";
  return false;
}

static bool func_UInt32ArrayType(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  OdString sValue = elem->GetText();
  OdUInt32 iVal = odStrToInt(sValue.c_str()); 
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
  case OdTrVisArrayWrapper::Type_Depth:
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

static bool func_Array(OdXmlGlesParser* pThis, TiXmlElement* elem, OdEnPathKey pk, const OdAnsiString& asPath)
{
  if (pThis->m_funcArray)
    return (*pThis->m_funcArray)(pThis, elem, pk, asPath);

  pThis->m_sError = L"func_Array error: m_funcArray is undefined";
  return false;
}

///////////////////////////////////////////////////////////////////////////////

OdXmlGlesParser::OdXmlGlesParser( OdXmlGlesLoaderIdMapper* pIdMapper )
  : m_funcArray(NULL)
{
  m_pIdMapper = pIdMapper;
}

//#ifdef EMCC // compiler to javascript
//bool OdXmlGlesParser::parseString(const char* pcszXmlContent, 
//                                  OdTrVisRendition* pRendition)
//{
//  m_pLoader = new OdXmlGlesLoader(pRendition);
//  m_sError.empty();
//
//  TiXmlDocument doc;
//  doc.Parse(pcszXmlContent, 0, TIXML_ENCODING_UTF8);
//  if (doc.Error())
//  {
//    m_sError = OdString_toUnicode(doc.ErrorDesc());
//    return false;
//  }
//
//  TiXmlElement* elemRoot = doc.FirstChildElement();
//  ODA_ASSERT_ONCE(elemRoot);
//  if (!elemRoot) 
//    return false;
//  OdString sName = elemRoot->Value();
//  if (sName != L"GsUpdate")
//  {
//    ODA_FAIL_ONCE();
//    m_sError.format(L"Unknown root tag name \"%ls\".", sName.c_str()); 
//    return false;
//  }
//
//  return parse(elemRoot, ""); // sName
//}
//#endif

bool OdXmlGlesParser::parse(const OdString& sXmlFilePathName, 
                            OdTrVisRendition* pRendition)
{
  m_pLoader = new OdXmlGlesLoader(pRendition, m_pIdMapper);
  m_sError.empty();

  TiXmlDocument doc;
  if (!doc.LoadFile(OdString_toUtf8(sXmlFilePathName)))
  {
    m_sError = OdString_toUnicode(doc.ErrorDesc());
    return false;
  }

  TiXmlElement* elemRoot = doc.FirstChildElement();
  ODA_ASSERT_ONCE(elemRoot);
  if (!elemRoot) 
    return false;
  bool bRet = false;
  while( elemRoot )
  {
    OdString sName = elemRoot->Value();
    if (sName != L"GsUpdate")
    {
      ODA_FAIL_ONCE();
      m_sError.format(L"Unknown root tag name \"%ls\".", sName.c_str()); 
      return false;
    }

    try
    {
      bRet = parse(elemRoot, ""); // sName
    }
    catch( const OdError& )
    {
      return false;
    }

    elemRoot = elemRoot->NextSiblingElement();
  }

  return bRet;
}

static OdEnPathKey pathKey(const OdAnsiString& asPath) // return 0 if missing
{
  OdUInt16 keyPath = 0; // means missing

  static std::map<OdAnsiString, OdUInt16> s_mapPathKeys;
  if (!s_mapPathKeys.size())
  {
    keyPath = 1;
#   define PATH_ENTRY(path, enKey, func) \
      s_mapPathKeys[path] = keyPath++;
#   include "PathEntryDefs.h"
    ODA_ASSERT_ONCE(keyPath > 1);
    keyPath = 0; // means missing
  }

  std::map<OdAnsiString, OdUInt16>::iterator itr = s_mapPathKeys.find(asPath);
  if (itr == s_mapPathKeys.end())
    return pk_None;
  keyPath = itr->second;
  ODA_ASSERT_ONCE(keyPath);
  return (OdEnPathKey) keyPath;
}

static TXmlGlesParserFunc pathFunc(const OdAnsiString& asPath) // return NULL if missing
{
  static std::map<OdAnsiString, TXmlGlesParserFunc> s_mapPathFuncs;
  if (!s_mapPathFuncs.size())
  {
#   define PATH_ENTRY(path, enKey, func) \
      s_mapPathFuncs[path] = func;
#   include "PathEntryDefs.h"
  }

  TXmlGlesParserFunc funcPath = NULL;
  std::map<OdAnsiString, TXmlGlesParserFunc>::iterator itr = s_mapPathFuncs.find(asPath);
  if (itr == s_mapPathFuncs.end())
    return funcPath;
  funcPath = itr->second;
  ODA_ASSERT_ONCE(funcPath);
  return funcPath;
}

bool OdXmlGlesParser::parse(TiXmlElement* elem, const OdAnsiString& asPath)
{
  for (elem = elem->FirstChildElement(); elem; elem = elem->NextSiblingElement())
  {
    // Unfortunately we can have different type of tags with same name.
    // Therefore we use path as key for this map.
    OdAnsiString asSubPath = asPath + (asPath.isEmpty() ? "" : "/") + elem->Value();

    if( asPath == L"/GsUpdate" )
    {
      bool stop = true;
    }

    OdEnPathKey pk = pathKey(asSubPath);
    TXmlGlesParserFunc funcPath = pathFunc(asSubPath);
    if (!pk || !funcPath)
    {
      //ODA_FAIL_ONCE(); // TODO
      //m_sError.format(L"Tag \"%ls\" is unsupported yet.", OdString_toUnicode(asSubPath).c_str());
      //return false;
      ODA_ASSERT(("Ignoring unexpected tag", 0));
      continue;
    }

    if (!(*funcPath)(this, elem, pk, asSubPath))
      return false;
  }

  return true;
}

const OdString& OdXmlGlesParser::errorMessage() const
{
  return m_sError;
}
