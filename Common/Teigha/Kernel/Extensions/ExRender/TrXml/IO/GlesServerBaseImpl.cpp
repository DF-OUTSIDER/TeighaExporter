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
// GlesServerBaseImpl.cpp
//

#include "OdaCommon.h"
#include "GlesServerBaseImpl.h"
#include "OdFakeDbStub.h"

OdGlesServerBaseImpl::OdGlesServerBaseImpl(const OdDbBaseDatabase *pDb) // = NULL
  : OdGLES2XmlServerBaseImpl(pDb)
  , m_bSkipShaders(true)
  , m_useFakeDbStub(false)
  , m_bEncodeTextureHex(false)
{
}

void OdGlesServerBaseImpl::setUseFakeDbStub(bool bVal)
{
  m_useFakeDbStub = bVal;
}

void OdGlesServerBaseImpl::setSkipShaders(bool bSkipShaders)
{
  m_bSkipShaders = bSkipShaders;
}

void OdGlesServerBaseImpl::setEncodeTextureHex(bool bValue)
{
  m_bEncodeTextureHex = bValue;
}

OdStringArray OdGlesServerBaseImpl::getRenderHandles() const
{
  OdStringArray handles;

  for (std::map<OdDbStub*, OdAnsiString>::const_iterator itr = m_mapHandles.begin(); 
       itr != m_mapHandles.end(); itr++)
    handles.push_back(itr->second.c_str());
  return handles;
}

void OdGlesServerBaseImpl::DropDbStub(const char* pTagName, OdDbStub* pDbStub)
{
  if (!m_useFakeDbStub || !pDbStub)
    OdGLES2XmlServerBaseImpl::DropDbStub(pTagName, pDbStub);
  else
    ident(pTagName, OdAnsiString().format(PRIX64, ((OdFakeDbStub*) pDbStub)->handle).c_str()); // ident(pTagName, (const char*) pDbStub);

  if (m_useFakeDbStub || !pDbStub)
    return;
  // collect handles of really usable in render entities
  if (m_mapHandles.find(pDbStub) == m_mapHandles.end())
    m_mapHandles[pDbStub] = m_sTmpBuf;
  ODA_ASSERT_ONCE(m_mapHandles[pDbStub] == m_sTmpBuf);
}

void OdGlesServerBaseImpl::DropBinaryStream(const char* pTagName, const OdUInt8* pData, OdUInt32 nData)
{
  if (m_bEncodeTextureHex)
    OdGLES2XmlServerBaseImpl::DropBinaryStream(pTagName, pData, nData);
  else
  {
    encodeToBase64(pData, nData, m_sTmpBuf);
    ident(pTagName, m_sTmpBuf.c_str());
  }
}
