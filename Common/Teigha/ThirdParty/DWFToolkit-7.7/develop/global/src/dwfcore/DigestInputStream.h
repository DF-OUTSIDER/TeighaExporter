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


#ifndef _DWFCORE_DIGEST_INPUT_STREAM_H
#define _DWFCORE_DIGEST_INPUT_STREAM_H


///
///\file        dwfcore/DigestInputStream.h
///\brief       This header contains the declaration for the DWFDigestInputStream interface and DWFDigestInputStreamT class.
///

#include "dwfcore/Digest.h"
#include "dwfcore/InputStream.h"

namespace DWFCore
{

///
///\class       DWFDigestInputStream    dwfcore/DigestInputStream.h "dwfcore/DigestInputStream.h"
///\brief       The interface for an input stream object that computes a digest for a complete sequence of bytes read.
///\since       1.2
///
///             Implementations of this can be used to determine a digest (MD5, SHA-1, 
///             SHA256, etc.) for the complete sequence of bytes that are read from another 
///             linked [chained] input stream. The bytes essentially pass through this 
///             implementation for digest computation.
///
class DWFDigestInputStream : virtual public DWFCoreMemory
                           , public DWFInputStream
{

public:

    ///
    ///         Constructor. 
    ///         If the default constructor is used, then the user must set the
    ///         digest and the input stream using \a setDigest and \a chainInputStream
    ///         respectively, otherwise an exception will be thrown.
    ///
    _DWFCORE_API
    DWFDigestInputStream()
        throw();

    ///
    ///         Constructor.
    ///         If either the digest or the inputstream is NULL, then it must be set
    ///         using \a setDigest or \a chainInputStream respectively.
    ///
    ///\param   pDigest         A pointer to the digest object that will be used to determine
    ///                         the digest of the stream data. This digest stream will take ownership
    ///                         of the digest. The digest should be allocated with the
    ///                         \b DWFCORE_ALLOC_OBJECT macro.
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
    ///\throw   None
    ///
    _DWFCORE_API
    DWFDigestInputStream( DWFDigest* pDigest,
                          DWFInputStream* pInputStream,
                          bool bOwnStream )
        throw();

    ///
    ///         Destructor
    ///
    _DWFCORE_API
    virtual ~DWFDigestInputStream()
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
    ///         Attaches (or detaches) another input stream to this object. Chaining simply refers
    ///         to the process of using one stream as the source for another.  Any number of these 
    ///         objects can be chained together to add functionality and data modification in a 
    ///         single \a read() invocation.
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
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void chainInputStream( DWFInputStream* pInputStream,
                                   bool            bOwnStream )
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
    ///         Return the digest computed on the bytes read, in hexadecimal form.
    ///         If no input stream has been linked, this will throw a DWFIllegalStateException.
    ///
    ///\return  The digest. The length depends on the algorithm used.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFString digest()
        throw( DWFException );

    ///
    ///         Return the digest computed on the bytes read, in terms of the raw digest bytes.
    ///         The bytes are return in a array of unsigned characters that must be deleted by
    ///         the caller using DWFCORE_FREE_MEMORY().
    ///         If no input stream has been linked, this will throw a DWFIllegalStateException.
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
    ///         Return the digest computed on the bytes read, in base 64 encoding.
    ///         If no input stream has been linked, this will throw a DWFIllegalStateException.
    ///
    ///\return  The digest in base 64 encoding. The length depends on the algorithm used.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual DWFString digestBase64()
        throw( DWFException );

    ///
    ///         Used to determine the availablity of data from the underlying
    ///         input stream.
    ///
    ///\return  The number of bytes available to read from the stream.
    ///
    ///\throw   DWFException
    ///
    virtual size_t available() const
        throw( DWFException )
    {
        return (_pInputStream == NULL) ? 0 : _pInputStream->available();
    }

    ///
    ///         Reads at most \a nBytesToRead from the underlying input stream. 
    ///         The underlying digest computation will be updated in the process.
    ///         If no input stream has been linked, this will throw a DWFIllegalStateException.
    ///
    ///\param   pBuffer         A pointer to a block of memory to receive the bytes.
    ///                         Any implementation receiving a NULL buffer pointer should throw a
    ///                         DWFInvalidArgumentException.
    ///\param   nBytesToRead    The number of bytes to copy into \a pDigest.
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

    DWFDigest*      _pDigest;

    DWFInputStream* _pInputStream;
    bool            _bOwnStream;

private:

    //
    // Unimplemented methods
    //

    DWFDigestInputStream( const DWFDigestInputStream& );
    DWFDigestInputStream& operator=( const DWFDigestInputStream& );
};

}



#endif


