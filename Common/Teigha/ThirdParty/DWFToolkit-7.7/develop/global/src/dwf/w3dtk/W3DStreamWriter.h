//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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

#ifndef _DWFW3DTK_STREAM_WRITER_H
#define _DWFW3DTK_STREAM_WRITER_H

///
///\file        dwf/w3dtk/W3DStreamWriter.h
///\brief       This file contains the W3DStreamWriter class declaration.
///

#include "dwfcore/OutputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/w3dtk/BStream.h"
#include "dwf/w3dtk/W3DOpcodeHandler.h"
#include "dwf/w3dtk/BStreamFileToolkit.h"


//
// default write 4k buffer
//
#ifndef DWFW3D_STREAM_WRITER_BUFFER_BYTES
#define DWFW3D_STREAM_WRITER_BUFFER_BYTES                   4096
#endif

//
// the maximum stack buffer allowed
// this only applies if stacking buffer was not specified explicitly
//
#ifndef DWFW3D_STREAM_WRITER_USE_STACK_BUFFER_MAX_BYTES
#define DWFW3D_STREAM_WRITER_USE_STACK_BUFFER_MAX_BYTES     16384
#endif

//
// for small enough buffers, use the stack - it will be faster
// ideally this value should be defined in the project
// this only applies if stacking buffer was not specified explicitly
//
#ifndef DWFW3D_STREAM_WRITER_USE_STACK_BUFFER
#if    (DWFW3D_STREAM_WRITER_BUFFER_BYTES <= DWFW3D_STREAM_WRITER_USE_STACK_BUFFER_MAX_BYTES)
#define DWFW3D_STREAM_WRITER_USE_STACK_BUFFER
#endif
#endif

//
// the minimum version number
// this is the earliest stream version that we ever supported
//
#ifndef DWFW3D_STREAM_WRITER_EARLIEST_VERSION
#define DWFW3D_STREAM_WRITER_EARLIEST_VERSION     1000
#endif

///
///\interface   W3DStreamWriter   dwf/w3dtk/W3DStreamWriter.h     "dwf/w3dtk/W3DStreamWriter.h"
///\brief       This class manages the interaction among the HSF streamer object,
///             the w3d graphics stream and the op-code handler serialization.
///\since       1.0.1000
///
class BBINFILETK_API W3DStreamWriter : public BaseOpcodeHandlerObserver
{

public:

    ///
    ///         Constructor
    ///
    ///\param   rW3DStream      The w3d graphics stream.
    ///\param   rToolkit        The HSF streamer.
    ///\throw   None
    ///
    W3DStreamWriter( DWFOutputStream&       rW3DStream,
                     BStreamFileToolkit&    rToolkit )
       throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    ~W3DStreamWriter()
        throw();

    ///
    ///         Prepares the writer for use.
    ///
    ///\param   nRequestedVersion   An optional parameter that controls the stream format by
    ///                             restricting opcode data and options to a target version number.
    ///                             By default, the latest current version is used.
    ///
    ///\throw   DWFException
    ///
    void open( unsigned int nRequestedVersion = 0 )
        throw( DWFException );

    ///
    ///         Flushes any pending operations
    ///         and releases all resources.
    ///
    ///\return  The minimum required stream version number to support
    ///         the data and options captured in all opcodes.  If no
    ///         such requirement exists the method returns zero.
    ///
    ///\throw   DWFException
    ///
    unsigned int close()
        throw( DWFException );

    ///
    ///\copydoc BaseOpcodeHandlerObserver::notify
    ///
    void notify( BBaseOpcodeHandler* pHandler,
                 const void*         pTag = NULL )
        throw( DWFException );

private:

    DWFOutputStream&    _rStream;
    BStreamFileToolkit& _rToolkit;
    bool                _bOpen;
    unsigned int        _nRequiredVersion;

#ifdef  DWFW3D_STREAM_WRITER_USE_STACK_BUFFER

    char                _pBuffer[DWFW3D_STREAM_WRITER_BUFFER_BYTES];

#else

    char*               _pBuffer;

#endif

private:

    //
    // Not implemented
    //

    W3DStreamWriter();
    W3DStreamWriter( const W3DStreamWriter& W3DStreamWriter );
    W3DStreamWriter& operator=( const W3DStreamWriter& W3DStreamWriter );
};



#endif
