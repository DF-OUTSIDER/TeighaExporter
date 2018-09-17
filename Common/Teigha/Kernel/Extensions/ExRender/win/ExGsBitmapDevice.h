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

#ifndef ODEXGSBITMAPDEVICE_INCLUDED
#define ODEXGSBITMAPDEVICE_INCLUDED

#include "TD_PackPush.h"

#include "Gs/GsBMPDevice.h"
#include "Gs/GsBaseVectorizeDevice.h"
#include "Gi/GiRasterImage.h"
#include "UInt8Array.h"
#include "RxDispatchImpl.h"

/** This class encapsulates Device Independent Bitmap objects.
  Library: Source code provided. 
  <group ExRender_Windows_Classes> 
*/
class GsDIBSection
{
public:  
  
  GsDIBSection();
  virtual ~GsDIBSection();
  
  /** \details
    Creates a DIB for this DIBSection object.
    \param bitsPerPixel [in] Color depth.
    \param xPixels [in] X pixels.
    \param yPixels [in] Y pixels.
  */
  void createDib(int xPixels, int yPixels, int bitsPerPixel);
  
  /** \details
    Sets the DIB palette for this DIBSection object.
    \param hPalette [in]  Handle of the palette.
  */
  void setPalette(HPALETTE hPalette);

  /** \details
    Returns the size (in bytes) the palette data of this DIBSection object.
  */
  OdUInt32 paletteDataSize() const
  {
    return m_paletteData.size();
  }

  /** \details
    Returns the DIB palette data of this DIBSection object.
    \param bytes [out]  Receives the palette data.
    
    \note
    It is up to the caller to allocate sufficient memory for the palette data.
  */
  void paletteData(OdUInt8* bytes) const
  {
    memcpy(bytes, m_paletteData.getPtr(), m_paletteData.size());
  }

  /** \details
    Returns the specified color of the DIB palette of this DIBSection object.  
  */
  ODCOLORREF color(OdUInt32 colorIndex) const
  {
    colorIndex *= 4;
    return ODRGBA(
      m_paletteData[colorIndex + 2], 
      m_paletteData[colorIndex + 1], 
      m_paletteData[colorIndex], 
      m_paletteData[colorIndex + 3]);
  }

  /** \details
    Returns the scan line size of this DIBSection object.
    
    \remarks
    This function returns the number of bytes between the beginning of scan line N and
    the beginning of scan line N+1 (taking into account any padding that is added to the 
    end of the scan line).
  */
  OdUInt32 scanLineSize() const;

  /** \details
    Returns the specified set of scanlines in BMP format from this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \param scnLines [out]  Receives the scan line data.
    \param firstScanline [in]  Index of first scanline to retrieve.
    \param numLines [in]  Number of scanlines to retrieve.

    \remarks

    * The number of accessible scanlines is equal to value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \note
    It us up to the caller to allocate sufficient memory for the scan line data.

    \note
    Implementation of this function with no arguments is optional; NULL can be returned 
    if it is inconvenient to implement. The caller must take 
    into account that the return value can be NULL.
  */
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const;

  /** \details
    Returns the specified set of scanlines in BMP format from this RasterImage object,
    or the pixel data in BMP format for this RasterImage object.

    \remarks

    * The number of accessible scanlines is equal to value returned by pixelHeight().
    * The number of accessible bytes in a scanline is equal to the value returned by scanLineSize().
    * The scanline returned by firstScanline == 0 is the first scanline in the image.
    * The scanline returned by firstScanline == (pixelHeight() - 1) is the last scanline in the image.

    \note
    It us up to the caller to allocate sufficient memory for the scan line data.

    \note
    Implementation of this function with no arguments is optional; NULL can be returned 
    if it is inconvenient to implement. The caller must take 
    into account that the return value can be NULL.
  */
  const OdUInt8* scanLines() const;

  /** \details
    Returns the handle to the device context for this DIBSection object.
  */
  HDC getHDC() const
  {
    return m_hdc;
  }

  /** \details
    Sets the device context for this DIBSection object.
    \param hDC [in]  Handle to the device context.
  */
  void setHDC(HDC hDC)
  {
    m_hdc = hDC;
  }

  /** \details
    Returns the image width in pixels of this DIBSection object.
  */
  OdUInt32 getDibWidth() const;

  /** \details
    Returns the image height in pixels of this DIBSection object.
  */
  OdUInt32 getDibHeight() const;

  /** \details
    Returns the color depth of this DIBSection object.
  */
  int getBitsPerPixel() const;

  void         *m_pBits;
  OdUInt32      m_nSizeImage;  
protected:
  /** \details
    Destroys the DIB bitmap associated with this DIBSection object.
  */
  void    destroy();  
protected:  
  HBITMAP       m_hbmp;
  HBITMAP       m_hbmOld;

  HDC           m_hdc;
  HPALETTE      m_hPal;
  OdUInt8Array  m_paletteData;
};


