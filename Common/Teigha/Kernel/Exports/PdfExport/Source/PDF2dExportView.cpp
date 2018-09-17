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



//////////////////////////////////////////////////////////////////////
// 
//
//////////////////////////////////////////////////////////////////////


#include "PdfExportCommon.h"

#include "Pdf2dExportView.h"
#include "2dSupport.h"
#include "OdRound.h"

#include "AbstractViewPE.h"
#include "RxVariantValue.h"
#include "ColorMapping.h"

#include "Gi/GiRasterWrappers.h"
#include "Ge/GeScale3d.h"
#include "Gi/GiDrawableImpl.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeEllipArc3d.h"
#include "PdfExportService.h"

#include "OleItemHandler.h"
#include "RxRasterServices.h"
#include "MemoryStream.h"

#include "PrcContextForPdfExport.h"
#include "Pdf2PrcExportParams.h"

#include "Ge/GeExtents3d.h"
#include "Gs/GsDbRootLinkage.h"
#include "Gi/GiPathNode.h"
#include "DynamicLinker.h"
#include "DbHyperlink.h"


#if defined(ODA_WINDOWS) && !defined(_WINRT)  && !defined(_WIN32_WCE)
#include "../../../Extensions/ExRender/win/GdiBmp.h"
#else
#define drawWithGdiBmp(pDrawable, pVect, origin, u, v, bitmapWidth, bitmapHeight, rect, bMonochrome)
#endif

namespace TD_PDF_2D_EXPORT {

void PDF2dExportView::onTraitsModified()
{
  if (!device()->recordingEnabled() && device()->currentPdfPage())
  {
    device()->enableRecording();
  }
  Od2dExportView::onTraitsModified();

  const OdPsPlotStyleData& plotStyle = effectivePlotStyle();
  const OdGiSubEntityTraitsData& entityTraits = effectiveTraits();

  OdCmTransparency tr = entityTraits.transparency();
  OdUInt8 alpha = tr.alpha();
  double  alphaPercent = tr.alphaPercent();
  if (alphaPercent == 0)
  {
    device()->enableRecording(false, false); //we shouldn't plot completely transparent entities
    return;
  }

  // color
  if (userGiContext()->isPlotGeneration())
  {
    if (plotStyle.color().isByColor())
      device()->set_Color(ODTOCOLORREF(plotStyle.color()));
    else if (plotStyle.color().colorMethod() != OdCmEntityColor::kNone)
      device()->set_ColorIndex(plotStyle.color().colorIndex());
    //else continue export with previous color
  }
  else
  {
    if (entityTraits.trueColor().isByColor())
      device()->set_Color(ODTOCOLORREF(entityTraits.trueColor()));
    else if (entityTraits.trueColor().colorMethod() != OdCmEntityColor::kNone)
      device()->set_ColorIndex(entityTraits.color());
    //else continue export with previous color
  }

  device()->set_Layer(entityTraits.layer());

  bool bLwDisplay = view().hasLweights() || view().lineweightToDcScale() != 0.0 || hasLineweightOverride();
  double lw = OdDb::kLnWt000;

  if (userGiContext()->isPlotGeneration() && userGiContext()->plotStyleType() != OdGiContext::kPsNone)
  {
    //LineWeight
    lw = lineweightToPixels(plotStyle.lineweight());

    OdPsPlotStyleData zeroLwPlotStyle = plotStyle;
    zeroLwPlotStyle.setLineweight(0);
    plotGenerator().setPlotStyle(zeroLwPlotStyle);
    plotGenerator().enable(true);

    OdPs::LineEndStyle  lnEndStyle = plotStyle.endStyle();
    OdPs::LineJoinStyle lnJoinStyle = plotStyle.joinStyle();
    if (hasLinestyleOverride())
    {
      lnEndStyle  = currentLineweightOverride()->endStyleOverride();
      lnJoinStyle = currentLineweightOverride()->joinStyleOverride();
    }
    TD_PDF::PDFLineCap  uLineCap = kRoundCap;
    TD_PDF::PDFLineJoin uLineJoin = kRoundJoin;
    if (lnEndStyle == OdPs::kLesSquare)
      uLineCap = kProjectingSquareCap;
    else if (lnEndStyle == OdPs::kLesButt)
      uLineCap = kButtCap;
    if (lnJoinStyle == OdPs::kLjsMiter)
      uLineJoin = kMiterJoin;
    else if (lnJoinStyle == OdPs::kLjsBevel)
      uLineJoin = kBevelJoin;
    device()->setCapJoin(uLineCap, uLineJoin);
  }
  else
  {
    lw = lineweightToPixels(entityTraits.lineWeight());
    plotGenerator().enable(false);
  }

  if (!m_pSolidHatch.isNull())
    lw = OdDb::kLnWt000; //for hatch we will draw just contour, so there is no need for lw, even if we won't stroke the path (kFillStrokeCloseRule or other rule)

  if (isPlotTransparency() && device()->exportParams().archived() != PDFExportParams::kPDFA_1b)
  {
    device()->dc_transparency(alphaPercent, alpha, GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kMergeLines));
  }

  if (bLwDisplay)
    device()->set_LineWeight(OdRoundToLong(lw));
  else
    device()->set_LineWeight(OdDb::kLnWt000); // Avoid dynamic sublists creation if 'lwdisplay' disabled, but now regeneration needed if 'lwdisplay' sysvar changed.

  if (!m_bDarkenGSSet && GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kMergeLines) && device()->exportParams().archived() != PDFExportParams::kPDFA_1b)
  {
    device()->dc_mergeLines();
    m_bDarkenGSSet = true;
  }

  // secondary color

  if( GETBIT(entityTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill) )
  {
    OdPsPlotStyleData plotStyleSecondary;

    if( userGiContext()->isPlotGeneration() )
    {
      OdCmEntityColor oldColor = m_effectiveEntityTraitsData.trueColor();
      m_effectiveEntityTraitsData.setTrueColor(entityTraits.secondaryTrueColor());
      plotStyleSecondary = effectivePlotStyle();
      m_effectiveEntityTraitsData.setTrueColor(oldColor);
    }

    if( userGiContext()->isPlotGeneration() )
    {
      if (plotStyleSecondary.color().isByColor())
        device()->set_SecondaryColor(ODTOCOLORREF(plotStyleSecondary.color()));
      else if (plotStyleSecondary.color().colorMethod() != OdCmEntityColor::kNone)
        device()->set_SecondaryColorIndex(plotStyleSecondary.color().colorIndex());
      //else continue export with previous color
    }
    else
    {
      if (entityTraits.secondaryTrueColor().isByColor())
        device()->set_SecondaryColor(ODTOCOLORREF(entityTraits.secondaryTrueColor()));
      else if (entityTraits.secondaryTrueColor().colorMethod() != OdCmEntityColor::kNone)
        device()->set_SecondaryColorIndex(entityTraits.secondaryTrueColor().colorIndex());
      //else continue export with previous color
    }
  }
}

