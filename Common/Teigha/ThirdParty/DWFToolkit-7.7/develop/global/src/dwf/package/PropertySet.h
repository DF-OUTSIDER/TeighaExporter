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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/PropertySet.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_PROPERTY_SET_H
#define _DWFTK_PROPERTY_SET_H

///
///\file        dwf/package/PropertySet.h
///\brief       This file contains the DWFPropertySet class declaration.
///

#include "dwfcore/Vector.h"

#include "dwf/Toolkit.h"
#include "dwf/package/utility/PropertyContainer.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFPropertySet     dwf/package/PropertySet.h      "dwf/package/PropertySet.h"
///\brief       Propertysets are used to collect related properties and property sets into one
///             element.
///\since       7.2.0
///
///             Collections of related properties can be stored in a DWFPropertySet. Property
///             sets can also contain other property sets. \e Open property sets on content
///             elements allow their properties to be directly visible as properties of the
///             element. Properties of a \e closed set can only be accessed by directly
///             querying the set for the properties it contains. If a property set is closed
///             then any subsets it contains will effectively be closed in that queries
///             will not reach them unless the main set is explicitly queried, or the query
///             is told to disregard the closed set flag.
///
class DWFPropertySet : public DWFPropertyContainer
                     , public DWFXMLBuildable
                       _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

    //
    // Management of this class will occur only through the content class
    //
    friend class DWFContent;

public:

    ///
    ///\brief   This type defines a list of DWFProperty pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFPropertySet*)          tList;
    ///
    ///\brief   This type defines a DWFIterator on the list of DWFProperty pointers.
    ///
    typedef DWFVectorIterator<DWFPropertySet*, _DWFTK_STL_ALLOCATOR<DWFPropertySet*> >          tListIterator;
    ///
    ///\brief   This type defines a mapped collection of property set pointers.
    ///
    typedef DWFStringKeySkipList<DWFPropertySet*>       tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFPropertySet pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFPropertySet*>         tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFPropertySet pointers.
    ///
    typedef DWFIterator<DWFPropertySet*>                tIterator;

    ///
    ///\brief   This enum defines the attributes that this class will not resolve when
    ///         parsing attributes for the reader.
    ///
    typedef enum teUnresolvedAttributes
    {
        ePropertySetReferences

    } teUnresolvedAttributes;

    ///
    ///\brief   ID based queries on property sets can use either the schema ID
    ///         or the unique property set ID.
    ///
    typedef enum teQueryIDType
    {
        ///
        ///     The globally unique identifier of the property set, returned by id().
        ///
        eUUID,
        ///
        ///     The schema identifier of the property set, returned by getSchemaID().
        ///
        eSchemaID,
        ///
        ///     The set identifier of property set, returned by getSetID().
        ///
        eSetID

    } teQueryIDType;

