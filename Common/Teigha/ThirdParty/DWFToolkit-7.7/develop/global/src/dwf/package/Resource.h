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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Resource.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_RESOURCE_H
#define _DWFTK_RESOURCE_H

///
///\file        dwf/package/Resource.h
///\brief       This file contains the DWFResource class declaration.
///


#include "dwfcore/STL.h"
#include "dwfcore/Owner.h"
#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
#include "dwfcore/InputStream.h"
#include "dwfcore/ZipFileDescriptor.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/writer/PackageWriter.h"
#include "dwf/package/utility/PropertyContainer.h"

namespace DWFToolkit
{

//forward decl
class DWFResource;

///
///\class   DWFResourceRelationship     dwf/package/Resource.h     "dwf/package/Resource.h"
///\brief   Resource to resource relationship.
///\since       7.3
///
///         This class defines relationships between DWFResource objects. A relationship
///         is specified by a pointer to a DWFResource object and a string describing the
///         type of the relationship.
///
class DWFResourceRelationship
    : public DWFXMLBuildable
    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
public:
    ///
    ///\brief   This type defines a list of Relationship pointers.
    ///
    typedef DWFOrderedVector<DWFResourceRelationship*> tList;

public:
    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read resource content
    ///         data from the DWF package.
    ///
    ///\param   pPackageReader  Provides access to resource content in the DWF package.
    ///\throw   None
    ///
    DWFResourceRelationship(DWFPackageReader* /*pPackageReader*/)
        throw()
        : _pResource(NULL)
        , _zResourceID()
        , _zType()
    { ; }

    ///
    ///         Constructor
    ///
    ///
    ///\param   pResource       A to the related resource.
    ///\param   zType           A description of the relationship. 
    ///                         
    ///\throw   None
    ///
    DWFResourceRelationship(const DWFResource* pResource, const DWFString& zType)
        throw()
        : _pResource(pResource)
        , _zResourceID()
        , _zType(zType)
    { ; }

    ///
    /// Destructor
    ///
    virtual ~DWFResourceRelationship()
        throw()
    { ; }

    ///
    ///         Returns the object ID of the related resource held by this relationship.
    ///
    ///         An empty string will be returned if the resource does not yet have an object ID.
    ///
    ///\return  Returns the object ID of the related resource held by this relationship.
    ///\throw   DWFException - if no resource/objectID was set.
    ///
    _DWFTK_API
    const DWFString& resourceID() const
        throw(DWFException);

    ///
    ///         Returns the description of this relationship.
    ///
    ///\return  Returns the description of this relationship.
    ///\throw   None
    ///
    const DWFString& type() const
        throw()
    {
        return _zType;
    }

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );


private:
    const DWFResource*  _pResource;
    DWFString           _zResourceID;
    DWFString           _zType;

private:
    // Intentionally not implemented.
    DWFResourceRelationship( const DWFResourceRelationship& );
    DWFResourceRelationship& operator=( const DWFResourceRelationship& );
};


///
///\ingroup     dwfpackage
///
///\class       DWFResource   dwf/package/Resource.h     "dwf/package/Resource.h"
///\brief       This class contains the base functionality for all resources in DWF section.
///\since       7.0.1
///
///             A resource is the common encapsulation of a set of data within a section.
///
class DWFResource : public DWFXMLBuildable

#ifndef DWFTK_READ_ONLY
                  , public DWFXMLSerializable
#endif
                  , public DWFPropertyContainer
                  , public DWFOwnable
                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFResource pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFResource*)                                     tList;
    ///
    ///\brief   This type defines an integer ordered list of DWFResource pointers.
    ///
    typedef DWFSkipList<uint32_t, DWFResource*>                                 tOrderedList;
    ///
    ///\brief   This type defines a mapped collection of DWFResource pointers.
    ///
    typedef DWFWCharKeySkipList<DWFResource*>                                   tMap;
    ///
    ///\brief   This type defines a multi-value mapped collection of DWFResource pointers.
    ///
    typedef std::multimap<const wchar_t*, DWFResource*, tDWFWCharCompareLess>   tMultiMap;
    ///
    ///\brief   This type defines an iterator over a collection of DWFStrings.
    ///
    typedef DWFIterator<DWFString>                                              tStringIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of DWFResource pointers.
    ///
    typedef DWFIterator<DWFResource*>                                           tIterator;
    ///
    ///\brief   This type defines a basic const iterator on a collection of DWFResource pointers.
    ///
    typedef DWFConstIterator<DWFResource*>                                      tConstIterator;
    ///
    ///\brief   This type defines a caching iterator on a collection of DWFResource pointers.
    ///
    typedef DWFCachingIterator<DWFResource*>                                    tCachingIterator;

