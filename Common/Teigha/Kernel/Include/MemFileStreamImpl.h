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

// MemFileStreamImpl.h : implementation for the OdMemFileStreamImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_MEMORYFILESTREAMIMPL_H_INCLUDED_)
#define _MEMORYFILESTREAMIMPL_H_INCLUDED_

#include "RxObjectImpl.h"
#include "MemoryStream.h"
#include "OdAlloc.h"

#ifdef OD_HAVE_UNISTD_FILE
#include <unistd.h>
#endif

#ifdef OD_HAVE_WINDOWS_STORAGE_FILE
#include <windows.storage.h>
#endif

#ifdef OD_HAVE_WRL_FILE
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#endif

//DD:EXPORT_ON

/** \details
    <group TD_Namespaces>
*/
namespace TmpFileHelper
{
#define TEMPFILE_PREFIX  "tx$"

  /** \details
    Returns the temporary file name.
  */
  static OdString getTempFile(OdDbBaseHostAppServices* pHostAppServices, const OdAnsiString& prefix = TEMPFILE_PREFIX)
  {
    OdString tmpDir = pHostAppServices->getTempPath();
#if defined(_WINRT)
    GUID result;
    if (CoCreateGuid(&result) < 0) 
        return OdString::kEmpty;
    tmpDir = tmpDir.format(L"%ls%llx%x%x.tmp", tmpDir.c_str(), result.Data1, result.Data2, result.Data3);

#elif defined(_WIN32)  // Win 32 + 64 bit
    OdChar tmpFile[MAX_PATH];
    ::GetTempFileName(tmpDir.c_str(), !prefix.isEmpty() ? OdString(prefix).c_str() : L"tmp", 0, tmpFile);
    if (odStrLen(tmpFile))
      tmpDir = OdString(tmpFile, (int)odStrLen(tmpFile));
#else
    char tmpFile[4096];  // sufficient for Win, Linux, Mac
    tmpFile[0] = 0;

    OdAnsiCharArray dstBuf;
    int lenStr = tmpDir.getLength();
    dstBuf.reserve(lenStr * 4); // UTF-8 can use up to 4 bytes per character
    OdCharMapper::unicodeToUtf8(tmpDir.c_str(), lenStr, dstBuf);
    strcat(tmpFile, (const char*)dstBuf.asArrayPtr());
    
    strcat(tmpFile, !prefix.isEmpty() ? prefix.c_str() : "tmp");
#ifdef OD_HAVE_MKSTEMPS_FUNC
    strcat(tmpFile, "XXXXXX.tmp");
    int fileDescriptor = mkstemps(tmpFile, 4);
#else
    strcat(tmpFile, "XXXXXX");
    int fileDescriptor = mkstemp(tmpFile);
#endif
    if (fileDescriptor == -1)
    {
      tmpFile[0] = 0;
    }
    else
      close(fileDescriptor);
    // Convert Utf8 back to Unicode
    OdAnsiString ansiTmpFile(tmpFile);
    OdCharArray buf;
    OdCharMapper::utf8ToUnicode(ansiTmpFile.c_str(), ansiTmpFile.getLength(), buf);
    tmpDir = OdString(buf.getPtr(), buf.size() - 1);
#endif
    return tmpDir;
  }

} // namespace

/*
same as OdMemoryStreamImpl, except that it uses a much larger PageSize, and a filled page
is swapped to an automatically created + associated temporary disk file;
the page data is then deleted, and if accessed, swapped-in from file again before accessing;
the PAGE's size should be at least 512 kB
*/

/** \details
  <group Other_Classes>

  Declares the page data.
*/
class PAGE
{
public:
  PAGE*    m_pNextPage;
  PAGE*    m_pPrevPage;
  OdUInt64 m_nPageStartAddr;
  OdUInt8* m_data;
  bool     m_modified;
  bool     m_savedToFile;
  OdUInt64 m_nFilePos;
};

