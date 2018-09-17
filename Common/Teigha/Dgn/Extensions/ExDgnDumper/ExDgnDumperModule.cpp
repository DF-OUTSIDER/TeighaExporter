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

#include "OdaCommon.h"
#include "ExDgnDumperModule.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "DgElement.h"
#include "FlatMemStream.h"
#include "MemoryStream.h"
#include "RxObjectImpl.h"
#include "DgDatabase.h"
#include "ExDgnElementDumperPE.h"

#include "DgLevelTableRecord.h"
#include "DgFontTableRecord.h"
#include "DgTextStyleTableRecord.h"
#include "DgDimStyleTableRecord.h"
#include "DgMultilineStyleTableRecord.h"
#include "DgLineStyleTableRecord.h"
#include "DgLineStyleDefTableRecord.h"
#include "DgDictionaryTableRecord.h"
#include "DgRegAppTableRecord.h"
#include "DgAttributeLinkage.h"
#include "DgApplicationData.h"
#include "DgLevelMask.h"
#include "DgXAttribute.h"
#include "DgProxyElement.h"
#include "DgMaterialTableRecord.h"
#include "DgLight.h"
#include "DgMesh.h"
#include "DgRasterFrame.h"
#include "DgOle2Frame.h"
#include "DgNamedGroup.h"
#include "DgACS.h"
#include "DgSectionClip.h"
#include "DgEnvironmentMap.h"
#include "DgItemTypeLibrary.h"
#include "DgPrintStyleTableRecord.h"

#include "RxInit.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)

#define VC_EXTRALEAN
#include "windows.h"

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID )
{
  if ( dwReason == DLL_PROCESS_ATTACH )
  {
    // remove this if you need per-thread initialization
    DisableThreadLibraryCalls( (HMODULE)hInstance );
  }
  return true;
}

#endif

//
// OdDgDumper
//
class OdDgDumperImpl : public OdExDgnDumperModule
{
public:

  OdDgDumperImpl(){};
  virtual ~OdDgDumperImpl(){};

  virtual void initApp();
  virtual void uninitApp();

private:
  OdDgElementDumperPE                   m_elementDumper;
  OdDgDatabaseDumperPE                  m_databaseDumper;

  // tables
  OdDgModelTableDumperPE                m_modelTableDumper;
  OdDgModelDumperPE                     m_modelDumper;
  OdDgTableDumperPE                     m_tableDumper;
  OdDgFontTableDumperPE                 m_fontTableDumper;
  OdDgLevelFilterTableDumperPE          m_levelFilterTableDumper;
  OdDgLineStyleTableDumperPE            m_lineStyleTableDumper;
  OdDgMaterialTableDumperPE             m_materialTableDumper;
  OdDgViewGroupTableDumperPE            m_viewGroupTableDumper;
  OdDgNamedViewTableDumperPE            m_namedViewTableDumper;
  OdDgSharedCellDefinitionTableDumperPE m_sharedCellDefinitionTableDumper;
  OdDgTagDefinitionSetTableDumperPE     m_tagSetDefinitionTableDumper;
  OdDgColorTableDumperPE                m_colorTableDumper;
  OdDgNonModelElementCollectionPE       m_nmElementCollection;
  OdDgPrototypeElementTablePE           m_prototypeElementTablePE;
  OdDgElementTemplateTablePE            m_elementTemplateTableDumper;
  OdDgLinkSetTablePE                    m_linkSetTableDumper;
  OdDgColorBookTablePE                  m_colorBookTableDumper;
  OdDgPrintStyleTablePE                 m_printStyleTableDumper;
  OdDgDictionaryTableDumperPE           m_dictionaryTableDumper;

