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

#ifndef ODLUTHELPERS_INCLUDED
#define ODLUTHELPERS_INCLUDED

#include "TD_PackPush.h"

#include "Gi/GiRasterImage.h"

static OdUInt32 computeLutScanlineLength(OdUInt32 uWidth, OdUInt32 nAlign = 4)
{
  if (uWidth % nAlign)
    uWidth += (nAlign - (uWidth % nAlign));
  return uWidth;
}

static void makeLutTexturePixels(OdUInt32 nInWidth, OdUInt32 nInHeight, OdUInt32 nInColorDepth, OdUInt32 nInScanLength, const OdUInt8 *pInBuf,
                                 OdUInt32 nOutWidth, OdUInt32 nOutHeight, OdUInt32 nOutScanLength, OdUInt8 *pOutBuf)
{
  const bool bRescale = (nInWidth != nOutWidth) || (nInHeight != nOutHeight);
  switch (nInColorDepth)
  {
    case 1:
      if (!bRescale)
      { // Simply copy bits
        const OdUInt8 *pScanIn = pInBuf;
        OdUInt8 *pScanOut = pOutBuf;
        for (OdUInt32 iy = 0; iy < nInHeight; iy++)
        {
          for (OdUInt32 ix = 0; ix < nInWidth; ix++)
          {
            pScanOut[ix] = (pScanIn[ix / 8] >> (0x07 - (ix % 8))) & 0x01;
          }
          pScanIn += nInScanLength; pScanOut += nOutScanLength;
        }
      }
      else
      { // Rescale and copy bits
        double aspectX = double(nInWidth - 1) / (nOutWidth - 1),
               aspectY = double(nInHeight - 1) / (nOutHeight - 1);
        const OdUInt8 *pScanIn;
        OdUInt8 *pScanOut = pOutBuf;
        for (OdUInt32 iy = 0; iy < nOutHeight; iy++)
        {
          pScanIn = pInBuf + (OdUInt32(aspectY * iy) * nInScanLength);
          for (OdUInt32 ix = 0; ix < nOutWidth; ix++)
          {
            OdUInt32 ixx = OdUInt32(aspectX * ix);
            pScanOut[ix] = (pScanIn[ixx / 8] >> (0x07 - (ixx % 8))) & 0x01;
          }
          pScanOut += nOutScanLength;
        }
      }
    break;
    case 4:
      if (!bRescale)
      { // Simply copy bits
        const OdUInt8 *pScanIn = pInBuf;
        OdUInt8 *pScanOut = pOutBuf;
        for (OdUInt32 iy = 0; iy < nInHeight; iy++)
        {
          for (OdUInt32 ix = 0; ix < nInWidth; ix++)
          {
            pScanOut[ix] = ((ix % 2) & 0x01) ? (pScanIn[ix / 2] & 0x0F) : ((pScanIn[ix / 2] & 0xF0) >> 4);
          }
          pScanIn += nInScanLength; pScanOut += nOutScanLength;
        }
      }
      else
      { // Rescale and copy bits
        double aspectX = double(nInWidth - 1) / (nOutWidth - 1),
               aspectY = double(nInHeight - 1) / (nOutHeight - 1);
        const OdUInt8 *pScanIn;
        OdUInt8 *pScanOut = pOutBuf;
        for (OdUInt32 iy = 0; iy < nOutHeight; iy++)
        {
          pScanIn = pInBuf + (OdUInt32(aspectY * iy) * nInScanLength);
          for (OdUInt32 ix = 0; ix < nOutWidth; ix++)
          {
            OdUInt32 ixx = OdUInt32(aspectX * ix);
            pScanOut[ix] = ((ixx % 2) & 0x01) ? (pScanIn[ixx / 2] & 0x0F) : ((pScanIn[ixx / 2] & 0xF0) >> 4);
          }
          pScanOut += nOutScanLength;
        }
      }
    break;
    case 8:
      if (!bRescale)
      { // Simply copy bits
        const OdUInt8 *pScanIn = pInBuf;
        OdUInt8 *pScanOut = pOutBuf;
        for (OdUInt32 iy = 0; iy < nInHeight; iy++)
        {
          ::memcpy(pScanOut, pScanIn, nInWidth);
          pScanIn += nInScanLength; pScanOut += nOutScanLength;
        }
      }
      else
      { // Rescale and copy bits
        double aspectX = double(nInWidth - 1) / (nOutWidth - 1),
               aspectY = double(nInHeight - 1) / (nOutHeight - 1);
        const OdUInt8 *pScanIn;
        OdUInt8 *pScanOut = pOutBuf;
        for (OdUInt32 iy = 0; iy < nOutHeight; iy++)
        {
          pScanIn = pInBuf + (OdUInt32(aspectY * iy) * nInScanLength);
          for (OdUInt32 ix = 0; ix < nOutWidth; ix++)
          {
            OdUInt32 ixx = OdUInt32(aspectX * ix);
            pScanOut[ix] = pScanIn[ixx];
          }
          pScanOut += nOutScanLength;
        }
      }
    break;
  }
}

static OdUInt32 computeLutPaletteLength(const OdGiRasterImage *pImg)
{
  return pImg->numColors() * 4;
}

static void makeLutPalettePixels(const OdGiRasterImage *pImg, OdUInt8 *pOutBuf, bool bBGRA = false)
{
  const OdUInt32 nColors = pImg->numColors();
  OdUInt8 *pOutColor = pOutBuf;
  if (bBGRA)
  {
    for (OdUInt32 nColor = 0; nColor < nColors; nColor++, pOutColor += 4)
    {
      ODCOLORREF clr = pImg->color(nColor);
      pOutColor[0] = ODGETBLUE(clr);
      pOutColor[1] = ODGETGREEN(clr);
      pOutColor[2] = ODGETRED(clr);
      pOutColor[3] = 255;
    }
  }
  else
  {
    for (OdUInt32 nColor = 0; nColor < nColors; nColor++, pOutColor += 4)
    {
      ODCOLORREF clr = pImg->color(nColor);
      pOutColor[0] = ODGETRED(clr);
      pOutColor[1] = ODGETGREEN(clr);
      pOutColor[2] = ODGETBLUE(clr);
      pOutColor[3] = 255;
    }
  }
  if (pImg->transparentColor() >= 0)
  {
    pOutColor = pOutBuf + 4 * pImg->transparentColor();
    pOutColor[3] = 0;
  }
}

#include "TD_PackPop.h"

#endif // ODLUTHELPERS_INCLUDED
