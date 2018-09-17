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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/reader/PackageReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_PACKAGE_READER_H
#define _DWFTK_PACKAGE_READER_H


///
///\file        dwf/package/reader/PackageReader.h
///\brief       This file contains the DWFPackageReader class declaration.
///

#include "dwfcore/SkipList.h"
#include "dwfcore/TempFile.h"
#include "dwfcore/InputStream.h"
#include "dwfcore/XMLParser.h"
#include "dwfcore/ZipFileDescriptor.h"
#include "dwfcore/Synchronization.h"

using namespace DWFCore;

#include "dwf/Toolkit.h"

#include "dwf/opc/Constants.h"
#include "dwf/opc/Package.h"

#ifdef HAVE_CONFIG_H
#include "dwf/config.h"
#endif

namespace DWFToolkit
{

//
// fwd decl
//
class DWFManifest;
class DWFManifestReader;
class DWFSectionBuilder;
class DWFXPackage;
class OPCPart;
class OPCZipFileReader;
class OPCCoreProperties;
class DWFXFixedPageResourceExtractor;
class DWFXDWFProperties;
class DWFXCustomProperties;

    //
    // dictates the size of the heap buffer
    // used to cache input stream data for the parser
    //
#ifndef DWFTK_XML_PARSER_BUFFER_BYTES
#define DWFTK_XML_PARSER_BUFFER_BYTES   16384
#endif

///
///\ingroup     dwfpackage
///
///\class       DWFPackageReader   dwf/package/reader/PackageReader.h     "dwf/package/reader/PackageReader.h"
///\brief       This class is used to process DWF package files.
///\since       7.0.1
///
///\note        This class is built specifically for handling DWF 6 (and higher) <b>packages</b>.
///             Please be sure to check the type with \a getPackageInfo() or \a GetPackageInfo()
///             and handle single graphics streams separately with the appropriate toolkits.
///
///\warning     This object uses an implementation of the zip library that is not thread-safe.
///             This implies that this class is <b>not re-entrant</b> and data should be extracted
///             completely in-whole.
///
///\todo        Provide a thread-safe reader implementation.
///
class DWFPackageReader 
    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///\brief   Enumerations for known data file types.
    ///
    typedef enum
    {
        ///
        ///     DWF 6 zip package format.
        ///
        eDWFPackage,
        ///
        ///     Password protected DWF 6 (and higher) zip package format.
        ///
        eDWFPackageEncrypted,
        ///
        ///     2D graphics stream/file resource.
        ///
        eW2DStream,
        ///
        ///     Legacy DWF 0.55 (or lower) data and graphics stream.
        ///
        eDWFStream,
        ///
        ///     A non-DWF zip package format.
        ///
        eZIPFile,
        ///
        ///     Unknown format.
        ///
        eUnknown,
        ///
        ///     DWFX zip package format
        ///
        eDWFXPackage,
		///
        ///     Password protected DWFX zip package format
        ///
        eDWFXPackageEncrypted

    } tePackageType;

    typedef struct
    {
        ///
        ///     The package format version as a composite integer.
        ///     This value is the result of the major revision * 100
        ///     + the minor revision.
        ///     For example, the current file format expressed in this field is \b 600.
        ///
        unsigned long   nVersion;
        ///
        ///     One of the tePackageType enumeration values.
        ///
        tePackageType   eType;
        ///
        ///     If the file format supports a document type indentifier
        ///     (for example, the 6.01 package version), it will be
        ///     recovered and stored in this field.
        ///
        DWFString       zTypeGUID;

        bool            bDigitallySigned;

    } tPackageInfo;

    typedef void (*tpfPasswordCallback)(DWFPackageReader& rReader, DWFResource& rResource, DWFString& zPassword);

public:

    ///
    ///         Constructor
    ///
    ///         Use this constructor for a file-based read configuration.
    ///
    ///\param   zDWFPackageFile     The DWF file to open and process.
    ///\param   zDWFPackagePassword An optional password for encrypted files.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPackageReader( const DWFFile&   zDWFPackageFile,
                      const DWFString& zDWFPackagePassword = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Constructor
    ///
    ///         Use this constructor for a stream-based read configuration.
    ///
    ///\param   rDWFPackageStream   The DWF stream to open and process.
    ///\param   zDWFPackagePassword An optional password for encrypted files.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPackageReader( DWFInputStream&   rDWFPackageStream,
                      const DWFString&  zDWFPackagePassword = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPackageReader()
        throw();

    ///
    ///         Set the DWF package password (if unknown at construction time).
    ///
    ///\param   zDWFPackagePassword An optional password for encrypted files.
    ///\throw   None
    ///
    _DWFTK_API
    void setPackagePassword( const DWFString& zDWFPackagePassword )
        throw();

    ///
    ///         Returns the DWF package information.
    ///
    ///\param   rInfo       The package information from the DWF header.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void getPackageInfo( tPackageInfo& rInfo )
        throw( DWFException );

