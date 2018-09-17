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

#ifndef _OD_ODEXGIRASTERIMAGE_H_
#define _OD_ODEXGIRASTERIMAGE_H_

#include "TD_PackPush.h"

#include "OdBinaryData.h"
#include "Gi/GiRasterImage.h"
#include "FlatMemStream.h"
#include "OdPlatform.h"
#include "RxRasterServices.h"
#include "StaticRxObject.h"

class OdExGiRasterImage;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdExGiRasterImage object pointers.
*/
typedef OdSmartPtr<OdExGiRasterImage> OdExGiRasterImagePtr;
/** \details
    This class represents a raster image within the Teigha vectorization framework.
    <group ExServices_Classes> 
    Library: Source code provided. 
*/
class OdExGiRasterImage : public OdGiRasterImageParam
{
public:
  OdRxObjectPtr clone() const
  {
    OdSmartPtr<OdExGiRasterImage> pRes = OdRxObjectImpl<OdExGiRasterImage>::createObject();
    pRes->m_bits = m_bits;
    pRes->m_palette = m_palette;
    pRes->m_header = m_header;
    pRes->m_imageSource = m_imageSource;
    pRes->m_sourceFileName = m_sourceFileName;
    pRes->m_transparencyMode = m_transparencyMode;
    return (OdRxObject*)pRes;
  }

  struct Header
  {
    Header(OdUInt32 width, OdUInt32 height, OdUInt16 bitPerPixel)
      : m_width(width), m_height(height)
      , m_bitPerPixel(bitPerPixel)
      , m_xPelsPerUnit(0.0)
      , m_yPelsPerUnit(0.0)
      , m_resUnits(OdGiRasterImage::kNone)
     {}
    Header()
      : m_width(0), m_height(0)
      , m_bitPerPixel(0)
      , m_xPelsPerUnit(0.0)
      , m_yPelsPerUnit(0.0)
      , m_resUnits(OdGiRasterImage::kNone)
    {}
    
    OdUInt32  m_width;
    OdUInt32  m_height;
    OdUInt16  m_bitPerPixel;
    double    m_xPelsPerUnit,
              m_yPelsPerUnit;
    OdGiRasterImage::Units 
              m_resUnits;
  };
  
  class Palette
  {
    OdBinaryData m_colorsData;
  public:
    const OdUInt8* getData() const { return m_colorsData.getPtr(); }
    OdBinaryData& data() { return m_colorsData; }
    const OdBinaryData& getBinData() const { return m_colorsData; }
    OdUInt32 numColors() const { return m_colorsData.size() / 4; }
    void setNumColors(OdUInt32 nColors) { m_colorsData.resize(nColors * 4); }
    void setColorAt(OdUInt32 nIndex, OdUInt8 blue, OdUInt8 green, OdUInt8 red, OdUInt8 alpha = 0)
    {
      ODA_ASSERT(nIndex < m_colorsData.size());
      nIndex *= 4;
      m_colorsData[nIndex]     = blue;
      m_colorsData[nIndex + 1] = green;
      m_colorsData[nIndex + 2] = red;
      m_colorsData[nIndex + 3] = alpha;
    }
    
    void colorAt(OdUInt32 nIndex, OdUInt8& blue, OdUInt8& green, OdUInt8& red, OdUInt8* pAlpha = 0) const
    {
      ODA_ASSERT(nIndex < m_colorsData.size());
      nIndex *= 4;
      blue    = m_colorsData[nIndex];
      green   = m_colorsData[nIndex + 1];
      red     = m_colorsData[nIndex + 2];
      if(pAlpha)
      {
        *pAlpha = m_colorsData[nIndex + 3];
      }
    }
  };
  
  private:
    Header           m_header;
    OdBinaryData     m_bits;
    Palette          m_palette;
    ImageSource      m_imageSource;
    OdString         m_sourceFileName;
    TransparencyMode m_transparencyMode;
  public:
    /** \param bitsPerPixel [in]  Color depth.
      \param xPixels [in]  X pixels.
      \param yPixels [in]  Y pixels.
    */
    OdExGiRasterImage(OdUInt32 xPixels, OdUInt32 yPixels, OdUInt8 bitsPerPixel);
    OdExGiRasterImage();
    
