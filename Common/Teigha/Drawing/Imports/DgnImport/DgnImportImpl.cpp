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

#include <OdaCommon.h>
#include <RxDispatchImpl.h>
#include <RxVariantValue.h>
#include <DbDictionary.h>
#include <RxDynamicModule.h>
#include <Ed/EdCommandStack.h>
#include <Ed/EdCommandContext.h>
#include <Ed/EdUserIO.h>
#include <DbCommandContext.h>
#include <OdModuleNames.h>
#include <StaticRxObject.h>
#include "DgnImportImpl.h"
#include "DgnImportLS.h"
#include <DgDatabase.h>
#include <ExDgnHostAppServices.h>
#include <ExHostAppServices.h>
#include "DgnImportCommon.h"
#include "DgnImportContext.h"

#include "DgLine.h"
#include "DgArc.h"
#include "DgLineString.h"
#include "DgShape.h"
#include "DgEllipse.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgReferenceAttach.h"
#include "DgRasterAttach.h"
#include "DgRasterFrame.h"
#include "DgOle2Frame.h"
#include "DgTagElement.h"
#include "DgMesh.h"
#include "DgDimension.h"
#include "DgSharedCellReference.h"
#include "DgComplexShape.h"
#include "DgComplexString.h"
#include "Dg3DObject.h"
#include "DgTable.h"
#include "DgMultiline.h"
#include "DgCurve.h"
#include "DbRotatedDimension.h"
#include "DbArcDimension.h"
#include "Db3PointAngularDimension.h"
#include "DbRadialDimension.h"
#include "DbDiametricDimension.h"
#include "DbOrdinateDimension.h"
#include "DgLight.h"
#include "DgRaster.h"
#include "DgBSplineSurface.h"
#include "DgCone.h"

#include "DgnImportPE.h"
#include "DgnImportLine.h"
#include "DgnImportTableElement.h"
#include "DgnImportArc.h"
#include "DgnImportLineString.h"
#include "DgnImportRasterAttach.h"
#include "DgnImportSharedCell.h"
#include "DgnImportText.h"
#include "DgnImportEllipse.h"
#include "DgnImportMesh.h"
#include "DgnImportBSplineCurve.h"
#include "DgnImportOle.h"
#include "DgnImportCellHeader.h"
#include "DgnImportXRef.h"
#include "DgnImportTextNode.h"
#include "DgnImportComplexShape.h"
#include "DgnImportComplexString.h"
#include "DgnImportShape.h"
#include "DgnImportDimensions.h"
#include "DgnImport3dObject.h"
#include "DgnImportMultiline.h"
#include "DgnImportDimAssoc.h"
#include "DgnImportLight.h"

