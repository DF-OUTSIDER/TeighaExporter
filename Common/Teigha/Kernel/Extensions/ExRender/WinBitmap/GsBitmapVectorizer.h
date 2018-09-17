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

#ifndef __OD_GS_BITMAP_VECTORIZER__
#define __OD_GS_BITMAP_VECTORIZER__

#include "TD_PackPush.h"

#include "Gs/GsBaseInclude.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiHLRemover.h"
#include "Gi/GiPerspectivePreprocessor.h"
#include "Gi/GiOrthoClipper.h"
#include "Gi/GiXform.h"
#include "Gi/GiRasterImage.h"
#include "ExGsHelpers.h"
#include "ExColorConverterCallback.h"

/** \details
  This class implements device -independent bitmap Vectorizer devices.
  \sa
  Source code provided.
  <group ExRender_Classes> 
*/
class OdGsBitmapVectorizeDevice : public OdGsBaseVectorizeDevice,
                                  public OdGiRasterImage
{
  OdUInt32 m_width, m_height, m_scanLineLength; 
  OdGsDCRect m_clipRegion;
  OdUInt8Array m_scanLines;
  OdUInt8* m_pScanLines;
  bool m_bEnableHLR;
  bool m_bPrecisionMode;
  // Raster image rendering support
  OdUInt32 m_rasterWidth, m_rasterHeight, m_rasterScanLineLength;
  OdGiRasterImagePtr m_pRasterImagePtr; // Store raster image till processing.
  OdUInt8Array m_rasterScanLines; // Temporary buffer, if image haven't accessible scan lines ptr.
  const OdUInt8 *m_pRasterScanLines; // Pointer to raster image scanlines or temporary buffer.
  OdUInt8Array m_rasterPalette; // Temporary buffer of raster image palette entries.
  const ODCOLORREF *m_pRasterPalette; // Pointer to raster image palette or temporary buffer.
  OdGeMatrix3d m_rasterTransform;
  enum RasterImageBitDepth
  {
    kRIBDMono,
    kRIBD16Clr,
    kRIBD256Clr,
    kRIBD24Bpp,
    kRIBD32Bpp
  } m_rasterBitDepth;
  bool m_bRasterTransparency, m_bRasterRGB;
  enum RasterImage32TransparencyType
  {
    kRI32TTDef,
    kRI32TT1,
    kRI32TT8
  } m_raster32TranspMode;
  // NRC viewports clipping
  long m_nrcOffsetX, m_nrcOffsetY;
  OdUInt32 m_nrcWidth, m_nrcHeight;
  OdUInt8 *m_pnrcClip;

protected:
  ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);

  typedef void (*blitFunc)(const OdUInt8 *pTexScanLines, const ODCOLORREF *pTexPalette, OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr);
  blitFunc chooseBlitFuncFor() const;
