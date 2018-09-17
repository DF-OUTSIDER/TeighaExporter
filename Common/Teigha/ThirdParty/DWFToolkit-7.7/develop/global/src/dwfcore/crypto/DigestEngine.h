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

#ifndef DWFCORE_DISABLE_CRYPTO

#ifndef _DWFCORE_DIGEST_ENGINE_H
#define _DWFCORE_DIGEST_ENGINE_H


///
///\file        dwfcore/crypto/DigestEngine.h
///\brief       This header contains the declaration for the DWFDigestEngine interface.
///

#include "dwfcore/Digest.h"
#include "dwfcore/crypto/CryptoEngine.h"

namespace DWFCore
{

///
///\class       DWFDigestEngine   dwfcore/crypto/DigestEngine.h    "dwfcore/crypto/DigestEngine.h"
///\brief       This class provides an interface for digest engines.
///\since       1.3
///
class DWFDigestEngine : public DWFCryptoEngine, public DWFDigest
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFDigestEngine()
        throw();

    ///
    ///         Reset the digest for a new computation. Derived implementations
    ///         of reset \b should still call this to clear up the local buffer.
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void reset()
        throw()
    {
        if (_zDigestBytes)
        {
            DWFCORE_FREE_MEMORY( _zDigestBytes );
        }
        _nBytes = 0;
    }

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFDigestEngine()
        throw();

    ///
    ///         Derived classes should use this to store the raw bytes of the digest.
    ///         The bytes are copied into a local buffer
    ///
    ///\param   zBytes          The bytes of the digest that will be copied for storing.
    ///                         If this is NULL, the stored bytes will be deleted.
    ///\param   nBytes          The number of bytes being copied from zBytes.
    _DWFCORE_API
    void _storeBytes( unsigned char* zBytes, size_t nBytes )
        throw( DWFException );

protected:

    unsigned char*  _zDigestBytes;
    size_t          _nBytes;
};

}

#endif
#endif

