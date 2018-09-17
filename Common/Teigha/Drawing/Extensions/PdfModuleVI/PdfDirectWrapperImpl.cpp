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
#include "PdfDirectWrapperImpl.h"

#include "ColorMapping.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiPathNode.h"
#include "Gi/GiTextStyle.h"
#include "MemoryStream.h"
#include "OdPlatformStreamer.h"
#include "Ge/GeKnotVector.h"
#include "RxRasterServices.h"

#include "DbUnderlayDefinition.h"
#include "OdColorAdjuster.h"
#include "resource.h"
#include "PdfUnderlayItemImpl.h"
#include "VI_PdfKeyPEImpl.h"
#include "VgWrappers.h"

#ifdef _WIN32
#include "shellapi.h"
#endif

#define PDF_UNDERLAY_CACHE
#ifdef PDF_UNDERLAY_CACHE
bool cacheUnderlay = true;
#else
bool cacheUnderlay = false;
#endif

// Registration
ODRX_CONS_DEFINE_MEMBERS(PdfDirectWrapperImpl, PdfDirectWrapper, RXIMPL_CONSTR);

#ifdef _WIN32
namespace EVALUATION_DIALOG
{
  INT_PTR CALLBACK myProc2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
  {
    if(message == WM_CLOSE)
    {
      DestroyWindow(hwnd);
      return TRUE;
    }

    if(message == WM_INITDIALOG)
    { 
      // center the dialog
      HMONITOR activeMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
      
      MONITORINFO mi;
      mi.cbSize = sizeof(MONITORINFO);
      GetMonitorInfo(activeMonitor, &mi);

      RECT rc = mi.rcWork;

      RECT r2;
      GetWindowRect(hwnd, &r2);
      POINT pt;
      pt.x = (rc.right - rc.left)/2 - (r2.right - r2.left)/2;
      pt.y = (rc.bottom - rc.top)/2 - (r2.bottom - r2.top)/2;
      SetWindowPos(hwnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE);


      HFONT font = CreateFont(  
        16,               // height of font
        0,                // average character width
        0,           // angle of escapement
        0,          // base-line orientation angle
        1000,              // font weight
        0,           // italic attribute option
        1,        // underline attribute option
        0,        // strikeout attribute option
        0xCC,          // character set identifier
        0,  // output precision
        0,    // clipping precision
        0,          // output quality
        0,   // pitch and family
        L"MS Sans Serif"           // typeface name
        );

      LPARAM lParam = true;

      HWND item = GetDlgItem(hwnd, IDC_URL);
      HWND item2 = GetDlgItem(hwnd, IDC_URL2);
      HWND item3 = GetDlgItem(hwnd, IDC_URL3);

      SendMessage(item, (UINT) WM_SETFONT, (WPARAM)font,  lParam);  
      SendMessage(item2, (UINT) WM_SETFONT, (WPARAM)font,  lParam);  
      SendMessage(item3, (UINT) WM_SETFONT, (WPARAM)font,  lParam);

      return TRUE;
    }

    if(message ==  WM_COMMAND)
    {
      if(LOWORD(wParam) == 1 || LOWORD(wParam) == 2)
      {
        SendMessage(hwnd, WM_CLOSE, 0, 0);
        return TRUE;
      }
    }

    if(message == WM_LBUTTONUP && wParam == 0)
    {
      POINT p;
      p.x = LOWORD(lParam);
      p.y = HIWORD(lParam);

      HWND child = ChildWindowFromPoint(hwnd, p);
      if (child == GetDlgItem(hwnd, IDC_URL))
      {
        ShellExecute(NULL, L"open", L"http://www.opendesign.com/the_oda_platform/thirdpartyintegrations/pdfintegration", NULL,NULL, SW_SHOW);
        return TRUE;
      }
      if (child == GetDlgItem(hwnd, IDC_URL2))
      {
        ShellExecute(NULL, L"open", L"http://visual-integrity.com/?p=3702", NULL,NULL, SW_SHOW);
        return TRUE;
      }
      if (child == GetDlgItem(hwnd, IDC_URL3))
      {
        ShellExecute(NULL, L"open", L"http://www.visual-integrity.com", NULL,NULL, SW_SHOW);
        return TRUE;
      }
    }

    if(message ==  WM_CTLCOLORSTATIC)
    {
      // Set the colour of the text for our URL
      if ((HWND)lParam == GetDlgItem(hwnd, IDC_URL) || 
          (HWND)lParam == GetDlgItem(hwnd, IDC_URL2) ||
          (HWND)lParam == GetDlgItem(hwnd, IDC_URL3)) 
      {
        // we're about to draw the static
        // set the text colour in (HDC)lParam
        SetBkMode((HDC)wParam,TRANSPARENT);
        SetTextColor((HDC)wParam, RGB(0,0,255));
        return (INT_PTR)CreateSolidBrush (GetSysColor(COLOR_MENU));
      }
    }

    return false; 
  }

  void showEvaluationDialog()
  {
    MSG msg;
    OdString viModule = OdPdfModuleVIModuleName;
    viModule = viModule + TD_DLL_VERSION_SUFFIX_STR + ".tx";
    HMODULE thisModule = GetModuleHandle(viModule.c_str());
    HWND dialog = CreateDialog(thisModule,  MAKEINTRESOURCE(IDD_PDFNOTIFICATION), 0, myProc2);
    OdString  sCopyright = TD_COPYRIGHT_S;
    
    sCopyright += ". All Rights Reserved.\r\nThis software may not be licensed, sold, distributed or included with\r\nother software products without the written consent of Open Design Alliance.";
    SetDlgItemText(dialog, IDC_INF_COPYRIGHT, sCopyright);

    while(IsWindow( dialog ) && GetMessage(&msg,NULL,0,0))  {
      if (!IsDialogMessage(dialog, &msg ))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg); 
      }
    } 
  }
}
#endif

// PdfDirectWrapper

OdSmartPtr<PdfDirectWrapper> PdfDirectWrapper::createObjectImpl()
{
  PdfDirectWrapperPtr ptrtmp = PdfDirectWrapperImpl::createObject();
  return ptrtmp;
}

static inline OdVI_PdfKeyPEImpl * getKey()
{
  return (OdVI_PdfKeyPEImpl*)(OdDbPdfDefinition::desc()->getX(OdPdfKeyPE::desc()).get());
}

static inline bool security(OdVI_PdfKeyPEImpl *pKey)
{
  return true;//pKey != 0;
}

