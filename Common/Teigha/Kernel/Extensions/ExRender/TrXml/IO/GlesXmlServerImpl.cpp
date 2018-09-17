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
// GlesXmlServerImpl.cpp
//

#include "OdaCommon.h"
#include "GlesXmlServerImpl.h"

OdGlesXmlServerImpl::OdGlesXmlServerImpl(const OdDbBaseDatabase *pDb) // = NULL
  : OdGlesServerBaseImpl(pDb)
  , m_indexNextFree(0)
{
}

void OdGlesXmlServerImpl::setOutput(OdStreamBuf* buf)
{
  m_pOutStream = buf;
}

// set output path name or format(if %d is present in path)
bool OdGlesXmlServerImpl::setOutPathName(const OdString& sPathNameFormat,
                                         OdUInt64 limitToSplit, // = OdUInt64() // if > OdUInt64() then willbe splited by next OnStateChanged
                                         int indexNextFree) // = 0
{
  m_indexNextFree = indexNextFree;
  OdString sPathName = sPathNameFormat;
  if (sPathNameFormat.find(L'%') >= 0)
    sPathName.format(sPathNameFormat.c_str(), m_indexNextFree++);
  m_pOutStream = ::odrxSystemServices()->createFile(sPathName.c_str(), Oda::kFileWrite, 
                                                    Oda::kShareDenyWrite, Oda::kCreateAlways);
  return !m_pOutStream.isNull();
}

int OdGlesXmlServerImpl::flushOut() // return next free index
{
  m_pOutStream = NULL;
  return m_indexNextFree;
}

void OdGlesXmlServerImpl::ident(const char* pTag, const char* pText)
{
  ODA_ASSERT_ONCE(m_pOutStream.get());
  for (int n = 0; n < m_nIdent; n++)
    m_pOutStream->putByte((OdUInt8(' ')));
  if (pTag)
  {
    m_pOutStream->putByte((OdUInt8('<')));
    m_pOutStream->putBytes(pTag, (OdUInt32)odStrLenA(pTag));
    m_pOutStream->putByte((OdUInt8('>')));
  }
  if (pText)
    m_pOutStream->putBytes(pText, (OdUInt32)odStrLenA(pText));
  if (pTag)
  {
    m_pOutStream->putBytes("</", 2);
    m_pOutStream->putBytes(pTag, (OdUInt32)odStrLenA(pTag));
    m_pOutStream->putByte((OdUInt8('>')));
  }
  m_pOutStream->putBytes("\r\n", 2);
}