/** \details

<group Other_Classes>
*/
class OdMemFileStreamDummyBase { };


/** \details
  <group Other_Classes>

  This class defines the interface for paged-memory file I/O objects.
*/
template <class TBase = OdMemFileStreamDummyBase>
class OdMemFileStreamImpl : public TBase
{
  friend class OdMemoryStream;   

  OdUInt64  m_numPages;
  PAGE*     m_pFirstPage;
  PAGE*     m_pCurrPage;
  PAGE*     m_pLastPage;

  /** \details
    <group Other_Classes>

     Helper class to swap in/out a PAGE from/to disk file.
  */
  class PageSwitcher
  {
    // the *singleton* data buffer

    OdArray<OdUInt8, OdMemoryAllocator<OdUInt8> >  m_dataBuffer;
    PAGE*          m_activePage;   
    OdString   m_sTmpFileName;
    OdStreamBufPtr m_pTmpFile;
    OdDbBaseHostAppServices* m_pHostAppServices;

    /** \details
      Releases the disk file. 
    */
    bool releaseDiskFile(bool deleteIt)
    {
      m_pTmpFile = NULL;
      if (deleteIt && !m_sTmpFileName.isEmpty()) {
#ifdef OD_HAVE_REMOVE_FUNC
      remove(m_sTmpFileName);
#else
      DeleteFile(OdString(m_sTmpFileName).c_str());
#endif
      }
      m_sTmpFileName.empty();
      m_activePage = 0;
      return true;
    };

  public:
    /** \details
      Constructor creates the empty page switcher.
    */
    PageSwitcher()
      : m_activePage(NULL)
    {
    };

    /** \details
      Destructor deletes the buffer for page data and releases the disk file.
    */
    ~PageSwitcher()
    {
      setBufferSize(0);
      releaseDiskFile(true);
    };

    void setAppServices(OdDbBaseHostAppServices* pHostAppServices)
    {
      m_pHostAppServices = pHostAppServices;
    }

    /** \details
      Sets the buffer size for storing page data. 

      \param pageSize [in]  page size.
    */
    inline void  setBufferSize(OdUInt32 pageSize) { m_dataBuffer.resize(pageSize); };

    /** \details
      Returns the page data as an array of 8-bit codes.

      \param pPage [in]  pointer to the page data.
    */
    inline OdUInt8*  dataBuffer(PAGE* pPage) { return m_dataBuffer.asArrayPtr();};

    /** \details
      Returns the size of the page data.

      \param pPage [in]  pointer to the page data.
    */
    inline OdUInt32  dataSize(PAGE* pPage)   { return m_dataBuffer.length(); };

    /** \details
      Returns the current active page data. 
    */
    inline PAGE* activePage() const { return m_activePage; };

    /** \details
      Makes the page data as active.

      \param pPage [in]  pointer to the page data.
    */
    inline void  setActivePage(PAGE* pPage) { m_activePage = pPage; };

    /** \details
      Checks whether the page data is current and active, and returns True if the page data 
      is active or False if the page data is inactive.

      \param pPage [in]  pointer to the page data.
    */
    inline bool  isActivePage(PAGE* pPage) const { return (pPage == m_activePage); };

    /** \details
      Checks whether the page data is modified, and returns True if the page data 
      is modified or False if the page data is not changed.

      \param pPage [in]  pointer to the page data.
    */
    inline bool  isModified(PAGE* pPage) const { return pPage->m_modified; };

    /** \details
      Marks the page data as modified.

      \param pPage [in]  pointer to the page data.
      \param modified [in]  True if the page data is modified, False if the page data is not changed.
    */
    inline void  setModified(PAGE* pPage, bool modified) { pPage->m_modified = modified; };

    /** \details
      Checks whether the page data is saved, and returns True if the page data 
      is saved or False if the page data is unsaved.

      \param pPage [in]  pointer to the page data.
    */
    inline bool  isSaved(PAGE* pPage) const { return pPage->m_savedToFile; };

