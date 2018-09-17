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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Entity.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//

#ifndef _DWFTK_ENTITY_H
#define _DWFTK_ENTITY_H

///
///\file        dwf/package/Entity.h
///\brief       This file contains the DWFEntity class declaration.
///

#include "dwfcore/STL.h"

#include "dwf/Toolkit.h"
#include "dwf/package/Class.h"
#include "dwf/package/Feature.h"

namespace DWFToolkit
{ 

///
///\ingroup     dwfpackage
///
///\class       DWFEntity     dwf/package/Entity.h      "dwf/package/Entity.h"
///\brief       Entities are used to represent a design element that can be used in a model
///             like \e a door knob or \e a laptop.  
///\since       7.2.0
///
///             Entities are used to represent a design element that can be used in a model
///             like \e a door knob or \e a laptop. They can be composed of multiple sub-parts
///             or child entities, just as a laptop can be composed of multiple parts.
///
class DWFEntity : public DWFContentElement
                _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

    //
    // Management of this class will occur only through the content class
    //
    friend class DWFContent;
    //
    // During reading, the DWFXMLElementBuilder needs to create these classes
    //
    friend class DWFXMLElementBuilder;

public:

    ///
    ///\brief   This type defines a list of DWFEntity pointers.
    ///
    typedef DWFOrderedVector<DWFEntity*>            tList;
    ///
    ///\brief   This type defines a sorted list of DWFEntity pointers.
    ///
    typedef DWFSortedVector<DWFEntity*>             tSortedList;
    ///
    ///\brief   This type defines a mapped collection of DWFEntity pointers.
    ///
    typedef DWFStringKeySkipList<DWFEntity*>        tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFEntity pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFEntity*>          tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFEntity pointers.
    ///
    typedef DWFIterator<DWFEntity*>                 tIterator;
    ///
    ///\brief   This type defines a basic const iterator on a collection of DWFEntity pointers.
    ///
    typedef DWFConstIterator<DWFEntity*>            tConstIterator;

    ///
    ///         This enum defines the attributes that this class will not resolve when
    ///         parsing attributes for the reader.
    ///
    typedef enum teUnresolvedAttributes
    {
        //
        ePropertySetReferences  = DWFPropertySet::ePropertySetReferences,

        //
        eChildEntityReferences  = DWFPropertySet::ePropertySetReferences + 1,

        //
        eClassReferences        = DWFPropertySet::ePropertySetReferences + 2,

        //
        eFeatureReferences      = DWFPropertySet::ePropertySetReferences + 3

    } teUnresolvedAttributes;


public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFEntity()
        throw();

    ///
    ///         Access the child entities via the iterator.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFEntity::tSortedList::ConstIterator* getChildren()
        throw()
    {
        return _oChildren.constIterator();
    }

    ///
    ///         Get the number of child entities.
    ///
    ///\return  Number of child entities.
    ///\throw   None
    ///
    size_t getChildCount() const
        throw()
    {
        return _oChildren.size();
    }

    ///
    ///         Access the parents via the iterator.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFEntity::tSortedList::ConstIterator* getParents()
        throw()
    {
        return _oParents.constIterator();
    }

    ///
    ///         Access a parent entity by its index in the parent list.
    ///
    ///\return  A pointer to a parent entity.
    ///\throw   None
    ///
    // Most often an entity will have only 1 parent - this allows simpler
    // easier access to the single parent using getParent(0)
    DWFEntity* getParent( size_t index ) const
        throw()
    {
        return _oParents[ index ];
    }

    ///
    ///         Get the number of parent entities.
    ///
    ///\return  Number of parent entities.
    ///\throw   None
    ///
    size_t getParentCount() const
        throw()
    {
        return _oParents.size();
    }

    ///
    ///         This tests whether or not the given entity a parent of this one.
    ///
    ///\param   pEntity     A pointer to an entity.
    ///\return  True if the entity is a parent, false otherwise.
    ///\throw   None
    ///
    bool isParent( DWFEntity* pEntity ) const
        throw()
    {
        size_t iLocation;
        return _oParents.findFirst( pEntity, iLocation );
    }

    ///
    ///         Get an iterator to access all classes in the entity's class list.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFClass::tList::Iterator* getClasses()
        throw()
    {
        return (_oClasses.size() > 0 ? _oClasses.iterator() : NULL);
    }

    ///
    ///         Get the number of classes.
    ///
    ///\return  Number of classes.
    ///\throw   None
    ///
    size_t getClassCount() const
        throw()
    {
        return _oClasses.size();
    }

    ///
    ///         This tests whether or not the given class is refered to by the entity.
    ///
    ///\param   pClass      A pointer to a class.
    ///\return  True if the class is in the class list.
    ///\throw   None
    ///
    bool hasClass( DWFClass* pClass ) const
        throw()
    {
        size_t iLocation;
        return _oClasses.findFirst( pClass, iLocation );
    }   

