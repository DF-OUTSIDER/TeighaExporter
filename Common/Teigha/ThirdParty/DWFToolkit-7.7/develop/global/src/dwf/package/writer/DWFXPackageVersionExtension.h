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


#ifndef _DWFTK_DWFX_PACKAGE_VERSION_EXTENSION_H
#define _DWFTK_DWFX_PACKAGE_VERSION_EXTENSION_H

///
///\file        dwf/package/writer/DWFXPackageVersionExtension.h
///\brief       This file contains the DWFXPackageVersionExtension interface declaration.
///

#ifndef DWFTK_READ_ONLY

#include "dwfcore/String.h"
#include "dwfcore/Exception.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"

namespace DWFToolkit
{
class DWFXPackageWriter;

///
///\ingroup     dwfpackage
///
///\class       DWFXPackageVersionExtension   dwf/package/writer/DWFXPackageVersionExtension.h     "dwf/package/writer/DWFXPackageVersionExtension.h"
///\brief       This is the plug-in interface for the DWFPackageWriter.
///\since       7.0.1
///
///             This interface is intended to be used to modify the publish-time
///             behavior of the DWFPackageWriter; specifically to capture and
///             implement version-specific differences in the DWF.  
///             As a general rule, version variations may be created to support
///             a new feature or introduce new functionality.  Eventually, these
///             might be removed from the format entirely but the ability
///             to produce the package variations must continue to exist.
///
///             The interface works by mimicking some of the DWFPackageWriter interface.
///             The writer, when used with an extension, will first make the matching
///             call on the extension. This methods must return a boolean value
///             that communicates the continuation intent back to the writer.  
///             If \e true, the writer will continue to process its own call path after
///             returning from the extension.
///             If \e false, the writer will return immediately from its own call path
///             after returning from the extension.
///
///             Note that this is a minimal implementation for DWFx
///             used only to bump up the file version for new features such as digital
///             signatures and PDF import.
///
class DWFXPackageVersionExtension _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///         Destructor
    /// 
    ///\throw   None
    ///
    virtual ~DWFXPackageVersionExtension()
        throw(){;}

    ///
    ///         Returns the DWFx File Format major version for this extension.
    ///
    ///\return  The integer value of the major format version.
    ///\throw   None
    ///
#ifdef EMCC
    _DWFTK_API
    virtual unsigned short major_version()
        throw() = 0;
#else
    _DWFTK_API
    virtual unsigned short major()
        throw() = 0;
#endif

    ///
    ///         Returns the DWFx File Format minor version for this extension.
    ///
    ///\return  The integer value of the minor format version.
    ///\throw   None
    ///
#ifdef EMCC
    _DWFTK_API
    virtual unsigned short minor_version()
        throw() = 0;
#else
    _DWFTK_API
    virtual unsigned short minor()
        throw() = 0;
#endif

    ///
    ///         Returns the DWFx File Format version of this extension as a wide string.
    ///
    ///\return  DWFString* containing the string version of the extension's version.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFString extensionVersionAsWideString()
        throw() = 0;

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFXPackageVersionExtension()
        throw(){;}
};

}

#endif
#endif

