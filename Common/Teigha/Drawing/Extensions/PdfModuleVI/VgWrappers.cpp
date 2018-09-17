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
#include "VgWrappers.h"
#include "OdString.h"

#ifdef VG_TRACE_MODE

void odPrintConsoleString(const wchar_t* fmt, ...)
{
  va_list argList;
  va_start(argList, fmt);
  OdString s;
  s.formatV(fmt, argList);
  va_end(argList);
#if defined(ODA_WINDOWS) && !defined(_WIN32_WCE)
  DWORD dw;
  HANDLE o = ::GetStdHandle(STD_OUTPUT_HANDLE);
  if (::GetFileType(o) == FILE_TYPE_CHAR)
    WriteConsoleW(o, s.c_str(), s.getLength(), &dw, 0);
  else
    WriteFile(o, (const char*)s, s.getLengthA(), &dw, 0);
#elif defined(ANDROID)
  printf("%s", (const char*)s);
#else
  printf("%ls", s.c_str());
#endif
}

int TraceVgSetFontPath(char *in)
{
  int ret = VgSetFontPath(in);
  odPrintConsoleString(L"VgSetFontPath(%ls); : %d\n", OdString(in).c_str(), ret);
  return ret;
}

#if defined(ODA_WINDOWS)
int TraceVgInit(char *options)
{
  int ret = VgInit(options);
  odPrintConsoleString(L"VgInit(%ls); : %d\n", OdString(options).c_str(), ret);
  return ret;
}
#else
int TraceVgInit()
{
  int ret = VgInit();
  odPrintConsoleString(L"VgInit(); : %d\n", ret);
  return ret;
}

#endif

int TraceVgIgnoreImages(bool a)
{
  int ret = VgIgnoreImages(a);
  odPrintConsoleString(L"VgIgnoreImages(%ls); : %d\n", a ? "true" : "false", ret);
  return ret;
}

int TraceVgIgnoreText(bool a)
{
  int ret = VgIgnoreText(a);
  odPrintConsoleString(L"VgIgnoreText(%ls); : %d\n", a ? "true" : "false", ret);
  return ret;
}

#if defined(_WIN32) && !defined(_WIN64)
int TraceVgPdfCheckAnnotationsOnPage(char *in, int pageno)
{
  int ret = VgPdfCheckAnnotationsOnPage(in, pageno);
  odPrintConsoleString(L"VgPdfCheckAnnotationsOnPage(%ls, %d); : %d\n", OdString(in).c_str(), pageno, ret);
  return ret;
}

int TraceVgPdfGetLayerInfoOnPage(char *in, int pageno, VgPsLayerInfo *layerInfo)
{
  int ret = VgPdfGetLayerInfoOnPage(in, pageno, layerInfo);
  odPrintConsoleString(L"VgPdfGetLayerInfoOnPage(%ls, %d, %x); : %d\n", OdString(in).c_str(), pageno, layerInfo, ret);
  return ret;
}
int TraceVgPdfGetPageSize(char *in, int pageno, double *extent, int cropbox)
{
  int ret = VgPdfGetPageSize(in, pageno, extent, cropbox);
  odPrintConsoleString(L"VgPdfGetPageSize(%ls, %d, %x, %d); : %d\n", OdString(in).c_str(), pageno, extent, cropbox, ret);
  return ret;
}
int TraceVgPdfNoPages(char *in)
{
  int ret = VgPdfNoPages(in);
  odPrintConsoleString(L"VgPdfNoPages(%ls); : %d\n", OdString(in).c_str(), ret);
  return ret;
}
int TraceVgSetImageClipRect(FLOAT left, FLOAT bottom, FLOAT right, FLOAT top)
{
  int ret = VgSetImageClipRect(left, bottom, right, top);
  odPrintConsoleString(L"VgSetImageClipRect(%g, %g, %g, %g); : %d\n", left, bottom, right, top, ret);
  return ret;
}
int TraceVgSetImageScale(BOOL mode)
{
  int ret = VgSetImageScale(mode);
  odPrintConsoleString(L"VgSetImageScale(%d); : %d\n", mode, ret);
  return ret;
}
int TraceVgSetInsertMode(UINT mode)
{
  int ret = VgSetInsertMode(mode);
  odPrintConsoleString(L"VgSetInsertMode(%d); : %d\n", mode, ret);
  return ret;
}
int TraceVgSetRenderAnnotations(BOOL value)
{
  int ret = VgSetRenderAnnotations(value);
  odPrintConsoleString(L"VgSetRenderAnnotations(%d); : %d\n", value, ret);
  return ret;
}
#else