void PDF2dExportView::beginViewVectorization()
{
  Od2dExportView ::beginViewVectorization();

  device()->m_bRasterClipping = false;

  OdUInt16 geomDPI = device()->exportParams().getGeomDPI();
  double scale = 72./(double)geomDPI;

  if (GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kZoomToExtentsMode) != 0 && device()->m_LwToDcScale != 0 && !hasLweights()) //We need to calculate LW without plotSettings in Z2E mode (CORE-11936)
    setLineweightToDcScale(device()->m_LwToDcScale);

  if(giContext().isPlotGeneration() && &output().destGeometry() != &OdGiEmptyGeometry::kVoid) // In extents mode destGeometry() should be empty. Code below change destGeometry(). So we don't use plot generator in the extents mode.
  {
    OdGiPlotGenerator& pgen = plotGenerator();
    pgen.setDrawContext(drawContext());
    OdInt32 dpi = 72;
    if(!device()->properties().isNull() && device()->properties()->has(OD_T("DPI"))) 
      dpi = OdRxVariantValue(device()->properties()->getAt(OD_T("DPI")))->getInt32();
    pgen.dot_per_inch(dpi);
    pgen.input().addSourceNode(screenOutput());
    pgen.output().setDestGeometry(*device());

    // BR : rev6501
    // PDF page size is set in mm, therefore deviation was calculated too rough.
    // Now we take into account dpi of PDF during calculation deviation.
    const OdGiDeviation& deviationObj = m_pModelToEyeProc->eyeDeviation();
    OdGeDoubleArray values(5);
    values.append(deviationObj.deviation(kOdGiMaxDevForCircle,   OdGePoint3d::kOrigin)/pgen.dot_per_mm());
    values.append(deviationObj.deviation(kOdGiMaxDevForCurve,    OdGePoint3d::kOrigin)/pgen.dot_per_mm());
    values.append(deviationObj.deviation(kOdGiMaxDevForBoundary, OdGePoint3d::kOrigin)/pgen.dot_per_mm());
    values.append(deviationObj.deviation(kOdGiMaxDevForIsoline,  OdGePoint3d::kOrigin)/pgen.dot_per_mm());
    values.append(deviationObj.deviation(kOdGiMaxDevForFacet,    OdGePoint3d::kOrigin)/pgen.dot_per_mm());
    pgen.setDeviation(values);
  }

  //////////////////////////////////////////////////////////////////////////
  OdGeDoubleArray dev(5);
  dev.append(calcScaledDeviation(kOdGiMaxDevForCircle, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForCurve, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForBoundary, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForIsoline, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForFacet, OdGePoint3d::kOrigin));
  device()->setDeviation(dev);
  //////////////////////////////////////////////////////////////////////////

  device()->StartOptimizator();

  OdUInt32  uCapStyle   = giContext().lineWeightConfiguration(OdGiContext::kLineCapStyle);
  OdUInt32  uJointStyle = giContext().lineWeightConfiguration(OdGiContext::kLineJoinStyle);

  TD_PDF::PDFLineCap  uLineCap  = kRoundCap; 
  TD_PDF::PDFLineJoin uLineJoin = kRoundJoin;

  if( uCapStyle == 0 )
    uLineCap = kButtCap;
  else if( uCapStyle == 1 )
    uLineCap = kProjectingSquareCap;

  if( uJointStyle == 0 )
    uLineJoin = kMiterJoin;
  else if( uJointStyle == 1 )
    uLineJoin = kBevelJoin;

  device()->setCapJoin(uLineCap, uLineJoin);

  // Init GsPdfView class members ...
  // m_curRGB = device()->getBackgroundColor();
  //device()->set_Color(device()->getBackgroundColor());

  // m_lineWeight = OdDb::LineWeight(-100);
  device()->set_LineWeight(0);

  setViewportBorderVisibility(false);

  device()->set_Color( subEntityTraits().trueColor().color() );


  device()->dc_mx();
  pushViewportClip();
  setDrawContextFlags(OdGiConveyorContext::kSpatialFilterSimplPline, false);
}

void PDF2dExportView::endViewVectorization()
{
  if (!device()->recordingEnabled() && device()->currentPdfPage())
  {
    device()->enableRecording();
  }

  if(giContext().isPlotGeneration())
  {
    plotGenerator().input().removeSourceNode(screenOutput());
    screenOutput().setDestGeometry(*device());
  }

  Od2dExportView ::endViewVectorization();
  device()->StopOptimizator();
  popViewportClip();
}

void PDF2dExportView::loadViewport()
{
  Od2dExportView::loadViewport();

  OdGsDCPoint min, max;
  screenRect(min, max);
  //////////////////////////////////////////////////////////////////////////
  OdGsDCRect rr = OdGsDCRect(min, max);
  rr.normalize();
  OdGsDCRect tmp = m_screenRect;
  tmp.intersectWith(rr);
  if (!tmp.is_null())
    m_screenRect = tmp;
  
  const int nrcCountsOut = 4;
  OdGsDCPointArray nrcPointsOut;
  OdGeExtents2d devPaperClip;
  nrcPointsOut.resize(4);
  nrcPointsOut[0] = m_screenRect.m_min;
  nrcPointsOut[1].x = m_screenRect.m_max.x;
  nrcPointsOut[1].y = m_screenRect.m_min.y;
  nrcPointsOut[2] = m_screenRect.m_max;
  nrcPointsOut[3].x = m_screenRect.m_min.x;
  nrcPointsOut[3].y = m_screenRect.m_max.y;

  devPaperClip.set(OdGePoint2d(double(m_screenRect.m_min.x), double(m_screenRect.m_min.y)),
    OdGePoint2d(OdRoundToLong(double(m_screenRect.m_max.x)), OdRoundToLong(double(m_screenRect.m_max.y))));

  device()->m_paperClip = devPaperClip;
  device()->dc_pushClip(1, &nrcCountsOut, nrcPointsOut);

  //expand clip box because of OdGiOrthoClipper clips the entities on the border
  if (m_screenRect.m_min.x > 0)
    m_screenRect.m_min.x--;
  if (m_screenRect.m_min.y > 0)
    m_screenRect.m_min.y--;
  m_screenRect.m_max.x++;
  m_screenRect.m_max.y++;
  //////////////////////////////////////////////////////////////////////////

  const OdGePoint2d pts[4] = { OdGePoint2d(m_screenRect.m_min.x, m_screenRect.m_min.y),
    OdGePoint2d((double)m_screenRect.m_max.x, (double)m_screenRect.m_min.y),
    OdGePoint2d(m_screenRect.m_max.x, m_screenRect.m_max.y),
    OdGePoint2d((double)m_screenRect.m_min.x, (double)m_screenRect.m_max.y) };
  const int ptsCounts = 4;
  m_pViewportClip->popClipStage();

  if (!view().isPerspective()) //workaround till CORE-11417
  {
    OdGeMatrix3d xToDc = eyeToScreenMatrix();

    setEyeToOutputTransform(xToDc);

    OdGeVector3d zAxis = OdGeVector3d::kZAxis;
    OdGeVector3d dcFront(0., 0., frontClip());
    OdGeVector3d dcBack(0., 0., backClip());
    dcFront.transformBy(xToDc);
    dcBack.transformBy(xToDc);
    zAxis.transformBy(xToDc);

    if (zAxis.z > 0.)
    {
      m_pViewportClip->set(ptsCounts, pts, isBackClipped(), dcBack.z, isFrontClipped(), dcFront.z);
    }
    else
    {
      m_pViewportClip->set(ptsCounts, pts, isFrontClipped(), dcFront.z, isBackClipped(), dcBack.z);
    }
    m_pXYProjector->enable();
  }
  else
  {
    m_pViewportClip->pushClipStage(1, &ptsCounts, 4, pts);
  }

  OdGeDoubleArray dev(5);
  dev.append(calcScaledDeviation(kOdGiMaxDevForCircle, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForCurve, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForBoundary, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForIsoline, OdGePoint3d::kOrigin));
  dev.append(calcScaledDeviation(kOdGiMaxDevForFacet, OdGePoint3d::kOrigin));
  m_pViewportClip->setDeviation(dev);
  //m_pViewportClip->disable();
}

void savePalleteFromDeviceToParams(PDF2PRCExportParams &params, const PDF2dExportDevice &device)
{
  // get color pallet from rendering device
  int numcolors;
  OdUInt64 ppallete = (OdUInt64) device.getLogicalPalette(numcolors);
  params.m_extraOptions->putAt("Pallet", OdRxVariantValue(ppallete));
  params.m_extraOptions->putAt("NumColors", OdRxVariantValue((OdUInt32)numcolors));
}

long OdSafeRoundToLong(double a)
{
  if (a >= 0.)
  {
    a += .5;
    if (a > double(LONG_MAX))
      return LONG_MAX;
    return long(a);
  }
  else
  {
    a -= .5;
    if (a < double(LONG_MIN))
      return LONG_MIN;
    return long(a);
  }
}