public:

    ///
    ///         Constructor
    ///
    ///\param   zLabel      The optional property set label.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPropertySet( const DWFString& zLabel = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPropertySet()
        throw();

    ///
    ///         Get the associated label.
    ///
    ///\return  A string representing the label.
    ///\throw   None
    ///
    const DWFString& getLabel() const
        throw()
    {
        return _zLabel;
    }

    ///
    ///         Set the label.
    ///
    ///\param   zLabel      The new label.
    ///\throw   None
    ///
    void setLabel( const DWFString& zLabel = /*NOXLATE*/L"" )
        throw()
    {
        _zLabel = zLabel;
    }

    ///
    ///         Get the property schema ID. A set is given this ID to specify that is conforms
    ///         to a publicly known schema. This is not necessarily unique, unlike the ID
    ///         returned by \b id().
    ///
    ///\return  The property set's schema ID.
    ///\throw   None
    ///
    const DWFString& getSchemaID() const
        throw()
    {
        return _zSchemaID;
    }

    ///
    ///         Set the schema identifier for this set.
    ///
    ///\throw   None
    ///
    void setSchemaID( const DWFString& zSchemaID )
        throw()
    {
        _zSchemaID = zSchemaID;
    }

    ///
    ///         Get the property set ID. This is not unique, unlike the ID returned 
    ///         by \b id(). Multiple sets can have the same property set ID. 
    ///         The property set ID is used to scope property set queries. For example
    ///         suppose a number of content elements have many property sets, and each has 
    ///         one property set with same set ID. We can query the each element for their
    ///         properties, scoping the query to only the given property set. This is
    ///         is useful in cases where we are interested in a specific subset of all
    ///         available properties on the set owner.
    ///
    ///\return  The property set's set ID.
    ///\throw   None
    ///
    const DWFString& getSetID() const
        throw()
    {
        return _zSetID;
    }

    ///
    ///         Set the set identifier, for various usage modes that require multiple sets to
    ///         have a common way of grouping them. This should not be confused with the ID returned
    ///         by \b id() which is always unique. Multiple sets may have the same set ID.
    ///
    ///\throw   None
    ///
    void setSetID( const DWFString& zSetID )
        throw()
    {
        _zSetID = zSetID;
    }

    ///
    ///         Determine whether or not the set is closed.
    ///
    ///\return  True if the set is closed.
    ///\throw   None
    ///
    bool isClosed() const
        throw()
    {
        return _bClosed;
    }

    ///
    ///         Make the property set closed. Every closed set is required to have a 
    ///         Set ID, so that it can be access by this ID.
    ///
    ///\param   zSetID      The Set ID of the property set.
    ///\throw   DWFException.
    ///
    _DWFTK_API
    void setClosed( const DWFString& zSetID )
        throw( DWFException );

    ///
    ///         Make the property set open. This will not remove the Set ID, if it has one.
    ///         The Set ID can be reset using the \a setSetID() call.
    ///
    ///\throw   None
    ///
    void unsetClosed()
        throw()
    {
        _bClosed = false;
    }

    /////////////////////
    //
    // Property Queries

    ///
    ///         Find a property with the given name and category, in this set or in contained or referenced
    ///         subsets.
    ///
    ///\param   zName               The property name.
    ///\param   zCategory           The category of the property. The empty string is a valid category name.
    ///\param   bSearchClosedSets   By default closed property sets will not be searched.
    ///\return  A pointer to the property if found, NULL otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFProperty* getProperty( const DWFString& zName,
                                            const DWFString& zCategory = /*NOXLATE*/L"",
                                            bool bSearchClosedSets = false )
        throw( DWFException );

    ///
    ///         Find a property with the given name and category, in a subset with a specific UUID or
    ///         schema ID.
    ///
    ///\param   zID                 The Schema ID or UUID of the property set to be searched.
    ///\param   zName               The property name.
    ///\param   zCategory           The category of the property. The empty string is a valid category name.
    ///\param   eIDType             Determines whether to get the property set based on schema ID or UUID.
    ///\return  A pointer to the property if found, NULL otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    virtual const DWFProperty* getProperty( const DWFString& zID,
                                            const DWFString& zName,
                                            const DWFString& zCategory,
                                            teQueryIDType eIDType = eSchemaID )
        throw();

    ///
    ///         Access all properties that this property set owns directly or indirectly in
    ///         contained property sets.
    ///
    ///\param   bSearchClosedSets       If false closed property sets will not be searched.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFProperty::tIterator* getAllProperties( bool bSearchClosedSets = false )
        throw();

    ///
    ///         Access all properties that this property set owns directly or indirectly in
    ///         contained property sets, that occur in the given category.
    ///
    ///\param   zCategory               The category within which to search.
    ///\param   bSearchClosedSets       If false closed property sets will not be searched.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFProperty::tIterator* getAllProperties( const DWFString& zCategory,
                                                      bool bSearchClosedSets = false )
        throw();

    ///
    ///         Find the property set with the property set ID. This will recursively search contained
    ///         property sets unless told not to.
    ///
    ///\param   zID                     The unique ID or the schema id of the set being requested.
    ///\param   eIDType                 This determines whether to query using the unique id or the schema id.
    ///\return  A pointer to a property set, or NULL if not found.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFPropertySet* getPropertySet( const DWFString& zID,
                                            teQueryIDType eIDType = eSchemaID ) const
        throw();

    ///
    ///         Access all property sets contained directly in this property set.
    ///         The search is done in BFS order,
    ///         i.e. all sets at a given level will be returned before the sets they
    ///         contain.
    ///
    ///\param   bReturnClosedSets       If false closed property sets will not be returned.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFPropertySet::tIterator* getPropertySets( bool bReturnClosedSets = false )
        throw();


    /////////////////////

    ///
    ///         Create and add a property set to this set. This set will own the new property set.
    ///
    ///\param   zLabel                  The option label for the property set.
    ///\return  A pointer to the created property set.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFPropertySet* addPropertySet( const DWFString& zLabel = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add an existing property set to this set.
    ///         This set <b> will take ownership </b> of the incoming set.
    ///
    ///\param   pPropertySet    The property set to be added.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addPropertySet( DWFPropertySet* pPropertySet )
        throw( DWFException );

    ///
    ///         Add a reference to an existing property set.
    ///         This set will not take ownership of the incoming set.
    ///
    ///\param   pPropertySet    The property set to be referenced.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void referencePropertySet( DWFPropertySet* pPropertySet )
        throw( DWFException );

    ///
    ///\internal For the PropertySet use the overload
    ///         <b> parseAttributeList( const char** ppAttributeList, tUnresolvedList& rUnresolved ) </b>
    ///
    virtual void parseAttributeList( const char** /*ppAttributeList*/ )
        throw( DWFException )
    {;}

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList(const char** ppAttributeList,tUnresolvedList& rUnresolved)
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList,
                                     tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///         This is mainly for internal use to set the content in which the property
    ///         resides directly in the collection of shared property sets, or indirectly
    ///         as a contained property set of a content element or another property set.
    ///
    ///\param   pContent                The associated content
    ///\throw   None
    ///
    _DWFTK_API
    void setContent( DWFContent* pContent )
        throw();

    ///
    ///         Determine which content this set is associated with.
    ///
    ///\throw   None
    ///
    DWFContent* getContent() const
        throw()
    {
        return _pContent;
    }

    ///
    ///         Set the parent.
    ///
    ///\param   pParent         The parent property set.
    ///\throw   None
    ///
    void setParentSet( DWFPropertySet* pParent )
        throw()
    {
        _pParent = pParent;
    }

    ///
    ///         get the parent.
    ///
    ///\return  Pointer to the parent property set.
    ///\throw   None
    ///
    DWFPropertySet* getParentSet() const
        throw()
    {
        return _pParent;
    }

    ///
    ///         For property sets \a addPropertySet() should be used rather than this call.
    ///         Inserts another property container directly into the container.
    ///         This cannot enforce compile time type checking, but it will throw an exception
    ///         if the container added is not a DWFPropertySet derived class. This has been
    ///         kept here for backwards compatibility in other parts of the code.
    ///
    ///\param   pPropertySet            The container to add to this container (must not be NULL).
    ///                                 This pointer will be owned by this container
    ///                                 and deleted using the \b DWFCORE_FREE_OBJECT.
    ///\throw   DWFException is thrown if the container is not a DWFPropertySet derived class.
    ///
    _DWFTK_API
    virtual void addPropertyContainer( DWFPropertyContainer* pPropertySet )
        throw( DWFException );

    ///
    ///         For property sets <b>\a referencePropertySet() should be used</b> rather than this call.
    ///         Inserts another property container directly into the container.
    ///         This cannot enforce compile time type checking, but it will throw an exception
    ///         if the container added is not a DWFPropertySet derived class. This has been
    ///         kept here for backwards compatibility in other parts of the code.
    ///
    ///\param   rPropertySet            The property set to add to this container (must not be NULL).
    ///                                 This pointer will be owned by this container
    ///                                 and deleted using the \b DWFCORE_FREE_OBJECT.
    ///\throw   DWFException is thrown if the container is not a DWFPropertySet derived class.
    ///
    _DWFTK_API
    virtual void referencePropertyContainer( const DWFPropertyContainer& rPropertySet )
        throw( DWFException );

