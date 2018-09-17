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

#ifndef _EX_DGNDUMPER_H_
#define _EX_DGNDUMPER_H_

#include "OdaCommon.h"
#include "RxObject.h"

#include "Gi/GiDrawable.h"
#include "RxVariant.h"
#include "DgDatabase.h"
#include "DgLine.h"
#include "DgLineString.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgShape.h"
#include "DgCurve.h"
#include "DgEllipse.h"
#include "DgArc.h"
#include "DgCone.h"
#include "DgComplexString.h"
#include "DgComplexShape.h"
#include "DgPointString.h"
#include "DgDimension.h"
#include "DgMultiline.h"
#include "DgBSplineCurve.h"
#include "Dg3DObject.h"
#include "DgRasterAttach.h"
#include "DgRaster.h"
#include "DgTagElement.h"
#include "DgCellHeader.h"
#include "DgBSplineSurface.h"
#include "DgLevelTableRecord.h"
#include "DgFontTableRecord.h"
#include "DgTextStyleTableRecord.h"
#include "DgDimStyleTableRecord.h"
#include "DgMultilineStyleTableRecord.h"
#include "DgLineStyleTableRecord.h"
#include "DgLineStyleDefTableRecord.h"
#include "DgRegAppTableRecord.h"
#include "DgView.h"
#include "DgSharedCellDefinition.h"
#include "DgSharedCellReference.h"
#include "DgTagSetDefinition.h"
#include "DgReferenceAttach.h"
#include "Ge/GeExtents3d.h"
#include "DgAttributeLinkage.h"
#include "DgRasterFrame.h"
#include "DgOle2Frame.h"
#include "DgNamedGroup.h"
#include "DgRasterAttach.h"
#include "DgElementTemplate.h"
#include "DgEnvironmentMap.h"
#include "Gi/GiMaterial.h"
#include "DgMaterialTableRecord.h"
#include "Ge/GeScale2d.h"
#include "Ge/GeScale3d.h"
#include "DgXAttribute.h"
#include "DgDisplayStyle.h"
#include "DgTerrain.h"
#include "DgItemTypeLibrary.h"
#include "DgPrintStyleTableRecord.h"

/** \details
  Library: Source code provided.
  <group Dg_Extension_Classes>
*/
class OdExDgnDumper
{
public:

  virtual void dumpFieldName( const OdString& fieldName ) = 0;
  virtual void dumpFieldValue( const OdString& fieldName ) = 0;

  // output for different types
  inline void writeFieldValue( const OdString& name, const OdString& value );
  inline void writeFieldValue( const OdString& name, OdUInt16 value );
  inline void writeFieldValueHex( const OdString& name, OdUInt16 value );
  inline void writeFieldValue( const OdString& name, OdUInt32 value );
  inline void writeFieldValueHex( const OdString& name, OdUInt32 value );
  inline void writeFieldValue( const OdString& name, double value );
  inline void writeFieldValue( const OdString& name, OdGePoint2d const& value );
  inline void writeFieldValue( const OdString& name, OdGePoint3d const& value );
  inline void writeFieldValue( const OdString& name, OdGeVector2d const& value );
  inline void writeFieldValue( const OdString& name, OdGeVector3d const& value );
  inline void writeFieldValue( const OdString& name, OdCmEntityColor const& value );
  inline void writeFieldValue( const OdString& name, OdDgLineStyleInfo const& value );
  inline void writeFieldValue( const OdString& name, OdUInt64 value );
  inline void writeFieldValueHex( const OdString& name, OdUInt64 value );
  inline void writeFieldValue( const OdString& name, OdUInt8 value );
  inline void writeFieldValueHex( const OdString& name, OdUInt8 value );
  inline void writeFieldValue( const OdString& name, OdDgLightColor const& value );
  inline void writeFieldValue( const OdString& name, bool value );
  inline void writeFieldValue( const OdString& name, OdAngleCoordinate const& value );
  inline void writeFieldValue( const OdString& name, OdInt16 value );
#if defined(ANDROID)
  inline void writeFieldValue( const OdString& name, OdChar value );
#endif
  inline void writeFieldValue( const OdString& name, OdInt32 value );
#if !(defined(ODA_UNIXOS) && (OD_SIZEOF_LONG == 8))
  inline void writeFieldValue( const OdString& name, int value );
#endif
  inline void writeFieldValue( const OdString& name, OdInt64 value );
  inline void writeFieldValue( const OdString& name, OdDgModel::WorkingUnit value );
  inline void writeFieldValue( const OdString& name, OdDg::TextJustification value );
  inline void writeFieldValue( const OdString& name, OdGeQuaternion const& value );
  //void writeFieldValue( const OdString& name, TextAttributes const& value );
  inline void writeFieldValue( const OdString& name, OdDgGraphicsElement::Class value );
  inline void writeFieldValue( const OdString& name, OdGeMatrix2d const& value );
  //void writeFieldValue( const OdString& name, OdDgDimension::ToolType value );
  inline void writeFieldValue( const OdString& name, OdDgDimPoint& value );
  inline void writeFieldValue( const OdString& name, OdDgDimTextInfo const& value );
  inline void writeFieldValue( const OdString& name, OdDgDimTextFormat const& value );
  inline void writeFieldValue( const OdString& name, OdDgDimTextFormat::Accuracy value );
  inline void writeFieldValue( const OdString& name, OdDgDimGeometry const& value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionPtr& value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionSymbolPtr& value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionTerminatorSymbologyPtr& value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionTolerancePtr& value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionTerminatorsPtr& value );
  inline void writeFieldValue( const OdString& name, OdDimOptionPrefixSuffixPtr& value );
  inline void writeFieldValue( const OdString& name, OdDgModel::UnitSystem value );
  inline void writeFieldValue( const OdString& name, OdDgModel::UnitBase value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionUnitsPtr& value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionAltFormatPtr& value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionOffsetPtr& value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionProxyCellPtr& value );
  inline void writeFieldValue( const OdString& name, OdDgMultilineSymbology const& value );
  inline void writeFieldValue( const OdString& name, OdDgMultilinePoint const& value );
  inline void writeFieldValue( const OdString& name, OdDgMultilineBreak const& value );
  inline void writeFieldValue( const OdString& name, OdDgMultilineProfile const& value );
  inline void writeFieldValue( const OdString& name, OdDgSurface::Type value );
  inline void writeFieldValue( const OdString& name, OdDgRaster::RasterFormat value );
  inline void writeFieldValue( const OdString& name, OdDgTagDefinition::Type value );
  inline void writeFieldValue( const OdString& name, OdDg::TextDirection value );
  inline void writeFieldValue( const OdString& name, OdGeMatrix3d const& value );
  inline void writeFieldValue( const OdString& name, OdGsDCRect const& value );
  inline void writeFieldValue( const OdString& name, OdDgElementId const& value );
  inline void writeFieldValue( const OdString& name, const OdVariant& value );
  inline void writeFieldValue( const OdString& name, OdGeExtents2d const& value );
  inline void writeFieldValue( const OdString& name, OdGeExtents3d const& value );
  inline void writeFieldValue( const OdString& name, OdDgTextExtendedProperties::LineSpacingType value );
  inline void writeFieldValue( const OdString& name, OdDg::LineStyleType value );
  inline void writeFieldValue( const OdString& name, OdDg::LineStyleUnitsType value );
  inline void writeFieldValue( const OdString& name, OdDgLineStyleResource::OdLsResourceType value );
  inline void writeFieldValue( const OdString& name, OdDgLineCodeResourceStrokeData::OdLsStrokeWidthMode value );
  inline void writeFieldValue( const OdString& name, OdDgLineCodeResourceStrokeData::OdLsStrokeCapsType value );
  inline void writeFieldValue( const OdString& name, OdDgLinePointResourceSymInfo::OdLsSymbolPosOnStroke value );
  inline void writeFieldValue( const OdString& name, OdDgDimLabelLine::LabelLineDimensionMode value );
  inline void writeFieldValue( const OdString& name, OdDgDimTextInfo::FitOptions value );
  inline void writeFieldValue( const OdString& name, OdDgDimension::PlacementAlignment value );
  inline void writeFieldValue( const OdString& name, OdDgDimTextInfo::TextLocation value );
  inline void writeFieldValue( const OdString& name, OdDgDimTool::TerminatorType iType );
  inline void writeFieldValue( const OdString& name, OdDgDimTool::TextType iType );
  inline void writeFieldValue( const OdString& name, OdDgDimTool::CustomSymbol iSymbol );
  inline void writeFieldValue( const OdString& name, OdDgDimTool::Leader iLeader );
  inline void writeFieldValue( const OdString& name, OdDgDimensionPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimSizeArrowPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimSizeStrokePtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimSingleLocationPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimStackedLocationPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimCustomLinearPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimAngleSizePtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimArcSizePtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimAngleLinesPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimAngleLocationPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimArcLocationPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimAngleAxisXPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimAngleAxisYPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimRadiusPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimRadiusExtendedPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimDiameterPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimDiameterExtendedPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimDiameterParallelPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimDiameterPerpendicularPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimOrdinatePtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimCenterPtr pElement );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionOffset::ChainType value );
  inline void writeFieldValue( const OdString& name, OdDgDimOptionOffset::LeaderAlignment value );
  inline void writeFieldValue( const OdString& name, OdDgDimTextInfo::TextAlignment value );
  inline void writeFieldValue( const OdString& name, OdDgDimTextFormat::LabelDisplayMode value );
  inline void writeFieldValue( const OdString& name, OdDgLevelFilterTable::OdDgFilterMemberType value );
  inline void writeFieldValue( const OdString& name, OdDgGradientFill::OdDgGradientType value );
  inline void writeFieldValue( const OdString& name, OdDgLineStyleModificationLinkage::OdDgLsModWidthMode value );
  inline void writeFieldValue( const OdString& name, OdDgLineStyleModificationLinkage::OdDgLsModShiftMode value );
  inline void writeFieldValue( const OdString& name, OdDgLineStyleModificationLinkage::OdDgLsModCornerMode value );
  inline void writeFieldValue( const OdString& name, OdDgDependencyLinkage::RootDataType value );
  inline void writeFieldValue( const OdString& name, OdDgAssocPointRoot::OdDgAssocPointRootType value );
  inline void writeFieldValue( const OdString& name, OdDgArcAssociation::OdDgArcAssociationKeyPoint value );
  inline void writeFieldValue( const OdString& name, OdDgOriginAssociation::OdDgOriginAssocOption value );
  inline void writeFieldValue( const OdString& name, OdDgLineStyleModificationLinkage::OdDgLsMultilineDataType value );
  inline void writeFieldValue( const OdString& name, OdDgRasterFrame::OdDgGeoPriority value );
  inline void writeFieldValue( const OdString& name, OdDgRasterFrame::OdDgRasterDrawingPlane value );
  inline void writeFieldValue( const OdString& name, OdDgRasterFrame::OdDgRasterColorMode value );
  inline void writeFieldValue( const OdString& name, OdDgOle2Frame::OdDgDrawAspect value );
  inline void writeFieldValue( const OdString& name, OdDgOle2Frame::OdDgOleObjectType value );
  inline void writeFieldValue( const OdString& name, OdDgOle2Frame::OdDgOleViewRotationMode value );
  inline void writeFieldValue( const OdString& name, OdDgNamedGroupItem::OdDgNamedGroupPropagateType value );
  inline void writeFieldValue( const OdString& name, OdDgRasterAttachmentHeader::OdDgRasterAttachmentPlane value );
  inline void writeFieldValue( const OdString& name, OdDgTemplateValueVariant value );
  inline void writeFieldValue( const OdString& name, OdDgModel::OdDgGridOrientationType value );
  inline void writeFieldValue( const OdString& name, OdDgModel::AcsType value );
  inline void writeFieldValue( const OdString& name,  OdDg::OdDgMaterialType value );
  inline void writeFieldValue( const OdString& name,  OdDgLinkNode::OdDgLinkNodeType value );
  inline void writeFieldValue( const OdString& name,  OdDgLinkNodePtr value );
  inline void writeFieldValue( const OdString& name,  OdDgFileLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgModelLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgRegionLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgPdfRegionLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgExcelSheetLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgWordHeadingLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgWordRegionLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgURLLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgUnknownLinkNode* value );
  inline void writeFieldValue( const OdString& name,  OdDgModelLinkNode::OdDgModelLinkNodeType value );
  inline void writeFieldValue( const OdString& name,  OdDgRegionLinkNode::OdDgRegionLinkNodeType value );
  inline void writeFieldValue( const OdString& name,  OdDgPdfRegionInfoItem::OdDgPdfRegionType value );
  inline void writeFieldValue( const OdString& name,  OdDgWordHeadingInfoItem::OdDgWordHeadingItemType value );
  inline void writeFieldValue( const OdString& name,  OdDgTextWhiteSpaceLinkage::OdDgSpecialSymbolType value );
  inline void writeFieldValue( const OdString& name,  OdDgEnvMapBackground::OdDgEnvMapBackgroungMode value );
  inline void writeFieldValue( const OdString& name,  OdDgEnvMapFog::OdDgEnvMapFogMode value );
  inline void writeFieldValue( const OdString& name,  OdDgEnvMapImage::OdDgEnvMapProjectionType value );
  inline void writeFieldValue( const OdString& name,  OdDgEnvMapImage::OdDgEnvMapFilterType value );
  inline void writeFieldValue( const OdString& name,  OdDgEnvMapGradient::OdDgEnvMapGradientMode value );
  inline void writeFieldValue( const OdString& name,  OdDgEnvMapLayer::OdDgEnvMapLayerType value );
  inline void writeFieldValue( const OdString& name,  OdDgView::OdDgDisplayMode value );
  inline void writeFieldValue( const OdString& name,  const OdGiMaterialColor& value);
  inline void writeFieldValue( const OdString& name,  const OdDgMaterialParam& value);
  inline void writeFieldValue( const OdString& name,  const OdGeScale2d& value);
  inline void writeFieldValue( const OdString& name,  const OdGeScale3d& value);
  inline void writeFieldValue( const OdString& name,  const OdDoubleColor& value);
  inline void writeFieldValue( const OdString& name,  OdDgMaterialPattern::OdDgMaterialPatternType value);
  inline void writeFieldValue( const OdString& name,  OdDgMaterialMap::MappingType value);
  inline void writeFieldValue( const OdString& name,  OdDgMaterialMap::ScaleMode value);
  inline void writeFieldValue( const OdString& name,  OdDgMaterialLayerPattern::OdDgMaterialLayerPatternType value);
  inline void writeFieldValue( const OdString& name,  OdDgModel::Type value);
  inline void writeFieldValue( const OdString& name,  OdDgModel::CellType value);
  inline void writeFieldValue( const OdString& name,  OdDgDimTextFormat::AngleValueDisplayMode value);
  inline void writeFieldValue( const OdString& name,  OdDg::OdDgDecimalAccuracy value);
  inline void writeFieldValue( const OdString& name,  OdDg::OdDgAngleReadoutDirectionMode value);
  inline void writeFieldValue( const OdString& name,  const OdDgXMLNode& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleTableRecord::OdDgDisplayStyleDisplayHeader& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleTableRecord::OdDgDisplayMode& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleTableRecord::OdDgEnvironmentTypeDisplayed& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendCoordinateAxis& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgSlopeDisplayMode& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendColorScheme& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendEdgeOverride& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendValuesCalcMethod& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgSteppedDisplayMethod& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendItem& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleTypeFilterXAttribute::OdDgFilterApplyToType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgDisplayStyleTypeFilterXAttribute::OdDgFilterElementType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainFeature::OdDgTerrainFeatureType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainControlElement::OdDgTerrainEdgeMethod& value);
  inline void writeFieldValue( const OdString& name,  const OdDgExtendedElementTypeXAttribute::OdDgExtendedElementType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainGraphicsSettingsXAttribute::OdDgTerrainGraphicsType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainSourceFeatureSettings::OdDgSourceFeatureType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainVertexFeatureSettings::OdDgTerrainVertexFeatureType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainOptions::OdDgTerrainPointDisplayType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainContourFeatureSettings::OdDgTerrainContourFeatureType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainOptions::OdDgTerrainContourSmoothingType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTerrainOptions::OdDgTerrainOptionsType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgItemType::OdDgItemTypeCategory& value);
  inline void writeFieldValue( const OdString& name,  const OdDgItemTypePropertyValue& value);
  inline void writeFieldValue( const OdString& name,  const OdDgItemTypePropertyUnits& value);
  inline void writeFieldValue( const OdString& name,  const OdDgItemTypePropertyInstanceValue& value);
  inline void writeFieldValue( const OdString& name,  const OdTimeStamp& value);
  inline void writeFieldValue( const OdString& name,  const OdDgBECXMLVariant::OdDgBECXMLVariantColor& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdDgBECXMLVariant::OdDgBECXMLVariantColor>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<bool>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdInt8>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdInt16>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdInt32>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdInt64>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<double>& value);
  inline void writeFieldValue( const OdString& name,  const OdBoolArray& value);
  inline void writeFieldValue( const OdString& name,  const OdInt8Array& value);
  inline void writeFieldValue( const OdString& name,  const OdInt16Array& value);
  inline void writeFieldValue( const OdString& name,  const OdInt32Array& value);
  inline void writeFieldValue( const OdString& name,  const OdInt64Array& value);
  inline void writeFieldValue( const OdString& name,  const OdDoubleArray& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdString>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdAnsiString>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdGePoint3d>& value);
  inline void writeFieldValue( const OdString& name,  const OdArray<OdTimeStamp>& value);
  inline void writeFieldValue( const OdString& name,  const OdDgBinXMLNode* pValue);
  inline void writeFieldValue( const OdString& name,  const OdDgBinXMLValue* pValue);
  inline void writeFieldValue( const OdString& name,  const OdDgGeometryCacheActionPtr& pValue);
  inline void writeFieldValue( const OdString& name,  const OdDgEdgeRunTableElementSchema::OdDgEdgeRunSide& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTableCellElement::OdDgTableCellElementTextAlignment& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTableCellElement::OdDgTableCellElementTextOrientation& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTableElement::OdDgTableElementBreakType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTableElement::OdDgTableElementSubTablePosition& value);
  inline void writeFieldValue( const OdString& name,  const OdDgTableSchemaDescriptionItem::OdDgTableSchemaDescriptionItemType& value);
  inline void writeFieldValue( const OdString& name,  const OdDgSchemaItemVariant& value);
  inline void writeFieldColorValue( const OdString& name, OdUInt32 value );
  inline void writeFieldLineStyleValue( const OdString& name, OdUInt32 value );
  inline void writeFieldLineWeightValue( const OdString& name, OdUInt32 value );
  inline void writeFieldValue( const OdString& name, OdDgInfiniteLineLinkage::InfiniteLineType type );
  inline void writeFieldValue( const OdString& name, OdDgVariable::OdDgVariableType type );
  inline void writeFieldValue( const OdString& name, OdDgVariable::OdDgVariableScope type );
  inline void writeFieldValue( const OdString& name, OdDgVariableValueVariant type );
  inline void writeFieldValue( const OdString& name, OdDgItemTypeBasedVariable::OdDgItemTypeBasedVariableUnits type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePrintArea type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePaperOrientation type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutUnits type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutSizeScaleMode type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutOriginMode type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutMirrorMode type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutFenceAlign type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleColorMode type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePropertyPubMethod type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePrintDefCreationMode type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleFenceCreationData::OdDgPrintStyleFenceCreationMode type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePdfPaperSizeMode type );
  inline void writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleRasterRotation type );
  inline void writeFieldValue( const OdString& name, OdDgConstraintsDataXAttr::OdDgConstraintsXAttributeType type );
  inline void writeFieldValue( const OdString& name, OdDg2dConstraintRepresentation::OdDgConstraintType type );
  inline void writeFieldValue( const OdString& name, const OdDgConstraintRepPoint& ptConstraint );
  inline void writeFieldValue( const OdString& name, const OdDgDimensionalConstraintRepresentation::OdDgDimensionalConstraintRepAngleQuadrant& uQuad );
  inline void writeFieldValue( const OdString& name, const OdDgDimensionalConstraintRepresentation::OdDgDimensionalConstraintYOffsetAlignment& uAlign );
  inline void writeFieldValue( const OdString& name, const OdArray<OdString>& arrData, bool bHideIfEmpty );
  inline void writeFieldValue( const OdString& name, const OdArray<OdUInt64>& arrData, bool bHideIfEmpty );
  inline void writeFieldValue( const OdString& name, const OdArray<OdUInt32>& arrData, bool bHideIfEmpty );
  inline void writeFieldValue( const OdString& name, const OdArray<OdInt32>& arrData, bool bHideIfEmpty );
  inline void writeFieldValue( const OdString& name, const OdArray<double>& arrData, bool bHideIfEmpty );
  inline void writeFieldValue( const OdString& name, const OdArray<bool>& arrData, bool bHideIfEmpty );
};

inline void OdExDgnDumper::writeFieldValue( const OdString& name, const OdString& value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%s", value.c_str() );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdUInt16 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%u", OdUInt32( value ) );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValueHex( const OdString& name, OdUInt16 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%04X", OdUInt32( value ) );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdUInt32 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%u", value );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValueHex( const OdString& name, OdUInt32 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%08X", value );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, double value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%g", value );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdGePoint2d const& value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%g; %g", value.x, value.y );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdGePoint3d const& value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%g; %g; %g", value.x, value.y, value.z );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdGeVector2d const& value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%g; %g", value.x, value.y );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdGeVector3d const& value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%g; %g; %g", value.x, value.y, value.z );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdCmEntityColor const& value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"R: %u G: %u B: %u",
    unsigned( value.red() ), unsigned( value.green() ), unsigned( value.blue() ) );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLineStyleInfo const& value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"Modifiers: %08X Scale: %g Shift: %g Start width: %g End width: %g",
    value.getModifiers(), value.getScale(), value.getShift(), value.getStartWidth(), value.getEndWidth() );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdUInt64 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%I64u", value );
  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValueHex( const OdString& name, OdUInt64 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%I64X", value );
  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdUInt8 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%u", unsigned( value ) );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValueHex( const OdString& name, OdUInt8 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%02X", unsigned( value ) );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLightColor const& value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"R: %g G: %g B: %g Intensity: %g",
    value.getRed(), value.getGreen(), value.getBlue(), value.getIntensityScale() );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, bool value )
{
  dumpFieldName( name );
  dumpFieldValue( value ? L"true" : L"false" );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdAngleCoordinate const& value )
{
  dumpFieldName( name );
  OdString val;
  OdString wsym("°");
  val.format( L"%d%s %u' %u\"",
    unsigned( value.getDegrees() ), wsym.c_str(), unsigned( value.getMinutes() ), unsigned( value.getSeconds() ) );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdInt16 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%i", int( value ) );
  dumpFieldValue( val );
}

#if defined(ANDROID)
inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdChar value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%c", value);
  dumpFieldValue( val );
}
#endif

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdInt32 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%li", int( value ) );
  dumpFieldValue( val );
}

#if !(defined(ODA_UNIXOS) && (OD_SIZEOF_LONG == 8))
inline void OdExDgnDumper::writeFieldValue( const OdString& name, int value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%li", value );
  dumpFieldValue( val );
}
#endif


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdInt64 value )
{
  dumpFieldName( name );
  OdString val;
  val.format( L"%iI64", int( value ) );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgModel::WorkingUnit value )
{
  dumpFieldName( name );

  OdString val;
  switch( value )
  {
  case OdDgModel::kWuUnitOfResolution : val.format( L"unit of resolution" ); break;
  case OdDgModel::kWuStorageUnit : val.format( L"storage unit" ); break;
  case OdDgModel::kWuWorldUnit : val.format( L"world unit" ); break;
  case OdDgModel::kWuMasterUnit : val.format( L"master unit" ); break;
  case OdDgModel::kWuSubUnit : val.format( L"sub unit" ); break;
  }
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDg::TextJustification value )
{
  dumpFieldName( name );

  OdString val;
  switch( value )
  {
  case OdDg::kLeftTop          : val.format( L"left top" ); break;
  case OdDg::kLeftCenter       : val.format( L"left center" ); break;
  case OdDg::kLeftBottom       : val.format( L"left bottom" ); break;
  case OdDg::kLeftMarginTop    : val.format( L"left margin top" ); break;
  case OdDg::kLeftMarginCenter : val.format( L"left margin center" ); break;
  case OdDg::kLeftMarginBottom : val.format( L"left margin bottom" ); break;
  case OdDg::kCenterTop        : val.format( L"center top" ); break;
  case OdDg::kCenterCenter     : val.format( L"center center" ); break;
  case OdDg::kCenterBottom     : val.format( L"center bottom" ); break;
  case OdDg::kRightMarginTop   : val.format( L"right margin top" ); break;
  case OdDg::kRightMarginCenter : val.format( L"right margin center" ); break;
  case OdDg::kRightMarginBottom : val.format( L"right margin bottom" ); break;
  case OdDg::kRightTop          : val.format( L"right top" ); break;
  case OdDg::kRightCenter       : val.format( L"right center" ); break;
  case OdDg::kRightBottom       : val.format( L"right bottom" ); break;
  }
  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimLabelLine::LabelLineDimensionMode value )
{
  dumpFieldName( name );
  OdString val;
  
  switch( value )
  {
    case OdDgDimLabelLine::kAngleLength :
      val = OD_T("Angle/Length"); break;
    case OdDgDimLabelLine::kLengthAbove :
      val = OD_T("Length above"); break;
    case OdDgDimLabelLine::kAngleAbove :
      val = OD_T("Angle above"); break;
    case OdDgDimLabelLine::kLengthBelow :
      val = OD_T("Length below"); break;
    case OdDgDimLabelLine::kAngleBelow :
      val = OD_T("Angle below"); break;
    case OdDgDimLabelLine::kLengthAngleAbove :
      val = OD_T("Length Angle above"); break;
    case OdDgDimLabelLine::kLengthAngleBelow :
      val = OD_T("Length Angle below"); break;
    default :
      val = OD_T("Length/Angle"); break;
  }

  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdGeQuaternion const& value )
{
  dumpFieldName( name );

  OdString val;
  val.format( L"%g; %g; %g; %g", value.w, value.x, value.y, value.z );
  dumpFieldValue( val );
}


