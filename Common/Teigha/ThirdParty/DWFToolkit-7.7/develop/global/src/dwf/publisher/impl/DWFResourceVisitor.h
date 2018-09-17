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

#ifndef _DWFTK_DWFRESOURCEVISITOR_H
#define _DWFTK_DWFRESOURCEVISITOR_H


///
///\file        dwf/publisher/impl/DWFResourceVisitor.h
///\brief       This file contains the DWFResourceVisitor class declaration.
///

#ifndef DWFTK_READ_ONLY



#include "dwfcore/STL.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/Exception.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Resource.h"

namespace DWFToolkit
{


///
///\ingroup         dwfpublish
///
///\class       DWFResourceVisitor   dwf/publisher/impl/DWFResourceVisitor.h    "dwf/publisher/impl/DWFResourceVisitor.h"
///\brief       Base implementation and interface defintion for objects (visitors) that
///             publishers can use with publishable sections to modify resource data.
///\since       7.2.0
///
class DWFResourceVisitor
    // _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE - intentionally omitted, as this is just an interface
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFResourceVisitor()
        throw()
    {;}

    ///
    ///         Default visitation implementation.  Specializations should
    ///         invoke this base method to ensure any composed visitors are called.
    ///
    ///\param   rSection              A section.
    ///\param   pResource             A resource from the publishable section.
    ///\throw   DWFException
    ///
    virtual DWFResource* visitResource( DWFSection& rSection, DWFResource* pResource )
        throw( DWFException )
    {
        if (_pComposite)
        {
            return _pComposite->visitResource( rSection, pResource );
        }

        return NULL;
    }

protected:

    ///
    ///         Constructor
    ///
    ///\param   pComposite      A pointer to a chained visitor.
    ///                         This pointer will not be deleted.
    ///\throw   None
    ///
    DWFResourceVisitor( DWFResourceVisitor* pComposite = NULL )
        throw()
        : _pComposite( pComposite )
    {;}

private:

    DWFResourceVisitor( const DWFResourceVisitor& );
    DWFResourceVisitor& operator=( const DWFResourceVisitor& );

private:

    DWFResourceVisitor* _pComposite;
};

}

#endif  

#endif