  OdDgTableRecordDumperPE               m_tableRecordDumper;
  OdDgLevelTableRecordDumperPE          m_levelTableRecordDumper;
  OdDgFontTableRecordDumperPE           m_fontTableRecordDumper;
  OdDgTextStyleTableRecordDumperPE      m_textStyleTableRecordDumper;
  OdDgDimStyleTableRecordDumperPE       m_dimStyleTableRecordDumper;
  OdDgMaterialTableRecordDumperPE       m_materialTableRecordDumper;
  OdDgMultilineStyleTableRecordDumperPE m_multilineStyleTableRecordDumper;
  OdDgLineStyleTableRecordDumperPE      m_lineStyleTableRecordDumper;
  OdDgLineStyleDefTableRecordDumperPE   m_lineStyleDefsTableRecordDumper;
  OdDgLevelFilterTableRecordDumperPE    m_levelFilterTableRecordDumper;
  OdDgDictionaryTableRecordDumperPE     m_dicTableRecordDumper;
  OdDgDictionaryDumperPE                m_dicDumper;
  OdDgRegAppTableRecordDumperPE         m_regAppTableRecordDumper;
  OdDgViewGroupDumperPE                 m_viewGroupDumper;
  OdDgViewDumperPE                      m_viewDumper;
  OdDgReferenceAttachmentLevelMaskGroupPE m_LevelMaskGroupDumper;
  OdDgLevelMaskDumperPE                 m_levelMaskDumper;
  OdDgSharedCellDefinitionDumperPE      m_sharedCellDefinitionDumper;
  OdDgSharedCellReferenceDumperPE       m_sharedCellReferenceDumper;
  OdDgTagDefinitionSetDumperPE          m_tagSetDefinitionDumper;
  OdDgItemTypeLibraryDumperPE           m_itemTypeLibDumper;
  OdDgXMLCustomSchemaContainerDumperPE  m_xmlCustomSchemaContainerDumper;
  OdDgTagDefinitionDumperPE             m_tagDefinitionDumper;
  OdDgACSDumperPE                       m_acsDumper;
  OdDgEnvironmentMapTableDumperPE       m_environmentTableDumper;
  OdDgDisplayStyleTableDumperPE         m_displayStyleTableDumper;
  OdDgDisplayStyleTableRecordDumperPE   m_displayStyleDumper;
  OdDgPrintStyleTableRecordDumperPE     m_printStyleDumper;
  OdDgTablePrototypeElementPE           m_tablePrototypeDumper;

  OdDgApplicationDataDumperPE           m_appDataDumper;

  //
  OdDgReferenceAttachmentHeaderDumperPE m_referenceAttachmentDumper;
  OdDgRasterAttachmentHeaderDumperPE    m_rasterAttachmentHeaderDumper;
  OdDgReferenceOverrideDumperPE         m_referenceOverrideDumper;
  OdDgReferenceOrderDumperPE            m_referenceOrderDumper;

  //
  OdDgRasterComponentDumperPE           m_rasterComponentDumper;

  //
  OdDgVariablesListPE                   m_localVariablesListDumper;
  OdDgVariableValuesSetPE               m_variationSetDumper;

  // graphics elements
  OdDgLine2dDumperPE                    m_line2dDumper;
  OdDgLine3dDumperPE                    m_line3dDumper;
  OdDgLineString2dDumperPE              m_lineString2dDumper;
  OdDgLineString3dDumperPE              m_lineString3dDumper;
  OdDgRasterFrameDumperPE               m_rasterFarmeDumper;
  OdDgText2dDumperPE                    m_text2dDumper;
  OdDgText3dDumperPE                    m_text3dDumper;
  OdDgShape2dDumperPE                   m_shape2dDumper;
  OdDgShape3dDumperPE                   m_shape3dDumper;
  OdDgCurve2dDumperPE                   m_curve2dDumper;
  OdDgCurve3dDumperPE                   m_curve3dDumper;
  OdDgEllipse2dDumperPE                 m_ellipse2dDumper;
  OdDgEllipse3dDumperPE                 m_ellipse3dDumper;
  OdDgArc2dDumperPE                     m_arc2dDumper;
  OdDgArc3dDumperPE                     m_arc3dDumper;
  OdDgConeDumperPE                      m_coneDumper;
  OdDgPointString2dDumperPE             m_pointString2dDumper;
  OdDgPointString3dDumperPE             m_pointString3dDumper;
  OdDgDimensionDumperPE                 m_dimensionDumper;
  OdDgMultilineDumperPE                 m_multilineDumper;
  OdDgTagElementDumperPE                m_tagElementDumper;
  OdDgNamedGroupDumperPE                m_namedGroupDumper;
  OdDgTerrainGraphicsDumperPE           m_terrainDumper;

