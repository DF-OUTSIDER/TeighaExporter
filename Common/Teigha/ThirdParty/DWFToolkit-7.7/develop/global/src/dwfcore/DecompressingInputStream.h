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


#ifndef _DWFCORE_DECOMPRESSING_INPUT_STREAM_H
#define _DWFCORE_DECOMPRESSING_INPUT_STREAM_H


///
///\file        dwfcore/DecompressingInputStream.h
///\brief       This header contains the declaration for the DWFDecompressingInputStream class.
///


    //
    // use either the built-in or sytem zlib header
    // depending on the pre-processor configuration
    //
#ifdef HAVE_CONFIG_H
#include "dwfcore/config.h"
#endif

#ifndef _ZLIB_H
#ifdef  DWFCORE_BUILD_ZLIB
#include "dwfcore/zlib/zlib.h"
#else
#ifdef ANDROID
#include <ZLib/zlib.h>
#else
#include <zlib.h>
#endif
#endif
#endif


#include "dwfcore/InputStream.h"



#ifndef DWFCORE_DECOMPRESSING_INPUT_STREAM_DECOMPRESSION_BUFFER_SIZE
///
///\ingroup     ExternalMacros
///\brief       Defines the size of the overrun buffer to allocate if needed.
///
///             Defines the size of the overrun buffer to allocate if needed.
///             This buffer is created if more bytes are decompressed than
///             will fit into the callers read buffer.  These bytes are cached
///             and returned in a subsequent \a read() request.
///
#define DWFCORE_DECOMPRESSING_INPUT_STREAM_DECOMPRESSION_BUFFER_SIZE    16384
#endif



namespace DWFCore
{

///
///\class       DWFDecompressingInputStream     dwfcore/DecompressingInputStream.h  "dwfcore/DecompressingInputStream.h"
///\brief       An input stream object that uses zlib to decompress bytes from
///             from another stream.
///\since       1.0.1
///
///             This implementation of the DWFInputStream is used to provide
///             zlib inflation for compressed bytes sourced by another input stream.
///
class DWFDecompressingInputStream : virtual public DWFCoreMemory
                                  , public DWFInputStream
{

public:

    ///
    ///         Constructor
    ///
    ///\param   pInputStream    An input stream from which to stream the compressed source data.
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pInputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFDecompressingInputStream( DWFInputStream* pInputStream,
                                 bool            bOwnStream )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFDecompressingInputStream()
        throw();

    ///
    ///         Used to determine the availability of data remaining in the stream.
    ///         The byte count returned is determined in the following order:
    ///         1. the number of inflated bytes left in the overrun buffer if it isn't empty;
    ///         2. if 1 isn't valid, the number of uncompressed bytes, if more than zero, 
    ///            in the z_stream that is pending to inflate will be returned; 
    ///         3. if both 1 and 2 are not valid, the number of the to be inflated bytes in the
    ///            original source input stream will be returned.
    ///         
    ///         NOTE: if the returned count is zero, it means the stream has been exhausted 
    ///               and no more bytes can be read out; if the returned count is more than
    ///               zero, it only means that there are more bytes can be retrieved, but it 
    ///               can NOT indicate the exact bytes that can be read.         
    ///
    ///\return  The number of bytes available to read from the stream.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    size_t available() const
        throw( DWFException );

    ///
    ///         Reads at most \a nBytesToRead uncompressed bytes from
    ///         into the buffer provided.  
    ///         If the overrun buffer contains inflated data, first these bytes
    ///         will be copied into \a pBuffer. If the copied bytes is less 
    ///         then \a nBytesToRead, it will continue to inflate the compressed data
    ///         in the z_stream or in the underlying input stream until the compressed 
    ///         bytes is no less than \a nBytesToRead or there are no more bytes to inflate.
    ///         And the more inflated bytes will be stored in the internal overrun buffer.
    ///         
    ///         NOTE: in one word, this method can be used as it for ordinary DWFInputStream.
    //                If the bytes returned is less than \a nBytesToRead, it indicates
    ///               that there are no more bytes to read next time. Since, the implementation
    ///               of this method tries to decompress bytes as more as possibly to fill 
    ///               the input \a pBuffer.
    ///
    ///\param   pBuffer         A pointer to a block of memory to receive the bytes.
    ///\param   nBytesToRead    The number of bytes to copy into \a pBuffer.
    ///                         This value should not exceed the capacity of the memory block at \a pBuffer.
    ///\return  The number of uncompressed bytes read into \a pBuffer.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    size_t read( void*  pBuffer,
                 size_t nBytesToRead )
        throw( DWFException );

    ///
    ///\warning This method is not implemented and will always throw an exception if invoked.
    ///
    ///\param   eOrigin     One of \b SEEK_SET, \b SEEK_CUR or \b SEEK_END.
    ///\param   nOffset     The number of bytes from \a eOrigin to move the internal cursor.
    ///\return  The previous cursor offset before the seek.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    off_t seek( int     eOrigin,
                off_t   nOffset )
        throw( DWFException );

private:

    DWFInputStream* _pSourceStream;
    bool            _bOwnStream;

    bool            _bDecompressionStreamInit;
    z_stream        _oDecompressionStream;

    size_t          _nSourceBufferBytes;
    unsigned char*  _pSourceBuffer;

    size_t          _nDecompressedBytes;
    size_t          _nDecompressionBufferSize;
    size_t          _nDecompressionBufferOffset;
    size_t          _nDecompressedBytesBuffered;
    unsigned char*  _pDecompressionBuffer;

    bool            _bPending;

private:

    //
    // Unimplemented methods
    //
    DWFDecompressingInputStream( const DWFDecompressingInputStream& );
    DWFDecompressingInputStream& operator=( const DWFDecompressingInputStream& );
};

}



#endif
