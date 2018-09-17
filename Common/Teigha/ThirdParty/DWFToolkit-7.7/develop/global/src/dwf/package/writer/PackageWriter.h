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

#ifndef _DWFTK_PACKAGE_WRITER_H
#define _DWFTK_PACKAGE_WRITER_H

///
///\file        dwf/package/writer/DWFPackageWriter.h
///\brief       This file contains the DWFPackageWriter class declaration.
///


#ifndef DWFTK_READ_ONLY


#include "dwfcore/STL.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/UUID.h"
#include "dwfcore/Vector.h"
#include "dwfcore/DigestOutputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/XML.h"
#include "dwf/package/Constants.h"
#include "dwf/package/utility/DWFPackageFileDescriptor.h"
#include "dwf/package/SignatureRequest.h"
#include "dwf/opc/Relationship.h"

namespace DWFToolkit
{

//
// fwd decl
//
class DWFPropertySet;
class DWFManifest;
class DWFResource;
class DWFInterface;
class DWFSection;
class DWFGlobalSection;
class DWFContentManager;
class DWFPropertyContainer;
class DWFSignatureRequest;
class DWFContentPresentationContainer;
class DWFPackageContentPresentations;
///
///\ingroup     dwfpackage
///
///\class       DWFPackageWriter   dwf/package/writer/DWFPackageWriter.h     "dwf/package/writer/DWFPackageWriter.h"
///\brief       Interface for creating DWF packages
///\since       7.0.1
///
///\todo        Add direct publishing to DWFOutputStream in addition to DWFFile respresentation.
///
class DWFPackageWriter      _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

        ///
        ///\brief   These enumerations are used during the serialization process
        ///         to alter the content produced by the serializable objects.
        ///
    typedef enum teDocumentType
    {
        eManifest               = 0x0100,
        eContentPresentation    = 0x0200,
        eDescriptor             = 0x0400,
        eObjectDefinition       = 0x0800,
        eGlobalContent          = 0x1000,
        eSectionContent         = 0x2000,
        eSignatureRequest       = 0x4000
    } teDocumentType;

        ///
        ///\brief   These enumerations are used during the serialization process
        ///         to alter the content produced by the serializable objects.
        ///
    typedef enum teSectionType
    {
        eNone               = 0x00,
        eEPlot              = 0x01,
        eEModel             = 0x02,
        eGlobal             = 0x04,
        eData               = 0x08,
        eSignatures         = 0x10

    } teSectionType;

    typedef enum teMergeContent {
        eNoMerge,
        eMergePriorityPrimary,
        eMergePrioritySecondary
    } teMergeContent;

    ///
    ///         Defines a utility class for a provider and receiver of raw resources that should 
    ///         resources will not be added to the manifest / descriptor hierarchy, but simply 
    ///         as a file in the package.be added to the container.
    ///
    /// \note   There is no contract, written or implied, which requires consumer code to 
    ///         preserve these raw resources during aggregation.
    ///
    class UndeclaredResource
    {
    public:
        ///
        ///         Interface for a receiver of raw resources that should be added to the container.
        ///         A provider will call this object for each resource to be provided.  
        ///
        class Receiver
        {
        public:
            ///
            /// Receives resource information, and ostensibly writes it to the package
            ///
            ///\note    The input stream will NOT be deleted by the receiver 
            ///
            ///\param   zStoredPath         The path within the package to be used for the resource.
            ///\param   pInputStream        The stream from which to read the resource content.
            ///\param   eZipMode            Override the zip mode in descriptor and use this instead.
            virtual void receiveResource( const DWFCore::DWFString &rStoragePath, DWFInputStream* pInStream, DWFZipFileDescriptor::teFileMode eZipMode ) throw() = 0;
        };

        ///
        ///         Interface for a provider of raw resources that should be added to the container.
        ///         The provider will call the receiver object for each resource to be provided.  
        ///
        class Provider
        {
        public:
            ///
            /// Provides resource information to the receiver
            ///
            /// \param pReceiver Points to the object that will be called for each resource to be added
            virtual void provideResources( Receiver* pReceiver ) throw() = 0;
        };

    };

