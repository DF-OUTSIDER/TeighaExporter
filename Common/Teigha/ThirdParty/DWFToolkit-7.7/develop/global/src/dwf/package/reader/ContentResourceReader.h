//
//  Copyright (c) 2005-2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/reader/ContentResourceReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_CONTENT_RESOURCE_READER_H
#define _DWFTK_CONTENT_RESOURCE_READER_H

///
///\file        dwf/package/reader/ContentResourceReader.h
///\brief       This file contains the DWFContentResourceReader class declaration.
///

#include <stack>
#include "dwfcore/STL.h"
#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFContentResourceReader   dwf/package/reader/ContentResourceReader.h     "dwf/package/reader/ContentResourceReader.h"
///\brief       This class implements the required XML parsing handlers to compose toolkit content objects
///             from the content resource documents and provide them via the typed callbacks.
///\since       7.2.0
///
///             This class contains the processing logic particular to the known versions
///             of the content definition documents.  The algorithm is tuned specifically for this
///             schema and is not intended to be generic.
///
class DWFContentResourceReader : public DWFCore::DWFXMLCallback
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
        ///     Invoke the \a provideContentResourceVersion() callback.
        ///
        eProvideVersion             =   0x01,

        ///
        ///     Invoke the \a provideInstance() element callback.
        ///
        eProvideInstances           =   0x10,

        ///
        ///     Equivalent to (eProvideVersion | eProvideObjectID | eProvideContentID)
        ///
        eProvideAttributes          =   0x0F,

        ///
        ///     Equivalent to eProvideInstances
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

public:

    ///
    ///         Constructor
    ///
    ///\param   nProviderFlags      A combination of \a teProviderType flags.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentResourceReader( unsigned int nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentResourceReader()
        throw();
    
    ///
    ///         To read a resource using the content resource reader, the reader needs
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
    ///         User defined filter on the object definition reader. The filter is
    ///         used to modify the parsing behavior.
    ///
    ///\return  A pointer to a filter, an object of a class derived from the content reader
    ///         with user defined behavior.
    ///\throw   None
    ///
    DWFContentResourceReader* filter() const
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
    void setFilter( DWFContentResourceReader* pFilter )
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
    void notifyCharacterData( const char* /*zCData*/,
                              int         /*nLength*/ )
        throw()
    {;}

    ///
    ///         Accepts the content resource version number attribute.
    ///
    ///\param   zVersion        The document version.
    ///\return  The filtered document version.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const char* provideContentResourceVersion( const char* zVersion )
        throw( DWFException );

    ///
    ///         Accepts instance element objects.
    ///
    ///\param   pInstance           The new instance. 
    ///                             The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   zRenderableID       The renderable element ID of the instance.
    ///\param   zResourceObjectID   The ID of the resource from which this instance is being read.
    ///\return  The filtered instance object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFInstance* provideInstance( DWFInstance* pInstance, 
                                          const DWFString& zRenderableID,
                                          const DWFString& zResourceObjectID )
        throw( DWFException );

    ///
    ///         Resolve the string based ID references into actual pointers.
    ///         This is called once the reader has finished reading the collection
    ///         of instances.
    ///
    ///\throw   None
    ///
    virtual void resolveInstances()
        throw()
    {;}

private:

    _DWFTK_API
    void _provideContentResourceVersion( const char* zVersion )
        throw( DWFException );

    _DWFTK_API
    void _provideInstance( DWFInstance* pInstance )
        throw( DWFException );

    _DWFTK_API
    DWFString _findAttributeValue( const char*  pAttributeName,
                                   const char** ppAttributeList )
        throw();

protected:

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

    DWFInstance*                _pCurrentInstance;
    DWFContentResourceReader*   _pFilter;

    DWFString                   _zRenderableID;

    DWFString                   _zResourceObjectID;

private:

    //
    // Not implemented
    //
    DWFContentResourceReader( const DWFContentResourceReader& );
    DWFContentResourceReader& operator=( const DWFContentResourceReader& );

};

}


#endif


