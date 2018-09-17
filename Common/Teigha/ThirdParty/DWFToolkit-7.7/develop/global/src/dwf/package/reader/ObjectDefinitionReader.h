//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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


#ifndef _DWFTK_OBJECT_DEFINITION_READER_H
#define _DWFTK_OBJECT_DEFINITION_READER_H


///
///\file        dwf/package/reader/ObjectDefinitionReader.h
///\brief       This file contains the DWFObjectDefinitionReader class declaration.
///

#include "dwfcore/Core.h"
#include "dwfcore/STL.h"
#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/package/XML.h"
#include "dwf/package/Property.h"
#include "dwf/package/DefinedObject.h"
#include "dwf/package/reader/PackageReader.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFObjectDefinitionReader   dwf/package/reader/ObjectDefinitionReader.h     "dwf/package/reader/ObjectDefinitionReader.h"
///\brief       This class implements the required XML parsing handlers to
///             compose toolkit objects from the object definitions documents and
///             provide them via the typed callbacks.
///\since       7.0.1
///
///             This class contains the processing logic particular to the known versions 
///             of the object definition documents.  The algorithm is tuned specifically for this
///             schema and is not intended to be generic.  
///
///\todo        Provide filtering for object definition parsing.
///
class DWFObjectDefinitionReader : public DWFCore::DWFXMLCallback
                                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         These enumeration flags alter the behavior of the parser
    ///         by restricting which elements are inflated into runtime
    ///         objects.  Generally this prevents unwanted object creation
    ///         (and thus memory allocations) but in some cases, additional
    ///         data processing can be avoided.
    ///
    typedef enum
    {
        ///
        ///     Parse the document only (no data objects will be created; no callbacks will be invoked.)
        ///
        eProvideNone                =   0x00,

        ///
        ///     Invoke the \a provideVersion() attribute callback.
        ///
        eProvideVersion             =   0x01,

        ///
        ///     Invoke the \a provideProperties() element callback.
        ///
        eProvideProperties          =   0x02,
        ///
        ///     Invoke the \a provideObject() element callback.
        ///
        eProvideObjects             =   0x04,
        ///
        ///     Invoke the \a provideInstance() element callback.
        ///
        eProvideInstances           =   0x08,

        ///
        ///     Equivalent to \a eProvideVersion
        ///
        eProvideAttributes          =   0x01,
        ///
        ///     Equivalent to (eProvideProperties | eProvideObjects | eProvideInstances)
        ///
        eProvideElements            =   0x0E,
		//
		//		Parse the customize property attributes
		//
		eProvideCustomizeAttributes	=	0x10,

        ///
        ///     Equivalent to (eProvideAttributes | eProvideElements | eProvideCustomizeAttributes)
        ///
        eProvideAll                 =   0xFF

    } teProviderType;

    typedef _DWFTK_STD_VECTOR(DWFString)    tStringVector;

public:

    ///
    ///         Constructor
    ///
    ///\param   pPackageReader      Provides the manifest document stream.
    ///\param   nProviderFlags      A combination of \a teProviderType flags.
    ///\throw   None
    ///
    _DWFTK_API
    DWFObjectDefinitionReader( DWFPackageReader* pPackageReader = NULL,
                               unsigned char     nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFObjectDefinitionReader()
        throw();

        //
        //
        // Provider callbacks for known attributes of the Manifest
        //
        //

    //
    //
    //
    _DWFTK_API
    DWFObjectDefinitionReader* filter() const
        throw();

    //
    //
    //
    _DWFTK_API
    void setFilter( DWFObjectDefinitionReader* pFilter )
        throw();

    ///
    ///         Accepts the object definition document version number attribute.
    ///
    ///\param   nVersion        The document version.
    ///\return  The filtered document version.
    ///\throw   None
    ///
    _DWFTK_API
    virtual double provideVersion( double nVersion)
        throw();

    ///
    ///         Accepts properties and property set references.
    ///
    ///\param   zID             The identifier of the incoming property set.
    ///\param   pPropertyRefs   A list of property set references (by ID).
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   pPropertyList   The properties that comprise this set.
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void provideProperties( const DWFString&        zID,
                                    tStringVector*          pPropertyRefs,
                                    DWFProperty::tList*     pPropertyList )
        throw();

    ///
    ///         Accepts defined object elements.
    ///
    ///\param   pObject         The new defined object. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered defined object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFDefinedObject* provideObject( DWFDefinedObject* pObject )
        throw();

    ///
    ///         Accepts defined object instance elements.
    ///
    ///\param   pInstance       The new defined object instance. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\return  The filtered instance object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFDefinedObjectInstance* provideInstance( DWFDefinedObjectInstance* pInstance )
        throw();

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
    _DWFTK_API
    void notifyStartNamespace( const char* zPrefix,
                               const char* zURI )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace()
    ///
    _DWFTK_API
    void notifyEndNamespace( const char*   zPrefix )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData()
    ///
    _DWFTK_API
    void notifyCharacterData( const char* zCData, 
                              int         nLength ) 
        throw();

protected:

    ///
    ///         DWF package content source.
    ///
    DWFPackageReader* _pPackageReader;

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

    unsigned char _nProviderFlags;
    unsigned char _nCurrentCollectionProvider;
    unsigned long _nInstanceSequence;

    DWFString               _zCurrentID;
    DWFDefinedObject*       _pCurrentObject;
    tStringVector*          _pCurrentRefs;
    DWFProperty::tList*     _pCurrentProperties;

    DWFObjectDefinitionReader* _pReaderFilter;

private:

    void _provideVersion( double nVersion)
        throw();

    void _provideProperties( const DWFString&           zID,
                             tStringVector*             pPropertyRefs,
                             DWFProperty::tList*        pPropertyList )
        throw();

    void _provideObject( DWFDefinedObject* pObject )
        throw();

    void _provideInstance( DWFDefinedObjectInstance* pInstance )
        throw();

    //
    // Not implemented
    //

    DWFObjectDefinitionReader( const DWFObjectDefinitionReader& );
    DWFObjectDefinitionReader& operator=( const DWFObjectDefinitionReader& );
};

}

#endif
