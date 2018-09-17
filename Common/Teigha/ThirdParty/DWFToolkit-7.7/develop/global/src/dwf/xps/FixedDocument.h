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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/xps/FixedDocument.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_XPSFIXEDDOCUMENT_H
#define _DWFTK_XPSFIXEDDOCUMENT_H

///
///\file        dwf/xps/FixedDocument.h
///\brief       This file contains the XPSFixedDocument class declarations.
///

#include "dwfcore/Owner.h"
using namespace DWFCore;

#include "dwf/opc/XMLPart.h"
#include "dwf/xps/FixedPage.h"

namespace DWFToolkit
{

///
///\ingroup     xps
///
///\interface   XPSFixedDocument    dwf/xps/FixedDocument.h "dwf/xps/FixedDocument.h"
///\brief       This class provides the interface to a FixedDocument as defined in the XPS specification.
///\since       7.4.0
///
class XPSFixedDocument : public OPCXMLPart
                       , protected DWFOwner
{

    ///
    ///\brief   "FixedDocument.fdoc"
    ///
    static const char* const kzName;

public:

    ///
    ///\brief   This type defines a list of XPSFixedDocument pointers.
    ///
    typedef DWFOrderedVector<XPSFixedDocument*>         tList;
    ///
    ///\brief   This type defines a mapped collection of XPSFixedDocument pointers.
    ///
    typedef DWFStringKeySkipList<XPSFixedDocument*>     tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of XPSFixedDocument
    ///         pointers. The iterator caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<XPSFixedDocument*>       tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of 
    ///         XPSFixedDocument pointers.
    ///
    typedef DWFIterator<XPSFixedDocument*>              tIterator;
    ///
    ///\brief   This type defines a basic const iterator on a collection of 
    ///         XPSFixedDocument pointers.
    ///
    typedef DWFConstIterator<XPSFixedDocument*>         tConstIterator;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    XPSFixedDocument()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~XPSFixedDocument()
        throw();

    ///
    ///         Add a fixed page to the document. This will take ownership 
    ///         of the page unless otherwise mentioned.
    ///
    ///\param   pFixedPage  The XPS fixed oage to be added to the document.
    ///\param   bOwn        Take ownership of the page if true, otherwise observe it.
    ///\return  True if it is added, false otherwise
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool addFixedPage( XPSFixedPage* pFixedPage,
                              bool bOwn = true )
        throw();

    ///
    ///         Insert a fixed page into the document at a position after
    ///         that of the provided page. If the \a pAfterPage argument is NULL, the 
    ///         new page will be inserted at the beginning. If the argument is not NULL,
    ///         but it does not exist in the document, the new page will not be inserted,
    ///         and the method will return false. 
    ///
    ///\param   pFixedPage  The XPS fixed page to be inserted into the document.
    ///\param   pAfterPage  The fixed page after which the new [age will be inserted.
    ///\param   bOwn        Take ownership of the document if true, otherwise observe it.
    ///\return  True if it is inserted, false otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool insertFixedPage( XPSFixedPage* pFixedPage, 
                                  XPSFixedPage* pAfterPage,
                                  bool bOwn = true )
        throw();

    ///
    ///         Remove a fixed page from the document. This does \b not delete 
    ///         the page. The caller must manage the lifetime of the removed page.
    ///
    ///\param   pFixedPage  The XPS fixed page to be removed.
    ///\return  True if it is found and removed, false otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool removeFixedPage( XPSFixedPage* pFixedPage )
        throw();

    ///
    ///         Find the fixed page with the given URI.
    ///
    ///\param   zURI        The URI of the page.
    ///\return  Pointer to the page, NULL if it is not found.
    ///\throw   None
    ///
    _DWFTK_API
    XPSFixedPage* const fixedPage( const DWFString& zURI ) const
        throw();

    ///
    ///         Get an iterator over all the pages in the document.
    ///
    ///\return  An iterator that must be deleted using \b DWFCORE_FREE_OBJECT.
    ///         The iterator maybe NULL.
    ///throw    None
    ///
    _DWFTK_API
    XPSFixedPage::tConstIterator* fixedPages() const
        throw()
    {
        return _oFixedPages.constIterator();
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

    XPSFixedPage::tList         _oFixedPages;

private:

    //
    // Not implemented
    //
    XPSFixedDocument( const XPSFixedDocument& );
    XPSFixedDocument& operator=( const XPSFixedDocument& );

};


//
// required for win32 dll external linkage
//
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_START
_declspec(selectany) const char* const XPSFixedDocument::kzName = "FixedDocument.fdoc";
//DNT_END

#endif
#endif

}

#endif

