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

#if !defined(OD_WIN_GDI_BMP_H_) 
#define OD_WIN_GDI_BMP_H_

#if defined(ODA_WINDOWS) && !defined(_WINRT) && !defined ( _WIN32_WCE )

#include "MemoryStream.h"
#include "DynamicLinker.h"
#include "RxRasterServices.h"
#include "Gi/GiViewportGeometry.h"

/** \details
  <group ExRender_Windows_Classes>
*/
class GDI_BMP : public OdRxObjectImpl<OdGiRasterImage>
{
  bool init(LONG biWidth, LONG biHeight)
  {
    uninit();
    m_hDC = ::CreateCompatibleDC(NULL);
    if(m_hDC)
    {
      // Fill in the header info.
      m_infoHeader.biWidth       = biWidth;
      m_infoHeader.biHeight      = biHeight;
      m_infoHeader.biPlanes      = 1;
      m_infoHeader.biBitCount    = (WORD)24;
      m_infoHeader.biCompression = BI_RGB;
      
      m_hBitmap = ::CreateDIBSection(m_hDC, (BITMAPINFO*)&m_infoHeader, DIB_RGB_COLORS, &m_pBits, NULL, 0);
      
      ::DeleteObject(::SelectObject(m_hDC, m_hBitmap));
    }
    return (m_hBitmap!=0);
  }
  void uninit()
  {
    ::memset(&m_infoHeader, 0, sizeof(BITMAPINFOHEADER));
    m_infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    if(m_hBitmap)
      ::DeleteObject(m_hBitmap);
    if(m_hDC)
      ::DeleteDC(m_hDC);
  }

  GDI_BMP()
    : m_hBitmap(0)
    , m_hDC(0)
    , m_pBits(0)
  {
    ::memset(&m_infoHeader, 0, sizeof(BITMAPINFOHEADER));
    m_infoHeader.biSize = sizeof(BITMAPINFOHEADER);
  }

public:
  HDC               m_hDC;
  BITMAPINFOHEADER  m_infoHeader;
  HBITMAP           m_hBitmap;
  void*             m_pBits;

  ~GDI_BMP()
  {
    uninit();
  }

  static OdSmartPtr<GDI_BMP> createObject(LONG biWidth, LONG biHeight)
  {
    OdSmartPtr<GDI_BMP> pRes(::new GDI_BMP, kOdRxObjAttach);
    if(pRes->init(biWidth, biHeight))
      return pRes;
    return OdSmartPtr<GDI_BMP>();
  }

  OdUInt32 pixelWidth() const { return m_infoHeader.biWidth; }
  OdUInt32 pixelHeight() const { return m_infoHeader.biHeight; }
  Units defaultResolution(double& /*xPelsPerUnit*/, double& /*yPelsPerUnit*/) const { return kNone; }
  OdUInt32 colorDepth() const { return m_infoHeader.biBitCount; }
  OdUInt32 numColors() const { return 0; }
  ODCOLORREF color(OdUInt32 /*nIndex*/) const { return 0; }
  OdUInt32 paletteDataSize() const { return 0; }
  void paletteData(OdUInt8* /*pBytes*/) const { }
  void scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines = 1) const
  {
    ::memcpy(pBytes, scanLines() + index * scanLineSize(), numLines * scanLineSize());
  }
  const OdUInt8* scanLines() const { return (const OdUInt8*)m_pBits; }
  PixelFormatInfo pixelFormat() const
  {
    PixelFormatInfo res;
    res.setBGR();
    return res;
  }
  OdUInt32 scanLinesAlignment() const { return 4; }

  ImageSource imageSource() const { return kFromOleObject; }
};

inline void drawWithGdiBmp(const OdGiSelfGdiDrawable* pDrawable,
                           OdGsBaseVectorizer* pVect,
                           const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                           OdUInt32 bitmapWidth, OdUInt32 bitmapHeight,
                           const OdGsDCRect& rect,
                           bool bMonochrome = false,
                           bool isExport = true, // true to skip SetWindowOrgEx (viewportOriginX/Y are 0)
                           OdUInt32 viewportOriginX = 0, OdUInt32 viewportOriginY = 0)
{
  OdSmartPtr<GDI_BMP> pBMP = GDI_BMP::createObject(bitmapWidth, bitmapHeight);
  if (pBMP.isNull())
    return;
  ODA_ASSERT_ONCE(!isExport || (!viewportOriginX && !viewportOriginY));
  if (!isExport)
    ::SetWindowOrgEx(pBMP->m_hDC, viewportOriginX, viewportOriginY, 0);

  if (!pDrawable->draw(static_cast<OdGiViewportDraw&>(*pVect), pBMP->m_hDC, rect))
    return; // CORE-11432
  if (bMonochrome)
  {
    OdRxRasterServicesPtr pRasterSvcs = ::odrxDynamicLinker()->loadModule(RX_RASTER_SERVICES_APPNAME);
    OdMemoryStreamPtr pStm = OdMemoryStream::createNew();

    const OdUInt32 flagsChain[3] = { OdRxRasterServices::kDithering, OdRxRasterServices::kDitheringCluster16x16, 0 };
    //ODA_ASSERT_VAR(bool bSuccess =)
      pRasterSvcs->convertRasterImage(pBMP, OdRxRasterServices::kTIFF, pStm, flagsChain);
    pStm->rewind();
    OdGiRasterImagePtr pConverted = pRasterSvcs->loadRasterImage(pStm);

    pVect->output().destGeometry().rasterImageProc(origin, 
                                                   u / pConverted->pixelWidth(), 
                                                   v / pConverted->pixelHeight(),
                                                   pConverted, 0, 0);
  } 
  else
    pVect->output().destGeometry().rasterImageProc(origin, 
                                                   u / pBMP->pixelWidth(), 
                                                   v / pBMP->pixelHeight(), 
                                                   pBMP, 0, 0);
}

#endif // defined(ODA_WINDOWS) && !defined(_WINRT)

#endif //OD_WIN_GDI_BMP_H_

