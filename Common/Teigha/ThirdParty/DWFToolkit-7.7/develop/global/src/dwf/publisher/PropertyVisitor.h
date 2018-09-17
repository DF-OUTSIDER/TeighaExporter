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

#ifndef _DWFTK_PROPERTYVISITOR_H
#define _DWFTK_PROPERTYVISITOR_H


///
///\file        dwf/publisher/PropertyVisitor.h
///\brief       This file contains the DWFPropertyVisitor class declaration.
///

#ifndef DWFTK_READ_ONLY



#include "dwfcore/STL.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/Exception.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Property.h"
#include "dwf/package/utility/PropertyContainer.h"



namespace DWFToolkit
{



///
///\ingroup         dwfpublish
///
///\class       DWFPropertyVisitor   dwf/publisher/PropertyVisitor.h    "dwf/publisher/PropertyVisitor.h"
///\brief       Base implementation and interface defintion for objects (visitors) that
///             publishers can use with publishable sources to extract their property data.
///\since       7.0.1
///
class DWFPropertyVisitor
    // _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE - intentionally omitted, as this is just an interface
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFPropertyVisitor()
        throw()
    {;}

    ///
    ///         Default visitation implementation.  Specializations should
    ///         invoke this base method to ensure any composed visitors are called.
    ///
    ///\param   rProperty   A property from the publishable source.
    ///\throw   DWFException
    ///
    virtual void visitProperty( DWFProperty& rProperty )
        throw( DWFException )
    {
        if (_pComposite)
        {
            _pComposite->visitProperty( rProperty );
        }
    }

    ///
    ///         Default visitation implementation.  Specializations should
    ///         invoke this base method to ensure any composed visitors are called.
    ///
    ///\param   rPropertyContainer   The property container from the publishable source.
    ///\throw   DWFException
    ///
    virtual void visitPropertyContainer( DWFPropertyContainer& rPropertyContainer )
        throw( DWFException )
    {
        if (_pComposite)
        {
            _pComposite->visitPropertyContainer( rPropertyContainer );
        }
    }

protected:

    ///
    ///         Constructor
    ///
    ///\param   pComposite      A pointer to a chained visitor.
    ///                         This pointer will not be deleted.
    ///\throw   None
    ///
    DWFPropertyVisitor( DWFPropertyVisitor* pComposite = NULL )
        throw()
        : _pComposite( pComposite )
    {;}

private:

    DWFPropertyVisitor( const DWFPropertyVisitor& );
    DWFPropertyVisitor& operator=( const DWFPropertyVisitor& );

private:

    DWFPropertyVisitor* _pComposite;
};

}

#endif  

#endif

