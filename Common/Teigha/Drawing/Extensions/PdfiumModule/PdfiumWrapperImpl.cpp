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
#include "ColorMapping.h"

#include "PdfiumWrapperImpl.h"
#include "FlatMemStream.h"
#include "DynamicLinker.h"
#include "Ge/GeExtents2d.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiPathNode.h"
#include "GiDefaultContext.h"
#include "OdColorAdjuster.h"
#include "PdfUnderlay.h"

#include "public/fpdf_annot.h"
#include "public/fpdf_transformpage.h"
#include "fpdfsdk/fsdk_define.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/cpdf_reference.h"
#include "core/fpdfapi/page/cpdf_page.h"

#define PDF_UNDERLAY_CACHE
#ifdef PDF_UNDERLAY_CACHE
bool cacheUnderlay = true;
#else
bool cacheUnderlay = false;
#endif

#define TOCHAR(a) ((char *)OdAnsiString(OdString(a)).c_str())

// Registration
ODRX_CONS_DEFINE_MEMBERS(PdfiumWrapperImpl, PdfDirectWrapper, RXIMPL_CONSTR);

// PdfDirectWrapper
OdSmartPtr<PdfDirectWrapper> PdfDirectWrapper::createObjectImpl()
{
  PdfDirectWrapperPtr ptrtmp = PdfiumWrapperImpl::createObject();
  return ptrtmp;
}

void PdfDirectWrapper::rxInitWrapper()
{
  if (odrxClassDictionary()->has(L"OdDbPdfUnderlayItemPE")) {
    //Underlay module already initialized
    throw OdError(eAlreadyActive);
  }

  FPDF_InitLibrary();// Initialize the library.

  // it should be after the code with throw OdError
  PdfDirectWrapper::rxInit();
  PdfiumWrapperImpl::rxInit();
  OdDbPdfUnderlayItemPE::rxInit();
  OdDbUnderlayItem::desc()->addX(OdDbPdfUnderlayItemPE::desc(), &m_PdfItemPE);
}

void PdfDirectWrapper::rxUninitWrapper()
{
  FPDF_DestroyLibrary();

  OdDbUnderlayItem::desc()->delX(OdDbPdfUnderlayItemPE::desc());
  PdfiumWrapperImpl::rxUninit();
  PdfDirectWrapper::rxUninit();
  OdDbPdfUnderlayItemPE::rxUninit();
}

// PdfiumWrapperImpl
PdfiumWrapperImpl::PdfiumWrapperImpl()
  : m_pDocument(0)
  , m_pONLayers(0)
  , m_pOFFLayers(0)
  , m_pLayers(0)
  , m_bDrawAnnots(false)
{
  m_Deviation = 0;
  m_CountOfPages = 0;
  m_layerInfoCachePage = -1;
  desc()->module()->addRef();
}

PdfiumWrapperImpl::~PdfiumWrapperImpl()
{
  if (m_Pages.size() > 0)
  {
    for (OdUInt32 i = 0; i < m_Pages.size(); i++)
      FPDF_ClosePage(m_Pages[i]);
    m_Pages.clear();
  }
  if (m_pDocument)
  {
    FPDF_CloseDocument(m_pDocument);
    m_pDocument = 0;
  }
  m_cash.clear();
  desc()->module()->release();
}

bool PdfiumWrapperImpl::checkAnnotationsOnPage(OdUInt32 nPage) const
{
  OdArray<bool>::size_type annotationsSize = m_Annotations.size();
  if (annotationsSize <= nPage)
  {
    m_Annotations.resize(nPage+1, false);
    for (OdArray<bool>::size_type f = annotationsSize; f <= nPage; ++f)
      m_Annotations[f] = 0 < FPDFPage_GetAnnotCount(m_Pages[nPage]);
  }
  return m_Annotations[nPage];
}

