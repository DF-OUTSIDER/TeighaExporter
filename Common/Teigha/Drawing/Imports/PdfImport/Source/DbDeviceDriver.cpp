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
#include "DbDeviceDriver.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "core/fxcrt/fx_system.h"
#include "core/fxge/cfx_font.h"
#include "core/fxge/cfx_windowsrenderdevice.h"
#include "core/fxge/cfx_graphstatedata.h"
#include "core/fxge/dib/cfx_dibextractor.h"
#include "core/fxge/dib/cfx_imagerenderer.h"
#include "core/fxge/dib/cstretchengine.h"
#include "core/fxge/fx_freetype.h"
#include "third_party/base/ptr_util.h"
#include "core/fxge/fx_freetype.h"
#include "core/fpdfapi/font/cpdf_font.h"
#include "core/fpdfapi/cpdf_modulemgr.h"
#include "core/fpdfapi/font/cpdf_font.h"
#include "core/fpdfapi/page/cpdf_form.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pathobject.h"
#include "core/fpdfapi/page/cpdf_textobject.h"
#include "core/fpdfapi/page/cpdf_shadingpattern.h"
#include "core/fpdfapi/page/cpdf_function.h"
#include "core/fpdfapi/page/cpdf_stitchfunc.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/cpdf_name.h"
#include "core/fpdfapi/parser/cpdf_parser.h"
#include "core/fpdfapi/parser/cpdf_reference.h"
#include "core/fpdfapi/parser/cpdf_array.h"
#include "core/fxge/cfx_unicodeencoding.h"
#include "core/fxge/cfx_cliprgn.h"
#include "third_party/base/ptr_util.h"


#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbHostAppServices.h"
#include "Db2LineAngularDimension.h"
#include "Db2dPolyline.h"
#include "Db2dVertex.h"
#include "Db3PointAngularDimension.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "Db3dSolid.h"
#include "DbAlignedDimension.h"
#include "DbArc.h"
#include "DbArcAlignedText.h"
#include "DbArcDimension.h"
#include "DbAttribute.h"
#include "DbAttributeDefinition.h"
#include "DbBlockReference.h"
#include "DbBlockTable.h"
#include "DbBody.h"
#include "DbCircle.h"
#include "DbDiametricDimension.h"
#include "DbDimAssoc.h"
#include "DbDimStyleTable.h"
#include "DbEllipse.h"
#include "DbFace.h"
#include "DbFaceRecord.h"
#include "DbFcf.h"
#include "DbField.h"
#include "DbGroup.h"
#include "DbHyperlink.h"
#include "DbLayerTable.h"
#include "DbLayout.h"
#include "DbLeader.h"
#include "DbMLeader.h"
#include "DbLine.h"
#include "DbLinetypeTable.h"
#include "DbMaterial.h"
#include "DbMInsertBlock.h"
#include "DbMline.h"
#include "DbMlineStyle.h"
#include "DbOrdinateDimension.h"
#include "DbPoint.h"
#include "DbPolyFaceMesh.h"
#include "DbPolyFaceMeshVertex.h"
#include "DbPolygonMesh.h"
#include "DbPolygonMeshVertex.h"
#include "DbPolyline.h"
#include "DbRadialDimension.h"
#include "DbRadialDimensionLarge.h"
#include "DbRasterImageDef.h"
#include "DbRasterVariables.h"
#include "DbRay.h"
#include "DbRegion.h"
#include "DbRotatedDimension.h"
#include "DbShape.h"
#include "DbSolid.h"
#include "DbSortentsTable.h"
#include "DbSpline.h"
#include "DbTable.h"
#include "DbMText.h"
#include "DbText.h"
#include "DbTextStyleTable.h"
#include "DbTrace.h"
#include "DbViewport.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbWipeout.h"
#include "DbXline.h"
#include "DbXrecord.h"
#include "RText.h"
#include "DbOle2Frame.h"
#include "DbHelix.h"
#include "DbUnderlayDefinition.h"
#include "DbUnderlayReference.h"
#include "HatchPatternManager.h"
#include "DbLayerTableRecord.h"
#include "OdPath.h"

#include "Ge/GeCircArc2d.h"
#include "Ge/GeEllipArc2d.h"
#include "Ge/GeSplineEnt2d.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve2d.h"
#include "Gi/GiMaterial.h"
#include "Gi/GiRasterWrappers.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GePolyline2d.h"
#include "Ge/GeLineSeg2d.h"

#include "MemoryStream.h"
#include "OdPlatformStreamer.h"
#include "RxRasterServices.h"
#include "Gs/LineWtIndex.h"
#include "OdRound.h"


class PdfImportModule;

const double OdDbDeviceDriver::DPI = 72.0;

OdDbDeviceDriver::OdDbDeviceDriver(const int width, const int height, const PdfPropertiesPtr& pProperties, size_t& error_counter, 
  const ByteString& layer_name, const std::map<ByteString, bool>& layers_info, bool& is_object_visible, 
  const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted, const OdString& images_path, OdPdfImport::ImportResult& error)
  :m_Width(width)
  ,m_Height(height)
  ,m_pProperties(pProperties)
  ,m_ErrorCounter(error_counter)
  ,m_CurrentLayerName(layer_name)
  ,m_LayersInfo(layers_info)
  ,m_IsObjectVisible(is_object_visible)
  ,m_ClipBoundary(clipBoundary)
  ,m_isClipBoundaryInverted(is_clip_boundary_inverted)
  ,m_ImagesPath(images_path)
  ,m_ImageError(error)
{
  OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());
  
  OdString pdf_path = m_pProperties->get_PdfPath();

  int i1 = pdf_path.reverseFind('\\');
  int i2 = pdf_path.reverseFind('/');
  int i = odmax(i1, i2);
  m_PdfFileName = pdf_path.mid(i + 1);
  pdf_path = pdf_path.left(i + 1);
  i = m_PdfFileName.find(L'.');
  if (i != -1)
    m_PdfFileName = m_PdfFileName.left(i);

  if (m_pProperties->get_ImportAsBlock() && !m_PdfFileName.isEmpty())
  {
      OdDbBlockTablePtr pTable = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);

      OdDbObjectId block_id = pTable->getAt(m_PdfFileName);

      if (block_id.isNull())
      {
        OdDbBlockTableRecordPtr pRecord = OdDbBlockTableRecord::createObject();
        pRecord->setName(m_PdfFileName);
        block_id = pTable->add(pRecord);

        OdDbBlockTableRecordPtr active_space = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
        OdDbBlockReferencePtr block_ref = OdDbBlockReference::createObject();
        block_ref->setBlockTableRecord(block_id);
        active_space->appendOdDbEntity(block_ref);
      }
      m_pBTR = block_id.safeOpenObject(OdDb::kForWrite);
  }
  else
  {
    m_pBTR = (OdDbBlockTableRecordPtr)
      (pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite));
  }

  m_TransforMatrix.setToIdentity();
  m_TransforMatrix *= OdGeMatrix3d::scaling(m_pProperties->get_Scaling());
  m_TransforMatrix *= OdGeMatrix3d::rotation(m_pProperties->get_Rotation(), OdGeVector3d::kZAxis);
  m_TransforMatrix.setTranslation(OdGeVector3d(m_pProperties->get_InsertionPointX(), m_pProperties->get_InsertionPointY(), 0.0));
}

