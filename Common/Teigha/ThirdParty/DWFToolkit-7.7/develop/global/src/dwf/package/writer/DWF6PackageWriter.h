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

#ifndef _DWFTK_DWF6_PACKAGE_WRITER_H
#define _DWFTK_DWF6_PACKAGE_WRITER_H

///
///\file        dwf/package/writer/DWF6PackageWriter.h
///\brief       This file contains the DWF6PackageWriter class declaration.
///


#ifndef DWFTK_READ_ONLY


#include "dwfcore/STL.h"
#include "dwfcore/DWFXMLSerializer.h"
#include "dwfcore/UUID.h"
#include "dwfcore/Vector.h"
#include "dwfcore/DigestOutputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/writer/PackageWriter.h"
#include "dwf/package/writer/DWF6PackageVersionExtension.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWF6PackageWriter   dwf/package/writer/DWF6PackageWriter.h     "dwf/package/writer/DWF6PackageWriter.h"
///\brief       This class is used to publish DWF6 package files.
///\since       7.4.0
///
///\todo        Add direct publishing to DWFOutputStream in addition to DWFFile respresentation.
///
class DWF6PackageWriter : public DWFPackageWriter
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:
    ///
    ///         Implements a receiver for raw resources that should be added to the container.
    ///         A provider will call this object for each resource to be provided.  These 
    ///         resources will not be added to the manifest / descriptor hierarchy, but simply 
    ///         as a file in the package.
    ///
    class DWF6UndeclaredResourceReceiver : public DWFPackageWriter::UndeclaredResource::Receiver
    {
        DWFPackageFileDescriptor* _pPackageDescriptor;
        DWFString _zDWFPassword;
        bool _bPKZIPCompliantPassword;

    public:
        ///
        ///  Constructor
        ///
        DWF6UndeclaredResourceReceiver( DWFPackageFileDescriptor* pPackageDescriptor, const DWFString& zDWFPassword, bool bPKZIPCompliantPassword )
        : _pPackageDescriptor( pPackageDescriptor )
        , _zDWFPassword( zDWFPassword )
        , _bPKZIPCompliantPassword( bPKZIPCompliantPassword )
        { ; }

        ///
        ///  \copydoc DWFPackageWriter::UndeclaredResource::Receiver::receiveResource( const DWFCore::DWFString &rStoragePath, DWFInputStream* pInStream, DWFZipFileDescriptor::teFileMode eZipMode )
        ///
        void receiveResource( const DWFCore::DWFString &rStoragePath, DWFInputStream* pInStream, DWFZipFileDescriptor::teFileMode eZipMode ) throw();
    };


public:
    ///
    ///         Constructor
    ///
    ///\param   rDWFPackageFile         Identifies the DWF file to publish.
    ///\param   zDWFPackagePassword     An optional password for protecting the DWF package file.
    ///\param   pVersionExtension       An optional plug-in for publishing package versions different from the curent one.
    ///\param   bNoPasswordSalting      If false, the provided password will be salted to prevent password recovery.
    ///\throw   None
    ///
    _DWFTK_API
    DWF6PackageWriter( const DWFFile&                rDWFPackageFile,
                       const DWFString&              zDWFPackagePassword = /*NOXLATE*/L"",
                       DWF6PackageVersionExtension*  pVersionExtension = NULL,
                       bool                          bNoPasswordSalting = true )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWF6PackageWriter()
        throw();

    ///
    ///\copydoc DWFPackageWriter::addSection(DWFSection* pSection, DWFInterface*  pInterface)
    ///
    _DWFTK_API
    virtual void addSection( DWFSection*    pSection,
                     DWFInterface*  pInterface = NULL )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageWriter::addGlobalSection(DWFGlobalSection* pSection)
    ///
    _DWFTK_API
    virtual void addGlobalSection( DWFGlobalSection* pSection )
        throw( DWFException );

    ///
    ///         Adds a request that a signature be generated when the DWF package is created.
    ///         This pointer will be deleted with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   pSignatureRequest       A class describing the signature that is being requested.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addSignatureRequest( DWFSignatureRequest* pSignatureRequest )
        throw( DWFException );

    ///
    ///         This is used to pass properties, conforming to the OPC-defined CoreProperties
    ///         specification, to the package writer for serialization. For DWF files the
    ///         properties get serialized into the manifest with the category set to the core
    ///         properties schema ID. The provided property set must have the core properties 
    ///         schema ID. Any properties that that do not conform to the schema will be ignored.
    ///
    ///\param   pSet    The property set containing the properties. The property set must
    ///                 have it's schema ID set to OPCXML::kzSchemaID_CoreProperties.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setCoreProperties( DWFPropertySet* pSet )
        throw( DWFException );

    ///
    ///         This is used to pass properties, conforming to the DWF properties
    ///         specification, to the package writer for serialization. For DWF files
    ///         the properties get serialized into the manifest with the DWF properties
    ///         schema ID. The provided property set must have the DWFProperties schema ID. 
    ///         Any properties that that do not conform to the schema will be ignored.
    ///
    ///\param   pSet    The property set containing the properties. The property set must
    ///                 have it's schema ID set to DWFXXML::kzSchemaID_DWFProperties.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setDWFProperties( DWFPropertySet* pSet )
        throw( DWFException );

    ///
    ///         This can be used to serialize all document level custom properties provided by 
    ///         the publisher.  This is is serilized into the manifest. The provided
    ///         property set should \b not contain nested any subsets since these will
    ///         be ignored during serialization.
    ///
    ///\param   pSet    The property set containing the properties.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void setCustomProperties( DWFPropertySet* pSet )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageWriter::write(const DWFString& zSourceProductVendor = /*NOXLATE*/L"", const DWFString& zSourceProductName = /*NOXLATE*/L"", \
    ///            const DWFString&                 zSourceProductVersion = /*NOXLATE*/L"", \
    ///            const DWFString&                 zDWFProductVendor = /*NOXLATE*/L"", \
    ///            const DWFString&                 zDWFProductVersion = /*NOXLATE*/L"", \
    ///            DWFZipFileDescriptor::teFileMode eCompressionMode = DWFZipFileDescriptor::eZipSmallest )
    ///
    _DWFTK_API
    void write( const DWFString&                 zSourceProductVendor = /*NOXLATE*/L"",
                const DWFString&                 zSourceProductName = /*NOXLATE*/L"",
                const DWFString&                 zSourceProductVersion = /*NOXLATE*/L"",
                const DWFString&                 zDWFProductVendor = /*NOXLATE*/L"",
                const DWFString&                 zDWFProductVersion = /*NOXLATE*/L"",
                DWFZipFileDescriptor::teFileMode eCompressionMode = DWFZipFileDescriptor::eZipSmallest )
        throw( DWFException );

