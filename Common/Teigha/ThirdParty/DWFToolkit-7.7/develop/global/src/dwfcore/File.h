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


#ifndef _DWFCORE_FILE_H
#define _DWFCORE_FILE_H


///
///\file        dwfcore/File.h
///\brief       This header contains the declaration for the DWFFile class.
///

///
///\example     Files/Files.cpp
///             This sample program exercises a variety of file operations including streaming I/O,
///             zip archiving and processing using several file-oriented core classes and interfaces
///             including: 
///                 - DWFCore::DWFFile
///                 - DWFCore::DWFTempFile
///                 - DWFCore::DWFFileInputStream
///                 - DWFCore::DWFFileOutputStream
///                 - DWFCore::DWFZipFileDescriptor
///                 - DWFCore::DWFStreamFileDescriptor
///
///             Also shown in this example are other useful concepts such as timing operations,
///             string usage, and stream buffering and monitoring using the following core classes
///             and interfaces:
///                 - DWFCore::DWFTimer
///                 - DWFCore::DWFString
///                 - DWFCore::DWFException
///                 - DWFCore::DWFBufferOutputStream
///                 - DWFCore::DWFMonitoredInputStream
///

#include "dwfcore/Core.h"
#include "dwfcore/String.h"


namespace DWFCore
{

///
///\class       DWFFile     dwfcore/File.h  "dwfcore/File.h"
///\brief       Generalized file specifier.
///\since       1.0.1
///
///             This class encapsulates only the most fundamental
///             aspect of a file, it's name.
///             It makes no assumptions as to how to interpret this
///             value.  File types requiring more information should
///             extend this class accordingly (e.g. PIDLs, permissions, etc).
///
class DWFFile   : virtual public DWFCoreMemory
{

public:

    ///
    ///         Constructor
    ///         This constructor takes a string that may only be either a US-ASCII or
    ///         UTF-8 encoded file name.
    ///
    ///\param   zName   The file name.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFFile( const ASCII_char_t* zName = NULL )
        throw();

    ///
    ///         Constructor
    ///         This constructor takes a file name encapsulated in a DWFString.
    ///
    ///\param   rName   The file name.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFFile( const DWFString& rName )
        throw();

    ///
    ///         Copy Constructor
    ///
    ///\param   rFile   The file to copy.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFFile( const DWFFile& rFile )
        throw();

    ///
    ///         Assignment Operator
    ///
    ///\param   rFile   The file to copy.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFFile& operator=( const DWFFile& rFile )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFFile()
        throw();

    ///
    ///         Returns the current file name.
    ///
    ///\return  The file name.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual const DWFString& name() const
        throw( DWFException );


protected:

    ///
    ///         The file name.
    ///
    DWFString   _zName;
};


}


#endif
