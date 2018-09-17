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

#ifndef _DWFCORE_CRYPTO_KEY_GENERATOR_FACTORY_H
#define _DWFCORE_CRYPTO_KEY_GENERATOR_FACTORY_H


///
///\file        dwfcore/crypto/CryptoKeyGeneratorFactory.h
///\brief       This header contains the declaration for the DWFCryptoKeyGeneratorFactory interface.
///


#include "dwfcore/crypto/CryptoKeyGenerator.h"


namespace DWFCore
{

///
///\interface   DWFCryptoKeyGeneratorFactory     dwfcore/crypto/CryptoKeyGeneratorFactory.h    "dwfcore/crypto/CryptoKeyGeneratorFactory.h"
///\brief       This class defines the interface for obtaining new crypto key generator implementation objects.
///\since       1.3
///
///             <write me>
///
class DWFCryptoKeyGeneratorFactory : virtual public DWFCoreMemory
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFCryptoKeyGeneratorFactory()
        throw()
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFCryptoKeyGeneratorFactory()
        throw()
    {;}

    ///
    ///         Builds a new instance of the crypto key generator implementation.
    ///
    ///\param   eKeyGeneratorType   Indicates which key generator to build as some factories may
    ///                             provide more than one. 
    ///\return  A pointer to the new KeyGenerator.  This object must be deleted by the caller
    ///         using the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFCryptoKeyGenerator* build( DWFCryptoKeyGenerator::teType eKeyGeneratorType )
        throw( DWFException ) = 0;
};

}


#endif
#endif

