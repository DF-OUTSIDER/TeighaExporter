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

#include "StdAfx.h"
#include "ExUndoController.h"
#include "FlatMemStream.h"
#include "StaticRxObject.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include "RxObjectImpl.h"


ExUndoController::ExUndoController()
  : m_nMaxSteps(0xFFFFFFFF)
  , m_nMaxMemory(0x01000000)
{
  m_nMemoryUsed = sizeof(ExUndoController);
}

void ExUndoController::setLimits(OdUInt32 nMaxSteps, OdUInt32 nMaxMemory)
{
  m_nMaxSteps  = nMaxSteps;
  m_nMaxMemory = nMaxMemory;
  freeExtra();
}

OdUInt32 ExUndoController::recordMemory(OdUInt32 nDataSize)
{
  return sizeof(OdUInt8Array) + sizeof(OdArrayBuffer) + sizeof(void*) * 2 + nDataSize;
}

OdUInt32 ExUndoController::frontRecordMemory() const
{
  return recordMemory(m_records.front().size());
}

OdUInt32 ExUndoController::backRecordMemory() const
{
  return recordMemory(m_records.back().size());
}

void ExUndoController::freeFrontRecord()
{
  ODA_ASSERT(m_records.size());
  m_nMemoryUsed -= frontRecordMemory();
  m_records.pop_front();
}

void ExUndoController::freeBackRecord()
{
  ODA_ASSERT(m_records.size());
  m_nMemoryUsed -= backRecordMemory();
  m_records.pop_back();
}

void ExUndoController::freeExtra()
{
  while(m_records.size())
  {
    if(m_nMemoryUsed <= m_nMaxMemory && m_records.size() <= m_nMaxSteps)
      return ;
    freeFrontRecord();
  }
}

bool ExUndoController::pushRecord(OdUInt32 nSizeOfRecToAppend)
{
  if(m_records.size() + 1 <= m_nMaxSteps && m_nMemoryUsed + nSizeOfRecToAppend <= m_nMaxMemory)
  {
    OdUInt8Array data(nSizeOfRecToAppend);
    data.resize(nSizeOfRecToAppend);
    m_records.push_back(data);
    m_nMemoryUsed += backRecordMemory();
    return true;
  }

  while(m_records.size())
  {
    if(m_nMemoryUsed + nSizeOfRecToAppend - frontRecordMemory() <= m_nMaxMemory)
    {
      OdUInt8Array data = m_records.front();
      m_records.pop_front();
      m_nMemoryUsed -= data.size();
      data.setPhysicalLength(nSizeOfRecToAppend);
      data.resize(nSizeOfRecToAppend);
      m_records.push_back(data);
      m_nMemoryUsed += nSizeOfRecToAppend;
      return true;
    }
    freeFrontRecord();
  }
  return false;
}

void ExUndoController::pushData(OdStreamBuf* pStream, OdUInt32 nSize, OdUInt32 opt)
{
  if(pushRecord(nSize + sizeof(OdUInt32)))
  {
    OdStaticRxObject<OdFlatMemStream> ms;
    ms.init(m_records.back().asArrayPtr(), nSize + sizeof(OdUInt32));
    ms.putBytes(&opt, sizeof(opt));
    pStream->copyDataTo(&ms, pStream->tell(), pStream->tell()+nSize);
  }
}

bool ExUndoController::hasData() const
{
  return !m_records.empty();
}

OdUInt32 ExUndoController::popData(OdStreamBuf* pStream)
{
  if(!hasData())
    throw OdError(eEndOfFile);
  OdUInt32 nSize = m_records.back().size();
  OdStaticRxObject<OdFlatMemStream> ms;
  ms.init(const_cast<OdUInt8*>(m_records.back().getPtr()), nSize);
  OdUInt32 opt;
  ms.getBytes(&opt, sizeof(opt));
  ms.copyDataTo(pStream, ms.tell(), nSize);
  freeBackRecord();
  return opt;
}

class ExUndoControllerRecord : public OdDbUndoControllerRecord
{
public:
  OdUInt32 m_options;
  OdUInt32 options() const
  {
    return m_options;
  }
};

class ExUndoControllerIterator : public OdRxIterator
{
public:
  OdList<OdUInt8Array>::const_reverse_iterator m_iter;
  OdList<OdUInt8Array>::const_reverse_iterator m_end;

  bool done() const
  {
    return (m_iter==m_end);
  }
  bool next()
  {
    if(done())
      return false;
    ++m_iter;
    return !done();
  }
  OdRxObjectPtr object() const
  {
    if(done())
      throw OdError(eIteratorDone);
    OdSmartPtr<ExUndoControllerRecord> pRec =
      OdRxObjectImpl<ExUndoControllerRecord>::createObject();
    for (unsigned i = 0; i < sizeof(pRec->m_options); ++i)
      ((OdUInt8*)&pRec->m_options)[i] = m_iter->at(i);
    return pRec.get();
  }
};

OdRxIteratorPtr ExUndoController::newRecordStackIterator() const
{
  OdSmartPtr<ExUndoControllerIterator> pIter =
    OdRxObjectImpl<ExUndoControllerIterator>::createObject();
  pIter->m_iter = m_records.rbegin();
  pIter->m_end = m_records.rend();
  return pIter;
}

void ExUndoController::clearData()
{
  m_records.clear();
  m_nMemoryUsed = 0;
}
