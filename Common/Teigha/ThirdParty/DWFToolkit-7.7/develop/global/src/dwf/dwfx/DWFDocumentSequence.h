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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/DWFDocumentSequence.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFDOCUMENTSEQUENCE_H
#define _DWFTK_DWFDOCUMENTSEQUENCE_H

#include "dwf/opc/XMLPart.h"
#include "dwf/dwfx/Package.h"
#include "dwf/dwfx/DWFDocument.h"
#include "dwf/dwfx/reader/DWFDocumentSequenceReader.h"

namespace DWFToolkit
{

class DWFXDWFDocumentSequence : public OPCXMLPart
                              , public DWFXDWFDocumentSequenceReader
                              , protected DWFOwner
{

public:

    ///
    ///\brief   "DWFDocumentSequence.dwfseq"
    ///
    static const char* const kzName;

public:

    _DWFTK_API
    DWFXDWFDocumentSequence()
        throw();

    _DWFTK_API
    virtual ~DWFXDWFDocumentSequence()
        throw();

    ///
    ///             Add a DWF document to the list of DWF documents. 
    ///             This will take not take ownership of the part unless requested.
    ///
    ///\param       pDWFDocument    The DWF document to be added.
    ///\param       bOwn            Take ownership of the part if true, otherwise observe it.
    ///\return      True if it is added, false otherwise
    ///\throw       None
    ///
    _DWFTK_API
    bool addDWFDocument( DWFXDWFDocument* pDWFDocument,
                         bool bOwn = true )
        throw();

    ///
    ///             Insert a DWF document into the sequence of documents at a position after
    ///             that of the provided one. If the \a pAfterDoc argument is NULL, the 
    ///             new document will be inserted at the beginning. If the argument is not NULL,
    ///             but it does not exist in the sequence, the new document will not be inserted,
    ///             and the method will return false. 
    ///
    ///\param       pDWFDocument    The DWF document to be inserted into the sequence.
    ///\param       pAfterDocument  The DWF document after which the new part will be inserted.
    ///\param       bOwn            Take ownership of the part if true, otherwise observe it.
    ///\return      True if it is inserted, false otherwise.
    ///\throw       None
    ///
    _DWFTK_API
    bool insertDWFDocument( DWFXDWFDocument* pDWFDocument,
                            DWFXDWFDocument* pAfterDocument,
                            bool bOwn = true )
        throw();

    ///
    ///             Remove a DWF document from the sequence of DWF documents. This does \b not
    ///             delete it. The caller must manage the lifetime of the removed document.
    ///
    ///\param       pDWFDocument    The DWF document to be removed from the sequence.
    ///\return      True if it is found and removed, false otherwise.
    ///\throw       None
    ///
    _DWFTK_API
    bool removeDWFDocument( DWFXDWFDocument* pDWFDocument )
        throw();

    ///
    ///             Access the list of document parts via an iterator.
    ///
    ///\return      An iterator that must be deleted using \b DWFCORE_FREE_OBJECT.
    ///             The iterator maybe NULL.
    ///\throw       None
    ///
    _DWFTK_API
    DWFXDWFDocument::tConstIterator* dwfDocuments() const
        throw()
    {
        return _oDWFDocuments.constIterator();
    }

    ///
    ///\internal    Access the list of document URIs. This is used during when reading
    ///             in the DWFDocumentSequence.
    ///
    ///\return      An iterator that must be deleted using \b DWFCORE_FREE_OBJECT.
    ///             The iterator maybe NULL.
    ///\throw       None
    ///
    _DWFTK_API
    DWFOrderedVector<DWFString>::ConstIterator* dwfDocumentURIs() const
        throw()
    {
        return _oDWFDocumentURIs.constIterator();
    }

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw();

#endif

    _DWFTK_API
    virtual DWFString provideDocumentURI( const DWFString& zDocumentURI )
        throw();

protected:

    _DWFTK_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

private:

    DWFXDWFDocument::tList      _oDWFDocuments;

    //
    //  This is populated when reading the DWFDocumentSequence, since
    //  _oDWFDocuments cannot be populated with actually creating the documents.
    //
    DWFOrderedVector<DWFString> _oDWFDocumentURIs;

private:

    //
    // Not implemented
    //
    DWFXDWFDocumentSequence( const DWFXDWFDocumentSequence& );
    DWFXDWFDocumentSequence& operator=( const DWFXDWFDocumentSequence& );

};


//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START
_declspec(selectany) const char* const DWFXDWFDocumentSequence::kzName = "DWFDocumentSequence.dwfseq";
//DNT_END

#endif
#endif

}

#endif
