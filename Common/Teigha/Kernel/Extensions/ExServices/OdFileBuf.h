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

#ifndef ODFILEBUF_DEFINED
#define ODFILEBUF_DEFINED

#include "TD_PackPush.h"
#include "OdaCommon.h"

#if defined(ODA_WINDOWS) && !defined(_WINRT)

#include "OdStreamBuf.h"
#include "RxObjectImpl.h"
#include "OdString.h"
#include "RxSystemServices.h"

class OdBaseFileBuf;

/** \remarks
  This template class is a specialization of the OdSmartPtr class for OdBaseFileBuf object pointers.
*/
typedef OdSmartPtr<OdBaseFileBuf> OdBaseFileBufPtr;

/** \details
  This class is the base file I/O class.
  \sa
  Source code provided.
  <group ExServices_Classes>
*/
class OdBaseFileBuf: public OdRxObjectImpl<OdStreamBuf>
{
private:
  bool m_bError;
protected:
  inline void enableErrorMode(bool bEnable = true)
  {
    m_bError = bEnable;
  }

  inline bool errorModeEnabled()
  {
    return m_bError;
  }

  inline void throwOdError(const OdError &pError)
  {
    enableErrorMode(true);
    throw pError;
  }

protected:
  HANDLE    m_hFile;
  OdString  m_sFileName;
  int       m_iFileShare;
  bool      m_bFileWritten;
  Oda::FileAccessMode m_accessMode;

  OdBaseFileBuf()
  {
    m_hFile = INVALID_HANDLE_VALUE;
    m_iFileShare = 0;
    m_bFileWritten = false;
    m_bError = false;
    m_accessMode = (Oda::FileAccessMode)0;
  }

public:
  /** \details
    Opens the specified file with this StreamBuf object.
    \param filename [in]  Filename.
    \param shareMode [in]  Share mode.
    \param accessMode [in]  Access mode.
    \param creationDisposition [in]  Creation disposition.
  */
  virtual void open(const OdString& filename,
    Oda::FileShareMode shareMode,
    Oda::FileAccessMode accessMode,
    Oda::FileCreationDisposition creationDisposition);
#ifndef TD_UNICODE
  virtual void open(const OdCharW* filename,
    Oda::FileShareMode shareMode,
    Oda::FileAccessMode accessMode,
    Oda::FileCreationDisposition creationDisposition);
#endif

  /** \details
    Closes the file opened by this StreamBuf object.
  */
  virtual void close();

  /** \details
    Returns the name of the file associated with this StreamBuf object.
  */
  virtual OdString fileName() { return m_sFileName; }

  /** \details
    Returns the share mode of this StreamBuf object.
  */
  virtual OdUInt32 getShareMode() { return OdUInt32(m_iFileShare); }

  /** \details
    Returns the length in bytes of the file associated with
    this StreamBuf object.
  */
  virtual OdUInt64 length();

  /** \details
    Moves the current position of the file pointer for
    this StreamBuf object to the specified location.

    \param offset [in]  Number of bytes.
    \param seekType [in]  Seek type.

    \remarks
    seekType must be one of the following:

    <table>
    Name                    Value       Description.
    OdDb::kSeekFromStart    SEEK_SET    Start of file.
    OdDb::kSeekFromCurrent  SEEK_CUR    Current position of file pointer.
    OdDb::kSeekFromEnd      SEEK_END    End of file.
    </table>
  */
  virtual OdUInt64 seek(OdInt64 offset, OdDb::FilerSeekType seekType);

  /** \details
    Returns the current position of the file pointer for
    this StreamBuf object.
  */
  virtual OdUInt64 tell();

  /** \details
    Returns true if and only if the current position of the file pointer for
    this StreamBuf object is at the end of file.
  */
  virtual bool isEof();

  /** \details
    Returns the byte at the current position of the file pointer
    for this StreamBuf object, and increments the pointer.
  */
  virtual OdUInt8 getByte();

  /** \details
    Returns the specified number of bytes, starting at the current position of the file pointer
    for this StreamBuf object, and increments the pointer by numBytes.
    \param buffer [in]  Character buffer.
    \param numBytes [in]  Number of bytes.
  */
  virtual void getBytes(void* buffer, OdUInt32 numBytes);

