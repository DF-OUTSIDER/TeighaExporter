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

#ifndef __OD_LINE_WT_INDEX__
#define __OD_LINE_WT_INDEX__

inline int lineWeightIndex(OdDb::LineWeight lw)
{
  switch(lw)
  {
  case OdDb::kLnWt000:
    return 0;
  case OdDb::kLnWt005:
    return 1;
  case OdDb::kLnWt009:
    return 2;
  case OdDb::kLnWt013:
    return 3;
  case OdDb::kLnWt015:
    return 4;
  case OdDb::kLnWt018:
    return 5;
  case OdDb::kLnWt020:
    return 6;
  case OdDb::kLnWt025:
    return 7;
  case OdDb::kLnWt030:
    return 8;
  case OdDb::kLnWt035:
    return 9;
  case OdDb::kLnWt040:
    return 10;
  case OdDb::kLnWt050:
    return 11;
  case OdDb::kLnWt053:
    return 12;
  case OdDb::kLnWt060:
    return 13;
  case OdDb::kLnWt070:
    return 14;
  case OdDb::kLnWt080:
    return 15;
  case OdDb::kLnWt090:
    return 16;
  case OdDb::kLnWt100:
    return 17;
  case OdDb::kLnWt106:
    return 18;
  case OdDb::kLnWt120:
    return 19;
  case OdDb::kLnWt140:
    return 20;
  case OdDb::kLnWt158:
    return 21;
  case OdDb::kLnWt200:
    return 22;
  case OdDb::kLnWt211:
    return 23;
  default:break;
  }
  return 0;
}

inline OdDb::LineWeight lineWeightByIndex(int lw)
{
  switch(lw)
  {
  case 0:
    return OdDb::kLnWt000;
  case 1:
    return OdDb::kLnWt005;
  case 2:
    return OdDb::kLnWt009;
  case 3:
    return OdDb::kLnWt013;
  case 4:
    return OdDb::kLnWt015;
  case 5:
    return OdDb::kLnWt018;
  case 6:
    return OdDb::kLnWt020;
  case 7:
    return OdDb::kLnWt025;
  case 8:
    return OdDb::kLnWt030;
  case 9:
    return OdDb::kLnWt035;
  case 10:
    return OdDb::kLnWt040;
  case 11:
    return OdDb::kLnWt050;
  case 12:
    return OdDb::kLnWt053;
  case 13:
    return OdDb::kLnWt060;
  case 14:
    return OdDb::kLnWt070;
  case 15:
    return OdDb::kLnWt080;
  case 16:
    return OdDb::kLnWt090;
  case 17:
    return OdDb::kLnWt100;
  case 18:
    return OdDb::kLnWt106;
  case 19:
    return OdDb::kLnWt120;
  case 20:
    return OdDb::kLnWt140;
  case 21:
    return OdDb::kLnWt158;
  case 22:
    return OdDb::kLnWt200;
  case 23:
    return OdDb::kLnWt211;
  }
  return OdDb::kLnWt000;
}

#endif // __OD_LINE_WT_INDEX__
