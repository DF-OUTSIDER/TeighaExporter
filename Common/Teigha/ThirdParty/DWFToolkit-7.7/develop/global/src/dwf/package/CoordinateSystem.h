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


#ifndef _DWFTK_COORDINATESYSTEM_H
#define _DWFTK_COORDINATESYSTEM_H

///
///\file        dwf/package/CoordinateSystem.h
///\brief       This file contains the DWFCoordinateSystem class declaration.
///


#include "dwfcore/STL.h"

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/writer/PackageWriter.h"
#include "dwf/package/utility/PropertyContainer.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFCoordinateSystem   dwf/package/CoordinateSystem.h     "dwf/package/CoordinateSystem.h"
///\brief       This class contains Coordinate System definitions.
///\since       7.3
///
class DWFCoordinateSystem : public DWFPropertyContainer,
                            public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
                          , public DWFXMLSerializable
#endif

                            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
public:

    ///
    ///\enum    teType
    ///\brief   Enumeration values that define the use of the coordinate system.
    ///
    typedef enum teType
    {
        ePublished,
        eMarkedup
    } teType;

public:

    ///
    ///\brief   This type defines a list of DWFCoordinateSystem pointers.
    ///
    typedef DWFOrderedVector<DWFCoordinateSystem*>    tList;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFCoordinateSystem( )
        throw();

    ///
    ///         Constructor
    ///
    ///\param   eType       Specifies whether this is a published or a marked up coordinate system.
    ///\param   anOrigin    The CoordinateSystem origin.
    ///\param   anRotation  The CoordinateSystem rotation matrix in row-major configuration.
    ///\param   zName       An optional user-friendly name for this CoordinateSystem.
    ///\param   zID         An optional identifier used when referencing this CoordinateSystem.
    ///\throw   None
    ///
    _DWFTK_API
    DWFCoordinateSystem( teType eType,
                         const double* anOrigin,
                         const double* anRotation,
                         const DWFString& zName = L"",
                         const DWFString& zID = L"" )
        throw();

    ///
    ///         Copy Constructor
    ///
    ///\param   rCoordinateSystem      The source CoordinateSystem from which to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFCoordinateSystem( const DWFCoordinateSystem& rCoordinateSystem )
        throw();

    ///
    ///         Assignment Operator
    ///
    ///\param   rCoordinateSystem      The source CoordinateSystem from which to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFCoordinateSystem& operator=( const DWFCoordinateSystem& rCoordinateSystem )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFCoordinateSystem()
        throw();

    ///
    ///         Sets the origin, rotation, name and optionally the ID for this CoordinateSystem
    ///
    ///\param   eType       Specifies whether this is a published or a marked up coordinate system.
    ///\param   anOrigin    The CoordinateSystem origin.
    ///\param   anRotation  The CoordinateSystem rotation matrix in row-major configuration.
    ///\param   zName       An optional user-friendly name for this CoordinateSystem.
    ///\param   zID         An optional identifier used when referencing this CoordinateSystem.
    ///         
    ///\return  None
    ///\throw   None
    ///
    _DWFTK_API
    void set( teType eType,
              const double* anOrigin,
              const double* anRotation,
              const DWFString& zName = L"",
              const DWFString& zID = L"" )
        throw();

    ///
    ///         Gets the origin of this CoordinateSystem
    ///
    ///\return  The origin of the CoordinateSystem.
    ///\throw   None
    ///
    const double* getOrigin( ) const
        throw()
    {
        return _anOrigin;
    }

    ///
    ///         Gets the rotation matrix of this CoordinateSystem
    ///
    ///\return  The rotation matrix of the CoordinateSystem as an array of doubles in row-major configuration.
    ///\throw   None
    ///
    const double* getRotation( ) const
        throw()
    {
        return (double*)_anRotation;
    }

    ///
    ///         Gets the user-friendly name for this CoordinateSystem
    ///
    ///\return  The user-friendly name
    ///\throw   None
    ///
    const DWFString& getName( ) const
        throw()
    {
        return _zName;
    }

    ///
    ///         Gets the ID for this CoordinateSystem
    ///
    ///\return  The ID
    ///\throw   None
    ///
    const DWFString& getID( ) const
        throw()
    {
        return _zID;
    }

    ///
    ///         Gets the type of this CoordinateSystem
    ///
    ///\return  The type
    ///\throw   None
    ///
     teType getType( ) const
        throw()
    {
        return _eType;
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

    teType          _eType;
    double          _anOrigin[3];
    double          _anRotation[3][3];
    DWFString       _zName;
    DWFString       _zID;
};

}

#endif
