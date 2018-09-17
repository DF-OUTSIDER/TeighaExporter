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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/publisher/impl/PublishedContentElement.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_PUBLISHED_CONTENT_ELEMENT_H
#define _DWFTK_PUBLISHED_CONTENT_ELEMENT_H


///
///\file        dwf/publisher/impl/PublishedContentElement.h
///\brief       This file contains the class declaration for DWFPublishedContentElement
///

#ifndef DWFTK_READ_ONLY

#include "dwf/package/Content.h"
#include "dwf/publisher/PublishedObject.h"

namespace DWFToolkit
{
 
//
// fwd declarations
//
class DWFSectionContentResource;

///
///\ingroup     dwfpublish
///
///\class       DWFPublishedContentElement   dwf/publisher/impl/PublishedContentElement.h    "dwf/publisher/impl/PublishedContentElement.h"
///\brief       This is an implementation of the DWFPublishedObject, to capture relationships between a graphics element
///             and some published structure, in this case the DWFPublishedDefinedObject.
///
///\since       7.2.0
///
class DWFPublishedContentElement : public DWFPublishedObject
                                   _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   Defines the data type used for published element keys.
    ///
    typedef DWFPublishedObject::tKey                                            tKey;
    ///
    ///\brief   This type defines a list of DWFPublishedObject pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFPublishedContentElement*)                      tList;
    ///
    ///\brief   This type defines a mapped collection of DWFPublishedObject pointers.
    ///
    typedef DWFSkipList<DWFPublishedObject::tKey, DWFPublishedContentElement*>  tMap;

public:

    ///
    ///\ingroup     dwfpublish
    ///
    ///\class       Factory   dwf/publisher/impl/PublishedContentElement.h    "dwf/publisher/impl/PublishedContentElement.h"
    ///\brief       This is an implementation of the DWFPublishedObject::Factory, to allow creation
    ///             of the published content elements.
    ///
    ///\since       7.2.0
    ///
    class Factory : public DWFPublishedObject::Factory
                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        Factory()
            throw();

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~Factory()
            throw();

        ///
        ///         Create a new object.
        ///
        ///\param   nKey    A unique key for this object.
        ///\param   zName   The object name.
        ///\return  The new object.
        ///\throw   DWFException
        ///
        _DWFTK_API
        virtual DWFPublishedObject* makePublishedObject( tKey              nKey,
                                                         const DWFString&  zName )
            throw( DWFException );

        ///
        ///         Returns a pre-built object for a given key.
        ///
        ///\param   nKey    A unique key for this object.
        ///\return  The existing object.
        ///\throw   DWFException
        ///
        _DWFTK_API
        virtual DWFPublishedObject& findPublishedObject( tKey nKey )
            throw( DWFException );

        ///
        ///         Get an iterator to the published content elements.
        ///
        ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
        ///\throw   DWFException
        ///
        _DWFTK_API
        DWFPublishedContentElement::tMap::Iterator* getPublishedObjects()
            throw()
        {
            return _oPublishedElements.iterator();
        }

    private:

        DWFPublishedContentElement::tMap    _oPublishedElements;

    private:

        //
        //  Not implemented
        //
        Factory( const Factory& );
        Factory& operator=( const Factory& );

    };

public:

    ///
    ///\ingroup     dwfpublish
    ///
    ///\class       NotificationSink   dwf/publisher/impl/PublishedContentElement.h   "dwf/publisher/impl/PublishedContentElement.h"
    ///\brief       Base implementation and interface definition to get information back during the visitation of published content 
    ///             element using the DWFPublishedContentElement::Visitor.
    ///\since       7.2.0
    ///
    class NotificationSink _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
    {

    public:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        NotificationSink()
            throw()
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~NotificationSink()
            throw()
        {;}

    public:

        ///
        ///         The callback derived classes should implement to get notified on instance 
        ///         creation events.
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual void notifyInstanceCreated( tKey         /*nKey*/,
                                            DWFInstance* /*pInstance*/ )
            throw()
        {;}

        ///
        ///         The callback derived classes should implement to get notified on object 
        ///         creation events.
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual void notifyObjectCreated( tKey       /*nKey*/,
                                          DWFObject* /*pObject*/ )
            throw()
        {;}

        ///
        ///         The callback derived classes should implement to get notified on feature 
        ///         creation events.
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual void notifyFeatureCreated( tKey        /*nKey*/,
                                           DWFFeature* /*pFeature*/ )
            throw()
        {;}

        ///
        ///         The callback derived classes should implement to get notified on entity 
        ///         creation events.
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual void notifyEntityCreated( tKey       /*nKey*/,
                                          DWFEntity* /*pEntity*/ )
            throw()
        {;}

    };