OdDbDeviceDriver::~OdDbDeviceDriver() 
{
  EndRendering();
}

int OdDbDeviceDriver::GetDeviceCaps(int caps_id) const {
  switch (caps_id)
  {
  case FXDC_DEVICE_CLASS:
    return FXDC_PRINTER;
  case FXDC_PIXEL_WIDTH:
    return m_Width;
  case FXDC_PIXEL_HEIGHT:
    return m_Height;
  case FXDC_BITS_PIXEL:
    return 72.;
  case FXDC_RENDER_CAPS:
    return FXRC_GET_BITS | FXRC_ALPHA_PATH | FXRC_ALPHA_IMAGE |
      FXRC_BLEND_MODE | FXRC_SOFT_CLIP | FXRC_SHADING | FXRC_FILLSTROKE_PATH;
  case FXDC_HORZ_SIZE:
    return m_Width;
  case FXDC_VERT_SIZE:
    return m_Height;
  }
  return 0;
}

bool OdDbDeviceDriver::SetClip_PathFill(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device,  int fill_mode) 
{
  return false;
}

bool OdDbDeviceDriver::SetClip_PathStroke(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState) 
{
  return false;
}

void fillKnots(const OdUInt32 size, OdGeKnotVector& knotsVec, OdGeDoubleArray& wts)
{
  OdGeDoubleArray knots;

  /* weight values */
  wts.resize(size, 1.);

  /* Knot points */
  knots.resize(size + 4, 0.);
  double kVal = 1.;
  int j = 0;
  for (j = 0; j < 4; j++)
    knots[j] = 0.;
  for (size_t i = 1; i < (size - 1) / 3; i++)
  {
    knots[j++] = kVal;
    knots[j++] = kVal;
    knots[j++] = kVal;
    kVal += 1.;
  }
  for (int i = 0; i < 4; i++)
    knots[j++] = kVal;

  knotsVec = knots;
  knotsVec.setTolerance(0.);
}

void OdDbDeviceDriver::savePath(const std::vector<BaseCurve>& paths, const OdCmColor color, const CFX_GraphStateData* pGraphState)
{
  OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());

  static const OdChar* layer_name = L"PDF_Geomertry";
  OdDbObjectId layer_id = getLayer(layer_name);

  OdDb::LineWeight line_widht(OdDb::kLnWtByLwDefault);

  if (m_pProperties->get_ApplyLineweight() && nullptr != pGraphState)
  {
    float pdf_line_widht = pGraphState->m_LineWidth / DPI * 25.4;
    OdDb::LineWeight prev_line_widht(OdDb::kLnWt000);
    for (int i = 0; i < 24; ++i)
    {
      line_widht = ::lineWeightByIndex(i);
      if ((pdf_line_widht > (float)prev_line_widht / 100.) && (pdf_line_widht < (float)line_widht / 100.))
      {
        if ((pdf_line_widht - (float)prev_line_widht / 100.) < ((float)line_widht / 100. - pdf_line_widht))
        {
          line_widht = prev_line_widht;
        }
        break;
      }
      prev_line_widht = line_widht;
    }
  }

  for (auto& fig : paths)
  {
    for (auto& seg : fig.getStorage())
    {
      if (FXPT_TYPE::LineTo == seg.second)
      {
        OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
        pPolyline->setDatabaseDefaults(m_pBTR->database());
        m_pBTR->appendOdDbEntity(pPolyline);
        pPolyline->setLayer(layer_id);

        for (OdUInt32 i = 0; i < seg.first.size(); ++i)
        {
          pPolyline->addVertexAt(i, seg.first[i]);
        }
        pPolyline->setColor(color);
        if (m_pProperties->get_ApplyLineweight())
          pPolyline->setLineWeight(line_widht);
      }
      else
      {
        OdGePoint3dArray ctrlPts;
        ctrlPts.resize(seg.first.size());

        for (OdUInt32 i = 0; i < seg.first.size(); i++)
        {
          ctrlPts[i] = OdGePoint3d(seg.first[i].x, seg.first[i].y, 0);
        }

        OdGeKnotVector knotsVec;
        OdGeDoubleArray wts;

        fillKnots(std::get<0>(seg).size(), knotsVec, wts);

        OdDbSplinePtr pSpline = OdDbSpline::createObject();
        pSpline->setDatabaseDefaults(m_pBTR->database());
        m_pBTR->appendOdDbEntity(pSpline);

        pSpline->setNurbsData(
          3,			/* int degree, */
          1, 			/* bool rational, */
          false, 		/* bool closed, */
          0,			/* bool periodic,*/
          ctrlPts,	        /* const OdGePoint3dArray& controlPoints, */
          knotsVec,		/* const OdGeDoubleArray& knots, */
          wts,			/* const OdGeDoubleArray& weights,*/
          0.0			/* double controlPtTol, */
        );
        pSpline->setColor(color);
        if (m_pProperties->get_ApplyLineweight())
          pSpline->setLineWeight(line_widht);
        pSpline->setLayer(layer_id);
      }
    }
  }
}

