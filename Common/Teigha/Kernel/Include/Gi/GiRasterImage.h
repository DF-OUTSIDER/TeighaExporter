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

#ifndef _OD_ODGIRASTERIMAGE_H_
#define _OD_ODGIRASTERIMAGE_H_

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    This class represents a raster image within the Teigha vectorization framework.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiRasterImage : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiRasterImage);

  enum Units
  {
    kNone = 0,
    kMillimeter,
    kCentimeter,
    kMeter,
    kKilometer,
    kInch,
    kFoot,
    kYard,
    kMile,
    kMicroinches,
    kMils,
    kAngstroms,
    kNanometers,
    kMicrons,
    kDecimeters,
    kDekameters,
    kHectometers,
    kGigameters,
    kAstronomical,
    kLightYears,
    kParsecs     
  };

  enum ImageSource
  {
    kUndefinedSource = -1, // Flag not set (unknown source).
    kFromDwg         = 0,  // Raster image loaded from raster image definition.
    kFromOleObject   = 1,  // Raster image generated from OLE data source.
    kFromRender      = 2,  // Raster image generated from rendering.

    kFromUnderlay    = 10, // Raster image is part of underlay.
    kFromImageBGRA32 = 11, // Raster image wraps OdGiImageBGRA32 image.

    // Aliases
    kFromPdfUnderlay = kFromOleObject, // Raster image generated from PdfUnderlay.
    kFromFile        = kFromDwg        // Raster image generated from disk file.
  };

  enum TransparencyMode
  {
    kTransparencyDef  = -1, // Default for native raster image (alpha 0 is transparent, 1-255 is opaque).
    kTransparencyOff  = 0,  // Alpha ignored for 32bpp formats.
    kTransparency1Bit = 1,  // Alpha 0-254 is transparent, 255 is opaque.
    kTransparency8Bit = 2   // Complete alpha level support for 32bpp formats.
  };

  typedef struct tagPixelFormatInfo
  {
    OdUInt8 redOffset;
    OdUInt8 numRedBits;
    OdUInt8 greenOffset;
    OdUInt8 numGreenBits;
    OdUInt8 blueOffset;
    OdUInt8 numBlueBits;
    OdUInt8 alphaOffset;
    OdUInt8 numAlphaBits;
    OdUInt8 bitsPerPixel;

    bool operator ==(const struct tagPixelFormatInfo& other) const
    {
      return (redOffset == other.redOffset) && (greenOffset == other.greenOffset) &&
        (blueOffset == other.blueOffset) && (alphaOffset == other.alphaOffset) && (bitsPerPixel == other.bitsPerPixel) &&
        (numRedBits == other.numRedBits) && (numGreenBits == other.numGreenBits) && (numBlueBits == other.numBlueBits) &&
        (numAlphaBits == other.numAlphaBits);
    }

    tagPixelFormatInfo()
      : redOffset(0)
      , numRedBits(0)
      , greenOffset(0)
      , numGreenBits(0)
      , blueOffset(0)
      , numBlueBits(0)
      , alphaOffset(0)
      , numAlphaBits(0)
      , bitsPerPixel(0)
    {}

    /** \details
      Returns true if and only if the pixel format of this RasterImage object is RGB.
    */
    bool isRGB() const
    {
      return bitsPerPixel == 24 && 
        redOffset == 0 && greenOffset == 8 && blueOffset == 16 && numRedBits == 8 && numGreenBits == 8 && numBlueBits == 8;
    }

    /** \details
      Sets the pixel format of this RasterImage object to RGB.
      \remarks
      No palette conversion is performed.
    */
    void setRGB()
    {
      bitsPerPixel = 24;
      redOffset    = 0;
      greenOffset  = 8;
      blueOffset   = 16;
      numRedBits   = 8;
      numGreenBits = 8;
      numBlueBits  = 8;
    }

    /** \details
      Returns true if and only if the pixel format of this RasterImage object is BGR.
    */
    bool isBGR() const
    {
      return bitsPerPixel == 24 && 
        redOffset == 16 && greenOffset == 8 && blueOffset == 0 && numRedBits == 8 && numGreenBits == 8 && numBlueBits == 8;
    }

    /** \details
      Sets the pixel format of this RasterImage object to BGR.
      \remarks
      No palette conversion is performed.
    */
    void setBGR()
    {
      bitsPerPixel = 24;
      redOffset    = 16;
      greenOffset  = 8;
      blueOffset   = 0;
      numRedBits   = 8;
      numGreenBits = 8;
      numBlueBits  = 8;
    }

    /** \details
      Returns true if and only if the pixel format of this RasterImage object is 16-bit BGR.
    */
    bool is16bitBGR() const
    {
      return bitsPerPixel == 16 && 
        redOffset == 10 && greenOffset == 5 && blueOffset == 0 && numRedBits == 5 && numGreenBits == 5 && numBlueBits == 5;
    }

    /** \details
      Sets the pixel format of this RasterImage object to 16-bit BGR.
      \remarks
      No palette conversion is performed.
    */
    void set16bitBGR()
    {
      bitsPerPixel = 16;
      redOffset    = 10;
      greenOffset  = 5;
      blueOffset   = 0;
      numRedBits   = 5;
      numGreenBits = 5;
      numBlueBits  = 5;
    }

    /** \details
      Returns true if and only if the pixel format of this RasterImage object is RGBA.
    */
    bool isRGBA() const
    {
      return bitsPerPixel == 32 && alphaOffset == 24 && numAlphaBits == 8 &&
        redOffset == 0 && greenOffset == 8 && blueOffset == 16 && numRedBits == 8 && numGreenBits == 8 && numBlueBits == 8;
    }

    /** \details
      Sets the pixel format of this RasterImage object to RGBA.
      \remarks
      No palette conversion is performed.
    */
    void setRGBA()
    {
      bitsPerPixel = 32;
      alphaOffset  = 24;
      numAlphaBits = 8;
      redOffset    = 0;
      greenOffset  = 8;
      blueOffset   = 16;
      numRedBits   = 8;
      numGreenBits = 8;
      numBlueBits  = 8;
    }

    /** \details
      Returns true if and only if the pixel format of this RasterImage object is BGRA.
    */
    bool isBGRA() const
    {
      return bitsPerPixel == 32 && alphaOffset == 24 && numAlphaBits == 8 &&
        redOffset == 16 && greenOffset == 8 && blueOffset == 0 && numRedBits == 8 && numGreenBits == 8 && numBlueBits == 8;
    }

    /** \details
      Sets the pixel format of this RasterImage object to BGRA.
      \remarks
      No palette conversion is performed.
    */
    void setBGRA()
    {
      bitsPerPixel = 32;
      alphaOffset  = 24;
      numAlphaBits = 8;
      redOffset    = 16;
      greenOffset  = 8;
      blueOffset   = 0;
      numRedBits   = 8;
      numGreenBits = 8;
      numBlueBits  = 8;
    }
  } PixelFormatInfo;

  /** \details
    Returns the image width in pixels of this RasterImage object.
  */
  virtual OdUInt32 pixelWidth() const = 0;

  /** \details
    Returns the image height in pixels of this RasterImage object.
  */
  virtual OdUInt32 pixelHeight() const = 0;

  /** \details
    Returns the default image resolution in pixels per unit of this RasterImage object.

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction).
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \remarks
    If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
   virtual Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;

  /** \details
    Returns the number of bits per pixel used for colors by this RasterImage object.
  */
  virtual OdUInt32 colorDepth() const = 0;

  /** \details
    Returns the number of colors in the palette of this RasterImage object.
  */
  virtual OdUInt32 numColors() const = 0;

  /** \details
    Returns the index of transparent color in the image.
    \remarks
    If there is no transparent color, -1 should be returned.
    
    \remarks
    The default implementation of this function returns -1.
  */
  virtual int transparentColor() const;

  /** \details
    Returns the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
  */
  virtual ODCOLORREF color(OdUInt32 colorIndex) const = 0;

  /** \details
    Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  virtual OdUInt32 paletteDataSize() const = 0;

  /** \details
    Returns the palette in BMP format of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  virtual void paletteData(OdUInt8* bytes) const = 0;

  /** \details
    Returns the scanline size of this RasterImage object.
    
    \remarks
    This function returns the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  virtual OdUInt32 scanLineSize() const;

  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scanline data is preallocated by the caller.
  */
  virtual const OdUInt8* scanLines() const = 0;
  
  /** \details
    Returns the specified set of scanlines in BMP format from this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scanline data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks
    * The number of accessible scanlines is equal to the value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \remarks
    It us up to the caller to allocate sufficient memory for the scanline data.
  */
  virtual void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const = 0;

  /** \details
    Returns the pixel format for this RasterImage object.
    
    \remarks
    This function returns the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  virtual PixelFormatInfo pixelFormat() const = 0;

  /** \details
    Returns the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
  */
  virtual OdUInt32 scanLinesAlignment() const = 0;

  /** \details
    Returns the image source.
  */
  virtual ImageSource imageSource() const;

  /** \details
    Returns the image source file name.
  */
  virtual const OdString &sourceFileName() const;

  /** \details
    Returns the transparency mode for 32bpp formats.
  */
  virtual TransparencyMode transparencyMode() const;

  /** \details
    Sets an image source and (optionally) source file name.
    \param source [in]  New image source value.
    \param pFileName [in]  Optional value for source file name.
    \returns Returns image with new image source.
  */
  OdSmartPtr<OdGiRasterImage> changeImageSource(ImageSource source, const OdChar *pFileName = NULL);

  /** \details
    Sets an image source file name.
    \param fileName [in]  New value for source file name.
    \returns Returns image with new image source file name.
  */
  OdSmartPtr<OdGiRasterImage> changeSourceFileName(const OdString &fileName);

  /** \details
    Sets transparency mode for 32bpp formats.
    \param mode [in]  New transparency mode.
    \returns Returns image with new transparency mode.
  */
  OdSmartPtr<OdGiRasterImage> changeTransparencyMode(TransparencyMode mode);

  /** \details
    Returns the size of a single scanline in BMP format for this RasterImage object.
    \param pixelWidth [in]  Image width in pixels.
    \param colorDepth [in]  Number of bits per pixel.
  */
  static OdUInt32 calcBMPScanLineSize(OdUInt32 pixelWidth, int colorDepth);

  /** \details
    Returns the 32-bit mask for the requested number of color bits and offset.
    \param numColorBits [in]  Number of color bits.
    \param colorOffset [in]  Offset of color bits.
  */
  static OdUInt32 calcColorMask(OdUInt8 numColorBits, OdUInt8 colorOffset = 0);

  /** \details
    Converts this raster image as specified.
    \param convertPaletteToRGB [in]  Color is converted to RGB if and only if true.
    \param brightness [in]  Image brightness [0 .. 100].
    \param contrast [in]  Image contrast [0 .. 100].
    \param fade [in]  Image fade value [0 .. 100].
    \param backgroundColor [in]  Background color.
    \param flipX [in]  Image is flipped about the X-axis if and only if true.
    \param flipY [in]  Image is flipped about the Y-axis if and only if true.
    \param rotate90 [in]  Image is rotated 90° if and only if true.
    \param pDestDesc [in]  Pointer to an OdGiRasterImageDesc object to receive the image parameters.
    \param transparency [in]  Image is transparent if true.
    \returns
    Returns a SmartPointer to the converted image.
  */
  OdSmartPtr<OdGiRasterImage> convert(bool convertPaletteToRGB,
    double brightness = 50.0, double contrast = 50.0, double fade = 0.0, ODCOLORREF backgroundColor = 0,
    bool flipX = false, bool flipY = false, bool rotate90 = false,
    const OdGiRasterImage* pDestDesc = 0,
    bool transparency = false) const;

  /** \details
    Crops this raster image using a rectangle.
    The format of returned raster image is identical to the format of the source raster image.

    \param x [in] X-coordinate of start point.
    \param y [in] Y-coordinate of start point.
    \param width [in] Width of the crop rectangle.
    \param height [in] Height of the crop rectangle.
    \remarks 
    All arguments are in pixels.
  */
  virtual OdSmartPtr<OdGiRasterImage> crop(OdUInt32 x, OdUInt32 y, OdUInt32 width, OdUInt32 height) const;

  /** \details
    Returns user-end implementation of raster image.
    \remarks
    If not overridden by user-end raster image implementation, always returns null.
  */
  virtual void *imp() const;
};

