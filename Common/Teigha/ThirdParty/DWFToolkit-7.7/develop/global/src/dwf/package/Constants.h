//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Constants.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_CONSTANTS_H
#define _DWFTK_CONSTANTS_H


///
///\file        dwf/package/Constants.h
///\brief       This file contains all DWF XML string declarations.
///


#include "dwfcore/Core.h"
#include "dwf/Toolkit.h"


///
///\ingroup     dwfpackage
///
///\class       DWFXML  dwf/package/Constants.h     "dwf/package/Constants.h"
///\brief       This class scopes the DWF XML strings known to the toolkit.
///\since       7.0.1
///
class DWFXML
{

public:

    ///
    ///\brief   "dwf:"
    ///
    static const char* const kzNamespace_DWF;
    ///
    ///\brief   "ePlot:"
    ///
    static const char* const kzNamespace_EPlot;
    ///
    ///\brief   "eModel:"
    ///
    static const char* const kzNamespace_EModel;
    ///
    ///\brief   "Data:"
    ///
    static const char* const kzNamespace_Data;
    ///
    ///\brief   "eCommon:"
    ///
    static const char* const kzNamespace_ECommon;
    ///
    ///\brief   "Signature:"
    ///
    static const char* const kzNamespace_Signatures;

    ///
    ///\brief   "Toc"
    ///
    static const char* const kzElement_TOC;
    ///
    ///\brief   "Node"
    ///
    static const char* const kzElement_Node;
    ///
    ///\brief   "Page"
    ///
    static const char* const kzElement_Page;
    ///
    ///\brief   "View"
    ///
    static const char* const kzElement_View;
    ///
    ///\brief   "Class"
    ///
    static const char* const kzElement_Class;
    ///
    ///\brief   "Group"
    ///
    static const char* const kzElement_Group;
    ///
    ///\brief   "Nodes"
    ///
    static const char* const kzElement_Nodes;
    ///
    ///\brief   "Paper"
    ///
    static const char* const kzElement_Paper;
    ///
    ///\brief   "Space"
    ///
    static const char* const kzElement_Space;
    ///
    ///\brief   "Units"
    ///
    static const char* const kzElement_Units;
    ///
    ///\brief   "Views"
    ///
    static const char* const kzElement_Views;
    ///
    ///\brief   "Entity"
    ///
    static const char* const kzElement_Entity;
    ///
    ///\brief   "Global"
    ///
    static const char* const kzElement_Global;
    ///
    ///\brief   "Groups"
    ///
    static const char* const kzElement_Groups;
    ///
    ///\brief   "Source"
    ///
    static const char* const kzElement_Source;
    ///
    ///\brief   "Object"
    ///
    static const char* const kzElement_Object;
    ///
    ///\brief   "Feature"
    ///
    static const char* const kzElement_Feature;
    ///
    ///\brief   "Objects"
    ///
    static const char* const kzElement_Objects;
    ///
    ///\brief   "Section"
    ///
    static const char* const kzElement_Section;
    ///
    ///\brief   "Classes"
    ///
    static const char* const kzElement_Classes;
    ///
    ///\brief   "Content"
    ///
    static const char* const kzElement_Content;
    ///
    ///\brief   "Contents"
    ///
    static const char* const kzElement_Contents;
    ///
    ///\brief   "Entities"
    ///
    static const char* const kzElement_Entities;
    ///
    ///\brief   "Features"
    ///
    static const char* const kzElement_Features;
    ///
    ///\brief   "Sections"
    ///
    static const char* const kzElement_Sections;
    ///
    ///\brief   "Manifest"
    ///
    static const char* const kzElement_Manifest;
    ///
    ///\brief   "Bookmark"
    ///
    static const char* const kzElement_Bookmark;
    ///
    ///\brief   "Instance"
    ///
    static const char* const kzElement_Instance;
    ///
    ///\brief   "Property"
    ///
    static const char* const kzElement_Property;
    ///
    ///\brief   "Resource"
    ///
    static const char* const kzElement_Resource;
    ///
    ///\brief   "Instances"
    ///
    static const char* const kzElement_Instances;
    ///
    ///\brief   "Interface"
    ///
    static const char* const kzElement_Interface;
    ///
    ///\brief   "Resources"
    ///
    static const char* const kzElement_Resources;
    ///
    ///\brief   "Interfaces"
    ///
    static const char* const kzElement_Interfaces;
    ///
    ///\brief   "Properties"
    ///
    static const char* const kzElement_Properties;
    ///
    ///\brief   "Dependency"
    ///
    static const char* const kzElement_Dependency;
    ///
    ///\brief   "Dependencies"
    ///
    static const char* const kzElement_Dependencies;
    ///
    ///\brief   "FontResource"
    ///
    static const char* const kzElement_FontResource;
    ///
    ///\brief   "Presentation"
    ///
    static const char* const kzElement_Presentation;
    ///
    ///\brief   "Presentations"
    ///
    static const char* const kzElement_Presentations;
    ///
    ///\brief   "ImageResource"
    ///
    static const char* const kzElement_ImageResource;
    ///
    ///\brief   "ReferenceNode"
    ///
    static const char* const kzElement_ReferenceNode;
    ///
    ///\brief   "ContentElement"
    ///
    static const char* const kzElement_ContentElement;
    ///
    ///\brief   "SectionContent"
    ///
    static const char* const kzElement_SectionContent;
    ///
    ///\brief   "GraphicResource"
    ///
    static const char* const kzElement_GraphicResource;
    //
    // Intentionally not documented.
    // This is a tag that we generated early in 7.2 development, and
    // are just keeping (for now) to keep from invalidating files created
    // during development.
    //
    static const char* const kzElement_ContentResource;
    ///
    ///\brief   "ObjectDefinition"
    ///
    static const char* const kzElement_ObjectDefinition;
    ///
    ///\brief   "SharedProperties"
    ///
    static const char* const kzElement_SharedProperties;
    ///
    ///\brief   L"PropertyReference"
    ///
    static const char* const kzElement_PropertyReference;
    ///
    ///\brief   L"PropertyReferences"
    ///
    static const char* const kzElement_PropertyReferences;
    ///
    ///\brief   "PageObjectDefinition"
    ///
    static const char* const kzElement_PageObjectDefinition;
    ///
    ///\brief   "SpaceObjectDefinition"
    ///
    static const char* const kzElement_SpaceObjectDefinition;
    ///
    ///\brief   "GlobalObjectDefinition"
    ///
    static const char* const kzElement_GlobalObjectDefinition;
    ///
    ///\brief   "ContentPresentation"
    ///
    static const char* const kzElement_ContentPresentation;
    ///
    ///\brief   "ContentPresentationResource"
    ///
    static const char* const kzElement_ContentPresentationResource;
    ///
    ///\brief   L"Camera"
    ///
    static const char* const kzElement_Camera;
    ///
    ///\brief   "Color"
    ///
    static const char* const kzElement_Color;
    ///
    ///\brief   "CuttingPlane"
    ///
    static const char* const kzElement_CuttingPlane;
    ///
    ///\brief   "ModellingMatrix"
    ///
    static const char* const kzElement_ModellingMatrix;
    ///
    ///\brief   "TextureMatrix"
    ///
    static const char* const kzElement_TextureMatrix;
    ///
    ///\brief   "Visibility"
    ///
    static const char* const kzElement_Visibility;
    ///
    ///\brief   "InstanceVisibility"
    ///
    static const char* const kzElement_InstanceVisibility;
    ///
    ///\brief   "InstanceTransparency"
    ///
    static const char* const kzElement_InstanceTransparency;
    ///
    ///\brief   "GeometricVariation"
    ///
    static const char* const kzElement_GeometricVariation;
    ///
    ///\brief   "ModelViewNode"
    ///
    static const char* const kzElement_ModelViewNode;
    ///
    ///\brief   "InstanceAttributes"
    ///
    static const char* const kzElement_InstanceAttributes;
    ///
    ///\brief   "ModelScene"
    ///
    static const char* const kzElement_ModelScene;
    ///
    ///\brief   "Channels"
    ///
    static const char* const kzElement_Channels;
    ///
    ///\brief   "Channel"
    ///
    static const char* const kzElement_Channel;
    ///
    ///\brief   "Planes"
    ///
    static const char* const kzElement_Planes;
    ///
    ///\brief   "Plane"
    ///
    static const char* const kzElement_Plane;
    ///
    ///\brief   "AttributeLock"
    ///
    static const char* const kzElement_AttributeLock;
    ///
    ///\brief   "AttributeUnlock"
    ///
    static const char* const kzElement_AttributeUnlock;
    ///
    ///\brief   "PropertyReferenceURI"
    ///
    static const char* const kzElement_PropertyReferenceURI;
    ///
    ///\brief   L"PropertyReferenceURIs"
    ///
    static const char* const kzElement_PropertyReferenceURIs;
    ///
    ///\brief   "DisplayMode"
    ///
    static const char* const kzElement_DisplayMode;
    ///
    ///\brief   "UserAttributes"
    ///
    static const char* const kzElement_UserAttributes;
    ///
    ///\brief   L"SignatureResource"
    ///
    static const char* const kzElement_SignatureResource;

