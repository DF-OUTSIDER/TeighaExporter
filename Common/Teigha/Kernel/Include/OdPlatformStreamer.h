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


#ifndef _OD_PLATFORMSTREAMER_H_
#define _OD_PLATFORMSTREAMER_H_

#include "OdPlatform.h"
#include "OdStreamBuf.h"
#include "OdString.h"

/** \details
    This class implements platform dependent stream I/O for various data types.
    <group Other_Classes>
*/
class OdPlatformStreamer
{
public:
  /** \details
    Reads a 16-bit integer from the specified StreamBuf object.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
  */
  static OdInt16    rdInt16   (OdStreamBuf& streamBuf);
  /** \details
    Reads a 32-bit integer from the specified StreamBuf object.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
  */
  static OdInt32    rdInt32   (OdStreamBuf& streamBuf);
  /** \details
    Reads a 64-bit integer from the specified StreamBuf object.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
  */
  static OdInt64    rdInt64   (OdStreamBuf& streamBuf);
  /** \details
    Reads a 32-bit floating point number from the specified StreamBuf object.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
  */
  static float      rdFloat   (OdStreamBuf& streamBuf);
  /** \details
    Reads a double from the specified StreamBuf object.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
  */
  static double     rdDouble  (OdStreamBuf& streamBuf);

  /** \details
    Reads two doubles from the specified StreamBuf object.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
    \param doubles [out]  Array of doubles to receive the data.
  */
  static void       rd2Doubles(OdStreamBuf& streamBuf, void* doubles);

  /** \details
    Reads three doubles from the specified StreamBuf object.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
    \param doubles [out]  Array of doubles to receive the data.
  */
  static void       rd3Doubles(OdStreamBuf& streamBuf, void* doubles);
  
  /** \details
    Reads the specified number of doubles from the specified StreamBuf object.
    \param streamBuf [in]  StreamBuf object from which the data are to be read.
    \param doubles [out]  Array of doubles to receive the data.
    \param numDoubles [in]  Number of doubles.
  */
  static void       rdDoubles (OdStreamBuf& streamBuf, int numDoubles, void* doubles);

  /** \details
    Writes a 16-bit integer to the specified StreamBuf object.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    \param value [in]  Value to be written.
  */
  static void       wrInt16   (OdStreamBuf& streamBuf, OdInt16 value);
  /** \details
    Writes a 32-bit integer to the specified StreamBuf object.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    \param value [in]  Value to be written.
  */
  static void       wrInt32   (OdStreamBuf& streamBuf, OdInt32 value);
  /** \details
    Writes a 64-bit integer to the specified StreamBuf object.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    \param value [in]  Value to be written.
  */
  static void       wrInt64   (OdStreamBuf& streamBuf, OdInt64 value);
  /** \details
    Writes a 32-bit floating point number to the specified StreamBuf object.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    \param value [in]  Value to be written.
  */
  static void       wrFloat   (OdStreamBuf& out, float val);
  /** \details
    Writes a double to the specified StreamBuf object.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    \param value [in]  Value to be written.
  */
  static void       wrDouble  (OdStreamBuf& streamBuf, double value);
  /** \details
    Writes two doubles to the specified StreamBuf object.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    \param doubles [in]  Array of doubles to be written.
  */
  static void       wr2Doubles(OdStreamBuf& streamBuf, const void* doubles);
  /** \details
    Writes three doubles to the specified StreamBuf object.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    \param doubles [in]  Array of doubles to be written.
  */
  static void       wr3Doubles(OdStreamBuf& streamBuf, const void* doubles);
  /** \details
    Writes the specified number of doubles to the specified StreamBuf object.
    \param streamBuf [out]  StreamBuf object to which the data are to be written.
    \param doubles [in]  Array of doubles to be written.
    \param numDoubles [in]  Number of doubles.
  */
  static void       wrDoubles (OdStreamBuf& streamBuf, int numDoubles, const void* doubles);

  /** \details
      Extracts a Unicode string from buffer, and returns it as an OdString.

      \param buffer [in/out] Buffer pointer to read the Unicode string from.
      \param size [in] Length of the Unicode string in characters (assumes 2 byte characters in buffer
               stored in little endian format), or -1 if the string is null terminated.

      \remarks
      The buffer pointer is incremented to the next byte past the string that
      was read in.  If size is > 0, size * 2 bytes will be read.  If size is -1,
      a null terminated string will be read from buffer, including the null byte.
  */
  inline static OdString getUnicodeStrFromBuffer(const OdUInt8*& buffer, OdInt32 size);