/** Library: Source code provided. 
  <group ExRender_Windows_Classes> 
*/
template <class T>
class ExGsBitmapDevice : public OdGiRasterImage
                       , public T
{
protected:
  /*using OdGiRasterImage::operator new;
  using OdGiRasterImage::operator delete;*/
  /*ODRX_USING_HEAP_OPERATORS(OdGiRasterImage);*/
  
  // 24.09.2004 GU
  // neither using nor ODRX_USING_HEAP_OPERATORS(OdGiRasterImage)
  // can't be compiled under Borland due to unknown reasons

  void* operator new(size_t s) 
  { 
    return OdGiRasterImage::operator new(s); 
  }
  void operator delete(void* p) 
  { 
    OdGiRasterImage::operator delete(p);
  }
  void* operator new[](size_t s) 
  { 
    return OdGiRasterImage::operator new(s);
  }
  void operator delete[](void* p) 
  { 
    OdGiRasterImage::operator delete(p);
  }
  void *operator new(size_t s, void* p)
  { 
    return OdGiRasterImage::operator new(s, p);
  }
  void *operator new[](size_t s, void* p) 
  { 
    return OdGiRasterImage::operator new(s, p);
  }

  /** \details
    Sets the number of bits per pixel used for colors by this BitmapDevice object.
    \param bitsPerPixel [in]  Bits per pixel.
  */
  void putBitPerPixel(OdUInt32 bitsPerPixel)
  {
    T::m_nColorDepth = bitsPerPixel;
    T::m_bColorIndexMode = (T::m_nColorDepth < 16);
  }

public:
  ExGsBitmapDevice()
  {
  }

  virtual ~ExGsBitmapDevice()
  {
    T::deleteContext();
  }

  bool supportPartialUpdate() const
  {
    return OdGsBaseVectorizeDevice::supportPartialUpdate();
  }

  /** \details
    Returns the color depth for this BitmapDevice object.
  */
  virtual int getDeviceColorDepth() const
  {
    return T::m_nColorDepth;
  }

  /** \details
    Returns the palette for this BitmapDevice object.
  */
  ODGSPALETTE getDevicePalette()
  {
    return T::m_logPalette;
  }
  /** \details
    Updates the specified rectangle of this BitmapDevice object.
    \param pUpdatedRectang [in]  Pointer to the rectangle to receive the region updated by this function.
  */
  void update(OdGsDCRect* pUpdatedRect = 0)
  {
    m_dibSection.createDib(T::width(), T::height(), T::m_nColorDepth);
    T::update(pUpdatedRect);
  }

  OdUInt32 pixelWidth() const 
  {
    return m_dibSection.getDibWidth();
  }

  OdUInt32 pixelHeight() const 
  {
    return m_dibSection.getDibHeight();
  }


  /** \note
    As implemented, this function does nothing but return kNone.
  */
  Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const
  {
    xPelsPerUnit = yPelsPerUnit = 0.0;
    return kNone;
  }

  OdUInt32 scanLineSize() const
  {
    return m_dibSection.scanLineSize();
  }

  void scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines = 1) const
  {
    m_dibSection.scanLines(pBytes, index, numLines);
  }

  const OdUInt8* scanLines() const
  {
    return m_dibSection.scanLines();
  }

  OdUInt32 paletteDataSize() const
  {
    return m_dibSection.paletteDataSize();
  }
  
  void paletteData(OdUInt8* pBytes) const
  {
    m_dibSection.paletteData(pBytes);
  }

  virtual OdUInt32 numColors() const
  {
    return paletteDataSize() / 4;
  }

  virtual ODCOLORREF color(OdUInt32 colorIndex) const
  {
    return m_dibSection.color(colorIndex);
  }

  virtual OdUInt32 colorDepth() const
  {
    return m_dibSection.getBitsPerPixel();
  }

  PixelFormatInfo pixelFormat() const
  {
    PixelFormatInfo res;
    res.setBGR();
    return res;
  }

  OdUInt32 scanLinesAlignment() const
  {
    return 4;
  }

  /** \details
    Returns a SmartPointer to this BitmapDevice object.
  */
  OdGiRasterImagePtr getRasterImage() 
  { 
    return this; 
  }

  /** \details
    This function does nothing but return.
  */
  void putRasterImage(OdGiRasterImage* ) {}

  /** \details
    Returns the number of bits per pixel used for colors by this BitmapDevice object.
  */
  OdUInt32 get_BitPerPixel() const 
  { 
    return colorDepth();
  }

  /** \details
    Sets the number of bits per pixel used for colors by this BitmapDevice object.
    \param bitsPerPixel [in]  Bits per pixel.
  */
  void put_BitPerPixel(OdUInt32 bitsPerPixel)
  {
    putBitPerPixel(bitsPerPixel);
  }
  /** \details
    Returns the a pointer to the dictionary containing the properties information for this BitmapDevice object.
    \remarks
    If no dictionary exists, one will be created.
  */
  OdRxDictionary* propertiesInfo() const
  {
    static OdRxDictionaryPtr pInfo;
    if(pInfo.isNull())
    {
      pInfo = ::odrxCreateRxDictionary();
      this->generatePropMap(pInfo);
    }
    return pInfo.get();
  }

  OdGiRasterImagePtr snapshotImageParams() const
  {
    return this;
  }
  OdGiRasterImagePtr snapshotRegion(const OdGsDCRect &area, bool bCrop) const
  {
    if (bCrop)
      return crop(area.m_min.x, area.m_min.y, area.m_max.x - area.m_min.x, area.m_max.y - area.m_min.y);
    return this;
  }
protected:
  /** \details
    Creates a context for this Device object.
  */
  virtual void createContext()
  {
    this->deleteContext();
    T::m_hDC = m_dibSection.getHDC();
    T::createContext();
  }

  /** \details
    Returns true if and only if the vectorization is to be rendered to the screen.
    \note
    The default implementation of this function always returns false.
  */
  bool renderToScreen() const { return false; }

  virtual void createPalette(ODGSPALETTE *logicalPalette)
  {
    T::createPalette(logicalPalette);

    if (T::m_pPal)
    {
      m_dibSection.setPalette(T::m_pPal);
    }
  }

  GsDIBSection  m_dibSection;
};

#include "TD_PackPop.h"

#endif // ODEXGSBITMAPDEVICE_INCLUDED