  /** \details
    Writes the byte to the current position of the file pointer
    for this StreamBuf object, and increments the pointer.
    \param value [in]  Byte to write.
  */
  virtual void putByte(OdUInt8 value);

  /** \details
    Writes the specified number of bytes, starting at the current position of the file pointer
    for this StreamBuf object, and increments the pointer by numBytes.
    \param buffer [in]  Character buffer.
    \param numBytes [in]  Number of bytes.
  */
  virtual void putBytes(const void* buffer, OdUInt32 numBytes);

  /** \details
    Sets the physical file size for the specified file associated with this 
    StreamBuf object to the current position of the file pointer.
  */
  virtual void truncate();

   /** \details
    Copies the specified bytes from this StreamBuf object to the specified StreamBuf object.
    \param pDestination [in]  Pointer to the StreamBuf object to receive the data.
    \param sourceStart [in]  Starting position of the file pointer of this StreamBuf object.
    \param sourceEnd [in]  Ending position of the file pointer of this StreamBuf object.
    \remarks
    Bytes are copied from sourceStart to sourceEnd inclusive.
   */
 virtual void copyDataTo(OdStreamBuf* pDestination, OdUInt64 sourceStart, OdUInt64 sourceEnd);
};

class OdRdFileBuf;

/** \remarks
  This template class is a specialization of the OdSmartPtr class for OdRdFileBuf object pointers.
*/
typedef OdSmartPtr<OdRdFileBuf> OdRdFileBufPtr;

/** \details
  This class implements file input.
  \sa
  Source code provided.
  <group ExServices_Classes>
*/
class OdRdFileBuf : public OdBaseFileBuf
{
  OdRdFileBuf(const OdRdFileBuf& source);
  OdRdFileBuf& operator = (const OdRdFileBuf& source);

protected:
  HANDLE      m_hFileMap;
  LPVOID      m_pFileMap;
  ULARGE_INTEGER  m_ulSize;
  ULARGE_INTEGER  m_ulPos;

  /*!DOM*/
  inline bool memBufferUsed() const { return m_pFileMap != 0; }

public:
  OdRdFileBuf(const OdString& filename)
  {
    m_hFileMap = INVALID_HANDLE_VALUE;
    m_pFileMap = NULL;
    m_ulSize.QuadPart = 0;
    m_ulPos.QuadPart = 0;

    open(filename);
  }

  /** \details
    \param filename [in]  Filename.
    \param shareMode [in]  Share mode.
    \param accessMode [in]  Access mode.
    \param creationDisposition [in]  Creation disposition.
    \remarks
    Opens filename if specified.
  */
  OdRdFileBuf(
    const OdString& filename,
    Oda::FileShareMode shareMode,
    Oda::FileAccessMode accessMode,
    Oda::FileCreationDisposition creationDisposition)
  {
    m_hFileMap = INVALID_HANDLE_VALUE;
    m_pFileMap = NULL;
    m_ulSize.QuadPart = 0;
    m_ulPos.QuadPart = 0;

    open(filename, shareMode, accessMode, creationDisposition);
  }

  OdRdFileBuf();

  ~OdRdFileBuf() { close(); }

  /** \param filename [in]  Filename.
    \param shareMode [in]  Share mode.
    \param accessMode [in]  Access mode.
    \param creationDisposition [in]  Creation disposition.
    \remarks
    Opens filename if specified.
  */
  static OdRdFileBufPtr createObject()
  {
    return OdRdFileBufPtr(new OdRdFileBuf(), kOdRxObjAttach);
  }

  static OdRdFileBufPtr createObject(
    const OdString& filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyWrite,
    Oda::FileAccessMode accessMode = Oda::kFileRead,
    Oda::FileCreationDisposition creationDisposition = Oda::kOpenExisting)
  {
    return OdRdFileBufPtr(new OdRdFileBuf(filename, shareMode, accessMode, creationDisposition), kOdRxObjAttach);
  }

