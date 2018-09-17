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


#ifndef _DWFCORE_MONITORED_INPUT_STREAM_H
#define _DWFCORE_MONITORED_INPUT_STREAM_H


///
///\file        dwfcore/MonitoredInputStream.h
///\brief       This header contains the declaration for the DWFMonitoredInputStream class.
///


#include "dwfcore/InputStream.h"
#include "dwfcore/OutputStream.h"


namespace DWFCore
{

///
///\class       DWFMonitoredInputStream     dwfcore/MonitoredInputStream.h  "dwfcore/MonitoredInputStream.h"
///\brief       An input stream whose read operation can be "watched".
///\since       1.0.1
///
///             This implementation of the DWFInputStream is useful when it is
///             desriable for one entity to passively receive the same data 
///             as the active stream reader.  There are two mechanisms provided for hooking
///             into the stream: an object may implement the Monitor interface,
///             or DWFOutputStream.  In both cases, whatever data was obtained during
///             the \a read() operation will be either posted to the monitor or written
///             into the output stream.
///
///             Example 1: Use a monitor to show progress:
///             \code
///             class ProgressMonitor : public DWFMonitoredInputStream::Monitor
///             {
///                 ProgressMonitor( size_t nTotalBytesInStream )
///                     : _nTotalBytes( nTotalBytesInStream )
///                     , _nBytesReadSoFar( 0 )
///                 {;}
///                 
///                 void notify( const void* const pBuffer,
///                              size_t            nBytesRequested,
///                              size_t            nBytesRead )
///                     throw( DWFException )
///                 {
///                     _nBytesReadSoFar += nBytesRead;
///                     float nProgress = 100.0 * (float)(_nBytesReadSoFar / _nTotalBytesInStream);
///
///                     _send_progress_function( nProgress );
///                 }
///             };
///             \endcode
///
///             Example 2: Copy a stream into a temporary file.
///             \code
///             DWFString zTemplate( L"_dwfcore_" );
///
///             //
///             // passing true as the second parameter will result in the
///             // temporary disk file being deleted when the DWFTempFile
///             // object is destroyed.
///             //
///             DWFTempFile* pTempFile = DWFTempFile::Create( zTemplate, true );
///             DWFOutputStream* pTempFileStream = pTempFile->getOuputStream();
///
///             //
///             // passing true here tells the monitor stream to assume
///             // ownership of the file stream and delete it when
///             // he is deleted himself
///             //
///             pMonitorStream->attachMonitor( pTempFileStream, true );
///
///             ... read from the monitor stream ...
///
///             DWFCORE_FREE_OBJECT( pMonitorStream );
///             DWFCORE_FREE_OBJECT( pTempFile );
///             \endcode
///
class DWFMonitoredInputStream : virtual public DWFCoreMemory
                              , public DWFInputStream
{

public:

    ///
    ///\interface   Monitor    dwfcore/MonitoredInputStream.h  "dwfcore/MonitoredInputStream.h"
    ///\brief       Callback interface for watching DWFMonitoredInputStream objects.
    ///\since       1.0.1
    ///\todo        Add notification method for stream seek.
    ///
    class Monitor
    {

    public:

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~Monitor()
            throw()
        {;}

        ///
        ///         Post-read data notification callback.
        ///
        ///\param   pBuffer         The data returned from the \a read() operation.
        ///\param   nBytesRequested The number of bytes originally requested to be read from the stream.
        ///\param   nBytesRead      The number of bytes actually read from the stream; and the size of 
        ///                         the data in \a pBuffer.
        ///\throw   DWFException
        ///
        _DWFCORE_API 
        virtual void notify( const void* const pBuffer,
                             size_t            nBytesRequested,
                             size_t            nBytesRead )
             throw( DWFException ) = 0;

    protected:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        Monitor()
            throw()
        {;}

    };

public:

    ///
    ///         Constructor
    ///
    ///\param   pStream     The source data stream to be monitored.
    ///\param   bOwnStream  If \e true, this object will assume ownership of \a pInputStream 
    ///                     and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                     If \e false, the caller retains ownership of \a pInputStream
    ///                     and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFMonitoredInputStream( DWFInputStream* pStream,
                             bool            bOwnStream )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API 
    virtual ~DWFMonitoredInputStream()
        throw();

    ///
    ///         Binds a monitor to the stream.  Any data obtained
    ///         from a \a read() call will result in this
    ///         monitor's \a notify() method being invoked.
    ///         Any previously bound monitor will be replaced; and
    ///         deleted if this stream currently owns it.
    ///
    ///\param   pMonitor    The monitor to which read data will be posted.
    ///\param   bOwnMonitor If \e true, this object will assume ownership of \a pMonitor 
    ///                     and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                     If \e false, the caller retains ownership of \a pMonitor
    ///                     and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API 
    void attach( Monitor* pMonitor, bool bOwnMonitor )
        throw( DWFException );

    ///
    ///         Binds a monitoring stream to the stream.  Any data obtained
    ///         from a \a read() call will result in this
    ///         stream's \a write() method being invoked.
    ///         Any previously bound monitor will be replaced; and
    ///         deleted if this stream currently owns it.
    ///
    ///\param   pMonitor    The stream to which read data will be written.
    ///\param   bOwnMonitor If \e true, this object will assume ownership of \a pMonitor 
    ///                     and delete it as necessary using \b DWFCORE_FREE_OBJECT.
    ///                     If \e false, the caller retains ownership of \a pMonitor
    ///                     and is responsible for deleting it.
    ///\throw   None
    ///
    _DWFCORE_API 
    void attach( DWFOutputStream* pMonitor, bool bOwnMonitor )
        throw( DWFException );

    ///
    ///         Unbinds any monitors from the stream.
    ///         All owned monitors will be deleted.
    ///
    ///\throw   None
    ///
    _DWFCORE_API 
    void detach()
        throw( DWFException );

    ///
    ///\copydoc DWFInputStream::available()
    ///
    _DWFCORE_API 
    size_t available() const
        throw( DWFException );

    ///
    ///\copydoc DWFInputStream::read()
    ///
    _DWFCORE_API 
    size_t read( void*  pBuffer,
                 size_t nBytesToRead )
        throw( DWFException );

    ///
    ///\copydoc DWFInputStream::seek()
    ///
    _DWFCORE_API 
    off_t seek( int    eOrigin,
                off_t  nOffset )
        throw( DWFException );


private:

    DWFInputStream*     _pStream;

    Monitor*            _pMonitor;
    DWFOutputStream*    _pMonitorStream;

    bool                _bOwnStream;
    bool                _bOwnMonitor;
    bool                _bOwnMonitorStream;

private:

    //
    // Unimplemented methods
    //

    DWFMonitoredInputStream( const DWFMonitoredInputStream& );
    DWFMonitoredInputStream& operator=( const DWFMonitoredInputStream& );
};

}



#endif