public:
    ///
    ///\ingroup     dwfpackage
    ///
    ///\class       NotificationSink   dwf/package/Resource.h     "dwf/package/Resource.h"
    ///\brief       Base implementation and interface definition to get information about changes to attributes of DWFResources.
    ///\since       7.4.0
    ///
    class NotificationSink _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
    {

    public:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        NotificationSink()
            throw()
        {;}

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~NotificationSink()
            throw()
        {;}

    public:

        ///
        ///         The callback derived classes should implement to get notified just before a resource 
        ///         href is being changed.
        ///
        ///\param   pResource          A pointer to the resource that is changing.
        ///\throw   None
        ///
        _DWFTK_API
        virtual void onBeforeResourceHRefChanged( DWFResource* pResource )
            throw()
            = 0;

        ///
        ///         The callback derived classes should implement to get notified just after a resource 
        ///         href has being changed.
        ///
        ///\param   pResource          A pointer to the resource that is changing.
        ///\throw   None
        ///
        _DWFTK_API
        virtual void onAfterResourceHRefChanged( DWFResource* pResource )
            throw()
            = 0;
    };

private:
    typedef _DWFTK_STD_VECTOR(NotificationSink*)    _tNotifySinks;


public:
    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read resource content
    ///         data from the DWF package.
    ///
    ///\param   pPackageReader  Provides access to resource content in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    DWFResource( DWFPackageReader* pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read resource content
    ///         data from the DWF package.
    ///
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   zRole           A functional description of the resource; 
    ///                         see the DWFToolkit::DWFXML class for the well-known roles.
    ///\param   zMIME           The MIME type of the resource data; see the DWFCore::DWFMIME
    ///                         class for well-known types.
    ///\param   zHREF           The location of the resource within the DWF package layout.
    ///                         This parameter should not be set manually by client applications
    ///                         in most cases.  The resource location is usually a composition
    ///                         of the section name and resource object ID which is assigned at publish time.
    ///                         
    ///\throw   None
    ///
    _DWFTK_API
    DWFResource( const DWFString& zTitle,
                 const DWFString& zRole,
                 const DWFString& zMIME,
                 const DWFString& zHREF = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFResource()
        throw();

    ///
    ///         Returns the resource title.
    ///
    ///\return  The title.
    ///\throw   None
    ///
    const DWFString& title() const
        throw()
    {
        return _zTitle;
    }

    ///
    ///         Assigns the resource title.
    ///
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\throw   None
    ///
    void setTitle( const DWFString& zTitle )
        throw()
    {
        _zTitle = zTitle;
    }

    ///
    ///         Returns the resource role.
    ///
    ///\return  The role.
    ///\throw   None
    ///
    const DWFString& role() const
        throw()
    {
        return _zRole;
    }

    ///
    ///         Assigns the resource role.
    ///
    ///\param   zRole           A functional description of the resource; 
    ///                         see the DWFToolkit::DWFXML class for the well-known roles.
    ///\throw   None
    ///
    void setRole( const DWFString& zRole )
        throw()
    {
        _zRole = zRole;
    }

    ///
    ///         Returns the resource MIME type.
    ///
    ///\return  The MIME type.
    ///\throw   None
    ///
    const DWFString& mime() const
        throw()
    {
        return _zMIME;
    }

    ///
    ///         Assigns the resource MIME type.
    ///
    ///\param   zMIME           The MIME type of the resource data; see the DWFCore::DWFMIME
    ///                         class for well-known types.
    ///\throw   None
    ///
    _DWFTK_API
    void setMIME( const DWFString& zMIME )
        throw();

    ///
    ///         Returns the resource HREF.
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
    ///         Add this content ID to the list of IDs of content associated with the resource. 
    ///         Certain resource, for example the content presentation resources, may be associated 
    ///         with more than one global content. For example a presentation may reference
    ///         objects from two different global contents. 
    ///         Derived resources may dissallow multiple contents by overriding this method.
    ///
    ///\param   zContentID      The content ID.
    ///\return  True if the ID was added, false if the ID already exists and hence was not added.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual bool addContentID( const DWFString& zContentID )
        throw( DWFException );

    ///
    ///         Remove this content ID from the list of IDs of content associated with the resource. 
    ///
    ///\param   zContentID      The content ID.
    ///\return  Return true if the ID was found and removed.
    ///\throw   None
    ///
    bool removeContentID( const DWFString& zContentID )
        throw()
    {
        return _oContentID.erase( zContentID );
    }

    ///
    ///         Returns the number content ID associated with this resource
    ///
    ///\return  The number of associated content IDs.
    ///\throw   None
    ///
    ///\since 7.2.0
    ///
    size_t contentIDCount() const
        throw()
    {
        return _oContentID.size();
    }

    ///
    ///         Access all associated content IDs using an iterator.
    ///
    ///\return  An iterator over the content IDs. The caller owns the iterator and is responsible for 
    ///         releasing it with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    ///\since 7.2.0
    ///
    tStringIterator* contentIDs()
        throw()
    {
        return _oContentID.iterator();
    }

    ///
    ///         Returns the resource object ID.
    ///
    ///\return  The object ID.
    ///\throw   None
    ///
    const DWFString& objectID() const
        throw()
    {
        return _zObjectID;
    }

    ///
    ///         Sets the resource object ID.
    ///
    ///\param   zObjectID   A unique identifier.
    ///\throw   None
    ///
    virtual void setObjectID( const DWFString& zObjectID )
        throw()
    {
        _zObjectID = zObjectID;
    }

    ///
    ///\internal    Returns the resource's internal ID. These are unique only at the package level and are
    ///             reset every time during serialization.
    ///
    ///\return      The internal ID.
    ///\throw       None
    ///\since       7.4.0
    ///
    const DWFString& internalID() const
        throw()
    {
        return _zInternalID;
    }

    ///
    ///\internal    Set the resource's internal ID. This has to be a hyperlink compatible 
    ///             string - so it must conform to the specification for xsd:ID. The ID is
    ///             reset during serialization and is unique only for the package.
    ///
    ///\param       zInternalID     A hyperlink compatible string - must conform to the specification for xsd:ID.
    ///\throw       None
    ///\since       7.4.0
    ///
    virtual void setInternalID( const DWFString& zInternalID )
        throw()
    {
        _zInternalID = zInternalID;
    }

    ///
    ///         Returns the object ID of the parent resource.
    ///
    ///\return  The parent object ID.
    ///\throw   None
    ///
    const DWFString& parentID() const
        throw()
    {
        return _zParentObjectID;
    }

    ///
    ///         Defines a relationship between two resources.
    ///
    ///         This relationship is required when the
    ///         contents of the child resource are directly applicable
    ///         to the parent.  One example of this is object definition
    ///         resources as children of the graphic resource that define
    ///         the entities to which the instances are bound.
    ///
    ///\param   pResource   The parent resource or NULL to delete an existing relationship.
    ///\throw   None
    ///
    _DWFTK_API
    void setParentResource( const DWFResource* pResource )
        throw();

    ///
    ///         Defines a relationship between two resources.
    ///
    ///         This relationship is required when the
    ///         contents of the child resource are directly applicable
    ///         to the parent.  One example of this is object definition
    ///         resources as children of the graphic resource that define
    ///         the entities to which the instances are bound.
    ///
    ///\param   zObjectID   The object ID of the parent resource.
    ///\throw   None
    ///
    void setParentObjectID( const DWFString& zObjectID )
        throw()
    {
        _zParentObjectID = zObjectID;
    }

    ///
    ///         Assigns the resource location.
    ///
    ///\param   zHRef           The location of the resource within the DWF package layout.
    ///                         This parameter should not be set manually by client applications
    ///                         in most cases.
    ///\throw   None
    ///
    void setHRef( const DWFString& zHRef )
        throw()
    {
        notifyBeforeHRefChanged();
        _zHRef = zHRef;
        notifyAfterHRefChanged();
    }

    ///
    ///         Assigns the target resource location.
    ///
    ///\param   zTargetHRef     The intended location of the resource within the DWF package layout.
    ///                         This parameter should not be set manually by client applications
    ///                         in most cases.
    ///\throw   None
    ///
    void setTargetHRef( const DWFString& zTargetHRef )
        throw()
    {
        _zTargetHRef = zTargetHRef;
    }

    ///
    ///         Returns the total uncompressed byte count of the resource data.
    ///
    ///         When a resource is being created for the purpose of serialization and if either
    ///         no input stream has been provided or if no size was specified when providing the
    ///         input stream then this method will return 0 since the stream will be queried for
    ///         its size just before serialization.
    ///         
    ///
    ///\return  The number of bytes.
    ///\throw   None
    ///
    const size_t size() const
        throw()
    {
        return _nSize;
    }

    ///
    ///\internal    This method is used during the publishing process
    ///             to construct a valid destination HREF for the resource.
    ///
    ///\param       zSectionName    The unique section identifier.
    ///\param       zObjectID       The object ID that will be used in the HREF.
    ///                             If the object ID of this resource is empty when this
    ///                             function call is made, it will be assigned.
    ///\throw       None
    ///
    _DWFTK_API
    virtual void setPublishedIdentity( const DWFString& zSectionName,
                                       const DWFString& zObjectID )
        throw();

    ///
    ///\internal    Returns the publishing target HREF for the resource.
    ///             This is used for the DWF6 package publishing.
    ///
    ///             This method is used during publishing by DWFPackageWriter::write()
    ///             to discover the final HREF of the resource in the DWF package.
    ///             This is different than the "standard" HREF attribute since the 
    ///             package writer needs to recharacterize and identify sections
    ///             as they are written out; thus, the final resource location will
    ///             change.  The original property is not modified since the resource
    ///             may be used elsewhere; the package writer does not assume that it
    ///             is the final user of the object.
    ///
    ///\return      The final published location of the resource.
    ///\throw       None
    ///
    const DWFString& publishedIdentity() const
        throw()
    {
        return _zTargetHRef;
    }

    ///
    ///         To avoid automatically renaming the resource during every publish, we
    ///         can request that this name to be set for the published identity.
    ///         \b Note: The argument is only the base name without the path. It may
    ///         include the extension.
    ///
    ///\param   zName   The name to set for the resource - this excludes the path or the
    ///                 extension.
    ///\throw   None
    ///\since   7.4.0
    ///
    virtual void setRequestedName( const DWFString& zName )
        throw()
    {
        _zRequestedIdentity = zName;
    }

    ///
    ///         Return the requested name for the resource.
    ///
    ///\return  The requested name.
    ///\throw   None
    ///\since   7.4.0
    ///
    virtual const DWFString& requestedName()
        throw()
    {
        return _zRequestedIdentity;
    }

    ///
    ///         Provides a stream for reading the resource data.
    ///
    ///\warning If an input stream is bound to the resource using
    ///         setInputStream() for publishing, this method will
    ///         return that stream pointer.  This is very important
    ///         to realize: the caller must delete that stream pointer
    ///         by contract of this interface, as such, this method
    ///         will return a bogus pointer from that point on.  
    ///         <b>Do not call this method more than once if setInputStream()
    ///         binds the stream pointer.</b>
    ///
    ///
    ///\param   bCache  If \e true, the uncompressed resource file
    ///                 will be stored in a temporary disk file
    ///                 for faster subsequent access.
    ///
    ///\return  A pointer to a data stream.
    ///         The caller is responsible for releasing the pointer
    ///         with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFInputStream* getInputStream( bool bCache = false )
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );


#ifndef DWFTK_READ_ONLY

    ///
    ///         Binds a data source stream for providing the resource data.
    ///
    ///         This method is for use when publishing a resource.
    ///         It is assumed that the source stream (provided here) will
    ///         be read once and discarded.
    ///
    ///\warning This function is provided to make the initial connection
    ///         between source data and the DWF package I/O data object -
    ///         the DWFResource.  That is, it is a <b>publish-time only</b>
    ///         method.  This is important to understand since the first
    ///         call to getInputStream() will return this pointer and the caller
    ///         is obligated to delete it.  
    ///
    ///\todo    A future revision of this class should address the potential
    ///         inconsistency in this function pair; possibly introducing
    ///         a stream provider/builder object that is given to the resource
    ///         instead of the stream interface itself. This mediator could
    ///         ensure the availability of new stream object with all data content
    ///         for every call to getInputStream().
    ///
    ///\param   pStream     A pointer to a data stream.  This object
    ///                     must be allocated with the \b DWFCORE_ALLOC_OBJECT macro.
    ///\param   nBytes      Represents the total size (in uncompressed bytes)
    ///                     of the resource.  If \e zero, the number of available
    ///                     bytes in the stream initially will be used; however,
    ///                     there is no guarantee that this amount will always reflect
    ///                     the true size of the source content.
    ///
    _DWFTK_API
    void setInputStream( DWFInputStream* pStream, size_t nBytes = 0 )
        throw( DWFException );

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

    ///
    ///         Get the custom zip mode of the resource. This is meaningful only if
    ///         \a useCustomZipMode returns true.
    ///
    ///\return  The zip mode that should be use to serialize this resource.
    ///\throw   None
    ///\since   7.3.0
    ///
    DWFZipFileDescriptor::teFileMode customZipMode()
        throw()
    {
        return _eCustomZipMode;
    }

    ///
    ///         Determines whether or not to use a custom zip mode to serialize the resource.
    ///
    ///\return  Returns true if the custom zip mode should be use to serialize this resource.
    ///\throw   None
    ///\since   7.3.0
    ///
    bool useCustomZipMode()
        throw()
    {
        return _bUseCustomZipMode;
    }

    ///
    ///         Set whether or not to use a custom zip mode to serialize the resource. If false, the
    ///         second argument will be ignored.
    ///
    ///\param   bUseCustomZipMode   True if the custom zip mode should be use to serialize this resource.
    ///\param   eZipMode            If enabling custom zip mode, this sets the mode to use.
    ///\throw   None
    ///\since   7.3.0
    ///
    _DWFTK_API
    void enableCustomZipMode( bool bUseCustomZipMode, DWFZipFileDescriptor::teFileMode eZipMode = DWFZipFileDescriptor::eZip )
        throw();

    ///
    ///         Add a relationship between this resource and another resource.
    ///
    ///\param   pResourceRel       A pointer to the relationship object to be added.
    ///                         
    ///\throw   DWFException
    ///\since   7.4.0
    ///
    _DWFTK_API
    void addRelationship( DWFResourceRelationship* pResourceRel)
        throw(DWFException);

    ///
    ///         Add a relationship between this resource and another resource.
    ///
    ///\param   pResource       A pointer to the related resource.
    ///\param   zType           A description of the relationship. 
    ///                         
    ///\throw   DWFException
    ///\since   7.4.0
    ///
    _DWFTK_API
    void addRelationship( const DWFResource* pResource, const DWFString& zType)
        throw(DWFException);

    ///
    ///         Return an iterator of all current relationships from this resource.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///\since   7.4.0
    ///
    DWFResourceRelationship::tList::ConstIterator* getRelationships()
        throw(DWFException)
    {
        return _oRelationships.constIterator();
    }

    ///
    ///         Remove a relationship with another resource
    ///
    ///\param   pResourceRel    a pointer to the resource relationship that will be removed
    ///\param   bDelete         specify wether to free the relationship object
    ///
    ///\throw   DWFException
    ///\since   7.4.0
    ///
    _DWFTK_API
    void removeRelationship( DWFResourceRelationship* pResourceRel, bool bDelete = false )
        throw( DWFException );

    ///         Add a NotificationSink to listen to changes on this resource.
    ///
    ///\param   pSink       A DWFResource::NotificationSink derived class
    ///\throw   none
    ///\since   7.4.0
    ///
    _DWFTK_API
    void setNotificationSink( DWFResource::NotificationSink* pSink )
        throw();

    ///
    ///         Remove a specific notification sink.
    ///
    ///\param   pSink       The sink to remove.
    ///\throw   None
    ///\since   7.4.0
    ///
    _DWFTK_API
    void removeNotificationSink( DWFResource::NotificationSink* pSink )
        throw();

    ///
    ///         Inform all NotificationSink's that the href of this resource is about to change.
    ///
    ///\throw   none
    ///\since   7.4.0
    ///
    _DWFTK_API
    void notifyBeforeHRefChanged()
        throw();

    ///
    ///         Inform all NotificationSink's that the href of this resource has changed.
    ///
    ///\throw   none
    ///\since   7.4.0
    ///
    _DWFTK_API
    void notifyAfterHRefChanged()
        throw();