void PdfDirectWrapper::rxInitWrapper()
{
  if (odrxClassDictionary()->has(L"OdDbPdfUnderlayItemPE")) {
    //Underlay module already initialized
    throw OdError(eAlreadyActive);
  }

  OdVI_PdfKeyPEImpl *pKey = getKey();
  if (security(pKey))
  {

//#ifndef OD_NO_GETENV
//    OdAnsiString FlySdkFontPath = getenv("FLYSDKFONTS");
// Please note that FLYSDKFONTS correct path is
// /_Oda/trunk_alexr/bin/macOsX_x86_10.5dlldbg/fonts
// but correct path for PDFFLY is
// /_Oda/trunk_alexr/bin/macOsX_x86_10.5dlldbg
// difference is in '/fonts'.

#ifndef _WIN32
    OdAnsiString FlySdkFontPath = getenv("FLYSDKFONTS");
    // Only for Linux
    VGSetFontPath(FlySdkFontPath.isEmpty() ? NULL : (char *)FlySdkFontPath.c_str());
    int ret = VGInit();
    if (ret != 0) {
//  Under Linux and Mac the next system variables must be defined
//  and macOsX_x86_10.5dlldbg/fonts must available
//  PDFFLY=/_Oda/trunk_alexr/bin/macOsX_x86_10.5dlldbg; export PDFFLY
//  PATH=$PDFFLY:$PATH; export PATH

      throw OdError(eLibIntegrityBroken);
    }
#else
    OdString FlySdkFontPath = _wgetenv(L"FLYSDKFONTS");
    VGSetFontPath(TOCHAR(FlySdkFontPath.isEmpty() ? L"" : FlySdkFontPath.c_str()));
    int ret = VGInit(0);
    if (ret != 0) {
      throw OdError(eLibIntegrityBroken);
    }
#endif

    if (pKey && pKey->method0() != 0)
    {
      VGPsSetLicense( pKey->method0() );
    } 
    else
    {
      #ifdef _WIN32
        EVALUATION_DIALOG::showEvaluationDialog();
      #endif
    }
  }

  // it should be after the code with throw OdError


  PdfDirectWrapper::rxInit();
  PdfDirectWrapperImpl::rxInit();
  OdDbPdfUnderlayItemPE::rxInit();
  OdDbUnderlayItem::desc()->addX(OdDbPdfUnderlayItemPE::desc(),   &m_PdfItemPE);
}

void PdfDirectWrapper::rxUninitWrapper()
{
  OdDbUnderlayItem::desc()->delX(OdDbPdfUnderlayItemPE::desc());

  PdfDirectWrapperImpl::rxUninit();
  PdfDirectWrapper::rxUninit();
  OdDbPdfUnderlayItemPE::rxUninit();
}

// PdfDirectWrapperImpl

PdfDirectWrapperImpl::PdfDirectWrapperImpl()
{
  m_Deviation = 0;
  m_CountOfPages = 0;
  m_layerInfoCachePage = -1;
  desc()->module()->addRef();
}

PdfDirectWrapperImpl::~PdfDirectWrapperImpl()
{
  m_cash.clear();
  desc()->module()->release();
}

bool PdfDirectWrapperImpl::checkAnnotationsOnPage(OdUInt32 nPage) const
{
  OdArray<bool>::size_type annotationsSize = m_Annotations.size();
  if (annotationsSize <= nPage)
  {
    m_Annotations.resize(nPage+1, false);
    for (OdArray<bool>::size_type f=annotationsSize; f<=nPage; ++f)
    {
      m_Annotations[f] = VGPdfCheckAnnotationsOnPage(TOCHAR(m_path.c_str()), nPage+1) == /*TRUE*/1;
    }
  }
  return m_Annotations[nPage];
}

const VgPsLayerInfo &PdfDirectWrapperImpl::getVgPsLayerInfo(int nPage) const
{
  if (nPage != m_layerInfoCachePage)
  {
    m_layerInfoCachePage = nPage;
    VGPdfGetLayerInfoOnPage(TOCHAR(m_path.c_str()), nPage+1, m_layerInfoCache);
  }

  return m_layerInfoCache;
}

OdResult PdfDirectWrapperImpl::loadFile(OdString path, OdString password)
{
  if (!security(getKey()))
  {
    return eKeyNotFound;
  }
  m_path = path;
  m_password = password;

  m_CountOfPages = VGPdfNoPages(TOCHAR(m_path.c_str()));

  m_Extents.resize(m_CountOfPages, OdGePoint2d::kOrigin);
  return eOk;
}

OdGiRasterImagePtr PdfDirectWrapperImpl::getThumbnail(int nPage) const
{
  if (!security(getKey()))
  {
    return OdGiRasterImagePtr();
  }

  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return OdGiRasterImagePtr();

  try
  {
    VGSetImageScale(1);

    // 170 is ARX default
    const static OdUInt8 ThumbnailSize = 170;
    VGSetImagePixWorH(ThumbnailSize, ThumbnailSize);

    BITMAPINFOHEADER bmi;
    unsigned char *imagedata = 0;
    VGPdfConvertPage2DIB(TOCHAR(m_path.c_str()), nPage+1, 0, &bmi, &imagedata, 0);
    VGSetImageScale(0);

    if (!imagedata || bmi.biSize != 40)
    {
      return OdGiRasterImagePtr();
    }

    OdGiRasterImagePtr pTmp = convertBmpToRaster(bmi, imagedata);

    VGPdfDeleteImageDIB(imagedata);

    return pTmp;
  }
  catch(...) 
  {
  
  }

  return OdGiRasterImagePtr();
}


int PdfDirectWrapperImpl::getItemsCount() const
{
  if (!security(getKey()))
  {
    return 0;
  }

  return m_CountOfPages;
}

OdString PdfDirectWrapperImpl::getItemName(int nPage) const
{
  if (!security(getKey()))
  {
    return OdString::kEmpty;
  }

  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return OdString::kEmpty;

  return OdString().format(OD_T("%d"), nPage+1);
}

const OdGePoint2d &PdfDirectWrapperImpl::getPageSize(int nPage) const
{
  OdGePoint2d &pCurExtents = m_Extents[nPage];

  if (pCurExtents.x == 0 && pCurExtents.y == 0)
  {
    double extent[4];
    VGPdfGetPageSize(TOCHAR(m_path.c_str()), nPage+1, extent, 1);
    pCurExtents.set((extent[2] - extent[0]), (extent[3] - extent[1]));
  }

  return pCurExtents;
}

OdResult PdfDirectWrapperImpl::getMediaBox(int nPage, OdGePoint2d& minc, OdGePoint2d& maxc) const
{
  if (!security(getKey()))
  {
    return eKeyNotFound;
  }

  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return eInvalidInput;

  double extent[4] = {0,0,0,0};
  VGPdfGetPageSize(TOCHAR(m_path.c_str()), nPage+1, extent, 1);

  minc.set(extent[0], extent[1]);
  maxc.set(extent[2], extent[3]);

  return eOk;
}

