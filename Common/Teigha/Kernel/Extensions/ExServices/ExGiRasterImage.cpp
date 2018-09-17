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
#include "RxObjectImpl.h"
#include "../ExServices/ExGiRasterImage.h"
#include "OdRound.h"

// for ExRasterModule
#include "OdPlatformStreamer.h"
#include "OdString.h"

// ExGiRasterImage implementation
#include "RxSystemServices.h" // (?)
#include "RxDynamicModule.h"

#include "UInt8Array.h"

#ifdef RASTER_SNOW_BOUND
#define STL_USING_ALGORITHM
#define STL_USING_VECTOR
#include "OdaSTL.h"
#endif

#ifdef RASTER_FREE_IMAGE
#include "FreeImagePlus.h"
#endif

#if defined(_MSC_VER)
#pragma warning ( disable : 4100 ) //  unreferenced formal parameter
#endif

OdExGiRasterImage::OdExGiRasterImage(OdUInt32 width, OdUInt32 height, OdUInt8 bitCount)
  : m_header(width, height, bitCount)
  , m_imageSource(kUndefinedSource)
  , m_transparencyMode(kTransparencyDef)
{
}

OdExGiRasterImage::OdExGiRasterImage()
  : m_header(0, 0, 0)
  , m_imageSource(kUndefinedSource)
  , m_transparencyMode(kTransparencyDef)
{
}

// Gets the color(RGB) in specified point of raster image
void OdExGiRasterImage::getColorAt(OdUInt32 x, OdUInt32 y, OdUInt8& blue, OdUInt8& green, OdUInt8& red) const
{
  int lineSize = scanLineSize();
  ODA_ASSERT(x < m_header.m_width);
  ODA_ASSERT(y < m_header.m_height);
  const OdUInt8* pIm = m_bits.asArrayPtr();
  const OdUInt8* ptr = pIm + y * lineSize + x * m_header.m_bitPerPixel / 8;
  if(m_header.m_bitPerPixel == 24)
  {
    red   = ptr[2];
    green = ptr[1];
    blue  = ptr[0];
  }
  else 
  {
    OdUInt32 index = 
      (
      ptr[0] >> 
      (
      ((8 / m_header.m_bitPerPixel) - 1 - (x % (8 / m_header.m_bitPerPixel)))
      * m_header.m_bitPerPixel
     )
     ) 
      & 
      ((1 << m_header.m_bitPerPixel) - 1);
    getPalColorAt(index, blue, green, red);
  }
}

// overrides
OdUInt32 OdExGiRasterImage::pixelWidth() const
{
  return m_header.m_width;
}

OdUInt32 OdExGiRasterImage::pixelHeight() const
{
  return m_header.m_height;
}

OdUInt32 OdExGiRasterImage::colorDepth() const
{
  return m_header.m_bitPerPixel;
}

OdGiRasterImage::PixelFormatInfo OdExGiRasterImage::pixelFormat() const
{
  OdGiRasterImage::PixelFormatInfo res;
  if(numColors())
  {
    res.setBGRA(); // palette is BGRA in BMP
  }
  else
  {
    switch(m_header.m_bitPerPixel)
    {
    case 16:
      res.set16bitBGR();
      break;
    case 24:
      res.setBGR();
      break;
    case 32:
      res.setBGRA();
      break;
    default:
      ODA_FAIL();
    }
  }
  return res;
}

OdUInt32 OdExGiRasterImage::scanLinesAlignment() const
{
  return 4;
}

OdGiRasterImage::Units OdExGiRasterImage::defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const
{
  xPelsPerUnit = m_header.m_xPelsPerUnit;
  yPelsPerUnit = m_header.m_yPelsPerUnit;
  return m_header.m_resUnits;
}

void OdExGiRasterImage::copyFrom(const OdRxObject* pOtherObj)
{
  OdExGiRasterImage* pFrom = (OdExGiRasterImage*)pOtherObj;
  ODA_ASSERT(pFrom);
  m_header           = pFrom->m_header;
  m_bits             = pFrom->m_bits;
  m_palette          = pFrom->m_palette;
  m_imageSource      = pFrom->m_imageSource;
  m_sourceFileName   = pFrom->m_sourceFileName;
  m_transparencyMode = pFrom->m_transparencyMode;
}

// MKU 05.04.2003
//  ---  OdGiRasterImage interface  ---

// Returns the number of colors in the palette of the image
OdUInt32 OdExGiRasterImage::numColors() const
{
  return getPalNumColors();
}

// Returns the color by index from the palette of the image
// Returns the color by index from the palette of the image
ODCOLORREF OdExGiRasterImage::color(OdUInt32 nIndex) const
{
  OdUInt8 blue, green, red, alpha;
  getPalColorAt(nIndex, blue, green, red, &alpha);
  return ODRGBA(red, green, blue, alpha);
}

void OdExGiRasterImage::scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines) const
{
  OdUInt32 scLSz(scanLineSize());
  memcpy(pBytes, m_bits.asArrayPtr() + index*scLSz, numLines*scLSz);
}

/** Description:
    Returns pointer to image's pixels in bmp format.
    Note that it is optional to have implementation of this function
    (to prevent dummy copying of pixels data). You can return NULL in your
    implementation if it is inconvenient in some case - any functionality
    uses this interface must take it into account.
*/
const OdUInt8* OdExGiRasterImage::scanLines() const
{
  return m_bits.asArrayPtr();
}

OdUInt32 OdExGiRasterImage::supportedParams() const
{
  return kImageSource | kTransparencyMode | kSourceFileName;
}

OdGiRasterImage::ImageSource OdExGiRasterImage::imageSource() const
{
  return m_imageSource;
}

void OdExGiRasterImage::setImageSource(ImageSource source)
{
  m_imageSource = source;
}

const OdString &OdExGiRasterImage::sourceFileName() const
{
  return m_sourceFileName;
}

void OdExGiRasterImage::setSourceFileName(const OdString &fileName)
{
  m_sourceFileName = fileName;
}

OdGiRasterImage::TransparencyMode OdExGiRasterImage::transparencyMode() const
{
  return m_transparencyMode;
}

void OdExGiRasterImage::setTransparencyMode(TransparencyMode mode)
{
  m_transparencyMode = mode;
}

// ExRasterModule implementation

/** Returns the class, loading the raster images from different formats
*/

#ifdef RASTER_JPEG6B

#undef FAR

extern "C"
{
#include "jpeglib.h"
}

/*
 * <setjmp.h> is used for the optional error recovery mechanism.
 */

#include <setjmp.h>

struct my_error_mgr 
{
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

static OdGiRasterImagePtr loadJpegImage(OdString fileName)
{
  OdExGiRasterImagePtr pExRastIm = OdRxObjectImpl<OdExGiRasterImage>::createObject();
  
  OdUInt32 i, j;
  /* This struct contains the JPEG decompression parameters and pointers to
  * working space (which is allocated as needed by the JPEG library).
  */
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  /* Step 1: allocate and initialize JPEG decompression object */
  
  // ...
  
  /* Now we can initialize the JPEG decompression object. */
  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
#ifdef _MSC_VER
#pragma warning(disable:4611)
#endif
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer))
  {
  /* If we get here, the JPEG code has signaled an error.
  * We need to clean up the JPEG object, close the input file, and return.
    */
    jpeg_destroy_decompress(&cinfo);
    return OdGiRasterImagePtr();
  }
  jpeg_create_decompress(&cinfo);
  
  /* Step 2: specify data source (eg, a file) */
  
#if defined(TD_UNICODE) && defined(_wfopen)
  FILE *infile = _wfopen(fileName, L"rb");
#else
  FILE *infile = fopen(fileName, "rb");
#endif
  if (NULL != infile)
  {
    jpeg_stdio_src(&cinfo, infile);
    /* Step 3: read file parameters with jpeg_read_header() */
  
    (void) jpeg_read_header(&cinfo, TRUE);
    /* Step 4: set parameters for decompression */
  
    /* setting output color space to RGB
    */
    cinfo.out_color_space = JCS_RGB;
  
    /* Step 5: Start decompressor */
  
    (void) jpeg_start_decompress(&cinfo);
  
    pExRastIm->setMetrics(cinfo.output_width, cinfo.output_height, 24);
  
    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */
    int bmpLineSize = pExRastIm->scanLineSize();//(header.m_header.m_width * 3/*number of components*/ + 3) & ~3;
  
    //scanLines.resize(cinfo.output_height * bmpLineSize);
    pExRastIm->bits().resize(cinfo.output_height * bmpLineSize);
    i = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {
      OdUInt8* ptr = /*scanLines.asArrayPtr()*/pExRastIm->bits().asArrayPtr()
        + (cinfo.output_height - i - 1) * 
        bmpLineSize;
      jpeg_read_scanlines(&cinfo, (JSAMPLE**)&ptr, 1);
      for(j = 0; j < cinfo.output_width; j ++)
      {
        OdUInt8 tmp;
        tmp = ptr[0];
        ptr[0] = ptr[2];
        ptr[2] = tmp;
        ptr += 3;
      }
      i ++;
    }
  
    /* Step 7: Finish decompression */
  
    jpeg_finish_decompress(&cinfo);
  
    /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */
    fclose(infile);
  }
  else 
  {
    pExRastIm = 0;
  }
  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* And we're done! */
  return pExRastIm;
}

#elif defined(RASTER_SNOW_BOUND)

#include "WINDOWS.H" // windows-specific
#pragma comment(lib, "SNBD7W9S.LIB" )
//#pragma comment(lib, "SNBD8W98.LIB" )
//#include "../../ThirdParty/SnowBound/NT_GLUE.H"
#include "IMGLIB.H"

static int raster_to_dib(char far* buffer, 
                         void far* private_data,
                         int ypos,
                         int bytes)
{
  OdUInt32 width, height;
  width  = ((OdExGiRasterImage*)private_data)->pixelWidth();
  height = ((OdExGiRasterImage*)private_data)->pixelHeight();
  //ODA_ASSERT(((OdExGiRasterImage*)private_data)->scanLineSize() == bytes);
  OdUInt8* sl = ((OdExGiRasterImage*)private_data)->bits().asArrayPtr();
  memcpy(sl + (height - ypos - 1) * bytes, buffer, bytes);
  return 0;
}

static int set_header(LPBITMAPINFOHEADER lpHeader, 
                      void far* private_data)
{
  ((OdExGiRasterImage*)private_data)->setMetrics(lpHeader->biWidth, lpHeader->biHeight, 
    (OdUInt8)(lpHeader->biBitCount));

  // Resizing...
  // SNB v7 reports incorrect size and scanlines for BMP32bpp and PNG32bpp which causes crash. v8 reports correct information here.
  bool bCorrectWinSize = OdGiRasterImage::calcBMPScanLineSize(lpHeader->biWidth, lpHeader->biBitCount) * lpHeader->biHeight == lpHeader->biSizeImage;
  ODA_ASSERT(bCorrectWinSize);
  if (!bCorrectWinSize)
    return -1; //USER_CANCEL;
  ((OdExGiRasterImage*)private_data)->bits().resize(lpHeader->biSizeImage);

  // Loading palette...
  if(lpHeader->biBitCount < 16)
  {
    OdUInt32 nBytes = 1 << (lpHeader->biBitCount + 2);
    ((OdExGiRasterImage*)private_data)->palette().data().resize(nBytes);
    memcpy(((OdExGiRasterImage*)private_data)->palette().data().asArrayPtr(),
      ((OdUInt8*)lpHeader) + sizeof(BITMAPINFOHEADER), nBytes);
  }
  return 0;
}

