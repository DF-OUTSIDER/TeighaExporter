//
//  Copyright (c) 2006 by Autodesk, Inc.
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


#ifndef _DWFCORE_ENCRYPTING_INPUT_STREAM_H
#define _DWFCORE_ENCRYPTING_INPUT_STREAM_H


///
///\file        dwfcore/DWFEncryptingInputStream.h
///\brief       This header contains the declaration for the DWFEncryptingInputStream class.
///

#include "dwfcore/InputStream.h"
#include "dwfcore/String.h"

namespace DWFCore
{

///
///\class       DWFEncryptingInputStream    dwfcore/DWFEncryptingInputStream.h "dwfcore/DWFEncryptingInputStream.h"
///\brief       The interface for an input stream object that encrypts a password string.
///\since       1.5
///
///
class DWFEncryptingInputStream : public DWFInputStream
{

public:

    ///
    ///         Constructor.
    ///
    ///\param   pInputStream    A pointer to a stream to chain to this object.
    ///                         If this pointer is NULL and another stream was previously
    ///                         chained to this object, it will be cleared (and deleted
    ///                         if owned.)
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro.           
    ///\param   rPassword       The password string that will be encrypted.
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFEncryptingInputStream( DWFInputStream* pInputStream,
                              const DWFString& rPassword )
        throw();

    ///
    ///         Destructor
    ///
    _DWFCORE_API
    virtual ~DWFEncryptingInputStream()
        throw();

    ///
    ///         Used to determine the availablity of data from the underlying
    ///         input stream.
    ///
    ///\return  The number of bytes available to read from the stream.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual size_t available() const
        throw( DWFException );

    ///
    ///         Reads at most \a nBytesToRead from the underlying input stream. 
    ///         If no input stream has been linked, this will throw a DWFIllegalStateException.
    ///
    ///\param   pBuffer         A pointer to a block of memory to receive the bytes.
    ///                         Any implementation receiving a NULL buffer pointer should throw a
    ///                         DWFInvalidArgumentException.
    ///\param   nBytesToRead    The number of bytes to copy.
    ///                         This value should not exceed the capacity of the memory block at \a pBuffer.
    ///\return  The number of actual bytes read.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual size_t read( void*  pBuffer,
                         size_t nBytesToRead )
        throw( DWFException );

    ///
    ///         Repositions the internal cursor on the linked input stream 
    ///         for subsequent read invocations.
    ///         This method may fail and throw an exception if the request either
    ///         exceeds the bounds of the underlying memory buffer, or exhausts the
    ///         underlying stream or the underlying stream does not support the method itself.
    ///         If no input stream has been linked, this will throw a DWFIllegalStateException.
    ///
    ///\param   eOrigin     One of \b SEEK_SET, \b SEEK_CUR or \b SEEK_END.
    ///\param   nOffset     The number of bytes from \a eOrigin to move the internal cursor.
    ///\return  The previous cursor offset before the seek.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual off_t seek( int     eOrigin,
                        off_t   nOffset )
        throw( DWFException );

private:

    DWFInputStream* _pInputStream;
	unsigned long   _aKeys[3];    

private:

    //
    // Unimplemented methods
    //

    DWFEncryptingInputStream( const DWFEncryptingInputStream& );
    DWFEncryptingInputStream& operator=( const DWFEncryptingInputStream& );
};

}



#endif


