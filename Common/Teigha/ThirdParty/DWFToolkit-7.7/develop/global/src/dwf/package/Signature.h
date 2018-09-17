//
//  Copyright (c) 2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Signature.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_SIGNATURE_H
#define _DWFTK_SIGNATURE_H

///
///\file        dwf/package/Signature.h
///\brief       This file contains the DWFSignature class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
#include "dwfcore/DigestOutputStream.h"
#include "dwfcore/DigestInputStream.h"
#include "dwfcore/crypto/CryptoEngine.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/reader/SignatureReader.h"


// forward decl
namespace DWFCore
{
class DWFCryptoKey;
class DWFAsymmetricAlgorithm;
}



namespace DWFToolkit
{

// forward decl
class DWFResource;


///
///\ingroup     dwfpackage
///
///\class       DWFSignature     dwf/package/Signature.h      "dwf/package/Signature.h"
///\brief       A DWFSignature is used to ...
///\since       7.3.0
///
///             The DWFSignature is used to ...
///
class DWFSignature
            : public DWFSignatureReader
            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:
    class Reference;

    ///
    ///\brief   This type defines a list of DWFSignature pointers.
    ///
    typedef DWFOrderedVector<DWFSignature*>             tList;
    ///
    ///\brief   This type defines a mapped collection of DWFSignature pointers.
    ///
    typedef DWFStringKeySkipList<DWFSignature*>         tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFSignature pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFSignature*>           tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFSignature pointers.
    ///
    typedef DWFIterator<DWFSignature*>                  tIterator;

    ///
    ///\brief   This type defines a list of DWFSignature::Reference pointers.
    ///
    typedef DWFOrderedVector<DWFSignature::Reference*>  tReferenceVector;

    ///
    ///\brief   This type defines a list of X509Data pointers.
    ///
    typedef DWFOrderedVector<X509Data*>                 tX509DataVector;


public:

    ///
    ///\class   DigestMethod     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a DigestMethod ...
    ///\since       7.3.0
    ///
    ///         A DigestMethod ...
    ///
    class DigestMethod
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        DigestMethod( )
            throw()
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~DigestMethod()
            throw()
        {;}

        ///
        ///         The public name of the algorithm used by this digest method.
        ///
        ///\return  The algorithm name.
        ///\throw   None
        ///
        _DWFTK_API
        virtual const wchar_t* algorithmName() const = 0;

        ///
        ///         An internal identifier of the algorithm used by this digest method.
        ///
        ///\return  The algorithm identifier.
        ///\throw   None
        ///
        _DWFTK_API
        virtual const DWFCryptoEngine::teType algorithmIdentifier() const = 0;

        ///
        ///         Given an output stream, create a new DWFDigestOutputStream, chain
        ///         the given stream to the new stream, and return the new stream.
        ///
        ///         This allows for the calculation of a digest value as output is
        ///         passing through the stream.
        ///
        ///\param   pStream         The stream to be digested.
        ///\param   bOwnStream      Indicates if the digest stream should take ownership of the lifetime of the pStream argument.
        ///\return  The new stream.
        ///\throw   None
        ///
        _DWFTK_API
        virtual DWFDigestOutputStream* chainOutputStream(DWFOutputStream* pStream, bool bOwnStream) const
            throw( DWFException ) = 0;

