//
//  Copyright (c) 2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/ZipFileReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCZIPreader_H
#define _DWFTK_OPCZIPreader_H


#include "dwf/opc/PhysicalLayerReader.h"

#include "dwfcore/ZipFileDescriptor.h"

namespace DWFToolkit
{

///
///\ingroup     opc
///
///\class       OPCZipFileReader     dwf/opc/ZipFileReader.h      "dwf/opc/ZipFileReader.h"
///\brief       The OPCZipFileReader provides an implementation of the OPCPhysicalLayerReader
///             interface for reading OPC documents. In particular, it is for reading from
///             files, in which random access is available. It is <b>NOT</b> intended for
///             streamed reading.
///\since       7.4.0
///
///             The OPCPhysicalLayerReader defines the interface for reading OPC documents.
///
class OPCZipFileReader
    : public OPCPhysicalLayerReader
    , public DWFOwner
    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\param   pZipFileDescr		A pointer to a DWFZipFileDescriptor describing the zip package to be read from.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    OPCZipFileReader(DWFZipFileDescriptor *pZipFileDescr)
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~OPCZipFileReader()
        throw();

    ///
    ///         Opens the archive.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void open()
        throw(DWFException);

    ///
    ///         Returns a stream from which the specified entity
    ///         can be extracted from the archive.
    ///
    ///\param   zName		The name of the entity to be read from the archive.
    ///\return  A pointer to a stream from which the file can be read.  The caller
    ///         must delete this pointer using the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\throw   DWFException
    ///
     _DWFTK_API
    virtual DWFInputStream* read( const DWFString& zName )
        throw(DWFException);

    ///
    ///         Returns a stream from which the specified entity
    ///         can be extracted from the archive.
    ///
    ///\param   zName   The name of the section/part to read.
    ///\param   bAutoDestroy If \e true this object will be deleted when the stream
    ///                      is deleted.
    _DWFTK_API
    virtual DWFInputStream* read( const DWFString& zName, bool bAutoDestroy )
        throw(DWFException);

    ///
    ///         Closes the archive.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void close()
        throw(DWFException);

    ///
    ///\copydoc DWFCore::DWFOwner::notifyOwnerChanged( DWFOwnable& rOwnable )
    ///
    _DWFTK_API
    virtual void notifyOwnerChanged( DWFOwnable& rOwnable )
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFOwner::notifyOwnableDeletion( DWFOwnable& rOwnable )
    ///
    _DWFTK_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

private:
    //
    // Not Implemented
    //
    OPCZipFileReader();
    OPCZipFileReader( const OPCZipFileReader& );
    OPCZipFileReader& operator=( const OPCZipFileReader& );

protected:
    DWFZipFileDescriptor *_pZipFileDescr;
    bool                  _bOwnZipFileDescriptor;

};

}

#endif