  virtual void open(const OdString& filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyWrite,
    Oda::FileAccessMode accessMode = Oda::kFileRead,
    Oda::FileCreationDisposition creationDisposition = Oda::kOpenExisting);
#ifndef TD_UNICODE
  virtual void open(const OdCharW* filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyWrite,
    Oda::FileAccessMode accessMode = Oda::kFileRead,
    Oda::FileCreationDisposition creationDisposition = Oda::kOpenExisting);
#endif

  virtual void close();

  virtual OdUInt64 length();
  virtual OdUInt64 seek(OdInt64 offset, OdDb::FilerSeekType seekType);
  virtual OdUInt64 tell();
  virtual bool isEof();
  virtual OdUInt8 getByte();
  virtual void getBytes(void* buffer, OdUInt32 numBytes);
  virtual void putByte(OdUInt8 value);
  virtual void putBytes(const void* buffer, OdUInt32 numBytes);
  virtual void truncate();

  virtual void copyDataTo(OdStreamBuf* pDestination, OdUInt64 sourceStart, OdUInt64 sourceEnd);
};

#define WRITING_BUFFER_LENGTH 1024*8
class OdWrFileBuf;

/** \remarks
  This template class is a specialization of the OdSmartPtr class for OdWrFileBuf object pointers.
*/
typedef OdSmartPtr<OdWrFileBuf> OdWrFileBufPtr;

/** \details
  This class implements file output.
  \sa
  Source code provided.
  <group ExServices_Classes>
*/
class OdWrFileBuf : public OdBaseFileBuf
{
  OdWrFileBuf(const OdWrFileBuf& source);
  OdWrFileBuf& operator = (const OdWrFileBuf& source);
  OdUInt8 m_pBuffer[WRITING_BUFFER_LENGTH+1]; // +1 is to suppress /analyse false positive
  OdUInt32 m_nBufferedSize;
  inline void flush()
  {
    if ( m_nBufferedSize == 0 )
      return;
    OdBaseFileBuf::putBytes(m_pBuffer, m_nBufferedSize);
    m_nBufferedSize = 0;
  }

public:

  /** \details
    \param filename [in]  Filename.
    \param shareMode [in]  Share mode.
    \param accessMode [in]  Access mode.
    \param creationDisposition [in]  Creation disposition.
    \remarks
    Opens filename if specified.
  */
  OdWrFileBuf(const OdString& filename):m_nBufferedSize(0) { open(filename); }
  OdWrFileBuf(
    const OdString& filename,
    Oda::FileShareMode shareMode,
    Oda::FileAccessMode accessMode,
    Oda::FileCreationDisposition creationDisposition):m_nBufferedSize(0)
  {
    open(filename, shareMode, accessMode, creationDisposition);
  }

  OdWrFileBuf();

  ~OdWrFileBuf();

  /** \param filename [in]  Filename.
    \param shareMode [in]  Share mode.
    \param accessMode [in]  Access mode.
    \param creationDisposition [in]  Creation disposition.
    \remarks
    Opens filename if specified.
  */
  static OdWrFileBufPtr createObject()
  {
    return OdWrFileBufPtr(new OdWrFileBuf(), kOdRxObjAttach);
  }
  static OdWrFileBufPtr createObject(const OdString& filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyNo,
    Oda::FileAccessMode accessMode = Oda::kFileWrite,
    Oda::FileCreationDisposition creationDisposition = Oda::kCreateAlways)
  {
    return OdWrFileBufPtr(new OdWrFileBuf(filename, shareMode, accessMode, creationDisposition), kOdRxObjAttach);
  }

  virtual void close();
  virtual OdUInt64 seek(OdInt64 offset, OdDb::FilerSeekType seekType);
  virtual void copyDataTo(OdStreamBuf* pDestination, OdUInt64 sourceStart, OdUInt64 sourceEnd);
  virtual OdUInt64 tell();
  virtual OdUInt64 length();

  virtual void putByte(OdUInt8 value);
  virtual void putBytes(const void* buffer, OdUInt32 numBytes);
  virtual OdUInt8 getByte();
  virtual void getBytes(void* buffer, OdUInt32 numBytes);