void OdDbDeviceDriver::saveSolid(const std::vector<BaseCurve>& paths, const OdCmColor color)
{
  OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());

  static const OdChar* layer_name = L"PDF_Solid Fills";
  static const OdCmTransparency transparency(0.5);

  if (!m_pProperties->get_ConvertSolidsToHatches() && 1 == paths.size() && 1 == paths[0].getStorage().size() 
    && paths[0].getClosed() && paths[0].isPolyline() &&
    (5 == paths[0].getStorage()[0].first.size()) && (paths[0].getStorage()[0].first.first().isEqualTo(paths[0].getStorage()[0].first.last())))
  {
    OdDbSolidPtr pSolid = OdDbSolid::createObject();
    pSolid->setDatabaseDefaults(m_pBTR->database());
    m_pBTR->appendOdDbEntity(pSolid);

    pSolid->setPointAt(0, OdGePoint3d(paths[0].getStorage()[0].first[0].x, paths[0].getStorage()[0].first[0].y, 0));
    pSolid->setPointAt(1, OdGePoint3d(paths[0].getStorage()[0].first[1].x, paths[0].getStorage()[0].first[1].y, 0));
    pSolid->setPointAt(2, OdGePoint3d(paths[0].getStorage()[0].first[3].x, paths[0].getStorage()[0].first[3].y, 0));
    pSolid->setPointAt(3, OdGePoint3d(paths[0].getStorage()[0].first[2].x, paths[0].getStorage()[0].first[2].y, 0));

    pSolid->setColor(color);
    pSolid->setLayer(getLayer(layer_name));
    pSolid->setTransparency(transparency);
    return;
  }

  OdDbHatchPtr pHatch = OdDbHatch::createObject();
  pHatch->setDatabaseDefaults(m_pBTR->database());
  OdDbObjectId redHatchId = m_pBTR->appendOdDbEntity(pHatch);

  pHatch->setAssociative(false);
  pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
  pHatch->setHatchStyle(OdDbHatch::kNormal);
  pHatch->setColor(color);
  pHatch->setTransparency(transparency);

  pHatch->setLayer(getLayer(layer_name));

  for (auto& fig : paths)
  {
    if (fig.isPolyline())
    {
      for (auto& seg : fig.getStorage())
      {
        OdGePoint2dArray  vertexPts;
        OdGeDoubleArray   vertexBulges;
        vertexPts.resize(seg.first.size());
        for (OdUInt32 i = 0; i < seg.first.size(); i++)
        {
          vertexPts[i].set(seg.first[i].x, seg.first[i].y);
        }
        pHatch->appendLoop(OdDbHatch::kExternal | OdDbHatch::kPolyline,
          vertexPts, vertexBulges);
      }
    }
    else
    {
      EdgeArray edgePtrs;
      edgePtrs.clear();
      for (auto& seg : fig.getStorage())
      {
        if (FXPT_TYPE::LineTo == seg.second)
        {
          for (OdUInt32 i = 0; i < seg.first.size() - 1; ++i)
          {
            OdGeLineSeg2d* poly = new OdGeLineSeg2d(seg.first[i], seg.first[i+1]);
            edgePtrs.append(poly);
          }
        }
        else
        {
          OdGeKnotVector knotsVec;
          OdGeDoubleArray wts;

          fillKnots(seg.first.size(), knotsVec, wts);

          OdGeNurbCurve2d *spline = new OdGeNurbCurve2d();

          spline->set(3, knotsVec, seg.first, wts, false);

          edgePtrs.push_back(spline);
        }
      }
      if (0 != edgePtrs.size())
      {
        try
        {
          pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);
        }
        catch (...)
        {
          savePath(paths, color, nullptr);
        }

        edgePtrs.clear();
      }
    }
  }
  return;
}

OdDbObjectId OdDbDeviceDriver::getLayer(const OdChar* name)
{
  OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());
  OdDbLayerTablePtr pLayers = pDb->getLayerTableId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectId layer_id;

  std::function<void()> createObjectLayers = [&]()
  {
    OdString layer_name = m_IsObjectVisible ? OdString(name) : (OdString(name) + OdString(L"_Invisible"));
    layer_id = pLayers->getAt(layer_name);
    if (layer_id.isNull())
    {
      OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::createObject();
      pLayer->setName(layer_name);
      layer_id = pLayers->add(pLayer);
      pLayer->setIsOff(!m_IsObjectVisible);
    }
  };

  switch (m_pProperties->get_LayersUseType())
  {
  case 0: // Use PDF Layers
  {
    OdString layer_name = m_CurrentLayerName.IsEmpty()? OdString(name): (OdString(L"PDF_") + OdString(m_CurrentLayerName.c_str()));
    layer_id = pLayers->getAt(layer_name);
    if (layer_id.isNull())
    {
      OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::createObject();
      pLayer->setName(layer_name);
      layer_id = pLayers->add(pLayer);
      bool state = true;
      std::map<ByteString,bool>::const_iterator iter = m_LayersInfo.find(m_CurrentLayerName);
      if (iter != m_LayersInfo.end())
        state = iter->second;
      pLayer->setIsOff(!state);
    }
  }
  break;
  case 1: // Create object layers
  {
    createObjectLayers();
  }
  break;
  case 2: // Use current layer
    if (m_IsObjectVisible)
      layer_id = pDb->getCLAYER();
    else
      createObjectLayers();
    break;
  default:
    break;
  }
  return layer_id;
}