  // complex graphics elements
  OdDgBSplineCurve2dDumperPE            m_bSplineCurve2dDumper;
  OdDgBSplineCurve3dDumperPE            m_bSplineCurve3dDumper;
  OdDgBSplineSurfaceDumperPE            m_bSplineSurfaceDumper;
  OdDgCellHeader2dDumperPE              m_cellHeader2dDumper;
  OdDgCellHeader3dDumperPE              m_cellHeader3dDumper;
  OdDgDistantLightDumperPE              m_distantLightDumperPE;
  OdDgPointLightDumperPE                m_pointLightDumperPE;
  OdDgSpotLightDumperPE                 m_spotLightDumperPE;
  OdDgAreaLightDumperPE                 m_areaLightDumperPE;
  OdDgSkyOpeningLightDumperPE           m_skyOpeningLightDumperPE;
  OdDgOle2Frame2dDumperPE               m_ole2Frame2dDumperPE;
  OdDgOle2Frame3dDumperPE               m_ole2Frame3dDumperPE;
  OdDgComplexStringDumperPE             m_complexStringDumper;
  OdDgComplexShapeDumperPE              m_complexShapeDumper;
  OdDgRasterHeader2dDumperPE            m_rasterHeader2dDumper;
  OdDgRasterHeader3dDumperPE            m_rasterHeader3dDumper;
  OdDgTextNode2dDumperPE                m_textNode2dDumper;
  OdDgTextNode3dDumperPE                m_textNode3dDumper;
  OdDgSurfaceDumperPE                   m_surfaceDumper;
  OdDgSolidDumperPE                     m_solidDumper;
  OdDgElementTemplateDumperPE           m_elementTemplateDumper;
  OdDgColorBookDumperPE                 m_colorBookDumper;
  OdDgLinkSetDumperPE                   m_linkSetDumper;

  OdDgTitleText2dDumperPE               m_titleText2dDumper;
  OdDgTitleText3dDumperPE               m_titleText3dDumper;
  OdDgSectionMarker2dDumperPE           m_sectionMarker2dDumper;
  OdDgSectionMarker3dDumperPE           m_sectionMarker3dDumper;
  OdDgArrowMarker2dDumperPE             m_arrowMarker2dDumper;
  OdDgArrowMarker3dDumperPE             m_arrowMarker3dDumper;
  OdDgDetailMarker2dDumperPE            m_detailMarker2dDumper;
  OdDgDetailMarker3dDumperPE            m_detailMarker3dDumper;

  OdDgSectionClipDumperPE               m_sectionClipDumper;

  OdDgProxyElementDumperPE              m_proxyDumper;
  OdDgComplexProxyElementDumperPE       m_complexProxyDumper;

  OdDgCompoundLineStyleResourceDumperPE m_compoundLsResDumper;
  OdDgLineCodeResourceDumperPE          m_lineCodeLsResDumper;
  OdDgLinePointResourceDumperPE         m_linePointLsResDumper;
  OdDgPointSymbolResourceDumperPE       m_pointSymbolLsResDumper;

