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

#undef verify       // deal with the "verify" macro issue on MacOS.

#ifndef DWFCORE_DISABLE_CRYPTO

#ifndef _DWFCORE_CRYPTO_ENGINE_ASYMMETRIC_ALGORITHM_H
#define _DWFCORE_CRYPTO_ENGINE_ASYMMETRIC_ALGORITHM_H


///
///\file        dwfcore/crypto/AsymmetricAlgorithm.h
///\brief       This header contains the declaration for the DWFAsymmetricAlgorithm interface.
///


#include "dwfcore/crypto/EncryptionEngine.h"


namespace DWFCore
{

///
///\class   DWFAsymmetricAlgorithm  dwfcore/crypto/AsymmetricAlgorithm.h    "dwfcore/crypto/AsymmetricAlgorithm.h"
///\brief   This class defines the interface for crypto engines that implement Asymmetric (pki) algorithms.
///\since   1.3
///
///             <write me>
///
class DWFAsymmetricAlgorithm : public DWFEncryptionEngine
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFAsymmetricAlgorithm()
        throw();

    ///
    ///\copydoc DWFEncryptionEngine::setKey
    ///
    _DWFCORE_API
    virtual void setKey( DWFCryptoKey&          rKey, 
                         DWFCryptoKey::teType   eKeyType )
        throw( DWFException );

    ///
    ///                     Performs digest signing via the asymmetric algorithm implemented in the engine.
    ///         
    ///\param               rDigestStream       Provides the message digest.
    ///\param               rSignatureStream    Returns the signature.
    ///\param               eDigestMethod       Identifies the algorithm used to generate the digest.
    ///                                         Note that not all digest algorithms may be supported.
    ///\throw               DWFException
    ///
    _DWFCORE_API
    virtual void sign( DWFInputStream&          rDigestStream, 
                       DWFOutputStream&         rSignatureStream, 
                       DWFCryptoEngine::teType  eDigestMethod = DWFCryptoEngine::eSHA1 )
        throw( DWFException ) = 0;

    ///
    ///                     Performs digest signature verification via the asymmetric algorithm implemented in the engine.
    ///         
    ///\param               rDigestStream       Provides the message digest.
    ///\param               rSignatureStream    Provides the signature.
    ///\param               eDigestMethod       Identifies the algorithm used to generate the digest.
    ///                                         Note that not all digest algorithms may be supported.
    ///\return              True if the signature is valid, false otherwise.
    ///
    ///\throw               DWFException
    ///
    _DWFCORE_API
    virtual bool verify( DWFInputStream&            rDigestStream, 
                         DWFInputStream&            rSignatureStream,
                         DWFCryptoEngine::teType    eDigestMethod = DWFCryptoEngine::eSHA1 )
        throw( DWFException ) = 0;

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFAsymmetricAlgorithm()
        throw();

    ///
    ///         Obtain a key stored by the engine.
    ///
    ///\param   eKeyType    One of \a eAsymmetricPublicKey or \a eAsymmetricPrivateKey
    ///\return  An immutable pointer to the key, which must not be deleted, if the key
    ///         is available; NULL otherwise.
    ///
    _DWFCORE_API
    const DWFCryptoKey* const _key( DWFCryptoKey::teType eKeyType ) const
        throw();

private:

    DWFCryptoKey*   _pPublicKey;
    DWFCryptoKey*   _pPrivateKey;
};

}

#endif
#endif
