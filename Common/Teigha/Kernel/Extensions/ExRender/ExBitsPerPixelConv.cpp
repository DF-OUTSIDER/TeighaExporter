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

#include "OdVector.h"
#include "Gi/GiRasterWrappers.h"

static inline RGBQUAD makeQuad(OdUInt8 r, OdUInt8 g, OdUInt8 b)
{
  RGBQUAD rq = { b, g, r, 0 };
  return rq;
}

static const RGBQUAD g_classic4BitPalette[16] = {
  { 0,   0,   0,   0 }, { 132, 0, 0,   0 }, { 0, 132, 0,   0 }, { 132, 132, 0,   0 },
  { 0,   0,   132, 0 }, { 132, 0, 132, 0 }, { 0, 132, 132, 0 }, { 132, 132, 132, 0 },
  { 198, 198, 198, 0 }, { 255, 0, 0,   0 }, { 0, 255, 0,   0 }, { 255, 255, 0,   0 },
  { 0,   0,   255, 0 }, { 255, 0, 255, 0 }, { 0, 255, 255, 0 }, { 255, 255, 255, 0 }
};

class OdBitPerPixelConverter
{
  protected:
    OdGiRasterImage   *m_pOriginalImage;
    OdGiRasterImagePtr m_pConvertedImage;
    OdUInt64           m_nRequestedImageBPP;
    bool               m_bEnabled;
  public:
    OdBitPerPixelConverter(OdGiRasterImage *pOriginalImage = NULL)
      : m_pOriginalImage(NULL)
      , m_nRequestedImageBPP(24)
      , m_bEnabled(true)
    {
      if (pOriginalImage)
        init(pOriginalImage);
    }

    void init(OdGiRasterImage *pOriginalImage)
    {
      if (!pOriginalImage)
        throw OdError(eNullPtr);
      m_pOriginalImage = pOriginalImage;
      m_nRequestedImageBPP = pOriginalImage->colorDepth();
    }

    void setRequestedBPP(OdUInt32 bpp)
    {
      m_nRequestedImageBPP = bpp;
    }
    OdUInt32 requestedBPP() const
    {
      if (m_bEnabled)
        return (OdUInt32)m_nRequestedImageBPP;
      if (!m_pOriginalImage)
        throw OdError(eNullPtr);
      return m_pOriginalImage->colorDepth();
    }

    void enable(bool bEnable) { m_bEnabled = bEnable; }
    bool isEnabled() const { return m_bEnabled; }

    OdGiRasterImage *getRasterImage(ODCOLORREF backgroundColor = 0)
    {
      if (!m_bEnabled)
        return m_pOriginalImage;
      if (!m_pOriginalImage)
        throw OdError(eNullPtr);
      if (m_nRequestedImageBPP != m_pOriginalImage->colorDepth())
      {
        OdSmartPtr<OdGiRasterImageDesc> pDesc = OdGiRasterImageDesc::createObject(m_pOriginalImage);
        pDesc->setColorDepth((OdUInt32)m_nRequestedImageBPP);
        if (pDesc->pixelFormat().isRGB()) pDesc->pixelFormat().setRGBA();
        else                              pDesc->pixelFormat().setBGRA();
        OdVector<RGBQUAD, OdMemoryAllocator<RGBQUAD> > palette;
        if (m_nRequestedImageBPP <= 8)
        { // Generate palette
          switch (m_nRequestedImageBPP)
          {
            case 1:
              palette.reserve(2);
              palette.push_back(makeQuad(0  , 0  , 0));
              palette.push_back(makeQuad(255, 255, 255));
            break;
            case 4:
              palette.reserve(16);
              palette.insert(palette.end(), g_classic4BitPalette, g_classic4BitPalette + 16);
            break;
            case 8:
              palette.reserve(256);
              while (palette.size() < 256)
                palette.push_back(makeQuad(OdCmEntityColor::mLUT[palette.size()][0], OdCmEntityColor::mLUT[palette.size()][1], OdCmEntityColor::mLUT[palette.size()][2]));
            break;
          }
          pDesc->setPalette(sizeof(RGBQUAD) * palette.size(), (OdUInt8*)palette.asArrayPtr());
        }
        m_pConvertedImage = m_pOriginalImage->convert(false, 50., 50., 0.0, backgroundColor, false, false, false, pDesc);
      }
      else
        m_pConvertedImage.release();
      if (!m_pConvertedImage.isNull())
        return m_pConvertedImage;
      return m_pOriginalImage;
    }

    class TurnToOriginal
    {
      protected:
        OdBitPerPixelConverter &m_bpp;
      public:
        TurnToOriginal(OdBitPerPixelConverter &bpp)
          : m_bpp(bpp) { m_bpp.enable(false); }
        ~TurnToOriginal() { m_bpp.enable(true); }
    };
};

//
