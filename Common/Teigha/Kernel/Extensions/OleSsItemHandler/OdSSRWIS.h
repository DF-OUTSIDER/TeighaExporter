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

#ifndef OdSSRWIS_H_
#define OdSSRWIS_H_

#include "OdStreamBuf.h"
#include "OdBinaryData.h"

#include "StructuredStorage.h"

//-----------------------------------------------------------------------------------------
//    SSRWIS functions to support read/write of stream
//-----------------------------------------------------------------------------------------

inline size_t OdSSRWIS_Read(void* in_pv, size_t in_size, size_t in_num, SSRWIS* in_pIS)
{
  OdStreamBuf* pStream = (OdStreamBuf*) in_pIS->m_pSrc;
  pStream->getBytes(in_pv, OdUInt32(in_size * in_num));
  return in_num;
}

inline size_t OdSSRWIS_Write(const void* in_pv, size_t in_size, size_t in_num, SSRWIS* in_pIS)
{
  OdStreamBuf* pStream = (OdStreamBuf*) in_pIS->m_pSrc;
  pStream->putBytes( in_pv, (OdUInt32)(in_size * in_num) );
  return in_num;
}

inline int OdSSRWIS_Close(SSRWIS* in_pv)
{
  in_pv->m_pSrc       = NULL;
  in_pv->m_pFclose    = NULL;
  in_pv->m_pFread     = NULL;
  in_pv->m_pFtell     = NULL;
  in_pv->m_pFseek     = NULL;
  in_pv->m_pFtruncate = NULL;
  in_pv->m_pFwrite    = NULL;
  return 0;
}

inline int OdSSRWIS_Seek(SSRWIS* in_pIS, SSRW_INT64  in_lOffset, int in_nWhence)
{
  OdStreamBuf* pStream = (OdStreamBuf*)(in_pIS->m_pSrc);

  if( in_nWhence == 0 && ((OdUInt64)(in_lOffset) > pStream->length()) )
  {
    OdBinaryData binData;
    binData.resize( in_lOffset - pStream->length() );
    pStream->seek(0, OdDb::kSeekFromEnd);
    pStream->putBytes( binData.asArrayPtr(), binData.size() );
  }

  pStream->seek( (OdInt64)(in_lOffset), (OdDb::FilerSeekType)(in_nWhence) );

  return 0;
}

//-----------------------------------------------------------------------------------------

inline SSRW_INT64 OdSSRWIS_Tell( SSRWIS*     in_pIS)
{
  OdStreamBuf* pStream = (OdStreamBuf*)(in_pIS->m_pSrc);

  return (SSRW_INT64)(pStream->tell());
}

//-----------------------------------------------------------------------------------------

inline long OdSSRWIS_Truncate( SSRWIS*     in_pIS, SSRW_INT64  in_llNewSIze )
{
  OdStreamBuf* pStream = (OdStreamBuf*)(in_pIS->m_pSrc);

  OdBinaryData binData;

  OdUInt64 maxLength = pStream->length() > (OdUInt64)(in_llNewSIze) ? pStream->length() : (OdUInt64)in_llNewSIze;

  binData.resize( maxLength );

  OdUInt64 uPos = pStream->tell();

  pStream->seek(0, OdDb::kSeekFromStart);
  pStream->getBytes( binData.asArrayPtr(), pStream->length() );
  pStream->seek(0, OdDb::kSeekFromStart);
  pStream->truncate();
  pStream->putBytes( binData.asArrayPtr(), in_llNewSIze );

  if( uPos < pStream->length() )
  {
    pStream->seek(uPos, OdDb::kSeekFromStart);
  }

  return 0;
}

//----------------------------------------------------------

