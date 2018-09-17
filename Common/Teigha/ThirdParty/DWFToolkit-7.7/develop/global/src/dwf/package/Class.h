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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Class.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_CLASS_H
#define _DWFTK_CLASS_H

///
///\file        dwf/package/Class.h
///\brief       This file contains the DWFClass class declaration.
///

#include "dwfcore/STL.h"

#include "dwf/Toolkit.h"
#include "dwf/package/ContentElement.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFClass     dwf/package/Class.h      "dwf/package/Class.h"
///\brief       A DWFClass is used to classify entities and features in the DWF content.
///\since       7.2.0
///
///             The DWFClass is used to provide a type for entities and features. Classes 
///             themselves can be typed by (base) classes. The properties on a class are
///             inherited by the classes, entities and features that they classify.
///
class DWFClass : public DWFContentElement
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
    ///\brief   This type defines a list of DWFClass pointers.
    ///
    typedef DWFOrderedVector<DWFClass*>             tList;
    ///
    ///\brief   This type defines a mapped collection of DWFClass pointers.
    ///
    typedef DWFStringKeySkipList<DWFClass*>         tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFClass pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFClass*>           tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFClass pointers.
    ///
    typedef DWFIterator<DWFClass*>                  tIterator;

    ///
    ///         This enum defines the attributes that this class will not resolve when
    ///         parsing attributes for the reader.
    ///
    typedef enum teUnresolvedAttributes
    {
        //
        ePropertySetReferences  = DWFPropertySet::ePropertySetReferences,

        //
        eBaseClassReferences    = DWFPropertySet::ePropertySetReferences + 1

    } teUnresolvedAttributes;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFClass()
        throw();

    ///
    ///         Access the base classes via the iterator.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFClass::tList::Iterator* getBaseClasses() 
        throw()
    {
        return (_oBaseClasses.size() > 0 ? _oBaseClasses.iterator() : NULL);
    }

    ///
    ///         Get the number of base classes.
    ///
    ///\return  Number of base classes.
    ///\throw   None
    ///
    size_t getBaseClassCount() const
        throw()
    {
        return _oBaseClasses.size();
    }

    ///
    ///         This tests whether or not the given class is a base class.
    ///
    ///\param   pClass  A pointer to a class.
    ///\return  True if the class is in the base class list.
    ///\throw   None
    ///
    bool isBaseClass( DWFClass* pClass ) const
        throw()
    {
        size_t iLocation;
        return _oBaseClasses.findFirst( pClass, iLocation );
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
    ///         Constructor.
    ///         The constructor is for use by the DWFContent and the DWFXMLElementBuilder,
    ///         during publishing and reading respectively. It should not be used to create
    ///         the element directly.
    ///
    ///\param   zID         The UUID assigned to the class
    ///\param   pContent    The content that owns this element.
    ///\throw   None
    ///
    _DWFTK_API
    DWFClass( const DWFString& zID = /*NOXLATE*/L"",
              DWFContent* pContent = NULL )
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFPropertySet::_serializeAttributes()
    ///
    _DWFTK_API
    virtual void _serializeAttributes( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#if defined(__BCPLUSPLUS__)
    public:
#endif
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
    ///         Add a base class to the existing base class list to
    ///         provide \e type information to this class.
    ///
    ///\param   pBaseClass     A pointer to an class.
    ///\throw   None
    ///
    void _addBaseClass( DWFClass* pBaseClass )
        throw()
    {
        _oBaseClasses.push_back( pBaseClass );
    }

    ///
    ///         Remove a base class if it is present in the baseclass list.
    ///
    ///\param   pBaseClass     A pointer to an class.
    ///\return  True if the class was found and removed, false otherwise.
    ///\throw   None
    ///
    bool _removeBaseClass( DWFClass* pBaseClass )
        throw()
    {
        return _oBaseClasses.erase( pBaseClass );
    }

    ///
    ///         Remove all base classes in the baseclass list.
    ///
    ///\throw   None
    ///
    void _removeBaseClasses()
        throw()
    {
        _oBaseClasses.clear();
    }

private:

    DWFClass::tList        _oBaseClasses;

private:

    DWFClass( const DWFClass& );
    DWFClass& operator=( const DWFClass& );
};

}

#endif

