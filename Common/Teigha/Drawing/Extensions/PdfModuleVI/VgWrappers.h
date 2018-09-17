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

#ifndef _VGWRAPPERS_INCLUDED_
#define _VGWRAPPERS_INCLUDED_

extern "C" 
{
#include "vgpsflow.h"
#include "custom.h"
}

// Uncomment the next line to enable tracing of Vg calls.
//#define VG_TRACE_MODE

#ifndef VG_TRACE_MODE

#ifdef _WIN32
#define VER10_555
#endif

#ifdef VER10_555
#define TOCHAR(a) ((OdChar *)(a))
#else
#define TOCHAR(a) ((char *)OdAnsiString(OdString(a)).c_str())
#endif

#ifdef VER10_555
#ifdef _WIN32
#define  VGInit(a)                                    VgInit(a)
#else
#define  VGInit()                                     VgInit()
#endif

#define  VGIgnoreImages(a)                            VgIgnoreImages(a)
#define  VGIgnoreText(a)                              VgIgnoreText(a)
#define  VGSetFontPath(a)                             VgSetUniFontPath(a)
#define  VGPdfCheckAnnotationsOnPage(a, b)            VgPdfUniCheckAnnotationsOnPage(a, b)/*????*/
#define  VGPdfConvertPage2Custom(a, b, c, d, e)       VgPdfUniConvertPage2Custom(a, b, c, d, e)/*????*/
#define  VGPdfConvertPage2DIB(a, b, c, d, e, f)       VgPdfUniConvertPage2DIB(a, b, c, d, e, f)/*????*/
#define  VGPdfDeleteImageDIB(a)                       VgPdfDeleteImageDIB(a)
#define  VGPdfGetLayerInfoOnPage(a, b, c)             VgPdfUniGetLayerInfoOnPage(a, b, c)  
#define  VGPdfGetMeasureDictInfo(a, b, c, d, e, f, g) VgPdfUniGetMeasureDictInfo(a, b, c, d, e, f, g)/*????*/
#define  VGPdfGetPageSize(a, b, c, d)                 VgPdfUniGetPageSize(a, b, c, d)
#define  VGPdfNoPages(a )                             VgPdfUniNoPages(a )
#define  VGPsSetLicense(a )                           VgPsSetLicense(a )
#define  VGSetAutomaticPageSize()                     VgSetAutomaticPageSize()
#define  VGSetDPI(a)                                  VgSetDPI(a)
#define  VGSetImageBgColor(a)                         VgSetImageBgColor(a)
#define  VGSetImageClipRect(a, b, c, d)               VgSetImageClipRect(a, b, c, d)
#define  VGSetImagePixWorH(a, b)                      VgSetImagePixWorH(a, b)
#define  VGSetColorMode(mode)                         VgSetColorMode(mode)
#define  VGSetImageScale(a)                           VgSetImageScale(a)
#define  VGSetInsertMode(a)                           VgSetInsertMode(a)
#define  VGSetRenderAnnotations(a )                   VgSetRenderAnnotations(a )

#else // VER10_555
#ifdef _WIN32
#define  VGInit(a)                                    VgInit(a)
#else
#define  VGInit()                                     VgInit()
#endif

#define  VGIgnoreImages(a)                            VgIgnoreImages(a)
#define  VGIgnoreText(a)                              VgIgnoreText(a)
#define  VGSetFontPath(a)                             VgSetFontPath(a)
#define  VGPdfCheckAnnotationsOnPage(a, b)            VgPdfCheckAnnotationsOnPage(a, b)
#define  VGPdfConvertPage2Custom(a, b, c, d, e)       VgPdfConvertPage2Custom(a, b, c, d, e)
#define  VGPdfConvertPage2DIB(a, b, c, d, e, f)       VgPdfConvertPage2DIB(a, b, c, d, e, f)
#define  VGPdfDeleteImageDIB(a)                       VgPdfDeleteImageDIB(a)
#define  VGPdfGetLayerInfoOnPage(a, b, c)             VgPdfGetLayerInfoOnPage(a, b, c)  
#define  VGPdfGetMeasureDictInfo(a, b, c, d, e, f, g) VgPdfGetMeasureDictInfo(a, b, c, d, e, f, g)
#define  VGPdfGetPageSize(a, b, c, d)                 VgPdfGetPageSize(a, b, c, d)
#define  VGPdfNoPages(a )                             VgPdfNoPages(a )
#define  VGPsSetLicense(a )                           VgPsSetLicense(a )
#define  VGSetAutomaticPageSize()                     VgSetAutomaticPageSize()
#define  VGSetDPI(a)                                  VgSetDPI(a)
#define  VGSetImageBgColor(a)                         VgSetImageBgColor(a)
#define  VGSetImageClipRect(a, b, c, d)               VgSetImageClipRect(a, b, c, d)
#define  VGSetImagePixWorH(a, b)                      VgSetImagePixWorH(a, b)
#define  VGSetColorMode(mode)                         VgSetColorMode(mode)
#define  VGSetImageScale(a)                           VgSetImageScale(a)
#define  VGSetInsertMode(a)                           VgSetInsertMode(a)
#define  VGSetRenderAnnotations(a )                   VgSetRenderAnnotations(a )
#endif // VER10_555

#else

#if defined(_WIN32) || defined(_WIN64)
int TraceVgInit(char *options);
#else
int TraceVgInit();
#endif

#if defined(_WIN32) && !defined(_WIN64)

