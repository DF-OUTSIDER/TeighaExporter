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

#ifndef _DWFTK_DWFX_PACKAGE_WRITER_H
#define _DWFTK_DWFX_PACKAGE_WRITER_H

///
///\file        dwf/package/writer/DWFXPackageWriter.h
///\brief       This file contains the DWFXPackageWriter class declaration.
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
#include "dwf/package/writer/DWFXPackageVersionExtension.h"
#include "dwf/opc/ZipWriter.h"

namespace DWFToolkit
{

//
// fwd decl
//
class OPCPart;
class OPCCoreProperties;
class DWFXPackage;
class DWFXDWFSection;
class DWFXDWFDocument;
class DWFXFixedPage;
class DWFXDWFProperties;
class DWFXCustomProperties;

///
///\ingroup     dwfpackage
///
///\class       DWFXPackageWriter   dwf/package/writer/DWFXPackageWriter.h     "dwf/package/writer/DWFXPackageWriter.h"
///\brief       This class is used to publish DWFX package files.
///\since       7.4.0
///
///\todo        Add direct publishing to DWFOutputStream in addition to DWFFile respresentation.
///
class DWFXPackageWriter : public DWFPackageWriter
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
public:
        ///
        ///\brief   These enumerations are used to specify whether proxy
        ///         content, if specified, is added to the package.
        ///         If \a eAlways is specified, proxy content will always be added.
        ///         If \a eIfNoPageOutput is specified, proxy content will only be added if no other FixedPage content would be generated.
        ///         If \a eIfOnlyPartialPageOutput is specified, proxy content only be added if at least one section exists which provides no FixedPage content.
        ///
    typedef enum teProxyGraphicsRulesType
    {
        eAlways                     = 0x00,
        eIfNoPageOutput             = 0x01,
        eIfOnlyPartialPageOutput    = 0x02
    } teProxyGraphicsRulesType;


public:

    ///
    ///         Implements a receiver for raw resources that should be added to the container.
    ///         A provider will call this object for each resource to be provided.  These 
    ///         resources will not be added to the manifest / descriptor hierarchy, but simply 
    ///         as a file in the package.
    ///
    class DWFXUndeclaredResourceReceiver : public DWFPackageWriter::UndeclaredResource::Receiver
    {
        DWFToolkit::OPCZipWriter* _pZipper;

    public:
        ///
        ///  Constructor
        ///
        DWFXUndeclaredResourceReceiver( DWFToolkit::OPCZipWriter* pZipper )
        : _pZipper( pZipper )
        { ; }

        ///
        ///  \copydoc DWFPackageWriter::UndeclaredResource::Receiver::receiveResource( const DWFCore::DWFString &rStoragePath, DWFInputStream* pInStream, DWFZipFileDescriptor::teFileMode eZipMode )
        ///
        void receiveResource( const DWFCore::DWFString &rStoragePath, DWFInputStream* pInStream, DWFZipFileDescriptor::teFileMode eZipMode ) throw();
    };

    ///
    ///         Implements a receiver for content types that should be added to the DWFX's OPC 
    ///         container's [ContentTypes].xml container.  A provider will call this object
    ///         for each content type to be provided.
    ///
    class DWFXUndeclaredContentTypeReceiver : public DWFPackageWriter::UndeclaredContentType::Receiver
    {
        DWFXPackage* _pPackage;

    public:
        ///
        ///  Constructor
        ///
        DWFXUndeclaredContentTypeReceiver( DWFXPackage* pPackage )
        : _pPackage( pPackage )
        { ; }

        ///
        ///  \copydoc DWFPackageWriter::UndeclaredContentType::Receiver::receiveContentType( const DWFCore::DWFString &rExtension, const DWFCore::DWFString &rContentType )
        ///
        void receiveContentType( const DWFCore::DWFString &rExtension, const DWFCore::DWFString &rContentType ) throw();
    };

    ///
    ///         Implements a receiver for content types that should be added to the DWFX's OPC 
    ///         container's [ContentTypes].xml container.  A provider will call this object
    ///         for each content type to be provided.
    ///
    class DWFXUndeclaredPackageRelationshipsReceiver : public DWFPackageWriter::UndeclaredPackageRelationships::Receiver
    {
        typedef _DWFTK_STD_VECTOR(OPCPart*) _tPartVector;

        DWFXPackage* _pPackage;
        _tPartVector _oPartVector;

    public:
        ///
        ///  Constructor
        ///
        DWFXUndeclaredPackageRelationshipsReceiver( DWFXPackage* pPackage )
        : _pPackage( pPackage )
        { ; }

        ///
        ///  Denstructor
        ///
        ~DWFXUndeclaredPackageRelationshipsReceiver();

        ///
        ///  \copydoc DWFPackageWriter::UndeclaredPackageRelationships::Receiver::receiveRelationship( DWFToolkit::OPCRelationship* pRelationship )
        ///
        void receiveRelationship( DWFToolkit::OPCRelationship* pRelationship ) throw();
    };


    ///
    ///         Constructor
    ///
    ///\param   rDWFPackageFile         Identifies the DWFX file to publish.
    ///\param   zDWFPackagePassword     Package password.
    ///\param   bNoPasswordSalting      No password salting, true or false.
    ///\param   pVersionExtension       Optional version extension object to modify the file format version
    ///\throw   None
    ///
    _DWFTK_API
    DWFXPackageWriter( const DWFFile&   rDWFPackageFile,
                       const DWFString& zDWFPackagePassword = /*NOXLATE*/L"",
                       DWFXPackageVersionExtension*  pVersionExtension = NULL,
                       bool             bNoPasswordSalting = true )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFXPackageWriter()
        throw();