    ///
    ///\brief L"CanonicalizationMethod" 
    ///
    static const char* const kzElement_CanonicalizationMethod;
    ///
    ///\brief L"DigestMethod" 
    ///
    static const char* const kzElement_DigestMethod;
    ///
    ///\brief L"DigestValue" 
    ///
    static const char* const kzElement_DigestValue;
    ///
    ///\brief L"DSAKeyValue" 
    ///
    static const char* const kzElement_DSAKeyValue;
    ///
    ///\brief L"P" 
    ///
    static const char* const kzElement_DSAKeyValueP;
    ///
    ///\brief L"Q" 
    ///
    static const char* const kzElement_DSAKeyValueQ;
    ///
    ///\brief L"G" 
    ///
    static const char* const kzElement_DSAKeyValueG;
    ///
    ///\brief L"Y" 
    ///
    static const char* const kzElement_DSAKeyValueY;
    ///
    ///\brief L"J" 
    ///
    static const char* const kzElement_DSAKeyValueJ;
    ///
    ///\brief L"Seed" 
    ///
    static const char* const kzElement_DSAKeyValueSeed;
    ///
    ///\brief L"PgenCounter" 
    ///
    static const char* const kzElement_DSAKeyValuePgenCounter;
    ///
    ///\brief L"KeyInfo" 
    ///
    static const char* const kzElement_KeyInfo;
    ///
    ///\brief L"KeyName" 
    ///
    static const char* const kzElement_KeyName;
    ///
    ///\brief L"KeyValue" 
    ///
    static const char* const kzElement_KeyValue;
    ///
    ///\brief L"Reference" 
    ///
    static const char* const kzElement_Reference;
    ///
    ///\brief L"RSAKeyValue" 
    ///
    static const char* const kzElement_RSAKeyValue;
    ///
    ///\brief L"Modulus" 
    ///
    static const char* const kzElement_RSAKeyValueModulus;
    ///
    ///\brief L"Exponent" 
    ///
    static const char* const kzElement_RSAKeyValueExponent;
    ///
    ///\brief L"Signature" 
    ///
    static const char* const kzElement_Signature;
    ///
    ///\brief L"SignatureMethod" 
    ///
    static const char* const kzElement_SignatureMethod;
    ///
    ///\brief L"SignatureValue" 
    ///
    static const char* const kzElement_SignatureValue;
    ///
    ///\brief L"SignedInfo" 
    ///
    static const char* const kzElement_SignedInfo;
    ///
    ///\brief L"X509Certificate" 
    ///
    static const char* const kzElement_X509Certificate;
    ///
    ///\brief L"X509CRL" 
    ///
    static const char* const kzElement_X509CRL;
    ///
    ///\brief L"X509Data" 
    ///
    static const char* const kzElement_X509Data;
    ///
    ///\brief L"X509IssuerSerial" 
    ///
    static const char* const kzElement_X509IssuerSerial;
    ///
    ///\brief L"X509IssuerName" 
    ///
    static const char* const kzElement_X509IssuerName;
    ///
    ///\brief L"X509SerialNumber" 
    ///
    static const char* const kzElement_X509SerialNumber;
    ///
    ///\brief L"X509SKI" 
    ///
    static const char* const kzElement_X509SKI;
    ///
    ///\brief L"X509SubjectName" 
    ///
    static const char* const kzElement_X509SubjectName;
    ///
    ///\brief L"CoordinateSystem" 
    ///
    static const char* const kzElement_CoordinateSystem;
    ///
    ///\brief L"CoordinateSystems" 
    ///
    static const char* const kzElement_CoordinateSystems;
    ///
    ///\brief L"Relationship" 
    ///
    static const char* const kzElement_Relationship;
    ///
    ///\brief L"Relationships" 
    ///
    static const char* const kzElement_Relationships;

