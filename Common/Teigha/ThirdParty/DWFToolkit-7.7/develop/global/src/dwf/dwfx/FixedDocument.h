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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/FixedDocument.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXFIXEDDOCUMENT_H
#define _DWFTK_DWFXFIXEDDOCUMENT_H

#include "dwf/xps/FixedDocument.h"

namespace DWFToolkit
{

//
//  fwd declaration
//
class DWFManifest;

class DWFXFixedDocument : public XPSFixedDocument
{

public:

    _DWFTK_API
    DWFXFixedDocument( DWFManifest* pManifest )
        throw();

    _DWFTK_API
    virtual ~DWFXFixedDocument()
        throw();

    ///
    ///         Access the manifest that corresponds to the document.
    ///         This is used mainly by the package serialization code
    ///         to determine the path to generate for the fixed document.
    ///
    ///\return  A pointer to the document manifest.
    ///\throw   None
    ///
    _DWFTK_API
    DWFManifest* const manifest() const
        throw()
    {
        return _pManifest;
    }

    /////
    /////\copydoc XPSFixedDocument::addFixedPage()
    /////
    //_DWFTK_API
    //virtual bool addFixedPage( XPSFixedPage* pFixedPage,
    //                          bool bOwn = true )
    //    throw();

    /////
    /////\copydoc XPSFixedDocument::insertFixedPage()
    /////
    //_DWFTK_API
    //virtual bool insertFixedPage( XPSFixedPage* pFixedPage, 
    //                              XPSFixedPage* pAfterPage,
    //                              bool bOwn = true )

    //    throw();

    /////
    /////\copydoc XPSFixedDocument::removeFixedPage()
    /////
    //_DWFTK_API
    //virtual bool removeFixedPage( XPSFixedPage* pFixedPage )
    //    throw();

private:

    DWFManifest*        _pManifest;

private:

    //
    // Not implemented
    //
    DWFXFixedDocument( const DWFXFixedDocument& );
    DWFXFixedDocument& operator=( const DWFXFixedDocument& );

};

}

#endif
