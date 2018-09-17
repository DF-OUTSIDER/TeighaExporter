//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#ifndef _DWFCORE_ZIPPING_OUTPUT_STREAM_H
#define _DWFCORE_ZIPPING_OUTPUT_STREAM_H

///
///\file        dwfcore/ZippingOutputStream.h
///\brief       This header contains the declaration for the DWFZippingOutputStream class.
///

#include "dwfcore/String.h"
#include "dwfcore/OutputStream.h"
#include "dwfcore/ZipFileDescriptor.h"



namespace DWFCore
{

///
///\class       DWFZippingOutputStream     dwfcore/ZippingOutputStream.h  "dwfcore/ZippingOutputStream.h"
///\brief       An output stream object that uses the zip library to archive files.
///             from another stream.
///\since       1.0.1
///
class DWFZippingOutputStream : virtual public DWFCoreMemory
                             , public DWFOutputStream
{

public:

    ///
    ///         Constructor
    ///
    ///\param   pZipStream      This data structure representing the file stream
    ///                         is obtained from the zip library.
    ///\param   eZipMode        This enumeration controls the level of compression
    ///                         to be used when deflating the data.  
    ///                         Note that \a DWFZipFileDescriptor::eUnzip is an invalid
    ///                         option for this method.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFZippingOutputStream( zipFile                            pZipStream,
                            DWFZipFileDescriptor::teFileMode   eZipMode )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API 
    virtual ~DWFZippingOutputStream()
        throw();

    ///
    ///         Prepares the stream to archive the given file.
    ///
    ///\param   zArchiveFile    The full path name of the file in the archive.
    ///\param   zPassword       An optional password to protect the archive.
    ///\param   bPKZIPCompliantPassword  An optional boolean to control whether passwording is PKZIP compliant, or if it should use a more secure, custom, form of passwording. The default is true, meaning compliant.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    void open( const DWFString& zArchiveFile,
               const DWFString& zPassword = "",
               bool bPKZIPCompliantPassword = true )
        throw( DWFException );

    ///
    ///\copydoc DWFOutputStream::flush()
    ///
    _DWFCORE_API 
    void flush()
        throw( DWFException );

    ///
    ///\copydoc DWFOutputStream::write()
    ///
    _DWFCORE_API 
    size_t write( const void*   pBuffer,
                  size_t        nBytesToWrite )
        throw( DWFException );

private:

    bool                                _bFileOpen;
    zipFile                             _pZipStream;
    DWFZipFileDescriptor::teFileMode    _eZipMode;

private:

    //
    // Unimplemented methods
    //

    DWFZippingOutputStream( const DWFZippingOutputStream& );
    DWFZippingOutputStream& operator=( const DWFZippingOutputStream& );
};

}



#endif
