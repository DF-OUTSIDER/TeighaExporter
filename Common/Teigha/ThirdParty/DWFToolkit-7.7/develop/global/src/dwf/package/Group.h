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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Group.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_GROUP_H
#define _DWFTK_GROUP_H

///
///\file        dwf/package/Group.h
///\brief       This file contains the DWFGroup class declaration.
///

#include "dwfcore/STL.h"

#include "dwf/Toolkit.h"
#include "dwf/package/ContentElement.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFGroup     dwf/package/Group.h      "dwf/package/Group.h"
///\brief       A DWFGroup is used to aggregate multiple elements of a model including groups
///             to provide properties on the aggregate.
///\since       7.2.0
///
///             A DWFGroup is used to aggregate multiple elements of a model including groups
///             to provide properties on the aggregate.
///
class DWFGroup : public DWFContentElement
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
    ///\brief   This type defines a list of DWFGroup pointers.
    ///
    typedef DWFOrderedVector<DWFGroup*>             tList;
    ///
    ///\brief   This type defines a mapped collection of DWFGroup pointers.
    ///
    typedef DWFStringKeySkipList<DWFGroup*>         tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFGroup pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFGroup*>           tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFGroup pointers.
    ///
    typedef DWFIterator<DWFGroup*>                  tIterator;

    ///
    ///         This enum defines the attributes that this class will not resolve when
    ///         parsing attributes for the reader.
    ///
    typedef enum teUnresolvedAttributes
    {
        //
        ePropertySetReferences    = DWFPropertySet::ePropertySetReferences,

        //
        eContentElementReferences = DWFPropertySet::ePropertySetReferences + 1

    } teUnresolvedAttributes;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFGroup()
        throw();

    ///
    ///         Access the group elements via the iterator.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFContentElement::tList::Iterator* getElements()
        throw()
    {
        return (_oContentElements.size() > 0 ? _oContentElements.iterator() : NULL);
    }

    ///
    ///         Get the number of elements in the group.
    ///
    ///\return  Number of elements.
    ///\throw   None
    ///
    size_t getElementCount() const
        throw()
    {
        return _oContentElements.size();
    }

    ///
    ///         This tests whether or not the group contains the element.
    ///
    ///\param   pElement    Pointer to a content element.
    ///\return  True if the group contains the element
    ///\throw   None
    ///
    bool hasElement( DWFContentElement* pElement ) const
        throw()
    {
        size_t iLocation;
        return _oContentElements.findFirst( pElement, iLocation );
    } 

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList(const char**,tUnresolvedList&)
    ///
    virtual void parseAttributeList( const char** ppAttributeList,
                                     tUnresolvedList& rUnresolved )
        throw( DWFException );

protected:

    ///
    ///         Constructor
    ///
    ///         The constructor is for use by the DWFContent and the DWFXMLElementBuilder,
    ///         during publishing and reading respectively. It should not be used to create
    ///         the element directly.
    ///
    ///\param   zID     The UUID to be assigned to the group.
    ///\param   pContent    The content that owns this element.
    ///\throw   None
    ///
    _DWFTK_API
    DWFGroup( const DWFString& zID = /*NOXLATE*/L"",
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

private:

    ///
    ///         Add a content element to the group.
    ///
    ///\param   pClass     A pointer to a content element.
    ///\throw   None
    ///
    void _addElement( DWFContentElement* pElement )
        throw()
    {
        _oContentElements.push_back( pElement );
    }

    ///
    ///         Remove a content element from the group.
    ///
    ///\param   pClass     A pointer to an feature.
    ///\return  True if the element was found and removed, false otherwise.
    ///\throw   None
    ///
    bool _removeElement( DWFContentElement* pElement )
        throw()
    {
        return _oContentElements.erase( pElement );
    }

    ///
    ///         Remove all elements in the group.
    ///
    ///\throw   None
    ///
    void _removeElements()
        throw()
    {
        _oContentElements.clear();
    }

private:

    DWFContentElement::tList        _oContentElements;

private:

    DWFGroup( const DWFGroup& );
    DWFGroup& operator=( const DWFGroup& );

};

}

#endif
