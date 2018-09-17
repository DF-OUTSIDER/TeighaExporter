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


#ifndef _DWFCORE_BUFFER_INPUT_STREAM_H
#define _DWFCORE_BUFFER_INPUT_STREAM_H


///
///\file        dwfcore/BufferInputStream.h
///\brief       This header contains the declaration for the DWFBufferInputStream interface.
///

#include "dwfcore/InputStream.h"


namespace DWFCore
{

///
///\class       DWFBufferInputStream    dwfcore/BufferInputStream.h "dwfcore/BufferInputStream.h"
///\brief       An input stream object that uses either a memory buffer or
///             another input stream as it's data source.
///\since       1.0.1
///
///             This implementation of the DWFInputStream can be used to provide
///             streaming read access to a memory buffer.  This class can also be 
///             used to link [chain] another input stream or streams to a single interface.
///
class DWFBufferInputStream  : virtual public DWFCoreMemory
                            , public DWFInputStream
{

public:

    ///
    ///         Constructor
    ///
    ///         Use this constructor to configure with a memory buffer source.
    ///
    ///\param   pBuffer         A memory block from which to stream data.
    ///\param   nBufferBytes    The number of bytes in the memory block \a pBuffer.
    ///\param   bOwnBuffer      If \e true, this object will assume ownership of \a pBuffer 
    ///                         and delete it as necessary using \b DWFCORE_FREE_MEMORY.
    ///                         If \e false (default), the caller retains ownership of \a pBuffer
    ///                         and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFBufferInputStream( const void*   pBuffer,
                          size_t        nBufferBytes,
                          bool          bOwnBuffer=false)
        throw();

    ///
    ///         Constructor
    ///
    ///         Use this constructor to configure with another stream source.
    ///
    ///\param   pInputStream    An input stream from which to stream data.
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pInputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFBufferInputStream( DWFInputStream* pInputStream,
                          bool            bOwnStream )
       throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFBufferInputStream()
        throw();

    ///
    ///         Used to determine the availablity of data that may be
    ///         provided from the underlying memory buffer or input stream.
    ///
    ///\return  The number of bytes available to read from the stream.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual size_t available() const
        throw( DWFException );

    ///
    ///         Reads at most \a nBytesToRead from the underlying memory buffer
    ///         or input stream into the buffer provided.
    ///         The stream is not required to fill the read buffer but it must always
    ///         report the number of bytes, including zero, that were obtained.  
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
    virtual size_t read( void*  pBuffer,
                         size_t nBytesToRead )
        throw( DWFException );

    ///
    ///         Repositions the internal cursor for subsequent read invocations.
    ///         This method may fail and throw an exception if the request either
    ///         exceeds the bounds of the underlying memory buffer, or exhausts the
    ///         underlying stream or the underlying stream does not support the method itself.
    ///
    ///\param   eOrigin     One of \b SEEK_SET, \b SEEK_CUR or \b SEEK_END.
    ///\param   nOffset     The number of bytes from \a eOrigin to move the internal cursor.
    ///\return  The previous cursor offset before the seek.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual off_t seek( int     eOrigin,
                        off_t   nOffset )
        throw( DWFException );

    ///
    ///         Attaches (or detaches) another input stream to object.
    ///         Chaining simply refers to the process of using one stream
    ///         as the source for another.  Any number of these objects can be chained 
    ///         together to add functionality and data modification a single \a read() invocation.
    ///
    ///\param   pInputStream    A pointer to a stream to chain to this object.
    ///                         If this pointer is NULL and another stream was previously
    ///                         chained to this object, it will be cleared (and deleted
    ///                         if owned.)
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro.
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pInputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///
    _DWFCORE_API
    void chainInputStream( DWFInputStream* pInputStream,
                           bool            bOwnStream )
       throw();

protected:

    const void*     _pBuffer;
    size_t          _iBufferPos;
    size_t          _nBufferBytes;
    size_t          _nAvailableBytes;

private:


    DWFInputStream* _pChainedStream;
    bool            _bOwnStream;
    bool            _bOwnBuffer;

private:

    //
    // Unimplemented methods
    //

    DWFBufferInputStream();
    DWFBufferInputStream( const DWFBufferInputStream& );
    DWFBufferInputStream& operator=( const DWFBufferInputStream& );
};

}



#endif
