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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/Constants.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_OPCCONSTANTS_H
#define _DWFTK_OPCCONSTANTS_H


///
///\file        dwf/opc/Constants.h
///\brief       This file contains all OPC XML string declarations.
///


#include "dwfcore/Core.h"
#include "dwf/Toolkit.h"


///
///\ingroup     opc
///
///\class       OPCXML  dwf/opc/Constants.h     "dwf/opc/Constants.h"
///\brief       This class scopes the OPC XML strings known to the toolkit.
///\since       7.4.0
///
class OPCXML
{

public:

    ///
    ///\brief   Pre-defined Schema ID for core properties
    ///
    static const char* const kzSchemaID_CoreProperties;

    ///
    ///\brief   "http://schemas.openxmlformats.org/package/2006/content-types"
    ///
    static const char* const kzNamespaceURI_ContentTypes;
    ///
    ///\brief   "http://schemas.openxmlformats.org/package/2006/metadata/core-properties"
    ///
    static const char* const kzNamespaceURI_CoreProperties;
    ///
    ///\brief   "http://schemas.openxmlformats.org/package/2006/relationships"
    ///
    static const char* const kzNamespaceURI_Relationsips;
    ///
    ///\brief   "http://purl.org/dc/elements/1.1/"
    ///
    static const char* const kzNamespaceURI_DublinCore;
    ///
    ///\brief   "http://purl.org/dc/terms/"
    ///
    static const char* const kzNamespaceURI_DublinCoreTerms;
    ///
    ///\brief   "http://www.w3.org/2001/XMLSchema-instance"
    ///
    static const char* const kzNamespaceURI_XSI;
    ///
    ///\brief   "dc:"
    ///
    static const char* const kzNamespace_DublinCore;
    ///
    ///\brief   "dcterms:"
    ///
    static const char* const kzNamespace_DublinCoreTerms;
    ///
    ///\brief   "xsi:"
    ///
    static const char* const kzNamespace_XSI;

    ///
    ///\brief   "Relationships"
    ///
    static const char* const kzElement_Relationships;
    ///
    ///\brief   "Relationship"
    ///
    static const char* const kzElement_Relationship;
    ///
    ///\brief   "Types"
    ///
    static const char* const kzElement_Types;
    ///
    ///\brief   "Default"
    ///
    static const char* const kzElement_Default;
    ///
    ///\brief   "Override"
    ///
    static const char* const kzElement_Override;
    ///
    ///\brief   "coreProperties"
    ///
    static const char* const kzElement_CoreProperties;

    ///
    ///\brief   "TargetMode"
    ///
    static const char* const kzAttribute_TargetMode;
    ///
    ///\brief   "Target"
    ///
    static const char* const kzAttribute_Target;
    ///
    ///\brief   "Type"
    ///
    static const char* const kzAttribute_Type;
    ///
    ///\brief   "Id"
    ///
    static const char* const kzAttribute_Id;
    ///
    ///\brief   "ContentType"
    ///
    static const char* const kzAttribute_ContentType;
    ///
    ///\brief   "Extension"
    ///
    static const char* const kzAttribute_Extension;
    ///
    ///\brief   "PartName"
    ///
    static const char* const kzAttribute_PartName;

    ///
    ///\brief   "Internal"
    ///
    static const char* const kzAttributeVal_TargetMode_Internal;
    ///
    ///\brief   "External"
    ///
    static const char* const kzAttributeVal_TargetMode_External;

    ///
    ///\brief   "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail"
    ///
    static const char* const kzRelationship_Thumbnail;
    ///
    ///\brief   "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties"
    ///
    static const char* const kzRelationship_CoreProperties;

    //
    // Properties in the OPC Namespace
    //
    ///
    ///\brief   L"keywords"
    ///
    static const char* const kzCoreProperty_Keywords;
    ///
    ///\brief   L"contentType"
    ///
    static const char* const kzCoreProperty_ContentType;
    ///
    ///\brief   L"category"
    ///
    static const char* const kzCoreProperty_Category;
    ///
    ///\brief   L"version"
    ///
    static const char* const kzCoreProperty_Version;
    ///
    ///\brief   L"revision"
    ///
    static const char* const kzCoreProperty_Revision;
    ///
    ///\brief   L"lastModifiedBy"
    ///
    static const char* const kzCoreProperty_LastModifiedBy;
    ///
    ///\brief   L"lastPrinted"
    ///
    static const char* const kzCoreProperty_LastPrinted;
    ///
    ///\brief   L"contentStatus"
    ///
    static const char* const kzCoreProperty_ContentStatus;

