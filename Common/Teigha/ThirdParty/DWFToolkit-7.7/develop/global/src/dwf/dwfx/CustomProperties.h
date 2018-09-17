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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/CustomProperties.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXCUSTOMPROPERTIES_H
#define _DWFTK_DWFXCUSTOMPROPERTIES_H

#include "dwf/opc/XMLPart.h"
#include "dwf/dwfx/reader/CustomPropertiesReader.h"
#include "dwf/package/PropertySet.h"

namespace DWFToolkit
{

class DWFXCustomProperties : public OPCXMLPart
                           , public DWFXCustomPropertiesReader
{

public:

    ///
    ///\brief   "CustomProperties.xml"
    ///
    static const char* const kzName;

public:

    ///
    ///         Constructor
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFXCustomProperties()
        throw( DWFException );

    ///
    ///         Destructor
    ///
    _DWFTK_API
    virtual ~DWFXCustomProperties()
        throw();

    ///
    ///         Inserts a property to the property set owned by this class.
    ///
    ///\param   pProperty               The property to add to the container (must not be NULL).
    ///\param   bOwnProperty            If \e true, the container will claim ownership of the property
    ///                                 and delete it using the \b DWFCORE_FREE_OBJECT macro upon
    ///                                 destruction of this container if it is still owned. If false
    ///                                 a copy of the property is created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void addProperty( DWFProperty* pProperty,
                      bool         bOwnProperty )
        throw( DWFException );    

    ///
    ///         Create and directly add a property to the property set owned by the class. 
    ///         The container will own the property.
    ///
    ///\param   zName       Identifies the property within its \a zCategory.
    ///\param   zValue      The data associated with the property.
    ///\param   zCategory   An optional grouping of properties.
    ///\param   zType       An optional type defining the data.
    ///\param   zUnits      An optional type defining the data.
    ///\return  A pointer to the created property.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFProperty* addProperty( const DWFString& zName,
                              const DWFString& zValue,
                              const DWFString& zCategory = /*NOXLATE*/L"",
                              const DWFString& zType = /*NOXLATE*/L"",
                              const DWFString& zUnits = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Removes a property from the class' property set
    ///
    ///\param   pProperty               The property to remove from the set (must not be NULL).
    ///\param   bDeleteIfOwned          If \e true, and if the container owns this property, the
    ///                                  property will be deleted.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void removeProperty( DWFProperty* pProperty,
                         bool         bDeleteIfOwned=true )
        throw( DWFException );

    ///
    ///         Locates a property in the property set of this class.
    ///
    ///\param   zName       The name of the property to look up (required.)
    ///\param   zCategory   The category of the property to look up (optional.)
    ///\return  A pointer to the property (may be NULL).  
    ///         This object is owned by the container by default and
    ///         must not be deleted by the caller without first explicitly
    ///         claiming ownership.
    ///\throw   None
    ///
    _DWFTK_API
    const DWFProperty* const findProperty( const DWFString& zName,
                                           const DWFString& zCategory = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Locates all properties in the container that share a category. If the category is
    ///         empty, all properties are returned.
    ///
    ///\param   zCategory   The property category to search by.
    ///\return  A pointer to an iterator for enumerating the properties (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Properties returned from the iterator are owned by the container 
    ///         by default and must not be deleted by the caller without first explicitly
    ///         claiming ownership.
    ///\throw   None
    ///
    _DWFTK_API
    DWFProperty::tMap::Iterator* getProperties( const DWFString& zCategory = /*NOXLATE*/L"" )
        throw();    

    ///
    ///         Return a PropertySet of this object's properties.
    ///
    ///\return  ..
    ///\throw   None
    ///
    _DWFTK_API
    DWFPropertySet* getPropertySet()
        throw()
    {
        return &_oPropertySet;
    }

public:

    _DWFTK_API
    virtual DWFProperty* provideProperty( DWFProperty* pProperty )
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc OPCXMLPart::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

private:

    DWFPropertySet  _oPropertySet;

};


//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START

_declspec(selectany) const char* const DWFXCustomProperties::kzName = "CustomProperties.xml";

//DNT_END

#endif
#endif

}

#endif



