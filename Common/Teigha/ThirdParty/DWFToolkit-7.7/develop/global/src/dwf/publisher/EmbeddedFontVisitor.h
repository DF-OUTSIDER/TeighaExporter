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

#ifndef _DWFTK_EMBEDDEDFONTVISITOR_H
#define _DWFTK_EMBEDDEDFONTVISITOR_H


///
///\file        dwf/publisher/EmbeddedFontVisitor.h
///\brief       This file contains the DWFEmbeddedFontVisitor class declaration.
///

#ifndef DWFTK_READ_ONLY



#include "dwfcore/STL.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/Exception.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/publisher/EmbeddedFont.h"



namespace DWFToolkit
{



///
///\ingroup         dwfpublish
///
///\class       DWFEmbeddedFontVisitor   dwf/publisher/EmbeddedFontVisitor.h    "dwf/publisher/EmbeddedFontVisitor.h"
///\brief       Base implementation and interface defintion for objects (visitors) that
///             publishers can use with publishable sources to extract their embedded font data.
///\since       7.0.1
///
class DWFEmbeddedFontVisitor _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFEmbeddedFontVisitor()
        throw()
    {;}

    ///
    ///         Default visitation implementation.  Specializations should
    ///         invoke this base method to ensure any composed visitors are called.
    ///
    ///\param   rFont       An embedded font from the publishable source.
    ///\throw   DWFException
    ///
    virtual void visitEmbeddedFont( const DWFEmbeddedFont& rFont )
        throw( DWFException )
    {
        if (_pComposite)
        {
            _pComposite->visitEmbeddedFont( rFont );
        }
    }

protected:

    ///
    ///         Constructor
    ///
    ///\param   pComposite      A pointer a chained visitor.
    ///                         This pointer will not be deleted.
    ///\throw   None
    ///
    DWFEmbeddedFontVisitor( DWFEmbeddedFontVisitor* pComposite = NULL )
        throw()
        : _pComposite( pComposite )
    {;}

private:

    DWFEmbeddedFontVisitor( const DWFEmbeddedFontVisitor& );
    DWFEmbeddedFontVisitor& operator=( const DWFEmbeddedFontVisitor& );

private:

    DWFEmbeddedFontVisitor* _pComposite;
};

}

#endif  

#endif

