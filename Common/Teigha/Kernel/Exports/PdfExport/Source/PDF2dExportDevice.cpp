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


#include "PdfExportCommon.h"

#include "2dExportDevice.h"
#include "2dSupport.h"

#include "DbBaseDatabase.h"

#include "Pdf2dExportDevice.h"
#include "Pdf2dExportView.h"
#include "PdfAux.h"

#include "PdfFontMatrix.h"
#include "PdfPageNodeDictionary.h"

#include "OdCharMapper.h"

#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeEllipArc3d.h"

#include "RxRasterServices.h"
#include "MemoryStream.h"
#include "Gi/GiRasterWrappers.h"

#include "DynamicLinker.h"

//////////////////////////////////////////////////////////////////////////////////
// PDF2dExportDevice
//////////////////////////////////////////////////////////////////////////////////
namespace TD_PDF_2D_EXPORT {

  PDF2dExportDevice::PDF2dExportDevice()
    : Od2dExportDevice(DeviceType(kPDFDevice | kSupport2dCircle | kSupport2dEllipse))
    , m_bGraphStateFixed(false)
    , m_bOnePointPoly(false)
    , m_num(0)
    , m_LayerOpen(false)
    , m_lineWeight(-HUGE_VAL)
    , m_CapStyle(TD_PDF::kLineCapNotSet)
    , m_JoinStyle(TD_PDF::kLineJoinNotSet)
    , m_hatchDPI(72)
    , m_bwBitmapDPI(400)
    , m_colorBitmapDPI(400)
    , m_fillRule(kFillStrokeCloseRule) //Bug 22740. It was PDF2dExportDevice::kFillEvenOddRule before
    , m_stateDrawingContour(0)
    , m_bHatchDrawingMode(false)
    , m_currentTransparencyState("")
    , m_bTransparencyGroup255(false)
    , m_bDwgPaletteNeeded(false)
    , m_bRasterClipping(true)
    , m_prc_context()
    , m_paperBox(OdGsDCRect::Null)
    , m_LwToDcScale(0)
    , m_pParamsHolder(NULL)
{
  m_LastPoint.x = m_LastPoint.y = -HUGE_VAL;

  m_curRGB = 0xFFFFFFFF;
  m_curColorIndex = 0;
  m_bRGBActive = true; // don't change it to false

  // Recording is disabled by default. Usually zoomExtents process is before main update();
  // So you should enable Recording before main update().
  enableRecording(false);
}

PDFDummyContentStream PDF2dExportDevice::m_DummyCCStream = PDFDummyContentStream();

void PDF2dExportDevice::enableRecording(bool bEnable /* = true */, bool bZ2E /* = true */)
{
  m_bRecordingEnabled = bEnable;
  if (bEnable)
  {
    m_OutputCCStream = m_pParamsHolder->ContentCommands();
    m_currentPdfPage = m_pParamsHolder->CurrentPage();
  }
  else
  {
    m_OutputCCStream = &m_DummyCCStream;
    if (bZ2E)
      m_currentPdfPage = 0;
  }
}

bool PDF2dExportDevice::recordingEnabled()
{
  return m_bRecordingEnabled;
}

void PDF2dExportDevice::setPaperBox(const OdGsDCRect& box)
{
  m_paperBox = box;
}

void savePalleteFromDeviceToParams(PDF2PRCExportParams &params, const PDF2dExportDevice &device);

struct PDF2PRCViewportParams
{
  OdGeVector3d x, y, z;
  double distanceToTarget;
  double scale;
  OdGeExtents2d rect;
  OdGeMatrix3d mxWTE;
  OdGeVector3d cameraVector;

  PDF2PRCViewportParams(const OdGeExtents3d &extentsAll,
    const OdGeMatrix3d &objectToDeviceMatr,
    const OdGeMatrix3d &worldToEyeTransformMatr,
    const OdGeMatrix3d &worldToCameraMatr,
    const OdGeVector3d &viewDir,
    OdUInt16 geomDPI
    )