OdCmColor toCmColor(ODCOLORREF color)
{
  OdCmColor col;
  col.setColorMethod(OdCmEntityColor::kByLayer);
  OdCmColor rgb_col;
  rgb_col.setRGB(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
  OdInt16 idx = 256;
  if (0 != color)
    idx = (OdInt16)OdCmEntityColor::lookUpACI(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
  col.setColorIndex(idx);
  ODCOLORREF ref = ODRGB(col.red(), col.green(), col.blue());
  if (color != ref)
    col = rgb_col;
  return col;
}

bool OdDbDeviceDriver::DrawPath(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState,
  uint32_t fill_color, uint32_t stroke_color, int fill_mode, int blend_type)
{
  if (!m_pProperties->get_ImportVectorGeometry())
    return true;
  try
  {
    const std::vector<FX_PATHPOINT>& pPoints = pPathData->GetPoints();

    if (0 == pPoints.size())
      return true;

    OdGePoint2dArray pt_cache;
    bool fill = m_pProperties->get_ImportSolidFills() && (0 != FXARGB_A(fill_color));
    bool stroke = (0 != FXARGB_A(stroke_color));
  
    if (!stroke && !fill)
      return true;

    ODCOLORREF od_stroke_color = ODRGB(FXARGB_R(stroke_color), FXARGB_G(stroke_color), FXARGB_B(stroke_color));
    ODCOLORREF od_fill_color = ODRGB(FXARGB_R(fill_color), FXARGB_G(fill_color), FXARGB_B(fill_color));

    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());

    std::vector<BaseCurve> paths;
    paths.resize(1);

    OdGeVector3d z_axis(OdGeVector3d::kZAxis);
    double elevation = 0.0;
    OdGeMatrix2d matrix(m_TransforMatrix.convertToLocal(z_axis, elevation));

    bool inClipBoundary = false;

    for (size_t i = 0; i < pPoints.size(); i++)
    {
      CFX_PointF pos = pPoints[i].m_Point;
      if (pObject2Device)
      {
        pos = pObject2Device->Transform(pos);
      }
      pos.y += std::abs(m_Height);

      OdGePoint2d point(pos.x / DPI, pos.y / DPI);

      if (pointInPolygon(point))
        inClipBoundary = true;
      point.transformBy(matrix);

      FXPT_TYPE point_type = pPoints[i].m_Type;
      if (point_type == FXPT_TYPE::MoveTo)
      {
        if (pt_cache.size())
        {
          paths.back().addSeg(pt_cache, pPoints[i - 1].m_Type);
          if (pPoints[i - 1].m_CloseFigure || paths.back().getStorage().front().first.first().isEqualTo(pt_cache.last()))
          {
            if (inClipBoundary)
            {
              paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
              paths.emplace_back(BaseCurve());
            }
            else
            {
              paths.back().clear();
              inClipBoundary = false;
            }

          }
        }
        pt_cache.clear();
        pt_cache.append(point);
      }
      else if (point_type == FXPT_TYPE::LineTo)
      {

        if (FXPT_TYPE::BezierTo == pPoints[i - 1].m_Type)
        {
          paths.back().addSeg(pt_cache, pPoints[i - 1].m_Type);
          if (pPoints[i - 1].m_CloseFigure || paths.back().getStorage().front().first.first().isEqualTo(pt_cache.last()))
          {
            if (inClipBoundary)
            {
              paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
              paths.emplace_back(BaseCurve());
            }
            else
            {
              paths.back().clear();
              inClipBoundary = false;
            }

          }

          OdGePoint2d ptTmp = pt_cache.last();
          pt_cache.clear();
          pt_cache.append(ptTmp);
        }
        pt_cache.append(point);
      }
      else if (point_type == FXPT_TYPE::BezierTo)
      {
        if (FXPT_TYPE::LineTo == pPoints[i - 1].m_Type)
        {
          paths.back().addSeg(pt_cache, pPoints[i - 1].m_Type);
          if (pPoints[i - 1].m_CloseFigure || paths.back().getStorage().front().first.first().isEqualTo(pt_cache.last()))
          {
            if (inClipBoundary)
            {
              paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
              paths.emplace_back(BaseCurve());
            }
            else
            {
              paths.back().clear();
              inClipBoundary = false;
            }

          }

          OdGePoint2d ptTmp = pt_cache.last();
          pt_cache.clear();
          pt_cache.append(ptTmp);
        }
        pt_cache.append(point);
      }
    }
    if (FXPT_TYPE::MoveTo != pPoints.back().m_Type)
    {
      paths.back().addSeg(pt_cache, pPoints.back().m_Type);
      if (pPoints.back().m_CloseFigure || paths.back().getStorage().front().first.first().isEqualTo(pt_cache.last()))
      {
        paths.back().setClosed(pPoints.back().m_CloseFigure);
      }
      if (!inClipBoundary)
        paths.back().clear();
    }

    if (m_pProperties->get_ImportSolidFills() && fill)
      saveSolid(paths, toCmColor(od_fill_color));

    if (stroke && (!fill || od_fill_color != od_stroke_color))
      savePath(paths, toCmColor(od_stroke_color), pGraphState);
  }
  catch (...)
  {
    ++m_ErrorCounter;
  }
  return true;
}

OdStreamBufPtr convertBmpToRaster(const BITMAPINFOHEADER &bmi, const unsigned char *imagedata)
{
  OdStreamBufPtr pFileBuf = OdMemoryStream::createNew();

  OdPlatformStreamer::wrInt16(*pFileBuf, 0x4d42);
  OdUInt32 scanLinesPos = 14 + 40;  // Below version is correct only on Windows
  OdUInt32 size = scanLinesPos + bmi.biWidth * 32 * Od_abs(bmi.biHeight);

  OdPlatformStreamer::wrInt32(*pFileBuf, size);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0); // reserved
  OdPlatformStreamer::wrInt32(*pFileBuf, scanLinesPos); // offBits
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biSize);  // not portable: sizeof(BITMAPINFOHEADER));
                                                       // save BITMAPINFOHEADER
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biWidth);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biHeight);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmi.biPlanes);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmi.biBitCount);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biCompression);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biSizeImage);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biXPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biYPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biClrUsed);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biClrImportant);

  pFileBuf->putBytes(imagedata, bmi.biSizeImage);

  pFileBuf->rewind();

  return pFileBuf;
}