#endif

static void OdLoadCompressedBMPData(OdStreamBuf *pBuf, OdUInt8 *pData, OdUInt32 uCompression,
                                    OdUInt32 scanLineSize, OdUInt32 /*uWidth*/, OdUInt32 uHeight)
{
  const OdUInt8 RLE_COMMAND     = 0;
  const OdUInt8 RLE_ENDOFLINE   = 0;
  const OdUInt8 RLE_ENDOFBITMAP = 1;
  const OdUInt8 RLE_DELTA       = 2;
  OdUInt32 scanLinesSize = scanLineSize * uHeight;
  switch (uCompression)
  {
    case 2L: // RLE4
      {
        OdUInt8 status_byte = 0;
        OdUInt8 second_byte = 0;
        int scanline = 0;
        int bits = 0;
        bool low_nibble = false;
        for (bool bContinue = true; bContinue;)
        {
          status_byte = pBuf->getByte();
          switch (status_byte) {
            case RLE_COMMAND:
              status_byte = pBuf->getByte();
              switch (status_byte)
              {
                case RLE_ENDOFLINE:
                  bits = 0;
                  scanline++;
                  low_nibble = false;
                break;
                case RLE_ENDOFBITMAP:
                  bContinue = false;
                break;
                case RLE_DELTA:
                  {
                    OdUInt8 delta_x;
                    OdUInt8 delta_y;
                    delta_x = pBuf->getByte();
                    delta_y = pBuf->getByte();
                    bits += delta_x / 2;
                    scanline += delta_y;
                  }
                break;
                default:
                  {
                    second_byte = pBuf->getByte();
                    OdUInt8 *sline = pData + (scanline * scanLineSize); // Get row
                    for (int i = 0; i < (int)status_byte; i++)
                    {
                      if (low_nibble)
                      {
                        if (/*(OdUInt32)*/(sline + bits) < /*(OdUInt32)*/(pData + scanLinesSize))
                        {
                          *(sline + bits) |= (second_byte & 0x0F);
                        }
                        if (i != (int)status_byte - 1)
                          second_byte = pBuf->getByte();
                        bits++;
                      }
                      else
                      {
                        if (/*(OdUInt32)*/(sline + bits) < /*(OdUInt32)*/(pData + scanLinesSize))
                        {
                          *(sline + bits) = (OdUInt8)(second_byte & 0xF0);
                        }
                      }
                      low_nibble = !low_nibble;
                    }
                    if ((((status_byte + 1) >> 1) & 1) == 1)
                      second_byte = pBuf->getByte();
                  }
                break;
              }
            break;
            default:
              {
                OdUInt8 *sline = pData + (scanline * scanLineSize); // Get row
                second_byte = pBuf->getByte();
                for (OdUInt32 i = 0; i < (OdUInt32)status_byte; i++)
                {
                  if (low_nibble)
                  {
                    if (/*(OdUInt32)*/(sline + bits) < /*(OdUInt32)*/(pData + scanLinesSize))
                    {
                      *(sline + bits) |= (second_byte & 0x0F);
                    }
                    bits++;
                  }
                  else
                  {
                    if (/*(OdUInt32)*/(sline + bits) < /*(OdUInt32)*/(pData + scanLinesSize))
                    {
                      *(sline + bits) = (OdUInt8)(second_byte & 0xF0);
                    }
                  }
                  low_nibble = !low_nibble;
                }
              }
            break;
          }
        }
      }
    break;
    case 1L: // RLE8
      {
        OdUInt8 status_byte = 0;
        OdUInt8 second_byte = 0;
        int scanline = 0;
        int bits = 0;
        for (bool bContinue = true; bContinue;)
        {
          status_byte = pBuf->getByte();
          switch (status_byte)
          {
            case RLE_COMMAND:
              status_byte = pBuf->getByte();
              switch (status_byte)
              {
                case RLE_ENDOFLINE:
                  bits = 0;
                  scanline++;
                break;
                case RLE_ENDOFBITMAP:
                  bContinue = false;
                break;
                case RLE_DELTA:
                  {
                    OdUInt8 delta_x;
                    OdUInt8 delta_y;
                    delta_x = pBuf->getByte();
                    delta_y = pBuf->getByte();
                    bits += delta_x;
                    scanline += delta_y;
                  }
                break;
                default:
                  {
                    OdUInt8 *sline = pData + (scanline * scanLineSize); // Get row
                    pBuf->getBytes(sline + bits, (OdUInt32)status_byte);
                    if ((status_byte & 1) == 1)
                      second_byte = pBuf->getByte();
                    bits += status_byte;
                  }
                break;
              }
            break;
            default:
              {
                OdUInt8 *sline = pData + (scanline * scanLineSize);
                second_byte = pBuf->getByte();
                for (OdUInt32 i = 0; i < (OdUInt32)status_byte; i++)
                {
                  if ((OdUInt32)bits < scanLineSize)
                  {
                    *(sline + bits) = second_byte;
                    bits++;
                  }
                  else
                  {
                    bContinue = true;
                    break;
                  }
                }
              }
            break;
          }
        }
      }
    break;
  }
}

inline OdUInt32 OdComputeMaskOffset(OdUInt32 uMask)
{
  if (uMask == 0) return 0;
  OdUInt32 rVal = 0;
  while ((uMask & 1) != 1)
  {
    rVal++;
    uMask >>= 1;
  }
  return rVal;
}

inline OdUInt32 OdComputeMaskMult(OdUInt32 uMask, OdUInt32 uOffset)
{
  if (uMask == 0) return 1;
  return 0xFF / (uMask >> uOffset);
}

static void OdLoadBitFieldsBMPData(OdStreamBuf *pBuf, OdExGiRasterImage &bitmap,
                                   OdUInt32 scanLineSize, OdUInt32 uWidth, OdUInt32 uHeight)
{
  // Bit fields isn't compressed, but contains RGBA offsets in palette. Supported only 16 and 32 bpp.
  OdUInt32 colorDepth = bitmap.colorDepth();
  ODA_ASSERT(colorDepth == 16 || colorDepth == 32);
  ODA_ASSERT(bitmap.palette().numColors() == 3 || bitmap.palette().numColors() == 4);
  OdUInt8 *pBits = bitmap.bits().asArrayPtr();
  OdUInt8 *pBitsOut = pBits;
  OdBinaryData bpp16bits;
  OdUInt32 scanLinesSize = scanLineSize * uHeight;
  if (colorDepth == 16)
  {
    bpp16bits.resize(scanLinesSize);
    pBits = bpp16bits.asArrayPtr();
    bitmap.setMetrics(uWidth, uHeight, 32);
    bitmap.bits().resize(bitmap.scanLineSize() * uHeight);
    pBitsOut = bitmap.bits().asArrayPtr();
  }
  pBuf->getBytes(pBits, scanLinesSize);
  OdUInt32 redMask = 0, blueMask = 0, greenMask = 0, alphaMask = 0;
  OdUInt32 redOffset = 0, greenOffset = 0, blueOffset = 0, alphaOffset = 0;
  OdUInt32 redMult = 1, greenMult = 1, blueMult = 1, alphaMult = 1;
  bool withAlpha = bitmap.palette().numColors() == 4;
  const OdUInt32 *palColors = (const OdUInt32*)bitmap.palette().getData();
  redMask = palColors[0]; redOffset = OdComputeMaskOffset(redMask); redMult = OdComputeMaskMult(redMask, redOffset);
  greenMask = palColors[1]; greenOffset = OdComputeMaskOffset(greenMask); greenMult = OdComputeMaskMult(greenMask, greenOffset);
  blueMask = palColors[2]; blueOffset = OdComputeMaskOffset(blueMask); blueMult = OdComputeMaskMult(blueMask, blueOffset);
  if (withAlpha) {
    alphaMask = palColors[3]; alphaOffset = OdComputeMaskOffset(alphaMask); alphaMult = OdComputeMaskMult(alphaMask, alphaOffset);
    if (alphaMask == 0)
      withAlpha = false;
  }
  OdUInt8 red, green, blue, alpha = 0xFF;
  colorDepth /= 8;
  for (OdUInt32 h = 0; h < uHeight; h++)
  {
    OdUInt8 *pInScanline = pBits + scanLineSize * h;
    OdUInt8 *pOutScanline = pBitsOut + bitmap.scanLineSize() * h;
    for (OdUInt32 w = 0; w < uWidth; w++, pInScanline += colorDepth, pOutScanline += 4)
    {
      if (colorDepth == 2)
      {
        red = OdUInt8(((*((OdUInt16*)pInScanline) & redMask) >> redOffset) * redMult);
        green = OdUInt8(((*((OdUInt16*)pInScanline) & greenMask) >> greenOffset) * greenMult);
        blue = OdUInt8(((*((OdUInt16*)pInScanline) & blueMask) >> blueOffset) * blueMult);
        if (withAlpha)
          alpha = OdUInt8(((*((OdUInt16*)pInScanline) & alphaMask) >> alphaOffset) * alphaMult);
      }
      else
      {
        red = OdUInt8(((*((OdUInt32*)pInScanline) & redMask) >> redOffset) * redMult);
        green = OdUInt8(((*((OdUInt32*)pInScanline) & greenMask) >> greenOffset) * greenMult);
        blue = OdUInt8(((*((OdUInt32*)pInScanline) & blueMask) >> blueOffset) * blueMult);
        if (withAlpha)
          alpha = OdUInt8(((*((OdUInt32*)pInScanline) & alphaMask) >> alphaOffset) * alphaMult);
      }
      pOutScanline[0] = blue;
      pOutScanline[1] = green;
      pOutScanline[2] = red;
      pOutScanline[3] = alpha;
    }
  }
  bitmap.palette().setNumColors(0);
}

// Flags chain helpers
static bool flagsChainContains(const OdUInt32 *pFlagsChain, OdUInt32 flag)
{
  if (!pFlagsChain)
    return false;
  while (*pFlagsChain)
  {
    if (*pFlagsChain == flag)
      return true;
    pFlagsChain += 2;
  }
  return false;
}
static OdUInt32 flagsChainGetUInt(const OdUInt32 *pFlagsChain, OdUInt32 flag)
{
  if (!pFlagsChain)
    return 0;
  while (*pFlagsChain)
  {
    if (*pFlagsChain == flag)
      return pFlagsChain[1];
    pFlagsChain += 2;
  }
  return 0;
}
static OdInt32 flagsChainGetInt(const OdUInt32 *pFlagsChain, OdUInt32 flag)
{
  if (!pFlagsChain)
    return -1;
  while (*pFlagsChain)
  {
    if (*pFlagsChain == flag)
      return (OdInt32)pFlagsChain[1];
    pFlagsChain += 2;
  }
  return -1;
}

#ifdef RASTER_SNOW_BOUND
static int getSBRasterType(const OdString& s);
static int imageTypeToSBRasterType(OdUInt32 type);
#endif // RASTER_SNOW_BOUND

