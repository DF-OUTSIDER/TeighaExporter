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
// GlesBinaryServerImpl.cpp
//

#include "OdaCommon.h"
#include "GlesBinaryServerImpl.h"
#include "UnivWriteFiler.h"

OdGlesBinaryServerImpl::OdGlesBinaryServerImpl(const OdDbBaseDatabase *pDb) // = NULL
  : OdGlesServerBaseImpl(pDb)
  , m_indexNextFree(0)
  , m_state(kUndefState)
{
}

OdGlesBinaryServerImpl::~OdGlesBinaryServerImpl()
{
  flushOut();
}

static std::map<OdAnsiString, OdUInt16> s_mapPathKeys;

static OdUInt16 pathKey(const OdAnsiString& asPath) // return 0 if missing
{
  OdUInt16 keyPath = 0; // means missing

  if (!s_mapPathKeys.size())
  {
    keyPath = 1;
#   define PATH_ENTRY(path, enKey, func) \
      s_mapPathKeys["GsUpdate/" path] = keyPath++;
#   include "PathEntryDefs.h"
    ODA_ASSERT_ONCE(keyPath > 1);
    keyPath = 0; // means missing
  }

  std::map<OdAnsiString, OdUInt16>::iterator itr = s_mapPathKeys.find(asPath);
  if (itr == s_mapPathKeys.end())
    return keyPath;
  keyPath = itr->second;
  ODA_ASSERT_ONCE(keyPath);
  return keyPath;
}

void OdGlesBinaryServerImpl::setOutput(OdStreamBuf * buf)
{
  flushOut();

  m_sPathNameFormat.empty();
  m_limitToSplit = OdUInt64();
  m_indexNextFree = 0;

  if (!buf)
    return;
  m_sPathNameFormat = buf->fileName();
  OdUInt32 nVersion = (OdUInt32)s_mapPathKeys.size();
  if (!nVersion)
  {
    pathKey(""); // fill map
    nVersion = (OdUInt32)s_mapPathKeys.size();
  }
  ODA_ASSERT_ONCE(m_pFiler.isNull()); // should be cleared via flushOut()
  m_pFiler = new OdUniversalWriteFiler(buf, nVersion, "TGS");
  m_state = kUndefState;
}

// set output path name or format(if %d is present in path)
bool OdGlesBinaryServerImpl::setOutPathName(const OdString& sPathNameFormat,
                                            OdUInt64 limitToSplit, // = OdUInt64() // if > OdUInt64() then willbe splited by next OnStateChanged
                                            int indexNextFree) // = 0
{
  if (!sPathNameFormat.isEmpty())
    m_sPathNameFormat = sPathNameFormat;
  if (limitToSplit != OdUInt64(-1))
    m_limitToSplit = limitToSplit;
  if (indexNextFree >= 0)
    m_indexNextFree = indexNextFree;
  OdString sPathName = m_sPathNameFormat;
  if (m_sPathNameFormat.find(L'%') >= 0)
    sPathName.format(m_sPathNameFormat.c_str(), m_indexNextFree++);

  OdUInt32 nVersion = (OdUInt32)s_mapPathKeys.size();
  if (!nVersion)
  {
    pathKey(""); // fill map
    nVersion = (OdUInt32)s_mapPathKeys.size();
  }
  ODA_ASSERT_ONCE(m_pFiler.isNull()); // should be cleared via flushOut()
  if (m_pFiler.get())
    m_pFiler->wrUInt16(0); // termination path key
  m_pFiler = new OdUniversalWriteFiler(sPathName, nVersion, "TGS");
  m_state = kUndefState;

  return !m_pFiler.isNull();
}

int OdGlesBinaryServerImpl::flushOut() // return next free index
{
  if (m_pFiler.get())
    m_pFiler->wrUInt16(0); // termination path key
  m_pFiler = NULL;
  return m_indexNextFree;
}

void OdGlesBinaryServerImpl::ident(const char* pTag, const char* pText)
{
  ODA_FAIL_ONCE(); // should be unusable for binary
}

void OdGlesBinaryServerImpl::IncreaseNesting(const char* pLevelName)
{
  OdAnsiString asPath;
  if (m_stackPaths.size())
    asPath = m_stackPaths.last() + '/';
  asPath += pLevelName;
  m_stackPaths.push_back(asPath);
  OdUInt32 key = pathKey(asPath);
  if (!key) 
    return;
  m_pFiler->wrUInt16(key);
  m_pFiler->wrBool(true); // isBegin at reading
}

