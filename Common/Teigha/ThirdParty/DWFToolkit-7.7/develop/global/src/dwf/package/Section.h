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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Section.h#2 $
//  $DateTime: 2011/10/11 01:26:00 $
//  $Author: caos $
//  $Change: 229047 $
//  $Revision: #2 $
//


#ifndef _DWFTK_SECTION_H
#define _DWFTK_SECTION_H


///
///\file        dwf/package/Section.h
///\brief       This file contains the DWFSection class declaration.
///


#include "dwfcore/STL.h"
#include "dwfcore/Owner.h"
#include "dwfcore/SkipList.h"
#include "dwfcore/XMLParser.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/Source.h"
#include "dwf/package/Content.h"
#include "dwf/package/Resource.h"
#include "dwf/package/Interface.h"
#include "dwf/package/ObjectDefinition.h"
#include "dwf/package/ObjectDefinitionResource.h"
#include "dwf/package/writer/PackageWriter.h"
#include "dwf/package/reader/PackageReader.h"
#include "dwf/package/reader/ObjectDefinitionReader.h"
#include "dwf/presentation/reader/ContentPresentationReader.h"
#include "dwf/package/reader/SectionDescriptorReader.h"
#include "dwf/package/utility/ResourceContainer.h"
#include "dwf/package/utility/PropertyContainer.h"
#include "dwf/package/utility/DefinedObjectContainer.h"
#include "dwf/presentation/ContentPresentation.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFSection   dwf/package/Section.h     "dwf/package/Section.h"
///\brief       This class contains the base functionality for all sections in a DWF package.
///\since       7.0.1
///
///             <b>Section Descriptors</b>\par
///             While the package manifest outlines general section information,
///             it is the section's descriptor that provides the details.
///             The section classes in the toolkit are structured for highly
///             efficient processing.  As such, the descriptors are never 
///             automatically loaded.  On-demand descriptor parsing allows for
///             the most flexibility when processing an entire DWF package.
///             That said, it is very strongly recommended that a section's descriptor
///             is read into the DWFSection object before using it's data and resources;
///             especially when copying or moving resources between sections.
///
class DWFSection : public    DWFXMLBuildable
#ifndef DWFTK_READ_ONLY
                 , public    DWFXMLSerializable
#endif
                 , public    DWFOwnable
                 , public    DWFResourceContainer
                 , public    DWFPropertyContainer
                 , protected DWFXMLElementBuilder
                 _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\struct  tBehavior   dwf/package/Section.h     "dwf/package/Section.h"
    ///\brief   This data structure encapsulates certain logic required for the section
    ///         to be processed correctly by the toolkit.
    ///
    typedef struct tBehavior
    {
        ///
        ///     If \e true, the section name will be modified
        ///     according to the current rules applied by
        ///     the DWFPackageWriter.
        ///     By default, this is \e true.
        ///
        bool    bRenameOnPublish;

        ///
        ///     If \e true, the publishing logic of the toolkit
        ///     will treat this section as a standard DWF section
        ///     and apply its particular rules for section descriptors.
        ///     By default, this is \e true.
        ///
        bool    bPublishDescriptor;

        //
        // if true, the publishing logic of the toolkit
        // will treat this section as a standard DWF section
        // with respect to it's package documentation, in particular,
        // the manifest will include a list of the sections resources.
        // by default, this is true
        //
        bool    bPublishResourcesToManifest;

        tBehavior( bool bRenameOnPub = true, 
                   bool bPubDescriptor = true,
                   bool bPublishResourcesToManifest = true )
                 : bRenameOnPublish( bRenameOnPub )
                 , bPublishDescriptor( bPubDescriptor )
                 , bPublishResourcesToManifest( bPublishResourcesToManifest )
        {}

    } tBehavior;

    ///
    ///\brief   This type defines a list of DWFSection pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFSection*)  tList;
    ///
    ///\brief   This type defines a mapped collection of DWFSection pointers.
    ///
    typedef DWFWCharKeySkipList<DWFSection*>                                    tMap;
    ///
    ///\brief   This type defines a multi-value mapped collection of DWFSection pointers.
    ///
    typedef std::multimap<const wchar_t*, DWFSection*, tDWFWCharCompareLess>    tMultiMap;
    ///
    ///\brief   This type defines a map of strings to strings
    ///
    typedef std::map<DWFString, DWFString>                                      tStringMap;
    ///
    ///\brief   This type defines a multi-map of strings to strings
    ///
    typedef _DWFTK_STD_MULTIMAP(DWFString, DWFString) tStringMultiMap;