//#pragma MARKMESSAGE("TODO: Examples. TextAttributes interface was modified.");
//inline void OdExDgnDumper::writeFieldValue( const OdString& name, TextAttributes const& value )
//{
//  writeShift();
//  val.format( L"%s:", name.c_str() );
//
//  m_nesting++;
//
//  writeFieldValue( "Is interchar", value.isInterchar );
//  writeFieldValue( "Interchar spacing", value.intercharSpacing );
//  writeFieldValue( "Is fixed width spacing", value.isFixedWidthSpacing );
//  writeFieldValue( "Is underlined", value.isUnderlined );
//  writeFieldValue( "Underline offset", value.underlineOffset );
//  writeFieldValue( "Is overlined", value.isOverlined );
//  writeFieldValue( "Overline offset", value.overlineOffset );
//  writeFieldValue( "Is slant", value.isSlant );
//  writeFieldValue( "Slang angle", value.slantAngle );
//  writeFieldValue( "Is vertical", value.isVertical );
//  writeFieldValue( "Is right-to-left", value.isRightToLeft );
//  writeFieldValue( "Codepage presents", value.isCodepagePresented );
//  writeFieldValue( "Codepage", value.codePage );
//  writeFieldValue( "Background presents", value.isBackgroundPresented );
//  writeFieldValue( "Background fill color", value.backgroundFillColor );
//  writeFieldValue( "Background border color", value.backgroundBorderColor );
//  writeFieldValue( "Background border style", value.backgroundBorderStyle );
//  writeFieldValue( "Background border weight", value.backgroundBorderWeight );
//  writeFieldValue( "Background border X", value.backgroundBorderX );
//  writeFieldValue( "Background border Y", value.backgroundBorderY );
//  writeFieldValue( "Is subscript", value.isSubscript );
//  writeFieldValue( "Is superscript", value.isSuperscript );
//  writeFieldValue( "Has text style", value.hasTextStyle );
//  writeFieldValue( "Text style", value.textStyle );
//  writeFieldValue( "Count of line breaks", value.lineBreakCount );
//  writeFieldValue( "Is bold", value.isBold );
//  writeFieldValue( "Has underline style", value.hasUnderlineStyle );
//  writeFieldValue( "Underline color", value.underlineColor );
//  writeFieldValue( "Underline style", value.underlineStyle );
//  writeFieldValue( "Underline weight", value.underlineWeight );
//  writeFieldValue( "Has overline style", value.hasOverlineStyle );
//  writeFieldValue( "Overline color", value.overlineColor );
//  writeFieldValue( "Overline style", value.overlineStyle );
//  writeFieldValue( "Overline weight", value.overlineWeight );
//  writeFieldValue( "Is word-wrap text node", value.isWordWrapTextNode );
//  writeFieldValue( "Is full justification", value.isFullJustification );
//  writeFieldValue( "Has color", value.hasColor );
//  writeFieldValue( "Color", value.color );
//  writeFieldValue( "Is ACAD interchar spacing", value.isAcadIntercharSpacing );
//  writeFieldValue( "Is backwards", value.isBackwards );
//  writeFieldValue( "Is upside-down", value.isUpsideDown );
//
//  m_nesting--;
//}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgGraphicsElement::Class value )
{
  dumpFieldName( name );

  OdString val;
  switch( value )
  {
  case OdDgGraphicsElement::kClassPrimary          : val.format( L"Primary" ); break;
  case OdDgGraphicsElement::kClassPatternComponent : val.format( L"Pattern component" ); break;
  case OdDgGraphicsElement::kClassConstruction     : val.format( L"Construction" ); break;
  case OdDgGraphicsElement::kClassDimension        : val.format( L"Dimension" ); break;
  case OdDgGraphicsElement::kClassPrimaryRule      : val.format( L"Primary rule" ); break;
  case OdDgGraphicsElement::kClassLinearPatterned  : val.format( L"Linear patterned" ); break;
  case OdDgGraphicsElement::kClassConstructionRule : val.format( L"Construction rule" ); break;
  }
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdGeMatrix2d const& value )
{
  dumpFieldName( name );

  OdString val;
  val.format( L"%g; %g; %g; %g", value[ 0 ][ 0 ], value[ 1 ][ 0 ], value[ 0 ][ 1 ], value[ 1 ][ 1 ] );
  dumpFieldValue( val );
}


//inline void OdExDgnDumper::writeFieldValue( OdString const& name, OdDgDimension::ToolType value )
//{
//  dumpFieldName( name );
//  OdDgDimTool tool;
//  OdDgDimensionPtr pDim = OdDgDimension::cast( m_object );
//  OdString val;
//  switch( value )
//  {
//  case OdDgDimension::kToolTypeInvalid             : val.format( LInvalid" ); break;
//  case OdDgDimension::kToolTypeSizeArrow          : val.format( LSize arrow" ); break;
//    m_nesting++;
//    pDim->getDimensionTool( tool );
//    if(static_cast<OdDgDimToolSizeArrow*>( &tool )->getArcSymbolFlag())
//      val.format( LTool Arc Symbol Flag set" );
//    val.format( LTool Datum Value: %d", static_cast<OdDgDimToolOrdinate*>( &tool )->getDatumValue() );
//    if(static_cast<OdDgDimToolSizeArrow*>( &tool )->getStackExtLinesFlag())
//      val.format( LTool StackExtLines Flag set" );
//    if(static_cast<OdDgDimToolSizeArrow*>( &tool )->getLeftExtLinesPresentFlag())
//      val.format( LTool LeftExtLinesPresent Flag set" );
//    if(static_cast<OdDgDimToolSizeArrow*>( &tool )->getRightExtLinesPresentFlag())
//      val.format( LTool RightExtLinesPresent Flag set" );
//    //static_cast<OdDgDimToolSizeArrow*>( &tool )->getFirstTerminator();
//    switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getFirstTerminator())
//    {      //OdDgDimTool::TerminatorType     kTtNone           = 0,  kTtArrow          = 1,  kTtStroke         = 2,  kTtCircle         = 3,  kTtFilledCircle   = 4,
//    case OdDgDimTool::kTtNone:
//      fprintf(DumpStream, "Tool First Terminator Type: kTtNone");
//      break;
//    case OdDgDimTool::kTtArrow:
//      fprintf(DumpStream, "Tool First Terminator Type: kTtArrow");
//      break;
//    case OdDgDimTool::kTtStroke:
//      fprintf(DumpStream, "Tool First Terminator Type: kTtStroke");
//      break;
//    case OdDgDimTool::kTtCircle:
//      fprintf(DumpStream, "Tool First Terminator Type: kTtCircle");
//      break;
//    case OdDgDimTool::kTtFilledCircle:
//      fprintf(DumpStream, "Tool First Terminator Type: kTtFilledCircle");
//      break;
//    }
//    //static_cast<OdDgDimToolSizeArrow*>( &tool )->getJointTerminator();
//    switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getJointTerminator())
//    {      //OdDgDimTool::TerminatorType     kTtNone           = 0,  kTtArrow          = 1,  kTtStroke         = 2,  kTtCircle         = 3,  kTtFilledCircle   = 4,
//    case OdDgDimTool::kTtNone:
//      fprintf(DumpStream, "Tool Joint Terminator Type: kTtNone");
//      break;
//    case OdDgDimTool::kTtArrow:
//      fprintf(DumpStream, "Tool Joint Terminator Type: kTtArrow");
//      break;
//    case OdDgDimTool::kTtStroke:
//      fprintf(DumpStream, "Tool Joint Terminator Type: kTtStroke");
//      break;
//    case OdDgDimTool::kTtCircle:
//      fprintf(DumpStream, "Tool Joint Terminator Type: kTtCircle");
//      break;
//    case OdDgDimTool::kTtFilledCircle:
//      fprintf(DumpStream, "Tool Joint Terminator Type: kTtFilledCircle");
//      break;
//    }
//    //static_cast<OdDgDimToolSizeArrow*>( &tool )->getLeftTerminator();
//    switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getLeftTerminator())
//    {      //OdDgDimTool::TerminatorType     kTtNone           = 0,  kTtArrow          = 1,  kTtStroke         = 2,  kTtCircle         = 3,  kTtFilledCircle   = 4,
//    case OdDgDimTool::kTtNone:
//      fprintf(DumpStream, "Tool Left Terminator Type: kTtNone");
//      break;
//    case OdDgDimTool::kTtArrow:
//      fprintf(DumpStream, "Tool Left Terminator Type: kTtArrow");
//      break;
//    case OdDgDimTool::kTtStroke:
//      fprintf(DumpStream, "Tool Left Terminator Type: kTtStroke");
//      break;
//    case OdDgDimTool::kTtCircle:
//      fprintf(DumpStream, "Tool Left Terminator Type: kTtCircle");
//      break;
//    case OdDgDimTool::kTtFilledCircle:
//      fprintf(DumpStream, "Tool Left Terminator Type: kTtFilledCircle");
//      break;
//    }
//    //static_cast<OdDgDimToolSizeArrow*>( &tool )->getRightTerminator();
//    switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getRightTerminator())
//    {      //OdDgDimTool::TerminatorType     kTtNone           = 0,  kTtArrow          = 1,  kTtStroke         = 2,  kTtCircle         = 3,  kTtFilledCircle   = 4,
//    case OdDgDimTool::kTtNone:
//      fprintf(DumpStream, "Tool Right Terminator Type: kTtNone");
//      break;
//    case OdDgDimTool::kTtArrow:
//      fprintf(DumpStream, "Tool Right Terminator Type: kTtArrow");
//      break;
//    case OdDgDimTool::kTtStroke:
//      fprintf(DumpStream, "Tool Right Terminator Type: kTtStroke");
//      break;
//    case OdDgDimTool::kTtCircle:
//      fprintf(DumpStream, "Tool Right Terminator Type: kTtCircle");
//      break;
//    case OdDgDimTool::kTtFilledCircle:
//      fprintf(DumpStream, "Tool Right Terminator Type: kTtFilledCircle");
//      break;
//    }
//    //static_cast<OdDgDimToolSizeArrow*>( &tool )->getPrefix();
//    switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getPrefix())
//    {      //OdDgDimTool::CustomSymbol       kCsNone         = 0    , kCsDiameter     = 1    , kCsRadius       = 2    , kCsSquare       = 3    , kCsSR           = 4    , kCsSDiameter    = 5
//    case OdDgDimTool::kCsNone:
//      fprintf(DumpStream, "Tool Prefix: kCsNone");
//      break;
//    case OdDgDimTool::kCsDiameter:
//      fprintf(DumpStream, "Tool Prefix: kCsDiameter");
//      break;
//    case OdDgDimTool::kCsRadius:
//      fprintf(DumpStream, "Tool Prefix: kCsRadius");
//      break;
//    case OdDgDimTool::kCsSquare:
//      fprintf(DumpStream, "Tool Prefix: kCsSquare");
//      break;
//    case OdDgDimTool::kCsSR:
//      fprintf(DumpStream, "Tool Prefix: kCsSR");
//      break;
//    case OdDgDimTool::kCsSDiameter:
//      fprintf(DumpStream, "Tool Prefix: kCsSDiameter");
//      break;
//    }
//    //static_cast<OdDgDimToolSizeArrow*>( &tool )->getSuffix();
//    switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getSuffix())
//    {      //OdDgDimTool::CustomSymbol       kCsNone         = 0    , kCsDiameter     = 1    , kCsRadius       = 2    , kCsSquare       = 3    , kCsSR           = 4    , kCsSDiameter    = 5
//    case OdDgDimTool::kCsNone:
//      fprintf(DumpStream, "Tool Suffix: kCsNone");
//      break;
//    case OdDgDimTool::kCsDiameter:
//      fprintf(DumpStream, "Tool Suffix: kCsDiameter");
//      break;
//    case OdDgDimTool::kCsRadius:
//      fprintf(DumpStream, "Tool Suffix: kCsRadius");
//      break;
//    case OdDgDimTool::kCsSquare:
//      fprintf(DumpStream, "Tool Suffix: kCsSquare");
//      break;
//    case OdDgDimTool::kCsSR:
//      fprintf(DumpStream, "Tool Suffix: kCsSR");
//      break;
//    case OdDgDimTool::kCsSDiameter:
//      fprintf(DumpStream, "Tool Suffix: kCsSDiameter");
//      break;
//    }
//    //static_cast<OdDgDimToolSizeArrow*>( &tool )->getTextType();
//    switch(static_cast<OdDgDimToolSizeArrow*>( &tool )->getTextType())
//    {      //kStandard = 0    , kVertical = 1    , kMixed    = 2
//    case OdDgDimTool::kStandard:
//      fprintf(DumpStream, "Tool Text Type: kStandard");
//      break;
//    case OdDgDimTool::kVertical:
//      fprintf(DumpStream, "Tool Text Type: kVertical");
//      break;
//    case OdDgDimTool::kMixed:
//      fprintf(DumpStream, "Tool Text Type: kMixed");
//      break;
//    }
//    m_nesting--;
//    break;
//  case OdDgDimension::kToolTypeSizeStroke         : val.format( LSize stroke" ); break;
//  case OdDgDimension::kToolTypeLocateSingle       : val.format( LLocate single" ); break;
//  case OdDgDimension::kToolTypeLocateStacked      : val.format( LLocate stacked" ); break;
//  case OdDgDimension::kToolTypeAngleSize          : val.format( LAngle size" ); break;
//  case OdDgDimension::kToolTypeArcSize            : val.format( LArc size" ); break;
//  case OdDgDimension::kToolTypeAngleLocation      : val.format( LAngle location" ); break;
//  case OdDgDimension::kToolTypeArcLocation        : val.format( LArc location" ); break;
//  case OdDgDimension::kToolTypeAngleLines         : val.format( LAngle lines" ); break;
//  case OdDgDimension::kToolTypeAngleAxis          : val.format( LAngle axis" ); break;
//  case OdDgDimension::kToolTypeRadius             : val.format( LRadius" ); break;
//  case OdDgDimension::kToolTypeDiameter           : val.format( LDiameter" ); break;
//  case OdDgDimension::kToolTypeDiameterPara       : val.format( LDiameter para" ); break;
//  case OdDgDimension::kToolTypeDiameterPerp       : val.format( LDiameter perp" ); break;
//  case OdDgDimension::kToolTypeCustomLinear       : val.format( LCustom linear" ); break;
//  case OdDgDimension::kToolTypeOrdinate           : val.format( LOrdinate" ); 
//    m_nesting++;
//    pDim->getDimensionTool( tool );
//    if(static_cast<OdDgDimToolOrdinate*>( &tool )->getArcSymbolFlag())
//      val.format( LTool Arc Symbol Flag set" );
//    val.format( LTool Datum Value: %d", static_cast<OdDgDimToolOrdinate*>( &tool )->getDatumValue() );
//    if(static_cast<OdDgDimToolOrdinate*>( &tool )->getDecrementInReverseDirectionFlag())
//      val.format( LTool DecrementInReverse Direction Flag set" );
//    if(static_cast<OdDgDimToolOrdinate*>( &tool )->getFreeLocationOfTxtFlag())
//      val.format( LTool FreeLocationOfTxt Flag set" );
//    if(static_cast<OdDgDimToolOrdinate*>( &tool )->getStackExtLinesFlag())
//      val.format( LTool StackExtLines Flag set" );
//    m_nesting--;
//    break;
//  case OdDgDimension::kToolTypeRadiusExtended     : val.format( LRadius extended" ); break;
//  case OdDgDimension::kToolTypeDiameterExtended   : val.format( LDiameter extended" ); break;
//  case OdDgDimension::kToolTypeCenter              : val.format( LCenter" ); break;
//  }
//  dumpFieldValue( val );
//}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimPoint& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldName( val );
  writeFieldValue( "  Point", value.getPoint() );
  writeFieldValue( "  Offset to dimension line", value.getOffsetToDimLine() );
  writeFieldValue( "  Offset Y", value.getOffsetY() );
  writeFieldValue( "  Text alignment", value.getJustification() );
  dumpFieldName( "  Flags:" );
  writeFieldValue( "    Associative", value.getAssociativeFlag() );
  writeFieldValue( "    Relative", value.getRelative() != 0 );
  writeFieldValue( "    WitnessControlLocal", value.getWitnessControlLocalFlag() );
  writeFieldValue( "    NoWitnessLine", value.getNoWitnessLineFlag() );
  writeFieldValue( "    UseAltSymbology", value.getUseAltSymbologyFlag() );

  if( value.getPrimaryTextFlag() )
  {
    writeFieldValue( "  Primary text", value.getPrimaryText() );
  }

  if( value.getPrimaryTopToleranceTextFlag() )
  {
    writeFieldValue( "  Primary Top text", value.getPrimaryTopToleranceText() );
  }

  if( value.getPrimaryBottomToleranceTextFlag() )
  {
    writeFieldValue( "  Primary Bottom text", value.getPrimaryBottomToleranceText() );
  }

  if( value.getSecondaryTextFlag() )
  {
    writeFieldValue( "  Secondary text", value.getSecondaryText() );
  }

  if( value.getSecondaryTopToleranceTextFlag() )
  {
    writeFieldValue( "  Secondary Top text", value.getSecondaryTopToleranceText() );
  }

  if( value.getSecondaryBottomToleranceTextFlag() )
  {
    writeFieldValue( "  Secondary Bottom text", value.getSecondaryBottomToleranceText() );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTextInfo::TextAlignment value )
{
  OdString strJust;

  switch( value )
  {
    case OdDgDimTextInfo::kLeftText:
    {
      strJust = OD_T("kLeftText");
    } break;

    case OdDgDimTextInfo::kCenterLeftText:
    {
      strJust = OD_T("kCenterLeftText");
    } break;

    case OdDgDimTextInfo::kCenterRightText:
    {
      strJust = OD_T("kCenterRightText");
    } break;

    case OdDgDimTextInfo::kRightText:
    {
      strJust = OD_T("kRightText");
    } break;

    case OdDgDimTextInfo::kManualText:
    {
      strJust = OD_T("kManualText");
    } break;
  }

  writeFieldValue(name, strJust);
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTextInfo const& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldName( val );
  writeFieldValue( "  Width", value.getWidth() );
  writeFieldValue( "  Height", value.getHeight() );
  writeFieldValue( "  Font ID", value.getFontEntryId() );
  writeFieldValue( "  Color", value.getColorIndex() );
  writeFieldValue( "  Weight", value.getWeight() );

  switch( value.getStackedFractionType() )
  {
    case OdDgDimTextInfo::kFractionFromText :
    {
      writeFieldValue( "  Stacked Fraction Type", OdString(L"kFractionFromText") );
    } break;

    case OdDgDimTextInfo::kFractionHorizontal :
    {
      writeFieldValue( "  Stacked Fraction Type", OdString(L"kFractionHorizontal") );
    } break;

    case OdDgDimTextInfo::kFractionDiagonal :
    {
      writeFieldValue( "  Stacked Fraction Type", OdString(L"kFractionDiagonal") );
    } break;
  }

  switch( value.getStackFractAlignment() )
  {
    case OdDgDimTextInfo::kFractAlignmentTop :
    {
      writeFieldValue( "  Stacked Fraction Alignment", OdString(L"kFractAlignmentTop") );
    } break;

    case OdDgDimTextInfo::kFractAlignmentCenter :
    {
      writeFieldValue( "  Stacked Fraction Alignment", OdString(L"kFractAlignmentCenter") );
    } break;

    case OdDgDimTextInfo::kFractAlignmentBottom :
    {
      writeFieldValue( "  Stacked Fraction Alignment", OdString(L"kFractAlignmentBottom") );
    } break;
  }

  dumpFieldName(  L"  Text flags:" );
  writeFieldValue( "    Use text color", value.getUseColorFlag() );
  writeFieldValue( "    Use weight ", value.getUseWeightFlag() );
  writeFieldValue( "    Show primary master units ", !value.getPrimaryNoMasterUnitsFlag());
  writeFieldValue( "    Has primary alt format ", value.getHasAltFormatFlag() );
  writeFieldValue( "    Show secondary master units ", !value.getSecNoMasterUnitsFlag());
  writeFieldValue( "    Has secondary alt format ", value.getHasSecAltFormatFlag() );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTextFormat const& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldName( val );

  writeFieldValue( "  Primary accuracy", value.getPrimaryAccuracy() );
  writeFieldValue( "  Secondary accuracy", value.getSecondaryAccuracy() );

  switch( value.getAngleMode() )
  { 
    case OdDgDimTextFormat::kAngle_D:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_D") );
    } break;

    case OdDgDimTextFormat::kAngle_DM:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_DM") );
    } break;

    case OdDgDimTextFormat::kAngle_DMS:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_DMS") );
    } break;

    case OdDgDimTextFormat::kAngle_C:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_C") );
    } break;

    case OdDgDimTextFormat::kAngle_Radian:
    {
      writeFieldValue( "  Angle display mode", OdString(L"kAngle_Radian") );
    } break;
  }

  dumpFieldName( "  Text Format Flags:" );
  writeFieldValue( "    AngleMeasure", value.getAngleMeasureFlag() );
  writeFieldValue( "    AngleFormat", value.getAngleFormatFlag() );
  writeFieldValue( "    PrimarySubUnits", value.getPrimarySubUnitsFlag() );
  writeFieldValue( "    PrimaryLabels", value.getPrimaryLabelsFlag() );
  writeFieldValue( "    PrimaryDelimiter", value.getPrimaryDelimiterFlag() );
  writeFieldValue( "    DecimalComma", value.getDecimalCommaFlag() );
  writeFieldValue( "    SuperScriptLSD", value.getSuperScriptLSDFlag() );
  writeFieldValue( "    RoundLSD", value.getRoundLSDFlag() );
  writeFieldValue( "    OmitLeadDelimiter", value.getOmitLeadDelimiterFlag() );
  writeFieldValue( "    LocalFileUnits", value.getLocalFileUnitsFlag() );
  writeFieldValue( "    UnusedDeprecated", value.getUnusedDeprecatedFlag() );
  writeFieldValue( "    ThousandSeparator", value.getThousandSepFlag() );
  writeFieldValue( "    MetricSpace", value.getMetricSpaceFlag() );
  writeFieldValue( "    SecondarySubUnits", value.getSecondarySubUnitsFlag() );
  writeFieldValue( "    SecondaryLabels", value.getSecondaryLabelsFlag() );
  writeFieldValue( "    SecondaryDelimiter", value.getSecondaryDelimiterFlag() );
  writeFieldValue( "    Radians", value.getRadiansFlag() );
  writeFieldValue( "    Show primary master units if zero", value.getPriAllowZeroMastFlag() );
  writeFieldValue( "    Show secondary master units if zero", value.getSecAllowZeroMastFlag() );
  writeFieldValue( "    Show primary sub units if zero", !value.getPriSubForbidZeroMastFlag() );
  writeFieldValue( "    Show secondary sub units if zero", !value.getSecSubForbidZeroMastFlag() );
  writeFieldValue( "    HideAngleSeconds", !value.getHideAngleSecondsFlag() );
  writeFieldValue( "    SkipNonStackedFractionSpace", !value.getSkipNonStackedFractionSpaceFlag() );
}


