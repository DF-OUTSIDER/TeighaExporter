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




// FlatMemStream.h: interface for the OdMemoryStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ODFLATMEMSTREAM_H_INCLUDED_)
#define _ODFLATMEMSTREAM_H_INCLUDED_

#include "TD_PackPush.h"

#include "OdStreamBuf.h"

class OdFlatMemStream;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdFlatMemStream object pointers.
*/
typedef OdSmartPtr<OdFlatMemStream> OdFlatMemStreamPtr;

/** \details
    Default formal parameter for the lightweight template instaniations.
    OdFlatMemStreamImpl<OdStreamBuf> -- instaniation with OdStreamBuf
    OdFlatMemStreamImpl<> -- lightweight instaniation without abstract interface overhead
    <group Other_Classes>
*/
class OdEmptyBaseClass {};

template <class TBase = OdEmptyBaseClass>
class OdStreamPosImpl : public TBase {
protected:
  OdUInt64  m_nEndPos;
  OdUInt64  m_nCurPos;
public:
  OdUInt64 left() { return m_nEndPos - m_nCurPos; }
  OdUInt64 length() { return m_nEndPos; }
  OdUInt64 tell() { return m_nCurPos; }
  OdUInt64 seek(OdInt64 offset, OdDb::FilerSeekType seekType);
  bool isEof() { return left()==0; }
};

/** \details
    This class implements memory-resident I/O objects.
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
template <class TBase = OdEmptyBaseClass>
class OdFlatMemStreamImpl : public OdStreamPosImpl<TBase> {
protected:
  using OdStreamPosImpl<TBase>::m_nEndPos;
  using OdStreamPosImpl<TBase>::m_nCurPos;
  void*     m_pMemData;
public:
  /** \details
    Returns the raw data associated with this FlatMemStream object.
  */
  OdUInt8* data() { return (OdUInt8*)m_pMemData; }

  using OdStreamPosImpl<TBase>::left;
protected:
  /** \details
    Appends the specified number of bytes to this FlatMemStream object.
    \param numBytes [in]  Number of bytes.
    \remarks
    The default implementation of this function does nothing but throw an eNotOpenForWrite exception
  */
  virtual void append(OdUInt64 numBytes) { throw OdError(eNotOpenForWrite); }

  /** \param buffer [in]  Character buffer.
    \param numBytes [in]  Number of bytes.
    \param curPosition [in]  Initial position of the file pointer.
  */
public:
  inline OdFlatMemStreamImpl() { init(0, 0, 0); }

  OdFlatMemStreamImpl(void* buffer, OdUInt64 numBytes, OdUInt64 nCurPos)
  { init(buffer, numBytes, nCurPos); }

  /** \details
    Initializes this FlatMemStream object.
    \param buffer [in]  Character buffer.
    \param numBytes [in]  Number of bytes.
    \param curPosition [in]  Initial position of the file pointer of the new object.
  */
  inline void init(void* buffer, OdUInt64 numBytes, OdUInt64 curPosition = 0);

  OdUInt8 getByte();
  void getBytes(void* buffer, OdUInt32 numBytes);
  
  // overrides existing byte(s)
  void putByte(OdUInt8 value);
  void putBytes(const void* buffer, OdUInt32 numBytes);
};


/** \details
    This class implements memory-resident I/O objects.
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdFlatMemStream : public OdFlatMemStreamImpl<OdStreamBuf>
{
protected:
  inline OdFlatMemStream() {}
public:
  ODRX_DECLARE_MEMBERS(OdFlatMemStream);
  /** \details
    Creates a new FlatMemStream object, and returns a SmartPointer to the new object.
    \param buffer [in]  Character buffer.
    \param numBytes [in]  Number of bytes.
    \param curPosition [in]  Initial position of the file pointer of the new object.
  */
  static OdFlatMemStreamPtr createNew(void* buffer, OdUInt64 numBytes, OdUInt64 curPosition = 0);

  void copyDataTo(OdStreamBuf* pDestination, OdUInt64 sourceStart, OdUInt64 sourceEnd);
};


/** \details
This class implements memory-resident stream buffer holding its own memory.
Corresponding C++ library: TD_Root
<group Other_Classes>
*/
class FIRSTDLL_EXPORT OdFlatMemStreamManaged : public OdFlatMemStream
{
  void init(OdUInt64 numBytes) { OdFlatMemStream::init(odrxAlloc((size_t)numBytes), numBytes); }
public:
  ~OdFlatMemStreamManaged() { ::odrxFree(m_pMemData); }
  /** \details
  Creates a new FlatMemStream object, and returns a SmartPointer to the new object.
  \param numBytes [in]  Number of bytes.
  \remarks
  Memory is allocated via odrxAlloc() and is freed via odrxFree() in destructor;
  */
  static OdFlatMemStreamPtr createNew(OdUInt64 numBytes);
};


template<class TBase>
inline void OdFlatMemStreamImpl<TBase>::init(void* pMemData, OdUInt64 nSize, OdUInt64 nCurPos) {
  m_pMemData = pMemData;
  m_nEndPos = nCurPos + nSize;
  m_nCurPos = nCurPos;
}


template<class TBase>
inline OdUInt64 OdStreamPosImpl<TBase>::seek(OdInt64 offset, OdDb::FilerSeekType whence) {
  OdUInt64 nNewPos;
  switch(whence) {
  case OdDb::kSeekFromEnd:
    nNewPos = m_nEndPos + offset;
    break;
  case OdDb::kSeekFromCurrent:
    nNewPos = m_nCurPos + offset;
    break;
  case OdDb::kSeekFromStart:
    nNewPos = offset;
    break;
  default:
    throw OdError(eInvalidInput);
    break;
  };
  if(nNewPos > m_nEndPos)
    throw OdError(eEndOfFile);
  return (m_nCurPos = nNewPos);
}

template<class TBase>
inline OdUInt8 OdFlatMemStreamImpl<TBase>::getByte() {
  if(left()==0)
    throw OdError(eEndOfFile);
  return *(data() + m_nCurPos++);
}

template<class TBase>
inline void OdFlatMemStreamImpl<TBase>::getBytes(void* buffer, OdUInt32 nLen) {
  if(left() < nLen)
    throw OdError(eEndOfFile);
  ::memcpy(buffer, data()+m_nCurPos, nLen);
  m_nCurPos += nLen;
}

template<class TBase>
inline void OdFlatMemStreamImpl<TBase>::putByte(OdUInt8 val) {
  if(m_nCurPos < m_nEndPos)
    *(data() + m_nCurPos++) = val;
  else
    throw OdError(eEndOfFile);
}

template<class TBase>
inline void OdFlatMemStreamImpl<TBase>::putBytes(const void* buffer, OdUInt32 nLen) {
  if(m_nCurPos + nLen <= m_nEndPos) {
    ::memcpy(data()+m_nCurPos, buffer, nLen);
    m_nCurPos += nLen;
  } else {
    throw OdError(eEndOfFile);
  }
}


#include "TD_PackPop.h"

#endif // !defined(_ODFLATMEMSTREAM_H_INCLUDED_)

