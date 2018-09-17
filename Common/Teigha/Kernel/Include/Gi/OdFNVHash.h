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

/*
 * Fowler/Noll/Vo hash
 *
 * The basis of this hash algorithm was taken from an idea sent
 * as reviewer comments to the IEEE POSIX P1003.2 committee by:
 *
 *      Phong Vo (http://www.research.att.com/info/kpv/)
 *      Glenn Fowler (http://www.research.att.com/~gsf/)
 *
 * In a subsequent ballot round:
 *
 *      Landon Curt Noll (http://www.isthe.com/chongo/)
 *
 * improved on their algorithm.  Some people tried this hash
 * and found that it worked rather well.  In an EMail message
 * to Landon, they named it the ``Fowler/Noll/Vo'' or FNV hash.
 *
 * FNV hashes are designed to be fast while maintaining a low
 * collision rate. The FNV speed allows one to quickly hash lots
 * of data while maintaining a reasonable collision rate.  See:
 *
 *      http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *
 * for more details as well as other forms of the FNV hash.
 *
 * Please do not copyright this code.  This code is in the public domain.
 *
 * LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * By:
 *	chongo <Landon Curt Noll> /\oo/\
 *      http://www.isthe.com/chongo/
 *
 * Share and Enjoy!	:-)
 */

#ifndef _OD_FNV1HASH_H_
#define _OD_FNV1HASH_H_

#include "OdString.h"

#include "TD_PackPush.h"

/** \details
  Perform a 32 bit Fowler/Noll/Vo hash on a buffer.

  \param buf [in]  Start of buffer to hash.
  \param len [in]  Length of buffer in octets.
  \param hval [in]  Previous hash value or initial basis if first call.

  \returns
  32 bit hash as a static hash type.

  \remarks
  To use the 32 bit FNV-0 historic hash, use 0 as the hval.
  To use the recommended 32 bit FNV-1 hash, use 0x811c9dc5 as the hval.
*/
inline OdUInt32 odFNV32HashBuf(const void *buf, size_t len, OdUInt32 hval = 0x811c9dc5)
{
  const OdUInt8 *bp = (const OdUInt8*)buf; // Start of buffer
  const OdUInt8 *be = bp + len; // End of buffer
  while (bp < be) // FNV-1 hash each octet in the buffer
  {
    // Multiply by the 32 bit FNV magic prime mod 2^32
#if defined(NO_FNV_GCC_OPTIMIZATION)
    hval *= 0x01000193;
#else
    hval += (hval << 1) + (hval << 4) + (hval << 7) +
            (hval << 8) + (hval << 24);
#endif
    // Xor the bottom with the current octet
    hval ^= (OdUInt32)*bp++;
  }
  // Return our new hash value
  return hval;
}

/** \details
  Perform a 32 bit Fowler/Noll/Vo hash on a string.

  \param str [in]  String to hash.
  \param hval [in]  Previous hash value or initial basis if first call.

  \returns
  32 bit hash as a static hash type.

  \remarks
  To use the 32 bit FNV-0 historic hash, use 0 as the hval.
  To use the recommended 32 bit FNV-1 hash, use 0x811c9dc5 as the hval.
*/
inline OdUInt32 odFNV32HashStr(const char *str, OdUInt32 hval = 0x811c9dc5)
{
  const OdUInt8 *s = (const OdUInt8*)str; // Unsigned string
  while (*s) // FNV-1 hash each octet in the buffer
  {
    // Multiply by the 32 bit FNV magic prime mod 2^32
#if defined(NO_FNV_GCC_OPTIMIZATION)
    hval *= 0x01000193;
#else
    hval += (hval << 1) + (hval << 4) + (hval << 7) +
            (hval << 8) + (hval << 24);
#endif
    // Xor the bottom with the current octet
    hval ^= (OdUInt32)*s++;
  }
  // Return our new hash value
  return hval;
}