typedef OdSmartPtr<OdGiRasterImage> OdGiRasterImagePtr;

/** \details
    This interface simply extends GiRasterImage to set the image source and transparency mode values.
    If the raster image can be cast to this interface, values will be set within this interface;
    otherwise the raster image wrapper will be created to reset these values.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiRasterImageParam : public OdGiRasterImage
{
public:
  ODRX_DECLARE_MEMBERS(OdGiRasterImageParam);

  enum ParamType
  {
    kImageSource      = (1 << 0),
    kTransparencyMode = (1 << 1),
    kSourceFileName   = (1 << 2)
  };

  /** \details
    Returns the ParamType bitflags with which parameters are supported by this interface.
  */
  virtual OdUInt32 supportedParams() const = 0;

  /** \details
    Sets an image source.
    \param source [in]  New image source value.
  */
  virtual void setImageSource(ImageSource source) { }

  /** \details
    Sets an image source file name.
    \param fileName [in]  New value for image source file name.
  */
  virtual void setSourceFileName(const OdString &fileName) { }

  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  virtual void setTransparencyMode(TransparencyMode mode) { }
};

typedef OdSmartPtr<OdGiRasterImageParam> OdGiRasterImageParamPtr;

#include "TD_PackPop.h"

#endif //#ifndef _OD_ODGIRASTERIMAGE_H_