void PdfDirectWrapperImpl::getItemExtents(int nPage, OdGePoint2d& minc, OdGePoint2d& maxc) const
{
  if (!security(getKey()))
  {
    return;
  }

  if (nPage >= getItemsCount() || nPage < 0 || m_path.isEmpty())
    return;
  const OdGePoint2d &pCurExtents = getPageSize(nPage);

  minc.set(0, 0);
  double xScale = getMeasureDictInfo(nPage);
  xScale *= 72.;
  maxc.set(pCurExtents.x / 72. * xScale, pCurExtents.y / 72. * xScale);
}

static void setupLayers(VgPsLayerInfo &layerInfo, const OdArray<OdUnderlayLayer> &layers)
{
  layerInfo.nol = 0;

  for(OdUInt32 f=0; f<layers.size(); ++f)
  {
    OdAnsiString tmp( layers[f].name() );
    bool isOn = layers[f].state() == OdUnderlayLayer::kOn;

    if (tmp == "Comments and Markups")
    {
      VGSetRenderAnnotations( isOn ? /*TRUE*/1 : /*FALSE*/0 );
    }
    else if (isOn)
    {
      if(tmp.getLength() < 512) // restriction of VgPsLayerInfo structure
      {
        strcpy(layerInfo.names[layerInfo.nol++], tmp.c_str());
        if(layerInfo.nol >= 128) // restriction of VgPsLayerInfo structure
          break;
      }
    }
  }
}

OdResult method2(OdUInt8Array &buffer,
  OdInt32 width,
  OdInt32 height,
  ODCOLORREF backgroundColor,
  OdGiRasterImagePtr &pRast,
  const OdDbUnderlayDrawContext& context)
{
  // init OdColorAdjuster
  bool bIsBackgroundLight = context.contrast >= 111;

  OdColorAdjuster dca(context.monochrome, bIsBackgroundLight ? context.contrast - 111 : context.contrast, context.fade, backgroundColor);
  bool isNeedColorRecompution = dca.isNeedColorRecompution();

  int bytesPerLine = width * 4;

  OdStreamBufPtr pMem = OdMemoryStream::createNew();

  OdPlatformStreamer::wrInt16(*pMem, 0x4d42);
  OdInt32 scanLinesPos = 54;
  OdInt32 size = scanLinesPos + bytesPerLine*height;
  OdPlatformStreamer::wrInt32(*pMem, size);
  OdPlatformStreamer::wrInt32(*pMem, 0); // reserved
  OdPlatformStreamer::wrInt32(*pMem, scanLinesPos); // offBits
  OdPlatformStreamer::wrInt32(*pMem, 40);
  // save BITMAPINFOHEADER
  OdPlatformStreamer::wrInt32(*pMem, width);
  OdPlatformStreamer::wrInt32(*pMem, height);
  OdPlatformStreamer::wrInt16(*pMem, 8);
  OdPlatformStreamer::wrInt16(*pMem, 32);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);
  OdPlatformStreamer::wrInt32(*pMem, 0);

  // Number of additional bytes at line end in the original RGB bitmap.
  int bytesAtEnd = (4 - ((width * 3) % 4)) % 4;

  OdUInt8Array::const_iterator pE = buffer.begin();
  OdUInt8Array::const_iterator pB = pE + (width * 3 + bytesAtEnd) * height - bytesAtEnd - 3;

  OdUInt8 *pCurBGRA = buffer.begin() + width * height * 4 - 4;

  // convert RGB to BGRA format.
  for (; pB >= pE; pB -= bytesAtEnd) // bitmap lines are 32 bits aligned. Skip it.
  {
    for (OdUInt32 p = width; p > 0; --p, pB -= 3)
    {
      ODA_ASSERT(pCurBGRA >= pE);
      // alfa component
      if (context.adjustColorForBackground)
      {
        if (bIsBackgroundLight && pB[0] == 254 && pB[1] == 254 && pB[2] == 254)
        {
#ifdef PDF_DEBUG
          pCurBGRA[0] = pCurBGRA[1] = pCurBGRA[2] = 10;
          pCurBGRA[3] = 50; // 100% transparent
#else
          pCurBGRA[0] = pCurBGRA[1] = pCurBGRA[2] = 255;
          pCurBGRA[3] = 0; // 100% transparent
#endif
        }
        else if (!bIsBackgroundLight && pB[0] == 1 && pB[1] == 1 && pB[2] == 1)
        {
          pCurBGRA[0] = pCurBGRA[1] = pCurBGRA[2] = 0;
          pCurBGRA[3] = 0; // 100% transparent
        }
        else
        {
          pCurBGRA[2] = pB[2];
          pCurBGRA[1] = pB[1];
          pCurBGRA[0] = pB[0];
          pCurBGRA[3] = 255; // not transparent

          if (bIsBackgroundLight && pB[0] == 255 && pB[1] == 255 && pB[2] == 255)
          {
            pCurBGRA[3] = 0; // 100% transparent
          }
          else if (!bIsBackgroundLight && pB[0] == 0 && pB[1] == 0 && pB[2] == 0)
          {
            pCurBGRA[2] = 255;
            pCurBGRA[1] = 255;
            pCurBGRA[0] = 255;
            pCurBGRA[3] = 255; // not transparent
          }
          else if (!bIsBackgroundLight && pB[0] == 255 && pB[1] == 255 && pB[2] == 255)
          {
            pCurBGRA[2] = 0;
            pCurBGRA[1] = 0;
            pCurBGRA[0] = 0;
            pCurBGRA[3] = 0; // 100% transparent
          }
        }

        if (isNeedColorRecompution)
          dca.computeShadedColorRGB(pCurBGRA[2], pCurBGRA[1], pCurBGRA[0]);
      }
      else
      {
        pCurBGRA[2] = pB[2];
        pCurBGRA[1] = pB[1];
        pCurBGRA[0] = pB[0];
        pCurBGRA[3] = 255; // not transparent

        if (pB[0] == 255 && pB[1] == 255 && pB[2] == 255)
        {
          pCurBGRA[3] = 0; // 100% transparent
        }

        if (isNeedColorRecompution)
          dca.computeShadedColorRGB(pCurBGRA[2], pCurBGRA[1], pCurBGRA[0]);
      }

      pCurBGRA -= 4;
    }
  }

  ODA_ASSERT(pCurBGRA + 4 == pE);
  ODA_ASSERT(pB + bytesAtEnd + 3 == pE);

  pMem->putBytes(buffer.getPtr(), buffer.size());

  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  if (!pRasSvcs.isNull())
  {
    pMem->rewind();

    //OdStreamBufPtr pF = odrxSystemServices()->createFile("f:\\pdfvi1.bmp", Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
    //pMem->copyDataTo(pF);
    //pMem->rewind();

    pRast = pRasSvcs->loadRasterImage(pMem);
  }

  return eOk;
}