    /** \details
      Returns the size in bytes of the pixel data for a specified RasterImage object.
      \param bitsPerPixel [in]  Color depth.
      \param xPixels [in]  X pixels.
      \param yPixels [in]  Y pixels.
    */
    static OdUInt32 bitDataSize(OdUInt32 xPixels, OdUInt32 yPixels, OdUInt16 bitsPerPixel)
    {
      return ((xPixels * bitsPerPixel +31) & ~31) /8 * yPixels;
    }
    
    /** \details
      Returns the size in bytes of the pixel data for this RasterImage object.
      \param bitsPerPixel [in]  Color depth.
      \param xPixels [in]  X pixels.
      \param yPixels [in]  Y pixels.
    */
    OdUInt32 bitDataSize()
    {
      return OdExGiRasterImage::bitDataSize(m_header.m_width, m_header.m_height, 
        m_header.m_bitPerPixel);
    }
    
    /** \details
      Returns a reference to the binary data for this RasterImage object.
    */
    OdBinaryData& bits() { return m_bits; }
    /** \details
      Returns a the binary data for this RasterImage object.
    */
    const OdBinaryData& getBits() const { return m_bits; }
 
    /** \details
      Returns an array of scan lines for this RasterImage object.
      \param numBytes [out]  Receives the number of bytes in the array.
    */
    const OdUInt8* getScanLines(OdUInt32& numBytes) const
    {
      numBytes = m_bits.size();
      return m_bits.asArrayPtr();
    }

    /** \details
      Sets the binary data for this RasterImage object.
      \param data [in]  Binary data.
      \param numBytes [in]  Number of bytes in the array.
    */
        void setBits(const OdBinaryData& data)
    {
      ODA_ASSERT(data.size() <= bitDataSize());
      m_bits = data;
    }
    
    void setBits(const OdUInt8* data, OdUInt32 numBytes)
    {
      ODA_ASSERT(numBytes <= bitDataSize());
      m_bits.resize(numBytes);
      ::memcpy(m_bits.asArrayPtr(), data, numBytes);
    }
    
    /** \details
      Sets the metrics of this ReasterImage object.
      
      \param bitsPerPixel [in]  Color depth.
      \param xPixels [in]  X pixels.
      \param yPixels [in]  Y pixels.
    */
    void setMetrics(OdUInt32 xPixels, OdUInt32 yPixels, OdUInt16 bitsPerPixel)
    {
      m_header.m_width = xPixels;
      m_header.m_height = yPixels;
      m_header.m_bitPerPixel = bitsPerPixel;
      m_transparencyMode = (bitsPerPixel < 32) ? kTransparencyDef : kTransparency8Bit;
    }
    
    /** \details
      Sets the default image resolution in pixels per unit of this RasterImage object.

      \param xPelsPerUnit [in]  Pixels per unit value (x direction).
      \param yPelsPerUnit [in]  Pixels per unit value (y direction).
      \param units [in]  Units.
    */
     void setDefaultResolution(OdGiRasterImage::Units units, double xPelsPerUnit, double yPelsPerUnit)
    {
      m_header.m_resUnits = units;
      m_header.m_xPelsPerUnit = xPelsPerUnit;
      m_header.m_yPelsPerUnit = yPelsPerUnit;
    }
    
    /** \details
      Sets the number of colors in the palette of this RasterImage object.
      \param numColors [in]  Number of colors.
    */
    void setPalNumColors(OdUInt32 numColors)
    {
      m_palette.setNumColors(numColors);
    }
    
    /** \details
      Returns the number of colors in the palette of this RasterImage object.
    */
    OdUInt32 getPalNumColors() const
    {
      return m_palette.numColors();
    }
    
    /** \details
      Returns the palette of this RasterImage object.
    */
    const Palette& getPalette() const
    {
      return m_palette;
    }

