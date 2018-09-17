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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/publisher/PublishedObject.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_PUBLISHED_OBJECT_H
#define _DWFTK_PUBLISHED_OBJECT_H


///
///\file        dwf/publisher/PublishedObject.h
///\brief       This file contains the class declarations for DWFPublishedObject and 
///             DWFPublishedObjectFactoryImpl
///


#ifndef DWFTK_READ_ONLY




#include "dwfcore/STL.h"
#include "dwfcore/String.h"
#include "dwfcore/Exception.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/PropertySet.h"


namespace DWFToolkit
{

//
// fwd decl
//
class DWFPropertyVisitor;
class DWFObject;
class DWFEntity;

///
///\ingroup         dwfpublish
///
///\class       DWFPublishedObject   dwf/publisher/PublishedObject.h    "dwf/publisher/PublishedObject.h"
///\brief       This class is used to capture the relationship between a graphics element/node
///             and some publishable structure and metadata.
///\since       7.0.1
///
class DWFPublishedObject : public DWFPropertySet
                           _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   Defines the data type used for object keys.
    ///
    typedef long    tKey;
    ///
    /// \brief   Defines the data type used for object indices.
    ///
    typedef int     tIndex;

    ///
    ///\brief   Data structure used for capturing cross-object references.
    ///
    typedef struct tReference
    {
        tKey                nKey;           // Key of the referring segment.
        tIndex              nIndex;         // Segment index in scenegraph.
        DWFPublishedObject* pObj;           // The published object being referred to.
        DWFString           zName;          // Optional name to override referred objects name.
        bool                bHideFromModel; // Prevent model from showing this.

        tReference() : nKey(-1), nIndex(0), pObj(NULL), zName(), bHideFromModel(false) {}

    } tReference;

    ///
    ///\brief   This type defines a list of tReference pointers.
    ///
    typedef _DWFTK_STD_VECTOR(tReference*)          tReferenceList;
    ///
    ///\brief   This type defines a list of DWFPublishedObject pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFPublishedObject*)  tList;
    ///
    ///\brief   This type defines a mapped collection of DWFPublishedObject pointers.
    ///
    typedef DWFSkipList<tKey, DWFPublishedObject*>  tMap;

public:

    ///
    ///\ingroup         dwfpublish
    ///
    ///\interface   Factory   dwf/publisher/PublishedObject.h    "dwf/publisher/PublishedObject.h"
    ///\since       7.0.1
    ///
    class Factory _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
    {

    public:

        ///
        ///         Copy Constructor
        ///
        ///\throw   None
        ///
        Factory( const Factory& )
            throw()
        {;}

        ///
        ///         Assignment Operator
        ///
        ///\throw   None
        ///
        Factory& operator=( const Factory& )
            throw()
        {
            return *this;
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Factory()
            throw()
        {;}

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
            throw( DWFException ) = 0;

        ///
        ///         Returns a pre-built object for a given key.
        ///
        ///         This is an optional method for those factories that also
        ///         keep their products around.
        ///
        ///\param   nKey    A unique key for this object.
        ///\return  The existing object.
        ///\throw   DWFException
        ///
        _DWFTK_API
        virtual DWFPublishedObject& findPublishedObject( tKey nKey )
            throw( DWFException ) = 0;

    protected:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        Factory()
            throw()
        {;}
    };

public:

    ///
    ///\ingroup         dwfpublish
    ///
    ///\interface   Visitor   dwf/publisher/PublishedObject.h    "dwf/publisher/PublishedObject.h"
    ///\brief       Base implementation and interface definition for objects (visitors) that
    ///             extract structure and metadata from the published objects.
    ///\since       7.0.1
    ///
    class Visitor _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
    {

    public:

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Visitor()
            throw()
        {;}

        ///
        ///         Default visitation implementation.  Specializations should
        ///         invoke this base method to ensure any composed visitors are called.
        ///
        ///\param   rObject     An object from the publishable source.
        ///\throw   DWFException
        virtual void visitPublishedObject( DWFPublishedObject& rObject )
            throw( DWFException )
        {
            if (_pComposite)
            {
                _pComposite->visitPublishedObject( rObject );
            }
        }