  virtual void open(const OdString& filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyNo,
    Oda::FileAccessMode accessMode = Oda::kFileWrite,
    Oda::FileCreationDisposition creationDisposition = Oda::kCreateAlways);
#ifndef TD_UNICODE
  virtual void open(const OdCharW* filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyNo,
    Oda::FileAccessMode accessMode = Oda::kFileWrite,
    Oda::FileCreationDisposition creationDisposition = Oda::kCreateAlways);
#endif
};

#else // #ifdef WIN32

#include <stdio.h>
#include <memory>
#include "OdString.h"
#include "RxObjectImpl.h"
#include "OdStreamBuf.h"
#include "RxSystemServices.h"

#define ERR_VAL ((OdUInt64)-1)

/** <group ExServices_Classes>
*/
class FileToRemoveOnClose
{
  char* fName;
public:
  void setFileName(const char* fileName)
  {
    free(fName);
    fName = (char*)malloc(strlen(fileName)+1);
    strcpy(fName, fileName);
  }
  FileToRemoveOnClose():fName(NULL){ }
  ~FileToRemoveOnClose()
  { 
    if ( fName && strlen(fName)>0 )
    {
#ifdef OD_HAVE_REMOVE_FUNC
      remove(fName);
#else
      DeleteFile(OdString(fName).c_str());
#endif
    }
    free(fName);
  }
};

class OdBaseFileBuf : public OdRxObjectImpl<OdStreamBuf>
{
  OdBaseFileBuf(const OdBaseFileBuf&);
  OdBaseFileBuf& operator = (const OdBaseFileBuf&);
  std::auto_ptr< FileToRemoveOnClose > m_pFtr;

public:
          OdBaseFileBuf();
  virtual ~OdBaseFileBuf(){ close(); };

  virtual void      open(
    const OdString& filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyNo,
    Oda::FileAccessMode accessMode = Oda::FileAccessMode(Oda::kFileRead | Oda::kFileWrite),
    Oda::FileCreationDisposition creationDisposition = Oda::kOpenExisting);

         virtual void      close();
  inline virtual OdString  fileName()     { return m_FileName; }
         virtual OdUInt8   getByte();
         virtual void      getBytes(void* buffer, OdUInt32 numBytes);
         virtual void      putByte(OdUInt8 value);
         virtual void      putBytes(const void* buffer, OdUInt32 numBytes);
  inline virtual OdUInt64  length()       { return m_length; };
//         virtual OdUInt64  length();
  inline virtual OdUInt32  getShareMode() { return (OdUInt32)m_shMode; };
  inline virtual OdUInt64  tell()         { return m_position; };
  inline virtual bool      isEof()        { return (m_position >= m_length); };
         virtual OdUInt64  seek(OdInt64 offset, OdDb::FilerSeekType seekType);
         virtual void      truncate();

protected:
  FILE *              m_fp;
  OdString            m_FileName;
  OdUInt64            m_length;
  OdUInt64            m_position;
  Oda::FileShareMode  m_shMode;
  bool                m_prevWasRead;

  void Unlink(const char* fileName){ m_pFtr->setFileName(fileName); }
};

class OdWrFileBuf;
typedef OdSmartPtr<OdWrFileBuf> OdWrFileBufPtr;

class OdWrFileBuf : public OdBaseFileBuf
{
  OdWrFileBuf(const OdWrFileBuf& source);
  OdWrFileBuf& operator = (const OdWrFileBuf& source);

public:
  OdWrFileBuf(const OdString& filename) { open(filename); }
  OdWrFileBuf(const OdString& filename, Oda::FileShareMode shareMode) { open(filename, shareMode); }
  OdWrFileBuf(const OdString& filename, Oda::FileShareMode shareMode, Oda::FileAccessMode accessMode, Oda::FileCreationDisposition creationDisposition) 
  { 
    open(filename, shareMode, accessMode, creationDisposition); 
  }
  OdWrFileBuf(){};
  ~OdWrFileBuf(){ close(); };

