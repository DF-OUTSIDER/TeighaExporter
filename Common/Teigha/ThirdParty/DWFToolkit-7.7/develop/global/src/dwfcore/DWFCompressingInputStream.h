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

#ifndef _DWFCORE_DWF_COMPRESSING_INPUT_STREAM_H
#define _DWFCORE_DWF_COMPRESSING_INPUT_STREAM_H

///
///\file        dwfcore/DWFCompressingInputStream.h
///\brief       This header contains the declaration for the DWFCompressingInputStream class.
///

#ifdef HAVE_CONFIG_H
#include "dwfcore/config.h"
#endif

//
// use either the built-in or sytem zlib header
// depending on the pre-processor configuration
//
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
#include "dwfcore/ZipFileDescriptor.h"

#ifndef DWFCORE_COMPRESSING_INPUT_STREAM_COMPRESSION_BUFFER_SIZE
///
///\ingroup     ExternalMacros
///\brief       Defines the size of the overrun buffer to allocate if needed.
///
///             Defines the size of the overrun buffer to allocate if needed.
///             This buffer is created if more bytes are compressed than
///             will fit into the callers read buffer.  These bytes are cached
///             and returned in a subsequent \a read() request.
///
#define DWFCORE_COMPRESSING_INPUT_STREAM_COMPRESSION_BUFFER_SIZE    16384
#endif

namespace DWFCore
{
///
///\class       DWFCompressingInputStream     dwfcore/DWFCompressingInputStream.h  "dwfcore/DWFCompressingInputStream.h"
///\brief       An input stream object that uses zlib to decompress bytes from
///             from another stream.
///\since       1.0.1
///
///             This implementation of the DWFInputStream is used to provide
///             to zlib deflate for compressed bytes sourced by another input stream.
///
class DWFCompressingInputStream : virtual public DWFCoreMemory
								, public DWFInputStream
{
public:
	///
    ///         Constructor
    ///
    ///\param   pInputStream    An input stream from which to stream the decompressed source data.
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pInputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///\throw   None
    ///
	_DWFCORE_API
		DWFCompressingInputStream( DWFInputStream * pInputStream, DWFZipFileDescriptor::teFileMode fileMode, 
							   bool bOwnStream )
		
		throw();
	
	///
    ///         Destructor
    ///
    ///\throw   None
    ///
	_DWFCORE_API
	virtual ~DWFCompressingInputStream()
		throw();
	
    ///
    ///         Used to determine the availablity of data remaining in the stream.
    ///         The byte count returned by this method will report either
    ///         the number of inflated bytes left in the overrun buffer or
    ///         if that buffer is empty, the number of uncompressed bytes remaining
    ///         in the underlying source stream.  Regardless of the source, if zero
    ///         bytes are returned as available, the stream is exhuasted and thus,
    ///         fully compressed.
    ///
    ///\return  The number of bytes available to read from the stream.
    ///\throw   DWFException
    ///
	_DWFCORE_API
	size_t available() const
		throw ( DWFException );
	
	///
    ///         Reads at most \a nBytesToRead compressed bytes from
    ///         into the buffer provided.  
    ///         If the overrun buffer contains inflated data, these will be copied
    ///         into \a pBuffer.  This buffer will always be exhausted prior to 
    ///         subsequent deflation of the uncompressed stream data, even if \a nBytesToRead
    ///         is larger than the number of inflated bytes in the overrun buffer.
    ///         If the overrun buffer is empty, at most \a nBytesToRead bytes will be 
    ///         decompressed from the underlying stream.
    ///         The stream is not required to fill the read buffer but it must always
    ///         report the number of bytes, including zero, that were obtained.  
    ///
    ///\param   pBuffer         A pointer to a block of memory to receive the bytes.
    ///\param   nBytesToRead    The number of bytes to copy into \a pBuffer.
    ///                         This value should not exceed the capacity of the memory block at \a pBuffer.
    ///\return  The number of compressed bytes read into \a pBuffer.
    ///\throw   DWFException
    ///
	_DWFCORE_API
	size_t read ( void * pBuffer, 
				  size_t nBytesToRead )
		throw ( DWFException );
	
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

	DWFInputStream*                                                 _pInputStream;
	bool		                                                _bOwnStream;
	z_stream							_oCompressionStream;
	bool								_bCompressionStreamInit;
	bool								_bCompressionStreamFinished;
	
	unsigned char*                                                  _pSourceBuffer;
	size_t								_nSourceBufferBytes;

	size_t								_nCompressionBytes;
	size_t								_nCompressionBufferSize;
	size_t                                                          _nCompressionBufferOffset;
	size_t                                                          _nCompressedBytesBuffered; 
	unsigned char *	                                                _pCompressionBuffer; 
	
	bool								_bPending;
	DWFZipFileDescriptor::teFileMode                                _oFileMode;
private:
	//
    // Unimplemented methods
    //
    DWFCompressingInputStream( const DWFCompressingInputStream& );
    DWFCompressingInputStream& operator=( const DWFCompressingInputStream& );
};
}

#endif
