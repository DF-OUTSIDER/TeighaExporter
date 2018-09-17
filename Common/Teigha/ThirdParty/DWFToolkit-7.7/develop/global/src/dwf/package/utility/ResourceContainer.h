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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/utility/ResourceContainer.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//



#ifndef _DWFTK_RESOURCE_CONTAINER_H
#define _DWFTK_RESOURCE_CONTAINER_H

///
///\file        dwf/package/utility/ResourceContainer.h
///\brief       This file contains the DWFResourceContainer class declaration.
///

#include "dwfcore/Owner.h"
#include "dwfcore/UUID.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Resource.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFResourceContainer   dwf/package/utility/ResourceContainer.h     "dwf/package/utility/ResourceContainer.h"
///\brief       An auto-mapping collection utility for DWFResource objects.
///\since       7.0.1
///
///             This class is a resource owner.  As resources are added to the container,
///             they are associated in internal maps for fast, multi-keyed look-up.
///
class DWFResourceContainer  : protected DWFOwner
                            , public DWFResource::NotificationSink
                            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\class       ResourceIterator   dwf/package/utility/ResourceContainer.h     "dwf/package/utility/ResourceContainer.h"
    ///\brief       An iterator implementation used to enumerate lists of DWFResource objects.
    ///\since       7.0.1
    ///
    class ResourceIterator : public DWFIterator<DWFResource*>
                             _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\param   iBegin  Points to the first resource.
        ///\param   iEnd    Points to the end of the resource list.
        ///\throw   None
        ///
        ResourceIterator( std :: multimap<const wchar_t*, DWFResource*, tDWFWCharCompareLess>::iterator iBegin,
                          std :: multimap<const wchar_t*, DWFResource*, tDWFWCharCompareLess>::iterator iEnd )
            throw()
            : _iBegin( iBegin )
            , _iEnd( iEnd )
            , _iCurrent( iBegin )
        {;}

        ///
        ///         Copy Constructor
        ///
        ///\param   i       The iterator to clone.
        ///\throw   None
        ///
        ResourceIterator( const ResourceIterator& i )
            throw()
            : _iBegin( i._iBegin )
            , _iEnd( i._iEnd )
            , _iCurrent( i._iCurrent )
        {;}

        ///
        ///         Assignment Operator
        ///
        ///\param   i       The iterator to clone.
        ///\throw   None
        ///
        ResourceIterator& operator=( const ResourceIterator& i )
            throw()
        {
            _iBegin = i._iBegin;
            _iEnd = i._iEnd;
            _iCurrent = i._iCurrent;

            return *this;
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ResourceIterator()
            throw()
        {;}

        ///
        ///\copydoc DWFCore::DWFIterator::reset()
        ///
        void reset()
            throw()
        {
            _iCurrent = _iBegin;
        }

        ///
        ///\copydoc DWFCore::DWFIterator::valid()
        ///
        bool valid()
            throw()
        {
            return (_iCurrent != _iEnd);
        }

        ///
        ///\copydoc DWFCore::DWFIterator::next()
        ///
        bool next()
            throw()
        {
            if (valid())
            {
                _iCurrent++;
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
        DWFResource*& get()
            throw( DWFException )
        {
            if (valid())
            {
                return _iCurrent->second;
            }
            else
            {
                _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
            }
        }

    private:

        DWFResource::tMultiMap::iterator _iBegin;
        DWFResource::tMultiMap::iterator _iEnd;
        DWFResource::tMultiMap::iterator _iCurrent;
    };

    ///
    ///\class       ResourceKVIterator   dwf/package/utility/ResourceContainer.h     "dwf/package/utility/ResourceContainer.h"
    ///\brief       A key-value iterator implementation used to enumerate keyed sets of DWFResource objects.
    ///\since       7.0.1
    ///
    class ResourceKVIterator : public DWFKVIterator<const wchar_t*, DWFResource*>
                               _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\param   iBegin  Points to the first resource.
        ///\param   iEnd    Points to the end of the resource list.
        ///\throw   None
        ///
        ResourceKVIterator( DWFResource::tMultiMap::iterator iBegin,
                            DWFResource::tMultiMap::iterator iEnd )
            throw()
            : _iBegin( iBegin )
            , _iEnd( iEnd )
            , _iCurrent( iBegin )
        {;}

        ///
        ///         Copy Constructor
        ///
        ///\param   i       The iterator to clone.
        ///\throw   None
        ///
        ResourceKVIterator( const ResourceKVIterator& i )
            throw()
            : _iBegin( i._iBegin )
            , _iEnd( i._iEnd )
            , _iCurrent( i._iCurrent )
        {;}

        ///
        ///         Assignment Operator
        ///
        ///\param   i       The iterator to clone.
        ///\throw   None
        ///
        ResourceKVIterator& operator=( const ResourceKVIterator& i )
            throw()
        {
            _iBegin = i._iBegin;
            _iEnd = i._iEnd;
            _iCurrent = i._iCurrent;

            return *this;
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ResourceKVIterator()
            throw()
        {;}

        ///
        ///\copydoc DWFCore::DWFKVIterator::reset()
        ///
        void reset()
            throw()
        {
            _iCurrent = _iBegin;
        }

        ///
        ///\copydoc DWFCore::DWFKVIterator::valid()
        ///
        bool valid()
            throw()
        {
            return (_iCurrent != _iEnd);
        }

        ///
        ///\copydoc DWFCore::DWFKVIterator::next()
        ///
        bool next()
            throw()
        {
            if (valid())
            {
                _iCurrent++;
                return valid();
            }
            else
            {
                return false;
            }
        }

        ///
        ///\copydoc DWFCore::DWFKVIterator::key()
        ///
        const wchar_t*& key()
            throw( DWFException )
        {
            if (valid())
            {
                return (const wchar_t*&)(_iCurrent->first);
            }
            else
            {
                _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
            }
        }

        ///
        ///\copydoc DWFCore::DWFKVIterator::value()
        ///
        DWFResource*& value()
            throw( DWFException )
        {
            if (valid())
            {
                return _iCurrent->second;
            }
            else
            {
                _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
            }
        }

    private:

        DWFResource::tMultiMap::iterator _iBegin;
        DWFResource::tMultiMap::iterator _iEnd;
        DWFResource::tMultiMap::iterator _iCurrent;
    };



public:

    ///
    ///         Destructor
    ///
    ///         This method will delete only those resources
    ///         that are owned by this object.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFResourceContainer()
        throw();

    ///
    ///         Returns the number of resources in the container.
    ///
    ///\return  The resource count.
    ///\throw   None
    ///
    _DWFTK_API
    size_t resourceCount() const
        throw();

    ///
    ///         Locates a resource in the container using its unique HREF.
    ///
    ///\param   zHRef       The unique resource HREF.
    ///\return  A pointer to the resource (may be NULL).  
    ///         This object is owned by the container by default and
    ///         must not be deleted by the caller without first explicitly
    ///         claiming ownership.
    ///\throw   None
    ///
    _DWFTK_API
    DWFResource* findResourceByHREF( const DWFString& zHRef )
        throw();

    ///
    ///         Locates the resource in the container with the given object ID.
    ///
    ///\param   zObjectID       The resource object ID to search by.
    ///\return  The resource with the given object ID.
    ///\throw   None
    ///
    _DWFTK_API
    DWFResource* findResourceByObjectID( const DWFString& zObjectID)
        throw();


    ///
    ///         Locates all resources in the container that share a role.
    ///
    ///\param   zRole       The resource role to search by.
    ///\return  A pointer an iterator for enumerating the resources (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Resources returned from the iterator are owned by the container 
    ///         by default and must not be deleted by the caller without first explicitly
    ///         claiming ownership.
    ///\throw   None
    ///
    _DWFTK_API
    ResourceIterator* findResourcesByRole( const DWFString& zRole )
        throw();

    ///
    ///         Locates all resources in the container that share a MIME type.
    ///
    ///\param   zMIME       The MIME type to search by.
    ///\return  A pointer an iterator for enumerating the resources (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Resources returned from the iterator are owned by the container 
    ///         by default and must not be deleted by the caller without first explicitly
    ///         claiming ownership.
    ///\throw   None
    ///
    _DWFTK_API
    ResourceIterator* findResourcesByMIME( const DWFString& zMIME )
        throw();

    ///
    ///         Returns all resources in the container.
    ///
    ///         Use this method to obtain an iterator that will return
    ///         resources ordered by their unique HREFs.  The HREF is
    ///         returned by the ResourceKVIterator::key() method.
    ///
    ///\return  A pointer an iterator for enumerating the resources (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Resources returned from the iterator are owned by the container 
    ///         by default and must not be deleted by the caller without first explicitly
    ///         claiming ownership.
    ///\throw   None
    ///
    _DWFTK_API
    ResourceKVIterator* getResourcesByHREF()
        throw();

    //
    // Returns all resources in a key-value iterator
    // where the key is the ObjectID of the resource
    //
    _DWFTK_API
    ResourceKVIterator* getResourcesByObjectID()
        throw();

    ///
    ///         Returns all resources in the container.
    ///
    ///         Use this method to obtain an iterator that will return
    ///         resources ordered by their roles.  The role is 
    ///         returned by the ResourceKVIterator::key() method.
    ///
    ///\return  A pointer an iterator for enumerating the resources (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Resources returned from the iterator are owned by the container 
    ///         by default and must not be deleted by the caller without first explicitly
    ///         claiming ownership.
    ///\throw   None
    ///
    _DWFTK_API
    ResourceKVIterator* getResourcesByRole()
        throw();

    ///
    ///         Returns all resources in the container.
    ///
    ///         Use this method to obtain an iterator that will return
    ///         resources ordered by their MIME types.  The MIME type is 
    ///         returned by the ResourceKVIterator::key() method.
    ///
    ///\return  A pointer an iterator for enumerating the resources (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Resources returned from the iterator are owned by the container 
    ///         by default and must not be deleted by the caller without first explicitly
    ///         claiming ownership.
    ///\throw   None
    ///
    _DWFTK_API
    ResourceKVIterator* getResourcesByMIME()
        throw();

    ///
    ///         Clears the resource from the container.
    ///
    ///\param   rResource       The resource object to remove from this container.
    ///\param   bDeleteIfOwned  If \e true and this container currently owns the resource,
    ///                         it will be deleted; otherwise, the resource must be 
    ///                         tracked by the caller who must either explicitly delete it
    ///                         using the \b DWFCORE_FREE_OBJECT macro or recognize that
    ///                         another object has claimed ownership of it.
    ///\return  The resource that was removed.  If the resource was deleted, this pointer
    ///         will be NULL.
    ///\throw   DWFException
    ///
    ///\todo    Passing in an object reference when it might be deleted is a little strange.
    ///         Change to take a pointer instead.
    ///
    _DWFTK_API
    DWFResource* removeResource( DWFResource& rResource,
                                 bool         bDeleteIfOwned )
        throw( DWFException );

    ///
    ///         Clears the resource from the container.
    ///
    ///\param   zHRef           A unique identifier of a resource object to remove from this container.
    ///\param   bDeleteIfOwned  If \e true and this container currently owns the resource,
    ///                         it will be deleted; otherwise, the resource must be 
    ///                         tracked by the caller who must either explicitly delete it
    ///                         using the \b DWFCORE_FREE_OBJECT macro or recognize that
    ///                         another object has claimed ownership of it.
    ///\return  The resource that was removed.  If the resource was deleted, this pointer
    ///         will be NULL.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFResource* removeResourceByHREF( const DWFString& zHRef,
                                       bool             bDeleteIfOwned )
        throw( DWFException );

    //
    // Clears the resource from the container
    // The caller now owns the resource and is responsible for deleting it,
    // unless bDeleteIfOwned and this container does own the resource,
    // in which case, the pointer will be freed and NULL is returned
    //
    _DWFTK_API
    DWFResource* removeResourceByObjectID( const DWFString& zObjectID,
                                           bool             bDeleteIfOwned )
        throw( DWFException );

    ///
    ///         Clears all resources that share a role from the container.
    ///
    ///\param   zRole           The role of the resources to remove from this container.
    ///\param   bDeleteIfOwned  If \e true and this container currently owns any of the resources,
    ///                         they will be deleted; otherwise, the resources found in the iterator
    ///                         must be tracked by the caller who must either explicitly delete them
    ///                         using the \b DWFCORE_FREE_OBJECT macro or recognize that
    ///                         another object has claimed ownership of some or all of them.
    ///\return  A pointer to an iterator for enumerating the resources that were removed
    ///         from the container but not deleted.
    ///\throw   DWFException
    ///
    _DWFTK_API
	virtual ResourceIterator* removeResourcesByRole( const DWFString& zRole,
                                             bool             bDeleteIfOwned )
        throw( DWFException );

    ///
    ///         Clears all resources that share a MIME type from the container.
    ///
    ///\param   zMIME           The MIME type of the resources to remove from this container.
    ///\param   bDeleteIfOwned  If \e true and this container currently owns any of the resources,
    ///                         they will be deleted; otherwise, the resources found in the iterator
    ///                         must be tracked by the caller who must either explicitly delete them
    ///                         using the \b DWFCORE_FREE_OBJECT macro or recognize that
    ///                         another object has claimed ownership of some or all of them.
    ///\return  A pointer to an iterator for enumerating the resources that were removed
    ///         from the container but not deleted.
    ///\throw   DWFException
    ///
    _DWFTK_API
    ResourceIterator* removeResourcesByMIME( const DWFString&   zMIME,
                                             bool               bDeleteIfOwned )
        throw( DWFException );

    ///
    ///         Inserts a resource to the container.
    ///
    ///\param   pResource               The resource to add to the container (must not be NULL).
    ///\param   bOwnResource            If \e true, the container will claim ownership of the resource
    ///                                 and delete it using the \b DWFCORE_FREE_OBJECT macro upon
    ///                                 destruction of this container if it is still owned.
    ///\param   bReplace                If \e true, \a pResource will replace an existing resource at the same HREF.
    ///\param   bDeleteReplacedIfOwned  If \a bReplace is \e true and \a pResource has the same HREF as one in the container,
    ///                                 and the container has ownership of the resource, then the previous object will
    ///                                 be deleted using the \b DWFCORE_FREE_OBJECT macro.
    ///\param   pParentResource -       Identifies another resource is associated as a parent in the meta-structure.
    ///
    ///\return  If \a pResource replaced an existing object but the previous was not deleted,
    ///         this method will return a pointer to that object.  The caller is responsible
    ///         for either claiming ownership of the resource or recognizing that it is already
    ///         owned by another object; otherwise, this method returns NULL.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFResource* addResource( DWFResource*       pResource,
                                      bool               bOwnResource,
                                      bool               bReplace = true,
                                      bool               bDeleteReplacedIfOwned = true,
                                      const DWFResource* pParentResource = NULL )
        throw( DWFException );

    ///
    ///\copydoc DWFResource::NotificationSink::onBeforeResourceHRefChanged(DWFResource* pResource)
    ///
    _DWFTK_API
    virtual void onBeforeResourceHRefChanged( DWFResource* pResource )
        throw();

    ///
    ///\copydoc DWFResource::NotificationSink::onAfterResourceHRefChanged(DWFResource* pResource)
    ///
    _DWFTK_API
    virtual void onAfterResourceHRefChanged( DWFResource* pResource )
        throw();