public:
  OdGsBitmapVectorizeDevice();
  virtual ~OdGsBitmapVectorizeDevice();

  void update(OdGsDCRect* pUpdatedRect);
  void initMetrics();
  /** \details
    Sets the clip region for this Vectorizer device.
    \param clipRect [in] Clip rectangle.
  */
  void setClipRegion(const OdGsDCRect& clipRect);
  /** \details
    Returns the clip region for this Vectorizer device.
  */
  const OdGsDCRect &clipRegion() const { return m_clipRegion; }
  
  OdGsModelPtr createModel();

  /** \details
    Sets the color of the specified pixel of this Vectorizer device, and increments the pointer.

    \param pPixel [in/out] Pointer to the pixel.
    \param red [in]) Red component.
    \param green [in] Green component.
    \param blue [in] Blue component.
    \param alpha [in] Alpha component.
  */
  inline void putPixel(OdUInt8* pPixel, OdUInt8 red, OdUInt8 green, OdUInt8 blue, OdUInt8 alpha)
  {
#if defined(_MSC_VER) && (_MSC_VER < 1300)
    // #8473 : VC6 in release builds call both (alpha == 255 and alpha != 0) cases. This switch fixes VC6 optimizer bug.
    switch (alpha)
    {
      case 255:
      {
        *(pPixel++) = blue;
        *(pPixel++) = green;
        *pPixel     = red;
      }
      break;
      case 0:
      return;
      default:
      {
        OdMergeRGBAlpha(pPixel[2], pPixel[1], pPixel[0], red, green, blue, alpha, pPixel[2], pPixel[1], pPixel[0]);
      }
    }
#else
    if (alpha == 255)
    {
      *(pPixel++) = blue;
      *(pPixel++) = green;
      *pPixel     = red;
    }
    else if (alpha != 0)
    {
      OdMergeRGBAlpha(pPixel[2], pPixel[1], pPixel[0], red, green, blue, alpha, pPixel[2], pPixel[1], pPixel[0]);
    }
#endif
  }
  /** \details
    Sets the color of the specified pixel of this Vectorizer device, and increments the pointer.

    \param pPixel [in/out] Pointer to the pixel.
    \param red [in] Red component.
    \param green [in] Green component.
    \param blue [in] Blue component.
    \remarks
    This method doesn't supports Alpha color component.
  */
  inline void putPixel(OdUInt8* pPixel, OdUInt8 red, OdUInt8 green, OdUInt8 blue)
  {
    *(pPixel++) = blue;
    *(pPixel++) = green;
    *pPixel     = red;
  }
  /** \details
    Returns a pointer to the specified pixel of this Vectorizer device.
    \param x [in]  X-coordinate.
    \param y [in]  Y-coordinate.    
  */
  inline OdUInt8* getPixPtr(int x, int y)
  {
    return m_pScanLines + m_scanLineLength*y + x*3;
  }

  /** \details
    Sets the color of the specified pixel of this Vectorizer device.
    \param x [in]  X-coordinate.
    \param y [in]  Y-coordinate.
    \param red [in]  Red component.
    \param green [in]  Green component.
    \param blue [in]  Blue component.
    \param alpha [in]  Alpha component.
  */
  inline void putPixelAlias(int x, int y, OdUInt8 red, OdUInt8 green, OdUInt8 blue, OdUInt8 alpha, double coef)
  {
    if (x >= m_clipRegion.m_min.x && x <= m_clipRegion.m_max.x &&
        y >= m_clipRegion.m_min.y && y <= m_clipRegion.m_max.y)
    {
      if (!m_pnrcClip || m_pnrcClip[(y - m_nrcOffsetY) * m_nrcWidth + (x - m_nrcOffsetX)])
      {
        OdUInt8 *pPixel = getPixPtr(x, y);
        int makeAlpha = coef * alpha;
        if (makeAlpha > 255) makeAlpha = 255;
        if (makeAlpha == 255)
        {
          *(pPixel++) = blue;
          *(pPixel++) = green;
          *pPixel     = red;
        }
        else if (makeAlpha != 0)
        {
          OdMergeRGBAlpha(pPixel[2], pPixel[1], pPixel[0], red, green, blue, (OdUInt8)makeAlpha, pPixel[2], pPixel[1], pPixel[0]);
        }
      }
    }
  }

  /** \details
    Sets the color of the specified pixel of this Vectorizer device.
    \param x [in]  X-coordinate.
    \param y [in]  Y-coordinate.
    \param color [in]  Color.
  */
  inline void putPixel(int x, int y, ODCOLORREF color)
  {
    ODA_ASSERT(x <= m_clipRegion.m_max.x && y <= m_clipRegion.m_max.y && 
      x >= m_clipRegion.m_min.x && y >= m_clipRegion.m_min.y);
    putPixel(getPixPtr(x, y), ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color), ODGETALPHA(color));
  }

  /** \details
    Draws single point with this Vectorizer device.
    \param x [in]  X-coordinate.
    \param y [in]  Y-coordinate.
    \param color [in]  Color.
  */
  inline void draw_point(int x, int y, ODCOLORREF color)
  {
    y = m_height - y;
    if (x >= m_clipRegion.m_min.x && x <= m_clipRegion.m_max.x &&
        y >= m_clipRegion.m_min.y && y <= m_clipRegion.m_max.y)
    {
      if (!m_pnrcClip || m_pnrcClip[(y - m_nrcOffsetY) * m_nrcWidth + (x - m_nrcOffsetX)])
        putPixel(x, y, color);
    }
  }

  /** \details
    Draws the specified horizontal line with this Vectorizer device.
    \param y [in]  Y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param color [in]  Color.
  */
  inline void draw_horizontalLine(int y, int x1, int x2, ODCOLORREF color)
  {
    if(y <= (int)m_clipRegion.m_max.y && y >= m_clipRegion.m_min.y)
    {
      int i;
      if(x1 > x2)
      {
        i = x1; x1 = x2; x2 = i;
      }
      if(x1 < m_clipRegion.m_min.x)
        x1 = m_clipRegion.m_min.x;
      if(x2 > (int)m_clipRegion.m_max.x)
        x2 = m_clipRegion.m_max.x;
      if(x1 <= x2)
      {
        OdUInt8 blue  = ODGETBLUE(color);
        OdUInt8 green = ODGETGREEN(color);
        OdUInt8 red   = ODGETRED(color);
        OdUInt8 alpha = ODGETALPHA(color);
        OdUInt8* pPtr = m_pScanLines + m_scanLineLength*y + x1*3;
        if (!m_pnrcClip)
        {
          if (alpha == 255)
          {
            for(i = 0; i < x2-x1+1; i ++)
            {
              *(pPtr++) = blue;
              *(pPtr++) = green;
              *(pPtr++) = red;
            }
          }
          else if (alpha != 0)
          {
            for(i = 0; i < x2-x1+1; i ++)
            {
              OdMergeRGBAlpha(pPtr[2], pPtr[1], pPtr[0], red, green, blue, alpha, pPtr[2], pPtr[1], pPtr[0]);
              pPtr += 3;
            }
          }
        }
        else
        {
          OdUInt8 *pStencil = m_pnrcClip + (y - m_nrcOffsetY) * m_nrcWidth + (x1 - m_nrcOffsetX);
          if (alpha == 255)
          {
            for(i = 0; i < x2-x1+1; i ++)
            {
              if (*pStencil)
              {
                *(pPtr++) = blue;
                *(pPtr++) = green;
                *(pPtr++) = red;
              }
              else
              {
                pPtr += 3;
              }
              pStencil++;
            }
          }
          else if (alpha != 0)
          {
            for(i = 0; i < x2-x1+1; i ++)
            {
              if (*pStencil)
              {
                OdMergeRGBAlpha(pPtr[2], pPtr[1], pPtr[0], red, green, blue, alpha, pPtr[2], pPtr[1], pPtr[0]);
              }
              pPtr += 3;
              pStencil++;
            }
          }
        }
      }
    }
  }

  /** \details
    Draws the specified horizontal line with this Vectorizer device into NRC clip buffer.
    \param y [in]  Y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
  */
  inline void draw_horizontalLineClip(int y, int x1, int x2)
  {
    if (y <= (int)m_clipRegion.m_max.y && y >= m_clipRegion.m_min.y)
    {
      int i;
      if (x1 > x2)
      {
        i = x1; x1 = x2; x2 = i;
      }
      if (x1 < m_clipRegion.m_min.x)
        x1 = m_clipRegion.m_min.x;
      if (x2 > (int)m_clipRegion.m_max.x)
        x2 = m_clipRegion.m_max.x;
      if (x1 <= x2)
      {
        OdUInt8* pPtr = m_pnrcClip + m_nrcWidth * (y - m_nrcOffsetY) + (x1 - m_nrcOffsetX);
        for (i = 0; i < x2 - x1 + 1; i++, pPtr++)
        {
          //*pPtr = (*pPtr & ~2) | (~(*pPtr) & 2);
          *pPtr |= 2;
        }
      }
    }
  }

  /** \details
    Draws the specified line with this Vectorizer device.
    \param y1 [in]  First y-coordinate.
    \param y2 [in]  Second y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param color [in]  Color.
  */
  void draw_line(int x1, int y1, int x2, int y2, ODCOLORREF color);
  /** \details
    Draws the specified anti-aliased line with this Vectorizer device.
    \param y1 [in]  First y-coordinate.
    \param y2 [in]  Second y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param color [in]  Color.
  */
  void draw_lineAntiAlias(double x1, double y1, double x2, double y2, ODCOLORREF color);
  /** \details
    Draws the specified triangle with this Vectorizer device.
    \param y1 [in]  First y-coordinate.
    \param y2 [in]  Second y-coordinate.
    \param y3 [in]  Third y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param x2 [in]  Third x-coordinate.
    \param color [in]  Color.
  */
  void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, ODCOLORREF color);
  /** \details
    Draws the specified gouraud triangle with this Vectorizer device.
    \param y1 [in]  First y-coordinate.
    \param y2 [in]  Second y-coordinate.
    \param y3 [in]  Third y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param x2 [in]  Third x-coordinate.
    \param color1 [in]  First color.
    \param color2 [in]  Second color.
    \param color3 [in]  Third color.
  */
  void draw_gouraud_triangle(int x1, int y1, ODCOLORREF color1,
                             int x2, int y2, ODCOLORREF color2,
                             int x3, int y3, ODCOLORREF color3, OdUInt8 alpha = 255);
  /** \details
    Draws the specified textured triangle with this Vectorizer device.
    \param y1 [in]  First y-coordinate.
    \param y2 [in]  Second y-coordinate.
    \param y3 [in]  Third y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param x2 [in]  Third x-coordinate.
    \param pt1 [in]  Not transformed (world) coordinate of first point.
    \param pt2 [in]  Not transformed (world) coordinate of second point.
    \param pt3 [in]  Not transformed (world) coordinate of third point.
  */
  void draw_textured_triangle(int x1, int y1, const OdGePoint3d &pt1,
                              int x2, int y2, const OdGePoint3d &pt2,
                              int x3, int y3, const OdGePoint3d &pt3, OdUInt8 alpha = 255);
  
  /** \details
    Draws the specified textured triangle with this Vectorizer device.
    \param y1 [in]  First y-coordinate.
    \param y2 [in]  Second y-coordinate.
    \param y3 [in]  Third y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param x2 [in]  Third x-coordinate.
    \param pt1 [in]  Not transformed (world) coordinate of first point.
    \param pt2 [in]  Not transformed (world) coordinate of second point.
    \param pt3 [in]  Not transformed (world) coordinate of third point.
    \param wrap [in]  Enable texture wrap addressing mode.
    \param notransform [in]  Disable pt1, pt2 and pt3 points transformation.
  */
  void draw_textured_triangle(int x1, int y1, const OdGePoint3d &pt1,
                              int x2, int y2, const OdGePoint3d &pt2,
                              int x3, int y3, const OdGePoint3d &pt3,
                              bool wrap, bool notransform, OdUInt8 alpha = 255);

  /** \details
    Draws the specified triangle with this Vectorizer device into NRC clipping buffer.
    \param y1 [in]  First y-coordinate.
    \param y2 [in]  Second y-coordinate.
    \param y3 [in]  Third y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param x2 [in]  Third x-coordinate.
  */
  void draw_clip_triangle(int x1, int y1, int x2, int y2, int x3, int y3);

  /** \details
    Draws the filled circle on specified position with specified radius.
    \param x [in]  X-coordinate.
    \param y [in]  Y-coordinate.
    \param radius [in]  Circle radius.
    \param color [in]  Circle color.
  */
  void draw_filled_circle(int x, int y, double radius, ODCOLORREF color);

  /** \details
    Draws the specified line with specified width with this Vectorizer device.
    \param y1 [in]  First y-coordinate.
    \param y2 [in]  Second y-coordinate.
    \param x1 [in]  First x-coordinate.
    \param x2 [in]  Second x-coordinate.
    \param lw [in]  Lineweight in pixels.
    \param color [in]  Color.
  */
  void draw_lwLine(int x1, int y1, int x2, int y2, int lw, ODCOLORREF color);

  // OdGiRasterImage interface

  virtual OdUInt32 pixelWidth() const;
  virtual OdUInt32 pixelHeight() const;
  virtual OdUInt32 colorDepth() const;
  virtual OdUInt32 numColors() const;
  virtual ODCOLORREF color(OdUInt32 colorIndex) const;
  virtual OdUInt32 paletteDataSize() const;
  virtual void paletteData(OdUInt8* pBytes) const;
  virtual void scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines = 1) const;
  virtual const OdUInt8* scanLines() const;
  virtual PixelFormatInfo pixelFormat() const;
  virtual OdUInt32 scanLinesAlignment() const;

  /** \details
    Returns the EnableSoftwareHLR flag for this Vectorizer device.
  */
  bool get_EnableSoftwareHLR() const
  {
    return m_bEnableHLR;
  }

  /** \details
    Controls the EnableSoftwareHLR flag for this Vectorizer device.
    \param enableSoftwareHLR [in]  Enables Software HLR.
  */
  void put_EnableSoftwareHLR(bool enableSoftwareHLR)
  {
    m_bEnableHLR = enableSoftwareHLR;
  }

  /** \details
    Returns the PrecisionMode flag for this Vectorizer device.
  */
  bool get_PrecisionMode() const
  {
    return m_bPrecisionMode;
  }

  /** \details
    Controls the PrecisionMode flag for this Vectorizer device.
    \param precisionMode [in]  Enables Precision Mode.
  */
  void put_PrecisionMode(bool precisionMode)
  {
    m_bPrecisionMode = precisionMode;
  }

  /** \details
    Returns color converter function.
  */
  OdColorConverterCallback *getColorConverter();

  // Raster image rendering support

  /** \details
    Checks raster image compatibility.
    \param pImage [in]  Pointer to raster image for check.
  */
  bool isRasterImageProcessible(const OdGiRasterImage *pImage) const;
  /** \details
    Prepare raster image for processing.
    \param pImage [in]  Pointer to raster image.
    \param bTransparent [in]  Transparency is enabled.
    \param rasterOrigin [in]  Bottom-left raster image corner coordinate.
    \param rasterU [in]  X-axis of raster image.
    \param rasterV [in]  Y-axis of raster image.
  */
  void setRasterImage(const OdGiRasterImage *pImage, bool bTransparent,
                      const OdGePoint3d &rasterOrigin, const OdGeVector3d &rasterU, const OdGeVector3d &rasterV);
  
  /** \details
    Prepare raster image for processing.
    \param pImage [in]  Pointer to raster image.
    \param bTransparent [in]  Transparency is enabled.
  */
  void setRasterImage(const OdGiRasterImage *pImage, bool bTransparent = false);
  /** \details
    Finalize raster image processing.
  */
  void clearRasterImage();
  /** \details
    Returns true if raster image is available.
  */
  bool hasRasterImage() const { return m_pRasterScanLines != NULL; }

  /** \details
    Sets non-rectangular viewport clipping.
    \param offsetX [in]  Offset of left viewport clipping edge.
    \param offsetY [in]  Offset of bottom viewport clipping edge.
    \param nrcWidth [in]  Width of viewport clipping area.
    \param nrcHeight [in]  Height of viewport clipping area.
    \param pBuffer [in]  Pointer to buffer, containing rasterized viewport clip.
  */
  void setNRCClipping(long offsetX, long offsetY, OdUInt32 nrcWidth, OdUInt32 nrcHeight, OdUInt8 *pBuffer);
  /** \details
    Removes non-rectangular viewport clipping.
  */
  void resetNRCClipping();

  // Snapshot support
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView);
};

