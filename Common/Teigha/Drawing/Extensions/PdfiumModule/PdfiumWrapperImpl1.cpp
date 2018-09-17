
//File added because of names conflict between cpdf_pageobject.h (in it's includes) and Gi enum TextRenderingMode
#include "OdaCommon.h"
#include "PdfiumWrapperImpl.h"

#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve3d.h"

#include "public/fpdf_edit.h"
#include "fpdfsdk/fsdk_define.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/cpdf_reference.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pageobject.h"
#include "core/fpdfapi/page/cpdf_pathobject.h"
#include "core/fpdfapi/render/cpdf_renderoptions.h"
#include "core/fpdfapi/cpdf_pagerendercontext.h"
#include "core/fxge/ifx_renderdevicedriver.h"

void PdfiumWrapperImpl::getPageLayerInfo(int nPage) const
{
  if (!m_pDocument || !m_pLayers)
    return;

  m_LayersCache.clear();
  m_layerInfoCachePage = nPage;

  int objCount = FPDFPage_CountObjects(m_Pages[nPage]);

  for (int i = 0; i < objCount; i++)
  {
    FPDF_PAGEOBJECT obj = FPDFPage_GetObject(m_Pages[nPage], i);
    if (obj)
    {
      CPDF_PageObject* pObj = CPDFPageObjectFromFPDFPageObject(obj);
      OdUInt32 countIt = 0;
      if (pObj->m_ContentMark.HasRef())
        countIt = pObj->m_ContentMark.CountItems();

      for (OdUInt32 j = 0; j < countIt; ++j)
      {
        const CPDF_ContentMarkItem& item = pObj->m_ContentMark.GetItem(j);
        if (item.GetName() == "OC" && item.GetParamType() == CPDF_ContentMarkItem::PropertiesDict)
        {
          CPDF_Dictionary* pDict = item.GetParam();
          if (pDict)
          {
            OdString name(pDict->GetUnicodeTextFor("Name").c_str());
            if (!m_LayersCache.contains(name))
              m_LayersCache.append(name);
          }
        }
      }
    }
  }

  //the code above will not find the layers which don't contain objects, so we need to check the page resources
  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(m_pDocument);
  if (pDoc)
  {
    OdString sOCG("OCG");
    CPDF_Dictionary* pPageDict = pDoc->GetPage(nPage);
    if (pPageDict)
    {
      CPDF_Dictionary* pResDict = pPageDict->GetDictFor("Resources");
      if (pResDict)
      {
        CPDF_Dictionary* pPropDict = pResDict->GetDictFor("Properties");
        if (pPropDict && pPropDict->GetCount())
        {
          CPDF_Dictionary::const_iterator iter = pPropDict->begin();
          while (iter != pPropDict->end())
          {
            CPDF_Dictionary* pOCG = (*iter).second->AsReference()->GetDirect()->AsDictionary();

            if (pOCG && !sOCG.compare(pOCG->GetUnicodeTextFor("Type").c_str()))
            {
              OdString name(pOCG->GetUnicodeTextFor("Name").c_str());
              if (!m_LayersCache.contains(name))
                m_LayersCache.append(name);
            }
            ++iter;
          }
        }
      }
    }
  }
}

