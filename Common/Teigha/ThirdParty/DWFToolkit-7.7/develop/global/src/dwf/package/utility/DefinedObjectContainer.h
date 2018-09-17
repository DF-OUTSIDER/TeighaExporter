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


#ifndef _DWFTK_DEFINED_OBJECT_CONTAINER_H
#define _DWFTK_DEFINED_OBJECT_CONTAINER_H

///
///\file        dwf/package/utility/DefinedObjectContainer.h
///\brief       This file contains the DWFDefinedObjectContainer class declaration.
///


#include "dwf/Toolkit.h"
#include "dwf/package/DefinedObject.h"
#include "dwf/package/utility/PropertyContainer.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFDefinedObjectContainer   dwf/package/utility/DefinedObjectContainer.h     "dwf/package/utility/DefinedObjectContainer.h"
///\brief       An auto-indexing collection utility for DWFDefinedObject objects.
///\since       7.0.1
///
class DWFDefinedObjectContainer     _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFDefinedObjectContainer()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFDefinedObjectContainer()
        throw();

    ///
    ///         Inserts an object into the container.
    ///
    ///\param   pObject     The object to add. 
    ///                     This  pointer is now owned by the container
    ///                     and will be released with the \b DWFCORE_FREE_OBJECT macro.
    ///\param   nGroup      An optional parameter that preserves order relative
    ///                     to the other children.  This value does not have to be unique,
    ///                     in fact, it is a group not a key.  Groups are sorted in ascending
    ///                     order, so children in group 0 will be ordered before
    ///                     children in group 1 (or any other group for that matter.)
    ///                     There is no rule defined for child ordering within a group.
    ///\throw   None
    ///
    _DWFTK_API
    void addObject( DWFDefinedObject* pObject, uint32_t nKey = 0 )
        throw();

    ///
    ///         Removes an object from the container.
    ///
    ///\param   pObject     The object to remove. 
    ///                     This  pointer is now owned by the caller.
    ///\throw   None
    ///\since   7.2.0
    ///
    _DWFTK_API
    void removeObject( DWFDefinedObject* pObject )
        throw();

    ///
    ///         Locates an object by its unique ID.
    ///
    ///\param   zID     Identifies the object.
    ///\return  A pointer to the object.  The caller must not delete this pointer.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFDefinedObject* findObject( const DWFString& zID )
        throw( DWFException );

    ///
    ///         Indicates whether or not any object has been added to the container.
    ///
    ///\return  \e true if there is at least one object in the container, \e false otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    bool empty() const
        throw();

    ///
    ///         Returns an interface with which properties can be associated
    ///         with the container itself.
    ///
    ///\return  A reference to a property container interface.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPropertyContainer& getObjectProperties()
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///         Returns an interface with which the container can be
    ///         serialized into an XML document.
    ///
    ///\return  A reference to a serialization inteface.
    ///\throw   None
    ///
    DWFXMLSerializable& getSerializableObjectContainer() const
        throw()
    {
        return (DWFXMLSerializable&)_oSerializer;
    }

#endif

#ifndef DWFTK_READ_ONLY

private:

    //
    // in order to avoid bizarre diamond patterns and
    // excessively overload base implementations,
    // all container classes must define and implementation this
    // internal [PRIVATE] class utility for serializing themselves into XML.
    //
    class _Serializer : public DWFXMLSerializable
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

        void is( DWFDefinedObjectContainer* pContainer )
        {
            _pContainer = pContainer;
        }

        //
        //
        //
        virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

    private:

        DWFDefinedObjectContainer* _pContainer;
    };

private:

    _Serializer                 _oSerializer;

#endif


protected:
#if defined(__BCPLUSPLUS__)
    public:
