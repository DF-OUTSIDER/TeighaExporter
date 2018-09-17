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


#ifndef _ODRAWBYTEDATA_INCLUDED_
#define _ODRAWBYTEDATA_INCLUDED_

#include "TD_PackPush.h"

#include "OleStorage.h"

/** \details

    <group Other_Classes> 
*/
class OdRawByteData : public OdByteData
{
  const OdUInt8* m_pData;
  OdUInt32       m_nDataLeft;
protected:
  OdRawByteData()
    : m_pData(0)
    , m_nDataLeft(0)
  {
  }
public:
  void init(const OdUInt8* pData, OdUInt32 length)
  {
    m_pData = pData;
    m_nDataLeft = length;
  }

  OdUInt32 bytesLeft() const
  {
    return m_nDataLeft;
  }

  void read(OdUInt32 nBytes, void* buffer)
  {
    if(m_nDataLeft >= nBytes)
    {
      m_nDataLeft -= nBytes;
      ::memcpy(buffer, m_pData, nBytes);
      m_pData += nBytes;
    }
    else
      throw OdError(eEndOfFile);
  }
};

#include "TD_PackPop.h"

#endif // _ODRAWBYTEDATA_INCLUDED_
