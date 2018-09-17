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


#ifndef _DWFTK_PACKAGE_VERSION_EXTENSION_6_01_H
#define _DWFTK_PACKAGE_VERSION_EXTENSION_6_01_H


///
///\file        dwf/package/writer/extensions/6.01/PackageVersionExtension.h
///\brief       This file contains the DWFPackageVersionExtension class declaration for <b>DWF 6.01 File Format</b>.
///

#ifndef DWFTK_READ_ONLY

#include "dwf/Version.h"
#include "dwf/package/writer/DWF6PackageVersionExtension.h"
namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFPackageVersionTypeInfoExtension   dwf/package/writer/PackageVersionExtension.h     "dwf/package/writer/PackageVersionExtension.h"
///\brief       This is the <b>DWF 6.01 File Format</b> plug-in for the DWFPackageWriter.
///\since       7.0.1
///
///             DWF 6.01 introduced the TYPEINFO file into the DWF File Format.
///             
///             <b>TYPEINFO</b>\par
///             This file must appear immediately after the 12 byte DWF header.
///             This file must always be named as <i>uuid</i>.TYPEINFO 
///             where <i>uuid</i> matches a section interface ID (see DWFInterface.)
///             Actually, the section interface uuid just happened to be convenient 
///             to use for the document type (that is, the document type information,
///             hence, type info -> TYPEINFO.) To reinforce that concept, the class
///             defines document type strings to use at construction time; though
///             they exactly match the section interface uuids as mentioned above.
///
///             For example, <b>75E513A9-6C41-4C91-BAA6-81E593FAAC10.TYPEINFO</b> indicates
///             that this package contains a single EModel section only.  There may be custom
///             section types but there will be no EPlot sections.
///             All of the first 3D DWF files were of this type and had this particular file.
///
class DWFPackageVersionTypeInfoExtension : public DWF6PackageVersionExtension
                                           _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\copydoc     DWFInterface::kzEPlot_ID
    ///         See DWFInterface::kzEPlot_ID
    ///
    static const wchar_t* const kzDocumentType_EPlot;
    ///
    ///\copydoc     DWFInterface::kzEModel_ID
    ///         See DWFInterface::kzEModel_ID
    ///
    static const wchar_t* const kzDocumentType_EModel;

public:

    ///
    ///         Constructor
    ///
    ///\param   zDocumentType       Used to name the TYPEINFO file.
    ///\throw   None
    ///
    _DWFTK_API
    DWFPackageVersionTypeInfoExtension( const DWFString& zDocumentType )
        throw();

    ///
    ///         Destructor
    /// 
    ///\throw   None
    /// 
    _DWFTK_API
    virtual ~DWFPackageVersionTypeInfoExtension()
        throw();

    ///
    ///\copydoc DWFPackageVersionExtension::major()
    ///
#ifdef EMCC
    _DWFTK_API
    unsigned short major_version()
        throw()
    {
        return _DWF_FORMAT_VERSION_TYPEINFO_MAJOR;
    }
#else
    _DWFTK_API
    unsigned short major()
        throw()
    {
        return _DWF_FORMAT_VERSION_TYPEINFO_MAJOR;
    }
#endif
    ///
    ///\copydoc DWFPackageVersionExtension::minor()
    ///
#ifdef EMCC
    _DWFTK_API
    unsigned short minor_version()
        throw()
    {
        return _DWF_FORMAT_VERSION_TYPEINFO_MINOR;
    }
#else
    _DWFTK_API
    unsigned short minor()
        throw()
    {
        return _DWF_FORMAT_VERSION_TYPEINFO_MINOR;
    }
#endif
    ///
    ///\copydoc DWFPackageVersionExtension::addSection()
    ///
    _DWFTK_API
    virtual bool addSection( DWFSection*   pSection,
                             DWFInterface* pInterface )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageVersionExtension::addGlobalSection()
    ///
    _DWFTK_API
    virtual bool addGlobalSection( DWFGlobalSection* pSection )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageVersionExtension::write()
    ///
    _DWFTK_API
    virtual bool write( const DWFString& zSourceProductVendor,
                        const DWFString& zSourceProductName,
                        const DWFString& zSourceProductVersion,
                        const DWFString& zDWFProductVendor,
                        const DWFString& zDWFProductVersion,
                        DWFZipFileDescriptor::teFileMode eCompressionMode )
        throw( DWFException );


    ///
    ///\copydoc DWFPackageVersionExtension::prewriteManifest()
    ///
    _DWFTK_API
    virtual void prewriteManifest( DWF6PackageWriter&            rPackageWriter, 
                                   DWFPackageFileDescriptor&    rPackageDescriptor,
                                   DWFXMLSerializer&            rXMLSerializer,
                                   const DWFString&             rPackagePassword )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageVersionExtension::postwriteManifest()
    ///
    _DWFTK_API
    virtual void postwriteManifest( DWF6PackageWriter&            rPackageWriter, 
                                    DWFPackageFileDescriptor&    rPackageDescriptor,
                                    DWFXMLSerializer&            rXMLSerializer,
                                    const DWFString&             rPackagePassword )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageVersionExtension::postwriteSections()
    ///
    _DWFTK_API
    virtual void postwriteSections( DWF6PackageWriter&            rPackageWriter, 
                                    DWFPackageFileDescriptor&    rPackageDescriptor,
                                    DWFXMLSerializer&            rXMLSerializer,
                                    const DWFString&             rPackagePassword )
        throw( DWFException );

private:

    DWFString   _zDocumentType;
};


    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

_declspec(selectany) const wchar_t* const DWFPackageVersionTypeInfoExtension::kzDocumentType_EPlot     = DWFInterface::kzEPlot_ID;
_declspec(selectany) const wchar_t* const DWFPackageVersionTypeInfoExtension::kzDocumentType_EModel    = DWFInterface::kzEModel_ID;

#endif
#endif


}


#endif
#endif