/** \details
  Perform a 32 bit Fowler/Noll/Vo FNV-1a hash on a buffer.

  \param buf [in]  Start of buffer to hash.
  \param len [in]  Length of buffer in octets.
  \param hval [in]  Previous hash value or initial basis if first call.

  \returns
  32 bit hash as a static hash type.

  \remarks
  To use the recommended 32 bit FNV-1a hash, use 0x811c9dc5 as the hval.
*/
inline OdUInt32 odFNV32aHashBuf(const void *buf, size_t len, OdUInt32 hval = 0x811c9dc5)
{
  const OdUInt8 *bp = (const OdUInt8*)buf; // Start of buffer
  const OdUInt8 *be = bp + len; // End of buffer
  while (bp < be) // FNV-1a hash each octet in the buffer
  {
    // Xor the bottom with the current octet
    hval ^= (OdUInt32)*bp++;
    // Multiply by the 32 bit FNV magic prime mod 2^32
#if defined(NO_FNV_GCC_OPTIMIZATION)
    hval *= 0x01000193;
#else
    hval += (hval << 1) + (hval << 4) + (hval << 7) +
            (hval << 8) + (hval << 24);
#endif
  }
  // Return our new hash value
  return hval;
}

/** \details
  Perform a 32 bit Fowler/Noll/Vo FNV-1a hash on a string.

  \param str [in]  String to hash.
  \param hval [in]  Previous hash value or initial basis if first call.

  \returns
  32 bit hash as a static hash type.

  \remarks
  To use the recommended 32 bit FNV-1a hash, use 0x811c9dc5 as the hval.
*/
inline OdUInt32 odFNV32aHashStr(const char *str, OdUInt32 hval = 0x811c9dc5)
{
  const OdUInt8 *s = (const OdUInt8*)str; // Unsigned string
  while (*s) // FNV-1a hash each octet in the buffer
  {
    // Xor the bottom with the current octet
    hval ^= (OdUInt32)*s++;
    // Multiply by the 32 bit FNV magic prime mod 2^32
#if defined(NO_FNV_GCC_OPTIMIZATION)
    hval *= 0x01000193;
#else
    hval += (hval << 1) + (hval << 4) + (hval << 7) +
            (hval << 8) + (hval << 24);
#endif
  }
  // Return our new hash value
  return hval;
}

/** \details
  Perform a 64 bit Fowler/Noll/Vo hash on a buffer.

  \param buf [in]  Start of buffer to hash.
  \param len [in]  Length of buffer in octets.
  \param hval [in]  Previous hash value or initial basis if first call.

  \returns
  64 bit hash as a static hash type.

  \remarks
  To use the 64 bit FNV-0 historic hash, use 0 as the hval.
  To use the recommended 64 bit FNV-1 hash, use 0xcbf29ce484222325 as the hval.
*/
inline OdUInt64 odFNV64HashBuf(const void *buf, size_t len, OdUInt64 hval = 0xcbf29ce484222325ULL)
{
  const OdUInt8 *bp = (const OdUInt8*)buf; // Start of buffer
  const OdUInt8 *be = bp + len; // Beyond end of buffer
  while (bp < be) // FNV-1 hash each octet in the buffer
  {
    // Multiply by the 64 bit FNV magic prime mod 2^32
#if defined(NO_FNV_GCC_OPTIMIZATION)
    hval *= 0x100000001b3;
#else
    hval += (hval << 1) + (hval << 4) + (hval << 5) +
            (hval << 7) + (hval << 8) + (hval << 40);
#endif
    // Xor the bottom with the current octet
    hval ^= (OdUInt64)*bp++;
  }
  // Return our new hash value
  return hval;
}

