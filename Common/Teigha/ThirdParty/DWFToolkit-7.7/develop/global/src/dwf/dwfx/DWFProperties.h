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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/DWFProperties.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXDWFPROPERTIES_H
#define _DWFTK_DWFXDWFPROPERTIES_H

#include "dwf/opc/XMLPart.h"
#include "dwf/dwfx/reader/DWFPropertiesReader.h"
#include "dwf/package/PropertySet.h"

namespace DWFToolkit
{

class DWFXDWFProperties : public OPCXMLPart
                        , public DWFXDWFPropertiesReader
                        , protected DWFPropertySet
{

public:

    ///
    ///\brief   "DWFProperties.xml"
    ///
    static const char* const kzName;

public:

    ///
    ///         Constructor
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFXDWFProperties()
        throw( DWFException );

    ///
    ///         Destructor
    ///
    _DWFTK_API
    virtual ~DWFXDWFProperties()
        throw();

    ///
    ///\copydoc DWFPropertySet::getSchemaID()
    ///
    _DWFTK_API
    const DWFString& getSchemaID() const
        throw()
    {
        return DWFPropertySet::getSchemaID();
    }

    _DWFTK_API
    void setSourceProductVendor( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString sourceProductVendor()
        throw();

    _DWFTK_API
    void setSourceProductName( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString sourceProductName()
        throw();

    _DWFTK_API
    void setSourceProductVersion( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString sourceProductVersion()
        throw();

    _DWFTK_API
    void setDWFProductVendor( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString dwfProductVendor()
        throw();

    _DWFTK_API
    void setDWFProductVersion( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString dwfProductVersion()
        throw();

    _DWFTK_API
    void setDWFToolkitVersion( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString dwfToolkitVersion()
        throw();

    _DWFTK_API
    void setDWFFormatVersion( const DWFString& zValue )
        throw();

    _DWFTK_API
    DWFString dwfFormatVersion()
        throw();

	_DWFTK_API
    void setPasswordProtected( const DWFString& zValue )
        throw();

	_DWFTK_API
    DWFString passwordProtected()
        throw();

    _DWFTK_API
    DWFPropertySet* getPropertySet()
        throw()
    {
        return this;
    }

public:

    _DWFTK_API
    virtual DWFString provideSourceProductVendor( const DWFString& zValue )
        throw();

    _DWFTK_API
    virtual DWFString provideSourceProductName( const DWFString& zValue )
        throw();

    _DWFTK_API
    virtual DWFString provideSourceProductVersion( const DWFString& zValue )
        throw();

    _DWFTK_API
    virtual DWFString provideDWFProductVendor( const DWFString& zValue )
        throw();

    _DWFTK_API
    virtual DWFString provideDWFProductVersion( const DWFString& zValue )
        throw();

    _DWFTK_API
    virtual DWFString provideDWFToolkitVersion( const DWFString& zValue )
        throw();

    _DWFTK_API
    virtual DWFString provideDWFFormatVersion( const DWFString& zValue )
        throw();

	_DWFTK_API
    virtual DWFString providePasswordEncrypted( const DWFString& zValue )
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

    DWFString _getProperty( const DWFString& zPropertyName )
        throw();

};


//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START

_declspec(selectany) const char* const DWFXDWFProperties::kzName = "DWFProperties.xml";

//DNT_END

#endif
#endif

}

#endif



