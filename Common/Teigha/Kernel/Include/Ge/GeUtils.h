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




#ifndef OD_GEGBLGEUTILS_H
#define OD_GEGBLGEUTILS_H /*!DOM*/

#include "TD_PackPush.h"

inline void moveIntoValidInter(const double& startAngleInV, const double& endAngleInV, double& v, const double period = Oda2PI)
{
  if (v < startAngleInV)
  {
    v = startAngleInV - fmod(startAngleInV - v, period);
    if (v < startAngleInV)
      v += period;
  }
  if (v > endAngleInV)
  {
    v = endAngleInV + fmod(v - endAngleInV, period);
    if (v > endAngleInV)
      v -= period;
  }
  if (v < startAngleInV)
  {
    double d1 = startAngleInV - v;
    double d2 = fabs(endAngleInV - v - period);
    if (d2 < d1)
      v += period;
  }
}

#include "TD_PackPop.h"

#endif  // OD_GEGBLGEUTILS_H

