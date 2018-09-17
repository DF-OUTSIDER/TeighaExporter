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


#ifndef _DWFTK_DUPLICATE_ATTRIBUTE_FILTER_H
#define _DWFTK_DUPLICATE_ATTRIBUTE_FILTER_H

///
///\file        dwf/package/reader/DuplicateAttributeFilter.h
///\brief       This file contains the DWFDuplicateAttributeFilter class declaration.
///


#include "dwfcore/BufferInputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"

namespace DWFToolkit
{


///
///\ingroup     dwfpackage
///
///\class       DWFDuplicateAttributeFilter   dwf/package/reader/DuplicateAttributeFilter.h     "dwf/package/reader/DuplicateAttributeFilter.h"
///\brief       This is a plug-in filter for DWFXMLCallback implementations that removes
///             duplicated element attributes in malformed XML streams.
///\since       7.0.1
///
///             Certain older versions of AutoCAD (e.g. 2002)
///             are known to produce section descriptor documents
///             with malformed XML, in particular, duplicating the <i>xmlns:EPlot</i> attribute.
///             The DWFXMLCallback interface provides the \a setStreamFilter() method for hooking up a filter
///             between the document source and the XML parser.
///             This filter implementation detects and discards duplicate attributes.
///
///\warning     First, filtering slows the read/parse process considerably, testing shows
///             anyhere from 9% - 31% over a small sample set, your mileage may vary.
///
///\warning     Second, the algorthim was developed with the above mentioned problem in mind,
///             as such, it may or may not really fix super bad XML, but then, it's not really
///             supposed to...
///
///
class DWFDuplicateAttributeFilter : public DWFBufferInputStream
                                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\param   pInputStream    An input stream from which to stream data.
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pInputStream
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFTK_API
    DWFDuplicateAttributeFilter( DWFInputStream* pInputStream = NULL,
                                 bool            bOwnStream = false )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFDuplicateAttributeFilter()
        throw();

    ///
    ///\copydoc DWFCore::DWFBufferInputStream::available()
    ///
    _DWFTK_API
    virtual size_t available() const
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFBufferInputStream::read()
    ///
    _DWFTK_API
    virtual size_t read( void*  pBuffer,
                         size_t nBytesToRead )
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFBufferInputStream::seek()
    ///
    _DWFTK_API
    virtual off_t seek( int     eOrigin,
                        off_t   nOffset )
        throw( DWFException );

private:

    //
    // not implemented
    //
    DWFDuplicateAttributeFilter( const DWFDuplicateAttributeFilter& );
    DWFDuplicateAttributeFilter& operator=( const DWFDuplicateAttributeFilter& );
};

}

#endif