    protected:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        Visitor( Visitor* pComposite = NULL )
            throw()
            : _pComposite( pComposite )
        {;}

    private:

        Visitor( const Visitor& );
        Visitor& operator=( const Visitor& );

    private:

        Visitor* _pComposite;
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
    DWFPublishedObject( tKey             nKey,
                        const DWFString& zName )
        throw();

    ///
    ///         Copy Constructor
    ///
    ///\param   rSource     The object to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPublishedObject( DWFPublishedObject& rSource)
        throw();

    ///
    ///        Assignment Operator
    ///
    ///\param   rSource     The object to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPublishedObject& operator=( DWFPublishedObject& rSource)
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPublishedObject()
        throw();

    ///
    ///         Returns the object's name
    ///
    ///         Objects may exist in a hierachy and the full path
    ///         of one branch may be expressed as a concatenation
    ///         of object names.  Names are not necessarily unique
    ///         among all objects in the same set.
    ///
    ///\return  The descriptive name.
    ///\throw   None
    ///
    const DWFString& name() const
        throw()
    {
        return getLabel() ;
    }

    ///
    ///         Returns the full path of the object 
    ///         expressed as a concatenation of hierachical
    ///         object names (including its own) and some
    ///         separation character.
    ///
    ///\param   zPath       A string object to receive the path.
    ///\throw   None
    ///
    _DWFTK_API
    void path( DWFString& zPath )
        throw();

    ///
    ///         Returns the object's unique key.
    ///
    ///         The key is used to bind the object to a data structure
    ///         that itself defines the graphic element or entry point
    ///         to it.  This data structure then provides the index of
    ///         (the association to) the actual graphics.  
    ///
    ///\return  The unique object identifier.
    ///\throw   None
    ///
    tKey key() const
        throw()
    {
        return _nKey;
    }

    //
    // return the object's instance id
    //
    unsigned int instance() const
        throw()
    {
        return _nIID;
    }

    //
    // assign the object's instance id
    //
    void setInstance( unsigned int nIID )
        throw()
    {
        _nIID = nIID;
    }

    //
    // return the scene index
    //
    tIndex index() const
        throw()
    {
        return _nIndex;
    }

    ///
    ///         Assigns an graphic index to the object.
    ///
    ///\param   nIndex  An indentifier that associates this object with the graphic node or element.
    ///\throw   None
    ///
    void setIndex( tIndex nIndex )
        throw()
    {
        _nIndex = nIndex;
    }

    ///
    ///         Returns the object's parent (if applicable.)
    ///
    ///\return  A pointer to the parent.  This pointer must not be deleted by the caller.
    ///\throw   None
    ///
    DWFPublishedObject* parent() const
        throw()
    {
        return _pParent;
    }

    ///
    ///         Assigns another as this object's parent.
    ///
    ///\param   pParentObject   A pointer to another object.
    ///                         This pointer remains the responsibility of the caller to delete.
    ///\throw   None
    ///
    void setParent( DWFPublishedObject* pParentObject )
        throw()
    {
        _pParent = pParentObject;
    }

    ///
    ///         Indicates that this object is a reference object.
    ///
    ///         A reference object is one that indicates reuse
    ///         of graphics in the scene and has additional considerations
    ///         when visited for reporting its metadata [structure].
    ///
    ///         For example, in the 3D model (DWFModel), published objects refer to
    ///         "interesting" segments in the scene graph that the publisher
    ///         wishes to identify as navigable entities and defined object
    ///         instances (DWFDefinedObjectInstance) from which to hang
    ///         object properties (DWFProperty).  The 3D API allows for graphics
    ///         to be defined once and applied many times in the scene resulting
    ///         in smaller files.  These are DWFIncludeSegment classes.
    ///         
    ///\throw   None
    ///
    void referenced()
        throw()
    {
        _bRef = true;
    }

