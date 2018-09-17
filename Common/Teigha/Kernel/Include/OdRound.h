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




#ifndef _ODROUND_INCLUDED_
#define _ODROUND_INCLUDED_

#include <math.h>
#include <limits.h>

inline double OdRound(double a)
{
  double aFloor = ::floor(a);
  if(a-aFloor >= 0.5)
    return aFloor+1.0;
  return aFloor;
}


inline long OdRoundToLong(double a)
{
  if (a >= 0.)
  {
    a += .5;
    if (a > double(LONG_MAX))
      throw OdError(eArithmeticOverflow);
    return long(a);
  }
  else
  {
    a -= .5;
    if (a < double(LONG_MIN))
      throw OdError(eArithmeticOverflow);
    return long(a);
  }
}

inline long OdTruncateToLong(double a)
{
  if (a >= 0.)
  {
    a += .5;
    if (a > double(LONG_MAX))
      return LONG_MAX;
    else
      return long(a);
  }
  else
  {
    a -= .5;
    if (a < double(LONG_MIN))
      return LONG_MIN;
    else
      return long(a);
  }
}

#endif //#ifndef _ODROUND_INCLUDED_
