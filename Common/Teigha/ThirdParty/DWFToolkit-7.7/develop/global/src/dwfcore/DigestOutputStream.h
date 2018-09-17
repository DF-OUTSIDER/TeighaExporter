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


#ifndef _DWFCORE_DIGEST_OUTPUT_STREAM_H
#define _DWFCORE_DIGEST_OUTPUT_STREAM_H


///
///\file        dwfcore/DigestOutputStream.h
///\brief       This header contains the declaration for the DWFDigestOutputStream interface and DWFDigestOutputStreamT class.
///

#include "dwfcore/Digest.h"
#include "dwfcore/OutputStream.h"

namespace DWFCore
{

///
///\class       DWFDigestOutputStream    dwfcore/DigestOutputStream.h "dwfcore/DigestOutputStream.h"
///\brief       The interface for an output stream object that computes a digest for a complete sequence of bytes written.
///\since       1.2
///
///             Implementations of this can be used to determine a digest (MD5, SHA-1, 
///             SHA256, etc.) for the complete sequence of bytes that are written to another 
///             linked [chained] output stream. The bytes essentially pass through this 
///             implementation for digest computation.
///
class DWFDigestOutputStream : virtual public DWFCoreMemory
                            , public DWFOutputStream
{

public:

    ///
    ///         Constructor. 
    ///         If the default constructor is used, then the user must set the
    ///         digest and the output stream using \a setDigest and \a chainOutputStream
    ///         respectively, otherwise an exception will be thrown.
    ///
    _DWFCORE_API
    DWFDigestOutputStream()
        throw();

    ///
    ///         Constructor.
    ///         If either the digest or the output stream is NULL, then it must be set
    ///         using \a setDigest or \a chainOutputStream respectively.
    ///
    ///\param   pDigest         A pointer to the digest object that will be used to determine
    ///                         the digest of the stream data. This digest stream will take ownership
    ///                         of the digest. The digest should be allocated with the
    ///                         \b DWFCORE_ALLOC_OBJECT macro.
    ///\param   pOutputStream   A pointer to a stream to chain to this object.
    ///                         If this pointer is NULL and another stream was previously
    ///                         chained to this object, it will be cleared (and deleted
    ///                         if owned.)
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro.
    ///                         if ownership will be transferred.               
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pOutputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pInputStream
    ///                         and is responsible for deleting it.
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    DWFDigestOutputStream( DWFDigest* pDigest,
                           DWFOutputStream* pOutputStream,
                           bool bOwnStream )
        throw();

    ///
    ///         Destructor
    ///
    _DWFCORE_API
    virtual ~DWFDigestOutputStream()
        throw();

    ///
    ///         Set the digest object to use to evaluate the digest of the streamed data. Any
    ///         This will delete any digest that the digest stream may already own.
    ///
    ///\param   pDigest         A pointer to the digest object that will be used to determine
    ///                         the digest of the stream data. This digest stream will take ownership
    ///                         of the new digest which should be allocated with the
    ///                         \b DWFCORE_ALLOC_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    void setDigest( DWFDigest* pDigest )
        throw();

    ///
    ///         Attaches (or detaches) another output stream to this object. Chaining simply refers
    ///         to the process of using one stream as the source for another.  Any number of these
    ///         objects can be chained together to add functionality and data modification in a
    ///         single \a write() invocation.
    ///
    ///\param   pOutputStream   A pointer to a stream to chain to this object.
    ///                         If this pointer is NULL and another stream was previously
    ///                         chained to this object, it will be cleared (and deleted
    ///                         if owned.)
    ///                         This object should be allocated with the \b DWFCORE_ALLOC_OBJECT macro.
    ///                         if ownership will be transferred.
    ///\param   bOwnStream      If \e true, this object will assume ownership of \a pOutputStream 
    ///                         and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                         If \e false, the caller retains ownership of \a pOutputStream
    ///                         and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void chainOutputStream( DWFOutputStream* pOutputStream,
                                    bool             bOwnStream )
        throw();

    ///
    ///         This returns whether or not the underlying digest is progressive, that is, the
    ///         digest can be returned on the number of bytes streamed so far, and still be updated.
    ///         If a digest is not progressive, \a digest() should be called only after all bytes
    ///         have been streamed through the digest stream.
    ///
    ///\return  True if the digest supports \a digest() calls while bytes are being streamed.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual bool isDigestProgressive()
        throw( DWFException );

    ///
    ///         Return the digest computed on the bytes written. If the digest being used is not
    ///         progressive, this should not be called until all bytes have been written.
    ///         If no output stream has been linked, this will throw a DWFIllegalStateException.
    ///
    ///\return  The digest. The length depends on the algorithm used.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFString digest()
        throw( DWFException );


    ///
    ///         Return the digest computed on the bytes written, in terms of the raw digest bytes.
    ///         The bytes are return in a array of unsigned characters that must be deleted by
    ///         the caller using DWFCORE_FREE_MEMORY().
    ///         If no output stream has been linked, this will throw a DWFIllegalStateException.
    ///
    ///\param   rpBytes     The array of bytes will be allocated and returned via this
    ///                     pointer. It should be initialized to NULL before calling this.
    ///\return  The size of the character array holding the raw bytes of the digest. 
    ///         The length depends on the algorithm used.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual size_t digestRawBytes( unsigned char*& rpBytes )
        throw( DWFException );

    ///
    ///         Return the digest computed on the bytes written, in base 64 encoding.
    ///         If no output stream has been linked, this will throw a DWFIllegalStateException.
    ///
    ///\return  The digest in base 64 encoding. The length depends on the algorithm used.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFString digestBase64()
        throw( DWFException );

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
    ///         Writes at most \a nBytesToWrite to the chained stream from the buffer provided.
    ///         The underlying digest computation will be updated in the process.
    ///         If no output stream has been linked, this will throw a DWFIllegalStateException.
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

private:
        
    DWFDigest*          _pDigest;

    DWFOutputStream*    _pOutputStream;
    bool                _bOwnStream;

private:

    //
    // Unimplemented methods
    //

    DWFDigestOutputStream( const DWFDigestOutputStream& );
    DWFDigestOutputStream& operator=( const DWFDigestOutputStream& );

};

}

#endif


