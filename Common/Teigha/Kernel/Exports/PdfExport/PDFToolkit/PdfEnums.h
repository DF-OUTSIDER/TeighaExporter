/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////




#ifndef _PDFENUMS_INCLUDED_
#define _PDFENUMS_INCLUDED_ /*!DOM*// 

namespace TD_PDF
{

enum PDFLineCap
{
  kLineCapNotSet       = -1,
  kButtCap             =  0,
  kRoundCap            =  1,
  kProjectingSquareCap =  2
};

enum PDFLineJoin
{
  kLineJoinNotSet = -1,
  kMiterJoin      =  0,
  kRoundJoin      =  1,
  kBevelJoin      =  2
};

enum LightingMode {
  kArtwork = 1,
  kNone,
  kWhite,
  kDay,
  kBright,
  kPrimaryColor,
  kNight,
  kBlue,
  kRed,
  kCube,
  kCADOptimized,
  kHeadlamp
};

enum RenderingMode {
  kSolid = 1,
  kSolidWireframe,
  kSolidOutline,
  kBoundingBox,
  kTransparent,
  kTransparentWireframe,
  kTransparentBoundingBox,
  kTransparentBoundingBoxOutline,
  kIllustration,
  kShadedIllustration,
  kWireframe,
  kShadedWireframe,
  kHiddenWireframe,
  kVertices,
  kShadedVertices
};

enum PDFTypeId
{
  kBody,
  kBaseString,
  kASCIIHexDecodeFilter,
  kArray,
  kFontDescriptorFlags,
  kFontDescriptor4CIDFont,
  kFontDescriptor,
  kFont,
  kExtGState,
  kDictionary,
  kProjectionDictionary,
  kCrossSectionDictionary,
  k3dNodeDictionary,
  k3dBackgroundDictionary,
  k3DRenderModeDictionary,
  k3DLightingSchemeDictionary,
  kDecodeParametersDictionary,
  kContentStream,
  kColorKeyMaskArray,
  kCMapDictionary,
  kCIDSystemInfo,
  kCIDFontWidthsArray,
  kCIDFont2,
  kCIDFont0,
  kCIDFont,
  kCID2GIDStream,
  kCatalogDictionary,
  kBoolean,
  kInteger,
  kIndexedRGBColorSpace,
  kFontFileStream,
  kIndexedColorSpace,
  kImageDictionary,
  kImage,
  kFontWidthsArray,
  kFontSubDictionary,
  kXObjectDictionary,
  kXObject,
  kType0Font,
  kTrueTypeFont,
  kTrailerDictionary,
  kTextString,
  kSubDictionary,
  kStreamFilter,
  kStreamDictionary,
  kStream,
  k3dStreamDictionary,
  kRGBStream,
  kResourceDictionary,
  kRectangle,
  kPageNodeDictionary,
  kPageDictionary,
  kBaseObject,
  kObject,
  kNumber,
  kName,
  kMemoryStream,
  kTmpStream,
  kLiteralString,
  kIStream,
  kNullObject,
  kType3Font,
  kFontMatrix,
  kDifferencesArray,
  kEncodingDictionary,
  kContentStream4Type3,
  kCharProcDictionary,
  kDate,
  kDocumentInformation,
  kPdfEmbeddedFontStreamDictionary,
  kFlateDecodeFilter,
  kFlateDecodeParameters,
  kCMap,
  kToUnicodeCMap,
  kOCGroup,
  kOCGroupDictionary,
  kOCPropertiesDictionary,
  kOCConfigurationDictionary,
  kUsageDictionary,
  kPropertiesSubDictionary,
  kShadingPattern,
  kShadingDictionary,
  kShadingT4StreamDictionary,
  kPatternSubDictionary,
  kShadingSubDictionary,
  kColorSpaceSubDictionary,
  kExtGStateSubDictionary,
  kXObjectSubDictionary,
  kShadingT4Stream,
  kTempFileStream,
  kAnnotationDictionary,
  k3dAnnotationDictionary,
  kLinkAnnotationDictionary,
  kMarkupAnnotationDictionary,
  kSquareAnnotationDictionary,
  kActionDictionary,
  kURIActionDictionary,
  k3dStream,
  k3dActivationDictionary,
  k3dViewDictionary,
  kCCITTFaxDecodeFilter,
  kCCITTFaxDecodeParameters,
  kDCTDecodeFilter,
  kDCTDecodeParameters,
  kDeviceGrayColorSpace,
  kOutputIntentsDictionary,
  kMetadataStream,
  kMetadataStreamDictionary,
  kICCBasedStream,
  kICCBasedStreamDictionary,
  kXObjectForm,
  kXObjectFormDictionary,
  kViewportDictionary,
  kMeasureDictionary,
  kNumberFormatDictionary,
  kInteractiveFormDictionary,
  kWidgetAnnotationDictionary,
  kAppearanceDictionary,
  kAppearanceCharacteristicsDictionary,
  kFieldDictionary,
  kFieldBtnDictionary,
  kFieldTextDictionary,
  kFieldCheckBoxDictionary,
  kFieldRadioBtnDictionary,
  kFieldSignDictionary,
  kFieldFlags,
  kType1Font,
  kAnnotationFlags,
  kJavaScriptActionDictionary,
  kGoTo3DViewActionDictionary,
  kNameTreeNodeNamesElement,
  kNameTreeNodeDictionary,
  kNamesDictionary,
  kTextStream,
  kBorderStyleDictionary,
  kFieldListBoxDictionary,
  kFieldChoiceDictionary,
  kFieldComboBoxDictionary,
  kStateAppearanceSubDictionary,
  kTriggerEventsDictionary,
  k3dAnimationStyleDictionary
};

};

#endif //_PDFENUMS_INCLUDED_

