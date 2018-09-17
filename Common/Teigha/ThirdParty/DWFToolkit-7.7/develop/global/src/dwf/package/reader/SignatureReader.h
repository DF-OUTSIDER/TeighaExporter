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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/reader/SignatureReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_SIGNATURE_READER_H
#define _DWFTK_SIGNATURE_READER_H

///
///\file        dwf/package/SignatureReader.h
///\brief       This file contains the DWFSignatureReader class declaration.
///

#include <stack>
#include "dwfcore/STL.h"
#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/X509.h"

namespace DWFToolkit
{

// forward decls
class DWFSignature;


///
///\ingroup     dwfpackage
///
///\class       DWFSignatureReader   dwf/package/reader/SignatureReader.h     "dwf/package/reader/SignatureReader.h"
///\brief       This class implements the required XML parsing handlers to compose toolkit signature objects
///             from the signature resource documents and provide them via the typed callbacks.
///\since       7.2.0
///
///             This class contains the processing logic particular to the known versions
///             of the signature documents.  The algorithm is tuned specifically for this
///             schema and is not intended to be generic.
///
class DWFSignatureReader : public DWFCore::DWFXMLCallback
                                 _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         These enumeration flags alter the behavior of the parser by restricting which
    ///         elements are inflated into runtime objects.  Generally these only prevent unwanted
    ///         object creation (and thus memory allocations) but in some cases, additional data
    ///         processing can be avoided.
    ///
    typedef enum teProviderType
    {
        ///
        ///     Parse the document only (no data objects will be created; no callbacks will be invoked.)
        ///
        eProvideNone                =   0x00,

        ///
        ///     Invoke the \a provideSignatureID() callback.
        ///
        eProvideSignatureID         =   0x01,

        ///
        ///     Invoke the \a provideSignature() element callback.
        ///
        eProvideSignature           =   0x10,

        ///
        ///     Equivalent to (eProvideSignatureID)
        ///
        eProvideAttributes          =   0x0F,

        ///
        ///     Equivalent to (eProvideSignatures)
        ///
        eProvideElements            =   0xF0,

        ///
        ///     Equivalent to (eProvideElements | eProvideAttributes)
        ///
        eProvideAll                 =   0xFF

    } teProviderType;

    ///
    ///\brief   Defined data type to collect unresolved attributes during the parsing process.
    ///
    typedef DWFXMLBuildable::tUnresolvedList            tUnresolvedList;


    struct RSAKeyValueStrings
    {
        DWFString _zModulus;
        DWFString _zExponent;
    };

    struct DSAKeyValueStrings
    {
        DWFString _zP;
        DWFString _zQ;
        DWFString _zG;
        DWFString _zY;
        DWFString _zJ;
        DWFString _zSeed;
        DWFString _zPgenCounter;
    };

public:

    ///
    ///         Constructor
    ///
    ///\param   pPackageReader      Provides the signature document stream.
    ///\param   nProviderFlags      A combination of \a teProviderType flags.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSignatureReader( DWFPackageReader* pPackageReader = NULL,
                        unsigned int      nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSignatureReader()
        throw();
    
    ///
    ///         To read a resource using the signature reader, the reader needs
    ///         to know the object ID of the resource it will read. If this is null
    ///         the reader will throw an exception later.
    ///
    ///\param   zObjectID   The object ID of the resource being read in.
    ///\throw   None
    ///
    _DWFTK_API
    void setResourceObjectID( const DWFString& zObjectID )
        throw();

    ///
    ///         User defined filter on the signature reader. The filter is
    ///         used to modify the parsing behavior.
    ///
    ///\return  A pointer to a filter, an object of a class derived from the signature reader
    ///         with user defined behavior.
    ///\throw   None
    ///
    DWFSignatureReader* filter() const
        throw()
    {
        return _pFilter;
    }

    ///
    ///         Use this to set the filter on the reader to modify the parsing behavior.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    void setFilter( DWFSignatureReader* pFilter )
        throw();

    ////

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartElement()
    ///
    _DWFTK_API
    void notifyStartElement( const char*   zName,
                             const char**  ppAttributeList )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndElement()
    ///
    _DWFTK_API
    void notifyEndElement( const char*     zName )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartNamespace()
    ///
    void notifyStartNamespace( const char* /*zPrefix*/,
                               const char* /*zURI*/ )
        throw()
    {;}

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace()
    ///
    void notifyEndNamespace( const char*   /*zPrefix*/ )
        throw()
    {;}

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData()
    ///
    _DWFTK_API
    void notifyCharacterData( const char* zCData,
                              int         nLength )
        throw();

    ///
    ///         Accepts the signature id attribute.
    ///
    ///\param   zId        The signature id.
    ///\return  The filtered signature id.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual const char* provideSignatureID( const char* zId )
        throw( DWFException );

    ///
    ///         Accepts references.
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

private:

    _DWFTK_API
    void _provideSignatureID( const char* zId )
        throw( DWFException );

    _DWFTK_API
    void _provideReference( const DWFString& zReferenceURI )
        throw( DWFException );

    _DWFTK_API
    void _provideSignatureMethod( const DWFString& zSigMethod )
        throw( DWFException );

    _DWFTK_API
    void _provideSignatureValue( const DWFString& zSigValue )
        throw( DWFException );

    _DWFTK_API
    void _provideX509Data( X509Data *pX509Data )
        throw( DWFException );

    _DWFTK_API
    void _provideDigestMethod( const DWFString& zDigestMethod )
        throw( DWFException );

    _DWFTK_API
    void _provideDigestValue( const DWFString& zDigestValue )
        throw( DWFException );

    _DWFTK_API
    void _provideRSAKeyValueStrings( const DWFSignatureReader::RSAKeyValueStrings& rRSAKeyValueStrings )
        throw( DWFException );

    _DWFTK_API
    void _provideDSAKeyValueStrings( const DWFSignatureReader::DSAKeyValueStrings& rDSAKeyValueStrings )
        throw( DWFException );

    _DWFTK_API
    void _provideKeyName( const DWFString& zKeyName )
        throw( DWFException );


    _DWFTK_API
    DWFString _findAttributeValue( const char*  pAttributeName,
                                   const char** ppAttributeList )
        throw();

protected:

    DWFPackageReader*           _pPackageReader;

    ///
    ///         Default building behavior and element object allocator for readers.
    ///
    ///
    DWFXMLElementBuilder        _oDefaultElementBuilder;

    ///
    ///         This will point to the default builder initially.
    ///         But implementation classes can alter this pointer as necessary
    ///         but they should all use only this pointer to do the actual work.
    ///
    DWFXMLElementBuilder*       _pElementBuilder;

private:

    unsigned int                _nCurrentCollectionProvider;
    unsigned int                _nProviderFlags;

    DWFSignatureReader*         _pFilter;

    DWFString                   _zResourceObjectID;
    DWFString                   _zCDataAccumulator;

    DWFSignatureReader::DSAKeyValueStrings _oCurrentDSAKeyValueStrings;
    DWFSignatureReader::RSAKeyValueStrings _oCurrentRSAKeyValueStrings;

    X509Data*                   _pCurrentX509Data;
    X509IssuerSerial*           _pCurrentX509IssuerSerial;

private:

    //
    // Not implemented
    //
    DWFSignatureReader( const DWFSignatureReader& );
    DWFSignatureReader& operator=( const DWFSignatureReader& );

};

}


#endif


