//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

#ifndef _DWFCORE_HASH_H
#define _DWFCORE_HASH_H


///
///\file        dwfcore/Hash.h
///\brief       This header contains the implementations for the hashing functions
///             used by the library.
///



#include "dwfcore/Core.h"



namespace DWFCore
{

///
///\struct      tDWFHashKernel    dwfcore/Hash.h    "dwfcore/Hash.h"
///\brief       Hashing function template.
///\since       1.0.1
///
///             This template defines a general hash operator.
///\param       T   The value type of the hash (generally expected as \e long or \e long \e long.
///\param       S   The content type of the hash
///
template<class T, class S>
struct tDWFHashKernel
{
    ///
    ///         \e T value hash operator for \e S type content.
    ///
    ///\param   _s  The content to hash.
    ///\return  The hash code.
    ///    
    virtual T operator()( const S* _s ) const
    {
        (void)_s;

        return 1L;
    }

    virtual ~tDWFHashKernel()
    {}
};

///
///\struct      tDWFDJB2HashKernel    dwfcore/Hash.h    "dwfcore/Hash.h"
///\brief       32-bit DJB2 hash function. 
///\since       1.0.1
///
///             This algorithm (k=33) was first reported by D.J. Bernstein 
///             many years ago in comp.lang.c. Another version of this
///             algorithm (now favored by Bernstein) uses xor: hash(i) =
///             hash(i - 1) * 33 ^ str[i]; the magic of number 33 (why it
///             works better than many other constants, prime or not) has
///             never been adequately explained.
///\param       S   The content type of the hash
///
template<class S>
struct tDWFDJB2HashKernel : tDWFHashKernel<uint32_t, S>
{
    ///
    ///         32-bit hash function for \e S type content.
    ///
    ///\param   _s      The content to hash.
    ///\param   _seed   The initial hash value.
    ///\return  The hash code.
    /// 
    uint32_t operator()( S* _s, uint32_t _seed = 5381 ) const
    {
        uint32_t nHash = _seed;
        S* pIn = (S*)_s;

        if (pIn)
        {
            unsigned int nVal = (unsigned int)*pIn++;

            while (nVal > 0)
            {
#ifdef  DWFCORE_HASH_DJB2_USE_ORIGINAL_VERSION
                nHash = ((nHash << 5) + nHash) + nVal;
#else
                nHash = nHash*33 ^ nVal;
#endif

                nVal = (unsigned int)*pIn++;
            };
        }

        return nHash;
    }
};

///
///\struct      tDWFFNV1A32HashKernel    dwfcore/Hash.h    "dwfcore/Hash.h"
///\brief       32-bit FNV 1-a hash function. 
///\since       1.0.1
///
///             http://www.isthe.com/chongo/src/fnv
///
///             The basis of this hash algorithm was taken from an idea sent
///             as reviewer comments to the IEEE POSIX P1003.2 committee by:
///             Phong Vo (http://www.research.att.com/info/kpv)
///             Glenn Fowler (http://www.research.att.com/~gsf/)
///
///             In a subsequent ballot round:
///             Landon Curt Noll (http://www.isthe.com/chongo)
///
///             improved on their algorithm.  Some people tried this hash
///             and found that it worked rather well.  In an EMail message
///             to Landon, they named it the ``Fowler/Noll/Vo'' or FNV hash.
/// 
///             FNV hashes are designed to be fast while maintaining a low
///             collision rate. The FNV speed allows one to quickly hash lots
///             of data while maintaining a reasonable collision rate.  See:
///             http://www.isthe.com/chongo/tech/comp/fnv/index.html
///             for more details as well as other forms of the FNV hash.
///             Comments, questions, bug fixes and suggestions welcome at
///             the address given in the above URL.
///
///\param     S   The content type of the hash
///
template<class S>
struct tDWFFNV1A32HashKernel : tDWFHashKernel<uint32_t, S>
{
    ///
    ///         32-bit hash operator for \e S type content.
    ///
    ///\param   _s      The content to hash.
    ///\param   _seed   The initial hash value.
    ///\return  The hash code.
    ///
    uint32_t operator()( S* _s, uint32_t _seed = 0x811c9dc5L ) const
    {
        //
        // note that initial value is non-zero
        //
        uint32_t nHash = _seed;
        S* pIn = _s;

            //
            // FNV-1a hash each octet in the buffer
            //
        if (pIn)
        {
            unsigned int nVal = (unsigned int)*pIn++;

            while (nVal > 0)
            {
                //
                // XOR the bottom with the current octet
                //
                nHash ^= (uint32_t)nVal;

                //
                // multiply by the 32 bit FNV magic prime mod 2^32
                // #define FNV_32_PRIME (0x01000193)
                //
#ifdef  DWFCORE_HASH_FNV_DONT_USE_GCC_OPTIMIZATION
                nHash *= (uint32_t)0x01000193L;
#else
                nHash += ((nHash<<1) + (nHash<<4) + (nHash<<7) + (nHash<<8) + (nHash<<24));
#endif
                nVal = (unsigned int)*pIn++;
            }
        }


        return nHash;
    }
};


///
///\struct      tDWFFNV1A64HashKernel    dwfcore/Hash.h    "dwfcore/Hash.h"
///\brief       64-bit FNV 1-a hash function.
///\since       1.0.1
///
///             http://www.isthe.com/chongo/src/fnv
///
///             The basis of this hash algorithm was taken from an idea sent
///             as reviewer comments to the IEEE POSIX P1003.2 committee by:
///             Phong Vo (http://www.research.att.com/info/kpv)
///             Glenn Fowler (http://www.research.att.com/~gsf/)
///
///             In a subsequent ballot round:
///             Landon Curt Noll (http://www.isthe.com/chongo)
///
///             improved on their algorithm.  Some people tried this hash
///             and found that it worked rather well.  In an EMail message
///             to Landon, they named it the ``Fowler/Noll/Vo'' or FNV hash.
/// 
///             FNV hashes are designed to be fast while maintaining a low
///             collision rate. The FNV speed allows one to quickly hash lots
///             of data while maintaining a reasonable collision rate.  See:
///             http://www.isthe.com/chongo/tech/comp/fnv/index.html
///             for more details as well as other forms of the FNV hash.
///             Comments, questions, bug fixes and suggestions welcome at
///             the address given in the above URL.
///
///\param     S   The content type of the hash
///
template<class S>
struct tDWFFNV1A64HashKernel : tDWFHashKernel<uint64_t, S>
{
///
///         64-bit hash operator for \e S type content.
///
///\param   _s      The content to hash.
///\param   _seed   The initial hash value.
///\return  The hash code.
///
#if     defined(_DWFCORE_WIN32_SYSTEM) && (_MSC_VER < 1310)
    //
    // MSVC 7.0 and below don't know about long long
    //
    uint64_t operator()( S* _s, uint64_t _seed = 0xcbf29ce484222325 ) const
#else
    uint64_t operator()( S* _s, uint64_t _seed = 0xcbf29ce484222325ULL ) const
#endif
    {
        //
        // note that initial value is non-zero
        //
        uint64_t nHash = _seed;
        S* pIn = _s;

            //
            // FNV-1a hash each octet in the buffer
            //
        if (pIn)
        {
            unsigned int nVal = (unsigned int)*pIn++;

            while (nVal > 0)
            {
                //
                // XOR the bottom with the current octet
                //
                nHash ^= (uint64_t)nVal;

                //
                // multiply by the 64 bit FNV magic prime mod 2^64
                // #define FNV_32_PRIME (0x01000193)
                //
#ifdef  DWFCORE_HASH_FNV_DONT_USE_GCC_OPTIMIZATION
                nHash *= (uint64_t)0x100000001b3;
#else
                nHash += ((nHash << 1) + (nHash << 4) + (nHash << 5) +
                          (nHash << 7) + (nHash << 8) + (nHash << 40));

#endif
                nVal = (unsigned int)*pIn++;
            }
        }


        return nHash;
    }
};

///
///\ingroup     InternalMacros
///\brief       Mixing function for Jenk96 string hashing kernels.
///
#define _DWFCORE_HASH_JENK96_MIX(a, b, c)   \
{                                           \
  a -= b; a -= c; a ^= (c>>13);             \
  b -= c; b -= a; b ^= (a<<8);              \
  c -= a; c -= b; c ^= (b>>13);             \
  a -= b; a -= c; a ^= (c>>12);             \
  b -= c; b -= a; b ^= (a<<16);             \
  c -= a; c -= b; c ^= (b>>5);              \
  a -= b; a -= c; a ^= (c>>3);              \
  b -= c; b -= a; b ^= (a<<10);             \
  c -= a; c -= b; c ^= (b>>15);             \
}

///
///\struct      tDWFJenk96CharHashKernel    dwfcore/Hash.h    "dwfcore/Hash.h"
///\brief       32-bit Jenk96 hash function for strings.
///\since       1.0.1
///
///             Hash algorithm by Robert Jenkins
///             http://burtleburtle.net/bob/hash/evahash.html
///
struct tDWFJenk96CharHashKernel : tDWFHashKernel<uint32_t,const char>
{
    ///
    ///         32-bit hash operator for \e const \e char* strings.
    ///
    ///\param   _s      The string to hash.
    ///\param   _seed   The initial hash value.
    ///\return  The hash code.
    ///
    uint32_t operator()( const char* _s, uint32_t _seed = 0x02364629L ) const
    {
        uint32_t nHash    = _seed;         // 37111337 - arbitrary prime (gde 4/04)
        uint32_t nBucket1 = 0x9e3779b9L;   // the golden ratio; an arbitrary value
        uint32_t nBucket2 = 0x9e3779b9L;   // the golden ratio; an arbitrary value
        const char* pIn = _s;
        unsigned char iByte = 0;

        if (pIn)
        {
            unsigned char nVal = *pIn++;
            while (nVal > 0)
            {
                //
                // "length" counter
                //
                iByte = 0;

                //
                // jenkins 12-byte cycle
                //
                nBucket1 += nVal;   iByte++;
                nVal = *pIn++;
                if (nVal > 0)
                {
                    nBucket1 += (nVal << 8);   iByte++;
                    nVal = *pIn++;
                    if (nVal > 0)
                    {
                        nBucket1 += (nVal << 16);   iByte++;
                        nVal = *pIn++;
                        if (nVal > 0)
                        {
                            nBucket1 += (nVal << 24);   iByte++;
                            nVal = *pIn++;
                            if (nVal > 0)
                            {
                                nBucket2 += nVal;   iByte++;
                                nVal = *pIn++;
                                if (nVal > 0)
                                {
                                    nBucket2 += (nVal << 8);   iByte++;
                                    nVal = *pIn++;
                                    if (nVal > 0)
                                    {
                                        nBucket2 += (nVal << 16);   iByte++;
                                        nVal = *pIn++;
                                        if (nVal > 0)
                                        {
                                            nBucket2 += (nVal << 24);   iByte++;
                                            nVal = *pIn++;
                                            if (nVal > 0)
                                            {
                                                nHash += (nVal << 8);   iByte++;
                                                nVal = *pIn++;
                                                if (nVal > 0)
                                                {
                                                    nHash += (nVal << 16);   iByte++;
                                                    nVal = *pIn++;
                                                    if (nVal > 0)
                                                    {
                                                        nHash += (nVal << 24);   iByte++;
                                                        nVal = *pIn++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (iByte < 12)
                {
                    nHash += iByte;
                }

                _DWFCORE_HASH_JENK96_MIX( nBucket1, nBucket2, nHash );
            }
        }

        return nHash;
    }
};

///
///\struct      tDWFJenk96WCharHashKernel    dwfcore/Hash.h    "dwfcore/Hash.h"
///\brief       32-bit Jenk96 hash function for \e const \e wchar_t* strings.
///\since       1.0.1
///
///             Hash algorithm by Robert Jenkins
///             http://burtleburtle.net/bob/hash/evahash.html
///
struct tDWFJenk96WCharHashKernel : tDWFHashKernel<uint32_t,const wchar_t>
{
    ///
    ///         32-bit hash operator for wide character strings.
    ///
    ///\param   _s      The string to hash.
    ///\param   _seed   The initial hash value.
    ///\return  The hash code.
    ///
    uint32_t operator()( const wchar_t* _s, uint32_t _seed = 0x02364629L ) const
    {
        uint32_t nHash    = _seed;         // 37111337 - arbitrary prime (gde 4/04)
        uint32_t nBucket1 = 0x9e3779b9L;   // the golden ratio; an arbitrary value
        uint32_t nBucket2 = 0x9e3779b9L;   // the golden ratio; an arbitrary value
        const wchar_t* pIn = _s;
        unsigned char iByte = 0;

        if (pIn)
        {
            unsigned char nVal1 = (unsigned char)((*pIn)&(0x00ff));
            unsigned char nVal2 = (unsigned char)((*pIn)>>8);
            while ((nVal1+nVal2) > 0)
            {
                //
                // "length" counter
                //
                iByte = 0;

                //
                // jenkins 12-byte cycle
                //
                nBucket1 += nVal1;          iByte++;
                nBucket1 += (nVal2 << 8);   iByte++;

                pIn++;
                nVal1 = (unsigned char)((*pIn)&(0x00ff));
                nVal2 = (unsigned char)((*pIn)>>8);
                if ((nVal1+nVal2) > 0)
                {
                    nBucket1 += (nVal1 << 16);  iByte++;
                    nBucket1 += (nVal2 << 24);  iByte++;

                    pIn++;
                    nVal1 = (unsigned char)((*pIn)&(0x00ff));
                    nVal2 = (unsigned char)((*pIn)>>8);
                    if ((nVal1+nVal2) > 0)
                    {
                        nBucket2 += nVal1;          iByte++;
                        nBucket2 += (nVal2 << 8);   iByte++;

                        pIn++;
                        nVal1 = (unsigned char)((*pIn)&(0x00ff));
                        nVal2 = (unsigned char)((*pIn)>>8);
                        if ((nVal1+nVal2) > 0)
                        {
                            nBucket2 += (nVal1 << 16);  iByte++;
                            nBucket2 += (nVal2 << 24);  iByte++;

                            pIn++;
                            nVal1 = (unsigned char)((*pIn)&(0x00ff));
                            nVal2 = (unsigned char)((*pIn)>>8);
                            if ((nVal1+nVal2) > 0)
                            {
                                nHash += nVal1;          iByte++;
                                nHash += (nVal2 << 8);   iByte++;

                                pIn++;
                                nVal1 = (unsigned char)((*pIn)&(0x00ff));
                                nVal2 = (unsigned char)((*pIn)>>8);
                                if ((nVal1+nVal2) > 0)
                                {
                                    nHash += (nVal1 << 16);  iByte++;
                                    nHash += (nVal2 << 24);  iByte++;
                                }
                            }
                        }
                    }
                }

                if (iByte < 12)
                {
                    nHash += iByte;
                }

                _DWFCORE_HASH_JENK96_MIX( nBucket1, nBucket2, nHash );
            }
        }

        return nHash;
    }
};


}

#endif

