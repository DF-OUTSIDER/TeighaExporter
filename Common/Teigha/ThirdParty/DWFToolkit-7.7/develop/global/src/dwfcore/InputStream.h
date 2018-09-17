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


#ifndef _DWFCORE_INPUT_STREAM_H
#define _DWFCORE_INPUT_STREAM_H


///
///\file        dwfcore/InputStream.h
///\brief       This header contains the declaration for the DWFInputStream interface.
///

#include "dwfcore/Core.h"
#include "dwfcore/Exception.h"


namespace DWFCore
{

///
///\interface   DWFInputStream  dwfcore/InputStream.h   "dwfcore/InputStream.h"
///\brief       Interface for input (reader) streams.
///\since       1.0.1
///
///             This class defines a contract for a streamable data source.
///             Implementors of this interface are considered valid sources 
///             from which input data may be obtained.
///             
///             Generally, the stream consumer would read from the stream
///             until it reports zero available bytes:
///             \code
///             size_t          nBytesRead = 0;
///             DWFInputStream* pInputStream;
///
///             while (pInputStream->available() > 0)
///             {
///                 nBytesRead = pInputStream->read( pBuffer, nBufferBytes );
///             }
///             \endcode
///
class DWFInputStream
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFInputStream()
        throw()
    {;}

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
    virtual size_t available() const
        throw( DWFException ) = 0;

    ///
    ///         Reads at most \a nBytesToRead from the stream into the buffer provided.
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
        throw( DWFException ) = 0;

    ///
    ///         Repositions the internal cursor for subsequent read invocations.
    ///         Not all implementations will support this method; it is recommended
    ///         that a DWFNotImplementedException be thrown if not.
    ///
    ///\warning This method may not be supported by all streams.
    ///
    ///\param   eOrigin     One of \b SEEK_SET, \b SEEK_CUR or \b SEEK_END.
    ///\param   nOffset     The number of bytes from \a eOrigin to move the internal cursor.
    ///\return  The previous cursor offset before the seek.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    virtual off_t seek( int    eOrigin,
                        off_t  nOffset )
        throw( DWFException ) = 0;

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFInputStream()
        throw()
    {;}

private:

    //
    // Unimplemented methods
    //

    DWFInputStream( const DWFInputStream& );
    DWFInputStream& operator=( const DWFInputStream& );
};

}


#endif