OdResult PdfiumWrapperImpl::loadFile(OdString path, OdString password)
{
  m_path = path;
  m_password = password;

  if (m_Pages.size() > 0)
  {
    for (OdUInt32 i = 0; i < m_Pages.size(); i++)
      FPDF_ClosePage(m_Pages[i]);
    m_Pages.clear();
  }

  if (m_pDocument)
    FPDF_CloseDocument(m_pDocument);
  m_pDocument = FPDF_LoadDocument(TOCHAR(m_path.c_str()), TOCHAR(m_password.c_str()));
  if (m_pDocument == NULL)
  {
    unsigned long error = FPDF_GetLastError();
    if (FPDF_ERR_PASSWORD == error)
      return eInvalidPassword;
    return eCantOpenFile;
  }

  m_CountOfPages = FPDF_GetPageCount(m_pDocument);

  for (int i = 0; i < m_CountOfPages; i++)
    m_Pages.append(FPDF_LoadPage(m_pDocument, i));

  m_Extents.resize(m_CountOfPages, OdGePoint2d::kOrigin);

  //load layers
  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(m_pDocument);
  if (!pDoc)
    return eLoadFailed;
  CPDF_Dictionary* pRoot = pDoc->GetRoot(); //required
  if (!pRoot)
    return eLoadFailed;
  if (pRoot->KeyExist("OCProperties"))
  {
    m_pLayers = pRoot->GetDictFor("OCProperties")->GetArrayFor("OCGs"); //required
    if (!m_pLayers)
      return eLoadFailed;
    CPDF_Dictionary* pD = pRoot->GetDictFor("OCProperties")->GetDictFor("D"); //required
    if (!pD)
      return eLoadFailed;

    m_pONLayers = pD->GetArrayFor("ON");
    m_pOFFLayers = pD->GetArrayFor("OFF");
    if (!m_pOFFLayers || !m_pONLayers)
    {
      if (!m_pONLayers)
        m_pONLayers = pD->SetNewFor<CPDF_Array>("ON");
      if (!m_pOFFLayers)
        m_pOFFLayers = pD->SetNewFor<CPDF_Array>("OFF");
      ByteString baseSt = pD->GetStringFor("BaseState");
      if (baseSt.GetStringLength() == 0 || baseSt == "ON")
      {
        m_pONLayers->Clear();
        for (OdUInt32 i = 0; i < m_pLayers->GetCount(); i++)
        {
          if (m_pOFFLayers->end() == std::find_if(m_pOFFLayers->begin(), m_pOFFLayers->end(),
            [&](const std::unique_ptr<CPDF_Object>& candidate)
          {
            return candidate->GetDirect() == m_pLayers->GetObjectAt(i)->GetDirect();
          }))
          {
            m_pONLayers->Add(m_pLayers->GetObjectAt(i)->Clone());
          }
        }
      }
      else if (baseSt == "OFF")
      {
        m_pOFFLayers->Clear();
        for (OdUInt32 i = 0; i < m_pLayers->GetCount(); i++)
        {
          if (m_pONLayers->end() == std::find_if(m_pONLayers->begin(), m_pONLayers->end(),
            [&](const std::unique_ptr<CPDF_Object>& candidate)
          {
            return candidate->GetDirect() == m_pLayers->GetObjectAt(i)->GetDirect();
          }))
          {
            m_pOFFLayers->Add(m_pLayers->GetObjectAt(i)->Clone());
          }
        }
      }
    }
  }
  return eOk;
}

OdGiRasterImagePtr PdfiumWrapperImpl::getThumbnail(int nPage) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return OdGiRasterImagePtr();

  try
  {
    // 170 is ARX default
    int pic_width = 170, pic_height = 170;

    double page_width = FPDF_GetPageWidth(m_Pages[nPage]);
    double page_height = FPDF_GetPageHeight(m_Pages[nPage]);
    if (page_width >= page_height)
      pic_height *= page_height / page_width;
    else
      pic_width *= page_width / page_height;
    
    FPDF_BITMAP bitmap = FPDFBitmap_Create(pic_width, pic_height, 0);
    FPDFBitmap_FillRect(bitmap, 0, 0, pic_width, pic_height, 0xFFFFFFFF);
    FPDF_RenderPageBitmap(bitmap, m_Pages[nPage], 0, 0, pic_width, pic_height, 0, 0);

    BITMAPINFOHEADER bmiHeader;
    bmiHeader.biSize = 40;
    bmiHeader.biWidth = pic_width;
    bmiHeader.biHeight = -pic_height;  // top-down image
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 32;
    bmiHeader.biCompression = 0L; //BI_RGB;
    bmiHeader.biSizeImage = FPDFBitmap_GetStride(bitmap)*pic_height;
    unsigned char *imagedata = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);

    if (!imagedata)
      return OdGiRasterImagePtr();

    OdGiRasterImagePtr pTmp = convertBmpToRaster(bmiHeader, imagedata);

    FPDFBitmap_Destroy(bitmap);
    return pTmp;
  }
  catch(...) 
  {
  }
  return OdGiRasterImagePtr();
}


int PdfiumWrapperImpl::getItemsCount() const
{
  return m_CountOfPages;
}

OdString PdfiumWrapperImpl::getItemName(int nPage) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return OdString::kEmpty;

  return OdString().format(OD_T("%d"), nPage+1);
}

const OdGePoint2d &PdfiumWrapperImpl::getPageSize(int nPage) const
{
  OdGePoint2d &pCurExtents = m_Extents[nPage];

  if (pCurExtents.x == 0 && pCurExtents.y == 0 && nPage < m_CountOfPages && 0 != m_Pages[nPage])
  {
    pCurExtents.x = FPDF_GetPageWidth(m_Pages[nPage]);
    pCurExtents.y = FPDF_GetPageHeight(m_Pages[nPage]);
  }
  return pCurExtents;
}

