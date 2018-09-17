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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/reader/ContentReader.h#2 $
//  $DateTime: 2011/10/11 01:26:00 $
//  $Author: caos $
//  $Change: 229047 $
//  $Revision: #2 $
//

#ifndef _DWFTK_CONTENT_READER_H
#define _DWFTK_CONTENT_READER_H

///
///\file        dwf/package/reader/ContentReader.h
///\brief       This file contains the DWFContentReader class declaration.
///

#include <stack>
#include "dwfcore/STL.h"
#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/utility/PropertyContainer.h"
namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFContentReader   dwf/package/reader/ContentReader.h     "dwf/package/reader/ContentReader.h"
///\brief       This class implements the required XML parsing handlers to compose toolkit content objects
///             from the content definitions documents and provide them via the typed callbacks.
///\since       7.2.0
///
///             This class contains the processing logic particular to the known versions
///             of the content definition documents.  The algorithm is tuned specifically for this
///             schema and is not intended to be generic.
///
class DWFContentReader : public DWFCore::DWFXMLCallback
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
        eProvideNone                =   0x000,

        ///
        ///     Invoke the \a provideClass() element callback.
        ///
        eProvideClasses             =   0x001,
        ///
        ///     Invoke the \a provideFeature() element callback.
        ///
        eProvideFeatures            =   0x002,
        ///
        ///     Invoke the \a provideEntity() element callback.
        ///
        eProvideEntities            =   0x004,
        ///
        ///     Invoke the \a provideObject() element callback.
        ///
        eProvideObjects             =   0x008,
        ///
        ///     Invoke the \a provideGroup() element callback.
        ///
        eProvideGroups              =   0x010,
        ///
        ///     Invoke the \a provideSharedProperty() element callback.
        ///
        eProvideSharedProperties    =   0x020,

        ///
        ///     Invoke the \a provideVersion() attribute callback.
        ///
        eProvideVersion             =   0x100,

        ///
        ///     Equivalent to (eProvideClasses | eProvideFeatures | eProvideEntities | eProvideObjects | eProvideGroups | eProvideInstances | eProvideSharedProperties )
        ///
        eProvideElements            =   0x0FF,
        ///
        ///     Equivalent to (eProvideVersion)
        ///
        eProvideAttributes          =   0xF00,
		//
		//		Parse the customize property attributes
		//
		eProvideCustomizeAttributes	=	0x1000,

        ///
        ///     Equivalent to (eProvideElements | eProvideAttributes | eProvideCustomizeAttributes)
        ///
        eProvideAll                 =   0xFFFF

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
    DWFContentReader( unsigned int nProviderFlags = eProvideAll )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentReader()
        throw();

    ///
    ///         User defined filter on the object definition reader. The filter is
    ///         used to modify the parsing behavior.
    ///
    ///\return  A pointer to a filter, an object of a class derived from the content reader
    ///         with user defined behavior.
    ///\throw   None
    ///
    DWFContentReader* filter() const
        throw()
    {
        return _pFilter;
    }

    ///
    ///         Use this to set the filter on the reader to modify the parsing behavior.
    ///
    ///\throw   None
    ///
    void setFilter( DWFContentReader* pFilter )
        throw()
    {
        _pFilter = pFilter;
    }

    ///
    ///         Get the current provider flags set on the reader.
    ///
    ///\return  The current set of provider flags set on the reader.
    ///\throw   None
    ///
    unsigned int providerFlags() const
    {
        return _nProviderFlags;
    }

    ///
    ///         Use this to set the provider flags before parsing the document, if they
    ///         need to be different from the default.
    ///
    ///\param   nProviderFlags      The new set of provider flags.
    ///\throw   None
    ///
    void setProviderFlags( unsigned int nProviderFlags )
    {
        _nProviderFlags = nProviderFlags;
    }

    ////

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartElement(const char* zName, const char**  ppAttributeList)
    ///
    _DWFTK_API
    void notifyStartElement( const char*   zName,
                             const char**  ppAttributeList )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndElement(const char* zName)
    ///
    _DWFTK_API
    void notifyEndElement( const char*     zName )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartNamespace(const char* /*zPrefix*/, const char* /*zURI*/)
    ///
    void notifyStartNamespace( const char* /*zPrefix*/,
                               const char* /*zURI*/ )
        throw()
    {;}

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace(const char* /*zPrefix*/)
    ///
    void notifyEndNamespace( const char*   /*zPrefix*/ )
        throw()
    {;}

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData(const char* /*zCData*/, int /*nLength*/)
    ///
    void notifyCharacterData( const char* /*zCData*/,
                              int         /*nLength*/ )
        throw()
    {;}

    ///
    ///         Accepts the content document version number attribute.
    ///
    ///\param   zVersion        The document version.
    ///\return  The filtered document version.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const char* provideVersion( const char* zVersion )
        throw( DWFException );

    ///
    ///         Accepts class element objects.
    ///
    ///\param   pClass          The new class. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   rUnresolved     List of unresolved attributes from the parsing process.
    ///\return  The filtered class object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFClass* provideClass( DWFClass* pClass,
                                    const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Accepts feature element objects.
    ///
    ///\param   pFeature            The new feature. 
    ///                             The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   rUnresolved     List of unresolved attributes from the parsing process.
    ///\return  The filtered feature object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFFeature* provideFeature( DWFFeature* pFeature,
                                        const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Accepts entity element objects.
    ///
    ///\param   pEntity             The new entity. 
    ///                             The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   rUnresolved     List of unresolved attributes from the parsing process.
    ///\return  The filtered entity object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFEntity* provideEntity( DWFEntity* pEntity,
                                      const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Accepts object element objects.
    ///
    ///\param   pObject         The new object. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   rUnresolved     List of unresolved attributes from the parsing process.
    ///\return  The filtered object object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFObject* provideObject( DWFObject* pObject,
                                      const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Accepts group element objects.
    ///
    ///\param   pGroup          The new group. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   rUnresolved     List of unresolved attributes from the parsing process.
    ///\return  The filtered group object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFGroup* provideGroup( DWFGroup* pGroup,
                                    const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Accepts property sets targeted for the shared property collection.
    ///
    ///\param   pPropertySet    The new shared property set. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   rUnresolved     List of unresolved attributes from the parsing process.
    ///\return  The filtered property object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFPropertySet* provideSharedProperty( DWFPropertySet* pPropertySet,
                                                   const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Accepts content objects that have parent content objects.
    ///
    ///\param   pObject         The new object. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   pParent         The content object that will become the parent.
    ///\param   rUnresolved     List of unresolved attributes from the parsing process.
    ///\return  The filtered object object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFObject* provideChildObject( DWFObject* pObject,
                                           DWFObject* pParent,
                                           const tUnresolvedList& rUnresolved )
       throw( DWFException );

    ///
    ///         Accepts property sets contained by content elements or other property sets.
    ///
    ///\param   pPropertySet    The new shared property set. 
    ///                         The acceptor is responsible for deleting this pointer with \b DWFCORE_FREE_OBJECT.
    ///\param   pContainer      The content element or property set that will own the property set.
    ///\param   rUnresolved     List of unresolved attributes from the parsing process.
    ///\return  The filtered property object.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFPropertySet* providePropertySet( DWFPropertySet* pPropertySet,
                                                DWFPropertyContainer* pContainer,
                                                const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         Resolve the string based ID references into actual pointers.
    ///         This is called once the reader has finished reading the collection
    ///         of classes.
    ///
    ///\throw   None
    ///
    virtual void resolveClasses()
        throw()
    {;}

    ///
    ///         Resolve the string based ID references into actual pointers.
    ///         This is called once the reader has finished reading the collection
    ///         of features.
    ///
    ///\throw   None
    ///
    virtual void resolveFeatures()
        throw()
    {;}

    ///
    ///         Resolve the string based ID references into actual pointers.
    ///         This is called once the reader has finished reading the collection
    ///         of entities.
    ///
    ///\throw   None
    ///
    virtual void resolveEntities()
        throw()
    {;}

    ///
    ///         Resolve the string based ID references into actual pointers.
    ///         This is called once the reader has finished reading the collection
    ///         of objects.
    ///
    ///\throw   None
    ///
    virtual void resolveObjects()
        throw()
    {;}

    ///
    ///         Resolve the string based ID references into actual pointers.
    ///         This is called once the reader has finished reading the collection
    ///         of groups.
    ///
    ///\throw   None
    ///
    virtual void resolveGroups()
        throw()
    {;}

    ///
    ///         Resolve the string based ID references into actual pointers.
    ///         This is called once the reader has finished reading all property sets,
    ///         i.e. when all collections are complete.
    ///
    ///\throw   None
    ///
    virtual void resolvePropertySets()
        throw()
    {;}

