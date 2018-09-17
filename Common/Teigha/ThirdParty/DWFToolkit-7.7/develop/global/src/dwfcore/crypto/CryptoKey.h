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

#ifndef _DWFCORE_CRYPTO_KEY_H
#define _DWFCORE_CRYPTO_KEY_H


///
///\file        dwfcore/crypto/CryptoKey.h
///\brief       This header contains the interface for DWFCryptoKey classes and the 
///             DWFCryptoKeyGenerator classes.
///


#include "dwfcore/Core.h"
#include "dwfcore/Exception.h"


namespace DWFCore
{

///
///\class       DWFCryptoKey     dwfcore/crypto/CryptoKey.h    "dwfcore/crypto/CryptoKey.h"
///\brief       This class provides the interface for algorithm specific crypto keys.
///\since       1.3
///
///             <write me>
///
class DWFCryptoKey : virtual public DWFCoreMemory
{

public:

    ///
    ///\enum    teType
    ///\brief   Enumeration values that define the intent and use of the key.
    ///
    typedef enum teType
    {
        ///
        /// Public key of asymmetric pair
        ///
        eAsymmetricPublicKey,

        ///
        /// Private ley of asymmetric pair
        ///
        eAsymmetricPrivateKey,

        ///
        /// User key for symmetric ciphers
        ///
        eSymmetricKey,

        ///
        /// Initialization vector for symmetric ciphers
        ///
        eInitializationVector

    } teType;

public:

    ///
    ///
    ///
    virtual ~DWFCryptoKey()
        throw()
    {;}

protected:

    ///
    ///
    ///
    DWFCryptoKey()
        throw()
    {;}

};


///
///\class   DWFRSAKeyInterface      dwfcore/crypto/CryptoKey.h      "dwfcore/crypto/CryptoKey.h"
///\brief   Interface to query and set crypto keys for the DSA algorithm
///\since   1.3
///
class DWFRSAKeyInterface
{

public:
    virtual ~DWFRSAKeyInterface() {}

    ///
    ///         Get the modulus of the RSA key.
    ///
    ///\param   ppN     A pointer to the binary form of N is returned with this parameter.
    ///                 The allocated memory must be released by the caller using 
    ///                 the \b DWFCORE_FREE_MEMORY macro.
    ///\return  Returns the number of bytes returned in ppN. If the key does not have
    ///         this will be 0.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void getN( unsigned char*& rpN, int& nBytes ) const
        throw( DWFException ) = 0;

    ///
    ///         Get the public exponent.
    ///
    ///\param   ppE     A pointer to the binary form of E is returned with this parameter.
    ///                 The allocated memory must be released by the caller using 
    ///                 the \b DWFCORE_FREE_MEMORY macro.
    ///\return  Returns the number of bytes returned in ppE. If the key does not have
    ///         this will be 0.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void getE( unsigned char*& rpE, int& nBytes ) const
        throw( DWFException ) = 0;

    ///
    ///         Get the private exponent.
    ///
    ///\param   ppD     A pointer to the binary form of D is returned with this parameter.
    ///                 The allocated memory must be released by the caller using 
    ///                 the \b DWFCORE_FREE_MEMORY macro.
    ///\return  Returns the number of bytes returned in ppD. If the key does not have
    ///         this will be 0.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void getD( unsigned char*& rpD, int& nBytes ) const
        throw( DWFException ) = 0;

    ///
    ///         Set the modulus of the RSA key.
    ///
    ///\param   zN      The array holding binary form of the modulus.
    ///\param   nBytes  The number of bytes in the array.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setN( unsigned char* zN, int nBytes )
        throw( DWFException ) = 0;

    ///
    ///         Set the public exponent.
    ///
    ///\param   zE      The array holding binary form of the exponent.
    ///\param   nBytes  The number of bytes in the array.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setE( unsigned char* zE, int nBytes )
        throw( DWFException ) = 0;

    ///
    ///         Set the private exponent.
    ///
    ///\param   zD      The array holding binary form of the exponent.
    ///\param   nBytes  The number of bytes in the array.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setD( unsigned char* zD, int nBytes )
        throw( DWFException ) = 0;

protected:

