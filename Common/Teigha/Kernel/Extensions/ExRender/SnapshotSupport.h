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

#ifndef __SNAPSHOT_SUPPORT__
#define __SNAPSHOT_SUPPORT__

#include "TD_PackPush.h"

#include "Gi/GiRasterImage.h"
#include "RxObjectImpl.h"
#include "Gs/GsBaseVectorizeDevice.h"

/** \details
  Library: Source code provided.
  <group ExRender_Classes> 
*/
struct OdGiSnapshotImageCallback
{
  virtual OdGiRasterImagePtr snapshotImageParams(OdGsBaseVectorizeDevice &runDevice) const = 0;
  virtual OdGiRasterImagePtr snapshotRegion(OdGsBaseVectorizeDevice &runDevice, const OdGsDCRect &area, bool bCrop) const = 0;
};

/** \details
  Library: Source code provided.
  <group ExRender_Classes> 
*/
class OdGiSnapshotImageImpl : public OdGiRasterImage
{
  protected:
    OdUInt8Array m_bits;
    OdGiRasterImagePtr m_pOrig;
    OdUInt32 m_nWidth;
    OdUInt32 m_nHeight;
    OdUInt32 m_nBPP;
    PixelFormatInfo m_pf;
    OdArray<ODCOLORREF> m_pal;
    OdUInt32 m_nAlignment;
  public:
    OdGiSnapshotImageImpl()
      : m_nWidth(0)
      , m_nHeight(0)
      , m_nBPP(24)
      , m_nAlignment(4)
    {
    }

