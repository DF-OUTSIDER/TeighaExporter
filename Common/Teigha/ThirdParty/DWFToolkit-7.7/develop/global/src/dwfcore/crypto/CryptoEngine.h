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

#ifndef _DWFCORE_CRYPTO_ENGINE_H
#define _DWFCORE_CRYPTO_ENGINE_H


///
///\file        dwfcore/crypto/CryptoEngine.h
///\brief       This header contains the declaration for the DWFCryptoEngine interface.
///


#include "dwfcore/Core.h"
#include "dwfcore/Exception.h"


namespace DWFCore
{

//
// fwd decl
//
class DWFCryptoEngineFactory;

///
///\interface   DWFCryptoEngine     dwfcore/crypto/CryptoEngine.h    "dwfcore/crypto/CryptoEngine.h"
///\brief       This class defines the interface for all encryption/decryption algorithm, ciphers, etc.
///\since       1.3
///
///             <write me>
///
class DWFCryptoEngine : virtual public DWFCoreMemory
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
        // Asymmetric algorithms
        //

        eRSA        = 100,
        eDSA        = 102,

        //
        // Symmetric ciphers
        //

        eDES        = 200,
        e3DES       = 201,
        eBlowfish   = 202,
        eTwofish    = 203,
        eAES        = 204,

        //
        // Digest algorithms
        //

        eMD2        = 300,
        eMD4        = 301,
        eMD5        = 302,
        eRIPEMD160  = 303,

        eSHA1       = 314,
        eSHA224     = 315,
        eSHA256     = 316,
        eSHA384     = 317,
        eSHA512     = 318,

        //
        // Custom algorithms
        //

        eGeek1      = 400,
        eGeek2      = 401,
        eGeek3      = 402

    } teType;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFCryptoEngine()
        throw()
    {;}

    ///
    ///         Uniquely identifies the algorithm or cipher implemented by the engine.
    ///
    ///\return  The identifier string
    ///\throw   None
    ///
    _DWFCORE_API
    virtual DWFCryptoEngine::teType identifier() const
        throw() = 0;

    ///
    ///         The engine factory is required to construct new instances of the engine implementation.
    ///
    ///\return  A pointer to the factory for this engine.  The caller is responsible for deleting this
    ///         object using the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFCryptoEngineFactory* factory()
        throw( DWFException ) = 0;

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFCryptoEngine()
        throw()
    {;}

};


}


#endif
#endif

