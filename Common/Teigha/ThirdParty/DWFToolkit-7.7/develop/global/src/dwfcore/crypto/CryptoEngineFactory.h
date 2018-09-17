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

#ifndef _DWFCORE_CRYPTO_ENGINE_FACTORY_H
#define _DWFCORE_CRYPTO_ENGINE_FACTORY_H


///
///\file        dwfcore/crypto/CryptoEngineFactory.h
///\brief       This header contains the declaration for the DWFCryptoEngineFactory interface.
///


#include "dwfcore/crypto/CryptoEngine.h"


namespace DWFCore
{

///
///\interface   DWFCryptoEngineFactory     dwfcore/crypto/CryptoEngineFactory.h    "dwfcore/crypto/CryptoEngineFactory.h"
///\brief       This class defines the interface for obtaining new crypto engine implementation objects.
///\since       1.3
///
///             <write me>
///
class DWFCryptoEngineFactory : virtual public DWFCoreMemory
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFCryptoEngineFactory()
        throw()
    {
        ;
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFCryptoEngineFactory()
        throw()
    {
        ;
    }

    ///
    ///         Builds a new instance of the crypto engine implementation.
    ///
    ///\param   eEngineType     Indicates which engine to build as some factories maybe provide more than one. 
    ///\return  A pointer to the new engine.  This object must be deleted by the caller
    ///         using the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFCryptoEngine* build( DWFCryptoEngine::teType eEngineType )
        throw( DWFException ) = 0;
};

}


#endif
#endif
