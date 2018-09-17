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
#include "custom.h"
#include "OdArray.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeExtents2d.h"
#include "VgWrappers.h"
#include "PdfUnderlay.h"
#include "MemoryStream.h"
#include "OdPlatformStreamer.h"
#include "RxRasterServices.h"

#define ENABLE_FLYSDK_TESTS
#ifdef ENABLE_FLYSDK_TESTS

namespace flysdk_tests
{
#if 0
  static OdGiRasterImagePtr convertBmpToRaster(const BITMAPINFOHEADER &bmi, const unsigned char *imagedata)
  {
    OdStreamBufPtr pFileBuf = OdMemoryStream::createNew();

    OdPlatformStreamer::wrInt16(*pFileBuf, 0x4d42);
    OdUInt32 scanLinesPos = 14 + 40;  // Below version is correct only on Windows
    OdUInt32 size = scanLinesPos + bmi.biWidth*32*Od_abs(bmi.biHeight);

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

    OdGiRasterImagePtr pTmp;
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if ( !pRasSvcs.isNull() )
    {
      pTmp = pRasSvcs->loadRasterImage(pFileBuf);
    }
    return pTmp;
  }

  void CORE11202_SnapsVsGeometry()
  {
    OdString path = "E:\\ODA\\share\\RegressionTest\\DataFiles\\tests\\bitmapTestSuite\\CORE11202\\OdWriteEx_internaltest.pdf";
    int nCountOfPages = VGPdfNoPages(TOCHAR(path.c_str()));

    VGIgnoreImages(true);

//       MediaBox [0 0 612 792]
//       0.18 0 0 0.18 16 31 cm
//       1284 1908 m
//       1284 1836 l
//       S
//       1248 1872 m
//       1320 1872 l
//       S
    double PDFLineX1 = 792 - 374.44;//(792. - 1908*0.18)-31.;// = 417.56;
    double PDFLineX2 = 792 - 361.4799999;//(792. - 1836*0.18)-31.;
    double PDFLineY1 = 240.63999999;//(1248. * 0.18)+16.;//(1836) * 0.18 + 31;
    double PDFLineY2 = 253.59999999;//(1320. * 0.18)+16.;//(1908) * 0.18 + 31;

    double clipRect_a = PDFLineX1/(72/25.4);
    double clipRect_b = PDFLineY1/(72/25.4);
    double clipRect_c = PDFLineX2/(72/25.4);
    double clipRect_d = PDFLineY2/(72/25.4);

    // This is FlySDK limitation. Origin of clipping area must be aligned by 1\72 of inch.
     clipRect_a = (/*FLOAT*/double)(OdInt32(clipRect_a / (25.5 / 72.)) * (25.5 / 72.));
     clipRect_b = (/*FLOAT*/double)(OdInt32(clipRect_b / (25.5 / 72.)) * (25.5 / 72.));
     clipRect_c = (/*FLOAT*/double)((OdInt32(clipRect_c / (25.5 / 72.))) * (25.5 / 72.));
     clipRect_d = (/*FLOAT*/double)((OdInt32(clipRect_d / (25.5 / 72.))) * (25.5 / 72.));

    VGSetImageClipRect(clipRect_a, clipRect_b, clipRect_c, clipRect_d);
    VGSetImageScale(0);
    VGSetDPI(1000);
    //VGSetImagePixWorH(1, 1);

    BITMAPINFOHEADER bmi;
    unsigned char *imagedata = 0;
    VGPdfConvertPage2DIB(TOCHAR(path.c_str()), 1, 0, &bmi, &imagedata, 0);

    VGSetImageClipRect(0., 0., 0., 0.);

    OdGiRasterImagePtr pRaster = convertBmpToRaster(bmi, imagedata);

    VGPdfDeleteImageDIB(imagedata);

    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (pRasSvcs.get())
      pRasSvcs->saveRasterImage(pRaster, "d:\\output.bmp");
  }

#endif
  int
  custom45(int type, CuTwoPoint *bbox, CU_OBJECT_ATTR *objattr,
                  CU_OBJECT_DATA *objdata, CU_OBJPTR objptr, void *userdata)
  {
    OdArray<OdString> &log = *((OdArray<OdString>*)userdata);

    switch (type) 
    {
    case CuPolyline:
      {
        OdInt32 count = objdata->irpoly.pt_count;

        OdString str;
        str.format(L"\nCuPolyline num of points %d\n", count);
        log.push_back(str);

        OdGePoint3d tmp;
        for(OdInt32 f=0; f<count; ++f){
          tmp.set(objdata->irpoly.pt_h[f].x, objdata->irpoly.pt_h[f].y, 0);
          str.format(L"%.10e,%.10e,%.10e\n", tmp.x, tmp.y, tmp.z);
          log.push_back(str);
        }
      }
      break;
    case CuPath:
      {
        CuPathDef *path = (CuPathDef *) &objdata->path.path;
        CuPathSeg *seg;

        int	i, noSeg;
        double	kVal = 1.0;

        while (path != NULL) {
          noSeg = path->nsegs;
          seg   = path->psegs;

          OdGePoint3dArray ctrlPoints;

          for (i = 0; i < noSeg - 1; i++) {

            if (seg[i].cubic == 1) {
              ctrlPoints.append(OdGePoint3d(seg[i].v1.x, seg[i].v1.y, 0));
              ctrlPoints.append(OdGePoint3d(seg[i].v2.x, seg[i].v2.y, 0));
              ctrlPoints.append(OdGePoint3d(seg[i].v3.x, seg[i].v3.y, 0));
            } else {
              ctrlPoints.append(OdGePoint3d(seg[i].v1.x, seg[i].v1.y, 0));
              ctrlPoints.append(OdGePoint3d(seg[i].v1.x, seg[i].v1.y, 0));
              ctrlPoints.append(OdGePoint3d(seg[i+1].v1.x, seg[i+1].v1.y, 0));
            }
          }

          OdInt32 count = ctrlPoints.size();

          OdString str;
          str.format(L"\nCuPath num of points %d\n", count);
          log.push_back(str);

          for(OdInt32 f=0; f<count; ++f){
            str.format(L"%.10e,%.10e,%.10e\n", ctrlPoints[f].x, ctrlPoints[f].y, ctrlPoints[f].z);
            log.push_back(str);
          }

          path = (CuPathDef *) path->next_contour;
        }
        break;
      }
    }

    return 0;
  }

  void ShiftedLines_1254(OdArray<OdString> &log)
  {
    if (log.size() != 1)
    {
      return;
    }
#ifdef _WIN32
    OdString pathToTestFile = log.first();
    log.clear();

    int m_CountOfPages = VGPdfNoPages(TOCHAR(pathToTestFile.c_str()));
#else
    OdAnsiString pathToTestFile = OdAnsiString(log.first());
    log.clear();

    int m_CountOfPages = VGPdfNoPages((char*)(pathToTestFile.c_str()));
#endif
    
    VgPdfConvertPage2Custom((char*)pathToTestFile.c_str(), custom45, 1, 0, &log);
  }
};

OdResult PdfUnderlayModule::forInternalUseOnly(OdUInt32 n, void *p)
{
  OdArray<OdString> &log = *((OdArray<OdString>*)p);
  switch(n)
  {
  case 0:
    flysdk_tests::ShiftedLines_1254(log);
    break;
  default:
    return eInvalidIndex;
  }

  return eOk;
}

#else

OdResult OD_DLL_EXPORT forInternalUseOnly(OdUInt32 , void *)
{
  return eNotImplemented;
}

#endif
