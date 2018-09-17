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




#ifndef ODDBHANDLE
#define ODDBHANDLE /*!DOM*/

#include "TD_PackPush.h"

#include "DbRootExport.h"
#include "OdArrayPreDef.h"
#include "OdaDefs.h"
#include "OdString.h"

/** \details
  Returns the decimal value of a hexadecimal digit.
  
  \param hexDigit [in]  Hex digit.
  
  Examples:  
  getHexValue('A') returns 10, etc.
*/
DBROOT_EXPORT int getHexValue(
  int hexDigit);

/** \details
  Returns the 64 bit integer value corresponding to the passed in string.
  \param pStr [in] String representation of an integer.
*/
//TOOLKIT_EXPORT OdInt64 atoi64(const OdChar* pStr);
//TOOLKIT_EXPORT void    i64toA(const OdInt64 &val, OdChar* pStr);

/** \details
    This class encapsulates the 64-bit integer handles associated with OdDbObject instances.
    
    Corresponding C++ library: TD_Db
    
    \remarks
    Handles are persistent and unique within a database. Handles are not, 
	however, unique between databases.
    
    <group OdDb_Classes>
*/
class DBROOT_EXPORT OdDbHandle
{
public:
  OdDbHandle() : m_val(0) {}

  OdDbHandle(const OdDbHandle& value) : m_val(value.m_val) {}

  OdDbHandle(OdUInt64 value) : m_val(value) {}
  
  OdDbHandle(const OdChar* value) { *this = value; }
  OdDbHandle(const char* value) { *this = value; }
  OdDbHandle(const OdString& value) { *this = (const OdChar*)value; }
  OdDbHandle(const OdAnsiString& value) { *this = (const char*)value; }
  OdDbHandle(int value) : m_val(value) {}
#if OD_SIZEOF_LONG == 4
  OdDbHandle(OdInt32 value) : m_val(value) {}
#endif
  OdDbHandle& operator=(OdUInt64 value) 
  {
    m_val = value;
    return *this;
  }
  
  OdDbHandle& operator=(const OdChar* value);
  OdDbHandle& operator=(const char* value);

  OdDbHandle& operator=(const OdDbHandle& value)
  {
    m_val = value.m_val;
    return *this;
  }
  OdDbHandle& operator=(int value)
  {
    m_val = value;
    return *this;
  }

  operator OdUInt64() const { return m_val; }
  
  /** \details
    Returns the hexidecmal representation of this Handle object as a null terminated string.
    
    \param pBuf [in]  Pointer to the buffer to receive the string.
    
    \remarks
    The buffer must be at least 17 bytes long.
  */
    void getIntoAsciiBuffer(
    OdChar* pBuf) const;
    
  /** \details
    Returns the hexidecmal representation of this Handle object as an OdString.
  */
  OdString ascii() const;
  /** \details
    Returns true and only if this Handle object is null.
  */
  bool isNull() const { return m_val == 0; }
  
  bool operator == (
    OdUInt64 value) const { return m_val == value; }
  
  bool operator != (
    OdUInt64 value) const { return m_val != value; }
  
  bool operator > (
    OdUInt64 value) const { return m_val > value; }
  
  bool operator < (
    OdUInt64 value) const { return m_val < value; }
  
  bool operator <= (
    OdUInt64 value) const { return m_val < value || m_val == value; }
  
  bool operator >= (
    OdUInt64 value) const { return m_val > value || m_val == value; }
  
  OdDbHandle& operator+=(
    const OdInt64& value)
  {
    m_val += value;
    ODA_ASSERT_ONCE(!isNull()); // check handle stuff overflow
    return *this;
  }
  OdDbHandle operator+(
    const OdInt64& value)
  {
    OdDbHandle res;
    res.m_val = m_val + value;
    return res;
  }

  /** \details
    Returns the this Handle object as an array OdUInt8[8].
    
    \param bytes [in]  array of bytes to receive the handle.
    
    \remarks
    The low order byte is returned in bytes[0].
    
    \remarks
    The buffer must be at least 17 bytes long.
  */
    void bytes(
    OdUInt8 * bytes) const
  {
    OdUInt64 val = m_val;
    *bytes++ = (OdUInt8)(val & 0xFF);
    val >>= 8;
    *bytes++ = (OdUInt8)(val & 0xFF);
    val >>= 8;
    *bytes++ = (OdUInt8)(val & 0xFF);
    val >>= 8;
    *bytes++ = (OdUInt8)(val & 0xFF);
    val >>= 8;
    *bytes++ = (OdUInt8)(val & 0xFF);
    val >>= 8;
    *bytes++ = (OdUInt8)(val & 0xFF);
    val >>= 8;
    *bytes++ = (OdUInt8)(val & 0xFF);
    val >>= 8;
    *bytes = (OdUInt8)(val & 0xFF);
  }

private:
    OdUInt64  m_val;
};


/** \details
  This template class is a specialization of the OdArray class for OdDbHandle objects.
*/
typedef OdArray<OdDbHandle> OdHandleArray;

#include "TD_PackPop.h"

#endif