#ifdef RASTER_FREE_IMAGE
static OdUInt32 mapExtensionToImageType(const OdString &fileName);
static FREE_IMAGE_FORMAT mapImageTypeToFIFormat(OdUInt32 type, int &flags, const OdUInt32 *pFlagsChain = NULL);
static void fiImageFromGiRasterImage(const OdGiRasterImage *pRaster, fipImage &fiImage, FREE_IMAGE_FORMAT fiFormat, const OdUInt32 *pFlagsChain = NULL);
static void fiImageTransform(fipImage &fiImage, const OdUInt32 *pFlagsChain = NULL);
#ifndef _WIN32
static bool isFileNameNonAscii(const OdString &str);
#endif
static FreeImageIO *createNewStreamFIAdapter(OdStreamBuf *pStreamBuf, bool bForWrite = false);
static void releaseStreamFIAdapter(FreeImageIO *pAdapter);
#endif // RASTER_FREE_IMAGE

#ifdef RASTER_FREE_IMAGE

static void convertFIImageToGiRasterImage(const fipImage &fiImage, OdExGiRasterImage& bitmap)
{
  BITMAPINFOHEADER *pBmpInfo = fiImage.getInfoHeader();
  bitmap.setDefaultResolution(OdGiRasterImage::kMeter, pBmpInfo->biXPelsPerMeter, pBmpInfo->biYPelsPerMeter);
  bitmap.setMetrics(pBmpInfo->biWidth, pBmpInfo->biHeight, pBmpInfo->biBitCount);
  if (fiImage.getPaletteSize())
  {
    bitmap.palette().data().resize(fiImage.getPaletteSize());
    ::memcpy(bitmap.palette().data().asArrayPtr(), fiImage.getPalette(), fiImage.getPaletteSize());
  }
  bitmap.bits().resize(fiImage.getScanWidth() * pBmpInfo->biHeight);
  OdUInt8 *pBuf = (OdUInt8*)bitmap.bits().asArrayPtr();
  for (OdUInt32 h = 0; h < (OdUInt32)pBmpInfo->biHeight; h++, pBuf += fiImage.getScanWidth())
    ::memcpy(pBuf, fiImage.getScanLine(h), fiImage.getScanWidth());
}

static bool identifyTIFF(OdUInt16 in1, OdStreamBuf *pBuf)
{
  OdUInt8 *sig1 = (OdUInt8*)&in1;
  if ((sig1[0] == 0x49 && sig1[1] == 0x49) || (sig1[0] == 0x4D && sig1[1] == 0x4D))
  {
    OdUInt8 sig[4] = { 0, 0, 0, 0 };
    OdUInt64 startPos = pBuf->tell();
    pBuf->getBytes(sig, 4);
    pBuf->seek(startPos, OdDb::kSeekFromStart);
    return (sig[0] == 0x49 && sig[1] == 0x49 && sig[2] == 0x2A && sig[3] == 0x00) ||
           (sig[0] == 0x4D && sig[1] == 0x4D && sig[2] == 0x00 && sig[3] == 0x2A);
  }
  return false;
}

static void applyTIFFOrientation(fipImage &fiImage)
{ // #10394 : take into account TIFF Exif orientation tag
  fipTag oTag;
  if (fiImage.getMetadata(FIMD_EXIF_MAIN, "Orientation", oTag) && oTag.isValid()) {
    ODA_ASSERT(oTag.getType() == FIDT_SHORT);
    OdUInt16 getShort = 0;
    getShort = *(OdUInt16*)oTag.getValue();
    switch (getShort)
    {
      case 2: // (*unused) top-right (do horizontal mirror)
        fiImage.flipHorizontal();
      break;
      case 3: // bottom-right (rotate 180 CW)
        fiImage.rotate(180.0);
      break;
      case 4: // (*unused) bottom-left (do vertical mirror)
        fiImage.flipVertical();
      break;
      case 5: // (*unused) left-top (rotate and mirror)
        fiImage.rotate(90.0);
        fiImage.flipVertical();
      break;
      case 6: // right-top (rotate 270 CW)
        fiImage.rotate(-90.0);
      break;
      case 7: // (*unused) right-bottom (rotate and mirror)
        fiImage.rotate(-90.0);
        fiImage.flipVertical();
      break;
      case 8: // left-bottom (rotate 90 CW)
        fiImage.rotate(90.0);
      break;
      // case 1: // do nothing
      // default:
    }
  }
}

#endif // RASTER_FREE_IMAGE

///////////////////////////////////////////////////////////////////
// Helper class OdGiSwappedRBImage - swaps RGB-BGR and RGBA-BGRA

struct OdGiSwappedRBImage : public OdGiRasterImage
{
  const OdGiRasterImage *m_pImg;

  OdUInt32 pixelWidth() const     { return m_pImg->pixelWidth(); }

  OdUInt32 pixelHeight() const    { return m_pImg->pixelHeight(); }

  Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const 
    { return m_pImg->defaultResolution(xPelsPerUnit, yPelsPerUnit); }

  OdUInt32 colorDepth() const     { return m_pImg->colorDepth(); }

  OdUInt32 numColors() const      { return m_pImg->numColors(); }

  int transparentColor() const    { return m_pImg->transparentColor(); }

  ODCOLORREF color(OdUInt32 colorIndex) const { return m_pImg->color(colorIndex); }

  OdUInt32 paletteDataSize() const { return numColors() * sizeof(RGBQUAD); }

  void paletteData(OdUInt8* bytes) const
  {
    RGBQUAD *pPalette = (RGBQUAD*)bytes;
    for (OdUInt32 nColor = 0; nColor < numColors(); nColor++)
    {
      ODCOLORREF decodedColor = color(nColor);
      pPalette[nColor].rgbBlue = ODGETBLUE(decodedColor);
      pPalette[nColor].rgbGreen = ODGETGREEN(decodedColor);
      pPalette[nColor].rgbRed = ODGETRED(decodedColor);
      pPalette[nColor].rgbReserved = 0;
    }
  }
  
  OdUInt32 scanLineSize() const { return m_pImg->scanLineSize(); }
  const OdUInt8* scanLines() const
  {
    if (m_pImg->colorDepth() <= 8)
      return m_pImg->scanLines();
    return NULL;
  }
  
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const
  {
    if (m_pImg->colorDepth() <= 8)
      m_pImg->scanLines(scnLines, firstScanline, numLines);
    else
    {
      const OdUInt32 nScanSize = m_pImg->scanLineSize();
      const OdUInt32 nScanPix = m_pImg->pixelWidth();
      OdUInt8Array origScan; origScan.resize(nScanSize);
      OdUInt8 *pScanIn = origScan.asArrayPtr();
      const OdUInt32 nBytes = m_pImg->colorDepth() / 8;
      OdUInt32 nRedByte = m_pImg->pixelFormat().redOffset / 8;
      OdUInt32 nBlueByte = m_pImg->pixelFormat().blueOffset / 8;
      for (OdUInt32 numLine = 0; numLine < numLines; numLine++, scnLines += nScanSize, pScanIn += nScanSize)
      {
        m_pImg->scanLines(pScanIn, firstScanline + numLine);
        OdUInt8 *pOut = scnLines;
        const OdUInt8 *pIn = pScanIn;
        for (OdUInt32 nPix = 0; nPix < nScanPix; nPix++, pOut += nBytes, pIn += nBytes)
        {
          const OdUInt8 *pRedIn = pIn, *pBlueIn = pIn; OdUInt8 *pRedOut = pOut, *pBlueOut = pOut;
          for (OdUInt32 nByte = 0; nByte < nBytes; nByte++)
          {
            if (nByte == nRedByte) { pRedIn = pIn + nByte; pBlueOut = pOut + nByte; }
            else if (nByte == nBlueByte) { pBlueIn = pIn + nByte; pRedOut = pOut + nByte; }
            else pOut[nByte] = pIn[nByte];
          }
          *pRedOut = *pRedIn;
          *pBlueOut = *pBlueIn;
        }
      }
    }
  }

  PixelFormatInfo pixelFormat() const
  {
    PixelFormatInfo res;
    PixelFormatInfo src = m_pImg->pixelFormat();
    if (src.isRGB())
      res.setBGR();
    else if (src.isRGBA())
      res.setBGRA();
    else if (src.isBGRA())
      res.setRGBA();
    else if (src.isBGR())
      res.setRGB();

    return res;
  }

  OdUInt32 scanLinesAlignment() const { return m_pImg->scanLinesAlignment(); }

  ImageSource imageSource() const { return m_pImg->imageSource(); }

  const OdString &sourceFileName() const { return m_pImg->sourceFileName(); }

  TransparencyMode transparencyMode() const { return m_pImg->transparencyMode(); }
};

