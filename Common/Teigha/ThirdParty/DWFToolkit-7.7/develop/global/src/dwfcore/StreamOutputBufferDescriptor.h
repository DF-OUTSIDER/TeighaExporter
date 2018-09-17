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


#ifndef _DWFCORE_STREAM_OUTPUT_BUFFER_DESCRIPTOR_H
#define _DWFCORE_STREAM_OUTPUT_BUFFER_DESCRIPTOR_H


///
///\file        dwfcore/StreamOutputBufferDescriptor.h
///\brief       This header contains the declaration for the DWFStreamOutputBufferDescriptor class.
///


#include "dwfcore/FileDescriptor.h"


namespace DWFCore
{

//
//  fwd declarations
//
class DWFInputStream;
class DWFOutputStream;


///
///\class       DWFStreamOutputBufferDescriptor   dwfcore/StreamOutputBufferDescriptor.h  "dwfcore/StreamOutputBufferDescriptor.h"
///\brief       This provides a descriptor interface to an output buffer.
///\since       1.3.0
///
///             This class provides the file descriptor interface to a output buffer.
///
class DWFStreamOutputBufferDescriptor : virtual public DWFCoreMemory
                                      , public DWFFileDescriptor
{

public:

    typedef enum teReadWriteMode {
        eRead,
        eWrite
    } teReadWriteMode;

public:

    ///
    ///         Constructor
    ///
    ///\param   nInitialBytes   The initial size of the buffer. This will grow as needed.
    ///\throw   None
    ///
    _DWFCORE_API 
    DWFStreamOutputBufferDescriptor( size_t nInitialBytes = 16384 )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFStreamOutputBufferDescriptor()
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
    ///         This clears out the buffer and prepares it for new output.
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    void reset()
        throw();

    ///
    ///\copydoc DWFFileDescriptor::size()
    ///
    _DWFCORE_API 
    size_t size()
        throw( DWFException );

    ///
    ///         Performs a read operation on the associated data
    ///         within the context of the descriptor implementation.
    ///         For this class this always throws an exception.
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
    ///         Performs a seek operation on the associated data within the 
    ///         context of the descriptor implementation. This is valid only
    ///         for \b eRead mode.
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
    ///         Performs a write operation on the associated buffer
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
    ///         Performs a flush operation on the associated data
    ///         within the context of the descriptor implementation.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    virtual void flush()
        throw( DWFException );

    ///
    ///         This provides read-only access to the underlying buffer.
    ///
    ///\return  A const pointer to the buffer.
    ///\throw   None.
    ///
    _DWFCORE_API
    const unsigned char* buffer()
        throw()
    {
        return _pBuffer;
    }

private:

    //
    //  The beginning of the output buffer
    //
    unsigned char*      _pBuffer;
    //
    //  The current location of the output pointer
    //
    unsigned char*      _pOutPtr;
    //
    //  The endpoint of the data. Unless the out pointer has been
    //  moved back using seek, usually this and _pOutPtr are the same.
    //
    unsigned char*      _pEndPtr;

    size_t              _nBufferBytes;
    size_t              _nInitialBytes;

private:

    //
    // Unimplemented methods
    //
    DWFStreamOutputBufferDescriptor( const DWFStreamOutputBufferDescriptor& );
    DWFStreamOutputBufferDescriptor& operator=( const DWFStreamOutputBufferDescriptor& );
};

}


#endif


