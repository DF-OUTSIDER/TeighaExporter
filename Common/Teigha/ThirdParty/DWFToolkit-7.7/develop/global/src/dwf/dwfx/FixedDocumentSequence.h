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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/FixedDocumentSequence.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXFIXEDDOCUMENTSEQUENCE_H
#define _DWFTK_DWFXFIXEDDOCUMENTSEQUENCE_H


///
///\file        dwf/dwfx/FixedDocumentSequence.h"
///\brief       <TODO>
///

#include "dwf/xps/FixedDocumentSequence.h"

namespace DWFToolkit
{

class DWFXFixedDocumentSequence : public XPSFixedDocumentSequence
{

public:

    _DWFTK_API
    DWFXFixedDocumentSequence()
        throw();

    _DWFTK_API
    virtual ~DWFXFixedDocumentSequence()
        throw();

    /////
    /////\copydoc XPSFixedDocument::addFixedDocument()
    /////
    //_DWFTK_API
    //virtual bool addFixedDocument( XPSFixedDocument* pFixedDocument,
    //                               bool bOwn = true )
    //    throw();

    /////
    /////\copydoc XPSFixedDocument::insertFixedDocument()
    /////
    //_DWFTK_API
    //virtual bool insertFixedDocument( XPSFixedDocument* pFixedDocument, 
    //                                  XPSFixedDocument* pAfterDocument,
    //                                  bool bOwn = true )
    //    throw();

    /////
    /////\copydoc XPSFixedDocument::removeFixedDocument()
    /////
    //_DWFTK_API
    //virtual bool removeFixedDocument( XPSFixedDocument* pFixedDoc )
    //    throw();


private:

    //
    // Not implemented
    //
    DWFXFixedDocumentSequence( const DWFXFixedDocumentSequence& );
    DWFXFixedDocumentSequence& operator=( const DWFXFixedDocumentSequence& );

};

}

#endif