        ///
        ///         Given an input stream, create a new DWFDigestInputStream, chain
        ///         the given stream to the new stream, and return the new stream.
        ///
        ///         This allows for the calculation of a digest value as input is
        ///         passing through the stream.
        ///
        ///\param   pStream         The stream to be digested.
        ///\param   bOwnStream      Indicates if the digest stream should take ownership of the lifetime of the pStream argument.
        ///\return  The new stream.
        ///\throw   None
        ///
        _DWFTK_API
        virtual DWFDigestInputStream* chainInputStream(DWFInputStream* pStream, bool bOwnStream) const
            throw( DWFException ) = 0;
    };

    ///
    ///\class   DigestMethodSHA1     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a DigestMethodSHA1 ...
    ///\since       7.3.0
    ///
    ///         A DigestMethodSHA1 ...
    ///
    class DigestMethodSHA1 : public DigestMethod
    {
    public:
        ///
        ///\copydoc DWFSignature::DigestMethod::algorithmName()
        ///
        virtual const wchar_t* algorithmName() const
        {
            return kzAlgorithmNameSHA1;
        };

        ///
        ///\copydoc DWFSignature::DigestMethod::algorithmIdentifier()
        ///
        virtual const DWFCryptoEngine::teType algorithmIdentifier() const
        {
            return DWFCryptoEngine::eSHA1;
        }

        ///
        ///\copydoc DWFSignature::DigestMethod::chainOutputStream()
        ///
        _DWFTK_API
        virtual DWFDigestOutputStream* chainOutputStream(DWFOutputStream* pStream, bool bOwnStream) const
            throw( DWFException );

        ///
        ///\copydoc DWFSignature::DigestMethod::chainInputStream()
        ///
        _DWFTK_API
        virtual DWFDigestInputStream* chainInputStream(DWFInputStream* pStream, bool bOwnStream) const
            throw( DWFException );

    private:
        static const wchar_t* const kzAlgorithmNameSHA1;
    };

    ///
    ///\class   DigestMethodMD5     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a DigestMethodMD5 ...
    ///\since       7.3.0
    ///
    ///         A DigestMethodMD5 ...
    ///
    class DigestMethodMD5 : public DigestMethod
    {
    public:
        ///
        ///\copydoc DWFSignature::DigestMethod::algorithmName()
        ///
        virtual const wchar_t* algorithmName() const
        {
            return kzAlgorithmNameMD5;
        };

        ///
        ///\copydoc DWFSignature::DigestMethod::algorithmIdentifier()
        ///
        virtual const DWFCryptoEngine::teType algorithmIdentifier() const
        {
            return DWFCryptoEngine::eMD5;
        }

        ///
        ///\copydoc DWFSignature::DigestMethod::chainOutputStream()
        ///
        _DWFTK_API
        virtual DWFDigestOutputStream* chainOutputStream(DWFOutputStream* pStream, bool bOwnStream) const
            throw( DWFException );

        ///
        ///\copydoc DWFSignature::DigestMethod::chainInputStream()
        ///
        _DWFTK_API
        virtual DWFDigestInputStream* chainInputStream(DWFInputStream* pStream, bool bOwnStream) const
            throw( DWFException );

    private:
        static const wchar_t* const kzAlgorithmNameMD5;
    };


    ///
    ///\class   DigestValue     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a DigestValue ...
    ///\since       7.3.0
    ///
    ///         A DigestValue ...
    ///
    class DigestValue
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        DigestValue()
            throw()
            : _zValue(L"")
        {
        }

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        DigestValue(const DWFString& zValue)
            throw()
            : _zValue(zValue)
        {
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~DigestValue()
            throw()
        {
        }


        ///
        ///         Returns the value of the digest, as a base64 encoded string.
        ///
        ///\return  The value of the digest, as a base64 encoded string.
        ///\throw   None
        ///
        virtual const DWFString& valueBase64() const
            throw()
        {
            return _zValue;
        }

        ///
        ///         Sets the value of the digest as a base64 encoded string.
        ///
        ///\param   zValue  The value of the digest, as a base64 encoded string.
        ///\throw   None
        ///
        virtual void setValueBase64(const DWFString& zValue)
            throw()
        {
            _zValue = zValue;
        }

        ///
        ///         Returns the value of the digest, as a byte array.
        ///
        ///\param   rpBytes A reference to a pointer where the bytes of the digest will be stored.
        ///                 This memory will be allocated with the \b DWFCORE_ALLOC_MEMORY macro,
        ///                 and must be released by the caller, by calling \b DWFCORE_FREE_MEMORY.
        ///\param   nBytes  The number of bytes.
        ///\throw   None
        ///
        _DWFTK_API
        virtual void getValueBytes(void*& rpBytes, size_t& nBytes) const
            throw();

        ///
        ///         Sets the value of the digest as a byte array.
        ///
        ///\param   pBytes A pointer to the bytes of the digest.
        ///\param   nBytes  The number of bytes.
        ///\throw   None
        ///
        _DWFTK_API
        virtual void setValueBytes(const void *pBytes, const size_t nBytes)
            throw();

    private:
        DWFString _zValue;
    };


    ///
    ///\class   Reference     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a Reference ...
    ///\since       7.3.0
    ///
    ///         A Reference ...
    ///
    class Reference
    {
        friend class DWFSignature;

    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        Reference()
            throw()
            : _zURI()
            , _pSignature(NULL)
            , _pDigestValue(NULL)
        {
            ;
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Reference()
            throw()
        {
            if(_pDigestValue!=NULL)
            {
                DWFCORE_FREE_OBJECT(_pDigestValue);
            }
        }

        ///
        ///         Returns the URI that this reference refers to.
        ///
        ///\return  The URI that this reference refers to.
        ///\throw   None
        ///
        virtual const DWFString& URI() const
             throw( DWFException )
        {
            return _zURI;
        }

        ///
        ///         Sets the URI that this reference refers to.
        ///
        ///\param   zURI  The URI that this reference should refer to.
        ///\throw   None
        ///
        virtual void setURI(const DWFString& zURI)
            throw()
        {
            _zURI = zURI;
        }


        ///
        ///         Returns the digest value of this reference.
        ///
        ///\throw   None
        ///
        const DigestValue* digestValue() const
            throw(DWFException)
        {
            return _pDigestValue;
        }

        ///
        ///         Sets the digest value.
        ///
        ///\param   pDigestValue  A pointer to the specified DigestValue.
        ///                       The Reference will take ownership of this pointer, and will delete the object by calling \b DWFCORE_FREE_OBJECT.
        ///\throw   None
        ///
        void setDigestValue(const DigestValue* pDigestValue)
            throw()
        {
            _pDigestValue = pDigestValue;
        }


        ///
        ///         Returns the DWFSignature that this reference belongs to.
        ///
        ///\throw   None
        ///
        DWFSignature* signature()
        {
            return _pSignature;
        }

    protected:
        ///
        ///         Sets the DWFSignature that this reference belongs to.
        ///
        ///\throw   None
        ///
        void setSignature(DWFSignature* pSignature)
            throw( DWFException )
        {
            if(_pSignature != NULL )
            {
                _DWFCORE_THROW( DWFInvalidArgumentException, /*NOXLATE*/ L"Can't re-use References." );
            }

            _pSignature = pSignature;
        }

    private:
        // The URI of what this Reference refers to
        DWFString               _zURI; 

        // The Signature to which this Reference belongs.
        DWFSignature*           _pSignature;

        // The Digest Value of this reference
        const DigestValue*      _pDigestValue;
    };


    ///
    ///\class   SignatureValue     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a SignatureValue ...
    ///\since       7.3.0
    ///
    ///         A SignatureValue ...
    ///
    class SignatureValue
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        SignatureValue()
            throw()
            : _zValue(L"")
        {
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~SignatureValue()
            throw()
        {
        }

        ///
        ///         Returns the value of the signature, as a base64 encoded string.
        ///
        ///\return  The value of the signature, as a base64 encoded string.
        ///\throw   None
        ///
        virtual const DWFString& valueBase64() const
            throw()
        {
            return _zValue;
        }

        ///
        ///         Sets the value of the signature as a base64 encoded string.
        ///
        ///\param   zValue  The value of the signature, as a base64 encoded string.
        ///\throw   None
        ///
        virtual void setValueBase64(const DWFString& zValue)
            throw()
        {
            _zValue = zValue;
        }

        ///
        ///         Returns the value of the signature, as a byte array.
        ///
        ///\param   rpBytes A reference to a pointer where the bytes of the signature will be stored.
        ///                 This memory will be allocated with the \b DWFCORE_ALLOC_MEMORY macro,
        ///                 and must be released by the caller, by calling \b DWFCORE_FREE_MEMORY.
        ///\param   nBytes  The number of bytes.
        ///\throw   None
        ///
        _DWFTK_API
        virtual void getValueBytes(void*& rpBytes, size_t& nBytes) const
            throw();

        ///
        ///         Sets the value of the signature as a byte array.
        ///
        ///\param   pBytes A pointer to the bytes of the signature.
        ///\param   nBytes  The number of bytes.
        ///\throw   None
        ///
        _DWFTK_API
        virtual void setValueBytes(const void *pBytes, const size_t nBytes)
            throw();


    private:
        DWFString _zValue;
    };


    ///
    ///\class   SignatureMethod     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a SignatureMethod ...
    ///\since       7.3.0
    ///
    ///         A SignatureMethod ...
    ///
    class SignatureMethod
    {
    public:
        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        SignatureMethod()
            throw();

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~SignatureMethod()
            throw();


        ///
        ///         The public name of the algorithm used by this signature method.
        ///
        ///\return  The algorithm name.
        ///\throw   None
        ///
        _DWFTK_API
        virtual const wchar_t* algorithmName() const
            throw() = 0;

        ///
        ///         Returns the \b DWFAsymmetricAlgorithm used by this signature method.
        ///
        ///\return  A pointer to the DWFAsymmetricAlgorithm used by this signature method.
        ///\throw   None
        ///
        _DWFTK_API
        virtual DWFAsymmetricAlgorithm* algorithm()
            throw();

        ///
        ///         Signs a digest value using a supplied key.
        ///
        ///\param   pDigestValue        The digest to be signed.
        ///\param   pDigestMethod       The method that was used to produce the given digest.
        ///\param   pPrivateKey         The private key to be used to sign the given digest.
        ///\param   pSignatureValue     A pointer to a SignatureValue into which the signed digest will be stored.
        ///\throw   None
        ///
        _DWFTK_API
        virtual void signDigest(const DigestValue* pDigestValue, const DigestMethod* pDigestMethod, DWFCryptoKey* pPrivateKey, SignatureValue* pSignatureValue)
            throw(DWFException);

        ///
        ///         Verifies a signature value against a current digest.
        ///
        ///\param   pDigestValue        The current digest.
        ///\param   pDigestMethod       The method that was used to produce the given digest.
        ///\param   pPublicKey          The public key to be used to decrypt the given signature.
        ///\param   pSignatureValue     A pointer to the SignatureValue to be verified.
        ///\throw   None
        ///
        _DWFTK_API
        virtual bool verifyDigest(const DigestValue* pDigestValue, const DigestMethod* pDigestMethod, DWFCryptoKey* pPublicKey, const SignatureValue* pSignatureValue)
            throw(DWFException);

    protected:
        ///
        ///         Create the asymmetric algorithm used for the signature. This has to be implemented by the 
        ///         derived classes of signature method.
        ///
        ///\return  A pointer to the asymmetric algorithm. DWFSignatureMethod will take ownership and free the object
        ///         when it is destroyed.
        ///\throw   None
        ///
        _DWFTK_API
        virtual DWFAsymmetricAlgorithm* createAlgorithm()
            throw() = 0;

    private:
        DWFAsymmetricAlgorithm* _pCachedAlgorithm;
    };

    ///
    ///\class   SignatureMethodDSA     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a SignatureMethodDSA ...
    ///\since       7.3.0
    ///
    ///         A SignatureMethodDSA ...
    ///
    class SignatureMethodDSA : public SignatureMethod
    {
    public:
        ///
        ///\copydoc DWFSignature::SignatureMethod::algorithmName()
        ///
        virtual const wchar_t* algorithmName() const
            throw()
        {
            return kzAlgorithmNameDSA_SHA1;
        };

        ///
        ///\copydoc DWFSignature::SignatureMethod::createAlgorithm()
        ///
        _DWFTK_API
        virtual DWFAsymmetricAlgorithm* createAlgorithm()
            throw();

    private:
        static const wchar_t* const kzAlgorithmNameDSA_SHA1;
    };

    ///
    ///\class   SignatureMethodRSA     dwf/package/Signature.h     "dwf/package/Signature.h"
    ///\brief   a SignatureMethodRSA ...
    ///\since       7.3.0
    ///
    ///         A SignatureMethodRSA ...
    ///
    class SignatureMethodRSA : public SignatureMethod
    {
    public:
        ///
        ///\copydoc DWFSignature::SignatureMethod::algorithmName()
        ///
        virtual const wchar_t* algorithmName() const
            throw()
        {
            return kzAlgorithmNameRSA_SHA1;
        };

    protected:
        ///
        ///\copydoc DWFSignature::SignatureMethod::createAlgorithm()
        ///
        _DWFTK_API
        virtual DWFAsymmetricAlgorithm* createAlgorithm()
            throw();

    private:
        static const wchar_t* const kzAlgorithmNameRSA_SHA1;
    };


public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFSignature()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSignature()
        throw();


    ///
    ///         Add a Reference to this signature. The Signature will then own the Reference, and manage its lifetime.
    //          Note that References should not be re-used.
    ///
    ///\param   pReference  A pointer to the Reference.
    ///\throw   None
    ///
    _DWFTK_API
    void addReference(Reference* pReference)
        throw( DWFException );

    ///
    ///         Gets an iterator that allows enumeration of the given References.
    ///
    ///\return  An iterator over the Reference.
    ///\throw   None
    ///
    tReferenceVector::Iterator* references()
        throw()
    {
        return _oReferences.iterator();
    }


    ///
    ///         Sets the digest method to be used.
    ///
    ///\param   pDigestMethod  A pointer to the requested DigestMethod.
    ///\throw   None
    ///
    _DWFTK_API
    void setDigestMethod(const DigestMethod* pDigestMethod)
        throw();

    ///
    ///         Returns the digest method to be used.
    ///
    ///\return  A pointer to the specified DigestMethod.
    ///\throw   None
    ///
    const DigestMethod* digestMethod() const
        throw()
    {
        return _pDigestMethod;
    }


    ///
    ///         Sets the signature method to be used.
    ///
    ///\param   pSignatureMethod  A pointer to the requested SignatureMethod.
    ///\throw   None
    ///
    _DWFTK_API
    void setSignatureMethod(SignatureMethod* pSignatureMethod)
        throw();

    ///
    ///         Returns the signature method to be used.
    ///
    ///\return  A pointer to the specified SignatureMethod.
    ///\throw   None
    ///
    SignatureMethod* signatureMethod() const
        throw(DWFException)
    {
        return _pSignatureMethod;
    }


    ///
    ///         Sets the signature value.
    ///
    ///\param   pSignatureValue  A pointer to the specified SignatureValue.
    ///\throw   None
    ///
    _DWFTK_API
    void setSignatureValue(const SignatureValue* pSignatureValue)
        throw();

    ///
    ///         Returns the signature method to be used.
    ///
    ///\return  A pointer to the specified SignatureMethod.
    ///\throw   None
    ///
    const SignatureValue* signatureValue() const
        throw(DWFException)
    {
        return _pSignatureValue;
    }


    ///
    ///         Adds an X509Data element to the signature.
    ///
    ///\param   pX509Data  A pointer to the pX509Data to be added.
    ///\throw   None
    ///
    _DWFTK_API
    void addX509Data(X509Data* pX509Data)
        throw();

    ///
    ///         Sets the public key.
    ///
    ///\param   pPublicKey  A pointer to the public key.
    ///\throw   None
    ///
    void setPublicKey(DWFCryptoKey* pPublicKey)
        throw()
    {
        _pPublicKey = pPublicKey;
    }

    ///
    ///         Returns the public key to be used.
    ///
    ///\return  A pointer to the specified public key.
    ///\throw   None
    ///
    DWFCryptoKey* publicKey() const
        throw()
    {
        return _pPublicKey;
    }

    ///
    ///         Sets the key name.
    ///
    ///\param   zKeyName  A reference to the key name string.
    ///\throw   None
    ///
    void setKeyName(const DWFString& zKeyName)
        throw()
    {
        _zKeyName = zKeyName;
    }

    ///
    ///         Returns the key name.
    ///
    ///\return  A reference to the key name string.
    ///\throw   None
    ///
    const DWFString& keyName() const
        throw()
    {
        return _zKeyName;
    }


    ///
    ///         Accepts References.
    ///
    ///\param   zReferenceURI           The reference URI. 
    ///\return  The filtered reference URI.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFString& provideReference( const DWFString& zReferenceURI )
        throw( DWFException );

    ///
    ///         Accepts signature method strings.
    ///
    ///\param   zSigMethod           The signature method. 
    ///\return  The filtered signature method.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFString& provideSignatureMethod( const DWFString& zSigMethod )
        throw( DWFException );

    ///
    ///         Accepts signature value strings.
    ///
    ///\param   zSigValue           The signature value. 
    ///\return  The filtered signature value.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFString& provideSignatureValue( const DWFString& zSigValue )
        throw( DWFException );

    ///
    ///         Accepts X509 Data Elements.
    ///
    ///         Note that the recipient of this call must take ownership of the X509Data object, and
    ///         eventually free it by calling DWFCORE_FREE_OBJECT.
    ///
    ///\param   pX509Data           The X509 Data Element. 
    ///\return  The filtered X509 Data Element.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual X509Data* provideX509Data( X509Data* pX509Data )
        throw( DWFException );


    ///
    ///         Accepts digest method strings.
    ///
    ///\param   zDigestMethod           The digest method value. 
    ///\return  The filtered digest method value.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFString& provideDigestMethod( const DWFString& zDigestMethod )
        throw( DWFException );

    ///
    ///         Accepts digest value strings.
    ///
    ///\param   zDigestValue           The digest value. 
    ///\return  The filtered digest value.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFString& provideDigestValue( const DWFString& zDigestValue )
        throw( DWFException );

    ///
    ///         Accepts RSA Key Value strings.
    ///
    ///\param   rRSAKeyValueStrings     The RSA Key Value strings. 
    ///\return  The filtered RSA Key Value strings.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFSignatureReader::RSAKeyValueStrings& provideRSAKeyValueStrings( const DWFSignatureReader::RSAKeyValueStrings& rRSAKeyValueStrings )
        throw( DWFException );

    ///
    ///         Accepts DSA Key Value strings.
    ///
    ///\param   pDSAKeyValueStrings     The DSA Key Value strings. 
    ///\return  The filtered DSA Key Value strings.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFSignatureReader::DSAKeyValueStrings& provideDSAKeyValueStrings( const DWFSignatureReader::DSAKeyValueStrings& pDSAKeyValueStrings )
        throw( DWFException );

    ///
    ///         Accepts KeyInfo KeyName strings.
    ///
    ///\param   zKeyName           The KeyName string. 
    ///\return  The filtered KeyName string.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const DWFString& provideKeyName( const DWFString& zKeyName )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///         Computes the digest value of the SignedInfo portion of this signature.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
        virtual void computeSignedInfoDigestValue(DWFSignature::DigestValue& signedInfoDigest, const DWFString& zNamespace)
        throw( DWFException );

    /// TODO: document this
    _DWFTK_API
    virtual bool validateSignedInfo(DWFCryptoKey* pPublicKey)
        throw( DWFException );

#endif

    /// TODO: document this
    _DWFTK_API
    virtual bool validateReferences(DWFPackageReader* pReader)
        throw( DWFException );



protected:

#ifndef DWFTK_READ_ONLY

    //
    // Serializes just the SignedInfo sub-tree
    //
    void _serializeSignedInfo( DWFXMLSerializer& rSerializer, const DWFString& zNamespace )
        throw( DWFException );

#endif

protected:
    float                   _nVersion;
    tReferenceVector        _oReferences;
    const DigestMethod*     _pDigestMethod;
    const SignatureValue*   _pSignatureValue;
    SignatureMethod*        _pSignatureMethod;
    tX509DataVector         _oX509DataVector;
    Reference*              _pCurrentReference;
    DWFCryptoKey*           _pPublicKey;
    DWFString               _zKeyName;


private:

    DWFSignature( const DWFSignature& );
    DWFSignature& operator=( const DWFSignature& );
};



    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC
//DNT_Start
_declspec(selectany) const wchar_t* const DWFSignature::DigestMethodSHA1::kzAlgorithmNameSHA1        = L"http://www.w3.org/2000/09/xmldsig#sha1";
_declspec(selectany) const wchar_t* const DWFSignature::DigestMethodMD5::kzAlgorithmNameMD5          = L"http://www.w3.org/2000/09/xmldsig#md5";
_declspec(selectany) const wchar_t* const DWFSignature::SignatureMethodDSA::kzAlgorithmNameDSA_SHA1  = L"http://www.w3.org/2000/09/xmldsig#dsa-sha1";
_declspec(selectany) const wchar_t* const DWFSignature::SignatureMethodRSA::kzAlgorithmNameRSA_SHA1  = L"http://www.w3.org/2000/09/xmldsig#rsa-sha1";
//DNT_End
#endif
#endif

}

#endif