OdDbRasterImagePtr OdDbDeviceDriver::createRasterImage(const RetainPtr<CFX_DIBSource>& pBitmap, const OdString& layer_name)
{
  try
  {
    OdString image_def_name = OdString().format(L"%ls%ls", m_PdfFileName.c_str(),
      odrxSystemServices()->createOdGUID().toString(OdGUID::StringFormat::Digits).c_str());

    RetainPtr<CFX_DIBSource> pBitmapImpl = pBitmap;

    if (FXDIB_Rgb != pBitmap->GetFormat())
    {
      pBitmapImpl = pBitmap->CloneConvert(FXDIB_Rgb);
    }

    int width = pBitmapImpl->GetWidth(), height = pBitmapImpl->GetHeight();
    struct {
      BITMAPINFOHEADER bmiHeader;
      uint32_t bmiColors[2];
    } bmi;
    memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biBitCount = pBitmapImpl->GetBPP();
    bmi.bmiHeader.biCompression = 0L;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biSizeImage = pBitmapImpl->GetPitch()*height;

    bmi.bmiColors[0] = 0xffffff;
    bmi.bmiColors[1] = 0;

    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());

    OdStreamBufPtr image_stream_buf = convertBmpToRaster(bmi.bmiHeader, pBitmapImpl->GetBuffer());

    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (pRasSvcs.isNull())
    {
      m_ImageError = OdPdfImport::image_file_error;
      return OdDbRasterImagePtr();
    }

    OdDbObjectId imageDictId = OdDbRasterImageDef::imageDictionary(pDb);
    if (imageDictId.isNull())
    {
      imageDictId = OdDbRasterImageDef::createImageDictionary(pDb);
    }
    OdDbDictionaryPtr pImageDict = imageDictId.safeOpenObject(OdDb::kForWrite);

    OdDbRasterImageDefPtr pImageDef = OdDbRasterImageDef::createObject();
    OdDbObjectId imageDefId = pImageDict->setAt(image_def_name, pImageDef);

    OdString image_file_name = m_ImagesPath + image_def_name + L".PNG";
    try
    {
      OdStreamBufPtr image_file = odSystemServices()->createFile(
        image_file_name, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);

      image_stream_buf->rewind();
      image_stream_buf->copyDataTo(image_file);
    }
    catch (...)
    {
      m_ImageError = OdPdfImport::image_file_error;
    }

    pImageDef->setSourceFileName(image_file_name);

    if(pImageDef->image().isNull())
      pImageDef->setImage(OdGiRasterImageDesc::createObject(width, height));

    OdDbRasterImagePtr pImage = OdDbRasterImage::createObject();
    pImage->setDatabaseDefaults(pDb);
    m_pBTR->appendOdDbEntity(pImage);

    pImage->setLayer(getLayer(layer_name));

    pImage->setImageDefId(imageDefId);

    pImage->setDisplayOpt(OdDbRasterImage::kShow, true);
    pImage->setDisplayOpt(OdDbRasterImage::kShowUnAligned, true);

    return pImage;

  }
  catch (...)
  {
    ++m_ErrorCounter;
  }
  return OdDbRasterImagePtr();
}


bool OdDbDeviceDriver::SetDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left,
  int top, int blend_type) 
{
  if (!m_pProperties->get_ImportRasterImages())
    return true;

  static const OdChar* layer_name = L"PDF_Images";

  OdGePoint3d point(left / DPI, (top + std::abs(m_Height)) / DPI, 0.0);
  OdGeVector3d width_vector(pSrcRect->right / DPI, 0.0, 0.0);
  OdGeVector3d height_vector(0.0, pSrcRect->bottom / DPI, 0.0);

  OdGeVector3d z_axis(OdGeVector3d::kZAxis);
  double elevation = 0.0;
  const OdGePoint2d point1 = point.convert2d();
  const OdGePoint2d point2 = (point.convert2d() + width_vector.convert2d());
  const OdGePoint2d point3 = (point.convert2d() + height_vector.convert2d());
  const OdGePoint2d point4 = (point.convert2d() + width_vector.convert2d() + height_vector.convert2d());

  if (!pointInPolygon(point1) && !pointInPolygon(point2) &&
    !pointInPolygon(point3) && !pointInPolygon(point4))
    return true;

  OdDbRasterImagePtr pImage = createRasterImage(pBitmap, layer_name);

  if (pImage.isNull())
    return true;

  pImage->setOrientation(point, width_vector, height_vector);

  pImage->transformBy(m_TransforMatrix);

  return true;
}

bool OdDbDeviceDriver::GetClipBox(FX_RECT* pRect) 
{
  pRect->left = 0;
  pRect->right = m_Width;
  pRect->top = 0;
  pRect->bottom = m_Height;
  return true;
}

bool OdDbDeviceDriver::StretchDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, int dest_left, int dest_top, int dest_width,
  int dest_height, const FX_RECT* pClipRect, uint32_t flags, int blend_type) 
{
  return false;
}

bool OdDbDeviceDriver::StartDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, int bitmap_alpha, uint32_t color, const CFX_Matrix* pMatrix,
  uint32_t render_flags, std::unique_ptr<CFX_ImageRenderer>* handle, int blend_type)
{
  if (!m_pProperties->get_ImportRasterImages())
    return true;

  static const OdChar* layer_name = L"PDF_Images";

  FX_RECT result_clip = pMatrix->GetUnitRect().GetClosestRect();
  int stretch_width =
    static_cast<int>(ceil(FXSYS_sqrt2(pMatrix->a, pMatrix->b)));
  int stretch_height =
    static_cast<int>(ceil(FXSYS_sqrt2(pMatrix->c, pMatrix->d)));
  CFX_Matrix stretch2dest(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, stretch_height);
  stretch2dest.Concat(
    CFX_Matrix(pMatrix->a / stretch_width, pMatrix->b / stretch_width,
      pMatrix->c / stretch_height, pMatrix->d / stretch_height,
      pMatrix->e, pMatrix->f));
  CFX_Matrix dest2stretch = stretch2dest.GetInverse();

  FX_RECT m_StretchClip =
    dest2stretch.TransformRect(CFX_FloatRect(result_clip)).GetOuterRect();
  m_StretchClip.Intersect(0, 0, stretch_width, stretch_height);

  OdGePoint3d point(0, 0, 0.0);
  OdGeVector3d vW(m_StretchClip.Width() /DPI, 0.0, 0.0);
  OdGeVector3d vH(0.0, m_StretchClip.Height() / DPI, 0.0);

  OdGeMatrix3d object_matrix;
  object_matrix.setToIdentity();
  object_matrix[0][0] = dest2stretch.a;
  object_matrix[0][1] = dest2stretch.b;
  object_matrix[1][0] = dest2stretch.c;
  object_matrix[1][1] = dest2stretch.d;
  object_matrix[0][3] = (stretch2dest.e) / DPI;
  object_matrix[1][3] = (stretch2dest.f + std::abs(m_Height)) / DPI;

  OdGeVector3d z_axis(OdGeVector3d::kZAxis);
  double elevation = 0.0;
  const OdGePoint2d point1 = point.convert2d().transformBy(object_matrix.convertToLocal(z_axis, elevation));
  const OdGePoint2d point2 = (point.convert2d() + vW.convert2d()).transformBy(object_matrix.convertToLocal(z_axis, elevation));
  const OdGePoint2d point3 = (point.convert2d() + vH.convert2d()).transformBy(object_matrix.convertToLocal(z_axis, elevation));
  const OdGePoint2d point4 = (point.convert2d() + vW.convert2d() + vH.convert2d()).transformBy(object_matrix.convertToLocal(z_axis, elevation));

  if (!pointInPolygon(point1) && !pointInPolygon(point2) &&
    !pointInPolygon(point3) && !pointInPolygon(point4))
    return true;

  OdDbRasterImagePtr pImage = createRasterImage(pBitmap, layer_name); 
  if (pImage.isNull())
    return true;
    
  pImage->setOrientation(point, vW, vH);

  pImage->transformBy(object_matrix);

  pImage->transformBy(m_TransforMatrix);

  return true;
}