    /** \details
      Marks the page data as saved.

      \param pPage [in]  pointer to the page data.
      \param saved [in]  True if the page data is saved, False if the page data is unsaved.
    */
    inline void  setSaved(PAGE* pPage, bool saved) { pPage->m_savedToFile = saved; };

    /** \details
      Returns the file position of the page data.

      \param pPage [in]  pointer to the page data.
    */
    inline OdUInt64 filePosition(PAGE* pPage) const { return pPage->m_nFilePos; };

    /** \details
      Sets the file position of the page data.

      \param pPage [in]  pointer to the page data.
      \param seekPos [in]  position in the file.
    */
    inline void  setFilePosition(PAGE* pPage, OdUInt64 seekPos) { pPage->m_nFilePos = seekPos; };

    /** \details
      Initializes the page data.

      \param pPage [in]  pointer to the page data.
    */
    void initializePage(PAGE* pPage)
    {
      ODA_ASSERT(pPage);
      pPage->m_pPrevPage = 0;
      pPage->m_pNextPage = 0;
      pPage->m_data = dataBuffer(pPage);
      setModified(pPage, false);
      setSaved(pPage, false);
      setFilePosition(pPage, 0);
    };

    /** \details
      Gets the temporary disk file as a stream. 
    */
    OdStreamBufPtr& diskFile()
    {
      if (!m_pTmpFile.isNull())
        return m_pTmpFile;

      // determine temporary filename
      if (m_pHostAppServices)
      {
        m_sTmpFileName = TmpFileHelper::getTempFile(m_pHostAppServices);
        m_pTmpFile = odrxSystemServices()->createFile(m_sTmpFileName,
          (Oda::FileAccessMode)(Oda::kFileRead | Oda::kFileWrite),
          Oda::kShareDenyReadWrite,
          Oda::kCreateAlways);
        ODA_ASSERT(!m_pTmpFile.isNull());
        return m_pTmpFile;
      } 
      else
        throw OdError(eNullObjectPointer);
    };

    /** \details
      Saves the specified page data in the disk file and marks it as saved.
      Returns True if saving is successful.

      \param pPage [in]  pointer to the page data.
    */
    bool savePage(PAGE* pPage)
    {
      if (pPage && isSaved(pPage) && !isModified(pPage)) // no need to save here
        return true;

      if (!pPage || diskFile().isNull())
        return false;

      if (isSaved(pPage))
      {
        diskFile()->seek(filePosition(pPage), OdDb::kSeekFromStart);
      }
      else
      {
        diskFile()->seek(0, OdDb::kSeekFromEnd);
        setFilePosition(pPage, diskFile()->tell());
      }
      diskFile()->putBytes(dataBuffer(pPage), dataSize(pPage));

      setModified(pPage, false);
      setSaved(pPage, true);
      return true;
    };

    /** \details
      Loads the specified page data from the disk file and makes it active. 
      Returns True if loading is successful.

      \param pPage [in]  pointer to the page data.
    */
    bool loadPage(PAGE* pPage)
    {
      if (!pPage)
        return false;
      if (isActivePage(pPage) && !isModified(pPage)) // no need to (re-)load here
        return true;

      if (isSaved(pPage) && !diskFile().isNull())
      {
        diskFile()->seek(filePosition(pPage), OdDb::kSeekFromStart);
        diskFile()->getBytes(dataBuffer(pPage), dataSize(pPage));
      }

      setModified(pPage, false);
      setActivePage(pPage);
      return true;
    };

    /** \details
      Checks whether the page data can be ensured. 

      \param pPage [in]  pointer to the page data.
    */
    bool ensurePage(PAGE* pPage)
    {
      ODA_ASSERT(pPage);
      return isActivePage(pPage) ? true : switchToPage(pPage);
    };