#if 0
void PdfiumWrapperImpl::createGeometryCache(int nPage, const OdArray<OdUnderlayLayer> &Layers) const
{
  if (!m_pDocument)
    return;

  m_geometryCache.clear(nPage);
  setupLayers(Layers);
  double scale = getMeasureDictInfo(nPage);

  int objType;
  int objCount = FPDFPage_CountObjects(m_Pages[nPage]);

  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(m_pDocument);
  const CPDF_OCContext::UsageType usage = CPDF_OCContext::View;
  std::unique_ptr<CPDF_RenderOptions> pOptions = pdfium::MakeUnique<CPDF_RenderOptions>();
  pOptions->SetOCContext(pdfium::MakeRetain<CPDF_OCContext>(pDoc, usage));

  CPDF_Page* pPage = CPDFPageFromFPDFPage(m_Pages[nPage]);
  if (!pPage)
  {
    return;
  }
  int rotation = pPage->GetPageRotation();
  OdGeMatrix2d rotMat;
  if (rotation)
  {
    double width = pPage->GetPageWidth();
    double height = pPage->GetPageHeight();
    rotMat.setToRotation(-OdaPI2*rotation);
    if (rotation == 1) //90
    {
      rotMat.setTranslation(OdGeVector2d(0, height));
    } 
    else if (rotation == 2) //180
    {
      rotMat.setTranslation(OdGeVector2d(width, height));
    }
    else if (rotation == 3) //270
    {
      rotMat.setTranslation(OdGeVector2d(width, 0));
    }
  }

  for (int i = 0; i < objCount; i++)
  {
    FPDF_PAGEOBJECT obj = FPDFPage_GetObject(m_Pages[nPage], i);
    if (obj)
    {
      //CHECK GEOMETRY BY LAYERS////////////////////////////////////////////////
      CPDF_PageObject* pObj = CPDFPageObjectFromFPDFPageObject(obj);
      if (pOptions->GetOCContext() && pObj->m_ContentMark.HasRef() && !pOptions->GetOCContext()->CheckObjectVisible(pObj))
        continue;

      objType = FPDFPageObj_GetType(obj);
      if (objType == FPDF_PAGEOBJ_PATH)
      {
        CPDF_PathObject* path = pObj->AsPath();
        if (!path)
          continue;
        CFX_Matrix ctm = path->m_Matrix;
        OdGeMatrix2d mat;
        mat[0][0] = ctm.a;
        mat[1][0] = ctm.b;
        mat[0][1] = ctm.c;
        mat[1][1] = ctm.d;
        mat[0][2] = ctm.e;
        mat[1][2] = ctm.f;
        int segCount = FPDFPath_CountSegments(obj);
        FPDF_PATHSEGMENT segment = 0;
        int segType = FPDF_SEGMENT_UNKNOWN, curSeg = FPDF_SEGMENT_UNKNOWN;
        OdGePoint2d pt;
        OdGePoint2dArray pt_cache;
        int iClose = 0;

        for (int j = 0; j < segCount; j++)
        {
          segment = FPDFPath_GetPathSegment(obj, j);
          segType = FPDFPathSegment_GetType(segment);
          float x, y;
          FPDFPathSegment_GetPoint(segment, &x, &y);
          pt.x = x;
          pt.y = y;
          
          pt.transformBy(mat);
          pt.transformBy(rotMat);
          //pt.x = (x*ctm.a + ctm.e)*25.4 / 72.;
          //pt.y = (y*ctm.d + ctm.f)*25.4 / 72.;
          pt *= scale;

          iClose = FPDFPathSegment_GetClose(segment);

          if (segType == FPDF_SEGMENT_MOVETO) //new figure
          {
            if (pt_cache.size())
            {
              if (curSeg == FPDF_SEGMENT_LINETO)
                savePolyline(pt_cache);
              else if (curSeg == FPDF_SEGMENT_BEZIERTO)
                saveSpline(pt_cache);
            }
            curSeg = FPDF_SEGMENT_MOVETO;
            pt_cache.clear();
            pt_cache.append(pt);
          } //segType == FPDF_SEGMENT_MOVETO
          else if (segType == FPDF_SEGMENT_LINETO)
          {
            if (curSeg == FPDF_SEGMENT_BEZIERTO)
            {
              saveSpline(pt_cache);
              OdGePoint2d ptTmp = pt_cache.last();
              pt_cache.clear();
              pt_cache.append(ptTmp);
            }
            curSeg = FPDF_SEGMENT_LINETO;
            pt_cache.append(pt);
          } //segType == FPDF_SEGMENT_LINETO
          else if (segType == FPDF_SEGMENT_BEZIERTO)
          {
            if (curSeg == FPDF_SEGMENT_LINETO)
            {
              savePolyline(pt_cache);
              OdGePoint2d ptTmp = pt_cache.last();
              pt_cache.clear();
              pt_cache.append(ptTmp);
            }
            curSeg = FPDF_SEGMENT_BEZIERTO;
            pt_cache.append(pt);
          } //segType == FPDF_SEGMENT_BEZIERTO
        }
        if (iClose != 0 && !pt_cache.first().isEqualTo(pt_cache.last()))
        {
          pt_cache.append(pt_cache.first());
        }
        if (curSeg == FPDF_SEGMENT_LINETO)
          savePolyline(pt_cache);
        else if (curSeg == FPDF_SEGMENT_BEZIERTO)
          saveSpline(pt_cache);
        pt_cache.clear();
      }
      else if (objType == FPDF_PAGEOBJ_FORM)
      {
        CPDF_FormObject* pForm = pObj->AsForm();
        if (!pForm)
          continue;
        //pForm->
      }
    }
  }
}
#else
class geomCacheDriver : public IFX_RenderDeviceDriver
{
  class BaseCurve
  {
  public:
    BaseCurve() {}
    ~BaseCurve() {}
    void addSeg(OdGePoint2dArray points, FXPT_TYPE type)
    {
      m_PointStorage.emplace_back(std::make_pair(points, type));
      if (type != FXPT_TYPE::LineTo)
      {
        m_isPolyline = false;
      }
    }
    void setClosed(bool state)
    {
      m_isClosed = state;
    }
    bool getClosed() const
    {
      return m_isClosed;
    }
    bool isPolyline() const
    {
      return m_isPolyline;
    }

