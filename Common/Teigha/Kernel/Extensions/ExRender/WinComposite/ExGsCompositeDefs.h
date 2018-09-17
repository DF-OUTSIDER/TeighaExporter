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
// Composite device definitions

#ifndef EXGSCOMPOSITEDEVICEDEFINITIONS
#define EXGSCOMPOSITEDEVICEDEFINITIONS

#include "TD_PackPush.h"

#include "OdaCommon.h"
#include "Gs/Gs.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct ExGsCompositeSyncFlags
{
  OdUInt32 m_syncFlags;
  ExGsCompositeSyncFlags() : m_syncFlags(0) { }
  void setSyncFlag(OdUInt32 flag) { SETBIT(m_syncFlags, flag, true); }
  bool isSyncFlagSet(OdUInt32 flag) const { return GETBIT(m_syncFlags, flag); }
  void resetSyncFlags() { m_syncFlags = 0; }
  void setSyncAll() { m_syncFlags = 0xFFFFFFFF; }
  bool isSynchronized() const { return m_syncFlags == 0; }
};

#define OD_GS_COMPOSITE_PROP(flag, getter, setter) \
  void setter(bool bSet) { SETBIT(m_compositeFlags, flag, bSet); } \
  bool getter() const { return GETBIT(m_compositeFlags, flag); }

#include "TD_PackPop.h"

#endif // EXGSCOMPOSITEDEVICEDEFINITIONS