  /** \details
      Writes a Unicode string to the passed in buffer, in little endian format, 
      using 2 bytes per character.  The trailing null character is NOT written.

      \param val [in]  Unicode string to be written.
      \param buffer [in]  Char buffer to which will receive the Unicode data.

      \remarks
      The buffer pointer is incremented to the next byte past the string that
      was written. 
  */
  inline static void putUnicodeStrToBuffer(const OdString& val, OdUInt8*& buffer);

  /** \details
    Same as putUnicodeStrToBuffer, but writes to a stream, prepending string length.
  */
  inline static void wrString(OdStreamBuf& streamBuf, const OdString& val);
  
  /** \details
    Inverse of wrString.
  */
  inline static OdString rdString(OdStreamBuf& streamBuf);
};

inline OdInt16 OdPlatformStreamer::rdInt16(OdStreamBuf& streamBuf)
{
  OdInt16 res;
  streamBuf.getBytes(&res, sizeof(res));
  odSwap2BytesNumber(res);
  return res;
}
inline OdInt32 OdPlatformStreamer::rdInt32(OdStreamBuf& streamBuf)
{
  OdInt32 res;
  streamBuf.getBytes(&res, sizeof(res));
  odSwap4BytesNumber(res);
  return res;
}
inline OdInt64 OdPlatformStreamer::rdInt64(OdStreamBuf& streamBuf)
{
  OdInt64 res;
  streamBuf.getBytes(&res, sizeof(res));
  odSwap8Bytes(&res);
  return res;
}
inline float OdPlatformStreamer::rdFloat(OdStreamBuf& streamBuf)
{
  float res;
  streamBuf.getBytes(&res, sizeof(res));
  odSwap4Bytes(&res);
  return res;
}
inline double OdPlatformStreamer::rdDouble(OdStreamBuf& streamBuf)
{
  double res;
  ODA_ASSUME(sizeof(res) == 8)
  streamBuf.getBytes(&res, 8);
  odSwap8Bytes(&res);
  // if unnormalized or NaN or infinity, set it to 0.0
  if (!isValidNonZeroIEEEDouble((OdUInt8 *)&res))
    return 0.0;
  return res;
}

inline void OdPlatformStreamer::rd2Doubles(OdStreamBuf& streamBuf, void* pRes2Doubles)
{
  ODA_ASSUME(sizeof(double) == 8)
  streamBuf.getBytes(pRes2Doubles, sizeof(double)*2);
  fixDouble((double*)pRes2Doubles);
  fixDouble((double*)pRes2Doubles+1);
}
inline void OdPlatformStreamer::rd3Doubles(OdStreamBuf& streamBuf, void* pRes3Doubles)
{
  ODA_ASSUME(sizeof(double) == 8)
  streamBuf.getBytes(pRes3Doubles, sizeof(double)*3);
  fixDouble((double*)pRes3Doubles);
  fixDouble((double*)pRes3Doubles+1);
  fixDouble((double*)pRes3Doubles+2);
}
inline void OdPlatformStreamer::rdDoubles(OdStreamBuf& streamBuf, int n, void* pResDoubles)
{
  ODA_ASSUME(sizeof(double) == 8)
  streamBuf.getBytes(pResDoubles, sizeof(double)*n);
  double* pD = (double*)pResDoubles;
  while (n--) { fixDouble(pD++); }
}

inline void OdPlatformStreamer::wrInt16(OdStreamBuf& streamBuf, OdInt16 val)
{
  odSwap2BytesNumber(val);
  streamBuf.putBytes(&val, sizeof(val));
}
inline void OdPlatformStreamer::wrInt32(OdStreamBuf& streamBuf, OdInt32 val)
{
  odSwap4BytesNumber(val);
  streamBuf.putBytes(&val, sizeof(val));
}
inline void OdPlatformStreamer::wrInt64(OdStreamBuf& streamBuf, OdInt64 val)
{
  odSwap8Bytes(&val);
  streamBuf.putBytes(&val, sizeof(val));
}
inline void OdPlatformStreamer::wrFloat(OdStreamBuf& streamBuf, float val)
{
  odSwap4Bytes(&val);
  streamBuf.putBytes(&val, sizeof(val));
}
inline void OdPlatformStreamer::wrDouble(OdStreamBuf& streamBuf, double val)
{
  ODA_ASSUME(sizeof(double) == 8)
  odSwap8Bytes(&val);
  streamBuf.putBytes(&val, sizeof(val));
}

