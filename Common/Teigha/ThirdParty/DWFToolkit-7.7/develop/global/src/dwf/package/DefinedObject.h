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


#ifndef _DWFTK_DEFINED_OBJECT_H
#define _DWFTK_DEFINED_OBJECT_H

///
///\file        dwf/package/DefinedObject.h
///\brief       This file contains the DWFDefinedObject and DWFDefinedObjectInstance class declarations.
///


#include "dwfcore/STL.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/utility/PropertyContainer.h"

namespace DWFToolkit
{

//
// fwd decl
//
class DWFDefinedObjectInstance;

///
///\class   DWFSTDMap     dwf/package/DefinedObject.h     "dwf/package/DefinedObject.h"
///\brief   This template class extends STL's map class, and defines an inner Iterator class.
///\since   7.3
///
///         This template class extends STL's map class, and defines an inner Iterator class. It
///         is designed to make it possible to easily change between STL-map and SkipList
///         implementations internally, while having minimal impact on client code.
///
// TODO: Find a more global home for this.
//
template <class Key, class Value, typename Comp=tDWFCompareLess<Key> >
class DWFSTDMap : public _DWFTK_STD_MAP_COMP(Key, Value, Comp)
{
public:
    typedef typename _DWFTK_STD_MAP_COMP(Key, Value, Comp)   _tMapType;
    typedef typename _tMapType::iterator                     _tMapIteratorType;

    ///
    ///\class   Iterator     dwf/package/DefinedObject.h     "dwf/package/DefinedObject.h"
    ///\brief   This class defines the iterator for enumerating items stored in this DWFMap.
    ///\since   7.3
    ///
    class Iterator : public DWFIterator<Value>
                     _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\param   iBegin  Points to the first section.
        ///\param   iEnd    Points to the end of the section list.
        ///\throw   None
        ///
        Iterator( _tMapIteratorType iBegin,
                  _tMapIteratorType iEnd )
            throw()
            : _imBegin( iBegin )
            , _imEnd( iEnd )
            , _imCurrent( iBegin )
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Iterator()
            throw()
        {;}

        ///
        ///\copydoc DWFCore::DWFIterator::reset()
        ///
        void reset()
            throw()
        {
		    _imCurrent = _imBegin;
        }

        ///
        ///\copydoc DWFCore::DWFIterator::valid()
        ///
        bool valid()
            throw()
        {
            return (_imCurrent != _imEnd);
        }

        ///
        ///\copydoc DWFCore::DWFIterator::next()
        ///
        bool next()
            throw()
        {
            if (valid())
            {
			    _imCurrent++;

                return valid();
            }
            else
            {
                return false;
            }
        }

        ///
        ///\copydoc DWFCore::DWFIterator::get()
        ///
        Value& get()
            throw( DWFException )
        {
            if (valid())
            {
                return _imCurrent->second;
            }
            else
            {
                _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
            }
        }

        //
        // Provided for backwards compatibility with the old skip-list based Iterator.
        //
        Value& value()
            throw( DWFException )
        {
            return get();
        }

    private:

        _tMapIteratorType _imBegin;
        _tMapIteratorType _imEnd;
        _tMapIteratorType _imCurrent;
    };

public:

    Iterator* getIterator()
    {
        return DWFCORE_ALLOC_OBJECT( Iterator( this->begin(), this->end() ) );
    }
};


///
///\ingroup     dwfpackage
///
///\class       DWFDefinedObject   dwf/package/DefinedObject.h     "dwf/package/DefinedObject.h"
///\brief       This class is used to represent an object in the DWF package.
///\since       7.0.1
///
///             A DWFDefinedObject corresponds to an \b Object element in an object definition document.
///             A defined object is a documented representation of a design component.  These components
///             are completely general and may be characterized as necessary by a publishing application.
///
///             A defined object has no corresponding representation in a DWF graphics stream.
///             That role is taken by the DWFDefinedObjectInstance which is, by definition, the association
///             of a specialized, attributed DWFDefinedObject with a graphical element or node.
///             The role of the DWFDefinedObject is to declare the abstract existence of a design component,
///             outline its relationship among other components and detail its fundamental property interface.
///             Associated instances bind these generalizations with visual data and assign and extend
///             their data definitions.
///
///             These objects exist in a traditional parent-children hierarchical structure with each other.
///             These are structural relationships only; that is, there is no attribute inheritance between
///             a parent and a child as a result of this relationship.  
///
class DWFDefinedObject : public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
                       , public DWFXMLSerializable
