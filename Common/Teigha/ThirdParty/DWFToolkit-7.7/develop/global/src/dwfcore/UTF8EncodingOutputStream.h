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


#ifndef _DWFCORE_UTF8_ENCODING_OUTPUT_STREAM_H
#define _DWFCORE_UTF8_ENCODING_OUTPUT_STREAM_H

///
///\file        dwfcore/UTF8EncodingOutputStream.h
///\brief       This header contains the declaration for the DWFUTF8EncodingOutputStream class.
///

#include "dwfcore/OutputStream.h"


namespace DWFCore
{

///
///\class       DWFUTF8EncodingOutputStream     dwfcore/UTF8EncodingOutputStream.h  "dwfcore/UTF8EncodingOutputStream.h"
///\brief       This stream implemented performs UTF-8 character encoding on all data that is written to it.
///\since       1.0.1
///
///             This class must be used with another DWFOutputStream.  As data is written into this
///             stream, it will be UTF-8 encoded and written into the chained stream.
///
class DWFUTF8EncodingOutputStream : virtual public DWFCoreMemory
                                  , public DWFOutputStream
{

public:

    ///
    ///         Constructor
    ///
    ///\param   pOutputStream   An output stream to which encoded data will be written.
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pInputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API 
    DWFUTF8EncodingOutputStream( DWFOutputStream* pOutputStream = NULL,
                                 bool             bOwnStream = false )
       throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API 
    virtual ~DWFUTF8EncodingOutputStream()
        throw();

    ///
    ///\copydoc DWFOutputStream::flush()
    ///
    _DWFCORE_API 
    void flush()
        throw( DWFException );

    ///
    ///\copydoc DWFOutputStream::write()
    ///
    _DWFCORE_API 
    virtual size_t write( const void*   pBuffer,
                          size_t        nBytesToWrite )
        throw( DWFException );

    ///
    ///         Returns the total number of encoded bytes written to the destination stream.
    ///         This is not the number of original bytes written to this stream.
    ///
    ///\return  The total number of encoded bytes written.
    ///\throw   None
    ///
    _DWFCORE_API 
    size_t bytes() const
        throw();

    ///
    ///         Binds a new destination stream to this object.  Any previously
    ///         bound stream will be replaced; and deleted if this stream
    ///         currently owns it.
    ///
    ///\param   pOutputStream   An output stream to which encoded data will be written.
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pInputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API 
    void attach( DWFOutputStream* pOutputStream,
                 bool             bOwnStream )
        throw();

    ///
    ///         Unbinds a destination stream from this objec and deletes it if
    ///         this stream currently owns it.
    ///
    ///\throw   None
    ///
    _DWFCORE_API 
    void detach()
        throw();

private:

    DWFOutputStream*    _pOutputStream;
    bool                _bOwnStream;
    size_t              _nBytesEncoded;

    char*               _pBuffer;
    size_t              _nBufferBytes;

private:

    //
    // Unimplemented methods
    //
    DWFUTF8EncodingOutputStream( const DWFUTF8EncodingOutputStream& );
    DWFUTF8EncodingOutputStream& operator=( const DWFUTF8EncodingOutputStream& );
};

}



#endif
