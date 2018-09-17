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


#ifndef _DWFCORE_STANDARD_FILE_DESCRIPTOR_WIN32_H
#define _DWFCORE_STANDARD_FILE_DESCRIPTOR_WIN32_H

#include "dwfcore/Core.h"
#include "dwfcore/String.h"
#include "dwfcore/FileDescriptor.h"


///
///\file        dwfcore/win32/StandardFileDescriptor.h
///\brief       This header contains the DWFStandardFileDescriptor class declaration for Microsoft Windows platforms.
///
///\note        No documentation will be generated from this file due to class name collision, 
///             such is the nature of Doyxgen.
///             Please refer to DWFCore::DWFStandardFileDescriptor documented in \a dwfcore/ansi/Mutex.h 
///             for similar documentation; however, take care to note the platform-dependent contructors.
///


#ifdef  _DWFCORE_WIN32_SYSTEM


namespace DWFCore
{

///
///\class       DWFStandardFileDescriptor   dwfcore/win32/StandardFileDescriptor.h "dwfcore/win32/StandardFileDescriptor.h"
///\brief       Descriptor implementing file access and control using the Win32 System API.
///\since       1.0.1
///
///\ingroup     CrossPlatformInterface
///
class DWFStandardFileDescriptor : virtual public DWFCoreMemory
                                , public DWFFileDescriptor
{

public:


    ///
    ///         Constructor
    ///
    ///         This constructor uses the Microsoft Windows API \e CreateFile() function.
    ///
    ///\param   rFile                   The file on which this descriptor will operate.
    ///\param   dwDesiredAccess         Please refer to Microsoft documentation for \a CreateFile().
    ///\param   dwShareMode             Please refer to Microsoft documentation for \a CreateFile().
    ///\param   pSecurityAttributes     Please refer to Microsoft documentation for \a CreateFile().
    ///\param   dwCreationDisposition   Please refer to Microsoft documentation for \a CreateFile().
    ///\param   dwFlagsAndAttributes    Please refer to Microsoft documentation for \a CreateFile().
    ///\throw   None
    ///
    _DWFCORE_API 
    DWFStandardFileDescriptor( DWFFile&                 rFile,
                               DWORD                    dwDesiredAccess,
                               DWORD                    dwShareMode,
                               LPSECURITY_ATTRIBUTES    pSecurityAttributes,
                               DWORD                    dwCreationDisposition,
                               DWORD                    dwFlagsAndAttributes )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor consumes a Win32 HANDLE data structure.
    ///
    ///\param   rFile       The file on which this descriptor will operate.
    ///\param   hFile       The handle.
    ///\throw   None
    ///
    _DWFCORE_API 
    DWFStandardFileDescriptor( DWFFile& rFile,
                               HANDLE   hFile )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API 
    virtual ~DWFStandardFileDescriptor()
        throw();

    ///
    ///\copydoc DWFFileDescriptor::open()
    ///
    _DWFCORE_API 
    void open()
        throw( DWFException );

    ///
    ///\copydoc DWFFileDescriptor::close()
    ///
    _DWFCORE_API 
    void close()
        throw( DWFException );

    ///
    ///\copydoc DWFFileDescriptor::size()
    ///
    _DWFCORE_API 
    size_t size()
        throw( DWFException );

    ///
    ///         Performs a read operation on the associated file
    ///         within the context of the descriptor implementation.
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
    size_t read( void* pBuffer, size_t nBytesToRead )
        throw( DWFException );

    ///
    ///         Performs a seek operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\param   eOrigin     One of \b SEEK_SET, \b SEEK_CUR or \b SEEK_END.
    ///\param   nOffset     The number of bytes from \a eOrigin to move the internal cursor.
    ///\return  The previous cursor offset before the seek.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    off_t seek( int eOrigin, off_t nOffset )
        throw( DWFException );

    ///
    ///         Performs a write operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\param   pBuffer         A pointer to a block of memory whose data will be written into the stream.
    ///                         Any implementation receiving a NULL buffer pointer should throw a
    ///                         DWFInvalidArgumentException.
    ///\param   nBytesToWrite   The number of bytes to copy from \a pBuffer.
    ///\return  The number of bytes actually written.
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    size_t write( const void* pBuffer, size_t nBytesToWrite )
        throw( DWFException );

    ///
    ///         Performs a flush operation on the associated file
    ///         within the context of the descriptor implementation.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API 
    void flush()
    {
        ; // NOP
    }

private:

    HANDLE                  _hFile;
    DWORD                   _dwDesiredAccess;
    DWORD                   _dwShareMode;
    DWORD                   _dwCreationDisposition;
    DWORD                   _dwFlagsAndAttributes;
    LPSECURITY_ATTRIBUTES   _pSecurityAttributes;

private:

    //
    // Unimplemented Methods
    //

    DWFStandardFileDescriptor( const DWFStandardFileDescriptor& );
    DWFStandardFileDescriptor& operator=( const DWFStandardFileDescriptor& );
};

}


#else
#error  This is a Win32 header file and is incompatible with your current system configuration
#endif 

#endif
