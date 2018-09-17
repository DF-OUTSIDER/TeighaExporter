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
#include "DbImpIdwStream.h"
#include "DbFiler.h"
#include "NextCodeDefs.h"

OdDbImpIdwStream::OdDbImpIdwStream()
{
}

OdDbImpIdwStream::~OdDbImpIdwStream()
{
}

OdResult OdDbImpIdwStream::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  // handle : x499 x49A x4BE of DetailAndSectionViews.dwg

  OdUInt32 ver = pFiler->rdInt32();
  ODA_ASSERT_ONCE(ver == 1);
  if (ver != 1)
    return eMakeMeProxy;
  
  OdUInt32 dataSize = pFiler->rdInt32();
  ODA_ASSERT_ONCE(dataSize);

  m_data.resize(dataSize);
  if (dataSize)
    pFiler->rdBytes(m_data.asArrayPtr(), dataSize);

  return eOk;
}

void OdDbImpIdwStream::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdUInt32 ver = 1;
  pFiler->wrInt32(ver);

  OdUInt32 dataSize = m_data.size();
  ODA_ASSERT_ONCE(dataSize);
  pFiler->wrInt32(dataSize);
  if (dataSize)
    pFiler->wrBytes(m_data.asArrayPtr(), dataSize);
}

OdResult OdDbImpIdwStream::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  // handle : x49A x4BE of DetailAndSectionViews.dxf

  if (!pFiler->atSubclassData(OdDbIdwStream::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdUInt32 ver = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(ver == 1);
  if (ver != 1)
    return eMakeMeProxy;

  NEXT_CODE(90)
  OdUInt32 dataSize = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(dataSize);
  m_data.resize(0);
  //m_data.resize(dataSize);
  if (dataSize)
  {
    OdUInt32 offset = 0,
             step = 127; 
    while (offset < dataSize)
    {
      NEXT_CODE(310) // BinaryChunk
      OdBinaryData chunk;
      pFiler->rdBinaryChunk(chunk);
      m_data.append(chunk);

      OdUInt32 step = chunk.size();
      ODA_ASSERT_ONCE(dataSize >= step);
      offset += step;
    }
    ODA_ASSERT_ONCE(offset == dataSize);
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpIdwStream::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrSubclassMarker(OdDbIdwStream::desc()->name());

  OdUInt32 ver = 1;
  pFiler->wrUInt32(90, ver);

  OdUInt32 dataSize = m_data.size();
  ODA_ASSERT_ONCE(dataSize);
  pFiler->wrUInt32(90, dataSize);
  if (dataSize)
  {
    OdUInt32 offset = 0,
             step = 127; 
    while (offset < dataSize)
    {
      const OdUInt8* pBuf = &m_data.asArrayPtr()[offset];
      if ((offset + step) > dataSize)
        step = dataSize - offset;
      pFiler->wrBinaryChunk(310, pBuf, step);
      offset += step;
    }
    ODA_ASSERT_ONCE(offset == dataSize);
  }
}
