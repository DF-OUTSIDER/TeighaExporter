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

#ifndef _DWFCORE_CRYPTO_X509_CERTIFICATE_H
#define _DWFCORE_CRYPTO_X509_CERTIFICATE_H


///
///\file        dwfcore/crypto/X509Certificate.h
///\brief       This header contains the declaration for the class describing an X509 certificate.
///


#include "dwfcore/String.h"
#include "dwfcore/crypto/CryptoKey.h"
#include "dwfcore/crypto/CryptoEngine.h"

#undef verify               // deal with the "verify" macro issue on MacOS.

namespace DWFCore
{

///
///\class       DWFX509Certificate     dwfcore/crypto/X509Certificate.h    "dwfcore/crypto/X509Certificate.h"
///\brief       This class describes an X509 certificate
///\since       1.3
///
///             <write me>
///
class DWFX509Certificate
{

public:

    class IssuerSerial
    {
    public:

        ///
        ///     ctor
        /// TODO: change these names to rep what they really are: country, etc.
        ///
        _DWFCORE_API
        IssuerSerial( unsigned int      nNumber,
                      const DWFString&  rCN,
                      const DWFString&  rC,
                      const DWFString&  rOU,
                      const DWFString&  rO,
                      const DWFString&  rL,
                      const DWFString&  rST,
                      const DWFString&  rT )
            throw( DWFException );

        ///
        ///     dtor
        ///
        _DWFCORE_API
        virtual ~IssuerSerial()
            throw()
        {;}

        ///
        ///     returns the cert name
        ///
        _DWFCORE_API
        const DWFString& name() const
            throw()
        {
            return _zName;
        }

        ///
        ///     returns the cert number
        ///
        _DWFCORE_API
        unsigned int number() const
            throw( DWFException )
        {
            return _nNumber;
        }

    private:

        DWFString       _zName;
        unsigned int    _nNumber;
    };

public:

    ///
    /// TODO: add the rest, extensions, etc, if needed
    ///
    _DWFCORE_API
    static DWFX509Certificate* Generate( const DWFCryptoKey&        rPrivateKey,
                                         const IssuerSerial*        pSerial,
                                         unsigned int               nSerialNumber,
                                         const DWFString&           rSubjectName,
                                         unsigned int               nDaysValid,
                                         DWFCryptoEngine::teType    eDigestMethod = DWFCryptoEngine::eSHA1,
                                         unsigned int               nVersion = 3 )
        throw( DWFException );

    ///
    ///
    ///
    _DWFCORE_API
    static DWFX509Certificate* Generate( const unsigned char* const pDERBuffer,
                                         size_t                     nDERBufferBytes )
        throw( DWFException );

public:

    ///
    ///         Destructor
    ///
    _DWFCORE_API
    virtual ~DWFX509Certificate()
        throw();

    ///
    ///         Returns a valid ASN1 DER encoded binary representation of the certificate.
    ///         This value may be base-64 encoded and used as the <X509Certificate> element
    ///         value in an XML digital signature.
    ///
    ///\param   ppDERBuffer     The ASN1 DER representation of the certificate is returned through
    ///                         this paramter, which must be freed by the caller using teh \bDWFCORE_FREE_MEMORY macro.
    ///\return  The number of bytes returned in *ppDERBuffer
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual size_t DER( unsigned char** ppDERBuffer )
        throw( DWFException ) = 0;

    ///
    ///         Returns the public key of the certificate.
    ///
    ///\return  A pointer to a public key object.  The caller must free this pointer
    ///         using the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    /// 
    _DWFCORE_API
    virtual DWFCryptoKey* key()
        throw( DWFException ) = 0;

    ///
    ///         Performs verification of the certificate against the provided public key.
    ///
    ///\param   rPublicKey      The public key to verify against.
    ///\return  If true, the certificate is verified against the key.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual bool verify( const DWFCryptoKey& rPublicKey )
        throw( DWFException ) = 0;

    ///
    ///         Returns the issuer serial information object of the certificate.
    ///
    ///\return  A pointer to a issuer serial object.  The caller must free this pointer
    ///         using the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    _DWFCORE_API
    virtual IssuerSerial* issuer()
        throw( DWFException ) = 0;

    ///
    ///         Returns the issuer serial number of the certificate.
    ///
    ///\return  The serial number.
    ///\throw   DWFException
    _DWFCORE_API
    virtual unsigned int number()
        throw( DWFException ) = 0;

    ///
    ///         Returns the subject name of the certificate.
    ///
    ///\return  A pointer to the subject name string.  The caller must free this pointer
    ///         using the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFString* subject()
        throw( DWFException ) = 0;

protected:

    ///
    ///         Constructor
    ///
    _DWFCORE_API
    DWFX509Certificate()
        throw();

protected:
    //
    // reserved for impl
    //

private:
    //
    // Not implemented - should they be? If so, need hook for engines...
    //
    DWFX509Certificate( const DWFX509Certificate& );
    DWFX509Certificate& operator=( const DWFX509Certificate& );


};

}

#endif
#endif