    //
    // Properties in the Dublin Core Namespace
    //
    ///
    ///\brief   L"creator"
    ///
    static const char* const kzCoreProperty_Creator;
    ///
    ///\brief   L"identifier"
    ///
    static const char* const kzCoreProperty_Identifier;
    ///
    ///\brief   L"title"
    ///
    static const char* const kzCoreProperty_Title;
    ///
    ///\brief   L"subject"
    ///
    static const char* const kzCoreProperty_Subject;
    ///
    ///\brief   L"description"
    ///
    static const char* const kzCoreProperty_Description;
    ///
    ///\brief   L"language"
    ///
    static const char* const kzCoreProperty_Language;

    //
    // Properties in the Dublin Core Namespace - under terms
    //
    ///
    ///\brief   L"created"
    ///
    static const char* const kzCoreProperty_Created;
    ///
    ///\brief   L"modified"
    ///
    static const char* const kzCoreProperty_Modified;

};


    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start

_declspec(selectany) const char* const OPCXML::kzSchemaID_CoreProperties = "6B241FB3-E174-4B01-9685-60381F320DA7";

_declspec(selectany) const char* const OPCXML::kzNamespaceURI_ContentTypes = "http://schemas.openxmlformats.org/package/2006/content-types";
_declspec(selectany) const char* const OPCXML::kzNamespaceURI_CoreProperties = "http://schemas.openxmlformats.org/package/2006/metadata/core-properties";
_declspec(selectany) const char* const OPCXML::kzNamespaceURI_Relationsips = "http://schemas.openxmlformats.org/package/2006/relationships";
_declspec(selectany) const char* const OPCXML::kzNamespaceURI_DublinCore = "http://purl.org/dc/elements/1.1/";
_declspec(selectany) const char* const OPCXML::kzNamespaceURI_DublinCoreTerms = "http://purl.org/dc/terms/";
_declspec(selectany) const char* const OPCXML::kzNamespaceURI_XSI = "http://www.w3.org/2001/XMLSchema-instance";
_declspec(selectany) const char* const OPCXML::kzNamespace_DublinCore = "dc:";
_declspec(selectany) const char* const OPCXML::kzNamespace_DublinCoreTerms = "dcterms:";
_declspec(selectany) const char* const OPCXML::kzNamespace_XSI = "xsi:";

_declspec(selectany) const char* const OPCXML::kzElement_Relationships = "Relationships";
_declspec(selectany) const char* const OPCXML::kzElement_Relationship = "Relationship";
_declspec(selectany) const char* const OPCXML::kzElement_Types = "Types";
_declspec(selectany) const char* const OPCXML::kzElement_Default = "Default";
_declspec(selectany) const char* const OPCXML::kzElement_Override = "Override";
_declspec(selectany) const char* const OPCXML::kzElement_CoreProperties = "coreProperties";

_declspec(selectany) const char* const OPCXML::kzAttribute_TargetMode = "TargetMode";
_declspec(selectany) const char* const OPCXML::kzAttribute_Target = "Target";
_declspec(selectany) const char* const OPCXML::kzAttribute_Type = "Type";
_declspec(selectany) const char* const OPCXML::kzAttribute_Id = "Id";
_declspec(selectany) const char* const OPCXML::kzAttribute_ContentType = "ContentType";
_declspec(selectany) const char* const OPCXML::kzAttribute_Extension = "Extension";
_declspec(selectany) const char* const OPCXML::kzAttribute_PartName = "PartName";

_declspec(selectany) const char* const OPCXML::kzAttributeVal_TargetMode_Internal = "Internal";
_declspec(selectany) const char* const OPCXML::kzAttributeVal_TargetMode_External = "External";

_declspec(selectany) const char* const OPCXML::kzRelationship_Thumbnail = "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail";
_declspec(selectany) const char* const OPCXML::kzRelationship_CoreProperties = "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties";

//
// Properties in the OPC Namespace
//
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Keywords = "keywords";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_ContentType = "contentType";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Category = "category";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Version = "version";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Revision = "revision";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_LastModifiedBy = "lastModifiedBy";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_LastPrinted = "lastPrinted";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_ContentStatus = "contentStatus";
//
// Properties in the Dublin Core Namespace
//
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Creator = "creator";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Identifier = "identifier";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Title = "title";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Subject = "subject";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Description = "description";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Language = "language";
//
// Properties in the Dublin Core Namespace - under terms
//
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Created = "created";
_declspec(selectany) const char* const OPCXML::kzCoreProperty_Modified = "modified";

//DNT_End


#endif
#endif

#endif
