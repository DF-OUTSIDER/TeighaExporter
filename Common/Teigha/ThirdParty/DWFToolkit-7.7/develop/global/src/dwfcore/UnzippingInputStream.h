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


#ifndef _DWFCORE_UNZIPPING_INPUT_STREAM_H
#define _DWFCORE_UNZIPPING_INPUT_STREAM_H

///
///\file        dwfcore/UnzippingInputStream.h
///\brief       This header contains the declaration for the DWFUnzippingInputStream class.
///


#include "dwfcore/String.h"
#include "dwfcore/InputStream.h"
#include "dwfcore/ZipFileDescriptor.h"
#include "dwfcore/zip/unzip.h"
#include "dwfcore/Owner.h"


namespace DWFCore
{


///
///\class       DWFUnzippingInputStream     dwfcore/UnzippingInputStream.h  "dwfcore/UnzippingInputStream.h"
///\brief       An input stream object that uses the zip library to extract file data from an archive.
///\since       1.0.1
///
class DWFUnzippingInputStream : virtual public DWFCoreMemory
                              , public DWFInputStream
                              , public DWFOwner
{

public:

    ///
    ///         Constructor
    ///
    ///\param   pUnzipStream    This data structure representing the file stream
    ///                         is obtained from the zip library.
    ///\throw   None
    ///
    _DWFCORE_API 
    DWFUnzippingInputStream( unzFile pUnzipStream )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API 
    virtual ~DWFUnzippingInputStream()
        throw();

    ///
    ///         Prepares the stream to extract the given file data from the archive.
    ///
    ///\param   zArchivedFile   The full path name of the file in the archive.
    ///\param   zPassword       The password required to access the protected zip archive.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    void open( const DWFString& zArchivedFile,
               const DWFString& zPassword = "" )
        throw( DWFException );

    ///
    ///\copydoc DWFInputStream::available()
    ///         The number of bytes is based on the \b uncompressed file size.
    ///
    _DWFCORE_API 
    size_t available() const
        throw( DWFException );

    ///
    ///\copydoc DWFInputStream::read()
    ///
    _DWFCORE_API 
    size_t read( void*  pBuffer, size_t nBytesToRead )
        throw( DWFException );

    ///
    ///\copydoc DWFInputStream::seek()
    ///
    _DWFCORE_API 
    off_t seek( int eOrigin, off_t nOffset )
        throw( DWFException );

    _DWFCORE_API
    void attach( DWFZipFileDescriptor* pFileDescriptor, bool bOwnDescriptor )
        throw();

    ///
    ///\copydoc DWFOwner::notifyOwnerChanged
    ///
    _DWFCORE_API
    virtual void notifyOwnerChanged( DWFOwnable& rOwnable )
        throw( DWFException );

    ///
    ///\copydoc DWFOwner::notifyOwnableDeletion
    ///
    _DWFCORE_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );
private:

    bool    _bFileOpen;
    size_t  _nFileBytesRemaining;
    unzFile _pUnzipStream;
    DWFZipFileDescriptor*   _pDescriptor;
    bool                    _bOwnDescriptor;

private:

    //
    // Unimplemented methods
    //

    DWFUnzippingInputStream( const DWFUnzippingInputStream& );
    DWFUnzippingInputStream& operator=( const DWFUnzippingInputStream& );
};

}



#endif