OdResult PdfiumWrapperImpl::getMediaBox(int nPage, OdGePoint2d& minc, OdGePoint2d& maxc) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty() || 0 == m_Pages[nPage])
    return eInvalidInput;

  float min_x = 0, min_y = 0, max_x = 0, max_y = 0;
  int res = FPDFPage_GetMediaBox(m_Pages[nPage], &min_x, &min_y, &max_x, &max_y);
  if (!res)
    return eFileInternalErr;

  CPDF_Page* pPage = CPDFPageFromFPDFPage(m_Pages[nPage]);
  if (!pPage)
    return eFileInternalErr;
  int rotation = pPage->GetPageRotation();

  if (rotation == 1 /*90*/ || rotation == 3 /*270*/) 
  {
    minc.x = min_y;
    minc.y = min_x;
    maxc.x = max_y;
    maxc.y = max_x;
  }
  else
  {
    minc.x = min_x;
    minc.y = min_y;
    maxc.x = max_x;
    maxc.y = max_y;
  }
  return eOk;
}

void PdfiumWrapperImpl::getItemExtents(int nPage, OdGePoint2d& minc, OdGePoint2d& maxc) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return;
  const OdGePoint2d &pCurExtents = getPageSize(nPage);

  minc.set(0, 0);
  double xScale = getMeasureDictInfo(nPage);
  xScale *= 72.;
  maxc.set(pCurExtents.x / 72. * xScale, pCurExtents.y / 72. * xScale);
}

void PdfiumWrapperImpl::setupLayers(const OdArray<OdUnderlayLayer> &layers) const
{
  if (!m_pLayers || !m_pOFFLayers || !m_pONLayers)
    return;
  m_pONLayers->Clear();
  m_pOFFLayers->Clear();
  for (OdUInt32 f = 0; f < layers.size(); ++f)
  {
    OdAnsiString tmp( layers[f].name() );
    bool isOn = layers[f].state() == OdUnderlayLayer::kOn;

    if (tmp == "Comments and Markups")
      m_bDrawAnnots = isOn;
    else
    {
      for (OdUInt32 i = 0; i < m_pLayers->GetCount(); i++)
      {
        CPDF_Dictionary* pOCG = m_pLayers->GetObjectAt(i)->AsReference()->GetDirect()->AsDictionary();
        if (pOCG)
        {
          if (!OdString(pOCG->GetUnicodeTextFor("Name").c_str()).compare(layers[f].name()))
          {
            if (isOn)
              m_pONLayers->Add(m_pLayers->GetObjectAt(i)->Clone());
            else
              m_pOFFLayers->Add(m_pLayers->GetObjectAt(i)->Clone());
          }
        }
      }
    }
  }
}

//#define PDF_DEBUG
#ifdef PDF_DEBUG
#include "RxRasterServices.h"
#endif

OdResult method2(unsigned char *buf, unsigned int bufLen, OdInt32 width, OdInt32 height, ODCOLORREF backgroundColor, const OdDbUnderlayDrawContext& context)
{
  // init OdColorAdjuster
  bool bIsBackgroundLight = context.contrast >= 111;

  OdColorAdjuster dca(context.monochrome, bIsBackgroundLight ? context.contrast - 111 : context.contrast, context.fade, backgroundColor);
  bool isNeedColorRecompution = dca.isNeedColorRecompution();

  OdUInt32 counter = 0;
  OdUInt8 *pCurBGRA = buf;
  // convert colors (if necessary) but not alpha channel.
  while (counter < bufLen)
  {
    if (context.adjustColorForBackground)
    {
      if (bIsBackgroundLight && pCurBGRA[0] == 254 && pCurBGRA[1] == 254 && pCurBGRA[2] == 254)
      {
        pCurBGRA[0] = pCurBGRA[1] = pCurBGRA[2] = 255;
      }
      else if (!bIsBackgroundLight && pCurBGRA[0] == 1 && pCurBGRA[1] == 1 && pCurBGRA[2] == 1)
      {
        pCurBGRA[0] = pCurBGRA[1] = pCurBGRA[2] = 0;
      }
      else
      {
        if (!bIsBackgroundLight && pCurBGRA[0] == 0 && pCurBGRA[1] == 0 && pCurBGRA[2] == 0)
        {
          pCurBGRA[2] = 255;
          pCurBGRA[1] = 255;
          pCurBGRA[0] = 255;
        }
        else if (!bIsBackgroundLight && pCurBGRA[0] == 255 && pCurBGRA[1] == 255 && pCurBGRA[2] == 255)
        {
          pCurBGRA[2] = 0;
          pCurBGRA[1] = 0;
          pCurBGRA[0] = 0;
        }
      }
      if (isNeedColorRecompution)
        dca.computeShadedColorRGB(pCurBGRA[2], pCurBGRA[1], pCurBGRA[0]);
    }
    else
    {
      if (isNeedColorRecompution)
        dca.computeShadedColorRGB(pCurBGRA[2], pCurBGRA[1], pCurBGRA[0]);
    }
    pCurBGRA += 4;
    counter += 4;
  }
  return eOk;
}