inline void OdExDgnDumper::writeFieldValue( OdString const& name, OdDgDimTextFormat::Accuracy value )
{
  dumpFieldName( name );

  OdString val;
  switch( value )
  {
  case OdDgDimTextFormat::kAccuracyNone              : val.format( L"1 digit" ); break;

  case OdDgDimTextFormat::kDecimal1              : val.format( L"Decimal, 2 digit" ); break;
  case OdDgDimTextFormat::kDecimal2             : val.format( L"Decimal, 3 digit" ); break;
  case OdDgDimTextFormat::kDecimal3       : val.format( L"Decimal, 4 digit" ); break;
  case OdDgDimTextFormat::kDecimal4       : val.format( L"Decimal, 5 digit" ); break;
  case OdDgDimTextFormat::kDecimal5       : val.format( L"Decimal, 6 digit" ); break;
  case OdDgDimTextFormat::kDecimal6       : val.format( L"Decimal, 7 digit" ); break;
  case OdDgDimTextFormat::kDecimal7       : val.format( L"Decimal, 8 digit" ); break;
  case OdDgDimTextFormat::kDecimal8       : val.format( L"Decimal, 9 digit" ); break;

  case OdDgDimTextFormat::kFractional2          : val.format( L"Fractional, 2-th" ); break;
  case OdDgDimTextFormat::kFractional4          : val.format( L"Fractional, 4-th" ); break;
  case OdDgDimTextFormat::kFractional8          : val.format( L"Fractional, 8-th" ); break;
  case OdDgDimTextFormat::kFractional16         : val.format( L"Fractional, 16-th" ); break;
  case OdDgDimTextFormat::kFractional32         : val.format( L"Fractional, 32-th" ); break;
  case OdDgDimTextFormat::kFractional64         : val.format( L"Fractional, 64-th" ); break;

  case OdDgDimTextFormat::kExponential1 : val.format( L"Exponential, 1 digit for mantissa" ); break;
  case OdDgDimTextFormat::kExponential2 : val.format( L"Exponential, 2 digit for mantissa" ); break;
  case OdDgDimTextFormat::kExponential3 : val.format( L"Exponential, 3 digit for mantissa" ); break;
  case OdDgDimTextFormat::kExponential4 : val.format( L"Exponential, 4 digit for mantissa" ); break;
  case OdDgDimTextFormat::kExponential5 : val.format( L"Exponential, 5 digit for mantissa" ); break;
  case OdDgDimTextFormat::kExponential6 : val.format( L"Exponential, 6 digit for mantissa" ); break;
  case OdDgDimTextFormat::kExponential7 : val.format( L"Exponential, 7 digit for mantissa" ); break;
  case OdDgDimTextFormat::kExponential8 : val.format( L"Exponential, 8 digit for mantissa" ); break;
  }
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimGeometry const& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldName( val );

  writeFieldValue( "  Witness line offset", value.getWitnessLineOffset() );
  writeFieldValue( "  Witness line extend", value.getWitnessLineExtend() );
  writeFieldValue( "  Text lift", value.getTextLift() );
  writeFieldValue( "  Text margin", value.getTextMargin() );
  writeFieldValue( "  Terminator width", value.getTerminatorWidth() );
  writeFieldValue( "  Terminator height", value.getTerminatorHeight() );
  writeFieldValue( "  Stack offset", value.getStackOffset() );
  writeFieldValue( "  Center size", value.getCenterSize() );

  if( value.getUseMargin() )
    writeFieldValue( "  Min leader", value.getMargin() );
  else
    writeFieldValue( "  Min leader", value.getTerminatorWidth()*2.0 );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimOptionPtr& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldName( val );

  if(!value.isNull())
  {
    switch( value->getType() )
    {
      case OdDgDimOption::kNone                             : 
      {
        writeFieldValue( "  Type", OdString(OD_T("kNone")) ); 
      } break;

      case OdDgDimOption::kTolerance                        :
      {
        OdDgDimOptionTolerancePtr pTolerOptions = value;
        writeFieldValue("", pTolerOptions);
      } break;

      case OdDgDimOption::kTerminators                      :
      {
        OdDgDimOptionTerminatorsPtr pTermOptions = value;
        writeFieldValue("", pTermOptions);
      } break;

      case OdDgDimOption::kPrefixSymbol                     :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kPrefixSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kSuffixSymbol                     :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kSuffixSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kDiameterSymbol                   :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kDiameterSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kPrefixSuffix                     :
      {
        OdDimOptionPrefixSuffixPtr pPrefixSuffixOptions = value;
        writeFieldValue("", pPrefixSuffixOptions);
      } break;

      case OdDgDimOption::kPrimaryUnits                     :
      {
        OdDgDimOptionUnitsPtr pUnitsOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kPrimaryUnits")) );
        writeFieldValue("", pUnitsOptions );
      } break;

      case OdDgDimOption::kSecondaryUnits                   :
      {
        OdDgDimOptionUnitsPtr pUnitsOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kSecondaryUnits")) );
        writeFieldValue("", pUnitsOptions );
      } break;

      case OdDgDimOption::kTerminatorSymbology              :
      {
        OdDgDimOptionTerminatorSymbologyPtr pTermSymbolOptions = value;
        writeFieldValue("", pTermSymbolOptions );
      } break;

      case OdDgDimOption::kView                             :
      {
        OdDgDimOptionViewPtr pViewOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kView")) );

        if( !pViewOptions.isNull() )
          writeFieldValue("  Rotation", pViewOptions->getQuaternion() );
      } break;

      case OdDgDimOption::kAlternatePrimaryUnits            :
      {
        OdDgDimOptionAltFormatPtr pAltOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kAlternativePrimaryUnits")) );
        writeFieldValue("", pAltOptions );
      } break;

      case OdDgDimOption::kOffset                           :
      {
        OdDgDimOptionOffsetPtr pOffsetOptions = value;
        writeFieldValue("", pOffsetOptions );
      } break;

      case OdDgDimOption::kAlternateSecondaryUnits          :
      {
        OdDgDimOptionAltFormatPtr pAltOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kAlternativeSecondaryUnits")) );
        writeFieldValue("", pAltOptions );
      } break;

      case OdDgDimOption::kAlternatePrefixSymbol            :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kAlternatePrefixSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kAlternateSuffixSymbol            :
      {
        OdDgDimOptionSymbolPtr pSymbolOptions = value;
        writeFieldValue("  Type", OdString(OD_T("kAlternateSuffixSymbol")) );
        writeFieldValue("", pSymbolOptions );
      } break;

      case OdDgDimOption::kProxyCell                        :
      {
        OdDgDimOptionProxyCellPtr pCellOptions = value;
        writeFieldValue("", pCellOptions );
      } break;
    }
  }
  else
  {
    writeFieldValue( "  Type", OdString(OD_T("[value unknown]")) );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDgDimOptionTolerancePtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kTolerance")) );

  if( value.isNull() )
    return;

  writeFieldValue("  Upper value", value->getToleranceUpper() );
  writeFieldValue("  Lower value", value->getToleranceLower() );
  writeFieldValue("  Stack if equal", value->getStackEqualFlag() );
  writeFieldValue("  Show sign for zero", value->getShowSignForZeroFlag() );
  writeFieldValue("  Left margin", value->getToleranceHorizSep() );
  writeFieldValue("  Separator margin", value->getToleranceVertSep() );
  writeFieldValue("  Font entry Id", value->getFontEntryId() );
  writeFieldValue("  Text Width", value->getToleranceTextWidth() );
  writeFieldValue("  Text Height", value->getToleranceTextHeight() );

  if( value->getTolerancePlusMinusSymbol() )
    writeFieldValue("  Plus/Minus symbol", value->getTolerancePlusMinusSymbol() );

  if( value->getTolerancePrefixSymbol() )
    writeFieldValue("  Prefix symbol", value->getTolerancePrefixSymbol() );

  if( value->getToleranceSuffixSymbol() )
    writeFieldValue("  Suffix symbol", value->getToleranceSuffixSymbol() );

  writeFieldValue("  Stack align", value->getStackAlign() );
}

inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDgDimOptionTerminatorsPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kTerminators")) );

  if( value.isNull() )
    return;
  
  if( value->getArrowTermStyle() == OdDgDimOptionTerminators::kTermDefault )
  {
    writeFieldValue("  Arrow style", OdString(OD_T("kTermDefault")) );
  }
  else if( value->getArrowTermStyle() == OdDgDimOptionTerminators::kTermSymbol )
  {
    writeFieldValue("  Arrow style", OdString(OD_T("kTermSymbol")) );
    writeFieldValue("  Arrow Font entry Id", value->getArrowFontID() );
    writeFieldValue("  Arrow Symbol code", value->getArrowSymbol() );
  }
  else if( value->getArrowTermStyle() == OdDgDimOptionTerminators::kTermCell )
  {
    writeFieldValue("  Arrow style", OdString(OD_T("kTermCell")) );
    writeFieldValue("  Arrow Cell Id", value->getArrowCellID() );
  }
  else
  {
    writeFieldValue("  Arrow style", OdString(OD_T("kTermScaledCell")) );
    writeFieldValue("  Arrow Cell Id", value->getArrowCellID() );
    writeFieldValue("  Arrow Cell scale", value->getSharedCellScale() );
  }

  if( value->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermDefault )
  {
    writeFieldValue("  Stroke style", OdString(OD_T("kTermDefault")) );
  }
  else if( value->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermSymbol )
  {
    writeFieldValue("  Stroke style", OdString(OD_T("kTermSymbol")) );
    writeFieldValue("  Stroke Font entry Id", value->getStrokeFontID() );
    writeFieldValue("  Stroke Symbol code", value->getStrokeSymbol() );
  }
  else if( value->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermCell )
  {
    writeFieldValue("  Stroke style", OdString(OD_T("kTermCell")) );
    writeFieldValue("  Stroke Cell Id", value->getStrokeCellID() );
  }
  else
  {
    writeFieldValue("  Stroke style", OdString(OD_T("kTermScaledCell")) );
    writeFieldValue("  Stroke Cell Id", value->getStrokeCellID() );
    writeFieldValue("  Stroke Cell scale", value->getSharedCellScale() );
  }

  if( value->getOriginTermStyle() == OdDgDimOptionTerminators::kTermDefault )
  {
    writeFieldValue("  Origin style", OdString(OD_T("kTermDefault")) );
  }
  else if( value->getOriginTermStyle() == OdDgDimOptionTerminators::kTermSymbol )
  {
    writeFieldValue("  Origin style", OdString(OD_T("kTermSymbol")) );
    writeFieldValue("  Origin Font entry Id", value->getOriginFontID() );
    writeFieldValue("  Origin Symbol code", value->getOriginSymbol() );
  }
  else if( value->getOriginTermStyle() == OdDgDimOptionTerminators::kTermCell )
  {
    writeFieldValue("  Origin style", OdString(OD_T("kTermCell")) );
    writeFieldValue("  Origin Cell Id", value->getOriginCellID() );
  }
  else
  {
    writeFieldValue("  Origin style", OdString(OD_T("kTermScaledCell")) );
    writeFieldValue("  Origin Cell Id", value->getOriginCellID() );
    writeFieldValue("  Origin Cell scale", value->getSharedCellScale() );
  }

  if( value->getDotTermStyle() == OdDgDimOptionTerminators::kTermDefault )
  {
    writeFieldValue("  Dot style", OdString(OD_T("kTermDefault")) );
  }
  else if( value->getDotTermStyle() == OdDgDimOptionTerminators::kTermSymbol )
  {
    writeFieldValue("  Dot style", OdString(OD_T("kTermSymbol")) );
    writeFieldValue("  Dot Font entry Id", value->getDotFontID() );
    writeFieldValue("  Dot Symbol code", value->getDotSymbol() );
  }
  else if( value->getDotTermStyle() == OdDgDimOptionTerminators::kTermCell )
  {
    writeFieldValue("  Dot style", OdString(OD_T("kTermCell")) );
    writeFieldValue("  Dot Cell Id", value->getDotCellID() );
  }
  else
  {
    writeFieldValue("  Dot style", OdString(OD_T("kTermScaledCell")) );
    writeFieldValue("  Dot Cell Id", value->getDotCellID() );
    writeFieldValue("  Dot Cell scale", value->getSharedCellScale() );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDgDimOptionTerminatorSymbologyPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kTerminatorSymbology")) );

  if( value.isNull() )
    return;

  writeFieldValue("  Use Line type", value->getStyleFlag());
  writeFieldValue("  Use Line weight", value->getWeightFlag());
  writeFieldValue("  Use Color", value->getColorFlag());

  if( value->getStyleFlag() )
    writeFieldValue("  Line Type entry Id", value->getStyle());

  if( value->getWeightFlag() )
    writeFieldValue("  Line Weight", value->getWeight());

  if( value->getColorFlag() )
    writeFieldValue("  Color", value->getColor());
}

inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDgDimOptionSymbolPtr& value )
{
  if( value.isNull() )
    return;

  writeFieldValue("  Font entry Id", value->getSymbolFont() );
  writeFieldValue("  Symbol code", value->getSymbolChar() );
}

inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDimOptionPrefixSuffixPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kPrefixSuffix")) );

  if( value.isNull() )
    return;

  if( value->getMainPrefix() )
  {
    writeFieldValue("  Main prefix", value->getMainPrefix() );
  }

  if( value->getMainSuffix() )
  {
    writeFieldValue("  Main suffix", value->getMainSuffix() );
  }

  if( value->getUpperPrefix() )
  {
    writeFieldValue("  Upper prefix", value->getUpperPrefix() );
  }

  if( value->getUpperSuffix() )
  {
    writeFieldValue("  Upper suffix", value->getUpperSuffix() );
  }

  if( value->getLowerPrefix() )
  {
    writeFieldValue("  Lower prefix", value->getLowerPrefix() );
  }

  if( value->getLowerSuffix() )
  {
    writeFieldValue("  Lower suffix", value->getLowerSuffix() );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgModel::UnitBase value )
{
  if( value == OdDgModel::kNone )
  {
    writeFieldValue( name, OdString(OD_T("kNone")) );
  }
  else if( value == OdDgModel::kMeter )
  {
    writeFieldValue( name, OdString(OD_T("kMeter")) );
  }
  else
  {
    writeFieldValue( name, OdString(OD_T("Unknown")) );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgModel::UnitSystem value )
{
  if( value == OdDgModel::kCustom )
  {
    writeFieldValue( name, OdString(OD_T("kCustom")) );
  }
  else if( value == OdDgModel::kMetric )
  {
    writeFieldValue( name, OdString(OD_T("kMetric")) );
  }
  else if( value == OdDgModel::kEnglish )
  {
    writeFieldValue( name, OdString(OD_T("kEnglish")) );
  }
  else
  {
    writeFieldValue( name, OdString(OD_T("Unknown")) );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDgDimOptionUnitsPtr& value )
{
  if( value.isNull() )
    return;

  OdDgModel::UnitDescription descr;
  value->getMasterUnit( descr );
  dumpFieldName( L"  Master units:" );
  writeFieldValue( L"    Unit base", descr.m_base );
  writeFieldValue( L"    Unit system", descr.m_system );
  writeFieldValue( L"    Numerator", descr.m_numerator );
  writeFieldValue( L"    Denominator", descr.m_denominator );
  writeFieldValue( L"    Name", descr.m_name );
  value->getSubUnit( descr );
  dumpFieldName( L"  Sub units:" );
  writeFieldValue( L"    Unit base", descr.m_base );
  writeFieldValue( L"    Unit system", descr.m_system );
  writeFieldValue( L"    Numerator", descr.m_numerator );
  writeFieldValue( L"    Denominator", descr.m_denominator );
  writeFieldValue( L"    Name", descr.m_name );
}

inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDgDimOptionAltFormatPtr& value )
{
  if( value.isNull() )
    return;

  writeFieldValue("  Accuracy", value->getAccuracy() );
  writeFieldValue("  Show sub units", value->getSubUnits() );
  writeFieldValue("  Show unit labels", value->getLabel() );
  writeFieldValue("  Show delimiter", value->getDelimiter() );
  writeFieldValue("  Show sub units only", value->getNoMasterUnits() );
  writeFieldValue("  Allow zero master units", value->getAllowZeroMasterUnits() );
  
  if( value->getMoreThanThreshold() )
  {
    if( value->getEqualToThreshold() )
    {
      writeFieldValue("  Condition", OdString(OD_T(">=")) );
    }
    else
    {
      writeFieldValue("  Condition", OdString(OD_T(">")) );
    }
  }
  else
  {
    if( value->getEqualToThreshold() )
    {
      writeFieldValue("  Condition", OdString(OD_T("<=")) );
    }
    else
    {
      writeFieldValue("  Condition", OdString(OD_T("<")) );
    }
  }

  writeFieldValue("  Threshold", value->getThreshold() );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimOptionOffset::ChainType value )
{
  dumpFieldName(name);

  OdString strValue;

  switch( value )
  {
    case OdDgDimOptionOffset::kNone:
    {
      strValue = OD_T("kNone");
    } break;

    case OdDgDimOptionOffset::kLine:
    {
      strValue = OD_T("kLine");
    } break;

    case OdDgDimOptionOffset::kArc:
    {
      strValue = OD_T("kArc");
    } break;

    case OdDgDimOptionOffset::kBSpline:
    {
      strValue = OD_T("kBSpline");
    } break;
  }

  dumpFieldValue(strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimOptionOffset::LeaderAlignment value )
{
  dumpFieldName(name);

  OdString strValue;

  switch( value )
  {
    case OdDgDimOptionOffset::kAutoAlignment:
    {
      strValue = OD_T("kAutoAlignment");
    } break;

    case OdDgDimOptionOffset::kLeftAlignment:
    {
      strValue = OD_T("kLeftAlignment");
    } break;

    case OdDgDimOptionOffset::kRightAlignment:
    {
      strValue = OD_T("kRightAlignment");
    } break;
  }

  dumpFieldValue(strValue);
}


inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDgDimOptionOffsetPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kOffset")) );

  if( value.isNull() )
    return;

  writeFieldValue("  Terminator", value->getTerminator() );
  writeFieldValue("  Chain type", value->getChainType() );
  writeFieldValue("  Elbow", value->getElbowFlag() );
  writeFieldValue("  Alignment", value->getAlignment() );
  writeFieldValue("  No dock on dim line", value->getNoDockOnDimLineFlag() );
}

inline void OdExDgnDumper::writeFieldValue( const OdString&, OdDgDimOptionProxyCellPtr& value )
{
  writeFieldValue("  Type", OdString(OD_T("kProxyCell")) );

  if( value.isNull() )
    return;

  writeFieldValue("  Origin", value->getOrigin() );
  writeFieldValue("  Rotation Matrix", value->getRotScale() );
  writeFieldValueHex("  Check Sum", value->getCheckSum() );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgMultilineSymbology const& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldName( val );

  writeFieldValue( "Style", value.getLineStyleEntryId() );
  writeFieldValue( "Weight", value.getLineWeight() );
  writeFieldValue( "Color", value.getColorIndex() );
  writeFieldValue( "Use style", value.getUseStyleFlag() );
  writeFieldValue( "Use weight", value.getUseWeightFlag() );
  writeFieldValue( "Use color", value.getUseColorFlag() );
  writeFieldValue( "Use class", value.getUseClassFlag() );
  writeFieldValue( "Inside arc", value.getCapInArcFlag() );
  writeFieldValue( "Outside arc", value.getCapOutArcFlag() );
  writeFieldValue( "Cap line", value.getCapLineFlag() );
  writeFieldValue( "Custom style", value.getCustomStyleFlag() );
  writeFieldValue( "Cap color from segment", value.getCapColorFromSegmentFlag() );
  writeFieldValue( "Construction class", value.getConstructionClassFlag() );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgMultilinePoint const& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldName( val );

  {
    OdGePoint3d point;

    value.getPoint( point );
    writeFieldValue( "Point", point );
  }

  {
    OdUInt32            i, j = value.getBreaksCount();
    OdDgMultilineBreak  break_;
    char                fieldName[ 20 ];

    writeFieldValue( "Number of breaks", j );

    for( i = 0; i < j; i++ )
    {
      sprintf( fieldName, "Break %d", i );
      value.getBreak( i, break_ );
      writeFieldValue( fieldName, break_ );
    }
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTextInfo::FitOptions value )
{
  dumpFieldName( name );

  OdString strVal;

  switch( value )
  {
    case OdDgDimTextInfo::kTermMovesFirst: strVal = "kTermMovesFirst"; break;
    case OdDgDimTextInfo::kTermReversed: strVal = "kTermReversed"; break;
    case OdDgDimTextInfo::kTermInside: strVal = "kTermInside"; break;
    case OdDgDimTextInfo::kTermOutside: strVal = "kTermOutside"; break;
    case OdDgDimTextInfo::kTextInside: strVal = "kTextInside"; break;
    case OdDgDimTextInfo::kTextMovesFirst: strVal = "kTextMovesFirst"; break;
    case OdDgDimTextInfo::kBothMoves: strVal = "kBothMoves"; break;
    case OdDgDimTextInfo::kSmallestMoves: strVal = "kSmallestMoves"; break;
  }

  dumpFieldValue( strVal );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTextInfo::TextLocation value )
{
  dumpFieldName( name );

  OdString strVal;

  switch( value )
  {
    case OdDgDimTextInfo::kTextInline: strVal = "kTextInline"; break;
    case OdDgDimTextInfo::kTextAbove: strVal  = "kTextAbove"; break;
    case OdDgDimTextInfo::kTextOutside: strVal = "kTextOutside"; break;
    default: strVal = "kTextTopLeft"; break;
  }

  dumpFieldValue( strVal );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgMultilineBreak const& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldName( val );

  writeFieldValue( "Lines mask", value.getLinesMask() );
  writeFieldValue( "Offset", value.getOffset() );
  writeFieldValue( "Length", value.getLength() );

  {
    OdString flagValue;

    switch( value.getFlags() )
    {
    case OdDgMultilineBreak::kStandardByDistance  : flagValue = "Standard by distance"; break;
    case OdDgMultilineBreak::kFromJoint           : flagValue = "from joint"; break;
    case OdDgMultilineBreak::kToJoint             : flagValue = "to joing"; break;
    }

    writeFieldValue( "Flag", flagValue );
  }
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgMultilineProfile const& value )
{
  OdString val;
  val.format( L"%s:", name.c_str() );
  dumpFieldValue( val );

  writeFieldValue( "Distance", value.getDistance() );

  {
    OdDgMultilineSymbology symbology;

    value.getSymbology( symbology );
    writeFieldValue( "Symbology", symbology );
  }
}


//#pragma MARKMESSAGE("TODO: Examples. SurfaceType interface was modified.");
//
//inline void OdExDgnDumper::writeFieldValue( const OdString& name, Dgn8::SurfaceType value )
//{
//  dumpFieldName( name );
//
//  switch( value )
//  {
//  case Dgn8::SURF_PROJECTION     : val.format( LSurface (or solid) of projection" ); break;
//  case Dgn8::SOLID_REVOLUTION    : val.format( LSolid of revolution" ); break;
//  case Dgn8::SURF_REVOLUTION     : val.format( LSurface of revolution" ); break;
//  case Dgn8::SURF_FORCEDWORD     : val.format( LUnknown type" ); break;
//  }
//}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgTextExtendedProperties::LineSpacingType value )
{
  dumpFieldName( name );

  OdString val;
  switch( value )
  {
  case OdDgTextExtendedProperties::kExact      : val.format( L"Exact" ); break;
  case OdDgTextExtendedProperties::kAutomatic  : val.format( L"Automatic" ); break;
  case OdDgTextExtendedProperties::kFromLineTop: val.format( L"FromLineTop" ); break;
  case OdDgTextExtendedProperties::kAtLeast    : val.format( L"AtLeast" ); break;
  }
  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgRaster::RasterFormat value )
{
  dumpFieldName( name );

  OdString val;
  switch( value )
  {
  case OdDgRaster::kBitmap     : val.format( L"Bitmap raster" ); break;
  case OdDgRaster::kByteData  : val.format( L"Byte data raster" ); break;
  case OdDgRaster::kBinaryRLE : val.format( L"Binary RLE raster" ); break;
  case OdDgRaster::kByteRLE   : val.format( L"Byte RLE raster" ); break;
  }
  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgTagDefinition::Type value )
{
  dumpFieldName( name );

  OdString val;
  switch( value )
  {
  case OdDgTagDefinition::kChar    : val.format( L"Char" ); break;
  case OdDgTagDefinition::kInt16    : val.format( L"Short int" ); break;
  case OdDgTagDefinition::kInt32    : val.format( L"Long int" ); break;
  case OdDgTagDefinition::kDouble  : val.format( L"Double" ); break;
  case OdDgTagDefinition::kBinary  : val.format( L"Binary" ); break;
  }
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDg::TextDirection value )
{
  dumpFieldName( name );

  OdString val;
  switch( value )
  {
  case OdDg::kHorizontal  : val.format( L"Horizontal" ); break;
  case OdDg::kVertical    : val.format( L"Vertical" ); break;
  case OdDg::kJapanese    : val.format( L"Japanese" ); break;
  case OdDg::kRightToLeft : val.format( L"Right-to-left" ); break;
  }
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdGeMatrix3d const& value )
{
  dumpFieldName( name );

  OdString val;
  val.format( L"%g; %g; %g; %g; %g; %g; %g; %g; %g",
    value( 0, 0 ), value( 1, 0 ), value( 2, 0 ), 
    value( 0, 1 ), value( 1, 1 ), value( 2, 1 ), 
    value( 0, 2 ), value( 1, 2 ), value( 2, 2 ) );
  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdGsDCRect const& value )
{
  dumpFieldName( name );

  OdString val;
  val.format( L"( 0x%X; 0x%X ) - ( 0x%X; 0x%X )\n", value.m_min.x, value.m_min.y, value.m_max.x, value.m_max.y );

  dumpFieldValue( val );
}


inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgElementId const& value )
{
  writeFieldValue(name, value.getHandle().ascii());
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, const OdVariant& val )
{
  switch( val.type() )
  {
    case OdVariant::kBool:
    {
      if( val.isArray() )
        writeFieldValue( name, val.getBoolArray() );
      else
        writeFieldValue( name, val.getBool() );
    }
    break;

    case OdVariant::kInt8:
    {
      if( val.isArray() )
        writeFieldValue( name, val.getInt8Array() );
      else
        writeFieldValue( name, val.getInt8() );
    }
    break;

    case OdVariant::kInt16:
    {
      if( val.isArray() )
        writeFieldValue( name, val.getInt16Array() );
      else
        writeFieldValue( name, val.getInt16() );
    }
    break;

    case OdVariant::kInt32:
    {
      if( val.isArray() )
        writeFieldValue( name, val.getInt32Array() );
      else
        writeFieldValue( name, val.getInt32() );
    }
    break;

    case OdVariant::kInt64:
    {
      if( val.isArray() )
        writeFieldValue( name, val.getInt64Array() );
      else
        writeFieldValue( name, val.getInt64() );
    }
    break;

    case OdVariant::kDouble:
    {
      if( val.isArray() )
        writeFieldValue( name, val.getDoubleArray() );
      else
        writeFieldValue( name, val.getDouble() );
    }
    break;

    case OdVariant::kString:
    {
      if( val.isArray() )
        writeFieldValue( name, val.getStringArray() );
      else
        writeFieldValue( name, val.getString() );
    }
    break;

    case OdVariant::kAnsiString:
    {
      if( val.isArray() )
        writeFieldValue( name, val.getAnsiStringArray() );
      else
        writeFieldValue( name, OdString(val.getAnsiString()) );
    }
    break;
  }
}

inline void OdExDgnDumper::writeFieldValue( OdString const& name, OdGeExtents2d const& value )
{
  dumpFieldName( name );

  OdGePoint2d min = value.minPoint(), max = value.maxPoint();

  OdString val;
  val.format( L"Low point: %g; %g;  High point: %g; %g",
    min.x, min.y,
    max.x, max.y
    );
  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( OdString const& name, OdGeExtents3d const& value )
{
  dumpFieldName( name );

  OdGePoint3d min = value.minPoint(), max = value.maxPoint();

  OdString val;
  val.format( L"Low point: %g; %g; %g;  High point: %g; %g; %g",
    min.x, min.y, min.z,
    max.x, max.y, max.z
    );
  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDg::LineStyleType value )
{
  dumpFieldName( name );

  OdString val;
  switch ( value )
  {
  case OdDg::kLsTypePointSymbol:
    val = L"kPointSymbol";
    break;
  case OdDg::kLsTypeCompound:
    val = L"kCompound";
    break;
  case OdDg::kLsTypeLineCode:
    val = L"kLineCode";
    break;
  case OdDg::kLsTypeLinePoint:
    val = L"kLinePoint";
    break;
  }

  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDg::LineStyleUnitsType value )
{
  dumpFieldName( name );

  OdString val;
  switch ( value )
  {
  case OdDg::kLsMasterUnits:
    val = L"kMasterUnits";
    break;
  case OdDg::kLsUORS:
    val = L"kUORs";
    break;
  case OdDg::kLsDeviceUnits:
    val = L"kDeviceUnits";
    break;
  }

  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLineStyleResource::OdLsResourceType value )
{
  dumpFieldName( name );

  OdString val;
  switch ( value )
  {
  case OdDgLineStyleResource::kLsUnknownRes:
    val = L"kUnknownRes";
    break;
  case OdDgLineStyleResource::kLsPointSymbolResV7:
    val = L"kPointSymbolV7Res";
    break;
  case OdDgLineStyleResource::kLsPointSymbolRes:
    val = L"kPointSymbolRes";
    break;
  case OdDgLineStyleResource::kLsCompoundRes:
    val = L"kCompoundRes";
    break;
  case OdDgLineStyleResource::kLsLineCodeRes:
    val = L"kLineCodeRes";
    break;
  case OdDgLineStyleResource::kLsLinePointRes:
    val = L"kLinePointRes";
    break;  
  case OdDgLineStyleResource::kLsInternalRes:
    val = L"kInternalRes";
    break;
  }

  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLineCodeResourceStrokeData::OdLsStrokeWidthMode value )
{
  dumpFieldName( name );

  OdString val;
  switch ( value )
  {
  case OdDgLineCodeResourceStrokeData::kLsNoWidth:
    val = L"kNoWidth";
    break;
  case OdDgLineCodeResourceStrokeData::kLsWidthLeft:
    val = L"kLeftWidth";
    break;
  case OdDgLineCodeResourceStrokeData::kLsWidthRight:
    val = L"kRightWidth";
    break;
  case OdDgLineCodeResourceStrokeData::kLsWidthFull:
    val = L"kFullWidth";
    break;
  }

  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLineCodeResourceStrokeData::OdLsStrokeCapsType value )
{
  dumpFieldName( name );

  OdString val;
  switch ( value )
  {
  case OdDgLineCodeResourceStrokeData::kLsCapsClosed:
    val = L"kCapsClosed";
    break;
  case OdDgLineCodeResourceStrokeData::kLsCapsOpen:
    val = L"kCapsOpen";
    break;
  case OdDgLineCodeResourceStrokeData::kLsCapsExtended:
    val = L"kCapsExtended";
    break;
  case OdDgLineCodeResourceStrokeData::kLsCapsHexagon:
    val = L"kCapsHexagon";
    break;
  case OdDgLineCodeResourceStrokeData::kLsCapsOctagon:
    val = L"kCapsOctagon";
    break;
  case OdDgLineCodeResourceStrokeData::kLsCapsDecagon:
    val = L"kCapsDecagon";
    break;
  case OdDgLineCodeResourceStrokeData::kLsCapsArc:
    val = L"kCapsArc";
    break;
  }

  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLinePointResourceSymInfo::OdLsSymbolPosOnStroke value )
{
  dumpFieldName( name );

  OdString val;
  switch ( value )
  {
  case OdDgLinePointResourceSymInfo::kLsNoSymbol:
    val = L"kNoSymbol";
    break;
  case OdDgLinePointResourceSymInfo::kLsAtOriginOfStroke:
    val = L"kAtOriginOfStroke";
    break;
  case OdDgLinePointResourceSymInfo::kLsAtEndOfStroke:
    val = L"kAtEndOfStroke";
    break;
  case OdDgLinePointResourceSymInfo::kLsAtCenterOfStroke:
    val = L"kAtCenterOfStroke";
    break;
  }

  dumpFieldValue( val );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTool::TerminatorType iType )
{
  switch( iType )
  {
    case OdDgDimTool::kTtNone:
    {
      writeFieldValue(name, OdString(OD_T("kTtNone")) );
    } break;

    case OdDgDimTool::kTtArrow:
    {
      writeFieldValue(name, OdString(OD_T("kTtArrow")) );
    } break;

    case OdDgDimTool::kTtStroke:
    {
      writeFieldValue(name, OdString(OD_T("kTtStroke")) );
    } break;

    case OdDgDimTool::kTtCircle:
    {
      writeFieldValue(name, OdString(OD_T("kTtCircle")) );
    } break;

    case OdDgDimTool::kTtFilledCircle:
    {
      writeFieldValue(name, OdString(OD_T("kTtFilledCircle")) );
    } break;
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTool::TextType iType )
{
  switch( iType )
  {
    case OdDgDimTool::kStandard:
    {
      writeFieldValue(name, OdString(OD_T("kStandard")) );
    } break;

    case OdDgDimTool::kVertical:
    {
      writeFieldValue(name, OdString(OD_T("kVertical")) );
    } break;

    case OdDgDimTool::kMixed:
    {
      writeFieldValue(name, OdString(OD_T("kMixed")) );
    } break;
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTool::CustomSymbol iSymbol )
{
  switch( iSymbol )
  {
    case OdDgDimTool::kCsNone:
    {
      writeFieldValue(name, OdString(OD_T("kCsNone")) );
    } break;

    case OdDgDimTool::kCsDiameter:
    {
      writeFieldValue(name, OdString(OD_T("kCsDiameter")) );
    } break;

    case OdDgDimTool::kCsRadius:
    {
      writeFieldValue(name, OdString(OD_T("kCsRadius")) );
    } break;

    case OdDgDimTool::kCsSquare:
    {
      writeFieldValue(name, OdString(OD_T("kCsSquare")) );
    } break;

    case OdDgDimTool::kCsSR:
    {
      writeFieldValue(name, OdString(OD_T("kCsSR")) );
    } break;

    case OdDgDimTool::kCsSDiameter:
    {
      writeFieldValue(name, OdString(OD_T("kCsSDiameter")) );
    } break;
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTool::Leader iLeader )
{
  switch( iLeader )
  {
    case OdDgDimTool::kRadius:
    {
      writeFieldValue(name, OdString(OD_T("kRadius")) );
    } break;

    case OdDgDimTool::kRadiusExt1:
    {
      writeFieldValue(name, OdString(OD_T("kRadiusExt1")) );
    } break;

    case OdDgDimTool::kRadiusExt2:
    {
      writeFieldValue(name, OdString(OD_T("kRadiusExt2")) );
    } break;

    case OdDgDimTool::kDiameter:
    {
      writeFieldValue(name, OdString(OD_T("kDiameter")) );
    } break;
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimensionPtr pElement )
{
  switch( pElement->getDimensionType() )
  {
    case OdDgDimension::kToolTypeSizeArrow:
    {
      OdDgDimSizeArrowPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeSizeStroke:
    {
      OdDgDimSizeStrokePtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeLocateSingle:
    {
      OdDgDimSingleLocationPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeLocateStacked:
    {
      OdDgDimStackedLocationPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeCustomLinear:
    {
      OdDgDimCustomLinearPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleSize:
    {
      OdDgDimAngleSizePtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleLines:
    {
      OdDgDimAngleLinesPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleLocation:
    {
      OdDgDimAngleLocationPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeArcLocation:
    {
      OdDgDimArcLocationPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleAxisX:
    {
      OdDgDimAngleAxisXPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeAngleAxisY:
    {
      OdDgDimAngleAxisYPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeArcSize:
    {
      OdDgDimArcSizePtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeRadius:
    {
      OdDgDimRadiusPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeRadiusExtended:
    {
      OdDgDimRadiusExtendedPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeDiameter:
    {
      OdDgDimDiameterPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeDiameterExtended:
    {
      OdDgDimDiameterExtendedPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeDiameterPara:
    {
      OdDgDimDiameterParallelPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeDiameterPerp:
    {
      OdDgDimDiameterPerpendicularPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeOrdinate:
    {
      OdDgDimOrdinatePtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;

    case OdDgDimension::kToolTypeCenter:
    {
      OdDgDimCenterPtr pDimSize = pElement;

      if( !pDimSize.isNull() )
        writeFieldValue( name, pDimSize );
    } break;
  }
}

#undef  OUT_LINEAR_DIM_TOOL_FIELDS
#define OUT_LINEAR_DIM_TOOL_FIELDS( type ) \
inline void OdExDgnDumper::writeFieldValue( const OdString& name, type pElement ) \
{ \
  dumpFieldName( name ); \
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );  \
  writeFieldValue("  LeftExtLinesPresentFlag", pElement->getLeftExtLinesPresentFlag() );  \
  writeFieldValue("  RightExtLinesPresentFlag", pElement->getRightExtLinesPresentFlag() );  \
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );  \
  writeFieldValue("  FirstTerminator", pElement->getFirstTerminator() );  \
  writeFieldValue("  LeftTerminator", pElement->getLeftTerminator() );  \
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() );  \
  writeFieldValue("  JointTerminator", pElement->getJointTerminator() );  \
  writeFieldValue("  TextType", pElement->getTextType() );  \
  writeFieldValue("  Prefix", pElement->getPrefix() );  \
  writeFieldValue("  Suffix", pElement->getSuffix() );  \
}

OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimSizeArrowPtr )
OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimSizeStrokePtr )
OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimSingleLocationPtr )
OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimStackedLocationPtr )
OUT_LINEAR_DIM_TOOL_FIELDS( OdDgDimCustomLinearPtr )

#undef  OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS
#define OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( type ) \
inline void OdExDgnDumper::writeFieldValue( const OdString& name, type pElement ) \
{ \
  dumpFieldName( name ); \
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );  \
  writeFieldValue("  LeftExtLinesPresentFlag", pElement->getLeftExtLinesPresentFlag() );  \
  writeFieldValue("  RightExtLinesPresentFlag", pElement->getRightExtLinesPresentFlag() ); \
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );  \
  writeFieldValue("  LeftTerminator", pElement->getLeftTerminator() ); \
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() ); \
  writeFieldValue("  Prefix", pElement->getPrefix() );  \
  writeFieldValue("  Suffix", pElement->getSuffix() );  \
}

OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( OdDgDimAngleSizePtr )
OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( OdDgDimAngleLinesPtr )
OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( OdDgDimDiameterParallelPtr )
OUT_ANGULAR_SHORT_DIM_TOOL_FIELDS( OdDgDimDiameterPerpendicularPtr )

#undef  OUT_ANGULAR_DIM_TOOL_FIELDS
#define OUT_ANGULAR_DIM_TOOL_FIELDS( type ) \
inline void OdExDgnDumper::writeFieldValue( const OdString& name, type pElement ) \
{ \
  dumpFieldName( name ); \
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );  \
  writeFieldValue("  LeftExtLinesPresentFlag", pElement->getLeftExtLinesPresentFlag() );  \
  writeFieldValue("  RightExtLinesPresentFlag", pElement->getRightExtLinesPresentFlag() ); \
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );  \
  writeFieldValue("  FirstTerminator", pElement->getFirstTerminator() );  \
  writeFieldValue("  LeftTerminator", pElement->getLeftTerminator() ); \
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() ); \
  writeFieldValue("  Prefix", pElement->getPrefix() );  \
  writeFieldValue("  Suffix", pElement->getSuffix() );  \
}

OUT_ANGULAR_DIM_TOOL_FIELDS( OdDgDimAngleLocationPtr )
OUT_ANGULAR_DIM_TOOL_FIELDS( OdDgDimArcLocationPtr )
OUT_ANGULAR_DIM_TOOL_FIELDS( OdDgDimAngleAxisXPtr )
OUT_ANGULAR_DIM_TOOL_FIELDS( OdDgDimAngleAxisYPtr )

#undef  OUT_RADIAL_DIM_TOOL_FIELDS
#define OUT_RADIAL_DIM_TOOL_FIELDS( type ) \
  inline void OdExDgnDumper::writeFieldValue( const OdString& name, type pElement ) \
{ \
  dumpFieldName( name ); \
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );  \
  writeFieldValue("  CenterMarkFlag", pElement->getCenterMarkFlag() );  \
  writeFieldValue("  CenterMarkLeftExtendFlag", pElement->getCenterMarkLeftExtendFlag() );  \
  writeFieldValue("  CenterMarkRightExtendFlag", pElement->getCenterMarkRightExtendFlag() );  \
  writeFieldValue("  CenterMarkTopExtendFlag", pElement->getCenterMarkTopExtendFlag() );  \
  writeFieldValue("  CenterMarkBottomExtendFlag", pElement->getCenterMarkBottomExtendFlag() );  \
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );  \
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() ); \
  writeFieldValue("  Prefix", pElement->getPrefix() );  \
  writeFieldValue("  Suffix", pElement->getSuffix() );  \
  writeFieldValue("  Leader", pElement->getLeader() );  \
}

OUT_RADIAL_DIM_TOOL_FIELDS( OdDgDimRadiusPtr )
OUT_RADIAL_DIM_TOOL_FIELDS( OdDgDimRadiusExtendedPtr )
OUT_RADIAL_DIM_TOOL_FIELDS( OdDgDimDiameterPtr )
OUT_RADIAL_DIM_TOOL_FIELDS( OdDgDimDiameterExtendedPtr )

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimArcSizePtr pElement ) 
{ 
  dumpFieldName( name );
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );
  writeFieldValue("  LeftExtLinesPresentFlag", pElement->getLeftExtLinesPresentFlag() );
  writeFieldValue("  RightExtLinesPresentFlag", pElement->getRightExtLinesPresentFlag() );
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );
  writeFieldValue("  ChordAlignFlag", pElement->getChordAlignFlag() );
  writeFieldValue("  LeftTerminator", pElement->getLeftTerminator() );
  writeFieldValue("  RightTerminator", pElement->getRightTerminator() );
  writeFieldValue("  Prefix", pElement->getPrefix() );
  writeFieldValue("  Suffix", pElement->getSuffix() );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimOrdinatePtr pElement ) 
{ 
  dumpFieldName( name ); 
  writeFieldValue("  StackExtLinesFlag", pElement->getStackExtLinesFlag() );   
  writeFieldValue("  ArcSymbolFlag", pElement->getArcSymbolFlag() );     
  writeFieldValue("  DecrementInReverseDirectionFlag", pElement->getDecrementInReverseDirectionFlag() );  
  writeFieldValue("  FreeLocationOfTxtFlag", pElement->getFreeLocationOfTxtFlag() );    
  writeFieldValue("  Datum value", pElement->getDatumValue() ); 
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimCenterPtr pElement ) 
{ 
  dumpFieldName( name );
  writeFieldValue("  CenterMarkLeftExtendFlag", pElement->getCenterMarkLeftExtendFlag() );
  writeFieldValue("  CenterMarkRightExtendFlag", pElement->getCenterMarkRightExtendFlag() );
  writeFieldValue("  CenterMarkTopExtendFlag", pElement->getCenterMarkTopExtendFlag() );
  writeFieldValue("  CenterMarkBottomExtendFlag", pElement->getCenterMarkBottomExtendFlag() );
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimension::PlacementAlignment value )
{
  OdString strAlign;

  switch( value )
  {
    case OdDgDimension::kPaView:
    {
      strAlign = OD_T("kPaView");
    } 
    break;

    case OdDgDimension::kPaDrawing:
    {
      strAlign = OD_T("kPaDrawing");
    } 
    break;

    case OdDgDimension::kPaTrue:
    {
      strAlign = OD_T("kPaTrue");
    } 
    break;

    case OdDgDimension::kPaArbitrary:
    {
      strAlign = OD_T("kPaArbitrary");
    } 
    break;
  }

  writeFieldValue(name, strAlign);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDimTextFormat::LabelDisplayMode value )
{
  OdString strAlign;

  switch( value )
  {
    case OdDgDimTextFormat::kMu:
    {
      strAlign = OD_T("Mu");
    } 
    break;

    case OdDgDimTextFormat::kMuLabel:
    {
      strAlign = OD_T("Mu Label");
    } 
    break;

    case OdDgDimTextFormat::kSu:
    {
      strAlign = OD_T("Su");
    } 
    break;

    case OdDgDimTextFormat::kSuLabel:
    {
      strAlign = OD_T("Su Label");
    } 
    break;

    case OdDgDimTextFormat::kMu_Su:
    {
      strAlign = OD_T("Mu-Su");
    } 
    break;

    case OdDgDimTextFormat::kMuLabelSuLabel:
    {
      strAlign = OD_T("Mu Label Su Label");
    } 
    break;

    case OdDgDimTextFormat::kMuLabel_SuLabel:
    {
      strAlign = OD_T("Mu Label-Su Label");
    } 
    break;
  }

  writeFieldValue(name, strAlign);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLevelFilterTable::OdDgFilterMemberType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgLevelFilterTable::kMemberTypeShort :
    {
      strValue = OD_T("kTypeShort");
    } break;

    case OdDgLevelFilterTable::kMemberTypeInt :
    {
      strValue = OD_T("kTypeInt");
    } break;

    case OdDgLevelFilterTable::kMemberTypeBool :
    {
      strValue = OD_T("kTypeBool");
    } break;

    case OdDgLevelFilterTable::kMemberTypeChar :
    {
      strValue = OD_T("kTypeChar");
    } break;

    case OdDgLevelFilterTable::kMemberTypeWChar :
    {
      strValue = OD_T("kTypeWChar");
    } break;

    case OdDgLevelFilterTable::kMemberTypeDouble :
    {
      strValue = OD_T("kTypeDouble");
    } break;

    case OdDgLevelFilterTable::kMemberTypeTime :
    {
      strValue = OD_T("kTypeTime");
    } break;

    default:
    {
      strValue = OD_T("kTypeNull");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgGradientFill::OdDgGradientType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgGradientFill::kCurved :
    {
      strValue = OD_T("kCurved");
    } break;

    case OdDgGradientFill::kCylindrical :
    {
      strValue = OD_T("kCylindrical");
    } break;

    case OdDgGradientFill::kSpherical :
    {
      strValue = OD_T("kSpherical");
    } break;

    case OdDgGradientFill::kHemispherical :
    {
      strValue = OD_T("kHemispherical");
    } break;

    default:
    {
      strValue = OD_T("kLinear");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLineStyleModificationLinkage::OdDgLsModWidthMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgLineStyleModificationLinkage::kLsModConstantWidth :
    {
      strValue = OD_T("kConstantWidth");
    } break;

    case OdDgLineStyleModificationLinkage::kLsModTaperedWidth :
    {
      strValue = OD_T("kTaperedWidth");
    } break;

    default:
    {
      strValue = OD_T("kNoWidth");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLineStyleModificationLinkage::OdDgLsModShiftMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgLineStyleModificationLinkage::kLsModCentered :
    {
      strValue = OD_T("kCentered");
    } break;

    case OdDgLineStyleModificationLinkage::kLsModDistance :
    {
      strValue = OD_T("kDistance");
    } break;

    case OdDgLineStyleModificationLinkage::kLsModFraction :
    {
      strValue = OD_T("kFraction");
    } break;

    default:
    {
      strValue = OD_T("kNoShift");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLineStyleModificationLinkage::OdDgLsModCornerMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgLineStyleModificationLinkage::kLsModBreakAtCorners :
    {
      strValue = OD_T("kBreakAtCorners");
    } break;

    case OdDgLineStyleModificationLinkage::kLsModRunThroughCorners :
    {
      strValue = OD_T("kRunThroughCorners");
    } break;

    default:
    {
      strValue = OD_T("kFromLineStyle");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgInfiniteLineLinkage::InfiniteLineType type )
{
  OdString strValue;

  switch( type )
  {
    case OdDgInfiniteLineLinkage::kFromStartPoint :
    {
      strValue = OD_T("kFromStartPoint");
    } break;

    case OdDgInfiniteLineLinkage::kFromEndPoint :
    {
      strValue = OD_T("kFromEndPoint");
    } break;

    case OdDgInfiniteLineLinkage::kInfiniteLine :
    {
      strValue = OD_T("kInfiniteLine");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgDependencyLinkage::RootDataType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgDependencyLinkage::kElementId :
    {
      strValue = OD_T("kElementId");
    } break;

    case OdDgDependencyLinkage::kElementId_V :
    {
      strValue = OD_T("kElementId V");
    } break;

    case OdDgDependencyLinkage::kAssocPoint :
    {
      strValue = OD_T("kAssocPoint");
    } break;

    case OdDgDependencyLinkage::kAssocPoint_I :
    {
      strValue = OD_T("kAssocPoint I");
    } break;

    case OdDgDependencyLinkage::kFarElementId :
    {
      strValue = OD_T("kFarElementId");
    } break;

    case OdDgDependencyLinkage::kFarElementId_V :
    {
      strValue = OD_T("kFarElementId V");
    } break;

    case OdDgDependencyLinkage::kPath_V :
    {
      strValue = OD_T("kPath V");
    } break;

    default:
    {
      strValue = OD_T("kUnknownType");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgAssocPointRoot::OdDgAssocPointRootType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgAssocPointRoot::kLinearAssociation :
    {
      strValue = OD_T("kLinearAssociation");
    } break;

    case OdDgAssocPointRoot::kIntersectAssociation :
    {
      strValue = OD_T("kIntersectAssociation");
    } break;

    case OdDgAssocPointRoot::kArcAssociation :
    {
      strValue = OD_T("kArcAssociation");
    } break;

    case OdDgAssocPointRoot::kMultilineAssociation :
    {
      strValue = OD_T("kMultilineAssociation");
    } break;

    case OdDgAssocPointRoot::kBSplineCurveAssociation :
    {
      strValue = OD_T("kBSplineCurveAssociation");
    } break;

    case OdDgAssocPointRoot::kProjectionAssociation :
    {
      strValue = OD_T("kProjectionAssociation");
    } break;

    case OdDgAssocPointRoot::kOriginAssociation :
    {
      strValue = OD_T("kOriginAssociation");
    } break;

    case OdDgAssocPointRoot::kIntersect2Association :
    {
      strValue = OD_T("kIntersect2Association");
    } break;

    case OdDgAssocPointRoot::kMeshVertexAssociation :
    {
      strValue = OD_T("kMeshVertexAssociation");
    } break;

    case OdDgAssocPointRoot::kMeshEdgeAssociation :
    {
      strValue = OD_T("kMeshEdgeAssociation");
    } break;

    case OdDgAssocPointRoot::kBSplineSurfaceAssociation :
    {
      strValue = OD_T("kBSplineSurfaceAssociation");
    } break;

    default:
    {
      strValue = OD_T("kUnknownAssociation");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgArcAssociation::OdDgArcAssociationKeyPoint value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgArcAssociation::kCenterPoint :
    {
      strValue = OD_T("kCenterPoint");
    } break;

    case OdDgArcAssociation::kStartPoint :
    {
      strValue = OD_T("kStartPoint");
    } break;

    case OdDgArcAssociation::kEndPoint :
    {
      strValue = OD_T("kEndPoint");
    } break;

    default:
    {
      strValue = OD_T("kAnglePoint");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgOriginAssociation::OdDgOriginAssocOption value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgOriginAssociation::kInsertionPoint :
    {
      strValue = OD_T("kInsertionPoint");
    } break;

    default:
    {
      strValue = OD_T("kUpperLeftPoint");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgLineStyleModificationLinkage::OdDgLsMultilineDataType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgLineStyleModificationLinkage::kLsMultilineTypeLine :
    {
      strValue = OD_T("kProfileLine");
    } break;

    case OdDgLineStyleModificationLinkage::kLsMultilineTypeStartCap :
    {
      strValue = OD_T("kStartCap");
    } break;

    case OdDgLineStyleModificationLinkage::kLsMultilineTypeEndCap :
    {
      strValue = OD_T("kEndCap");
    } break;

    default:
    {
      strValue = OD_T("kJoints");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgRasterFrame::OdDgGeoPriority value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgRasterFrame::kAttachment :
    {
      strValue = OD_T("kAttachment");
    } break;

    case OdDgRasterFrame::kRasterHeader :
    {
      strValue = OD_T("kRasterHeader");
    } break;

    case OdDgRasterFrame::kSisterFile :
    {
      strValue = OD_T("kSisterFile");
    } break;

    default:
    {
      strValue = OD_T("kFromPreferences");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgRasterFrame::OdDgRasterDrawingPlane value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgRasterFrame::kBackgroundPlane :
    {
      strValue = OD_T("kBackgroundPlane");
    } break;

    case OdDgRasterFrame::kForegroundPlane :
    {
      strValue = OD_T("kForegroundPlane");
    } break;

    case OdDgRasterFrame::kDesignPlane :
    {
      strValue = OD_T("kDesignPlane");
    } break;

    default:
    {
      strValue = OD_T("kUnknownPlane");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgRasterFrame::OdDgRasterColorMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgRasterFrame::k2ColorsMode :
    {
      strValue = OD_T("k2ColorsMode");
    } break;

    case OdDgRasterFrame::kMonochromeMode :
    {
      strValue = OD_T("kMonochromeMode");
    } break;

    case OdDgRasterFrame::k16ColorsMode :
    {
      strValue = OD_T("k16ColorsMode");
    } break;

    case OdDgRasterFrame::k256ColorsMode :
    {
      strValue = OD_T("k256ColorsMode");
    } break;

    case OdDgRasterFrame::kGrayscaleMode :
    {
      strValue = OD_T("kGrayscaleMode");
    } break;

    case OdDgRasterFrame::kRGBMode :
    {
      strValue = OD_T("kRGBMode");
    } break;

    case OdDgRasterFrame::kRGBAMode :
    {
      strValue = OD_T("kRGBAMode");
    } break;

    default:
    {
      strValue = OD_T("kUnknownMode");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgOle2Frame::OdDgDrawAspect value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgOle2Frame::kContent:
    {
      strValue = OD_T("kContent");
    } break;

    case OdDgOle2Frame::kThumbnail:
    {
      strValue = OD_T("kThumbnail");
    } break;

    case OdDgOle2Frame::kIcon :
    {
      strValue = OD_T("kIcon");
    } break;

    default:
    {
      strValue = OD_T("kDocPrint");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgOle2Frame::OdDgOleObjectType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgOle2Frame::kLinked:
    {
      strValue = OD_T("kLinked");
    } break;

    case OdDgOle2Frame::kEmbedded:
    {
      strValue = OD_T("kEmbedded");
    } break;

    case OdDgOle2Frame::kPicture :
    {
      strValue = OD_T("kPicture");
    } break;

    default:
    {
      strValue = OD_T("kUnknown");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgOle2Frame::OdDgOleViewRotationMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgOle2Frame::kViewIndependent:
    {
      strValue = OD_T("kViewIndependent");
    } break;

    case OdDgOle2Frame::kViewProjection:
    {
      strValue = OD_T("kViewProjection");
    } break;

    default:
    {
      strValue = OD_T("kViewDependent");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgNamedGroupItem::OdDgNamedGroupPropagateType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgNamedGroupItem::kApplyNever:
    {
      strValue = OD_T("kApplyNever");
    } break;

    case OdDgNamedGroupItem::kApplyAlways:
    {
      strValue = OD_T("kApplyAlways");
    } break;

    default:
    {
      strValue = OD_T("kApplyIfGroupLockOn");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgRasterAttachmentHeader::OdDgRasterAttachmentPlane value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgRasterAttachmentHeader::kBackground:
    {
      strValue = OD_T("kBackground");
    } break;

    case OdDgRasterAttachmentHeader::kDesignPlane:
    {
      strValue = OD_T("kDesignPlane");
    } break;

    case OdDgRasterAttachmentHeader::kForeground:
    {
      strValue = OD_T("kForeground");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgTemplateValueVariant value )
{
  OdString strValue;

  switch(  value.getDataType() )
  {
    case OdDgTemplateValueVariant::kBoolean :
    {
      if( value.getBoolValue() )
      {
        strValue = L"Bool: true";
      }
      else
      {
        strValue = L"Bool: false";
      }
    }
    break;

    case OdDgTemplateValueVariant::kByte :
    {
      strValue.format(L"Byte: %d", value.getByteValue() );
    }
    break;

    case OdDgTemplateValueVariant::kShort :
    {
      strValue.format(L"Short: %d", value.getShortValue() );
    }
    break;

    case OdDgTemplateValueVariant::kDouble :
    {
      strValue.format(L"Double: %f", value.getDoubleValue() );
    }
    break;

    case OdDgTemplateValueVariant::kPoint3d :
    {
      strValue.format(L"Point3d: %f, %f, %f", value.getPoint3dValue().x, value.getPoint3dValue().y, value.getPoint3dValue().z);
    }
    break;

    case OdDgTemplateValueVariant::kString :
    {
      if( value.getStringValue().isEmpty() )
      {
        strValue = L"String: \"(none)\"";
      }
      else
      {
        strValue = L"String: \"" + value.getStringValue() + "\"";
      }
    }
    break;

    case OdDgTemplateValueVariant::kColor :
    {
      OdDgTemplateColorValue clr = value.getColorValue();

      switch( clr.getColorType() )
      {
        case OdDgTemplateColorValue::kColorByLevel:
        {
          strValue = L"Color: kColorByLevel";
        } break;

        case OdDgTemplateColorValue::kColorByCell:
        {
          strValue = L"Color: kColorByCell";
        } break;

        case OdDgTemplateColorValue::kColorBook:
        {
          strValue.format( L"( %d, %d, %d )", ODGETRED(clr.getColorRGB()),ODGETGREEN(clr.getColorRGB()), ODGETBLUE(clr.getColorRGB()));

          strValue = L"Color From Book: " + clr.getBookAndColorName() + L" " + strValue;
        } break;

        case OdDgTemplateColorValue::kColorRGB:
        {
          strValue.format( L"Color RGB: ( %d, %d, %d )", ODGETRED(clr.getColorRGB()),ODGETGREEN(clr.getColorRGB()), ODGETBLUE(clr.getColorRGB()));
        } break;

        case OdDgTemplateColorValue::kColorIndex:
        {
          strValue.format( L"Color Index: %d", clr.getColorIndex() );
        } break;
      }
    }
    break;

    default:
    {
      strValue = L"Type is not defined.";
    }
    break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgModel::OdDgGridOrientationType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgModel::kACS:
    {
      strValue = OD_T("kACS");
    } break;

    case OdDgModel::kTop:
    {
      strValue = OD_T("kTop");
    } break;

    case OdDgModel::kFront:
    {
      strValue = OD_T("kFront");
    } break;

    case OdDgModel::kRight:
    {
      strValue = OD_T("kRight");
    } break;

    default:
    {
      strValue = OD_T("kView");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgModel::AcsType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgModel::kCylindrical:
    {
      strValue = OD_T("kCylindrical");
    } break;

    case OdDgModel::kSpherical:
    {
      strValue = OD_T("kSpherical");
    } break;

    default:
    {
      strValue = OD_T("kRectangular");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDg::OdDgMaterialType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDg::kLevelOverride:
    {
      strValue = OD_T("kLevelOverride");
    } break;

    case OdDg::kByLevelAssigned:
    {
      strValue = OD_T("kByLevelAssigned");
    } break;

    default:
    {
      strValue = OD_T("kElementAssugned");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgLinkNode::OdDgLinkNodeType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgLinkNode::kFolderLink:
    {
      strValue = OD_T("kFolderLink");
    } break;

    case OdDgLinkNode::kFileLink:
    {
      strValue = OD_T("kFileLink");
    } break;

    case OdDgLinkNode::kModelLink:
    {
      strValue = OD_T("kModelLink");
    } break;

    case OdDgLinkNode::kRegionLink:
    {
      strValue = OD_T("kRegionLink");
    } break;

    case OdDgLinkNode::kPDFRegionLink:
    {
      strValue = OD_T("kPdfRegionLink");
    } break;

    case OdDgLinkNode::kExcelSheetLink:
    {
      strValue = OD_T("kExcelSheetLink");
    } break;

    case OdDgLinkNode::kWordHeadingLink:
    {
      strValue = OD_T("kWordHeadingLink");
    } break;

    case OdDgLinkNode::kWordRegionLink:
    {
      strValue = OD_T("kWordRegionLink");
    } break;

    case OdDgLinkNode::kURLLink:
    {
      strValue = OD_T("kUrlLink");
    } break;

    default:
    {
      strValue = OD_T("kUnknownLink");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgLinkNodePtr value )
{
  switch( value->getType() )
  {
    case OdDgLinkNode::kFolderLink:
    {
      OdString strOffset;

      for( OdInt32 i = 0; i < name.getLength(); i++ )
      {
        strOffset += L" ";
      }

      if( name.getLength() > 0 && name[0] == L'+' )
      {
        dumpFieldName( L"---------------------" );
      }

      writeFieldValue( strOffset + L"Name ", value->getName() );
      writeFieldValue( strOffset + L"Type ", value->getType() );

      if( name.getLength() > 0 && name[0] == L'+' )
      {
        dumpFieldName( L"---------------------" );
      }

      OdDgFolderLinkNode* pLink = dynamic_cast<OdDgFolderLinkNode*>(value.get());

      for( OdUInt32 i = 0; i < pLink->getChildCount(); i++ )
      {
        writeFieldValue( name + L"  ", pLink->getChild(i) );
      }
    } break;

    case OdDgLinkNode::kFileLink:
    {
      OdDgFileLinkNode* pLink = dynamic_cast<OdDgFileLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;

    case OdDgLinkNode::kModelLink:
    {
      OdDgModelLinkNode* pLink = dynamic_cast<OdDgModelLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;

    case OdDgLinkNode::kRegionLink:
    {
      OdDgRegionLinkNode* pLink = dynamic_cast<OdDgRegionLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;

    case OdDgLinkNode::kPDFRegionLink:
    {
      OdDgPdfRegionLinkNode* pLink = dynamic_cast<OdDgPdfRegionLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;

    case OdDgLinkNode::kExcelSheetLink:
    {
      OdDgExcelSheetLinkNode* pLink = dynamic_cast<OdDgExcelSheetLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;

    case OdDgLinkNode::kWordHeadingLink:
    {
      OdDgWordHeadingLinkNode* pLink = dynamic_cast<OdDgWordHeadingLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;

    case OdDgLinkNode::kWordRegionLink:
    {
      OdDgWordRegionLinkNode* pLink = dynamic_cast<OdDgWordRegionLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;

    case OdDgLinkNode::kURLLink:
    {
      OdDgURLLinkNode* pLink = dynamic_cast<OdDgURLLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;

    case OdDgLinkNode::kUnknownLink:
    {
      OdDgUnknownLinkNode* pLink = dynamic_cast<OdDgUnknownLinkNode*>(value.get());

      if( pLink )
      {
        writeFieldValue( name, pLink );
      }
    } break;
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgFileLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"FileName", value->getFileName());
  writeFieldValue( strOffset + L"FullPath", value->getFullPath());

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgModelLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"FileName", value->getFileName());
  writeFieldValue( strOffset + L"FullPath", value->getFullPath());
  writeFieldValue( strOffset + L"ModelName", value->getModelName());
  writeFieldValue( strOffset + L"ModelTypeName", value->getModelTypeName());
  writeFieldValue( strOffset + L"ModelType", value->getModelType());

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgRegionLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"FileName", value->getFileName());
  writeFieldValue( strOffset + L"FullPath", value->getFullPath());
  writeFieldValue( strOffset + L"ModelName", value->getModelName());
  writeFieldValue( strOffset + L"RegionName", value->getRegionName());
  writeFieldValue( strOffset + L"RegionTypeName", value->getRegionTypeName());
  writeFieldValue( strOffset + L"RegionType", value->getRegionType());

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgPdfRegionLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"FileName", value->getFileName());
  writeFieldValue( strOffset + L"FullPath", value->getFullPath());

  for( OdUInt32 i = 0; i < value->getPdfInfoItemCount(); i++ )
  {
    OdDgPdfRegionInfoItem rgnItem = value->getPdfInfoItem( i );

    writeFieldValue( strOffset + L"Region Number", i);
    writeFieldValue( strOffset + L"  RegionName", rgnItem.getRegionName());
    writeFieldValue( strOffset + L"  RegionTypeName", rgnItem.getRegionTypeName());
    writeFieldValue( strOffset + L"  RegionType", rgnItem.getRegionType());
    writeFieldValue( strOffset + L"  SequenceNumber", rgnItem.getSequenceNumber());
  }

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgExcelSheetLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"FileName", value->getFileName());
  writeFieldValue( strOffset + L"FullPath", value->getFullPath());
  writeFieldValue( strOffset + L"ExcelSheetName", value->getSheetName());

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgWordHeadingLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"FileName", value->getFileName());
  writeFieldValue( strOffset + L"FullPath", value->getFullPath());

  for( OdUInt32 i = 0; i < value->getInfoItemCount(); i++ )
  {
    OdDgWordHeadingInfoItem wordItem = value->getInfoItem( i );

    writeFieldValue( strOffset + L"Heading Number", i);
    writeFieldValue( strOffset + L"  Name", wordItem.getName());
    writeFieldValue( strOffset + L"  TypeName", wordItem.getTypeName());
    writeFieldValue( strOffset + L"  Type", wordItem.getType());
    writeFieldValue( strOffset + L"  SequenceNumber", wordItem.getSequenceNumber());
    writeFieldValue( strOffset + L"  Style", wordItem.getWordHeadingStyle());
  }

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgWordRegionLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"FileName", value->getFileName());
  writeFieldValue( strOffset + L"FullPath", value->getFullPath());
  writeFieldValue( strOffset + L"RegionName", value->getRegionName());
  writeFieldValue( strOffset + L"RegionTypeName", value->getRegionTypeName());
  writeFieldValue( strOffset + L"RegionType", value->getRegionType());

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgURLLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"URL", value->getURL());

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgUnknownLinkNode* value )
{
  OdString strOffset;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    strOffset += L" ";
  }

  writeFieldValue( strOffset + L"Name ", value->getName() );
  writeFieldValue( strOffset + L"Type", value->getType() );
  writeFieldValue( strOffset + L"ValidFlag", value->getValidFlag());
  writeFieldValue( strOffset + L"HandlerName", value->getHandlerName());
  writeFieldValue( strOffset + L"HandlerData", value->getHandler());

  if( name.getLength() > 0 && name[0] == L'+' )
  {
    dumpFieldName( L" " );
  }
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgModelLinkNode::OdDgModelLinkNodeType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgModelLinkNode::kDesignModel:
    {
      strValue = OD_T("kDesignModel");
    } break;

    case OdDgModelLinkNode::kSheetModel:
    {
      strValue = OD_T("kSheetModel");
    } break;

    default:
    {
      strValue = OD_T("kUnknownModel");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgRegionLinkNode::OdDgRegionLinkNodeType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgRegionLinkNode::kReference:
    {
      strValue = OD_T("kReference");
    } break;

    case OdDgRegionLinkNode::kSavedView:
    {
      strValue = OD_T("kSavedView");
    } break;

    default:
    {
      strValue = OD_T("kUnknownRegion");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgPdfRegionInfoItem::OdDgPdfRegionType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPdfRegionInfoItem::kBookmark:
    {
      strValue = OD_T("kBookmark");
    } break;

    case OdDgPdfRegionInfoItem::kHeading:
    {
      strValue = OD_T("kHeading");
    } break;

    default:
    {
      strValue = OD_T("kUnknownRegion");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgWordHeadingInfoItem::OdDgWordHeadingItemType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgWordHeadingInfoItem::kBookmark:
    {
      strValue = OD_T("kBookmark");
    } break;

    case OdDgWordHeadingInfoItem::kHeading:
    {
      strValue = OD_T("kHeading");
    } break;

    default:
    {
      strValue = OD_T("kUnknown");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgTextWhiteSpaceLinkage::OdDgSpecialSymbolType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgTextWhiteSpaceLinkage::kParagraphBreak:
    {
      strValue = OD_T("kParagraphBreak");
    } break;

    case OdDgTextWhiteSpaceLinkage::kNewLineBreak:
    {
      strValue = OD_T("kNewLineBreak");
    } break;

    default:
    {
      strValue = OD_T("kTab");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgEnvMapBackground::OdDgEnvMapBackgroungMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgEnvMapBackground::kColor:
    {
      strValue = OD_T("kColor");
    } break;

    case OdDgEnvMapBackground::kImage:
    {
      strValue = OD_T("kImage");
    } break;

    default:
    {
      strValue = OD_T("kBlank");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgEnvMapFog::OdDgEnvMapFogMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgEnvMapFog::kLinear:
    {
      strValue = OD_T("kLinear");
    } break;

    default:
    {
      strValue = OD_T("kExponential");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgEnvMapImage::OdDgEnvMapProjectionType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgEnvMapImage::kPlanar:
    {
      strValue = OD_T("kPlanar");
    } break;

    case OdDgEnvMapImage::kCylindrical:
    {
      strValue = OD_T("kCylindrical");
    } break;

    case OdDgEnvMapImage::kSpherical:
    {
      strValue = OD_T("kSpherical");
    } break;

    case OdDgEnvMapImage::kCubic:
    {
      strValue = OD_T("kCubic");
    } break;

    case OdDgEnvMapImage::kFront:
    {
      strValue = OD_T("kFront");
    } break;

    default:
    {
      strValue = OD_T("kLightProbe");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgEnvMapImage::OdDgEnvMapFilterType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgEnvMapImage::kBicubic:
    {
      strValue = OD_T("kBicubic");
    } break;

    case OdDgEnvMapImage::kBilinear:
    {
      strValue = OD_T("kBilinear");
    } break;

    default:
    {
      strValue = OD_T("kNearest");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgEnvMapGradient::OdDgEnvMapGradientMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgEnvMapGradient::k2Colors:
    {
      strValue = OD_T("k2Colors");
    } break;

    default:
    {
      strValue = OD_T("k4Colors");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgEnvMapLayer::OdDgEnvMapLayerType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgEnvMapLayer::kSkyLight:
    {
      strValue = OD_T("kSkyLight");
    } break;

    case OdDgEnvMapLayer::kLightProbe:
    {
      strValue = OD_T("kLightProbe");
    } break;

    case OdDgEnvMapLayer::kImage:
    {
      strValue = OD_T("kImage");
    } break;

    case OdDgEnvMapLayer::kImageCube:
    {
      strValue = OD_T("kImageCube");
    } break;

    default:
    {
      strValue = OD_T("kGradient");
    } break;
  }

  writeFieldValue(name, strValue);
}

inline void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgView::OdDgDisplayMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgView::kWireMesh:
    {
      strValue = OD_T("kWireMesh");
    } break;

    case OdDgView::kHiddenLine:
    {
      strValue = OD_T("kHiddenLine");
    } break;

    case OdDgView::kFilledHiddenLine:
    {
      strValue = OD_T("kFilledHiddenLine");
    } break;

    case OdDgView::kConstantShade:
    {
      strValue = OD_T("kConstantShade");
    } break;

    case OdDgView::kSmoothShade:
    {
      strValue = OD_T("kSmoothShade");
    } break;

    case OdDgView::kPhongShade:
    {
      strValue = OD_T("kPhongShade");
    } break;

    default:
    {
      strValue = OD_T("kWireFrame");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdGiMaterialColor& value)
{
  OdString strValue;

  strValue.format(L"Clr = %08X, factor = %f, ", ODRGB(value.color().red(),value.color().green(),value.color().blue()), value.factor());

  if( value.method() == OdGiMaterialColor::kInherit )
  {
    strValue += L"kInherit";
  }
  else
  {
    strValue += L"kOverride";
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgMaterialParam& value)
{
  if( value.getIsComplexParam() )
  {
    dumpFieldName( name + L" :");

    int nSpaces = 0;

    while( (nSpaces < name.getLength()) && (name[nSpaces] == L' ') )
    {
      nSpaces++;
    }

    nSpaces+=2;

    OdString strSpaces;

    for( OdUInt32 j = 0; j < (OdUInt32)nSpaces; j++ )
    {
      strSpaces += L" ";
    }

    for( OdUInt32 i = 0; i < value.getChildCount(); i++ )
    {
      OdString strChildName = strSpaces;

      OdDgMaterialParam childParam = value.getChild(i);

      strChildName += childParam.getParamName();

      writeFieldValue( strChildName, childParam );
    }
  }
  else
  {
    writeFieldValue( name, value.getParamValue() );
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdGeScale2d& value)
{
  OdString strValue;
  strValue.format(L"%f, %f", value.sx, value.sy);

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdGeScale3d& value)
{
  OdString strValue;
  strValue.format(L"%f, %f, %f", value.sx, value.sy, value.sz);

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDoubleColor& value)
{
  OdString strValue;
  strValue.format(L"r = %f, g = %f, b = %f", value.dRed, value.dGreen, value.dBlue);

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgMaterialPattern::OdDgMaterialPatternType value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgMaterialPattern::kUnknownPattern:
    {
      strValue = OD_T("kUnknownPattern");
    } break;

    case OdDgMaterialPattern::kDiffusePattern:
    {
      strValue = OD_T("kDiffusePattern");
    } break;

    case OdDgMaterialPattern::kBumpMapPattern:
    {
      strValue = OD_T("kBumpMapPattern");
    } break;

    case OdDgMaterialPattern::kSpecularPattern:
    {
      strValue = OD_T("kSpecularPattern");
    } break;

    case OdDgMaterialPattern::kReflectPattern:
    {
      strValue = OD_T("kReflectPattern");
    } break;

    case OdDgMaterialPattern::kOpacityPattern:
    {
      strValue = OD_T("kOpacityPattern");
    } break;

    case OdDgMaterialPattern::kDiffuseMapPattern:
    {
      strValue = OD_T("kDiffuseMapPattern");
    } break;

    case OdDgMaterialPattern::kFinishPattern:
    {
      strValue = OD_T("kFinishPattern");
    } break;

    case OdDgMaterialPattern::kTranslucencyPattern:
    {
      strValue = OD_T("kTranslucencyPattern");
    } break;

    case OdDgMaterialPattern::kGlowPattern:
    {
      strValue = OD_T("kGlowPattern");
    } break;

    case OdDgMaterialPattern::kClearcoatPattern:
    {
      strValue = OD_T("kClearcoatPattern");
    } break;

    case OdDgMaterialPattern::kAnisotropyPattern:
    {
      strValue = OD_T("kAnisotropyPattern");
    } break;

    case OdDgMaterialPattern::kSpecularColorPattern:
    {
      strValue = OD_T("kSpecularColorPattern");
    } break;

    case OdDgMaterialPattern::kTransparentColorPattern:
    {
      strValue = OD_T("kTransparentColorPattern");
    } break;

    case OdDgMaterialPattern::kSubSurfaceColorPattern:
    {
      strValue = OD_T("kSubSurfaceColorPattern");
    } break;

    case OdDgMaterialPattern::kDisplacementPattern:
    {
      strValue = OD_T("kDisplacementPattern");
    } break;

    case OdDgMaterialPattern::kNormalPattern:
    {
      strValue = OD_T("kNormalPattern");
    } break;

    case OdDgMaterialPattern::kFurLengthPattern:
    {
      strValue = OD_T("kFurLengthPattern");
    } break;

    case OdDgMaterialPattern::kFurDensityPattern:
    {
      strValue = OD_T("kFurDensityPattern");
    } break;

    case OdDgMaterialPattern::kFurJitterPattern:
    {
      strValue = OD_T("kFurJitterPattern");
    } break;

    case OdDgMaterialPattern::kFurFlexPattern:
    {
      strValue = OD_T("kFurFlexPattern");
    } break;

    case OdDgMaterialPattern::kFurClimpsPattern:
    {
      strValue = OD_T("kFurClimpsPattern");
    } break;

    case OdDgMaterialPattern::kGlowColorPattern:
    {
      strValue = OD_T("kGlowColorPattern");
    } break;

    case OdDgMaterialPattern::kReflectColorPattern:
    {
      strValue = OD_T("kReflectColorPattern");
    } break;

    case OdDgMaterialPattern::kRefractionRoughnessPattern:
    {
      strValue = OD_T("kRefractionRoughnessPattern");
    } break;

    case OdDgMaterialPattern::kFrenselPattern:
    {
      strValue = OD_T("kFrenselPattern");
    } break;

    case OdDgMaterialPattern::kGeometryPattern:
    {
      strValue = OD_T("kGeometryPattern");
    } break;

    default:
    {
      strValue.format(OD_T("kUndefinedType_%d"), (OdUInt32)(value));
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgMaterialMap::MappingType value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgMaterialMap::kElevationDrape:
    {
      strValue = OD_T("kElevationDrape");
    } break;

    case OdDgMaterialMap::kPlanar:
    {
      strValue = OD_T("kPlanar");
    } break;

    case OdDgMaterialMap::kDirectionalDrape:
    {
      strValue = OD_T("kDirectionalDrape");
    } break;

    case OdDgMaterialMap::kCubic:
    {
      strValue = OD_T("kCubic");
    } break;

    case OdDgMaterialMap::kSpherical:
    {
      strValue = OD_T("kSpherical");
    } break;

    case OdDgMaterialMap::kCylindrical:
    {
      strValue = OD_T("kCylindrical");
    } break;

    default:
    {
      strValue.format(OD_T("kParametric"));
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgMaterialMap::ScaleMode value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgMaterialMap::kMasterUnitMode:
    {
      strValue = OD_T("kMasterUnitMode");
    } break;

    case OdDgMaterialMap::kSubUnitMode:
    {
      strValue = OD_T("kSubUnitMode");
    } break;

    case OdDgMaterialMap::kMetersMode:
    {
      strValue = OD_T("kMetersMode");
    } break;

    case OdDgMaterialMap::kMillimetrersMode:
    {
      strValue = OD_T("kMillimetrersMode");
    } break;

    case OdDgMaterialMap::kFeetMode:
    {
      strValue = OD_T("kFeetMode");
    } break;

    case OdDgMaterialMap::kInchesMode:
    {
      strValue = OD_T("kInchesMode");
    } break;

    default:
    {
      strValue.format(OD_T("kSurfaceMode"));
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgMaterialLayerPattern::OdDgMaterialLayerPatternType value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgMaterialLayerPattern::kLayerNormal:
    {
      strValue = OD_T("kLayerNormal");
    } break;

    case OdDgMaterialLayerPattern::kLayerAdd:
    {
      strValue = OD_T("kLayerAdd");
    } break;

    case OdDgMaterialLayerPattern::kLayerSubtract:
    {
      strValue = OD_T("kLayerSubtract");
    } break;

    case OdDgMaterialLayerPattern::kLayerAlpha:
    {
      strValue = OD_T("kLayerAlpha");
    } break;

    case OdDgMaterialLayerPattern::kLayerGradient:
    {
      strValue = OD_T("kLayerGradient");
    } break;

    case OdDgMaterialLayerPattern::kLayerImage:
    {
      strValue = OD_T("kLayerImage");
    } break;

    case OdDgMaterialLayerPattern::kLayerProcedure:
    {
      strValue = OD_T("kLayerProcedure");
    } break;

    case OdDgMaterialLayerPattern::kLayerGamma:
    {
      strValue = OD_T("kLayerGamma");
    } break;

    case OdDgMaterialLayerPattern::kLayerTint:
    {
      strValue = OD_T("kLayerTint");
    } break;

    case OdDgMaterialLayerPattern::kLayerGroupStart:
    {
      strValue = OD_T("kLayerGroupStart");
    } break;

    case OdDgMaterialLayerPattern::kLayerGroupEnd:
    {
      strValue = OD_T("kLayerGroupEnd");
    } break;

    case OdDgMaterialLayerPattern::kLayerAlphaBgStart:
    {
      strValue = OD_T("kLayerAlphaBgStart");
    } break;

    case OdDgMaterialLayerPattern::kLayerAlphaBgEnd:
    {
      strValue = OD_T("kLayerAlphaBgEnd");
    } break;

    default:
    {
      strValue.format(OD_T("kLayerUnknown"));
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgModel::Type value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgModel::kDesignModel:
    {
      strValue = OD_T("kDesignModel");
    } break;

    case OdDgModel::kSheetModel:
    {
      strValue = OD_T("kSheetModel");
    } break;

    case OdDgModel::kExtractionModel:
    {
      strValue = OD_T("kExtractionModel");
    } break;

    default:
    {
      strValue.format(L"kModelType_%d",value);
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgModel::CellType value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgModel::kNormalGraphics:
    {
      strValue = OD_T("kGraphics");
    } break;

    case OdDgModel::kPoint:
    {
      strValue = OD_T("kPoint");
    } break;

    case OdDgModel::kMenu:
    {
      strValue = OD_T("kMenu");
    } break;

    default:
    {
      strValue.format(L"kCellType_%d",value);
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDgDimTextFormat::AngleValueDisplayMode value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDimTextFormat::kAngle_DM:
    {
      strValue = OD_T("kAngle_DM");
    } break;

    case OdDgDimTextFormat::kAngle_DMS:
    {
      strValue = OD_T("kAngle_DMS");
    } break;

    case OdDgDimTextFormat::kAngle_C:
    {
      strValue = OD_T("kAngle_C");
    } break;

    case OdDgDimTextFormat::kAngle_Radian:
    {
      strValue = OD_T("kAngle_Radian");
    } break;

    default:
    {
      strValue.format(L"kAngle_D");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDg::OdDgDecimalAccuracy value)
{
  OdString strValue;

  switch( value )
  {
    case OdDg::kDecimalAccuracy0 :
    {
      strValue = OD_T("kDecimalAccuracy0");
    } break;

    case OdDg::kDecimalAccuracy1 :
    {
      strValue = OD_T("kDecimalAccuracy1");
    } break;

    case OdDg::kDecimalAccuracy2 :
    {
      strValue = OD_T("kDecimalAccuracy2");
    } break;

    case OdDg::kDecimalAccuracy3 :
    {
      strValue = OD_T("kDecimalAccuracy3");
    } break;

    case OdDg::kDecimalAccuracy4 :
    {
      strValue = OD_T("kDecimalAccuracy4");
    } break;

    case OdDg::kDecimalAccuracy5 :
    {
      strValue = OD_T("kDecimalAccuracy5");
    } break;

    case OdDg::kDecimalAccuracy6 :
    {
      strValue = OD_T("kDecimalAccuracy6");
    } break;

    case OdDg::kDecimalAccuracy7 :
    {
      strValue = OD_T("kDecimalAccuracy7");
    } break;

    default:
    {
      strValue.format(L"kDecimalAccuracy8");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  OdDg::OdDgAngleReadoutDirectionMode value)
{
  OdString strValue;

  switch( value )
  {
    case OdDg::kAzimut:
    {
      strValue = OD_T("kAzimut");
    } break;

    default:
    {
      strValue.format(L"kBearing");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgXMLNode& value)
{
  OdUInt32 nSpaces = 0;

  for( OdInt32 n = 0; n < name.getLength(); n++ )
  {
    if( name.getAt(n) != L' ' )
    {
      break;
    }

    nSpaces++;
  }

  dumpFieldName( name );

  for( OdUInt32  i = 0; i < value.getPropertiesCount(); i++ )
  {
    const OdDgXMLProperty& xmlProp = value.getProperty(i);

    if( xmlProp.isXMLNode() )
    {
      OdString strNodeName = xmlProp.getName() + L":";

      for( OdUInt32 j = 0; j < nSpaces + 2; j++ )
      {
        strNodeName = L" " + strNodeName;
      }

      writeFieldValue( strNodeName, xmlProp.asXMLNode());
    }
    else
    {
      OdString strPropName = xmlProp.getName();

      for( OdUInt32 j = 0; j < nSpaces + 2; j++ )
      {
        strPropName = L" " + strPropName;
      }

      writeFieldValue( strPropName, xmlProp.asString() );
    }
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleTableRecord::OdDgDisplayStyleDisplayHeader& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleTableRecord::kNoHeader:
    {
      strValue = OD_T("kNoHeader");
    } break;

    case OdDgDisplayStyleTableRecord::kHeight:
    {
      strValue = OD_T("kHeight");
    } break;

    case OdDgDisplayStyleTableRecord::kSlope:
    {
      strValue = OD_T("kSlope");
    } break;

    case OdDgDisplayStyleTableRecord::kAspectAngle:
    {
      strValue = OD_T("kAspectAngle");
    } break;

    case OdDgDisplayStyleTableRecord::kElementRange:
    {
      strValue = OD_T("kElementRange");
    } break;

    case OdDgDisplayStyleTableRecord::kHillShade:
    {
      strValue = OD_T("kHillShade");
    } break;

    default:
    {
      strValue.format(L"kUnknownHeader 0x%08x", value );
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleTableRecord::OdDgDisplayMode& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleTableRecord::kWireframe:
    {
      strValue = OD_T("kWireframe");
    } break;

    case OdDgDisplayStyleTableRecord::kVisibleEdges:
    {
      strValue = OD_T("kVisibleEdges");
    } break;

    case OdDgDisplayStyleTableRecord::kFilledVisibleEdges:
    {
      strValue = OD_T("kFilledVisibleEdges");
    } break;

    case OdDgDisplayStyleTableRecord::kShaded:
    {
      strValue = OD_T("kShaded");
    } break;

    default:
    {
      strValue.format(L"kUnknownMode %d", value );
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleTableRecord::OdDgEnvironmentTypeDisplayed& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleTableRecord::kBackgroundColor:
    {
      strValue = OD_T("kBackgroundColor");
    } break;

    case OdDgDisplayStyleTableRecord::kEnvironment:
    {
      strValue = OD_T("kEnvironment");
    } break;

    default:
    {
      strValue.format(L"kUnknownType %d", value );
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendCoordinateAxis& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleLegendXAttribute::kYAxis :
    {
      strValue = OD_T("kYAxis");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kXAxis :
    {
      strValue = OD_T("kXAxis");
    } break;

    default:
    {
      strValue = OD_T("kZAxis");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgSlopeDisplayMode& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleLegendXAttribute::kPercent :
    {
      strValue = OD_T("kPercent");
    } break;

    default:
    {
      strValue = OD_T("kAngle");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendColorScheme& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleLegendXAttribute::kRedBlue :
    {
      strValue = OD_T("kRedBlue");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kBlueRed :
    {
      strValue = OD_T("kBlueRed");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kMonochrome :
    {
      strValue = OD_T("kMonochrome");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kTopographic :
    {
      strValue = OD_T("kTopographic");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kSeaMountain :
    {
      strValue = OD_T("kSeaMountain");
    } break;

    default:
    {
      strValue = OD_T("kCustom");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendEdgeOverride& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleLegendXAttribute::kAllVisible :
    {
      strValue = OD_T("kAllVisible");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kAllInvisible :
    {
      strValue = OD_T("kAllInvisible");
    } break;

    default:
    {
      strValue = OD_T("kNone");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendValuesCalcMethod& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleLegendXAttribute::kStep :
    {
      strValue = OD_T("kStep");
    } break;

    default:
    {
      strValue = OD_T("kCount");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgSteppedDisplayMethod& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleLegendXAttribute::kAccurate :
    {
      strValue = OD_T("kAccurate");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kIsolines :
    {
      strValue = OD_T("kIsolines");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kFast :
    {
      strValue = OD_T("kFast");
    } break;

    case OdDgDisplayStyleLegendXAttribute::kFastIsolines :
    {
      strValue = OD_T("kFastIsolines");
    } break;

    default:
    {
      strValue = OD_T("kOff");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleLegendXAttribute::OdDgLegendItem& value)
{
  OdString strValue;
  strValue.format( L"%0.6f, 0x%08X, %d", value.m_dItemPosition, value.m_uItemColor, value.m_bVisibleItem ? 1 : 0 );
  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleTypeFilterXAttribute::OdDgFilterApplyToType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleTypeFilterXAttribute::kSelectedElementTypes :
    {
      strValue = OD_T("kSelectedElementTypes");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kAllExceptSelectedElementTypes :
    {
      strValue = OD_T("kAllExceptSelectedElementTypes");
    } break;

    default:
    {
      strValue = OD_T("kAllElementTypes");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgDisplayStyleTypeFilterXAttribute::OdDgFilterElementType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgDisplayStyleTypeFilterXAttribute::kBSplineSurface :
    {
      strValue = OD_T("kBSplineSurface");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kBSplineCurve :
    {
      strValue = OD_T("kBSplineCurve");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kCellHeader :
    {
      strValue = OD_T("kCellHeader");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSpiralCurve :
    {
      strValue = OD_T("kSpiralCurve");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kAssociativeRegion :
    {
      strValue = OD_T("kAssociativeRegion");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kDimension :
    {
      strValue = OD_T("kDimension");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSmartSolidAndSurface :
    {
      strValue = OD_T("kSmartSolidAndSurface");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSharedCell :
    {
      strValue = OD_T("kSharedCell");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kNote :
    {
      strValue = OD_T("kNote");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kMultiline :
    {
      strValue = OD_T("kMultiline");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kGroupedHole :
    {
      strValue = OD_T("kGroupedHole");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kTag :
    {
      strValue = OD_T("kTag");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kLineString :
    {
      strValue = OD_T("kLineString");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kRasterImage :
    {
      strValue = OD_T("kRasterImage");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kTextNode :
    {
      strValue = OD_T("kTextNode");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kRasterAttachment :
    {
      strValue = OD_T("kRasterAttachment");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kComplexString :
    {
      strValue = OD_T("kComplexString");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kMesh :
    {
      strValue = OD_T("kMesh");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kEllipse :
    {
      strValue = OD_T("kEllipse");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kExtendedElement :
    {
      strValue = OD_T("kExtendedElement");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kText :
    {
      strValue = OD_T("kText");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSavedView :
    {
      strValue = OD_T("kSavedView");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSolid :
    {
      strValue = OD_T("kSolid");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSectionClip :
    {
      strValue = OD_T("kSectionClip");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kCone :
    {
      strValue = OD_T("kCone");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSectionCallout :
    {
      strValue = OD_T("kSectionCallout");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kFeatureSolid :
    {
      strValue = OD_T("kFeatureSolid");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kDetailCallout2dView :
    {
      strValue = OD_T("kDetailCallout2dView");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kDigitalSignature :
    {
      strValue = OD_T("kDigitalSignature");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kElevationCallout :
    {
      strValue = OD_T("kElevationCallout");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kLine :
    {
      strValue = OD_T("kLine");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kTitleText :
    {
      strValue = OD_T("kTitleText");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kCurve :
    {
      strValue = OD_T("kCurve");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSurface :
    {
      strValue = OD_T("kSurface");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kPointString :
    {
      strValue = OD_T("kPointString");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kMarkupContentAreaHeader :
    {
      strValue = OD_T("kMarkupContentAreaHeader");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kOleObject :
    {
      strValue = OD_T("kOleObject");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kAnnotationCell :
    {
      strValue = OD_T("kAnnotationCell");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kShape :
    {
      strValue = OD_T("kShape");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kComplexShape :
    {
      strValue = OD_T("kComplexShape");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kDrawingTitle :
    {
      strValue = OD_T("kDrawingTitle");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kDrawingTitleUnderlined :
    {
      strValue = OD_T("kDrawingTitleUnderlined");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSheetAnnotationClip :
    {
      strValue = OD_T("kSheetAnnotationClip");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kPlanCallout :
    {
      strValue = OD_T("kPlanCallout");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kSectionCalloutOrthogonal :
    {
      strValue = OD_T("kSectionCalloutOrthogonal");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kInteriorElevationCallout :
    {
      strValue = OD_T("kInteriorElevationCallout");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kDetailCallout :
    {
      strValue = OD_T("kDetailCallout");
    } break;

    case OdDgDisplayStyleTypeFilterXAttribute::kArc :
    {
      strValue = OD_T("kArc");
    } break;

    default:
    {
      strValue = OD_T("kUnkownType");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainFeature::OdDgTerrainFeatureType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainFeature::kDataPoints :
    {
      strValue = OD_T("kDataPoints");
    } break;

    case OdDgTerrainFeature::kHole :
    {
      strValue = OD_T("kHole");
    } break;

    case OdDgTerrainFeature::kBreakLine :
    {
      strValue = OD_T("kBreakLine");
    } break;

    case OdDgTerrainFeature::kContour :
    {
      strValue = OD_T("kContour");
    } break;

    case OdDgTerrainFeature::kVoid :
    {
      strValue = OD_T("kVoid");
    } break;

    case OdDgTerrainFeature::kIsland :
    {
      strValue = OD_T("kIsland");
    } break;

    case OdDgTerrainFeature::kOuterBoundary :
    {
      strValue = OD_T("kOuterBoundary");
    } break;

    default:
    {
      strValue = OD_T("kUnknown");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainControlElement::OdDgTerrainEdgeMethod& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainControlElement::kNone :
    {
      strValue = OD_T("kNone");
    } break;

    case OdDgTerrainControlElement::kSlivers :
    {
      strValue = OD_T("kSlivers");
    } break;

    case OdDgTerrainControlElement::kMaxTriangleLength :
    {
      strValue = OD_T("kMaxTriangleLength");
    } break;

    default:
    {
      strValue = OD_T("kUnknown");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgExtendedElementTypeXAttribute::OdDgExtendedElementType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgExtendedElementTypeXAttribute::kSectionClipElement :
    {
      strValue = OD_T("kSectionClipElement");
    } break;

    case OdDgExtendedElementTypeXAttribute::kXRefViewsElement :
    {
      strValue = OD_T("kXRefViewsElement");
    } break;

    case OdDgExtendedElementTypeXAttribute::kDisplayStyleElement :
    {
      strValue = OD_T("kDisplayStyleElement");
    } break;


    case OdDgExtendedElementTypeXAttribute::kDetailingSymbolElement :
    {
      strValue = OD_T("kDetailingSymbolElement");
    } break;

    case OdDgExtendedElementTypeXAttribute::kLuxSetupElement :
    {
      strValue = OD_T("kLuxSetupElement");
    } break;

    case OdDgExtendedElementTypeXAttribute::kEnvironmentMapTableElement :
    {
      strValue = OD_T("kEnvironmentMapTableElement");
    } break;

    case OdDgExtendedElementTypeXAttribute::kTerrainCtrlElement :
    {
      strValue = OD_T("kTerrainCtrlElement");
    } break;

    case OdDgExtendedElementTypeXAttribute::kTerrainGraphElement :
    {
      strValue = OD_T("kTerrainGraphElement");
    } break;

    case OdDgExtendedElementTypeXAttribute::kTerrainGraphElementCE :
    {
      strValue = OD_T("kTerrainGraphElementCE");
    } break;

    case OdDgExtendedElementTypeXAttribute::kItemLibrary :
    {
      strValue = OD_T("kItemLibrary");
    } break;

    case OdDgExtendedElementTypeXAttribute::kTableElement :
    {
      strValue = OD_T("kTableElement");
    } break;

    case OdDgExtendedElementTypeXAttribute::kTablePrototype :
    {
      strValue = OD_T("kTablePrototype");
    } break;

    case OdDgExtendedElementTypeXAttribute::kVariables :
    {
      strValue = OD_T("kVariables");
    } break;

    case OdDgExtendedElementTypeXAttribute::kConstraints2d :
    {
      strValue = OD_T("kConstraints2d");
    } break;

    case OdDgExtendedElementTypeXAttribute::kExtendedElementWithGrCache :
    {
      strValue = OD_T("kExtendedElementWithGrCache");
    } break;

    default:
    {
      strValue.format(OD_T("UnregisterdType_0x%08X"), value );
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainGraphicsSettingsXAttribute::OdDgTerrainGraphicsType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainGraphicsSettingsXAttribute::kNoSettings :
    {
      strValue = OD_T("kNoSettings");
    } break;

    case OdDgTerrainGraphicsSettingsXAttribute::kSourceFeatureSettings :
    {
      strValue = OD_T("kSourceFeatureSettings");
    } break;

    case OdDgTerrainGraphicsSettingsXAttribute::kVertexSettings :
    {
      strValue = OD_T("kVertexSettings");
    } break;

    case OdDgTerrainGraphicsSettingsXAttribute::kTriangleSettings :
    {
      strValue = OD_T("kTriangleSettings");
    } break;

    case OdDgTerrainGraphicsSettingsXAttribute::kContourSettings :
    {
      strValue = OD_T("kContourSettings");
    } break;

    default:
    {
      strValue = OD_T("kUnknownSettingsValue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainSourceFeatureSettings::OdDgSourceFeatureType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainSourceFeatureSettings::kBreakLineFeature :
    {
      strValue = OD_T("kBreakLineFeature");
    } break;

    case OdDgTerrainSourceFeatureSettings::kHoleFeature :
    {
      strValue = OD_T("kHoleFeature");
    } break;

    case OdDgTerrainSourceFeatureSettings::kIslandFeature :
    {
      strValue = OD_T("kIslandFeature");
    } break;

    case OdDgTerrainSourceFeatureSettings::kVoidFeature :
    {
      strValue = OD_T("kVoidFeature");
    } break;

    case OdDgTerrainSourceFeatureSettings::kBoundaryFeature :
    {
      strValue = OD_T("kBoundaryFeature");
    } break;

    case OdDgTerrainSourceFeatureSettings::kContourFeature :
    {
      strValue = OD_T("kContourFeature");
    } break;

    default:
    {
      strValue = OD_T("kUnknownFeatureValue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainVertexFeatureSettings::OdDgTerrainVertexFeatureType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainVertexFeatureSettings::kSourceVertex :
    {
      strValue = OD_T("kSourceVertex");
    } break;

    case OdDgTerrainVertexFeatureSettings::kTriangleVertex :
    {
      strValue = OD_T("kTriangleVertex");
    } break;

    case OdDgTerrainVertexFeatureSettings::kLowPoints :
    {
      strValue = OD_T("kLowPoints");
    } break;

    case OdDgTerrainVertexFeatureSettings::kHighPoints :
    {
      strValue = OD_T("kHighPoints");
    } break;

    case OdDgTerrainVertexFeatureSettings::kFlowArrows :
    {
      strValue = OD_T("kFlowArrows");
    } break;

    default:
    {
      strValue = OD_T("kUnknownSettingsValue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainOptions::OdDgTerrainPointDisplayType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainOptions::kElement :
    {
      strValue = OD_T("kElement");
    } break;

    case OdDgTerrainOptions::kSymbol :
    {
      strValue = OD_T("kSymbol");
    } break;

    case OdDgTerrainOptions::kCell :
    {
      strValue = OD_T("kCell");
    } break;

    default:
    {
      strValue = OD_T("kUnknownValue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainOptions::OdDgTerrainContourSmoothingType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainOptions::kNoneSmoothing :
    {
      strValue = OD_T("kNoneSmoothing");
    } break;

    case OdDgTerrainOptions::kVertexSmoothing :
    {
      strValue = OD_T("kVertexSmoothing");
    } break;

    case OdDgTerrainOptions::kSplineSmoothing :
    {
      strValue = OD_T("kSplineSmoothing");
    } break;

    case OdDgTerrainOptions::kSplineWithoutOverlapDetectionSmoothing :
    {
        strValue = OD_T("kSplineWithoutOverlapDetectionSmoothing");
    } break;

    default:
    {
      strValue = OD_T("kUnknownValue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainContourFeatureSettings::OdDgTerrainContourFeatureType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainContourFeatureSettings::kMajorContour :
    {
      strValue = OD_T("kMajorContour");
    } break;

    case OdDgTerrainContourFeatureSettings::kMinorContour :
    {
      strValue = OD_T("kMinorContour");
    } break;

    default:
    {
      strValue = OD_T("kUnknownValue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgItemType::OdDgItemTypeCategory& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgItemType::kItemType :
    {
      strValue = OD_T("kItemType");
    } break;

    case OdDgItemType::kPropertyType :
    {
      strValue = OD_T("kPropertyType");
    } break;

    case OdDgItemType::kCustomAttribute :
    {
      strValue = OD_T("kCustomAttribute");
    } break;

    default:
    {
      strValue = OD_T("kUnknownValue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePrintArea value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kVeiw :
    {
      strValue = OD_T("kView");
    } break;

    case OdDgPrintStyleTableRecord::kSheet :
    {
      strValue = OD_T("kSheet");
    } break;

    case OdDgPrintStyleTableRecord::kFence :
    {
      strValue = OD_T("kFence");
    } break;

    default:
    {
      strValue = OD_T("kUndefinedArea");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyle3StateBoolean value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kFalse :
    {
      strValue = OD_T("kFalse");
    } break;

    case OdDgPrintStyleTableRecord::kTrue :
    {
      strValue = OD_T("kTrue");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleColorMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kMonochrome :
    {
      strValue = OD_T("kMonochrome");
    } break;

    case OdDgPrintStyleTableRecord::kGrayscale :
    {
      strValue = OD_T("kGrayscale");
    } break;

    case OdDgPrintStyleTableRecord::kTrueColor :
    {
      strValue = OD_T("kTrueColor");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePropertyPubMethod value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kNone :
    {
      strValue = OD_T("kNone");
    } break;

    case OdDgPrintStyleTableRecord::kAll :
    {
      strValue = OD_T("kAll");
    } break;

    case OdDgPrintStyleTableRecord::kFilterByFile :
    {
      strValue = OD_T("kFilterByFile");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePdfPaperSizeMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kPaperNotSet :
    {
      strValue = OD_T("kPaperNotSet");
    } break;

    case OdDgPrintStyleTableRecord::kAutoPaperSize :
    {
      strValue = OD_T("kAutoPaperSize");
    } break;

    case OdDgPrintStyleTableRecord::kDefinedPaperSize :
    {
      strValue = OD_T("kDefinedPaperSize");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleRasterRotation value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kRot0 :
    {
      strValue = OD_T("kRot0");
    } break;

    case OdDgPrintStyleTableRecord::kRot90 :
    {
      strValue = OD_T("kRot90");
    } break;

    case OdDgPrintStyleTableRecord::kRot180 :
    {
      strValue = OD_T("kRot180");
    } break;

    case OdDgPrintStyleTableRecord::kRot270 :
    {
      strValue = OD_T("kRot270");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgConstraintsDataXAttr::OdDgConstraintsXAttributeType value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgConstraintsDataXAttr::k2dConstraints :
    {
      strValue = OD_T("k2dConstraints");
    } break;

    case OdDgConstraintsDataXAttr::kDimConstraints :
    {
      strValue = OD_T("kDimConstraints");
    } break;

    case OdDgConstraintsDataXAttr::kVersionConstraints :
    {
      strValue = OD_T("kVersionConstraints");
    } break;

    case OdDgConstraintsDataXAttr::kIdMapConstraints :
    {
      strValue = OD_T("kIdMapConstraints");
    } break;

    default:
    {
      strValue = OD_T("kFlagsConstraints");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdDgDimensionalConstraintRepresentation::OdDgDimensionalConstraintRepAngleQuadrant& uQuad )
{
  OdString strValue;

  switch( uQuad )
  {
    case OdDgDimensionalConstraintRepresentation::kQuadrant1:
    {
      strValue = OD_T("kQuadrant1");
    } break;

    case OdDgDimensionalConstraintRepresentation::kQuadrant2:
    {
      strValue = OD_T("kQuadrant2");
    } break;

    case OdDgDimensionalConstraintRepresentation::kQuadrant3:
    {
      strValue = OD_T("kQuadrant3");
    } break;

    default:
    {
      strValue = OD_T("kQuadrant0");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdDgDimensionalConstraintRepresentation::OdDgDimensionalConstraintYOffsetAlignment& uAlign )
{
  OdString strValue;

  switch( uAlign )
  {
    case OdDgDimensionalConstraintRepresentation::kPositiveY:
    {
      strValue = OD_T("kPositiveY");
    } break;

    case OdDgDimensionalConstraintRepresentation::kPositiveX:
    {
      strValue = OD_T("kPositiveX");
    } break;

    case OdDgDimensionalConstraintRepresentation::kNegativeY:
    {
      strValue = OD_T("kNegativeY");
    } break;

    case OdDgDimensionalConstraintRepresentation::kNegativeX:
    {
      strValue = OD_T("kNegativeX");
    } break;

    default:
    {
      strValue = OD_T("kTrue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDg2dConstraintRepresentation::OdDgConstraintType type )
{
  OdString strValue;

  switch( type )
  {
    case OdDg2dConstraintRepresentation::kDistanceConstraint:
    {
      strValue = OD_T("kDistanceConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kAngleConstraint:
    {
      strValue = OD_T("kAngleConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kRadiusConstraint:
    {
      strValue = OD_T("kRadiusConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kEllipArcPrimaryAxisConstraint:
    {
      strValue = OD_T("kEllipArcPrimaryAxisConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kEllipArcSecondaryAxisConstraint:
    {
      strValue = OD_T("kEllipArcSecondaryAxisConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kEqualDistanceConstraint:
    {
      strValue = OD_T("kEqualDistanceConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kAreaConstraint:
    {
      strValue = OD_T("kAreaConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kPerimeterConstraint:
    {
      strValue = OD_T("kPerimeterConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kFix2dConstraint:
    {
      strValue = OD_T("kFix2dConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kConcentricConstraint:
    {
      strValue = OD_T("kConcentricConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kParallelConstraint:
    {
      strValue = OD_T("kParallelConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kPerpendicularConstraint:
    {
      strValue = OD_T("kPerpendicularConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kCoincidentConstraint:
    {
      strValue = OD_T("kCoincidentConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kTangentConstraint:
    {
      strValue = OD_T("kTangentConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kEqualConstraint:
    {
      strValue = OD_T("kEqualConstraint");
    } break;

    case OdDg2dConstraintRepresentation::kDimensionPlaneConstraint:
    {
      strValue = OD_T("kDimensionPlaneConstraint");
    } break;

    default:
    {
      strValue.format(L"Constraint type 0x%02X", type );
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdDgConstraintRepPoint& ptConstraint )
{
  OdString strValue;
  strValue.format(L"%d", ptConstraint.getEelementIdMapIndex() );

  OdString strTopoType;

  if( ptConstraint.getTopologyFlags() != 0 )
    strTopoType.format(L"(%d, %04X) : ", ptConstraint.getTopologyId(), ptConstraint.getTopologyFlags() );
  else
    strTopoType.format(L"(%d) : ", ptConstraint.getTopologyId() );

  OdString strPath;
  strPath.format(L"0x%02X = ", ptConstraint.getType() );

  for( OdUInt32 i = 0; i < ptConstraint.getItemsCount(); i++ )
  {
    OdString strPathItem;

    if( i == (ptConstraint.getItemsCount() - 1) )
      strPathItem.format(L"%d", ptConstraint.getItem(i) );
    else
      strPathItem.format(L"%d, ", ptConstraint.getItem(i) );

    strPath += strPathItem;
  }

  strPath += L" : ";

  OdString strFlags;

  if( ptConstraint.getSkipTopLevelComplexElementFlag() )
    strFlags.format(L"Cplx %d of %d => %d", ptConstraint.getSegmentIndex(), ptConstraint.getNumberOfSegments(), ptConstraint.getSnapType() );
  else
    strFlags.format(L"%d of %d => %d", ptConstraint.getSegmentIndex(), ptConstraint.getNumberOfSegments(), ptConstraint.getSnapType() );

  strValue += strTopoType + strPath + strFlags;

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePrintDefCreationMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kFromPaperSheetModels :
    {
      strValue = OD_T("kFromPaperSheetModels");
    } break;

    case OdDgPrintStyleTableRecord::kFromAllModels :
    {
      strValue = OD_T("kFromAllModels");
    } break;

    case OdDgPrintStyleTableRecord::kFromAllSheetModels :
    {
      strValue = OD_T("kFromAllSheetModels");
    } break;

    case OdDgPrintStyleTableRecord::kFromAllDesignModels :
    {
      strValue = OD_T("kFromAllDesignModels");
    } break;

    case OdDgPrintStyleTableRecord::kFromListedModels :
    {
      strValue = OD_T("kFromListedModels");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleFenceCreationData::OdDgPrintStyleFenceCreationMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleFenceCreationData::kFromShape :
    {
      strValue = OD_T("kFromShape");
    } break;

    case OdDgPrintStyleFenceCreationData::kFromAllShapes :
    {
      strValue = OD_T("kFromAllShapes");
    } break;

    case OdDgPrintStyleFenceCreationData::kFromCell :
    {
      strValue = OD_T("kFromCell");
    } break;

    case OdDgPrintStyleFenceCreationData::kFromAllCells :
    {
      strValue = OD_T("kFromAllCells");
    } break;

    case OdDgPrintStyleFenceCreationData::kFitToMasterModel :
    {
      strValue = OD_T("kFitToMasterModel");
    } break;

    case OdDgPrintStyleFenceCreationData::kFitToMasterModelAndXRefs :
    {
      strValue = OD_T("kFitToMasterModelAndXRefs");
    } break;

    case OdDgPrintStyleFenceCreationData::kFitToElementRange :
    {
      strValue = OD_T("kFitToElementRange");
    } break;

    case OdDgPrintStyleFenceCreationData::kFromPoints :
    {
      strValue = OD_T("kFromPoints");
    } break;

    default:
    {
      strValue = OD_T("kNone");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStylePaperOrientation value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kLandscape :
    {
      strValue = OD_T("kLandscape");
    } break;

    case OdDgPrintStyleTableRecord::kPortrait :
    {
      strValue = OD_T("kPortrait");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutUnits value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kFeet :
    {
      strValue = OD_T("kFeet");
    } break;

    case OdDgPrintStyleTableRecord::kInches :
    {
      strValue = OD_T("kInches");
    } break;

    case OdDgPrintStyleTableRecord::kMeters :
    {
      strValue = OD_T("kMeters");
    } break;

    case OdDgPrintStyleTableRecord::kDecimeters :
    {
      strValue = OD_T("kDecimeters");
    } break;

    case OdDgPrintStyleTableRecord::kCentimeters :
    {
      strValue = OD_T("kCentimeters");
    } break;

    case OdDgPrintStyleTableRecord::kMillimeters :
    {
      strValue = OD_T("kMillimeters");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutSizeScaleMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kMaximize :
    {
      strValue = OD_T("kMaximize");
    } break;

    case OdDgPrintStyleTableRecord::kSizeX :
    {
      strValue = OD_T("kSizeX");
    } break;

    case OdDgPrintStyleTableRecord::kSizeY :
    {
      strValue = OD_T("kSizeY");
    } break;

    case OdDgPrintStyleTableRecord::kScale :
    {
      strValue = OD_T("kScale");
    } break;

    case OdDgPrintStyleTableRecord::kRescaleFactor :
    {
      strValue = OD_T("kRescaleFactor");
    } break;

    case OdDgPrintStyleTableRecord::kRescaleSize :
    {
      strValue = OD_T("kRescaleSize");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutOriginMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kAutoCenter :
    {
      strValue = OD_T("kAutoCenter");
    } break;

    case OdDgPrintStyleTableRecord::kUserDefinedOrigin :
    {
      strValue = OD_T("kUserDefinedOrigin");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutMirrorMode value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kMirrorNone :
    {
      strValue = OD_T("kMirrorNone");
    } break;

    case OdDgPrintStyleTableRecord::kMirrorAboutX :
    {
      strValue = OD_T("kMirrorAboutX");
    } break;

    case OdDgPrintStyleTableRecord::kMirrorAboutY :
    {
      strValue = OD_T("kMirrorAboutY");
    } break;

    case OdDgPrintStyleTableRecord::kMirrorAboutBoth :
    {
      strValue = OD_T("kMirrorAboutX");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgPrintStyleTableRecord::OdDgPrintStyleLayoutFenceAlign value )
{
  OdString strValue;

  switch( value )
  {
    case OdDgPrintStyleTableRecord::kXAxisAlign :
    {
      strValue = OD_T("kXAxisAlign");
    } break;

    case OdDgPrintStyleTableRecord::kYAxisAlign :
    {
      strValue = OD_T("kXAxisAlign");
    } break;

    case OdDgPrintStyleTableRecord::kNoneAlign :
    {
      strValue = OD_T("kNoneAlign");
    } break;

    default:
    {
      strValue = OD_T("kUndefined");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTerrainOptions::OdDgTerrainOptionsType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTerrainOptions::kTriangles :
    {
      strValue = OD_T("kTriangles");
    } break;

    case OdDgTerrainOptions::kMajorContour :
    {
      strValue = OD_T("kMajorContour");
    } break;

    case OdDgTerrainOptions::kMinorContour :
    {
      strValue = OD_T("kMinorContour");
    } break;

    case OdDgTerrainOptions::kSourceVertex :
    {
      strValue = OD_T("kSourceVertex");
    } break;

    case OdDgTerrainOptions::kTriangleVertices :
    {
      strValue = OD_T("kTriangleVertices");
    } break;

    case OdDgTerrainOptions::kLowPoints :
    {
      strValue = OD_T("kLowPoints");
    } break;

    case OdDgTerrainOptions::kHighPoints :
    {
      strValue = OD_T("kHighPoints");
    } break;

    case OdDgTerrainOptions::kFlowArrows :
    {
      strValue = OD_T("kFlowArrows");
    } break;

    case OdDgTerrainOptions::kSourceBreakLine :
    {
      strValue = OD_T("kSourceBreakLine");
    } break;

    case OdDgTerrainOptions::kSourceHole :
    {
      strValue = OD_T("kSourceHole");
    } break;

    case OdDgTerrainOptions::kSourceIsland :
    {
      strValue = OD_T("kSourceIsland");
    } break;

    case OdDgTerrainOptions::kSourceVoid :
    {
      strValue = OD_T("kSourceVoid");
    } break;

    case OdDgTerrainOptions::kSourceBoundary :
    {
      strValue = OD_T("kSourceBoundary");
    } break;

    case OdDgTerrainOptions::kSourceContour :
    {
      strValue = OD_T("kSourceContour");
    } break;

    default:
    {
      strValue = OD_T("kUnknownSettingsValue");
    } break;
  }

  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgItemTypePropertyValue& value)
{
  switch( value.getPropertyType() )
  {
    case OdDgItemTypeProperty::kString:
      writeFieldValue(name, value.getStringValue()); break;
    case OdDgItemTypeProperty::kDouble:
      writeFieldValue(name, value.getDoubleValue()); break;
    case OdDgItemTypeProperty::kInteger:
      writeFieldValue(name, value.getIntegerValue()); break;
    case OdDgItemTypeProperty::kBoolean:
      writeFieldValue(name, value.getBooleanValue()); break;
    case OdDgItemTypeProperty::kPoint3d:
      writeFieldValue(name, value.getPointValue()); break;
    case OdDgItemTypeProperty::kDateTime:
      writeFieldValue(name, value.getDateTimeValue()); break;
    default:
      writeFieldValue(name, OdString(L"Unknown data type")); break;
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdTimeStamp& value)
{
  OdString strValue;
  value.strftime(L"%d.%m.%Y %H:%M:%S", strValue);
  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgItemTypePropertyUnits& value)
{
  OdString strValue = value.getUnitsName();
  writeFieldValue(name, strValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgItemTypePropertyInstanceValue& value)
{
  switch( value.getType() )
  {
    case OdDgItemTypePropertyInstanceValue::kBool:
      writeFieldValue(name, value.getBoolValue()); break;
    case OdDgItemTypePropertyInstanceValue::kBoolArray:
      writeFieldValue(name, value.getBoolArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kByte:
      writeFieldValue(name, value.getByteValue()); break;
    case OdDgItemTypePropertyInstanceValue::kByteArray:
      writeFieldValue(name, value.getByteArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kShort:
      writeFieldValue(name, value.getShortValue()); break;
    case OdDgItemTypePropertyInstanceValue::kShortArray:
      writeFieldValue(name, value.getShortArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kInt32:
      writeFieldValue(name, value.getIntValue()); break;
    case OdDgItemTypePropertyInstanceValue::kInt32Array:
      writeFieldValue(name, value.getIntArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kInt64:
      writeFieldValue(name, value.getInt64Value()); break;
    case OdDgItemTypePropertyInstanceValue::kInt64Array:
      writeFieldValue(name, value.getInt64ArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kString:
      writeFieldValue(name, value.getStringValue()); break;
    case OdDgItemTypePropertyInstanceValue::kStringArray:
      writeFieldValue(name, value.getStringArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kDouble:
      writeFieldValue(name, value.getDoubleValue()); break;
    case OdDgItemTypePropertyInstanceValue::kDoubleArray:
      writeFieldValue(name, value.getDoubleArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kPoint3d:
      writeFieldValue(name, value.getPoint3dValue()); break;
    case OdDgItemTypePropertyInstanceValue::kPoint3dArray:
      writeFieldValue(name, value.getPoint3dArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kDateAndTime:
      writeFieldValue(name, value.getDateTimeValue()); break;
    case OdDgItemTypePropertyInstanceValue::kDateTimeArray:
      writeFieldValue(name, value.getDateTimeArrayValue()); break;
    case OdDgItemTypePropertyInstanceValue::kComplexProperty:
    {
      OdDgItemTypeComplexPropertyInstance complexProp = value.getComplexPropertyValue();

      for( OdUInt32 i = 0; i < complexProp.size(); i++ )
      {
        writeFieldValue( name + L"->" + convertItemNameToDisplayLabel(complexProp[i].getName()), complexProp[i].getValue() );
      }
    } break;
    case OdDgItemTypePropertyInstanceValue::kComplexPropertyArray:
    {
      OdDgItemTypeComplexPropertyArrayInstance arrComplexProp = value.getComplexPropertyArrayValue();

      OdString strNumber;

      for( OdUInt32 i = 0; i < arrComplexProp.getComplexPropertyCount(); i++ )
      {
        strNumber.format(L"[%d]", i );

        OdDgItemTypeComplexPropertyInstance curProp = arrComplexProp.getComplexProperty(i);

        for( OdUInt32 j = 0; j < curProp.size(); j++ )
        {
          writeFieldValue( name + strNumber + L"->" + convertItemNameToDisplayLabel(curProp[j].getName()), curProp[j].getValue() );
        }
      }
    } break;
    default:
      writeFieldValue(name, OdString(L"Unknown data type")); break;
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<bool>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      if( value[i] )
        strData += L"true";
      else
        strData += L"false";
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdBoolArray& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      if( value[i] )
        strData += L"true";
      else
        strData += L"false";
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdInt8>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%d",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdInt8Array& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%d",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdInt16>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%d",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdInt16Array& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%d",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdInt32>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%d",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdInt32Array& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%d",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdInt64>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%i64",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdInt64Array& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%i64",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}


void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<double>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%f",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDoubleArray& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"%f",value[i] );

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdString>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      strData += L"\""+ value[i] + L"\"";
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdAnsiString>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      strData += L"\""+ OdString(value[i]) + L"\"";
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdGePoint3d>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strNumber;
      strNumber.format(L"(%f,%f,%f)",value[i].x,value[i].y,value[i].z);

      strData += strNumber;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdTimeStamp>& value)
{
  OdString strData;

  if( value.isEmpty() )
  {
    strData = L"<empty>";
  }
  else
  {
    strData = L"{ ";

    for( OdUInt32 i = 0; i < value.size(); i++ )
    {
      if( i!= 0)
        strData += L", ";

      OdString strValue;
      value[i].strftime(L"(%d.%m.%Y %H:%M:%S)", strValue);

      strData += strValue;
    }

    strData += L" }";
  }

  writeFieldValue( name, strData );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgEdgeRunTableElementSchema::OdDgEdgeRunSide& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgEdgeRunTableElementSchema::kTopEdge :
    {
      strValue = OD_T("kTopEdge");
    } break;

    case OdDgEdgeRunTableElementSchema::kBottomEdge :
    {
      strValue = OD_T("kBottomEdge");
    } break;

    case OdDgEdgeRunTableElementSchema::kLeftEdge :
    {
      strValue = OD_T("kLeftEdge");
    } break;

    case OdDgEdgeRunTableElementSchema::kRightEdge :
    {
      strValue = OD_T("kRightEdge");
    } break;

    default:
    {
      strValue.format(OD_T("UnknownValue_%d"), value);
    } break;
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTableCellElement::OdDgTableCellElementTextAlignment& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTableCellElement::kLeftTop :
    {
      strValue = OD_T("kLeftTop");
    } break;

    case OdDgTableCellElement::kLeftMiddle :
    {
      strValue = OD_T("kLeftMiddle");
    } break;

    case OdDgTableCellElement::kLeftBottom :
    {
      strValue = OD_T("kLeftBottom");
    } break;

    case OdDgTableCellElement::kCenterTop :
    {
      strValue = OD_T("kCenterTop");
    } break;

    case OdDgTableCellElement::kCenterMiddle :
    {
      strValue = OD_T("kCentertMiddle");
    } break;

    case OdDgTableCellElement::kCenterBottom :
    {
      strValue = OD_T("kCenterBottom");
    } break;

    case OdDgTableCellElement::kRightTop :
    {
      strValue = OD_T("kRightTop");
    } break;

    case OdDgTableCellElement::kRightMiddle :
    {
      strValue = OD_T("kRightMiddle");
    } break;

    case OdDgTableCellElement::kRightBottom :
    {
      strValue = OD_T("kRightBottom");
    } break;

    default:
    {
      strValue.format(OD_T("UnknownValue_%d"), value);
    } break;
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTableCellElement::OdDgTableCellElementTextOrientation& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTableCellElement::kHorizontal :
    {
      strValue = OD_T("kHorizontal");
    } break;

    case OdDgTableCellElement::kRotateRight :
    {
      strValue = OD_T("kRotateRight");
    } break;

    case OdDgTableCellElement::kRotateLeft :
    {
      strValue = OD_T("kRotateLeft");
    } break;

    case OdDgTableCellElement::kVertical :
    {
      strValue = OD_T("kVertical");
    } break;

    default:
    {
      strValue.format(OD_T("UnknownValue_%d"), value);
    } break;
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTableElement::OdDgTableElementBreakType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTableElement::kHorizontalBreak :
    {
      strValue = OD_T("kHorizontalBreak");
    } break;

    case OdDgTableElement::kNoBreak :
    {
      strValue = OD_T("kNoBreak");
    } break;

    default:
    {
      strValue.format(OD_T("UnknownValue_%d"), value);
    } break;
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTableElement::OdDgTableElementSubTablePosition& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTableElement::kRight :
    {
      strValue = OD_T("kRight");
    } break;

    case OdDgTableElement::kLeft :
    {
      strValue = OD_T("kLeft");
    } break;

    case OdDgTableElement::kAbove :
    {
      strValue = OD_T("kAbove");
    } break;

    case OdDgTableElement::kBelow :
    {
      strValue = OD_T("kBelow");
    } break;

    case OdDgTableElement::kManual :
    {
      strValue = OD_T("kManual");
    } break;

    default:
    {
      strValue.format(OD_T("UnknownValue_%d"), value);
    } break;
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgTableSchemaDescriptionItem::OdDgTableSchemaDescriptionItemType& value)
{
  OdString strValue;

  switch( value )
  {
    case OdDgTableSchemaDescriptionItem::kGroup :
    {
      strValue = OD_T("kGroup");
    } break;

    case OdDgTableSchemaDescriptionItem::kBoolean :
    {
      strValue = OD_T("kBoolean");
    } break;

    case OdDgTableSchemaDescriptionItem::kDouble :
    {
      strValue = OD_T("kDouble");
    } break;

    case OdDgTableSchemaDescriptionItem::kUInt32 :
    {
      strValue = OD_T("kUInt32");
    } break;

    case OdDgTableSchemaDescriptionItem::kPoint3d :
    {
      strValue = OD_T("kPoint3d");
    } break;

    case OdDgTableSchemaDescriptionItem::kString :
    {
      strValue = OD_T("kString");
    } break;

    case OdDgTableSchemaDescriptionItem::kDateTime :
    {
      strValue = OD_T("kDateTime");
    } break;

    case OdDgTableSchemaDescriptionItem::kExpressionDesc :
    {
      strValue = OD_T("kExpressionDesc");
    } break;

    default:
    {
      strValue.format(OD_T("UnknownValue_%d"), value);
    } break;
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgVariable::OdDgVariableType type )
{
  OdString strValue = L"kProxy";

  switch( type )
  {
    case OdDgVariable::kDistance: strValue = L"kDistance"; break;
    case OdDgVariable::kAngle: strValue = L"kAngle"; break;
    case OdDgVariable::kNumber: strValue = L"kNumber"; break;
    case OdDgVariable::kInteger: strValue = L"kInteger"; break;
    case OdDgVariable::kBoolean: strValue = L"kBoolean"; break;
    case OdDgVariable::kArea: strValue = L"kArea"; break;
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgVariable::OdDgVariableScope type )
{
  if( type == OdDgVariable::kDefinition )
    writeFieldValue(name, OdString(L"kDefinition") );
  else
    writeFieldValue(name, OdString(L"kInstance") );
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgVariableValueVariant type )
{
  switch( type.getType() )
  {
    case OdDgVariable::kInteger:
      writeFieldValue(name, type.getIntegerValue() ); break;
    case OdDgVariable::kBoolean:
      writeFieldValue(name, type.getBooleanValue() ); break;
    case OdDgVariable::kProxy:
      writeFieldValue(name, OdString(L"<Error!>") ); break;
    default:
      writeFieldValue(name, type.getDoubleValue() ); break;
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name, OdDgItemTypeBasedVariable::OdDgItemTypeBasedVariableUnits type )
{
  switch( type )
  {
    case OdDgItemTypeBasedVariable::kAcre:
      writeFieldValue(name, OdString(OD_T("kAcre")) ); break;
    case OdDgItemTypeBasedVariable::kCentimetreSquared:
      writeFieldValue(name, OdString(OD_T("kCentimetreSquared")) ); break;
    case OdDgItemTypeBasedVariable::kFootFoot:
      writeFieldValue(name, OdString(OD_T("kFootFoot")) ); break;
    case OdDgItemTypeBasedVariable::kFootMile:
      writeFieldValue(name, OdString(OD_T("kFootMile")) ); break;
    case OdDgItemTypeBasedVariable::kFootSquared:
      writeFieldValue(name, OdString(OD_T("kFootSquared")) ); break;
    case OdDgItemTypeBasedVariable::kHectare:
      writeFieldValue(name, OdString(OD_T("kHectare")) ); break;
    case OdDgItemTypeBasedVariable::kInchFoot:
      writeFieldValue(name, OdString(OD_T("kInchFoot")) ); break;
    case OdDgItemTypeBasedVariable::kInchMetre:
      writeFieldValue(name, OdString(OD_T("kInchMetre")) ); break;
    case OdDgItemTypeBasedVariable::kInchMile:
      writeFieldValue(name, OdString(OD_T("kInchMile")) ); break;
    case OdDgItemTypeBasedVariable::kInchSquared:
      writeFieldValue(name, OdString(OD_T("kInchSquared")) ); break;
    case OdDgItemTypeBasedVariable::kKilometreSquared:
      writeFieldValue(name, OdString(OD_T("kKilometreSquared")) ); break;
    case OdDgItemTypeBasedVariable::kMeterKilometre:
      writeFieldValue(name, OdString(OD_T("kMeterKilometre")) ); break;
    case OdDgItemTypeBasedVariable::kMetreMetre:
      writeFieldValue(name, OdString(OD_T("kMetreMetre")) ); break;
    case OdDgItemTypeBasedVariable::kMetreSquared:
      writeFieldValue(name, OdString(OD_T("kMetreSquared")) ); break;
    case OdDgItemTypeBasedVariable::kMileSquared:
      writeFieldValue(name, OdString(OD_T("kMileSquared")) ); break;
    case OdDgItemTypeBasedVariable::kMillimetreKilometre:
      writeFieldValue(name, OdString(OD_T("kMillimetreKilometre")) ); break;
    case OdDgItemTypeBasedVariable::kMillimetreMetre:
      writeFieldValue(name, OdString(OD_T("kMillimetreMetre")) ); break;
    case OdDgItemTypeBasedVariable::kMillimetreMile:
      writeFieldValue(name, OdString(OD_T("kMillimetreMile")) ); break;
    case OdDgItemTypeBasedVariable::kMillimetreSquared:
      writeFieldValue(name, OdString(OD_T("kMillimetreSquared")) ); break;
    case OdDgItemTypeBasedVariable::kYardSquared:
      writeFieldValue(name, OdString(OD_T("kYardSquared")) ); break;
    case OdDgItemTypeBasedVariable::kDegree:
      writeFieldValue(name, OdString(OD_T("kDegree")) ); break;
    case OdDgItemTypeBasedVariable::kRadian:
      writeFieldValue(name, OdString(OD_T("kRadian")) ); break;
    case OdDgItemTypeBasedVariable::kAngleMinute:
      writeFieldValue(name, OdString(OD_T("kAngleMinute")) ); break;
    case OdDgItemTypeBasedVariable::kAngleQuadrant:
      writeFieldValue(name, OdString(OD_T("kAngleQuadrant")) ); break;
    case OdDgItemTypeBasedVariable::kAngleSecond:
      writeFieldValue(name, OdString(OD_T("kAngleSecond")) ); break;
    case OdDgItemTypeBasedVariable::kRevolution:
      writeFieldValue(name, OdString(OD_T("kRevolution")) ); break;
    case OdDgItemTypeBasedVariable::kGradian:
      writeFieldValue(name, OdString(OD_T("kGradian")) ); break;
    case OdDgItemTypeBasedVariable::kMetre:
      writeFieldValue(name, OdString(OD_T("kMetre")) ); break;
    case OdDgItemTypeBasedVariable::kMicroInch:
      writeFieldValue(name, OdString(OD_T("kMicroInch")) ); break;
    case OdDgItemTypeBasedVariable::kMicroMetre:
      writeFieldValue(name, OdString(OD_T("kMicroMetre")) ); break;
    case OdDgItemTypeBasedVariable::kCentimetre:
      writeFieldValue(name, OdString(OD_T("kCentimetre")) ); break;
    case OdDgItemTypeBasedVariable::kDecimetre:
      writeFieldValue(name, OdString(OD_T("kDecimetre")) ); break;
    case OdDgItemTypeBasedVariable::kFoot:
      writeFieldValue(name, OdString(OD_T("kFoot")) ); break;
    case OdDgItemTypeBasedVariable::kInch:
      writeFieldValue(name, OdString(OD_T("kInch")) ); break;
    case OdDgItemTypeBasedVariable::kKilometre:
      writeFieldValue(name, OdString(OD_T("kKilometre")) ); break;
    case OdDgItemTypeBasedVariable::kMilliFoot:
      writeFieldValue(name, OdString(OD_T("kMilliFoot")) ); break;
    case OdDgItemTypeBasedVariable::kMile:
      writeFieldValue(name, OdString(OD_T("kMile")) ); break;
    case OdDgItemTypeBasedVariable::kMilliInch:
      writeFieldValue(name, OdString(OD_T("kMilliInch")) ); break;
    case OdDgItemTypeBasedVariable::kMilliMetre:
      writeFieldValue(name, OdString(OD_T("kMilliMetre")) ); break;
    case OdDgItemTypeBasedVariable::kYard:
      writeFieldValue(name, OdString(OD_T("kYard")) ); break;
    default:
      writeFieldValue(name, OdString(OD_T("kMasterUnits")) ); break;
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgBECXMLVariant::OdDgBECXMLVariantColor& value)
{
  OdString strFieldValue;

  if( (value.m_uDataFlags & OdDgBECXMLVariant::kColorFlags) && value.m_varColorFlags )
  {
    if( value.m_varColorFlags == 1 )
    {
      strFieldValue = L"ByLevel";
    }
    else if( value.m_varColorFlags == 2 )
    {
      strFieldValue = L"ByCell";
    }
  }
  else if( value.m_uDataFlags & OdDgBECXMLVariant::kColorBookName )
  {
    strFieldValue.format(L"( %d, %d )",value.m_varColorBookIndex, value.m_varColorIndexInBook );
    strFieldValue = L"ColorBook: " + value.m_varColorBookName + L" " + strFieldValue;
  }
  else if( value.m_uDataFlags & OdDgBECXMLVariant::kColorRGB )
  {
    strFieldValue.format(L"RGB : %08X",value.m_varRGB );
  }
  else if( value.m_uDataFlags & OdDgBECXMLVariant::kColorIndex )
  {
    strFieldValue.format(L"ColorIndex : %d",value.m_varColorIndex );
  }

  writeFieldValue(name, strFieldValue);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdArray<OdDgBECXMLVariant::OdDgBECXMLVariantColor>& value)
{
  OdString strFieldTotal = L"{";

  for( OdUInt32 i = 0; i < value.size(); i++ )
  {
    OdString strFieldValue;

    if( (value[i].m_uDataFlags & OdDgBECXMLVariant::kColorFlags) && value[i].m_varColorFlags )
    {
      if( value[i].m_varColorFlags == 1 )
      {
        strFieldValue = L"ByLevel";
      }
      else if( value[i].m_varColorFlags == 2 )
      {
        strFieldValue = L"ByCell";
      }
    }
    else if( value[i].m_uDataFlags & OdDgBECXMLVariant::kColorBookName )
    {
      strFieldValue.format(L"( %d, %d )",value[i].m_varColorBookIndex, value[i].m_varColorIndexInBook );
      strFieldValue = L"ColorBook: " + value[i].m_varColorBookName + L" " + strFieldValue;
    }
    else if( value[i].m_uDataFlags & OdDgBECXMLVariant::kColorRGB )
    {
      strFieldValue.format(L"RGB : %08X",value[i].m_varRGB );
    }
    else if( value[i].m_uDataFlags & OdDgBECXMLVariant::kColorIndex )
    {
      strFieldValue.format(L"ColorIndex : %d",value[i].m_varColorIndex );
    }

    strFieldValue = L"(" + strFieldValue + L"),";

    strFieldTotal += strFieldValue;
  }

  strFieldTotal = strFieldTotal.left(strFieldTotal.getLength() - 1) + L"}";

  writeFieldValue(name, strFieldTotal);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgBinXMLNode* pValue)
{
  OdString strSpaces;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    if( name.getAt(i) == L' ')
      strSpaces += L' ';
    else
      break;
  }

  dumpFieldName( strSpaces + L"+ " + pValue->getName() );

  strSpaces += L"  ";

  for( OdUInt32 j = 0; j < pValue->getChildCount(); j++ )
  {
    OdDgBinXMLItemPtr pItem = pValue->getChildItem(j);

    if( pItem->isXmlNode() )
    {
      OdDgBinXMLNode* pNode = (OdDgBinXMLNode*)(pItem.get());
      writeFieldValue( strSpaces, pNode );
    }
    else
    {
      OdDgBinXMLValue* pValueItem = (OdDgBinXMLValue*)(pItem.get());
      writeFieldValue( strSpaces, pValueItem );
    }
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgBinXMLValue* pValue)
{
  OdString strSpaces;

  for( OdInt32 i = 0; i < name.getLength(); i++ )
  {
    if( name.getAt(i) == L' ')
      strSpaces += L' ';
    else
      break;
  }

  OdString strName = strSpaces + L"- " + pValue->getName();

  OdDgBECXMLVariant var = pValue->getValue();

  if( var.m_uFlags & OdDgBECXMLVariant::kBool )
    writeFieldValue(strName, var.m_varBool);
  else if( var.m_uFlags & OdDgBECXMLVariant::kBoolArray )
    writeFieldValue(strName, var.m_varBoolArray);
  else if( var.m_uFlags & OdDgBECXMLVariant::kByte )
    writeFieldValue(strName, var.m_varByte);
  else if( var.m_uFlags & OdDgBECXMLVariant::kByteOrBool )
    writeFieldValue(strName, var.m_varByte);
  else if( var.m_uFlags & OdDgBECXMLVariant::kByteArray )
    writeFieldValue(strName, var.m_varByteArray);
  else if( var.m_uFlags & OdDgBECXMLVariant::kShort )
    writeFieldValue(strName, var.m_varShort);
  else if( var.m_uFlags & OdDgBECXMLVariant::kShortArray )
    writeFieldValue(strName, var.m_varShortArray);
  else if( var.m_uFlags & OdDgBECXMLVariant::kInt32 )
    writeFieldValue(strName, var.m_varInt32);
  else if( var.m_uFlags & OdDgBECXMLVariant::kInt32Array )
    writeFieldValue(strName, var.m_varInt32Array);
  else if( var.m_uFlags & OdDgBECXMLVariant::kInt64 )
    writeFieldValue(strName, var.m_varInt64);
  else if( var.m_uFlags & OdDgBECXMLVariant::kInt64Array )
    writeFieldValue(strName, var.m_varInt64Array);
  else if( var.m_uFlags & OdDgBECXMLVariant::kDouble )
    writeFieldValue(strName, var.m_varDouble);
  else if( var.m_uFlags & OdDgBECXMLVariant::kDoubleArray )
    writeFieldValue(strName, var.m_varDoubleArray);
  else if( var.m_uFlags & OdDgBECXMLVariant::kString )
    writeFieldValue(strName, var.m_varString);
  else if( var.m_uFlags & OdDgBECXMLVariant::kStringArray )
    writeFieldValue(strName, var.m_varStringArray);
  else if( var.m_uFlags & OdDgBECXMLVariant::kPoint3d )
    writeFieldValue(strName, var.m_varPoint3d);
  else if( var.m_uFlags & OdDgBECXMLVariant::kPoint3dArray )
    writeFieldValue(strName, var.m_varPoint3dArray);
  else if( var.m_uFlags & OdDgBECXMLVariant::kDateAndTime )
    writeFieldValue(strName, var.m_varDateTime);
  else if( var.m_uFlags & OdDgBECXMLVariant::kDateTimeArray )
    writeFieldValue(strName, var.m_varDateTimeArray);
  else if( var.m_uFlags & OdDgBECXMLVariant::kColorData )
    writeFieldValue(strName, var.m_varColorData);
  else if( var.m_uFlags & OdDgBECXMLVariant::kColorDataArray )
    writeFieldValue(strName, var.m_varColorDataArray);
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgSchemaItemVariant& value)
{
  switch( value.getType() )
  {
    case OdDgSchemaItemVariant::kEmptyVar:
      writeFieldValue( name, OdString(OD_T("<Empty>")) ); break;
    case OdDgSchemaItemVariant::kStringVar:
      writeFieldValue( name, value.getStringValue() ); break;
    case OdDgSchemaItemVariant::kDoubleVar:
      writeFieldValue( name, value.getDoubleValue() ); break;
    case OdDgSchemaItemVariant::kInt32Var:
      writeFieldValue( name, value.getInt32Value() ); break;
    case OdDgSchemaItemVariant::kBoolVar:
      writeFieldValue( name, value.getBoolValue() ); break;
    case OdDgSchemaItemVariant::kPoint3dVar:
      writeFieldValue( name, value.getPoint3dValue() ); break;
    case OdDgSchemaItemVariant::kDateTimeVar:
      writeFieldValue( name, value.getDateTimeValue() ); break;
    case OdDgSchemaItemVariant::kItemGroupVar:
    {
      OdDgItemGroupVariable varData = value.getItemGroupValue();
      OdString strVal;

      OdUInt32Array arrIndexes = varData.getGroupItemIdArray();

      for( OdUInt32 i = 0; i < arrIndexes.size(); i++)
      {
        OdString strIndex;

        if( i < arrIndexes.size() - 1 )
          strIndex.format(L"%d,",arrIndexes[i]);
        else
          strIndex.format(L"%d",arrIndexes[i]);

        strVal += strIndex;
      }

      if( arrIndexes.isEmpty() )
        writeFieldValue( name, OdString(L"<Empty>") );
      else
        writeFieldValue( name, strVal );
    } break;

    case OdDgSchemaItemVariant::kStringArrayVar:
    {
      OdArray<OdString> varData = value.getStringArrayValue();
      OdString strVal = L"( ";

      for( OdUInt32 i = 0; i < varData.size(); i++)
        strVal += L"\""+ varData[i] + L"\",";

      if( varData.isEmpty() )
        writeFieldValue( name, OdString(L"<Empty>") );
      else
      {
        strVal = strVal.left(strVal.getLength()-1) + L" )";
        writeFieldValue( name, strVal );
      }
    } break;

    case OdDgSchemaItemVariant::kDoubleArrayVar:
    {
      OdArray<double> varData = value.getDoubleArrayValue();
      OdString strVal = L"( ";

      for( OdUInt32 i = 0; i < varData.size(); i++)
      {
        OdString strItem;
        strItem.format(L"%f,", varData[i]);
        strVal += strItem;
      }

      if( varData.isEmpty() )
        writeFieldValue( name, OdString(L"<Empty>") );
      else
      {
        strVal = strVal.left(strVal.getLength()-1) + L" )";
        writeFieldValue( name, strVal );
      }
    } break;

    case OdDgSchemaItemVariant::kInt32ArrayVar:
    {
      OdArray<OdInt32> varData = value.getInt32ArrayValue();
      OdString strVal = L"( ";

      for( OdUInt32 i = 0; i < varData.size(); i++)
      {
        OdString strItem;
        strItem.format(L"%d,", varData[i]);
        strVal += strItem;
      }

      if( varData.isEmpty() )
        writeFieldValue( name, OdString(L"<Empty>") );
      else
      {
        strVal = strVal.left(strVal.getLength()-1) + L" )";
        writeFieldValue( name, strVal );
      }
    } break;

    case OdDgSchemaItemVariant::kBoolArrayVar:
    {
      OdArray<bool> varData = value.getBoolArrayValue();
      OdString strVal = L"( ";

      for( OdUInt32 i = 0; i < varData.size(); i++)
        varData[i] ? strVal += L"true," : strVal += L"false,";

      if( varData.isEmpty() )
        writeFieldValue( name, OdString(L"<Empty>") );
      else
      {
        strVal = strVal.left(strVal.getLength()-1) + L" )";
        writeFieldValue( name, strVal );
      }
    } break;

    case OdDgSchemaItemVariant::kPoint3dArrayVar:
    {
      OdArray<OdGePoint3d> varData = value.getPoint3dArrayValue();
      OdString strVal = L"( ";

      for( OdUInt32 i = 0; i < varData.size(); i++)
      {
        OdString strItem;
        strItem.format(L"<%f,%f,%f>,", varData[i].x, varData[i].y, varData[i].z);
        strVal += strItem;
      }

      if( varData.isEmpty() )
        writeFieldValue( name, OdString(L"<Empty>") );
      else
      {
        strVal = strVal.left(strVal.getLength()-1) + L" )";
        writeFieldValue( name, strVal );
      }
    } break;

    case OdDgSchemaItemVariant::kDateTimeArrayVar:
    {
      OdArray<OdTimeStamp> varData = value.getDateTimeArrayValue();
      OdString strVal = L"( ";

      for( OdUInt32 i = 0; i < varData.size(); i++)
      {
        OdString strItem;
        varData[i].strftime(L"<%d.%m.%Y %H:%M:%S>,", strItem);
        strVal += strItem;
      }

      if( varData.isEmpty() )
        writeFieldValue( name, OdString(L"<Empty>") );
      else
      {
        strVal = strVal.left(strVal.getLength()-1) + L" )";
        writeFieldValue( name, strVal );
      }
    } break;

    case OdDgSchemaItemVariant::kExpressionGroupVar:
    {
      OdDgExpressionGroupVariable varData = value.getExpressionGroupValue();

      if( varData.getExpressionCount() == 0 )
        writeFieldValue( name, OdString(L"<Empty>") );
      else
      {
        OdString strPrefix = L" ";

        for( OdInt32 k = 0; k < name.getLength(); k++ )
        {
          if( name.getAt(k) == L' ' )
            strPrefix = L" " + strPrefix;
          else
            break;
        }

        for( OdUInt32 i = 0; i < varData.getExpressionCount(); i++ )
        {
          // Convert to string form. But now we dump them as binary data.
          OdDgExpressionVariablePtr pExpression = varData.getExpression(i);

          OdString strExpression;
          strExpression.format(L"Expression %d:", i);
          writeFieldValue( strPrefix + L" = " + strExpression, pExpression->getResultVariableName() + L" = " + pExpression->getExpressionString() );
        }
      }
    } break;
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name,  const OdDgGeometryCacheActionPtr& pValue)
{
  OdString strName;
  OdString strValue;

  switch( pValue->getType() )
  {
    case OdDgGeometryCacheAction::kBrepData:
    {
      strName = L"kBrepData";

      OdDgBrepGeometryCacheActionPtr pBrepCache = pValue;
      strValue.format(L"Parasolid stream size = %d", pBrepCache->getBrepData().size() );
    } break;

    case OdDgGeometryCacheAction::kBSplineSurface:
    {
      strName = L"kBSplineSurface";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kCone:
    {
      strName = L"kCylinder";
      OdDgConeGeometryCacheActionPtr pCylinderCache = pValue;
      OdGeVector3d vrRef = pCylinderCache->getRefVector();
      OdGeVector3d vrDir = pCylinderCache->getYDirectionVector();
      OdGePoint3d  ptOrigin1 = pCylinderCache->getStartOrigin();
      OdGePoint3d  ptOrigin2 = pCylinderCache->getEndOrigin();
      strValue.format(L"Ref Vector (%f, %f, %f), Dir Vector (%f, %f, %f), Start Origin (%f, %f, %f), End Origin (%f, %f, %f), Start radius = %f, End radius = %f",
                      vrRef.x, vrRef.y, vrRef.z, vrDir.x, vrDir.y, vrDir.z,
                      ptOrigin1.x, ptOrigin1.y, ptOrigin1.z, ptOrigin2.x, ptOrigin2.y, ptOrigin2.z,
                      pCylinderCache->getStartRadius(), pCylinderCache->getEndRadius()
                     );
    } break;

    case OdDgGeometryCacheAction::kMesh:
    {
      strName = L"kMesh";

      OdDgMeshGeometryCacheActionPtr pMeshCache = pValue;
      strValue.format(L"%d faces, %d vertices", pMeshCache->getFaceCount(), pMeshCache->getVertexCount() );
    } break;

    case OdDgGeometryCacheAction::kArc3d:
    {
      strName = L"kArc3d";

      OdDgArc3dGeometryCacheActionPtr pArcCache = pValue;
      OdGePoint3d ptOrigin = pArcCache->getOrigin();
      strValue.format(L"origin = (%f,%f,%f), radius = %d",ptOrigin.x, ptOrigin.y, ptOrigin.z, pArcCache->getPrimaryRadius() );
    } break;

    case OdDgGeometryCacheAction::kArc2d:
    {
      strName = L"kArc2d";

      OdDgArc2dGeometryCacheActionPtr pArcCache = pValue;
      OdGePoint2d ptOrigin = pArcCache->getOrigin();
      strValue.format(L"origin = (%f,%f), radius = %d",ptOrigin.x, ptOrigin.y, pArcCache->getPrimaryRadius() );
    } break;

    case OdDgGeometryCacheAction::kEllipse3d:
    {
      strName = L"kEllipse3d";

      OdDgEllipse3dGeometryCacheActionPtr pEllipseCache = pValue;
      OdGePoint3d ptOrigin = pEllipseCache->getOrigin();
      strValue.format(L"origin = (%f,%f,%f), radius1 = %d, radius2 = %d",ptOrigin.x, ptOrigin.y, ptOrigin.z, pEllipseCache->getPrimaryRadius(), pEllipseCache->getSecondaryRadius() );
    } break;

    case OdDgGeometryCacheAction::kTorus:
    {
      strName = L"kTorus";

      OdDgTorusGeometryCacheActionPtr pTorusCache = pValue;
      OdGePoint3d ptOrigin = pTorusCache->getOrigin();
      strValue.format(L"origin = (%f,%f,%f), radius1 = %d, radius2 = %d",ptOrigin.x, ptOrigin.y, ptOrigin.z, pTorusCache->getPrimaryRadius(), pTorusCache->getSecondaryRadius() );
    } break;

    case OdDgGeometryCacheAction::kSphere:
    {
      strName = L"kSphere";

      OdDgSphereGeometryCacheActionPtr pSphereCache = pValue;
      OdGePoint3d ptOrigin = pSphereCache->getOrigin();
      strValue.format(L"origin = (%f,%f,%f), radius1 = %d, radius2 = %d",ptOrigin.x, ptOrigin.y, ptOrigin.z, pSphereCache->getPrimaryRadius(), pSphereCache->getSecondaryRadius() );
    } break;

    case OdDgGeometryCacheAction::kHatchPattern:
    {
      strName = L"kHatchPattern";

      OdDgHatchPatternGeometryCacheActionPtr pHatchCache = pValue;
      strValue.format(L"number of cache items = %d",pHatchCache->getGeometryItemsCount() );
    } break;

    case OdDgGeometryCacheAction::kSlabSolid:
    {
      strName = L"kSlabSolid";

      OdDgSlabSolidGeometryCacheActionPtr pBoxCache = pValue;
      OdGePoint3d ptOrigin1 = pBoxCache->getStartOrigin();
      OdGePoint3d ptOrigin2 = pBoxCache->getEndOrigin();
      strValue.format(L"origin1 = (%f,%f,%f), origin2 = (%f,%f,%f)",ptOrigin1.x, ptOrigin1.y, ptOrigin1.z, ptOrigin2.x, ptOrigin2.y, ptOrigin2.z );
    } break;

    case OdDgGeometryCacheAction::kEllipse2d:
    {
      strName = L"kEllipse2d";

      OdDgEllipse2dGeometryCacheActionPtr pEllipseCache = pValue;
      OdGePoint2d ptOrigin = pEllipseCache->getOrigin();
      strValue.format(L"origin = (%f,%f), radius1 = %d, radius2 = %d",ptOrigin.x, ptOrigin.y, pEllipseCache->getPrimaryRadius(), pEllipseCache->getSecondaryRadius() );
    } break;

    case OdDgGeometryCacheAction::kBSplineCurve3d:
    {
      strName = L"kBSplineCurve3d";

      OdDgBSplineCurve3dGeometryCacheActionPtr pCurveCache = pValue;
      strValue.format(L"Order %d, %d poles, %d knots", pCurveCache->getOrder(), pCurveCache->getControlPoints().size(),
        pCurveCache->getKnots().length() );
    } break;

    case OdDgGeometryCacheAction::kPolyline3d:
    {
      strName = L"kPolyline3d";

      OdDgPolyline3dGeometryCacheActionPtr pPolylineCache = pValue;

      for( OdUInt32 i = 0; i < pPolylineCache->getVertexCount(); i++ )
      {
        OdString strToAdd;
        OdGePoint3d ptVertex = pPolylineCache->getVertex(i);
        strToAdd.format(L"(%f,%f,%f) -> ", ptVertex.x, ptVertex.y, ptVertex.y);
        strValue += strToAdd;
      }

      if( strValue.getLength() > 4 )
      {
        strValue = strValue.left( strValue.getLength() - 4);
      }
    } break;

    case OdDgGeometryCacheAction::kPolyline2d:
    {
      strName = L"kPolyline2d";

      OdDgPolyline2dGeometryCacheActionPtr pPolylineCache = pValue;

      for( OdUInt32 i = 0; i < pPolylineCache->getVertexCount(); i++ )
      {
        OdString strToAdd;
        OdGePoint2d ptVertex = pPolylineCache->getVertex(i);
        strToAdd.format(L"(%f,%f) -> ", ptVertex.x, ptVertex.y);
        strValue += strToAdd;
      }

      if( strValue.getLength() > 4 )
      {
        strValue = strValue.left( strValue.getLength() - 4);
      }
    } break;

    case OdDgGeometryCacheAction::kPolygon3d:
    {
      strName = L"kPolygon3d";

      OdDgPolygon3dGeometryCacheActionPtr pPolylineCache = pValue;

      for( OdUInt32 i = 0; i < pPolylineCache->getVertexCount(); i++ )
      {
        OdString strToAdd;
        OdGePoint3d ptVertex = pPolylineCache->getVertex(i);
        strToAdd.format(L"(%f,%f,%f) -> ", ptVertex.x, ptVertex.y, ptVertex.y);
        strValue += strToAdd;
      }

      if( strValue.getLength() > 4 )
      {
        strValue = strValue.left( strValue.getLength() - 4);
      }
    } break;

    case OdDgGeometryCacheAction::kPolygon2d:
    {
      strName = L"kPolygon2d";

      OdDgPolygon2dGeometryCacheActionPtr pPolylineCache = pValue;

      for( OdUInt32 i = 0; i < pPolylineCache->getVertexCount(); i++ )
      {
        OdString strToAdd;
        OdGePoint2d ptVertex = pPolylineCache->getVertex(i);
        strToAdd.format(L"(%f,%f) -> ", ptVertex.x, ptVertex.y);
        strValue += strToAdd;
      }

      if( strValue.getLength() > 4 )
      {
        strValue = strValue.left( strValue.getLength() - 4);
      }
    } break;

    case OdDgGeometryCacheAction::kPushTransform:
    {
      strName = L"kPushTransform";

      OdDgPushTransformGeometryCacheActionPtr pCache = pValue;

      OdGeMatrix3d mat = pCache->getTransformMatrix();

      strValue.format(L"((%f,%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f,%f))",
                      mat[0][0],mat[0][1],mat[0][2],mat[0][3],
                      mat[1][0],mat[1][1],mat[1][2],mat[1][3],
                      mat[2][0],mat[2][1],mat[2][2],mat[2][3],
                      mat[3][0],mat[3][1],mat[3][2],mat[3][3]
                     );
    } break;

    case OdDgGeometryCacheAction::kPopTransform:
    {
      strName  = L"kPopTransform";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kBeginSolid:
    {
      strName  = L"kBeginSolid";
      OdDgBeginSolidGeometryCacheActionPtr pCache = pValue;
      strValue.format(L"isSolid = %d", pCache->isSolid() );
    } break;

    case OdDgGeometryCacheAction::kBeginExtrusionSolid:
    {
      strName  = L"kBeginExtrusionSolid";
      OdDgBeginExtrusionSolidGeometryCacheActionPtr pCache = pValue;
      OdGeVector3d vrExtrusion = pCache->getExtrusionVector();
      strValue.format(L"isSolid = %d, vrExtrusion = (%f,%f,%f)", pCache->isSolid(), vrExtrusion.x, vrExtrusion.y, vrExtrusion.z );
    } break;

    case OdDgGeometryCacheAction::kBeginRevolutionSolid:
    {
      strName  = L"kBeginRevolutionSolid";
      OdDgBeginRevolutionSolidGeometryCacheActionPtr pCache = pValue;
      strValue.format(L"isSolid = %d", pCache->getSolidFlag() );
    } break;

    case OdDgGeometryCacheAction::kEndSolid:
    {
      strName  = L"kEndSolid";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kModifiedProfileBegin:
    {
      strName  = L"kModifiedProfileBegin";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kModifiedProfileEnd:
    {
      strName  = L"kModifiedProfileEnd";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kBeginComplexShapeOrGrHole:
    {
      strName  = L"kBeginComplex";
      OdDgBeginComplexGeometryCacheActionPtr pCache = pValue;
      strValue.format(L"subType = %d", pCache->getActionSubtype() );
    } break;

    case OdDgGeometryCacheAction::kBeginComplexString:
    {
      strName  = L"kBeginComplexString";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kEndComplex:
    {
      strName  = L"kEndComplex";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kMarkHole:
    {
      strName  = L"kMarkHole";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kSetTrainsData:
    {
      strName = L"kSetTraitsData";

      OdDgSetTraitsDataGeometryCacheActionPtr pCache = pValue;

      strValue = L"(";

      if( pCache->getColorFlag() )
      {
        OdString strToAdd;
        strToAdd.format(L"Color = %d, ", pCache->getColorIndex());
        strValue += strToAdd;
      }

      if( pCache->getFillColorFlag() )
      {
        OdString strToAdd;
        strToAdd.format(L"FillColor = %d, ", pCache->getFillColorIndex());
        strValue += strToAdd;
      }

      if( pCache->getLineWeightFlag() )
      {
        OdString strToAdd;
        strToAdd.format(L"LineWeight = %d, ", pCache->getLineWeight());
        strValue += strToAdd;
      }

      if( pCache->getLineTypeFlag() )
      {
        OdString strToAdd;
        strToAdd.format(L"LineType = %d, ", pCache->getLineType());
        strValue += strToAdd;
      }

      if( pCache->getFillModeFlag() )
      {
        OdString strToAdd;
        strToAdd.format(L"FillMode = %d, ", pCache->getFillMode());
        strValue += strToAdd;
      }

      if( pCache->getTransparencyFlag() )
      {
        OdString strToAdd;
        strToAdd.format(L"Transparency = %f, ", pCache->getTransparency());
        strValue += strToAdd;
      }

      if( pCache->getSubitemIndexFlag() )
      {
        OdString strSubitems;
        strSubitems.format(L"subItemIndex = %d, ", pCache->getSubitemIndex() );
        strValue += strSubitems;
      }

      if( pCache->getMaterialIdFlag() )
      {
        OdString strToAdd;
        strToAdd.format(L"MaterialId = %d, ", pCache->getMateralId());
        strValue += strToAdd;
      }

      if( pCache->getAlternativeRGBColor() )
      {
        OdString strToAdd;
        strToAdd.format(L"AlternativeRGBColor = (%d,%d,%d), ", ODGETRED(pCache->getAlternativeRGBColor()), 
          ODGETGREEN(pCache->getAlternativeRGBColor()), ODGETBLUE(pCache->getAlternativeRGBColor()) );
        strValue += strToAdd;
      }

      if( strValue.getLength() > 2 )
        strValue = strValue.left(strValue.getLength()-2);
      else
        strValue += "Reset All";

      strValue += L")";

    } break;

    case OdDgGeometryCacheAction::kOriginAndRotation:
    {
      strName = L"kOriginAndRotation";

      OdDgOriginAndRotationGeometryCacheActionPtr pCache = pValue;

      OdGePoint3d    ptOrigin     = pCache->getOrigin();
      OdGeQuaternion quatRotation = pCache->getRotation();

      if( pCache->getRotationFlag() )
        strValue.format(L"Pt:(%f,%f,%f), Dir:(%f,%f,%f,%f)", ptOrigin.x, ptOrigin.y, ptOrigin.z, quatRotation.w, quatRotation.x, quatRotation.y, quatRotation.z);
      else
        strValue.format(L"Pt:(%f,%f,%f)", ptOrigin.x, ptOrigin.y, ptOrigin.z );
    } break;

    case OdDgGeometryCacheAction::kExtraDataSectionBegin:
    {
      OdDgExtraDataSectionBeginGeometryCacheActionPtr pCache = pValue;

      if( pCache->isLevelIndexData() )
      {
        strName  = L"kSetLevelIndex";
        strValue.format(L"Level Index = %d", pCache->getLevelIndex() );
      }
      else
      {
        strName  = L"kExtraDataSectionBegin";
        strValue.format(L"Type = %d, Data Size = %d", pCache->getExtraProxyDataType(), pCache->getExtraProxyData().size() );
      }
    } break;

    case OdDgGeometryCacheAction::kExtraDataSectionEnd:
    {
      strName  = L"kExtraDataSectionEnd";
      strValue = L"<empty>";
    } break;

    case OdDgGeometryCacheAction::kByLevelOrGradientData:
    {
      strName = L"kByLevelOrGradientData";

      OdDgByLevelOrGradientDataGeometryCacheActionPtr pCache = pValue;

      strValue = L"(";

      if( pCache->getUseByLevelDataFlag() )
      {
        if( pCache->getColorByLevelFlag() )
          strValue += L"Color, ";

        if( pCache->getFillColorByLevelFlag() )
          strValue += L"FillColor, ";

        if( pCache->getLineWeightByLevelFlag() )
          strValue += L"LineWeight, ";

        if( pCache->getLineTypeByLevelFlag() )
          strValue += L"LineType, ";
      }

      if( pCache->getUseGradientDataFlag() )
      {
        if( pCache->getEndOfUseGradientDataFlag() )
          strValue += L"X-Gradient-X, ";
        else
          strValue += L"Gradient, ";
      }

      if( strValue.getLength() > 2 )
        strValue = strValue.left(strValue.getLength()-2);
      else
        strValue += "Reset All";

      strValue += L")";
    } break;

    case OdDgGeometryCacheAction::kText:
    {
      strName = L"kText";

      OdDgTextGeometryCacheActionPtr pCache = pValue;
      strValue = L"\"" + pCache->getText() + L"\"";
    } break;

    default:
    {
      strName.format(L"kAction_0x%02x", (OdUInt16)(pValue->getType()));
      OdBinaryData binData = pValue->getProxyData();
      //strValue.format(L"-= Proxy Data Size = %d =-", binData.size() );

      for( OdUInt32 l = 0; l < binData.size(); l++ )
      {
        OdString strV;
        strV.format(L"%02x,", binData[l]);
        strValue += strV;
      };

      strValue = strValue.left(strValue.getLength() - 1);
    } break;
  }

  if( pValue->isCorrupted() )
  {
    strValue = L"!!! Data Corrupted";
  }

  strName = name + strName;

  writeFieldValue(strName, strValue);
}

void OdExDgnDumper::writeFieldColorValue( const OdString& name, OdUInt32 value )
{
  if( value == OdDg::kColorByLevel )
  {
    writeFieldValue( name, OdString(L"kColorByLevel"));
  }
  else if( value == OdDg::kColorByCell )
  {
    writeFieldValue( name, OdString(L"kColorByCell"));
  }
  else
  {
    writeFieldValue( name, value );
  }
}

void OdExDgnDumper::writeFieldLineStyleValue( const OdString& name, OdUInt32 value )
{
  if( value == OdDg::kLineStyleByLevel )
  {
    writeFieldValue( name, OdString(L"kLineStyleByLevel"));
  }
  else if( value == OdDg::kLineStyleByCell )
  {
    writeFieldValue( name, OdString(L"kLineStyleByCell"));
  }
  else
  {
    writeFieldValueHex( name, value );
  }
}

void OdExDgnDumper::writeFieldLineWeightValue( const OdString& name, OdUInt32 value )
{
  if( value == OdDg::kLineWeightByLevel )
  {
    writeFieldValue( name, OdString(L"kLineWeightByLevel"));
  }
  else if( value == OdDg::kLineWeightByCell )
  {
    writeFieldValue( name, OdString(L"kLineWeightByCell"));
  }
  else
  {
    writeFieldValue( name, value );
  }
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdArray<OdString>& arrData, bool bHideIfEmpty )
{
  if( bHideIfEmpty && arrData.isEmpty() )
    return;

  OdString strValue;

  for( OdUInt32 i = 0; i < arrData.size(); i++ )
    strValue += arrData[i] + L", ";

  if( !strValue.isEmpty() )
  {
    strValue = strValue.left(strValue.getLength() - 2);
    strValue = L"( " + strValue + L" )";
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdArray<OdUInt64>& arrData, bool bHideIfEmpty )
{
  if( bHideIfEmpty && arrData.isEmpty() )
    return;

  OdString strValue;

  for( OdUInt32 i = 0; i < arrData.size(); i++ )
  {
    OdString strItem;
    strItem.format(L"%I64u",arrData[i]);
    strValue += strItem + L", ";
  }

  if( !strValue.isEmpty() )
  {
    strValue = strValue.left(strValue.getLength() - 2);
    strValue = L"( " + strValue + L" )";
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdArray<OdUInt32>& arrData, bool bHideIfEmpty )
{
  if( bHideIfEmpty && arrData.isEmpty() )
    return;

  OdString strValue;

  for( OdUInt32 i = 0; i < arrData.size(); i++ )
  {
    OdString strItem;
    strItem.format(L"%u",arrData[i]);
    strValue += strItem + L", ";
  }

  if( !strValue.isEmpty() )
  {
    strValue = strValue.left(strValue.getLength() - 2);
    strValue = L"( " + strValue + L" )";
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdArray<OdInt32>& arrData, bool bHideIfEmpty )
{
  if( bHideIfEmpty && arrData.isEmpty() )
    return;

  OdString strValue;

  for( OdUInt32 i = 0; i < arrData.size(); i++ )
  {
    OdString strItem;
    strItem.format(L"%d",arrData[i]);
    strValue += strItem + L", ";
  }

  if( !strValue.isEmpty() )
  {
    strValue = strValue.left(strValue.getLength() - 2);
    strValue = L"( " + strValue + L" )";
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdArray<double>& arrData, bool bHideIfEmpty )
{
  if( bHideIfEmpty && arrData.isEmpty() )
    return;

  OdString strValue;

  for( OdUInt32 i = 0; i < arrData.size(); i++ )
  {
    OdString strItem;
    strItem.format(L"%f",arrData[i]);
    strValue += strItem + L", ";
  }

  if( !strValue.isEmpty() )
  {
    strValue = strValue.left(strValue.getLength() - 2);
    strValue = L"( " + strValue + L" )";
  }

  writeFieldValue( name, strValue );
}

void OdExDgnDumper::writeFieldValue( const OdString& name, const OdArray<bool>& arrData, bool bHideIfEmpty )
{
  if( bHideIfEmpty && arrData.isEmpty() )
    return;

  OdString strValue;

  for( OdUInt32 i = 0; i < arrData.size(); i++ )
  {
    if( arrData[i] )
      strValue += "true, ";
    else
      strValue += "false, ";
  }

  if( !strValue.isEmpty() )
  {
    strValue = strValue.left(strValue.getLength() - 2);
    strValue = L"( " + strValue + L" )";
  }

  writeFieldValue( name, strValue );
}

#endif // _EX_DGNDUMPER_H_
