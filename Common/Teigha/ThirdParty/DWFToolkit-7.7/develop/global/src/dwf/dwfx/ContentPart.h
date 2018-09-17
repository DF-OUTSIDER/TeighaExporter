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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/ContentPart.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_CONTENTPART_H
#define _DWFTK_CONTENTPART_H

///
///\file        dwf/dwfx/ContentPart.h
///\brief       This file contains the DWFXContentPart class declaration.
///

#include "dwf/opc/XMLPart.h"

namespace DWFToolkit
{

//
// fwd declaration
//
class DWFContent;

///
///\ingroup     dwfx
///
///\class       DWFXContentPart dwf/dwfx/ContentPart.h     "dwf/dwfx/ContentPart.h"
///\brief       A DWFXContentPart provides an OPC wrapper for the DWFContent class, allowing
///             access to it as an OPC part.
///\since       7.4.0
///
///             A DWFXContentPart provides an OPC wrapper for the DWFContent class, allowing
///             access to it as an OPC part. This allows the content to be serialized to a
///             DWFXPackage.
///
class DWFXContentPart : public OPCXMLPart
{

public:

    ///
    ///\brief   This type defines a list of DWFXContentPart pointers.
    ///
    typedef DWFOrderedVector<DWFXContentPart*>          tList;
    ///
    ///\brief   This type defines a mapped collection of DWFXContentPart pointers.
    ///
    typedef DWFStringKeySkipList<DWFXContentPart*>      tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFXContentPart
    ///         pointers. The iterator caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFXContentPart*>        tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         DWFXContentPart pointers.
    ///
    typedef DWFIterator<DWFXContentPart*>               tIterator;
    ///
    ///\brief   This type defines a basic const iterator on a collection of 
    ///         DWFXContentPart pointers.
    ///
    typedef DWFConstIterator<DWFXContentPart*>          tConstIterator;

public:

    _DWFTK_API
    DWFXContentPart( DWFContent* pContent )
        throw();

    _DWFTK_API
    virtual ~DWFXContentPart()
        throw();

    _DWFTK_API
    DWFContent* const content() const
    {
        return _pContent;
    }

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

private:

    DWFContent* _pContent;

private:

    //
    // Not implemented
    //
    DWFXContentPart( const DWFXContentPart& );
    DWFXContentPart& operator=( const DWFXContentPart& );

};

}

#endif