void PdfiumWrapperImpl::viewportDraw(int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const
{
  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return;

  // it returns sizes in PDF dots
  const OdGePoint2d &pCurExtents = getPageSize(nPage);

  double measureScale = getMeasureDictInfo(nPage) * 72.;

  double maxXminX = pCurExtents.x * measureScale;
  double maxYminY = pCurExtents.y * measureScale;

  OdGePoint2d pixelDensity;
  pVd->viewport().getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, pixelDensity);
  double ds = pVd->geometry().getModelToWorldTransform().scale();

  double imageWidth = maxXminX;
  double imageHeight = maxYminY;

  // imageWidth, imageHeight are in screen dots
  imageWidth = imageWidth / 72. * pixelDensity.x * ds;
  imageHeight = imageHeight / 72. * pixelDensity.x * ds;

  //double DPI = imageWidth / (pCurExtents.x / 72.) + 0.5;
  //             ||
  //             \/
  // ((pCurExtents.x * measureScale) / 72.) * pixelDensity.x * ds
  //-------------------------------------------------------------      + 0.5
  //                    (pCurExtents.x / 72.)
  //             ||
  //             \/
  double DPI = pixelDensity.x * ds * measureScale + 0.5;

  OdGeExtents2d resUnCS;
  bool bIsVisible;

  double uScale = maxXminX / 72.;
  double vScale = maxYminY / 72.;

  try
  {
    method1(uScale, vScale, pVd, resUnCS, bIsVisible);
  }
  catch (OdError &/*err*/)
  { // This is occurs when camera looking exactly in plane of underlay 
    // draw nothing. 
    return;
  }

  // resUnCS is visible part of bitmap (0,0)-(1,1)
  if (bIsVisible)
  {
    // calculate the sizes of resulted bitmap
    int ImagePixW = (int)(imageWidth + 0.5);
    int ImagePixH = (int)(imageHeight + 0.5);

    if (ImagePixW > 0 && ImagePixH > 0)
    {
      // ImagePixW/ImagePixH must be equal to W/H of clipping area
      // Calculate extents of clipping area

      // The w and H of pdf page in 1/72 units
      double dW = maxXminX;
      double dH = maxYminY;

      // calculate the visible area of bitmap (in bitmap pixels)
      // btw : 1 bitmap pixels = 1 screen pixel
      double a = resUnCS.minPoint().x * dW;
      double b = resUnCS.minPoint().y * dH;
      double c = resUnCS.maxPoint().x * dW;
      double d = resUnCS.maxPoint().y * dH;

      double clipRect_a = (OdInt32)(a / measureScale) - 1;
      double clipRect_b = (OdInt32)(b / measureScale) - 1;
      double clipRect_c = (OdInt32)((c / measureScale) + 1.5);
      double clipRect_d = (OdInt32)((d / measureScale) + 1.5);

      if (clipRect_a < 0)
      {
        clipRect_a = 0;
      }
      if (clipRect_b < 0)
      {
        clipRect_b = 0;
      }
      if (clipRect_c > maxXminX / measureScale)
      {
        clipRect_c = (OdUInt32)(maxXminX / measureScale);
      }
      if (clipRect_d > maxYminY / measureScale)
      {
        clipRect_d = (OdUInt32)(maxYminY / measureScale);
      }

      OdGeExtents2d clipRect(OdGePoint2d(clipRect_a, clipRect_b), OdGePoint2d(clipRect_c, clipRect_d));

      if (clipRect_c - clipRect_a > 0 && clipRect_d - clipRect_b > 0.)
      {
        //////////////////////////////////////////////////////////////////////////
        OdDbObjectId obj;
        const OdGiPathNode* curr = pVd->currentGiPath();
        if (curr)
        {
          obj = curr->persistentDrawableId();
        }
        if (obj.isNull())
        {
          return;
        }
        ImageCache& pdfInfo = m_cash.getPdfInfo(pVd->viewport().viewportId(), obj);
        TD_AUTOLOCK(m_cash.m_lock);

        OdGePoint3d clippedOrigin;
        OdGeVector3d u;
        OdGeVector3d v;

        // fade/contract depends on background color
        ODCOLORREF backgroundColor = 0;
        OdGiDefaultContextPtr pCtx = OdGiDefaultContext::cast(pVd->context());
        if (pCtx.get())
        {
          backgroundColor = pCtx->paletteBackground();
        }
        else
        {
          ODA_ASSERT(0);
        }

        if (!pdfInfo.m_ext.isValidExtents() || !pdfInfo.m_ext.contains(clipRect) || (pdfInfo.m_dpi < DPI && DPI <= 3000.) || pdfInfo.m_dpi > 2 * DPI ||
          !pdfInfo.isUContextEqual(pdfInfo.m_drawCtx, context) || pdfInfo.m_nPage != nPage || pVd->context()->isPlotGeneration() ||
          pdfInfo.m_backgroundColor != backgroundColor || !cacheUnderlay) //need update
        {
          if (!pVd->context()->isPlotGeneration() && pdfInfo.m_ext.isValidExtents()/*in first render reserving area is not necessary*/)
          {
            clipRect = extendVisiblePart(clipRect);
          }

          if (pdfInfo.m_dpi < DPI && !pVd->context()->isPlotGeneration() && pdfInfo.m_dpi != 0.)
          {
            DPI *= 1.25;//when zooming in we need the higher DPI to avoid quality degrading.
            pdfInfo.m_dpi = DPI*1.25;//I guess it gives us more performance with less image quality degradation
          }
          else
            pdfInfo.m_dpi = DPI;//when zooming out or no zooming, DPI doesn't matter, so we don't increase it

          pdfInfo.m_ext = clipRect;
          pdfInfo.m_drawCtx = context;
          pdfInfo.m_nPage = nPage;

          setupLayers(context.layers);

          // Set some background color
          // This value is a random background color. But antialiasing effect depends of nDefColor whiteness.
          // kPDPageDoLazyErase is disabled - bitmap background clearing disabled.
          // Note that nDefColor can be black or white (0,0,0 or 255,255,255) only
          OdUInt8 nDefColor = 255;

          // at black background with adjustColorForBackground = true
          // artifacts possible
          //bool fullAntialiasing = true;

          // fill background with black or white to true antialiasing effect.
          // Note that PDF generates BGR bitmap (3 bytes, not BGRA).
          OdDbUnderlayDrawContext contextWithPaleteFlag = context;
          if (pCtx.get())
          {
            pdfInfo.m_backgroundColor = backgroundColor;
            if (odcmIsBackgroundLight(backgroundColor))
            {
              // code to send palette flag (black or white palette)
              contextWithPaleteFlag.contrast += 111;
            }
            else
            {
              nDefColor = 0;
            }

            // init nDefColor with 1 or 254 is a potential problem. All pixels with nDefColor color will be transparent.
            // Fix of this problem requires two renderings with different bg colors and creation of transparency mask.
            if (contextWithPaleteFlag.adjustColorForBackground)
            {
              if (odcmIsBackgroundLight(backgroundColor))
              {
                nDefColor = 254;
              }
              else
              {
                //fullAntialiasing = false;
                nDefColor = 1;
              }
            }
          }
          OdUInt32 bgcolor = nDefColor | nDefColor << 8 | nDefColor << 16 | 0x01 << 24;

          if (pVd->context()->isPlotGeneration())
          { //picture may occur too big because of hi dpi for plot, so we try to slice it to several pieces
            //calculate result picture size:
            OdUInt64 resW = (clipRect.maxPoint().x - clipRect.minPoint().x) * DPI / 72.;
            OdUInt64 resH = (clipRect.maxPoint().y - clipRect.minPoint().y) * DPI / 72.;
            OdUInt64 resSize = resH * resW * 4 /*3 byte per pixel + alpha channel*/;
            OdUInt32 numParts = (OdUInt32)((double)resSize / 90000000.) + 1; //90000000. - it is about the limit of picture size to be created
            if (numParts > 1)
            {
              return sliceImage(contextWithPaleteFlag, clipRect, pVd, numParts, nPage, backgroundColor, DPI);
            }
          }

          int flags = m_bDrawAnnots ? (FPDF_ANNOT | FPDF_RENDER_NO_SMOOTHIMAGE) : FPDF_RENDER_NO_SMOOTHIMAGE;

          double dpi_ratio = DPI / 72.;//default dpi is 72. for pdf files

          int pic_width = clipRect.maxPoint().x - clipRect.minPoint().x;
          int pic_height = clipRect.maxPoint().y - clipRect.minPoint().y;

          while (pic_width*dpi_ratio * pic_height*dpi_ratio > 22000000) //It gives us a bit less than 90000000 bytes for 4 bytes-for-pixel bitmap
            dpi_ratio *= .95;

          pic_width *= dpi_ratio;
          pic_height *= dpi_ratio;

          FPDF_BITMAP bitmap = FPDFBitmap_Create(pic_width, pic_height, 1);//FPDFBitmap_CreateEx(pic_width, pic_height, FPDFBitmap_BGR, 0, 0);
          FPDFBitmap_FillRect(bitmap, 0, 0, pic_width, pic_height, bgcolor);
          FPDF_RenderPageBitmap(bitmap, m_Pages[nPage],
            -clipRect.minPoint().x*dpi_ratio,
            clipRect.maxPoint().y*dpi_ratio - (int)pCurExtents.y*dpi_ratio /*start y-coordinate is inverted (top coordinate instead of down in OdGeExtents2d)*/,
            (int)pCurExtents.x*dpi_ratio,
            (int)pCurExtents.y*dpi_ratio, 0, flags);

          BITMAPINFOHEADER bmiHeader;
          bmiHeader.biSize = 40;
          bmiHeader.biWidth = pic_width;
          bmiHeader.biHeight = -pic_height;//top-down image
          bmiHeader.biPlanes = 1;
          bmiHeader.biBitCount = 32;
          bmiHeader.biCompression = 0L; //BI_RGB;
          bmiHeader.biSizeImage = FPDFBitmap_GetStride(bitmap)*pic_height;
          unsigned char *imagedata = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);

          if (!imagedata)
            return;

          // allocate 4 bytes per pixel (FLYSdk needs only 3) to future conversion from BGR to RGBA.
          OdUInt8Array buffer;
          buffer.resize(bmiHeader.biSizeImage, 255);

          memcpy(buffer.asArrayPtr(), imagedata, bmiHeader.biSizeImage);

#ifdef PDF_DEBUG
          {
            OdGiRasterImagePtr rast1;
            rast1 = convertBmpToRaster(bmiHeader, imagedata);
            OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
            if (!pRasSvcs.isNull())
              pRasSvcs->saveRasterImage(rast1, "f:\\pdfium1.bmp");// > loadRasterImage(pMem);
          }
#endif

          OdGiRasterImagePtr rast;
          OdResult res = method2(imagedata, bmiHeader.biSizeImage, Od_abs(bmiHeader.biWidth), Od_abs(bmiHeader.biHeight), backgroundColor, contextWithPaleteFlag);
          if (res == eOk)
          {
            rast = convertBmpToRaster(bmiHeader, imagedata);

            pdfInfo.m_pImage = rast;
            pdfInfo.m_biWidth = bmiHeader.biWidth;
            pdfInfo.m_biHeight = bmiHeader.biHeight;
            FPDFBitmap_Destroy(bitmap);

            if (pdfInfo.m_pImage.isNull())
              return;
          }
          clippedOrigin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * clipRect.minPoint().x / 72. + OdGeVector3d::kYAxis * clipRect.minPoint().y / 72.;
          u = OdGeVector3d((clipRect.maxPoint().x - clipRect.minPoint().x) / 72. / pdfInfo.m_biWidth, 0, 0);
          v = OdGeVector3d(0, (clipRect.maxPoint().y - clipRect.minPoint().y) / 72. / -pdfInfo.m_biHeight, 0);
        }
        else
        {
          clippedOrigin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * pdfInfo.m_ext.minPoint().x / 72. + OdGeVector3d::kYAxis * pdfInfo.m_ext.minPoint().y / 72.;
          u = OdGeVector3d((pdfInfo.m_ext.maxPoint().x - pdfInfo.m_ext.minPoint().x) / 72. / pdfInfo.m_biWidth, 0, 0);
          v = OdGeVector3d(0, (pdfInfo.m_ext.maxPoint().y - pdfInfo.m_ext.minPoint().y) / 72. / -pdfInfo.m_biHeight, 0);
        }

        u *= measureScale;
        v *= measureScale;
        clippedOrigin *= measureScale;

        pVd->geometry().rasterImageDc(clippedOrigin, u, v, pdfInfo.m_pImage, 0, 0, true);

#ifdef PDF_DEBUG
        {
          OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
          if (!pRasSvcs.isNull())
            pRasSvcs->saveRasterImage(pdfInfo.m_pImage, "f:\\pdfium2.bmp");// > loadRasterImage(pMem);
          OdGePoint3d pnts[2];
          pnts[0] = clippedOrigin;
          pnts[1] = clippedOrigin + u * pdfInfo.m_biWidth - v * pdfInfo.m_biHeight;
          pVd->geometry().polyline(2, pnts);
        }
#endif
      }
    }
  }
}
void PdfiumWrapperImpl::sliceImage(const OdDbUnderlayDrawContext& contextWithPaleteFlag, const OdGeExtents2d& clipRect, const OdGiViewportDraw* pVd,
  OdUInt32 numParts, int nPage, ODCOLORREF backgroundColor, double DPI) const
{
  DPI /= sqrt((double)numParts); //as in PDFVI - to repeat the autocad behavior
  int flags = m_bDrawAnnots ? (FPDF_ANNOT | FPDF_RENDER_NO_SMOOTHIMAGE) : FPDF_RENDER_NO_SMOOTHIMAGE;
  double dpi_ratio = DPI / 72.;
  double measureScale = getMeasureDictInfo(nPage) * 72.;
  const OdGePoint2d &pCurExtents = getPageSize(nPage);
  OdGePoint2d minP = clipRect.minPoint();
  OdGePoint2d maxP = clipRect.maxPoint();

  double step = (maxP.y - minP.y) / (double)numParts;

  maxP.y = minP.y + step;
  maxP.y = (OdInt32)(maxP.y + .5);
  OdGeExtents2d tmpClipRect(minP, maxP);
  OdGePoint3d origin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * tmpClipRect.minPoint().x / 72. + OdGeVector3d::kYAxis * tmpClipRect.minPoint().y / 72.;

  for (OdUInt32 counter = 1; counter <= numParts; counter++)
  {
    int pic_width = tmpClipRect.maxPoint().x - tmpClipRect.minPoint().x;
    int pic_height = tmpClipRect.maxPoint().y - tmpClipRect.minPoint().y;

    pic_width *= dpi_ratio;
    pic_height *= dpi_ratio;

    FPDF_BITMAP bitmap = FPDFBitmap_Create(pic_width, pic_height, 1);
    FPDFBitmap_FillRect(bitmap, 0, 0, pic_width, pic_height, backgroundColor);
    FPDF_RenderPageBitmap(bitmap, m_Pages[nPage], (int)(-tmpClipRect.minPoint().x*dpi_ratio),
      (int)(tmpClipRect.maxPoint().y*dpi_ratio) - (int)pCurExtents.y*dpi_ratio /*start y-coordinate is inverted (top coordinate instead of down in OdGeExtents2d)*/,
      (int)pCurExtents.x*dpi_ratio,
      (int)pCurExtents.y*dpi_ratio, 0, flags);

    BITMAPINFOHEADER bmiHeader;
    bmiHeader.biSize = 40;
    bmiHeader.biWidth = pic_width;
    bmiHeader.biHeight = -pic_height;  // top-down image
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 32;
    bmiHeader.biCompression = 0L; //BI_RGB;
    bmiHeader.biSizeImage = FPDFBitmap_GetStride(bitmap)*pic_height;
    unsigned char *imagedata = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);

    if (!imagedata)
      return;

    // allocate 4 bytes per pixel (FLYSdk needs only 3) to future conversion from BGR to RGBA.
    OdUInt8Array buffer;
    buffer.resize(Od_abs(bmiHeader.biWidth*bmiHeader.biHeight) * 4, 255);
    memcpy(buffer.asArrayPtr(), imagedata, bmiHeader.biSizeImage);

    OdGiRasterImagePtr pImage;
    OdResult res = method2(imagedata, bmiHeader.biSizeImage, Od_abs(bmiHeader.biWidth), Od_abs(bmiHeader.biHeight), backgroundColor, contextWithPaleteFlag);
    if (res == eOk)
    {
      pImage = convertBmpToRaster(bmiHeader, imagedata);

      FPDFBitmap_Destroy(bitmap);
      if (pImage.isNull())
        return;
    }
    OdGePoint3d clippedOrigin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * tmpClipRect.minPoint().x / 72. + OdGeVector3d::kYAxis * tmpClipRect.minPoint().y / 72.;
    OdGeVector3d u = OdGeVector3d((tmpClipRect.maxPoint().x - tmpClipRect.minPoint().x) / (72. * bmiHeader.biWidth), 0, 0);
    OdGeVector3d v = OdGeVector3d(0, (tmpClipRect.maxPoint().y - tmpClipRect.minPoint().y) / (72. * -bmiHeader.biHeight), 0);

    u *= measureScale;
    v *= measureScale;
    clippedOrigin *= measureScale;

    pVd->geometry().rasterImageDc(origin, u, v, pImage, 0, 0, true);
    origin.y += -bmiHeader.biHeight*v.y;

    minP.y = tmpClipRect.maxPoint().y;
    maxP.y += step;
    if (maxP.y > clipRect.maxPoint().y)
      maxP.y = clipRect.maxPoint().y;
    else
      maxP.y = (OdInt32)(maxP.y + .5);
    tmpClipRect.set(minP, maxP);
  }
}