private:

    _DWFTK_API
    tStringIterator* _tokenizeBySpace( char* szInput )
        throw();

protected:

    ///
    ///     The total number of uncompressed bytes in the resource.
    ///
    size_t _nSize;
    DWFString _zTargetHRef;

    DWFOrderedVector<DWFString> _oContentID;

    DWFResourceRelationship::tList _oRelationships;

    DWFPackageReader*   _pPackageReader;
    //
    bool        _bDWFXProtected;


private:

    DWFString _zTitle;
    DWFString _zRole;
    DWFString _zMIME;
    DWFString _zHRef;
    DWFString _zObjectID;
    DWFString _zParentObjectID;

    DWFString _zRequestedIdentity;

    //
    //  This is used for DWFXFixedPages - to allow identification of embedded resources by looking at the 
    //  Name attribute of the top-level Canvas and ImageBrush elements.
    //
    DWFString _zInternalID;

    //
    //  The custom zip mode to use for the resource - meaningful only if _bUseCustomZipMode is true.
    //
    DWFZipFileDescriptor::teFileMode    _eCustomZipMode;
    //
    //  If true, use the zip mode defined in the resource to serialize it.
    //
    bool                                _bUseCustomZipMode;
    //
    //

    DWFInputStream*     _pStream;


    //
    //  The notification sinks are used to notify an observer/listener of
    //  resource modification events.
    //
    _tNotifySinks                       _oNotifySinks;

private:

    DWFResource( const DWFResource& );
    DWFResource& operator=( const DWFResource& );
};

}

#endif