    : x(1, 0, 0)
    , y(0, 1, 0)
    , z(0, 0, -1)
    , distanceToTarget(extentsAll.maxPoint().distanceTo(extentsAll.minPoint()))
  {
    // camera vector (position)
    cameraVector = viewDir * distanceToTarget;

    // scale
    OdGePoint2d pnt1(extentsAll.minPoint().x, extentsAll.minPoint().y);
    OdGePoint2d pnt2(extentsAll.maxPoint().x, extentsAll.maxPoint().y);
    OdGePoint2d pnt3(extentsAll.maxPoint().x, extentsAll.minPoint().y);
    scale = odmax(pnt1.distanceTo(pnt3), pnt2.distanceTo(pnt3));
    scale = 1. / scale;

    // rect
    OdGePoint3d minPoint = extentsAll.minPoint();
    minPoint.transformBy(worldToCameraMatr);
    minPoint.transformBy(objectToDeviceMatr);
    minPoint.scaleBy(/*0.12*/ 72. / geomDPI);
    OdGePoint3d maxPoint = extentsAll.maxPoint();
    maxPoint.transformBy(worldToCameraMatr);
    maxPoint.transformBy(objectToDeviceMatr);
    maxPoint.scaleBy(/*0.12*/ 72. / geomDPI);
    rect.set((const OdGePoint2d&)minPoint, (const OdGePoint2d&)maxPoint);

    // Camera 2 World matrix
    mxWTE = worldToEyeTransformMatr; // World to Eye matrix
    mxWTE.invert(); // pdf file requires Camera 2 World matrix!
    mxWTE.setTranslation(OdGeVector3d(0, 0, 0)); // camera translation (camera target) by default is is the target of the acad camera. clear it.
    OdGeVector3d extentsCenter = extentsAll.center().asVector();
    extentsCenter.transformBy(worldToCameraMatr);
    mxWTE.setTranslation(extentsCenter);         // set camera translation to our desired target (that is the center of the extents, independent of acad camera)
  }
};

void PDF2dExportDevice::finishPRCVectorization()
{
  for (std::map< OdUInt32, std::pair<OdRxObjectPtr, ViewExtents> > ::iterator it=m_prc_context.begin();
    it!=m_prc_context.end(); 
    ++it
    )
  {
    OdGeExtents3d &extentsAll = it->second.second.m_extents;

    bool hasBodies = extentsAll.isValidExtents();
    if (hasBodies)
    {
      // set export params
      PDF2PRCExportParams prcParams;
      prcParams.m_viewportIdx = it->first;
      // - compression
      PDF3D_ENUMS::PRCCompressionLevel compressLev;
      bool hasBrepCompression = exportParams().hasPrcBrepCompression(compressLev);
      prcParams.setPRCCompression(compressLev, hasBrepCompression, exportParams().hasPrcTessellationCompression());

      // export prc file from context to stream
      savePalleteFromDeviceToParams(prcParams, *this);
      OdStreamBufPtr pStream = OdMemoryStream::createNew();
      OdResult result = dc_prc(pStream, NULL, prcParams);
      if (result != ::eOk)
      {
        throw OdError(result); // should not fail at this point; if it does - something bad has happened
      }

      // write stream into pdf file
      if (pStream->length() == 0)
      {
        return;
      }
      OdPrcContextForPdfExport *pPrcContext = exportParams().getPRCContext();
      if (pPrcContext && pPrcContext->isKindOf(OdPrcContextForPdfExportWrapper::desc()))
      {
        ((OdPrcContextForPdfExportWrapper *) pPrcContext)->getOutputPRC().insert(std::pair<OdUInt32, OdStreamBufPtr>(it->first, pStream));
      }
      OdGsView *pView = viewAt(it->second.second.m_viewIdx);
      if (pView)
      {
        OdGeVector3d cameraVector = pView->position() - pView->target();
        cameraVector.normalize();
        OdGeVector3d normalToCamera = pView->upVector();
        OdGeVector3d third = normalToCamera.crossProduct(cameraVector);
        OdGeMatrix3d cameraView;
        cameraView.setCoordSystem(OdGePoint3d(0.0f ,0.0f ,0.0f), third, normalToCamera, cameraVector);
        PDF2PRCViewportParams viewportParams(extentsAll, pView->objectToDeviceMatrix(), pView->viewingMatrix(), cameraView, cameraVector, exportParams().getGeomDPI());
        result = dc_prc(pStream, 
          viewportParams.rect, 
          viewportParams.cameraVector, 
          viewportParams.x, 
          viewportParams.y, 
          viewportParams.z, 
          viewportParams.distanceToTarget,
          viewportParams.scale,
          &viewportParams.mxWTE);
      }
      else
      {
        result = ::eNullObjectPointer;
      }
      ODA_ASSERT(result == ::eOk);
    }
  }
}

TD_PDF::PDFResourceDictionaryPtr PDF2dExportDevice::getResourceDictionary()
{
  PDFResourceDictionaryPtr pResDict;
  if (m_currentPdfPage)
  {
    if (m_pParamsHolder->getParams().export2XObject())
    {
      PDFXObjectForm* pXobj = dynamic_cast<PDFXObjectForm*>(m_pParamsHolder->ContentCommands());
      if (NULL != pXobj)
        pResDict = pXobj->dictionary()->getResources();
    }
    else
    {
      if (m_currentPdfPage->HasItem("Resources"))
        pResDict = m_currentPdfPage->getResources();
    }
  }
  return pResDict;
}

void PDF2dExportDevice::update(OdGsDCRect* pUpdatedRect)
{
  if (isLayersSupported() && m_LayerOpen)
  {
    CloseLayerBracket();
    m_curLayerName = "";
  }

  // try to use PdfExport parameter background color.
  ODCOLORREF bg = getBackgroundColor();
  if (m_currentPdfPage && bg != ODRGB(255, 255, 255) && !m_paperBox.is_null())
  {
    set_Color(bg);

    double x = m_paperBox.m_max.x - m_paperBox.m_min.x;
    double y = m_paperBox.m_max.y - m_paperBox.m_min.y;

    OdGePoint2d rect[4];
    rect[0].set(0, 0);
    rect[1].set(x, 0);
    rect[2].set(x, y);
    rect[3].set(0, y);
    dc_polygon(4, rect);
  }

  Od2dExportDevice::update(pUpdatedRect);

  if (exportParams().getPRCMode() != PDFExportParams::kDisabled)
  {
    finishPRCVectorization();
  }

  if (isLayersSupported() && m_LayerOpen)
  {
    CloseLayerBracket();
    m_curLayerName = "";
  }
}

double PDF2dExportDevice::renderAreaMultiplier(OdUInt32 width, OdUInt32 height) const
{
  OdUInt32 dimMax = odmax(width, height);
  OdUInt32 view3dDPI = 150;//exportParams().view3dDPI;
  double koef = view3dDPI / 72.;
  if (dimMax * koef > 4096.) // 3d renderer limitation
    koef = 4096. / dimMax;
  return koef;
}

//====================================================================
//
OdGsDevicePtr PDF2dExportDevice::createObject()
{
  OdGsDevicePtr pRes = OdRxObjectImpl<PDF2dExportDevice, OdGsDevice>::createObject();
  return pRes;
}


//====================================================================
//
OdGsViewPtr PDF2dExportDevice::createView(const OdGsClientViewInfo* pViewInfo, bool bEnableLayVisPerView)
{
  OdGsViewPtr pView = PDF2dExportView::createObject();
  PDF2dExportView* pMyView = static_cast<PDF2dExportView*>(pView.get());

  pMyView->init(this, pViewInfo, bEnableLayVisPerView);

  pMyView->setAnalyticLinetypingCircles(true);
  pMyView->setAnalyticLinetypingComplexCurves(true);

  pMyView->screenOutput().setDestGeometry(*this);

  return pMyView;
}


//====================================================================
//
void PDF2dExportDevice::setContext(PdfExportParamsHolderPointer params_holder)
{
  m_pParamsHolder = params_holder;
  DWG_PALETTE_NAME = PDFName::createObject(m_pParamsHolder->document(), ("DWGPalette"));
  PDF_OC_NAME = PDFName::createObject(m_pParamsHolder->document(), ("OC"));
}

bool PDF2dExportDevice::isFrozenLayer(const OdString &layerName) const
{
  return m_frozenLayers.find(layerName) != m_frozenLayers.end();
}

//====================================================================
//

//====================================================================
//
void PDF2dExportDevice::set_Color(ODCOLORREF rgb)
{
  if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
    convertColorToGrayscale(rgb);
  else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
    rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

  if (!m_bRGBActive || m_curRGB != rgb)
  {
    //m_bRGBActive = true;
    m_curRGB = rgb; // If the color was changed

    dc_color(rgb);
  }
}

//====================================================================
//
void PDF2dExportDevice::set_SecondaryColor(ODCOLORREF rgb)
{
  if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
    convertColorToGrayscale(rgb);
  else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
    rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

  if (!m_bRGBActive || m_curSecondaryColorRGB != rgb)
  {
    //m_bRGBActive = true;
    m_curSecondaryColorRGB = rgb; // If the color was changed
  }
}

//====================================================================
//
void PDF2dExportDevice::set_Layer(OdDbStub* layer)
{
  if(!layer)
    return;
  OdRxObjectPtr pLayer(userGiContext()->openDrawable(layer));
  if (pLayer.isNull())
    return;
  OdString strLayerName = OdDbBaseLayerPEPtr(pLayer)->name(pLayer);
  if (m_curLayerName != strLayerName)
  {
    m_curLayerName = strLayerName;
    if (isLayersSupported())
      dc_layer(m_curLayerName, m_LayerOpen);
  }
}

OdString PDF2dExportDevice::layerName(OdDbStub* layer)
{
  if (!layer)
    return OdString();
  OdRxObjectPtr pLayer(userGiContext()->openDrawable(layer));
  if (pLayer.isNull())
    return OdString();
  OdString strLayerName = OdDbBaseLayerPEPtr(pLayer)->name(pLayer);
  return strLayerName;
}

void PDF2dExportDevice::convertColorToGrayscale(ODCOLORREF& rgb)
{
  OdUInt8 gray = (OdUInt8)((30 * ODGETRED(rgb) + 59 * ODGETGREEN(rgb) + 11 * ODGETBLUE(rgb)) / 100);
  rgb = ODRGBA(gray, gray, gray, ODGETALPHA(rgb));
}

//====================================================================
//
void PDF2dExportDevice::set_ColorIndex(OdUInt16 colorIndex)
{
  if (m_bRGBActive || m_curColorIndex != colorIndex)
  {
    ODCOLORREF rgb = getColor(colorIndex);
    if (exportParams().colorPolicy() == PDFExportParams::kGrayscale || exportParams().colorPolicy() == PDFExportParams::kMono)
    {
      if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
        convertColorToGrayscale(rgb);
      else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
        rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

      m_curRGB = rgb;
      dc_color(rgb);
    }

    else
    {
      //m_bRGBActive = false;
      m_curColorIndex = colorIndex; // If the color was changed
      dc_color(colorIndex);
    }
  }
}

//====================================================================
//
void PDF2dExportDevice::set_SecondaryColorIndex(OdUInt16 colorIndex)
{
  if (m_bRGBActive || m_curSecondaryColorIndex != colorIndex)
  {
    ODCOLORREF rgb = getColor(colorIndex);

    if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
      convertColorToGrayscale(rgb);
    else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
      rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

    m_curSecondaryColorRGB = rgb;
    
    //m_bRGBActive = false;
    m_curSecondaryColorIndex = colorIndex; // If the color was changed
  }
}

//==========================================================================================
//***************************************************************************
// getLineWeightInMM()
//
//  To translate the OdDb lineweight (in 100ths of a millimeter) to WT_Line_Weight code
//***************************************************************************
//int GsPdfDevice::getLineWeightInMM(const OdDb::LineWeight lineWeight)
double getLineWeightInMM(const OdDb::LineWeight lineWeight)
{
  double wtLineWeight = 0;

  switch (lineWeight)
  {
  case OdDb::kLnWt000:      wtLineWeight = 0;       break;
  case OdDb::kLnWt005:      wtLineWeight = 0.05;    break;
  case OdDb::kLnWt009:      wtLineWeight = 0.09;    break;
  case OdDb::kLnWt013:      wtLineWeight = 0.13;    break;
  case OdDb::kLnWt015:      wtLineWeight = 0.15;    break;
  case OdDb::kLnWt018:      wtLineWeight = 0.18;    break;
  case OdDb::kLnWt020:      wtLineWeight = 0.20;    break;
  case OdDb::kLnWt025:      wtLineWeight = 0.25;    break;
  case OdDb::kLnWt030:      wtLineWeight = 0.30;    break;
  case OdDb::kLnWt035:      wtLineWeight = 0.35;    break;
  case OdDb::kLnWt040:      wtLineWeight = 0.40;    break;
  case OdDb::kLnWt050:      wtLineWeight = 0.50;    break;
  case OdDb::kLnWt053:      wtLineWeight = 0.53;    break;
  case OdDb::kLnWt060:      wtLineWeight = 0.60;    break;
  case OdDb::kLnWt070:      wtLineWeight = 0.70;    break;
  case OdDb::kLnWt080:      wtLineWeight = 0.80;    break;
  case OdDb::kLnWt090:      wtLineWeight = 0.90;    break;
  case OdDb::kLnWt100:      wtLineWeight = 1.00;    break;
  case OdDb::kLnWt106:      wtLineWeight = 1.06;    break;
  case OdDb::kLnWt120:      wtLineWeight = 1.20;    break;
  case OdDb::kLnWt140:      wtLineWeight = 1.40;    break;
  case OdDb::kLnWt158:      wtLineWeight = 1.58;    break;
  case OdDb::kLnWt200:      wtLineWeight = 2.00;    break;
  case OdDb::kLnWt211:      wtLineWeight = 2.11;    break;

  case OdDb::kLnWtByLayer:      // -1,
  case OdDb::kLnWtByBlock:      // -2,
  case OdDb::kLnWtByLwDefault:  // -3
    ODA_ASSERT(false);
  default:
    wtLineWeight = 0;
    break;
  };

  return wtLineWeight;

}

//====================================================================
//
void PDF2dExportDevice::set_LineWeight(double lineWeight)
{
  if (m_lineWeight != lineWeight)
  {
    // If the lineWeight was changed
    m_lineWeight = lineWeight;
    dc_lineWeight( fabs(lineWeight) ); // temporary fix for 6776
  }
}

//====================================================================
//
void PDF2dExportDevice::onTraitsModified(const OdGiSubEntityTraitsData& currTraits)
{
  Od2dExportDevice::onTraitsModified(currTraits);
}

class PdfBug16745Wrap : public OdGiRasterImage
{
public:
  PdfBug16745Wrap() {};

