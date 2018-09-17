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

// to satisfy VC2005, else it refuses to compile WinDirectX project
#if !defined(_WIN32_WCE) && (_MSC_VER >= 1400)
  #define POINTER_64 __ptr64
#endif

#include "OdaCommon.h"
#include "CommonDeviceProps.h"

#include "Gi/GiCommonDraw.h"
#include "Gs/GsDbRootLinkage.h"

OdCommonDeviceProps::OdCommonDeviceProps()
: m_bDoubleBufferEnabled(true)
, m_bDrawSelection(false)
, m_bDrawDragging(false)
, m_bBlocksCacheEnabled(false)
, m_bDynamicSubEntHltEnabled(false)
, m_nGsMultithreadProps(0)
, m_nMaxMtRegenThreads(4)
{
}

OdCommonDeviceProps::~OdCommonDeviceProps()
{
}

ODRX_DECLARE_PROPERTY(DoubleBufferEnabled)
ODRX_DECLARE_PROPERTY(DrawSelection)
ODRX_DECLARE_PROPERTY(DrawDragging)
ODRX_DECLARE_PROPERTY(BlocksCache)
ODRX_DECLARE_PROPERTY(DynamicSubEntHlt)
ODRX_DECLARE_PROPERTY(EnableMultithread)
ODRX_DECLARE_PROPERTY(ForcePartialUpdate)
ODRX_DECLARE_PROPERTY(MaxRegenThreads)
ODRX_DECLARE_PROPERTY(ColorConverter)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(OdCommonDeviceProps);
  ODRX_GENERATE_PROPERTY(DoubleBufferEnabled)
  ODRX_GENERATE_PROPERTY(DrawSelection)
  ODRX_GENERATE_PROPERTY(DrawDragging)
  ODRX_GENERATE_PROPERTY(BlocksCache)
  ODRX_GENERATE_PROPERTY(DynamicSubEntHlt)
  ODRX_GENERATE_PROPERTY(EnableMultithread)
  ODRX_GENERATE_PROPERTY(ForcePartialUpdate)
  ODRX_GENERATE_PROPERTY(MaxRegenThreads)
  ODRX_GENERATE_PROPERTY(ColorConverter)
ODRX_END_DYNAMIC_PROPERTY_MAP(OdCommonDeviceProps);

ODRX_DEFINE_PROPERTY_METHODS(DoubleBufferEnabled, OdCommonDeviceProps, doubleBufferEnabled,     setDoubleBufferEnabled,     getBool)
ODRX_DEFINE_PROPERTY_METHODS(DrawSelection,       OdCommonDeviceProps, isDrawSelection,         setDrawSelection,           getBool)
ODRX_DEFINE_PROPERTY_METHODS(DrawDragging,        OdCommonDeviceProps, isDrawDragging,          setDrawDragging,            getBool)
ODRX_DEFINE_PROPERTY_METHODS(BlocksCache,         OdCommonDeviceProps, blocksCacheEnabled,      setBlocksCacheEnabled,      getBool)
ODRX_DEFINE_PROPERTY_METHODS(DynamicSubEntHlt,    OdCommonDeviceProps, dynamicSubEntHltEnabled, setDynamicSubEntHltEnabled, getBool)
ODRX_DEFINE_PROPERTY_METHODS(EnableMultithread,   OdCommonDeviceProps, gsMultithreadEnabled,    setGsMultithreadEnabled,    getBool)
ODRX_DEFINE_PROPERTY_METHODS(ForcePartialUpdate,  OdCommonDeviceProps, forcePartialUpdateForMt, setForcePartialUpdateForMt, getBool)
ODRX_DEFINE_PROPERTY_METHODS(MaxRegenThreads,     OdCommonDeviceProps, maxMtRegenThreads,       setMaxMtRegenThreads,       getUInt16)
ODRX_DEFINE_PROPERTY_OBJECT (ColorConverter,      OdCommonDeviceProps, getColorConverter,       setColorConverter,          OdColorConverterCallback)

bool OdCommonDeviceProps::checkMtRegenEnabled(OdGiContext *pGiContext) const
{
  if (!gsMultithreadEnabled())
  {
    OdDbBaseHostAppServices *pAppServ = (pGiContext) ? OdGsDbRootLinkage::getDatabaseDbBaseHostAppServices(pGiContext->database()) : NULL;
    if (!pAppServ) return false;
    return GETBIT(pAppServ->getMtMode(), 2);
  }
  return true;
}

OdUInt16 OdCommonDeviceProps::getMtRegenThreadsCount(OdGiContext *pGiContext) const
{
  if (!gsMultithreadEnabled())
  {
    OdDbBaseHostAppServices *pAppServ = (pGiContext) ? OdGsDbRootLinkage::getDatabaseDbBaseHostAppServices(pGiContext->database()) : NULL;
    if (!pAppServ) return 1;
    return (OdUInt16)pAppServ->numThreads(OdDb::kMTRendering);
  }
  return m_nMaxMtRegenThreads;
}

//