#endif

    ///
    ///\brief   The contained objects keyed by object ID
    ///
    DWFDefinedObject::tMap  _oKeyedObjects;
    ///
    ///\brief   The internal property container exposed through \a getObjectProperties().
    ///
    DWFPropertyContainer    _oObjectProperties;

    ///
    ///\brief   This type is used to describe the object groups.
    ///
    typedef std::multimap<uint32_t, DWFDefinedObject*>   _tGroupedObjectMap;
    ///
    ///\brief   The object groups.
    ///
    _tGroupedObjectMap      _oGroupedObjects;

private:

    DWFDefinedObjectContainer( const DWFDefinedObjectContainer& );
    DWFDefinedObjectContainer& operator=( const DWFDefinedObjectContainer& );
};


///
///\ingroup     dwfpackage
///
///\class       DWFDefinedObjectInstanceContainer   dwf/package/utility/DefinedObjectContainer.h     "dwf/package/utility/DefinedObjectContainer.h"
///\brief       An auto-indexing collection utility for DWFDefinedObjectInstance objects.
///\since       7.0.1
///

class DWFDefinedObjectInstanceContainer _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFDefinedObjectInstanceContainer()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFDefinedObjectInstanceContainer()
        throw();

    ///
    ///         Inserts an instance into the container.
    ///
    ///\param   pInstance   The instance to add. 
    ///                     This  pointer is now owned by the container
    ///                     and will be released with the \b DWFCORE_FREE_OBJECT macro.
    ///\param   nGroup      An optional parameter that preserves order relative
    ///                     to the other children.  This value does not have to be unique,
    ///                     in fact, it is a group not a key.  Groups are sorted in ascending
    ///                     order, so children in group 0 will be ordered before
    ///                     children in group 1 (or any other group for that matter.)
    ///                     There is no rule defined for child ordering within a group.
    ///\throw   None
    ///
    _DWFTK_API
    void addInstance( DWFDefinedObjectInstance* pInstance, uint32_t nKey = 0 )
        throw();

    ///
    ///         Removes an instance from the container.
    ///
    ///\param   pInstance   The instance to remove. 
    ///                     This  pointer is now owned by the caller.
    ///\throw   None
    ///\since   7.2.0
    ///
    _DWFTK_API
    void removeInstance( DWFDefinedObjectInstance* pInstance )
        throw();

    ///
    ///         Indicates whether or not any instances have been added to the container.
    ///
    ///\return  \e true if there is at least one instance in the container, \e false otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    bool empty() const
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///         Returns an interface with which the container can be
    ///         serialized into an XML document.
    ///
    ///\return  A reference to a serialization interface.
    ///\throw   None
    ///
    DWFXMLSerializable& getSerializableInstanceContainer() const
        throw()
    {
        return (DWFXMLSerializable&)_oSerializer;
    }

#endif


#ifndef DWFTK_READ_ONLY

private:

    //
    // in order to avoid bizarre diamond patterns and
    // excessively overload base implementations,
    // all container classes must define and implementation this
    // internal [PRIVATE] class utility for serializing themselves into XML.
    //
    class _Serializer : public DWFXMLSerializable
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

        void is( DWFDefinedObjectInstanceContainer* pContainer )
        {
            _pContainer = pContainer;
        }

        //
        //
        //
        virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

    private:

        DWFDefinedObjectInstanceContainer* _pContainer;
    };

private:

    _Serializer _oSerializer;

#endif


protected:
#if defined(__BCPLUSPLUS__)
    public:
#endif

    ///
    ///\brief   This type is used to describe the instance groups.
    ///
    typedef std::multimap<uint32_t, DWFDefinedObjectInstance*>   _tGroupedInstanceMap;
    ///
    ///\brief   The instance groups.
    ///
    _tGroupedInstanceMap    _oGroupedInstances;

private:

    DWFDefinedObjectInstanceContainer( const DWFDefinedObjectInstanceContainer& );
    DWFDefinedObjectInstanceContainer& operator=( const DWFDefinedObjectInstanceContainer& );
};

}

#endif
