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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/ZipWriter.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCZIPWRITER_H
#define _DWFTK_OPCZIPWRITER_H


#include "dwf/opc/PhysicalLayerWriter.h"

#include "dwfcore/ZipFileDescriptor.h"

namespace DWFToolkit
{

class OPCZipWriter
    : public OPCPhysicalLayerWriter
    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///\param   pZipFileDescr		A pointer to a DWFZipFileDescriptor describing the zip package to be written to.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    OPCZipWriter(DWFZipFileDescriptor *pZipFileDescr)
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~OPCZipWriter()
        throw();

    ///
    ///\copydoc OPCPhysicalLayerWriter::open()
    ///
    _DWFTK_API
    virtual void open()
        throw(DWFException);

    ///
    ///\copydoc OPCPhysicalLayerWriter::write()
    ///
    _DWFTK_API
    virtual void write( const DWFString &zName, DWFInputStream* pInpStream )
        throw(DWFException);


    ///
    ///\copydoc OPCPhysicalLayerWriter::write()
    ///\param   eZipMode        Override the zip mode in descriptor and use this instead.
    ///
    _DWFTK_API
    virtual void write( const DWFString &zName, DWFInputStream* pInpStream, DWFZipFileDescriptor::teFileMode eZipMode )
        throw(DWFException);


    ///
    ///\copydoc OPCPhysicalLayerWriter::close()
    ///
    _DWFTK_API
    virtual void close()
        throw(DWFException);

private:
    //
    // Not Implemented
    //
    OPCZipWriter();
    OPCZipWriter( const OPCZipWriter& );
    OPCZipWriter& operator=( const OPCZipWriter& );

protected:
    DWFZipFileDescriptor *_pZipFileDescr;

};

}

#endif