    /** \details
      Switches to the page data. If the specified page data is not active, the 
      method saves the current active page data and loads the specified page data.
      Returns True if the page data is loaded successfully or is active.

      \param pPage [in]  pointer to the page data.
    */
    bool switchToPage(PAGE* pPage)
    {
      ODA_ASSERT(pPage);
      if (isActivePage(pPage))
        return true;
      savePage(activePage());
      return loadPage(pPage);
    };

    /** \details
      Rewinds the page data.

      \param pPage [in]  pointer to the page data.
    */
    bool rewind(PAGE* pPage)
    {
      ODA_ASSERT(pPage);
      // 'pPage' is loaded at next read/write access
      return isActivePage(pPage) ? true : savePage(activePage());
    }

    /** \details
      Truncates the page data.

      \param pPage [in]  pointer to the page data.
    */
    bool truncate(PAGE* pPage)
    {
      ODA_ASSERT(pPage);
      bool wasSaved = isSaved(pPage);
      setModified(pPage, false);
      setSaved(pPage, false);
      if (!wasSaved || diskFile().isNull())
      {
        return true;
      }

      diskFile()->seek(filePosition(pPage), OdDb::kSeekFromStart);
      diskFile()->truncate();
      return true;
    };
  }; // class PageSwitcher

  PageSwitcher m_pageSwitcher;

  /** \details
    Copies the file stream.
  */
  OdMemFileStreamImpl& operator= (const OdMemFileStreamImpl&) {return *this;}

protected:
  OdUInt64  m_nCurPos;
  OdUInt64  m_nEndPos;
  OdUInt32  m_nPageDataSize;

  /** \details
    Returns the page switcher.
  */
  inline PageSwitcher& pageSwitcher() { return m_pageSwitcher; };

  /** \details
    Returns the allocated memory size for all pages.
  */
  inline OdUInt64 allocated() const { return m_numPages * m_nPageDataSize; }
  inline OdUInt64 curPageNo() const { return (m_nCurPos / m_nPageDataSize); }
  inline OdUInt32 posInCurPage() const { return OdUInt32(m_nCurPos % m_nPageDataSize); }
  inline OdUInt32 leftInCurPage() const { return (m_pCurrPage ? m_nPageDataSize - posInCurPage() : 0); }

  /** \details
    Adds new page data.
  */
  void addPage();

  /** \details
    Finds the next page data.
  */
  void seekNextPage(bool bCreateNew = false);

  /** \details
    Finds the previous page data.
  */
  inline void seekPrevPage()
  {
    if(m_pCurrPage->m_pPrevPage)
    {
      m_nCurPos -= posInCurPage();
      m_nCurPos -= m_nPageDataSize;
      m_pCurrPage = m_pCurrPage->m_pPrevPage;
      pageSwitcher().switchToPage(m_pCurrPage);
    }
    else
      throw OdError(eEndOfFile);
  }

  inline OdUInt8* currPos() { return m_pCurrPage->m_data + posInCurPage(); }
  inline const OdUInt8* currPos() const { return m_pCurrPage->m_data + posInCurPage(); }

public:
  OdMemFileStreamImpl(OdUInt32 nPageDataSize = 0x00200000) // 2MB
    : m_numPages(0)
    , m_pFirstPage(0)
    , m_pCurrPage(0)
    , m_pLastPage(0)
    , m_nCurPos(0)
    , m_nEndPos(0)
    , m_nPageDataSize(0)
  {
    setPageDataSize(nPageDataSize);
    setAppServices(0);
  }

  void setAppServices(OdDbBaseHostAppServices* pHostAppServices)
  {
    pageSwitcher().setAppServices(pHostAppServices);
  }

  /** \details
    Returns the page data size.
  */
  OdUInt32 pageDataSize() const { return m_nPageDataSize; }

  /** \details
    Sets the page data size and buffer size for page switcher.

    \param nPageSize [in]  page data size.
  */
  void setPageDataSize(OdUInt32 nPageSize)
  {
    m_nPageDataSize = nPageSize;
    pageSwitcher().setBufferSize(nPageSize);
  }

