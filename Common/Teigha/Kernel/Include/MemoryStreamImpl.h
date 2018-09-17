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




// MemoryStreamImpl.h: implementation for the OdMemoryStreamImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_MEMORYSTREAMIMPL_H_INCLUDED_)
#define _MEMORYSTREAMIMPL_H_INCLUDED_

#include "RxObjectImpl.h"
#include "MemoryStream.h"
#include "OdAlloc.h"

//DD:EXPORT_ON


/** \details

    {group:Other_Classes}
*/
class OdMemoryStreamDummyBase { };

/** \details

    {group:Other_Classes}
*/
template <class TBase = OdMemoryStreamDummyBase>
class OdMemoryStreamImpl : public TBase
{
  friend class OdMemoryStream;
  struct PAGE
  {
    PAGE*    m_pNextPage;
    PAGE*    m_pPrevPage;
    OdUInt64 m_nPageStartAddr;
    OdUInt8  m_data[1];
  };
  OdUInt64  m_numPages;
  PAGE*     m_pFirstPage;
  PAGE*     m_pCurrPage;
  PAGE*     m_pLastPage;
  OdMemoryStreamImpl& operator= (const OdMemoryStreamImpl&) {return *this;}
protected:
  OdUInt64  m_nCurPos;
  OdUInt64  m_nEndPos;
  OdUInt32  m_nPageDataSize;

  inline OdUInt64 allocated() const { return m_numPages * m_nPageDataSize; }
  inline OdUInt64 curPageNo() const { return (m_nCurPos / m_nPageDataSize); }
  inline OdUInt32 posInCurPage() const { return OdUInt32(m_nCurPos % m_nPageDataSize); }
  inline OdUInt32 leftInCurPage() const { return (m_pCurrPage ? m_nPageDataSize - posInCurPage() : 0); }
  void addPage();
  void seekNextPage(bool bCreateNew = false);
  inline void seekPrevPage()
  {
    if(m_pCurrPage->m_pPrevPage)
    {
      m_nCurPos -= posInCurPage();
      m_nCurPos -= m_nPageDataSize;
      m_pCurrPage = m_pCurrPage->m_pPrevPage;
    }
    else
      throw OdError(eEndOfFile);
  }
  inline OdUInt8* currPos() { return m_pCurrPage->m_data + posInCurPage(); }
  inline const OdUInt8* currPos() const { return m_pCurrPage->m_data + posInCurPage(); }
public:
  OdMemoryStreamImpl(OdUInt32 nPageDataSize = 0x2000)
    : m_numPages(0)
    , m_pFirstPage(0)
    , m_pCurrPage(0)
    , m_pLastPage(0)
    , m_nCurPos(0)
    , m_nEndPos(0)
    , m_nPageDataSize(nPageDataSize)
  {
  }

  // The methods correspond to OdMemoryStream interface
  //
  OdUInt32 pageDataSize() const { return m_nPageDataSize; }

  void setPageDataSize(OdUInt32 nPageSize) { m_nPageDataSize = nPageSize; }

  void reserve(OdUInt64 nSize)
  {
    while(nSize > allocated())
      addPage();
  }

  // The methods correspond to OdStreamBuf interface
  //
  OdUInt64 length() 
	{
		return m_nEndPos; 
	}

  OdUInt64 seek(OdInt64 offset, OdDb::FilerSeekType whence);

  const void *pageAlignedAddress(OdUInt32 nLen);

  OdUInt64 tell()
  {
    return m_nCurPos;
  }

  bool isEof()
  {
    return (m_nCurPos>=m_nEndPos);
  }

  OdUInt8 getByte();
  void getBytes(void* buffer, OdUInt32 nLen);

  void putByte(OdUInt8 val);
  void putBytes(const void* buffer, OdUInt32 nLen);

  //void copyDataTo(TBase* pDest, OdUInt32 nSrcStart = 0, OdUInt32 nSrcEnd = 0)
  //{
  //  TBase::copyDataTo(pDest, nSrcStart, nSrcEnd);
  //}

  void rewind()
  {
    m_nCurPos = 0;
    m_pCurrPage = m_pFirstPage;
  }