/** \details
  Perform a 64 bit Fowler/Noll/Vo hash on a string.

  \param str [in]  String to hash.
  \param hval [in]  Previous hash value or initial basis if first call.

  \returns
  64 bit hash as a static hash type.

  \remarks
  To use the 64 bit FNV-0 historic hash, use 0 as the hval.
  To use the recommended 64 bit FNV-1 hash, use 0xcbf29ce484222325 as the hval.
*/
inline OdUInt64 odFNV64HashStr(const char *str, OdUInt64 hval = 0xcbf29ce484222325ULL)
{
  const OdUInt8 *s = (const OdUInt8*)str; // Unsigned string
  while (*s) // FNV-1 hash each octet in the buffer
  {
    // Multiply by the 64 bit FNV magic prime mod 2^32
#if defined(NO_FNV_GCC_OPTIMIZATION)
    hval *= 0x100000001b3;
#else
    hval += (hval << 1) + (hval << 4) + (hval << 5) +
            (hval << 7) + (hval << 8) + (hval << 40);
#endif
    // Xor the bottom with the current octet
    hval ^= (OdUInt64)*s++;
  }
  // Return our new hash value
  return hval;
}

/** \details
  Perform a 64 bit Fowler/Noll/Vo FNV-1a hash on a buffer.

  \param buf [in]  Start of buffer to hash.
  \param len [in]  Length of buffer in octets.
  \param hval [in]  Previous hash value or initial basis if first call.

  \returns
  64 bit hash as a static hash type.

  \remarks
  To use the recommended 64 bit FNV-1a hash, use 0xcbf29ce484222325 as the hval.
*/
inline OdUInt64 odFNV64aHashBuf(const void *buf, size_t len, OdUInt64 hval = 0xcbf29ce484222325ULL)
{
  const OdUInt8 *bp = (const OdUInt8*)buf; // Start of buffer
  const OdUInt8 *be = bp + len; // Beyond end of buffer
  while (bp < be) // FNV-1a hash each octet in the buffer
  {
    // Xor the bottom with the current octet
    hval ^= (OdUInt64)*bp++;
    // Multiply by the 64 bit FNV magic prime mod 2^32
#if defined(NO_FNV_GCC_OPTIMIZATION)
    hval *= 0x100000001b3;
#else
    hval += (hval << 1) + (hval << 4) + (hval << 5) +
            (hval << 7) + (hval << 8) + (hval << 40);
#endif
  }
  // Return our new hash value
  return hval;
}

/** \details
  Perform a 64 bit Fowler/Noll/Vo FNV-1a hash on a string.

  \param str [in]  String to hash.
  \param hval [in]  Previous hash value or initial basis if first call.

  \returns
  64 bit hash as a static hash type.

  \remarks
  To use the recommended 64 bit FNV-1a hash, use 0xcbf29ce484222325 as the hval.
*/
inline OdUInt64 odFNV64aHashStr(const char *str, OdUInt64 hval = 0xcbf29ce484222325ULL)
{
  const OdUInt8 *s = (const OdUInt8*)str; // Unsigned string
  while (*s) // FNV-1a hash each octet in the buffer
  {
    // Xor the bottom with the current octet
    hval ^= (OdUInt64)*s++;
    // Multiply by the 64 bit FNV magic prime mod 2^32
#if defined(NO_FNV_GCC_OPTIMIZATION)
    hval *= 0x100000001b3;
#else
    hval += (hval << 1) + (hval << 4) + (hval << 5) +
            (hval << 7) + (hval << 8) + (hval << 40);
#endif
  }
  // Return our new hash value
  return hval;
}

// C++ wrappers for hash functions