    ///
    ///         Defines a utility class for a provider and receiver of content types that should 
    ///         be added to the DWFX's OPC container's [ContentTypes].xml container.  
    ///
    /// \note   There is no contract, written or implied, which requires consumer code to 
    ///         preserve these content types during aggregation.

    class UndeclaredContentType
    {
    public:
        ///
        ///         Interface for a receiver of content types that should be added to the DWFX's OPC 
        ///         container's [ContentTypes].xml container.  A provider will call this object
        ///         for each content type to be provided.
        ///
        class Receiver
        {
        public:
            ///
            /// Receives content type information, and ostensibly writes it to the package
            ///
            ///\param   rExtension          The OPC content type extension. Used only for DWFX packages.
            ///\param   rType               The OPC extension type. Used only for DWFX packages.
            virtual void receiveContentType( const DWFCore::DWFString &rExtension, const DWFCore::DWFString &rContentType ) throw() = 0;
        };

        ///
        ///         Interface for a provider of content types that should be added to the DWFX's OPC 
        ///         container's [ContentTypes].xml container.  The provider will call the receiver 
        ///         object for each content type to be provided.
        ///
        class Provider
        {
        public:
            ///
            /// Provides content type information to the receiver
            ///
            /// \param pReceiver Points to the object that will be called for each content type to be added
            virtual void provideContentTypes( Receiver* pReceiver ) throw() = 0;
        };


    };

    ///
    ///         Defines a utility class for a provider and receiver of package relationships
    ///         that should be added to the DWFX's OPC container's top level _rels/.rels file.
    ///
    class UndeclaredPackageRelationships
    {
    public:
        ///
        ///         Interface for a receiver of content types that should be added to the DWFX's OPC 
        ///         container's [ContentTypes].xml container.  A provider will call this object
        ///         for each content type to be provided.
        ///
        class Receiver
        {
        public:
            ///
            /// Receives a relationship object, and ostensibly writes it to the package (/_rels/.rels)
            ///
            /// \param   pRelationship      The OPC Relationship object
            virtual void receiveRelationship( DWFToolkit::OPCRelationship* pRelationship ) throw() = 0;
        };

        ///
        ///         Interface for a provider of content types that should be added to the DWFX's OPC 
        ///         container's top level _rels/.rels file.  The provider will call the receiver 
        ///         object for each relationship to be provided.
        ///
        class Provider
        {
        public:
            ///
            /// Provides content type information to the receiver
            ///
            /// \param pReceiver Points to the object that will be called for each content type to be added
            virtual void provideRelationships( Receiver* pReceiver ) throw() = 0;
        };

    };

public:

    ///
    ///         Constructor
    ///
    ///\param   rDWFPackageFile         Identifies the DWF file to publish.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPackageWriter( const DWFFile& rDWFPackageFile )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPackageWriter()
        throw();
	
	///
    ///         The will salt any provided password to prevent usage of zip password recovery tools. Note: this will
    ///         make the DWF package non-PKZIP compliant.
    ///
    ///\param   bEnable                 Set this to true to enable salting.
    ///\throw   None
    ///
    _DWFTK_API
    virtual void enablePasswordSalting( bool bEnable )
        throw();

