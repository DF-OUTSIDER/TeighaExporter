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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/publisher/impl/PublishedDefinedObject.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_PUBLISHED_DEFINED_OBJECT_H
#define _DWFTK_PUBLISHED_DEFINED_OBJECT_H


///
///\file        dwf/publisher/impl/PublishedDefinedObject.h
///\brief       This file contains the class declaration for PublishedDefinedObject
///

#ifndef DWFTK_READ_ONLY

#include "dwf/publisher/PublishedObject.h"
#include "dwf/package/ObjectDefinitionResource.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpublish
///
///\class       DWFPublishedDefinedObject   dwf/publisher/impl/PublishedDefinedObject.h    "dwf/publisher/impl/PublishedDefinedObject.h"
///\brief       This is an implementation of the DWFPublishedObject, to capture relationships between a graphics element
///             and some published structure, in this case the DWFPublishedDefinedObject.
///
///\since       7.2.0
///
class DWFPublishedDefinedObject : public DWFPublishedObject
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
    typedef _DWFTK_STD_VECTOR(DWFPublishedDefinedObject*)                       tList;
    ///
    ///\brief   This type defines a mapped collection of DWFPublishedObject pointers.
    ///
    typedef DWFSkipList<DWFPublishedObject::tKey, DWFPublishedDefinedObject*>   tMap;

public:

    ///
    ///\ingroup     dwfpublish
    ///
    ///\class       DWFPublishedObjectFactoryImpl   dwf/publisher/PublishedObject.h    "dwf/publisher/PublishedObject.h"
    ///\brief       This is an implementation of the DWFPublishedObject::Factory, to allow creation
    ///             of the published objects. This was created to support the deprecated object definition
    ///             model of the DWFToolkit 7.1.0.
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
        virtual DWFPublishedObject* makePublishedObject( DWFPublishedObject::tKey nKey,
                                                         const DWFString&         zName )
            throw( DWFException );

        ///
        ///         Returns a pre-built object for a given key.
        ///
        ///\param   nKey    A unique key for this object.
        ///\return  The existing object.
        ///\throw   DWFException
        ///
        _DWFTK_API
        virtual DWFPublishedObject& findPublishedObject( DWFPublishedObject::tKey nKey )
            throw( DWFException );

        ///
        ///         Get an iterator to the stored published objects.
        ///
        ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
        ///\throw   DWFException
        ///
        _DWFTK_API
        DWFPublishedDefinedObject::tMap::Iterator* getPublishedObjects()
            throw()
        {
            return _oPublishedObjects.iterator();
        }

    private:

        DWFPublishedDefinedObject::tMap        _oPublishedObjects;

    private:

        //
        //  Not implemented
        //
        Factory( const Factory& );
        Factory& operator=( const Factory& );

    };

public:

    ///
    ///\ingroup         dwfpublish
    ///
    ///\class       DWFPublishedDefinedObjectVisitor   dwf/publisher/impl/PublishedDefinedObject.h   "dwf/publisher/impl/PublishedDefinedObject.h"
    ///\brief       An implementation of the published object visitor to support content element generation.
    ///\since       7.2.0
    ///
    ///             This is an implementation of the DWFPublisheObject::Visitor base class for creating 
    ///             DWF defined objects and instances for the toolkit version 7.1, from the DWFPublishedObjects
    ///             created by the 3D Publishing API.
    ///
    class Visitor : public DWFPublishedObject::Visitor
                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///\brief   The name of the DWFProperty that identifies an object.
        ///
        static const wchar_t* const kz_PropName_Published_Object;
        ///
        ///\brief   The DWFProperty category that marks it as hidden.
        ///
        static const wchar_t* const kz_PropCategory_Hidden;

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
        ///         Set the object definition resource for the visitor to work on.
        ///
        ///\throw   None
        ///
        _DWFTK_API
        void setObjectDefinitionResource( DWFObjectDefinitionResource* pObjectDefinition )
        {
            _pObjectDefinition = pObjectDefinition;
        }

        ///
        ///         Get the object definition resource that the visitor is referencing.
        ///
        ///\return  Pointer to the object definition resource used by the visitor.
        ///\throw   None
        ///
        _DWFTK_API
        DWFObjectDefinitionResource* getObjectDefinitionResource() const
        {
            return _pObjectDefinition;
        }

		///
        ///         Set the object definition resource for the visitor to collect instances 
		///         of referenced defined objects.
        ///
        ///\throw   None
        ///
		_DWFTK_API
			void setReferencedObjectDefinitionResource( DWFObjectDefinitionResource* pReferencedObjectDefinition )
		{
			_pReferencedObjectDefinition = pReferencedObjectDefinition;
		}

		///
		///         Get the object definition resource collecting referenced instances but won't
		///         wrote to ObjectDefinition file.
		///
		///\return  Pointer to the object definition resource used by the visitor.
		///\throw   None
		///
		_DWFTK_API
			DWFObjectDefinitionResource* getReferencedObjectDefinitionResource() const
		{
			return _pReferencedObjectDefinition;
		}

        ///
        ///\copydoc DWFPublishedObject::Visitor::visitPublishedObject
        ///
        _DWFTK_API
        void visitPublishedObject( DWFPublishedObject& rObject )
            throw( DWFException );

    private:

        _DWFTK_API
        void _definePublishedObject( DWFPublishedObject&        rPublishedObject,
                                     DWFDefinedObject**         ppDefinedObject,
                                     DWFDefinedObjectInstance** ppDefinedInstance )
            throw( DWFException );

        _DWFTK_API
        DWFDefinedObject* _findDefinedObject( DWFPublishedObject& rPublishedObject ) const
            throw();

    private:

        DWFObjectDefinitionResource*    _pObjectDefinition;

		// This is used to collect the instances not be handled by _pObjectDefinition.
		DWFObjectDefinitionResource*    _pReferencedObjectDefinition;

        unsigned int                    _nNextInstanceID;

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
    DWFPublishedDefinedObject( tKey             nKey,
                               const DWFString& zName )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPublishedDefinedObject()
        throw();

    ///
    ///\copydoc DWFPropertyContainer::addPropertyContainer(DWFPropertyContainer* pContainer)
    ///
    _DWFTK_API
    virtual void addPropertyContainer( DWFPropertyContainer* pContainer )
        throw( DWFException );

    ///
    ///\copydoc DWFPropertyContainer::referencePropertyContainer(const DWFPropertyContainer& rContainer)
    ///
    _DWFTK_API
    virtual void referencePropertyContainer( const DWFPropertyContainer& rContainer )
        throw( DWFException );

};

//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start
_declspec(selectany) const wchar_t* const DWFPublishedDefinedObject::Visitor::kz_PropName_Published_Object =    L"_name";
_declspec(selectany) const wchar_t* const DWFPublishedDefinedObject::Visitor::kz_PropCategory_Hidden =          L"hidden";
//DNT_End

#endif
#endif

}

#endif
#endif