    const std::vector<std::pair<OdGePoint2dArray, FXPT_TYPE>>& getStorage() const
    {
      return m_PointStorage;
    }
  protected:
    std::vector<std::pair<OdGePoint2dArray, FXPT_TYPE>> m_PointStorage;
    bool                                                m_isClosed{ false };
    bool                                                m_isPolyline{ true };

  };
public:
  explicit geomCacheDriver(int width, int height, double scale, const PdfiumWrapperImplPtr& wrp)
    : m_pWrapper(wrp)
    , m_Width(width)
    , m_Height(height)
    , m_Scale(scale)
  {};
  ~geomCacheDriver() override
  {
    EndRendering();
  }
protected:
  const PdfiumWrapperImplPtr m_pWrapper;
  int                        m_Width;
  int                        m_Height;
  double                     m_Scale;

  void SaveState() override {};
  void RestoreState(bool bKeepSaved) override {};
  bool SetClip_PathFill(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, int fill_mode) override { return false; };
  bool SetDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left, int top, int blend_type) override { return false; };
  bool StretchDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, int dest_left, int dest_top, int dest_width,
    int dest_height, const FX_RECT* pClipRect, uint32_t flags, int blend_type) override { return false; };
  bool StartDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, int bitmap_alpha, uint32_t color, const CFX_Matrix* pMatrix, uint32_t render_flags,
    std::unique_ptr<CFX_ImageRenderer>* handle, int blend_type) override { return false; };
  bool DrawDeviceText(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device,
    float font_size, uint32_t color)
  {
    return true; //if false is returned, some texts will be handled as geometry (snap geometry will be cached for them) 
  }
  bool GetClipBox(FX_RECT* pRect)
  {
    pRect->left = 0;
    pRect->right = m_Width;
    pRect->top = 0;
    pRect->bottom = m_Height;
    return true;
  }
  int GetDeviceCaps(int caps_id) const {
    switch (caps_id)
    {
    case FXDC_DEVICE_CLASS:
      return FXDC_DISPLAY;
    case FXDC_PIXEL_WIDTH:
      return m_Width;
    case FXDC_PIXEL_HEIGHT:
      return m_Height;
    case FXDC_BITS_PIXEL:
      return 72.;
    case FXDC_RENDER_CAPS:
      return FXRC_GET_BITS | FXRC_ALPHA_PATH | FXRC_ALPHA_IMAGE |
        FXRC_BLEND_MODE | FXRC_SOFT_CLIP;
    case FXDC_HORZ_SIZE:
      return m_Width;
    case FXDC_VERT_SIZE:
      return m_Height;
    }
    return 0;
  }
  bool DrawPath(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState,
    uint32_t fill_color, uint32_t stroke_color, int fill_mode, int blend_type) override
  {
    const std::vector<FX_PATHPOINT>& pPoints = pPathData->GetPoints();

    if (0 == pPoints.size())
      return true;

    OdGePoint2dArray pt_cache;

    std::vector<BaseCurve> paths;
    paths.resize(1);

    for (size_t i = 0; i < pPoints.size(); i++)
    {
      CFX_PointF pos = pPoints[i].m_Point;
      if (pObject2Device)
      {
        pos = pObject2Device->Transform(pos);
      }
      pos.y += abs(m_Height);

      OdGePoint2d point(pos.x /*/ DPI*/, pos.y /*/ DPI*/);

      OdGeMatrix2d matrix;
      matrix.setToIdentity();
      matrix *= OdGeMatrix2d::scaling(m_Scale);
//      matrix *= OdGeMatrix2d::rotation(m_pProperties->get_Rotation());
//      matrix.setTranslation(OdGeVector2d(m_pProperties->get_InsertionPointX(), m_pProperties->get_InsertionPointY()));

      point.transformBy(matrix);

      FXPT_TYPE point_type = pPoints[i].m_Type;
      if (point_type == FXPT_TYPE::MoveTo)
      {
        if (pt_cache.size())
        {
          paths.back().addSeg(pt_cache, pPoints[i - 1].m_Type);
          if (pPoints[i - 1].m_CloseFigure)
          {
            paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
            paths.emplace_back(BaseCurve());
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
          if (pPoints[i - 1].m_CloseFigure)
          {
            paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
            paths.emplace_back(BaseCurve());
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
          if (pPoints[i - 1].m_CloseFigure)
          {
            paths.back().setClosed(pPoints[i - 1].m_CloseFigure);
            paths.emplace_back(BaseCurve());
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
      if (pPoints.back().m_CloseFigure)
        paths.back().setClosed(pPoints.back().m_CloseFigure);
    }
    savePath(paths, pGraphState);
    return true;
  };

  void savePath(const std::vector<BaseCurve>& paths, const CFX_GraphStateData* pGraphState)
  {
    for (auto& fig : paths)
    {
      for (auto& seg : fig.getStorage())
      {
        if (FXPT_TYPE::LineTo == seg.second)
          m_pWrapper->savePolyline(seg.first);
        else
          m_pWrapper->saveSpline(seg.first);
      }
    }
  };
};

void PdfiumWrapperImpl::createGeometryCache(int nPage, const OdArray<OdUnderlayLayer> &Layers) const
{
  if (!m_pDocument)
    return;

  m_geometryCache.clear(nPage);
  setupLayers(Layers);
  double scale = getMeasureDictInfo(nPage);

  CPDF_Page* pPage = CPDFPageFromFPDFPage(m_Pages[nPage]);
  if (!pPage)
    return;

  double page_width = FPDF_GetPageWidth(m_Pages[nPage]);
  double page_height = FPDF_GetPageHeight(m_Pages[nPage]);

  CPDF_PageRenderContext* pContext = new CPDF_PageRenderContext;
  pPage->SetRenderContext(pdfium::WrapUnique(pContext));

  CFX_RenderDevice* pDevice = new CFX_RenderDevice;

  pDevice->SetDeviceDriver(pdfium::MakeUnique<geomCacheDriver>(page_width, -page_height, scale, this));
  pContext->m_pDevice.reset(pDevice);

  FPDF_RenderPage_Retail(pContext, m_Pages[nPage], 0, 0, page_width, -page_height, 0, 0, true, nullptr);

  pPage->SetRenderContext(nullptr);
}
#endif

void PdfiumWrapperImpl::savePolyline(const OdGePoint2dArray& pt_cache) const
{
  OdGeExtents2d objExt;
  OdInt32 count = pt_cache.size();
  OdGePoint3dArray points;
  points.resize(count);

  for (int i = 0; i < count; ++i)
  {
    points[i].set(pt_cache[i].x, pt_cache[i].y, 0.);
    objExt.addPoint(pt_cache[i]);
  }

  m_geometryCache.m_polylines.push_back(GeometryCache::Polyline(points, objExt));
}

void PdfiumWrapperImpl::saveSpline(const OdGePoint2dArray& pt_cache) const
{
  OdGePoint3dArray ctrlPts;
  ctrlPts.resize(pt_cache.size());
  OdGeExtents2d objExt;
  OdGeKnotVector knotsVec;
  OdGeDoubleArray knots;
  OdGeDoubleArray wts;

  for (OdUInt32 i = 0; i < pt_cache.size(); i++)
  {
    objExt.addPoint(pt_cache[i]);
    ctrlPts[i] = OdGePoint3d(pt_cache[i].x, pt_cache[i].y, 0);
  }

  /* weight values */
  //int nKnots = (noSeg - 1) * 3 + 1;
  wts.resize(pt_cache.size());
  for (OdUInt32 i = 0; i < pt_cache.size(); i++)
    wts[i] = 1.;

  /* Knot points */
  knots.resize(pt_cache.size() + 4);
  double kVal = 1.;
  int j = 0;
  for (j = 0; j < 4; j++)
    knots[j] = 0.;
  for (OdUInt32 i = 1; i < (pt_cache.size() - 1) / 3; i++)
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

  OdGeNurbCurve3d spline;
  spline.set(3, knotsVec, ctrlPts, wts, false);
  m_geometryCache.m_splines.push_back(std::pair<OdGeNurbCurve3d, OdGeExtents2d>(spline, objExt));
}
