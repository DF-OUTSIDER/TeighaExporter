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


#ifndef _DWFCORE_BUFFER_OUTPUT_STREAM_H
#define _DWFCORE_BUFFER_OUTPUT_STREAM_H


///
///\file        dwfcore/BufferOutputStream.h
///\brief       This header contains the declaration for the DWFBufferOutputStream interface.
///


#include "dwfcore/OutputStream.h"


namespace DWFCore
{

///
///\class       DWFBufferOutputStream   dwfcore/BufferOutputStream.h    "dwfcore/BufferOutputStream.h"
///\brief       An output stream object that caches data and, depending on the configuration,
///             writes it to another stream.
///\since       1.0.1
///
///             This implementation of the DWFOutputStream can be used to
///             provide stream writing functionality for several different data
///             sinks.  This class can be used to accumulate bytes in a fixed
///             or growable memory buffer or to underlying [chained] stream.
///             Chaining streams can be useful for adjusting data flow (I/O buffering)
///             and modifying data on the fly.
///
class DWFBufferOutputStream  : virtual public DWFCoreMemory
                             , public DWFOutputStream
{

public:

    ///
    ///         Constructor
    ///
    ///         Use this constructor to configure with an automatically
    ///         resizing (and internally allocated) memory buffer.
    ///
    ///\param   nInitialBufferBytes Specifies the initial size of the internal buffer.
    ///\param   nMaxBufferBytes     Specifics the maximum length to which the internal
    ///                             buffer will be allowed to grow.  \b -1 indicates
    ///                             that no limit will be enforced.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFBufferOutputStream( size_t   nInitialBufferBytes,
                           off_t    nMaxBufferBytes = -1 )
        throw();

    ///
    ///         Constructor
    ///
    ///         Use this constructor to provide a caller-managed buffer.
    ///
    ///\param   pBuffer         A pointer to a memory block into which the stream will write.
    ///                         Providing a NULL pointer to this constructor will generate
    ///                         to exceptions when the stream is used.
    ///\param   nBufferBytes    The number of bytes in the memory block \a pBuffer.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFBufferOutputStream( void*    pBuffer,
                           size_t   nBufferBytes )
        throw();

    ///
    ///         Constructor
    ///
    ///         Use this constructor to provide another output stream as the data sink.
    ///
    ///\param   pOutputStream   An output stream to which data will be written.
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pInputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///\param   nBufferBytes    If non-zero, an internal buffer will be used to cache \a write() invocations.
    ///                         Once this buffer fills or \a flush() is invoked, the bytes will be written
    ///                         into the chained output stream.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFBufferOutputStream( DWFOutputStream* pOutputStream,
                           bool             bOwnStream,
                           size_t           nBufferBytes )
       throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFBufferOutputStream()
        throw();

    ///
    ///         Flushes any remaining bytes from the stream.
    ///         This method only applies if an output stream is chained to this object.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void flush()
        throw( DWFException );

    ///
    ///         Writes at most \a nBytesToWrite into the stream from the buffer provided.
    ///         If this object has been configured with a chained stream and cache buffer,
    ///         this method will first attempt to fill the cache before writing to the
    ///         underlying stream.
    ///
    ///\param   pBuffer         A pointer to a block of memory whose data will be written into the stream.
    ///\param   nBytesToWrite   The number of bytes to copy from \a pBuffer.
    ///\return  The number of bytes actually written.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual size_t write( const void*   pBuffer,
                          size_t        nBytesToWrite )
        throw( DWFException );

    ///
    ///         Returns the total number of bytes written to the stream.
    ///         Depending on the configuration of this object this method
    ///         can be used to determine the size of the memory block returned
    ///         from \a buffer().
    ///
    ///\return  The total bytes written to the stream.
    ///\throw   None
    ///
    virtual size_t bytes() const
        throw()
    {
        return _iBufferPos;
    }

    ///
    ///         Returns a pointer to the internal buffer, if one exists.
    ///         Depending on the configuration, this may be some or all
    ///         of the bytes that were written to the stream.
    ///         This pointer must not be deleted by the caller.
    ///
    ///\return  A pointer to the buffer.
    ///\throw   None
    ///
    virtual const void* buffer() const
        throw()
    {
        return _pBuffer;
    }

    ///
    ///         Makes a copy of the internal buffer, if one exists.
    ///         The caller should release this buffer with the \b DWFCORE_FREE_MEMORY macro.
    ///
    ///\param   ppBuffer    Receives allocated buffer.
    ///\return  The number of bytes in the buffer.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual size_t buffer( void** ppBuffer ) const
        throw( DWFException );

protected:

    void*               _pBuffer;

    size_t              _iBufferPos;
    size_t              _nBufferBytes;
    size_t              _nBufferBytesRemaining;

private:

    DWFOutputStream*    _pOutputStream;


    bool                _bOwnStream;
    off_t               _nMaxBufferBytes;
    size_t              _nInitialBufferBytes;

private:

    //
    // Unimplemented methods
    //

    DWFBufferOutputStream( const DWFBufferOutputStream& );
    DWFBufferOutputStream& operator=( const DWFBufferOutputStream& );
};

}



#endif
