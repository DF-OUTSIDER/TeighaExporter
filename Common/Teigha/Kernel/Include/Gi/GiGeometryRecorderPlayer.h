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

#ifndef __OD_GI_GEOMETRY_RECORDER_PLAYER__
#define __OD_GI_GEOMETRY_RECORDER_PLAYER__

#include "Gi/GiConveyorGeometry.h"

#include "TD_PackPush.h"

/** \details
  Records conveyor geometry and plays it.

  Corresponding C++ library: TD_Gi
  <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiGeometryRecorderPlayer: public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiGeometryRecorderPlayer);
  virtual OdGiConveyorGeometry& recorderGeometry() = 0;
  virtual void play(OdGiConveyorGeometry& destGeometry) = 0;
  virtual void rewind() = 0;
};

typedef OdSmartPtr<OdGiGeometryRecorderPlayer> OdGiGeometryRecorderPlayerPtr;

#include "TD_PackPop.h"

#endif // __OD_GI_GEOMETRY_RECORDER_PLAYER__
