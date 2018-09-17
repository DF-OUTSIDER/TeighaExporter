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




#ifndef ODSTREAMBUF_DEFINED
#define ODSTREAMBUF_DEFINED

#include "RxObject.h"

#include "TD_PackPush.h"

class OdString;

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  enum FilerSeekType
  {
    kSeekFromStart   = SEEK_SET, // Start of file.
    kSeekFromCurrent = SEEK_CUR, // Current position of file pointer.
    kSeekFromEnd     = SEEK_END  // End of file.
  };
}

/** \details
    This class is the base class for classes that provide platform-dependent I/O for Teigha.
    
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdStreamBuf : public OdRxObject
{
protected:
  OdStreamBuf();
public:
  ODRX_DECLARE_MEMBERS(OdStreamBuf);

  /** \details
    Returns the name of the file associated with this StreamBuf object.
  */
  virtual OdString  fileName();

  /** \details
    Returns true if and only if the current position of the file pointer for
    this StreamBuf object is at the end of file.
  */
  virtual bool      isEof();
  /** \details
    Returns the current position of the file pointer for
    this StreamBuf object.
  */
  virtual OdUInt64  tell();
  /** \details
    Returns the length in bytes of the file associated with
    this StreamBuf object.
  */
  virtual OdUInt64  length();

  /** \details
    Sets the length of the file associated with
    this StreamBuf object to zero.
  */
  virtual void      truncate();
  /** \details
    Sets the current position of the file pointer for
    this StreamBuf object to zero.
  */
  virtual void      rewind();
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
  virtual OdUInt64  seek(OdInt64 offset, OdDb::FilerSeekType seekType);

  /** \details
    Returns the byte at the current position of the file pointer
    for this StreamBuf object, and increments the pointer.
  */
  virtual OdUInt8   getByte();
  /** \details
    Returns the specified number of bytes, starting at the current position of the file pointer
    for this StreamBuf object, and increments the pointer by numBytes.
    \param buffer [in]  Character buffer.
    \param numBytes [in]  Number of bytes.
  */
  virtual void      getBytes(void* buffer, OdUInt32 numBytes);

  /** \details
    Writes the byte to the current position of the file pointer
    for this StreamBuf object, and increments the pointer.
    \param value [in]  Byte to write. 
  */
  virtual void      putByte(OdUInt8 value);
  /** \details
    Writes the specified number of bytes, starting at the current position of the file pointer
    for this StreamBuf object, and increments the pointer by numBytes.
    \param buffer [in]  Character buffer.
    \param numBytes [in]  Number of bytes.
  */
  virtual void      putBytes(const void* buffer, OdUInt32 numBytes);

  /** \details
    Copies the specified bytes from this StreamBuf object to the specified StreamBuf object.
    \param pDestination [in]  Pointer to the StreamBuf object to receive the data.
    \param sourceStart [in]  Starting position of the file pointer of this StreamBuf object.
    \param sourceEnd [in]  Ending position of the file pointer of this StreamBuf object.
    \remarks
    Bytes are copied from sourceStart to sourceEnd inclusive.
  */
  virtual void      copyDataTo(OdStreamBuf* pDestination, OdUInt64 sourceStart = 0, OdUInt64 sourceEnd = 0);
  /** \details
    Returns the share mode for this StreamBuf object.
    
    \remarks
    getShareMode returns one of the following:
    
    <table>
    Name                         Value       Description
    Oda::kShareDenyReadWrite     0x10        deny read/write mode
    Oda::kShareDenyWrite         0x20        deny write mode
    Oda::kShareDenyRead          0x30        deny read mode
    Oda::kShareDenyNo            0x40        deny none mode
    </table>
  */
  virtual OdUInt32  getShareMode();
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdStreamBuf object pointers.
*/
typedef OdSmartPtr<OdStreamBuf> OdStreamBufPtr;

#include "TD_PackPop.h"

#endif //ODSTREAMBUF_DEFINED