    ///
    ///         Adds a section to the DWF package.
    ///
    ///\param   pSection                The section to publish into the package.
    ///                                 Ownership of this section pointer will be claimed automatically.
    ///                                 This means that, left as is, the pointer will be deleted
    ///                                 once the package is published.  
    ///                                 In order to prevent this, the caller can explicitly reclaim
    ///                                 ownership of the section by calling DWFSection::own().
    ///
    ///\param   pInterface              This object should be provided if \a pSection is a custom type that
    ///                                 should be declared in the manifest (it will be ignored for known types.)
    ///                                 Note that the DWF Specification dictates that at least one interface
    ///                                 be declared in the manifest.  If none exist, an exception will be thrown at write time.
    ///                                 This pointer will be deleted with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addSection( DWFSection*    pSection,
                             DWFInterface*  pInterface = NULL )
        throw( DWFException );

    ///
    ///         Adds a type global section to the DWF package.
    ///
    ///\param   pSection                The global section to publish into the package.
    ///                                 Ownership of this section pointer will be claimed automatically.
    ///                                 This means that, left as is, the pointer will be deleted
    ///                                 once the package is published.  
    ///                                 In order to prevent this, the caller can explicitly reclaim
    ///                                 ownership of the section by calling \a DWFSection::own().
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addGlobalSection( DWFGlobalSection* pSection )
        throw( DWFException );

    ///
    ///         Adds an interface to the DWF package manifest.
    /// 
    ///         This method is provided in addition to the \a addSection() method
    ///         as it may be necessary to, in the case of dumb package copying
    ///         for example, to include this information in the target package
    ///         without having any ability to corelate the source sections
    ///         and interfaces.  
    ///         This pointer will be deleted with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///         This method will check for duplicate interfaces and
    ///         if \a pInternface is a duplicate, it will
    ///         be deleted and the previously stored one will be returned.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFInterface* addInterface( DWFInterface* pInterface )
        throw( DWFException );

    ///
    ///         This attaches the content manager to the manifest owned
    ///         by the package writer. The manifest can take ownership of
    ///         the manager if requested to.
    ///
    ///\param   pContentManager         The content manager to attach.
    ///\param   bTakesOwnership         Set this to true if the ownership is supposed change.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual void attachContentManager( DWFContentManager* pContentManager, bool bTakesOwnership = true )
        throw( DWFException );

    ///
    ///         This returns the content manager associated with this writer.
    ///         If a content manager was not attached to the writer, a new one
    ///         will be created by the writer's manifest and returned here. Note
    ///         the caller should not delete the content manager unless it was attached
    ///         using \a attachContentManager with \a bTakeOwnership set to false.
    ///
    ///\return  Pointer to the content manager.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual DWFContentManager* getContentManager()
        throw( DWFException );

    ///
    ///         Inserts a package level presentation to the container.
    ///
    ///\param   pPresentation       The presentation to add to the container (must
    ///                             not be NULL). This  pointer is now owned by the
    ///                             container and will be released with the \b
    ///                             DWFCORE_FREE_OBJECT macro.
    ///\param   bOwnPresentation    If \e true, the container will claim ownership of the presentation
    ///                             and delete it using the \b DWFCORE_FREE_OBJECT macro upon
    ///                             destruction of this container if it is still owned.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addPresentation( DWFContentPresentation* pPresentation, bool bOwnPresentation = true )
        throw( DWFException );

    ///
    ///         Sets a provider for raw resources that should be added to the container.
    ///
    ///\param   pProvider   An object which is used to provide undeclared resources 
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setUndeclaredResourceProvider( UndeclaredResource::Provider* pProvider )
        throw( DWFException );

    ///
    ///         Sets a provider for content types that should be added to the DWFX's OPC 
    ///         container's [ContentTypes].xml container.  The provider will call an 
    ///         UndeclaredContentType::Receiver object for each content type to be provided.
    ///
    ///\param   pProvider   An object which is used to provide undeclared content types (DWFX) 
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setUndeclaredContentTypeProvider( UndeclaredContentType::Provider* pProvider )
        throw( DWFException );

    ///
    ///         Sets a provider for relationships that should be added to the DWFX's OPC 
    ///         container's /_rels/.rels container.  The provider will call an 
    ///         UndeclaredPackageRelationships::Receiver object for each relationship to 
    ///         be provided.
    ///
    ///\param   pProvider   An object which is used to provide undeclared content types (DWFX) 
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setUndeclaredPackageRelationshipsProvider( UndeclaredPackageRelationships::Provider* pProvider )
        throw( DWFException );


    ///
    ///         Returns the property container interface
    ///         for the package manifest.  
    ///
    ///         This can be used to add package-wide properties to the DWF.
    ///
    ///\return  An interface with which to add package manifest properties.
    ///\throw   DWFException
    ///
    ///\deprecated  This legacy API was deprecated in version 7.4.0. Instead use the following
    ///             calls \a setCoreProperties(), \a setDWFProperties and \a setCustomProperties.
    ///
    _DWFTK_API
    virtual DWFPropertyContainer& getManifestProperties()
        throw( DWFException );

    ///
    ///         This is used to pass properties, conforming to the OPC-defined CoreProperties
    ///         specification, to the package writer for serialization. For DWFX files the
    ///         properties get serialized into the core properties separate part. 
    ///         The provided property set must have the core properties schema ID. 
    ///         Any properties that that do not conform to the schema will be ignored.
    ///
    ///\param   pSet    The property set containing the properties. The property set must
    ///                 have it's schema ID set to OPCXML::kzSchemaID_CoreProperties.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setCoreProperties( DWFPropertySet* pSet )
        throw( DWFException ) = 0;

    ///
    ///         This is used to pass properties, conforming to the DWF properties
    ///         specification, to the package writer for serialization. For DWFX files
    ///         the properties get serialized into the separate DWF Properties part. 
    ///         The provided property set must have the DWFProperties schema ID. 
    ///         Any properties that that do not conform to the schema will be ignored.
    ///
    ///\param   pSet    The property set containing the properties. The property set must
    ///                 have it's schema ID set to DWFXXML::kzSchemaID_DWFProperties.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setDWFProperties( DWFPropertySet* pSet )
        throw( DWFException ) = 0;

    ///
    ///         This can be used to serialize all documents custom properties provided by 
    ///         the publisher.  This is is serilized into a separate part in DWFX. The provided
    ///         property set should \b not contain nested any subsets since these will
    ///         be ignored during serialization.
    ///
    ///\param   pSet    The property set containing the properties.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setCustomProperties( DWFPropertySet* pSet )
        throw( DWFException ) = 0;

    ///
    ///         Initialize the writer for aggregation. This lets the writer collect
    ///         information required for aggregation as the sections are added.
    ///         This should be called right after the writer has been created.
    ///
    ///\param   eMergeType      This defines whether or not to merge the content if there
    ///                         multiple contents.
    ///\throw   None
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    virtual void initializeForAggregation( teMergeContent eMergeType = eMergePriorityPrimary )
        throw( DWFException );

    ///
    ///         Writes out the DWF package.
    ///
    ///         Any sections and other objects owned by the writer and/or internal manifest
    ///         will be deleted at the completion of this method.  Be sure to claim ownership
    ///         of any such objects that are necessary to survive after this call.
    ///
    ///         This method exposes the standard DWF package properties directly.
    ///         It is highly recommended that at DWF publishers set these correctly with as
    ///         much detail as possible.
    ///
    ///         Setting standard DWF package properties:
    ///         \code
    ///         Autodesk DWF Writer is a printer driver DLL that is used to generate DWF packages
    ///         through the Windows printing system, it is a component that another application
    ///         would use (indirectly) to publish the DWF.  The following is an example of how
    ///         it might populate these properties when printing from Microsoft Word:
    ///
    ///         zSourceProductVendor    = L"Microsoft Corporation"          <--- from Word 2003
    ///         zSourceProductName      = L"Microsoft Office 2003"          <--- from Word 2003
    ///         zSourceProductVersion   = L"11.0.5604"                      <--- from Word 2003
    ///         zDWFProductVendor       = L"Autodesk, Inc."                 <--- from DWF Writer 2
    ///         zDWFProductVersion      = L"2.0.4.0"                        <--- from DWF Writer 2
    ///         \endcode
    ///
    ///\param   zSourceProductVendor    A standard property detailing the vendor of the software
    ///                                 that produced the original content and used the DWF product
    ///                                 in order to produce the DWF package.
    ///\param   zSourceProductName      A standard property detailing the name of the software
    ///                                 that produced the original content and used the DWF product
    ///                                 in order to produce the DWF package.
    ///\param   zSourceProductVersion   A standard property detailing the version of the software
    ///                                 that produced the original content and used the DWF product
    ///                                 in order to produce the DWF package.
    ///\param   zDWFProductVendor       A standard property that details the vendor of the software component
    ///                                 that was used by the source product to publish the DWF package.
    ///\param   zDWFProductVersion      A standard property that details the version of the software component
    ///                                 that was used by the source product to publish the DWF package.
    ///\param   eCompressionMode        An enumeration that adjusts the zip library compression options.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void write( const DWFString&                 zSourceProductVendor = /*NOXLATE*/L"",
                        const DWFString&                 zSourceProductName = /*NOXLATE*/L"",
                        const DWFString&                 zSourceProductVersion = /*NOXLATE*/L"",
                        const DWFString&                 zDWFProductVendor = /*NOXLATE*/L"",
                        const DWFString&                 zDWFProductVersion = /*NOXLATE*/L"",
                        DWFZipFileDescriptor::teFileMode eCompressionMode = DWFZipFileDescriptor::eZipSmallest )
        throw( DWFException ) = 0;