/** \details
  This class implements device-independent bitmap vectorizer views.
  \sa
  Source code provided.
  <group ExRender_Classes> 
*/
class OdGsBitmapVectorizeView : public OdGsBaseVectorizeViewDef
                              , public OdGiGeometrySimplifier
                              , protected OdGsPropertiesDirectRenderOutput
{
  // traits
  ODCOLORREF          m_curColor;
  int                 m_curLineweight;
  OdPs::LineEndStyle  m_curLesStyle;
  OdPs::LineJoinStyle m_curLjsStyle;
  bool                m_bCurLwdStyleDef;
  OdColorConverterCallback*      m_pColorConverter;
  // conveyor
  OdGiPerspectivePreprocessorPtr m_pPerspPrep;
  OdGiOrthoClipperPtr            m_pZClip;
  OdGiXformPtr                   m_pPerspXform;
  OdGiHLRemoverPtr               m_pHLR;
  // viewport clipping
  OdUInt8Array m_pNRCScanlines;
  OdUInt32     m_nNRCWidth, m_nNRCHeight;
  long         m_nNRCOffsetX, m_nNRCOffsetY;
  bool         m_bClipStage;
  // extended lineweight style
  bool                m_bLwdPoints;
  OdPs::LineEndStyle  m_defLesStyle;
  OdPs::LineJoinStyle m_defLjsStyle;
  bool                m_bDefLwdStyleDef;
  // lineas anti-aliasing
  bool                m_bLinesAntiAlias;
  // render transparency
  bool                m_bDrawTransparency;
  // override lineweight
  bool                m_bLwdOverride;
public:
  OdGsBitmapVectorizeView();
  virtual ~OdGsBitmapVectorizeView();

  void setMode(OdGsView::RenderMode mode);

  void beginViewVectorization();
  void loadViewport();
  void endViewVectorization();

  /** \details
    Returns the EnableSoftwareHLR flag for this Vectorizer device.
  */
  bool enabledSoftwareHLR() { return (static_cast<OdGsBitmapVectorizeDevice*>(device()))->get_EnableSoftwareHLR(); }

  // graphics data
  // required by simplifier

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList);
  void triangleOut( const OdInt32* vertices, const OdGeVector3d* pNormal );
  
  void initTexture(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage,
    bool transparency,
    double brightness,
    double contrast,
    double fade);
  void uninitTexture();

  void generateMeshFaces(OdInt32 rows, OdInt32 columns, const OdGiFaceData* pFaceData);

  void polypointProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                     const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency = 0,
                     const OdGeVector3d* pNormals = 0, const OdGeVector3d* pExtrusions = 0,
                     const OdGsMarker* pSubEntMarkers = 0, OdInt32 nPointSize = 0);

  void onTraitsModified();

  void updateLineweightOverride(const OdGiLineweightOverride &lwdOverride);

  TD_USING(OdGiGeometrySimplifier::polylineOut);

  /** \details
    Returns the OdGsBitmapVectorizeDevice object associated with this Vectorizer View object.
  */
  OdGsBitmapVectorizeDevice* device()
  {
    return static_cast<OdGsBitmapVectorizeDevice*>(OdGsBaseVectorizeView::device());
  }

  // Snapshot support

  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);