OdGiRasterImagePtr ExRasterModule::loadRasterImage(OdStreamBuf *pBuf, const OdUInt32 *pFlagsChain)
{
  OdExGiRasterImagePtr pExRastIm = OdRxObjectImpl<OdExGiRasterImage>::createObject();
  OdExGiRasterImage* pRaster = pExRastIm;

  OdExGiRasterImage& bitmap = *pRaster;

  OdUInt32 preferFormat = (OdUInt32)kUnknown;
  if (::flagsChainContains(pFlagsChain, kLoadFmt))
    preferFormat = ::flagsChainGetUInt(pFlagsChain, kLoadFmt);

  OdUInt64 startPos = pBuf->tell();
  OdInt16 type = OdPlatformStreamer::rdInt16(*pBuf); // type
  if(type != 19778) // BMP
  {
#if defined(RASTER_SNOW_BOUND)
    pBuf->seek(startPos,OdDb::kSeekFromStart);
    std::vector<char> buf;
    buf.resize((unsigned int)(pBuf->length() - startPos));
    pBuf->getBytes(&buf[0], (OdUInt32)buf.size());
    if (preferFormat != kUnknown)
    {
      int typeIn = IMGLOW_get_filetype_mem(&buf[0]);
      int typeOut = imageTypeToSBRasterType(preferFormat);
      if (typeIn != typeOut)
        return OdGiRasterImagePtr();
    }
    int imghandle = IMGLOW_decompress_bitmap_mem(&buf[0],0,raster_to_dib, pExRastIm, set_header );
    if(imghandle > 0)
    {
      IMG_delete_bitmap(imghandle);
    }
    if (imghandle >= 0)
      return pExRastIm;
    else
      return OdGiRasterImagePtr();
#elif defined(RASTER_FREE_IMAGE)
    pBuf->seek(startPos,OdDb::kSeekFromStart);
    bool bIsTIFF = (!::flagsChainContains(pFlagsChain, kNoTIFFRotation)) ? ::identifyTIFF(type, pBuf) : false;
    FreeImageIO *pAdapter = ::createNewStreamFIAdapter(pBuf);
    if (preferFormat != kUnknown)
    {
      FREE_IMAGE_FORMAT fifIn = fipImage::identifyFIFFromHandle(pAdapter, (fi_handle)pAdapter);
      int tmpFlags = 0;
      FREE_IMAGE_FORMAT fifOut = ::mapImageTypeToFIFormat(preferFormat, tmpFlags);
      if (fifIn != fifOut)
        return OdGiRasterImagePtr();
    }
    fipImage fiImage;
    if (fiImage.loadFromHandle(pAdapter, (fi_handle)pAdapter) != FALSE)
    {
      ::releaseStreamFIAdapter(pAdapter);
      if (bIsTIFF)
        ::applyTIFFOrientation(fiImage);
      ::convertFIImageToGiRasterImage(fiImage, bitmap);
      return pExRastIm;
    }
    ::releaseStreamFIAdapter(pAdapter);
    return OdGiRasterImagePtr();
#else
  // scanLines.clear();
    bitmap.bits().clear();
    return OdGiRasterImagePtr();
#endif
  }

  if ((preferFormat != kUnknown) && (preferFormat != kBMP))
    return OdGiRasterImagePtr();

  OdUInt32 size = OdPlatformStreamer::rdInt32(*pBuf);
  OdPlatformStreamer::rdInt32(*pBuf); // reserved
  OdUInt32 scanLinesPos = OdPlatformStreamer::rdInt32(*pBuf); // offBits
  
  // This is a header(OdGiBitmapInfoHeader or OdGiBitmapCoreHeader) position
  //OdUInt32 headerPos = pFileBuf->tell();
  OdUInt32 headerLen = OdPlatformStreamer::rdInt32(*pBuf);
  OdUInt32 compr(0L);
  OdUInt32 width, height;
  OdUInt8 colDepth;
  OdInt32 signedSize;
  bool bUpsideDown = false;
  if(headerLen == 40 || headerLen == 56)
  {
    // OdGiBitmapInfoHeader used
    // header.m_header.m_size = headerLen;
    signedSize = OdPlatformStreamer::rdInt32(*pBuf);;
    ODA_ASSERT(signedSize > 0);
    width  = (OdUInt32)signedSize;
    signedSize = OdPlatformStreamer::rdInt32(*pBuf);
    // AMark : #6001 : BITMAPINFOHEADER could have negative height (BTW, compressed images could not have negative height)
    ODA_ASSERT(signedSize != 0);
    if (signedSize < 0)
    {
      signedSize = -signedSize;
      bUpsideDown = true;
    }
    height = (OdUInt32)signedSize;
    //header.m_header.m_planes =
    OdPlatformStreamer::rdInt16(*pBuf);
    colDepth = (OdUInt8)OdPlatformStreamer::rdInt16(*pBuf);
    compr = OdPlatformStreamer::rdInt32(*pBuf);
    //header.m_header.m_sizeImage =
    OdPlatformStreamer::rdInt32(*pBuf);
    double xPelsPerMeter = OdPlatformStreamer::rdInt32(*pBuf);
    double yPelsPerMeter = OdPlatformStreamer::rdInt32(*pBuf);
    bitmap.setDefaultResolution(OdGiRasterImage::kMeter, xPelsPerMeter, yPelsPerMeter);

    //header.m_header.m_clrUsed =
    OdPlatformStreamer::rdInt32(*pBuf);
    //header.m_header.m_clrImportant =
    OdPlatformStreamer::rdInt32(*pBuf);

    bitmap.setMetrics(width, height, colDepth);
  }
  else
  {
    // OdGiBitmapCoreHeader used
    ODA_ASSERT(headerLen == 12);
    // header.m_header.m_size = headerLen;
    width = (OdUInt32)OdPlatformStreamer::rdInt16(*pBuf);
    height = (OdUInt32)OdPlatformStreamer::rdInt16(*pBuf);
    /*
    header.m_header.m_planes =
    */
    OdPlatformStreamer::rdInt16(*pBuf);
    colDepth = (OdUInt8)OdPlatformStreamer::rdInt16(*pBuf);
    /*
    header.m_header.m_compression = 0L; // No compression
    header.m_header.m_sizeImage = 0L; // default
    header.m_header.m_xPelsPerMeter = 0L; // default
    header.m_header.m_yPelsPerMeter = 0L; // default
    header.m_header.m_clrUsed = 0L; // default
    header.m_header.m_clrImportant = 0L; // default
    */
    bitmap.setMetrics(width, height, colDepth);
  }
  if(compr > 3L) // BI_RGB, BI_RLE8, BI_RLE4, BI_BITFIELDS supported
  {
    bitmap.bits().clear();
    return OdGiRasterImagePtr(); // such bitmaps is not supported yet
  }
  // palette
  if(scanLinesPos!=0) // if zero -- there is no palette (cr1475, GU)
  {
    OdUInt32 paletteSize = OdUInt32(scanLinesPos - (pBuf->tell() - startPos));
    bitmap.palette().data().resize(paletteSize);
    if(paletteSize)
      pBuf->getBytes(bitmap.palette().data().asArrayPtr(), paletteSize);
  }
  OdUInt32 scanLinesSize = bitmap.scanLineSize() * height;
  bitmap.bits().resize(scanLinesSize);
  if (compr == 0L)
  {
    // no compression
    try { // AutoCAD nevermind about end of file, it doesn't fill pixels by anything in this case.
      if (!bUpsideDown)
      {
        pBuf->getBytes(bitmap.bits().asArrayPtr(), scanLinesSize);
      }
      else
      {
        OdUInt32 scanLineSize = scanLinesSize / height;
        OdUInt8 *pBits = bitmap.bits().asArrayPtr() + (scanLinesSize - scanLineSize);
        for (OdUInt32 y = 0; y < height; y++, pBits -= scanLineSize)
        {
          pBuf->getBytes(pBits, scanLineSize);
        }
      }
    }
    catch (OdError_FileException ex) {
      if (ex.code() != eEndOfFile)
        throw;
    }
  }
  else if (compr == 1L || compr == 2L)
  {
    ODA_ASSERT(!bUpsideDown); // Compressed images could not have negative height (MSDN)
    // read compressed bitmap (RLE8 or RLE4)
    OdLoadCompressedBMPData(pBuf, bitmap.bits().asArrayPtr(), compr, scanLinesSize / height, width, height);
  }
  else
  {
    ODA_ASSERT(!bUpsideDown); // Compressed images could not have negative height (MSDN)
    // read compressed bitmap (BITFIELDS)
    OdLoadBitFieldsBMPData(pBuf, bitmap, scanLinesSize / height, width, height);
  }

  return pExRastIm; 
}

inline OdGiRasterImagePtr embedFileName(const OdString &fileName, OdGiRasterImagePtr image)
{
  if (!image.isNull())
    image->changeImageSource(OdGiRasterImage::kFromFile, fileName);
  return image;
}

OdGiRasterImagePtr ExRasterModule::loadRasterImage(const OdString &fileName, const OdUInt32 *pFlagsChain)
{
  OdUInt32 preferFormat = (OdUInt32)kUnknown;
  if (::flagsChainContains(pFlagsChain, kLoadFmt))
    preferFormat = ::flagsChainGetUInt(pFlagsChain, kLoadFmt);

#if defined(RASTER_FREE_IMAGE)
  try
  {

#ifdef _WIN32
    // Only for this configuration Unicode fully supported
    FREE_IMAGE_FORMAT fiFormat = fipImage::identifyFIFU((const wchar_t*)fileName);
#else
    if (::isFileNameNonAscii(fileName))
    {
      // Can't use with non-unicode methods
      OdStreamBufPtr pFileBuf = odrxSystemServices()->createFile(fileName);
      return ::embedFileName(fileName, loadRasterImage(pFileBuf, pFlagsChain));
    }
    FREE_IMAGE_FORMAT fiFormat = fipImage::identifyFIF((const char*)fileName);
#endif
    if (preferFormat != kUnknown)
    {
      int tmpFlags = 0;
      if (fiFormat != ::mapImageTypeToFIFormat(preferFormat, tmpFlags))
        return OdGiRasterImagePtr();
    }
    if (fiFormat == FIF_BMP)
    { // Process by internal reader
      OdStreamBufPtr pFileBuf = odrxSystemServices()->createFile(fileName);
      return ::embedFileName(fileName, loadRasterImage(pFileBuf, pFlagsChain));
    }
    else if (fiFormat != FIF_UNKNOWN)
    {
      OdExGiRasterImagePtr pExRastIm = OdRxObjectImpl<OdExGiRasterImage>::createObject();
      OdExGiRasterImage* pRaster = pExRastIm;
      OdExGiRasterImage& bitmap = *pRaster;
      fipImage fiImage;
#ifdef _WIN32
      if (fiImage.loadU((const wchar_t*)fileName) != FALSE)
#else
      if (fiImage.load((const char*)fileName) != FALSE)
#endif
      {
        if ((fiFormat == FIF_TIFF) && (!::flagsChainContains(pFlagsChain, kNoTIFFRotation)))
          ::applyTIFFOrientation(fiImage);
        ::convertFIImageToGiRasterImage(fiImage, bitmap);
        return ::embedFileName(fileName, pExRastIm);
      }
    }

  }
  catch (...)
  {}
  return OdGiRasterImagePtr();

#elif defined(RASTER_JPEG6B)

  {
    OdString right3 = fileName.right(3);
    OdString right4 = fileName.right(4);
    right3.makeLower();
    right4.makeLower();

    if((right3 == OD_T("jpg")) || (right4 == OD_T("jpeg")))
    {
      if ((preferFormat != kUnknown) && (preferFormat != kJPEG))
        return OdGiRasterImagePtr();
      return ::embedFileName(fileName, loadJpegImage(fileName));
    }
    else if((right3 == OD_T("bmp")) || (right3 == OD_T("dib")) || (right3 == OD_T("rle")))
    {
      if ((preferFormat != kUnknown) && (preferFormat != kBMP))
        return OdGiRasterImagePtr();
      OdStreamBufPtr pFileBuf;
      try
      {
        pFileBuf = odrxSystemServices()->createFile(fileName);
      }
      catch (...)
      {
        return OdGiRasterImagePtr();
      }
      return ::embedFileName(fileName, loadRasterImage(pFileBuf, pFlagsChain));
    }
    else
    {
      return OdGiRasterImagePtr();
    }
  }

#elif defined(RASTER_SNOW_BOUND)

  {
    OdExGiRasterImagePtr pExRastIm = OdRxObjectImpl<OdExGiRasterImage>::createObject();
    HFILE fd = _lopen( fileName, OF_READ );
    if(fd!=HFILE_ERROR)
    {
      if (preferFormat != kUnknown)
      {
        int typeIn = getSBRasterType(fileName);
        int typeOut = imageTypeToSBRasterType(preferFormat);
        if (typeIn != typeOut)
          return OdGiRasterImagePtr();
      }
      int imghandle = IMGLOW_decompress_bitmap( fd, 0, 0, raster_to_dib, 
        pExRastIm, set_header );
      _lclose( fd );
      // int imghandle = IMG_decompress_bitmap((char*)name.c_str());
      if(imghandle > 0)
      {
        IMG_delete_bitmap(imghandle);
      }
      if (imghandle >= 0)
        return ::embedFileName(fileName, pExRastIm);
    }
    return OdGiRasterImagePtr(); // File not found, or format is not supported
  }

#else

  OdString right3 = fileName.right(3);
  right3.makeLower();
  if((right3 == OD_T("bmp")) || (right3 == OD_T("dib")) || (right3 == OD_T("rle")))
  {
    if ((preferFormat != kUnknown) && (preferFormat != kBMP))
      return OdGiRasterImagePtr();
    OdStreamBufPtr pFileBuf;
    try
    {
      pFileBuf = odrxSystemServices()->createFile(fileName);
    }
    catch (...)
    {
      return OdGiRasterImagePtr();
    }
    return ::embedFileName(fileName, loadRasterImage(pFileBuf, pFlagsChain));
  }
  else
  {
    return OdGiRasterImagePtr();
  }

#endif
}