void PDF2dExportView::pushViewportClip()
{
  long min_x, max_x, min_y, max_y;
  OdGsDCPoint wtMin, wtMax;
  screenRect(wtMin, wtMax);
  m_screenRect = OdGsDCRect(wtMin, wtMax);
  m_screenRect.normalize();
  OdGsDCRect outRect = device()->outputRect();
  outRect.normalize();
  m_screenRect.intersectWith(outRect);

  if (device()->m_clipPoints.size() == 4) //for now there is always rectangular clipping
  {
    OdGePoint2dArray clipPoints = device()->m_clipPoints;
    int nrcCountsOut = 4;
    //find min & max points of clip rectangle
    min_x = clipPoints[0].x, max_x = clipPoints[0].x, min_y = clipPoints[0].y, max_y = clipPoints[0].y;
    for (int f = 1; f < nrcCountsOut - 1; ++f)
    {
      if (clipPoints[f].x < min_x)
        min_x = clipPoints[f].x;
      else if (clipPoints[f].x > max_x)
        max_x = clipPoints[f].x;
      if (clipPoints[f].y < min_y)
        min_y = clipPoints[f].y;
      else if (clipPoints[f].y > max_y)
        max_y = clipPoints[f].y;
    }
    OdGsDCRect clipRect(OdGsDCPoint(min_x, min_y), OdGsDCPoint(max_x, max_y));
    OdGePoint2d crMinP(min_x, min_y), crMaxP(max_x, max_y), srMinP(m_screenRect.m_min.x, m_screenRect.m_min.y), srMaxP(m_screenRect.m_max.x, m_screenRect.m_max.y);
    //if difference between these points is just 1 unit (distanceTo <= sqrt(2)), then additional clip is not needed
    if (!crMinP.isEqualTo(srMinP, sqrt(2.)) || !crMaxP.isEqualTo(srMaxP, sqrt(2.)))
      m_screenRect.intersectWith(clipRect);
    if (m_screenRect.is_null()) //screen rect is not intersecting with clip rect
      m_screenRect = OdGsDCRect(0, 0, 1, 1);
  }

  if(isNonRectClipped())
  {
    OdGsDCPointArray nrcPoints;
    OdIntArray nrcCounts;
    viewportClipRegion(nrcCounts, nrcPoints);
    device()->dc_pushClip(nrcCounts.size(), nrcCounts.getPtr(), nrcPoints);
    m_bAdditionalClip = true;
  }

  //temporarily fix for screen rect (till translation matrix (cm) is not added)
  OdUInt16 geomDPI = device()->exportParams().getGeomDPI();
  double scale = 72. / (double)geomDPI;
  m_screenRect.m_min.x = scale * (double)m_screenRect.m_min.x;
  m_screenRect.m_min.y = scale * (double)m_screenRect.m_min.y;
  if (m_screenRect.m_max.x > 0)
    m_screenRect.m_max.x = OdSafeRoundToLong(scale * (double)m_screenRect.m_max.x) + 1;   //we have to expand clipping borders a bit
  if (m_screenRect.m_max.y > 0)
    m_screenRect.m_max.y = OdSafeRoundToLong(scale * (double)m_screenRect.m_max.y) + 1;

  if (m_screenRect.m_min.x > 0)
    m_screenRect.m_min.x -= 1;
  if (m_screenRect.m_min.y > 0)
    m_screenRect.m_min.y -= 1;

  m_screenRect.m_min.x = (double)m_screenRect.m_min.x / scale;
  m_screenRect.m_min.y = (double)m_screenRect.m_min.y / scale;
  m_screenRect.m_max.x = (double)m_screenRect.m_max.x / scale;
  m_screenRect.m_max.y = (double)m_screenRect.m_max.y / scale;

// The code below was moved in loadViewport() to provide more accurate clipping 
//   const int nrcCountsOut = 4;
//   OdGsDCPointArray nrcPointsOut;
//   OdGeExtents2d devPaperClip;
//   nrcPointsOut.resize(4);
//   nrcPointsOut[0]   = m_screenRect.m_min;
//   nrcPointsOut[1].x = m_screenRect.m_max.x;
//   nrcPointsOut[1].y = m_screenRect.m_min.y;
//   nrcPointsOut[2]   = m_screenRect.m_max;
//   nrcPointsOut[3].x = m_screenRect.m_min.x;
//   nrcPointsOut[3].y = m_screenRect.m_max.y;
// 
//   devPaperClip.set(OdGePoint2d(double(m_screenRect.m_min.x), double(m_screenRect.m_min.y)),
//                    OdGePoint2d(OdRoundToLong(double(m_screenRect.m_max.x)), OdRoundToLong(double(m_screenRect.m_max.y))));
// 
//   device()->m_paperClip = devPaperClip;
//   device()->dc_pushClip(1, &nrcCountsOut, nrcPointsOut);
}

void PDF2dExportView::popViewportClip()
{
  if (m_bAdditionalClip)
  {
    device()->dc_popClip();
    m_bAdditionalClip = false;
  }
  device()->dc_popClip();
  device()->m_paperClip = OdGeExtents2d();
}

OdGiPlotGenerator& PDF2dExportView::plotGenerator()
{
  if(m_pPlotstyler.isNull())
  {
    m_pPlotstyler = OdGiPlotGenerator::createObject();
    OdGeDoubleArray devs(5,1);
    devs.insert(devs.end(), 5, .5);
    m_pPlotstyler->setDeviation(devs);
    m_pPlotstyler->setExternalEllipseOut(device());
  }
  return *m_pPlotstyler;
}

bool PDF2dExportView::viewExtents(OdGeBoundBlock3d& extents) const
{
  m_bExtentsMode = true;
  bool b = Od2dExportView::viewExtents(extents);
  m_bExtentsMode = false;
  return b;
}

#if 0

static OdDbBlockReferencePtr cloneHatchToNewDatabase(OdDbDatabasePtr &pNewDb, const OdDbHatch *pH, const OdGeMatrix3d &mx, const OdGiLayerTraitsData& traits)
{
  // Clone hatch to new block
  pNewDb = pH->database()->appServices()->createDatabase();

  OdDbObjectIdArray objectIds;
  objectIds.push_back( pH->objectId() );

  OdDbBlockTablePtr pBlockTable = pNewDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);

  OdDbBlockTableRecordPtr pNewBlockRecord = OdDbBlockTableRecord::createObject();
  pNewBlockRecord->setName("NewBlock");

  OdDbObjectId ownerId = pBlockTable->add(pNewBlockRecord);
  
  OdDbIdMappingPtr pMap = OdDbIdMapping::createObject();
  pMap->setDestDb(pNewDb);

  pH->database()->wblockCloneObjects(objectIds, ownerId, *pMap, OdDb::kDrcReplace, false);

  // fill traits
  OdDbIdPair idPair;
  idPair.setKey(pH->objectId());
  pMap->compute(idPair);
  OdDbHatchPtr pNewHatch = idPair.value().safeOpenObject(OdDb::kForWrite);

  OdCmEntityColor cmEntColor = traits.trueColor();
  OdCmColor cmColor; 
  if(cmEntColor.isByColor())
  {
    cmColor.setRGB(cmEntColor.red(), cmEntColor.green(), cmEntColor.blue());
  }
  else
  {
    cmColor.setColorIndex(cmEntColor.color());
  }
  pNewHatch->setColor(cmColor);

  // add block reference to block with cloned hatch to model space
  OdDbBlockTableRecordPtr pModel = pNewDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  OdDbBlockReferencePtr pRef = OdDbBlockReference::createObject();
  pRef->setBlockTableRecord(ownerId);
  pModel->appendOdDbEntity(pRef);

  // apply object to device transform to block reference
  pRef->transformBy(mx);

  return pRef;
}

static OdGsDevicePtr setupDevice(OdDbDatabasePtr pNewDb, OdDbBlockReferencePtr pRef, const OdGeExtents3d &extents, double dScale)
{
  // use OpenGl module as rendering device

  OdGsModulePtr pGs;

  try
  {
    pGs = odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName, false);
  }
  catch (OdError_FileNotFound e)
  {
  	
  }
  catch (...)
  {
    return OdGsModulePtr();
  }
  
  if (pGs.isNull())
  {
    try
    {
      pGs = odrxDynamicLinker()->loadModule(OdWinBitmapModuleName, false);
    }
    catch (...)
    {
    }

    if (pGs.isNull())
    {
      return OdGsModulePtr();
    }
  }

  OdGsDevicePtr pDevice = pGs->createBitmapDevice();

  OdGiContextForDbDatabasePtr pCtx = OdGiContextForDbDatabase::createObject();
  pCtx->setDatabase(pNewDb);

  pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pCtx);

  OdGePoint3d  target = (extents.maxPoint() + extents.minPoint().asVector()) / 2.;
  OdGeVector3d direction = OdGeVector3d::kZAxis;
  OdGeVector3d upVector = OdGeVector3d::kYAxis;
  OdGePoint3d  minP = extents.minPoint();
  double       fieldWidth  = extents.maxPoint().x - extents.minPoint().x
             , fieldHeight = extents.maxPoint().y - extents.minPoint().y;

  OdGsViewPtr pDV = OdGsModelLayoutHelperPtr(pDevice)->activeView();
  OdAbstractViewPEPtr(pDV)->setView(pDV, target, direction, upVector, fieldWidth, fieldHeight, false);

  pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(24)));

  ODCOLORREF bg = ODRGB(255,255,255); // palette background
  const ODCOLORREF* palette = odcmAcadPalette(bg);

  ODGSPALETTE pPalCpy;
  pPalCpy.insert(pPalCpy.begin(), palette, palette+256);

  pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);

  OdGsDCRect gsRect(0, (fieldWidth + 0.5)*dScale, (fieldHeight + 0.5)*dScale, 0 );
  if (gsRect.m_max.x == 0 || gsRect.m_min.y == 0)
    return OdGsDevicePtr();

  pDevice->onSize(gsRect);

  return pDevice;
}

