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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Object.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_OBJECT_H
#define _DWFTK_OBJECT_H

///
///\file        dwf/package/Object.h
///\brief       This file contains the DWFObject class declaration.
///

#include "dwfcore/STL.h"

#include "dwf/Toolkit.h"
#include "dwf/package/Feature.h"
#include "dwf/package/Renderable.h"

namespace DWFToolkit
{

//
// fwd declaration
//
class DWFEntity;


///
///\ingroup     dwfpackage
///
///\class       DWFObject     dwf/package/Object.h      "dwf/package/Object.h"
///\brief       Objects represent realizations of entities (design elements) in a model.  
///\since       7.2.0
///
///             Objects represent realizations of entities (design elements) in a model. They
///             define the final model that a publisher intends to show.
///
class DWFObject : public DWFRenderable
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
    ///\brief   This type defines a list of DWFObject pointers.
    ///
    typedef DWFOrderedVector<DWFObject*>            tList;
    ///
    ///\brief   This type defines a mapped collection of DWFObject pointers.
    ///
    typedef DWFStringKeySkipList<DWFObject*>        tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFObject pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFObject*>          tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFObject pointers.
    ///
    typedef DWFIterator<DWFObject*>                 tIterator;

    ///
    ///         This enum defines the attributes that this class will not resolve when
    ///         parsing attributes for the reader.
    ///
    typedef enum teUnresolvedAttributes
    {
        //
        ePropertySetReferences  = DWFPropertySet::ePropertySetReferences,

        //
        eEntityReference        = DWFPropertySet::ePropertySetReferences + 1,

        //
        eFeatureReferences      = DWFPropertySet::ePropertySetReferences + 2

    } teUnresolvedAttributes;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFObject()
        throw();

    ///
    ///         Get the entity that this object realizes.
    ///
    ///\return  A pointer to an entity.
    ///\throw   None
    ///
    DWFEntity* getEntity()
        throw()
    {
        return _pEntity;
    }

    ///
    ///         Access the child objects via the iterator.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFObject::tList::Iterator* getChildren()
        throw()
    {
        return (_oChildren.size() > 0 ? _oChildren.iterator() : NULL);
    }

    ///
    ///         Get the number of child objects.
    ///
    ///\return  Number of child objects.
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
    ///\return  A pointer to the parent object.
    ///         This pointer may be NULL if it has no parent.
    ///\throw   None
    ///
    DWFObject* getParent() const
        throw()
    {
        return _pParent;
    }

    ///
    ///         Get an iterator to access all associated with an object.
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
    ///         Get the number of features associated with the object.
    ///
    ///\return  Number of features associated with the object.
    ///\throw   None
    ///
    size_t getFeatureCount() const
        throw()
    {
        return _oFeatures.size();
    }

    ///
    ///         This tests whether or not the given feature is refered to by the object.
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
    ///
    ///         The constructor is for use by the DWFContent during publishing.
    ///         It should not be used to create the element directly.
    ///
    ///\param   zID         The UUID assigned to the object.
    ///\param   pEntity     The entity that this object realizes.
    ///\param   pContent    The content that owns this element.
    ///\throw   A DWFInvalidArgumentException is thrown if the entity pointer is NULL.
    ///
    _DWFTK_API
    DWFObject( const DWFString&     zID, 
               DWFEntity*           pEntity,
               DWFContent*          pContent = NULL )
        throw( DWFInvalidArgumentException );

    ///
    ///         Constructor
    ///
    ///         The constructor is for use by the DWFXMLElementBuilder during reading.
    ///         It should not be used to create the element directly.
    ///
    ///\throw   None.
    ///
    _DWFTK_API
    DWFObject()
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
    ///         Add a child object the existing list of child objects.
    ///
    ///\param   pChild      A pointer to the child object to be added.
    ///\throw   None
    ///
    _DWFTK_API
    void _addChild( DWFObject* pChild )
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
    bool _removeChild( DWFObject* pChild)
        throw();

    ///
    ///         Remove all child objects.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    void _removeChildren()
        throw();

    ///
    ///         Add a feature the object.
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
    ///         Remove a feature if it is present in the object's feature list.
    ///
    ///\param   pFeature     A pointer to an feature.
    ///\return  True if the feature was found and removed, false otherwise.
    ///\throw   None
    ///
    bool _removeFeature( DWFFeature* pFeature )
        throw()
    {
        return _oFeatures.erase( pFeature );
    }

    ///
    ///         Remove all features in the object's feature list.
    ///
    ///\throw   None
    ///
    void _removeFeatures()
        throw()
    {
        _oFeatures.clear();
    }

    ///
    ///         Set the entity that this object realizes. This is should only be used during the reading
    ///         process.
    ///
    void _setEntity( DWFEntity* pEntity )
    {
        _pEntity = pEntity;
    }

private:
    
    // Entity that this object realizes.
    DWFEntity*                  _pEntity;

    // If this object has no parent, this will be NULL.
    DWFObject*                  _pParent;

    // An object can be composed of multiple child entities.
    DWFObject::tList            _oChildren;

    // References (pointers) to features that this object uses.
    DWFFeature::tList           _oFeatures;

private:

    //
    // Not Implemented
    //
    DWFObject( const DWFObject& );
    DWFObject& operator=( const DWFObject& );
};

}

#endif

