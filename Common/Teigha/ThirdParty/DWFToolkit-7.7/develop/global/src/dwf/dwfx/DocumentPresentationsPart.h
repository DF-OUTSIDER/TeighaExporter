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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/DocumentPresentationsPart.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DOCUMENTPRESENTATIONSPART_H
#define _DWFTK_DOCUMENTPRESENTATIONSPART_H


#include "dwf/opc/XMLPart.h"

namespace DWFToolkit
{

//
// fwd declaration
//
class DWFPackageContentPresentations;

class DWFXDocumentPresentationsPart : public OPCXMLPart
{

public:

    _DWFTK_API
    DWFXDocumentPresentationsPart( DWFPackageContentPresentations* pPackageContentPresentations )
        throw();

    _DWFTK_API
    virtual ~DWFXDocumentPresentationsPart()
        throw();


    _DWFTK_API
    DWFPackageContentPresentations* const presentations() const
        throw()
    {
        return _pPackageContentPresentations;
    }

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

private:

    DWFPackageContentPresentations* _pPackageContentPresentations;


private:

    //
    // Not implemented
    //
    DWFXDocumentPresentationsPart( const DWFXDocumentPresentationsPart& );
    DWFXDocumentPresentationsPart& operator=( const DWFXDocumentPresentationsPart& );

};

}

#endif