  void truncate()
  {
    if(m_pCurrPage)
    {
      PAGE* pNext, *pCurr = m_pCurrPage->m_pNextPage;
      m_pCurrPage->m_pNextPage = 0;
      m_pLastPage = m_pCurrPage;
      m_nEndPos = m_nCurPos;
      while(pCurr)
      {
        pNext = pCurr->m_pNextPage;
        ::odrxFree(pCurr);
        --m_numPages;
        pCurr = pNext;
      }
    }
  }

  ~OdMemoryStreamImpl();
};

////////////////////////////////////////////////////////
/** \details

    {group:Other_Classes}
*/
template <class TBase>
OdMemoryStreamImpl<TBase>::~OdMemoryStreamImpl()
{
  PAGE* pNext, *pCurr = m_pFirstPage;
  while(pCurr)
  {
    pNext = pCurr->m_pNextPage;
    ::odrxFree(pCurr);
    pCurr = pNext;
  }
  m_pFirstPage = 0;
}

template <class TBase>
void OdMemoryStreamImpl<TBase>::addPage()
{
  PAGE* pPage = (PAGE*)::odrxAlloc(sizeof(PAGE)-1+m_nPageDataSize);
  if (!pPage)
    throw OdError(eOutOfMemory);
  pPage->m_pPrevPage = m_pLastPage;
  pPage->m_pNextPage = 0;
  if(m_pLastPage)
  {
    m_pLastPage->m_pNextPage = pPage;
    if(!m_pCurrPage)
      m_pCurrPage = pPage;
    pPage->m_nPageStartAddr = pPage->m_pPrevPage->m_nPageStartAddr + m_nPageDataSize;
  }
  else
  {
    m_pFirstPage = m_pCurrPage = pPage;
    pPage->m_nPageStartAddr = 0;
  }
  m_pLastPage = pPage;
  ++m_numPages;
}

template <class TBase>
void OdMemoryStreamImpl<TBase>::seekNextPage(bool bCreateNew)
{
  if(bCreateNew)
  {
    if(!m_pCurrPage)
    {
      addPage();
      return;
    }
    if(!m_pCurrPage->m_pNextPage)
      addPage();
  }
  if(m_pCurrPage->m_pNextPage)
  {
    m_pCurrPage = m_pCurrPage->m_pNextPage;
    m_nCurPos += leftInCurPage();
  }
  else
    throw OdError(eEndOfFile);
}

template <class TBase>
OdUInt64 OdMemoryStreamImpl<TBase>::seek(OdInt64 offset, OdDb::FilerSeekType from)
{
  OdUInt64 nNewPos;
	switch(from)
	{
	case OdDb::kSeekFromEnd:
		nNewPos = m_nEndPos + offset;
		break;
	case OdDb::kSeekFromCurrent:
		nNewPos = m_nCurPos + offset;
		break;
	case OdDb::kSeekFromStart:
    if(!offset)
    {
      rewind();
      return 0;
    }
		nNewPos = offset;
		break;
	default:
		throw OdError(eInvalidInput);
		break;
	};
  if(nNewPos != m_nCurPos)
  {
    if (nNewPos > m_nEndPos)
      throw OdError( eEndOfFile );

    OdInt64 nFromEnd = m_nEndPos - nNewPos;

    bool bForward = false;
    bool bBack = false;
    if (m_pCurrPage)
    {
      if (nNewPos >= m_pCurrPage->m_nPageStartAddr)
      {
        bForward = true;
      }
      else
      {
        bBack = true;
      }
    }

    if (nFromEnd == 0)
    {
      m_pCurrPage = m_pLastPage;
      bForward = true;
    }
    else if (bForward)
    {
      if (nFromEnd < ((OdInt64)nNewPos - (OdInt64)m_pCurrPage->m_nPageStartAddr))
      {
        m_pCurrPage = m_pLastPage;
        bForward = false;
      }
    }
    else if (bBack)
    {
      if (nNewPos < (m_pCurrPage->m_nPageStartAddr - nNewPos))
      {
        m_pCurrPage = m_pFirstPage;
        bForward = true;
      }
    }
    else
    {
      if (nNewPos <= (OdUInt64)nFromEnd)
      {
        m_pCurrPage = m_pFirstPage;
        bForward = true;
      }
      else
      {
        m_pCurrPage = m_pLastPage;
      }
    }
    m_nCurPos = nNewPos;
    if (bForward)
    {
      nNewPos = nNewPos / m_nPageDataSize * m_nPageDataSize ;
      while(m_pCurrPage && m_pCurrPage->m_nPageStartAddr < nNewPos)
      {
        m_pCurrPage = m_pCurrPage->m_pNextPage;
      }
    }
    else
    {
      while(m_pCurrPage->m_nPageStartAddr > m_nCurPos)
      {
        m_pCurrPage = m_pCurrPage->m_pPrevPage;
      }
    }
  }
  return m_nCurPos;
}