#endif
                       , public DWFPropertyContainer
                       _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFDefinedObject pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFDefinedObject*)    tList;

    ///
    ///\brief   This type defines a mapped collection of DWFDefinedObject pointers.
    ///
    ///\note    Prior to version 7.3, this had been of type DWFWCharKeySkipList<DWFDefinedObject*>.
    ///         It was changed to be an STL map for efficiency in large data sets.
    /// 
	typedef DWFSTDMap<const wchar_t*, DWFDefinedObject*, tDWFWCharCompareLess> tMap;

    ///
    ///\brief   This type defines a DWFStringVectorIterator with the proper STL allocator.
    ///
    typedef DWFStringVectorIterator< _DWFTK_STL_ALLOCATOR< DWFString > >     _tStringVectorIterator;


public:

    ///
    ///         Constructor
    ///
    ///\param   zID     A unique ID for the object.  
    ///                 Object IDs should be unique within the section in which they exist.
    ///\throw   None
    ///
    _DWFTK_API
    DWFDefinedObject( const DWFString& zID = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFDefinedObject()
        throw();

    ///
    ///         Returns the ID of the object.
    ///
    ///\return  The object ID
    ///\throw   None
    ///
    _DWFTK_API
    const DWFString& id() const
        throw();

    ///
    ///         Assigns an ID to the object.
    ///
    ///         Object IDs should be unique within the section in which they exist.
    ///\param   zID     The object ID.
    ///\throw   None
    ///
    _DWFTK_API
    void identify( const DWFString& zID )
        throw();

    ///
    ///         Creates a new instance of this object "at" a given node in a graphics stream.
    ///
    ///         Each new instance created by this method is tracked and can be recovered
    ///         by calling \a getInstance().  
    ///
    ///\param   zNode   The graphics node identifier.
    ///\return  A pointer to the new instance.
    ///         The caller must release this pointer with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFDefinedObjectInstance* instance( const DWFString& zNode )
        throw( DWFException );

    ///
    ///         Looks up the instance of this object "at" a given node.
    ///
    ///         The instance must have been previously created with \a instance().
    ///
    ///\param   zNode   The graphic node identifier.
    ///\return  A pointer to the instance.  The caller must not delete this pointer
    ///         as it is owned by another object.
    ///\throw   DWFException
    ///
    ///\warning This method will return an invalid pointer if the object was
    ///         deleted beforehand.  As a general rule, this method is useful
    ///         only during publish-time data creation when it is necessary to 
    ///         look an instance back up after visiting it previously.
    ///
    ///\todo    Provide better ownership tracking.
    ///
    _DWFTK_API
    DWFDefinedObjectInstance* getInstance( const DWFString& zNode )
        throw( DWFException );

    ///
    ///         Removes the given instance from our instance tracking collection.
    ///
    ///         The instance must have been previously created with \a instance().
    ///
    ///         Ownership of the instance passes to the caller, who must release this object with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   zNode   The graphic node identifier.
    ///\throw   DWFException
    ///\since   7.2.0
    ///
    ///\todo    Provide better ownership tracking.
    ///
    _DWFTK_API
    void removeInstance( const DWFString& zNode )
        throw( DWFException );

    ///
    ///         Returns a list of property set reference identifiers.
    ///
    ///         These references are assembled during parsing of an object definition document.
    ///         They are temporary links that are used by the DWFObjectDefinition to build
    ///         the complete list of instance properties.
    ///
    ///\return  A pointer to an iterator that returns property set reference IDs.
    ///         This pointer must be deleted with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    ///\deprecated  This legacy API should not be used by new application code.
    ///             It has been left in until the property reference logic is
    ///             merged into the DWFPropertyContainer construct.
    ///
    ///\todo        Remove string-referenced property set references in favor of DWFPropertyContainer implementation.
    ///
    _DWFTK_API
    DWFIterator<DWFString>* getPropertyRefs()
        throw();

    ///
    ///         Adds a property set reference to the object.
    ///
    ///         These references are used by the DWFObjectDefinitionReader to
    ///         quickly process property relationships and defer the resolution
    ///         until a later time.
    ///
    ///\param   zReferenceID    The property set reference ID.
    ///\throw   DWFException
    ///
    ///\deprecated  This legacy API should not be used by new application code.
    ///             It has been left in until the parsing logic is updated
    ///             to make DWFPropertyContainer associations without
    ///             sacrificing performance.
    ///
    _DWFTK_API
    void addPropertyReference( const char* zReferenceID )
        throw( DWFException );

    ///
    ///         Add another object as a child.
    ///
    ///         This is a structural relationship only.  No property information
    ///         is inherited as a result of this association.
    ///
    ///\param   pChild  The child object.  This pointer is now owned by this
    ///                 object and will be deleted with the \b DWFCORE_FREE_OBJECT
    ///                 macro; as such, it must be allocated with the \b DWFCORE_ALLOC_OBJECT
    ///                 macro.
    ///\param   nKey    Key for insertion.

    ///\throw   None
    ///
    _DWFTK_API
    void addChild( DWFDefinedObject* pChild, uint32_t nKey = 0 )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList(const char** ppAttributeList)
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

protected:

    ///
    ///         Uniquely identifies the object in the section.
    ///
    DWFString               _zID;

    typedef _DWFTK_STD_VECTOR(DWFString) _tStringList;
    ///
    ///         A collection of property set references built up
    ///         from the DWFObjectDefinitionReader processor.
    ///
   _tStringList  _oPropertyRefs;

    ///
    ///         A reference collection of the instances created
    ///         from this object's \a instance() method.
    ///
    DWFDefinedObject::tMap  _oInstances;

    ///
    ///         Defines the multi-value mapping type used for grouping child objects.
    ///
    typedef std::multimap<uint32_t, DWFDefinedObject*>   _tGroupedChildMap;

    ///
    ///         The collection of child objects by group.
    ///
    _tGroupedChildMap       _oChildren;

private:

    DWFDefinedObject( const DWFDefinedObject& );
    DWFDefinedObject& operator=( const DWFDefinedObject& );
};