  OdDgLinearAssociationDumperPE         m_LinearAssociationDumper;
  OdDgIntersectAssociationDumperPE      m_IntersectAssociationDumper;
  OdDgArcAssociationDumperPE            m_ArcAssociationDumper;
  OdDgMultilineAssociationDumperPE      m_MultilineAssociationDumper;
  OdDgBSplineCurveAssociationDumperPE   m_BSplineCurveAssociationDumper;
  OdDgProjectionAssociationDumperPE     m_ProjectionAssociationDumper;
  OdDgOriginAssociationDumperPE         m_OriginAssociationDumper;
  OdDgIntersect2AssociationDumperPE     m_Intersect2AssociationDumper;
  OdDgMeshVertexAssociationDumperPE     m_MeshVertexAssociationDumper;
  OdDgMeshEdgeAssociationDumperPE       m_MeshEdgeAssociationDumper;
  OdDgBSplineSurfaceAssociationDumperPE m_BSplineSurfaceAssociationDumper;
  OdDgUnknownAssociationDumperPE        m_UnknownAssociationDumper;

  OdDgMeshFaceLoopsDumperPE             m_meshFaceLoopsDumper;
};

ODRX_DEFINE_DYNAMIC_MODULE(OdDgDumperImpl);

//----------------------------------------------------------
//
// OdDgDumperImpl
//
//----------------------------------------------------------