    ///
    ///\brief   "id"
    ///
    static const char* const kzAttribute_ID;
    ///
    ///\brief   "uri"
    ///
    static const char* const kzAttribute_URI;
    ///
    ///\brief   "name"
    ///
    static const char* const kzAttribute_Name;
    ///
    ///\brief   "type"
    ///
    static const char* const kzAttribute_Type;
    ///
    ///\brief   "href"
    ///
    static const char* const kzAttribute_HRef;
    ///
    ///\brief   "refs"
    ///
    static const char* const kzAttribute_Refs;
    ///
    ///\brief   "mime"
    ///
    static const char* const kzAttribute_MIME;
    ///
    ///\brief   "role"
    ///
    static const char* const kzAttribute_Role;
    ///
    ///\brief   "clip"
    ///
    static const char* const kzAttribute_Clip;
    ///
    ///\brief   "show"
    ///
    static const char* const kzAttribute_Show;
    ///
    ///\brief   "size"
    ///
    static const char* const kzAttribute_Size;
    ///
    ///\brief   "node"
    ///
    static const char* const kzAttribute_Node;
    ///
    ///\brief   "nodes"
    ///
    static const char* const kzAttribute_Nodes;
    ///
    ///\brief   "label"
    ///
    static const char* const kzAttribute_Label;
    ///
    ///\brief   "value"
    ///
    static const char* const kzAttribute_Value;
    ///
    ///\brief   "units"
    ///
    static const char* const kzAttribute_Units;
    ///
    ///\brief   "title"
    ///
    static const char* const kzAttribute_Title;
    ///
    ///\brief   "color"
    ///
    static const char* const kzAttribute_Color;
    ///
    ///\brief   "setId"
    ///
    static const char* const kzAttribute_SetID;
    ///
    ///\brief   "width"
    ///
    static const char* const kzAttribute_Width;
    ///
    ///\brief   "height"
    ///
    static const char* const kzAttribute_Height;
    ///
    ///\brief   "hidden"
    ///
    static const char* const kzAttribute_Hidden;
    ///
    ///\brief   "zorder"
    ///
    static const char* const kzAttribute_ZOrder;
    ///
    ///\brief   "closed"
    ///
    static const char* const kzAttribute_Closed;
    ///
    ///\brief   "object"
    ///
    static const char* const kzAttribute_Object;
    ///
    ///\brief   "author"
    ///
    static const char* const kzAttribute_Author;
    ///
    ///\brief   "visible"
    ///
    static const char* const kzAttribute_Visible;
    ///
    ///\brief   "extents"
    ///
    static const char* const kzAttribute_Extents;
    ///
    ///\brief   "version"
    ///
    static const char* const kzAttribute_Version;
    ///
    ///\brief   "request"
    ///
    static const char* const kzAttribute_Request;
    ///
    ///\brief   "scanned"
    ///
    static const char* const kzAttribute_Scanned;
    ///
    ///\brief   "schemaId"
    ///
    static const char* const kzAttribute_SchemaID;
    ///
    ///\brief   "children"
    ///
    static const char* const kzAttribute_Children;
    ///
    ///\brief   "provider"
    ///
    static const char* const kzAttribute_Provider;
    ///
    ///\brief   "objectId"
    ///
    static const char* const kzAttribute_ObjectID;
    ///
    ///\brief   "category"
    ///
    static const char* const kzAttribute_Category;
    ///
    ///\brief   "contentId"
    ///
    static const char* const kzAttribute_ContentID;
    ///
    ///\brief   "transform"
    ///
    static const char* const kzAttribute_Transform;
    ///
    ///\brief   "plotOrder"
    ///
    static const char* const kzAttribute_PlotOrder;
    ///
    ///\brief   "privilege"
    ///
    static const char* const kzAttribute_Privilege;
    ///
    ///\brief   "contentIds"
    ///
    static const char* const kzAttribute_ContentIDs;
    ///
    ///\brief   "internalId"
    ///
    static const char* const kzAttribute_InternalID;
    ///
    ///\brief   "colorDepth"
    ///
    static const char* const kzAttribute_ColorDepth;
    ///
    ///\brief   "transparent"
    ///
    static const char* const kzAttribute_Transparent;
    ///
    ///\brief   "exclusivity"
    ///
    static const char* const kzAttribute_Exclusivity;
    ///
    ///\brief   "description"
    ///
    static const char* const kzAttribute_Description;
    ///
    ///\brief   "orientation"
    ///
    static const char* const kzAttribute_Orientation;
    ///
    ///\brief   "logfontName"
    ///
    static const char* const kzAttribute_LogfontName;
    ///
    ///\brief   "resourceURI"
    ///
    static const char* const kzAttribute_ResourceURI;
    ///
    ///\brief   "invertColors"
    ///
    static const char* const kzAttribute_InvertColors;
    ///
    ///\brief   "creationTime"
    ///
    static const char* const kzAttribute_CreationTime;
    ///
    ///\brief   "canonicalName"
    ///
    static const char* const kzAttribute_CanonicalName;
    ///
    ///\brief   "characterCode"
    ///
    static const char* const kzAttribute_CharacterCode;
    ///
    ///\brief   "schemaLocation"
    ///
    static const char* const kzAttribute_SchemaLocation;
    ///
    ///\brief   "defaultIconURI"
    ///
    static const char* const kzAttribute_DefaultIconURI;
    ///
    ///\brief   "activeIconURI"
    ///
    static const char* const kzAttribute_ActiveIconURI;
    ///
    ///\brief   "parentObjectId"
    ///
    static const char* const kzAttribute_ParentObjectID;
    ///
    ///\brief   "originalExtents"
    ///
    static const char* const kzAttribute_OriginalExtents;
    ///
    ///\brief   "expandedIconURI"
    ///
    static const char* const kzAttribute_ExpandedIconURI;
    ///
    ///\brief   "modificationTime"
    ///
    static const char* const kzAttribute_ModificationTime;
    ///
    ///\brief   "primaryContentId"
    ///
    static const char* const kzAttribute_PrimaryContentID;
    ///
    ///\brief   "scannedResolution"
    ///
    static const char* const kzAttribute_ScannedResolution;
    ///
    ///\brief   "geometricVariation"
    ///
    static const char* const kzAttribute_GeometricVariation;
    ///
    ///\brief   "effectiveResolution"
    ///
    static const char* const kzAttribute_EffectiveResolution;
    ///
    ///\brief   "classRefs"
    ///
    static const char* const kzAttribute_ClassRefs;
    ///
    ///\brief   "entityRef"
    ///
    static const char* const kzAttribute_EntityRef;
    ///
    ///\brief   "entityRefs"
    ///
    static const char* const kzAttribute_EntityRefs;
    ///
    ///\brief   "featureRefs"
    ///
    static const char* const kzAttribute_FeatureRefs;
    ///
    ///\brief   "renderableRef"
    ///
    static const char* const kzAttribute_RenderableRef;
    ///
    ///\brief   "contentElementRefs"
    ///
    static const char* const kzAttribute_ContentElementRefs;
    ///
    ///\brief   "positionX"
    ///
    static const char* const kzAttribute_PositionX;
    ///
    ///\brief   "positionY"
    ///
    static const char* const kzAttribute_PositionY;
    ///
    ///\brief   "positionZ"
    ///
    static const char* const kzAttribute_PositionZ;
    ///
    ///\brief   "targetX"
    ///
    static const char* const kzAttribute_TargetX;
    ///
    ///\brief   "targetY"
    ///
    static const char* const kzAttribute_TargetY;
    ///
    ///\brief   "targetZ"
    ///
    static const char* const kzAttribute_TargetZ;
    ///
    ///\brief   "upVectorX"
    ///
    static const char* const kzAttribute_UpVectorX;
    ///
    ///\brief   "upVectorY"
    ///
    static const char* const kzAttribute_UpVectorY;
    ///
    ///\brief   "upVectorZ"
    ///
    static const char* const kzAttribute_UpVectorZ;
    ///
    ///\brief   "fieldWidth"
    ///
    static const char* const kzAttribute_FieldWidth;
    ///
    ///\brief   "fieldHeight"
    ///
    static const char* const kzAttribute_FieldHeight;
    ///
    ///\brief   "projectionType"
    ///
    static const char* const kzAttribute_ProjectionType;
    ///
    ///\brief   "resetFlags"
    ///
    static const char* const kzAttribute_ResetFlags;
    ///
    ///\brief   "mask"
    ///
    static const char* const kzAttribute_Mask;
    ///
    ///\brief   "red"
    ///
    static const char* const kzAttribute_Red;
    ///
    ///\brief   "green"
    ///
    static const char* const kzAttribute_Green;
    ///
    ///\brief   "blue"
    ///
    static const char* const kzAttribute_Blue;
    ///
    ///\brief   "gloss"
    ///
    static const char* const kzAttribute_Gloss;
    ///
    ///\brief   "index"
    ///
    static const char* const kzAttribute_Index;
    ///
    ///\brief   "count"
    ///
    static const char* const kzAttribute_Count;
    ///
    ///\brief   "a"
    ///
    static const char* const kzAttribute_A;
    ///
    ///\brief   "b"
    ///
    static const char* const kzAttribute_B;
    ///
    ///\brief   "c"
    ///
    static const char* const kzAttribute_C;
    ///
    ///\brief   "d"
    ///
    static const char* const kzAttribute_D;
    ///
    ///\brief   "elements"
    ///
    static const char* const kzAttribute_Elements;
    ///
    ///\brief   "state"
    ///
    static const char* const kzAttribute_State;
    ///
    ///\brief   "mode"
    ///
    static const char* const kzAttribute_Mode;
    ///
    ///\brief   "propertyReferenceID"
    ///
    static const char* const kzAttribute_PropertyReferenceID;
    ///
    ///\brief   "smoothTransition"
    ///
    static const char* const kzAttribute_SmoothTransition;
    ///
    ///\brief   "useDefaultPropertyIfMissing"
    ///
    static const char* const kzAttribute_UseDefaultPropertyIfMissing;
    ///
    ///\brief "Algorithm" 
    ///
    static const char* const kzAttribute_Algorithm;
    ///
    ///\brief "originX" 
    ///
    static const char* const kzAttribute_OriginX;
    ///
    ///\brief "originY" 
    ///
    static const char* const kzAttribute_OriginY;
    ///
    ///\brief "originZ" 
    ///
    static const char* const kzAttribute_OriginZ;
    ///
    ///\brief "rotation" 
    ///
    static const char* const kzAttribute_Rotation;
    ///
    ///\brief "TransitionType" 
    ///
    static const char* const kzAttribute_TransitionType;
    ///
    ///\brief "Duration" 
    ///
    static const char* const kzAttribute_Duration;
    ///
    ///\brief "LeftRightAngle" 
    ///
    static const char* const kzAttribute_LeftRightAngle;
    ///
    ///\brief "UpDownAngle" 
    ///
    static const char* const kzAttribute_UpDownAngle;
    ///
    ///\brief "CameraPosition" 
    ///
    static const char* const kzAttribute_CameraPosition;
	///
	///\brief "CinematicType" 
	///
	static const char* const kzAttribute_CinematicType;
    ///
    ///\brief "InOutDistance" 
    ///
    static const char* const kzAttribute_InOutDistance;
    ///
    ///\brief "LeftRightDistance" 
    ///
    static const char* const kzAttribute_LeftRightDistance;
    ///
    ///\brief "UpDownDistance" 
    ///
    static const char* const kzAttribute_UpDownDistance;
    ///
    ///\brief "LockCamera" 
    ///
    static const char* const kzAttribute_LockCamera;