bool PDF2dExportView::doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable)
{
  try
  {
    OdDbHatchPtr pH = OdDbHatch::cast(pDrawable);

    // TODO : add new export parameter - export non grad hatch as bitmap
    // TODO2: probably DPI for hatch should added as parameter too
    if (pH.isNull() || m_bExtentsMode /*|| !pH->isGradient()*/)
    {
      return Od2dExportView::doDraw(drawableFlags, pDrawable);
    }

    // create new database and clone hatch to model space
    OdDbDatabasePtr pNewDb;

    const OdGiLayerTraitsData& traits = effectiveLayerTraits();

    OdDbBlockReferencePtr pRef = cloneHatchToNewDatabase(pNewDb, pH, objectToDeviceMatrix(), traits);

    // create bitmap from model space of new database

    OdGeExtents3d extents;
    if (pRef->getGeomExtents(extents) != ::eOk || !extents.isValidExtents())
    {
      return Od2dExportView::doDraw(drawableFlags, pDrawable);
    }

    // can be used for changing of DPI (1. = 72 DPI)
    double dScale = 2.;

    // Setup rendering device to render viewport with cloned hatch
    OdGsDevicePtr pDevice = setupDevice(pNewDb, pRef, extents, dScale);
    if (pDevice.isNull())
    {
      return Od2dExportView::doDraw(drawableFlags, pDrawable);
    }

    // create mask 1 with white background
    pDevice->setBackgroundColor(ODRGB(255,255,255));
    pDevice->update();
    OdGiRasterImagePtr pRas1 = OdGiRasterImagePtr(pDevice->properties()->getAt(OD_T("RasterImage")));

    // convert from RGB to RGBA
    OdGiRasterImagePtr pCopyImg = convertToRGBA(pRas1);

    // create mask 2 with black background
    pDevice->setBackgroundColor(ODRGB(0,0,0));
    pDevice->update();
    pRas1 = OdGiRasterImagePtr(pDevice->properties()->getAt(OD_T("RasterImage")));

    // Merge both images to create Bitmap with transparency
    createTransparentImage(pCopyImg, pRas1);

    // render it
    OdGePoint3d  minP = extents.minPoint();
    device()->rasterImageProc( minP, OdGeVector3d::kXAxis/dScale, OdGeVector3d::kYAxis/dScale, pCopyImg, 0, 0);

  // Test code to draw image boundaries
  #if 0
    device()->set_Color(ODRGB(0,255,0));

    OdGePoint3dArray pnts;
    pnts.push_back(minP);
    pnts.push_back(minP+OdGeVector3d::kXAxis/dScale*pCopyImg->pixelWidth());
    pnts.push_back(minP+OdGeVector3d::kXAxis/dScale*pCopyImg->pixelWidth()+OdGeVector3d::kYAxis/dScale*pCopyImg->pixelHeight());
    pnts.push_back(minP+OdGeVector3d::kYAxis/dScale*pCopyImg->pixelHeight());
    pnts.push_back(minP);

    device()->polylineOut(pnts.size(), pnts.asArrayPtr());

    OdGePoint3dArray pnts2;
    pnts2.push_back(extents.minPoint());
    pnts2.push_back(extents.maxPoint());
    
    device()->polylineOut(pnts2.size(), pnts2.asArrayPtr());
  #endif
  }
  catch (...)
  {
    return Od2dExportView::doDraw(drawableFlags, pDrawable);
  }

  return true;
}
#endif

#define ENABLE_HATCH_AS_BITMAP
#ifdef ENABLE_HATCH_AS_BITMAP

static void createTransparentImage(OdGiRasterImagePtr pCopyImg, OdGiRasterImagePtr pRas1)
{
  OdUInt32 srcScanLineSz = pCopyImg->scanLineSize();
  OdUInt32 srcScanLineSz2 = pRas1->scanLineSize();
  OdUInt8Array srcScLnBuf2;
  srcScLnBuf2.resize(srcScanLineSz2, 0);
  OdUInt8 *pSrcScLnBuf2 = srcScLnBuf2.asArrayPtr();

  OdUInt32 dstY, dstX, dstHeight = pCopyImg->pixelHeight(), dstWidth = pCopyImg->pixelWidth();

  OdUInt8Array tmpScanline4Copy;
  const OdUInt8 *pScanline4Copy = pCopyImg->scanLines();
  if (!pScanline4Copy)
  {
    tmpScanline4Copy.resize(srcScanLineSz * pCopyImg->pixelHeight());
    pCopyImg->scanLines(tmpScanline4Copy.asArrayPtr(), 0, pCopyImg->pixelHeight());
    pScanline4Copy = tmpScanline4Copy.getPtr();
  }

  for(dstY = 0; dstY < dstHeight; dstY ++)
  {
    OdUInt8 *pRGBA =(OdUInt8 *)pScanline4Copy + dstY*srcScanLineSz;

    pRas1->scanLines(pSrcScLnBuf2, dstY);
    OdUInt8 *pRGB  = pSrcScLnBuf2;

    dstX = 0;
    for(; dstX < dstWidth; dstX ++)
    {
      if (pRGB[0]==0 && pRGB[1]==0 && pRGB[2]==0 &&
        pRGBA[0]==255 && pRGBA[1]==255 && pRGBA[2]==255)
      {
        pRGBA[3] = 0; // transparent
      }
      else
      {
        pRGBA[3] = 255;
      }

      pRGB += 3;
      pRGBA += 4;
    }
  }
}

static OdGiRasterImagePtr convertToRGBA(OdGiRasterImagePtr pRas1)
{
  OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
  pDesc->setPixelWidth(pRas1->pixelWidth());
  pDesc->setPixelHeight(pRas1->pixelHeight());
  pDesc->setColorDepth(32);
  pDesc->pixelFormat().setRGBA();
  pDesc->setScanLinesAlignment(4);

  return pRas1->convert(true, 50, 50, 0, ODRGB(255,255,255), false, false, false, pDesc, true);
}

static OdGsDevicePtr getRenderingDevice()
{
  OdGsModulePtr pGs;

#if 0
  try
  {
    pGs = odrxDynamicLinker()->loadModule(OdWinDirectXModuleName, false);
  }
  catch (...)
  {
  }

  if (pGs.isNull())
#endif
  {
    try
    {
      pGs = odrxDynamicLinker()->loadModule(OdWinBitmapModuleName, false);
    }
    catch (...)
    {
    }  
  }

  if (pGs.isNull())
    return OdGsDevicePtr();

  OdGsDevicePtr pDevice = pGs->createBitmapDevice();
  // AMark : enable precision triangles rendering mode for bitmap device
  OdRxDictionaryPtr pDevProps = pDevice->properties();
  if (!pDevProps.isNull())
  {
    if (pDevProps->has(OD_T("PrecisionMode")))
      pDevProps->putAt(OD_T("PrecisionMode"), OdRxVariantValue(true));
  }
  return pDevice;
}

class myDrawable : public OdStaticRxObject<OdGiDrawableImpl<OdGiDrawable> >
{
  OdInt32 m_numVertices;
  const OdGePoint3d* m_vertexList;
  OdInt32 m_faceListSize;
  const OdInt32* m_faceList;
  const OdGiEdgeData* m_pEdgeData;
  const OdGiFaceData* m_pFaceData;
  const OdGiVertexData* m_pVertexData;
  OdGsBaseVectorizeView *m_pView;
  OdGiSubEntityTraitsData* m_traits;
public:
  void set(OdGiSubEntityTraitsData* traits, OdGsBaseVectorizeView *pView, OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData)
  {
    m_pView = pView;
    m_numVertices = numVertices;
    m_vertexList = vertexList;
    m_faceListSize = faceListSize;
    m_faceList = faceList;
    m_pEdgeData = pEdgeData;
    m_pFaceData = pFaceData;
    m_pVertexData = pVertexData;
    m_traits = traits;
  }
  
  OdUInt32 subSetAttributes (OdGiDrawableTraits * traits) const
  {
    return 0;
  }
  
  virtual bool subWorldDraw (OdGiWorldDraw * wd) const
  {
    wd->subEntityTraits().setTrueColor(m_traits->trueColor());
    wd->subEntityTraits().setFillType(m_traits->fillType());
    wd->geometry().shell(m_numVertices, m_vertexList, m_faceListSize, m_faceList, m_pEdgeData, m_pFaceData, m_pVertexData);
    return false;
  }                           
};

static OdGeNurbCurve3d* convert2dto3d(const OdGeNurbCurve2d* pNc2d)
{
  OdGePoint3dArray ctrlPts;
  ctrlPts.resize(pNc2d->numControlPoints());
  OdGeDoubleArray  weights;
  weights.resize(pNc2d->numWeights());
  ODA_ASSERT(weights.size()==0 || weights.size()==ctrlPts.size());
  for(OdInt32 i = 0; i < pNc2d->numControlPoints(); i ++)
  {
    ctrlPts[i].x = pNc2d->controlPointAt(i).x;
    ctrlPts[i].y = pNc2d->controlPointAt(i).y;
    ctrlPts[i].z = 0.0;
    if(pNc2d->numWeights())
      weights[i] = pNc2d->weightAt(i);
  }
  return new OdGeNurbCurve3d(pNc2d->degree(), pNc2d->knots(), ctrlPts, weights);
}