protected:
  void getViewportClipRect(OdGsDCRect &pRect);
  // override viewport frame rendering
  void drawViewportFrame();
  // Background rendering support
  void display(bool update);
  void directRenderOutputPoint(const OdGePoint3d &point, const OdCmEntityColor &color);
  void directRenderOutputLineFlat(const OdGePoint3d &pt1, const OdGePoint3d &pt2,
                                  const OdCmEntityColor &color);
  void directRenderOutputTriangleFlat(const OdGePoint3d &pt1, const OdGePoint3d &pt2, const OdGePoint3d &pt3,
                                      const OdCmEntityColor &color);
  void directRenderOutputTriangleGouraud(const OdGePoint3d &pt1, const OdGePoint3d &pt2, const OdGePoint3d &pt3,
                                         const OdCmEntityColor &clr1, const OdCmEntityColor &clr2, const OdCmEntityColor &clr3);
  void directRenderOutputPolygoneFlat(OdUInt32 nPts, const OdGePoint3d *pPts,
                                      const OdCmEntityColor &color);
  void directRenderOutputPolygoneGouraud(OdUInt32 nPts, const OdGePoint3d *pPts,
                                         const OdCmEntityColor *pClrs);
  void directRenderOutputImage(const OdGePoint3d *pRect, const OdGiRasterImage *pImage,
                               const OdGsPropertiesDirectRenderOutput::DirectRenderImageParams &driParams);
  OdUInt32 directRenderOutputFlags() const;
  //
};

#include "TD_PackPop.h"

#endif // __OD_GS_BITMAP_VECTORIZER__
