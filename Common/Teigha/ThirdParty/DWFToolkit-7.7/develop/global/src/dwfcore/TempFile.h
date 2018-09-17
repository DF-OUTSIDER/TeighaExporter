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


#ifndef _DWFCORE_TEMPFILE_H
#define _DWFCORE_TEMPFILE_H

///
///\file        dwfcore/TempFile.h
///\brief       This header contains the declaration of the DWFTempFile class.
///

#include "dwfcore/Core.h"
#include "dwfcore/String.h"
#include "dwfcore/FileInputStream.h"
#include "dwfcore/FileOutputStream.h"



namespace DWFCore
{


///
///\class           DWFTempFile   dwfcore/TempFile.h  "dwfcore/TempFile.h"
///\brief           This class provides a simple, platform-independent mechanism
///                 for creating, using and managing temporary disk files.
///\since           1.0.1
///
///\note            This class has platform-specific implementations.
///\ingroup         CrossPlatformImpl
///
class DWFTempFile : virtual public DWFCoreMemory
{

public:

    ///
    ///             Create and open a new temporary file.
    ///
    ///\param       zTemplate           An optional string that wil be used in the temporary filename.
    ///\param       bDeleteOnDestroy    If \e true, the disk file associated with the new object will be
    ///                                 deleted when it is deleted.  If \e false, the temporary
    ///                                 file will be left on disk.  The filename can be obtained from
    ///                                 the descriptor - see \a getOutputStream().
    ///\return      A pointer to a new temporary file object.  This pointer must be deleted by the caller
    ///             using the \b DWFCORE_FREE_OBJECT macro.
    ///\throw       DWFException
    _DWFCORE_API
    static DWFTempFile* Create( DWFString&  zTemplate,
                                bool        bDeleteOnDestroy )
        throw( DWFException );

    ///
    ///             Destructor
    ///
    ///             This method will delete the disk file if the object was created
    ///             with \a bDeleteOnDestroy set to \e true.
    ///
    ///\throw       None
    ///
    _DWFCORE_API
    virtual ~DWFTempFile()
        throw();

    ///
    ///             Returns a stream that can be used to write data into the disk file.
    ///             In some cases (the file is not deleted from disk) it may be necessary
    ///             to obtain the disk file information.  This data is stored in the DWFFile
    ///             object associated with the DWFFileDescriptor to which this stream is bound.
    ///
    ///             Example - Recover the filename of the temporary file on disk:
    ///             \code
    ///             //
    ///             // Acquire the stream from the object.
    ///             // The descriptor is available from this stream.
    ///             //
    ///             // NOTE: We could use an input stream here as well!
    ///             //       It just depends on the state of the DWFTempFile object
    ///             //       and what we have access to when we need to obtain this info.
    ///             //
    ///             DWFFileOutputStream& rTempStream = pTempFile->getOutputStream();
    ///
    ///             //
    ///             // the file descriptor has the DWFFile object we need
    ///             //
    ///             DWFFileDescriptor* pTempDescriptor = rTempStream.descriptor();
    ///
    ///             //
    ///             // the file object has the useful information
    ///             //
    ///             const DWFFile& rTempFileObj = pTempDescriptor->file();
    ///
    ///             //
    ///             // copy the filename down for later...
    ///             //
    ///             DWFString zTempFilename = rTempFileObj.name();
    ///             \endcode
    ///
    ///\return      A reference to the \b only stream available for writing data into the temporary file.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    DWFFileOutputStream& getOutputStream()
        throw( DWFException );

    ///
    ///             Returns a stream that can be used to read data from the disk file.
    ///             This stream will have shared read-access on the file and
    ///             the caller is reponsible for deleting this stream pointer with the
    ///             \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\note        One of these streams can be used to obtain the actual disk filename - see \a getOutputStream().
    ///\return      A pointer to a new read stream.  Any number of this streams may exists simultaneously.
    ///\throw       DWFException
    ///
    _DWFCORE_API
    DWFFileInputStream* getInputStream()
        throw( DWFException );

private:

    //
    // Constructor
    //
    DWFTempFile( DWFFileOutputStream* pOutputStream,
                 bool                 bDeleteOnDestroy )
        throw();

private:

    DWFFile                 _oFile;
    DWFFileOutputStream*    _pOutputStream;
    bool                    _bDeleteOnDestroy;

private:

    //
    // Not Implemented
    //
    DWFTempFile( const DWFTempFile& rFile );
    DWFTempFile& operator=( const DWFTempFile& rFile );

};



}


#endif