CPDF_Font* createFont(std::unique_ptr<CPDF_Document>& pDoc, CFX_Font* pFont, const ByteString& basefont)
{

  ByteString base_font_name = basefont;
  CPDF_Dictionary* pDict = pDoc->NewIndirect<CPDF_Dictionary>();
  pDict->SetNewFor<CPDF_Name>("Type", "Font");
  pDict->SetNewFor<CPDF_Name>("Subtype", "TrueType");
  
  if (pFont->IsBold() && pFont->IsItalic())
    base_font_name += ",BoldItalic";
  else if (pFont->IsBold())
    base_font_name += ",Bold";
  else if (pFont->IsItalic())
    base_font_name += ",Italic";

  pDict->SetNewFor<CPDF_Name>("BaseFont", base_font_name);

  CPDF_Dictionary* pDesc = pDoc->NewIndirect<CPDF_Dictionary>();
  pDesc->SetNewFor<CPDF_Name>("Type", "FontDescriptor");
  pDesc->SetNewFor<CPDF_Name>("FontName", base_font_name);
  pDict->SetNewFor<CPDF_Reference>("FontDescriptor", pDoc.get(),
    pDesc->GetObjNum());

  return pDoc->LoadFont(pDict);
}

bool OdDbDeviceDriver::DrawDeviceTextImpl(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device,
  float font_size, uint32_t color)
{
  try
  {

    CFX_Font* pImpFont;

    //fake doc for default font
    auto pDoc = pdfium::MakeUnique<CPDF_Document>(nullptr);

    pDoc->CreateNewDoc();

    CPDF_Font* loadedFont = createFont(pDoc, pFont, pFont->GetFaceName());

    OdString face_name(loadedFont->GetFace()->family_name);
    if (-1 != face_name.find(L"Chrome Sans"))
    {
      loadedFont = createFont(pDoc, pFont, "Arial");
    }
    pImpFont = loadedFont->GetFont();

    OdString font_face_name(loadedFont->GetFace()->family_name);

    OdString font_style_name = OdString(L"PDF ") + font_face_name;

    if (pFont->IsBold())
      font_style_name += L" Bold";

    if (pFont->IsItalic())
      font_style_name += L" Italic";


    OdGeMatrix3d object_matrix;
    object_matrix.setToIdentity();
    object_matrix[0][0] = pObject2Device->a;
    object_matrix[0][1] = -pObject2Device->b;
    object_matrix[1][0] = -pObject2Device->c;
    object_matrix[1][1] = pObject2Device->d;
    object_matrix[0][3] = (pObject2Device->e) / DPI;
    object_matrix[1][3] = (pObject2Device->f + std::abs(m_Height)) / DPI;

    OdString content_str;

    CFX_UnicodeEncoding encoding(pImpFont);

    double text_width(0), underline_position(0);
    double first_symbol_pos(0.0);
    bool is_first_simbol(true);

    for (int iChar = 0; iChar < nChars; ++iChar)
    {
      const FXTEXT_CHARPOS& charpos = pCharPos[iChar];

      content_str += charpos.m_Unicode;

      CFX_Matrix matrix;
      if (charpos.m_bGlyphAdjust)
      {
        matrix = CFX_Matrix(charpos.m_AdjustMatrix[0], charpos.m_AdjustMatrix[1],
          charpos.m_AdjustMatrix[2], charpos.m_AdjustMatrix[3], 0, 0);
      }

      matrix.Concat(CFX_Matrix(font_size, 0, 0, font_size, charpos.m_Origin.x, charpos.m_Origin.y));
      const CFX_PathData* pPath = pFont->LoadGlyphPath(charpos.m_GlyphIndex, charpos.m_FontCharWidth);
      if (!pPath)
        continue;

      matrix.Scale(pObject2Device->GetXUnit(), pObject2Device->GetYUnit());

      CFX_PathData TransformedPath(*pPath);
      TransformedPath.Transform(&matrix);
      CFX_FloatRect rect = TransformedPath.GetBoundingBox();
      text_width = odmax(text_width, rect.right);

      if (is_first_simbol)
      {
        first_symbol_pos = matrix.e;
        is_first_simbol = false;
      }

      uint32_t glyph_index = encoding.GlyphFromCharCode(charpos.m_Unicode);
      const CFX_PathData* pPathImpl = pImpFont->LoadGlyphPath(glyph_index, charpos.m_FontCharWidth);
      if (!pPathImpl)
        continue;
      CFX_PathData TransformedPathImpl(*pPathImpl);
      TransformedPathImpl.Transform(&matrix);
      CFX_FloatRect rect1 = TransformedPathImpl.GetBoundingBox();
      underline_position = odmin(underline_position, rect1.bottom);
    }

    content_str.trimLeft();
    content_str.trimRight();
    if (content_str.isEmpty())
      return true;

    first_symbol_pos /= DPI;

    text_width /= DPI;
    underline_position /= DPI;

    text_width -= first_symbol_pos;


    OdGePoint3d dir_point(0., underline_position, 0.);
    OdGeMatrix3d dir_matrix(object_matrix);
    dir_matrix.setTranslation(OdGeVector3d());
    dir_point.transformBy(dir_matrix);
    dir_point /= dir_matrix.scale();

    object_matrix[1][3] += dir_point.y;
    object_matrix[0][3] += dir_point.x;

    //////////////////////////////////////////////////////////////////////////
    OdGePoint3d dir_point_fs(first_symbol_pos, 0., 0.);
    dir_point_fs.transformBy(dir_matrix);
    dir_point_fs /= dir_matrix.scale();

    object_matrix[1][3] += dir_point_fs.y;
    object_matrix[0][3] += dir_point_fs.x;
    //////////////////////////////////////////////////////////////////////////

    FXFT_Face font_face = pImpFont->GetFace();

    FT_ULong charA = FT_ULong('A');

    uint32_t glyph_index = encoding.GlyphFromCharCode(charA);

    double font_height = 1000;
    if (0 != glyph_index)
    {
      if (!FXFT_Load_Glyph(font_face, glyph_index, FXFT_LOAD_NO_SCALE | FXFT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH))
        font_height = (double)FXFT_Get_Glyph_HoriBearingY(font_face);
      int em = FXFT_Get_Face_UnitsPerEM(font_face);
      if (em == 0)
      {
        font_height = FXFT_Get_Glyph_HoriBearingY(font_face);
      }
      else
      {
        font_height = FXFT_Get_Glyph_HoriBearingY(font_face) * 1000. / em;
      }
    }

    font_height = font_height * font_size * 10. / DPI;

    font_height = font_height / 10000.;

    OdDbDatabasePtr pDb = OdDbDatabasePtr(m_pProperties->get_Database());

    OdDbTextStyleTablePtr pStyles = pDb->getTextStyleTableId().safeOpenObject(OdDb::kForWrite);

    OdDbObjectId styleId = pStyles->getAt(font_style_name);

    if (styleId.isNull())
    {
      OdDbTextStyleTableRecordPtr pStyle = OdDbTextStyleTableRecord::createObject();

      pStyle->setName(font_style_name);

      styleId = pStyles->add(pStyle);

      pStyle->setTextSize(0.);

      pStyle->setFont(font_face_name, pFont->IsBold(), pFont->IsItalic(), false, 0);
    }

    OdGeVector3d z_axis(OdGeVector3d::kZAxis);
    double elevation = 0.0;
    const OdGePoint2d point1 = OdGePoint2d().transformBy(object_matrix.convertToLocal(z_axis, elevation));
    const OdGePoint2d point2 = OdGePoint2d(text_width, 0.).transformBy(object_matrix.convertToLocal(z_axis, elevation));
    const OdGePoint2d point3 = OdGePoint2d(0., font_height).transformBy(object_matrix.convertToLocal(z_axis, elevation));
    const OdGePoint2d point4 = OdGePoint2d(text_width, font_height).transformBy(object_matrix.convertToLocal(z_axis, elevation));

    if (!pointInPolygon(point1) && !pointInPolygon(point2) &&
      !pointInPolygon(point3) && !pointInPolygon(point4))
      return true;


    OdDbMTextPtr pMText = OdDbMText::createObject();
    pMText->setDatabaseDefaults(m_pBTR->database());
    m_pBTR->appendOdDbEntity(pMText);

    static const OdChar* layer_name = L"PDF_Text";
    pMText->setLayer(getLayer(layer_name));

    OdCmColor od_color = toCmColor(ODRGB(FXARGB_R(color), FXARGB_G(color), FXARGB_B(color)));
    pMText->setColor(od_color);

    pMText->setAttachment(OdDbMText::kBottomLeft);

    pMText->setDirection(OdGeVector3d::kXAxis);

    pMText->setTextHeight(font_height);

    pMText->setTextStyle(styleId);

    pMText->setContents(content_str);

    pMText->transformBy(object_matrix);

    const double coef = text_width / pMText->actualWidth();
    if (!OdEqual(coef, 1.0))
    {
      OdString format_str = OdString().format(L"\\W%f;", coef);
      format_str.replace(L',', L'.');
      content_str = format_str + content_str;
    }
    pMText->setContents(content_str);

    pMText->subTransformBy(m_TransforMatrix);

  }
  catch (...)
  {
    ++m_ErrorCounter;
  }

  return true;
}


