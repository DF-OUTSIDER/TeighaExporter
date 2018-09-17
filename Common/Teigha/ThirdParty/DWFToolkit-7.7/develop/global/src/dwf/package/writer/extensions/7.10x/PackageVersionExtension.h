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


#ifndef _DWFTK_PACKAGE_VERSION_EXTENSION_7_10_H
#define _DWFTK_PACKAGE_VERSION_EXTENSION_7_10_H


///
///\file        dwf/package/writer/extensions/7.10x/PackageVersionExtension.h
///\brief       This file contains the DWFPackageVersionExtension class declaration for <b>DWFx 7.10 File Format</b>.
///

#ifndef DWFTK_READ_ONLY

#include "dwf/Version.h"
#include "dwf/package/writer/DWFXPackageVersionExtension.h"
namespace DWFToolkit
{

#define _DWFX_FORMAT_VERSION_710_MAJOR                   7       // Internal Use Only
#define _DWFX_FORMAT_VERSION_710_MINOR                   1       // Internal Use Only

///\ingroup     DWFxFileFormat
///\brief       The current DWFx File Format version number as a wide character string.
///
#define _DWFX_FORMAT_VERSION_710_WIDE_STRING     L"7.10"

///
///\ingroup     dwfpackage
///
///\class       DWFXPackageVersion710Extension   dwf/package/writer/extensions/7.10x/PackageVersionExtension.h     "dwf/package/writer/extensions/7.10x/PackageVersionExtension.h"
///\brief       This is the <b>DWFx 7.10 File Format</b> plug-in for the DWFPackageWriter.
///\since       7.1.0
///
///             THIS EXTENSION IS FOR DWFx files, not DWF.
///             The current DWFx Format version is 7.00. This version extension allows
///             DWFx 7.10 files to be created. DWF 7.10 files can contain new features such
///             as digital signatures and PDF import.
///             
///
class DWFXPackageVersion710Extension : public DWFXPackageVersionExtension
                                     _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFXPackageVersion710Extension()
        throw()
    {
        ;
    }

    ///
    ///         Destructor
    /// 
    ///\throw   None
    /// 
    _DWFTK_API
    virtual ~DWFXPackageVersion710Extension()
        throw()
    {
        ;
    }

    ///
    ///\copydoc DWFXPackageVersion710Extension::major()
    ///
#ifdef EMCC
    _DWFTK_API
    unsigned short major_version()
        throw()
    {
        return _DWFX_FORMAT_VERSION_710_MAJOR;
    }
#else
    _DWFTK_API
    unsigned short major()
        throw()
    {
        return _DWFX_FORMAT_VERSION_710_MAJOR;
    }
#endif
    ///
    ///\copydoc DWFXPackageVersion710Extension::minor()
    ///
#ifdef EMCC
    _DWFTK_API
    unsigned short minor_version()
        throw()
    {
        return _DWFX_FORMAT_VERSION_710_MINOR;
    }
#else
    _DWFTK_API
    unsigned short minor()
        throw()
    {
        return _DWFX_FORMAT_VERSION_710_MINOR;
    }
#endif

    ///
    ///\copydoc DWFXPackageVersion710Extension::versionAsWideString()
    ///
    ///
    _DWFTK_API
    DWFString extensionVersionAsWideString()
		throw()
    {
        return DWFString(_DWFX_FORMAT_VERSION_710_WIDE_STRING);
    }

};

}


#endif
#endif