void OdDgDumperImpl::initApp()
{
  OdDgRxObjectDumperPE::rxInit();
  OdDgElementDumperPE::rxInit();

  OdDgElement::desc()->addX( OdDgElementDumperPE::desc(), &m_elementDumper );
  OdDgDatabase::desc()->addX( OdDgElementDumperPE::desc(), &m_databaseDumper );

  OdDgModelTable::desc()->addX( OdDgElementDumperPE::desc(), &m_modelTableDumper );
  OdDgModel::desc()->addX( OdDgElementDumperPE::desc(), &m_modelDumper );

  OdDgLevelTable::desc()->addX( OdDgElementDumperPE::desc(), &m_tableDumper );
  OdDgLevelFilterTable::desc()->addX( OdDgElementDumperPE::desc(), &m_levelFilterTableDumper );
  OdDgFontTable::desc()->addX( OdDgElementDumperPE::desc(), &m_fontTableDumper );
  OdDgTextStyleTable::desc()->addX( OdDgElementDumperPE::desc(), &m_tableDumper );
  OdDgDimStyleTable::desc()->addX( OdDgElementDumperPE::desc(), &m_tableDumper );
  OdDgMaterialTable::desc()->addX( OdDgElementDumperPE::desc(), &m_materialTableDumper );
  OdDgMultilineStyleTable::desc()->addX( OdDgElementDumperPE::desc(), &m_tableDumper );
  OdDgLineStyleTable::desc()->addX( OdDgElementDumperPE::desc(), &m_lineStyleTableDumper );
  OdDgLineStyleDefTable::desc()->addX( OdDgElementDumperPE::desc(), &m_tableDumper );
  OdDgDictionaryTable::desc()->addX( OdDgElementDumperPE::desc(), &m_dictionaryTableDumper );
  OdDgRegAppTable::desc()->addX( OdDgElementDumperPE::desc(), &m_tableDumper );

  // NonDBRO collections
  OdDgViewGroupTable::desc()->addX( OdDgElementDumperPE::desc(), &m_viewGroupTableDumper );
  OdDgNamedViewTable::desc()->addX( OdDgElementDumperPE::desc(), &m_namedViewTableDumper );
  OdDgSharedCellDefinitionTable::desc()->addX( OdDgElementDumperPE::desc(), &m_sharedCellDefinitionTableDumper );
  OdDgTagDefinitionSetTable::desc()->addX( OdDgElementDumperPE::desc(), &m_tagSetDefinitionTableDumper );
  OdDgColorTable::desc()->addX( OdDgElementDumperPE::desc(), &m_colorTableDumper );
  OdDgNonModelElementCollection::desc()->addX( OdDgElementDumperPE::desc(), &m_nmElementCollection );
  OdDgElementTemplateTable::desc()->addX( OdDgElementDumperPE::desc(), &m_elementTemplateTableDumper );
  OdDgColorBookTable::desc()->addX( OdDgElementDumperPE::desc(), &m_colorBookTableDumper );
  OdDgLinkSetTable::desc()->addX( OdDgElementDumperPE::desc(), &m_linkSetTableDumper );
  OdDgPrintStyleTable::desc()->addX( OdDgElementDumperPE::desc(), &m_printStyleTableDumper );
  OdDgPrototypeElementTable::desc()->addX( OdDgElementDumperPE::desc(), &m_prototypeElementTablePE );

  // Records
  OdDgLevelTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_levelTableRecordDumper );
  OdDgLevelFilterTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_levelFilterTableRecordDumper );
  OdDgFontTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_fontTableRecordDumper );
  OdDgTextStyleTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_textStyleTableRecordDumper );
  OdDgDimStyleTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_dimStyleTableRecordDumper );
  OdDgMaterialTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_materialTableRecordDumper );
  OdDgMultilineStyleTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_multilineStyleTableRecordDumper );
  OdDgLineStyleTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_lineStyleTableRecordDumper );
  OdDgLineStyleDefTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_lineStyleDefsTableRecordDumper );
  OdDgDictionaryTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_dicTableRecordDumper );
  OdDgDictionary::desc()->addX( OdDgElementDumperPE::desc(), &m_dicDumper );
  OdDgRegAppTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_regAppTableRecordDumper );

  OdDgViewGroup::desc()->addX( OdDgElementDumperPE::desc(), &m_viewGroupDumper );
  OdDgView::desc()->addX( OdDgElementDumperPE::desc(), &m_viewDumper );
  OdDgLevelMask::desc()->addX( OdDgElementDumperPE::desc(), &m_levelMaskDumper );
  OdDgReferenceAttachmentLevelMaskGroup::desc()->addX( OdDgElementDumperPE::desc(), &m_LevelMaskGroupDumper );
  OdDgSharedCellDefinition::desc()->addX( OdDgElementDumperPE::desc(), &m_sharedCellDefinitionDumper );
  OdDgSharedCellReference::desc()->addX( OdDgElementDumperPE::desc(), &m_sharedCellReferenceDumper );
  OdDgTagDefinitionSet::desc()->addX( OdDgElementDumperPE::desc(), &m_tagSetDefinitionDumper );
  OdDgTagDefinition::desc()->addX( OdDgElementDumperPE::desc(), &m_tagDefinitionDumper );
  OdDgItemTypeLibrary::desc()->addX( OdDgElementDumperPE::desc(), &m_itemTypeLibDumper );
  OdDgXMLCustomSchemaContainer::desc()->addX( OdDgElementDumperPE::desc(), &m_xmlCustomSchemaContainerDumper );

  OdDgApplicationData::desc()->addX( OdDgElementDumperPE::desc(), &m_appDataDumper );
  OdDgACS::desc()->addX( OdDgElementDumperPE::desc(), &m_acsDumper );
  OdDgEnvironmentMapTable::desc()->addX( OdDgElementDumperPE::desc(), &m_environmentTableDumper );
  OdDgDisplayStyleTable::desc()->addX( OdDgElementDumperPE::desc(), &m_displayStyleTableDumper );
  OdDgDisplayStyleTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_displayStyleDumper );
  OdDgTablePrototypeElement::desc()->addX(OdDgTablePrototypeElementPE::desc(), &m_tablePrototypeDumper );
  OdDgPrintStyleTableRecord::desc()->addX( OdDgElementDumperPE::desc(), &m_printStyleDumper );
  //OdDgRasterAttachmentComponentClip::desc()->addX(OdDgRasterAttachmentComponentClipDumperPE::desc(), &m_rastClipCompDumper);

  OdDgLine2d::desc()->addX( OdDgElementDumperPE::desc(), &m_line2dDumper );
  OdDgLine3d::desc()->addX( OdDgElementDumperPE::desc(), &m_line3dDumper );
  OdDgLineString2d::desc()->addX( OdDgElementDumperPE::desc(), &m_lineString2dDumper );
  OdDgLineString3d::desc()->addX( OdDgElementDumperPE::desc(), &m_lineString3dDumper );
  OdDgRasterFrame::desc()->addX( OdDgElementDumperPE::desc(), &m_rasterFarmeDumper );
  OdDgText2d::desc()->addX( OdDgElementDumperPE::desc(), &m_text2dDumper );
  OdDgText3d::desc()->addX( OdDgElementDumperPE::desc(), &m_text3dDumper );
  OdDgTextNode2d::desc()->addX( OdDgElementDumperPE::desc(), &m_textNode2dDumper );
  OdDgTextNode3d::desc()->addX( OdDgElementDumperPE::desc(), &m_textNode3dDumper );
  OdDgShape2d::desc()->addX( OdDgElementDumperPE::desc(), &m_shape2dDumper );
  OdDgShape3d::desc()->addX( OdDgElementDumperPE::desc(), &m_shape3dDumper );
  OdDgCurve2d::desc()->addX( OdDgElementDumperPE::desc(), &m_curve2dDumper );
  OdDgCurve3d::desc()->addX( OdDgElementDumperPE::desc(), &m_curve3dDumper );
  OdDgEllipse2d::desc()->addX( OdDgElementDumperPE::desc(), &m_ellipse2dDumper );
  OdDgEllipse3d::desc()->addX( OdDgElementDumperPE::desc(), &m_ellipse3dDumper );
  OdDgArc2d::desc()->addX( OdDgElementDumperPE::desc(), &m_arc2dDumper );
  OdDgArc3d::desc()->addX( OdDgElementDumperPE::desc(), &m_arc3dDumper );
  OdDgCone::desc()->addX( OdDgElementDumperPE::desc(), &m_coneDumper );
  OdDgComplexString::desc()->addX( OdDgElementDumperPE::desc(), &m_complexStringDumper );
  OdDgComplexShape::desc()->addX( OdDgElementDumperPE::desc(), &m_complexShapeDumper );
  OdDgPointString2d::desc()->addX( OdDgElementDumperPE::desc(), &m_pointString2dDumper );
  OdDgPointString3d::desc()->addX( OdDgElementDumperPE::desc(), &m_pointString3dDumper );
  OdDgDimension::desc()->addX( OdDgElementDumperPE::desc(), &m_dimensionDumper );
  OdDgNamedGroupHeader::desc()->addX( OdDgElementDumperPE::desc(), &m_namedGroupDumper );
  OdDgMultiline::desc()->addX( OdDgElementDumperPE::desc(), &m_multilineDumper );
  OdDgBSplineCurve2d::desc()->addX( OdDgElementDumperPE::desc(), &m_bSplineCurve2dDumper );
  OdDgBSplineCurve3d::desc()->addX( OdDgElementDumperPE::desc(), &m_bSplineCurve3dDumper );
  OdDgSurface::desc()->addX( OdDgElementDumperPE::desc(), &m_surfaceDumper );
  OdDgSolid::desc()->addX( OdDgElementDumperPE::desc(), &m_solidDumper );
  OdDgRasterAttachmentHeader::desc()->addX( OdDgElementDumperPE::desc(), &m_rasterAttachmentHeaderDumper );
  OdDgRasterHeader2d::desc()->addX( OdDgElementDumperPE::desc(), &m_rasterHeader2dDumper );
  OdDgRasterHeader3d::desc()->addX( OdDgElementDumperPE::desc(), &m_rasterHeader3dDumper );
  OdDgRasterComponent::desc()->addX( OdDgElementDumperPE::desc(), &m_rasterComponentDumper );
  OdDgTagElement::desc()->addX( OdDgElementDumperPE::desc(), &m_tagElementDumper );
  OdDgCellHeader2d::desc()->addX( OdDgElementDumperPE::desc(), &m_cellHeader2dDumper );
  OdDgCellHeader3d::desc()->addX( OdDgElementDumperPE::desc(), &m_cellHeader3dDumper );
  OdDgLightDistant::desc()->addX( OdDgElementDumperPE::desc(), &m_distantLightDumperPE );
  OdDgLightPoint::desc()->addX( OdDgElementDumperPE::desc(), &m_pointLightDumperPE );
  OdDgLightSpot::desc()->addX( OdDgElementDumperPE::desc(), &m_spotLightDumperPE );
  OdDgLightArea::desc()->addX( OdDgElementDumperPE::desc(), &m_areaLightDumperPE );
  OdDgLightOpenSky::desc()->addX( OdDgElementDumperPE::desc(), &m_skyOpeningLightDumperPE );
  OdDgOle2Frame2d::desc()->addX( OdDgElementDumperPE::desc(), &m_ole2Frame2dDumperPE );
  OdDgOle2Frame3d::desc()->addX( OdDgElementDumperPE::desc(), &m_ole2Frame3dDumperPE );
  OdDgBSplineSurface::desc()->addX( OdDgElementDumperPE::desc(), &m_bSplineSurfaceDumper );
  OdDgReferenceAttachmentHeader::desc()->addX( OdDgElementDumperPE::desc(), &m_referenceAttachmentDumper );
  OdDgReferenceOverride::desc()->addX( OdDgElementDumperPE::desc(), &m_referenceOverrideDumper );
  OdDgReferenceOrder::desc()->addX( OdDgElementDumperPE::desc(), &m_referenceOrderDumper );
  OdDgMeshFaceLoops::desc()->addX( OdDgElementDumperPE::desc(), &m_meshFaceLoopsDumper );
  OdDgElementTemplate::desc()->addX( OdDgElementDumperPE::desc(), &m_elementTemplateDumper );
  OdDgColorBook::desc()->addX( OdDgElementDumperPE::desc(), &m_colorBookDumper );
  OdDgLinkSet::desc()->addX( OdDgElementDumperPE::desc(), &m_linkSetDumper );
  OdDgTerrain::desc()->addX( OdDgElementDumperPE::desc(), &m_terrainDumper );

  OdDgTitleText2d::desc()->addX( OdDgElementDumperPE::desc(), &m_titleText2dDumper );
  OdDgTitleText3d::desc()->addX( OdDgElementDumperPE::desc(), &m_titleText3dDumper );
  OdDgSectionMarker2d::desc()->addX( OdDgElementDumperPE::desc(), &m_sectionMarker2dDumper );
  OdDgSectionMarker3d::desc()->addX( OdDgElementDumperPE::desc(), &m_sectionMarker3dDumper );
  OdDgArrowMarker2d::desc()->addX( OdDgElementDumperPE::desc(), &m_arrowMarker2dDumper );
  OdDgArrowMarker3d::desc()->addX( OdDgElementDumperPE::desc(), &m_arrowMarker3dDumper );
  OdDgDetailMarker2d::desc()->addX( OdDgElementDumperPE::desc(), &m_detailMarker2dDumper );
  OdDgDetailMarker3d::desc()->addX( OdDgElementDumperPE::desc(), &m_detailMarker3dDumper );

  OdDgSectionClip::desc()->addX( OdDgElementDumperPE::desc(), &m_sectionClipDumper );

  OdDgProxyElement::desc()->addX( OdDgElementDumperPE::desc(), &m_proxyDumper );
  OdDgComplexProxyElement::desc()->addX( OdDgElementDumperPE::desc(), &m_complexProxyDumper );

  OdDgVariablesList::desc()->addX( OdDgElementDumperPE::desc(), &m_localVariablesListDumper );
  OdDgVariableValuesSet::desc()->addX( OdDgElementDumperPE::desc(), &m_variationSetDumper );

  OdDgCompoundLineStyleResource::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_compoundLsResDumper );
  OdDgLineCodeResource::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_lineCodeLsResDumper );
  OdDgLinePointResource::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_linePointLsResDumper );
  OdDgPointSymbolResource::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_pointSymbolLsResDumper );

  OdDgLinearAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_LinearAssociationDumper );
  OdDgIntersectAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_IntersectAssociationDumper );
  OdDgArcAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_ArcAssociationDumper );
  OdDgMultilineAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_MultilineAssociationDumper);
  OdDgBSplineCurveAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_BSplineCurveAssociationDumper);
  OdDgProjectionAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_ProjectionAssociationDumper);
  OdDgOriginAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_OriginAssociationDumper);
  OdDgIntersect2Association::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_Intersect2AssociationDumper);
  OdDgMeshVertexAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_MeshVertexAssociationDumper);
  OdDgMeshEdgeAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_MeshEdgeAssociationDumper);
  OdDgBSplineSurfaceAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_BSplineSurfaceAssociationDumper);
  OdDgUnknownAssociation::desc()->addX( OdDgRxObjectDumperPE::desc(), &m_UnknownAssociationDumper);
}