#ifdef RASTER_SNOW_BOUND
static int getSBRasterType( const OdString& s )
{
  // return IMGLOW_get_filetype( const_cast<char*>( s.c_str() ) );
  OdString ext = s.right(4);
  ext.makeLower();
  if ( ext == OD_T(".tif") || ext == OD_T("tiff") )
    return TIFF_UNCOMPRESSED;
  else if ( ext == OD_T(".bmp") )
    return BMP_UNCOMPRESSED;
  else if ( ext == OD_T(".gif") )
    return GIF;
  else if ( ext == OD_T(".jpg") || ext == OD_T("jpeg") )
    return JPEG;
  else if ( ext == OD_T(".png") )
    return PNG;
  else return -1;
}

static OdUInt32 SBRasterTypeToImageType(int nSb)
{
  switch (nSb)
  {
    case TIFF_UNCOMPRESSED:
    return OdRxRasterServices::kTIFF;
    case BMP_UNCOMPRESSED:
    return OdRxRasterServices::kBMP;
    case GIF:
    return OdRxRasterServices::kGIF;
    case JPEG:
    return OdRxRasterServices::kJPEG;
    case PNG:
    return OdRxRasterServices::kPNG;
  }
  return OdRxRasterServices::kUnknown;
}

static int imageTypeToSBRasterType(OdUInt32 type)
{
  switch (type)
  {
    case OdRxRasterServices::kTIFF:
    return TIFF_UNCOMPRESSED;
    case OdRxRasterServices::kBMP:
    return BMP_UNCOMPRESSED;
    case OdRxRasterServices::kGIF:
    return GIF;
    case OdRxRasterServices::kJPEG:
    return JPEG;
    case OdRxRasterServices::kPNG:
    return PNG;
  }
  return -1;
}

int get_dib_data( char FAR *private_data, char FAR *dst_ptr, int ypos, int rast_size )
{
  const OdGiRasterImage* img = (const OdGiRasterImage*)private_data;
  img->scanLines((OdUInt8*)dst_ptr, img->pixelHeight() - ypos - 1);
  return rast_size;
}
#endif // RASTER_SNOW_BOUND

