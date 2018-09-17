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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/ContentElement.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//


#ifndef _DWFTK_CONTENT_ELEMENT_H
#define _DWFTK_CONTENT_ELEMENT_H

///
///\file        dwf/package/ContentElement.h
///\brief       This file contains the DWFContentElement class declaration.
///

#include "dwf/package/PropertySet.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFContentElement     dwf/package/ContentElement.h      "dwf/package/ContentElement.h"
///\brief       ContentElement is the base class for the object defintion elements
///             class, feature, entity, object and group.
///\since       7.2.0
///
///             The DWFContentElement class is the base class for the object definition elements
///             used to define a model. It provides the property management API specific to
///             the object definition elements, including property inheritance.
///
class DWFContentElement : public DWFPropertySet
                          _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

    // Management of this will occur only through the content class
    friend class DWFContent;

public:

    ///
    ///\brief   This type defines a list of DWFContentElement pointers.
    ///
    typedef DWFOrderedVector<DWFContentElement*>            tList;
    ///
    ///\brief   This type defines a mapped collection of DWFContenteElement pointers.
    ///
    typedef DWFStringKeySkipList<DWFContentElement*>        tMap;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFContenteElement pointers.
    ///
    typedef DWFIterator<DWFContentElement*>                 tIterator;
    ///
    ///\brief   This type defines an STL vector of DWFContentElement pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFContentElement*)  tVector;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentElement()
        throw();

    ///
    ///         Find a property with the given name and category. Properties can
    ///         come from multiple possible sources. They may be:
    ///             1.  owned by the element. This will have priority over any
    ///                 property with the same name in any property set.
    ///             2.  in property sets owned by the element. If two property
    ///                 sets in the element have the same property, then the one
    ///                 listed first will be returned. The order is defined by the
    ///                 order in which the property sets were added to the element.
    ///             3.  owned by property sets referenced by the element.
    ///             4.  in property sets owned by referenced property containers.
    ///                 If two property sets in the referenced container have the
    ///                 property, again, the first found is returned.
    ///             5.  provided as inherited properties from content elements
    ///                 that are higher up in the object defintion model, that
    ///                 this element is a direct or indirect descendent of. The
    ///                 property of the ancestral element may itself be discovered
    ///                 from sources, as given by points 1 through 5 here.
    ///         The search is performed in the above order so the most derived
    ///         element provides the property. By default closed property sets will not
    ///         be searched, unless the user requests it.
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
    ///         Find a property with the given name and category, in a subset with a specific schema ID.
    ///
    ///\param   zSchemaID           The Schema ID of the property set to be searched.
    ///\param   zName               The property name.
    ///\param   zCategory           The category of the property.
    ///\return  A pointer to the property if found, NULL otherwise.
    ///\throw   None
    ///
    virtual const DWFProperty* getPropertyBySchemaID( const DWFString& zSchemaID,
                                                      const DWFString& zName,
                                                      const DWFString& zCategory = /*NOXLATE*/L"" )
        throw()
    {
        return DWFPropertySet::getProperty( zSchemaID, zName, zCategory, eSchemaID );
    }

    ///
    ///         Find a property with the given name and category, in a subset with a specific set ID.
    ///
    ///\param   zSetID              The Set ID of the property set to be searched.
    ///\param   zName               The property name.
    ///\param   zCategory           The category of the property.
    ///\return  A pointer to the property if found, NULL otherwise.
    ///\throw   None
    ///
    virtual const DWFProperty* getPropertyBySetID( const DWFString& zSetID,
                                                   const DWFString& zName,
                                                   const DWFString& zCategory = /*NOXLATE*/L"" )
        throw()
    {
        return DWFPropertySet::getProperty( zSetID, zName, zCategory, eSetID );
    }

    ///
    ///         Access all properties that this element owns, references, and/or inherits.
    ///         These are the default properties.
    ///
    ///\param   bSearchClosedSets   By default closed property sets will not be searched.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFProperty::tIterator* getAllProperties( bool bSearchClosedSets = false )
        throw();

    ///
    ///         Access all properties that this element owns, references, inherits,
    ///         provided they are in the given category. These are a subset of the default properties.
    ///
    ///\param   zCategory           The category of the property. The empty string is a valid category name.
    ///\param   bSearchClosedSets   By default closed property sets will not be searched.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFProperty::tIterator* getAllProperties( const DWFString& zCategory,
                                                      bool bSearchClosedSets = false )
        throw();

    _DWFTK_API
    virtual DWFProperty::tIterator* getAllPropertiesBySchemaID( const DWFString& zSchemaID )
        throw();

    _DWFTK_API
    virtual DWFProperty::tIterator* getAllPropertiesBySetID( const DWFString& zSetID )
        throw();

    ///         Find a property set with the given UUID. Property sets can come
    ///         from one of multiple sources. They may be:
    ///             1.  owned by the element.
    ///             2.  owned by a property containers referenced by the element.
    ///             3.  inherited from elements that are higher up in the object
    ///                 defintion model, that this element is a direct or indirect
    ///                 descendent of. The property set of the ancestral element
    ///                 is itself be discovered from sources, as given by points
    ///                 1 through 3 here.
    ///
    ///\param   zID                 The Schema ID or UUID of the property set to be searched.
    ///\param   eIDType             Determines whether to get the property set based on schema ID or UUID.
    ///\return  A pointer to the property set if found, NULL otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFPropertySet* getPropertySet( const DWFString& zID,
                                            teQueryIDType eIDType = eSchemaID )
        throw();

    ///         Access all property sets available to the element, namely:
    ///             1.  owned by the element.
    ///             2.  owned by a property sets referenced by the element.
    ///             3.  inherited from elements that are higher up in the object
    ///                 defintion model, that this element is a direct or indirect
    ///                 descendent of. The property set of the ancestral element
    ///                 is itself discovered from sources, as given by points
    ///                 1 through 3 here.
    ///         By default closed property sets will not be returned unless
    ///         requested by the caller.
    ///
    ///\param   bReturnClosedSets   By default closed property sets will not be returned.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFPropertySet::tIterator* getPropertySets( bool bReturnClosedSets = false )
        throw();

    ///
    ///\internal For the ContentElement derived classes use the overload
    ///         <b> parseAttributeList( const char** ppAttributeList, tUnresolvedList& rUnresolved ) </b>
    ///
    virtual void parseAttributeList( const char** /*ppAttributeList*/ )
        throw( DWFException )
    {;}

protected:

    ///
    ///         Constructor
    ///
    ///\param   zID         The UUID assigned to the object.
    ///\param   pContent    The content that owns this element.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentElement( const DWFString& zID = /*NOXLATE*/L"",
                       DWFContent* pContent = NULL )
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFPropertySet::_serializeAttributes()
    ///
    _DWFTK_API
    virtual void _serializeAttributes( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

    ///
    ///\copydoc DWFPropertySet::_serializeXML()
    ///
    _DWFTK_API
    virtual void _serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

    ///
    ///\internal    This inserts into the provided list the ancestors elements
    ///             from which this element can inherit properties. This should be
    ///             implemented in all classes derived from DWFContentElement if they
    ///             have classes from which to inherit properties.
    ///
    ///\param       rAncestorElements   Reference to an vector of content elements to which
    ///                                 the elements's "property ancestors" will be added.
    ///\throw       None
    ///
    virtual void insertPropertyAncestors( DWFContentElement::tVector& /*rAncestorElements*/ ) const
        throw()
    {;}

private:

    //
    // Not Implemented
    //
    DWFContentElement( const DWFContentElement& );
    DWFContentElement& operator=( const DWFContentElement& );
};

}

#endif
