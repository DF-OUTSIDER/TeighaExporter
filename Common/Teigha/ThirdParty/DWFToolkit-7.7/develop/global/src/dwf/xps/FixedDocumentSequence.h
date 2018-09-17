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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/xps/FixedDocumentSequence.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_XPSFIXEDDOCUMENTSEQUENCE_H
#define _DWFTK_XPSFIXEDDOCUMENTSEQUENCE_H

///
///\file        dwf/xps/FixedDocumentSequence.h
///\brief       This file contains the XPSFixedDocumentSequence class declarations.
///

#include "dwfcore/Owner.h"
using namespace DWFCore;

#include "dwf/opc/XMLPart.h"
#include "dwf/xps/FixedDocument.h"

namespace DWFToolkit
{

///
///\ingroup     xps
///
///\interface   XPSFixedDocumentSequence   dwf/xps/FixedDocumentSequence.h    "dwf/xps/FixedDocumentSequence.h"
///\brief       This class provides the interface to a FixedDocumentSequence as defined in the XPS specification.
///
///\since       7.4.0
///
///             The class provides the interfece for a FixedDocumentSequence as defined in the XPS specification.
///             It manages the lifetime and logical order the order of the fixed documents that will be serialized
///             into the associated \a XPSPackage.
///
class XPSFixedDocumentSequence : public OPCXMLPart
                               , protected DWFOwner
{

    ///
    ///\brief   "FixedDocumentSequence.fdseq"
    ///
    static const char* const kzName;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    XPSFixedDocumentSequence()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~XPSFixedDocumentSequence()
        throw();

    ///
    ///         Add a fixed document to the sequence of documents. This will take ownership 
    ///         of the document unless requested.
    ///
    ///\param   pFixedDocument  The XPS fixed document to be added to the sequence.
    ///\param   bOwn            Take ownership of the document if true, otherwise observe it.
    ///\return  True if it is added, false otherwise
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool addFixedDocument( XPSFixedDocument* pFixedDocument,
                                   bool bOwn = true )
        throw();

    ///
    ///         Insert a fixed document into the sequence of documents at a position after
    ///         that of the provided document. If the \a pAfterDoc argument is NULL, the 
    ///         new document will be inserted at the beginning. If the argument is not NULL,
    ///         but it does not exist in the sequence, the new document will not be inserted,
    ///         and the method will return false. 
    ///
    ///\param   pFixedDocument  The XPS fixed document to be inserted into the sequence.
    ///\param   pAfterDocument  The fixed document after which the new document will be inserted.
    ///\param   bOwn            Take ownership of the document if true, otherwise observe it.
    ///\return  True if it is inserted, false otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool insertFixedDocument( XPSFixedDocument* pFixedDocument, 
                                      XPSFixedDocument* pAfterDocument,
                                      bool bOwn = true )
        throw();

    ///
    ///         Remove a fixed document from the sequence of documents. This does \b not
    ///         delete the document. The caller must manage the lifetime of the removed
    ///         document.
    ///
    ///\param   pFixedDoc  The XPS fixed document to be removed from the sequence.
    ///\return  True if it is found and removed, false otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool removeFixedDocument( XPSFixedDocument* pFixedDoc )
        throw();

    ///
    ///         Find the fixed document with the given URI.
    ///
    ///\param   zURI        The URI of the document.
    ///\return  Pointer to the document, NULL if it is not found.
    ///\throw   None
    ///
    _DWFTK_API
    XPSFixedDocument* const fixedDocument( const DWFString& zURI ) const
        throw();

    ///
    ///         Get an iterator over all the documents in the document sequence.
    ///
    ///\return  An iterator that must be deleted using \b DWFCORE_FREE_OBJECT.
    ///         The iterator maybe NULL.
    ///throw    None
    ///
    _DWFTK_API
    XPSFixedDocument::tConstIterator* fixedDocuments() const
        throw()
    {
        return _oFixedDocuments.constIterator();
    }

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

protected:

    _DWFTK_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

private:

    XPSFixedDocument::tList     _oFixedDocuments;

private:

    //
    // Not implemented
    //
    XPSFixedDocumentSequence( const XPSFixedDocumentSequence& );
    XPSFixedDocumentSequence& operator=( const XPSFixedDocumentSequence& );

};


//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START
_declspec(selectany) const char* const XPSFixedDocumentSequence::kzName = "FixedDocumentSequence.fdseq";
//DNT_END

#endif
#endif

}

#endif