    /** \details
      Returns the size in bytes of the palette of this RasterImage object.
    */
    OdUInt32 paletteDataSize() const
    {
      return m_palette.getBinData().size();
    }

    /** \details
        Returns the palette data of this RasterImage object.
        \param bytes [in]  Receives the data.
    */
    void paletteData(OdUInt8* bytes) const
    {
      const OdBinaryData& palData = m_palette.getBinData();
      memcpy(bytes, palData.asArrayPtr(), palData.size());
    }

    /** \details
        Returns the palette of this RasterImage object.
        \param bytes [in]  Receives the data.
    */
    Palette& palette()
    {
      return m_palette;
    }
    /** \details
      Sets the specified color in the palette of this RasterImage object.
      
      \param paletteIndex [in]  Color number.
      \param blue [in]  Blue component.
      \param green [in]  Green component.
      \param red [in]  Red component.
      \param alpha [in]  Alpha component.
    */
    void setPalColorAt(OdUInt32 paletteIndex, OdUInt8 blue, OdUInt8 green, OdUInt8 red, OdUInt8 alpha = 0)
    {
      m_palette.setColorAt(paletteIndex, blue, green, red, alpha);
    }
    
    /** \details
      Returns the specified color in the palette of this RasterImage object.
      
      \param paletteIndex [in]  Color number.
      \param blue [out]  Receives the Blue component.
      \param green [out]  Receives the Green component.
      \param red [out]  Receives the Red component.
      \param pAlpha [out]  Pointer to the object to receive the alpha component..
    */
    void getPalColorAt(OdUInt32 paletteIndex, OdUInt8& blue, OdUInt8& green, OdUInt8& red, OdUInt8* pAlpha = 0) const
    {
      m_palette.colorAt(paletteIndex, blue, green, red, pAlpha);
    }
    
    // Gets the color(RGB) in specified point of raster image

    /** \details
      Returns the specified color at the specified pixel of this RasterImage object.
      
      \param x [in]  X-coordinate.
      \param y [in]  Y-Coordinate
      \param blue [out]  Receives the Blue component.
      \param green [out]  Receives the Green component.
      \param red [out]  Receives the Red component.
    */
    void getColorAt(OdUInt32 x, OdUInt32 y, OdUInt8& blue, OdUInt8& green, OdUInt8& red) const;
    
    // virtual overrides

    void copyFrom(const OdRxObject* pOtherObj);
    OdUInt32 pixelWidth() const;
    OdUInt32 pixelHeight() const;
    OdUInt32 colorDepth() const;
    OdGiRasterImage::Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const;
    
    /** Returns bitmap info header
    */
    // void getBitmapInfoHeader(BITMAPINFOHEADER& bmih) const;

    
    OdUInt32 numColors() const;

    virtual ODCOLORREF color(OdUInt32 colorIndex) const;

    void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;

    /** \details
        Returns pointer to image's pixels in bmp format.
        Note that it is optional to have implementation of this function
        (to prevent dummy copying of pixels data). You can return NULL in your
        implementation if it is inconvenient in some case - any functionality
        uses this interface must take it into account.
    */

    virtual const OdUInt8* scanLines() const;

    virtual PixelFormatInfo pixelFormat() const;
    
    virtual OdUInt32 scanLinesAlignment() const;

    virtual OdUInt32 supportedParams() const;
    virtual ImageSource imageSource() const;
    virtual void setImageSource(ImageSource source);
    virtual const OdString &sourceFileName() const;
    virtual void setSourceFileName(const OdString &fileName);
    virtual TransparencyMode transparencyMode() const;
    virtual void setTransparencyMode(TransparencyMode mode);
};

/** \details
    This class implements platform-dependent loading and saving
    of Raster Image files for Teigha.
 
    <group ExServices_Classes> 
    Library: Source code provided. 
*/
class ExRasterModule : public OdRxRasterServices
{
public:
  /** \details
      Loads the specified Raster Image file.
      \param filename [in]  Filename of the Raster Image file to be read.
      \param pFlagsChain [in]  Optional zero-terminated loading flag pairs array.

      \remarks
      The returned pointer is expected to be passed to OdGiViewportGeometry::rasterImageDc().
  */
  OdGiRasterImagePtr loadRasterImage(const OdString &filename, const OdUInt32 *pFlagsChain = NULL);
  