bool OdDbDeviceDriver::DrawDeviceText(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device,
  float font_size, uint32_t color)
{
  if (m_pProperties->get_ImportTrueTypeTextAsGeometry())
    return false;
  if (!m_pProperties->get_ImportTrueTypeText())
    return true;
  try
  {

    bool is_first_simbol(true);
    double text_width(0.0), first_pos(0.0);
    std::vector<std::vector<FXTEXT_CHARPOS> > storage;
    storage.push_back(std::vector<FXTEXT_CHARPOS>());
    for (int iChar = 0; iChar < nChars; ++iChar)
    {
      const FXTEXT_CHARPOS& charpos = pCharPos[iChar];

      CFX_Matrix matrix;
      if (charpos.m_bGlyphAdjust)
      {
        matrix = CFX_Matrix(charpos.m_AdjustMatrix[0], charpos.m_AdjustMatrix[1],
          charpos.m_AdjustMatrix[2], charpos.m_AdjustMatrix[3], 0, 0);
      }

      double glyph_width = pFont->GetGlyphWidth(charpos.m_GlyphIndex);

      matrix.Concat(CFX_Matrix(font_size, 0, 0, font_size, charpos.m_Origin.x, charpos.m_Origin.y));

      matrix.Scale(pObject2Device->GetXUnit(), pObject2Device->GetYUnit());

      if (is_first_simbol)
      {
        first_pos = matrix.e;
        is_first_simbol = false;
      }

      double new_symbol_pos = matrix.e;
      if (!OdEqual(text_width, 0.) && (new_symbol_pos > (first_pos + text_width)))
      {
        first_pos = matrix.e;
        text_width = 0.0;
        storage.push_back(std::vector<FXTEXT_CHARPOS>());
      }

      storage.back().push_back(charpos);

      text_width += matrix.TransformDistance(glyph_width / 1000.) + 0.1*matrix.TransformDistance(glyph_width / 1000.);

    }

    for (auto& elem : storage)
    {
      DrawDeviceTextImpl((int)elem.size(), elem.data(), pFont, pObject2Device, font_size, color);
    }
  }
  catch (...)
  {
    ++m_ErrorCounter;
  }
  return true;
}