// save as bmp
static void saveBitmap(const OdGiRasterImage* img, OdStreamBuf* pFileBuf)
{
  // saving file as bmp BITMAPINFOHEADER
  // (let code be crossplatform)
  OdUInt32 imgWidth(img->pixelWidth()), imgHeight(img->pixelHeight());
  double xPelsPerUnut, yPelsPerUnut;
  img->defaultResolution(xPelsPerUnut, yPelsPerUnut);
  OdUInt16 colDepth = OdUInt16(img->colorDepth());
  OdUInt32 palSz(img->paletteDataSize());
  OdUInt32 scLnSz(img->scanLineSize());
  const OdUInt32 bmpScSize = OdGiRasterImage::calcBMPScanLineSize(imgWidth, colDepth);

  OdPlatformStreamer::wrInt16(*pFileBuf, 0x4d42);
  OdUInt32 scanLinesPos = 14 + 40 + palSz;  // Below version is correct only on Windows
  //(OdUInt32)sizeof(BITMAPFILEHEADER) + (OdUInt32)sizeof(BITMAPINFOHEADER) + palSz;
  OdUInt32 size = scanLinesPos + bmpScSize*imgHeight;
  OdPlatformStreamer::wrInt32(*pFileBuf, size);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0); // reserved
  OdPlatformStreamer::wrInt32(*pFileBuf, scanLinesPos); // offBits
  OdPlatformStreamer::wrInt32(*pFileBuf, 40);  // not portable: sizeof(BITMAPINFOHEADER));
  // save BITMAPINFOHEADER
  OdPlatformStreamer::wrInt32(*pFileBuf, imgWidth);
  OdPlatformStreamer::wrInt32(*pFileBuf, imgHeight);
  OdPlatformStreamer::wrInt16(*pFileBuf, 1);
  OdPlatformStreamer::wrInt16(*pFileBuf, colDepth);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0);
  OdPlatformStreamer::wrInt32(*pFileBuf, (OdInt32)xPelsPerUnut);
  OdPlatformStreamer::wrInt32(*pFileBuf, (OdInt32)yPelsPerUnut);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0);

  // palette
  OdUInt8Array palette;
  palette.resize(palSz);
  img->paletteData(palette.asArrayPtr());
  pFileBuf->putBytes(palette.asArrayPtr(), palSz);

  // scanLines
  
  const OdUInt8* pScLns = img->scanLines();
  if (pScLns && (scLnSz == bmpScSize))
  {
    // we have pointer to data in bmp format
    pFileBuf->putBytes(pScLns, scLnSz * imgHeight);
  }
  else
  {
    // loop - to not allocate very big chunk
    OdUInt8Array scLn;
    scLn.resize(bmpScSize, 0);
    OdUInt8 *pPixels = scLn.asArrayPtr();
    for(OdUInt32 i = 0; i < imgHeight; i ++)
    {
      img->scanLines(pPixels, i);
      pFileBuf->putBytes(pPixels, bmpScSize);
    }
  }
}
static void saveBitmap(const OdGiRasterImage* img, const OdString& path)
{
  OdStreamBufPtr pFileBuf =
    ::odrxSystemServices()->createFile(path, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
  ::saveBitmap(img, pFileBuf);
}

bool ExRasterModule::saveRasterImage(const OdGiRasterImage* rasterImage, const OdString& path,
                                     const OdUInt32 *pFlagsChain)
{
  OdUInt32 type = (OdUInt32)kUnknown;
  if (!path.right(4).iCompare(OD_T(".bmp")))
    type = kBMP;
#if defined(RASTER_JPEG6B)
  else if (!path.right(4).iCompare(OD_T(".jpg")) || !path.right(5).iCompare(OD_T(".jpeg")))
    type = kJPEG;
#elif defined(RASTER_SNOW_BOUND)
  else
  {
    int sbType = getSBRasterType(path);
    type = SBRasterTypeToImageType(sbType);
  }
#elif defined(RASTER_FREE_IMAGE)
  else
    type = ::mapExtensionToImageType(path);
#endif
  if (type == kUnknown)
    return false;
  return saveRasterImage(rasterImage, path, type, pFlagsChain);
}

bool ExRasterModule::saveRasterImage(const OdGiRasterImage* img, const OdString& path,
                                     OdUInt32 type, const OdUInt32 *pFlagsChain)
{
  OdGiRasterImagePtr pConvertedImg;
  if (img->pixelFormat().isRGB() || img->pixelFormat().isRGBA())
  { // Do RGB->BGR conversion
    pConvertedImg = OdRxObjectImpl<OdGiSwappedRBImage>::createObject();
    static_cast<OdGiSwappedRBImage*>(pConvertedImg.get())->m_pImg = img;
    img = pConvertedImg.get();
  }

  if (type == kBMP)
  {
    ::saveBitmap(img, path);
    return true;
  }
#if defined(RASTER_JPEG6B)
  if (type == kJPEG)
  {
    OdStreamBufPtr pSb = ::odrxSystemServices()->createFile(path, Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
    if (pSb.isNull())
      return false;
    return convertRasterImage(img, OdRxRasterServices::kJPEG, pSb, pFlagsChain);
  }
  return false;
#elif defined(RASTER_SNOW_BOUND)
  int typeSB =  imageTypeToSBRasterType( type );
  if ( typeSB < 0 ) return false; // unknown extension
  int fd = _lcreat( path, OF_WRITE );
  OdBinaryData headerMemCont;
  headerMemCont.resize( sizeof(BITMAPINFOHEADER) + 4 * img->numColors() );
  BITMAPINFO* pHeader = (BITMAPINFO*)headerMemCont.getPtr();
  // img->getBitmapInfoHeader( pHeader->bmiHeader );
  pHeader->bmiHeader.biBitCount     = img->colorDepth();
  pHeader->bmiHeader.biClrImportant = 0;
  pHeader->bmiHeader.biClrUsed      = 0;
  pHeader->bmiHeader.biCompression  = 0;
  pHeader->bmiHeader.biHeight       = img->pixelHeight();
  pHeader->bmiHeader.biPlanes       = 1;
  pHeader->bmiHeader.biSize         = 40;
  pHeader->bmiHeader.biSizeImage    = 0;
  pHeader->bmiHeader.biWidth        = img->pixelWidth();
  double xPelsPerMeter = 0, yPelsPerMeter = 0;
  img->defaultResolution(xPelsPerMeter, yPelsPerMeter);
  pHeader->bmiHeader.biXPelsPerMeter = (long)xPelsPerMeter;
  pHeader->bmiHeader.biYPelsPerMeter = (long)yPelsPerMeter;

  /*OdUInt32 pal_size = 0;
  memcpy( pHeader->bmiColors, img->getPaletteData( pal_size ), img->numColors()*4);*/
  img->paletteData((OdUInt8*)pHeader->bmiColors);
  int retval = IMGLOW_save_bitmap( fd, &pHeader->bmiHeader, typeSB, get_dib_data, (void*)img );
  _lclose( fd );
  if ( retval < 0 ) 
  {
    ODA_TRACE1( "IMGLOW_save_bitmap() failed, error code = %d\n", retval );
  }
  return retval >= 0;
#elif defined(RASTER_FREE_IMAGE)
  int fiFlags;
  FREE_IMAGE_FORMAT fiFormat = ::mapImageTypeToFIFormat(type, fiFlags, pFlagsChain);
  if (fiFormat != FIF_UNKNOWN && img != NULL)
  {
    fipImage fiImage;
    ::fiImageFromGiRasterImage(img, fiImage, fiFormat, pFlagsChain);
    ::fiImageTransform(fiImage, pFlagsChain);
#ifdef _WIN32
    return fiImage.saveU((const wchar_t*)path, fiFlags) != FALSE;
#else
    if (!isFileNameNonAscii(path))
      return fiImage.save((const char*)path, fiFlags) != FALSE;
    else
    {
      // File name contains unicode characters, try to save via ODA file
      OdStreamBufPtr pSb = ::odrxSystemServices()->createFile(path, Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
      if (pSb.isNull())
        return false;
      FreeImageIO *pAdapter = ::createNewStreamFIAdapter(pSb, true);
      bool bResult = fiImage.saveToHandle(fiFormat, pAdapter, (fi_handle)pAdapter, fiFlags) != FALSE;
      ::releaseStreamFIAdapter(pAdapter);
      return bResult;
    }
#endif
  }
  return false;
#else
  return false;
#endif
}

#ifdef RASTER_JPEG6B

////////////////////////////////////////////////////////
class CJpeg6bConvertor
{
protected:
  jpeg_error_mgr          m_JErrMgr;
  jmp_buf                 m_jmp_buf;

  ///////////////////////////////////////////////////////////
  // Error handling
  static void f_error_exit (j_common_ptr cinfo);
  static void f_output_message (j_common_ptr cinfo);

  virtual j_common_ptr  GetCommonInfo() = 0;
  void    Init();

public:
  CJpeg6bConvertor()  {}
    virtual ~CJpeg6bConvertor()
    {}
};


void CJpeg6bConvertor::Init()
{
  GetCommonInfo()->err = jpeg_std_error(&m_JErrMgr);
  m_JErrMgr.error_exit = f_error_exit;
  m_JErrMgr.output_message = f_output_message;
  GetCommonInfo()->client_data = this;
}

/////////////////////////////////////////////////////////////////
// Error handling

void CJpeg6bConvertor::f_error_exit (j_common_ptr cinfo)
{
  CJpeg6bConvertor * pC = (CJpeg6bConvertor *)cinfo->client_data;
//  TRACE("Error signaled by JPEG library");  
  longjmp(pC->m_jmp_buf, 1);
}

void CJpeg6bConvertor::f_output_message (j_common_ptr cinfo)
{
//#ifdef _DEBUG
//  char buf[512];
//  (cinfo->err->format_message)(cinfo, buf);
//  TRACE(buf);
//#endif
}



////////////////////////////////////////////////////////
class CJpeg6bCompressor : public CJpeg6bConvertor,
                          public jpeg_compress_struct
{
  virtual j_common_ptr  GetCommonInfo()
  {
    return (j_common_ptr)(jpeg_compress_struct*)this;
  }

  ///////////////////////////////////////////////////////////
  // Output data source
  static void init_destination (j_compress_ptr cinfo);
  static boolean empty_output_buffer (j_compress_ptr cinfo);
  static void term_destination (j_compress_ptr cinfo);

  OdUInt8 *         m_pOutBuf;
  long                    m_nOutBufSize;

  jpeg_destination_mgr    m_JDestinationMgr;

public:
    CJpeg6bCompressor();
    ~CJpeg6bCompressor();

    long Compress(unsigned int Height, unsigned int Width, const OdUInt8 * pData,
                    OdUInt8 ** pOutput, int Quality = -1);
};


//////////////////////////////////////////////////////////////////////////
// Implementation

CJpeg6bCompressor::CJpeg6bCompressor()
    : CJpeg6bConvertor()
    , m_pOutBuf(NULL)
    , m_nOutBufSize(0)
{
  Init();
  jpeg_create_compress(this);

  dest = & m_JDestinationMgr;

  m_JDestinationMgr.init_destination = init_destination;
  m_JDestinationMgr.empty_output_buffer = empty_output_buffer;
  m_JDestinationMgr.term_destination = term_destination;
}

CJpeg6bCompressor::~CJpeg6bCompressor()
{
  jpeg_destroy_compress(this);
  if (m_pOutBuf)
    delete [] m_pOutBuf;
}




////////////////////////////////////////////////////////////////////
// Output data source
void CJpeg6bCompressor::init_destination (j_compress_ptr cinfo)
{
  CJpeg6bCompressor * pC= (CJpeg6bCompressor*)cinfo;
  if (pC->m_pOutBuf == NULL)
  {
    pC->m_nOutBufSize = pC->image_width * pC->image_height * 3 / 10;
    pC->m_pOutBuf = new OdUInt8[pC->m_nOutBufSize];
    if (pC->m_pOutBuf == NULL)
      f_error_exit((j_common_ptr)cinfo);
  }
  pC->m_JDestinationMgr.next_output_byte = (JOCTET*)pC->m_pOutBuf;
  pC->m_JDestinationMgr.free_in_buffer = pC->m_nOutBufSize;
}

boolean CJpeg6bCompressor::empty_output_buffer (j_compress_ptr cinfo)
{
  // Not enough memory was allocated
  CJpeg6bCompressor * pC= (CJpeg6bCompressor*)cinfo;
  long nNewSize = pC->m_nOutBufSize *
    (pC->image_height * 3) / (odmax(pC->next_scanline, 1) * 2);

  OdUInt8 * pNewBuf = new OdUInt8[nNewSize];
  if (pNewBuf == NULL)
    f_error_exit((j_common_ptr)cinfo);

  memcpy(pNewBuf, pC->m_pOutBuf, pC->m_nOutBufSize);
  delete [] pC->m_pOutBuf;
  pC->m_pOutBuf = pNewBuf;
  pC->m_JDestinationMgr.next_output_byte = (JOCTET*)pC->m_pOutBuf;
  pC->m_JDestinationMgr.free_in_buffer = nNewSize - pC->m_nOutBufSize;
  pC->m_nOutBufSize = nNewSize;
  return TRUE;
}

void CJpeg6bCompressor::term_destination (j_compress_ptr cinfo)
{
}


//////////////////////////////////////////////////////////////////
long CJpeg6bCompressor::Compress( unsigned int Height, unsigned int Width,
                                  const OdUInt8 * pData, OdUInt8 ** pOutput, int Quality )
{
  if (setjmp(m_jmp_buf))
  {   // Error signalled
    return 0;
  }
  image_width = Width;
  image_height = Height;
	input_components = 3;
	in_color_space = JCS_RGB;
    jpeg_set_defaults(this);

	if (Quality != -1)
        jpeg_set_quality(this, Quality, TRUE);

    jpeg_start_compress(this, TRUE);

	JSAMPROW row_pointer[1];

  {
    row_pointer[0] = (JSAMPLE*)pData;

    while (next_scanline < Height)
    {
      jpeg_write_scanlines(this, row_pointer, 1);
      row_pointer[0] += Width * 3;
    }
  }

  jpeg_finish_compress(this);

  long nOutputLen = (long)(m_nOutBufSize - m_JDestinationMgr.free_in_buffer);
  *pOutput = m_pOutBuf;
  m_pOutBuf = NULL;   // Detach memory
  m_nOutBufSize = 0;
  return nOutputLen;
}	

#endif


#if !defined(RASTER_FREE_IMAGE)

static bool convertRasterImageToJPEG( OdGiRasterImagePtr pRaster, OdUInt32 /*type*/, OdStreamBuf* pStreamBuf)
{
  int nColorBits = pRaster->colorDepth();

  // for RGB only
  if (nColorBits != 24)
  {
    return false;
  }

  OdUInt32 imageHeight = pRaster->pixelHeight();
  OdUInt32 imageWidth  = pRaster->pixelWidth();
  int outClrDepth = nColorBits;

  OdUInt32 scanlineWidth = pRaster->scanLineSize();

  OdBinaryData scanLineBuf;
  scanLineBuf.resize(scanlineWidth);
  OdUInt8* scanLine = scanLineBuf.asArrayPtr();

  OdUInt32 outScLnSz = outClrDepth * imageWidth / 8;
  if(outClrDepth * imageWidth % 8)
    outScLnSz ++;

  OdUInt8Array outScLns;
  outScLns.resize(outScLnSz * imageHeight);

  memset(outScLns.asArrayPtr(), 0, outScLnSz * imageHeight);
  for (OdUInt32 indexHeight = 0; indexHeight < imageHeight; indexHeight++)
  {
    pRaster->scanLines(scanLine, imageHeight-indexHeight-1);
    OdUInt8* pDestCurScLn = outScLns.asArrayPtr() + outScLnSz*indexHeight;

    for (OdUInt32 i = 0; i < imageWidth; i ++)
    {
      OdUInt8 red(0), green(0), blue(0);
      
      const OdUInt8* ptr = scanLine + i * nColorBits / 8;

      red   = ptr[2];
      green = ptr[1];
      blue  = ptr[0];

      OdUInt8* dstPtr = pDestCurScLn + i*outClrDepth/8;
      dstPtr[0] = red;
      dstPtr[1] = green;
      dstPtr[2] = blue;
    }
  }

#if defined(RASTER_JPEG6B)

  CJpeg6bCompressor JpegCompressor;
  OdUInt8 * pJpegData;

  OdUInt32 nOutputLen = JpegCompressor.Compress( imageHeight, imageWidth, 
                                                 (const OdUInt8*)outScLns.asArrayPtr(), 
                                                 &pJpegData );

  if ( !nOutputLen ){
    return false;
  }

  pStreamBuf->putBytes( pJpegData, nOutputLen );

  return true;

#elif defined(RASTER_SNOW_BOUND)

  return false;

#else
  return false;
#endif
}

#else // !defined(RASTER_FREE_IMAGE)

static OdUInt32 mapFiFormatToImageType(FREE_IMAGE_FORMAT fiFormat)
{
  // Convert format back to ODA (there is no guarantee does ImageType correlate with FREE_IMAGE_FORMAT, so make mapping to be safe)
  switch (fiFormat)
  {
    case FIF_BMP: return OdRxRasterServices::kBMP;
    case FIF_ICO: return OdRxRasterServices::kICO;
    case FIF_JPEG: return OdRxRasterServices::kJPEG;
    case FIF_JNG: return OdRxRasterServices::kJNG;
    case FIF_KOALA: return OdRxRasterServices::kKOALA;
    case FIF_LBM: return OdRxRasterServices::kLBM;
    case FIF_MNG: return OdRxRasterServices::kMNG;
    case FIF_PBM: return OdRxRasterServices::kPBM;
    case FIF_PBMRAW: return OdRxRasterServices::kPBMRAW;
    case FIF_PCD: return OdRxRasterServices::kPCD;
    case FIF_PCX: return OdRxRasterServices::kPCX;
    case FIF_PGM: return OdRxRasterServices::kPGM;
    case FIF_PGMRAW: return OdRxRasterServices::kPGMRAW;
    case FIF_PNG: return OdRxRasterServices::kPNG;
    case FIF_PPM: return OdRxRasterServices::kPPM;
    case FIF_PPMRAW: return OdRxRasterServices::kPPMRAW;
    case FIF_RAS: return OdRxRasterServices::kRAS;
    case FIF_TARGA: return OdRxRasterServices::kTARGA;
    case FIF_TIFF: return OdRxRasterServices::kTIFF;
    case FIF_WBMP: return OdRxRasterServices::kWBMP;
    case FIF_PSD: return OdRxRasterServices::kPSD;
    case FIF_CUT: return OdRxRasterServices::kCUT;
    case FIF_XBM: return OdRxRasterServices::kXBM;
    case FIF_XPM: return OdRxRasterServices::kXPM;
    case FIF_DDS: return OdRxRasterServices::kDDS;
    case FIF_GIF: return OdRxRasterServices::kGIF;
    case FIF_HDR: return OdRxRasterServices::kHDR;
    case FIF_FAXG3: return OdRxRasterServices::kFAXG3;
    case FIF_SGI: return OdRxRasterServices::kSGI;
    case FIF_EXR: return OdRxRasterServices::kEXR;
    case FIF_J2K: return OdRxRasterServices::kJ2K;
    case FIF_JP2: return OdRxRasterServices::kJP2;
    case FIF_PFM: return OdRxRasterServices::kPFM;
    case FIF_PICT: return OdRxRasterServices::kPICT;
    case FIF_RAW: return OdRxRasterServices::kRAW;
  }
  return (OdUInt32)OdRxRasterServices::kUnknown;
}

static OdUInt32 mapExtensionToImageType(const OdString &fileName)
{
  // FreeImage supports Unicode only for WIN32, so convert to ANSI to unblock this functionality for other platforms
  OdAnsiString fromUnicode(fileName);
  FREE_IMAGE_FORMAT fiFormat = ::FreeImage_GetFIFFromFilename(fromUnicode.c_str());
  return mapFiFormatToImageType(fiFormat);
}

static FREE_IMAGE_FORMAT mapImageTypeToFIFormat(OdUInt32 type, int &flags, const OdUInt32 *pFlagsChain)
{
  flags = 0;
  switch (type)
  {
    // @@@TODO: implement for other types.
    case OdRxRasterServices::kBMP:
      flags = BMP_DEFAULT;
    return FIF_BMP;
    case OdRxRasterServices::kJPEG:
      if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kJpegQuality))
      {
        flags = JPEG_SUBSAMPLING_420;
        OdUInt32 qlty = ::flagsChainGetUInt(pFlagsChain, OdRxRasterServices::kJpegQuality);
        if (qlty < 20) flags |= JPEG_QUALITYBAD;
        else if (qlty < 40) flags |= JPEG_QUALITYAVERAGE;
        else if (qlty < 60) flags |= JPEG_QUALITYNORMAL;
        else if (qlty < 80) flags |= JPEG_QUALITYGOOD;
        else flags |= JPEG_QUALITYSUPERB;
      }
      else
        flags = JPEG_DEFAULT;
    return FIF_JPEG;
    // OdRxRasterServices::kJNG
    // OdRxRasterServices::kKOALA
    // OdRxRasterServices::kLBM
    // OdRxRasterServices::kIFF = kLBM,
    // OdRxRasterServices::kMNG
    // OdRxRasterServices::kPBM
    // OdRxRasterServices::kPBMRAW
    // OdRxRasterServices::kPCD
    // OdRxRasterServices::kPCX
    // OdRxRasterServices::kPGM
    // OdRxRasterServices::kPGMRAW
    case OdRxRasterServices::kPNG:
      flags = PNG_DEFAULT;
    return FIF_PNG;
    // OdRxRasterServices::kPPM
    // OdRxRasterServices::kPPMRAW
    // OdRxRasterServices::kRAS
    case OdRxRasterServices::kTARGA:
      flags = TARGA_DEFAULT;
    return FIF_TARGA;
    case OdRxRasterServices::kTIFF:
      if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kTiffCompression))
      {
        OdUInt32 compr = ::flagsChainGetUInt(pFlagsChain, OdRxRasterServices::kTiffCompression);
        switch (compr)
        {
          case OdRxRasterServices::kTiffCompressionDeflate: flags = TIFF_DEFLATE; break;
          case OdRxRasterServices::kTiffCompressionLzw: flags = TIFF_LZW; break;
          case OdRxRasterServices::kTiffCompressionJpeg: flags = TIFF_JPEG; break;
          case OdRxRasterServices::kTiffCompressionCCITTFax3: flags = TIFF_CCITTFAX3; break;
          case OdRxRasterServices::kTiffCompressionCCITTFax4: flags = TIFF_CCITTFAX4; break;
          // ODA extension for IbEnabler, actually it is CCITFax4 compression
          case OdRxRasterServices::kTiffCompressionEmbedded: flags = TIFF_JPEG | TIFF_NONE; break;
          default: flags = TIFF_NONE;
        }
      }
      else
        flags = TIFF_NONE;
    return FIF_TIFF;
    // OdRxRasterServices::kWBMP
    // OdRxRasterServices::kPSD
    // OdRxRasterServices::kCUT
    // OdRxRasterServices::kXBM
    // OdRxRasterServices::kXPM
    // OdRxRasterServices::kDDS
    case OdRxRasterServices::kGIF:
      flags = GIF_DEFAULT;
    return FIF_GIF;
    // OdRxRasterServices::kHDR
    // OdRxRasterServices::kFAXG3
    // OdRxRasterServices::kSGI
    // OdRxRasterServices::kEXR
    // OdRxRasterServices::kJ2K
    // OdRxRasterServices::kJP2
    // OdRxRasterServices::kPFM
    // OdRxRasterServices::kPICT
    // OdRxRasterServices::kRAW
    default:
      return FIF_UNKNOWN;
  }
}

