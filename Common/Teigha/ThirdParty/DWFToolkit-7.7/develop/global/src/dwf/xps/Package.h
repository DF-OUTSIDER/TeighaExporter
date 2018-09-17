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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/xps/Package.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_XPSPACKAGE_H
#define _DWFTK_XPSPACKAGE_H

///
///\file        dwf/xps/Package.h
///\brief       This file contains the XPSPackage class declarations.
///

#include "dwf/opc/Package.h"
#include "dwf/xps/FixedDocumentSequence.h"

namespace DWFToolkit
{

class XPSPackage : public OPCPackage
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    XPSPackage()
        throw( DWFException );

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~XPSPackage()
        throw();

    ///
    ///         Set the XPS Fixed Document sequence on the package. If the package has any
    ///         pre-existing fixed document sequence, the ownership of that will be relinquished
    ///         and the old document sequence will be returned. The package will own the new one.
    ///
    ///\param   pFixedDocumentSequence   The new fixed document sequence.
    ///\param   bOwn                        If true, take ownership of the sequence part.
    ///\return  Pointer to the old fixed document sequence. Most often this should be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    XPSFixedDocumentSequence* setFixedDocumentSequence( XPSFixedDocumentSequence* pFixedDocumentSequence, 
                                                        bool bOwn = true )
        throw();

    _DWFTK_API
    XPSFixedDocumentSequence * const xpsFixedDocumentSequence() const
        throw();

private:

    XPSFixedDocumentSequence *  _pFixedDocumentSequence;

private:

    //
    //  Not implemented
    //
    XPSPackage( const XPSPackage& );
    XPSPackage& operator=( const XPSPackage& );

};

}

#endif
