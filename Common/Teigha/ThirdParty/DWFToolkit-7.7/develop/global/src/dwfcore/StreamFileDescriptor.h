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


#ifndef _DWFCORE_STREAM_FILE_DESCRIPTOR_H
#define _DWFCORE_STREAM_FILE_DESCRIPTOR_H


///
///\file        dwfcore/StreamFileDescriptor.h
///\brief       This header contains the declaration for the DWFStreamFileDescriptor class.
///


#include "dwfcore/String.h"
#include "dwfcore/FileDescriptor.h"


namespace DWFCore
{

///
///\class       DWFStreamFileDescriptor   dwfcore/StreamFileDescriptor.h  "dwfcore/StreamFileDescriptor.h"
///\brief       Descriptor implementing file stream access and control.
///\since       1.0.1
///
///             This class provides access and control to a disk file
///             using the standard c runtime file stream API.
///
///\note        This class has platform-specific implementations.
///\ingroup     CrossPlatformImpl
///
class DWFStreamFileDescriptor : virtual public DWFCoreMemory
                              , public DWFFileDescriptor
{

public:

    ///
    ///         Constructor
    ///
    ///\param   rFile           The file on which this descriptor will operate.
    ///\param   zStreamIOFlags  The same flags that can be passed to \e fopen()
    ///                         that set the file mode should be used here.
    ///\throw   None
    ///
    _DWFCORE_API 
    DWFStreamFileDescriptor( const DWFFile&   rFile,
                             const DWFString& zStreamIOFlags )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFStreamFileDescriptor()
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
    virtual void flush()
        throw( DWFException );

private:

    FILE*       _fp;
    size_t      _nFilesize;
    DWFString   _zFlags;

private:

    //
    // Unimplemented methods
    //

    DWFStreamFileDescriptor();
    DWFStreamFileDescriptor( const DWFStreamFileDescriptor& );
    DWFStreamFileDescriptor& operator=( const DWFStreamFileDescriptor& );
};

}


#endif
