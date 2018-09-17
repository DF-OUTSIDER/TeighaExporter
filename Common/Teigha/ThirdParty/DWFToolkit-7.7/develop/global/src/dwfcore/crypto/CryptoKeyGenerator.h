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

#ifndef _DWFCORE_CRYPTO_KEY_GENERATOR_H
#define _DWFCORE_CRYPTO_KEY_GENERATOR_H

///
///\file        dwfcore/crypto/CryptoKeyGenerator.h
///\brief       This header contains the declaration for the DWFCryptoKeyGenerator class and
///             the key generator interfaces
///

#include "dwfcore/crypto/CryptoKey.h"

namespace DWFCore
{

///
///\class       DWFKeyGenerator     dwfcore/crypto/CryptoKey.h    "dwfcore/crypto/CryptoKey.h"
///\brief       This utility class provides an interface for generating new crypto keys.
///\since       1.3
///
///             <write me>
///
class DWFCryptoKeyGenerator : virtual public DWFCoreMemory
{

public:

    ///
    /// Enumeration for various algorithms that 
    /// can be passed around as crypto engines.
    ///
    typedef enum teType
    {
        eReserved   = 0,

        //
        // Asymmetric algorithm key generators
        //

        eRSAKeyGenerator        = 10,
        eDSAKeyGenerator        = 11,

        //
        // Symmetric cipher key generators
        //

        eSymmetricKeyGenerator  = 20

    } teType;

public:

    ///
    /// Destructor
    ///
    _DWFCORE_API
    virtual ~DWFCryptoKeyGenerator()
        throw()
    {;}

    ///
    ///         Uniquely identifies the implemented key generator.
    ///
    ///\return  The identifier string
    ///\throw   None
    ///
    _DWFCORE_API
    virtual DWFCryptoKeyGenerator::teType identifier() const
        throw() = 0;

protected:

    ///
    /// Constructor
    ///
    _DWFCORE_API
    DWFCryptoKeyGenerator()
        throw()
    {;}

};



///
///\interface   DWFRSAKeyGeneratorInterface     dwfcore/crypto/CryptoKeyGenerator.h    "dwfcore/crypto/CryptoKeyGenerator.h"
///\brief       Interface for a key generator for RSA keys.
///\since       1.3
///
///             <write me>
///
class DWFRSAKeyGeneratorInterface : public DWFCryptoKeyGenerator
{

public:

    ///
    /// Interface for generating keys for RSA encryption engines.
    ///
    ///\param   nBitLength      The number of bits in the modulus.
    ///\param   rpPublicKey     The public key will be created and 
    ///                         returned via this reference to a crypto key pointer.
    ///\param   rpPrivateKey    The private key will be created and 
    ///                         returned via this reference to a crypto key pointer.
    ///\param   zSalt           A buffer for bytes for seeding/randomizing any pseudo-random
    ///                         number generator that the key generator implementation may use.
    ///\param   nSaltLength     The number of bytes in the zSalt buffer.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void GenerateKeyPair( unsigned int      nBitLength,
                                  DWFCryptoKey*&    rpPublicKey, 
                                  DWFCryptoKey*&    rpPrivateKey,
                                  unsigned char*    zSalt = NULL,
                                  unsigned int      nSaltLength = 0 )
        throw( DWFException ) = 0;

protected:

    ///
    /// Constructor
    ///
    _DWFCORE_API
    DWFRSAKeyGeneratorInterface()
        throw()
    {;}
};



///
///\interface   DWFDSAKeyGeneratorInterface     dwfcore/crypto/CryptoKeyGenerator.h    "dwfcore/crypto/CryptoKeyGenerator.h"
///\brief       Interface for a key generator for DSA keys.
///\since       1.3
///
///             <write me>
///
class DWFDSAKeyGeneratorInterface : public DWFCryptoKeyGenerator
{

public:

    ///
    /// Interface for generating keys for DSA encryption engines.
    ///
    ///\param   nBitLength      The number of bits in the DSA generated prime.
    ///\param   rpPublicKey     The public key will be created and 
    ///                         returned via this reference to a crypto key pointer.
    ///\param   rpPrivateKey    The private key will be created and 
    ///                         returned via this reference to a crypto key pointer.
    ///\param   zSalt           This is used to seed the prime number generation.
    ///\param   nSaltLength     The number of bytes in the zSalt buffer.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void GenerateKeyPair( unsigned int      nBitLength,
                                  DWFCryptoKey*&    rpPublicKey, 
                                  DWFCryptoKey*&    rpPrivateKey,
                                  unsigned char*    zSalt = NULL,
                                  unsigned int      nSaltLength = 0 )
        throw( DWFException ) = 0;

protected:

    ///
    /// Constructor
    ///
    _DWFCORE_API
    DWFDSAKeyGeneratorInterface()
        throw()
    {;}
};



///
///\interface   DWFSymmetricKeyGeneratorInterface     dwfcore/crypto/CryptoKeyGenerator.h    "dwfcore/crypto/CryptoKeyGenerator.h"
///\brief       Interface for a key generator for symmetric ciphers.
///\since       1.3
///
///             <write me>
///
class DWFSymmetricKeyGeneratorInterface : public DWFCryptoKeyGenerator
{

public:

    ///
    /// Interface for generating keys for DSA encryption engines.
    ///
    ///\param   nBitLength      The bit length of the symmetric key.
    ///\param   rpSymmetricKey  The cipher key will be created and 
    ///                         returned via this reference to a crypto key pointer.
    ///\param   zSalt           This is used to seed the prime number generation.
    ///\param   nSaltLength     The number of bytes in the zSalt buffer.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void GenerateKeys( unsigned int     nBitLength,
                               DWFCryptoKey*&   rpSymmetricKey, 
                               unsigned char*   zSalt = NULL,
                               unsigned int     nSaltLength = 0 )
        throw( DWFException ) = 0;

protected:

    ///
    /// Constructor
    ///
    _DWFCORE_API
    DWFSymmetricKeyGeneratorInterface()
        throw()
    {;}
};


}


#endif
#endif