    ///
    ///\copydoc DWFPackageWriter::addSection(DWFSection* pSection, DWFInterface*  pInterface = NULL)
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
    ///         This is used to pass proxy graphics content to the
    ///         package writer for serialization. 
    ///         The provided section should have resources that can
    ///         be serialized such that valid XPS graphics are produced.
    ///
    ///\param   pSection        The section containing the proxy content resources.
    ///                         This pointer will be deleted with the \b DWFCORE_FREE_OBJECT macro.
    ///\param   eRules          Values of type teProxyGraphicsRulesType, specifying in which cases the proxy content should be used.
    ///\param   bAsFirstPage    A boolean specifying whether the proxy graphics should be the first page (true), or the last page (false).
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addProxyGraphicsSection( DWFSection* pSection, teProxyGraphicsRulesType eRules=eAlways, bool bAsFirstPage=true )
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
        throw( DWFException );

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
        throw( DWFException );

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
        throw( DWFException );

	///
    ///         This is used to add default proxy page to the package from a source proxy page XPS file.
    ///         The provided proxy page XPS file should be with a full path name.
    ///
    ///\param   zProxyPageXPSPath		A specified culture proxy page XPS file.
    ///\throw   DWFException
    ///
	_DWFTK_API
    virtual void addProxyPage( const DWFString& zProxyPageXPSPath )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageWriter::write(const DWFString&                 zSourceProductVendor = /*NOXLATE*/L"",\
    ///            const DWFString&                 zSourceProductName = /*NOXLATE*/L"",\
    ///            const DWFString&                 zSourceProductVersion = /*NOXLATE*/L"",\
    ///            const DWFString&                 zDWFProductVendor = /*NOXLATE*/L"",\
    ///            const DWFString&                 zDWFProductVersion = /*NOXLATE*/L"",\
    ///            DWFZipFileDescriptor::teFileMode eCompressionMode = DWFZipFileDescriptor::eZipSmallest)
    ///
    _DWFTK_API
    void write( const DWFString&                 zSourceProductVendor = /*NOXLATE*/L"",
                const DWFString&                 zSourceProductName = /*NOXLATE*/L"",
                const DWFString&                 zSourceProductVersion = /*NOXLATE*/L"",
                const DWFString&                 zDWFProductVendor = /*NOXLATE*/L"",
                const DWFString&                 zDWFProductVersion = /*NOXLATE*/L"",
                DWFZipFileDescriptor::teFileMode eCompressionMode = DWFZipFileDescriptor::eZipSmallest )
        throw( DWFException );

    ///
    ///         This DWFX-only method allows one to directly set the core properties on the 
    ///         package
    ///

private:

    typedef _DWFTK_STD_MAP(DWFResource*, OPCPart*)           _tResourcePartMap;
    typedef _DWFTK_STD_MAP(DWFString, DWFResource*)          _tResourceIDMap;

private:

    DWFString                       _zDWFPassword;

    DWFXPackage*                    _pPackage;

    _tResourcePartMap               _oResourcePartMap;
    _tResourceIDMap                 _oResourceIDMap;

    OPCCoreProperties*              _pCoreProperties;
    DWFXDWFProperties*              _pDWFProperties;
    DWFXCustomProperties*           _pCustomProperties;

    // Proxy graphics info
    DWFSection*                     _pProxyGraphicsSection;
    teProxyGraphicsRulesType        _eProxyGraphicsRules;
    bool                            _bProxyGraphicsGoOnFirstPage;
    DWFXPackageVersionExtension*    _pVersionExtension;

private:

    //
    //
    //
    void _writeSection( DWFSection* pSection, DWFXDWFDocument* pDocument )
        throw( DWFException );

    //
    //  Use this to iterate over section resources to determined whether 
    //  we will create a fixed page or not for this section.
    //
    bool _fixedPageDesired( DWFSection* pSection )
        throw( DWFException );

    //
    //
    //
    DWFXFixedPage* _createFixedPage( DWFSection* pSection, bool bAppend=true )
        throw( DWFException );

    //
    //
    //
    void _addResourcePartsByRole( const DWFString& zRole,
                                  DWFXDWFSection*  pDescriptorPart,
                                  DWFXFixedPage*   pFixedPage = NULL )
        throw( DWFException );

    //
    //
    //
    void _addResourcePart( DWFResource&    rResource,
                           DWFXDWFSection* pDescriptorPart,
                           DWFXFixedPage*  pFixedPage = NULL )
        throw( DWFException );

    //
    //  After adding all resources to the DWFXPackage call this to add
    //  resource relationships
    //
    void _addResourceRelationships()
        throw( DWFException );

    void _addRelationships( DWFSection& rSection )
        throw( DWFException );

    void _addRelationships( DWFResource& rResource )
        throw( DWFException );

    //
    //  This sets the DWFToolkitVersion in DWF Properties. It also supports
    //  the old method of setting the properties provided in the argument via
    //  the arguments to DWFPackageWriter::write(...). If any properties have
    //  been directly added to the manifest based on the old API usage, this
    //  moves the properties into the appropriate places.
    //
    void _updateProperties( const DWFString& zSourceProductVendor,
                            const DWFString& zSourceProductName,
                            const DWFString& zSourceProductVersion,
                            const DWFString& zDWFProductVendor,
                            const DWFString& zDWFProductVersion )
        throw( DWFException );

private:

    //
    //  Not implemented
    //
    DWFXPackageWriter();
    DWFXPackageWriter( const DWFXPackageWriter& );
    DWFXPackageWriter& operator=( const DWFXPackageWriter& );

};

}

#endif
#endif