inline RootStorage* createFileStorage(void* pSrc)
{
  StgMode rwMode = STG_RW; 

  SSRWIS* pStreamIO = (SSRWIS*) malloc(sizeof(SSRWIS)); // will be free via closeStructuredStorage (see also SsrwOpenMemForReading)
  pStreamIO->m_pSrc = pSrc; // OdStreamBuf*
  pStreamIO->m_pFclose     = OdSSRWIS_Close;
  pStreamIO->m_pFread      = OdSSRWIS_Read;
  pStreamIO->m_pFwrite     = OdSSRWIS_Write;
  pStreamIO->m_pFseek      = OdSSRWIS_Seek;
  pStreamIO->m_pFtell      = OdSSRWIS_Tell;
  pStreamIO->m_pFtruncate  = OdSSRWIS_Truncate;

  RootStorage* pRootStorage = 0;
  int res = createStructuredStorageInSource(pStreamIO, rwMode, &pRootStorage, 0);
  if (res == SSTG_OK)
    return pRootStorage;
  return NULL;
}

inline RootStorage* openFileStorage(void* pSrc, StgMode rwMode = STG_READ)
{
  SSRWIS* pStreamIO = (SSRWIS*) malloc(sizeof(SSRWIS)); // will be free via closeStructuredStorage (see also SsrwOpenMemForReading)
  pStreamIO->m_pSrc = pSrc; // OdStreamBuf*
  pStreamIO->m_pFclose     = OdSSRWIS_Close;
  pStreamIO->m_pFread      = OdSSRWIS_Read;
  pStreamIO->m_pFwrite     = OdSSRWIS_Write;
  pStreamIO->m_pFseek      = OdSSRWIS_Seek;
  pStreamIO->m_pFtell      = OdSSRWIS_Tell;
  pStreamIO->m_pFtruncate  = OdSSRWIS_Truncate;

  RootStorage* pRootStorage = NULL;
  int res = openStructuredStorageInSource(pStreamIO, rwMode, &pRootStorage);
  if (res == SSTG_OK)
    return pRootStorage;
  return NULL;
}

inline Storage* getStorage(RootStorage* pRootStorage)
{
  Storage* pStorage = NULL;
  int res = getStorageFromRoot(pRootStorage, &pStorage);
  if (res == SSTG_OK)
    return pStorage;
  return NULL;
}

inline Stream* createStorageStream(Storage* pStorage, const OdString& name)
{
  Stream* pStream = 0;
  int res = ::createStream(pStorage, name, &pStream);
  if (res == SSTG_OK)
    return pStream;
  return NULL;
}

inline Stream* openStorageStream(Storage* pStorage, const OdString& name)
{
  Stream* pStream = 0;
  int res = ::openStream(pStorage, name, &pStream);
  if (res == SSTG_OK)
    return pStream;
  return NULL;
}

inline unsigned long getStorageStreamSize(Stream* pStream)
{
  unsigned long len = 0L;
  if (streamGetBytesAvailableToRead(pStream, &len) == SSTG_OK)
    return len;
  return 0;
}

inline void writeStorageStream(Stream** ppStream, OdBinaryData data, bool bCloseStream = true)
{
  unsigned int nSize = (unsigned int) data.size();
  unsigned int cnt = nSize;
  if (streamWrite(*ppStream, data.asArrayPtr(), &cnt) != SSTG_OK || nSize != cnt)
  {
    ODA_FAIL_ONCE();
  }
  if (bCloseStream)
  {
    if (closeStream(ppStream) != SSTG_OK || *ppStream)
    {
      ODA_FAIL_ONCE();
    }
  }
}

inline OdBinaryData readStorageStream(Stream** ppStream, unsigned long nOffset = 0, bool bCloseStream = true)
{
  OdBinaryData data;
  unsigned long nSize = getStorageStreamSize(*ppStream);
  if (nSize > nOffset && streamSeek(*ppStream, nOffset, STG_CUR) == SSTG_OK)
  {
    nSize -= nOffset;
    unsigned int cnt = (unsigned int) nSize;
    data.resize(cnt);
    if (streamRead(*ppStream, data.asArrayPtr(), &cnt) != SSTG_OK || nSize != cnt)
    {
      ODA_FAIL_ONCE();
    }
  }
  if (bCloseStream)
  {
    if (closeStream(ppStream) != SSTG_OK || *ppStream)
    {
      ODA_FAIL_ONCE();
    }
  }
  return data;
}

#endif // defined(OdSSRWIS_H_)