  void setData(OdGiRasterImagePtr pImage, OdGiRasterImage::PixelFormatInfo pixelFormat)
  {
    m_pImage = pImage;
    m_PixelFormat = pixelFormat;
  };

  virtual OdUInt32 pixelWidth() const { return m_pImage->pixelWidth(); }
  virtual OdUInt32 pixelHeight() const { return m_pImage->pixelHeight(); }
  virtual OdUInt32 colorDepth() const { return m_pImage->colorDepth(); }
  virtual OdUInt32 numColors() const { return m_pImage->numColors(); }
  virtual ODCOLORREF color(OdUInt32 colorIndex) const { return m_pImage->color(colorIndex); }
  virtual OdUInt32 paletteDataSize() const { return m_pImage->paletteDataSize(); }
  virtual void paletteData(OdUInt8* bytes) const { m_pImage->paletteData(bytes); }
  virtual PixelFormatInfo pixelFormat() const
  {
    return m_PixelFormat;
  }
  virtual Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const { return m_pImage->defaultResolution(xPelsPerUnit, yPelsPerUnit); }
  virtual int transparentColor() const { return m_pImage->transparentColor(); }
  virtual OdUInt32 scanLineSize() const { return m_pImage->scanLineSize(); }
  virtual ImageSource imageSource() const { return m_pImage->imageSource(); }
  virtual const OdString &sourceFileName() const { return m_pImage->sourceFileName(); }
  virtual TransparencyMode transparencyMode() const { return m_pImage->transparencyMode(); }

