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


#ifndef _DWFTK_UNITS_H
#define _DWFTK_UNITS_H

///
///\file        dwf/package/Units.h
///\brief       This file contains the DWFUnits class declaration.
///

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/writer/PackageWriter.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class   DWFUnits    dwf/package/Units.h     "dwf/package/Units.h"
///\brief   This class is used to describe model viewing and measurement units.
///\since       7.0.1
///
///         The data provided by this class is always used in conjunction with
///         the transformation provided by each graphic resource in the section.
///         See DWFEModelSection for more on using DWFUnits.
///
class DWFUnits : public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
               , public DWFXMLSerializable
#endif
               _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    typedef enum
    {
        eMillimeters,
        eCentimeters,
        eMeters,

        eInches,
        eFeet

    } teType;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFUnits()
        throw();

    ///
    ///         Constructor
    ///
    ///\param   eType   The enumeration value identifying the unit type.
    ///\throw   None
    ///
    _DWFTK_API
    DWFUnits( teType eType )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFUnits()
        throw();

    ///
    ///         Returns an enumeration value identifying the unit type.
    ///
    ///\throw   None
    ///
    const DWFString& type() const
        throw()
    {
        return _zType;
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

    DWFString _zType;
};

}

#endif
