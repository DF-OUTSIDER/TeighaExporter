//
//  Copyright (c) 2005-2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/ContentManager.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//

#ifndef _DWFTK_CONTENT_MANAGER_H
#define _DWFTK_CONTENT_MANAGER_H

///
///\file        dwf/package/ContentManager.h
///\brief       This file contains the DWFContentManager class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/UUID.h"
#include "dwfcore/Owner.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/Content.h"
#include "dwf/package/Section.h"
namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFContentManager   dwf/package/ContentManager.h     "dwf/package/ContentManager.h"
///\brief       This class is used to manage one or more global content objects.
///\since       7.2.0
///
///             This class manages the one or more global contents available in DWF
///             package. The manager also manages the associations between each content and
///             the sections that refer to the content. Even if there are multiple contents
///             one of them will always be considered the primary content of the package.
///             A normalization of the content will merge the multiple contents into the
///             primary content.
///
///
class DWFContentManager : protected DWFOwner
                        , public DWFOwnable
#ifndef DWFTK_READ_ONLY
                        , public DWFXMLSerializable
#endif
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the parsing process
    ///         when the DWFPackageReader associated with
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read content
    ///         information from the DWF package.
    ///
    ///\param   pPackageReader  Provides access to content in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentManager( DWFPackageReader* pPackageReader = NULL )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentManager()
        throw();

    ///
    ///         Access all managed content via the iterator
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    DWFContent::tMap::Iterator* getContents()
        throw()
    {
        return _oContent.iterator();
    }

    ///
    ///         Get the content by its UUID. If no UUID is provided then the primary
    ///         content will be returned.
    ///
    ///\param   zContentID          UUID of the content being requested.
    ///\return  A pointer to a content. The pointer maybe NULL if there is no content with the given ID.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFContent* getContent( const DWFString& zContentID = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Get the content with the given HREF.
    ///
    ///\param   zHREF               The HREF of the content we are looking for.
    ///\return  A pointer to the content.
    ///\throw   None
    ///
    _DWFTK_API
    DWFContent* getContentByHREF( const DWFString& zHREF )
        throw();

    ///
    ///         Get the number of content libraries
    ///
    ///\return  The number of contents in the content manager.
    ///\throw   None
    ///
    size_t getContentCount() const
        throw()
    {
        return _oContent.size();
    }

    ///
    ///         Create a a DWFContent and add it the the list of contents. If an ID
    ///         is provided this will be the UUID for the content, otherwise the
    ///         manager will provide one. If this is the first content, it will be
    ///         set as the primary content. If the content is being added during 
    ///         a read, the content maybe created with a package reader at this point.
    ///
    ///\param   zID                 UUID to be assigned to content.
    ///\param   pReader             Pointer to a package reader.
    ///\return  A pointer to the newly created DWFContent.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFContent* addContent( const DWFString& zID = /*NOXLATE*/L"", DWFPackageReader* pReader = NULL )
        throw( DWFException );

    ///
    ///         Insert a pre-created content into the content manager. This is normally used during
    ///         the package reading process to reconstruct the content and content manager. 
    ///         Note: Loading from a stream may cause the cotent to already be created. If we have
    ///         a pre-existing content with the same ID and neither are in a "loaded" state the new one
    ///         (the one in the argument) will be discarded. Otherwise this keeps the content that is
    ///         in a loaded state.
    ///
    ///\param   pContent            The content getting set.
    ///\return  A pointer to the inserted content.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFContent* insertContent( DWFContent* pContent )
        throw( DWFException );

    ///
    ///         Remove completely all the content owned by the content manager. The content
    ///         is removed and returned to the caller in the provided container. 
    ///
    ///\param   roRemovedContent    A container of removed contents.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void removeContents( DWFContent::tList& roRemovedContent )
        throw();

    ///
    ///         Remove completely the specified content. If this is the primary
    ///         content, another content needs to be set as the primary, if available.
    ///         If a content ID is provided for the new primary, it will be set as
    ///         such, otherwise the first one available in the map will be set.
    ///
    ///\param   zContentID          The ID of the content being removed.
    ///\param   zNewPrimaryID       The ID of the conent to be set as primary.
    ///\param   bDeleteIfOwned      If true, manager will delete it.
    ///\return  Pointer to the returned content. If the deleted internally this will retunr NULL.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFContent* removeContent( const DWFString& zContentID,
                               const DWFString& zNewPrimaryID = /*NOXLATE*/L"",
                               bool bDeleteIfOwned = true )
        throw( DWFException );

    ///
    ///         Merge the multiple contents into the primary content. This will match
    ///         the content element UUIDs and drop duplicate elements. If there is only one
    ///         content this does nothing.
    ///
    ///         This will destroy all other contents in the content manager.
    ///
    ///\param   bPrimaryHasPriority If priority is given to the primary content, in the case
    ///                             of duplicates the elements coming in from other contents
    ///                             will be dropped. If this is false, the content element being
    ///                             merged in will be kept.
    ///\throw   None
    ///
    _DWFTK_API
    void mergeContent( bool bPrimaryHasPriority = true )
        throw();

    ///
    ///         Get the DWFUUID to get a new ID.
    ///
    ///\return  Pointer to the DWFUUID used by this content manager.
    ///\throw   None
    ///
    DWFUUID* getIDProvider() const
        throw()
    {
        return _pUUIDProvider;
    }

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

    ///
    ///\copydoc DWFCore::DWFOwner::notifyOwnableDeletion()
    ///
    _DWFTK_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

private:

    typedef std::map<DWFString, DWFContent*>        _tResourceToContentMap;

private:

    //
    //  One of the content objects will be the primary object of the package.
    //  During a merge, all content gets merged into this.
    //
    DWFContent*                         _pPrimaryContent;

    //
    //  The list of contents that are managed by this class. This is the main
    //  container of the contents. This maps the content GUIDs to the contents.
    //
    DWFContent::tMap                    _oContent;

    //
    // This will provide the UUIDs required to create new items in the content
    // The content will ask for this ID provider wherever it is needed.
    //
    DWFUUID*                            _pUUIDProvider;

    //
    // This is used during the load of a DWF package with content libraries
    //
    DWFPackageReader*                   _pPackageReader;

private:

    DWFContentManager( const DWFContentManager& );
    DWFContentManager& operator=( const DWFContentManager& );

};

}

#endif