int TraceVgSetFontPath(char *in);
int TraceVgPdfCheckAnnotationsOnPage(char *in, int pageno);
int TraceVgPdfConvertPage2Custom(char *in, CU_OBJECT_CB cbfunc, int pageno, VgPsLayerInfo *layerInfo, void *userData);
int TraceVgPdfConvertPage2DIB(char *in, int pageno, VgPsLayerInfo *layerInfo, BITMAPINFOHEADER *bmi, unsigned char **imagedata, VgPsProgCB pgCB);
int TraceVgPdfDeleteImageDIB(unsigned char *imagedata);
int TraceVgPdfGetLayerInfoOnPage(char * in, int pageno, VgPsLayerInfo *layerInfo);
int TraceVgPdfGetMeasureDictInfo(char *in, int pageno, double *bbox, double *xScale, double *yScale, int *ctElX, int *ctElY);
int TraceVgPdfGetPageSize(char *in, int pageno, double *extent, int cropbox);
int TraceVgPdfNoPages(char *in);
int TraceVgPsSetLicense(int value);
int TraceVgSetAutomaticPageSize();
int TraceVgSetDPI(int dpival);
int TraceVgSetImageBgColor(int *imagebg);
int TraceVgSetImageClipRect(FLOAT left, FLOAT bottom, FLOAT right, FLOAT top);
int TraceVgSetImagePixWorH(int w, int h);
int TraceVgSetColorMode(int mode);
int TraceVgSetImageScale(BOOL val);
int TraceVgSetInsertMode(UINT value);
int TraceVgSetRenderAnnotations(BOOL value);
int TraceVgIgnoreImages(bool a);
int TraceVgIgnoreText(bool a);

#else

int TraceVgSetFontPath(char *in);
int TraceVgPdfCheckAnnotationsOnPage(char *in, int pageno);
int TraceVgPdfConvertPage2Custom(char *in, CU_OBJECT_CB cbfunc, int pageno, VgPsLayerInfo *layerInfo, void *userData);
int TraceVgPdfConvertPage2DIB(char *in, int pageno, VgPsLayerInfo *layerInfo, BITMAPINFOHEADER *bmi, unsigned char **imagedata, VgPsProgCB pgCB);
int TraceVgPdfDeleteImageDIB(unsigned char *imagedata);
int TraceVgPdfGetLayerInfoOnPage(char *in, int pageno, VgPsLayerInfo *layerInfo);
int TraceVgPdfGetMeasureDictInfo(char *in, int pageno, double *bbox, double *xScale, double *yScale, int *ctElX, int *ctElY);
int TraceVgPdfGetPageSize(char *in, int pageno, double *extent, int cropbox);
int TraceVgPdfNoPages(char *in);
int TraceVgPsSetLicense(int value);
int TraceVgSetAutomaticPageSize();
int TraceVgSetDPI(int dpival);
int TraceVgSetImageBgColor(int *imagebg);
int TraceVgSetImageClipRect(double left, double bottom, double right, double top);
int TraceVgSetImagePixWorH(int w, int h);
int TraceVgSetColorMode(int mode);
int TraceVgSetImageScale(int mode);
int TraceVgSetInsertMode(int mode);
int TraceVgSetRenderAnnotations(int value);
int TraceVgIgnoreImages(bool a);
int TraceVgIgnoreText(bool a);
#endif

#if defined(_WIN32) || defined(_WIN64)
#define  VGInit(a)                                    TraceVgInit(a)
#else
#define  VGInit()                                     TraceVgInit()
#endif
#define  VGIgnoreImages(a)                            TraceVgIgnoreImages(a)
#define  VGIgnoreText(a)                              TraceVgIgnoreText(a)
#define  VGSetFontPath(a)                             TraceVgSetFontPath(a)
#define  VGPdfCheckAnnotationsOnPage(a, b)            TraceVgPdfCheckAnnotationsOnPage(a, b)
#define  VGPdfConvertPage2Custom(a, b, c, d, e)       TraceVgPdfConvertPage2Custom(a, b, c, d, e)
#define  VGPdfConvertPage2DIB(a, b, c, d, e, f)          TraceVgPdfConvertPage2DIB(a, b, c, d, e, f)
#define  VGPdfDeleteImageDIB(a)                       TraceVgPdfDeleteImageDIB(a)
#define  VGPdfGetLayerInfoOnPage(a, b, c)             TraceVgPdfGetLayerInfoOnPage(a, b, c)  
#define  VGPdfGetMeasureDictInfo(a, b, c, d, e, f, g) TraceVgPdfGetMeasureDictInfo(a, b, c, d, e, f, g)
#define  VGPdfGetPageSize(a, b, c, d)                 TraceVgPdfGetPageSize(a, b, c, d)
#define  VGPdfNoPages(a )                             TraceVgPdfNoPages(a )
#define  VGPsSetLicense(a )                           TraceVgPsSetLicense(a )
#define  VGSetAutomaticPageSize()                     TraceVgSetAutomaticPageSize()
#define  VGSetDPI(a)                                  TraceVgSetDPI(a)
#define  VGSetImageBgColor(a)                         TraceVgSetImageBgColor(a)
#define  VGSetImageClipRect(a, b, c, d)               TraceVgSetImageClipRect(a, b, c, d)
#define  VGSetImagePixWorH(a, b)                      TraceVgSetImagePixWorH(a, b)
#define  VGSetColorMode(mode)                         TraceVgSetColorMode(mode)
#define  VGSetImageScale(a)                           TraceVgSetImageScale(a)
#define  VGSetInsertMode(a)                           TraceVgSetInsertMode(a)
#define  VGSetRenderAnnotations(a )                   TraceVgSetRenderAnnotations(a )

#endif

#endif // _VGWRAPPERS_INCLUDED_
