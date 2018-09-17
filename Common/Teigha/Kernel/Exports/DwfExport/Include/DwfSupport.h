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
// DwfSupport.h : the auxiliary methods for helping to write into DWF File
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DWFSUPPORT_DEFINED
#define DWFSUPPORT_DEFINED

#include "whiptk/whip_toolkit.h"

#include "OdaCommon.h"
#include "Gs/Gs.h"

#include "Ge/GeLineSeg2d.h"
#include "Ge/GePoint3d.h"


///////////////////////////////////////////////////////////
//
// Auxiliary methods
//

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DWF_EXPORT {

#define RoundDoubleToWTInteger32(X) (WT_Integer32(long(X+.5)))
#define RoundDoubleToWTInteger16(X) (WT_Integer16(short(X+.5)))
#define RoundDoubleToWTUInteger16(X) (WT_Unsigned_Integer16((unsigned short)(X+.5)))


inline void RoundPoint(WT_Logical_Point& to, const OdGePoint2d& from)
{
  OdGePoint2d point(from);

  to.m_x = RoundDoubleToWTInteger32(point.x);
  to.m_y = RoundDoubleToWTInteger32(point.y);
}

void boxAnticlockwise(WT_Logical_Point* bounds, 
                      const OdGePoint3d& ptMin, 
                      const OdGePoint3d& ptMax);

void boxClockwise(WT_Logical_Point* bounds, 
                  const OdGePoint3d& ptMin, 
                  const OdGePoint3d& ptMax);

}
#endif /* DWFSUPPORT_DEFINED */
