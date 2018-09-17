//
//  Copyright (c) 2006 by Autodesk, Inc.
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


#include "dwfcore/Core.h"
#include "dwfcore/String.h"

#ifndef _DWFCORE_DIGEST_H
#define _DWFCORE_DIGEST_H

///
///\file        dwfcore/Digest.h
///\brief       This header contains the declaration for the DWFDigest, DWF interface.
///

///
///\example     Digest/main.cpp
///

//
// fwd declarations that must live outside the namespace (coming from C)
//
struct md5_state_s;
struct SHA1Context;

namespace DWFCore
{

///
///\class       DWFDigest   dwfcore/Digest.h    "dwfcore/Digest.h"
///\brief       The digest computes a one-way hash given a sequence of bytes.
///\since       1.2
///
///             This provides the interface for all digest algorithms.
///
class DWFDigest : virtual public DWFCoreMemory
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFDigest()
        throw()
    {;}

    ///
    ///         Update the digest computation with the new bytes.
    ///
    ///\param   pBuffer     The buffer holding the latest sequence of bytes.
    ///\param   nBytes      The number of bytes in the buffer.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void update( const char* pBuffer, size_t nBytes )
        throw( DWFException ) = 0;

    ///
    ///         Determine if the digest of the bytes seen so far can be retrieved during
    ///         the computation, without affecting the computation. If it is not
    ///         progressive then \a digest() should not be called until all bytes have
    ///         passed throught the computation.
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual bool isProgressive()
        throw() = 0;

    ///
    ///         Get the digest of all bytes read so far, in hexadecimal characters. 
    ///         For digest's that are not progressive this should not be called until 
    ///         all bytes have been read. Non-progressive digests will always finalize
    ///         the computation on this call.
    ///
    ///\return  A string containing the digest. The length depends on the exact algorithm.
    ///\throw   None
    ///
    _DWFCORE_API
    virtual DWFString digest()
        throw( DWFException ) = 0;

    ///
    ///         Get the digest of all bytes read so far, in raw bytes.
    ///         For digest's that are not progressive this should not be called until 
    ///         all bytes have been read. Non-progressive digests will always finalize
    ///         the computation on this call. 
    ///
    ///\param   rpBytes     The array of bytes will be allocated and returned via this
    ///                     pointer. It should be initialized to NULL before calling this.
    ///\return  A character array containing the raw bytes, that the caller must delete 
    ///         using DWFCORE_FREE_MEMORY(), and the length of the array. The array will
    ///         \b not have a terminating null characters.
    ///\throw   None
    ///
    _DWFCORE_API
    virtual size_t digestRawBytes( unsigned char*& rpBytes )
        throw( DWFException ) = 0;

    ///
    ///         Get the digest of all bytes read so far, in a base-64 encoded string.
    ///         For digest's that are not progressive this should not be called until 
    ///         all bytes have been read. Non-progressive digests will always finalize
    ///         the computation on this call. 
    ///
    ///\return  A string containing the digest in base-64 encoding. The length depends
    ///         on the exact algorithm.
    ///\throw   None
    ///
    _DWFCORE_API
    virtual DWFString digestBase64()
        throw( DWFException ) = 0;

    ///
    ///         Reset the digest for a new computation.
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void reset()
        throw() = 0;
};



//
// fwd declaration
//
struct _DWFMD5Digest_t;

///
///\class       DWFMD5Digest   dwfcore/Digest.h    "dwfcore/Digest.h"
///\brief       This class computes a one-way hash given a sequence of bytes using the MD5
///             algorithm.
///\since       1.2
///
///             This provides the interface for the MD5 digest algorithms.
///
class DWFMD5Digest : public DWFDigest
{

public:

    ///         Constructor
    ///
    ///\throw   None
    _DWFCORE_API
    DWFMD5Digest()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFMD5Digest()
        throw();

    ///
    ///\copydoc DWFDigest::update()
    ///
    _DWFCORE_API
    virtual void update( const char* pBuffer, size_t nBytes )
        throw( DWFException );

    ///
    ///\copydoc DWFDigest::isProgressive()
    ///
    _DWFCORE_API
    virtual bool isProgressive()
        throw();

    ///
    ///\copydoc DWFDigest::digest()
    ///
    _DWFCORE_API
    virtual DWFString digest()
        throw( DWFException );

    ///
    ///\copydoc DWFDigest::digestRawBytes()
    ///
    _DWFCORE_API
    virtual size_t digestRawBytes( unsigned char*& rpBytes )
        throw( DWFException );

    ///
    ///\copydoc DWFDigest::digestBase64()
    ///
    _DWFCORE_API
    virtual DWFString digestBase64()
        throw( DWFException );

    ///
    ///\copydoc DWFDigest::reset()
    ///
    _DWFCORE_API
    virtual void reset()
        throw();

private:

    // This does the actual digest computation. The public digest calls take the result and
    // format it into the requested output form.
    void _computeDigest( _DWFMD5Digest_t& oDigest )
        throw( DWFException );

private:

    // A pointer to the structure that stores the state of the MD5 computation as bytes are read in
    md5_state_s* _pState;

};


//
// fwd declaration
//
struct _DWFSHA1Digest_t;

///
///\class       DWFSHA1Digest   dwfcore/Digest.h    "dwfcore/Digest.h"
///\brief       This class computes a one-way hash given a sequence of bytes using the SHA1
///             algorithm.
///\since       1.3
///
///             This provides the interface for the SHA1 digest algorithms.
///
class DWFSHA1Digest : public DWFDigest
{

public:

    ///         Constructor
    ///
    ///\throw   None
    _DWFCORE_API
    DWFSHA1Digest()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFSHA1Digest()
        throw();

    ///
    ///\copydoc DWFDigest::update()
    ///
    _DWFCORE_API
    virtual void update( const char* pBuffer, size_t nBytes )
        throw( DWFException );

    ///
    ///\copydoc DWFDigest::isProgressive()
    ///
    _DWFCORE_API
    virtual bool isProgressive()
        throw();

    ///
    ///\copydoc DWFDigest::digest()
    ///
    _DWFCORE_API
    virtual DWFString digest()
        throw( DWFException );

    ///
    ///\copydoc DWFDigest::digestRawBytes()
    ///
    _DWFCORE_API
    virtual size_t digestRawBytes( unsigned char*& rpBytes )
        throw( DWFException );

    ///
    ///\copydoc DWFDigest::digestBase64()
    ///
    _DWFCORE_API
    virtual DWFString digestBase64()
        throw( DWFException );

    ///
    ///\copydoc DWFDigest::reset()
    ///
    _DWFCORE_API
    virtual void reset()
        throw();

private:

    // This does the actual digest computation. The public digest calls take the result and
    // format it into the requested output form.
    void _computeDigest( unsigned int _oDigest[5] )
        throw( DWFException );

private:

    // A pointer to the structure that stores the state of the MD5 computation as bytes are read in
    SHA1Context*    _pState;
};


}

#endif