  /** \details
    Reserves the specified number of bytes for the file memory stream object.
  */
  void reserve(OdUInt64 nSize)
  {
    while(nSize > allocated())
      addPage();
  }

  /** \details
    Returns the length in bytes of the file associated with this stream object.
  */
  OdUInt64 length() 
  {
    return m_nEndPos; 
  }

  /** \details
    Moves the current position of the file pointer for this stream object to the specified location.

    \param offset [in]  number of bytes
    \param seekType [in]  seek type
  */
  OdUInt64 seek(OdInt64 offset, OdDb::FilerSeekType whence);

  /** \details
    Returns the current position of the file pointer for this stream object.
  */
  OdUInt64 tell()
  {
    return m_nCurPos;
  }

  /** \details
    Checks whether the file pointer has reached the end of a file, and returns True if the  
    current position for this stream object is at the end of file.
  */
  bool isEof()
  {
    return (m_nCurPos>=m_nEndPos);
  }

  /** \details
    Returns the byte at the current position of the file pointer for this stream object and 
    increments the pointer.
  */
  OdUInt8 getByte();

  /** \details
    Returns the specified number of bytes, starting at the current position of the file pointer 
    for this stream object, and increments the pointer on the specified number of bytes.

    \param buffer [in]  pointer to the character buffer
    \param nLen [in]  number of bytes
  */
  void getBytes(void* buffer, OdUInt32 nLen);

  /** \details
    Writes the byte to the current position of the file pointer for this stream object, and 
    increments the pointer.

    \param val [in]  byte to be wrote
  */
  void putByte(OdUInt8 val);

  /** \details
    Writes the specified number of bytes, starting at the current position of the file pointer 
    for this stream object, and increments the pointer on the specified number of bytes.

    \param buffer [in]  pointer to the character buffer
    \param nLen [in]  number of bytes
  */
  void putBytes(const void* buffer, OdUInt32 nLen);

  //void copyDataTo(TBase* pDest, OdUInt32 nSrcStart = 0, OdUInt32 nSrcEnd = 0)
  //{
  //  TBase::copyDataTo(pDest, nSrcStart, nSrcEnd);
  //}

  /** \details
    Sets the current position of the file pointer for this stream object to zero.
  */
  void rewind()
  {
    m_nCurPos = 0;
    m_pCurrPage = m_pFirstPage;
    if(m_pCurrPage)
    {
      pageSwitcher().rewind(m_pCurrPage);
    }
  }

  /** \details
    Sets the length of the file associated with this stream object to zero.
  */
  void truncate()
  {
    if(m_pCurrPage)
    {
      pageSwitcher().truncate(m_pCurrPage);
      PAGE* pNext, *pCurr = m_pCurrPage->m_pNextPage;
      m_pCurrPage->m_pNextPage = 0;
      m_pLastPage = m_pCurrPage;
      m_nEndPos = m_nCurPos;
      while(pCurr)
      {
        pNext = pCurr->m_pNextPage;
        pCurr->m_data = 0;
        if (pageSwitcher().activePage() == pCurr)
        {
          pageSwitcher().setActivePage(0);
        }
        ::odrxFree(pCurr);
        --m_numPages;
        pCurr = pNext;
      }
    }
  }

  /** \details
    Destructor deletes stream object.
  */
  ~OdMemFileStreamImpl();
};

////////////////////////////////////////////////////////

template <class TBase>
OdMemFileStreamImpl<TBase>::~OdMemFileStreamImpl()
{
  PAGE* pNext, *pCurr = m_pFirstPage;
  while(pCurr)
  {
    pNext = pCurr->m_pNextPage;
    pCurr->m_data = 0;
    if (pageSwitcher().activePage() == pCurr)
    {
      int t=0;
    }
    ::odrxFree(pCurr);
    pCurr = pNext;
  }
  m_pFirstPage = 0;
}

