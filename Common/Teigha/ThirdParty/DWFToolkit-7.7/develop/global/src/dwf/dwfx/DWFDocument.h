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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/DWFDocument.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFDOCUMENT_H
#define _DWFTK_DWFDOCUMENT_H

#include "dwfcore/Owner.h"
using namespace DWFCore;

#include "dwf/opc/XMLPart.h"
#include "dwf/dwfx/ContentPart.h"
#include "dwf/dwfx/DocumentPresentationsPart.h"
#include "dwf/dwfx/DWFSection.h"

namespace DWFToolkit
{

//
// fwd declaration
//
class DWFManifest;
class DWFXDWFProperties;
class DWFXCustomProperties;

class DWFXDWFDocument : public OPCXMLPart
                      , protected DWFOwner
{

public:

    ///
    ///\brief   This type defines a list of DWFXDWFDocument pointers.
    ///
    typedef DWFOrderedVector<DWFXDWFDocument*>              tList;
    ///
    ///\brief   This type defines a mapped collection of DWFXDWFDocument pointers.
    ///
    typedef DWFStringKeySkipList<DWFXDWFDocument*>          tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFXDWFDocument
    ///         pointers. The iterator caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<DWFXDWFDocument*>            tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         DWFXDWFDocument pointers.
    ///
    typedef DWFIterator<DWFXDWFDocument*>                   tIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         DWFXDWFDocument pointers.
    ///
    typedef DWFConstIterator<DWFXDWFDocument*>              tConstIterator;

public:

    ///
    ///\brief   "manifest.xml"
    ///
    static const char* const kzName;

public:

    _DWFTK_API
    DWFXDWFDocument( DWFManifest* pManifest )
        throw( DWFException );

    _DWFTK_API
    virtual ~DWFXDWFDocument()
        throw();

    _DWFTK_API
    void addContentPart( DWFXContentPart* pContentPart, 
                         bool bOwn = true )
        throw();

    _DWFTK_API
    bool removeContentPart( DWFXContentPart* pContentPart )
        throw();

    _DWFTK_API
    void addDocumentPresentationsPart( DWFXDocumentPresentationsPart* pPresentationPart, 
                                       bool bOwn = true )
        throw();

    _DWFTK_API
    bool removeDocumentPresentationsPart()
        throw();

    _DWFTK_API
    void addDWFSection( DWFXDWFSection* pSection,
                        bool bOwn = true )
        throw();

    _DWFTK_API
    bool removeDWFSection( DWFXDWFSection* pSection )
        throw();
    

    _DWFTK_API
    DWFManifest* const manifest() const
        throw()
    {
        return _pManifest;
    }

    _DWFTK_API
    DWFXContentPart::tConstIterator* contentParts() const
        throw()
    {
        return _oContentParts.constIterator();
    }

    _DWFTK_API
    DWFXDocumentPresentationsPart* const documentPresentationsPart() const
        throw()
    {
        return _pPresentationsPart;
    }

    _DWFTK_API
    DWFXDWFSection::tConstIterator* dwfSections() const
    {
        return _oDWFSections.constIterator();
    }

    ///
    ///         Use this to set the document level DWF properties it. If an old DWFProperties
    ///         exists its relationships will be copied over and it will be deleted in 
    ///         the process if the this document owns it.
    ///
    ///\param   pDWFProperties      The new DWF properties part.
    ///\param   bOwn                The document will take ownship of the part if this is true.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void virtual setDWFProperties( DWFXDWFProperties* pDWFProperties,
                                   bool bOwn = true )
        throw( DWFException );

    ///
    ///         Get the DWF properties part. 
    ///
    ///\return  Pointer to the document's DWF properties part.
    ///\throw   None
    ///
    _DWFTK_API
    DWFXDWFProperties* const dwfProperties() const
        throw()
    {
        return _pDWFProperties;
    }

    ///
    ///         Use this to set the document level custom published properties it. If an 
    ///         old set of custom properties exist, the relationships will be copied over
    ///         and it will be deleted in the process if the this document owns it.
    ///
    ///\param   pCustomProperties   The new custom properties part.
    ///\param   bOwn                The document will take ownship of the part if this is true.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void virtual setCustomProperties( DWFXCustomProperties* pCustomProperties,
                                      bool bOwn = true )
        throw( DWFException );

    ///
    ///         Get the custom properties part. 
    ///
    ///\return  Pointer to the document's custom properties part.
    ///\throw   None
    ///
    _DWFTK_API
    DWFXCustomProperties* const customProperties() const
        throw()
    {
        return _pCustomProperties;
    }    

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

protected:

    _DWFTK_API
    void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

private:

    DWFManifest*                    _pManifest;
    DWFXDWFProperties*              _pDWFProperties;
    DWFXCustomProperties*           _pCustomProperties;

    DWFXContentPart::tList          _oContentParts;
    DWFXDocumentPresentationsPart*  _pPresentationsPart;

    DWFXDWFSection::tList           _oDWFSections;

private:

    //
    // Not implemented
    //
    DWFXDWFDocument( const DWFXDWFDocument& );
    DWFXDWFDocument& operator=( const DWFXDWFDocument& );

};

//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START
_declspec(selectany) const char* const DWFXDWFDocument::kzName = "manifest.xml";
//DNT_END

#endif
#endif


}

#endif
