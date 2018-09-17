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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/PhysicalLayerReader.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCPHYSICALLAYERREADER_H
#define _DWFTK_OPCPHYSICALLAYERREADER_H

#include "dwfcore/String.h"
#include "dwfcore/InputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"

namespace DWFToolkit
{

///
///\ingroup     opc
///
///\class       OPCPhysicalLayerReader     dwf/opc/PhysicalLayerReader.h      "dwf/opc/PhysicalLayerReader.h"
///\brief       The OPCPhysicalLayerReader defines the interface for reading OPC documents.
///\since       7.4.0
///
///             The OPCPhysicalLayerReader defines the interface for reading OPC documents.
///
class OPCPhysicalLayerReader _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
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
    OPCPhysicalLayerReader()
        throw();

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~OPCPhysicalLayerReader()
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
