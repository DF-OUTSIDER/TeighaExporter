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

#ifndef __OD_GI_RASTER_WRAPPERS__
#define __OD_GI_RASTER_WRAPPERS__

#include "Gi/GiExport.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiImage.h"
#include "UInt8Array.h"

#include "TD_PackPush.h"

/** \details
    This class is a dummy implementation of the OdGiRasterImage interface.

    \remarks
    This class is intended to be used to preset image parameters for 
    OdDbRasterImageDef objects without the actual loading of raster image files.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiRasterImageDesc : public OdGiRasterImageParam
{
  OdUInt32                          m_pixelWidth, m_pixelHeight, m_colorDepth, m_palSize, m_alignment;
  OdGiRasterImage::PixelFormatInfo  m_pixFmt;
  Units                             m_units;
  double                            m_xPelsPerUnit, m_yPelsPerUnit;
  OdUInt8*                          m_pPal;
  OdGiRasterImage::ImageSource      m_imageSource;
  OdGiRasterImage::TransparencyMode m_transparencyMode;
  OdString                          m_fileName;
public:
  OdGiRasterImageDesc();
  virtual ~OdGiRasterImageDesc();

  /** \details
    Creates an OdGiRasterImageDesc object with the specified parameters. 
    \param pixelWidth [in]  Image width in pixels.
    \param pixelHeight [in]  Image height in pixels.
    \param units [in]  Units / pixel.
    \param xPelsPerUnit [in]  Pixels per unit value (x direction).
    \param yPelsPerUnit [in]  Pixels per unit value (y direction).

    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdUInt32 pixelWidth, OdUInt32 pixelHeight, Units units = kNone, double xPelsPerUnit = 0.0, double yPelsPerUnit = 0.0);
  
  /** \details
    Creates an OdGiRasterImageDesc object with the specified parameters. 
    \param pixelWidth [in]  Image width in pixels.
    \param pixelHeight [in]  Image height in pixels.
    \param colorDepth [in]  Number of bits per pixel used for colors.
    \param units [in]  Units / pixel.
    \param xPelsPerUnit [in]  Pixels per unit value (x direction).
    \param yPelsPerUnit [in]  Pixels per unit value (y direction).
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdUInt32 pixelWidth, OdUInt32 pixelHeight, OdUInt32 colorDepth, Units units = kNone, double xPelsPerUnit = 0.0, double yPelsPerUnit = 0.0);
  
  /** \details
    Creates an OdGiRasterImageDesc object with the specified parameters. 

    \param pSourceImage [in]  Source raster image to copy parameters from.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage *pSourceImage);

  /** \details
    Copies parameters from an original OdGiRasterImage object.
    \param pSourceImage [in]  Pointer to the original image object.
  */
  void setFrom(const OdGiRasterImage *pSourceImage);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
  /** \details
    Returns the image width in pixels of this RasterImage object.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Returns the image height in pixels of this RasterImage object.
  */
  OdUInt32 pixelHeight() const;
   /** \details
    Returns the number of bits per pixel used for colors by this RasterImage object.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Returns the number of colors in the palette of this RasterImage object.
  */
  OdUInt32 numColors() const;
  /** \details
    Returns the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Returns the palette in BMP format of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Returns the scanline size of this RasterImage object.
    
    \remarks
    This function returns the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Returns NULL for an OdGiRasterImageDesc object.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Returns the specified set of scanlines in BMP format for this RasterImage object,
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

    \remarks
    Returns nothing for an OdGiRasterImageDesc object.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Returns the pixel format for this RasterImage object.
    
    \remarks
    This function returns the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  PixelFormatInfo pixelFormat() const;
  /** \details
    Returns the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Returns the default image resolution in pixels per unit of this RasterImage object.

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction).
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \remarks
    If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;

  /** \details
    Sets the image width in pixels of this RasterImageDesc object.
    \param pixelWidth [in]  Pixel width.
  */
  void setPixelWidth(OdUInt32 pixelWidth);
  /** \details
    Sets the image height in pixels of this RasterImageDesc object.
    \param pixelHeight [in]  Pixel height.
  */
  void setPixelHeight(OdUInt32 pixelHeight);

  /** \details
    Sets the number of bits per pixel used for colors by this RasterImageDesc object.
    \param colorDepth [in]  Color depth.
  */
  void setColorDepth(OdUInt32 colorDepth);
  /** \details
    Returns the pixel format for this RasterImage object.
    
    \remarks
    This function returns the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  OdGiRasterImage::PixelFormatInfo& pixelFormat();

  /** \details
    Sets the palette in BMP format for this RasterImageDesc object.
    \param paletteData [in]  Palette data.
    \param paletteSize [in]  Palette size in bytes.
  */
  void setPalette(OdUInt32 paletteSize, OdUInt8* paletteData);

  /** \details
    Sets the scanline alignment, in bytes, for this RasterImage object.
    \param alignment [in]  Scanline alignment.
    Example:
    Alignment is 4 for Windows BMP.
  */
  void setScanLinesAlignment(OdUInt32 alignment);

  /** \details
    Sets the default raster image resolution for this RasterImage object.
    \param units [in]  Units / pixel.
    \param xPelsPerUnit [in]  Pixels per unit value (x direction).
    \param yPelsPerUnit [in]  Pixels per unit value (y direction).
  */
  void setDefaultResolution(Units units, double xPelsPerUnit, double yPelsPerUnit);
  /** \details
    Returns the ParamType bitflags with which parameters are supported by this interface.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Returns the image source.
  */
  OdGiRasterImage::ImageSource imageSource() const;
  /** \details
    Sets an image source.
    \param source [in]  New image source value.
  */
  void setImageSource(OdGiRasterImage::ImageSource source);
  /** \details
    Returns the image source file name.
  */
  const OdString &sourceFileName() const;
  /** \details
    Sets an image source file name.
    \param fileName [in]  New value for image source file name.
  */
  void setSourceFileName(const OdString &fileName);
  /** \details
    Returns the transparency mode for 32bpp formats.
  */
  OdGiRasterImage::TransparencyMode transparencyMode() const;
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(OdGiRasterImage::TransparencyMode mode);
};

