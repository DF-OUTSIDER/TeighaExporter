//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
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


#ifndef _DWFTK_INTERFACE_H
#define _DWFTK_INTERFACE_H


///
///\file        dwf/package/Interface.h
///\brief       This file contains the DWFInterface class declaration.
///

#include "dwfcore/STL.h"


#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/writer/PackageWriter.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFInterface   dwf/package/Interface.h     "dwf/package/Interface.h"
///\brief       This class identifies the existence of a section type in a DWF package.
///\since       7.0.1
///
class DWFInterface : public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
                   , public DWFXMLSerializable
#endif

                   _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   L"ePlot"
    ///
    static const wchar_t* const kzEPlot_Name;
    ///
    ///\brief   L"http://www.autodesk.com/viewers";
    ///
    static const wchar_t* const kzEPlot_HRef;
    ///
    ///\brief   L"715941D4-1AC2-4545-8185-BC40E053B551";
    ///
    static const wchar_t* const kzEPlot_ID;
    ///
    ///\brief   L"eModel"
    ///
    static const wchar_t* const kzEModel_Name;
    ///
    ///\brief   L"http://www.autodesk.com/viewers";
    ///
    static const wchar_t* const kzEModel_HRef;
    ///
    ///\brief   L"75E513A9-6C41-4C91-BAA6-81E593FAAC10";
    ///
    static const wchar_t* const kzEModel_ID;
    ///
    ///\brief   L"Data"
    ///
    static const wchar_t* const kzData_Name;
    ///
    ///\brief   L"http://www.autodesk.com/viewers";
    ///
    static const wchar_t* const kzData_HRef;
    ///
    ///\brief   L"12D1F3EE-1178-4C8F-A706-F27E156F7522";
    ///
    static const wchar_t* const kzData_ID;
    ///
    ///\brief   L"Signatures"
    ///
    static const wchar_t* const kzSignatures_Name;
    ///
    ///\brief   L"http://www.autodesk.com/viewers";
    ///
    static const wchar_t* const kzSignatures_HRef;
    ///
    ///\brief   L"22D1F3EE-1178-4C8F-A706-F27E156F7522";
    ///
    static const wchar_t* const kzSignatures_ID;

public:

    ///
    ///\brief   This type defines a list of DWFInterface pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFInterface*)    tList;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFInterface()
        throw();

    ///
    ///         Constructor
    ///
    ///\param   zName       The name of the section interface. Usually corresponds to the section type,
    ///                     either exactly or in abbreviation.
    ///\param   zHRef       A location where more information or specifications for handling this section can be found.
    ///\param   zObjectID   Uniquely identifies the section interface.
    ///\throw   None
    ///
    _DWFTK_API
    DWFInterface( const DWFString& zName,
                  const DWFString& zHRef,
                  const DWFString& zObjectID )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFInterface()
        throw();

    ///
    ///         Returns the interface name.
    ///
    ///         The name of the section interface. Usually corresponds to the section type,
    ///         either exactly or in abbreviation.
    ///
    ///\return  The interface name.
    ///\throw   None
    ///
    const DWFString& name() const
        throw()
    {
        return _zName;
    }

    ///
    ///         Returns the interface link location where more information or specifications for handling this section can be found.
    ///
    ///\return  The interface HRef.
    ///\throw   None
    ///
    const DWFString& href() const
        throw()
    {
        return _zHRef;
    }

    ///
    ///         Returns the interface identifier that uniquely identifies the section interface
    ///
    ///\return  The interface object ID.
    ///\throw   None
    ///
    const DWFString& objectID() const
        throw()
    {
        return _zObjectID;
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
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

private:

    DWFString _zName;
    DWFString _zHRef;
    DWFString _zObjectID;
};


    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start

_declspec(selectany) const wchar_t* const DWFInterface::kzEPlot_Name    = L"ePlot";
_declspec(selectany) const wchar_t* const DWFInterface::kzEPlot_HRef    = L"http://www.autodesk.com/viewers";
_declspec(selectany) const wchar_t* const DWFInterface::kzEPlot_ID      = L"715941D4-1AC2-4545-8185-BC40E053B551";

_declspec(selectany) const wchar_t* const DWFInterface::kzEModel_Name   = L"eModel";
_declspec(selectany) const wchar_t* const DWFInterface::kzEModel_HRef   = L"http://www.autodesk.com/viewers";
_declspec(selectany) const wchar_t* const DWFInterface::kzEModel_ID     = L"75E513A9-6C41-4C91-BAA6-81E593FAAC10";

_declspec(selectany) const wchar_t* const DWFInterface::kzData_Name   = L"Data";
_declspec(selectany) const wchar_t* const DWFInterface::kzData_HRef   = L"http://www.autodesk.com/viewers";
_declspec(selectany) const wchar_t* const DWFInterface::kzData_ID     = L"12D1F3EE-1178-4C8F-A706-F27E156F7522";

_declspec(selectany) const wchar_t* const DWFInterface::kzSignatures_Name   = L"Signatures";
_declspec(selectany) const wchar_t* const DWFInterface::kzSignatures_HRef   = L"http://www.autodesk.com/viewers";
_declspec(selectany) const wchar_t* const DWFInterface::kzSignatures_ID     = L"22D1F3EE-1178-4C8F-A706-F27E156F7522";

//DNT_End

#endif
#endif

}

#endif