    ///
    ///         Returns a flag indicating whether or not this is a reference object.
    ///
    ///\return  A flag indicating whether or not this is a reference object.
    ///\throw   None
    ///
    bool isReferenced() const
        throw()
    {
        return _bRef;
    }

    ///
    ///         Creates a reference to an existing object.
    ///
    ///\param   pObject             The object to reference.
    ///\param   nKey                The unique identifier of the object from which the
    ///                             association is being made.
    ///\param   pzInstanceName      An optional name for the reference relationship.
    ///\param   bPropertiesOnly     The reference is non-structural; only copy the properties
    ///                             to the referencing (\b this) object.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addReference( DWFPublishedObject*      pObject,
                               DWFPublishedObject::tKey nKey,
                               const DWFString*         pzInstanceName = NULL,
                               bool                     bPropertiesOnly = false )
        throw( DWFException );

    ///
    ///         Returns the list of objects that this object makes reference to.
    ///
    ///\return  A reference to the internal list of object references.
    ///\throw   DWFException
    ///
    ///\todo    Don't return the internal collection.  Use a DWFIterator instead.
    ///
    _DWFTK_API
    tReferenceList& references()
        throw( DWFException );

    ///
    ///         Receives an object visitor and provides itself to its interface.
    ///
    ///\param   rVisitor    The published object visitor.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void accept( DWFPublishedObject::Visitor& rVisitor )
        throw( DWFException );

    ///
    ///         Receives a property visitor and provides itself to its interface.
    ///
    ///\param   rVisitor    The property visitor.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void accept( DWFPropertyVisitor& rVisitor )
        throw( DWFException );

    ///
    ///         Prevent the published object from publishing the object in 
    ///         the default model presentation. If the provided key is the
    ///         same the key of this object, then this object is set to be hidden. 
    ///         If not, the references are searched to match the key value,
    ///         and the flag is set on that reference.
    ///
    ///\param   nKey        Key of the published object to hide from the model.
    ///\return  False if the given key not found.
    ///\throw   None
    ///
    ///\since   7.2.0
    ///
    virtual bool setHideFromDefaultModel( DWFPublishedObject::tKey /*nKey*/ )
        throw()
    {
        return true;
    }

    ///
    ///         Set a content entity to associate with the segment.
    ///         Implement in derived classes that supported content entities
    ///         \sa DWFPublishedContentElement::setContentEntity()
    ///
    ///\param   pEntity         The content entity to associate.
    ///\param   nKey            The unique identifier of the object from which the association is being made.
    ///\param   pzInstanceName  An optional name for the generated content object. Maybe NULL
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    void setContentEntity( DWFEntity*       /*pEntity*/,
                           tKey             /*nKey*/, 
                           const DWFString* /*pzInstanceName*/ )
        throw( DWFException )
    {;}

    ///
    ///         Set an content object to associate with the segment.
    ///         Implement in derived classes that supported content objects
    ///         \sa DWFPublishedContentElement::setContentObject()
    ///
    ///\param   pObject         The content object to associate.
    ///\param   nKey            The unique identifier of the object from which the association is being made.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    virtual void setContentObject( DWFObject*   /*pObject*/,
                                   tKey         /*nKey*/ )
        throw( DWFException )
    {;}

    ///
    ///         Get any content object associated through the published object.
    ///         Implement in derived classes that supported content objects
    ///         \sa DWFPublishedContentElement::getContentObject()
    ///
    ///\return  A pointer to the DWFObject if it has been set. If a entity has been set this 
    ///         will return NULL.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    virtual DWFObject* getContentObject()
        throw()
    {
        return NULL;
    }

protected:

    bool                _bRef;
    
    tKey                _nKey;

    unsigned int        _nIID;
    unsigned int        _nIndex;
    DWFPublishedObject* _pParent;

    tReferenceList      _oReferenceList;

private:

    DWFPublishedObject();
};

}


#endif  
#endif