public:

    ///
    ///\class   Factory     dwf/package/Section.h     "dwf/package/Section.h"
    ///\brief   Class factory for DWFSection objects.
    ///\since       7.0.1
    ///
    ///         Section factories are used by the DWFSectionBuilder.
    ///         Any custom sections that require creation by the DWFPackageReader
    ///         must provide a section factory.
    ///
    class Factory _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
    {

    public:

        ///
        ///\brief   This type defines a list of DWFSection::Factory pointers.
        ///
        typedef _DWFTK_STD_VECTOR(DWFSection::Factory*)     tList;
        ///
        ///\brief   This type defines a mapped collection of DWFSection::Factory pointers.
        ///
        typedef DWFWCharKeySkipList<DWFSection::Factory*>   tMap;

    public:

        ///
        ///         Constructor
        ///
        ///\param   zType   The section type.
        ///\throw   None
        ///
        _DWFTK_API
        Factory( const DWFString& zType )
            throw();

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~Factory()
            throw();

        ///
        ///         The type of section this factory builds.
        ///
        ///\return  The section type.
        ///\throw   None
        ///
        _DWFTK_API
        const DWFString& type() const
            throw();

        ///
        ///         Create a new section.
        ///
        ///         This creation method is generally only used by the 
        ///         parsing process when the DWFPackageReader associated with 
        ///         (and providing read access to) the DWF package file is available.
        ///         The subsequent binding makes it possible to read section content
        ///         data from the DWF package.
        ///         
        ///\param   zName           A string that uniquely identifies the section in the DWF package.
        ///\param   zTitle          The descriptive and display friendly title text.
        ///\param   pPackageReader  A package reader to bind to the section.
        ///\throw   DWFException
        ///
        _DWFTK_API
        virtual DWFSection* build( const DWFString&     zName,
                                   const DWFString&     zTitle,
                                   DWFPackageReader*    pPackageReader )
            throw( DWFException );

        ///
        ///         Create a new section.
        ///
        ///         This creation method is generally only used by the 
        ///         parsing process when the DWFPackageReader associated with 
        ///         (and providing read access to) the DWF package file is available.
        ///         The subsequent binding makes it possible to read section content
        ///         data from the DWF package.
        ///         
        ///\param   zType           The section type identifier.
        ///\param   zName           A string that uniquely identifies the section in the DWF package.
        ///\param   zTitle          The descriptive and display friendly title text.
        ///\param   pPackageReader  A package reader to bind to the section.
        ///\throw   DWFException
        ///
        _DWFTK_API
        virtual DWFSection* build( const DWFString&     zType,
                                   const DWFString&     zName,
                                   const DWFString&     zTitle,
                                   DWFPackageReader*    pPackageReader )
            throw( DWFException );

    private:

        DWFString _zType;
    };

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read section content
    ///         data from the DWF package.
    ///
    ///\param   pPackageReader  Provides access to section content in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSection( DWFPackageReader* pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read section content
    ///         data from the DWF package.
    ///
    ///\param   zType           The section type identifier.
    ///\param   zName           A string that uniquely identifies the section in the DWF package.
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   pPackageReader  Provides access to section content in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSection( const DWFString&  zType,
                const DWFString&  zName,
                const DWFString&  zTitle,
                DWFPackageReader* pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is generally used by applications and publishers for
    ///         creating new section objects.
    ///
    ///\param   zType           The section type identifier.
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   zObjectID       Uniquely identifies the section.
    ///\param   nVersion        Identifies the version of the section type and/or descriptor.
    ///\param   nPlotOrder      Indicates the initial position in the package to which the section was/will be published.
    ///\param   rSource         Describes the original source of the data in the section (a drawing file, for example.)
    ///\throw   None
    ///
    _DWFTK_API
    DWFSection( const DWFString& zType,
                const DWFString& zTitle,
                const DWFString& zObjectID,
                double           nVersion,
                double           nPlotOrder,
                const DWFSource& rSource )
        throw();

    ///
    ///         Copy Constructor
    ///
    ///\param   rSection    The source from which to construct this object.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSection( const DWFSection& rSection)
        throw();

    ///
    ///         Assignment Operator
    ///
    ///\param   rSection    The source from which to construct this object.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSection& operator=( const DWFSection& rSection)
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSection()
        throw();

    ///
    ///         Returns the section name.
    ///
    ///         The section name refers to the string that uniquely identifies the section in the DWF package.
    ///         The \a title() method should be used to retrieve the descriptive and
    ///         display friendly text title of the section.
    ///
    ///\return  A string that uniquely identifies the section in the DWF package.
    ///\throw   None
    ///
    virtual const DWFString& name() const
        throw()
    {
        return _zName;
    }

    ///
    ///         Change the section name.
    ///
    ///         This method is generally used during the internal publishing process
    ///         in DWFPackageWriter::addSection() to ensure uniqueness.
    ///
    ///         The section name refers to the string that uniquely identifies the section in the DWF package.
    ///         Usually this name is a string representation of a UUID. 
    ///         The \a title() method should be used to retrieve the descriptive and
    ///         display friendly text title of the section.
    ///         
    ///\param   zName   A string that uniquely identifies the section in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void rename( const DWFString& zName )
        throw();

    ///
    ///         Returns the section type.
    ///
    ///\return  The section type identifier.
    ///\throw   None
    ///
    const DWFString& type() const
        throw()
    {
        return _zType;
    }

    ///
    ///         Returns the section title.
    ///
    ///\return  The descriptive and display friendly title text.
    ///\throw   None
    ///
    virtual const DWFString& title() const
        throw()
    {
        return _zTitle;
    }

    ///
    ///         Change the section title.
    ///
    ///\param   zTitle   A string for the new descriptive and display friendly title text.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual void retitle( const DWFString& zTitle )
        throw()
    {
        _zTitle = zTitle;
    }

    ///
    ///         Sets the section label.
    ///
    ///\param   zLabel   A string containing the label.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual void setLabel( const DWFString& zLabel )
        throw()
    {
        _zLabel = zLabel;
    }

    ///
    ///         Gets the section label.
    ///
    ///\return  A string containing the label.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual const DWFString& label()
        throw()
    {
        return _zLabel;
    }

    ///
    ///         Sets the resource containing an icon for the section label.
    ///
    ///\param   rResource       The resource containing the icon.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual void setLabelIconResource( DWFResource& rResource )
        throw()
    {
        _pLabelIconResource = &rResource;
    }

    ///
    ///         Sets the objectid of the resource containing an icon for the section label.
    ///
    ///\param   zResourceID   A string containing the objectid of the resource.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual void setLabelIconResourceID( const DWFString& zResourceID )
        throw()
    {
        _zLabelIconResourceID = zResourceID;
    }

    ///
    ///         Gets the objectid of the resource containing an icon for the section label.
    ///
    ///\return  A string containing the objectid of the resource containing an icon for the section label.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual const DWFString& labelIconResourceID()
        throw()
    {
        return _zLabelIconResourceID;
    }

    ///
    ///         Gets the initial URI.
    ///
    ///\return  A string containing the URI to be applied after the section is loaded.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual const DWFString& initialURI()
        throw()
    {
        return _zInitialURI;
    }

    ///
    ///         Sets the initial URI.
    ///
    ///\param   zInitialURI       A string containing the URI to be applied after the section is loaded.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual void setInitialURI( const DWFString& zInitialURI )
        throw()
    {
        _zInitialURI = zInitialURI;
    }

    ///
    ///         Returns an object that can be used to identify
    ///         the source of the original data from which the
    ///         section content was created.
    ///
    ///         Source will always exist but may contain empty elements.
    ///
    ///\return  The source object.
    ///\throw   None
    ///
    virtual const DWFSource& source() const
        throw()
    {
        return _oSource;
    }

    ///
    ///         Identifies/updates the source information for this section.
    ///
    ///         This method will overwrite any existing data.
    ///
    ///\param   rSource     The source object from which to copy data.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void addSource( const DWFSource& rSource )
        throw();

    ///
    ///         Returns the interface associated with the section type.
    ///
    ///         An interface identifier is used in the DWF package manifest
    ///         for quick reference, to document all section types in the package.
    ///         
    ///\return  A pointer to a new interface object.  This object will be allocated
    ///         using the \b DWFCORE_ALLOC_OBJECT macro and must be deleted by the
    ///         caller using the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    virtual DWFInterface* buildInterface()
        throw( DWFException )
    {
        return NULL;
    }

    ///
    ///         Returns a unique identifier for the section.
    ///
    ///         This identifier should be immutable for the life of the section;
    ///         remaining unchanged during relocation and aggregation (the same
    ///         is not true of the \a name()).
    ///
    ///\return  A reference to the unique section identifier.
    ///\throw   None
    ///
    virtual const DWFString& objectID() const
        throw()
    {
        return _zObjectID;
    }

    ///
    ///         Returns the version of the section and/or descriptor.
    ///
    ///         Section types/classes should always be versioned to ensure
    ///         backwards compatability.
    ///
    ///\return  The version number.
    ///\throw   None
    ///
    virtual double version() const
        throw()
    {
        return _nVersion;
    }

    ///
    ///         Returns the order in which the section was published.
    ///
    ///\return  The plot order.
    ///\throw   None
    ///
    virtual double order() const
        throw()
    {
        return _nPlotOrder;
    }

    ///
    ///         Changes the plot order value.
    ///
    ///\param   nPlotOrder  The new value.
    ///\throw   None
    ///
    ///\note    Sections are always re-ordered by the DWFPackageWriter.
    ///
    virtual void reorder( double nPlotOrder )
        throw()
    {
        _nPlotOrder = nPlotOrder;
    }

    ///
    ///         Set the content manager from which the section's content resources can get their
    ///         contents during package reading.
    ///
    ///\param   pContentManager Pointer to the content manager.
    ///\throw   None
    ///
    virtual void setContentManager( DWFContentManager* pContentManager )
        throw()
    {
        _pContentManager = pContentManager;
    }

    ///
    ///         Get the content manager. This method is available for usage after a section
    ///         has been read in from a package. There is no gaurantee that this will be set
    ///         during publishing.
    ///
    ///\return  Pointer to the content manager. May be NULL.
    ///\throw   None
    ///
    virtual DWFContentManager* getContentManager() const
        throw()
    {
        return _pContentManager;
    }

    ///
    ///\copydoc DWFResourceContainer::addResource()
    ///
    _DWFTK_API
    virtual DWFResource* addResource( DWFResource*       pResource,
                                      bool               bOwnResource,
                                      bool               bReplace = true,
                                      bool               bDeleteReplacedIfOwned = true,
                                      const DWFResource* pParentResource = NULL )
        throw( DWFException );

    ///
    ///         Locates the one descriptor resource in the section
    ///         and parses it accordingly with the reader provided.
    ///
    ///\param   pSectionDescriptorReader    An optional interface pointer to an object that
    ///                                     knows how to read and parse the descriptor associated
    ///                                     with this section [type].  If NULL, this base implementation
    ///                                     will always throw an DWFInvalidArgumentException as there is
    ///                                     no generic parser.  
    ///
    ///\return  A reference to the descriptor document resource.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual
    const DWFResource& readDescriptor( DWFSectionDescriptorReader* pSectionDescriptorReader = NULL ) const
        throw( DWFException );

    ///
    ///         Processes the section descriptor using the reader interface provided.
    ///         This allows the caller to participate directly in the parsing process.
    ///         No in-memory data representation will be created nor cached by the DWFSection implicitly.
    ///
    ///\param   rSectionDescriptorReader    An interface reference to an object that knows how to read
    ///                                     and parse the descriptor associated with this section [type].
    ///\param   rResource                   The descriptor document resource for this section.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual 
    void readDescriptor( DWFSectionDescriptorReader& rSectionDescriptorReader,
                         DWFResource&                rResource ) const
        throw( DWFException );

    ///
    ///         Processes the section descriptor using the reader interface provided.
    ///         This allows the caller to participate directly in the parsing process.
    ///         No in-memory data representation will be created nor cached by the DWFSection implicitly.
    ///
    ///\param   rSectionDescriptorReader    An interface reference to an object that knows how to read
    ///                                     and parse the descriptor associated with this section [type].
    ///\param   rSectionDescriptorStream    A stream providing the descriptor document for this section.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual 
    void readDescriptor( DWFSectionDescriptorReader& rSectionDescriptorReader,
                         DWFInputStream&             rSectionDescriptorStream ) const
        throw( DWFException );

    ///
    ///         This parses the given section content resource using the default
    ///         content resource reader implemented by the associated content.
    ///         The resource must have a role of \b kzRole_ContentDefinition.
    ///         If a reader filter is provided, the filter will be set on the default reader.
    ///         By default for any resource the associated global content will be loaded
    ///         if it has not been. If \b bLoadContent is false, the content resource will
    ///         not be parsed if the content is not loaded.
    ///
    ///
    ///\param   pResource       Pointer to the content resource to read. 
    ///\param   pReaderFilter   A parsing filter. This will be set on the default content reader.
    ///\param   bLoadContent    If this is false the content resource will not load if the
    ///                         global content has not been loaded.
    ///\return  Pointer to content into which the resource was read. NULL if the read failed.
    ///         Do not delete the content pointer.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFContent* getContentDefinition( DWFResource* pResource, 
                                              DWFContentResourceReader* pReaderFilter = NULL,
                                              bool bLoadContent = true,
											  unsigned int nProviderFlags = DWFContentReader::eProvideAll)
        throw( DWFException );

    ///
    ///         This parses all given section content resources, i.e. resources with the 
    ///         the role of kzRole_ContentDefinition, using the default
    ///         content resource reader implemented by the associated contents.
    ///         If a reader filter is provided, the filter will be set on the default reader.
    ///         By default for any resource the associated global content will be loaded
    ///         if it has not been. If \b bLoadContent is false, the content resource will
    ///         not be parsed if the content is not loaded.
    ///
    ///\param   pReaderFilter   A parsing filter. This will be set on the default content reader.
    ///\param   bLoadContent    If this is false the content resource will not load if the
    ///                         global content has not been loaded.
    ///\return  Pointer to an iterator over content that was successfully read. 
    ///         The caller is reponsible for deleting this pointer with the 
    ///         \b DWFCORE_FREE_OBJECT macro. Do not delete the content pointers.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFContent::tIterator* getContentDefinition( DWFContentResourceReader* pReaderFilter = NULL,
                                                         bool bLoadContent = true,
														 unsigned int nProviderFlags = DWFContentReader::eProvideAll)
        throw( DWFException );

    ///
    ///         This parses all given section content resources, i.e. resources with the 
    ///         the role of kzRole_ContentDefinition, using the default
    ///         content resource reader implemented by the associated contents.
    ///         If a reader filter is provided, the filter will be set on the default reader.
    ///         By default for any resource the associated global content will be loaded
    ///         if it has not been. If \b bLoadContent is false, the content resource will
    ///         not be parsed if the content is not loaded. This method returns the loaded
    ///         contents in the provided map, where the key is content resource object ID, and
    ///         the value is the corresponding loaded content.
    ///
    ///\param   oResourceObjectIDContent    A mapping from resource object ID to the global content into
    ///                                     which the corresponding information was loaded.
    ///\param   pReaderFilter       A parsing filter. This will be set on the default content reader.
    ///\param   bLoadContent        If this is false the content resource will not load if the
    ///                             global content has not been loaded.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void getContentDefinition( DWFContent::tMap& oResourceObjectIDContent,
                                       DWFContentResourceReader* pReaderFilter = NULL,
                                       bool bLoadContent = true,
									   unsigned int nProviderFlags = DWFContentReader::eProvideAll )
        throw( DWFException );

    ///
    ///         This parses one or all content resources, i.e. resources with the 
    ///         the role of kzRole_ContentDefinition, in the section using the 
    ///         provided reader, allowing the caller to participate directly in the
    ///         parsing process. No in-memory representation of the content will be
    ///         created or cached by the DWFSection or the assocaited DWFContent.
    ///
    ///\param   rReader         The custom content resource reader.
    ///\param   pResource       Pointer to the content resource to read. If this is NULL all
    ///                         content resources in the section are read.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void getContentDefinition( DWFContentResourceReader& rReader,
                                       DWFResource* pResource = NULL )
        throw( DWFException );

    ///
    ///         Processes content definitions using the reader interface provided.
    ///         This allows the caller to participate directly in the parsing process.
    ///         No in-memory data representation will be created nor cached by the DWFSection implicitly.
    ///
    ///\param   rReader                     An interface reference to an object that knows how to read 
    ///                                     and parse content resource documents.
    ///\param   rContentResourceStream      A stream providing a content resource document for this section.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void getContentDefinition( DWFContentResourceReader& rReader,
                                       DWFInputStream& rContentResourceStream )
        throw( DWFException );

    ///
    ///         Get the IDs of the all contents associated with this section. This
    ///         does not load the global contents or any resources.
    ///
    ///\param   oResourceIDContentIDMap     The container in which the resource object IDs
    ///                                     and the associated content IDs are returned.
    ///
    _DWFTK_API
    virtual void getAssociatedContentIDs( tStringMultiMap& oResourceIDContentIDMap ) const
        throw();

    ///
    ///         If the content or the ID of the content associated with a resource changes,
    ///         then the section needs to be made aware of that change through this method.
    ///         This will update the resource, and then update the sections collection of
    ///         the resource object ID to content ID mapping.
    ///
    ///\param   zResourceObjectID           The object ID of the resource whose content ID is being updated.
    ///\param   zOldContentID               The old content ID that will be removed.
    ///\param   zNewContentID               The new content ID that will be inserted.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void updateResourceContentMapping( const DWFString& zResourceObjectID,
                                               const DWFString& zOldContentID,
                                               const DWFString& zNewContentID )
        throw( DWFException );

    ///
    ///         If the content or the ID of the content associated with a resource changes,
    ///         then the section needs to be made aware of that change through this method.
    ///         This will update the resource, and then update the sections collection of
    ///         the resource object ID to content ID mapping.
    ///
    ///\param   pResource                   The resource whose content ID is being updated.
    ///\param   zOldContentID               The old content ID that will be removed.
    ///\param   zNewContentID               The new content ID that will be inserted.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void updateResourceContentMapping( DWFResource* pResource,
                                               const DWFString& zOldContentID,
                                               const DWFString& zNewContentID )
        throw( DWFException );

    ///
    ///         Locates all object definitions in the section and
    ///         parses them with the default reader implemented by DWFObjectDefinition.
    ///
    ///\return  A pointer to a new DWFObjectDefinition.  The caller is reponsible for
    ///         deleting this pointer with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFObjectDefinition* getObjectDefinition(unsigned char nProviderFlags = DWFObjectDefinitionReader::eProvideAll) const
        throw( DWFException );

    ///
    ///         Processes the object definition resource provided using the default reader
    ///         implemented by DWFObjectDefinition.
    ///
    ///         rObjectDefinitionFilter     A parsing filter.  It will be set on the 
    ///                                     new DWFObjectDefinition before processing begins.
    ///
    ///         rResource                   An object definition resource to process.
    ///
    ///\return  A pointer to a new DWFObjectDefinition.  The caller is reponsible for
    ///         deleting this pointer with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFObjectDefinition* getObjectDefinition( DWFObjectDefinitionReader& rObjectDefinitionFilter,
                                                      DWFResource&               rResource ) const
        throw( DWFException );

    ///
    ///         Processes object definitions using the reader interface provided.
    ///         This allows the caller to participate directly in the parsing process.
    ///         No in-memory data representation will be created nor cached by the DWFSection implicitly.
    ///
    ///\param   rObjectDefinitionReader     An interface reference to an object that knows how to read 
    ///                                     and parse object definition documents.
    ///\param   pResource                   An optional pointer to an object definition resource.
    ///                                     If NULL, all object definition resources will be processed.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual 
    void getObjectDefinition( DWFObjectDefinitionReader& rObjectDefinitionReader,
                              DWFResource*               pResource = NULL ) const
        throw( DWFException );

    ///
    ///         Processes object definitions using the reader interface provided.
    ///         This allows the caller to participate directly in the parsing process.
    ///         No in-memory data representation will be created nor cached by the DWFSection implicitly.
    ///
    ///\param   rObjectDefinitionReader     An interface reference to an object that knows how to read 
    ///                                     and parse object definition documents.
    ///\param   rObjectDefinitionStream     A stream providing an object definition document for this section.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual 
    void getObjectDefinition( DWFObjectDefinitionReader& rObjectDefinitionReader,
                              DWFInputStream&            rObjectDefinitionStream ) const
        throw( DWFException );

    ///
    ///         Reads all content presentations resources in the section and
    ///         parses them with the default reader. The resources can then be
    ///         directly accessed to get the presentations.
    ///         
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual
    void readContentPresentations()
        throw( DWFException );

    ///
    ///         Processes the content presentation resource provided using the default reader
    ///         implemented by DWFContentPresentationResource.
    ///
    ///         rContentPresentationFilter     A parsing filter.  It will be set on the 
    ///                                        new DWFContentPresentation before processing begins.
    ///
    ///         rResource                   An object definition resource to process.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual
    void readContentPresentations( DWFContentPresentationReader& rContentPresentationFilter,
                                   DWFResource&                  rResource ) const
        throw( DWFException );

    ///
    ///         Processes content presentations using the reader interface provided.
    ///         This allows the caller to participate directly in the parsing process.
    ///         No in-memory data representation will be created nor cached by the DWFSection implicitly.
    ///
    ///\param   rContentPresentationReader     An interface reference to an object that knows how to read 
    ///                                        and parse object definition documents.
    ///\param   pResource                      An optional pointer to a content presentation resource.
    ///                                        If NULL, all content definition resources will be processed.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual 
    void readContentPresentations( DWFContentPresentationReader& rContentPresentationReader,
                                   DWFResource*                  pResource) const
        throw( DWFException );

    ///
    ///         Processes object definitions using the reader interface provided.
    ///         This allows the caller to participate directly in the parsing process.
    ///         No in-memory data representation will be created nor cached by the DWFSection implicitly.
    ///
    ///\param   rContentPresentationReader     An interface reference to an object that knows how to read 
    ///                                        and parse object definition documents.
    ///\param   rContentPresentationStream     A stream providing an object definition document for this section.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual 
    void readContentPresentations( DWFContentPresentationReader& rContentPresentationReader,
                                   DWFInputStream&               rContentPresentationStream ) const
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

    ///
    ///         The section logic for the toolkit.
    ///
    ///\return  A copy of the behavior data.
    ///\throw   None
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFSection::tBehavior behavior() const
        throw();

    ///
    ///         Sets the section logic for the toolkit.
    ///
    ///\param   rBehavior   The behavior logic.
    ///\throw   None
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual void applyBehavior( const DWFSection::tBehavior& rBehavior )
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML(DWFXMLSerializer& rSerializer, unsigned int nFlags )
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