int TraceVgPdfGetLayerInfoOnPage(char *in, int pageno, VgPsLayerInfo *layerInfo)
{
  int ret = VgPdfGetLayerInfoOnPage(in, pageno, layerInfo);
  odPrintConsoleString(L"VgPdfGetLayerInfoOnPage(%s, %d, %x); : %d\n", in, pageno, layerInfo, ret);
  return ret;
}
int TraceVgPdfGetPageSize(char *in, int pageno, double *extent, int cropbox)
{
  int ret = VgPdfGetPageSize(in, pageno, extent, cropbox);
  odPrintConsoleString(L"VgPdfGetPageSize(%s, %d, %x, %d); : %d\n", in, pageno, extent, cropbox, ret);
  return ret;
}
int TraceVgPdfNoPages(char *in)
{
  int ret = VgPdfNoPages(in);
  odPrintConsoleString(L"VgPdfNoPages(%s); : %d\n", in, ret);
  return ret;
}
int TraceVgSetImageClipRect(double left, double bottom, double right, double top)
{
  int ret = VgSetImageClipRect(left, bottom, right, top);
  odPrintConsoleString(L"VgSetImageClipRect(%g, %g, %g, %g); : %d\n", left, bottom, right, top, ret);
  return ret;
}
int TraceVgSetImageScale(int mode)
{
  int ret = VgSetImageScale(mode);
  odPrintConsoleString(L"VgSetImageScale(%d); : %d\n", mode, ret);
  return ret;
}
int TraceVgSetInsertMode(int mode)
{
  int ret = VgSetInsertMode(mode);
  odPrintConsoleString(L"VgSetInsertMode(%d); : %d\n", mode, ret);
  return ret;
}
int TraceVgSetRenderAnnotations(int value)
{
  int ret = VgSetRenderAnnotations(value);
  odPrintConsoleString(L"VgSetRenderAnnotations(%d); : %d\n", value, ret);
  return ret;
}

int TraceVgPdfCheckAnnotationsOnPage(char *in, int pageno)
{
  int ret = VgPdfCheckAnnotationsOnPage(in, pageno);
  odPrintConsoleString(L"VgPdfCheckAnnotationsOnPage(%s, %d); : %d\n", in, pageno, ret);
  return ret;
}

#endif

int TraceVgPdfConvertPage2Custom(char *in, CU_OBJECT_CB cbfunc, int pageno, VgPsLayerInfo *layerInfo, void *userData)
{
  odPrintConsoleString(L"VgPdfConvertPage2Custom(%ls, %x, %d, %x, %x);\n", OdString(in).c_str(), cbfunc, pageno, layerInfo, userData);

  if (layerInfo)
  {
    odPrintConsoleString(L"layerInfo : %d\n", layerInfo->nol);
    for(int f=0;f<layerInfo->nol;++f)
    {
      odPrintConsoleString(L"            %ls\n", OdString(layerInfo->names[f]).c_str());  
    }
  }

  int ret = VgPdfConvertPage2Custom(in, cbfunc, pageno, layerInfo, userData);
  odPrintConsoleString(L"VgPdfConvertPage2Custom return : %d\n", ret);
  return ret;
}
int TraceVgPdfConvertPage2DIB(char *in, int pageno, VgPsLayerInfo *layerInfo, BITMAPINFOHEADER *bmi, unsigned char **imagedata, VgPsProgCB pgCB)
{
  int ret = VgPdfConvertPage2DIB(in, pageno, layerInfo, bmi, imagedata, pgCB);
  odPrintConsoleString(L"VgPdfConvertPage2DIB(%ls, %d, %x, %x, %x, %x); : %d\n", OdString(in).c_str(), pageno, layerInfo, bmi, imagedata, pgCB, ret);
  return ret;
}
int TraceVgPdfDeleteImageDIB(unsigned char *imagedata)
{
  int ret = VgPdfDeleteImageDIB(imagedata);
  odPrintConsoleString(L"VgPdfDeleteImageDIB(%x); : %d\n", imagedata, ret);
  return ret;
}
int TraceVgPdfGetMeasureDictInfo(char *in, int pageno, double *bbox, double *xScale, double *yScale, int *ctElX, int *ctElY)
{
  int ret = VgPdfGetMeasureDictInfo(in, pageno, bbox, xScale, yScale, ctElX, ctElY);
  odPrintConsoleString(L"VgPdfGetMeasureDictInfo(%ls, %d, %x, %x, %x, %x, %x); : %d\n", OdString(in).c_str(), pageno, bbox, xScale, yScale, ctElX, ctElY, ret);
  return ret;
}

int TraceVgPsSetLicense(int value)
{
  int ret = VgPsSetLicense(value);
  odPrintConsoleString(L"VgPsSetLicense(%d); : %d\n", value, ret);
  return ret;
}
int TraceVgSetAutomaticPageSize()
{
  int ret = VgSetAutomaticPageSize();
  odPrintConsoleString(L"VgSetAutomaticPageSize(); : %d\n", ret);
  return ret;
}
int TraceVgSetDPI(int dpival)
{
  int ret = VgSetDPI(dpival);
  odPrintConsoleString(L"VgSetDPI(%d); : %d\n", dpival, ret);
  return ret;
}
int TraceVgSetImageBgColor(int *imagebg)
{
  int ret = VgSetImageBgColor(imagebg);
  odPrintConsoleString(L"VgSetImageBgColor(%x); : %d\n", imagebg, ret);
  return ret;
}
int TraceVgSetImagePixWorH(int w, int h)
{
  int ret = VgSetImagePixWorH(w, h);
  odPrintConsoleString(L"VgSetImagePixWorH(%d, %d); : %d\n", w, h, ret);
  return ret;
}

#endif
