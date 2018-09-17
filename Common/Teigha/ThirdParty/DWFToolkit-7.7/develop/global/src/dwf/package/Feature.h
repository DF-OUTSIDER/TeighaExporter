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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Feature.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//

#ifndef _DWFTK_FEATURE_H
#define _DWFTK_FEATURE_H

///
///\file        dwf/package/Feature.h
///\brief       This file contains the DWFFeature class declaration.
///

#include "dwfcore/STL.h"

#include "dwf/Toolkit.h"
#include "dwf/package/Class.h"
#include "dwf/package/Renderable.h"

namespace DWFToolkit
{


///
///\ingroup     dwfpackage
///
///\class       DWFFeature     dwf/package/Feature.h      "dwf/package/Feature.h"
///\brief       Features are used to represent a design aspect of an entity or an object, e.g.
///             a hole or an extrusion.
///\since       7.2.0
///
///             Features are used to represent a design aspect of an entity or an object, e.g.
///             a hole or an extrusion. In and of themselves, they cannot be considered as
///             individual design items and hence cannot be realized or rendered without a 
///             corresponding entity or object.
///
class DWFFeature : public DWFRenderable
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
    ///\brief   This type defines a list of DWFFeature pointers.
    ///
    typedef DWFOrderedVector<DWFFeature*>           tList;
    ///
    ///\brief   This type defines a mapped collection of DWFFeature pointers.
    ///
    typedef DWFStringKeySkipList<DWFFeature*>       tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFFeature pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFFeature*>         tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFFeature pointers.
    ///
    typedef DWFIterator<DWFFeature*>                tIterator;

    ///
    ///         This enum defines the attributes that this class will not resolve when
    ///         parsing attributes for the reader.
    ///
    typedef enum teUnresolvedAttributes
    {
        //
        ePropertySetReferences  = DWFPropertySet::ePropertySetReferences,

        //
        eClassReferences        = DWFPropertySet::ePropertySetReferences + 1

    } teUnresolvedAttributes;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFFeature()
        throw();

    ///
    ///         Get an iterator to access all classes in the feature's class list.
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
    ///         This tests whether or not the given class is refered to by the feature.
    ///
    ///\param   pClass  A pointer to a class.
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
    ///\param   zID         The UUID to be assigned to the feature.
    ///\param   pContent    The content that owns this element.
    ///\throw   None
    ///
    _DWFTK_API
    DWFFeature( const DWFString& zID = /*NOXLATE*/L"",
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
    ///         Add a class to provide \e type information to this feature.
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
    ///         Remove a class if it is present in the feature's class list.
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
    ///         Remove all classes in the feature's class list.
    ///
    ///\throw   None
    ///
    void _removeClasses()
        throw()
    {
        _oClasses.clear();
    }

private:

    // References (pointers) to classes that classify this entity.
    DWFClass::tList    _oClasses;

private:

    //
    // Not Implemented
    //
    DWFFeature( const DWFFeature& );
    DWFFeature& operator=( const DWFFeature& );

};

}

#endif

