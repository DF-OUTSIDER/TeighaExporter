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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/Constants.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_DWFXCONSTANTS_H
#define _DWFTK_DWFXCONSTANTS_H


///
///\file        dwf/dwfx/Constants.h
///\brief       This file contains all DWFX XML string declarations.
///


#include "dwfcore/Core.h"
#include "dwfcore/Exception.h"
#include "dwfcore/STL.h"
#include "dwfcore/Pointer.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"


namespace DWFToolkit
{

///
///\ingroup     dwfx
///
///\class       DWFXXML  dwf/dwfx/Constants.h     "dwf/dwfx/Constants.h"
///\brief       This class scopes the DWXF XML strings known to the toolkit.
///\since       7.4.0
///
class DWFXXML
{

public:
    ///
    ///\brief   Pre-defined Schema ID for dwf properties
    ///
    static const char* const kzSchemaID_DWFProperties;

    ///
    ///\brief   "http://schemas.dwf.autodesk.com/dwfx/2006/11"
    ///
    static const char* const kzNamespaceURI_DWFX;

    ///
    ///\brief   "DWFDocumentSequence"
    ///
    static const char* const kzElement_DWFDocumentSequence;
    ///
    ///\brief   "ManifestReference"
    ///
    static const char* const kzElement_ManifestReference;
    ///
    ///\brief   "DWFProperties"
    ///
    static const char* const kzElement_DWFProperties;

    ///
    ///\brief   "Source"
    ///
    static const char* const kzAttribute_Source;

    ///
    ///\brief   "dwfresource_"
    ///
    static const char* const kzPrefix_ResourceInternalID;

    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/document"
    ///
    static const wchar_t* const kzRelationship_Document;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/content"
    ///
    static const wchar_t* const kzRelationship_Content;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/documentpresentations"
    ///
    static const wchar_t* const kzRelationship_DocumentPresentations;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/section"
    ///
    static const wchar_t* const kzRelationship_Section;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/documentsequence"
    ///
    static const wchar_t* const kzRelationship_DocumentSequence;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/requiredresource"
    ///
    static const wchar_t* const kzRelationship_RequiredResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/graphics2dresource"
    ///
    static const wchar_t* const kzRelationship_Graphics2dResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/3dstreaminggraphicsresource"
    ///
    static const wchar_t* const kzRelationship_Graphics3dResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/descriptorresource"
    ///
    static const wchar_t* const kzRelationship_DescriptorResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/thumbnailresource"
    ///
    static const wchar_t* const kzRelationship_ThumbnailResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/2dvectoroverlayresource"
    ///
    static const wchar_t* const kzRelationship_Graphics2dOverlayResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/2dvectormarkupresource"
    ///
    static const wchar_t* const kzRelationship_Graphics2dMarkupResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/RMLmarkupresource"
    ///
    static const wchar_t* const kzRelationship_RmlMarkupResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/previewresource"
    ///
    static const wchar_t* const kzRelationship_PreviewResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/overlaypreviewresource"
    ///
    static const wchar_t* const kzRelationship_OverlayPreviewResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/markuppreviewresource"
    ///
    static const wchar_t* const kzRelationship_MarkupPreviewResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/rasteroverlayresource"
    ///
    static const wchar_t* const kzRelationship_RasterOverlayResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/rastermarkupresource"
    ///
    static const wchar_t* const kzRelationship_RasterMarkupResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/fontresource"
    ///
    static const wchar_t* const kzRelationship_FontResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/metadataresource"
    ///
    static const wchar_t* const kzRelationship_MetadataResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/objectdefinitionresource"
    ///
    static const wchar_t* const kzRelationship_ObjectDefinitionResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/markupobjectdefinitionresource"
    ///
    static const wchar_t* const kzRelationship_MarkupObjectDefinitionResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/textureresource"
    ///
    static const wchar_t* const kzRelationship_TextureResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/contentpresentationresource"
    ///
    static const wchar_t* const kzRelationship_ContentPresentationResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/markupcontentpresentationresource"
    ///
    static const wchar_t* const kzRelationship_MarkupContentPresentationResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/animationresource"
    ///
    static const wchar_t* const kzRelationship_AnimationResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/noteresource"
    ///
    static const wchar_t* const kzRelationship_NoteResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/3Dmarkupgraphicsresource"
    ///
    static const wchar_t* const kzRelationship_Graphics3dMarkupResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/contentdefinitionresource"
    ///
    static const wchar_t* const kzRelationship_ContentDefinitionResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/iconresource"
    ///
    static const wchar_t* const kzRelationship_IconResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/observationmeshresource"
    ///
    static const wchar_t* const kzRelationship_ObservationMeshResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/geographiccoordinatesystemresource"
    ///
    static const wchar_t* const kzRelationship_GeographicCoordinateSystemResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/graphics2dextensionresource"
    ///
    static const wchar_t* const kzRelationship_Graphics2dExtensionResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/graphics2ddictionaryresource"
    ///
    static const wchar_t* const kzRelationship_Graphics2dDictionaryResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/rasterreferenceresource"
    ///
    static const wchar_t* const kzRelationship_RasterReferenceResource;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/dwfproperties"
    ///
    static const wchar_t* const kzRelationship_DWFProperties;
    ///
    ///\brief   L"http://schemas.autodesk.com/dwfx/2007/relationships/customproperties"
    ///
    static const wchar_t* const kzRelationship_CustomProperties;

public:

    _DWFTK_API
    static const wchar_t* const GetRelationship( const wchar_t* const zRole )
        throw( DWFException );

    _DWFTK_API
    static const wchar_t* const GetRole( const wchar_t* const zRelationship )
        throw( DWFException );

private:

    //
    // Constructor
    //
    DWFXXML()
        throw();

    //
    //
    //
    static void _Build()
        throw( DWFException );

private:

    typedef DWFWCharKeySkipList<const wchar_t*>                                 _tList;

    static DWFPointer<_tList>       _kapRoleMap;         // Role -> Relationship
    static DWFPointer<_tList>       _kapRelationshipMap; // Relationship -> Role
};



    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start
_declspec(selectany) const char* const DWFXXML::kzSchemaID_DWFProperties = "6B241FB6-E174-4B01-9685-60381F320DA7";

_declspec(selectany) const char* const DWFXXML::kzNamespaceURI_DWFX = "http://schemas.dwf.autodesk.com/dwfx/2006/11";

_declspec(selectany) const char* const DWFXXML::kzElement_DWFDocumentSequence = "DWFDocumentSequence";
_declspec(selectany) const char* const DWFXXML::kzElement_ManifestReference = "ManifestReference";
_declspec(selectany) const char* const DWFXXML::kzElement_DWFProperties = "DWFProperties";

_declspec(selectany) const char* const DWFXXML::kzAttribute_Source = "Source";

_declspec(selectany) const char* const DWFXXML::kzPrefix_ResourceInternalID = "dwfresource_";

_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Document                             = L"http://schemas.autodesk.com/dwfx/2007/relationships/document";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Content                              = L"http://schemas.autodesk.com/dwfx/2007/relationships/content";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_DocumentPresentations                = L"http://schemas.autodesk.com/dwfx/2007/relationships/documentpresentations";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Section                              = L"http://schemas.autodesk.com/dwfx/2007/relationships/section";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_DocumentSequence                     = L"http://schemas.autodesk.com/dwfx/2007/relationships/documentsequence";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_RequiredResource                     = L"http://schemas.autodesk.com/dwfx/2007/relationships/requiredresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Graphics2dResource                   = L"http://schemas.autodesk.com/dwfx/2007/relationships/graphics2dresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Graphics3dResource                   = L"http://schemas.autodesk.com/dwfx/2007/relationships/3dstreaminggraphicsresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_DescriptorResource                   = L"http://schemas.autodesk.com/dwfx/2007/relationships/descriptorresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_ThumbnailResource                    = L"http://schemas.autodesk.com/dwfx/2007/relationships/thumbnailresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Graphics2dOverlayResource            = L"http://schemas.autodesk.com/dwfx/2007/relationships/2dvectoroverlayresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Graphics2dMarkupResource             = L"http://schemas.autodesk.com/dwfx/2007/relationships/2dvectormarkupresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_RmlMarkupResource                    = L"http://schemas.autodesk.com/dwfx/2007/relationships/RMLmarkupresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_PreviewResource                      = L"http://schemas.autodesk.com/dwfx/2007/relationships/previewresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_OverlayPreviewResource               = L"http://schemas.autodesk.com/dwfx/2007/relationships/overlaypreviewresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_MarkupPreviewResource                = L"http://schemas.autodesk.com/dwfx/2007/relationships/markuppreviewresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_RasterOverlayResource                = L"http://schemas.autodesk.com/dwfx/2007/relationships/rasteroverlayresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_RasterMarkupResource                 = L"http://schemas.autodesk.com/dwfx/2007/relationships/rastermarkupresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_FontResource                         = L"http://schemas.autodesk.com/dwfx/2007/relationships/fontresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_MetadataResource                     = L"http://schemas.autodesk.com/dwfx/2007/relationships/metadataresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_ObjectDefinitionResource             = L"http://schemas.autodesk.com/dwfx/2007/relationships/objectdefinitionresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_MarkupObjectDefinitionResource       = L"http://schemas.autodesk.com/dwfx/2007/relationships/markupobjectdefinitionresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_TextureResource                      = L"http://schemas.autodesk.com/dwfx/2007/relationships/textureresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_ContentPresentationResource          = L"http://schemas.autodesk.com/dwfx/2007/relationships/contentpresentationresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_MarkupContentPresentationResource    = L"http://schemas.autodesk.com/dwfx/2007/relationships/markupcontentpresentationresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_AnimationResource                    = L"http://schemas.autodesk.com/dwfx/2007/relationships/animationresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_NoteResource                         = L"http://schemas.autodesk.com/dwfx/2007/relationships/noteresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Graphics3dMarkupResource             = L"http://schemas.autodesk.com/dwfx/2007/relationships/3Dmarkupgraphicsresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_ContentDefinitionResource            = L"http://schemas.autodesk.com/dwfx/2007/relationships/contentdefinitionresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_IconResource                         = L"http://schemas.autodesk.com/dwfx/2007/relationships/iconresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_ObservationMeshResource              = L"http://schemas.autodesk.com/dwfx/2007/relationships/observationmeshresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_GeographicCoordinateSystemResource   = L"http://schemas.autodesk.com/dwfx/2007/relationships/geographiccoordinatesystemresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Graphics2dExtensionResource          = L"http://schemas.autodesk.com/dwfx/2007/relationships/graphics2dextensionresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_Graphics2dDictionaryResource         = L"http://schemas.autodesk.com/dwfx/2007/relationships/graphics2ddictionaryresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_RasterReferenceResource              = L"http://schemas.autodesk.com/dwfx/2007/relationships/rasterreferenceresource";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_DWFProperties                        = L"http://schemas.autodesk.com/dwfx/2007/relationships/dwfproperties";
_declspec(selectany) const wchar_t* const DWFXXML::kzRelationship_CustomProperties                     = L"http://schemas.autodesk.com/dwfx/2007/relationships/customproperties";
//DNT_End

#endif
#endif

}

#endif