void OdGlesBinaryServerImpl::DecreaseNesting(const char*) // pLevelName
{
  ODA_ASSERT_ONCE(m_stackPaths.size());
  if (!m_stackPaths.size())
    return;
  OdAnsiString asPath = m_stackPaths.last();
  m_stackPaths.removeLast();
  OdUInt32 key = pathKey(asPath);
  if (!key) 
    return;
  m_pFiler->wrUInt16(key);
  m_pFiler->wrBool(false); // isBegin at reading
}

bool OdGlesBinaryServerImpl::wrPathKey(const char* pTagName)
{
  ODA_ASSERT_ONCE(m_pFiler.get() && m_stackPaths.size() && pTagName);
  OdAnsiString asPath = m_stackPaths.last() + "/" + pTagName;
  OdUInt32 key = pathKey(asPath);
  if (!key) 
    return false;
  m_pFiler->wrUInt16(key);
  return true;
}

void OdGlesBinaryServerImpl::DropId(const char* pTagName, OdUInt64 data, 
                                    OdTrVisUniqueID* pId, // = NULL
                                    OdInt32 enAdv) // = -1
{
  if (!wrPathKey(pTagName))
    return;

  if (pId && data > OdUInt64(100)) // is not an address
  {
    m_sTmpBuf = pId->transform(*(dynamic_cast<OdTrVisUniqueID::DefUIDTransformer*>(this)));
    m_pFiler->wrAnsiString(m_sTmpBuf);
  }
  else if (   data == OdUInt64() // special
           && (   !odStrCmpA(pTagName, "TextureID")
               || !odStrCmpA(pTagName, "MaterialID")))
  {
    m_pFiler->wrAnsiString("0");
  }
  else
  {
    ODA_ASSERT_ONCE(data <= OdUInt64(100));

    // will be reading by func_StaticId
    ODA_ASSERT_ONCE(   !odStrCmpA(pTagName, "ShaderID")
                    || !odStrCmpA(pTagName, "ShaderId")
                    || !odStrCmpA(pTagName, "ProgramID")
                    || !odStrCmpA(pTagName, "VertexShaderID")
                    || !odStrCmpA(pTagName, "FragmentShaderID")
                    || !odStrCmpA(pTagName, "BaseProgramID"));

    m_pFiler->wrUInt8((OdUInt8)(data & 0xff));
  }
}

void OdGlesBinaryServerImpl::DropBoolean(const char* pTagName, bool data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrBool(data);
}

void OdGlesBinaryServerImpl::DropUInt8(const char* pTagName, OdUInt8 data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt8(data);
}

void OdGlesBinaryServerImpl::DropUInt16(const char* pTagName, OdUInt16 data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt16(data);
}

void OdGlesBinaryServerImpl::DropUInt32(const char* pTagName, OdUInt32 data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(data);
}

void OdGlesBinaryServerImpl::DropUInt64(const char* pTagName, OdUInt64 data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt64(data);
}

void OdGlesBinaryServerImpl::DropInt16(const char* pTagName, OdInt16 data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrInt16(data);
}

void OdGlesBinaryServerImpl::DropInt32(const char* pTagName, OdInt32 data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrInt32(data);
}

void OdGlesBinaryServerImpl::DropChars(const char* pTagName, const char* pStr)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrAnsiString(pStr);
}

void OdGlesBinaryServerImpl::DropInts(const char* pTagName, OdUInt32 nData, const OdUInt16* pData)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(nData);
  for (OdUInt32 idx = 0; idx < nData; idx++)
    m_pFiler->wrUInt16(pData[idx]);
}

void OdGlesBinaryServerImpl::DropFloats(const char* pTagName, OdUInt32 nData, const float *pData)
{
  ODA_ASSERT_ONCE(nData && !(nData % 3) && pData); // test
  if (  odStrCmpA(pTagName, "ArrayData")
      || !nData || (nData % 3)
      || (odabs(pData[0]) < 10000 && odabs(pData[1]) < 10000 && odabs(pData[2]) < 10000))
  {
    ODA_ASSERT_ONCE(nData && pData);

    if (!wrPathKey(pTagName))
      return;
    m_pFiler->wrUInt32(nData);
    for (OdUInt32 idx = 0; idx < nData; idx++)
      m_pFiler->wrFloat(pData[idx]);

    return;
  }

  float offs[3];
  #if !defined(__BORLANDC__) && !defined(__hpux)
  offs[0] = ceilf(pData[0]) - 1.0f; offs[1] = ceilf(pData[1]) - 1.0f; offs[2] = ceilf(pData[2]) - 1.0f; // by first
  #else
  // float-mathematics is missing in Builder
  offs[0] = pData[0]; offs[1] = pData[1]; offs[2] = pData[2];
  #endif

  DropFloat3("ArrayOffset", offs[0], offs[1], offs[2]);

  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(nData);
  for (OdUInt32 idx = 0; idx < nData; idx++)
    m_pFiler->wrFloat(pData[idx] - offs[idx % 3]);
}

