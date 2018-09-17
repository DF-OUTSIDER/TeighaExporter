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


#ifndef _DWFCORE_STANDARD_FILE_DESCRIPTOR_ANSI_H
#define _DWFCORE_STANDARD_FILE_DESCRIPTOR_ANSI_H


///
///\file        dwfcore/ansi/StandardFileDescriptor.h
///\brief       This header contains the DWFStandardFileDescriptor class declaration for all platforms.
///


#include <fcntl.h>
#include "dwfcore/String.h"
#include "dwfcore/FileDescriptor.h"



namespace DWFCore
{

//
// Generic file descriptor implementation
//
class DWFStandardFileDescriptor : public DWFFileDescriptor
{

public:

    ///
    ///         Constructor
    ///
    ///         This constructor uses the \a open() function.
    ///
    ///\param   rFile       The file on which this descriptor will operate.
    ///\param   eIOFlags    Flag or combination of flags indicating how the file will be accessible.
    ///\param   eIOMode     Specifies the permissions to use when creating a new file.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFStandardFileDescriptor( DWFFile& rFile,
                               int      eIOFlags,
                               int      eIOMode )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor consumes a file handle/descriptor identifier.
    ///
    ///\param   rFile       The file on which this descriptor will operate.
    ///\param   iHandle     The handle.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFStandardFileDescriptor( DWFFile& rFile,
                               int      iHandle )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API 
    virtual ~DWFStandardFileDescriptor()
        throw();

    ///
    ///\copydoc DWFFileDescriptor::open()
    ///
    _DWFCORE_API 
    void open()
        throw( DWFException );

    ///
    ///\copydoc DWFFileDescriptor::close()
    ///
    _DWFCORE_API 
    void close()
        throw( DWFException );

    ///
    ///\copydoc DWFFileDescriptor::size()
    ///
    _DWFCORE_API 
    size_t size()
        throw( DWFException );

    ///
    ///         Performs a read operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\param   pBuffer         A pointer to a block of memory to receive the bytes.
    ///                         Any implementation receiving a NULL buffer pointer should throw a
    ///                         DWFInvalidArgumentException.
    ///\param   nBytesToRead    The number of bytes to copy into \a pBuffer.
    ///                         This value should not exceed the capacity of the memory block at \a pBuffer.
    ///\return  The number of actual bytes read.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    size_t read( void* pBuffer, size_t nBytesToRead )
        throw( DWFException );

    ///
    ///         Performs a seek operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\param   eOrigin     One of \b SEEK_SET, \b SEEK_CUR or \b SEEK_END.
    ///\param   nOffset     The number of bytes from \a eOrigin to move the internal cursor.
    ///\return  The previous cursor offset before the seek.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    off_t seek( int eOrigin, off_t nOffset )
        throw( DWFException );

    ///
    ///         Performs a write operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\param   pBuffer         A pointer to a block of memory whose data will be written into the stream.
    ///                         Any implementation receiving a NULL buffer pointer should throw a
    ///                         DWFInvalidArgumentException.
    ///\param   nBytesToWrite   The number of bytes to copy from \a pBuffer.
    ///\return  The number of bytes actually written.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    size_t write( const void* pBuffer, size_t nBytesToWrite )
        throw( DWFException );

    ///
    ///         Performs a flush operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    void flush()
    {
        ; // NOP
    }


private:

    int _iHandle;
    int _eIOFlags;
    int _eIOMode;

private:

    //
    // Unimplemented Methods
    //

    DWFStandardFileDescriptor( const DWFStandardFileDescriptor& );
    DWFStandardFileDescriptor& operator=( const DWFStandardFileDescriptor& );
};

}


#endif
