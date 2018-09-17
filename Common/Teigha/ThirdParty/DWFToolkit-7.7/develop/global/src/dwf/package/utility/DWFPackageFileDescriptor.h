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


#ifndef _DWFTK_PACKAGE_FILE_DESCRIPTOR_H
#define _DWFTK_PACKAGE_FILE_DESCRIPTOR_H

///
///\file        dwf/package/utility/DWFPackageFileDescriptor.h
///\brief       This file contains the DWFPackageFileDescriptor class declaration.
///


#ifndef DWFTK_READ_ONLY

#include "dwfcore/ZipFileDescriptor.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"


//
//
//
namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFPackageFileDescriptor   dwf/package/utility/DWFPackageFileDescriptor.h     "dwf/package/utility/DWFPackageFileDescriptor.h"
///\brief       A special file descriptor for DWF package files.
///\since       7.0.1
///
///             This file descriptor is used for writing DWF packages to disk.
///             It adds the DWF file header and provides compression options that
///             the DWFCore::DWFZipFileDescriptor does not.
///
class DWFPackageFileDescriptor : public DWFZipFileDescriptor
                                 _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\param   rFile           The DWF package to create.
    ///\param   eMode           An enumeration that adjusts the zip library compression options.
    ///\param   nVersionMajor   The DWF File Format major version to create (default is the current version.)
    ///\param   nVersionMinor   The DWF File Format minor version to create (default is the current version.)
    ///\throw   None
    ///
    _DWFTK_API
    DWFPackageFileDescriptor( const DWFFile&                   rFile,
                              DWFZipFileDescriptor::teFileMode eMode = DWFZipFileDescriptor::eZipSmallest,
                              unsigned short                   nVersionMajor = _DWF_FORMAT_VERSION_CURRENT_MAJOR,
                              unsigned short                   nVersionMinor = _DWF_FORMAT_VERSION_CURRENT_MINOR )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPackageFileDescriptor()
        throw();

    ///
    ///\copydoc DWFCore::DWFZipFileDescriptor::open();
    ///
    _DWFTK_API
    void open()
        throw( DWFException );

private:

    unsigned short _nMajor;
    unsigned short _nMinor;

private:

    DWFPackageFileDescriptor();
    DWFPackageFileDescriptor( const DWFPackageFileDescriptor& );
    DWFPackageFileDescriptor& operator=( const DWFPackageFileDescriptor& );
};

}


#endif
#endif