namespace TD_DGN_IMPORT{
OdDgnImport::ImportResult DgnImporter::import()
{
  OdSmartPtr<OdDbHostAppServices> svc = _properties->get_Services();
  if (svc.isNull())
    return OdDgnImport::fail;

  registerElementLoaders( svc );

  OdDgnImportContext::prepareToImport(this);
  OdDgnImportContext::setConvertColorIndexToTrueColorFlag( _properties->get_ConvertDgnColorIndicesToTrueColors() );
  OdDgnImportContext::setConvertEmptyDataFieldsToSpacesFlag( _properties->get_ConvertEmptyDataFieldsToSpaces() );

  _idMap.clear();
  OdDbDatabasePtr pDb = _properties->get_Database();
  bool isInitialized = !pDb.isNull();
  if (!isInitialized)
    pDb = svc->createDatabase(false);

  OdSmartPtr<OdDgHostAppServices> _services = _properties->get_DgnServices();
  _services->cleanupLineStyleDefRscMap();

  if (_services.isNull())
    _services = OdRxObjectImpl<OdExDgnHostAppServices>::createObject();
  if (!_properties->get_MS_SYMBRSRC().isEmpty())
    _services->setMS_SYMBRSRC(const_cast<OdStringArray&>(_properties->get_MS_SYMBRSRC()));

  try
  {
    OdDgDatabasePtr dgn = _services->readFile(_properties->get_DgnPath());
    if (dgn.isNull())
      return OdDgnImport::bad_file;

    m_pDgnDb           = dgn.get();
    m_pHostAppServices = svc.get();

    DgnLSImporter::prepareLS(dgn); // Populate additional database-resident objects before getting handseed
    pDb->getOdDbObjectId(dgn->getHandseed(), true);
    if (!isInitialized)
      pDb->initialize();
    OdDgnImportContext::setPalette( dgn );
    OdDgnImportContext::setImportInvisible( !_properties->get_DontImportInvisibleElements() );

    // Get dgn model to import it to dwg model space.

    OdDgModelPtr pModelSpaceModel;
    OdDgElementId idModelSpaceModel;

    bool modelSpaceFound = false;
    bool bImportActiveModel = _properties->get_ImportActiveModelToModelSpace();

    if( bImportActiveModel )
    {
      idModelSpaceModel = dgn->getActiveModelId();

      if( !idModelSpaceModel.isNull() )
      {
        pModelSpaceModel = idModelSpaceModel.safeOpenObject();
        modelSpaceFound = true;
      }
    }

    if( !modelSpaceFound )
    {
      for( OdDgElementIteratorPtr mi = dgn->getModelTable()->createIterator(); !mi->done(); mi->step())
      {
        OdDgModelPtr m = mi->item().safeOpenObject();

        if( !m->isKindOf(OdDgSheetModel::desc()) )
        {
          pModelSpaceModel = m;
          modelSpaceFound = true;
          break;
        }
        else
        {
          pModelSpaceModel = m;
        }
      }
    }

    // Copy tables

    copyRegApps(dgn, pDb);
    copyTextStyles(dgn, pDb);
    copyMaterials(dgn, pDb);
    copyLineStyles(dgn, pDb); // Should be called after text styles processed because import blocks
    copyLayers(dgn, pDb); // Should be called after line styles, because depends from id's
    copyDisplayStyles(dgn, pDb);
    copyViewports(dgn, pDb, pModelSpaceModel);
    copyNamedViews(dgn, pDb);
    copyMLineStyles(dgn, pDb, pModelSpaceModel);
    copyDimStyles(dgn, pDb);
    copyBlocks(dgn, pDb);

    pDb->setLWDISPLAY( true );
    pDb->setXCLIPFRAME(false);

    if( modelSpaceFound )
    {
      copyModelSpace(pModelSpaceModel, pDb);
    }

    if( !modelSpaceFound || _properties->get_ImportPaperSpaceModels() )
    {
      for( OdDgElementIteratorPtr mi = dgn->getModelTable()->createIterator(); !mi->done(); mi->step())
      {
        if( mi->item() == idModelSpaceModel )
        {
          continue;
        }

        OdDgModelPtr m = mi->item().safeOpenObject();

        if (m->isKindOf(OdDgSheetModel::desc()) && _properties->get_ImportPaperSpaceModels() )
        {
          copyLayout(m, pDb);
        }
        else
        {
          if( modelSpaceFound )
          {
            continue;
          }
          
          copyModelSpace(m, pDb);
          modelSpaceFound = true;
        }
      }
    }

    if( !_properties->get_BreakDimensionAssociation() )
    {
      setDimensionAssociation( dgn, pDb);
    }
    else
    {
      OdDgnImportContext::clearDimAssocDescriptors();
    }
  }
  catch (OdError& e)
  {
    OdDgnImportContext::clearDimAssocDescriptors();
    OdDgnImportContext::restoreAfterImport();

    unregisterElementLoaders( svc );
    svc->warning(e);
    return OdDgnImport::fail;
  }
  catch(...)
  {
    unregisterElementLoaders( svc );
    throw;
  }

  OdDgnImportContext::restoreAfterImport();

  unregisterElementLoaders( svc );
  removeUnusedResources();
  _properties->put_Database(pDb);
  return OdDgnImport::success;
}

//-----------------------------------------------------------------------------------------------------

void DgnImporter::createShxFontFromRscFont( const OdString& strFontName )
{
  if( !m_pDgnDb )
  {
    return;
  }

  OdDgnImportContext::createShxFontFromRscFont( strFontName, m_pDgnDb, m_pHostAppServices, _properties->get_shxFontsPath() );
}

//-----------------------------------------------------------------------------------------------------

void DgnImporter::registerElementLoaders( OdDbHostAppServices* pHostAppServices )
{
  if( OdDgnImportContext::getInternalImportFlag() )
  {
    return;
  }

  if( !_properties->get_RecomputeDimensionsAfterImport() )
  {
    bReloadRecomputeDim = pHostAppServices->getRecomputeDimBlocksRequired();

    ::odrxDynamicLinker()->unloadModule(OdRecomputeDimBlockModuleName);
    pHostAppServices->setRecomputeDimBlocksRequired(false);
  }

  setImportPEToElements( _properties->get_ExplodeTextNodes() );
}

//-----------------------------------------------------------------------------------------------------

void DgnImporter::unregisterElementLoaders( OdDbHostAppServices* pHostAppServices )
{
  if( OdDgnImportContext::getInternalImportFlag() )
  {
    return;
  }

  removeImportPEFromElements();

  if( !_properties->get_RecomputeDimensionsAfterImport() && bReloadRecomputeDim )
  {
    pHostAppServices->setRecomputeDimBlocksRequired(true); 
    ::odrxDynamicLinker()->loadApp(OdRecomputeDimBlockModuleName, false);
  }

  OdDgnImportContext::clearRscFontArray();
  OdDgnImportContext::clearMultilineStyles();
}

//-----------------------------------------------------------------------------------------------------

void DgnImporter::setImportPEToElements( bool bExplodeTextNode )
{
  static OdStaticRxObject<IgnoredElementImportPE> ignoreImp;
  static OdStaticRxObject<OdDgnImportPE> defaultImporter;
  OdDgElement::desc()->addX(OdDgnImportPE::desc(), &defaultImporter);
  static OdStaticRxObject<OdDgnLine2dImportPE> line2dImp;
  OdDgLine2d::desc()->addX(OdDgnImportPE::desc(), &line2dImp);
  static OdStaticRxObject<OdDgnLine3dImportPE> line3dImp;
  OdDgLine3d::desc()->addX(OdDgnImportPE::desc(), &line3dImp);
  static OdStaticRxObject<OdDgnArc3dImportPE> arc3dImp;
  OdDgArc3d::desc()->addX(OdDgnImportPE::desc(), &arc3dImp);
  static OdStaticRxObject<OdDgnArc2dImportPE> arc2dImp;
  OdDgArc2d::desc()->addX(OdDgnImportPE::desc(), &arc2dImp);
  static OdStaticRxObject<OdDgLineString2dImportPE> linestring2dImp;
  OdDgLineString2d::desc()->addX(OdDgnImportPE::desc(), &linestring2dImp);
  static OdStaticRxObject<OdDgLineString3dImportPE> linestring3dImp;
  OdDgLineString3d::desc()->addX(OdDgnImportPE::desc(), &linestring3dImp);
  static OdStaticRxObject<OdDgSharedCellReferenceImportPE> blockReferenceImp;
  OdDgSharedCellReference::desc()->addX(OdDgnImportPE::desc(), &blockReferenceImp);
  static OdStaticRxObject<OdDgText2dImportPE> text2dImp;
  OdDgText2d::desc()->addX(OdDgnImportPE::desc(), &text2dImp);
  static OdStaticRxObject<OdDgText3dImportPE> text3dImp;
  OdDgText3d::desc()->addX(OdDgnImportPE::desc(), &text3dImp);
  static OdStaticRxObject<OdDgnEllipse2dImportPE> ellipse2dImp;
  OdDgEllipse2d::desc()->addX(OdDgnImportPE::desc(), &ellipse2dImp);
  static OdStaticRxObject<OdDgnEllipse3dImportPE> ellipse3dImp;
  OdDgEllipse3d::desc()->addX(OdDgnImportPE::desc(), &ellipse3dImp);
  static OdStaticRxObject<OdDgMeshFaceLoopsImportPE> meshFaceLoopsImp;
  OdDgMeshFaceLoops::desc()->addX(OdDgnImportPE::desc(), &meshFaceLoopsImp);
  OdDgMeshPointCloud::desc()->addX(OdDgnImportPE::desc(), &ignoreImp);
  static OdStaticRxObject<OdDgMeshTriangleListImportPE> meshTriangleListImp;
  OdDgMeshTriangleList::desc()->addX(OdDgnImportPE::desc(), &meshTriangleListImp);
  static OdStaticRxObject<OdDgMeshQuadListImportPE> meshQuadListImp;
  OdDgMeshQuadList::desc()->addX(OdDgnImportPE::desc(), &meshQuadListImp);
  static OdStaticRxObject<OdDgMeshTriangleGridImportPE> meshTriangleGridImp;
  OdDgMeshTriangleGrid::desc()->addX(OdDgnImportPE::desc(), &meshTriangleGridImp);
  static OdStaticRxObject<OdDgMeshQuadGridImportPE> meshQuadGridImp;
  OdDgMeshQuadGrid::desc()->addX(OdDgnImportPE::desc(), &meshQuadGridImp);
  static OdStaticRxObject<OdDgBSplineCurveImportPE<OdDgBSplineCurve2d> > bspline2dImp;
  OdDgBSplineCurve2d::desc()->addX(OdDgnImportPE::desc(), &bspline2dImp);
  static OdStaticRxObject<OdDgBSplineCurveImportPE<OdDgBSplineCurve3d> > bspline3dImp;
  OdDgBSplineCurve3d::desc()->addX(OdDgnImportPE::desc(), &bspline3dImp);
  static OdStaticRxObject<OdDgTagImportPE> tagImp;
  OdDgTagElement::desc()->addX(OdDgnImportPE::desc(), &tagImp);
  static OdStaticRxObject<OdDgOle2FrameImportPE<OdDgOle2Frame2d> > ole2Frame2dImp;
  OdDgOle2Frame2d::desc()->addX(OdDgnImportPE::desc(), &ole2Frame2dImp);
  static OdStaticRxObject<OdDgOle2FrameImportPE<OdDgOle2Frame3d> > ole2Frame3dImp;
  OdDgOle2Frame3d::desc()->addX(OdDgnImportPE::desc(), &ole2Frame3dImp);
  static OdStaticRxObject<OdDgRasterFrameImportPE> rasterImp;
  OdDgRasterFrame::desc()->addX(OdDgnImportPE::desc(), &rasterImp);
  static OdStaticRxObject<OdDgRasterHeader2dImportPE> rasterHeader2dImp;
  OdDgRasterHeader2d::desc()->addX(OdDgnImportPE::desc(), &rasterHeader2dImp);
  static OdStaticRxObject<OdDgRasterHeader3dImportPE> rasterHeader3dImp;
  OdDgRasterHeader3d::desc()->addX(OdDgnImportPE::desc(), &rasterHeader3dImp);
  static OdStaticRxObject<OdDgnRasterAttachmentImportPE> rasterHeaderImp;
  OdDgRasterAttachmentHeader::desc()->addX(OdDgnImportPE::desc(), &rasterHeaderImp);
  static OdStaticRxObject<OdDgnCellHeaderImportPE<OdDgCellHeader2d> > cellHeader2dImp;
  OdDgCellHeader2d::desc()->addX(OdDgnImportPE::desc(), &cellHeader2dImp);
  static OdStaticRxObject<OdDgnCellHeaderImportPE<OdDgCellHeader3d> > cellHeader3dImp;
  OdDgCellHeader3d::desc()->addX(OdDgnImportPE::desc(), &cellHeader3dImp);
  static OdStaticRxObject<OdDgReferenceAttachmentImportPE> referenceAttachmentImp;
  OdDgReferenceAttachmentHeader::desc()->addX(OdDgnImportPE::desc(), &referenceAttachmentImp);  
  static OdStaticRxObject<OdDgShapeImportPE<OdDgShape2d> > shape2dImp;
  OdDgShape2d::desc()->addX(OdDgnImportPE::desc(), &shape2dImp);
  static OdStaticRxObject<OdDgShapeImportPE<OdDgShape3d> > shape3dImp;
  OdDgShape3d::desc()->addX(OdDgnImportPE::desc(), &shape3dImp); 
  static OdStaticRxObject<OdDgComplexShapeImportPE> complexShapeImp;
  OdDgComplexShape::desc()->addX(OdDgnImportPE::desc(), &complexShapeImp); 
  static OdStaticRxObject<OdDgComplexStringImportPE> complexStringImp;
  OdDgComplexString::desc()->addX(OdDgnImportPE::desc(), &complexStringImp); 
  static OdStaticRxObject<OdDgnSolidImportPE> solidImp;
  OdDgSolid::desc()->addX(OdDgnImportPE::desc(), &solidImp); 
  static OdStaticRxObject<OdDgnSurfaceImportPE> surfaceImp;
  OdDgSurface::desc()->addX(OdDgnImportPE::desc(), &surfaceImp);
  static OdStaticRxObject<OdDgCurveImportPE<OdDgCurve2d> > curve2dImp;
  OdDgCurve2d::desc()->addX(OdDgnImportPE::desc(), &curve2dImp);
  static OdStaticRxObject<OdDgCurveImportPE<OdDgCurve3d> > curve3dImp;
  OdDgCurve3d::desc()->addX(OdDgnImportPE::desc(), &curve3dImp);
  static OdStaticRxObject<OdDgnBSplineSurfaceImportPE> bSplineSurfaceImp;
  OdDgBSplineSurface::desc()->addX(OdDgnImportPE::desc(), &bSplineSurfaceImp);
  static OdStaticRxObject<OdDgnConeImportPE> coneImp;
  OdDgCone::desc()->addX(OdDgnImportPE::desc(), &coneImp);

  if( !bExplodeTextNode )
  {
    static OdStaticRxObject<OdDgnTextNodeImportPE<OdDgTextNode2d> > textNode2dImp;
    OdDgTextNode2d::desc()->addX(OdDgnImportPE::desc(), &textNode2dImp);
    static OdStaticRxObject<OdDgnTextNodeImportPE<OdDgTextNode3d> > textNode3dImp;
    OdDgTextNode3d::desc()->addX(OdDgnImportPE::desc(), &textNode3dImp);
  }
  else
  {
    static OdStaticRxObject<OdDgnComplexElementImportPE<OdDgTextNode3d> > textNode3dImp;
    OdDgTextNode3d::desc()->addX(OdDgnImportPE::desc(), &textNode3dImp);
    static OdStaticRxObject<OdDgnComplexElementImportPE<OdDgTextNode2d> > textNode2dImp;
    OdDgTextNode2d::desc()->addX(OdDgnImportPE::desc(), &textNode2dImp);
  }

  static OdStaticRxObject<OdDgnLinearDimensionImportPE> dimensionLinearImp;
  OdDgDimSizeArrow::desc()->addX(OdDgnImportPE::desc(), &dimensionLinearImp);
  OdDgDimSizeStroke::desc()->addX(OdDgnImportPE::desc(), &dimensionLinearImp);
  OdDgDimSingleLocation::desc()->addX(OdDgnImportPE::desc(), &dimensionLinearImp);
  OdDgDimCustomLinear::desc()->addX(OdDgnImportPE::desc(), &dimensionLinearImp);
  OdDgDimStackedLocation::desc()->addX(OdDgnImportPE::desc(), &dimensionLinearImp);

  static OdStaticRxObject<OdDgnAngularDimensionImportPE> dimensionAngularImp;
  OdDgDimAngleSize::desc()->addX(OdDgnImportPE::desc(), &dimensionAngularImp);
  OdDgDimArcSize::desc()->addX(OdDgnImportPE::desc(), &dimensionAngularImp);
  OdDgDimAngleLocation::desc()->addX(OdDgnImportPE::desc(), &dimensionAngularImp);
  OdDgDimArcLocation::desc()->addX(OdDgnImportPE::desc(), &dimensionAngularImp);
  OdDgDimAngleLines::desc()->addX(OdDgnImportPE::desc(), &dimensionAngularImp);
  OdDgDimAngleAxisX::desc()->addX(OdDgnImportPE::desc(), &dimensionAngularImp);
  OdDgDimAngleAxisY::desc()->addX(OdDgnImportPE::desc(), &dimensionAngularImp);

  static OdStaticRxObject<OdDgnDiameterPerpDimensionImportPE> dimensionDiamPerpImp;
  OdDgDimDiameterPerpendicular::desc()->addX(OdDgnImportPE::desc(), &dimensionDiamPerpImp);

  static OdStaticRxObject<OdDgnDiameterParallelDimensionImportPE> dimensionDiamParallelImp;
  OdDgDimDiameterParallel::desc()->addX(OdDgnImportPE::desc(), &dimensionDiamParallelImp);

  static OdStaticRxObject<OdDgnDiameterDimensionImportPE> dimensionDiamImp;
  OdDgDimDiameter::desc()->addX(OdDgnImportPE::desc(), &dimensionDiamImp);
  OdDgDimDiameterExtended::desc()->addX(OdDgnImportPE::desc(), &dimensionDiamImp);

  static OdStaticRxObject<OdDgnRadiusDimensionImportPE> dimensionRadiusImp;
  OdDgDimRadius::desc()->addX(OdDgnImportPE::desc(), &dimensionRadiusImp);
  OdDgDimRadiusExtended::desc()->addX(OdDgnImportPE::desc(), &dimensionRadiusImp);

  static OdStaticRxObject<OdDgnOrdinateDimensionImportPE> dimensionOrdinateImp;
  OdDgDimOrdinate::desc()->addX(OdDgnImportPE::desc(), &dimensionOrdinateImp);

  static OdStaticRxObject<OdDgnCenterMarkDimensionImportPE> dimensionCenterMarkImp;
  OdDgDimCenter::desc()->addX(OdDgnImportPE::desc(), &dimensionCenterMarkImp);

  static OdStaticRxObject<OdDgnLabelLineDimensionImportPE> dimensionLabelLineImp;
  OdDgDimLabelLine::desc()->addX(OdDgnImportPE::desc(), &dimensionLabelLineImp);

  static OdStaticRxObject<OdDgnNoteDimensionImportPE> dimensionNoteImp;
  OdDgDimNote::desc()->addX(OdDgnImportPE::desc(), &dimensionNoteImp);

  static OdStaticRxObject<OdDgnMultilineImportPE> multilineImpl;
  OdDgMultiline::desc()->addX(OdDgnImportPE::desc(), &multilineImpl);

  static OdStaticRxObject<OdDgnPointLightImportPE> pointLightImpl;
  OdDgLightPoint::desc()->addX(OdDgnImportPE::desc(), &pointLightImpl);

  static OdStaticRxObject<OdDgnDirectionLightImportPE> directionLightImpl;
  OdDgLightDistant::desc()->addX(OdDgnImportPE::desc(), &directionLightImpl);

  static OdStaticRxObject<OdDgnSpotLightImportPE> spotLightImpl;
  OdDgLightSpot::desc()->addX(OdDgnImportPE::desc(), &spotLightImpl);

  static OdStaticRxObject<OdDgnAreaLightImportPE> areaLightImpl;
  OdDgLightArea::desc()->addX(OdDgnImportPE::desc(), &areaLightImpl);

  static OdStaticRxObject<OdDgnTableElementImportPE> tableElementImp;
  OdDgTableElement::desc()->addX(OdDgnImportPE::desc(), &tableElementImp);
}

//-----------------------------------------------------------------------------------------------------

void DgnImporter::removeImportPEFromElements()
{
  OdDgElement::desc()->delX(OdDgnImportPE::desc());
  OdDgLine2d::desc()->delX(OdDgnImportPE::desc());
  OdDgLine3d::desc()->delX(OdDgnImportPE::desc());
  OdDgArc3d::desc()->delX(OdDgnImportPE::desc());
  OdDgArc2d::desc()->delX(OdDgnImportPE::desc());
  OdDgLineString2d::desc()->delX(OdDgnImportPE::desc());
  OdDgLineString3d::desc()->delX(OdDgnImportPE::desc());
  OdDgSharedCellReference::desc()->delX(OdDgnImportPE::desc());
  OdDgText2d::desc()->delX(OdDgnImportPE::desc());
  OdDgText3d::desc()->delX(OdDgnImportPE::desc());
  OdDgEllipse2d::desc()->delX(OdDgnImportPE::desc());
  OdDgEllipse3d::desc()->delX(OdDgnImportPE::desc());
  OdDgMeshFaceLoops::desc()->delX(OdDgnImportPE::desc());
  OdDgMeshPointCloud::desc()->delX(OdDgnImportPE::desc());
  OdDgMeshTriangleList::desc()->delX(OdDgnImportPE::desc());
  OdDgMeshQuadList::desc()->delX(OdDgnImportPE::desc());
  OdDgMeshTriangleGrid::desc()->delX(OdDgnImportPE::desc());
  OdDgMeshQuadGrid::desc()->delX(OdDgnImportPE::desc());
  OdDgBSplineCurve3d::desc()->delX(OdDgnImportPE::desc());
  OdDgBSplineCurve2d::desc()->delX(OdDgnImportPE::desc());
  OdDgTagElement::desc()->delX(OdDgnImportPE::desc());
  OdDgOle2Frame2d::desc()->delX(OdDgnImportPE::desc());
  OdDgOle2Frame3d::desc()->delX(OdDgnImportPE::desc());
  OdDgTextNode3d::desc()->delX(OdDgnImportPE::desc());
  OdDgTextNode2d::desc()->delX(OdDgnImportPE::desc());
  OdDgRasterFrame::desc()->delX(OdDgnImportPE::desc());
  OdDgRasterHeader2d::desc()->delX(OdDgnImportPE::desc());
  OdDgRasterHeader3d::desc()->delX(OdDgnImportPE::desc());
  OdDgRasterAttachmentHeader::desc()->delX(OdDgnImportPE::desc());
  OdDgCellHeader2d::desc()->delX(OdDgnImportPE::desc());
  OdDgCellHeader3d::desc()->delX(OdDgnImportPE::desc());
  OdDgShape2d::desc()->delX(OdDgnImportPE::desc());
  OdDgShape3d::desc()->delX(OdDgnImportPE::desc());
  OdDgSolid::desc()->delX(OdDgnImportPE::desc());
  OdDgSurface::desc()->delX(OdDgnImportPE::desc());
  OdDgReferenceAttachmentHeader::desc()->delX(OdDgnImportPE::desc());
  OdDgComplexShape::desc()->delX(OdDgnImportPE::desc());
  OdDgComplexString::desc()->delX(OdDgnImportPE::desc());
  OdDgDimSizeArrow::desc()->delX(OdDgnImportPE::desc());
  OdDgDimSizeStroke::desc()->delX(OdDgnImportPE::desc());
  OdDgDimSingleLocation::desc()->delX(OdDgnImportPE::desc());
  OdDgDimCustomLinear::desc()->delX(OdDgnImportPE::desc());
  OdDgDimStackedLocation::desc()->delX(OdDgnImportPE::desc());
  OdDgDimAngleSize::desc()->delX(OdDgnImportPE::desc());
  OdDgDimArcSize::desc()->delX(OdDgnImportPE::desc());
  OdDgDimAngleLocation::desc()->delX(OdDgnImportPE::desc());
  OdDgDimArcLocation::desc()->delX(OdDgnImportPE::desc());
  OdDgDimAngleLines::desc()->delX(OdDgnImportPE::desc());
  OdDgDimAngleAxisX::desc()->delX(OdDgnImportPE::desc());
  OdDgDimAngleAxisY::desc()->delX(OdDgnImportPE::desc());
  OdDgDimDiameterPerpendicular::desc()->delX(OdDgnImportPE::desc());
  OdDgDimDiameterParallel::desc()->delX(OdDgnImportPE::desc());
  OdDgDimDiameter::desc()->delX(OdDgnImportPE::desc());
  OdDgDimRadius::desc()->delX(OdDgnImportPE::desc());
  OdDgDimDiameterExtended::desc()->delX(OdDgnImportPE::desc());
  OdDgDimRadiusExtended::desc()->delX(OdDgnImportPE::desc());
  OdDgDimOrdinate::desc()->delX(OdDgnImportPE::desc());
  OdDgDimCenter::desc()->delX(OdDgnImportPE::desc());
  OdDgDimLabelLine::desc()->delX(OdDgnImportPE::desc());
  OdDgDimNote::desc()->delX(OdDgnImportPE::desc());
  OdDgMultiline::desc()->delX(OdDgnImportPE::desc());
  OdDgCurve3d::desc()->delX(OdDgnImportPE::desc());
  OdDgCurve2d::desc()->delX(OdDgnImportPE::desc());
  OdDgLightPoint::desc()->delX(OdDgnImportPE::desc());
  OdDgLightArea::desc()->delX(OdDgnImportPE::desc());
  OdDgLightDistant::desc()->delX(OdDgnImportPE::desc());
  OdDgLightSpot::desc()->delX(OdDgnImportPE::desc());
  OdDgTableElement::desc()->delX(OdDgnImportPE::desc());
  OdDgBSplineSurface::desc()->delX(OdDgnImportPE::desc());
  OdDgCone::desc()->delX(OdDgnImportPE::desc());
}

//-----------------------------------------------------------------------------------------------------

bool DgnImporter::setTextNodeToMTextImportPE()
{
  bool bRet = !_properties->get_ExplodeTextNodes();

  if( bRet )
  {
    OdDgTextNode3d::desc()->delX(OdDgnImportPE::desc());
    OdDgTextNode2d::desc()->delX(OdDgnImportPE::desc());

    static OdStaticRxObject<OdDgnTextNodeImportPE<OdDgTextNode2d> > textNode2dToMTextImp;
    OdDgTextNode2d::desc()->addX(OdDgnImportPE::desc(), &textNode2dToMTextImp);
    static OdStaticRxObject<OdDgnTextNodeImportPE<OdDgTextNode3d> > textNode3dToMTextImp;
    OdDgTextNode3d::desc()->addX(OdDgnImportPE::desc(), &textNode3dToMTextImp);
  }

  return bRet;
}

//-----------------------------------------------------------------------------------------------------

void DgnImporter::setTextNodeToEntitySetImportPE()
{
  OdDgTextNode3d::desc()->delX(OdDgnImportPE::desc());
  OdDgTextNode2d::desc()->delX(OdDgnImportPE::desc());

  static OdStaticRxObject<OdDgnComplexElementImportPE<OdDgTextNode3d> > textNode3dToEntitySetImp;
  OdDgTextNode3d::desc()->addX(OdDgnImportPE::desc(), &textNode3dToEntitySetImp);
  static OdStaticRxObject<OdDgnComplexElementImportPE<OdDgTextNode2d> > textNode2dToEntitySetImp;
  OdDgTextNode2d::desc()->addX(OdDgnImportPE::desc(), &textNode2dToEntitySetImp);
}

//-----------------------------------------------------------------------------------------------------

void DgnImporter::setDimensionAssociation( OdDgDatabase* pDg, OdDbDatabase* pDb )
{
  OdUInt32 nDimensionToUpdateAssoc = OdDgnImportContext::getDimAssocDescriptorCount();

  for( OdUInt32 i = 0; i < nDimensionToUpdateAssoc; i++ )
  {
    OdDbObjectId                  idDimension;
    OdDgnImportDimAssocDescriptor assocDiscr;

    OdDgnImportContext::getDimAssocDescriptor( i, idDimension, assocDiscr );

    OdDbDimensionPtr pDimension = idDimension.openObject(OdDb::kForWrite);

    if( !pDimension.isNull() )
    {
      if( pDimension->isKindOf(OdDbRotatedDimension::desc()) )
      {
        createLinearDimensionAssoc( pDimension, assocDiscr, pDg );
      }
      else if( pDimension->isKindOf(OdDb3PointAngularDimension::desc()) ||
               pDimension->isKindOf(OdDbArcDimension::desc())
             )
      {
        createAngularDimensionAssoc( pDimension, assocDiscr, pDg );
      }
      else if( pDimension->isKindOf(OdDbRadialDimension::desc()) ||
               pDimension->isKindOf(OdDbDiametricDimension::desc())
             )
      {
        createRadialDimensionAssoc( pDimension, assocDiscr, pDg );
      }
      else if( pDimension->isKindOf(OdDbOrdinateDimension::desc()) )
      {
        createOrdinateDimensionAssoc( pDimension, assocDiscr, pDg );
      }
    }
  }

  OdDgnImportContext::clearDimAssocDescriptors();
}

//-----------------------------------------------------------------------------------------------------

OdDb::LineWeight DgnImporter::getLineWeightByDgnIndex( OdUInt32 uIndex )
{
  OdDgnImportLineWeightsMapPtr pLWMap = _properties->get_LineWeightsMap();
  return pLWMap->getLineWeightByDgnIndex( uIndex );
}

//-----------------------------------------------------------------------------------------------------

OdRxDictionaryPtr DgnProperties::createObject()
{
  return OdRxObjectImpl<DgnProperties, OdRxDictionary>::createObject();
}

ODRX_DECLARE_PROPERTY(DgnPath)
ODRX_DECLARE_PROPERTY(Database)
ODRX_DECLARE_PROPERTY(Services)
ODRX_DECLARE_PROPERTY(DgnServices)
ODRX_DECLARE_PROPERTY(ImportActiveModelToModelSpace)
ODRX_DECLARE_PROPERTY(ImportPaperSpaceModels)
ODRX_DECLARE_PROPERTY(XRefImportMode)
ODRX_DECLARE_PROPERTY(ExplodeTextNodes)
ODRX_DECLARE_PROPERTY(RecomputeDimensionsAfterImport)
ODRX_DECLARE_PROPERTY(MS_SYMBRSRC)
ODRX_DECLARE_PROPERTY(DontImportInvisibleElements)
ODRX_DECLARE_PROPERTY(ImportViewIndex)
ODRX_DECLARE_PROPERTY( 3dEllipseImportMode )
ODRX_DECLARE_PROPERTY( 2dEllipseImportMode )
ODRX_DECLARE_PROPERTY( 3dShapeImportMode )
ODRX_DECLARE_PROPERTY( 2dShapeImportMode )
ODRX_DECLARE_PROPERTY( 3dObjectImportMode )
ODRX_DECLARE_PROPERTY( 3dClosedBSplineCurveImportMode )
ODRX_DECLARE_PROPERTY( 2dClosedBSplineCurveImportMode )
ODRX_DECLARE_PROPERTY( shxFontsPath )
ODRX_DECLARE_PROPERTY( BreakDimensionAssociation )
ODRX_DECLARE_PROPERTY( LineStyleImporter )
ODRX_DECLARE_PROPERTY( LineWeightsMap )
ODRX_DECLARE_PROPERTY( ConvertDgnColorIndicesToTrueColors )
ODRX_DECLARE_PROPERTY( ConvertEmptyDataFieldsToSpaces )
ODRX_DECLARE_PROPERTY( EraseUnusedResources )

ODRX_DEFINE_PROPERTY(DgnPath, DgnProperties, getString)
ODRX_DEFINE_PROPERTY_OBJECT(Database, DgnProperties,  get_Database, put_Database, OdDbDatabase)
ODRX_DEFINE_PROPERTY_OBJECT(Services, DgnProperties,  get_Services, put_Services, OdDbHostAppServices)
ODRX_DEFINE_PROPERTY_OBJECT(DgnServices, DgnProperties,  get_DgnServices, put_DgnServices, OdDgHostAppServices)
ODRX_DEFINE_PROPERTY(XRefImportMode, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(ExplodeTextNodes, DgnProperties, getBool)
ODRX_DEFINE_PROPERTY(RecomputeDimensionsAfterImport, DgnProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportPaperSpaceModels, DgnProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportActiveModelToModelSpace, DgnProperties, getBool)
ODRX_DEFINE_PROPERTY(MS_SYMBRSRC, DgnProperties, getStringArray)
ODRX_DEFINE_PROPERTY(DontImportInvisibleElements, DgnProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportViewIndex, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(3dEllipseImportMode, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(2dEllipseImportMode, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(3dShapeImportMode, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(3dObjectImportMode, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(2dShapeImportMode, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(3dClosedBSplineCurveImportMode, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(2dClosedBSplineCurveImportMode, DgnProperties, getInt8)
ODRX_DEFINE_PROPERTY(shxFontsPath, DgnProperties, getString)
ODRX_DEFINE_PROPERTY(BreakDimensionAssociation, DgnProperties, getBool)
ODRX_DEFINE_PROPERTY_OBJECT(LineStyleImporter, DgnProperties,  get_LineStyleImporter, put_LineStyleImporter, OdDgnImportLineStyle)
ODRX_DEFINE_PROPERTY_OBJECT(LineWeightsMap, DgnProperties,  get_LineWeightsMap, put_LineWeightsMap, OdDgnImportLineWeightsMap)
ODRX_DEFINE_PROPERTY(ConvertDgnColorIndicesToTrueColors, DgnProperties, getBool)
ODRX_DEFINE_PROPERTY(ConvertEmptyDataFieldsToSpaces, DgnProperties, getBool)
ODRX_DEFINE_PROPERTY(EraseUnusedResources, DgnProperties, getBool)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP( DgnProperties );
  ODRX_GENERATE_PROPERTY( DgnPath )
  ODRX_GENERATE_PROPERTY( Database )
  ODRX_GENERATE_PROPERTY( Services )
  ODRX_GENERATE_PROPERTY( DgnServices )
  ODRX_GENERATE_PROPERTY( ImportActiveModelToModelSpace )
  ODRX_GENERATE_PROPERTY( ImportPaperSpaceModels )
  ODRX_GENERATE_PROPERTY( XRefImportMode )
  ODRX_GENERATE_PROPERTY( ExplodeTextNodes )
  ODRX_GENERATE_PROPERTY( RecomputeDimensionsAfterImport )
  ODRX_GENERATE_PROPERTY( MS_SYMBRSRC )
  ODRX_GENERATE_PROPERTY( DontImportInvisibleElements )
  ODRX_GENERATE_PROPERTY( ImportViewIndex )
  ODRX_GENERATE_PROPERTY( 3dEllipseImportMode )
  ODRX_GENERATE_PROPERTY( 2dEllipseImportMode )
  ODRX_GENERATE_PROPERTY( 3dShapeImportMode )
  ODRX_GENERATE_PROPERTY( 3dObjectImportMode )
  ODRX_GENERATE_PROPERTY( 2dShapeImportMode )
  ODRX_GENERATE_PROPERTY( 3dClosedBSplineCurveImportMode )
  ODRX_GENERATE_PROPERTY( 2dClosedBSplineCurveImportMode )
  ODRX_GENERATE_PROPERTY( shxFontsPath )
  ODRX_GENERATE_PROPERTY( BreakDimensionAssociation )
  ODRX_GENERATE_PROPERTY( LineStyleImporter )
  ODRX_GENERATE_PROPERTY( LineWeightsMap )
  ODRX_GENERATE_PROPERTY( ConvertDgnColorIndicesToTrueColors )
  ODRX_GENERATE_PROPERTY( ConvertEmptyDataFieldsToSpaces )
  ODRX_GENERATE_PROPERTY( EraseUnusedResources )
ODRX_END_DYNAMIC_PROPERTY_MAP( DgnProperties );

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma warning(disable:4355)
#endif

DgnImporter::DgnImporter() : _properties( DgnProperties::createObject() )
{
  bReloadRecomputeDim = true;
}

OdRxDictionaryPtr DgnImporter::properties() { return _properties; }

OdDgnImportLineStylePtr g_pDgnImportLS;

class DgnImportModule : public OdDgnImportModule
{
  OdRxModulePtr m_pDGNDirect;
  OdRxModulePtr m_pDgnLS;

public:
  virtual void initApp()
  {
    m_pDGNDirect = ::odrxDynamicLinker()->loadModule(OdDgnDbModuleName);
    OdDgnImportPE::rxInit();
    m_pDgnLS = ::odrxDynamicLinker()->loadModule(OdDgnLSModuleName);

    g_pDgnImportLS = OdRxObjectImpl<OdDgnImportLineStyleImpl>::createObject();
  }
  virtual void uninitApp()
  {
    m_pDGNDirect.release();
    ::odrxDynamicLinker()->unloadModule(OdDgnDbModuleName);
    OdDgnImportPE::rxUninit();
    m_pDgnLS.release();

    g_pDgnImportLS.release();
  }
  
  virtual OdDgnImportPtr create ()
  {
      return OdRxObjectImpl<DgnImporter>::createObject();
  }
};
}
using namespace TD_DGN_IMPORT;
ODRX_DEFINE_DYNAMIC_MODULE(DgnImportModule)

