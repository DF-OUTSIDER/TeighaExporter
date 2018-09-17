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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/xps/FixedPage.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_XPSFIXEDPAGE_H
#define _DWFTK_XPSFIXEDPAGE_H

///
///\file        dwf/xps/FixedPage.h
///\brief       This file contains the XPSFixedPage class declarations.
///

#include "dwf/opc/XMLPart.h"

namespace DWFToolkit
{

///
///\ingroup     xps
///
///\interface   XPSFixedPage    dwf/xps/FixedPage.h "dwf/xps/FixedPage.h"
///\brief       This class provides the interface to a FixedPage as defined in the XPS specification.
///\since       7.4.0
///
class XPSFixedPage : public OPCXMLPart
{

public:

    ///
    ///\brief   This type defines a list of XPSFixedPage pointers.
    ///
    typedef DWFOrderedVector<XPSFixedPage*>             tList;
    ///
    ///\brief   This type defines a mapped collection of XPSFixedPage pointers.
    ///
    typedef DWFStringKeySkipList<XPSFixedPage*>         tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of XPSFixedPage
    ///         pointers. The iterator caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<XPSFixedPage*>           tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         XPSFixedPage pointers.
    ///
    typedef DWFIterator<XPSFixedPage*>                  tIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         XPSFixedPage pointers.
    ///
    typedef DWFConstIterator<XPSFixedPage*>             tConstIterator;

public:

    ///
    ///\brief   "FixedPage.fpage"
    ///
    static const char* const kzName;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    XPSFixedPage()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~XPSFixedPage()
        throw();

    ///
    ///         Get the page height.
    ///
    ///\return  The printed page height.
    ///\throw   None
    ///
    _DWFTK_API
    double height() const
        throw()
    {
        return _dHeight;
    }

    ///
    ///         Get the page width.
    ///
    ///\return  The printed page width.
    ///\throw   None
    ///
    _DWFTK_API
    double width() const
        throw()
    {
        return _dWidth;
    }

    ///
    ///         Set the page width and height.
    ///
    ///\param   dWidth  The printed page width.
    ///\param   dHeight The printed page height.
    ///\throw   None
    ///
    _DWFTK_API
    void setPageSize( double dWidth, double dHeight )
        throw();

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException ) = 0;

#endif

private:

    double  _dWidth;
    double  _dHeight;

private:

    //
    // Not implemented
    //
    XPSFixedPage( const XPSFixedPage& );
    XPSFixedPage& operator=( const XPSFixedPage& );

};

//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START
_declspec(selectany) const char* const XPSFixedPage::kzName = "FixedPage.fpage";
//DNT_END

#endif
#endif


}

#endif
