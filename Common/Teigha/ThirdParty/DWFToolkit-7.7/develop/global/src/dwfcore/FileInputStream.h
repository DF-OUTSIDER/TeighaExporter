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


#ifndef _DWFCORE_FILE_INPUT_STREAM_H
#define _DWFCORE_FILE_INPUT_STREAM_H


///
///\file        dwfcore/FileInputStream.h
///\brief       This header contains the declaration for the DWFFileInputStream interface.
///


#include "dwfcore/InputStream.h"
#include "dwfcore/FileDescriptor.h"


namespace DWFCore
{

///
///\class       DWFFileInputStream    dwfcore/FileInputStream.h "dwfcore/FileInputStream.h"
///\brief       An input stream object that provides streaming read functionality from a file descriptor.
///\since       1.0.1
///
class DWFFileInputStream : virtual public DWFCoreMemory
                         , public DWFInputStream
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFFileInputStream()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFFileInputStream()
        throw();

    ///
    ///         Binds this stream to the file descriptor.
    ///
    ///\param   pFileDescriptor A file descriptor from which to stream the source data.
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro
    ///                         if ownership will be transferred.
    ///\param   bOwnDescriptor  If \e true, this object will assume ownership of \a pFileDescriptor 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pFileDescriptor
    ///                         and is responsible for deleting it.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    void attach( DWFFileDescriptor* pFileDescriptor,
                 bool               bOwnDescriptor )
        throw( DWFException );

    ///
    ///         Unbinds this stream from a previously bound file descriptor.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    void detach()
        throw( DWFException );

    ///
    ///         Used to determine the availablity of data
    ///         that can be provided by the stream.  This method is not
    ///         required to report the exact number of bytes that \e will
    ///         be obtain from a subsequent \a read() invocation.
    ///         This method \b must report zero bytes once the stream data
    ///         has been expired.  Once this method returns zero, it may
    ///         never again report a non-zero value.
    ///
    ///\return  The number of bytes available to read from the stream.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    size_t available() const
        throw( DWFException );

    ///
    ///         Reads at most \a nBytesToRead from the stream into the buffer provided.
    ///         The stream is not required to fill the read buffer but it must always
    ///         report the number of bytes, including zero, that were obtained.  
    ///
    ///\param   pBuffer         A pointer to a block of memory to receive the bytes.
    ///\param   nBytesToRead    The number of bytes to copy into \a pBuffer.
    ///                         This value should not exceed the capacity of the memory block at \a pBuffer.
    ///\return  The number of actual bytes read.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    size_t read( void*  pBuffer,
                 size_t nBytesToRead )
        throw( DWFException );

    ///
    ///         Repositions the file descriptor for subsequent read invocations.
    ///
    ///\warning This method may not be supported by all file descriptors.
    ///
    ///\param   eOrigin     One of \b SEEK_SET, \b SEEK_CUR or \b SEEK_END.
    ///\param   nOffset     The number of bytes from \a eOrigin to move the internal cursor.
    ///\return  The previous cursor offset before the seek.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    off_t seek( int    eOrigin,
                off_t  nOffset )
        throw( DWFException );

    ///
    ///         Returns the file descriptor to which this stream is bound.
    ///
    ///\return  The file descriptor.
    ///\throw   DWFException
    ///
    const DWFFileDescriptor* descriptor() const
        throw()
    {
        return _pFileDescriptor;
    }

private:

    bool               _bOwner;
    size_t             _nAvailableBytes;
    DWFFileDescriptor* _pFileDescriptor;

private:

    //
    // Unimplemented methods
    //

    DWFFileInputStream( const DWFFileInputStream& );
    DWFFileInputStream& operator=( const DWFFileInputStream& );
};

}



#endif