static OdGeNurbCurve3d* ellipse2dtoNurbs3d(const OdGeEllipArc2d& arc)
{
  OdGeEllipArc3d arc3d;
  OdGePoint3d center(arc.center().x, arc.center().y, 0.);
  OdGeVector3d axisMajor(arc.majorAxis().x, arc.majorAxis().y, 0.);
  OdGeVector3d axisMinor(arc.minorAxis().x, arc.minorAxis().y, 0.);
  arc3d.set(center, axisMajor, axisMinor, arc.majorRadius(), arc.minorRadius(), arc.startAng(), arc.endAng());
  return new OdGeNurbCurve3d(arc3d);
}

void PDF2dExportView::shell(OdInt32 numVertices,
           const OdGePoint3d* vertexList,
           OdInt32 faceListSize,
           const OdInt32* faceList,
           const OdGiEdgeData* pEdgeData,
           const OdGiFaceData* pFaceData,
           const OdGiVertexData* pVertexData)
{
  if (m_bHatchToBitmapMode)
  {
    OdDbBaseHatchPE* pH = OdGsDbRootLinkage::getDbBaseHatchPE(m_pSolidHatch);
    if (pH)
    {
      PDF2dExportDevice::PolygonFillRules prevRule = device()->setFillRule(PDF2dExportDevice::kNoFill);
      try
      {
        {
          OdUInt32 f(0);
          double dev(1e-3);
          OdGiWorldDraw* pWd = static_cast<OdGiWorldDraw*>(&m_effectiveLayerTraits);
          if (pWd)
          {
            dev = pWd->deviation(kOdGiMaxDevForCurve, OdGePoint3d::kOrigin);
            if (OdZero(dev))
              dev = 1e-3;
          }
  
          for (int l = 0; l < pH->numLoops(m_pSolidHatch); ++l)
          {
            bool bEmptyContour = true;
            OdInt32 type = pH->loopTypeAt(m_pSolidHatch, l);
            OdDbBaseHatchPE::HatchStyle hs = pH->hatchStyle(m_pSolidHatch);
            if (
              (hs == OdDbBaseHatchPE::kIgnore && (type & OdDbBaseHatchPE::kOutermost)) ||
              (hs == OdDbBaseHatchPE::kOuter && ((~type & OdDbBaseHatchPE::kOutermost) && (~type & OdDbBaseHatchPE::kExternal))) ||
              type & OdDbBaseHatchPE::kDuplicate ||
              type & OdDbBaseHatchPE::kTextbox || type & OdDbBaseHatchPE::kTextIsland || // TextBox + TextIsland loops are ignored by ACAD
              ((~type & OdDbBaseHatchPE::kPolyline) && (type & OdDbBaseHatchPE::kNotClosed))
               )
            {
              goto nextloop;
              //continue;
            }

            device()->startDrawingContour();
            if ((type & OdDbBaseHatchPE::kPolyline) != 0)
            {
              OdGePoint2dArray vertices;
              OdGeDoubleArray bulges;
              pH->getLoopAt(m_pSolidHatch, l, vertices, bulges);
              const bool bClosed = !(type & OdDbBaseHatchPE::kNotClosed);
              
              OdGePoint3dArray pnts3d;
              const unsigned int n = bClosed ? vertices.size() : vertices.size() - 1;
              for(unsigned i = 0; i < n; ++i)
              {
                const OdGePoint2d& pt = vertices[i];
                const double bulge = bulges.isEmpty() ? 0. : bulges[i];
                if(OdZero(bulge))
                {
                  pnts3d.push_back(OdGePoint3d(pt.x, pt.y, 0.));
                  continue;
                }

                if(!pnts3d.isEmpty())
                {
                  pnts3d.push_back(OdGePoint3d(pt.x, pt.y, 0.));
                  polyline(pnts3d.logicalLength(), pnts3d.asArrayPtr());
                  pnts3d.clear();
                  bEmptyContour = false;
                }

                const OdGePoint2d ptNext = vertices[(i + 1)%n];
                if (pt.isEqualTo(ptNext))
                {
                  continue;
                }
    
                OdGeCircArc2d arc(pt, ptNext, bulge, false); 
                OdGeNurbCurve3d* nurb3d = ellipse2dtoNurbs3d(arc);
                nurbs(*nurb3d);
                delete nurb3d;
                bEmptyContour = false;
              }
              if(!pnts3d.isEmpty())
              {
                pnts3d.push_back(bClosed ? OdGePoint3d(vertices[0].x, vertices[0].y, 0.) : OdGePoint3d(vertices.last().x, vertices.last().y, 0.));
                polyline(pnts3d.logicalLength(), pnts3d.asArrayPtr());
                bEmptyContour = false;
              }
            }
            else
            {
              OdArray<OdGeCurve2d*> edgePtrs;
              pH->getLoopAt(m_pSolidHatch, l, edgePtrs);

              OdGePoint2d pLast;
              for(f=0; f<edgePtrs.size(); ++f)
              {
                OdGeCurve2d* cv = edgePtrs.getAt(f);

                // try to get nurbs
                OdGeNurbCurve3d* nurb3d = NULL;
                if(cv->type() == OdGe::kNurbCurve2d)
                {
                  nurb3d = convert2dto3d((OdGeNurbCurve2d*)cv);
                }
                else if(cv->type() == OdGe::kEllipArc2d)
                {
                  nurb3d = ellipse2dtoNurbs3d(*((OdGeEllipArc2d*)cv));
                }
                else if(cv->type() == OdGe::kCircArc2d)
                {
                  nurb3d = ellipse2dtoNurbs3d(*((OdGeCircArc2d*)cv));
                }

                if(nurb3d != NULL)
                {
                  nurbs(*nurb3d);
                  //CORE-11119 PdfExport: Export solid hatch regression - hatch w/o one edge
                  OdGeInterval intvl;
                  nurb3d->getInterval(intvl);
                  double param1 = intvl.lowerBound();
                  double param2 = intvl.upperBound();
                  OdGePoint3dArray pnts;
                  nurb3d->appendSamplePoints(param1, param2, dev, pnts);
                  //OdGePoint2d p1 = OdGePoint2d(pnts[0].x, pnts[0].y);
                  pLast = OdGePoint2d(pnts.last().x, pnts.last().y);
                  //////////////////////////////////////////////////////////////////////////
                  delete nurb3d;
                  bEmptyContour = false;
                }
                else
                {
                  OdGeInterval intvl;
                  cv->getInterval(intvl);
                  double param1 = intvl.lowerBound();
                  double param2 = intvl.upperBound();
                  OdGePoint2dArray pnts;
                  cv->appendSamplePoints(param1, param2, dev, pnts);

                  /*const*/ unsigned int nPt = pnts.logicalLength();

                  if(nPt > 0)
                  {
                    OdGePoint3dArray pnts3d;
                    //CORE-11119 PdfExport: Export solid hatch regression - hatch w/o one edge
                    OdUInt32 k = 0, kk = 0;
                    if (!pLast.isEqualTo(pnts[0], 1.e-6) && f)
                    {
                      pnts3d.setLogicalLength(++nPt);
                      pnts3d[k].set(pLast.x, pLast.y, 0.);
                      k++;
                    } 
                    else
                    {
                      pnts3d.setLogicalLength(nPt);
                    }
                    //////////////////////////////////////////////////////////////////////////
                    for(k; k < nPt; ++k)
                    {
                      pLast = pnts[kk];
                      pnts3d[k].set(pnts[kk].x, pnts[kk].y, 0.);
                      ++kk;
                    }
                    polyline(nPt, pnts3d.asArrayPtr());
                    bEmptyContour = false;
                  }
                }
              }
            }

nextloop:
            if (l == pH->numLoops(m_pSolidHatch) - 1)
              device()->setFillRule(PDF2dExportDevice::kFillStrokeCloseRule); //Bug 22740. It was PDF2dExportDevice::kFillEvenOddRule before

            // last path will be closed by fill command
            if (l == pH->numLoops(m_pSolidHatch) - 1 && bEmptyContour)
              device()->dc_fill();
            
            device()->endDrawingContour();
          } // loops                
        }
        device()->setFillRule(prevRule);
        return;
      }
      catch (...)
      {
        device()->endDrawingContour();
        device()->setFillRule(prevRule);
        Od2dExportView::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
      }
    }
    
    try
    {
      // use OdWinBitmapModuleName module as rendering device
      OdGsDevicePtr pDevice = getRenderingDevice();
      if (pDevice.isNull())
      {
        Od2dExportView::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);    
        return;
      }
     
      onTraitsModified();
      OdSmartPtr<myDrawable> myTmpDrawable = OdRxObjectImpl<myDrawable>::createObject();
      OdGiSubEntityTraitsData et = effectiveTraits();
      if (userGiContext()->isPlotGeneration() && userGiContext()->plotStyleType() != OdGiContext::kPsNone)
      {
        const OdPsPlotStyleData& ps = effectivePlotStyle();
        if (ps.color().isByColor())
          et.setTrueColor(ps.color());
        else
          et.setColor(ps.color().colorIndex());
      }
      myTmpDrawable->set(const_cast<OdGiSubEntityTraitsData*>(&et), this, numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);

      OdGeExtents3d extents;
      for(OdInt32  f=0; f<numVertices; ++f)
      {
        extents.addPoint(vertexList[f]);
      }

      OdGsViewPtr pNewView = pDevice->createView();
      pNewView->add(myTmpDrawable, 0);
      pDevice->addView(pNewView);

      OdGePoint3d target = (extents.minPoint() + extents.maxPoint().asVector())/2.;
      OdGeVector3d direction = OdGeVector3d::kZAxis;
      OdGeVector3d upVector = OdGeVector3d::kYAxis;
      double fieldWidth = extents.maxPoint().x - extents.minPoint().x;
      double fieldHeight = extents.maxPoint().y - extents.minPoint().y;

      OdAbstractViewPEPtr(pNewView)->setView(pNewView, target, direction, upVector, fieldWidth, fieldHeight, false);

      pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(24)));

      ODCOLORREF bg = ODRGB(255,255,255); // palette background
      const ODCOLORREF* palette = odcmAcadPalette(bg);

      ODGSPALETTE pPalCpy;
      pPalCpy.insert(pPalCpy.begin(), palette, palette+256);

      pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);

      OdGeVector3d v(1,1,0);
      v.transformBy( objectToDeviceMatrix().invert() );
      //////////////////////////////////////////////////////////////////////////
      OdGeMatrix3d m;
      m.setToScaling((double)device()->exportParams().getGeomDPI() / 72.);
      v.transformBy(m);
      //////////////////////////////////////////////////////////////////////////
      double dCoef = fabs(v.x);

      double dDPIScale = static_cast<PDF2dExportDevice*>(m_pDevice)->getHatchDPI();
      dDPIScale /= 72.;

      long sizeW, sizeH;
      if (fieldHeight < fieldWidth)
      {
        sizeH = long(odmax((fieldHeight/dCoef+0.5)*dDPIScale, 1));
        sizeW = long(odmax(double(sizeH)/fieldHeight*fieldWidth+0.5, 1));
      }
      else
      {
        sizeW = long(odmax((fieldWidth/dCoef+0.5)*dDPIScale, 1));
        sizeH = long(odmax(double(sizeW)/fieldWidth*fieldHeight+0.5, 1));
      }

      if (OdUInt64((OdUInt64)sizeW*(OdUInt64)sizeH) > 8000 * 8000) // image is too big to be created
      {
        return Od2dExportView::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
      }

      OdGsDCRect UpdatedRect(0, sizeW, 0, sizeH);
      pDevice->onSize(UpdatedRect);

      if (device()->getPalette())
      {
        pDevice->setLogicalPalette(device()->getPalette(), 256);
      }

      pDevice->setUserGiContext( userGiContext() );

      // create mask 1 with white background
      pDevice->setBackgroundColor(ODRGB(255,255,255));
      pDevice->update();
      OdGiRasterImagePtr pRas1 = OdGiRasterImagePtr(pDevice->properties()->getAt(OD_T("RasterImage")));

      // convert from RGB to RGBA
      OdGiRasterImagePtr pCopyImg = convertToRGBA(pRas1);

      // create mask 2 with black background
      pDevice->setBackgroundColor(ODRGB(0,0,0));
      pDevice->update();
      pRas1 = OdGiRasterImagePtr(pDevice->properties()->getAt(OD_T("RasterImage")));

      // Merge both images to create Bitmap with transparency
      createTransparentImage(pCopyImg, pRas1);

      device()->setHatchDrawingMode(true);
      rasterImageDc(extents.minPoint()+OdGeVector3d::kYAxis*fieldHeight, OdGeVector3d::kXAxis*fieldWidth/sizeW, -OdGeVector3d::kYAxis*fieldHeight/sizeH, pCopyImg, 0, 0);
      device()->setHatchDrawingMode(false);
    }
    catch (...)
    {
      Od2dExportView::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
    }
  }
  else
  {
    Od2dExportView::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
  }
}