protected:

    ///
    ///\internal    Assigns a transient UUID.
    ///             This method is only intended for internal use
    ///             within parseAttributeList().
    ///
    ///\param       zID     The UUID to assign.
    ///\throw       None
    ///
    virtual void identify( const DWFString& zID )
        throw()
    {
        DWFPropertyContainer::identify( zID );
    }

#ifndef DWFTK_READ_ONLY

    ///
    ///\internal    This is used to serialize the attributes, when serializing to XML
    ///
    ///\param       rSerializer     The object that accepts the serialization of the object.
    ///\param       nFlags          An implementation-defined set of options for altering
    ///                             the serialization process of the attributes.
    ///\throw       None
    _DWFTK_API
    virtual void _serializeAttributes( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

    ///
    ///\internal    This is used to serialize the members of this object via
    ///             the base DWFPropertyContainer's private _oSerializer.
    ///
    ///\param       rSerializer     The object that accepts the serialization of the object.
    ///\param       nFlags          An implementation-defined set of options for altering
    ///                             the serialization process of this object.
    ///\throw       None
    ///
    _DWFTK_API
    virtual void _serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

protected:

    _DWFTK_API
    virtual void _getAllProperties( DWFProperty::tList& oProperties,
                                    DWFProperty::tMap& oMap,
                                    bool bSearchClosedSets = false )
        throw();

    _DWFTK_API
    virtual void _getAllProperties( DWFProperty::tList& oProperties,
                                    DWFProperty::tMap& oMap,
                                    const DWFString& zCategory,
                                    bool bSearchClosedSets = false )
        throw();

    _DWFTK_API
    virtual void _getPropertySets( DWFPropertySet::tList& oPropertySets,
                                  bool bReturnClosedSets = false )
        throw();

private:

    void _getProperties( DWFProperty::tList& oProperties,
                         DWFProperty::tMap& oMap,
                         const DWFProperty::tList& oOrderedProperties )
        throw();


    void _getProperties( DWFProperty::tList& oProperties,
                         DWFProperty::tMap& oMap,
                         const DWFString& zCategory,
                         DWFProperty::tMap& oSetProperties )
        throw();

protected:

    DWFString                   _zLabel;

    DWFString                   _zSchemaID;
    DWFString                   _zSetID;

    bool                        _bClosed;

    DWFContent*                 _pContent;

    //
    //  Parent Container - should be a property set or a content element.
    //  If null, it still may have an owner, just not either of the above two.
    //
    DWFPropertySet*             _pParent;

private:

    //
    // Not Implemented
    //
    DWFPropertySet( const DWFPropertySet& );
    DWFPropertySet& operator=( const DWFPropertySet& );
};

}

#endif

