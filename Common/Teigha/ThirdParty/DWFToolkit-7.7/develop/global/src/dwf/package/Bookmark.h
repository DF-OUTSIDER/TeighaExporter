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


#ifndef _DWFTK_BOOKMARK_H
#define _DWFTK_BOOKMARK_H


///
///\file        dwf/package/Bookmark.h
///\brief       This file contains the DWFBookmark class declaration.
///


#include "dwfcore/STL.h"


#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/writer/PackageWriter.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFBookmark     dwf/package/Bookmark.h      "dwf/package/Bookmark.h"
///\brief       A bookmark represents a global hyperlink in DWF package.
///\since       7.0.1
///
///             The DWFBookmark is a composite object that defines a hierarchy (usually
///             displayed in a tree view) of hyperlinks.  A bookmark contains a friendly
///             name and an HREF and optionally, a collection of child bookmarks.
///             A bookmark is generally used for intra-DWF linking but there are no
///             specified restrictions on the nature of the HREF.
///
class DWFBookmark : public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
                  , public DWFXMLSerializable
#endif
                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFBookmark pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFBookmark*) tList;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFBookmark()
        throw();

    ///
    ///         Constructor
    ///
    ///\param   zName   The friendly (display) name.
    ///\param   zHRef   The hyperlink location.
    ///\throw   None
    ///
    _DWFTK_API
    DWFBookmark( const DWFString& zName,
                 const DWFString& zHRef )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFBookmark()
        throw();

    ///
    ///         Returns the friendly (display) name of the bookmark.
    ///
    ///\return  The name.
    ///\throw   None
    ///
    const DWFString& name() const
        throw()
    {
        return _zName;
    }

    ///
    ///         Returns the hyperlink location of the bookmark.
    ///
    ///\return  The HREF.
    ///\throw   None
    ///
    const DWFString& href() const
        throw()
    {
        return _zHRef;
    }

    ///
    ///         Returns the collection of child bookmarks.
    ///
    ///\return  A reference to the child collection.
    ///\throw   None
    ///\todo    Return an iterator not as a direct reference to the internal collection.
    ///
    const DWFBookmark::tList& getChildBookmarks() const
        throw()
    {
        return _oChildren;
    }

    ///
    ///         Adds a bookmark to collection of child bookmarks.
    ///
    ///\param   pBookmark   A pointer to a bookmark to add.
    ///                     \a pBookmark will now be owned by this object
    ///                     and ultimately deleted with the \b DWFCORE_FREE_OBJECT
    ///                     macro.  Therefore it is required that \a pBookmark
    ///                     is allocated with the \b DWFCORE_ALLOC_OBJECT macro.
    ///\throw   None
    ///
    _DWFTK_API
    void addChildBookmark( DWFBookmark* pBookmark )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );


#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

private:

    //
    // recursive helper
    //
    void _serialize( DWFBookmark& rBookmark, DWFXMLSerializer& rSerializer, const DWFString& zNamespace )
        throw( DWFException );

#endif

private:

    DWFString   _zName;
    DWFString   _zHRef;

    DWFBookmark::tList _oChildren;
};

}

#endif