  virtual const OdUInt8* scanLines() const { return m_pImage->scanLines(); }
  virtual void scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines = 1) const { m_pImage->scanLines(pBytes, index, numLines); }

  virtual OdUInt32 scanLinesAlignment() const { return m_pImage->scanLinesAlignment(); }

private:
  OdGiRasterImagePtr m_pImage;
  OdGiRasterImage::PixelFormatInfo m_PixelFormat;

};

void PDF2dExportDevice::rasterImageProc(const OdGePoint3d& origin,
                                        const OdGeVector3d& u,
                                        const OdGeVector3d& v,
                                        const OdGiRasterImage* pImg, // image object
                                        const OdGePoint2d* uvBoundary, // may not be null
                                        OdUInt32 numBoundPts,
                                        bool transparency,
                                        double brightness,
                                        double contrast,
                                        double fade)
{
  const OdGePoint2d* pUVBound = uvBoundary;

  OdGePoint2d tmp[4];
  if (numBoundPts == 2)
  {
    numBoundPts = 4;
    tmp[0].set((uvBoundary[0].x), (uvBoundary[0].y));
    tmp[1].set((uvBoundary[0].x), (uvBoundary[1].y));
    tmp[2].set((uvBoundary[1].x), (uvBoundary[1].y));
    tmp[3].set((uvBoundary[1].x), (uvBoundary[0].y));

    pUVBound = tmp;
  }

  OdGePoint2dArray newBoundary;
  if (numBoundPts)
  {
    OdGeMatrix2d m = createImageClipTransform(pImg, origin, u, v); // transform boundary from image coordinates to device coordinates
    for (OdUInt32 i = 0; i < numBoundPts; i++)
    {
      OdGePoint2d pt(pUVBound[i].x + .5, pUVBound[i].y + .5);
      newBoundary.push_back(pt.transformBy(m));
    }
    pUVBound = newBoundary.asArrayPtr();
  }

  // It need for bitonal images.
  ODCOLORREF entityColor(0);

  const OdCmEntityColor& color = m_effTraits.trueColor();
  if (color.isByColor()) {
    entityColor = ODRGB(color.red(), color.green(), color.blue());
  }
  else {
    entityColor = getColor(color.colorIndex());
  }

  if (!m_bHatchDrawingMode)
  {
    OdGiRasterImage::PixelFormatInfo pfOrig = pImg->pixelFormat();
    OdUInt32 current_dpi = OdRoundToLong((double)exportParams().getGeomDPI() / u.length());
    OdUInt32 necessary_dpi = pImg->numColors() == 2 ? m_bwBitmapDPI : m_colorBitmapDPI;
    if (OdGreater(current_dpi, necessary_dpi, .1*(double)current_dpi))//set the tolerance to 10% for avoid rounding errors etc.
      //current dpi == necessary dpi for OLE images (see PDF2dExportView::ownerDrawDc)
      /*current dpi can't be less than necessary dpi for non-OLE images (because of for these images the dpi in that stage is always
      maximum that it can be, so - here is no point to increase it)*/
    {
      double iScale = (double)necessary_dpi / (double)current_dpi;

      OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
      OdMemoryStreamPtr pStm = OdMemoryStream::createNew();
      OdUInt32 flags[9] = { OdRxRasterServices::kRescale, OdRxRasterServices::kRescaleBox, OdRxRasterServices::kRescaleWidth,(OdUInt32)((double)pImg->pixelWidth()*iScale),
        OdRxRasterServices::kRescaleHeight, (OdUInt32)((double)pImg->pixelHeight()*iScale), 0, 0, 0 };
      if (pImg->numColors() == 2)
      {
        flags[6] = OdRxRasterServices::kDithering;
        flags[7] = OdRxRasterServices::kDitheringFS;
      }

      if (pRasSvcs->convertRasterImage(pImg, OdRxRasterServices::kBMP, pStm, flags))
      {
        pStm->rewind();
        OdGiRasterImagePtr pImage = pRasSvcs->loadRasterImage(pStm);

        if (!pImage.isNull())
        {
          OdGiRasterImage::PixelFormatInfo pfConverted = pImage->pixelFormat();
          if (pfOrig == pfConverted || pImg->numColors())
            dc_raster_image(origin, u / iScale, v / iScale, pImage, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
          else
          { //workaround for bug 16745
            OdSmartPtr<PdfBug16745Wrap> pWrapper = OdRxObjectImpl<PdfBug16745Wrap>::createObject();
            pWrapper->setData(pImage, pfOrig);
            dc_raster_image(origin, u / iScale, v / iScale, pWrapper, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
          }
        }
        else //if null, let's export the picture the old way at least
          dc_raster_image(origin, u, v, pImg, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
      }
      else //if false, let's export the picture the old way at least
        dc_raster_image(origin, u, v, pImg, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
    }
    else
      dc_raster_image(origin, u, v, pImg, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
  }
  else dc_raster_image(origin, u, v, pImg, pUVBound, numBoundPts, transparency, brightness, contrast, fade, entityColor);
}

void PDF2dExportDevice::metafileProc(
          const OdGePoint3d& /*origin*/,
		      const OdGeVector3d& /*u*/,
		      const OdGeVector3d& /*v*/,
          const OdGiMetafile* /*pMetafile*/,
          bool /*bDcAligned*/,       // reserved
          bool /*bAllowClipping*/) // reserved
{
  ODA_ASSERT(0);
}

static bool isAtFont(const OdGiTextStyle* pTextStyle)
{
  if (!pTextStyle->isTtfFont())
    return false;

  const OdTtfDescriptor &descr = pTextStyle->ttfdescriptor();
  bool ret = !descr.typeface().isEmpty() && descr.typeface()[0] == '@';
  return ret;
}

void PDF2dExportDevice::textProc(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            const OdChar* msg, 
            OdInt32 length, 
            bool bRaw, 
            const OdGiTextStyle* pTextStyle,
            const OdGeVector3d* pExtrusion)
{
  OdGeVector2d u2d = u.convert2d();
  OdGeVector2d v2d = v.convert2d();
  if (OdEqual(u2d.length(), 0) || OdEqual(v2d.length(), 0)) //the text is perpendicular to eye - no necessary to draw it (BZ17053)
    return;
  PolygonFillRules currentFillRule = m_fillRule;
  m_fillRule = kFillNonzeroRule; //Bug CORE-11184 - Text renders darker/thicker during PDF Export in 4.2.2 (reason is stroking the path by default)
  if (!isTTFTextGeometry() && (pTextStyle->isTtfFont() && !isAtFont(pTextStyle))
      || !isSHXTextGeometry() && pTextStyle->isShxFont())
  {
    Od2dExportDevice::textProc( position, u, v, msg, length, bRaw, pTextStyle, pExtrusion );
  }
  else
  {
    OdGiGeometrySimplifier::textProc( position, u, v, msg, length, bRaw, pTextStyle, pExtrusion );
  }
  m_fillRule = currentFillRule;
}

void PDF2dExportDevice::nurbsProc(const OdGeNurbCurve3d& inputNurb)
{
  OdGeNurbCurve3d nurbsCurve(inputNurb);
  nurbsCurve.hardTrimByParams(nurbsCurve.startParam(), nurbsCurve.endParam());

  unsigned int i;
  int degree;
  bool rational;
  bool periodic;
  OdGeKnotVector aKt;
  OdGePoint3dArray aCP;
  OdGeDoubleArray aWt;
  nurbsCurve.getDefinitionData(degree, rational, periodic, aKt, aCP, aWt);
  if(degree < 2 || degree > 3 || rational)
  {
    Od2dExportDevice::nurbsProc(nurbsCurve);
    return;
  }
  OdGeNurbCurve3d crvAux(nurbsCurve);
  OdGeDoubleArray aWtEmpty;
  //crvAux.set(degree, aKt, aCP, aWtEmpty, periodic);
  if(degree == 2)
  {
    crvAux.elevateDegree(1);
    crvAux.getDefinitionData(degree, rational, periodic, aKt, aCP, aWt);
  }

  OdGeDoubleArray knots;
  aKt.getDistinctKnots(knots, NULL);
  OdGeKnotVector aKt2d;
  aKt2d.setLogicalLength(8);
  aKt2d[0] = aKt2d[1] = aKt2d[2] = aKt2d[3] = 0.;
  aKt2d[4] = aKt2d[5] = aKt2d[6] = aKt2d[7] = 1.;
  OdGePoint2dArray aCP2d;
  aCP2d.setLogicalLength(4);
  OdGeCurve3d* pCrv = &crvAux;
  PDFIContentCommands *pOut = cc();
  if(m_stateDrawingContour == 2) // first curve of contour
  {
    pOut->m(aCP[0].x, aCP[0].y);
    m_stateDrawingContour = 1;
  }
  for(i = 1; i < knots.length(); ++i)
  {
    OdGeCurve3d *crv1 = NULL;
    OdGeCurve3d *crv2 = NULL;
    if(i < knots.length() - 1)
    {
      try
      {
        pCrv->getSplitCurves(knots[i], crv1, crv2);
      }
      catch(...)
      {
        // split may fail on bad geometry, output polyline
        Od2dExportDevice::nurbsProc(*((OdGeNurbCurve3d*)pCrv));
        if(i > 1)
        {
          delete pCrv;
        }
        return;
      }
      if(i > 1)
      {
        delete pCrv;
      }
    }
    else
    {
      crv1 = pCrv;
    }

    if(crv1 == NULL)  // this can be if bad knots only (bug 8389)
    {
      return;
    }

    ODA_ASSERT(crv1->isKindOf(OdGe::kNurbCurve3d));
    OdGeNurbCurve3d* nurbs1 = (OdGeNurbCurve3d*)crv1;
    if(nurbs1->numControlPoints() != 4)  // bad geometry, knots are too close
    {
      Od2dExportDevice::nurbsProc(*nurbs1);
    }
    else
    {
      for(int j = 0; j < 4; ++j)
      {
        aCP2d[j] = nurbs1->controlPointAt(j).convert2d();
      }

      OdGeNurbCurve2d nurbs2d;
      double period;

      ODA_ASSERT(!periodic || (periodic && ((knots.length() > 2 && !nurbs1->isPeriodic(period))
        || (knots.length() == 2 && nurbs1->isPeriodic(period)))));

      nurbs2d.set(3, aKt2d, aCP2d, aWtEmpty, nurbs1->isPeriodic(period));

      if(m_stateDrawingContour > 0)
      {
        pOut->c(aCP2d[1], aCP2d[2], aCP2d[3]);
      }
      else
      {
        dc_nurb(nurbs2d);
      }
    }

    if(knots.length() > 2)
    {
      delete crv1;
    }

    pCrv = crv2;
  }
}


void PDF2dExportDevice::shxText( const OdGePoint3d& position,
                                const OdGeVector3d& u,
                                const OdGeVector3d& v,
                                const OdChar* msg,
                                OdInt32 length,
                                bool raw,
                                const OdGiTextStyle* pStyle,
                                const OdGeVector3d* pExtrusion )
{
  ODA_ASSERT(!pExtrusion);

  bool bASCII(true);

  bool bAlwaysUnicode = GETBIT(getDeviceType(), Od2dExportDevice::kUnicodeOnly);
  if ( bAlwaysUnicode )
    bASCII = false;

  OdString sProcessedText;
  OdArray<OdUInt16>    arrayUnicode;
  OdBoolArray isInBigFont;
  OdGePoint2dArray Advances;

  if (!bASCII || !textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText, arrayUnicode, true, &isInBigFont, &Advances))
  {
    bASCII = false;
    arrayUnicode.clear();
    isInBigFont.clear();
    Advances.clear();

    textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText, arrayUnicode, false, &isInBigFont, &Advances);
  }

  setSHXText(position, u, v, msg, length, raw, pStyle, OdString(OdUS2W(arrayUnicode.asArrayPtr()), arrayUnicode.size()), isInBigFont, Advances);
}

void PDF2dExportDevice::setSHXText(const OdGePoint3d& position,
                                   const OdGeVector3d& u,      // normal
                                   const OdGeVector3d& v,      // direction
                                   const OdChar* /*msg*/, OdInt32 /*length*/, bool raw,
                                   const OdGiTextStyle* pTextStyle,
                                   const OdString &pUnicode,
                                   const OdBoolArray &isInBigFont,
                                   const OdGePoint2dArray &Advances)
{
  bool isShxFont = pTextStyle->isShxFont();
  ODA_ASSERT(isShxFont);
  if (!isShxFont)
    return;

  bool bVertical = pTextStyle->isVertical();

  OdGePoint2d pos2d = position.convert2d();
  OdGeVector2d u2d = u.convert2d();
  OdGeVector2d v2d = v.convert2d();

  OdUInt16Array oneUnicodeChar;
  OdDoubleArray oneCharSpace;
  //OdBoolArray oneFlag;
  OdAnsiString oneCharText;

  PDFFontPtr pT3Font, prevT3Font;
  OdAnsiString fontName, prevFontName;

  OdUInt32 nStartIndx = 0;
  OdUInt32 nLen = (OdUInt32)pUnicode.getLength();

  PDFResourceDictionaryPtr pResDict = getResourceDictionary();
  if (pResDict.isNull())
  {
    pResDict = PDFPageNodeDictionaryPtr(document().Root()->getPages())->getResources();
  }

  if (!bVertical)
  {
    for(OdUInt32 f = 0; f < nLen; ++f)
    {
      OdAnsiString tmp;
      getFontType3Optimizer().getFontForCharacter(document(), *pTextStyle, pUnicode.getAt(f), pT3Font, fontName, m_lineWeight, u2d, v2d, pResDict);
      getFontType3Optimizer().addUnicodeChar(*pTextStyle, pT3Font, pUnicode.getAt(f), tmp, isInBigFont[f]);

      // pUnicode is in MBCS for bigfont.
      OdChar realUnicode = pUnicode.getAt(f);
      if (isInBigFont[f])
      {
        OdString bigFontName = pTextStyle->bigFontFileName();
        OdCodePageId bigFontCodePage = OdCharMapper::getCpByBigFont(bigFontName);
        OdCharMapper::codepageToUnicode(realUnicode, bigFontCodePage, realUnicode);
      }

      if (prevFontName.isEmpty())
      {
        prevFontName = fontName;
        prevT3Font = pT3Font;
      }

      if (!OdEqual(pTextStyle->trackingPercent(), 1.))
      {
        //calculate char spacing
        OdGePoint3d min, max;
        OdGePoint3d pEndPos;
        userGiContext()->textExtentsBox(*pTextStyle, &realUnicode, 1, raw ? kOdGiRawText : 0, min, max, &pEndPos);
        double charSpace = (pEndPos.x - pEndPos.x / pTextStyle->trackingPercent())*(-1000.) / TD_PDF_HELPER_FUNCS::textHeight(*pTextStyle);
        oneCharSpace.append(charSpace);
      }

      if (fontName != prevFontName || (f+1) == nLen)
      {
        if (f+1 == nLen)
        {
          oneUnicodeChar.append(realUnicode);
          //oneFlag.append(isInBigFont[f]);
          oneCharText += tmp;
        }

        // all text will be in \OOO form.
        dc_drawtext(prevFontName, Advances[nStartIndx], u2d, v2d, *oneCharText.c_str(), oneCharText.getLength(), *pTextStyle, oneCharSpace);

        nStartIndx = f;

        oneUnicodeChar.resize(1, realUnicode);
        //oneFlag.resize(1, isInBigFont[f]);
        oneCharText = tmp;
      }
      else
      {
        oneCharText += tmp;
        oneUnicodeChar.append(realUnicode);
        //oneFlag.append(isInBigFont[f]);
      }

      prevFontName = fontName;
      prevT3Font = pT3Font;
    }
  }
  else
  { // vertical text
    for(OdUInt32 f = 0; f < nLen; ++f)
    {
      OdAnsiString oneCharText;
      getFontType3Optimizer().getFontForCharacter(document(), *pTextStyle, pUnicode.getAt(f), pT3Font, fontName, m_lineWeight, u2d, v2d, pResDict);
      getFontType3Optimizer().addUnicodeChar(*pTextStyle, pT3Font, pUnicode.getAt(f), oneCharText, isInBigFont[f]);

      if (!OdEqual(pTextStyle->trackingPercent(), 1.))
      {
        //calculate char spacing
        OdGePoint3d min, max;
        OdGePoint3d pEndPos;
        OdChar realUnicode = pUnicode.getAt(f);
        userGiContext()->textExtentsBox(*pTextStyle, &realUnicode, 1, raw ? kOdGiRawText : 0, min, max, &pEndPos);
        double charSpace = (pEndPos.x - pEndPos.x / pTextStyle->trackingPercent())*(-1000.) / TD_PDF_HELPER_FUNCS::textHeight(*pTextStyle);
        oneCharSpace.append(charSpace);
      }

      // all text will be in \OOO form.
      dc_drawtext(fontName, Advances[f], u2d, v2d, *oneCharText.c_str(), oneCharText.getLength(), *pTextStyle, oneCharSpace);
    } // end of vertical way
  }
}


void PDF2dExportDevice::polygonOut( OdInt32 nbPoints, 
                                   const OdGePoint3d* pVertexList, 
                                   const OdGeVector3d* /*pNormal*/)
{
  OdGePoint2dArray pnts2d;
  pnts2d.resize(nbPoints);

  for(OdInt32 f=0; f<nbPoints; ++f)
  {
    pnts2d[f] = pVertexList[f].convert2d();
  }

  dc_polygon(nbPoints, pnts2d.asArrayPtr());
}

void PDF2dExportDevice::shellFaceOut( OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* pNormal )
{
  OdInt32 nLoopSize = *pFaceList;
  const bool bSingleLoop = nLoopSize+1 == faceListSize;

  // gradient hatch has different vertex colors, so single loop should be triangulated
  const OdGiVertexData* vd = vertexData();
  if ((vd && vd->trueColors()) || !bSingleLoop)
  {
    Od2dExportDevice::shellFaceOut( faceListSize, pFaceList, pNormal );
    return;
  }

  OdGePoint2dArray pnts2d;
  pnts2d.resize(pFaceList[0]);

  for(OdInt32 f=1; f<=pFaceList[0]; ++f)
  {
    pnts2d[f-1] = vertexDataList()[pFaceList[f]].convert2d();
  }

  dc_polygon(pFaceList[0], pnts2d.asArrayPtr());
}

#ifdef BR_CR_10507
void PDF2dExportDevice::shellProc(OdInt32 nbVertex,
                                  const OdGePoint3d* pVertexList,
                                  OdInt32 faceListSize,
                                  const OdInt32* pFaceList,
                                  const OdGiEdgeData* pEdgeData,
                                  const OdGiFaceData* pFaceData,
                                  const OdGiVertexData* pVertexData)
{
  bool bEdgesOn;
  if (fillMode(bEdgesOn))
  {
    // draw all edges, including internal and invisible, by face color
    // to avoid appearing of unpainted places between faces.
    OdGiSubEntityTraits& traits = m_pDrawCtx->subEntityTraits();
    const OdDb::LineWeight lw = traits.lineWeight();
    traits.setLineWeight(OdDb::kLnWt000);
    m_pDrawCtx->onTraitsModified();

    generateShellWires(faceListSize, pFaceList, NULL, pFaceData);

    traits.setLineWeight(lw);
    m_pDrawCtx->onTraitsModified();
  }
}
#endif

bool PDF2dExportDevice::ellipArcProc(const OdGeEllipArc3d& arc, double width)
{
  OdGeEllipArc3d ellipArc(arc);
  if (OdZero(ellipArc.majorRadius()) || OdZero(ellipArc.minorRadius()))
  {
    return false;
  }

  OdGe::ErrorCondition ret = OdGe::kOk;
  OdGeVector3d normal = ellipArc.normal();
  normal.normalize(OdGeContext::gTol, ret);
  if(ret != OdGe::kOk)
  {
    return false;
  }
  if(normal.z < 0)
  {
    ellipArc.reverseParam();
  }
 
  double widthOld = m_lineWeight;
  setCapJoin(kButtCap, kRoundJoin);
  set_LineWeight(fabs(width));
  dc_ellipse(ellipArc);

  OdUInt32  uCapStyle   = userGiContext()->lineWeightConfiguration(OdGiContext::kLineCapStyle);
  OdUInt32  uJointStyle = userGiContext()->lineWeightConfiguration(OdGiContext::kLineJoinStyle);

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

  setCapJoin(uLineCap, uLineJoin);

  set_LineWeight(fabs(widthOld));

  return true;
}

bool PDF2dExportDevice::plotGeneratorEllipseOut(const OdGeEllipArc3d& arc, double width)
{
  bool bRet = ellipArcProc(arc, width);
  return bRet;
}

void PDF2dExportDevice::startDrawingContour()
{
  m_stateDrawingContour = 2;
  ClosePolyline();
}

void PDF2dExportDevice::endDrawingContour()
{
  if(m_stateDrawingContour == 0)
  {
    return;
  }
  m_stateDrawingContour = 0;
  dc_fill();
}

}