void OdGlesBinaryServerImpl::DropFloat(const char* pTagName, float data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrFloat(data);
}

void OdGlesBinaryServerImpl::DropFloat2(const char* pTagName, float data1, float data2)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(2);
  m_pFiler->wrFloat(data1); m_pFiler->wrFloat(data2);
}

void OdGlesBinaryServerImpl::DropFloat3(const char* pTagName, float data1, float data2, float data3)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(3);
  m_pFiler->wrFloat(data1); m_pFiler->wrFloat(data2); m_pFiler->wrFloat(data3);
}

void OdGlesBinaryServerImpl::DropFloat4(const char* pTagName, float data1, float data2, float data3, float data4)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(4);
  m_pFiler->wrFloat(data1); m_pFiler->wrFloat(data2); m_pFiler->wrFloat(data3); m_pFiler->wrFloat(data4);
}

void OdGlesBinaryServerImpl::DropDouble(const char* pTagName, double data) 
{ 
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrDouble(data); 
}

void OdGlesBinaryServerImpl::DropDouble2(const char* pTagName, double data1, double data2)
{ 
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(2);
  m_pFiler->wrDouble(data1); m_pFiler->wrDouble(data2); 
}

void OdGlesBinaryServerImpl::DropDouble3(const char* pTagName, double data1, double data2, double data3)
{ 
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(3);
  m_pFiler->wrDouble(data1); m_pFiler->wrDouble(data2); m_pFiler->wrDouble(data3); 
}

void OdGlesBinaryServerImpl::DropDouble4(const char* pTagName, double data1, double data2, double data3, double data4)
{ 
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(4);
  m_pFiler->wrDouble(data1); m_pFiler->wrDouble(data2); m_pFiler->wrDouble(data3); m_pFiler->wrDouble(data4);  
}

void OdGlesBinaryServerImpl::DropUInt8RGB(const char* pTagName, OdUInt8 data1, OdUInt8 data2, OdUInt8 data3, OdUInt8 data4)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt8(data1); m_pFiler->wrUInt8(data2); m_pFiler->wrUInt8(data3); m_pFiler->wrUInt8(data4);
}

void OdGlesBinaryServerImpl::DropMatrix(const char* pTagName, const OdGeMatrix3d &data)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrMatrix3d(data);
}

void OdGlesBinaryServerImpl::DropBinaryStream(const char* pTagName, const OdUInt8 * pData, OdUInt32 nData)
{
  if (!wrPathKey(pTagName))
    return;
  m_pFiler->wrUInt32(nData);
  m_pFiler->wrRawData(pData, nData);
}

void OdGlesBinaryServerImpl::DropDbStub(const char *pTagName, OdDbStub *pDbStub)
{
  // possible via paper object for example //ODA_ASSERT_ONCE(pDbStub);
  if (!pDbStub)
    return;
  if (!wrPathKey(pTagName))
    return;
  m_sTmpBuf.empty();

  if (m_useFakeDbStub)
  {
    m_pFiler->wrAnsiString((const char*) pDbStub);
    return;
  }

  append(m_sTmpBuf, pDbStub);
  m_pFiler->wrAnsiString(m_sTmpBuf);
  // collect handles of really usable in render entities
  m_mapHandles[pDbStub] = m_sTmpBuf;
}

bool OdGlesBinaryServerImpl::UpdateClientDependentSectionOnly() const
{
  return false;
}

void OdGlesBinaryServerImpl::OnStateChanged(OutputState newState)
{
  OutputState prevState = m_state;
  m_state = newState;
  if (m_limitToSplit == OdUInt64())
    return;
  ODA_ASSERT_ONCE(m_pFiler.get() && m_pFiler->stream().get());

// TODO split via limit
//  m_pFiler->stream()->length()
//bool OdGlesBinaryServerImpl::setOutPathName(const OdString& sPathNameFormat,
//                                            OdUInt64 limitToSplit, // = OdUInt64() // if > OdUInt64() then willbe splited by next OnStateChanged
//                                            int indexNextFree) // = 0
}