#else

void PDF2dExportView::shell(OdInt32 numVertices,
                            const OdGePoint3d* vertexList,
                            OdInt32 faceListSize,
                            const OdInt32* faceList,
                            const OdGiEdgeData* pEdgeData,
                            const OdGiFaceData* pFaceData,
                            const OdGiVertexData* pVertexData)
{
  Od2dExportView::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
}

#endif

void PDF2dExportView::processHyperlinks(const OdGiDrawable* pDrawable, const OdString &layerName)
{
  if (pDrawable && !OdGsDbRootLinkage::isLayoutDrawable(pDrawable))
  {
    OdDbHyperlinkCollectionPtr collection;
    try
    {
      collection = OdDbEntityHyperlinkPEPtr(pDrawable)->getHyperlinkCollection(pDrawable, true);
    }
    catch (const OdError&)
    {
      return;
    }
    if (!collection.isNull() && collection->count() > 0)
    {
      OdString hLink = collection->item(0)->name();
      OdGeExtents3d extents;

      if (!hLink.isEmpty() && ::eOk == pDrawable->getGeomExtents(extents))
      {
        OdGeMatrix3d mx = objectToDeviceMatrix();
        extents.transformBy(mx);
        //////////////////////////////////////////////////////////////////////////
        OdGeMatrix3d m;
        //unscale extents because of annotation coordinates are not in the content stream
        m.setToScaling(72. / (double)device()->exportParams().getGeomDPI());
        extents.transformBy(m);
        //////////////////////////////////////////////////////////////////////////
        OdGeExtents2d rect;
        rect.set((const OdGePoint2d&)(extents.minPoint()), (const OdGePoint2d&)(extents.maxPoint()));

        OdString layer;
        if (device()->isLayersSupported())
          layer = layerName.isEmpty() ? device()->layerName(subEntityTraits().layer()) : layerName;

        device()->dc_hyperlink(rect, hLink, layer);
      }
    }
  }
}

void PDF2dExportView::text(const OdGePoint3d& position,
  const OdGeVector3d& normal,
  const OdGeVector3d& direction,
  const OdChar* msg,
  OdInt32 length,
  bool raw,
  const OdGiTextStyle* pStyle)
{
  bool bTextAsGeom = false;
  if (pStyle->isTtfFont() && GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kTTFTextAsGeometry) ||
    pStyle->isShxFont() && GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kSHXTextAsGeometry))
    bTextAsGeom = true;

  if (effectivelyVisible() && !regenAbort())
  {
    onTraitsModified();
    if (length > 0) //lenght can be < 0 for missing xrefs
    {
      OdGeVector3d u, v;
      ::odgiCalculateTextBasis(u, v, normal, direction,
        pStyle->textSize(), pStyle->xScale(),
        pStyle->obliquingAngle(),
        pStyle->isBackward(),
        pStyle->isUpsideDown()
        );
      onTextProcessing(position, u, v);
      OdGePoint3d pMin, pMax, endPos;
      device()->userGiContext()->textExtentsBox(*pStyle, msg, length, 0, pMin, pMax, &endPos);

      OdGeVector3d uNorm(u.normal()), vNorm(v.normal());
      OdGeBoundBlock3d bbChk(position + uNorm * pMin.x + vNorm * pMin.y, uNorm * (endPos.x - pMin.x), vNorm * (pMax.y - pMin.y), OdGePoint3d::kOrigin.asVector());
      OdUInt32 bbStatus = 0;

      m_modelEntryPoint.geometry().conveyorBoundaryInfoProc(bbChk, bbStatus);

      if (GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClipFull)) // Text is fully clipped, skip vectorization
        return;
      else
      {
        if (bTextAsGeom && pStyle->isShxFont() && GETBIT(device()->exportParams().searchableTextType(), PDFExportParams::kSHX) ||
          bTextAsGeom && pStyle->isTtfFont() && GETBIT(device()->exportParams().searchableTextType(), PDFExportParams::kTTF))
        {
          m_SearchableText += OD_T(" ") + OdString(msg, length);
        }
        if (GETBIT(bbStatus, OdGiConveyorGeometry::kBoundaryProcClip))
        { // Text is partially clipped, so we do not clip it at all
          bool clipperNeedsRestart = false;
          if (!m_pViewportClip.isNull() && m_pViewportClip->enabled() && !bTextAsGeom)
          {
            m_pViewportClip->disable();
            clipperNeedsRestart = true;
          }
          OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pStyle);
          if (clipperNeedsRestart)
            m_pViewportClip->enable();
        }
        else
          return OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pStyle);
      }
    } 
    else
    {
      return OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pStyle);
    }
  }
}

