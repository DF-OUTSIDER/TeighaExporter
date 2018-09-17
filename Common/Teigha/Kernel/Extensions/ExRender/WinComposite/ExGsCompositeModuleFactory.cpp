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
// Maps GsModule with RenderMode

#include "OdaCommon.h"
#include "ExGsCompositeModuleFactory.h"

ExGsCompositeModuleFactory::ExGsCompositeModuleFactory()
{
  for (long i = 0; i < OdGsView::kNone; i++)
  {
    m_map[i].m_renderMode = (OdGsView::RenderMode)i;
    m_map[i].m_renderFlags = 0;
  }
}

ExGsCompositeModuleFactory::~ExGsCompositeModuleFactory()
{
}

#define CHECK_RM_RANGE(mode) \
  if (mode < OdGsView::k2DOptimized || mode > OdGsView::kGouraudShadedWithWireframe) \
  { \
    ODA_FAIL(); \
  }

void ExGsCompositeModuleFactory::setModuleForState(OdGsView::RenderMode mode, const OdGsModule *pGsModule, OdUInt32 flags)
{
  CHECK_RM_RANGE(mode);
  m_map[mode].m_pRenderModule = pGsModule;
  m_map[mode].m_renderFlags = flags;
}

const OdGsModule *ExGsCompositeModuleFactory::moduleForState(OdGsView::RenderMode mode) const
{
  CHECK_RM_RANGE(mode);
  return m_map[mode].m_pRenderModule.get();
}

OdGsModule *ExGsCompositeModuleFactory::moduleForState(OdGsView::RenderMode mode)
{
  CHECK_RM_RANGE(mode);
  return m_map[mode].m_pRenderModule.get();
}

OdUInt32 ExGsCompositeModuleFactory::flagsForState(OdGsView::RenderMode mode) const
{
  CHECK_RM_RANGE(mode);
  return m_map[mode].m_renderFlags;
}

void ExGsCompositeModuleFactory::setModuleForAllStates(const OdGsModule *pGsModule, OdUInt32 flags)
{
  for (OdUInt32 rm = 0; rm < OdGsView::kNone; rm++)
  {
    m_map[rm].m_pRenderModule = pGsModule;
    m_map[rm].m_renderFlags = flags;
  }
}

void ExGsCompositeModuleFactory::resetModules()
{
  for (OdUInt32 rm = 0; rm < OdGsView::kNone; rm++)
  {
    m_map[rm].m_pRenderModule.release();
    m_map[rm].m_renderFlags = 0;
  }
}

bool ExGsCompositeModuleFactory::isSimilarModules(bool bCheckFlags) const
{
  for (OdUInt32 rm = 1; rm < OdGsView::kNone; rm++)
  {
    if (m_map[rm].m_pRenderModule.get() != m_map[0].m_pRenderModule.get())
      return false;
    if (bCheckFlags)
    {
      if (m_map[rm].m_renderFlags != m_map[0].m_renderFlags)
        return false;
    }
  }
  return true;
}

bool ExGsCompositeModuleFactory::isValid() const
{
  for (OdUInt32 rm = 0; rm < OdGsView::kNone; rm++)
  {
    if (m_map[rm].m_pRenderModule.isNull())
      return false;
  }
  return true;
}

bool ExGsCompositeModuleFactory::validate()
{
  OdGsModulePtr pModule; OdUInt32 rm, flags;
  for (rm = 0; rm < OdGsView::kNone; rm++)
  {
    if (!m_map[rm].m_pRenderModule.isNull())
    {
      pModule = m_map[rm].m_pRenderModule;
      flags = m_map[rm].m_renderFlags;
      break;
    }
  }
  if (pModule.isNull())
    return false;
  for (rm = 0; rm < OdGsView::kNone; rm++)
  {
    if (m_map[rm].m_pRenderModule.isNull())
    {
      m_map[rm].m_pRenderModule = pModule;
      m_map[rm].m_renderFlags = flags;
    }
  }
  return true;
}

//
