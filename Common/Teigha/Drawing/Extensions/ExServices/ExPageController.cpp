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
#include "ExPageController.h"
#include "DbSystemServices.h"
#include "DbHostAppServices.h"
#include "DbDatabase.h"
#include "OdString.h"
#include "FlatMemStream.h"

//----------------------------------------------------------
//
// ExUnloadController
//
//----------------------------------------------------------
ExUnloadController::ExUnloadController()
  : m_pDb(0)
{
}

OdStreamBufPtr ExUnloadController::read(Key)
{
  ODA_FAIL(); return (OdStreamBuf*)0;
}

bool ExUnloadController::write(Key&, OdStreamBuf*)
{
  ODA_FAIL(); return false;
}

void ExUnloadController::setDatabase(OdDbDatabase* pDb) 
{
  m_pDb = pDb;
}

OdDbDatabase* ExUnloadController::database() 
{
  return m_pDb; 
}

int ExUnloadController::pagingType() const
{
  return OdDb::kUnload;
}



class MyPageStream : public OdStreamBuf
{
  OdFlatMemStreamPtr m_pMemStream;
  OdBinaryData          m_buff;

public:
  bool init(OdStreamBufPtr pBuf)
  {
    OdUInt32 len;
    pBuf->getBytes(&len, sizeof(OdUInt32));
    
    m_buff.resize(len);
    pBuf->getBytes(m_buff.asArrayPtr(), len);

    m_pMemStream = OdFlatMemStream::createNew(m_buff.asArrayPtr(), len);
    return true;
  }

  OdUInt8 getByte()
  {
    return m_pMemStream->getByte();
  }

  void getBytes(void* buffer, OdUInt32 nLen)
  {
    m_pMemStream->getBytes(buffer, nLen);
  }

  OdUInt64 seek(OdInt64 offset, OdDb::FilerSeekType whence)
  {
    return m_pMemStream->seek(offset, whence);
  } 

  OdUInt64 tell()
  {
    return m_pMemStream->tell();
  }


};

typedef OdSmartPtr<MyPageStream> MyPageStreamPtr;

//----------------------------------------------------------
//
// ExPageController
//
//----------------------------------------------------------
ExPageController::ExPageController()
{
}

ExPageController::~ExPageController()
{
  if (!pPagingFile.isNull())
  {
    OdAnsiString fName(pPagingFile->fileName());
    pPagingFile.release();
#ifdef OD_HAVE_REMOVE_FUNC
    remove(fName.c_str());
#else
    OdString tmp(fName.c_str());
    DeleteFile(tmp.c_str());
#endif
  }
}

OdStreamBufPtr ExPageController::read(Key key)
{
  if ( pPagingFile.isNull() )
    return (OdStreamBuf*)0;
  
  if (pPagingFile->seek(key, OdDb::kSeekFromStart) != key)
    return (OdStreamBuf*)0;

  OdUInt32 len(0);
  pPagingFile->getBytes(&len, sizeof(OdUInt32)); 
  pPagingFile->seek(-(int)sizeof(OdUInt32), OdDb::kSeekFromCurrent);
  m_mFreeOffsets[len].append(key);

  MyPageStreamPtr pRet = OdRxObjectImpl<MyPageStream>::createObject();
  if (!pRet->init(pPagingFile))
    return (OdStreamBuf*)0;

  return pRet;
}

OdInt64 ExPageController::getOffsetInPageFile(OdUInt32 length)
{
  if (m_mFreeOffsets.empty())
  {
    pPagingFile->seek(0, OdDb::kSeekFromEnd);
    return pPagingFile->tell();
  }

  std::map<OdUInt32, Offsets>::iterator itr;
  itr = m_mFreeOffsets.find(length);
  if (itr == m_mFreeOffsets.end())
  {
    pPagingFile->seek(0, OdDb::kSeekFromEnd);
    return pPagingFile->tell();
  }
  else
  {
    Offsets &offsets = itr->second; 
    OdInt64 offset = offsets.last();
    offsets.removeLast();
    if (offsets.isEmpty())
      m_mFreeOffsets.erase(itr);
   
    return offset;
  }
}

bool ExPageController::write(Key& key, OdStreamBuf* pStreamSrc)
{
  if ( pPagingFile.isNull() )
    return false;

  OdUInt32 len = (OdUInt32)pStreamSrc->length();
  key = getOffsetInPageFile(len);
  pPagingFile->seek(key, OdDb::kSeekFromStart);
  pPagingFile->putBytes(&len, sizeof(OdUInt32));

  m_buffTmp.resize(len);
  pStreamSrc->getBytes(m_buffTmp.asArrayPtr(), len);
  pPagingFile->putBytes(m_buffTmp.asArrayPtr(), len);

  return true;
}

void ExPageController::setDatabase(OdDbDatabase* pDb)
{
  ExUnloadController::setDatabase(pDb);
  OdString tmpDir = pDb->appServices()->getTempPath();
  tmpDir += OdString().format(L"page%lu.tmp", (OdUInt64)(OdIntPtr)database());
  
  pPagingFile = odrxSystemServices()->createFile(tmpDir, Oda::FileAccessMode(Oda::kFileRead | Oda::kFileWrite),
                                                         Oda::kShareDenyReadWrite, Oda::kCreateAlways);
  if ( pPagingFile.isNull() )
    throw OdError(eFileAccessErr);
}

int ExPageController::pagingType() const
{
  return OdDb::kUnload|OdDb::kPage;
}
