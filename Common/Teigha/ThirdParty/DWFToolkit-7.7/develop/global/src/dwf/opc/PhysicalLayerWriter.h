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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/PhysicalLayerWriter.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCPHYSICALLAYERWRITER_H
#define _DWFTK_OPCPHYSICALLAYERWRITER_H

#include "dwfcore/String.h"
#include "dwfcore/InputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"

namespace DWFToolkit
{

///
///\ingroup     opc
///
///\class       OPCPhysicalLayerWriter     dwf/opc/PhysicalLayerWriter.h      "dwf/opc/PhysicalLayerWriter.h"
///\brief       The OPCPhysicalLayerWriter defines the interface for writing out OPC documents.
///\since       7.4.0
///
///             The OPCPhysicalLayerWriter defines the interface for writing out OPC documents.
///
class OPCPhysicalLayerWriter _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

protected:
    ///
    ///         Constructor
    ///
    ///			This should never be called by client code, as this class is to be considered abstract.
    ///
    ///\throw   None
    ///
    _DWFTK_API
    OPCPhysicalLayerWriter()
        throw();

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~OPCPhysicalLayerWriter()
        throw();
    ///
    ///         Opens the archive.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void open()
        throw(DWFException) = 0;

    ///
    ///         Writes to the archive.
    ///
    ///\param   zName		The name of the entity to be stored in the archive.
    ///\param   pInpStream	A pointer to an DWFInputStream containing the data to be stored in the archive.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void write( const DWFString& zName, DWFInputStream* pInpStream )
        throw(DWFException) = 0;

    ///
    ///         Closes the archive.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void close()
        throw(DWFException) = 0;

};

}

#endif