static void fiImageFromGiRasterImage(const OdGiRasterImage *pRaster, fipImage &fiImage, FREE_IMAGE_FORMAT fiFormat, const OdUInt32 *pFlagsChain)
{
  fiImage.setSize(FIT_BITMAP, (unsigned)pRaster->pixelWidth(), (unsigned)pRaster->pixelHeight(), (unsigned)pRaster->colorDepth());
  OdUInt32 odaScanSize = pRaster->scanLineSize();
  OdUInt32 scanlineWidth = odmin(fiImage.getScanWidth(), (unsigned)odaScanSize);
  OdUInt32 nScans = pRaster->pixelHeight();
  OdUInt8Array slTmp(odaScanSize, 1);
  slTmp.resize(odaScanSize);
  OdUInt8 *pSlTmp = slTmp.asArrayPtr();
  for (OdUInt32 nScan = 0; nScan < nScans; nScan++)
  {
    pRaster->scanLines(pSlTmp, nScan);
    ::memcpy(fiImage.getScanLine((unsigned)nScan), pSlTmp, scanlineWidth);
  }
  if (fiImage.getPaletteSize() && pRaster->paletteDataSize())
  {
    unsigned fiImagePalSize = fiImage.getPaletteSize();
    unsigned pRasterPalSize = pRaster->paletteDataSize();
    ODA_ASSERT(fiImagePalSize == pRasterPalSize);

    OdUInt8Array palData(pRaster->paletteDataSize(), 1);
    palData.resize(pRaster->paletteDataSize());
    pRaster->paletteData(palData.asArrayPtr());
    ::memcpy(fiImage.getPalette(), palData.getPtr(), odmin(palData.size(), fiImage.getPaletteSize()));
  }
  if (fiFormat == FIF_GIF && pRaster->colorDepth() > 8)
  {
    if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kQuantizeNone))
      fiImage.convertTo8Bits();
    else
    {
      // CORE-13306
      FREE_IMAGE_QUANTIZE algorithm = FIQ_WUQUANT;
      if (pFlagsChain)
      {
        if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kQuantizeWU))
          algorithm = FIQ_WUQUANT;
        else if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kQuantizeNN))
          algorithm = FIQ_NNQUANT;
        else if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kQuantizeWU))
          algorithm = FIQ_LFPQUANT;
      }
      fiImage.convertTo24Bits();
      fiImage.colorQuantize(algorithm);
    }
  }
}

static void fiImageTransform(fipImage &fiImage, const OdUInt32 *pFlagsChain)
{
  if (!pFlagsChain)
    return;
  if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kRescale))
  {
    OdUInt32 filter = ::flagsChainGetUInt(pFlagsChain, OdRxRasterServices::kRescale);
    OdUInt32 width = ::flagsChainGetUInt(pFlagsChain, OdRxRasterServices::kRescaleWidth);
    OdUInt32 height = ::flagsChainGetUInt(pFlagsChain, OdRxRasterServices::kRescaleHeight);
    if (width && height)
    {
      switch (filter)
      {
      case OdRxRasterServices::kRescaleBicubic:
        fiImage.rescale(width, height, FILTER_BICUBIC);
        break;
      case OdRxRasterServices::kRescaleBox:
        fiImage.rescale(width, height, FILTER_BOX);
        break;
      case OdRxRasterServices::kRescaleBspline:
        fiImage.rescale(width, height, FILTER_BSPLINE);
        break;
      case OdRxRasterServices::kRescaleCatmullrom:
        fiImage.rescale(width, height, FILTER_CATMULLROM);
        break;
      case OdRxRasterServices::kRescaleLanczos3:
        fiImage.rescale(width, height, FILTER_LANCZOS3);
        break;
      case OdRxRasterServices::kRescaleBilinear:
        fiImage.rescale(width, height, FILTER_BILINEAR);
        break;
      }
    }
  }
  //dithering must follow after rescaling because of rescaling converts bitional images to 8-bit images
  if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kDithering))
    switch (::flagsChainGetUInt(pFlagsChain, OdRxRasterServices::kDithering))
    {
      case OdRxRasterServices::kDitheringBayer16x16:
        fiImage.dither(FID_BAYER16x16);
        break;
      case OdRxRasterServices::kDitheringBayer4x4:
        fiImage.dither(FID_BAYER4x4);
        break;
      case OdRxRasterServices::kDitheringBayer8x8:
        fiImage.dither(FID_BAYER8x8);
        break;
      case OdRxRasterServices::kDitheringCluster16x16:
        fiImage.dither(FID_CLUSTER16x16);
        break;
      case OdRxRasterServices::kDitheringCluster6x6:
        fiImage.dither(FID_CLUSTER6x6);
        break;
      case OdRxRasterServices::kDitheringCluster8x8:
        fiImage.dither(FID_CLUSTER8x8);
        break;
      case OdRxRasterServices::kDitheringFS:
        fiImage.dither(FID_FS);
        break;
    }
  if (::flagsChainContains(pFlagsChain, OdRxRasterServices::kTransparentColor))
  {
    BYTE transparentColor = (BYTE)::flagsChainGetUInt(pFlagsChain, OdRxRasterServices::kTransparentColor);
    BYTE transparencyTable[256];
    memset(transparencyTable, 0xFF, 256);
    transparencyTable[transparentColor] = 0;
    fiImage.setTransparencyTable(transparencyTable, 256);
  }
}

struct odStreamFIAdapter : public FreeImageIO
{
  OdStreamBuf* m_pStreamBuf;
  bool m_isWriteMode;

  static unsigned ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle)
  {
    odStreamFIAdapter *pAdapter = (odStreamFIAdapter*)handle;
    OdUInt64 diff = pAdapter->m_pStreamBuf->tell();
    OdUInt32 nBytesToRead = (OdUInt32)(size * count);
    if (diff + nBytesToRead > pAdapter->m_pStreamBuf->length())
      nBytesToRead = pAdapter->m_pStreamBuf->length() - diff;
    if (nBytesToRead)
      pAdapter->m_pStreamBuf->getBytes(buffer, nBytesToRead);
    //return (unsigned)(pAdapter->m_pStreamBuf->tell() - diff) / size;
    return (unsigned)(nBytesToRead / size);
  }
  static unsigned WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle)
  {
    odStreamFIAdapter *pAdapter = (odStreamFIAdapter*)handle;
    OdUInt64 diff = pAdapter->m_pStreamBuf->tell();
    pAdapter->m_pStreamBuf->putBytes(buffer, (OdUInt32)(size * count));
    return (unsigned)(pAdapter->m_pStreamBuf->tell() - diff) / size;
  }
  static int SeekProc(fi_handle handle, long offset, int origin)
  {
    odStreamFIAdapter *pAdapter = (odStreamFIAdapter*)handle;
    OdUInt64 curPos = pAdapter->m_pStreamBuf->tell();
    OdUInt64 endPos = offset;
    OdDb::FilerSeekType sType = OdDb::kSeekFromStart;
    switch (origin)
    {
      case SEEK_CUR:
        sType = OdDb::kSeekFromCurrent;
        endPos += curPos;
      break;
      case SEEK_END:
        sType = OdDb::kSeekFromEnd;
        endPos = 0;
      break;
    }
    if (pAdapter->m_isWriteMode)
    {
      OdUInt64 dataLen = pAdapter->m_pStreamBuf->length();
      if (endPos > dataLen)
      {
        OdUInt64 gapLen = endPos - dataLen;
        pAdapter->m_pStreamBuf->seek(0, OdDb::kSeekFromEnd);
        while (gapLen--)
          pAdapter->m_pStreamBuf->putByte(0);
        pAdapter->m_pStreamBuf->seek(curPos, OdDb::kSeekFromStart);
      }
    }
    OdUInt64 out = pAdapter->m_pStreamBuf->seek((OdInt64)offset, sType);
    return (out == curPos) ? FALSE : TRUE;
  }
  static long TellProc(fi_handle handle)
  {
    odStreamFIAdapter *pAdapter = (odStreamFIAdapter*)handle;
    return (long)pAdapter->m_pStreamBuf->tell();
  }
  explicit odStreamFIAdapter(OdStreamBuf* pStreamBuf, bool isWriteMode = false)
    : m_pStreamBuf(pStreamBuf)
    , m_isWriteMode(isWriteMode)
  {
    read_proc = ReadProc;
    write_proc = WriteProc;
    seek_proc = SeekProc;
    tell_proc = TellProc;
  }
};

