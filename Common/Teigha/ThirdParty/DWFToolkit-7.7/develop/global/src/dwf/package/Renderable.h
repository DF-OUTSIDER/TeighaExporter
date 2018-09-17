//
//  Copyright (c) 2005-2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Renderable.h#1 $ $DateTime: 2011/02/14 01:16:30 $ $Author: caos $ $Change: 197964 $
//

#ifndef _DWFTK_RENDERABLE_H
#define _DWFTK_RENDERABLE_H

///
///\file        dwf/package/Renderable.h
///\brief       This file contains the DWFRenderable class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/SkipList.h"

#include "dwf/Toolkit.h"
#include "dwf/package/ContentElement.h"

namespace DWFToolkit
{

//
// fwd declarations
//
class DWFContent;

///
///\ingroup     dwfpackage
///
///\class       DWFRenderable   dwf/package/Renderable.h        "dwf/package/Renderable.h"
///\brief       DWFObject and DWFFeature derive from this class to allow polymorphic access to them.
///\since       7.2.0
///
///             This class exists solely to allow polymorphic access to DWFObject and DWFFeature, both
///             of which derive from this.
///
class DWFRenderable : public DWFContentElement
                      _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFRenderable pointers.
    ///
    typedef DWFOrderedVector<DWFRenderable*>            tList;
    ///
    ///\brief   This type defines a mapped collection of DWFRenderable pointers.
    ///
    typedef DWFStringKeySkipList<DWFRenderable*>        tMap;


protected:

    // Constructor
    DWFRenderable( const DWFString& zID = /*NOXLATE*/L"",
                   DWFContent* pContent = NULL )
        throw()
                 : DWFContentElement( zID, pContent )
    {}

    // Destructor
    virtual ~DWFRenderable()
        throw()
    {}

private:

    // Not implemented
    DWFRenderable( const DWFRenderable& );
    DWFRenderable& operator=( const DWFRenderable& );
};

}

#endif