void PDF2dExportView::processPRC(const OdGiDrawable* pDrawable)
{
  if ((device()->exportParams().getPRCMode() != PDFExportParams::kDisabled))
  {
    m_bPRCMode = false;

    // apply filter to find you whether entity should be exported or not
    bool canExportToPRC = false;
    OdUInt32 viewportidx = -1;
    const OdGiPathNode* pGiPath = currentGiPath();

    if (!m_bExtentsMode && device()->exportParams().getPRCContext())
    {
      canExportToPRC = device()->exportParams().getPRCContext()->shouldExportAsPRC(pDrawable, pGiPath, &m_viewInfo, viewportidx);
    }
    
    if (canExportToPRC)
    {
      // setup
      PDF2PRCExportParams prcParams;

      // brep or mesh mode
      prcParams.m_exportAsBrep = device()->exportParams().getPRCMode() == PDFExportParams::kAsBrep;
      
      //get traits data
      prcParams.m_pTraitsData = &effectiveTraits();
      prcParams.m_pByBlockTraitsData = byBlockTraits();
      
      // compression
      PDF3D_ENUMS::PRCCompressionLevel compressLev;
      bool hasBrepCompression = device()->exportParams().hasPrcBrepCompression(compressLev);
      prcParams.setPRCCompression(compressLev, hasBrepCompression, device()->exportParams().hasPrcTessellationCompression());

      prcParams.m_bodyTransformationMatr = modelToWorldTransform();
      
      OdGeExtents3d extents;
      OdGeVector3d cameraVector = viewDir();
      OdGeVector3d normalToCamera = upVector();
      OdGeVector3d third = normalToCamera.crossProduct(cameraVector);
      OdGeMatrix3d cameraView;
      // using OdGePoint3d::kOrigin instead of position() is correct
      cameraView.setCoordSystem(OdGePoint3d::kOrigin, third, normalToCamera, cameraVector);

      device()->exportParams().getPRCContext()->getExtents(pDrawable, m_viewInfo.viewportObjectId, &(drawContext())->giContext(), cameraView, modelToWorldTransform(), extents);
      if (!extents.isValidExtents())
      {
        m_bPRCMode = false;
        return;
      }
      m_bPRCMode = true;

      // user context
      prcParams.m_extraOptions->putAt("user data", device()->exportParams().getPRCContext()->userData());

      // export drawable to prc file and/or stream
      int viewIdx = 0;
      for (int i = 0; i < device()->numViews(); ++i)
      {
        if (device()->viewAt(i) == this)
        {
          viewIdx = i;
          break;
        }
      }

      std::map< OdUInt32, std::pair<OdRxObjectPtr, ViewExtents> > ::iterator it = device()->m_prc_context.find(viewportidx);
      if (it == device()->m_prc_context.end())
      {
        // no extents for this viewport yet => create new ones
        it = device()->m_prc_context.insert(
          std::pair<OdUInt32, std::pair<OdRxObjectPtr, ViewExtents> > (viewportidx, std::pair<OdRxObjectPtr, ViewExtents> ((OdRxObject *)NULL, ViewExtents()))
        ).first;

        //is this code correct? (m_viewIdx sets one time per every viewport)
        it->second.second.m_viewIdx = viewIdx;
      }
      it->second.second.m_extents.addExt(extents);
      prcParams.m_doWriteToStream = false;
      prcParams.m_viewportIdx = viewportidx;
      prcParams.m_viewIdx = viewIdx;
      savePalleteFromDeviceToParams(prcParams, *device());
      OdStreamBufPtr pStream = OdMemoryStream::createNew();
      OdResult result = device()->dc_prc(pStream, pDrawable, prcParams);
      ODA_ASSERT(result == ::eOk);
    }
  }
}

bool PDF2dExportView::doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable)
{
  if (m_bExtentsMode)
  {
    return Od2dExportView::doDraw(drawableFlags, pDrawable);
  }

  // simulation of frozen BlockRefs
  bool isOffLayersEnabled = device()->isLayersSupported() && device()->isOffLayersEnabled();

  if (isOffLayersEnabled && device()->frozenLayerSimulationEnabled())
  {
    // start rendering of BlockReference
    if (OdGsDbRootLinkage::isBlockRefDrawable(pDrawable))
    {
      OdString name = device()->layerName(subEntityTraits().layer());
      bool isFrozen = device()->isFrozenLayer(name);
      if (isFrozen)
      {
        name += PDF_FROZEN_LAYER;
        device()->open_Frozen_Layer(name);
      }

      if (GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kExportHyperlinks))
        processHyperlinks(pDrawable, name);
      m_SearchableText.empty();
      bool bRet = Od2dExportView::doDraw(drawableFlags, pDrawable);
      if (!m_SearchableText.isEmpty())
        processSearchableText(pDrawable);

      if (isFrozen)
      {
        device()->close_Frozen_Layer();
      }
      return bRet;
    }
  }

  if (GETBIT(device()->exportParams().exportFlags(), PDFExportParams::kExportHyperlinks))//check if there is a hyperlink
    processHyperlinks(pDrawable);

  OdDbBaseHatchPE* pH = 0;
  if (OdGsDbRootLinkage::isHatchDrawable(pDrawable))
  {
    pH = OdGsDbRootLinkage::getDbBaseHatchPE(pDrawable);
  }

  PDFExportParams::PRCSupport pramsMode = device()->exportParams().getPRCMode();
  bool canExportHatchToPrc = (device()->exportParams().getPRCMode() != PDFExportParams::kDisabled) && pH;

  if (!pH || m_bExtentsMode || canExportHatchToPrc) //(pH.isNull() || m_bExtentsMode /*|| !pH->isGradient()*/)
  {
    bool canProcessPRC = true;
    if (m_drawableFilterFunction && (device()->exportParams().getPRCMode() != PDFExportParams::kDisabled))
    {
      const OdUInt32 filterFlags = giContext().drawableFilterFunction(m_drawableFilterFunction, pDrawable, drawableFlags);
      if (GETBIT(filterFlags, OdGiContext::kSkipVectorization))
      {
        canProcessPRC = false;
      }
    }
    if (canProcessPRC)
    {
      processPRC(pDrawable);
    }

    if (m_bPRCMode == false)
    {
      m_SearchableText.empty();
      bool res = Od2dExportView::doDraw(drawableFlags, pDrawable);
      if (!m_SearchableText.isEmpty())
        processSearchableText(pDrawable);
      return res;
    }
    return true;
  }

  // export non solid hatch using bitmap||vectorizer
  m_bHatchToBitmapMode = device()->exportParams().otherHatchesExportType() == PDFExportParams::kBitmap;//!(GETBIT(device()->exportParams().hatchesExportType(), 4));

  bool clipperNeedsRestart = false;
  if (pH->isSolidFill(pDrawable) && !pH->isGradient(pDrawable))
  {
    m_bHatchToBitmapMode = true;
    // export solid hatch as pdf paths
    if (device()->exportParams().solidHatchesExportType() == PDFExportParams::kPdfPaths)
    {
      m_pSolidHatch = pDrawable;
      OdGeExtents3d extents;
      if (::eOk == pDrawable->getGeomExtents(extents))
      {
        OdGeMatrix3d mx = objectToDeviceMatrix();
        extents.transformBy(mx);
        if (m_pViewportClip->checkExtentsVisibility(extents) == OdGiOrthoClipperEx::kStatusClipped)
        {
          m_pViewportClip->disable();
          clipperNeedsRestart = true;
        }
      }
    }
    // or using vectorizer
    else if (device()->exportParams().solidHatchesExportType() == PDFExportParams::kDrawing)
      m_bHatchToBitmapMode = false;
    // else as a bitmap (as a default)    
  }
  else if (pH->isSolidFill(pDrawable) && pH->isGradient(pDrawable))
    m_bHatchToBitmapMode = device()->exportParams().gradientHatchesExportType() == PDFExportParams::kBitmap ? true : false;

  bool b = Od2dExportView::doDraw(drawableFlags, pDrawable);
  if (clipperNeedsRestart)
    m_pViewportClip->enable();

  m_bHatchToBitmapMode = false;
  m_pSolidHatch = 0;
  return b;
}

void PDF2dExportView::processSearchableText(const OdGiDrawable* pDrawable, const OdString &layerName)
{
  if (pDrawable && !m_SearchableText.isEmpty())
  {
    OdGeExtents3d extents;
    if (::eOk == pDrawable->getGeomExtents(extents))
    {
      OdGeMatrix3d mx = objectToDeviceMatrix();
      extents.transformBy(mx);
      //////////////////////////////////////////////////////////////////////////
      OdGeMatrix3d m;
      //unscale extents because of annotation coordinates are not in the content stream
      m.setToScaling(72. / (double)device()->exportParams().getGeomDPI());
      extents.transformBy(m);
      //////////////////////////////////////////////////////////////////////////
      OdGeExtents2d rect;
      rect.set((const OdGePoint2d&)(extents.minPoint()), (const OdGePoint2d&)(extents.maxPoint()));

      OdString layer;
      if (device()->isLayersSupported())
        layer = layerName.isEmpty() ? device()->layerName(subEntityTraits().layer()) : layerName;

      OdString sTitle = OD_T("Text");
      device()->dc_textAnnot(rect, sTitle, m_SearchableText, layer);
    }
    m_SearchableText.empty();
  }
}

