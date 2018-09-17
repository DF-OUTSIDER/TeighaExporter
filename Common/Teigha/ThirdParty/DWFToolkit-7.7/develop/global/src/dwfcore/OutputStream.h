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


#ifndef _DWFCORE_OUTPUT_STREAM_H
#define _DWFCORE_OUTPUT_STREAM_H


///
///\file        dwfcore/OutputStream.h
///\brief       This header contains the declaration for the DWFOutputStream interface.
///

#include "dwfcore/Core.h"
#include "dwfcore/Exception.h"


namespace DWFCore
{

///
///\interface   DWFOutputStream     dwfcore/OutputStream.h  "dwfcore/OutputStream.h"
///\brief       Interface for output (writer) streams.
///\since       1.0.1
///
///             This class defines a contract for a streamable data sink.
///             Implementors of this interface are considered valid targets 
///             to which output data may be written.
///
class DWFOutputStream
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFOutputStream()
        throw ()
    {;}

    ///
    ///         Writes any remaining bytes from the stream into the data sink.
    ///         This method may not have any function for some implementations.
    ///         It is recommended that, rather than throwing an exception, the
    ///         method simply return.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    virtual void flush()
        throw( DWFException ) = 0;

    ///
    ///         Writes at most \a nBytesToWrite into the stream from the buffer provided.
    ///
    ///\param   pBuffer         A pointer to a block of memory whose data will be written into the stream.
    ///                         Any implementation receiving a NULL buffer pointer should throw a
    ///                         DWFInvalidArgumentException.
    ///\param   nBytesToWrite   The number of bytes to copy from \a pBuffer.
    ///\return  The number of bytes actually written.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    virtual size_t write( const void*   pBuffer,
                          size_t        nBytesToWrite )
        throw( DWFException )= 0;

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFOutputStream()
        throw()
    {;}

private:

    //
    // Unimplemented methods
    //

    DWFOutputStream( const DWFOutputStream& );
    DWFOutputStream& operator=( const DWFOutputStream& );
};

}


#endif