    ///
    ///
    ///
    DWFRSAKeyInterface()
        throw()
    {;}

};




///
///\class   DWFDSAKeyInterface      dwfcore/crypto/CryptoKey.h      "dwfcore/crypto/CryptoKey.h"
///\brief   Interface to query and set crypto keys for the DSA algorithm
///\since   1.3
///
class DWFDSAKeyInterface
{

public:
    virtual ~DWFDSAKeyInterface() {}

    ///
    ///         Get the parameter P.
    ///
    ///\param   ppP     A pointer to the binary form of P is returned with this parameter.
    ///                 The allocated memory must be released by the caller using 
    ///                 the \b DWFCORE_FREE_MEMORY macro.
    ///\return  Returns the number of bytes returned in ppP. If the key does not have
    ///         this will be 0.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void getP( unsigned char*& rpP, int& nBytes ) const
        throw( DWFException ) = 0;

    ///
    ///         Get the parameter P.
    ///
    ///\param   ppQ     A pointer to the binary form of Q is returned with this parameter.
    ///                 The allocated memory must be released by the caller using 
    ///                 the \b DWFCORE_FREE_MEMORY macro.
    ///\return  Returns the number of bytes returned in ppQ. If the key does not have
    ///         this will be 0.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void getQ( unsigned char*& rpQ, int& nBytes ) const
        throw( DWFException ) = 0;

    ///
    ///         Get the parameter G.
    ///
    ///\param   ppG     A pointer to the binary form of G is returned with this parameter.
    ///                 The allocated memory must be released by the caller using 
    ///                 the \b DWFCORE_FREE_MEMORY macro.
    ///\return  Returns the number of bytes returned in ppG. If the key does not have
    ///         this will be 0.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void getG( unsigned char*& rpG, int& nBytes ) const
        throw( DWFException ) = 0;

    ///
    ///         Get the public key parameter Y.
    ///
    ///\param   ppY     A pointer to the binary form of Y is returned with this parameter.
    ///                 The allocated memory must be released by the caller using 
    ///                 the \b DWFCORE_FREE_MEMORY macro.
    ///\return  Returns the number of bytes returned in ppY. If the key does not have
    ///         this will be 0.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void getY( unsigned char*& rpY, int& nBytes ) const
        throw( DWFException ) = 0;

    ///
    ///         Get the private key parameter X.
    ///
    ///\param   ppX     A pointer to the binary form of X is returned with this parameter.
    ///                 The allocated memory must be released by the caller using 
    ///                 the \b DWFCORE_FREE_MEMORY macro.
    ///\return  Returns the number of bytes returned in ppX. If the key does not have
    ///         this will be 0.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void getX( unsigned char*& rpX, int& nBytes ) const
        throw( DWFException ) = 0;

    ///
    ///         Set the parameter P.
    ///
    ///\param   zP      The array holding binary form of P.
    ///\param   nPBytes The number of bytes in the array for P.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setP( unsigned char* zP, int nPBytes )
        throw( DWFException ) = 0;

    ///
    ///         Set the parameter Q.
    ///
    ///\param   zQ      The array holding binary form of Q.
    ///\param   nQBytes The number of bytes in the array for Q.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setQ( unsigned char* zQ, int nQBytes )
        throw( DWFException ) = 0;

    ///
    ///         Set the paramter G.
    ///
    ///\param   zG      The array holding binary form of G.
    ///\param   nBytes  The number of bytes in the array.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setG( unsigned char* zG, int nBytes )
        throw( DWFException ) = 0;

    ///
    ///         Set the public key paramter Y.
    ///
    ///\param   zY      The array holding binary form of Y.
    ///\param   nBytes  The number of bytes in the array.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setY( unsigned char* zY, int nBytes )
        throw( DWFException ) = 0;

    ///
    ///         Set the private key paramter X
    ///
    ///\param   zX      The array holding binary form of X.
    ///\param   nBytes  The number of bytes in the array.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void setX( unsigned char* zX, int nBytes )
        throw( DWFException ) = 0;

protected:

    ///
    ///
    ///
    DWFDSAKeyInterface()
        throw()
    {;}

};


}

#endif
#endif

