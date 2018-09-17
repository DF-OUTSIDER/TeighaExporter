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


#ifndef _DWFCORE_FILE_OUTPUT_STREAM_H
#define _DWFCORE_FILE_OUTPUT_STREAM_H

///
///\file        dwfcore/FileOutputStream.h
///\brief       This header contains the declaration for the DWFFileOutputStream interface.
///


#include "dwfcore/OutputStream.h"
#include "dwfcore/FileDescriptor.h"


namespace DWFCore
{

///
///\class       DWFFileOutputStream    dwfcore/FileOutputStream.h "dwfcore/FileOutputStream.h"
///\brief       An output stream object that provides streaming write functionality to a file descriptor.
///\since       1.0.1
///
class DWFFileOutputStream : virtual public DWFCoreMemory
                          , public DWFOutputStream
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFFileOutputStream()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFFileOutputStream()
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
    ///         Flushes any remaining bytes from the stream into the file descriptor.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API void flush()
        throw( DWFException );

    ///
    ///         Writes at most \a nBytesToWrite into the stream from the buffer provided.
    ///
    ///\param   pBuffer         A pointer to a block of memory whose data will be written into the stream.
    ///\param   nBytesToWrite   The number of bytes to copy from \a pBuffer.
    ///\return  The number of bytes actually written.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    size_t write( const void*   pBuffer,
                  size_t        nBytesToWrite )
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
    DWFFileDescriptor* _pFileDescriptor;

private:

    //
    // Unimplemented methods
    //

    DWFFileOutputStream( const DWFFileOutputStream& );
    DWFFileOutputStream& operator=( const DWFFileOutputStream& );
};

}



#endif