template <typename Type, size_t szf = sizeof(Type)>
class OdFNVHashWrap { };
template <typename Type>
class OdFNVHashWrap<Type, 4>
{
  public:
    // For buffer
    static inline Type buf(const void *buf, size_t len) { return (Type)::odFNV32HashBuf(buf, len); };
    static inline Type buf(const void *buf, size_t len, Type hval) { return (Type)::odFNV32HashBuf(buf, len, (OdUInt32)hval); };
    // For strings
    static inline Type str(const char *str) { return (Type)::odFNV32HashStr(str); };
    static inline Type str(const char *str, Type hval) { return (Type)::odFNV32HashStr(str, (OdUInt32)hval); };
    static inline Type str(const OdChar *str) { return buf(str, odStrLen(str) * sizeof(OdChar)); };
    static inline Type str(const OdChar *str, Type hval) { return buf(str, odStrLen(str) * sizeof(OdChar), hval); };
    static inline Type str(const OdString &str) { return buf(str.c_str(), str.getLength() * sizeof(OdChar)); };
    static inline Type str(const OdString &str, Type hval) { return buf(str.c_str(), str.getLength() * sizeof(OdChar), hval); };
    // For other types
    template <typename DataType>
    static inline Type type(DataType data) { return buf(&data, sizeof(DataType)); }
    template <typename DataType>
    static inline Type type(DataType data, Type hval) { return buf(&data, sizeof(DataType), hval); }
    // For typed arrays
    template <typename DataType>
    static inline Type tArray(const DataType *pData, size_t nVals) { return buf(pData, sizeof(DataType) * nVals); }
    template <typename DataType>
    static inline Type tArray(const DataType *pData, size_t nVals, Type hval) { return buf(pData, sizeof(DataType) * nVals, hval); }
    // For ODA arrays and vectors
    template <typename ArrayType>
    static inline Type odArray(const ArrayType &arry) { return tArray(arry.getPtr(), arry.size()); }
    template <typename ArrayType>
    static inline Type odArray(const ArrayType &arry, Type hval) { return tArray(arry.getPtr(), arry.size(), hval); }
};
template <typename Type>
class OdFNVHashWrap<Type, 8>
{
  public:
    // For buffer
    static inline Type buf(const void *buf, size_t len) { return (Type)::odFNV64HashBuf(buf, len); };
    static inline Type buf(const void *buf, size_t len, Type hval) { return (Type)::odFNV64HashBuf(buf, len, (OdUInt64)hval); };
    // For strings
    static inline Type str(const char *str) { return (Type)::odFNV64HashStr(str); };
    static inline Type str(const char *str, Type hval) { return (Type)::odFNV64HashStr(str, (OdUInt32)hval); };
    static inline Type str(const OdChar *str) { return buf(str, odStrLen(str) * sizeof(OdChar)); };
    static inline Type str(const OdChar *str, Type hval) { return buf(str, odStrLen(str) * sizeof(OdChar), hval); };
    static inline Type str(const OdString &str) { return buf(str.c_str(), str.getLength() * sizeof(OdChar)); };
    static inline Type str(const OdString &str, Type hval) { return buf(str.c_str(), str.getLength() * sizeof(OdChar), hval); };
    // For other types
    template <typename DataType>
    static inline Type type(DataType data) { return buf(&data, sizeof(DataType)); }
    template <typename DataType>
    static inline Type type(DataType data, Type hval) { return buf(&data, sizeof(DataType), hval); }
    // For typed arrays
    template <typename DataType>
    static inline Type tArray(const DataType *pData, size_t nVals) { return buf(pData, sizeof(DataType) * nVals); }
    template <typename DataType>
    static inline Type tArray(const DataType *pData, size_t nVals, Type hval) { return buf(pData, sizeof(DataType) * nVals, hval); }
    // For ODA arrays and vectors
    template <typename ArrayType>
    static inline Type odArray(const ArrayType &arry) { return tArray(arry.getPtr(), arry.size()); }
    template <typename ArrayType>
    static inline Type odArray(const ArrayType &arry, Type hval) { return tArray(arry.getPtr(), arry.size(), hval); }
};

