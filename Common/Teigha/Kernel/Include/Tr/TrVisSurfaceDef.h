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
// GLES2 device surface definition

#ifndef ODTRVISSURFACEDEF
#define ODTRVISSURFACEDEF

#include "TD_PackPush.h"

#include "TrVisDefs.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisSurfaceDef
{
  // Flags
  enum SurfaceDefFlags
  {
    kTransparentBackground = (1 << 0) // Disable surface background and depth buffer filling
  };

  // Size of output surface
  OdGsDCRect m_surfaceSize;
  // Color of surface background
  ODCOLORREF m_backgroundColor;
  // Flags
  OdUInt32   m_surfaceFlags;

  // Reset surface flags
  void resetSurfaceFlags(OdUInt32 surfaceFlags = 0) { m_surfaceFlags = surfaceFlags; }

  // TransparentBackground flag
  bool isTransparentBackground() const { return GETBIT(m_surfaceFlags, kTransparentBackground); }
  void setTransparentBackground(bool bSet) { SETBIT(m_surfaceFlags, kTransparentBackground, bSet); }

  void setDefault()
  {
    m_surfaceSize.set_null();
    m_backgroundColor = ODRGBA(0, 0, 0, 255);
    m_surfaceFlags = 0;
  }
};

#include "TD_PackPop.h"

#endif // ODTRVISSURFACEDEF
