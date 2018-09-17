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

#ifndef _DWFCOLORADJUSTER_INCLUDED_
#define _DWFCOLORADJUSTER_INCLUDED_

#include "OdaCommon.h"
#include "CmColorBase.h"

class OdDwfColorAdjuster
{
protected:
  bool m_bMonochrome;
  double m_fContrast;
  double m_fFade;
  ODCOLORREF m_crBackColor;
  
  //bool m_bOptimize;
public:
  OdDwfColorAdjuster()
    : m_bMonochrome(false), m_fContrast(100.0), m_fFade(0.0), m_crBackColor(ODRGB(0, 0, 0))//, m_bOptimize(false)
  {
    //m_bOptimize = isNeedToCompute();
  }

  OdDwfColorAdjuster(
    bool bMonochrome,
    double fContrast,
    double fFade,
    ODCOLORREF crBackColor = ODRGB(0, 0, 0))
    : m_bMonochrome(bMonochrome), m_fContrast(fContrast), m_fFade(fFade), m_crBackColor(crBackColor)//, m_bOptimize(false)
  {
    //m_bOptimize = isNeedToCompute();
  }

  inline void setMonochrome(bool flag)
  {
    m_bMonochrome = flag;
    //m_bOptimize = isNeedToCompute();
  }
  inline bool getMonochrome() const
  {
    return m_bMonochrome;
  }
  inline void setContrast(double val)
  {
    m_fContrast = val;
    //m_bOptimize = isNeedToCompute();
  }
  inline double getContrast() const
  {
    return m_fContrast;
  }
  inline void setFade(double val)
  {
    m_fFade = val;
    //m_bOptimize = isNeedToCompute();
  }
  inline double getFade() const
  {
    return m_fFade;
  }

  inline bool isNeedColorRecompution() const
  {
    return isNeedToCompute();//m_bOptimize;
  }
  inline void setBackColor(ODCOLORREF crBackColor)
  {
    m_crBackColor = crBackColor;
  }
  inline ODCOLORREF getBackColor() const
  {
    return m_crBackColor;
  }

  void computeShadedColorRGB(OdUInt8 &nR, OdUInt8 &nG, OdUInt8 &nB) const;
  ODCOLORREF computeShadedColor(ODCOLORREF color) const;
  OdCmEntityColor computeShadedColorCm(ODCOLORREF color) const;
protected:
  bool isNeedToCompute() const;
}; // end of OdDwfColorAdjuster

#endif // _DWFCOLORADJUSTER_INCLUDED_