class OdGiImageBGRA32;

/** \details
    Emulates OdGiImageBGRA32 within the OdGiRasterImage interface.
    \remarks
    Stores only a pointer to the original image, not a SmartPointer, so deletion of the original image before
    this class may cause an access violation error.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiRasterImageBGRA32 : public OdGiRasterImageParam
{
protected:
  OdGiImageBGRA32 *m_pBGRAImage;
  OdGiRasterImage::TransparencyMode m_transparencyMode;
public:
  OdGiRasterImageBGRA32();
  virtual ~OdGiRasterImageBGRA32();
  
  /** \details
    Creates an OdGiRasterImageBGRA32 object with the specified parameters.
    \param pImage [in]  Input BGRA32 image.
    \param transparencyMode [in]  Transparency mode.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdGiImageBGRA32 *pImage, OdGiRasterImage::TransparencyMode transparencyMode = OdGiRasterImage::kTransparency8Bit);
  /** \details
    Returns the image width in pixels of this RasterImage object.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Returns the image height in pixels of this RasterImage object.
  */
  OdUInt32 pixelHeight() const;
  /** \details
    Returns the number of bits per pixel used for colors by this RasterImage object.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Returns the number of colors in the palette of this RasterImage object.
  */
  OdUInt32 numColors() const;
  /** \details
    Returns the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Returns the palette in BMP format of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Returns the scanline size of this RasterImage object.
    
    \remarks
    This function returns the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Returns the specified set of scanlines in BMP format for this RasterImage object,
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
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Returns the pixel format for this RasterImage object.
    
    \remarks
    This function returns the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  OdGiRasterImage::PixelFormatInfo pixelFormat() const;
  /** \details
    Returns the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Returns the default image resolution in pixels per unit of this RasterImage object. 

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction). 
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \remarks If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  OdGiRasterImage::Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
  /** \details
    Returns the ParamType bitflags with which parameters are supported by this interface.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Returns the image source.
  */
  OdGiRasterImage::ImageSource imageSource() const;
  /** \details
    Returns the transparency mode for 32bpp formats.
  */
  OdGiRasterImage::TransparencyMode transparencyMode() const;
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(OdGiRasterImage::TransparencyMode mode);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    Extends the OdGiRasterImageBGRA32 class to keep a copy of OdGiImageBGRA32 inside.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiRasterImageBGRA32Holder : public OdGiRasterImageBGRA32
{
protected:
  OdGiImageBGRA32 m_ImageCopy;
public:
  OdGiRasterImageBGRA32Holder();
  ~OdGiRasterImageBGRA32Holder();
  
  /** \details
    Creates an OdGiRasterImageBGRA32Holder object with specified parameters.
    \param pImage [in]  Input BGRA32 image.
    \param transparencyMode [in]  Transparency mode.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdGiImageBGRA32 *pImage, OdGiRasterImage::TransparencyMode transparencyMode = OdGiRasterImage::kTransparency8Bit);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    Extends the OdGiRasterImageBGRA32 class to keep a copy of OdGiImageBGRA32 and pixels inside.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiRasterImageBGRA32Copy : public OdGiRasterImageBGRA32Holder
{
private:
  OdGiPixelBGRA32Array m_pxArray;
public:
  OdGiRasterImageBGRA32Copy();
  ~OdGiRasterImageBGRA32Copy();
  
  /** \details
    Creates an OdGiRasterImageBGRA32Copy object with specified parameters.
    \param pImage [in]  Input BGRA32 image.
    \param transparencyMode [in]  Transparency mode.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(OdGiImageBGRA32 *pImage, OdGiRasterImage::TransparencyMode transparencyMode = OdGiRasterImage::kTransparency8Bit);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This class stores a full copy of original raster image data.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiRasterImageHolder : public OdGiRasterImageParam
{
  OdUInt32                          m_pixelWidth, m_pixelHeight, m_colorDepth, m_alignment;
  OdGiRasterImage::PixelFormatInfo  m_pixFmt;
  Units                             m_units;
  double                            m_xPelsPerUnit, m_yPelsPerUnit;
  OdGiRasterImage::ImageSource      m_imageSource;
  OdString                          m_fileName;
  OdGiRasterImage::TransparencyMode m_transparencyMode;
  int                               m_transparentColor;
  OdUInt8Array                      m_palData;
  OdUInt8Array                      m_pixData;
public:
  OdGiRasterImageHolder();
  virtual ~OdGiRasterImageHolder();

  /** \details
    Stores data from an original OdGiRasterImage object.
    \param pOrig [in]  Pointer to the original image object.
  */
  void setFrom(const OdGiRasterImage* pOrig);

  /** \details
    Creates an OdGiRasterImageHolder object from an original OdGiRasterImage object.
    \param pImage [in]  Pointer to the original image object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pImage);
  /** \details
    Returns the image width in pixels of this RasterImage object.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Returns the image height in pixels of this RasterImage object.
  */
  OdUInt32 pixelHeight() const;
  /** \details
    Returns the default image resolution in pixels per unit of this RasterImage object. 

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction). 
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \remarks If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
  /** \details
    Returns the number of bits per pixel used for colors by this RasterImage object.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Returns the number of colors in the palette of this RasterImage object.
  */
  OdUInt32 numColors() const;
  /** \details
    Returns the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Returns the scanline size of this RasterImage object.
    
    \remarks
    This function returns the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Returns the specified set of scanlines in BMP format for this RasterImage object,
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
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Returns the pixel format for this RasterImage object.
    
    \remarks
    This function returns the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  PixelFormatInfo pixelFormat() const;
  /** \details
    Returns the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Returns the index of transparent color in the image.

    \remarks
    If there is no transparent color, -1 should be returned.
	  The default implementation of this function returns -1.
  */
  int transparentColor() const;
  /** \details
    Returns the image source.
  */
  ImageSource imageSource() const;
  /** \details
    Returns the image source file name.
  */
  const OdString &sourceFileName() const;
  /** \details
    Returns the transparency mode for 32bpp formats.
  */
  TransparencyMode transparencyMode() const;
  /** \details
    Returns the ParamType bitflags with which parameters are supported by this interface.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Sets an image source.
    \param source [in]  New image source value.
  */
  void setImageSource(ImageSource source);
  /** \details
    Sets an image source file name.
    \param fileName [in]  New value for image source file name.
  */
  void setSourceFileName(const OdString &fileName);
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(TransparencyMode mode);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This class is the base class for raster image transformer objects.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.

    The default methods for this class do nothing but return the
    corresponding values from the original object.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiRasterImageWrapper : public OdGiRasterImageParam
{
  OdGiRasterImagePtr m_pOrig;
public:
  OdGiRasterImageWrapper();
  virtual ~OdGiRasterImageWrapper();

  /** \details
    Sets the original OdGiRasterImage object associated with this RasterImageWrapper object.
    \param pOrig [in]  Pointer to the original image object.
  */
  void setOriginal(const OdGiRasterImage* pOrig);
  /** \details
    Returns the original OdGiRasterImage object associated with this RasterImageWrapper object.
  */
  const OdGiRasterImage *original() const;
  /** \details
    Copies the original OdGiRasterImage object associated with this RasterImageWrapper object.
  */
  OdGiRasterImagePtr cloneOriginal() const;
  /** \details
    Returns the image width in pixels of this RasterImage object.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Returns the image height in pixels of this RasterImage object.
  */
  OdUInt32 pixelHeight() const;
  /** \details
    Returns the default image resolution in pixels per unit of this RasterImage object. 

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction). 
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \remarks If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
  /** \details
    Returns the number of bits per pixel used for colors by this RasterImage object.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Returns the number of colors in the palette of this RasterImage object.
  */
  OdUInt32 numColors() const;
  /** \details
    Returns the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Returns the scanline size of this RasterImage object.
    
    \remarks
    This function returns the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
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
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Returns the pixel format for this RasterImage object.

    \remarks
    This function returns the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  PixelFormatInfo pixelFormat() const;
  /** \details
    Returns the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Returns the index of transparent color in the image.

    \remarks If there is no transparent color, -1 should be returned. 
	  The default implementation of this function returns -1.
  */
  int transparentColor() const;
  /** \details
    Returns the image source.
  */
  ImageSource imageSource() const;
  /** \details
    Returns the image source file name.
  */
  const OdString &sourceFileName() const;
  /** \details
    Returns the transparency mode for 32bpp formats.
  */
  TransparencyMode transparencyMode() const;
  /** \details
    Returns the ParamType bitflags with which parameters are supported by this interface.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Sets an image source.
    \param source [in]  New image source value.
  */
  void setImageSource(ImageSource source);
  /** \details
    Sets an image source file name.
    \param fileName [in]  New value for image source file name.
  */
  void setSourceFileName(const OdString &fileName);
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(TransparencyMode mode);
  /** \details
    Returns the user-end implementation of raster image functionality.
    \remarks
    If not overridden by a user-end raster image implementation, always returns null.
  */
  void* imp() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This raster image transformer class transforms OdGiRasterImage objects to Bitonal images.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiBitonalRasterTransformer : public OdGiRasterImageWrapper
{
  RGBQUAD m_palette[2];
  int m_transpColor;
public:
  OdGiBitonalRasterTransformer();
  virtual ~OdGiBitonalRasterTransformer();

  /** \details
      Creates a bitonal raster transformer.
      \param pOrig [in]  Original raster image pointer.
      \param foregroundColor [in]  Foreground color.
      \param backgroundColor [in]  Background color.
      \param transparent [in]  Image is transparent if and only if true.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig, ODCOLORREF foregroundColor, ODCOLORREF backgroundColor, bool transparent);

  /** \details
      Sets bitonal raster transformer parameters.
      \param pOrig [in]  Original raster image pointer.
      \param foregroundColor [in]  Foreground color.
      \param backgroundColor [in]  Background color.
      \param transparent [in]  Image is transparent if and only if true.
  */
  void setOriginal(const OdGiRasterImage* pOrig, ODCOLORREF foregroundColor, ODCOLORREF backgroundColor, bool transparent);

  /** \details
      Returns the number of colors in the palette of this RasterImage object.
  */
  OdUInt32 numColors() const;
  /** \details
      Returns the number of colors in the palette of this RasterImage object.

      \param colorIndex [in]  Color index.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
      Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
      Returns the palette in BMP format of this RasterImage object.

      \param bytes [out] Output palette data.

      \remarks It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
      Returns the pixel format for this RasterImage object.

      \remarks This function returns the pixel format of the palette, unless there is no palette, in which case it returns the pixel format of the image itself.
  */
  PixelFormatInfo pixelFormat() const;
  /** \details
      Returns the index of transparent color in the image.

      \remarks
      If there is no transparent color, -1 should be returned.
	    The default implementation of this function returns -1.
  */
  int transparentColor() const;
  /** \details
      Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
      Copies data of a source object to the current object.

      \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This raster image transformer class mirrors an image upside down.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiUpsideDownRasterTransformer : public OdGiRasterImageWrapper
{
public:
  OdGiUpsideDownRasterTransformer();
  virtual ~OdGiUpsideDownRasterTransformer();

  /** \details
    Creates a new upside down image transformer.
    \param pOrig [in]  Original raster image pointer.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig);
  /** \details
    Returns the specified set of scanlines in BMP format for this RasterImage object,
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
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This raster image transformer class mirrors an image right to left.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiLeftToRightRasterTransformer : public OdGiRasterImageWrapper
{
public:
  OdGiLeftToRightRasterTransformer();
  virtual ~OdGiLeftToRightRasterTransformer();

  /** \details
    Creates a new left to right image transformer.
    \param pOrig [in]  Original raster image pointer.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig);
  /** \details
    Returns the specified set of scanlines in BMP format for this RasterImage object,
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
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    the scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};

/** \details
    This raster image transformer class negates image colors.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiInversionRasterTransformer : public OdGiRasterImageWrapper
{
public:
  OdGiInversionRasterTransformer();
  virtual ~OdGiInversionRasterTransformer();

  /** \details
    Creates a new inversion image transformer.
    \param pOrig [in]  Original raster image pointer.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig);
  /** \details
    Returns the number of colors in the palette of this RasterImage object.

    \param colorIndex [in]  Color index.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Returns the palette in BMP format of this RasterImage object.

    \param bytes [out] Output palette data.

    \remarks It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  
  /** \details
    Returns the specified set of scanlines in BMP format for this RasterImage object,
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
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
protected:
  virtual ODCOLORREF colorXform(ODCOLORREF color) const;
};

/** \details
    This raster image transformer class converts image colors into grayscale.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiGrayscaleRasterTransformer : public OdGiInversionRasterTransformer
{
public:
  OdGiGrayscaleRasterTransformer();
  virtual ~OdGiGrayscaleRasterTransformer();

  /** \details
      Creates a new grayscale image transformer.
      \param pOrig [in]  Original raster image pointer.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig);
  /** \details
      Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
      Copies data of a source object to the current object.

      \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
protected:
  virtual ODCOLORREF colorXform(ODCOLORREF color) const;
};

/** \details
    This raster image transformer class converts image colors into monochrome.

    Corresponding C++ library: TD_Gi

    \remarks
    Transforming an image does not affect the orignal.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMonochromaticRasterTransformer : public OdGiGrayscaleRasterTransformer
{
  int m_threshold;
public:
  OdGiMonochromaticRasterTransformer();
  virtual ~OdGiMonochromaticRasterTransformer();

  /** \details
      Creates a new monochromatic image transformer.
      \param pOrig [in]  Original raster image pointer.
      \param threshold [in]  Threshold between light and dark image components.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig, int threshold = 127);
  /** \details
      Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
      Copies data of a source object to the current object.

      \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
  /** \details
      Sets a threshold value to this object.

      \param treshold [in] Input threshold value.
  */
  void setThreshold(OdUInt8 treshold) { m_threshold = treshold; if (m_threshold > 254) m_threshold = 254; };
  /** \details
      Returns the threshold value of this object.
  */
  int threshold() const { return m_threshold; }
protected:
  virtual ODCOLORREF colorXform(ODCOLORREF color) const;
};

/** \details
    Extends the original OdGiRasterImage with alpha channel data.
    \remarks
    Stores only a pointer to the original image, not a SmartPointer, so deletion of the original image before
    this class may cause an access violation error.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiRasterImageAlphaChannelAdder : public OdGiRasterImageParam
{
protected:
  const OdGiRasterImage*  m_pOrigImage;
  ODCOLORREF              m_clPixelAlpha;
  OdUInt8                 m_nPixelAlphaThreshold;
public:
  OdGiRasterImageAlphaChannelAdder();
  virtual ~OdGiRasterImageAlphaChannelAdder();
  
  /** \details
    Creates an OdGiRasterImageAlphaChannelAdder object with the specified parameters.
    \param pOrig [in]  Input original image.
    \param cutColor [in]  Alpha pixel color.
    \param threshold [in] Alpha pixel color threshold.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  static OdGiRasterImagePtr createObject( const OdGiRasterImage* pOrig, ODCOLORREF cutColor, OdUInt8 threshold = 0 );

  /** \details
    Returns the scanline size of this RasterImage object.
    
    \remarks
    This function returns the number of bytes between the beginning of scanline N and
    the beginning of scanline N+1 (taking into account any padding that is added to the 
    end of the scanline).
  */
  OdUInt32 scanLineSize() const;
  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Returns the specified set of scanlines in BMP format for this RasterImage object,
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
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;

  /** \details
    Returns the image width in pixels of this RasterImage object.
  */
  OdUInt32 pixelWidth() const;
  /** \details
    Returns the image height in pixels of this RasterImage object.
  */
  OdUInt32 pixelHeight() const;
  /** \details
    Returns the number of bits per pixel used for colors by this RasterImage object.
  */
  OdUInt32 colorDepth() const;
  /** \details
    Returns the number of colors in the palette of this RasterImage object.
  */
  OdUInt32 numColors() const;
  /** \details
    Returns the color for the specified index from the palette of this RasterImage object.
    \param colorIndex [in]  Color index.
  */
  ODCOLORREF color(OdUInt32 colorIndex) const;
  /** \details
    Returns the size (in bytes) of the palette data of this RasterImage object.
  */
  OdUInt32 paletteDataSize() const;
  /** \details
    Returns the palette in BMP format of this RasterImage object.
    \param bytes [out]  Receives the palette data.
    
    \remarks
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const;
  /** \details
    Returns the pixel format for this RasterImage object.
    
    \remarks
    This function returns the pixel format of the palette,
    unless there is no palette, in which case it returns
    the pixel format of the image itself.
  */
  OdGiRasterImage::PixelFormatInfo pixelFormat() const;
  /** \details
    Returns the scanline alignment, in bytes, for this RasterImage object.
    Example:
    scanLinesAlignment() returns 4 for Windows BMP.
  */
  OdUInt32 scanLinesAlignment() const;
  /** \details
    Returns the default image resolution in pixels per unit of this RasterImage object. 

    \param xPelsPerUnit [out]  Receives the pixels per unit value (x direction). 
    \param yPelsPerUnit [out]  Receives the pixels per unit value (y direction).

    \remarks If the returned value is kNone, then xPelsPerUnit and yPelsPerUnit are not set.
  */
  OdGiRasterImage::Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
  /** \details
    Returns the ParamType bitflags with which parameters are supported by this interface.
  */
  OdUInt32 supportedParams() const;
  /** \details
    Returns the image source.
  */
  OdGiRasterImage::ImageSource imageSource() const;
  /** \details
    Returns the transparency mode for 32bpp formats.
  */
  OdGiRasterImage::TransparencyMode transparencyMode() const;
  /** \details
    Sets an image transparency mode.
    \param mode [in]  New transparency mode value.
  */
  void setTransparencyMode(OdGiRasterImage::TransparencyMode mode);
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);

protected:
  OdUInt8 getAlphaValue( OdUInt8 red, OdUInt8 green, OdUInt8 blue ) const;
};

