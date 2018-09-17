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

#include "OdaCommon.h"

#include "DbSystemServices.h"
#include "DbHostAppServices.h"

#include "OdGeoDataModule.h"
#include "RxDynamicModule.h"
#include "cs_map.h"

#include "OdDbGeoCoordinateSystemImpl.h"

#if defined(ODA_WINDOWS)
#include <tchar.h>
#endif

ODRX_DEFINE_DYNAMIC_MODULE(OdGeoDataModule);

bool getCsdFilesFolders(OdStringArray& aDirs);

OdGeoDataModule::OdGeoDataModule()
{
}

OdGeoDataModule::~OdGeoDataModule()
{
}

void OdGeoDataModule::initApp()
{
  OdStringArray aDirs;
  getCsdFilesFolders(aDirs);
  int st = -1;
  for(unsigned int i = 0; i < aDirs.size(); ++i)
  {
    if((st = CS_altdr(aDirs[i])) == 0)
    {
      break;
    }
  }

  if (st != 0)
  {
    OdString strPath;
    if (aDirs.size())
      strPath = aDirs[0];
    for(unsigned int i = 1; i < aDirs.size(); ++i)
    {
      strPath += L"; ";
      strPath += aDirs[i];
    }
    OdError_FileNotFound err(strPath);
    throw err;
  }
  
  OdDbGeoCoordinateSystemCategoryImpl::rxInit();
  OdDbGeoCoordinateSystemImpl::rxInit();
  OdDbGeoCoordinateSystemTransformerImpl::rxInit();

  OdDbGeoDataPEImpl::rxInit();
  OdDbGeoData::desc()->addX(OdDbGeoDataPE::desc(), &m_pGeoDataPE);
  
  OdDbGeoCoordinateSystemCategoryPEImpl::rxInit();
  OdDbGeoCoordinateSystemCategory::desc()->addX(OdDbGeoCoordinateSystemCategoryPE::desc(), &m_pGeoCoordinateSystemCategoryPE);

  OdDbGeoCoordinateSystemPEImpl::rxInit();
  OdDbGeoCoordinateSystem::desc()->addX(OdDbGeoCoordinateSystemPE::desc(), &m_pGeoCoordinateSystemPE);

  OdDbGeoCoordinateSystemTransformerPEImpl::rxInit();
  OdDbGeoCoordinateSystemTransformer::desc()->addX(OdDbGeoCoordinateSystemTransformerPE::desc(), &m_pGeoCoordinateSystemTransformerPE);
}

void OdGeoDataModule::uninitApp()
{
  CS_recvr();

  OdDbGeoCoordinateSystemTransformer::desc()->delX(OdDbGeoCoordinateSystemTransformerPE::desc());
  OdDbGeoCoordinateSystemTransformerPEImpl::rxUninit();

  OdDbGeoCoordinateSystem::desc()->delX(OdDbGeoCoordinateSystemPE::desc());
  OdDbGeoCoordinateSystemPEImpl::rxUninit();

  OdDbGeoCoordinateSystemCategory::desc()->delX(OdDbGeoCoordinateSystemCategoryPE::desc());
  OdDbGeoCoordinateSystemCategoryPEImpl::rxUninit();
  
  OdDbGeoData::desc()->delX(OdDbGeoDataPE::desc());
  OdDbGeoDataPEImpl::rxUninit();

  OdDbGeoCoordinateSystemTransformerImpl::rxUninit();
  OdDbGeoCoordinateSystemImpl::rxUninit();
  OdDbGeoCoordinateSystemCategoryImpl::rxUninit();
}

#ifndef ODA_UNIXOS
#define separatorChar ';'
#else
#define separatorChar ':'
#endif

bool getCsdFilesFolders(OdStringArray& aDirs)
{
  OdString sEnv;
#ifndef OD_NO_GETENV
#if !defined(_WINRT)
  // TODO add getenv support to WinRT
#if defined(ODA_WINDOWS)
  sEnv = (OdChar *)_tgetenv(L"CS_MAP_DIR");
#else
  sEnv = getenv("CS_MAP_DIR");
#endif
#endif
#endif
  aDirs.clear();
  sEnv.trimRight();

  while (!sEnv.trimLeft().isEmpty())
  {
    OdString sDir;
    int pos = sEnv.find(separatorChar);
    if (pos >= 0)
    {
      sDir = sEnv.left(pos);
      sEnv = sEnv.mid(pos + 1);
    }  
    else
    {
      sDir = sEnv;
      sEnv.empty();
    }
    if (!sDir.trimRight().isEmpty())
      aDirs.append(sDir);
  }

#if defined(ODA_WINDOWS) && !defined(_WINRT)
  TCHAR path[MAX_PATH];
  ::GetModuleFileName(::GetModuleHandle(0), path, MAX_PATH);
  OdString sPath(path);
  int nLen = sPath.reverseFind('\\');
  sPath = sPath.left(nLen);
  aDirs.append(sPath + "\\CSDicts");
  //aDirs.append(sPath + "\\..\\..\\ThirdParty\\csmap\\Dictionaries");
#endif

  return aDirs.size() != 0;
}