template <class TBase>
const void *OdMemoryStreamImpl<TBase>::pageAlignedAddress(OdUInt32 nLen)
{
#ifndef TD_STRICT_ALIGNMENT
  if (leftInCurPage() >= nLen)
    return currPos();
#endif
  return NULL;
}

template <class TBase>
OdUInt8 OdMemoryStreamImpl<TBase>::getByte()
{
  OdUInt8 ret;
  if(m_nCurPos < m_nEndPos)
  {
    OdUInt32 nPosInPage = posInCurPage();
    ret = m_pCurrPage->m_data[nPosInPage];
    ++m_nCurPos;
    if ((nPosInPage + 1) == m_nPageDataSize)
      m_pCurrPage = m_pCurrPage->m_pNextPage;
  }
  else
  {
    throw OdError(eEndOfFile);
  }
  return ret;
}

template <class TBase>
void OdMemoryStreamImpl<TBase>::getBytes(void* buffer, OdUInt32 nLen)
{
  if (nLen==0)
    return;

  OdUInt64 nNewPos = m_nCurPos + nLen;
  if(nNewPos <= m_nEndPos)
  {
    OdUInt8* pDest = (OdUInt8*)buffer;
    OdUInt32 nToCopy = odmin(leftInCurPage(), nLen);
    if(nToCopy)
    {
      ::memcpy(pDest, currPos(), nToCopy);
      pDest += nToCopy;
      nLen -= nToCopy;
    }
    while(nLen)
    {
      seekNextPage();
      nToCopy = odmin(m_nPageDataSize, nLen);
      ::memcpy(pDest, m_pCurrPage->m_data, nToCopy);
      pDest += nToCopy;
      nLen -= nToCopy;
    }
    m_nCurPos = nNewPos;
    if(m_nCurPos && (m_nCurPos % m_nPageDataSize)==0)
      m_pCurrPage = m_pCurrPage->m_pNextPage;
  }
  else
  {
    throw OdError(eEndOfFile);
  }
}

template <class TBase>
void OdMemoryStreamImpl<TBase>::putByte(OdUInt8 val)
{
  if(!m_pCurrPage)
  {
    seekNextPage(true);
  }
  OdUInt64 nPosInPage = posInCurPage();
  m_pCurrPage->m_data[nPosInPage] = val;
  ++m_nCurPos;
  m_nEndPos = odmax(m_nCurPos, m_nEndPos);
  if ((nPosInPage + 1) == m_nPageDataSize)
    m_pCurrPage = m_pCurrPage->m_pNextPage;
}

template <class TBase>
void OdMemoryStreamImpl<TBase>::putBytes(const void* buffer, OdUInt32 nLen)
{
  if (nLen == 0)
  {
    return;
  }
  const OdUInt8* pSrc = (const OdUInt8*)buffer;
  OdUInt32 nToCopy = odmin(leftInCurPage(), nLen);
  if(nToCopy)
  {
    ::memcpy(currPos(), pSrc, nToCopy);
    pSrc += nToCopy;
    nLen -= nToCopy;
  }
  while(nLen)
  {
    seekNextPage(true);
    nToCopy = odmin(m_nPageDataSize, nLen);
    ::memcpy(m_pCurrPage->m_data, pSrc, nToCopy);
    pSrc += nToCopy;
    nLen -= nToCopy;
  }
  m_nCurPos += nToCopy;
  if(m_nCurPos && (m_nCurPos % m_nPageDataSize)==0)
    m_pCurrPage = m_pCurrPage->m_pNextPage;
  m_nEndPos = odmax(m_nCurPos, m_nEndPos);
}

//DD:EXPORT_OFF

#endif // !defined(_MEMORYSTREAMIMPL_H_INCLUDED_)