int PdfiumWrapperImpl::getLevelsCount(int nPage) const
{
  if (m_path.isEmpty() || nPage < 0 || nPage >= PdfiumWrapperImpl::getItemsCount())
    return 0;

  if (nPage != m_layerInfoCachePage)
    getPageLayerInfo(nPage);

  int levelsCount = m_LayersCache.size();
  
  if (checkAnnotationsOnPage(nPage))
    levelsCount++;

  return levelsCount;
}

OdString PdfiumWrapperImpl::getLevelName(int nPage, int nLevel) const
{
  if (m_path.isEmpty() || nLevel < 0 || nLevel > PdfiumWrapperImpl::getLevelsCount(nPage))
    return OdString::kEmpty;

  if (checkAnnotationsOnPage(nPage) && nLevel+1 == PdfiumWrapperImpl::getLevelsCount(nPage) )
    return "Comments and Markups";

  if (!m_LayersCache.size())
    return "0";
  return m_LayersCache[nLevel];
}

OdUInt8 PdfiumWrapperImpl::isLevelEnabled(int nPage, int nLevel) const
{
  if (m_path.isEmpty() || nPage < 0 || nPage >= getItemsCount() || nLevel < 0 || nLevel > PdfiumWrapperImpl::getLevelsCount(nPage))
    return 0;

  if (nLevel+1 == PdfiumWrapperImpl::getLevelsCount(nPage) && checkAnnotationsOnPage(nPage))
    return 1;

  OdString name;
  if ((int)m_LayersCache.size() > nLevel)
    name = m_LayersCache[nLevel];

  if (m_pONLayers)
  {
    for (OdUInt32 i = 0; i < m_pONLayers->GetCount(); i++)
    {
      CPDF_Dictionary* pOCG_ON = m_pONLayers->GetObjectAt(i)->AsReference()->GetDirect()->AsDictionary();
      ODA_ASSERT(pOCG_ON);
      if (!pOCG_ON)
        return 0;
      OdString nameON(pOCG_ON->GetUnicodeTextFor("Name").c_str());
      if (!name.compare(nameON))
        return 1;
    }
  }
  return 0;
}