//#define PDF_DEBUG

void PdfDirectWrapperImpl::viewportDraw(int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const
{
  if (!security(getKey()))
  {
    return;
  }

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

    //imageWidth = ImagePixW;
    //imageHeight= ImagePixH;


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
          //////////////////////////////////////////////////////////////////////////
          // Turn off parsing/handling of images.
          // Every call of VgPdfConvertPage2Custom() may create image file. 
          // Image width/height are the same as original size and may be very big (eg. 6000x4000 pixels). 
          VGIgnoreImages(true);

          if (!pVd->context()->isPlotGeneration() && pdfInfo.m_ext.isValidExtents()/*in first render reserving area is not necessary*/)
          {
            clipRect = extendVisiblePart(clipRect);
          }
          VGSetImageClipRect(clipRect.minPoint().x, clipRect.minPoint().y, clipRect.maxPoint().x, clipRect.maxPoint().y);

          VGSetImageScale(0);

          // Image is created ridiculously slow if you zoom in too much.
          // The bigger the zoom the slowly generation of image in VI lib. 
          // Although result image size is not big.
          // The most time the app spends in VgPdfConvertPage2DIB() creating small clipped image.
          // We have to reduce image quality when zoom of PDF is too big. 
          // 3000 dpi correspond to quality based on previous Adobe libs.
          const double maxDPI = 3000.;

          if (pdfInfo.m_dpi < DPI && DPI <= maxDPI && !pVd->context()->isPlotGeneration() && pdfInfo.m_dpi != 0.)
          {
            DPI *= 1.25;//when zooming in we need the higher DPI to avoid quality degrading.
            pdfInfo.m_dpi = DPI*1.25;//I guess it gives us more performance with less image quality degrading
          }
          else
            pdfInfo.m_dpi = DPI;//when zooming out or no zooming, DPI doesn't matter, so we don't increase it

          pdfInfo.m_ext = clipRect;
          pdfInfo.m_drawCtx = context;
          pdfInfo.m_nPage = nPage;

          //VGSetDPI(DPI);
          VGSetColorMode(0); // CORE-11424: #define IMGCUSTOM 0 #define IMGNAIVE 1

          VGSetImagePixWorH(ImagePixW, ImagePixH);

          AutoVgPsLayerInfo layerInfo;
          setupLayers(layerInfo, context.layers);

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

          int bgcolor[4] = { nDefColor, nDefColor, nDefColor, 0 };
          //int bgcolor[4] = {111, 0, 0, 0};
          VGSetImageBgColor(bgcolor);

          if (pVd->context()->isPlotGeneration())
          { //picture may occur too big because of hi dpi for plot, so we try to slice it to several pieces
            //calculate result picture size:
            OdUInt64 resW = (clipRect.maxPoint().x - clipRect.minPoint().x) * DPI / 72.;
            OdUInt64 resH = (clipRect.maxPoint().y - clipRect.minPoint().y) * DPI / 72.;
            OdUInt64 resSize = resH * resW * 3 /*3 byte per pixel*/;
            OdUInt32 numParts = (OdUInt32)((double)resSize / 90000000.) + 1; //90000000. - it is about the limit of picture size to be created
            if (numParts > 1)
            {
              VGSetDPI(DPI / sqrt((double)numParts));
              return sliceImage(layerInfo, contextWithPaleteFlag, clipRect, pVd, numParts, nPage, backgroundColor);
            }
          }
          if (!pVd->context()->isPlotGeneration())
          {
            if (DPI > maxDPI)
              DPI = maxDPI;
            if (pdfInfo.m_dpi > maxDPI)
              pdfInfo.m_dpi = maxDPI;
          }
          VGSetDPI(DPI);

          BITMAPINFOHEADER bmi;
          unsigned char *imagedata = 0;
          if (layerInfo.info().nol == 0)
            VGPdfConvertPage2DIB(TOCHAR(m_path.c_str()), nPage + 1, 0, &bmi, &imagedata, 0);
          else
            VGPdfConvertPage2DIB(TOCHAR(m_path.c_str()), nPage + 1, layerInfo, &bmi, &imagedata, 0);

          //VgSetImageClipRect should be disabled or it will affect to preview in insert underlay
          VGSetImageClipRect(0., 0., 0., 0.);

          if (!imagedata || bmi.biSize != 40)
          {
            return;
          }

          // allocate 4 bytes per pixel (FLYSdk needs only 3) to future conversion from BGR to RGBA.
          OdUInt8Array buffer;
          buffer.resize(Od_abs(bmi.biWidth*bmi.biHeight) * 4, 255);
          memcpy(buffer.asArrayPtr(), imagedata, bmi.biSizeImage);
#ifdef PDF_DEBUG
          {
            OdGiRasterImagePtr rast1;
            rast1 = convertBmpToRaster(bmi, imagedata);
            OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
            if (!pRasSvcs.isNull())
              pRasSvcs->saveRasterImage(rast1, "f:\\pdfvi1.bmp");// > loadRasterImage(pMem);
          }
#endif

          OdGiRasterImagePtr rast;
          OdResult res = method2(buffer, Od_abs(bmi.biWidth), Od_abs(bmi.biHeight), backgroundColor, rast, contextWithPaleteFlag);
          if (res == eOk && !rast.isNull())
          {
            bmi.biBitCount = 32;
            bmi.biSizeImage = Od_abs(bmi.biWidth*bmi.biHeight) * 4;
            pdfInfo.m_pImage = convertBmpToRaster(bmi, buffer.asArrayPtr());
            pdfInfo.m_biWidth = bmi.biWidth;
            pdfInfo.m_biHeight = bmi.biHeight;

            VGPdfDeleteImageDIB(imagedata);

            if (pdfInfo.m_pImage.isNull())
            {
              return;
            }
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

        u *= measureScale /** ds*/; //ds coeff affects rendering
        v *= measureScale /** ds*/;
        clippedOrigin *= measureScale /** ds*/;

        pVd->geometry().rasterImageDc(clippedOrigin, u, v, pdfInfo.m_pImage, 0, 0, true);

#ifdef PDF_DEBUG
        {
          OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
          if (!pRasSvcs.isNull())
            pRasSvcs->saveRasterImage(pdfInfo.m_pImage, "f:\\pdfvi2.bmp");

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

void PdfDirectWrapperImpl::sliceImage(VgPsLayerInfo& layerInfo, const OdDbUnderlayDrawContext& contextWithPaleteFlag, const OdGeExtents2d& clipRect, const OdGiViewportDraw* pVd,
  OdUInt32 numParts, int nPage, ODCOLORREF backgroundColor) const
{
  double measureScale = getMeasureDictInfo(nPage) * 72.;
  OdGePoint2d minP = clipRect.minPoint();
  OdGePoint2d maxP = clipRect.maxPoint();

  double step = (maxP.y - minP.y) / (double)numParts;

  maxP.y = minP.y + step;
  maxP.y = (OdInt32)(maxP.y + .5);
  OdGeExtents2d tmpClipRect(minP, maxP);
  OdGePoint3d origin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * tmpClipRect.minPoint().x / 72. + OdGeVector3d::kYAxis * tmpClipRect.minPoint().y / 72.;

  for (OdUInt32 counter = 1; counter <= numParts; counter++)
  {
    VGSetImageClipRect(tmpClipRect.minPoint().x, tmpClipRect.minPoint().y, tmpClipRect.maxPoint().x, tmpClipRect.maxPoint().y);

    BITMAPINFOHEADER bmi;
    unsigned char *imagedata = 0;
    if (layerInfo.nol == 0)
      VGPdfConvertPage2DIB(TOCHAR(m_path.c_str()), nPage + 1, 0, &bmi, &imagedata, 0);
    else
      VGPdfConvertPage2DIB(TOCHAR(m_path.c_str()), nPage + 1, &layerInfo, &bmi, &imagedata, 0);

    //VgSetImageClipRect should be disabled or it will affect to preview in insert underlay
    VGSetImageClipRect(0., 0., 0., 0.);

    if (!imagedata || bmi.biSize != 40)
    {
      return;
    }

    // allocate 4 bytes per pixel (FLYSdk needs only 3) to future conversion from BGR to RGBA.
    OdUInt8Array buffer;
    buffer.resize(Od_abs(bmi.biWidth*bmi.biHeight) * 4, 255);
    memcpy(buffer.asArrayPtr(), imagedata, bmi.biSizeImage);

    OdGiRasterImagePtr rast, pImage;
    OdResult res = method2(buffer, Od_abs(bmi.biWidth), Od_abs(bmi.biHeight), backgroundColor, rast, contextWithPaleteFlag);
    if (res == eOk && !rast.isNull())
    {
      bmi.biBitCount = 32;
      bmi.biSizeImage = Od_abs(bmi.biWidth*bmi.biHeight) * 4;
      pImage = convertBmpToRaster(bmi, buffer.asArrayPtr());

      VGPdfDeleteImageDIB(imagedata);

      if (pImage.isNull())
        return;
    }

    OdGePoint3d clippedOrigin = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis * tmpClipRect.minPoint().x / 72. + OdGeVector3d::kYAxis * tmpClipRect.minPoint().y / 72.;
    OdGeVector3d u = OdGeVector3d((tmpClipRect.maxPoint().x - tmpClipRect.minPoint().x) / (72. * bmi.biWidth), 0, 0);
    OdGeVector3d v = OdGeVector3d(0, (tmpClipRect.maxPoint().y - tmpClipRect.minPoint().y) / (72. * -bmi.biHeight), 0);

    u *= measureScale;
    v *= measureScale;
    clippedOrigin *= measureScale;

    pVd->geometry().rasterImageDc(origin, u, v, pImage, 0, 0, true);
    origin.y += -bmi.biHeight*v.y;

    minP.y = tmpClipRect.maxPoint().y;
    maxP.y += step;
    if (maxP.y > clipRect.maxPoint().y)
      maxP.y = clipRect.maxPoint().y;
    else
      maxP.y = (OdInt32)(maxP.y + .5);
    tmpClipRect.set(minP, maxP);
  }
}

void PdfDirectWrapperImpl::createGeometryCache(int nPage, const OdArray<OdUnderlayLayer> &Layers) const
{
  if (!security(getKey()))
    return;

  m_geometryCache.clear(nPage);

  AutoVgPsLayerInfo layerInfo;
  setupLayers(layerInfo, Layers);

  VGSetInsertMode(0); //TGC_ASPAGE == 0
  VGSetAutomaticPageSize();

  // Turn off parsing/handling of images.
  // Every call of VgPdfConvertPage2Custom() may create image file. 
  // Image width/height are the same as original size and may be very big (eg. 6000x4000 pixels). 
  VGIgnoreImages(true); 

  //Ignore texts. We shouldn't snap to them or select the underlay by clicking on them, anyway
  //so let's get rid of them for the sake of performance (CORE-12402)
  VGIgnoreText(true);

  ConversionData data(m_geometryCache, getMeasureDictInfo(nPage) * 72.);
  VGPdfConvertPage2Custom(TOCHAR(m_path.c_str()), saveGeometry, nPage + 1, layerInfo, &data);
}

inline OdGePoint2d point2d(const CuPoint& pt, const double& scale)
{
  return OdGePoint2d(pt.x*scale, pt.y*scale);
}

inline OdGePoint3d point3d(const CuPoint& pt, const double& scale)
{
  return OdGePoint3d(pt.x*scale, pt.y*scale, 0.);
}

void PdfDirectWrapperImpl::savePolyline(CU_OBJECT_DATA* objdata, const double& dScale, 
                                        GeometryCache::Polylines& polylines)
{
  OdGeExtents2d objExt;
  OdInt32 count = objdata->irpoly.pt_count;
  OdGePoint3dArray points;
  points.resize(count);
  CuPoint* pt = objdata->irpoly.pt_h;
  for (int i = 0; i < count; ++i)
  {
    OdGePoint2d pt2d(point2d(pt[i], dScale));
    points[i].set(pt2d.x, pt2d.y, 0.);
    objExt.addPoint(pt2d);
  }

  polylines.push_back(GeometryCache::Polyline(points, objExt));
}

void PdfDirectWrapperImpl::saveLine(CU_OBJECT_DATA* objdata, const double& dScale, 
                                    GeometryCache::Polylines& polylines)
{
  CuPoint* pt = objdata->irpoly.pt_h;
  OdGePoint3dArray pnts;
  pnts.append(point3d(pt[0], dScale));
  pnts.append(point3d(pt[1], dScale));

  OdGeExtents2d objExt;
  objExt.addPoint(pnts[0].convert2d());
  objExt.addPoint(pnts[1].convert2d());

  polylines.push_back(GeometryCache::Polyline(pnts, objExt));
  //lines.push_back(GeometryCache::Line(point2d(pt[0], dScale), point2d(pt[1], dScale)));
}

void PdfDirectWrapperImpl::saveSplines(CU_OBJECT_DATA* objdata, const double& dScale, 
                                       GeometryCache::Splines& splines,
                                       GeometryCache::Polylines& polylines)
{
  OdGePoint3dArray ctrlPts;
  OdGeKnotVector knotsVec;
  OdGeDoubleArray knots;
  OdGeDoubleArray wts;

  CuPathDef* path = (CuPathDef*)&objdata->path.path;
  while (path != NULL)
  {
    bool bPLine = true;
    int noSeg = path->nsegs;
    CuPathSeg* seg = path->psegs;

    ctrlPts.clear();
    ctrlPts.reserve(noSeg+4);
    for (int i = 0; i < noSeg - 1; i++)
    {
      CuPathSeg& s = seg[i];
      if (s.cubic == 1)
      {
        bPLine = false;
        ctrlPts.append(point3d(s.v1, dScale));
        ctrlPts.append(point3d(s.v2, dScale));
        ctrlPts.append(point3d(s.v3, dScale));
      }
      else
      {
        OdGePoint3d ctrlPt(point3d(seg[i].v1, dScale));
        ctrlPts.append(ctrlPt);
        ctrlPts.append(ctrlPt);
        ctrlPts.append(point3d(seg[i+1].v1, dScale));
      }
    }

    /* check if beginpoint is endpoint, if not add it ! */
    CuPoint& v0 = seg[0].v1;
    CuPoint& vn = seg[noSeg-1].v1;
    if (path->cyclic != 0 && (fabs(v0.x - vn.x) > 0.1 || fabs(v0.y - vn.y) > 0.1))
    {
      OdGePoint3d pn(point3d(vn, dScale));
      ctrlPts.append(pn);
      ctrlPts.append(pn);
      OdGePoint3d p0(point3d(v0, dScale));
      ctrlPts.append(p0);
      ctrlPts.append(p0);
      noSeg++;
    }
    else
      ctrlPts.append(point3d(vn, dScale));

    OdGeExtents2d objExt;
    if (!bPLine)
    {
      /* weight values */
      int nKnots = (noSeg - 1) * 3 + 1;
      wts.resize(nKnots);
      for (int i = 0; i < nKnots; i++)
        wts[i] = 1.;

      /* Knot points */
      knots.resize(nKnots + 4);
      double kVal = 1.;
      int j = 0;
      for (j = 0; j < 4; j++)
        knots[j] = 0.;
      for (int i = 1; i < noSeg - 1; i++)
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

      // update extents
      for (OdGePoint3dArray::iterator i = ctrlPts.begin(); i != ctrlPts.end(); ++i)
        objExt.addPoint(i->convert2d());

      OdGeNurbCurve3d spline;
      spline.set(3, knotsVec, ctrlPts, wts, false);
      splines.push_back(std::pair<OdGeNurbCurve3d, OdGeExtents2d>(spline, objExt));
      //splines.last().first.set(3, knotsVec, ctrlPts, wts, false);
    } 
    else
    {
      OdGePoint3dArray pnts;
      pnts.append(ctrlPts[0]);
      for (OdUInt32 i = 1; i < ctrlPts.size(); i++)
      {
        if (!OdEqual(pnts.last().x, ctrlPts[i].x, 1.0e-6) || !OdEqual(pnts.last().y, ctrlPts[i].y, 1.0e-6))
        {
          pnts.append(ctrlPts[i]);
          objExt.addPoint(ctrlPts[i].convert2d());
        }
      }
      if (pnts.size() == 1)
      {
        pnts.append(pnts[0]);
        objExt.addPoint(pnts[0].convert2d());
      }
      polylines.push_back(GeometryCache::Polyline(pnts, objExt));
    }

    path = (CuPathDef*)path->next_contour;
  }
}

void PdfDirectWrapperImpl::saveEllipse(CU_OBJECT_DATA* objdata, const double& dScale, 
                                       GeometryCache::Ellipses& ellipses)
{
  OdGeVector3d majorAxis;
  OdGeVector3d minorAxis;
  double majorR, minorR;

  double xrad = objdata->ellipse.xrad*dScale;
  double yrad = objdata->ellipse.yrad*dScale;
  if (xrad >= yrad)
  {
    majorAxis = OdGeVector3d::kXAxis;
    minorAxis = OdGeVector3d::kYAxis;
    majorR = xrad;
    minorR = yrad/xrad;
  }
  else
  {
    majorAxis = OdGeVector3d::kYAxis;
    minorAxis = OdGeVector3d::kXAxis;
    majorR = yrad;
    minorR = xrad/yrad;
  }

  OdGePoint3d center(point3d(objdata->ellipse.center, dScale));
  OdGeEllipArc3d arc(center, majorAxis, minorAxis, majorR, minorR);
  OdGeExtents3d objExt3d;
  arc.getGeomExtents(objExt3d);
  OdGeExtents2d objExt(objExt3d.minPoint().convert2d(), objExt3d.maxPoint().convert2d());

  ellipses.push_back(GeometryCache::Ellipse(arc, objExt));
}

int PdfDirectWrapperImpl::saveGeometry(int type, CuTwoPoint* bbox, CU_OBJECT_ATTR* objattr,
                                       CU_OBJECT_DATA* objdata, CU_OBJPTR objptr, void* userdata)
{
  ConversionData* data = (ConversionData*)userdata;
  double dScale = data->m_scale/25.4;

  OdGeExtents2d objExt;
  if (bbox)
  {
    objExt.set(OdGePoint2d(bbox->first.x*dScale, bbox->first.y*dScale), 
      OdGePoint2d(bbox->second.x*dScale, bbox->second.y*dScale));
  }

  switch (type)
  {
  case CuPolygon:
  case CuPolyline:
    if (objdata->irpoly.pt_count == 2)
      saveLine(objdata, dScale, data->m_cache.m_polylines);
    else
      savePolyline(objdata, dScale, data->m_cache.m_polylines);
    break;
  case CuPath:
    saveSplines(objdata, dScale, data->m_cache.m_splines, data->m_cache.m_polylines);
    break;
  case CuEllipse:
    saveEllipse(objdata, dScale, data->m_cache.m_ellipses);
    break;
  }

  return 0;
}

int PdfDirectWrapperImpl::getLevelsCount(int nPage) const
{
  if (!security(getKey()))
  {
    return 0;
  }

  if (m_path.isEmpty() || nPage < 0 || nPage >= PdfDirectWrapperImpl::getItemsCount())
    return 0;

  try
  {
    const VgPsLayerInfo &layerInfo = getVgPsLayerInfo(nPage);

    if (checkAnnotationsOnPage(nPage))
    {
      return layerInfo.nol + 1;
    }

    return layerInfo.nol;
  }
  catch(...)
  {
    return 0;
  }
}

OdString PdfDirectWrapperImpl::getLevelName(int nPage, int nLevel) const
{
  if (!security(getKey()))
  {
    return OdString::kEmpty;
  }

  if (m_path.isEmpty() || nLevel < 0 || nLevel > PdfDirectWrapperImpl::getLevelsCount(nPage))
    return OdString::kEmpty;

  if (checkAnnotationsOnPage(nPage) && nLevel+1 == PdfDirectWrapperImpl::getLevelsCount(nPage) )
  {
    return "Comments and Markups";
  }

  const VgPsLayerInfo &layerInfo = getVgPsLayerInfo(nPage);
  
  return layerInfo.names[nLevel];
}

PdfDirectWrapperImpl::LevelsInfo::iterator PdfDirectWrapperImpl::fillLevelInfo(int nPage) const
{
  PdfDirectWrapperImpl::LevelsInfo::iterator pIt = m_LevelsInfo.find(nPage);
  if (pIt == m_LevelsInfo.end())
  {
    LevelsAtPage lvs;

    const VgPsLayerInfo &layerInfo = getVgPsLayerInfo(nPage);

    LevelsAtPage lvsAtPage;

    for(int f=0; f<layerInfo.nol; ++f)
    {
      // Layers are enabled by default
      lvsAtPage.insert(LevelsAtPage::value_type(layerInfo.names[f], true));
    }

    if (checkAnnotationsOnPage(nPage))
    {
      lvsAtPage.insert(LevelsAtPage::value_type("Comments and Markups", true));
    }

    m_LevelsInfo.insert( PdfDirectWrapperImpl::LevelsInfo::value_type(nPage, lvsAtPage) );
    pIt = m_LevelsInfo.find(nPage);
  }

  return pIt;
}

OdUInt8 PdfDirectWrapperImpl::isLevelEnabled(int nPage, int nLevel) const
{
  if (!security(getKey()))
  {
    return 0;
  }

  if (m_path.isEmpty() || nPage < 0 || nPage >= getItemsCount() || nLevel < 0 || nLevel > PdfDirectWrapperImpl::getLevelsCount(nPage))
  {
    return 0;
  }

  LevelsInfo::iterator pIt = fillLevelInfo(nPage);
  if (nLevel+1 == PdfDirectWrapperImpl::getLevelsCount(nPage) && checkAnnotationsOnPage(nPage))
  {
    return (pIt->second.find("Comments and Markups"))->second;
  }
//   else
//   {
//     return 0;
//   } 

  const VgPsLayerInfo &layerInfo = getVgPsLayerInfo(nPage);

  return (pIt->second.find(layerInfo.names[nPage]))->second;
}

OdUInt8 PdfDirectWrapperImpl::isLevelEnabled(int nPage, const OdString &layerName) const
{
  throw OdError(eNotImplementedYet);
  if (!security(getKey()))
  {
    return 0;
  }

  if (m_path.isEmpty())
    return 0;

  return true;
}

void PdfDirectWrapperImpl::enableLevel(int nPage, int nLevel, OdUInt8 flag)
{
  throw OdError(eNotImplementedYet);
  if (!security(getKey()))
  {
    return;
  }

  if (m_path.isEmpty() || nLevel < 0 || nLevel > PdfDirectWrapperImpl::getLevelsCount(nPage))
    return;

  if (nLevel+1 == PdfDirectWrapperImpl::getLevelsCount(nPage) )
  {
    if (checkAnnotationsOnPage(nPage))
    {
      //m_CommentsLayer.resize(nPage, false);
      //m_CommentsLayer[nPage] = flag;
      return;
    }
    else
    {
      return;
    }
  }


}

struct grabberInfo
{
  OdGiGeometry *pGrabber;
  double dScale;
};

int
DoCustomObjFunc(int type, CuTwoPoint *bbox, CU_OBJECT_ATTR *objattr,
                CU_OBJECT_DATA *objdata, CU_OBJPTR objptr, void *userdata)
{
  OdGiGeometry *pGrabber = ((grabberInfo *)userdata)->pGrabber;
  double dScale = ((grabberInfo *)userdata)->dScale;

  switch (type) 
  {
    case CuPolygon:
      {
        OdInt32 count = objdata->irpoly.pt_count;
        OdGePoint3dArray points;
        points.resize(count);
        for(OdInt32 f=0; f<count; ++f){
          points[f].set(objdata->irpoly.pt_h[f].x/25.4*dScale, objdata->irpoly.pt_h[f].y/25.4*dScale, 0);
        }

        if (count > 1 && false == points.first().isEqualTo(points.last())){
          points.append(points.first());
          ++count;
        }

        pGrabber->polyline(count, points.asArrayPtr());
      }
      break;
    case CuPolyline:
      {
        OdInt32 count = objdata->irpoly.pt_count;
        OdGePoint3dArray points;
        points.resize(count);
        for(OdInt32 f=0; f<count; ++f){
          points[f].set(objdata->irpoly.pt_h[f].x/25.4*dScale, objdata->irpoly.pt_h[f].y/25.4*dScale, 0);
        }
        pGrabber->polyline(count, points.asArrayPtr());
      }
    break;

    case CuPath:
      {
        CuPathDef *path = (CuPathDef *) &objdata->path.path;
        CuPathSeg *seg;

        int  i, j, noSeg;
        double  kVal = 1.0;

        while (path != NULL) {
          noSeg = path->nsegs;
          seg   = path->psegs;

          OdGeNurbCurve3d spline;
          OdGePoint3dArray ctrlPoints;
          OdGeKnotVector   knotsVec;
          double     *knots;
          OdGeDoubleArray  wts;

          for (i = 0; i < noSeg - 1; i++) {

            if (seg[i].cubic == 1) {
              ctrlPoints.append(OdGePoint3d(seg[i].v1.x/25.4*dScale, seg[i].v1.y/25.4*dScale, 0));
              ctrlPoints.append(OdGePoint3d(seg[i].v2.x/25.4*dScale, seg[i].v2.y/25.4*dScale, 0));
              ctrlPoints.append(OdGePoint3d(seg[i].v3.x/25.4*dScale, seg[i].v3.y/25.4*dScale, 0));
            } else {
              ctrlPoints.append(OdGePoint3d(seg[i].v1.x/25.4*dScale, seg[i].v1.y/25.4*dScale, 0));
              ctrlPoints.append(OdGePoint3d(seg[i].v1.x/25.4*dScale, seg[i].v1.y/25.4*dScale, 0));
              ctrlPoints.append(OdGePoint3d(seg[i+1].v1.x/25.4*dScale, seg[i+1].v1.y/25.4*dScale, 0));
            }
          }

          /* check if beginpoint is endpoint, if not add it ! */

          if (path->cyclic != 0 
          && (fabs(seg[0].v1.x - seg[noSeg-1].v1.x) > 0.1 
           || fabs(seg[0].v1.y - seg[noSeg-1].v1.y) > 0.1))
          {
              ctrlPoints.append(OdGePoint3d(seg[noSeg-1].v1.x/25.4*dScale, seg[noSeg-1].v1.y/25.4*dScale, 0));
              ctrlPoints.append(OdGePoint3d(seg[noSeg-1].v1.x/25.4*dScale, seg[noSeg-1].v1.y/25.4*dScale, 0));
              ctrlPoints.append(OdGePoint3d(seg[0].v1.x/25.4*dScale, seg[0].v1.y/25.4*dScale, 0));
              ctrlPoints.append(OdGePoint3d(seg[0].v1.x/25.4*dScale, seg[0].v1.y/25.4*dScale, 0));
              noSeg++;
          } else {
            ctrlPoints.append(OdGePoint3d(seg[noSeg-1].v1.x/25.4*dScale, seg[noSeg-1].v1.y/25.4*dScale, 0));
          }


          /* weight values */
          wts.resize((noSeg-1)*3 + 1);
          for (i=0; i < (noSeg-1)*3 + 1; i++) wts[i] = 1.0;


          /* Knot points */
          knots = (double *)malloc(((noSeg-1)*3 + 5)*sizeof(double));
          for (j=0; j< 4; j++)  knots[j] = 0.0;
          for (i = 1; i < noSeg - 1; i++) {
            knots[j++] = kVal;  
            knots[j++] = kVal;  
            knots[j++] = kVal;  
            kVal += 1.0;
          }
          for (i=0; i< 4; i++)  knots[j++] = kVal;

          knotsVec.set((noSeg-1)*3 + 5, knots, 0.0);

          spline.set(3, knotsVec, ctrlPoints, wts, false);

          pGrabber->nurbs(spline);

          path = (CuPathDef *) path->next_contour;
          free(knots);
        }
      }

      break;
    case CuEllipse:
      {
      double xrad = objdata->ellipse.xrad/25.4*dScale;
      double yrad = objdata->ellipse.yrad/25.4*dScale;

      OdGePoint3d center(objdata->ellipse.center.x/25.4*dScale, objdata->ellipse.center.y/25.4*dScale, 0);

      OdGeVector3d normal(OdGeVector3d::kZAxis);
      OdGeVector3d majorAxis;
      OdGeVector3d minorAxis;
      double majorR, minorR;

      if (xrad >= yrad)
      {
        majorAxis = OdGeVector3d::kXAxis;
        minorAxis = OdGeVector3d::kYAxis;
        majorR = xrad;
        minorR = yrad/xrad;
      }
      else
      {
        majorAxis = OdGeVector3d::kYAxis;
        minorAxis = OdGeVector3d::kXAxis;
        majorR = yrad;
        minorR = xrad/yrad;
      }

      OdGeEllipArc3d arc(center, majorAxis, minorAxis, majorR, minorR);
      pGrabber->ellipArc(arc);
      }
      break;
  }

  return (0);
}

int
DoCustomObjFuncExplode(int type, CuTwoPoint *bbox, CU_OBJECT_ATTR *objattr,
                CU_OBJECT_DATA *objdata, CU_OBJPTR objptr, void *userdata)
{
  OdGiGeometry *pGrabber = ((grabberInfo *)userdata)->pGrabber;
  double dScale = ((grabberInfo *)userdata)->dScale;

  switch (type) 
  {
    case CuEllipse:
    case CuPath:
    case CuPolygon:
    case CuPolyline:
      return DoCustomObjFunc(type, bbox, objattr, objdata, objptr, userdata);
    case CuImage:
      
      break;
    case CuText:
      { 
        OdChar buf[1028];
        for (OdInt32 i=0; i < objdata->text.strlen; i++)
          buf[i] = objdata->text.string[i];
        buf[objdata->text.strlen] = '\0';

#define  NORMAL    0
#define BOLD    1
#define  ITALIC    2
#define BOLDITALIC  3

        OdString str;
        str.format(L"\\f%s|b%d|i%d;\\C%d;%s", objattr->text.fname, objattr->text.style & BOLD, objattr->text.style & ITALIC, objattr->text.cIndex, buf);

          OdGiTextStyle style;
        style.set(objattr->text.fname, objattr->text.fname, objattr->text.capheight, dScale, 0.0, 0.0, false, false, false, false, false);
        pGrabber->text(OdGePoint3d(objdata->text.pos.x, objdata->text.pos.y, 0), OdGeVector3d::kZAxis, 
          OdGeVector3d(0, 0, objdata->text.rotation), str.c_str(), objdata->text.strlen, true, &style);
      }

      break;
    default:
      break;
  }

  return 0;
}

double PdfDirectWrapperImpl::getMeasureDictInfo(int nPage) const
{
  double bbox[4];
  double xScale = 0; 
  double yScale = 0;

  int ctElX = 0;
  int ctElY = 0;

  VGPdfGetMeasureDictInfo(TOCHAR(m_path.c_str()), nPage+1, bbox, &xScale, &yScale, &ctElX, &ctElY);

  if (ctElX > 1)
  {
    xScale = 0.;
  }

  double measureScale = (xScale == 0.) ? 1./72. : (xScale);
  return measureScale;
}

OdResult PdfDirectWrapperImpl::getExplodeGeometry(int nPage, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer> &Layers, bool bIgnoreImages) const
{
  if (!security(getKey()))
  {
    return eKeyNotFound;
  }

  if (m_path.isEmpty() || nPage < 0 || nPage > PdfDirectWrapperImpl::getItemsCount() || !pGrabber)
    return eInvalidInput;
  VGSetInsertMode(0);
  VGSetAutomaticPageSize();

  // Turn off parsing/handling of images.
  // Every call of VgPdfConvertPage2Custom() may create image file. 
  // Image width/height are the same as original size and may be very big (eg. 6000x4000 pixels). 
  VGIgnoreImages(bIgnoreImages); 

  double measureScale = getMeasureDictInfo(nPage) * 72.;
  struct grabberInfo
  {
    OdGiGeometry *pGrabber;
    double dScale;
  } tmp;

  tmp.dScale = measureScale;
  tmp.pGrabber = pGrabber;

  AutoVgPsLayerInfo layerInfo;
  setupLayers(layerInfo, Layers);
  
  VGPdfConvertPage2Custom(TOCHAR(m_path.c_str()), DoCustomObjFuncExplode, nPage + 1, layerInfo, &tmp);
  
  return eOk;
}
