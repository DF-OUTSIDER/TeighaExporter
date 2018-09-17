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
#include "ExFileUndoController.h"
#include "FlatMemStream.h"
#include "StaticRxObject.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include "RxObjectImpl.h"


ExFileUndoController::ExFileUndoController()
{
}

void ExFileUndoController::pushData(OdStreamBuf* pStream, OdUInt32 nSize, OdUInt32 opt)
{
  if (m_pStorage.isNull())
  {
    throw OdError(eFileAccessErr);
  }

  OdList<UndoRecord>::iterator pNewRecord = m_records.append();
  pNewRecord->options = opt;
  pNewRecord->blockSize = nSize;

  OdUInt64 curPosition = pStream->tell();
  pStream->copyDataTo(m_pStorage, curPosition, curPosition+nSize);
}

bool ExFileUndoController::hasData() const
{
  return !m_records.empty();
}

OdUInt32 ExFileUndoController::popData(OdStreamBuf* pStream)
{
  if(!hasData())
    throw OdError(eEndOfFile);

  UndoRecord &backRecord = m_records.back();
  OdUInt32 opt = backRecord.options;
  
  OdUInt64 curPosition = m_pStorage->tell();
  m_pStorage->copyDataTo(pStream, curPosition - backRecord.blockSize, curPosition);
  m_pStorage->seek(-(OdInt64)backRecord.blockSize, OdDb::kSeekFromEnd);
  m_pStorage->truncate();
  m_records.pop_back();
  return opt;
}

class ExFileUndoControllerRecord : public OdDbUndoControllerRecord
{
public:
  OdUInt32 m_options;
  OdUInt32 options() const
  {
    return m_options;
  }
};

class ExFileUndoControllerIterator : public OdRxIterator
{
public:
  OdList<ExFileUndoController::UndoRecord>::const_reverse_iterator m_iter;
  OdList<ExFileUndoController::UndoRecord>::const_reverse_iterator m_end;

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
    OdSmartPtr<ExFileUndoControllerRecord> pRec =
      OdRxObjectImpl<ExFileUndoControllerRecord>::createObject();
    pRec->m_options = m_iter->options;
    return pRec.get();
  }
};

OdRxIteratorPtr ExFileUndoController::newRecordStackIterator() const
{
  OdSmartPtr<ExFileUndoControllerIterator> pIter =
    OdRxObjectImpl<ExFileUndoControllerIterator>::createObject();
  pIter->m_iter = m_records.rbegin();
  pIter->m_end = m_records.rend();
  return pIter;
}

void ExFileUndoController::clearData()
{
  m_records.clear();
  if (!m_pStorage.isNull())
  {
    m_pStorage->rewind();
    m_pStorage->truncate();
  }
}

void ExFileUndoController::setStorage(OdStreamBufPtr pStorage)
{
  if (pStorage.isNull())
  {
    throw OdError(eInvalidInput);
  }

  clearData();
  m_pStorage = pStorage;
  clearData();
}
