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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Content.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_CONTENT_H
#define _DWFTK_CONTENT_H

///
///\file        dwf/package/Content.h
///\brief       This file contains the DWFContent class declaration.
///

#include <set>
#include "dwfcore/STL.h"
#include "dwfcore/UUID.h"
#include "dwfcore/Owner.h"
#include "dwfcore/XMLParser.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/Class.h"
#include "dwf/package/Group.h"
#include "dwf/package/Object.h"
#include "dwf/package/Entity.h"
#include "dwf/package/Feature.h"
#include "dwf/package/Instance.h"
#include "dwf/package/PropertySet.h"
#include "dwf/package/reader/ContentReader.h"
#include "dwf/package/reader/ContentResourceReader.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFContent     dwf/package/Content.h      "dwf/package/Content.h"
///\brief       Content is the container and management class for all object defintion elements 
///\since       7.2.0
///
///             The DWFContent class provides the management API for all object definition
///             elements. The elements should be created and destroyed via the content class.
///
class DWFContent : public DWFContentReader
                 , public DWFContentResourceReader
                 , public DWFOwnable
                 , public DWFXMLBuildable
#ifndef DWFTK_READ_ONLY
                 , public DWFXMLSerializable
#endif
                 _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

    //
    // Certain content operations should only be performed by the content manager
    //
    friend class DWFContentManager;

public:

    ///
    ///\brief   This type defines a list of DWFContent pointers.
    ///
    typedef DWFOrderedVector<DWFContent*>           tList;
    ///
    ///\brief   This type defines a mapped collection of DWFContent pointers.
    ///
    typedef DWFStringKeySkipList<DWFContent*>       tMap;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFContent pointers.
    ///
    typedef DWFIterator<DWFContent*>                tIterator;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFContent pointers.
    ///
    typedef DWFCachingIterator<DWFContent*>         tCachingIterator;

    ///
    ///\brief   Defined data type to collect unresolved attributes during the parsing process.
    ///
    typedef DWFXMLBuildable::tUnresolvedList        tUnresolvedList;

    ///
    ///\brief   This type defines a basic iterator on a collection of DWFString objects.
    ///
    typedef DWFIterator<DWFString>                  DWFStringIterator;