    ///
    ///\brief "SourceProductVendor" 
    ///
    static const char* const kzDWFProperty_SourceProductVendor;
    ///
    ///\brief "SourceProductName" 
    ///
    static const char* const kzDWFProperty_SourceProductName;
    ///
    ///\brief "SourceProductVersion" 
    ///
    static const char* const kzDWFProperty_SourceProductVersion;
    ///
    ///\brief "DWFProductVendor" 
    ///
    static const char* const kzDWFProperty_DWFProductVendor;
    ///
    ///\brief "DWFProductVersion" 
    ///
    static const char* const kzDWFProperty_DWFProductVersion;
    ///
    ///\brief "DWFToolkitVersion" 
    ///
    static const char* const kzDWFProperty_DWFToolkitVersion;
    ///
    ///\brief "DWFFormatVersion" 
    ///
    static const char* const kzDWFProperty_DWFFormatVersion;
	///
 	///\brief "PasswordEncryptedDocument"
 	///
 	static const char* const kzDWFProperty_PasswordEncryptedDocument;


    ///
    ///\brief   L"2d streaming graphics"
    ///
    static const wchar_t* const kzRole_Graphics2d;
    ///
    ///\brief   L"3d streaming graphics"
    ///
    static const wchar_t* const kzRole_Graphics3d;
    ///
    ///\brief   L"descriptor"
    ///
    static const wchar_t* const kzRole_Descriptor;
    ///
    ///\brief   L"thumbnail"
    ///
    static const wchar_t* const kzRole_Thumbnail;
    ///
    ///\brief   L"2d vector overlay"
    ///
    static const wchar_t* const kzRole_Graphics2dOverlay;
    ///
    ///\brief   L"2d vector markup"
    ///
    static const wchar_t* const kzRole_Graphics2dMarkup;
    ///
    ///\brief   L"RML markup"
    ///
    static const wchar_t* const kzRole_RmlMarkup;
    ///
    ///\brief   L"preview"
    ///
    static const wchar_t* const kzRole_Preview;
    ///
    ///\brief   L"overlay preview"
    ///
    static const wchar_t* const kzRole_OverlayPreview;
    ///
    ///\brief   L"markup preview"
    ///
    static const wchar_t* const kzRole_MarkupPreview;
    ///
    ///\brief   L"raster overlay"
    ///
    static const wchar_t* const kzRole_RasterOverlay;
    ///
    ///\brief   L"raster markup"
    ///
    static const wchar_t* const kzRole_RasterMarkup;
    ///
    ///\brief   L"font"
    ///
    static const wchar_t* const kzRole_Font;
    ///
    ///\brief   L"metadata"
    ///
    static const wchar_t* const kzRole_Metadata;
    ///
    ///\brief   L"package signature"
    ///
    static const wchar_t* const kzRole_PackageSignature;
    ///
    ///\brief   L"section signature"
    ///
    static const wchar_t* const kzRole_SectionSignature;
    ///
    ///\brief   L"fileset signature"
    ///
    static const wchar_t* const kzRole_FilesetSignature;
    ///
    ///\brief   L"signature"
    ///
    static const wchar_t* const kzRole_Signature;
    ///
    ///\brief   L"object definition"
    ///
    static const wchar_t* const kzRole_ObjectDefinition;
    ///
    ///\brief   L"markup object definition"
    ///
    static const wchar_t* const kzRole_MarkupObjectDefinition;
    ///
    ///\brief   L"texture"
    ///
    static const wchar_t* const kzRole_Texture;
    ///
    ///\brief   L"content presentation"
    ///
    static const wchar_t* const kzRole_ContentPresentation;
    ///
    ///\brief   L"markup content presentation"
    ///
    static const wchar_t* const kzRole_MarkupContentPresentation;
    ///
    ///\brief   L"animation"
    ///
    static const wchar_t* const kzRole_Animation;
    ///
    ///\brief   L"note"
    ///
    static const wchar_t* const kzRole_Note;
    ///
    ///\brief   L"3D markup graphics"
    ///
    static const wchar_t* const kzRole_Graphics3dMarkup;
    ///
    ///\brief   L"3D markup"
    ///
    static const wchar_t* const kzRole_3dMarkup;
    ///
    ///\brief   L"content definition"
    ///
    static const wchar_t* const kzRole_ContentDefinition;
    ///
    ///\brief   L"icon"
    ///
    static const wchar_t* const kzRole_Icon;
    ///
    ///\brief   L"observation mesh"
    ///
    static const wchar_t* const kzRole_ObservationMesh;
    ///
    ///\brief   L"geographic coordinate system"
    ///
    static const wchar_t* const kzRole_GeographicCoordinateSystem;
    ///
    ///\brief   L"2d graphics extension"
    ///
    static const wchar_t* const kzRole_Graphics2dExtension;
    ///
    ///\brief   L"2d graphics dictionary"
    ///
    static const wchar_t* const kzRole_Graphics2dDictionary;
    ///
    ///\brief   L"raster reference"
    ///
    static const wchar_t* const kzRole_RasterReference;
};

    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start