OdUInt8 PdfiumWrapperImpl::isLevelEnabled(int nPage, const OdString &layerName) const
{
  if (m_path.isEmpty() || nPage < 0 || nPage >= getItemsCount() || !PdfiumWrapperImpl::getLevelsCount(nPage))
    return 0;

  if (!layerName.compare(OdString(L"Comments and Markups")))
    return 1;

  if (m_pONLayers)
  {
    for (OdUInt32 i = 0; i < m_pONLayers->GetCount(); i++)
    {
      CPDF_Dictionary* pOCG_ON = m_pONLayers->GetObjectAt(i)->AsReference()->GetDirect()->AsDictionary();
      ODA_ASSERT(pOCG_ON);
      if (!pOCG_ON)
        return 0;
      OdString nameON(pOCG_ON->GetUnicodeTextFor("Name").c_str());
      if (!layerName.compare(nameON))
        return 1;
    }
  }
  return 0;
}

void PdfiumWrapperImpl::enableLevel(int nPage, int nLevel, OdUInt8 flag)
{
  throw OdError(eNotImplementedYet);
}

double PdfiumWrapperImpl::getMeasureDictInfo(int nPage) const
{
  if (nPage > m_CountOfPages || 0 == m_Pages[nPage])
    return 0;

  double xScale = 0.; 

  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(m_pDocument);
  if (pDoc)
  {
    CPDF_Dictionary* pPageDict = pDoc->GetPage(nPage);
    if (pPageDict)
    {
      CPDF_Array* pVPArr = pPageDict->GetArrayFor("VP");
      if (pVPArr && pVPArr->GetCount())
      {
        CPDF_Dictionary* pVP = pVPArr->GetDictAt(0);
        if (pVP)
        {
          CPDF_Dictionary* pMeasureDict = pVP->GetDictFor("Measure");
          if (pMeasureDict)
          {
            CPDF_Array* pXArr = pMeasureDict->GetArrayFor("X");
            if (pXArr && pXArr->GetCount() == 1) //see INT-763 for more info about pXArr->GetCount()
            {
              CPDF_Dictionary* pX = pXArr->GetDictAt(0);
              if (pX)
                xScale = pX->GetFloatFor("C");
            }
          }
        }
      }
    }
  }

  double measureScale = (xScale == 0.) ? 1./72. : (xScale);
  return measureScale;
}

OdResult PdfiumWrapperImpl::getExplodeGeometry(int nPage, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer> &Layers, bool bIgnoreImages) const
{
  return eNotImplementedYet;
}

OdResult PdfUnderlayModule::forInternalUseOnly(OdUInt32 n, void *p)
{
  return eNotImplemented;
}
//"PDFiumModule.tx"