  static OdWrFileBufPtr createObject()
  {
    return OdWrFileBufPtr(new OdWrFileBuf(), kOdRxObjAttach);
  }
  static OdWrFileBufPtr createObject(const OdString& filename, Oda::FileShareMode shareMode = Oda::kShareDenyNo)
  {
    return OdWrFileBufPtr(new OdWrFileBuf(filename, shareMode), kOdRxObjAttach);
  }

  virtual void open(const OdString& filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyNo,
    Oda::FileAccessMode accessMode = Oda::kFileWrite,
    Oda::FileCreationDisposition creationDisposition = Oda::kCreateAlways);
};


#define NUM_BUFFERS 8

class OdRdFileBuf;
typedef OdSmartPtr<OdRdFileBuf> OdRdFileBufPtr;

class OdRdFileBuf : public OdBaseFileBuf
{
  OdRdFileBuf(const OdRdFileBuf&);
  OdRdFileBuf& operator = (const OdRdFileBuf&);

public:
  OdRdFileBuf(const OdString& filename) : m_Counter(0L) { init(); open(filename); }
  OdRdFileBuf(const OdString& filename, Oda::FileShareMode shareMode) : m_Counter(0L)
  {
    init();
    open(filename, shareMode);
  }
  OdRdFileBuf(const OdString& filename, Oda::FileShareMode shareMode, Oda::FileAccessMode accessMode, Oda::FileCreationDisposition creationDisposition) : m_Counter(0L)
  {
    init();
    open(filename, shareMode, accessMode, creationDisposition);
  }
  OdRdFileBuf();
  static OdRdFileBufPtr createObject()
  {
    return OdRdFileBufPtr(new OdRdFileBuf(), kOdRxObjAttach);
  }
  static OdRdFileBufPtr createObject(const OdString& filename, Oda::FileShareMode shareMode = Oda::kShareDenyNo)
  {
    return OdRdFileBufPtr(new OdRdFileBuf(filename, shareMode), kOdRxObjAttach);
  }
  ~OdRdFileBuf(){ close(); };

  virtual void open(
    const OdString& filename,
    Oda::FileShareMode shareMode = Oda::kShareDenyNo,
    Oda::FileAccessMode accessMode = Oda::kFileRead,
    Oda::FileCreationDisposition creationDisposition = Oda::kOpenExisting);

  virtual void close();

  virtual OdUInt64  seek(OdInt64 offset, OdDb::FilerSeekType seekType);
  virtual OdUInt64  tell();
  virtual bool      isEof();
  virtual OdUInt8   getByte();
  virtual void      getBytes(void* buffer, OdUInt32 numBytes);
  virtual void      putByte(OdUInt8 value) { ODA_FAIL();  throw OdError(eNotApplicable); };
  virtual void      putBytes(const void* buffer, OdUInt32 numBytes) { ODA_FAIL();  throw OdError(eNotApplicable); };
  virtual void      truncate();

protected:
  struct blockstru
  {
    OdUInt8*  buf;        /* this buffer */
    OdUInt64  startaddr;  /* address from which it came in the file */
    int       validbytes; /* number of valid bytes it holds */
    OdInt32   counter;    /* least recently used counter */
  };

  OdUInt64  m_PhysFilePos; /* where the file pointer is */
  OdUInt64  m_BufPos;      /* position from which buf was filled */
  int       m_BytesLeft;   /* bytes left in buf */
  int       m_BufBytes;    /* valid bytes read into buffer */
  OdUInt8*  m_pNextChar;   /* pointer to next char in buffer */
  OdUInt8*  m_pCurBuf;     /* pointer to the buffer currently being used */
  int       m_UsingBlock;  /* which block is currently in use */
  struct blockstru         /* the data being held */
            m_DataBlock[NUM_BUFFERS];

  static const int m_BufSize; /* size of each read buffer */
  static const int  m_PosMask; /* mask to allow position check */
  OdInt32    m_Counter;

  bool filbuf();
  void init();
};

#endif // #ifdef WIN32
#include "TD_PackPop.h"
#endif // ODFILEBUF_DEFINED
