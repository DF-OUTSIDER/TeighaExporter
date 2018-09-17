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

#ifndef _DWFCORE_ENCRYPTION_ENGINE_H
#define _DWFCORE_ENCRYPTION_ENGINE_H


///
///\file        dwfcore/crypto/EncryptionEngine.h
///\brief       This header contains the declaration for the DWFEncryptionEngine interface.
///


#include "dwfcore/InputStream.h"
#include "dwfcore/OutputStream.h"
#include "dwfcore/crypto/CryptoKey.h"
#include "dwfcore/crypto/CryptoEngine.h"

namespace DWFCore
{

///
///\class       DWFEncryptionEngine   dwfcore/crypto/EncryptionEngine.h    "dwfcore/crypto/EncryptionEngine.h"
///\brief       This class provides an interface for encryption engines.
///\since       1.3
///
class DWFEncryptionEngine : public DWFCryptoEngine
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFEncryptionEngine()
        throw()
    {;}

    ///
    ///         Stores a key for use in the encryption engine. The key \b must exist 
    ///         for the scope of the engines usage. 
    ///
    ///\param   rKey        The key object.
    ///\param   eKeyType    Describes the key and indicates the engine operation.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setKey( DWFCryptoKey&          rKey, 
                         DWFCryptoKey::teType   eKeyType )
        throw( DWFException ) = 0;

    ///
    ///         Encrypts data according to the algorithm of the implementing engine.
    ///
    ///\param   rDataStream         This stream provides the source data for the encryption engine.
    ///\param   rEncryptedStream    This stream provides the encrypted data back to the caller.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void encrypt( DWFInputStream& rDataStream, DWFOutputStream& rEncryptedStream )
        throw( DWFException ) = 0;

    ///
    ///         Decrypts data according to the algorithm of the implementing engine.
    ///
    ///\param   rEncryptedStream    This stream provides the encrypted data for the decryption engine.
    ///\param   rDataStream         This stream provides the decrypted data back to the caller.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void decrypt( DWFInputStream& rEncryptedStream, DWFOutputStream& rDataStream )
        throw( DWFException ) = 0;

protected:

    ///
    ///         Constructor
    ///
    DWFEncryptionEngine()
        throw()
        : DWFCryptoEngine()
    {;}
};

}

#endif
#endif