void PDF2dExportView::update()
{
  Od2dExportView::update();
}

//====================================================================
//

void PDF2dExportView::setText(const OdGePoint3d& position,
                              const OdGeVector3d& u,      // normal
                              const OdGeVector3d& v,      // direction
                              const OdChar* /*msg*/, OdInt32 /*length*/, bool raw,
                              const OdGiTextStyle* pTextStyle,
                              bool  /*bBox*/,
                              const Od2dFont* /*pFont*/,
                              const OdArray<OdUInt16> *pUnicode)
{
  bool isTtfFont = pTextStyle->isTtfFont();
  ODA_ASSERT(isTtfFont);
  if (!isTtfFont)
    return;

  bool bASCII = false/*true*/;
  bool bSkip(false);

  PDFFontPtr pNewFont;
  OdAnsiString fontName;
    
  fontName = device()->dc_truetype_font(*pTextStyle, &pNewFont);
  if (fontName.isEmpty())
  {
    bSkip = true; ODA_ASSERT(0); // Should Pdf support all TTF ??
  }
  else
  {
    ODA_ASSERT(!pNewFont.isNull());
    bool bConvertText2Unicode = pNewFont->getSubtype()->str() == ("Type0");
    bASCII = !bConvertText2Unicode;
  }

  if (!bSkip)
  {
/*
    if (bASCII)
    {
      dc_drawtext(fontName, position.convert2d(), u.convert2d(), v.convert2d(), *msg, length, *pTextStyle);
      getFontOptimizer().addText(pNewFont, msg);
    }
    else
*/
    {
      // Note: pUnicode canbe modified inside dc_drawunicodetext (for Arabic fonts).
      OdArray<OdUInt16> pUnicodeTmp = *pUnicode;
      device()->dc_drawunicodetext(fontName, position.convert2d(), u.convert2d(), v.convert2d(), &pUnicodeTmp, *pTextStyle, raw);
      device()->getFontOptimizer().addUnicodeText(pNewFont, pUnicodeTmp);
    }
  }
  else
  {
    bSkip = true;
  }
}

//////////////////////////////////////////////////////////////////
double PDF2dExportView::calcScaledDeviation(const OdGiDeviationType type, const OdGePoint3d& pt) const
{
  if (!OdEqual(dcScreenMax().x, dcScreenMin().x) &&
    !OdEqual(dcScreenMax().y, dcScreenMin().y))
  {
    OdGePoint2d pixelsPerUnit;
    OdGsBaseVectorizer::getNumPixelsInUnitSquare(pt, pixelsPerUnit, false);
    pixelsPerUnit.x = fabs(pixelsPerUnit.x);
    pixelsPerUnit.y = fabs(pixelsPerUnit.y);
    if (type == kOdGiMaxDevForCircle)
    {
      unsigned int viewres = userGiContext()->circleZoomPercent(m_viewInfo.viewportObjectId);
      if (viewres < 1 || viewres > 20000) viewres = 100;
      double cft = double(viewres) / 100;
      pixelsPerUnit.x *= cft;
      pixelsPerUnit.y *= cft;
    }
    double deviation = 0.5 / pow(odmax(pixelsPerUnit.x, pixelsPerUnit.y), 1./64.);
    //calculate deviation this way gives us the same result as Autocad export
    //because the increased deviation will be compensated with increased radius of arcs
    if (OdGsBaseVectorizer::isPerspective())
    {
      OdGePoint3d eyePt(pt);
      eyePt.transformBy(viewingMatrix());
      deviation *= 1.0 - eyePt.z / focalLength();
      if (deviation < 0.0)
        deviation = -deviation;
    }
    return deviation;
  }
  return 1e100;
}

//double PDF2dExportView::deviation(const OdGiDeviationType dt, const OdGePoint3d &p) const
//{
//  // Deviation related bugs - should be tested, if deviation was modified.
//  // 3838
//  // 4063 - gradient hatch
//  // 4360
//  // 6175 - hatch size
//  // 13956 - hatch again
//
//  // Members reported that for the huge drawings, if you made zoomIn in .pdf. The small part will look
//  // bad (circles to lines). You can add / 100. here to reduce tessellation deviation for full drawing.
//
//  double baseDeviation = OdGsBaseVectorizer::deviation(dt, p);
//  // now apply geomDPI, baseDeviation is for 72 DPI
//  OdUInt16 geomDPI = ((PDF2dExportView*)this)->device()->exportParams().getGeomDPI();
//  double scale = geomDPI / 72.;
//  baseDeviation /= scale;
//  return baseDeviation;
//}

void PDF2dExportView::ownerDrawDc(const OdGePoint3d& _origin,
                                  const OdGeVector3d& _u,
                                  const OdGeVector3d& _v,
                                  const OdGiSelfGdiDrawable* pDrawable,
                                  bool bDcAligned,
                                  bool bAllowClipping)
{
  if (mode() != OdGsView::k2DOptimized)
    return;

  OdOleItemHandlerPtr pOLE = OdOleItemHandler::cast(pDrawable);
  if (pOLE.isNull()) {
    Od2dExportView::ownerDrawDc(_origin, _u, _v, pDrawable, bDcAligned, bAllowClipping);
    return;
  }

  OdOleItemHandler::PlotQuality plotQuality = pOLE->outputQuality();
  bool bMonochrome = false;
  double iScale = 1.;
  if (plotQuality == OdOleItemHandler::kMonochrome) {
    iScale = 72. / (double)device()->exportParams().bwImagesDPI();
    bMonochrome = true;
  }
  else
    iScale = 72. / (double)device()->exportParams().colorImagesDPI();

  OdGeMatrix3d x = getModelToEyeTransform();

  OdGePoint3d origin(x * _origin);
  OdGeVector3d u(x * _u), v(x * _v);

  OdGeExtents3d ext(origin, origin);
  ext.addPoint(origin + u);
  ext.addPoint(origin + v);
  ext.addPoint(origin + u + v);

  OdGePoint3d imgMin = ext.minPoint();
  OdGePoint3d imgMax = ext.maxPoint();

  OdGePoint3d origin2;
  OdGeVector3d u2;
  OdGeVector3d v2;
  origin2 = imgMin;
  v2 = u2 = imgMax - imgMin;
  u2.z = u2.y = 0.;
  v2.z = v2.x = 0.;

  x = eyeToOutputTransform();
  origin2.transformBy(x);
  u2.transformBy(x);
  v2.transformBy(x);

  OdGeVector3d u2Tmp = (u2 / ((double)device()->exportParams().getGeomDPI() / 72.)) / iScale; //downscale vectors because od geomDPI (see SUP-716)
  OdGeVector3d v2Tmp = (v2 / ((double)device()->exportParams().getGeomDPI() / 72.)) / iScale;
  //u2 /= iScale;
  //v2 /= iScale;

  OdUInt64 rasterWidth, rasterHeight;
  try
  {
    rasterWidth = OdRoundToLong(fabs(u2Tmp.x));
    rasterHeight = OdRoundToLong(fabs(v2Tmp.y));
    if (rasterWidth * rasterHeight > 48 * 1024 * 1024) // CreateDibBitmap limitation (huge rasters are scaled anyway)
    {
      double scale = sqrt((((double)(rasterWidth) / 1024.) * ((double)(rasterHeight) / 1024.)) / 48.);//that is equal to (rasterWidth * rasterHeight) / (48 * 1024 * 1024);
      if (OdLessOrEqual(scale, 1.))
        scale = 1.1;
      while (rasterWidth * rasterHeight > 48 * 1024 * 1024)
      {
        rasterWidth  = OdRoundToLong((double)rasterWidth  / scale);
        rasterHeight = OdRoundToLong((double)rasterHeight / scale);
      }
    }
  }
  catch (const OdError&)
  {
    // #10225 : Can't render if arithmetic overflow thrown
    return;
  }
  //u2 *= iScale;
  //v2 *= iScale;

  OdGsDCRect rect; // RECT 
  rect.m_min.x = 0; // left
  rect.m_min.y = rasterHeight; // top
  rect.m_max.x = rasterWidth; // right
  rect.m_max.y = 0; // bottom 

  OdSmartPtr<OdGiSelfGiDrawablePE> pOdGiSelfGiDrawablePE = OdGiSelfGiDrawablePE::cast(pDrawable);
  if (pOdGiSelfGiDrawablePE.get() && pOdGiSelfGiDrawablePE->draw(const_cast<OdGiSelfGdiDrawable*>(pDrawable), &output().destGeometry(), origin2, u2, v2, rect, true))
    return;
  drawWithGdiBmp(pDrawable, this, origin2, u2, v2, rasterWidth, rasterHeight, rect, bMonochrome);
}

}