public:

    ///
    ///\ingroup     dwfpublish
    ///
    ///\class       Visitor   dwf/publisher/impl/PublishedContentElement.h   "dwf/publisher/impl/PublishedContentElement.h"
    ///\brief       An implementation of the published object visitor to support content element generation.
    ///\since       7.2.0
    ///
    ///             This is an implementation of the DWFPublisheObject::Visitor base class for creating DWF content
    ///             from the DWFPublishedObjects created by the 3D Publishing API.
    ///
    class Visitor : public DWFPublishedObject::Visitor
                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public: 

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        Visitor()
            throw( DWFException );

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~Visitor()
            throw()
        {;}

        ///
        ///         Set the content library that creates, stores and 
        ///         manages the content elements.
        ///
        ///\param   pContent    Pointer to the content library.
        ///\throw   None
        ///
        _DWFTK_API
        void setContent( DWFContent* pContent )
            throw()
        {
            _pContent = pContent;
        }

        ///
        ///         Get the content library that creates, stores and 
        ///         manages the content elements.
        ///
        ///\return  Pointer to the content library.
        ///\throw   None
        ///
        _DWFTK_API
        DWFContent* getContent() const
            throw()
        {
            return _pContent;
        }

        ///
        ///         This sets the resource the is associated with the instances that the
        ///         visitor generates. The visitor does not own the resource and hence will
        ///         not delete it.
        ///
        ///\param   pResource     The resource.
        ///\throw   None
        ///
        _DWFTK_API
        void setContentResource( DWFSectionContentResource* pResource )
            throw();

        ///
        ///         Get the resource used to generate the instances.
        ///
        ///\return  Pointer to the content resource used to generate instances.
        ///\throw   None
        ///
        _DWFTK_API
        DWFSectionContentResource* getContentResource() const
            throw()
        {
            return _pContentResource;
        }

        ///
        ///\copydoc DWFPublishedObject::Visitor::visitPublishedObject( DWFPublishedObject& rObject )
        ///
        _DWFTK_API
        void visitPublishedObject( DWFPublishedObject& rObject )
            throw( DWFException );

        ///
        ///         To get notifications when an DWFInstance, DWFObject or DWFEntity is created
        ///         during the visitation, use this method to set a notification sink. This will
        ///         not take ownership of the sink - only use it.
        ///
        ///\param   pSink       A DWFPublishedContentElement derived class
        ///\throw   None
        ///
        _DWFTK_API
        void setNotificationSink( DWFPublishedContentElement::NotificationSink* pSink )
            throw();

        ///
        ///         Remove a specific notification sink.
        ///
        ///\param   pSink       The sink to remove.
        ///\throw   None
        ///
        _DWFTK_API
        void removeNotificationSink( DWFPublishedContentElement::NotificationSink* pSink )
            throw();
        
        _DWFTK_API
        DWFSortedVector<DWFInstance*>& instancesExcludedFromModel()
            throw()
        {
            return _oExcludedInstances;
        }

        ///
        ///         Clear out the map and sinks. This should be called before reusing the visitor
        ///         for a new model.
        ///
        ///\throw   None
        ///
        _DWFTK_API
        void reset()
            throw();

    private:

        _DWFTK_API
        DWFObject* _getParentContentObject( DWFPublishedContentElement* pPublishedElement )
            throw( DWFException );

        //
        //  Find an element corresponding to the provided published index.
        //
        _DWFTK_API
        DWFContentElement* _findContentElement( DWFPublishedObject::tIndex index )
            throw();

        //
        //  Given a published content element, which must be a referenced element,
        //  get the corresponding entity. If no entity is found then a new one is
        //  created.
        //
        _DWFTK_API
        DWFEntity* _getReferencedEntity( DWFPublishedObject* pPublishedObject )
            throw( DWFException );

        _DWFTK_API
        void _copyProperties( DWFPropertyContainer* pFrom, DWFPropertyContainer* pTo, bool bTakeContainers = true )
            throw();

        _DWFTK_API
        void _notifyInstanceCreated( tKey nKey, DWFInstance* pInstance )
            throw();

        _DWFTK_API
        void _notifyObjectCreated( tKey nKey, DWFObject* pObject )
            throw();

        _DWFTK_API
        void _notifyEntityCreated( tKey nKey, DWFEntity* pEntity )
            throw();

    private:

        typedef std::map<DWFPublishedObject::tIndex, DWFContentElement*>            _tMapElement;
        typedef _DWFTK_STD_VECTOR(DWFPublishedContentElement::NotificationSink*)    _tNotifySinks;

    private:

        //
        //  The content library into which the metadata will go.
        //
        DWFContent*                         _pContent;
        //
        //  The resource associated with the generated instances.
        //
        DWFSectionContentResource*          _pContentResource;
        //
        //  The resrouce object ID used to generate the instances.
        //
        DWFString                           _zResourceID;

        //
        //  Instances to exclude from the model because of the HideFromDefaultModel flag
        //
        DWFSortedVector<DWFInstance*>       _oExcludedInstances;

        //
        //  Map from the published object index to a content element pointer 
        //
        _tMapElement                        _oIndexToElement;       

        //
        //  The notification sinks are used to notify an observer/listener of
        //  entity/object/instance creation events.
        //
        _tNotifySinks                       _oNotifySinks;
    };

