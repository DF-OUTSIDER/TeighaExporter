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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/Package.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXPACKAGE_H
#define _DWFTK_DWFXPACKAGE_H

///
///\file        dwf/dwfx/Package.h
///\brief       This file contains the DWFXPackage class declarations.
///

#include "dwf/xps/Package.h"

namespace DWFToolkit
{

//
//  fwd declaration
//
class DWFXDWFDocumentSequence;
class DWFXResourcePart;
class DWFResource;

class DWFXPackage : public XPSPackage
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFXPackage()
        throw( DWFException );

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFXPackage()
        throw();

    ///
    ///         Set the DWFDocumentSequence part for the package. The document sequence
    ///         must be created outside of the package and set with this method. If the package
    ///         already had a document sequence, it will \b not be deleted. It is returned 
    ///         by this method and it is up to the caller to delete it.
    ///
    ///\param   pDWFDocumentSequence    The DWFDocumentSequence part.
    ///\param   bOwn                    The package will take ownership of the part if this is true.
    ///\return  Pointer to the old document sequence if the package had one.
    ///\throw   None
    ///
    _DWFTK_API
    DWFXDWFDocumentSequence* setDWFDocumentSequence( DWFXDWFDocumentSequence* pDWFDocumentSequence, 
                                                     bool bOwn = true )
        throw();

    ///
    ///         Get the DWFDocumentSequence part of the package. If it has not been set
    ///         this will return NULL.
    ///
    ///\return  Pointer to the package's DWFDocumentSequence part.
    ///\throw   None
    ///
    _DWFTK_API
    DWFXDWFDocumentSequence* const dwfDocumentSequence() const
        throw()
    {
        return _pDWFDocumentSequence;
    }

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serialize( OPCPhysicalLayerWriter& rWriter )
        throw( DWFException );

#endif

private:

#ifndef DWFTK_READ_ONLY

    //
    //  Ensure URIs are set and valid. Also add the parts to the OPC PartContainer aspect of
    //  the package for OPC based serialization. Update the package ContentTypes during
    //  the traversal of the container heirarchy.
    //
    void _updateURIsAndOPCContainment()
        throw( DWFException );

    //
    //  Factoring out the code to update the URI of the ResourcePart, and update the 
    //  ContentTypes from them.
    //
    void _updateResourcePartInfo( DWFXResourcePart* pPart,
                                  const DWFString& zPath,
                                  DWFUUID& uuidGenerator )
        throw( DWFException );

    //
    //  The internal IDs of the resource are package unique and are set
    //  during 
    //
    void _updateInternalID( DWFResource* pResource )
        throw( DWFException );

#endif

private:

    DWFXDWFDocumentSequence*    _pDWFDocumentSequence;

    unsigned int                _nResourceInternalID;

private:

    //
    //  Not implemented
    //
    DWFXPackage( const DWFXPackage& );
    DWFXPackage& operator=( const DWFXPackage& );

};

}

#endif