  /** \details
      Loads the specified Raster Image file.

      \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
      \param pFlagsChain [in]  Optional zero-terminated loading flag pairs array.

      \remarks
      The returned pointer is expected to be passed to OdGiViewportGeometry::rasterImageDc().
  */
  OdGiRasterImagePtr loadRasterImage(OdStreamBuf *pStreamBuf, const OdUInt32 *pFlagsChain = NULL);

  /** \details
      Saves specified Raster Image to the specified file.
      \param rasterImage [in]  Raster image to be saved.
      \param filename [in]  Filename of the Raster Image file to be written.
      \param pFlagsChain [in]  Optional zero-terminated saving flag pairs array.
  */
  bool saveRasterImage(const OdGiRasterImage* rasterImage, const OdString& filename,
                       const OdUInt32 *pFlagsChain = NULL);
  
  /** \details
      Saves specified Raster Image to the specified file.
      \param rasterImage [in]  Raster image to be saved.
      \param filename [in]  Filename of the Raster Image file to be written.
      \param type [in]  Image format type to be written.
      \param pFlagsChain [in]  Optional zero-terminated saving flag pairs array.
  */
  bool saveRasterImage(const OdGiRasterImage* rasterImage, const OdString& filename,
                       OdUInt32 type, const OdUInt32 *pFlagsChain = NULL);

  /** \details
      Try to convert raster image (RGB) to JPEG or other type.
      \param pRaster [in]  Raster image to be converted.
      \param type [in]  Image format type to be converted.
      \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be stored.
      \param pFlagsChain [in]  Optional zero-terminated saving flag pairs array.
  */
  bool convertRasterImage(const OdGiRasterImage* pRaster, OdUInt32 type,
                          OdStreamBuf* pStreamBuf, const OdUInt32 *pFlagsChain = NULL);

  /** \details
      Try to convert raster image to other type.
      \param pSrcStream [in]  Pointer to the StreamBuf object from which the data are to be converted.
      \param pDstStream [in]  Pointer to the StreamBuf object to which the data are to be converted.
      \param type [in]  Image format type to be converted.
      \param pFlagsChainSrc [in]  Optional zero-terminated loading flag pairs array.
      \param pFlagsChainDst [in]  Optional zero-terminated saving flag pairs array.
  */
  bool convertRasterImage(OdStreamBuf* pSrcStream, OdStreamBuf* pDstStream, OdUInt32 type,
                          const OdUInt32 *pFlagsChainSrc = NULL, const OdUInt32 *pFlagsChainDst = NULL);

  /** \details
      Returns array of supported image format types.
  */
  OdUInt32Array getRasterImageTypes() const;

  /** \details
      Get file extension and filter name by type.
      \param type [in]  Image format type to be formatted.
      \param psFilterName [out]  Output filter name (can be Null).
  */
  OdString mapTypeToExtension(OdUInt32 type, OdString* psFilterName) const;

  /** \details
      Get image format type by file extension.
      \param extension [in]  File extension.
  */
  OdUInt32 mapExtensionToType(const OdString& extension) const;

  /** \details
      Try to detect image format type from input stream.
      \param filename [in]  Filename of the Raster Image file to be checked.
  */
  OdUInt32 getImageFormat(const OdString &filename) const;
  
  /** \details
      Try to detect image format type from input stream.

      \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be checked.
  */
  OdUInt32 getImageFormat(OdStreamBuf* pStreamBuf) const;

  /** \details
      Initialize module.
  */
  void initApp();
  /** \details
      Uninitialize module.
  */
  void uninitApp();
};

#include "TD_PackPop.h"

#endif //#ifndef _OD_ODEXGIRASTERIMAGE_H_
