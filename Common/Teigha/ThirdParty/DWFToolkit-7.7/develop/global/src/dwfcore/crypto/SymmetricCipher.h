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

#ifndef _DWFCORE_CRYPTO_ENGINE_SYMMETRIC_CIPHER_H
#define _DWFCORE_CRYPTO_ENGINE_SYMMETRIC_CIPHER_H


///
///\file        dwfcore/crypto/SymmetricCipher.h
///\brief       This header contains the declaration for the DWFSymmetricCipher interface.
///


#include "dwfcore/crypto/EncryptionEngine.h"


namespace DWFCore
{

///
///\class   DWFSymmetricCipher  dwfcore/crypto/SymmetricCipher.h    "dwfcore/crypto/SymmetricCipher.h"
///\brief   This class defines the interface for crypto engines that implement symmetric block and stream ciphers.
///\since   1.3
///
///             <write me>
///
class DWFSymmetricCipher : public DWFEncryptionEngine
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFSymmetricCipher()
        throw();

    ///
    ///\copydoc DWFCryptoEngine::setKey
    ///
    _DWFCORE_API
    virtual void setKey( DWFCryptoKey&          rKey,
                         DWFCryptoKey::teType   eKeyType = DWFCryptoKey::eSymmetricKey )
        throw( DWFException );

protected:


    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFSymmetricCipher()
        throw();

    ///
    ///         Obtain a key stored by the engine.
    ///
    ///\param   eKeyType    One of \a eSymmetricKey or \a eInitializationVector
    ///\return  An immutable pointer to the key, which must not be deleted, if the key
    ///         is available; NULL otherwise.
    ///
    _DWFCORE_API
    const DWFCryptoKey* const _key( DWFCryptoKey::teType eKeyType ) const
        throw();

private:

    DWFCryptoKey*   _pIV;
    DWFCryptoKey*   _pKey;
};

}


#endif
#endif