_declspec(selectany) const char* const DWFXML::kzNamespace_DWF                          = "dwf:";
_declspec(selectany) const char* const DWFXML::kzNamespace_EPlot                        = "ePlot:";
_declspec(selectany) const char* const DWFXML::kzNamespace_EModel                       = "eModel:";
_declspec(selectany) const char* const DWFXML::kzNamespace_Data                         = "Data:";
_declspec(selectany) const char* const DWFXML::kzNamespace_ECommon                      = "eCommon:";
_declspec(selectany) const char* const DWFXML::kzNamespace_Signatures                   = "Signatures:";

_declspec(selectany) const char* const DWFXML::kzElement_TOC                            = "Toc";
_declspec(selectany) const char* const DWFXML::kzElement_Node                           = "Node";
_declspec(selectany) const char* const DWFXML::kzElement_Page                           = "Page";
_declspec(selectany) const char* const DWFXML::kzElement_View                           = "View";
_declspec(selectany) const char* const DWFXML::kzElement_Class                          = "Class";
_declspec(selectany) const char* const DWFXML::kzElement_Group                          = "Group";
_declspec(selectany) const char* const DWFXML::kzElement_Nodes                          = "Nodes";
_declspec(selectany) const char* const DWFXML::kzElement_Paper                          = "Paper";
_declspec(selectany) const char* const DWFXML::kzElement_Space                          = "Space";
_declspec(selectany) const char* const DWFXML::kzElement_Units                          = "Units";
_declspec(selectany) const char* const DWFXML::kzElement_Views                          = "Views";
_declspec(selectany) const char* const DWFXML::kzElement_Entity                         = "Entity";
_declspec(selectany) const char* const DWFXML::kzElement_Global                         = "Global";
_declspec(selectany) const char* const DWFXML::kzElement_Groups                         = "Groups";
_declspec(selectany) const char* const DWFXML::kzElement_Source                         = "Source";
_declspec(selectany) const char* const DWFXML::kzElement_Object                         = "Object";
_declspec(selectany) const char* const DWFXML::kzElement_Feature                        = "Feature";
_declspec(selectany) const char* const DWFXML::kzElement_Objects                        = "Objects";
_declspec(selectany) const char* const DWFXML::kzElement_Section                        = "Section";
_declspec(selectany) const char* const DWFXML::kzElement_Classes                        = "Classes";
_declspec(selectany) const char* const DWFXML::kzElement_Content                        = "Content";
_declspec(selectany) const char* const DWFXML::kzElement_Contents                       = "Contents";
_declspec(selectany) const char* const DWFXML::kzElement_Entities                       = "Entities";
_declspec(selectany) const char* const DWFXML::kzElement_Features                       = "Features";
_declspec(selectany) const char* const DWFXML::kzElement_Sections                       = "Sections";
_declspec(selectany) const char* const DWFXML::kzElement_Manifest                       = "Manifest";
_declspec(selectany) const char* const DWFXML::kzElement_Bookmark                       = "Bookmark";
_declspec(selectany) const char* const DWFXML::kzElement_Property                       = "Property";
_declspec(selectany) const char* const DWFXML::kzElement_Resource                       = "Resource";
_declspec(selectany) const char* const DWFXML::kzElement_Instance                       = "Instance";
_declspec(selectany) const char* const DWFXML::kzElement_Instances                      = "Instances";
_declspec(selectany) const char* const DWFXML::kzElement_Interface                      = "Interface";
_declspec(selectany) const char* const DWFXML::kzElement_Resources                      = "Resources";
_declspec(selectany) const char* const DWFXML::kzElement_Interfaces                     = "Interfaces";
_declspec(selectany) const char* const DWFXML::kzElement_Properties                     = "Properties";
_declspec(selectany) const char* const DWFXML::kzElement_Dependency                     = "Dependency";
_declspec(selectany) const char* const DWFXML::kzElement_Dependencies                   = "Dependencies";
_declspec(selectany) const char* const DWFXML::kzElement_FontResource                   = "FontResource";
_declspec(selectany) const char* const DWFXML::kzElement_Presentation                   = "Presentation";
_declspec(selectany) const char* const DWFXML::kzElement_Presentations                  = "Presentations";
_declspec(selectany) const char* const DWFXML::kzElement_ImageResource                  = "ImageResource";
_declspec(selectany) const char* const DWFXML::kzElement_ReferenceNode                  = "ReferenceNode";
_declspec(selectany) const char* const DWFXML::kzElement_ContentElement                 = "ContentElement";
_declspec(selectany) const char* const DWFXML::kzElement_SectionContent                 = "SectionContent";
_declspec(selectany) const char* const DWFXML::kzElement_GraphicResource                = "GraphicResource";
_declspec(selectany) const char* const DWFXML::kzElement_ContentResource                = "ContentResource";
_declspec(selectany) const char* const DWFXML::kzElement_ObjectDefinition               = "ObjectDefinition";
_declspec(selectany) const char* const DWFXML::kzElement_SharedProperties               = "SharedProperties";
_declspec(selectany) const char* const DWFXML::kzElement_PropertyReference              = "PropertyReference";
_declspec(selectany) const char* const DWFXML::kzElement_PropertyReferences             = "PropertyReferences";
_declspec(selectany) const char* const DWFXML::kzElement_ContentPresentation            = "ContentPresentation";
_declspec(selectany) const char* const DWFXML::kzElement_PageObjectDefinition           = "PageObjectDefinition";
_declspec(selectany) const char* const DWFXML::kzElement_SpaceObjectDefinition          = "SpaceObjectDefinition";
_declspec(selectany) const char* const DWFXML::kzElement_GlobalObjectDefinition         = "GlobalObjectDefinition";
_declspec(selectany) const char* const DWFXML::kzElement_ContentPresentationResource    = "ContentPresentationResource";
_declspec(selectany) const char* const DWFXML::kzElement_Camera                         = "Camera";
_declspec(selectany) const char* const DWFXML::kzElement_Color                          = "Color";
_declspec(selectany) const char* const DWFXML::kzElement_CuttingPlane                   = "CuttingPlane";
_declspec(selectany) const char* const DWFXML::kzElement_ModellingMatrix                = "ModellingMatrix";
_declspec(selectany) const char* const DWFXML::kzElement_TextureMatrix                  = "TextureMatrix";
_declspec(selectany) const char* const DWFXML::kzElement_Visibility                     = "Visibility";
_declspec(selectany) const char* const DWFXML::kzElement_InstanceVisibility             = "InstanceVisibility";
_declspec(selectany) const char* const DWFXML::kzElement_InstanceTransparency           = "InstanceTransparency";
_declspec(selectany) const char* const DWFXML::kzElement_GeometricVariation             = "GeometricVariation";
_declspec(selectany) const char* const DWFXML::kzElement_ModelViewNode                  = "ModelViewNode";
_declspec(selectany) const char* const DWFXML::kzElement_InstanceAttributes             = "InstanceAttributes";
_declspec(selectany) const char* const DWFXML::kzElement_ModelScene                     = "ModelScene";
_declspec(selectany) const char* const DWFXML::kzElement_Channels                       = "Channels";
_declspec(selectany) const char* const DWFXML::kzElement_Channel                        = "Channel";
_declspec(selectany) const char* const DWFXML::kzElement_Planes                         = "Planes";
_declspec(selectany) const char* const DWFXML::kzElement_Plane                          = "Plane";
_declspec(selectany) const char* const DWFXML::kzElement_AttributeLock                  = "AttributeLock";
_declspec(selectany) const char* const DWFXML::kzElement_AttributeUnlock                = "AttributeUnlock";
_declspec(selectany) const char* const DWFXML::kzElement_PropertyReferenceURI           = "PropertyReferenceURI";
_declspec(selectany) const char* const DWFXML::kzElement_PropertyReferenceURIs          = "PropertyReferenceURIs";
_declspec(selectany) const char* const DWFXML::kzElement_DisplayMode                    = "DisplayMode";
_declspec(selectany) const char* const DWFXML::kzElement_UserAttributes                 = "UserAttributes";
_declspec(selectany) const char* const DWFXML::kzElement_SignatureResource              = "SignatureResource";
_declspec(selectany) const char* const DWFXML::kzElement_CanonicalizationMethod         = "CanonicalizationMethod";
_declspec(selectany) const char* const DWFXML::kzElement_DigestMethod                   = "DigestMethod";
_declspec(selectany) const char* const DWFXML::kzElement_DigestValue                    = "DigestValue";
_declspec(selectany) const char* const DWFXML::kzElement_DSAKeyValue                    = "DSAKeyValue";
_declspec(selectany) const char* const DWFXML::kzElement_DSAKeyValueP                   = "P";
_declspec(selectany) const char* const DWFXML::kzElement_DSAKeyValueQ                   = "Q";
_declspec(selectany) const char* const DWFXML::kzElement_DSAKeyValueG                   = "G";
_declspec(selectany) const char* const DWFXML::kzElement_DSAKeyValueY                   = "Y";
_declspec(selectany) const char* const DWFXML::kzElement_DSAKeyValueJ                   = "J";
_declspec(selectany) const char* const DWFXML::kzElement_DSAKeyValueSeed                = "Seed";
_declspec(selectany) const char* const DWFXML::kzElement_DSAKeyValuePgenCounter         = "PgenCounter";
_declspec(selectany) const char* const DWFXML::kzElement_KeyInfo                        = "KeyInfo";
_declspec(selectany) const char* const DWFXML::kzElement_KeyName                        = "KeyName";
_declspec(selectany) const char* const DWFXML::kzElement_KeyValue                       = "KeyValue";
_declspec(selectany) const char* const DWFXML::kzElement_Reference                      = "Reference";
_declspec(selectany) const char* const DWFXML::kzElement_RSAKeyValue                    = "RSAKeyValue";
_declspec(selectany) const char* const DWFXML::kzElement_RSAKeyValueModulus             = "Modulus";
_declspec(selectany) const char* const DWFXML::kzElement_RSAKeyValueExponent            = "Exponent";
_declspec(selectany) const char* const DWFXML::kzElement_Signature                      = "Signature";
_declspec(selectany) const char* const DWFXML::kzElement_SignatureMethod                = "SignatureMethod";
_declspec(selectany) const char* const DWFXML::kzElement_SignatureValue                 = "SignatureValue";
_declspec(selectany) const char* const DWFXML::kzElement_SignedInfo                     = "SignedInfo";
_declspec(selectany) const char* const DWFXML::kzElement_X509Certificate                = "X509Certificate";
_declspec(selectany) const char* const DWFXML::kzElement_X509CRL                        = "X509CRL";
_declspec(selectany) const char* const DWFXML::kzElement_X509Data                       = "X509Data";
_declspec(selectany) const char* const DWFXML::kzElement_X509IssuerSerial               = "X509IssuerSerial";
_declspec(selectany) const char* const DWFXML::kzElement_X509IssuerName                 = "X509IssuerName";
_declspec(selectany) const char* const DWFXML::kzElement_X509SerialNumber               = "X509SerialNumber";
_declspec(selectany) const char* const DWFXML::kzElement_X509SKI                        = "X509SKI";
_declspec(selectany) const char* const DWFXML::kzElement_X509SubjectName                = "X509SubjectName";
_declspec(selectany) const char* const DWFXML::kzElement_CoordinateSystem               = "CoordinateSystem";
_declspec(selectany) const char* const DWFXML::kzElement_CoordinateSystems              = "CoordinateSystems";
_declspec(selectany) const char* const DWFXML::kzElement_Relationship                   = "Relationship";
_declspec(selectany) const char* const DWFXML::kzElement_Relationships                  = "Relationships";