inline OdString OdPlatformStreamer::getUnicodeStrFromBuffer(const OdUInt8*& buffer, OdInt32 size)
{
  OdString ret;
#if ODCHAR_IS_INT16LE
  if (size == -1)
  {
    // null terminated
    ret = (OdChar*)buffer;
    buffer += ((ret.getLength() + 1) * 2);
  }
  else
  {
    OdUInt8* pBuf = (OdUInt8*)ret.getBuffer(size+1);
    OD_BYTES_FROM_BUFFPTR(buffer, pBuf, (size<<1));
    ret.releaseBuffer(size);
  }
#else
  OdInt32 finalSize = size;
  if (size == -1 && buffer)
  {
    // null terminated
    size = 1; 
    OdUInt16* tmp16 = (OdUInt16*)buffer;
    for ( ; *tmp16; tmp16++)
      size++;
    finalSize = size - 1;
  }
  OdChar* pBuf = ret.getBuffer(size + 1);
  for (OdInt32 i = 0; i < size; i++)
  {
    *pBuf++ = OdUInt16(OD_INT16_FROM_BUFFPTR(buffer));
  }
  *pBuf = L'\0';
  ret.releaseBuffer(finalSize);
#endif

  return ret;
}

#ifndef ODCHAR_IS_INT16LE
#error "OdaDefs.h must be included before OdPlatformStreamer.h"
#endif

void OdPlatformStreamer::putUnicodeStrToBuffer(const OdString& val, OdUInt8*& buffer)
{
  OdInt32 nLen = val.getLength();
#if ODCHAR_IS_INT16LE
  OD_BYTES_TO_BUFFPTR(buffer, val.c_str(), nLen*sizeof(OdChar));
#else
  const OdChar* p = val.c_str();
  for (OdInt32 i = 0; i < nLen; i++)
  {
    OD_INT16_TO_BUFFPTR(buffer, (OdInt16)*p++);
  }
#endif
}

void OdPlatformStreamer::wrString(OdStreamBuf& streamBuf, const OdString& val)
{
  OdInt32 nLen = val.getLength();
  wrInt32(streamBuf, nLen);
#if ODCHAR_IS_INT16LE
  streamBuf.putBytes(val.c_str(), nLen * 2);
#else
  const OdChar* p = val.c_str();
  for (OdInt32 i = 0; i < nLen; ++i)
    wrInt16(streamBuf, (OdInt16)*p++);
#endif
}

OdString OdPlatformStreamer::rdString(OdStreamBuf& streamBuf)
{
  OdString ret;
  OdInt32 nLen = rdInt32(streamBuf);
  OdChar* pBuf = ret.getBufferSetLength(nLen);
#if ODCHAR_IS_INT16LE
  streamBuf.getBytes(pBuf, nLen * 2);
#else
  for (OdInt32 i = 0; i < nLen; ++i)
    *pBuf++ = rdInt16(streamBuf);
#endif
  return ret;
}


// Verify that preprocessor symbol ODCHAR_IS_INT16LE has been correctly defined
#if ODCHAR_IS_INT16LE
#ifdef ODA_BIGENDIAN
#error "ODCHAR_IS_INT16LE is defined for a big endian platform"
#endif
ODA_ASSUME(sizeof(OdChar) == 2);
#endif


#ifdef ODA_BIGENDIAN

inline void OdPlatformStreamer::wr2Doubles(OdStreamBuf& streamBuf, const void* p2Doubles)
{
  wrDouble(streamBuf, *(((double*)p2Doubles)+0));
  wrDouble(streamBuf, *(((double*)p2Doubles)+1));
}
inline void OdPlatformStreamer::wr3Doubles(OdStreamBuf& streamBuf, const void* p3Doubles)
{
  wrDouble(streamBuf, *(((double*)p3Doubles)+0));
  wrDouble(streamBuf, *(((double*)p3Doubles)+1));
  wrDouble(streamBuf, *(((double*)p3Doubles)+2));
}
inline void OdPlatformStreamer::wrDoubles(OdStreamBuf& streamBuf, int n, const void* pDoubles)
{
  while(n--) wrDouble(streamBuf, ((double*)pDoubles)[n]);
}

#else // #ifdef ODA_BIGENDIAN

inline void OdPlatformStreamer::wr2Doubles(OdStreamBuf& streamBuf, const void* p2Doubles)
{
  ODA_ASSUME(sizeof(double) == 8)
  streamBuf.putBytes(p2Doubles, sizeof(double)*2);
}
inline void OdPlatformStreamer::wr3Doubles(OdStreamBuf& streamBuf, const void* p3Doubles)
{
  ODA_ASSUME(sizeof(double) == 8)
  streamBuf.putBytes(p3Doubles, sizeof(double)*3);
}
inline void OdPlatformStreamer::wrDoubles(OdStreamBuf& streamBuf, int n, const void* pDoubles)
{
  ODA_ASSUME(sizeof(double) == 8)
  streamBuf.putBytes(pDoubles, sizeof(double) * n);
}

#endif // ODA_BIGENDIAN

#endif // _OD_PLATFORMSTREAMER_H_

