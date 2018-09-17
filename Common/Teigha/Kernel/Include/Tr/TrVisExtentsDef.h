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
// GLES2 device scene extents definition

#ifndef ODTRVISEXTENTSDEF
#define ODTRVISEXTENTSDEF

#include "TD_PackPush.h"

#include "Ge/GeExtents3d.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisExtentsDef
{
  OdGeExtents3d m_sceneExtents;
  OdGeExtents3d m_realExtents;

  void setDefault()
  {
    m_sceneExtents = OdGeExtents3d::kInvalid;
    m_realExtents  = OdGeExtents3d::kInvalid;;
  }
  void reset() { setDefault(); }

  const OdGeExtents3d &sceneExtents() const { return m_sceneExtents; }
  const OdGeExtents3d &realExtents() const { return m_realExtents; }

  bool operator ==(const OdTrVisExtentsDef &extents) const
  {
    return m_sceneExtents.minPoint().isEqualTo(extents.m_sceneExtents.minPoint()) &&
           m_sceneExtents.maxPoint().isEqualTo(m_sceneExtents.maxPoint()) &&
           m_realExtents.minPoint().isEqualTo(m_realExtents.minPoint()) &&
           m_realExtents.maxPoint().isEqualTo(m_realExtents.maxPoint());
  }
  bool operator !=(const OdTrVisExtentsDef &extents) const
  {
    return !m_sceneExtents.minPoint().isEqualTo(extents.m_sceneExtents.minPoint()) ||
           !m_sceneExtents.maxPoint().isEqualTo(m_sceneExtents.maxPoint()) ||
           !m_realExtents.minPoint().isEqualTo(m_realExtents.minPoint()) ||
           !m_realExtents.maxPoint().isEqualTo(m_realExtents.maxPoint());
  }

  void operator +=(const OdTrVisExtentsDef &extents)
  {
    if (extents.m_sceneExtents.isValidExtents())
      m_sceneExtents.addExt(extents.m_sceneExtents);
    if (extents.m_realExtents.isValidExtents())
      m_realExtents.addExt(extents.m_realExtents);
  }

  const OdGeExtents3d &preferRealExtents() const
  {
    if (m_realExtents.isValidExtents())
      return m_realExtents;
    return m_sceneExtents;
  }
  const OdGeExtents3d &preferSceneExtents() const
  {
    if (m_sceneExtents.isValidExtents())
      return m_sceneExtents;
    return m_realExtents;
  }
};

#include "TD_PackPop.h"

#endif // ODTRVISEXTENTSDEF
