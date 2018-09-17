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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/ResourcePart.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_RESOURCEPART_H
#define _DWFTK_RESOURCEPART_H

///
///\file        dwf/dwfx/ResourcePart.h
///\brief       This file contains the DWFXResourcePart class declaration.
///


#include "dwf/opc/Part.h"

namespace DWFToolkit
{

//
// fwd declaration
//
class DWFResource;

///
///\ingroup     dwfx
///
///\class       DWFXResourcePart   dwf/dwfx/ResourcePart.h     "dwf/dwfx/ResourcePart.h"
///\brief       This class wraps a DWF resource into an OPC part.
///\since       7.4.0
///
class DWFXResourcePart : public OPCPart
{

public:

    ///
    ///\brief   This type defines a list of DWFXResourcePart pointers.
    ///
    typedef DWFOrderedVector<DWFXResourcePart*>             tList;
    ///
    ///\brief   This type defines a mapped collection of DWFXResourcePart pointers.
    ///
    typedef DWFStringKeySkipList<DWFXResourcePart*>         tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFXResourcePart
    ///         pointers. The iterator caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFXResourcePart*>           tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         DWFXResourcePart pointers.
    ///
    typedef DWFIterator<DWFXResourcePart*>                  tIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         DWFXResourcePart pointers.
    ///
    typedef DWFConstIterator<DWFXResourcePart*>             tConstIterator;

public:

    _DWFTK_API
    DWFXResourcePart( DWFResource* pResource )
        throw();

    _DWFTK_API
    virtual ~DWFXResourcePart()
        throw();

    _DWFTK_API
    virtual DWFInputStream* getInputStream()
        throw();

    _DWFTK_API
    DWFResource* const resource() const
        throw()
    {
        return _pResource;
    }

private:

    DWFResource* _pResource;

};

}

#endif