#if 0 // @@@TODO: implement at least nearest, bilinear and bicubic resamplers
/** \details
    This is a raster image resampler class with the simplest, nearest interpolation filter.

    Corresponding C++ library: TD_Gi

    \remarks
    Resampling an image does not affect the orignal.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiNearestRasterResampler : public OdGiRasterImageWrapper
{
  OdUInt32 m_newPixWidth, m_newPixHeight;
public:
  OdGiNearestRasterResampler();
  virtual ~OdGiNearestRasterResampler();

  /** \details
    Create new image nearest interpolation resampler.
    \param pOrig [in]  Original raster image pointer.
    \param newPixWidth [in]  New image width.
    \param newPixHeight [in]  New image width.
  */
  static OdGiRasterImagePtr createObject(const OdGiRasterImage* pOrig, OdUInt32 newPixWidth, OdUInt32 newPixHeight);
  /** \details
    Returns the specified set of scanlines in BMP format for this RasterImage object,
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
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;
  
  /** \details
    Returns the set of scanlines.

    \remarks
    Implementation of this function is optional; NULL can be returned
    if it is inconvenient to implement. The caller must take
    into account that the return value can be NULL. If this
    method returns NULL, scanlines can be accessed by a second version
    of the scanLines() method with arguments where a memory buffer for
    scalines data is preallocated by the caller.
  */
  const OdUInt8* scanLines() const;
  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  OdRxObjectPtr clone() const;
  /** \details
    Copies data of a source object to the current object.

    \param pSource [in] Input source object.
  */
  void copyFrom(const OdRxObject *pSource);
};
#endif

#include "TD_PackPop.h"

#endif //  __OD_GI_RASTER_WRAPPERS__
