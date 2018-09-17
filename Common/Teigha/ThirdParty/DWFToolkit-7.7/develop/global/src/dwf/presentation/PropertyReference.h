//
//  Copyright (c) 2006 by Autodesk, Inc.
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

//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/presentation/PropertyReference.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $

#ifndef _DWFTK_PROPERTY_REFERENCE_H
#define _DWFTK_PROPERTY_REFERENCE_H


///
///\file        dwf/presentation/PropertyReference.h
///\brief       This file contains the DWFPresentationNode class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/Vector.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpresentation
///
///\class       DWFPropertyReference     dwf/package/PropertyReference.h      "dwf/package/PropertyReference.h"
///\brief       The DWFPropertyReference class allows definition of references to properties.
///\since       7.2
///
///             The DWFPropertyReference class allows definition of references to properties.
///

class DWFPropertyReference
                            : public DWFXMLBuildable
#ifndef DWFTK_READ_ONLY
                            , public DWFXMLSerializable
#endif
                            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:
    ///
    ///\brief   This type defines a list of DWFPropertyReference pointers.
    ///
    typedef DWFOrderedVector<DWFPropertyReference*>            tList;

public:

    ///
    ///         Constructor
    ///
    ///\param   zID     A unique string identifying this property reference. If not provided, one will be assigned.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPropertyReference( const DWFString& zID = /*NOXLATE*/L"" )
        throw();
    ///
    ///         Destructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPropertyReference()
        throw();


    ///
    ///         Sets an id for this view.
    ///
    ///\param   zID       A string that specifies the id.
    ///\throw   None
    ///
    virtual void setID( const DWFString& zID )
        throw()
    {
        _zID = zID;
    }

    ///
    ///         Returns the ID for this node.
    ///
    ///\return  The id for this node
    ///\throw   None
    ///
    virtual const DWFString& id() const
        throw()
    {
        return _zID;
    }

    ///
    ///         Defines the property reference.
    ///
    ///         If the containing view has a property set or schema id defined this property
    ///         referenced will be scoped by it.
    //          
    ///
    ///\param   zName                   A string containing the name of the property
    ///\param   zCategory               A string containing the category to which
    ///                                 the property belongs. If the property is
    ///                                 not grouped under any category, this can be null
    ///\param   zLabel                  The label for this property reference. This
    ///                                 can be null. If it is null, the property name
    ///                                 will be used as the label.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setReference( const DWFString& zName,
                               const DWFString& zCategory = /*NOXLATE*/L"",
                               const DWFString& zLabel = /*NOXLATE*/L"" )
        throw( DWFException );
    ///
    ///         Defines the reference to a property contained in a property set
    ///         that's identified by its set id.
    ///
    ///\param   zPropertySetID          A string containing the set ID of the
    ///                                 property set that contains this property
    ///                                 reference. This must not be null.
    ///\param   zName                   A string containing the name of the property
    ///\param   zCategory               A string containing the category to which
    ///                                 the property belongs. If the property is
    ///                                 not grouped under any category, this can be null
    ///\param   zLabel                  The label for this property reference. This
    ///                                 can be null. If it is null, the property name
    ///                                 will be used as the label.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setReferenceWithSetID( const DWFString& zPropertySetID,
                                        const DWFString& zName,
                                        const DWFString& zCategory = /*NOXLATE*/L"",
                                        const DWFString& zLabel = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Defines the reference to a property contained in a property set
    ///         that's identified by its set id.
    ///
    ///\param   zPropertySchemaID       A string containing the schema ID of the
    ///                                 property set that contains this property
    ///                                 reference. This must not be null.
    ///\param   zName                   A string containing the name of the property
    ///\param   zCategory               A string containing the category to which
    ///                                 the property belongs. If the property is
    ///                                 not grouped under any category, this can be null
    ///\param   zLabel                  The label for this property reference. This
    ///                                 can be null. If it is null, the property name
    ///                                 will be used as the label.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setReferenceWithSchemaID( const DWFString& zPropertySchemaID,
                                           const DWFString& zName,
                                           const DWFString& zCategory = /*NOXLATE*/L"",                                           
                                           const DWFString& zLabel = /*NOXLATE*/L"" )
        throw( DWFException );
    ///
    ///         Returns the name of the property being referenced 
    ///
    ///\return  The name of the property being referenced 
    ///\throw   None
    ///
    virtual const DWFString& name() const
        throw()
    {
        return _zName;
    }

    ///
    ///         Returns the category to which the property being referenced belongs
    ///         to. This can be null to indicate no category has been specified.
    ///
    ///\return  The category to which the property being referenced belongs to.
    //          
    ///\throw   None
    ///
    virtual const DWFString& category() const
        throw()
    {
        return _zCategory;
    }

    ///
    ///         Returns the identifier for the property set to which the
    ///         property being referenced belongs to. This can be null to
    ///         indicate the property does not belong to a set.
    ///
    ///\return  The identifier for the property set to which the property being
    ///         referenced belong to.
    ///\throw   None
    ///
    virtual const DWFString& propertySetID() const
        throw()
    {
        return _zPropertySetID;
    }

    ///
    ///         Returns the identifier for the schema of the property set to which the
    ///         property being referenced belongs to. This can be null to
    ///         indicate the property does not belong to a set.
    ///
    ///\return  The identifier for the schema of the property set to which the property being
    ///         referenced belong to.
    ///\throw   None
    ///
    virtual const DWFString& propertySchemaID() const
        throw()
    {
        return _zPropertySchemaID;
    }

    ///
    ///         Returns the label for this property reference. If this is null
    ///         the property name should be used as the label.
    ///
    ///\return  The label to be used for this property
    ///\throw   None
    ///
    virtual const DWFString& label() const
        throw()
    {
        return _zLabel;
    }

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
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

    DWFString                         _zID;
    DWFString                         _zName;
    DWFString                         _zCategory;
    DWFString                         _zPropertySetID;
    DWFString                         _zPropertySchemaID;
    DWFString                         _zLabel;
};

}

#endif