void OdDgDumperImpl::uninitApp()
{
  OdDgDatabase::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgModelTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgModel::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgLevelTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLevelFilterTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgFontTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTextStyleTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDimStyleTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgMaterialTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgMultilineStyleTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLineStyleTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLineStyleDefTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDictionaryTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgRegAppTable::desc()->delX( OdDgElementDumperPE::desc() );

  // NonDBRO collections
  OdDgViewGroupTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgNamedViewTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgSharedCellDefinitionTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTagDefinitionSetTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgColorTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgNonModelElementCollection::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgElementTemplateTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgColorBookTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLinkSetTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgPrintStyleTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgPrototypeElementTable::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgTitleText2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTitleText3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgSectionMarker2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgSectionMarker3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgArrowMarker2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgArrowMarker3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDetailMarker2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDetailMarker3d::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgSectionClip::desc()->delX( OdDgElementDumperPE::desc() );

  // Records
  OdDgLevelTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLevelFilterTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgFontTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTextStyleTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDimStyleTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgMaterialTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgMultilineStyleTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLineStyleTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLineStyleDefTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDictionaryTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDictionary::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgRegAppTableRecord::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgReferenceAttachmentLevelMaskGroup::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgViewGroup::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgView::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLevelMask::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgSharedCellDefinition::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgSharedCellReference::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTagDefinitionSet::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTagDefinition::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgItemTypeLibrary::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgXMLCustomSchemaContainer::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgApplicationData::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgACS::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgEnvironmentMapTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDisplayStyleTable::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDisplayStyleTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgPrintStyleTableRecord::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTablePrototypeElement::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgLine2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLine3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLineString2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLineString3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgText2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgText3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTextNode2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTextNode3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgShape2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgShape3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgCurve2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgCurve3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgEllipse2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgEllipse3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgArc2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgArc3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgCone::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgComplexString::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgComplexShape::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgPointString2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgPointString3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgDimension::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgNamedGroupHeader::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgMultiline::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgBSplineCurve2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgBSplineCurve3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgSurface::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgSolid::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgRasterAttachmentHeader::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgRasterHeader2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgRasterHeader3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgRasterComponent::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTagElement::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgCellHeader2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgCellHeader3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLightDistant::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLightPoint::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLightSpot::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLightArea::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLightOpenSky::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgOle2Frame2d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgOle2Frame3d::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgBSplineSurface::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgMeshFaceLoops::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgColorBook::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgElementTemplate::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgLinkSet::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgTerrain::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgVariablesList::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgVariableValuesSet::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgReferenceAttachmentHeader::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgReferenceOverride::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgReferenceOrder::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgProxyElement::desc()->delX( OdDgElementDumperPE::desc() );
  OdDgComplexProxyElement::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgCompoundLineStyleResource::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgLineCodeResource::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgLinePointResource::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgPointSymbolResource::desc()->delX( OdDgRxObjectDumperPE::desc() );

  OdDgLinearAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgIntersectAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgArcAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgMultilineAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgBSplineCurveAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgProjectionAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgOriginAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgIntersect2Association::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgMeshVertexAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgMeshEdgeAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgBSplineSurfaceAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );
  OdDgUnknownAssociation::desc()->delX( OdDgRxObjectDumperPE::desc() );

  OdDgRasterFrame::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgElement::desc()->delX( OdDgElementDumperPE::desc() );

  OdDgElementDumperPE::rxUninit();
  OdDgRxObjectDumperPE::rxUninit();
}