protected:

    //
    // 
    //
    virtual void _ensureManifest()
        throw( DWFException );

    //
    //  This will aggregate content from other packages, and merge if the merge flag is set
    //
    virtual void _aggregateContent()
        throw( DWFException );

    //
    //  If aggregation was performed this will cleanup, and more important - restore
    //  contents to their original owner content managers.
    //
    virtual void _postAggregationCleanup()
        throw();



protected:
    typedef _DWFTK_STD_VECTOR(DWFSection*)                  _tSectionVector;
    typedef _DWFTK_STD_VECTOR(DWFGlobalSection*)            _tGlobalSectionVector;
    typedef _DWFTK_STD_MAP(DWFSection*, DWFContentManager*) _tSectionToContentManager;
    typedef DWFSortedVector<DWFContentManager*>             _tSortedContentManagerVector;
    typedef _DWFTK_STD_MAP(DWFContent*, DWFContentManager*) _tContentToContentManager;
    typedef _DWFTK_STD_VECTOR(DWFOutputStream*)             _tStreamsVector;

protected:

    DWFFile                         _rDWFPackage;

    DWFUUID                         _oUUID;

    DWFManifest*                    _pPackageManifest;

    double                          _nNextPlot;

    bool                            _bEPlotGlobalAdded;
    bool                            _bEModelGlobalAdded;

    size_t                          _nEPlotSectionCount;
    size_t                          _nEModelSectionCount;

    _tSectionVector                 _oSections;
    _tGlobalSectionVector           _oGlobalSections;

    bool                            _bAggregating;
    teMergeContent                  _eMergeType;
	
	bool                            _bNoPasswordSalting;

    //
    //  The section and the "original" content manager that it was associated with before being added
    //
    _tSectionToContentManager       _oSectionToContentManager;

    //
    //  The contents that get aggregated and the "original" content managers they came from
    //
    _tContentToContentManager       _oContentToContentManager;

    //
    //  A provider for undeclared resource types
    //
    UndeclaredResource::Provider*   _pUndeclaredResourceProvider;

    //
    //  A provider for undeclared content types (DWFX)
    //
    UndeclaredContentType::Provider* _pUndeclaredContentTypeProvider;

    //
    //  A provider for undeclared package relationships (DWFX)
    //
    UndeclaredPackageRelationships::Provider* _pUndeclaredPackageRelationshipsProvider;

    DWFPackageContentPresentations* _pPackageContentPresentations;

private:

    DWFPackageWriter();
    DWFPackageWriter( const DWFPackageWriter& );
    DWFPackageWriter& operator=( const DWFPackageWriter& );
};

}

#endif
#endif

