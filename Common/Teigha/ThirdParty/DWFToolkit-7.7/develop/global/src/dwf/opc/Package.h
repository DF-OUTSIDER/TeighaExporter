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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/Package.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCPACKAGE_H
#define _DWFTK_OPCPACKAGE_H


#include "dwf/opc/PartContainer.h"
#include "dwf/opc/RelationshipContainerInterface.h"

namespace DWFToolkit
{

//
//  fwd declarations
//
class OPCRelationshipContainer;
class OPCContentTypes;
class OPCCoreProperties;
class OPCPhysicalLayerWriter;
class OPCPhysicalLayerReader;

class OPCPackage : public OPCRelationshipContainerInterface
                 , public OPCPartContainer
{

public:

    ///
    ///\brief   URI of the package relationships
    ///
    static const wchar_t* const kzURI_PackageRels;
    static const wchar_t* const kzURI_PackageDigitalSignature;
    static const wchar_t* const kzURI_PackageDigitalSignatureRels;
    static const wchar_t* const kzURI_PackageDigitalSignatureExt;
    static const wchar_t* const kzURI_PackageDigitalSignatureXPS;

    _DWFTK_API
    OPCPackage()
        throw( DWFException );

    _DWFTK_API
    virtual ~OPCPackage()
        throw();


    OPCContentTypes * const contentTypes() const
        throw()
    {
        return _pContentTypes;
    }

    ///
    ///         Get the core properties of the package.
    ///
    ///\return  Pointer to the package's core properties part.
    ///\throw   None
    ///
    OPCCoreProperties * const coreProperties() const
        throw()
    {
        return _pCoreProperties;
    }

    ///
    ///         Replace the core properties part with the one provided. This will 
    ///         delete the existing core properties if the package owns it 
    ///         and transfer over any of it's relationships to the new core 
    ///         properties part.
    ///
    ///\param   pCoreProperties     The new coreproperties part to replace the
    ///                             existing one in the package.
    ///\param   bOwn                Take ownership of the core properties part
    ///                             if this is true.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void virtual setCoreProperties( OPCCoreProperties* pCorePropertiesPart, 
                                    bool bOwn = true )
        throw( DWFException );

    _DWFTK_API
    void setThumbnail (OPCPart* pThumbnailPart)
        throw();

    _DWFTK_API
    void readRelationships (OPCPhysicalLayerReader* pReader)
        throw( DWFException);

    ///
    ///\copydoc OPCRelationshipContainerInterface::addRelationship()
    ///
    _DWFTK_API
    virtual OPCRelationship* addRelationship( OPCPart* pTargetPart, 
                                              const DWFString& zRelationshipType, 
                                              OPCRelationship::teTargetMode eTargetMode = OPCRelationship::eInternal )
        throw( DWFException );

    ///
    ///\copydoc OPCRelationshipContainerInterface::deleteRelationship()
    ///
    _DWFTK_API
    virtual bool deleteRelationship( OPCRelationship* pRelationship )
        throw();

    ///
    ///\copydoc OPCRelationshipContainerInterface::relationshipCount()
    ///
    _DWFTK_API
    virtual size_t relationshipCount() const
        throw();

    ///
    ///\copydoc OPCRelationshipContainerInterface::relationships()
    ///
    _DWFTK_API
    virtual OPCRelationship::tIterator* relationships()
        throw();

    ///
    ///\copydoc OPCRelationshipContainerInterface::relationshipsByTarget( const DWFString& )
    ///
    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByTarget( const DWFString& zTargetURI ) const
        throw();

    ///
    ///\copydoc OPCRelationshipContainerInterface::relationshipsByTarget( OPCPart* )
    ///
    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByTarget( OPCPart* pPart ) const
        throw();

    ///
    ///\copydoc OPCRelationshipContainerInterface::relationshipsByType()
    ///
    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByType( const DWFString& zType ) const
        throw();

    ///
    ///\copydoc OPCRelationshipContainerInterface::deleteRelationshipsByTarget()
    ///
    _DWFTK_API
    virtual void deleteRelationshipsByTarget( OPCPart* pPart )
        throw( DWFException );

    ///
    ///\copydoc OPCRelationshipContainerInterface::getInputStream()
    ///
    _DWFTK_API
    virtual DWFInputStream* getRelationshipsInputStream()
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serialize( OPCPhysicalLayerWriter& rWriter )
        throw( DWFException );

#endif

private:

    OPCRelationshipContainer *  _pRelsContainer;

    OPCCoreProperties *         _pCoreProperties;
    OPCContentTypes *           _pContentTypes;

    OPCPart*                    _pThumbnailPart;

};

#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

_declspec(selectany) const wchar_t* const OPCPackage::kzURI_PackageRels        = L"/_rels/.rels";
_declspec(selectany) const wchar_t* const OPCPackage::kzURI_PackageDigitalSignature         = L"/package/services/digital-signature/origin.psdsor";
_declspec(selectany) const wchar_t* const OPCPackage::kzURI_PackageDigitalSignatureRels     = L"/package/services/digital-signature/_rels/origin.psdsor.rels";
_declspec(selectany) const wchar_t* const OPCPackage::kzURI_PackageDigitalSignatureExt      = L".psdsxs";
_declspec(selectany) const wchar_t* const OPCPackage::kzURI_PackageDigitalSignatureXPS      = L"http://schemas.microsoft.com/xps";
#endif
#endif

}

#endif
