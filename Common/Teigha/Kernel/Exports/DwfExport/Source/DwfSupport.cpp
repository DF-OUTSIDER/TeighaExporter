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


///////////////////////////////////////////////////////////////////////////////
//
// DwfSupport.cpp : the methods for writing to DWF File and Auxiliary methods.
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "DwfSupport.h"

namespace TD_DWF_EXPORT {

///////////////////////////////////////////////////////////////////////////////
// Auxiliary methods
///////////////////////////////////////////////////////////////////////////////

//
// boxAnticlockwise()
//
//  To set a direction of box detour clockwise
//
void boxAnticlockwise(WT_Logical_Point* bounds, const OdGePoint3d& ptMin, const OdGePoint3d& ptMax)
{

  bounds[0].m_x = RoundDoubleToWTInteger32(ptMin.x);
  bounds[0].m_y = RoundDoubleToWTInteger32(ptMin.y);

  bounds[1].m_x = RoundDoubleToWTInteger32(ptMax.x);  
  bounds[1].m_y = RoundDoubleToWTInteger32(ptMin.y);  

  bounds[2].m_x = RoundDoubleToWTInteger32(ptMax.x);
  bounds[2].m_y = RoundDoubleToWTInteger32(ptMax.y);

  bounds[3].m_x = RoundDoubleToWTInteger32(ptMin.x);  
  bounds[3].m_y = RoundDoubleToWTInteger32(ptMax.y);
}

//
// boxClockwise()
//
//  To set a direction of box detour clockwise
//
void boxClockwise(WT_Logical_Point* bounds, const OdGePoint3d& ptMin, const OdGePoint3d& ptMax)
{

  bounds[0].m_x = RoundDoubleToWTInteger32(ptMin.x);
  bounds[0].m_y = RoundDoubleToWTInteger32(ptMin.y);

  bounds[1].m_x = RoundDoubleToWTInteger32(ptMin.x);  
  bounds[1].m_y = RoundDoubleToWTInteger32(ptMax.y);  

  bounds[2].m_x = RoundDoubleToWTInteger32(ptMax.x);
  bounds[2].m_y = RoundDoubleToWTInteger32(ptMax.y);

  bounds[3].m_x = RoundDoubleToWTInteger32(ptMax.x);  
  bounds[3].m_y = RoundDoubleToWTInteger32(ptMin.y);
}
}