bool OdDbDeviceDriver::DrawCosmeticLine(const CFX_PointF& ptMoveTo, const CFX_PointF& ptLineTo, uint32_t color, int blend_type)
{
  if (!m_pProperties->get_ImportVectorGeometry())
    return true;
  try
  {
    ODCOLORREF od_color = ODRGB(FXARGB_R(color), FXARGB_G(color), FXARGB_B(color));

    OdGePoint2d point1(ptMoveTo.x / DPI, (ptMoveTo.y + std::abs(m_Height)) / DPI);
    OdGePoint2d point2(ptLineTo.x / DPI, (ptLineTo.y + std::abs(m_Height)) / DPI);

    if (!pointInPolygon(point1) && !pointInPolygon(point2))
      return true;

    OdDb::LineWeight line_widht(OdDb::kLnWtByLwDefault);

    static const OdChar* layer_name = L"PDF_Geomertry";
    OdDbObjectId layer_id = getLayer(layer_name);

    OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
    pPolyline->setDatabaseDefaults(m_pBTR->database());
    m_pBTR->appendOdDbEntity(pPolyline);
    pPolyline->setLayer(layer_id);

    pPolyline->addVertexAt(0, point1);
    pPolyline->addVertexAt(0, point2);

    pPolyline->setColor(toCmColor(od_color));
    if (m_pProperties->get_ApplyLineweight())
      pPolyline->setLineWeight(line_widht);

  }
  catch (...)
  {
    ++m_ErrorCounter;
  }

  return true;
}

bool OdDbDeviceDriver::SetPixel(int x, int y, uint32_t color)
{
  if (!m_pProperties->get_ImportVectorGeometry())
    return true;
  try
  {

    ODCOLORREF od_color = ODRGB(FXARGB_R(color), FXARGB_G(color), FXARGB_B(color));

    OdGePoint2d od_point(x / DPI, (y + std::abs(m_Height)) / DPI);

    if (!pointInPolygon(od_point))
      return true;

    static const OdChar* layer_name = L"PDF_Geomertry";
    OdDbObjectId layer_id = getLayer(layer_name);

    OdDbPointPtr pPoint = OdDbPoint::createObject();
    pPoint->setDatabaseDefaults(m_pBTR->database());
    m_pBTR->appendOdDbEntity(pPoint);
    pPoint->setLayer(layer_id);

    pPoint->setPosition(OdGePoint3d(od_point.x, od_point.y, 0));

    pPoint->setColor(toCmColor(od_color));
  }
  catch (...)
  {
    ++m_ErrorCounter;
  }
  return true;
}

bool OdDbDeviceDriver::FillRectWithBlend(const FX_RECT* pRect, uint32_t fill_color, int blend_type)
{
  return false;
}

bool OdDbDeviceDriver::DrawShadingAsBitmap(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left, int top, int blend_type)
{
  if (!m_pProperties->get_ImportGradientFills())
    return true;

  static const OdChar* layer_name = L"PDF_Gradient Fills";

  OdGePoint3d point(left / DPI, (top + std::abs(m_Height)) / DPI, 0.0);
  OdGeVector3d width_vector(pSrcRect->right / DPI, 0.0, 0.0);
  OdGeVector3d height_vector(0.0, pSrcRect->bottom / DPI, 0.0);

  OdGeVector3d z_axis(OdGeVector3d::kZAxis);
  double elevation = 0.0;
  const OdGePoint2d point1 = point.convert2d();
  const OdGePoint2d point2 = (point.convert2d() + width_vector.convert2d());
  const OdGePoint2d point3 = (point.convert2d() + height_vector.convert2d());
  const OdGePoint2d point4 = (point.convert2d() + width_vector.convert2d() + height_vector.convert2d());

  if (!pointInPolygon(point1) && !pointInPolygon(point2) &&
    !pointInPolygon(point3) && !pointInPolygon(point4))
    return true;

  OdDbRasterImagePtr pImage = createRasterImage(pBitmap, layer_name);
  if (pImage.isNull())
    return true;

  pImage->setOrientation(point, width_vector, height_vector);

  pImage->transformBy(m_TransforMatrix);

  pImage->setFade(40);

  pImage->setDisplayOpt(OdDbRasterImage::kTransparent, true);

  OdDbObjectIdArray arObjId;
  arObjId.append(pImage->id());
  OdDbSortentsTablePtr pSortentsTable = m_pBTR->getSortentsTable(true);
  pSortentsTable->moveToBottom(arObjId);

  return true;
}

bool OdDbDeviceDriver::DrawShading(const CPDF_ShadingPattern* pPattern, const CFX_Matrix* pMatrix, const FX_RECT& clip_rect, int alpha, bool bAlphaMode)
{
  return false;
}

bool OdDbDeviceDriver::SetBitsWithMask(const RetainPtr<CFX_DIBSource>& pBitmap, const RetainPtr<CFX_DIBSource>& pMask, int left, int top, int bitmap_alpha, int blend_type)
{
  return false;
}

int OdDbDeviceDriver::GetDriverType() const
{
  return 1;
}

bool OdDbDeviceDriver::pointInPolygon(const OdGePoint2d& point)
{
  const OdUInt32 points_count = m_ClipBoundary.size();
  if (0 == points_count)
    return true;
  bool res = false;

  for (OdUInt32 i = 0, j = points_count - 1; i < points_count; j = i++)
  {
    if (((m_ClipBoundary[i].y >= point.y) != (m_ClipBoundary[j].y >= point.y)) &&
      (point.x <= (m_ClipBoundary[j].x - m_ClipBoundary[i].x) * (point.y - m_ClipBoundary[i].y) / 
      (m_ClipBoundary[j].y - m_ClipBoundary[i].y) + m_ClipBoundary[i].x))
      res = !res;
  }
  return m_isClipBoundaryInverted ? !res : res;
}