    ///
    ///         Get an iterator to access all features associated with the entity.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFFeature::tList::Iterator* getFeatures()
        throw()
    {
        return (_oFeatures.size() > 0 ? _oFeatures.iterator() : NULL);
    }

    ///
    ///         Get the number of features associated with the entity.
    ///
    ///\return  Number of features associated with the entity.
    ///\throw   None
    ///
    size_t getFeatureCount() const
        throw()
    {
        return _oFeatures.size();
    }

    ///
    ///         This tests whether or not the given feature is refered to by the entity.
    ///
    ///\param   pFeature    A pointer to a feature.
    ///\return  True if the feature is in the feature list.
    ///\throw   None
    ///
    bool hasFeature( DWFFeature* pFeature ) const
        throw()
    {
        size_t iLocation;
        return _oFeatures.findFirst( pFeature, iLocation );
    }   

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList(const char**,tUnresolvedList&)
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList,
                                     tUnresolvedList& rUnresolved )
        throw( DWFException );

protected:

    ///
    ///         Constructor
    ///         The constructor is for use by the DWFContent and the DWFXMLElementBuilder,
    ///         during publishing and reading respectively. It should not be used to create
    ///         the element directly.
    ///
    ///\param   zID         The UUID to be assigned to the entity.
    ///\param   pContent    The content that owns this element.
    ///\throw   None
    ///
    _DWFTK_API
    DWFEntity( const DWFString& zID = /*NOXLATE*/L"",
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
    ///\copydoc DWFContentElement::insertPropertyAncestors()
    ///
    _DWFTK_API
    virtual void insertPropertyAncestors( DWFContentElement::tVector& rAncestorElements ) const
        throw();

private:

    ///
    ///         Add a class to provide \e type information to this entity.
    ///
    ///\param   pClass     A pointer to an class.
    ///\throw   None
    ///
    void _addClass( DWFClass* pClass )
        throw()
    {
        _oClasses.push_back( pClass );
    }

    ///
    ///         Remove a class if it is present in the entity's class list.
    ///
    ///\param   pClass     A pointer to an class.
    ///\return  True if the class was found and removed, false otherwise.
    ///\throw   None
    ///
    bool _removeClass( DWFClass* pClass )
        throw()
    {
        return _oClasses.erase( pClass );
    }

    ///
    ///         Remove all classes in the entity's class list.
    ///
    ///\throw   None
    ///
    void _removeClasses()
        throw()
    {
        _oClasses.clear();
    }

    ///
    ///         Add a feature to this entity.
    ///
    ///\param   pFeature     A pointer to a feature.
    ///\throw   None
    ///
    void _addFeature( DWFFeature* pFeature )
        throw()
    {
        _oFeatures.push_back( pFeature );
    }

    ///
    ///         Remove a feature if it is present in the entity's feature list.
    ///
    ///\param   pFeature     A pointer to a feature.
    ///\return  True if the feature was found and removed, false otherwise.
    ///\throw   None
    ///
    bool _removeFeature( DWFFeature* pFeature )
        throw()
    {
        return _oFeatures.erase( pFeature );
    }

    ///
    ///         Remove all features in the entity's feature list.
    ///
    ///\throw   None
    ///
    void _removeFeatures()
        throw()
    {
        _oFeatures.clear();
    }

    ///
    ///         Add a child entity the existing list of child entities.
    ///
    ///\param   pChild      A pointer to the child entity to be added.
    ///\throw   None
    ///
    _DWFTK_API
    void _addChild( DWFEntity* pChild )
        throw();

    ///
    ///         Remove the specific child if it exists in the list of
    ///         children.
    ///
    ///\param   pChild      A pointer to the child to be removed.
    ///\return  True if the child was found and removed, 
    ///\throw   None
    ///
    _DWFTK_API
    bool _removeChild( DWFEntity* pChild )
        throw();

    ///
    ///         Remove all child entities.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    void _removeChildren()
        throw();

private:

    // An entity can have multiple parents, if multiple design elements need 
    // to use this common one. The parent and child lists are initialized 
    // duplicates not allowed.
    DWFEntity::tSortedList      _oParents;
    // An entity can be composed of multiple child entities.
    DWFEntity::tSortedList      _oChildren;

    // References (pointers) to classes that classify this entity.
    DWFClass::tList             _oClasses;

    // References (pointers) to features that this entity uses.
    DWFFeature::tList           _oFeatures;

private:

    //
    // Not Implemented
    //
    DWFEntity( const DWFEntity& );
    DWFEntity& operator=( const DWFEntity& );

};

}

#endif

