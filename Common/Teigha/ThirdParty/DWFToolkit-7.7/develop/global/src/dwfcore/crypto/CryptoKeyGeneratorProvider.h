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

#ifndef _DWFCORE_CRYPTO_KEY_GENERATOR_PROVIDER_H
#define _DWFCORE_CRYPTO_KEY_GENERATOR_PROVIDER_H


///
///\file        dwfcore/crypto/CryptoKeyGeneratorProvider.h
///\brief       This header contains the declaration for the DWFCryptoKeyGeneratorProvider singleton utility class.
///

#include "dwfcore/SkipList.h"
#include "dwfcore/Synchronization.h"
#include "dwfcore/crypto/CryptoKeyGenerator.h"
#include "dwfcore/crypto/CryptoKeyGeneratorFactory.h"

namespace DWFCore
{

///
///\class       DWFCryptoKeyGeneratorProvider    dwfcore/crypto/CryptoKeyGeneratorProvider.h    "dwfcore/crypto/CryptoKeyGeneratorProvider.h"
///\brief       Singleton utility class for interacting with crypto engines implementations.
///\since       1.3
///
///             <write me>
///
class DWFCryptoKeyGeneratorProvider : virtual public DWFCoreMemory
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFCryptoKeyGeneratorProvider()
        throw();

    ///
    ///         Registers a new crypto key generator with the provider.
    ///
    ///\param   eKeyGenType     Indicates what kind of keys this generator this create.
    ///\param   pGenerator      The crypto key generator factory.  This pointer must not be NULL.
    ///                         This pointer will be owned and eventually deleted by the provider.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    static void RegisterKeyGenerator( DWFCryptoKeyGenerator::teType eKeyGenType,
                                      DWFCryptoKeyGeneratorFactory* pFactory )
        throw( DWFException );

    ///
    ///         Provides the caller with a crypto key generator implementation corresponding
    ///         to the type requested. 
    ///
    ///\param   eKeyGenType     This type describes the key generator implementation.
    ///\return  A pointer to a new key generator; this pointer is owned by the caller and must be
    ///         deleted using the \b DWFCORE_FREE_OBJECT macro. If no generator matches the request,
    ///         this method returns NULL.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    static DWFCryptoKeyGenerator* ProvideKeyGenerator( DWFCryptoKeyGenerator::teType eKeyGenType )
        throw( DWFException );

private:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFCryptoKeyGeneratorProvider()
        throw();

    ///
    ///         Obtain a reference to the provider singleton.
    ///
    ///\return  The cryptokey generator provider.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    static DWFCryptoKeyGeneratorProvider& _Instance()
        throw( DWFException );


private:

    //
    // Not Implemented
    //
    DWFCryptoKeyGeneratorProvider( const DWFCryptoKeyGeneratorProvider& );
    DWFCryptoKeyGeneratorProvider& operator=( const DWFCryptoKeyGeneratorProvider& );

private:

    //
    //  Typedefs used internally by the provider
    //
    typedef DWFSkipList<int, DWFCryptoKeyGeneratorFactory*>   _tFactoryList;

private:

    //
    //  Mutex used for constructing the singleton in a thread safe manner
    //
    static DWFThreadMutex   _oKeyGeneratorLock;

    //
    //  This stores a map from generator type to generator factory.
    //
    _tFactoryList           _oFactories;
};

}

#endif
#endif