template <typename Type, size_t szf = sizeof(Type)>
class OdFNVaHashWrap { };
template <typename Type>
class OdFNVaHashWrap<Type, 4>
{
  public:
    // For buffer
    static inline Type buf(const void *buf, size_t len) { return (Type)::odFNV32aHashBuf(buf, len); };
    static inline Type buf(const void *buf, size_t len, Type hval) { return (Type)::odFNV32aHashBuf(buf, len, (OdUInt32)hval); };
    // For strings
    static inline Type str(const char *str) { return (Type)::odFNV32aHashStr(str); };
    static inline Type str(const char *str, Type hval) { return (Type)::odFNV32aHashStr(str, (OdUInt32)hval); };
    static inline Type str(const OdChar *str) { return buf(str, odStrLen(str) * sizeof(OdChar)); };
    static inline Type str(const OdChar *str, Type hval) { return buf(str, odStrLen(str) * sizeof(OdChar), hval); };
    static inline Type str(const OdString &str) { return buf(str.c_str(), str.getLength() * sizeof(OdChar)); };
    static inline Type str(const OdString &str, Type hval) { return buf(str.c_str(), str.getLength() * sizeof(OdChar), hval); };
    // For other types
    template <typename DataType>
    static inline Type type(DataType data) { return buf(&data, sizeof(DataType)); }
    template <typename DataType>
    static inline Type type(DataType data, Type hval) { return buf(&data, sizeof(DataType), hval); }
    // For typed arrays
    template <typename DataType>
    static inline Type tArray(const DataType *pData, size_t nVals) { return buf(pData, sizeof(DataType) * nVals); }
    template <typename DataType>
    static inline Type tArray(const DataType *pData, size_t nVals, Type hval) { return buf(pData, sizeof(DataType) * nVals, hval); }
    // For ODA arrays and vectors
    template <typename ArrayType>
    static inline Type odArray(const ArrayType &arry) { return tArray(arry.getPtr(), arry.size()); }
    template <typename ArrayType>
    static inline Type odArray(const ArrayType &arry, Type hval) { return tArray(arry.getPtr(), arry.size(), hval); }
};
template <typename Type>
class OdFNVaHashWrap<Type, 8>
{
  public:
    // For buffer
    static inline Type buf(const void *buf, size_t len) { return (Type)::odFNV64aHashBuf(buf, len); };
    static inline Type buf(const void *buf, size_t len, Type hval) { return (Type)::odFNV64aHashBuf(buf, len, (OdUInt64)hval); };
    // For strings
    static inline Type str(const char *str) { return (Type)::odFNV64aHashStr(str); };
    static inline Type str(const char *str, Type hval) { return (Type)::odFNV64aHashStr(str, (OdUInt32)hval); };
    static inline Type str(const OdChar *str) { return buf(str, odStrLen(str) * sizeof(OdChar)); };
    static inline Type str(const OdChar *str, Type hval) { return buf(str, odStrLen(str) * sizeof(OdChar), hval); };
    static inline Type str(const OdString &str) { return buf(str.c_str(), str.getLength() * sizeof(OdChar)); };
    static inline Type str(const OdString &str, Type hval) { return buf(str.c_str(), str.getLength() * sizeof(OdChar), hval); };
    // For other types
    template <typename DataType>
    static inline Type type(DataType data) { return buf(&data, sizeof(DataType)); }
    template <typename DataType>
    static inline Type type(DataType data, Type hval) { return buf(&data, sizeof(DataType), hval); }
    // For typed arrays
    template <typename DataType>
    static inline Type tArray(const DataType *pData, size_t nVals) { return buf(pData, sizeof(DataType) * nVals); }
    template <typename DataType>
    static inline Type tArray(const DataType *pData, size_t nVals, Type hval) { return buf(pData, sizeof(DataType) * nVals, hval); }
    // For ODA arrays and vectors
    template <typename ArrayType>
    static inline Type odArray(const ArrayType &arry) { return tArray(arry.getPtr(), arry.size()); }
    template <typename ArrayType>
    static inline Type odArray(const ArrayType &arry, Type hval) { return tArray(arry.getPtr(), arry.size(), hval); }
};

#include "TD_PackPop.h"

#endif //_OD_FNV1HASH_H_