///
///\ingroup     dwfpackage
///
///\class       DWFDefinedObjectInstance   dwf/package/DefinedObject.h     "dwf/package/DefinedObject.h"
///\brief       This class is used to represent the realization of a defined object as a node in a graphics stream.
///\since       7.0.1
///
///             A DWFDefinedObjectInstance corresponds to an \b Instance element in an object definition document.
///             A defined object instance is a documented representation of the association between a general
///             object definition (DWFDefinedObject) and a visual component in the DWF package.
///
///             A defined object instance is very much like a C++ object that has had its member data 
///             populated in a final state.  This object may be an instantiation of a class that implements
///             one or more property accessor interfaces.  These correspond to the property set references
///             made in the document.  
///
///             These objects exist in a traditional parent-children hierarchical structure with each other.
///             These are structural relationships only; that is, there is no attribute inheritance between
///             a parent and a child as a result of this relationship.  
///
class DWFDefinedObjectInstance : public DWFDefinedObject
                                 _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{


public:

    ///
    ///\brief   This type defines a list of DWFDefinedObjectInstance pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFDefinedObjectInstance*)    tList;
    ///
    ///\brief   This type defines a mapped collection of DWFDefinedObjectInstance pointers.
    ///
    ///\note    Prior to version 7.3, this had been of type DWFWCharKeySkipList<DWFDefinedObjectInstance*>.
    ///         It was changed to be an STL map for efficiency in large data sets.
    /// 
	typedef DWFSTDMap<const wchar_t*, DWFDefinedObjectInstance*, tDWFWCharCompareLess> tMap;


public:

    ///
    ///         Constructor
    ///
    ///\param   nSequence   Describes the order in which the instance was encountered during parsing.
    ///\throw   None
    ///
    _DWFTK_API
    DWFDefinedObjectInstance( unsigned long nSequence )
        throw();

    ///
    ///         Constructor
    ///
    ///\param   rObject     The defined object of which this is an instance.
    ///\param   zNode       The graphic node associated with this instance.
    ///\throw   None
    ///
    _DWFTK_API
    DWFDefinedObjectInstance( DWFDefinedObject& rObject,
                              const DWFString&  zNode )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFDefinedObjectInstance()
        throw();

    ///
    ///         The graphic node associated with this instance.
    ///
    ///\return  The node ID.
    ///\throw   None
    ///
    _DWFTK_API
    const DWFString& node() const
        throw();

    ///
    ///         The identifier of the object of which this is an instance.
    ///
    ///\return  The object ID.
    ///\throw   None
    ///
    _DWFTK_API
    const DWFString& object() const
        throw();

    ///
    ///         The instance parsing and creation order.
    ///
    ///\return  The sequence number.
    ///\throw   None
    ///
    _DWFTK_API
    unsigned long sequence() const
        throw();

    ///
    ///         A collection of child instance identifiers.
    ///
    ///\return  A pointer to an iterator returning instance IDs.
    ///         This pointer is owned by the caller and must be deleted
    ///         with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    _DWFTK_API
    DWFIterator<DWFString>* children()
        throw();

    ///
    ///         Add another instance as a child.
    ///
    ///         This is a structural relationship only.  No property information
    ///         is inherited as a result of this association.
    ///
    ///\param   pChild  The child instance.  This pointer is now owner by this
    ///                 object and will be deleted with the \b DWFCORE_FREE_OBJECT
    ///                 macro; as such, it must be allocated with the \b DWFCORE_ALLOC_OBJECT
    ///                 macro.
    ///\param   nKey    Key for insertion.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    void addChild( DWFDefinedObjectInstance* pChild, uint32_t nKey = 0 )
        throw();

    ///
    ///\internal    This method is used by DWFObjectDefinition during the full
    ///             resolution and construction of the instance tree.
    ///         
    ///\param       pObject     The object that was resolved as the root [type]
    ///                         for this instance.
    ///\throw       None
    ///
    _DWFTK_API
    void resolveObject( DWFDefinedObject* pObject )
        throw();

    ///
    ///\internal    This method is used by DWFObjectDefinition during the full
    ///             resolution and construction of the instance tree.
    ///         
    ///\param       pInstance   An instance that was resolved as a child of this instance.
    ///\throw       None
    ///
    _DWFTK_API
    void resolveChild( DWFDefinedObjectInstance* pInstance )
        throw();

    ///
    ///\internal    This method is used by DWFObjectDefinition during the full
    ///             resolution and construction of the instance tree.
    ///         
    ///\param       pInstance   The instance that was resolved as the parent of this instance.
    ///\throw       None
    ///
    _DWFTK_API
    void resolveParent( DWFDefinedObjectInstance* pInstance )
        throw();

    ///
    ///             Returns the object that was resolved by
    ///             DWFObjectDefinition::getRootInstances() as the
    ///             root for this instance.
    ///         
    ///\return      The object resolved as this instance's root [type].
    ///\throw       None
    ///
    _DWFTK_API
    DWFDefinedObject* resolvedObject() const
        throw();

    ///
    ///             Returns the instance that was resolved
    ///             DWFObjectDefinition::getRootInstances() as the
    ///             parent of this instance.
    ///         
    ///\return      The instance resolved as this instance's parent.
    ///\throw       None
    ///
    _DWFTK_API
    DWFDefinedObjectInstance* resolvedParent() const
        throw();

    ///
    ///             Returns the instances that were resolved
    ///             DWFObjectDefinition::getRootInstances() as the
    ///             children of this instance.
    ///         
    ///\return      A pointer to an iterator of child instances.
    ///             This pointer is owned by the caller and must
    ///             be deleted with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw       None
    ///
    _DWFTK_API
    DWFDefinedObjectInstance::tMap::Iterator* resolvedChildren()
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList(const char** ppAttributeList)
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

private:

    unsigned long           _nSequence;

    DWFString               _zNode;
    DWFString               _zObject;
    typedef _DWFTK_STD_VECTOR(DWFString)  _tStringList;
    _tStringList  _oChildIDs;

    typedef std::multimap<uint32_t, DWFDefinedObjectInstance*>   _tGroupedChildMap;
    _tGroupedChildMap       _oChildren;

    DWFDefinedObject*                       _pResolvedObject;
    DWFDefinedObjectInstance*               _pResolvedParent;
    DWFDefinedObjectInstance::tMap          _oResolvedChildren;

private:

    DWFDefinedObjectInstance( const DWFDefinedObjectInstance& );
    DWFDefinedObjectInstance& operator=( const DWFDefinedObjectInstance& );
};



}

#endif