public:

    ///
    ///         Constructor
    ///
    ///\param   nKey        Unique identifier for the object.
    ///\param   zName       Descriptive name for the object.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPublishedContentElement( tKey             nKey,
                                const DWFString& zName )
        throw();

    ///
    ///         Copy Constructor
    ///
    ///\param   rSource     The object to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPublishedContentElement( DWFPublishedContentElement& rSource)
        throw();

    ///
    ///        Assignment Operator
    ///
    ///\param   rSource     The object to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPublishedContentElement& operator=( DWFPublishedContentElement& rSource)
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPublishedContentElement()
        throw();

    ///
    ///         Creates a reference to an existing object. This should be defined in the
    ///         derived classes.
    ///
    ///\param   pObject             The object to reference.
    ///\param   nKey                The unique identifier of the object from which the association is being made.
    ///\param   pzInstanceName      An optional name for the reference relationship. Maybe NULL
    ///\param   bFromNamedSegment   Is this addRereference call arising from a named segment.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addReference( DWFPublishedObject*  pObject,
                               tKey                 nKey,
                               const DWFString*     pzInstanceName,
                               bool                 bFromNamedSegment = false )
        throw( DWFException );

    ///
    ///         Determine whether the object is set to be hidden from the default model presentation.
    ///
    ///\return  True if it is set to be hidden from the default model presentation.
    ///\throw   None
    ///
    _DWFTK_API
    bool hideFromDefaultModel() const
        throw()
    {
        return _bHideFromDefaultModel;
    }

    ///
    ///\copydoc DWFPublishedObject::setHideFromDefaultModel(DWFPublishedObject::tKey)
    ///
    _DWFTK_API
    virtual bool setHideFromDefaultModel( DWFPublishedObject::tKey nKey )
        throw();

    ///
    ///         Set a content entity to associate with the segment. Instead of 
    ///         generating an entity for this segment's content object, the
    ///         generated content object will be a realization of the provided
    ///         entity.
    ///
    ///\param   pEntity             The content entity to associate.
    ///\param   nKey                The unique identifier of the object from which the association is being made.
    ///\param   pzInstanceName      An optional name for the generated content object. Maybe NULL
    ///\throw   DWFException        
    ///
    _DWFTK_API
    virtual void setContentEntity( DWFEntity* pEntity,
                                   tKey nKey, 
                                   const DWFString* pzInstanceName )
        throw( DWFException );

    ///
    ///         Set an content object to associate with the segment. Instead of generating 
    ///         an object for the segment, it will use the provided object, and just create
    ///         an instance for it. The name cannot be set since the object should already
    ///         have a name.
    ///
    ///\param   pObject             The content object to associate.
    ///\param   nKey                The unique identifier of the object from which the association is being made.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setContentObject( DWFObject* pObject, 
                                   tKey nKey )
        throw( DWFException );

    //
    //  TODO: Implement or remove before final release of 7.2.0
    //
    /////
    /////         Get any content element that has been explicitly associated with
    /////         the published object using setContentEntity or setContentObject.
    /////
    /////\return  A pointer to the DWFContentElement if it has been set. 
    /////\throw   None
    /////
    /////\since   7.2.0
    /////
    //_DWFTK_API
    //virtual DWFContentElement* getContentElement()
    //    throw()
    //{
    //    return _pContentRef;
    //}

    _DWFTK_API
    tReference* getNamedSegmentIncludeRef() const
    {
        return _pNamedSegIncludeRef;
    }

private:

    bool            _bHideFromDefaultModel;

    //
    //  If include is applied to a named segment, use this ref for the include
    //
    tReference*     _pNamedSegIncludeRef;

    //
    //  TODO: Implement or remove before final release of 7.2.0
    //
    ////
    ////  This will parallel the _oReference list in the base class. Whenever an item
    ////  is added in the base _oReference, one will be added here, even if it is NULL.
    ////  This will track any pre-existing content library elements that are associated
    ////  with the segment through the setContentEntity or setContentObject calls.
    ////
    //DWFContentElement::tList    _oContentRef;
    //
    ////
    ////  This is NULL if a pre-existing content library element is associated with this
    ////  segment.
    ////
    //DWFContentElement*          _pContentRef;
};

}

#endif
#endif

