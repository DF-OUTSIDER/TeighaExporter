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

#ifndef _DWFCORE_CRYPTO_ENGINE_PROVIDER_H
#define _DWFCORE_CRYPTO_ENGINE_PROVIDER_H


///
///\file        dwfcore/crypto/CryptoEngineProvider.h
///\brief       This header contains the declaration for the DWFCryptoEngineProvider singleton utility class.
///

#include "dwfcore/SkipList.h"
#include "dwfcore/Synchronization.h"
#include "dwfcore/crypto/CryptoEngine.h"
#include "dwfcore/crypto/CryptoEngineFactory.h"

namespace DWFCore
{

///
///\class       DWFCryptoEngineProvider    dwfcore/crypto/CryptoEngineProvider.h    "dwfcore/crypto/CryptoEngineProvider.h"
///\brief       Singleton utility class for interacting with crypto engines implementations.
///\since       1.3
///
///             <write me>
///
class DWFCryptoEngineProvider  : virtual public DWFCoreMemory
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFCryptoEngineProvider()
        throw();

    ///
    ///         Registers a new crypto engine, via it's associated factory, with the provider.
    ///
    ///\param   eEngineType Indicates which engine this factory will create.
    ///\param   pFactory        The crypto engine factory.  This pointer must not be NULL.
    ///                         This pointer will be owned and eventually deleted by the provider.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    static void RegisterEngine( DWFCryptoEngine::teType eEngineType,
                                DWFCryptoEngineFactory* pFactory )
        throw( DWFException );

    ///
    ///         Provides the caller with a crypto engine implementation corresponding to the engine type
    ///         requested. 
    ///
    ///\param   eEngineType     This type describes the crypto engine implementation.
    ///\return  A pointer to a new crypto engine, this pointer is owned by the caller and must be
    ///         deleted using the \b DWFCORE_FREE_OBJECT macro.  If no engine matches the request,
    ///         this method returns NULL.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    static DWFCryptoEngine* ProvideEngine( DWFCryptoEngine::teType eEngineType )
        throw( DWFException );

private:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFCryptoEngineProvider()
        throw();

    ///
    ///         Obtain a reference to the provider singleton.
    ///
    ///\return  The crypto engine provider.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    static DWFCryptoEngineProvider& _Instance()
        throw( DWFException );


private:

    //
    // Not Implemented
    //
    DWFCryptoEngineProvider( const DWFCryptoEngineProvider& );
    DWFCryptoEngineProvider& operator=( const DWFCryptoEngineProvider& );

private:

    //
    //  Typedefs used internally by the provider
    //
    typedef DWFSkipList<int, DWFCryptoEngineFactory*>   _tFactoryList;

private:

    //
    //  Mutex used for constructing the singleton in a thread safe manner
    //
    static DWFThreadMutex       _oEngineLock;

    //
    //  This stores a map from engine type to engine factory.
    //
    _tFactoryList               _oFactories;
};

}

#endif
#endif