template <class TBase>
void OdMemFileStreamImpl<TBase>::addPage()
{
  PAGE* pPage = (PAGE*)::odrxAlloc(sizeof(PAGE));
  if (!pPage)
    throw OdError(eOutOfMemory);

  pageSwitcher().initializePage(pPage);

  pPage->m_pPrevPage = m_pLastPage;
  if(m_pLastPage)
  {
    m_pLastPage->m_pNextPage = pPage;
    if(!m_pCurrPage)
    {
      m_pCurrPage = pPage;
      pageSwitcher().switchToPage(m_pCurrPage);
    }
    pPage->m_nPageStartAddr = pPage->m_pPrevPage->m_nPageStartAddr + m_nPageDataSize;
  }
  else
  {
    m_pFirstPage = m_pCurrPage = pPage;
    pPage->m_nPageStartAddr = 0;
    pageSwitcher().switchToPage(m_pCurrPage);
  }
  m_pLastPage = pPage;
  ++m_numPages;
}

template <class TBase>
void OdMemFileStreamImpl<TBase>::seekNextPage(bool bCreateNew)
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
    pageSwitcher().switchToPage(m_pCurrPage);
  }
  else
    throw OdError(eEndOfFile);
}

template <class TBase>
OdUInt64 OdMemFileStreamImpl<TBase>::seek(OdInt64 offset, OdDb::FilerSeekType from)
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
    pageSwitcher().switchToPage(m_pCurrPage);
  }
  return m_nCurPos;
}

template <class TBase>
OdUInt8 OdMemFileStreamImpl<TBase>::getByte()
{
  OdUInt8 ret;
  if(m_nCurPos < m_nEndPos)
  {
    pageSwitcher().ensurePage(m_pCurrPage);
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
void OdMemFileStreamImpl<TBase>::getBytes(void* buffer, OdUInt32 nLen)
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
      pageSwitcher().ensurePage(m_pCurrPage);
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
void OdMemFileStreamImpl<TBase>::putByte(OdUInt8 val)
{
  if(!m_pCurrPage)
  {
    seekNextPage(true);
  }
  pageSwitcher().ensurePage(m_pCurrPage);
  pageSwitcher().setModified(m_pCurrPage, true);
  OdUInt64 nPosInPage = posInCurPage();
  m_pCurrPage->m_data[nPosInPage] = val;
  ++m_nCurPos;
  m_nEndPos = odmax(m_nCurPos, m_nEndPos);
  if ((nPosInPage + 1) == m_nPageDataSize)
  {
    m_pCurrPage = m_pCurrPage->m_pNextPage;
  }
}

template <class TBase>
void OdMemFileStreamImpl<TBase>::putBytes(const void* buffer, OdUInt32 nLen)
{
  if (nLen == 0)
  {
    return;
  }
  const OdUInt8* pSrc = (const OdUInt8*)buffer;
  OdUInt32 nToCopy = odmin(leftInCurPage(), nLen);
  if(nToCopy)
  {
    pageSwitcher().ensurePage(m_pCurrPage);
    pageSwitcher().setModified(m_pCurrPage, true);
    ::memcpy(currPos(), pSrc, nToCopy);
    pSrc += nToCopy;
    nLen -= nToCopy;
  }
  while(nLen)
  {
    seekNextPage(true);
    pageSwitcher().setModified(m_pCurrPage, true);
    nToCopy = odmin(m_nPageDataSize, nLen);
    ::memcpy(m_pCurrPage->m_data, pSrc, nToCopy);
    pSrc += nToCopy;
    nLen -= nToCopy;
  }
  m_nCurPos += nToCopy;
  if(m_nCurPos && (m_nCurPos % m_nPageDataSize)==0)
  {
    m_pCurrPage = m_pCurrPage->m_pNextPage;
  }
  m_nEndPos = odmax(m_nCurPos, m_nEndPos);
}

//DD:EXPORT_OFF

#endif // !defined(_MEMORYFILESTREAMIMPL_H_INCLUDED_)

