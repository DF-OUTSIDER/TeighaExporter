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


#ifndef _DWFCORE_FILE_DESCRIPTOR_H
#define _DWFCORE_FILE_DESCRIPTOR_H

///
///\file        dwfcore/FileDescriptor.h
///\brief       This header contains the declaration for the DWFFileDescriptor class.
///

#include "dwfcore/File.h"


namespace DWFCore
{

///
///\interface   DWFFileDescriptor   dwfcore/FileDescriptor.h  "dwfcore/FileDescriptor.h"
///\brief       Interface for file description and control.
///\since       1.0.1
///
///             This interface is intended as a common way to interact with any
///             type of file in just about any way.  Implementations may simply
///             wrap standard runtime behavior (e.g. DWFStreamFileDescriptor)
///             or may integrate type-specific functionality (e.g. DWFZipFileDescriptor.)
///
class DWFFileDescriptor
{

public:

    ///
    ///         Constructor
    ///
    ///\param   rFile   The file to associate with this descriptor.
    ///                 This object will be copied locally.
    ///\throw   None
    ///
     
    DWFFileDescriptor( const DWFFile& rFile )
        throw() 
        : _oFile( rFile )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    
    virtual ~DWFFileDescriptor()
        throw()
    {;}

    ///
    ///         Returns the file associated with this descriptor.
    ///
    ///\return  The file.
    ///\throw   None
    ///
    
    const DWFFile& file() const
        throw()
    {
        return _oFile;
    }

    ///
    ///         Performs an open operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void open()
        throw( DWFException ) = 0;

    ///
    ///         Performs an close operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void close()
        throw( DWFException ) = 0;

    ///
    ///         Returns the size of the associated file, in bytes,
    ///         within the context of the descriptor implementation.
    ///
    ///\return  The size of the file, in bytes.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual size_t size()
        throw( DWFException ) = 0;

    ///
    ///         Performs a read operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\note    This method may not be supported by all descriptors.
    ///\warning This method is not implemented and will always throw an exception if invoked.
    ///
    ///\param   pBuffer         A pointer to a block of memory to receive the bytes.
    ///                         Any implementation receiving a NULL buffer pointer should throw a
    ///                         DWFInvalidArgumentException.
    ///\param   nBytesToRead    The number of bytes to copy into \a pBuffer.
    ///                         This value should not exceed the capacity of the memory block at \a pBuffer.
    ///\return  The number of actual bytes read.
    ///\throw   DWFException
    ///
    
    virtual size_t read( void* pBuffer, size_t nBytesToRead )
        throw( DWFException )
    {
        (void)pBuffer;
        (void)nBytesToRead;

        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Read operation not implemented for this descriptor" );
    }

    ///
    ///         Performs a seek operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\note    This method may not be supported by all descriptors.
    ///\warning This method is not implemented and will always throw an exception if invoked.
    ///
    ///\param   eOrigin     One of \b SEEK_SET, \b SEEK_CUR or \b SEEK_END.
    ///\param   nOffset     The number of bytes from \a eOrigin to move the internal cursor.
    ///\return  The previous cursor offset before the seek.
    ///\throw   DWFException
    ///
    
    virtual off_t seek( int eOrigin, off_t nOffset )
        throw( DWFException )
    {
        (void)eOrigin;
        (void)nOffset;

        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Seek operation not implemented for this descriptor" );
    }

    ///
    ///         Performs a write operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\note    This method may not be supported by all descriptors.
    ///\warning This method is not implemented and will always throw an exception if invoked.
    ///
    ///\param   pBuffer         A pointer to a block of memory whose data will be written into the stream.
    ///                         Any implementation receiving a NULL buffer pointer should throw a
    ///                         DWFInvalidArgumentException.
    ///\param   nBytesToWrite   The number of bytes to copy from \a pBuffer.
    ///\return  The number of bytes actually written.
    ///\throw   DWFException
    ///
     
    virtual size_t write( const void* pBuffer, size_t nBytesToWrite )
        throw( DWFException )
    {
        (void)pBuffer;
        (void)nBytesToWrite;

        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Write operation not implemented for this descriptor" );
    }

    ///
    ///         Performs a flush operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\note    This method may not be supported by all descriptors.
    ///\warning This method is not implemented and will always throw an exception if invoked.
    ///
    ///\throw   DWFException
    ///
    
    virtual void flush()
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Flush operation not implemented for this descriptor" );
    }

protected:

    ///
    ///         The associated file.
    ///
    DWFFile     _oFile;

private:

    //
    // Unimplemented methods
    //

    DWFFileDescriptor( const DWFFileDescriptor& );
    DWFFileDescriptor& operator=( const DWFFileDescriptor& );
};


}

#endif


