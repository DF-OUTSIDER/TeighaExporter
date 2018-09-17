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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/ContentTypes.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCCONTENTTYPES_H
#define _DWFTK_OPCCONTENTTYPES_H


#include "dwf/opc/XMLPart.h"

namespace DWFToolkit
{

///
///\ingroup     opc
///
///\class       OPCContentTypes     dwf/opc/ContentTypes.h      "dwf/opc/ContentTypes.h"
///\brief       The OPCContentTypes defines mappings from extensions of part names to content types.
///\since       7.4.0
///
///             The OPCContentTypes class defines a mappings from extensions of part names to the 
///             corresponding content types. It also allows overrides, i.e., mappings of specific
///             part names to content types that are not covered by the content types or are not 
///             consistent with them.
///
class OPCContentTypes : public OPCXMLPart
{

    ///
    ///\brief   "[ContentTypes].xml"
    ///
    static const char* const kzName;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    OPCContentTypes()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~OPCContentTypes()
        throw();

    ///
    ///         Add the extension and associated content type. If the extension 
    ///         already exists in the content types, the pair will not be added
    ///         and the method will return false.
    ///\param   zExtension  The extension being associated with the type.
    ///\param   zType       The type.
    ///\return  True if the pair was added, false otherwise.
    ///\throw   None.
    ///
    _DWFTK_API
    bool addContentType( const DWFString& zExtension, const DWFString& zType )
        throw();

    ///
    ///         Add an override to an existing content type, or undefined
    ///         content type.
    ///
    ///\param   zPartURI  The URI of the OPC part.
    ///\param   zType       The type
    ///\return  True if the pair was added, false otherwise.
    ///\throw   None.
    ///
    _DWFTK_API
    bool addOverride( const DWFString& zPartURI, const DWFString& zType )
        throw();

    ///
    ///         Given an extension, get the content type. If the extension is
    ///         not listed in the content types, then this will return an
    ///         empty string.
    ///
    ///\param   zExtension  The extension for which we want the type.
    ///\return  The type corresponding to the extension. This is empty if the extension is not found.
    ///\throw   None.
    ///
    _DWFTK_API
    DWFString extensionType( const DWFString& zExtension ) const
        throw();

    ///
    ///         Given a part name, get the content type. This will query the overrides first and if 
    ///         nothing is found, the extension is extracted and the extension-to-type map is 
    ///         queried.
    ///
    ///\param   zPartURI    The URI of the part.
    ///\return  The type corresponding to the URI. This is empty if the partname or its extension are not found.
    ///\throw   None.
    ///
    _DWFTK_API
    DWFString partType( const DWFString& zPartURI ) const
        throw();


#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

private:

    typedef std::map<DWFString, DWFString> _tTypeMap;

private:

    _tTypeMap _oExtensions;
    _tTypeMap _oOverrides;

private:

    //
    // Not implemented
    //
    OPCContentTypes( const OPCContentTypes& );
    OPCContentTypes& operator=( const OPCContentTypes& );

};

//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START
_declspec(selectany) const char* const OPCContentTypes::kzName = "[Content_Types].xml";
//DNT_END

#endif
#endif


}

#endif