#ifndef DWFTK_READ_ONLY

    ///
    ///         Returns an interface with which the container can be
    ///         serialized into an XML document.
    ///
    ///\return  A reference to a serialization interface.
    ///\throw   None
    ///
    DWFXMLSerializable& getSerializable() const
        throw()
    {
        return (DWFXMLSerializable&)_oSerializer;
    }

#endif

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFResourceContainer()
        throw();

    ///
    ///\internal    Allows the specializing class (sections) to associate a (their)
    ///             name with the container so when the resources are serialized
    ///             their HREF attributes are updated correctly.
    ///
    ///             This is obviously an internal publishing API but may be useful
    ///             for specialized section classes.
    ///
    ///\param       zName   The new name.
    ///\throw       None
    ///
    void rename( const DWFString& zName )
        throw()
    {
        _zName = zName;
    }

    ///
    ///\copydoc DWFCore::DWFOwner::notifyOwnerChanged()
    ///
    _DWFTK_API
    virtual void notifyOwnerChanged( DWFOwnable& rOwnable )
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFOwner::notifyOwnableDeletion()
    ///
    _DWFTK_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

private:

    //
    // in order to avoid bizarre diamond patterns and
    // excessively overload base implementations,
    // all container classes must define and implementation this
    // internal [PRIVATE] class utility for serializing themselves into XML.
    //
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4275)
#endif
    class _DWFTK_API  _Serializer : public DWFXMLSerializable
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        _Serializer()
            throw()
            : DWFXMLSerializable()
            , _pContainer( NULL )
        {;}

        virtual ~_Serializer()
            throw()
        {;}

        void is( DWFResourceContainer* pContainer )
        {
            _pContainer = pContainer;
        }

        //
        //
        //
        virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

    private:

        DWFResourceContainer* _pContainer;
    };
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif


protected:
    _DWFTK_API
    virtual DWFResource* _remove( DWFResource* pResource, bool bDeleteIfOwned )
        throw();

#if defined(__BCPLUSPLUS__)
    public:
#else
protected:
#endif
    ///
    ///\brief       This type defines a multi-value mapped collection of DWFResource pointers.
    ///
    typedef std::multimap<const DWFResource*, DWFResource*> tResourcePointerMultiMap;

    ///
    ///\brief       Used to generate sequential identifiers for serialization.
    ///
    DWFUUID                     _oUUID;
    ///
    ///\brief       The main collection of resources, keyed uniquely by their HREFs.
    ///
    DWFResource::tMap           _oResourcesByHREF;
    DWFResource::tMap           _oResourcesByObjectID;
    DWFResource::tMultiMap      _oResourcesByRole;
    ///
    ///\brief       A reference collection of the resources, grouped by their MIME types.
    ///
    DWFResource::tMultiMap      _oResourcesByMIME;
    ///
    ///\brief       A reference mapping of the resources and their children.
    ///
    tResourcePointerMultiMap    _oResourceHierarchy;

private:
#if defined(__BCPLUSPLUS__)
    public:
#endif

    DWFString                   _zName;

#ifndef DWFTK_READ_ONLY

    _Serializer                 _oSerializer;

#endif


private:

    DWFResourceContainer( const DWFResourceContainer& );
    DWFResourceContainer& operator=( const DWFResourceContainer& );
};

}

#endif
