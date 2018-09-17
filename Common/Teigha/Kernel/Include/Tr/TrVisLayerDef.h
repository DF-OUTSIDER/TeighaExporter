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
// GLES2 device layer definition

#ifndef ODTRVISLAYERDEF
#define ODTRVISLAYERDEF

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "OdArray.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisLayerProps
{
  // Flags
  enum LayerPropsFlags
  {
    kPersistent = (1 << 0), // This layer cannot be deleted
    kTemporary  = (1 << 1), // Non-DBRO layer
    kFrozen     = (1 << 2), // Layer is frozen
    kOff        = (1 << 3), // Layer is disabled
    kVisible    = (1 << 4), // True if !kFrozen and !kOff
    kPlottable  = (1 << 5), // Layer can be printed
    kLocked     = (1 << 6) // Layer is locked
  };
  // Layer flags
  OdUInt16 m_flags;

  void resetFlags(OdUInt16 flags = 0) { m_flags = flags; }
  void updateVisibility() { setVisible(!GETBIT(m_flags, kFrozen) && !GETBIT(m_flags, kOff)); }

  void setPersistent(bool bSet) { SETBIT(m_flags, kPersistent, bSet); }
  bool isPersistent() const { return GETBIT(m_flags, kPersistent); }

  void setTemporary(bool bSet) { SETBIT(m_flags, kTemporary, bSet); }
  bool isTemporary() const { return GETBIT(m_flags, kTemporary); }

  void setFrozen(bool bSet) { SETBIT(m_flags, kFrozen, bSet); updateVisibility(); }
  bool isFrozen() const { return GETBIT(m_flags, kFrozen); }

  void setOff(bool bSet) { SETBIT(m_flags, kOff, bSet); updateVisibility(); }
  bool isOff() const { return GETBIT(m_flags, kOff); }

  void setVisible(bool bSet) { SETBIT(m_flags, kVisible, bSet); }
  bool isVisible() const { return GETBIT(m_flags, kVisible); }

  void setPlottable(bool bSet) { SETBIT(m_flags, kPlottable, bSet); }
  bool isPlottable() const { return GETBIT(m_flags, kPlottable); }

  void setLocked(bool bSet) { SETBIT(m_flags, kLocked, bSet); }
  bool isLocked() const { return GETBIT(m_flags, kLocked); }

  bool operator ==(const OdTrVisLayerProps &l2) const { return m_flags == l2.m_flags; }
  bool operator !=(const OdTrVisLayerProps &l2) const { return m_flags != l2.m_flags; }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisLayerDef
{
  // Layer name
  OdString m_name;
  // Linked DbId
  OdDbStub *m_pOwner;
  // Layer properties
  OdTrVisLayerProps m_props;

  void setDefault()
  {
    m_name.empty();
    m_pOwner = NULL;
    m_props.resetFlags();
  }

  OdUInt32 diff(const OdTrVisLayerDef &l2) const
  {
    if (m_name != l2.m_name)
      return 1;
    if (m_props != l2.m_props)
      return 1;
    return 0;
  }

  void propagateFlag(OdTrVisLayerProps::LayerPropsFlags nFlag, bool bSet)
  {
    const bool bUpdateVisibility = (nFlag == OdTrVisLayerProps::kFrozen) || (nFlag == OdTrVisLayerProps::kOff);
    SETBIT(m_props.m_flags, nFlag, bSet);
    if (bUpdateVisibility)
      m_props.updateVisibility();
  }
  void propSetFrozen(bool bSet)    { propagateFlag(OdTrVisLayerProps::kFrozen, bSet); }
  void propSetOff(bool bSet)       { propagateFlag(OdTrVisLayerProps::kOff, bSet); }
  void propSetVisible(bool bSet)   { propagateFlag(OdTrVisLayerProps::kVisible, bSet); }
  void propSetPlottable(bool bSet) { propagateFlag(OdTrVisLayerProps::kPlottable, bSet); }
  void propSetLocked(bool bSet)    { propagateFlag(OdTrVisLayerProps::kLocked, bSet); }
};

#include "TD_PackPop.h"

#endif // ODTRVISLAYERDEF