protected:

    static const wchar_t* const kzProperty_Label;
    static const wchar_t* const kzProperty_LabelIconResourceID;
    static const wchar_t* const kzProperty_InitialURI;
    static const wchar_t* const kzPropertyCategory_Hidden;

protected:

    typedef std::multimap<DWFResource*, DWFString>    tResourceStringMultiMap;

protected:

    DWFString           _zType;
    DWFString           _zName;
    DWFString           _zTitle;
    DWFString           _zLabel;
    DWFString           _zLabelIconResourceID;
    DWFResource*        _pLabelIconResource;
    DWFString           _zInitialURI;
    DWFString           _zObjectID;
    double              _nVersion;
    double              _nPlotOrder;
    DWFSource           _oSource;

    DWFPackageReader*           _pPackageReader;
    DWFContentManager*          _pContentManager;
    
    //
    //  Track resources that are associated with contents, mapping them to the content ID.
    //
    tResourceStringMultiMap     _oResourceToContentID;

    ///
    ///         The current section logic used by the toolkit.
    ///
    tBehavior                   _tBehavior;

    bool                        _bContentPresentationsRead;

protected:

    ///
    ///         Builds a generic or untyped resource.
    ///
    _DWFTK_API
    virtual DWFResource* buildResource( const char**      ppAttributeList,
                                        DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a font resource.
    ///
    _DWFTK_API
    virtual DWFFontResource* buildFontResource( const char**      ppAttributeList,
                                                DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a image resource.
    ///
    _DWFTK_API
    virtual DWFImageResource* buildImageResource( const char**      ppAttributeList,
                                                  DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Builds a graphic resource.
    ///
    _DWFTK_API
    DWFGraphicResource* buildGraphicResource( const char**      ppAttributeList,
                                              DWFPackageReader* pPackageReader )
        throw( DWFException );

    //
    //
    //
    _DWFTK_API
    void _parseDocument( DWFInputStream& rDocumentStream,
                         DWFCore::DWFXMLCallback& rDocumentReader ) const
        throw( DWFException );

private:

    //
    //  This traps the resource containers _remove calls to remove the mapping of the
    //  resource to contentID, and then passes the call onto the base class.
    //
    _DWFTK_API
    virtual DWFResource* _remove( DWFResource* pResource, bool bDeleteIfOwned )
        throw();
};

    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start
_declspec(selectany) const wchar_t* const DWFSection::kzProperty_Label                  = L"_Label";
_declspec(selectany) const wchar_t* const DWFSection::kzProperty_LabelIconResourceID    = L"_LabelIconResourceID";
_declspec(selectany) const wchar_t* const DWFSection::kzProperty_InitialURI             = L"_InitialURI";
_declspec(selectany) const wchar_t* const DWFSection::kzPropertyCategory_Hidden         = L"hidden";
//DNT_End

#endif
#endif

}

#endif