public:

    ///
    ///\brief   ".content.xml"
    ///
    static const char* const kzExtension_ContentXML;

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is used by the parsing process when the 
    ///         DWFPackageReader associated with (and providing read access to)
    ///         the DWF package file is available.
    ///         The subsequent binding makes it possible to read content
    ///         information from the DWF package.
    ///
    ///\param   pPackageReader  Provides access to content in the DWF package.
    ///\param   zID             The ID to assign to the content. This should be
    ///                         empty if it will be read in later.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFContent( DWFPackageReader* pPackageReader, const DWFString& zID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Constructor
    ///
    ///         This constructor is generally used when the content is being 
    ///         created by the manager. This should never be called directly.
    ///
    ///\param   pContentManager The manager of this content.
    ///\param   zID             The ID to assign to the content. If this is 
    ///                         empty, the content manager will provide an ID.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFContent( DWFContentManager* pContentManager, const DWFString& zID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    ~DWFContent()
        throw();

    ///
    ///         Get the UUID of the content
    ///
    ///\return  The UUID of the content
    ///\throw   None
    ///
    const DWFString& id() const
        throw()
    {
        return _zID;
    }

    ///
    ///         Sets the href of the content.
    ///
    ///\param   zHRef   The href of the content
    ///\return  None
    ///\throw   None
    ///
    void setHRef( const DWFString& zHRef)
        throw()
    {
        _zHREF = zHRef;
    }

    ///
    ///         Get the href of the content.
    ///
    ///\return  The href of the content
    ///\throw   None
    ///
    _DWFTK_API
    DWFString href()
        throw();

    ///
    ///         Get the loaded state of the content. If a package is being read in from a file or a stream,
    ///         we may have the content in an unloaded state in which none of it's elements have been read in.
    ///
    ///\return  The "loaded" state of the content - true or false
    ///\throw   None
    ///
    bool isLoaded() const
        throw()
    {
        return _bElementsLoaded;
    }

    ///
    ///         Determine whether a content resource has been loaded into the content. Note
    ///         this will return true the moment instance from that resource has been added
    ///         the content.
    ///
    ///\param   zResourceObjectID   The object ID of the resource.
    ///\return  The "loaded" state of content resource from a section - true or false.
    ///\throw   None
    ///  
    _DWFTK_API
    bool isResourceLoaded( const DWFString& zResourceObjectID )
        throw();

    ///
    ///         Find all classes classified by a given base class.
    ///
    ///\param   pBaseClass  The class being referred to as the base class.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFClass::tIterator* findClassByBaseClass( DWFClass* pBaseClass )
        throw();

    ///
    ///         Find all classes classified by a given id.
    ///
    ///\param   zID  The UUID of the class being referred to as the base class.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFClass::tIterator* findClassByBaseClass( const DWFString& zID )
        throw();

    ///
    ///         Find all entities classified by a given class.
    ///
    ///\param   pClass      The class being referred by the entities.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFEntity::tIterator* findEntitiesByClass( DWFClass* pClass )
        throw();

    ///
    ///         Find all entities classified by a given class.
    ///
    ///\param   zID         The UUID of the class being referred by the entities.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFEntity::tIterator* findEntitiesByClass( const DWFString& zID )
        throw();

    ///
    ///         Find all features classified by a given class.
    ///
    ///\param   pClass      The class being referred by the features.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFFeature::tIterator* findFeaturesByClass( DWFClass* pClass )
        throw();

    ///
    ///         Find all features classified by a given class.
    ///
    ///\param   zID         The UUID of the class being referred by the features.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFFeature::tIterator* findFeaturesByClass( const DWFString& zID )
        throw();

    ///
    ///         Find all objects realizing the given entity.
    ///
    ///\param   pEntity     The entity being realized.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFObject::tIterator* findObjectsByEntity( DWFEntity* pEntity )
        throw();

    ///
    ///         Find all objects realizing the given entity.
    ///
    ///\param   zID         The UUID of the entity being realized.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFObject::tIterator* findObjectsByEntity( const DWFString& zID )
        throw();

    ///
    ///         Find all entities using the given feature.
    ///
    ///\param   pFeature   The feature being used.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFEntity::tIterator* findEntitiesByFeature( DWFFeature* pFeature )
        throw();

    ///
    ///         Find all entities using the given feature.
    ///
    ///\param   zID         The UUID of the feature being used.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFEntity::tIterator* findEntitiesByFeature( const DWFString& zID )
        throw();

    ///
    ///         Find all objects using the given feature.
    ///
    ///\param   pFeature    The feature being used.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFObject::tIterator* findObjectsByFeature( DWFFeature* pFeature )
        throw();

    ///
    ///         Find all objects using the given feature.
    ///
    ///\param   zID         The UUID of the feature being used.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFObject::tIterator* findObjectsByFeature( const DWFString& zID )
        throw();

    ///
    ///         Find all groups containing a given element.
    ///
    ///\param   pElement    The content element being contained by the groups.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFGroup::tIterator* findGroupsByElement( DWFContentElement* pElement )
        throw();

    ///
    ///         Find all groups containing a given element.
    ///
    ///\param   zID         The UUID of the content element contained by the groups.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFGroup::tIterator* findGroupsByElement( const DWFString& zID )
        throw();

    ///
    ///         Find all instances rendering the given renderable, i.e., object or feature.
    ///
    ///\param   pRendered           The renderable (object or feature).
    ///\param   zResourceObjectID   If this is provided, only instances in that resource will be returned.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL. If the item is not find this will be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFInstance::tIterator* findInstancesByRenderable( DWFRenderable* pRendered, 
                                                       const DWFString& zResourceObjectID = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Find all instances rendering the given renderable, i.e., object or feature.
    ///
    ///\param   zRenderedID         The UUID of the renderable (object or feature).
    ///\param   zResourceObjectID   If this is provided, only instances in that resource will be returned.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFInstance::tIterator* findInstancesByRenderable( const DWFString& zRenderedID,
                                                       const DWFString& zResourceObjectID = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Find all instances associated with a resource object ID.
    ///
    ///\param   zResourceObjectID   The resource object ID for which we want all instances.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   DWFException    This is thrown if no such resource ID is found
    ///
    _DWFTK_API
    DWFInstance::tIterator* findInstancesByResourceID( const DWFString& zResourceObjectID ) const
        throw( DWFException );

    ///
    ///         Find all resource object IDs that are associated with instances of the given 
    ///         renderable (object/feature).
    ///
    ///\param   pRenderable     Pointer to the renderable object or feature.
    ///\return  A pointer to an iterator over resource object IDs (DWFStrings) that must be deleted by 
    ///         the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFIterator<DWFString>* findResourceIDsByRenderable( DWFRenderable* pRenderable )
        throw();

    ///
    ///         Find all resource object IDs that are associated with instances of the given 
    ///         renderable (object/feature).
    ///
    ///\param   zID             The UUID of the renderable (object or feature).
    ///\return  A pointer to an iterator over resource object IDs (DWFStrings) that must be deleted by 
    ///         the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    DWFIterator<DWFString>* findResourceIDsByRenderable( const DWFString& zID )
        throw();

    ///
    ///         This should only be used by the reading code to set the manager when the content
    ///         has been created by the reader.
    ///
    ///\param   pContentManager The content manager pointer.
    ///\throw   None
    ///
    void setContentManager( DWFContentManager* pContentManager )
    {
        _pContentManager = pContentManager;
    }

    ///
    ///         Get the content manager managing this content
    ///
    ///\return  A pointer to the content manager
    ///\throw   None
    ///
    DWFContentManager* getContentManager()
        throw()
    {
        return _pContentManager;
    }

    ///
    ///         Get a class by its UUID.
    ///
    ///\param   zID         The UUID of the class.
    ///\return  A pointer to the class. 
    ///         This pointer may be NULL if the class is not found in this content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFClass* getClass( const DWFString& zID )
        throw();

    ///
    ///         Get a feature by its UUID.
    ///
    ///\param   zID         The UUID of the feature.
    ///\return  A pointer to the feature. 
    ///         This pointer may be NULL if the feature is not found in this content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFFeature* getFeature( const DWFString& zID )
        throw();

    ///
    ///         Get an entity by its UUID.
    ///
    ///\param   zID         The UUID of the entity.
    ///\return  A pointer to the class. 
    ///         This pointer may be NULL if the entity is not found in this content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFEntity* getEntity( const DWFString& zID )
        throw();

    ///
    ///         Get an object by its UUID.
    ///
    ///\param   zID         The UUID of the object.
    ///\return  A pointer to the object.
    ///         This pointer may be NULL if the object is not found in this content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFObject* getObject( const DWFString& zID )
        throw();

    ///
    ///         Get a group by its UUID.
    ///
    ///\param   zID         The UUID of the group.
    ///\return  A pointer to the group.
    ///         This pointer may be NULL if the group is not found in this content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFGroup* getGroup( const DWFString& zID )
        throw();

    ///
    ///         Get an element by its UUID. This will be slower than the specific type
    ///         based queries since all types will be queried individually, until the
    ///         element is found or the list of types is exhausted.
    ///
    ///\param   zID         The UUID of the element.
    ///\return  A pointer to the element.
    ///         This pointer may be NULL if the element is not found in this content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentElement* getElement( const DWFString& zID )
        throw();

    ///
    ///         Get an instance by its UUID.
    ///
    ///\param   zID         The UUID of the instance.
    ///\return  A pointer to the instance.
    ///         This pointer may be NULL if the instance is not found in this content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFInstance* getInstance( const DWFString& zID )
        throw();

    ///
    ///         Get a shared property set by its UUID.
    ///
    ///\param   zID         The UUID of the property set.
    ///\return  A pointer to the property set.
    ///         This pointer may be NULL if the set is not found in this content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPropertySet* getSharedPropertySet( const DWFString& zID )
        throw();

    ///
    ///         Get an iterator over all classes in the content, keyed off their IDs.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFClass::tMap::Iterator* getClasses()
        throw()
    {
        return _oClasses.iterator();
    }

    ///
    ///         Get an iterator over all features in the content, keyed off their IDs.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFFeature::tMap::Iterator* getFeatures()
        throw()
    {
        return _oFeatures.iterator();
    }


    ///
    ///         Get an iterator over all entities in the content, keyed off their IDs.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFEntity::tMap::Iterator* getEntities()
        throw()
    {
        return _oEntities.iterator();
    }


    ///
    ///         Get an iterator over all objects in the content, keyed off their IDs.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFObject::tMap::Iterator* getObjects()
        throw()
    {
        return _oObjects.iterator();
    }

    ///
    ///         Get an iterator over all groups in the content, keyed off their IDs.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFGroup::tMap::Iterator* getGroups()
        throw()
    {
        return _oGroups.iterator();
    }

    ///
    ///         Get an iterator over all instances in the content, keyed off their IDs.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFInstance::tMap::Iterator* getInstances()
        throw()
    {
        return _oInstances.iterator();
    }

    ///
    ///         Get an iterator over all shared property sets in the content, keyed off their IDs.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFPropertySet::tMap::Iterator* getSharedPropertySets()
        throw()
    {
        return _oSharedProperties.iterator();
    }

    ///
    ///         Add a DWFClass to the content
    ///
    ///\param   oBaseClass      The list of class to set as its base classes.
    ///\param   zUUID           Optionally, provide the UUID for the new class.
    ///\return  A pointer to the new class.
    ///         This pointer may be NULL if the class could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFClass* addClass( const DWFClass::tList& oBaseClass,
                        const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFClass to the content
    ///
    ///\param   pBaseClass      A base class of the new class.
    ///\param   zUUID           Optionally, provide the UUID for the new class.
    ///\return  A pointer to the new class.
    ///         This pointer may be NULL if the class could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFClass* addClass( DWFClass* pBaseClass = NULL,
                        const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFEntity to the content
    ///
    ///\param   oClass          The list of classes that provide type information to it.
    ///\param   pParentEntity   Pointer to an entity that will be the parent of the new one.
    ///                         This may be NULL.
    ///\param   zUUID           Optionally, provide the UUID for the new entity.
    ///\return  A pointer to the new entity.
    ///         This pointer may be NULL if the entity could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFEntity* addEntity( const DWFClass::tList& oClass,
                          DWFEntity* pParentEntity = NULL,
                          const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFEntity to the content
    ///
    ///\param   pClass          A class that provide type information to it.
    ///\param   pParentEntity   Pointer to an entity that will be the parent of the new one.
    ///                         This may be NULL.
    ///\param   zUUID           Optionally, provide the UUID for the new entity.
    ///\return  A pointer to the new entity.
    ///         This pointer may be NULL if the entity could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFEntity* addEntity( DWFClass* pClass,
                          DWFEntity* pParentEntity = NULL,
                          const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFEntity to the content
    ///
    ///\param   pParentEntity   Pointer to an entity that will be the parent of the new one.
    ///                         This may be NULL.
    ///\param   zUUID           Optionally, provide the UUID for the new entity.
    ///\return  A pointer to the new entity.
    ///         This pointer may be NULL if the entity could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFEntity* addEntity( DWFEntity* pParentEntity = NULL,
                          const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFObject to the content
    ///
    ///\param   pRealizedEntity Pointer to the entity the new object will realize. 
    ///                         This should not be NULL.
    ///\param   pParentObject   Pointer to an object that will be the parent of the new one.
    ///                         This may be NULL.
    ///\param   zUUID           Optionally, provide the UUID for the new object.
    ///\return  A pointer to the new object.
    ///         This pointer may be NULL if the entity could not be created.
    ///\throw   DWFException.
    ///
    _DWFTK_API
    DWFObject* addObject( DWFEntity* pRealizedEntity,
                          DWFObject* pParentObject = NULL,
                          const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFFeature to the content
    ///
    ///\param   oClass          The list of classes that provide type information to it.
    ///\param   zUUID           Optionally, provide the UUID for the new feature.
    ///\return  A pointer to the new feature.
    ///         This pointer may be NULL if the feature could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFFeature* addFeature( const DWFClass::tList& oClass,
                            const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFFeature to the content
    ///
    ///\param   pClass          A class that provide type information to the feature.
    ///\param   zUUID           Optionally, provide the UUID for the new feature.
    ///\return  A pointer to the new feature.
    ///         This pointer may be NULL if the feature could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFFeature* addFeature( DWFClass* pClass = NULL,
                            const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFGroup to the content
    ///
    ///\param   oElements       The list of elements to add to the group.
    ///\param   zUUID           Optionally, provide the UUID for the new group.
    ///\return  A pointer to the new group. This pointer may be NULL if the
    ///         instance could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFGroup* addGroup( const DWFContentElement::tList& oElements,
                        const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFGroup to the content
    ///
    ///\param   zUUID           Optionally, provide the UUID for the new group.
    ///\return  A pointer to the new group. This pointer may be NULL if the
    ///         instance could not be created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFGroup* addGroup( const DWFString& zUUID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Add a DWFPropertySet to the content to hold a property
    ///         collection shared by multiple items.
    ///
    ///\param   zLabel          An optional label for the property set.
    ///\param   zUUID           Optionally, provide the UUID for the new set.
    ///\return  A pointer to the new property set. This pointer may be
    ///         NULL if the container could not be created.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPropertySet* addSharedPropertySet( const DWFString& zLabel = /*NOXLATE*/L"",
                                          const DWFString& zUUID = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Add a DWFInstance to the content
    ///
    ///\param   zResourceID                 The resource that is associated with and will serialize this instance.
    ///\param   pRenderable                 The object or feature that this instance is rendering.
    ///\param   oNodeID                     The graphics node ID associated with the renderable item.
    ///\param   nGraphicsAttribute          The visibility and transparency of the instance given by the flags in DWFInstance::teAttributes.
    ///\param   nGeometricVariationIndex    The index of the geometric variation to show. -1 indicates there are none.
    ///\return  A pointer to the new instance.
    ///         This pointer may be NULL if the instance could not be created.
    ///\throw   A DWFInvalidArgumentException is thrown if the pointer to the renderable item is NULL.
    ///
    _DWFTK_API
    DWFInstance* addInstance( const DWFString& zResourceID, 
                              DWFRenderable* pRenderable,
                              int oNodeID,
                              unsigned int nGraphicsAttribute = DWFInstance::eVisible,
                              int nGeometricVariationIndex = -1 )
        throw( DWFException );

    ///
    ///         Delete a class from the DWFContent. This will also remove references
    ///         to the class in other classes, entities and features.
    ///
    ///\param   pClass      Pointer to the class to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeClass( DWFClass* pClass )
        throw();

    ///
    ///         Delete a class from the DWFContent. This will also remove references
    ///         to the class in other classes, entities and features.
    ///
    ///\param   zID         UUID of the class to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeClass( const DWFString& zID )
        throw();

    ///
    ///         Delete an entity from the DWFContent. This also results in the
    ///         deletion of objects that realize this entity. This in turn
    ///         deletes all descendent objects. For object deleted the 
    ///         instance is also deleted.
    ///
    ///\param   pEntity     Pointer to the entity to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeEntity( DWFEntity* pEntity )
        throw();

    ///
    ///         Delete an entity from the DWFContent. This also results in the
    ///         deletion of objects that realize this entity. This in turn
    ///         deletes all descendent objects. For object deleted the 
    ///         instance is also deleted.
    ///
    ///\param   zID         UUID of the entity to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeEntity( const DWFString& zID )
        throw();

    ///
    ///         Delete an object from the DWFContent. All descendent objects
    ///         are also be deleted. This in turn requires that all associated
    ///         instances be deleted.
    ///
    ///\param   pObject     Pointer to the object to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeObject( DWFObject* pObject )
        throw();

    ///
    ///         Delete an object from the DWFContent. All descendent objects
    ///         are also be deleted. This in turn requires that all associated
    ///         instances be deleted.
    ///
    ///\param   zID         UUID of the object to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeObject( const DWFString& zID )
        throw();

    ///
    ///         Delete a feature from the DWFContent. This in turn requires
    ///         that all associated instances be deleted. This will also remove
    ///         all references to the feature from objects and entities
    ///         refering to it.
    ///
    ///\param   pFeature    Pointer to the feature to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeFeature( DWFFeature* pFeature )
        throw();

    ///
    ///         Delete a feature from the DWFContent. This in turn requires
    ///         that all associated instances be deleted. This will also remove
    ///         all references to the feature from objects and entities
    ///         refering to it.
    ///
    ///\param   zID         UUID of the feature to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeFeature( const DWFString& zID )
        throw();

    ///
    ///         Delete a group from the DWFContent.
    ///
    ///\param   pGroup    Pointer to the group to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeGroup( DWFGroup* pGroup )
        throw();

    ///
    ///         Delete a group from the DWFContent.
    ///
    ///\param   zID         UUID of the group to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeGroup( const DWFString& zID )
        throw();

    ///
    ///         Delete an instance from the DWFContent.
    ///
    ///\param   pInstance   Pointer to the instance being deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeInstance( DWFInstance* pInstance )
        throw();

    ///
    ///         Delete an instance from the DWFContent.
    ///
    ///\param   zID         UUID of the instance to be deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeInstance( const DWFString& zID )
        throw();

    ///
    ///         Delete a shared property set from the DWFContent.
    ///
    ///\param   pPropertySet   Pointer to the set being deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeSharedPropertySet( DWFPropertySet* pPropertySet )
        throw();
    
    ///
    ///         Delete a shared property set from the DWFContent.
    ///
    ///\param   zID   ID of the property set being deleted.
    ///\throw   None
    ///
    _DWFTK_API
    void removeSharedPropertySet( const DWFString& zID )
        throw();

    ///
    ///         Add a baseclass to a class so that it can inherit properties from the baseclass.
    ///
    ///\param   pClass      Pointer to the class which will inherit properties from the other class.
    ///\param   pBaseClass  Pointer to the class to be used as the base class.
    ///\throw   None
    ///
    _DWFTK_API
    void addBaseClassToClass( DWFClass* pClass,
                              DWFClass* pBaseClass )
        throw();

    ///
    ///         Add a class to an entity so that it can inherit properties from the class.
    ///
    ///\param   pEntity     Pointer to the entity which will inherit properties from the class.
    ///\param   pClass      Pointer to the class.
    ///\throw   None
    ///
    _DWFTK_API
    void addClassToEntity( DWFEntity* pEntity, 
                           DWFClass* pClass )
        throw();
    
    ///
    ///         Add a class to a feature so that it can inherit properties from the class.
    ///
    ///\param   pFeature    Pointer to the feature which will inherit properties from the class.
    ///\param   pClass      Pointer to the class.
    ///\throw   None
    ///
    _DWFTK_API
    void addClassToFeature( DWFFeature* pFeature,
                            DWFClass* pClass )
        throw();

    ///
    ///         Add a feature to an entity.
    ///
    ///\param   pEntity     Pointer to the entity.
    ///\param   pFeature    Pointer to the feature.
    ///\throw   None
    ///
    _DWFTK_API
    void addFeatureToEntity( DWFEntity* pEntity,
                             DWFFeature* pFeature )
        throw();

    ///
    ///         Add a feature to an object.
    ///
    ///\param   pObject     Pointer to the object.
    ///\param   pFeature    Pointer to the feature.
    ///\throw   None
    ///
    _DWFTK_API
    void addFeatureToObject( DWFObject* pObject,
                             DWFFeature* pFeature )
        throw();

    ///
    ///         Create a reference from one entity to another. The user has to be
    ///         careful not to introduce cyclic dependencies in the entity-entity
    ///         relationship. Note that this is not a strong relationship in that
    ///         deleting an entity does not delete child entities. This just allows
    ///         the creation of a tree of entities to convey the composition. An
    ///         entity may be the child of multiple entities. The relationship
    ///         implies that the parent entity uses the child entity or subentity in 
    ///         conveying the design concept.
    ///
    ///\param   pParentEntity   Pointer to the higher level entity. 
    ///\param   pChildEntity    Pointer to the sub entity.
    ///\throw   None
    ///
    _DWFTK_API
    void addChildEntity( DWFEntity* pParentEntity,
                         DWFEntity* pChildEntity )
        throw();
    
    ///
    ///         Add a child object to the parent object. Unlike entities, this is
    ///         a strong relationship in that the existence of the child depends
    ///         parent - deleting the parent deletes the child. The corresponding 
    ///         entities that these objects realize may or may not have the 
    ///         corresponding entity-subentity relationship.
    ///
    ///\param   pParentObject   Pointer to the parent object. 
    ///\param   pChildObject    Pointer to the child object.
    ///\throw   None
    ///
    _DWFTK_API
    void addChildObject( DWFObject* pParentObject,
                         DWFObject* pChildObject )
        throw();

    ///
    ///         Add a content element (class, entity, feature, object, group) to a group.
    ///
    ///\param   pGroup      Pointer to the group to add the element to.
    ///\param   pElement    Pointer to the element to add.
    ///\throw   None
    ///
    _DWFTK_API
    void addElementToGroup( DWFGroup* pGroup,
                            DWFContentElement* pElement )
        throw();

    ///
    ///         Add a shared property set to an element. Property queries on the element
    ///         will treat the properties and property sets in this set as properties of 
    ///         the element. The property set must already be in the contents collection
    ///         of shared properties.
    ///
    ///\param   pElement        Pointer to the element to which the container will be added.
    ///\param   pPropertySet    Pointer to the shared property set. 
    ///\throw   DWFException    The exception is thrown if the set is not in the content already.
    ///
    _DWFTK_API
    void addSharedPropertySetToElement( DWFContentElement* pElement,
                                        DWFPropertySet* pPropertySet )
        throw( DWFException );

    ///
    ///         Add a shared property set to a property set or property set derived class in
    ///         the content. The referenced property set should be in the contents collection
    ///         of shared properties.
    ///
    ///\param   pReferingSet    Pointer to the set to which the container will be added.
    ///\param   pPropertySet    Pointer to the shared property set. 
    ///\throw   DWFException    The exception is thrown if the set is not in the content already.
    ///
    _DWFTK_API
    void addReferenceToPropertySet( DWFPropertySet* pReferingSet,
                                    DWFPropertySet* pPropertySet )
        throw( DWFException );

    ///
    ///         Remove reference to a baseclass from a class.
    ///
    ///\param   pClass      Pointer to the class.
    ///\param   pBaseClass  Pointer to the class being refered to as the base class.
    ///\throw   None
    ///
    _DWFTK_API
    void removeBaseClassFromClass( DWFClass* pClass,
                                   DWFClass* pBaseClass )
        throw();
    
    ///
    ///         Remove reference to a class from an entity.
    ///
    ///\param   pEntity     Pointer to the entity.
    ///\param   pClass      Pointer to the class being referenced.
    ///\throw   None
    ///
    _DWFTK_API
    void removeClassFromEntity( DWFEntity* pEntity,
                                DWFClass* pClass )
        throw();
    
    ///
    ///         Remove reference to a class from a feature.
    ///
    ///\param   pFeature    Pointer to the feature.
    ///\param   pClass      Pointer to the class being referenced.
    ///\throw   None
    ///
    _DWFTK_API
    void removeClassFromFeature( DWFFeature* pFeature,
                                 DWFClass* pClass )
        throw();

    ///
    ///         Remove reference to a feature from an entity.
    ///
    ///\param   pEntity     Pointer to the entity.
    ///\param   pFeature    Pointer to the feature being referenced.
    ///\throw   None
    ///
    _DWFTK_API
    void removeFeatureFromEntity( DWFEntity* pEntity,
                                  DWFFeature* pFeature )
        throw();
    
    ///
    ///         Remove reference to a feature from an object.
    ///
    ///\param   pObject     Pointer to the object.
    ///\param   pFeature    Pointer to the feature being referenced.
    ///\throw   None
    ///
    _DWFTK_API
    void removeFeatureFromObject( DWFObject* pObject,
                                  DWFFeature* pFeature )
        throw();

    ///
    ///         Remove reference to a child entity from a entity.
    ///
    ///\param   pParentEntity   Pointer to the parent entity.
    ///\param   pChildEntity    Pointer to the child entity.
    ///\throw   None
    ///
    _DWFTK_API
    void removeChildEntity( DWFEntity* pParentEntity,
                            DWFEntity* pChildEntity )
        throw();
    
    ///
    ///         Remove a child object from an object.
    ///
    ///\param   pParentObject   Pointer to the parent object.
    ///\param   pChildObject    Pointer to the child object.
    ///\throw   None
    ///
    _DWFTK_API
    void removeChildObject( DWFObject* pParentObject,
                            DWFObject* pChildObject )
        throw();

    ///
    ///         Remove a content element from a group.
    ///
    ///\param   pGroup          Pointer to the group.
    ///\param   pElement        Pointer to the content element.
    ///\throw   None
    ///
    _DWFTK_API
    void removeElementFromGroup( DWFGroup* pGroup,
                                 DWFContentElement* pElement )
        throw();

    ///
    ///         Remove a shared property set from a content element.
    ///
    ///\param   pElement        Pointer to the content element.
    ///\param   pPropertySet    Pointer to the property set.
    ///\throw   None
    ///
    _DWFTK_API
    void removeSharedPropertySetFromElement( DWFContentElement* pElement,
                                             DWFPropertySet* pPropertySet )
        throw();

    ///
    ///         Get the UUID generator. This is used to generate IDs for various
    ///         items that require ID, such that there is some correlation in the
    ///         ID values - thus aiding compression.
    ///
    ///\return  Pointer to the UUID generator.
    ///\throw   DWDFException
    ///
    _DWFTK_API
    DWFUUID* getIDProvider() const
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

	///
    ///\copydoc DWFXMLSerializable::addNamespace()
    ///
    _DWFTK_API
    const DWFXMLNamespace& addNamespace( const DWFString& zNamespace, 
                                         const DWFString& zXMLNS )
        throw( DWFException );

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

    ///
    ///\internal    This allows the section content resource to let the content
    ///             know that its section is about to be serialized and that the
    ///             section should serialize the section's instances.
    ///
    ///\param       zResourceID     The ID of the resource, whose instance will be serialized.
    ///\throw       None
    ///
    void setResourceForSerialization( const DWFString& zResourceID )
        throw()
    {
        _zSerializingResource = zResourceID;
    }

#endif

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

public:

    ///
    ///         Access the content reader base of the content.
    ///
    ///\return  A content reader pointer
    ///\throw   None
    ///  
    DWFContentReader* getContentReader()
    {
        return this;
    }

    ///
    ///         Access the content resource reader base of the content.
    ///
    ///\return  A content resource reader pointer
    ///\throw   None
    ///  
    DWFContentResourceReader* getContentResourceReader()
    {
        return this;
    }

    ///
    ///         This parses the content document using the default content reader
    ///         implemented by this content class, and loads the resulting objects
    ///         into the content. If a filter is provided, it will be set on the
    ///         default reader.
    ///         If the content has already been loaded, this will do nothing until
    ///         the content has been unload()-ed.
    ///
    ///\param   pReaderFilter   If provided, this will be set on the default reader.
    ///\param   nProviderFlags  DWFContentReader provider flags
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void load( DWFContentReader* pReaderFilter = NULL,
                       unsigned int nProviderFlags = DWFContentReader::eProvideAll )
        throw( DWFException );

    ///
    ///         This parses the content document using the provided content reader.
    ///         This allows the user to directly participate in the parsing process.
    ///         No in-memory representation of the content will be created or cached
    ///         by the DWFContent.
    ///
    ///\param   rCustomReader   The custom content reader.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void load( DWFContentReader& rCustomReader )
        throw( DWFException );

    ///
    ///         This completely unloads the content, included DWFInstances that
    ///         may have been loaded via the section getContent calls.
    ///         If \a bSkipIfAnyResourceModified is true, and the list of 
    ///         instances associated with that any of the section content resources
    ///         has been modified, the unload will not happen and the method will 
    ///         return false.
    ///\param   bSkipIfAnyResourceModified  Prevent unload of the resource
    ///                                     if this is true and the instance list
    ///                                     has changed, unload anyway if false.
    ///\return  True if the unload was performed.
    ///
    ///\throw   DWFExcepion
    ///
    _DWFTK_API
    virtual bool unload( bool bSkipIfAnyResourceModified = true )
        throw( DWFException );

    ///
    ///         This unloads the content all instances that were loaded from
    ///         the resource with the provide resource object identifier.
    ///         If \a bSkipIfResourceModified is true, and the list of 
    ///         instances associated with that resource ID has been
    ///         modified, the unload will not happen and the method will 
    ///         return false.
    ///
    ///\param   zResourceObjectID       The object identifier of the resource 
    ///                                 whose instances will be unloaded.
    ///\param   bSkipIfResourceModified Prevent unload of the resource
    ///                                 if this is true and the instance list
    ///                                 has changed.
    ///\return  True if the resource object ID was found and the resource 
    ///         instances were unloaded.
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool unloadResourceInstances( const DWFString& zResourceObjectID, 
                                          bool bSkipIfResourceModified = true )
        throw();


    ///
    ///         If the content is being read in from a package, the input stream
    ///         can be requested using this call.
    ///
    ///\return  The input stream for the content.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFInputStream* getInputStream()
        throw( DWFException );

    ///
    ///         Merge the content elements and shared property sets from the input
    ///         content into this content.
    ///         If bLocalHasPriority is true, upon an UUID collision, the local content
    ///         will be kept, otherwise the source content properties are copied.
    ///         This should not be called directly. Instead the corresponding method
    ///         on the DWFPackageWriter or the DWFContentManager should be called.
    ///
    _DWFTK_API
    void mergeContent( DWFContent* pContent, bool bLocalHasPriority )
        throw( DWFException );

    ///
    ///         Get the collection of resource object IDs for resources that have
    ///         had their instance list modified.
    ///
    ///\return  A pointed to an iterator over resource object IDs. The caller must
    ///         delete the iterator using \b DWFCORE_FREE_OBJECT.
    ///\throw   None
    ///\since   7.3
    ///
    _DWFTK_API
    DWFStringIterator* modifiedContentResources()
        throw();

    ///
    ///         Determine whether list of instances from a resource were
    ///         modified, using the resource object ID.
    ///
    ///\throw   None
    ///\since   7.3
    ///
    bool isSectionContentResourceModified( const DWFString zResourceObjectID ) const
        throw()
    {
        return (_oModifiedContentResources.count( zResourceObjectID ) == 1);
    }

public:

    ///
    ///\internal    This notification is called by the section content resource
    ///             when it is created.
    ///
    ///\param       zObjectID       The object ID of the resource.
    ///\throw       None
    ///
    _DWFTK_API
    void notifyContentResourceAdded( const DWFString& zObjectID )
        throw();

    ///
    ///\internal    This notification is called by the section content resource
    ///             whenever the ID of a resource associated with content
    ///             instances, is changed.
    ///
    ///\param       zOldObjectID    The old object ID of the resource.
    ///\param       zNewObjectID    The new object ID of the resource.
    ///\throw       None
    ///
    _DWFTK_API
    void notifyResourceIDUpdated( const DWFString& zOldObjectID, const DWFString& zNewObjectID )
        throw();

public:

    ///
    ///\copydoc DWFContentReader::provideVersion()
    ///
    _DWFTK_API
    virtual const char* provideVersion( const char* zVersion )
        throw( DWFException );

    ///
    ///\copydoc DWFContentReader::provideClass()
    ///
    _DWFTK_API
    virtual DWFClass* provideClass( DWFClass* pClass,
                                    const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///\copydoc DWFContentReader::provideFeature()
    ///
    _DWFTK_API
    virtual DWFFeature* provideFeature( DWFFeature* pFeature,
                                        const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///\copydoc DWFContentReader::provideEntity()
    ///
    _DWFTK_API
    virtual DWFEntity* provideEntity( DWFEntity* pEntity,
                                      const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///\copydoc DWFContentReader::provideObject()
    ///
    _DWFTK_API
    virtual DWFObject* provideObject( DWFObject* pObject,
                                      const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///\copydoc DWFContentReader::provideChildObject()
    ///
    _DWFTK_API
    virtual DWFObject* provideChildObject( DWFObject* pObject,
                                           DWFObject* pParent,
                                           const tUnresolvedList& rUnresolved )
       throw( DWFException );

    ///
    ///\copydoc DWFContentReader::provideGroup()
    ///
    _DWFTK_API
    virtual DWFGroup* provideGroup( DWFGroup* pGroup,
                                    const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///\copydoc DWFContentReader::provideSharedProperty()
    ///
    _DWFTK_API
    virtual DWFPropertySet* provideSharedProperty( DWFPropertySet* pPropertySet,
                                                   const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///\copydoc DWFContentReader::providePropertySet()
    ///
    _DWFTK_API
    virtual DWFPropertySet* providePropertySet( DWFPropertySet* pPropertySet,
                                                DWFPropertyContainer* pContainer,
                                                const tUnresolvedList& rUnresolved )
        throw( DWFException );

    ///
    ///\copydoc DWFContentResourceReader::provideInstance()
    ///
    _DWFTK_API
    virtual DWFInstance* provideInstance( DWFInstance* pInstance, 
                                          const DWFString& zRenderableID,
                                          const DWFString& zResourceObjectID )
        throw( DWFException );

    ///
    ///\copydoc DWFContentReader::resolveClasses()
    ///
    _DWFTK_API
    virtual void resolveClasses()
        throw();

    ///
    ///\copydoc DWFContentReader::resolveFeatures()
    ///
    _DWFTK_API
    virtual void resolveFeatures()
        throw();

    ///
    ///\copydoc DWFContentReader::resolveEntities()
    ///
    _DWFTK_API
    virtual void resolveEntities()
        throw();

    ///
    ///\copydoc DWFContentReader::resolveObjects()
    ///
    _DWFTK_API
    virtual void resolveObjects()
        throw();

    ///
    ///\copydoc DWFContentReader::resolveGroups()
    ///
    _DWFTK_API
    virtual void resolveGroups()
        throw();

    ///
    ///\copydoc DWFContentReader::resolvePropertySets()
    ///
    _DWFTK_API
    virtual void resolvePropertySets()
        throw();

    ///
    ///\copydoc DWFContentResourceReader::resolveInstances()
    ///
    _DWFTK_API
    virtual void resolveInstances()
        throw();

private:

    //
    //  Let the content manager reset the id for aggregation purposes
    //  This will not reset the href unless package reader is null.
    //
    _DWFTK_API
    void identify( const DWFString& zID )
        throw();

private:

    //
    // Remove all mapping between given class and other classes
    //
    _DWFTK_API
    void _removeClassToClassMappings( DWFClass* pClass )
        throw();

    //
    // Remove all mappings of the given class to entities
    //
    _DWFTK_API
    void _removeClassToEntityMappings( DWFClass* pClass )
        throw();

    //
    // Remove all mappings of the given class to features
    //
    _DWFTK_API
    void _removeClassToFeatureMappings( DWFClass* pClass )
        throw();

    //
    // Remove all mappings of class to the given entity
    //
    _DWFTK_API
    void _removeClassToEntityMappings( DWFEntity* pEntity )
        throw();

    //
    // Remove all mappings of features to the given entity
    //
    _DWFTK_API
    void _removeFeatureToEntityMappings( DWFEntity* pEntity )
        throw();

    //
    // Remove references to and from pEntity with its parent and child entities
    //
    _DWFTK_API
    void _removeEntityToEntityMappings( DWFEntity* pEntity )
        throw();

    //
    // Remove all mappings of features to the given object
    //
    _DWFTK_API
    void _removeFeatureToObjectMappings( DWFObject* pObject )
        throw();

    //
    // Remove all mappings of groups to the given element
    //
    _DWFTK_API
    void _removeGroupToElementMappings( DWFContentElement* pElement )
        throw();

    //
    // Remove mappings between classes and the feature
    //
    _DWFTK_API
    void _removeClassToFeatureMappings( DWFFeature* pFeature )
        throw();

    //
    // Remove mappings between the feature and entities
    //
    _DWFTK_API
    void _removeFeatureToEntityMappings( DWFFeature* pFeature )
        throw();

    //
    // Remove mappings between the feature and objects
    //
    _DWFTK_API
    void _removeFeatureToObjectMappings( DWFFeature* pFeature )
        throw();

    //
    // The is called to remove an instance. Internally in some places it is
    // called to remove instance but not the mapping, e.g., when deleting objects
    // since the object deletion handles the map cleanup.
    // 
    _DWFTK_API
    void _removeInstance( DWFInstance* pInstance, 
                          bool bRemoveMappings = true )
        throw();

private:

    void _identify( const DWFString& zID )
        throw()
    {
        _zID = zID;
    }

    //
    // Splitting the serialization into smaller chunks
    //
    _DWFTK_API
    void _serializeXMLSharedProperties( DWFXMLSerializer& rSerializer, 
                                        unsigned int nFlags )
        throw( DWFException );

    _DWFTK_API
    void _serializeXMLClasses( DWFXMLSerializer& rSerializer, 
                               unsigned int nFlags )
        throw( DWFException );

    _DWFTK_API
    void _serializeXMLFeatures( DWFXMLSerializer& rSerializer, 
                                unsigned int nFlags )
        throw( DWFException );

    _DWFTK_API
    void _serializeXMLEntities( DWFXMLSerializer& rSerializer, 
                                unsigned int nFlags )
        throw( DWFException );

    _DWFTK_API
    void _serializeXMLObjects( DWFXMLSerializer& rSerializer, 
                               unsigned int nFlags )
        throw( DWFException );

    _DWFTK_API
    void _serializeXMLGroups( DWFXMLSerializer& rSerializer, 
                              unsigned int nFlags )
        throw( DWFException );

    _DWFTK_API
    void _parseContentDocument( DWFInputStream& rDocumentStream,
                                DWFCore::DWFXMLCallback& rDocumentReader )
        throw( DWFException );

private:

    typedef DWFIterator<DWFString>              _tStringIterator;

private:

    _tStringIterator* _tokenizeBySpace( const DWFString& zInput )
        throw();

    void _mergeSharedProperties( DWFContent* pContent, bool bLocalHasPriority )
        throw();

    void _mergeClasses( DWFContent* pContent, bool bLocalHasPriority )
        throw();

    void _mergeFeatures( DWFContent* pContent, bool bLocalHasPriority )
        throw();

    void _mergeEntities( DWFContent* pContent, bool bLocalHasPriority )
        throw();

    void _mergeObjects( DWFContent* pContent, bool bLocalHasPriority )
        throw( DWFException );

    void _mergeGroups( DWFContent* pContent, bool bLocalHasPriority )
        throw();

    void _mergeRefsSharedProperties( DWFContent* pContent )
        throw( DWFException );

    void _mergeRefsClasses( DWFContent* pContent )
        throw( DWFException );

    void _mergeRefsFeatures( DWFContent* pContent )
        throw( DWFException );

    void _mergeRefsEntities( DWFContent* pContent )
        throw( DWFException );

    void _mergeRefsObjects( DWFContent* pContent )
        throw( DWFException );

    void _mergeRefsGroups( DWFContent* pContent )
        throw( DWFException );

    void _acquireOwnedProperties( DWFPropertySet* pFrom, DWFPropertySet* pTo,
                                  bool bLocalHasPriority, bool bTargetExisted )
        throw();

    void _acquirePropertySetReferences( DWFPropertySet* pFrom, DWFPropertySet* pTo )
        throw( DWFException );

    void _acquireChildObjects( DWFObject* pFrom, DWFObject* pTo,
                               bool bLocalHasPriority )
        throw( DWFException );


private:

    //
    // Maps to enable property inheritance heirarchy
    //
    typedef std::multimap<DWFClass*, DWFClass*>                     _tClassToClassMultiMap;
    typedef std::multimap<DWFClass*, DWFEntity*>                    _tClassToEntityMultiMap;
    typedef std::multimap<DWFClass*, DWFFeature*>                   _tClassToFeatureMultiMap;
    typedef std::multimap<DWFEntity*, DWFObject*>                   _tEntityToObjectMultiMap;
    typedef std::multimap<DWFFeature*, DWFEntity*>                  _tFeatureToEntityMultiMap;
    typedef std::multimap<DWFFeature*, DWFObject*>                  _tFeatureToObjectMultiMap;

    //
    // Maps to allow lower objects to determine their who references them
    //
    typedef std::multimap<DWFContentElement*, DWFGroup*>            _tElementToGroupMultiMap;
    typedef std::multimap<DWFPropertySet*, DWFPropertySet*>         _tSetToSetMultiMap;

    //
    // Instances are referenced via a two dimensional map, from resource object ID and renderable object
    //
    typedef std::multimap<DWFRenderable*, DWFInstance*>             _tRenderableToInstanceMultiMap;
    typedef std::map<DWFString, _tRenderableToInstanceMultiMap*>    _tResourceRenderableToInstanceMap;
    typedef _tResourceRenderableToInstanceMap::iterator             _tResourceRIMapIterator;
    typedef _tResourceRenderableToInstanceMap::const_iterator       _tResourceRIMapConstIterator;

    //
    //  These are used for intermediate storage during reading or merging, 
    //  to allow delayed resolution of IDs to pointers.
    //
    typedef std::multimap<DWFClass*, DWFString>                     _tClassToRefs;
    typedef std::multimap<DWFEntity*, DWFString>                    _tEntityToRefs;
    typedef std::multimap<DWFGroup*, DWFString>                     _tGroupToRefs;
    typedef std::multimap<DWFPropertyContainer*, DWFString>         _tContainerToRef;
    typedef std::map<DWFPropertySet*, DWFPropertySet*>              _tSetToSet;

    //
    //  
    //
    typedef std::set<DWFString>                                     _tSetOfIDs;

	//
	// Used for storage of extended namespaces.
	//
#ifndef DWFTK_READ_ONLY
    typedef std::map<DWFString, DWFXMLNamespace>    _tNamespaceMap;
    _tNamespaceMap     _oAddedNamespaces;
#endif

private:

    DWFString                           _zID;
    DWFString                           _zHREF;

    DWFContentManager*                  _pContentManager;
    DWFPackageReader*                   _pPackageReader;

    //
    //  Container classes: mapping UUID to element pointer
    //
    DWFClass::tMap                      _oClasses;
    DWFFeature::tMap                    _oFeatures;
    DWFEntity::tMap                     _oEntities;
    DWFObject::tMap                     _oObjects;
    DWFGroup::tMap                      _oGroups;
    DWFInstance::tMap                   _oInstances;
    //
    //  Container for common properties referenced by the ODM elements. 
    //  These will be explicitly deleted in the content destructor.
    //
    DWFPropertySet::tMap                _oSharedProperties;
    //
    //  Container for property set lookup, for properties not in the shared
    //  property container - after completing read. These are deleted by
    //  the owner elements and sets.
    //
    DWFPropertySet::tMap                _oRefProperties;

    //
    //  Mappings allowing downward traversal in the ODM heirarchy.
    //
    _tClassToClassMultiMap              _oBaseClassToClass;
    _tClassToEntityMultiMap             _oClassToEntity;
    _tClassToFeatureMultiMap            _oClassToFeature;
    _tEntityToObjectMultiMap            _oEntityToObject;
    _tFeatureToEntityMultiMap           _oFeatureToEntity;
    _tFeatureToObjectMultiMap           _oFeatureToObject;
    _tElementToGroupMultiMap            _oElementToGroup;
    _tSetToSetMultiMap                  _oSetToSet;
    //
    //  mapping resource object GUIDs and object pointer to instance pointer
    //
    _tResourceRenderableToInstanceMap   _oResourceRenderableToInstance;

    //
    //  The load state of the content. If a content is created as a result of reading a
    //  package, then the content can be in a state where the content elements have not
    //  been loaded.
    //  If the content is created for publishing, or be other means this will be true.
    //
    bool                                _bElementsLoaded;

    //
    //  Version of the schema for generated XML
    //
    DWFString                           _zVersion;

    //
    //  We need to track the which resources (section content resources) need to go through
    //  the DWFSectionContentResource::serializeXML for saving. This is important especially
    //  in the cases where a the (Section Content) resource, i.e., the instance information
    //  is read in from a file and is then modified.
    //
    _tSetOfIDs                          _oModifiedContentResources;


    //
    //  This is used only when an instance resource is serializing. It's SectionContentResource
    //  lets the content know which set of instances to serialize
    //
    DWFString                           _zSerializingResource;

    //  These are read time storage structures required for delayed resolution string based
    //  IDs to pointers. Each element type's resolve step should empty the map.
    //
    _tClassToRefs                       _oReadClassBaseClassRefs;
    _tEntityToRefs                      _oReadEntityChildEntityRefs;
    _tGroupToRefs                       _oReadGroupElementRefs;
    _tContainerToRef                    _oReadPropertySetRefs;

    //
    //  This is used when another content is merged into this content, to track the mapping
    //  between elements and sets in the source content to created/existing elements or 
    //  sets in this content.
    //
    _tSetToSet                          _oMapFromSetToSet;

private:

    DWFContent( const DWFContent& );
    DWFContent& operator=( const DWFContent& );

};

//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START
_declspec(selectany) const char* const DWFContent::kzExtension_ContentXML   = ".content.xml";
//DNT_END

#endif
#endif

}

#endif

