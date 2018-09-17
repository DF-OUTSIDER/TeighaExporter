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


#ifndef _DWFTK_PAPER_H
#define _DWFTK_PAPER_H

///
///\file        dwf/package/Paper.h
///\brief       This file contains the DWFPaper class declaration.
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
///\class       DWFPaper   dwf/package/Paper.h     "dwf/package/Paper.h"
///\brief       This class represents the virtual paper upon which all 2D graphics
///             are plotted.
///\since       7.0.1
///
class DWFPaper : public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
               , public DWFXMLSerializable
#endif

               _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFPaper pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFPaper*)    tList;

public:

    typedef enum
    {
        eMillimeters,
        eInches,

        eUnknown

    } teUnits;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFPaper()
        throw();

    ///
    ///         Constructor
    ///
    ///\param   nWidth      The paper width in \a eUnits.
    ///\param   nHeight     The paper height in \a eUnits.
    ///\param   eUnits      The units in which the paper dimensions apply.
    ///\param   nColorARGB  The paper color (as a 32-bit value)
    ///\param   anClip      An optional clipping path to be applied to the paper graphics.
    ///\param   bShow       Indicates whether or not the paper should be shown under the graphics.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPaper( double        nWidth,
              double        nHeight,
              teUnits       eUnits = eUnknown,
              unsigned int  nColorARGB = 0x00ffffff,
              const double* anClip = NULL,
              bool          bShow = true )
        throw();

    ///
    ///         Copy Constructor
    ///
    ///\param   rPaper      The source paper from which to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPaper( const DWFPaper& rPaper )
        throw();

    ///
    ///         Assignment Operator
    ///
    ///\param   rPaper      The source paper from which to copy.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPaper& operator=( const DWFPaper& rPaper )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPaper()
        throw();

    ///
    ///         Indicates whether or not the paper should be shown under the graphics.
    ///
    ///         The graphics must still be drawn on the paper regardless of this setting.
    ///\return  The show state.
    ///\throw   None
    ///
    bool show() const
        throw()
    {
        return _bShow;
    }

    ///
    ///         Returns the horizontal dimension in \a units().
    ///
    ///\return  The paper width.
    ///\throw   None
    ///
    double width() const
        throw()
    {
        return _nWidth;
    }

    ///
    ///         Returns the vertical dimension in \a units().
    ///
    ///\return  The paper height.
    ///\throw   None
    ///
    double height() const
        throw()
    {
        return _nHeight;
    }

    ///
    ///         Returns the 32-bit paper color.
    ///
    ///\return  The color value.
    ///\throw   None
    ///
    unsigned int color() const
        throw()
    {
        return _nColorARGB;
    }

    ///
    ///         Returns the path by which the graphic data should be clipped.
    ///
    ///\return  The clipping path. This pointer should not be deleted by the caller.
    ///\throw   None
    ///

    const double* const clip() const
        throw()
    {
        return (const double* const)_anClip;
    }

    ///
    ///         Returns the units that apply to the paper dimensions.
    ///
    ///\return  The units enumeration.
    ///\throw   None
    ///
    const teUnits units() const
        throw()
    {
        return _eUnits;
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

    bool            _bShow;
    double          _nWidth;
    double          _nHeight;
    unsigned int    _nColorARGB;
    double          _anClip[4];
    teUnits         _eUnits;
};

}

#endif