_declspec(selectany) const char* const DWFXML::kzAttribute_ID                           = "id";
_declspec(selectany) const char* const DWFXML::kzAttribute_URI                          = "uri";
_declspec(selectany) const char* const DWFXML::kzAttribute_Name                         = "name";
_declspec(selectany) const char* const DWFXML::kzAttribute_Type                         = "type";
_declspec(selectany) const char* const DWFXML::kzAttribute_HRef                         = "href";
_declspec(selectany) const char* const DWFXML::kzAttribute_Refs                         = "refs";
_declspec(selectany) const char* const DWFXML::kzAttribute_MIME                         = "mime";
_declspec(selectany) const char* const DWFXML::kzAttribute_Role                         = "role";
_declspec(selectany) const char* const DWFXML::kzAttribute_Clip                         = "clip";
_declspec(selectany) const char* const DWFXML::kzAttribute_Show                         = "show";
_declspec(selectany) const char* const DWFXML::kzAttribute_Size                         = "size";
_declspec(selectany) const char* const DWFXML::kzAttribute_Node                         = "node";
_declspec(selectany) const char* const DWFXML::kzAttribute_Nodes                        = "nodes";
_declspec(selectany) const char* const DWFXML::kzAttribute_Label                        = "label";
_declspec(selectany) const char* const DWFXML::kzAttribute_Value                        = "value";
_declspec(selectany) const char* const DWFXML::kzAttribute_Units                        = "units";
_declspec(selectany) const char* const DWFXML::kzAttribute_Title                        = "title";
_declspec(selectany) const char* const DWFXML::kzAttribute_Color                        = "color";
_declspec(selectany) const char* const DWFXML::kzAttribute_SetID                        = "setId";
_declspec(selectany) const char* const DWFXML::kzAttribute_Width                        = "width";
_declspec(selectany) const char* const DWFXML::kzAttribute_Height                       = "height";
_declspec(selectany) const char* const DWFXML::kzAttribute_Hidden                       = "hidden";
_declspec(selectany) const char* const DWFXML::kzAttribute_ZOrder                       = "zOrder";
_declspec(selectany) const char* const DWFXML::kzAttribute_Closed                       = "closed";
_declspec(selectany) const char* const DWFXML::kzAttribute_Object                       = "object";
_declspec(selectany) const char* const DWFXML::kzAttribute_Author                       = "author";
_declspec(selectany) const char* const DWFXML::kzAttribute_Visible                      = "visible";
_declspec(selectany) const char* const DWFXML::kzAttribute_Extents                      = "extents";
_declspec(selectany) const char* const DWFXML::kzAttribute_Version                      = "version";
_declspec(selectany) const char* const DWFXML::kzAttribute_Request                      = "request";
_declspec(selectany) const char* const DWFXML::kzAttribute_Scanned                      = "scanned";
_declspec(selectany) const char* const DWFXML::kzAttribute_SchemaID                     = "schemaId";
_declspec(selectany) const char* const DWFXML::kzAttribute_Children                     = "children";
_declspec(selectany) const char* const DWFXML::kzAttribute_Provider                     = "provider";
_declspec(selectany) const char* const DWFXML::kzAttribute_ObjectID                     = "objectId";
_declspec(selectany) const char* const DWFXML::kzAttribute_Category                     = "category";
_declspec(selectany) const char* const DWFXML::kzAttribute_ContentID                    = "contentId";
_declspec(selectany) const char* const DWFXML::kzAttribute_Transform                    = "transform";
_declspec(selectany) const char* const DWFXML::kzAttribute_PlotOrder                    = "plotOrder";
_declspec(selectany) const char* const DWFXML::kzAttribute_Privilege                    = "privilege";
_declspec(selectany) const char* const DWFXML::kzAttribute_ContentIDs                   = "contentIds";
_declspec(selectany) const char* const DWFXML::kzAttribute_InternalID                   = "internalId";
_declspec(selectany) const char* const DWFXML::kzAttribute_ColorDepth                   = "colorDepth";
_declspec(selectany) const char* const DWFXML::kzAttribute_Transparent                  = "transparent";
_declspec(selectany) const char* const DWFXML::kzAttribute_Exclusivity                  = "exclusivity";
_declspec(selectany) const char* const DWFXML::kzAttribute_Description                  = "description";
_declspec(selectany) const char* const DWFXML::kzAttribute_Orientation                  = "orientation";
_declspec(selectany) const char* const DWFXML::kzAttribute_LogfontName                  = "logfontName";
_declspec(selectany) const char* const DWFXML::kzAttribute_ResourceURI                  = "resourceURI";
_declspec(selectany) const char* const DWFXML::kzAttribute_InvertColors                 = "invertColors";
_declspec(selectany) const char* const DWFXML::kzAttribute_CreationTime                 = "creationTime";
_declspec(selectany) const char* const DWFXML::kzAttribute_CanonicalName                = "canonicalName";
_declspec(selectany) const char* const DWFXML::kzAttribute_CharacterCode                = "characterCode";
_declspec(selectany) const char* const DWFXML::kzAttribute_SchemaLocation               = "schemaLocation";
_declspec(selectany) const char* const DWFXML::kzAttribute_DefaultIconURI               = "defaultIconURI";
_declspec(selectany) const char* const DWFXML::kzAttribute_ActiveIconURI                = "activeIconURI";
_declspec(selectany) const char* const DWFXML::kzAttribute_ParentObjectID               = "parentObjectId";
_declspec(selectany) const char* const DWFXML::kzAttribute_OriginalExtents              = "originalExtents";
_declspec(selectany) const char* const DWFXML::kzAttribute_ExpandedIconURI              = "expandedIconURI";
_declspec(selectany) const char* const DWFXML::kzAttribute_ModificationTime             = "modificationTime";
_declspec(selectany) const char* const DWFXML::kzAttribute_PrimaryContentID             = "primaryContentId";
_declspec(selectany) const char* const DWFXML::kzAttribute_ScannedResolution            = "scannedResolution";
_declspec(selectany) const char* const DWFXML::kzAttribute_GeometricVariation           = "geometricVariation";
_declspec(selectany) const char* const DWFXML::kzAttribute_EffectiveResolution          = "effectiveResolution";
_declspec(selectany) const char* const DWFXML::kzAttribute_ClassRefs                    = "classRefs";
_declspec(selectany) const char* const DWFXML::kzAttribute_EntityRef                    = "entityRef";
_declspec(selectany) const char* const DWFXML::kzAttribute_EntityRefs                   = "entityRefs";
_declspec(selectany) const char* const DWFXML::kzAttribute_FeatureRefs                  = "featureRefs";
_declspec(selectany) const char* const DWFXML::kzAttribute_RenderableRef                = "renderableRef";
_declspec(selectany) const char* const DWFXML::kzAttribute_ContentElementRefs           = "contentElementRefs";
_declspec(selectany) const char* const DWFXML::kzAttribute_PositionX                    = "positionX";
_declspec(selectany) const char* const DWFXML::kzAttribute_PositionY                    = "positionY";
_declspec(selectany) const char* const DWFXML::kzAttribute_PositionZ                    = "positionZ";
_declspec(selectany) const char* const DWFXML::kzAttribute_TargetX                      = "targetX";
_declspec(selectany) const char* const DWFXML::kzAttribute_TargetY                      = "targetY";
_declspec(selectany) const char* const DWFXML::kzAttribute_TargetZ                      = "targetZ";
_declspec(selectany) const char* const DWFXML::kzAttribute_UpVectorX                    = "upVectorX";
_declspec(selectany) const char* const DWFXML::kzAttribute_UpVectorY                    = "upVectorY";
_declspec(selectany) const char* const DWFXML::kzAttribute_UpVectorZ                    = "upVectorZ";
_declspec(selectany) const char* const DWFXML::kzAttribute_FieldWidth                   = "fieldWidth";
_declspec(selectany) const char* const DWFXML::kzAttribute_FieldHeight                  = "fieldHeight";
_declspec(selectany) const char* const DWFXML::kzAttribute_ProjectionType               = "projectionType";
_declspec(selectany) const char* const DWFXML::kzAttribute_ResetFlags                   = "resetFlags";
_declspec(selectany) const char* const DWFXML::kzAttribute_Mask                         = "mask";
_declspec(selectany) const char* const DWFXML::kzAttribute_Red                          = "red";
_declspec(selectany) const char* const DWFXML::kzAttribute_Green                        = "green";
_declspec(selectany) const char* const DWFXML::kzAttribute_Blue                         = "blue";
_declspec(selectany) const char* const DWFXML::kzAttribute_Gloss                        = "gloss";
_declspec(selectany) const char* const DWFXML::kzAttribute_Index                        = "index";
_declspec(selectany) const char* const DWFXML::kzAttribute_Count                        = "count";
_declspec(selectany) const char* const DWFXML::kzAttribute_A                            = "a";
_declspec(selectany) const char* const DWFXML::kzAttribute_B                            = "b";
_declspec(selectany) const char* const DWFXML::kzAttribute_C                            = "c";
_declspec(selectany) const char* const DWFXML::kzAttribute_D                            = "d";
_declspec(selectany) const char* const DWFXML::kzAttribute_Elements                     = "elements";
_declspec(selectany) const char* const DWFXML::kzAttribute_State                        = "state";
_declspec(selectany) const char* const DWFXML::kzAttribute_Mode                         = "mode";
_declspec(selectany) const char* const DWFXML::kzAttribute_PropertyReferenceID          = "propertyReferenceID";
_declspec(selectany) const char* const DWFXML::kzAttribute_SmoothTransition             = "smoothTransition";
_declspec(selectany) const char* const DWFXML::kzAttribute_UseDefaultPropertyIfMissing  = "useDefaultPropertyIfMissing";
_declspec(selectany) const char* const DWFXML::kzAttribute_Algorithm                    = "Algorithm";
_declspec(selectany) const char* const DWFXML::kzAttribute_OriginX                      = "originX";
_declspec(selectany) const char* const DWFXML::kzAttribute_OriginY                      = "originY";
_declspec(selectany) const char* const DWFXML::kzAttribute_OriginZ                      = "originZ";
_declspec(selectany) const char* const DWFXML::kzAttribute_Rotation                     = "rotation";
_declspec(selectany) const char* const DWFXML::kzAttribute_TransitionType               = "TransitionType";
_declspec(selectany) const char* const DWFXML::kzAttribute_Duration                     = "Duration";
_declspec(selectany) const char* const DWFXML::kzAttribute_LeftRightAngle               = "LeftRightAngle";
_declspec(selectany) const char* const DWFXML::kzAttribute_UpDownAngle                  = "UpDownAngle";
_declspec(selectany) const char* const DWFXML::kzAttribute_CameraPosition               = "CameraPosition";
_declspec(selectany) const char* const DWFXML::kzAttribute_CinematicType                = "CinematicType";
_declspec(selectany) const char* const DWFXML::kzAttribute_InOutDistance                = "InOutDistance";
_declspec(selectany) const char* const DWFXML::kzAttribute_LeftRightDistance            = "LeftRightDistance";
_declspec(selectany) const char* const DWFXML::kzAttribute_UpDownDistance               = "UpDownDistance";
_declspec(selectany) const char* const DWFXML::kzAttribute_LockCamera                   = "LockCamera";