    ///
    ///         Returns the DWF package information as read from the
    ///         buffer provided which is expected to contain the
    ///         DWF file header in the first 12 bytes.
    ///
    ///\param   pBuffer     The memory buffer from which the DWF header can be extracted.
    ///\param   rInfo       The package information from the DWF header.
    ///\throw   DWFException
    ///
    _DWFTK_API
    static void GetPackageInfo( const unsigned char* pBuffer,
                                tPackageInfo&        rInfo )
        throw( DWFException );

    ///
    ///
    ///         Processes the package manifest, assembles and returns
    ///         an in-memory data structure representation.
    ///
    ///\param   pFilter     Another reader may be provided as a filter
    ///                     for parsing and construction of the DWFManifest.
    ///                     See DWFManifestReader::setFilter() for details.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFManifest& getManifest( DWFManifestReader* pFilter = NULL )
        throw( DWFException );

    ///
    ///         Processes the package manifest using the reader
    ///         callback provided.
    ///
    ///         This allows the user to participate directly in the parsing process.
    ///         No in-memory data representation will be created
    ///         nor cached by the DWFPackageReader implicitly.
    ///
    ///\param   rManifestReader     The reader implementation for the manifest.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void getManifest( DWFManifestReader& rManifestReader )
        throw( DWFException );

    ///
    ///         Returns a stream for reading the archived file.
    ///
    ///\param   zFilename   The full path of the file in the package.
    ///\param   bCache      If \e true, a local copy of the file contents will be
    ///                     generated and cached until this package reader object
    ///                     is destroyed.
    ///
    ///\return  A stream from which the file contents can be read.
    ///         The caller must delete this pointer with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFInputStream* extract( const DWFString& zFilename,
                                     bool             bCache = false )
        throw( DWFException );

    ///
    ///         Returns a builder for creating new sections.
    ///
    ///         The section builder is made available to the various
    ///         document reader implementations (particularly the DWFManifestReader)
    ///         so that strongly typed section objects can be instantiated as needed.
    ///         See the DWFSectionBuilder interface for details.
    ///
    ///\return  The section builder.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFSectionBuilder& getSectionBuilder()
        throw( DWFException );


    ///
    ///         Returns ...
    ///
    ///         ...
    ///
    ///\return  The ...
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFXDWFProperties* getDWFProperties()
        throw( DWFException );

    ///
    ///         Returns ...
    ///
    ///         ...
    ///
    ///\return  The ...
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFXCustomProperties* getCustomProperties()
        throw( DWFException );


    ///
    ///         Returns ...
    ///
    ///         ...
    ///
    ///\return  The ...
    ///\throw   DWFException
    ///
    _DWFTK_API
    OPCCoreProperties* getCoreProperties()
        throw( DWFException );


protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFPackageReader()
        throw();

    ///
    ///         Opens the package file and configures
    ///         the descriptor or stream for reading.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFZipFileDescriptor* _open()
        throw( DWFException );

private:

    //
    //
    //
    void _makeSectionBuilder()
        throw( DWFException );

    //
    //
    //
    void _getManifestFilename( DWFString& zManifestFilename )
        throw( DWFException );

    //
    //
    //
    OPCPart* _getDWFXManifestPart(bool bInitializeStream, bool bLoadRelationships)
        throw( DWFException );

    //
    //
    //
    bool _isOPCSigned()
        throw( DWFException );

    bool _findSignatureResource(OPCRelationshipContainer &oSignatureRels)
        throw( DWFException );

private:
    //
    // necessary to prevent too blatant exposure to accessing passwords
    //
    friend class DWFResource;
    int _requestResourcePassword( DWFResource& rResource, DWFString& zPassword );

protected:

    ///
    ///         Files that have been extracted and requested to be cached
    ///         are stored in temporary files.  These files are tracked by
    ///         this collection.
    ///
    DWFStringKeySkipList<DWFTempFile*> _oLocalFileCache;

private:

    DWFFile                 _zDWFPackage;
    DWFString               _zDWFPassword;
    DWFInputStream*         _pDWFStream;

    DWFXPackage*            _pDWFXPackage;
    tPackageInfo            _tPackageInfo;

    DWFManifest*            _pPackageManifest;
    DWFSectionBuilder*      _pSectionBuilder;
    DWFZipFileIndex         _oZipFileIndex;

    typedef _DWFTK_STD_MAP(DWFString, DWFXFixedPageResourceExtractor*)  _tFixedPageResourceExtractorMap;
    _tFixedPageResourceExtractorMap                                     _oFixedPageResourceExtractorMap;  

    DWFString               _zCachedDWFXManifestName;
    DWFXDWFProperties*      _pDWFXDWFProperties;
    DWFXCustomProperties*   _pDWFXCustomProperties;
    OPCCoreProperties*      _pOPCCoreProperties;
    DWFThreadMutex          _oExtractMutex;

private:

    DWFPackageReader( const DWFPackageReader& );
    DWFPackageReader& operator=( const DWFPackageReader& );
};

}

#endif