private:

    //
    //  This is used only in cases where the content is available but not loaded - may happen
    //  during aggregation.
    //
    void _serializeContent( DWFContent* pContent )
        throw( DWFException );

    //
    //
    //
    void _serializeResource( const DWFString&   zSection,
                             DWFResource&       rResource )
        throw( DWFException );


    //
    //  This is called from package writer to add any document-wide properties to
    //  the manifest.
    //
    virtual void _addPropertiesToManifest( const DWFString& zSourceProductVendor,
                                           const DWFString& zSourceProductName,
                                           const DWFString& zSourceProductVersion,
                                           const DWFString& zDWFProductVendor,
                                           const DWFString& zDWFProductVersion )
        throw( DWFException );

    //
    //  Determine the extension if necessary based on section type counts and by
    //  looking for content, content presentations, or raster overlays.
    //
    void _determinePackageVersionExtension( const DWFString& zTypeInfo )
        throw( DWFException );

    //
    //  Methods for signature management
    //

    //
    //  If signatures are getting written we need to ensure that a signature section exists
    //
    DWFSection* _ensureSignatureSection()
        throw( DWFException );

    //
    //  Preprocess the signature requestes, creating mappings from the items being signed to
    //  the signature references pointing to them. A requests consists of one or more reference.
    //
    void _partitionSignatureReferences()
        throw( DWFException );

    //
    //  Add the reference to the mapped vector corresponding to the item pointer provided.
    //  This will be used for signature processing.
    //
    void _addToItemReferenceMap( void* pItem, DWFSignatureRequest::Reference* pReference )
        throw();

    //
    //  If a mapped vector exists, this will return a pointer to that vector, NULL otherwise. If a URI is
    //  provided, this will also set the URI on th references.
    //
    DWFSignatureRequest::tReferenceVector* _signatureReferences( void* pItem, const DWFString& zURI = /*NOXLATE*/L"")
        throw();

    //
    // For each of the digests covered by the given resources, produce a digestStream, and chain to the given initial stream.
    //
    DWFOutputStream* _wrapOutputStreamForReferences( DWFOutputStream* pInitialOutputStream, void* pItem, const DWFString& zURI = /*NOXLATE*/L"" )
        throw( DWFException );


private:

    typedef _DWFTK_STD_VECTOR(DWFSignatureRequest*)                        _tSignatureRequestVector;
    typedef _DWFTK_STD_MAP(void*, DWFSignatureRequest::tReferenceVector*)  _tItemReferenceMap;

private:

    DWFString                       _zDWFPassword;
    DWFPackageFileDescriptor*       _pPackageDescriptor;
    DWFXMLSerializer*               _pXMLSerializer;
    DWF6PackageVersionExtension*    _pVersionExtension;

    DWFPropertySet*                 _pDWFProperties;
    DWFPropertySet*                 _pCustomProperties;

    //
    // For Digital Signatures, we generate many digest streams, which will need
    // to be deleted after all signatures have been generated. Note that we DO NOT
    // take ownership of the real streams, and they can be deleted as soon as
    // everything has been written to them.
    //
    std::vector<DWFOutputStream*>           _oStreamsToDelete;

    //
    // The list of signature request that we have been asked to generate.
    //
    _tSignatureRequestVector                _oSignatureRequests;
    DWFSignatureRequest::tReferenceVector   _oManifestReferences;
    _tItemReferenceMap                      _oItemSignatureReferenceMap;

private:

    DWF6PackageWriter();
    DWF6PackageWriter( const DWF6PackageWriter& );
    DWF6PackageWriter& operator=( const DWF6PackageWriter& );
};

}

#endif
#endif