    void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines = 1) const
    {
      const OdUInt8 *pBits = m_bits.getPtr();
      OdUInt32 scSize = scanLineSize();
      for (OdUInt32 i = 0; i < numLines; i++, scnLines += scSize)
      {
        ::memcpy(scnLines, pBits + (firstScanline + i) * scSize, scSize);
      }
    }
    const OdUInt8* scanLines() const { return m_bits.getPtr(); }
    OdUInt8* scanLines() { return m_bits.asArrayPtr(); }

    OdUInt32 pixelWidth() const { return m_nWidth; }
    OdUInt32 pixelHeight() const { return m_nHeight; }
    Units defaultResolution(double& xPelsPerUnit, double& yPelsPerUnit) const
    {
      if (m_pOrig.isNull())
        return OdGiRasterImage::defaultResolution(xPelsPerUnit, yPelsPerUnit);
      return m_pOrig->defaultResolution(xPelsPerUnit, yPelsPerUnit);
    }
    OdUInt32 colorDepth() const { return m_nBPP; }
    OdUInt32 numColors() const { return m_pal.size(); }
    int transparentColor() const {
      if (m_pOrig.isNull())
        return OdGiRasterImage::transparentColor();
      return m_pOrig->transparentColor();
    }
    ODCOLORREF color(OdUInt32 colorIndex) const { return m_pal[colorIndex]; }
    OdUInt32 paletteDataSize() const { return m_pal.size() * sizeof(ODCOLORREF); }
    void paletteData(OdUInt8* bytes) const { ::memcpy(bytes, m_pal.getPtr(), paletteDataSize()); }
    PixelFormatInfo pixelFormat() const { return m_pf; }
    OdUInt32 scanLinesAlignment() const { return m_nAlignment; }

    ImageSource imageSource() const { return kFromRender; }
    TransparencyMode transparencyMode() const { return kTransparencyOff; }

    PixelFormatInfo &pixelFormat() { return m_pf; }

    void setAlignment(OdUInt32 nAlignment) { m_nAlignment = nAlignment; }

    void setupForOriginal(OdGiRasterImage *pImage)
    {
      m_nBPP = pImage->colorDepth();
      m_pf = pImage->pixelFormat();
      if (m_nBPP <= 8)
      {
        m_pal.resize(pImage->numColors());
        pImage->paletteData((OdUInt8*)m_pal.asArrayPtr());
      }
      else
        m_pal.clear();
      m_nAlignment = pImage->scanLinesAlignment();
    }

    void setupSurface(OdUInt32 nWidth, OdUInt32 nHeight, OdUInt32 nBPP)
    {
      m_nWidth = nWidth; m_nHeight = nHeight; m_nBPP = nBPP;
      if (nWidth && nHeight)
      {
        m_bits.resize(scanLineSize() * m_nHeight);
        ::memset(m_bits.asArrayPtr(), 0, m_bits.size());
      }
    }

    static OdGsDCRect normalizeRect(const OdGsDCRect &region)
    {
      return OdGsDCRect(odmin(region.m_min.x, region.m_max.x), odmax(region.m_min.x, region.m_max.x),
                        odmin(region.m_min.y, region.m_max.y), odmax(region.m_min.y, region.m_max.y));
    }

    static bool computeMergeArea(OdGsBaseVectorizeDevice &runDevice, OdGsView *pView, const OdGsDCRect &region, OdGsDCRect &mergeArea, OdGiRasterImagePtr & /*pNrcMask*/)
    {
      OdGsDCRect normRegion(normalizeRect(region));
      normRegion &= normalizeRect(runDevice.outputRect());
      if (!normRegion.is_null())
      {
        if (pView)
        {
          OdGsDCRect vptRect;
          OdGePoint2d minVpt, maxVpt;
          pView->getViewport(minVpt, maxVpt);
          pView->getViewport(vptRect);
          vptRect.m_min.y = runDevice.height() - vptRect.m_min.y;
          vptRect.m_max.y = runDevice.height() - vptRect.m_max.y;
          OdGsDCRect mgdRect((long)(minVpt.x * runDevice.width()) , (long)(maxVpt.x * runDevice.width()),
                             (long)(minVpt.y * runDevice.height()), (long)(maxVpt.y * runDevice.height()));
          mgdRect &= normalizeRect(vptRect);
          ODA_ASSERT(!mgdRect.is_null());
          normRegion &= normalizeRect(mgdRect);
          if (normRegion.is_null())
            return false;
          // @@@TODO: compute mask for nrc clip region (1bpp - 0 - fade, 1 - keep)
        }
        mergeArea = normRegion;
        return (mergeArea.m_min.x < mergeArea.m_max.x) && (mergeArea.m_min.y < mergeArea.m_max.y);
      }
      return false;
    }

    void merge(const OdGsDCPoint &drawTo, OdGiRasterImage *pImage)
    {
      ODA_ASSERT(colorDepth() == pImage->colorDepth());
      OdUInt32 nScans = pImage->pixelHeight(), nPix = pImage->pixelWidth();
      OdUInt8 *pScanIn = new OdUInt8[pImage->scanLineSize()];
      OdUInt32 nScanLenOut = scanLineSize();
      OdUInt8 *pScanOut = m_bits.asArrayPtr() + nScanLenOut * drawTo.y;
      for (OdUInt32 nScan = 0; nScan < nScans; nScan++, pScanOut += nScanLenOut)
      {
        pImage->scanLines(pScanIn, nScan);
        switch (colorDepth())
        {
          case 1:
            {
              for (OdUInt32 nPx = 0; nPx < nPix; nPx++)
              {
                OdUInt32 nPxO = nPx + drawTo.x;
                OdUInt8 pId = (pScanIn[nPx / 8] >> (0x07 - (nPx % 8))) & 0x01;
                pScanOut[nPxO / 8] = ((pScanOut[nPxO / 8] & ~(0x01 << (0x07 - (nPxO % 8)))) | (pId << (0x07 - (nPxO % 8))));
              }
            }
          break;
          case 4:
            {
              for (OdUInt32 nPx = 0; nPx < nPix; nPx++)
              {
                OdUInt32 nPxO = nPx + drawTo.x;
                OdUInt8 pId = ((nPx % 2) & 0x01) ? (pScanIn[nPx / 2] & 0x0F) : ((pScanIn[nPx / 2] & 0xF0) >> 4);
                if ((nPxO % 2) & 0x01)
                  pScanOut[nPxO / 2] = (pScanOut[nPxO / 2] & 0x0F) | pId;
                else
                  pScanOut[nPxO / 2] = (pScanOut[nPxO / 2] & 0xF0) | (pId << 4);
              }
            }
          break;
          case 8:
            ::memcpy(pScanOut + drawTo.x, pScanIn, nPix);
          break;
          case 16:
            ::memcpy(pScanOut + drawTo.x * 2, pScanIn, nPix * 2);
          break;
          case 24:
            ::memcpy(pScanOut + (drawTo.x << 1) + drawTo.x, pScanIn, (nPix << 1) + nPix);
          break;
          case 32:
            ::memcpy(pScanOut + (drawTo.x << 2), pScanIn, nPix << 2);
          break;
        }
      }
      delete []pScanIn;
    }

    static void getSnapshot(OdGsBaseVectorizeDevice &runDevice, OdGiSnapshotImageCallback *pCallback,
                            OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView)
    {
      OdSmartPtr<OdGiSnapshotImageImpl> pOutput = OdRxObjectImpl<OdGiSnapshotImageImpl>::createObject();
      { OdGiRasterImagePtr pDesc = pCallback->snapshotImageParams(runDevice);
        if (!pDesc.isNull())
          pOutput->setupForOriginal(pDesc);
        else
        {
          pOutput->pixelFormat().setBGR();
          pOutput->setAlignment(4);
          pOutput->setupSurface(0, 0, 24);
        }
      }
      pOutput->setupSurface(Od_abs(region.m_max.x - region.m_min.x), Od_abs(region.m_min.y - region.m_max.y), pOutput->colorDepth());
      if (!pOutput->pixelWidth() || !pOutput->pixelHeight())
        throw OdError(eInvalidInput);
      OdGsDCRect mergeArea; OdGiRasterImagePtr pNrcMask;
      if (OdGiSnapshotImageImpl::computeMergeArea(runDevice, pView, region, mergeArea, pNrcMask))
      {
        OdGsDCPoint drawTo(mergeArea.m_min.x - odmin(region.m_min.x, region.m_max.x),
                           mergeArea.m_min.y - odmin(region.m_min.y, region.m_max.y));
        const bool bCrop = mergeArea != OdGiSnapshotImageImpl::normalizeRect(runDevice.outputRect());
        OdGiRasterImagePtr pCrop = pCallback->snapshotRegion(runDevice, mergeArea, bCrop);
        if (!pCrop.isNull() && pCrop->pixelWidth() && pCrop->pixelHeight())
          pOutput->merge(drawTo, pCrop);
        // @@@TODO: apply nrc mask to shade area outside viewport
      }
      pImage = pOutput;
    }
};

#include "TD_PackPop.h"

#endif // __SNAPSHOT_SUPPORT__
