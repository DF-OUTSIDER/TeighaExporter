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

#include "OdColorAdjuster.h"

#ifndef ODCLAMPTORANGE
#define ODCLAMPTORANGE(val, minlim, maxlim) \
  if (val < minlim) val = minlim; \
  else if (val > maxlim) val = maxlim
#endif

bool OdColorAdjuster::isNeedToCompute() const
{
  return !(!m_bMonochrome && m_fContrast == 100.0 && m_fFade == 0.0);
}

void OdColorAdjuster::computeShadedColorRGB(OdUInt8 &nR, OdUInt8 &nG, OdUInt8 &nB) const
{
  double r = double(nR) / 255, g = double(nG) / 255, b = double(nB) / 255;
  double br = double(ODGETRED(m_crBackColor)) / 255, bg = double(ODGETGREEN(m_crBackColor)) / 255, bb = double(ODGETBLUE(m_crBackColor)) / 255;
  double tmp;
  if (m_fFade != 0.0)
  {
    tmp = 1.0 - (m_fFade / 100);
    r *= tmp; g *= tmp; b *= tmp;
    r += br * (1.0 - tmp); g += bg * (1.0 - tmp); b += bb * (1.0 - tmp);
    ODCLAMPTORANGE(r, 0.0, 1.0);
    ODCLAMPTORANGE(g, 0.0, 1.0);
    ODCLAMPTORANGE(b, 0.0, 1.0);
  }
  // compute faded contrast color
  if (m_fContrast != 100.0)
  {
    // Originally it uses HSL, but we can compute it more fast without HSL using
    r += (1.0 - m_fContrast / 100) * (m_fFade / 100 * 0.75 + 0.2);
    g += (1.0 - m_fContrast / 100) * (m_fFade / 100 * 0.75 + 0.2);
    b += (1.0 - m_fContrast / 100) * (m_fFade / 100 * 0.75 + 0.2);
    ODCLAMPTORANGE(r, 0.0, 1.0);
    ODCLAMPTORANGE(g, 0.0, 1.0);
    ODCLAMPTORANGE(b, 0.0, 1.0);
  }
  if (m_bMonochrome)
  {
    r = g = b = r * 0.299 + g * 0.587 + b * 0.114;
  }

  nR = OdUInt8(r * 255);
  nG = OdUInt8(g * 255);
  nB = OdUInt8(b * 255);
}

ODCOLORREF OdColorAdjuster::computeShadedColor(ODCOLORREF color) const
{
  OdUInt8 nR = ODGETRED(color);
  OdUInt8 nG = ODGETGREEN(color); 
  OdUInt8 nB = ODGETBLUE(color);

  computeShadedColorRGB(nR, nG, nB);

  return ODRGBA(nR, nG, nB, ODGETALPHA(color));
}

OdCmEntityColor OdColorAdjuster::computeShadedColorCm(ODCOLORREF color) const
{
  OdUInt8 nR = ODGETRED(color);
  OdUInt8 nG = ODGETGREEN(color); 
  OdUInt8 nB = ODGETBLUE(color);

  computeShadedColorRGB(nR, nG, nB);

  return OdCmEntityColor(nR, nG, nB);
}

//