private:

    _DWFTK_API
    void _provideClass()
        throw( DWFException );

    _DWFTK_API
    void _provideFeature()
        throw( DWFException );

    _DWFTK_API
    void _provideEntity()
        throw( DWFException );

    _DWFTK_API
    void _provideObject()
        throw( DWFException );

    _DWFTK_API
    void _provideGroup()
        throw( DWFException );

    _DWFTK_API
    void _provideSharedProperty()
        throw( DWFException );

    _DWFTK_API
    void _provideChildObject()
        throw( DWFException );

    _DWFTK_API
    void _providePropertySet()
        throw( DWFException );

    _DWFTK_API
    void _provideVersion( const char* zVersion )
        throw( DWFException );

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

    typedef std::stack< DWFString >                         _tStringStack;
    typedef std::stack< DWFPropertyContainer* >             _tPropertyContainerStack;

    typedef std::stack< tUnresolvedList, _DWFTK_STD_VECTOR(tUnresolvedList) >
                                                            _tUnresolvedListStack;

private:

    unsigned int                _nCurrentCollectionProvider;
    unsigned int                _nProviderFlags;

    DWFXMLBuildable*            _pCurrentElement;
    DWFContentReader*           _pFilter;

    //
    //  As we go deeper into the nested object XML elements, information to complete
    //  the objects are stored on the stacks, and used during the element close. The
    //  object itself is stored on the PropertyContainerStack.
    //
    //
    //  Property Sets can also be arbitrarily nested. The properties and the nested
    //  property sets can live inside top level property sets or as a part of 
    //  content elements, namely Classes, Features, Entities, Objects  and Groups.
    //
    //  Content Element Property Structure:
    //  <Object id="xyz1" label="abc1">
    //      <Property .../>
    //      <Property .../>
    //      ...
    //      <Properties id="xyz21" label="abc21"/>
    //          ...
    //      </Properties>
    //      <Properties id="xyz22" label="abc22"/>
    //          ...
    //      </Properties>
    //      ...
    //  </Object>
    //
    //  Property Set Structure:
    //  <Properties id="zyx1" label="cab1"> // This is a property set
    //      <Property .../>
    //      <Property .../>
    //      ...
    //      <Properties id="zyx21" label="cab21"/>
    //          ...
    //      </Properties>
    //      <Properties id="zyx22" label="cab22"/>
    //          ...
    //      </Properties>
    //      ...
    //  </Properties>
    //

    _tPropertyContainerStack            _oPropertyContainer;
    _tUnresolvedListStack               _oUnresolved;

private:

    //
    // Not implemented
    //
    DWFContentReader( const DWFContentReader& );
    DWFContentReader& operator=( const DWFContentReader& );

};

}

#endif