_declspec(selectany) const char* const DWFXML::kzDWFProperty_SourceProductVendor        = "SourceProductVendor";
_declspec(selectany) const char* const DWFXML::kzDWFProperty_SourceProductName          = "SourceProductName";
_declspec(selectany) const char* const DWFXML::kzDWFProperty_SourceProductVersion       = "SourceProductVersion";
_declspec(selectany) const char* const DWFXML::kzDWFProperty_DWFProductVendor           = "DWFProductVendor";
_declspec(selectany) const char* const DWFXML::kzDWFProperty_DWFProductVersion          = "DWFProductVersion";
_declspec(selectany) const char* const DWFXML::kzDWFProperty_DWFToolkitVersion          = "DWFToolkitVersion";
_declspec(selectany) const char* const DWFXML::kzDWFProperty_DWFFormatVersion           = "DWFFormatVersion";
_declspec(selectany) const char* const DWFXML::kzDWFProperty_PasswordEncryptedDocument  = "_PasswordEncryptedDocument";

_declspec(selectany) const wchar_t* const DWFXML::kzRole_Graphics2d                 = L"2d streaming graphics";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Graphics3d                 = L"3d streaming graphics";   
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Descriptor                 = L"descriptor";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Thumbnail                  = L"thumbnail";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Graphics2dOverlay          = L"2d vector overlay";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Graphics2dMarkup           = L"2d vector markup";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_RmlMarkup                  = L"RML markup";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Preview                    = L"preview";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_OverlayPreview             = L"overlay preview";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_MarkupPreview              = L"markup preview";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_RasterOverlay              = L"raster overlay";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_RasterMarkup               = L"raster markup";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Font                       = L"font";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Metadata                   = L"metadata";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_PackageSignature           = L"package signature";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_SectionSignature           = L"section signature";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_FilesetSignature           = L"fileset signature";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Signature                  = L"signature";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_ObjectDefinition           = L"object definition";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_MarkupObjectDefinition     = L"markup object definition";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Texture                    = L"texture";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_ContentPresentation        = L"content presentation";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_MarkupContentPresentation  = L"markup content presentation";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Animation                  = L"animation";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Note                       = L"note";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Graphics3dMarkup           = L"3D markup graphics";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_3dMarkup                   = L"3D markup";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_ContentDefinition          = L"content definition";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Icon                       = L"icon";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_ObservationMesh            = L"observation mesh";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_GeographicCoordinateSystem = L"geographic coordinate system";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Graphics2dExtension        = L"2d graphics extension";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_Graphics2dDictionary       = L"2d graphics dictionary";
_declspec(selectany) const wchar_t* const DWFXML::kzRole_RasterReference            = L"raster reference";

//DNT_End


#endif
#endif

#endif