#ifndef _WIN32
static bool isFileNameNonAscii(const OdString &str)
{
  const OdChar *pBuf = str.c_str();
  OdUInt32 len = str.getLength();
  while (len-- > 0)
  {
    if (*pBuf > 127)
      return true;
    pBuf++;
  }
  return false;
}
#endif

static FreeImageIO *createNewStreamFIAdapter(OdStreamBuf *pStreamBuf, bool bForWrite)
{
  return new odStreamFIAdapter(pStreamBuf, bForWrite);
}

static void releaseStreamFIAdapter(FreeImageIO *pAdapter)
{
  delete static_cast<odStreamFIAdapter*>(pAdapter);
}

#endif // !defined(RASTER_FREE_IMAGE)

bool ExRasterModule::convertRasterImage(const OdGiRasterImage* pRaster, OdUInt32 type,
                                        OdStreamBuf* pStreamBuf, const OdUInt32 *pFlagsChain)
{
  bool bRet = false;
#if !defined(RASTER_FREE_IMAGE)
  switch( type )
  {
    case OdRxRasterServices::kBMP:
      bRet = true; try { ::saveBitmap(pRaster, pStreamBuf); } catch(const OdError &) { bRet = false; }
      break;
    case OdRxRasterServices::kJPEG:
      bRet = convertRasterImageToJPEG( pRaster, type, pStreamBuf);
      break;
    case kUnknown:
      break;
  }
#else // !defined(RASTER_FREE_IMAGE)
  int fiFlags;
  FREE_IMAGE_FORMAT fiFormat = ::mapImageTypeToFIFormat(type, fiFlags, pFlagsChain);
  if (fiFormat != FIF_UNKNOWN && pRaster != NULL && pStreamBuf != NULL)
  { // Format processible
    fipImage fiImage;
    ::fiImageFromGiRasterImage(pRaster, fiImage, fiFormat, pFlagsChain);
    ::fiImageTransform(fiImage, pFlagsChain);
    odStreamFIAdapter adapter(pStreamBuf, true);
    bRet = fiImage.saveToHandle(fiFormat, &adapter, (fi_handle)&adapter, fiFlags) != FALSE;
    //if (bRet)
    //  pStreamBuf->rewind();
  }
#endif // !defined(RASTER_FREE_IMAGE)
  return bRet;
}

bool ExRasterModule::convertRasterImage(OdStreamBuf* pSrcStream, OdStreamBuf* pDstStream, OdUInt32 type,
                                        const OdUInt32 *pFlagsChainSrc, const OdUInt32 *pFlagsChainDst)
{
  bool bRes = false;

#if defined(RASTER_FREE_IMAGE)
  int fiFlags;
  FREE_IMAGE_FORMAT fiFormat = ::mapImageTypeToFIFormat(type, fiFlags, pFlagsChainDst);
  if (fiFormat != FIF_UNKNOWN && pSrcStream != NULL && pDstStream != NULL)
  { // Format processible
    fipImage fiImage;
    odStreamFIAdapter adapter(pSrcStream);
    if (::flagsChainContains(pFlagsChainSrc, kLoadFmt))
    {
      OdUInt32 preferFormat = ::flagsChainGetUInt(pFlagsChainSrc, kLoadFmt);
      if (preferFormat != kUnknown)
      {
        FREE_IMAGE_FORMAT fifIn = fipImage::identifyFIFFromHandle(&adapter, (fi_handle)&adapter);
        int tmpFlags = 0;
        FREE_IMAGE_FORMAT fifOut = ::mapImageTypeToFIFormat(preferFormat, tmpFlags);
        if (fifIn != fifOut)
          return false;
      }
    }
    if (fiImage.loadFromHandle(&adapter, (fi_handle)&adapter) != FALSE)
    {
      adapter.m_pStreamBuf = pDstStream;
      adapter.m_isWriteMode = true;
      bRes = fiImage.saveToHandle(fiFormat, &adapter, (fi_handle)&adapter, fiFlags) != FALSE;
      if (bRes)
        pDstStream->seek(0, OdDb::kSeekFromStart); // ?
    }
  }
#endif

  return bRes;
}

OdString ExRasterModule::mapTypeToExtension(OdUInt32 type, OdString* psFilterName) const
{
  if (psFilterName)
    psFilterName->empty();
  OdString sExt,
           sFilterName;

#ifdef RASTER_FREE_IMAGE
  int flags = 0;
  FREE_IMAGE_FORMAT fiFormat = ::mapImageTypeToFIFormat(type, flags);
  if (fiFormat == FIF_UNKNOWN) // TODO inside mapImageTypeToFIFormat
    return OdString::kEmpty;
  sExt = FreeImage_GetFIFExtensionList(fiFormat);
  sExt.makeLower();
  sFilterName = FreeImage_GetFIFDescription(fiFormat);
  if (sFilterName.isEmpty())
    (sFilterName = sExt).makeUpper();
  if (!sExt.isEmpty())
    sExt = OD_T(".") + sExt;
  if (!sFilterName.isEmpty())
  {
    OdString sExtensions = sExt;
    sExtensions.replace(OD_T(","), OD_T(";*."));
    sFilterName += OD_T(" (*") + sExtensions + OD_T(")");
  }
  int pos = sExt.find(__OD_T(','));
  if (pos > 1)
    sExt = sExt.left(pos); // first extension only
#endif

  if (sExt.isEmpty())
  {
    switch (type)
    {
    case kBMP: sExt = OD_T(".bmp"); sFilterName = OD_T("Bitmap (*.bmp)"); break;
    case kICO: sExt = OD_T(".ico"); sFilterName = OD_T("ICO (*.ico)"); break;
    case kJPEG: sExt = OD_T(".jpg"); sFilterName = OD_T("JPEG (*.jpg;*.jpeg)"); break;
    //case kJNG: sExt = OD_T(".jng"); sFilterName = OD_T("JNG (*.jng)"); break;
    //case kKOALA:
    //case kLBM:
    //case kIFF:
    //case kMNG:
    //case kPBM:
    //case kPBMRAW:
    //case kPCD:
    //case kPCX:
    //case kPGM:
    //case kPGMRAW:
    case kPNG: sExt = OD_T(".png"); sFilterName = OD_T("PNG (*.png)"); break;
    //case kPPM:
    //case kPPMRAW:
    //case kRAS;
    case kTARGA: sExt = OD_T(".tga"); sFilterName = OD_T("Targa (*.tga)"); break;
    case kTIFF: sExt = OD_T(".tif"); sFilterName = OD_T("TIFF (*.tif;*tiff)"); break;
    //case kWBMP:
    //case kPSD:
    //case kCUT:
    //case kXBM:
    //case kXPM:
    //case kDDS:
    case kGIF: sExt = OD_T(".gif"); sFilterName = OD_T("GIF (*.gif)"); break;
    //case kHDR:
    //case kFAXG3:
    //case kSGI:
    //case kEXR:
    //case kJ2K:
    //case kJP2:
    //case kPFM:
    //case kPICT:
    //case kRAW:
    } // switch
    ODA_ASSERT_ONCE(!sExt.isEmpty()); // TODO
  } // if

  if (psFilterName)
    *psFilterName = sFilterName;
  return sExt;
}

OdUInt32Array ExRasterModule::getRasterImageTypes() const
{
  OdUInt32Array types;
  types.push_back(kBMP);
#if defined(RASTER_JPEG6B)
  types.push_back(kJPEG);
#elif defined(RASTER_SNOW_BOUND)
  types.push_back(kJPEG);
  types.push_back(kTIFF);
  types.push_back(kGIF);
  types.push_back(kPNG);
#elif defined(RASTER_FREE_IMAGE)
  for (FREE_IMAGE_FORMAT fiFormat = FIF_ICO; fiFormat <= FIF_RAW; fiFormat = FREE_IMAGE_FORMAT(fiFormat + 1))
  {
    if (!FreeImage_IsPluginEnabled(fiFormat))
      continue;
    OdUInt32 type = mapFiFormatToImageType(fiFormat);
    types.push_back(type);
  }
#endif
  return types;
}

OdUInt32 ExRasterModule::mapExtensionToType(const OdString& extension) const
{
  OdString extensionItl(extension);
  if (extensionItl.find('.') < 0)
    extensionItl.insert(0, OD_T("a."));
#if defined(RASTER_FREE_IMAGE)
  return ::mapExtensionToImageType(extensionItl);
#elif defined(RASTER_SNOW_BOUND)
  return SBRasterTypeToImageType(getSBRasterType(extensionItl));
#else
  if (!extensionItl.right(4).iCompare(OD_T(".bmp")))
    return kBMP;
#if defined(RASTER_JPEG6B)
  else if (!extensionItl.right(4).iCompare(OD_T(".jpg")) || !extensionItl.right(5).iCompare(OD_T(".jpeg")))
    return kJPEG;
#endif
  return kUnknown;
#endif
}

OdUInt32 ExRasterModule::getImageFormat(const OdString &filename) const
{
  OdStreamBufPtr pFileBuf;
  try
  {
    pFileBuf = odrxSystemServices()->createFile(filename);
  }
  catch (...)
  {
    return (OdUInt32)kUnknown;
  }
  return getImageFormat(pFileBuf);
}

OdUInt32 ExRasterModule::getImageFormat(OdStreamBuf* pBuf) const
{
  OdUInt32 type = (OdUInt32)kUnknown;
  OdUInt64 startPos = pBuf->tell();
  OdInt16 type16 = OdPlatformStreamer::rdInt16(*pBuf); // type
  if (type16 == 19778) // BMP
    type = kBMP;
  else
  {
#if defined(RASTER_SNOW_BOUND)
    pBuf->seek(startPos, OdDb::kSeekFromStart);
    std::vector<char> buf;
    buf.resize(10);
    pBuf->getBytes(&buf[0], 10);
    int typeIn = IMGLOW_get_filetype_mem(&buf[0]);
    type = SBRasterTypeToImageType(typeIn);
#elif defined(RASTER_FREE_IMAGE)
    pBuf->seek(startPos, OdDb::kSeekFromStart);
    odStreamFIAdapter adapter(pBuf);
    FREE_IMAGE_FORMAT fifIn = fipImage::identifyFIFFromHandle(&adapter, (fi_handle)&adapter);
    type = ::mapFiFormatToImageType(fifIn);
#elif defined(RASTER_JPEG6B)
    if ((((OdUInt8*)(&type16))[0] == 0xFF) && (((OdUInt8*)(&type16))[1] == 0xD8))
      type = kJPEG;
#endif
  }
  pBuf->seek(startPos, OdDb::kSeekFromStart);
  return type;
}

/** Description:
    The user override of this function should register any custom objects defined in the 
    custom application, using the OdRxObject::rxInit function.  It should also register
    custom commands defined in the module.
*/
void ExRasterModule::initApp()
{
  ODA_TRACE0("ExRasterModule::initApp() called\n");
#if defined(RASTER_FREE_IMAGE) && defined(FREEIMAGE_LIB)
  FreeImage_Initialise(FALSE);
#endif
}

/** Description:
    The user override of this function should unregister any custom objects defined in the
    custom application, using the OdRxObject::rxUninit function.  It should also
    remove any custom commands that were registered in the initApp function.
*/
void ExRasterModule::uninitApp()
{
#if defined(RASTER_FREE_IMAGE) && defined(FREEIMAGE_LIB)
  FreeImage_DeInitialise();
#endif
}

ODRX_DEFINE_DYNAMIC_MODULE(ExRasterModule);

//
