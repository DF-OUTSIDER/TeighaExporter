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
#include "BmpTilesGen.h"
#include "RxDictionary.h"
#include <algorithm>

BmpTilesGen::BmpTilesGen()
{
}

BmpTilesGen::BmpTilesGen(OdGsDevice* pDevice, const OdGsDCRect& rcOverall)
{
  init(pDevice, rcOverall);
}

void BmpTilesGen::init(OdGsDevice* pDevice, const OdGsDCRect& rcOverall)
{
  m_rcOverall = rcOverall;
  m_pDevice = pDevice;
}

OdGiRasterImagePtr BmpTilesGen::regenTile(const OdGsDCRect& rcTile)
{
  // check that rects have the same orientation
  ODA_ASSERT((rcTile.m_min.x < rcTile.m_max.x)==(m_rcOverall.m_min.x < m_rcOverall.m_max.x) &&
             (rcTile.m_min.y < rcTile.m_max.y)==(m_rcOverall.m_min.y < m_rcOverall.m_max.y));

  OdGsDCRect dcrc;
  OdGsDCRect rcVp;
  if(rcTile.m_min.x < rcTile.m_max.x)
  {
    if(rcTile.m_min.y < rcTile.m_max.y)
    {
      dcrc = OdGsDCRect(0, rcTile.m_max.x-rcTile.m_min.x, 0, rcTile.m_max.y-rcTile.m_min.y);
      rcVp = OdGsDCRect(m_rcOverall.m_min.x-rcTile.m_min.x,
                        m_rcOverall.m_max.x-rcTile.m_min.x,
                        m_rcOverall.m_min.y-rcTile.m_min.y,
                        m_rcOverall.m_max.y-rcTile.m_min.y);
    }
    else
    {
      dcrc = OdGsDCRect(0, rcTile.m_max.x-rcTile.m_min.x, rcTile.m_min.y-rcTile.m_max.y, 0);
      rcVp = OdGsDCRect(m_rcOverall.m_min.x-rcTile.m_min.x,
                        m_rcOverall.m_max.x-rcTile.m_min.x,
                        m_rcOverall.m_min.y-rcTile.m_max.y,
                        m_rcOverall.m_max.y-rcTile.m_max.y);
    }
  }
  else
  {
    if(rcTile.m_min.y < rcTile.m_max.y)
    {
      dcrc = OdGsDCRect(rcTile.m_min.x-rcTile.m_max.x, 0, 0, rcTile.m_max.y-rcTile.m_min.y);
      rcVp = OdGsDCRect(m_rcOverall.m_min.x-rcTile.m_max.x,
                        m_rcOverall.m_max.x-rcTile.m_max.x,
                        m_rcOverall.m_min.y-rcTile.m_min.y,
                        m_rcOverall.m_max.y-rcTile.m_min.y);
    }
    else
    {
      dcrc = OdGsDCRect(rcTile.m_min.x-rcTile.m_max.x, 0, rcTile.m_min.y-rcTile.m_max.y, 0);
      rcVp = OdGsDCRect(m_rcOverall.m_min.x-rcTile.m_max.x,
                        m_rcOverall.m_max.x-rcTile.m_max.x,
                        m_rcOverall.m_min.y-rcTile.m_max.y,
                        m_rcOverall.m_max.y-rcTile.m_max.y);
    }
  }

  /*
  if(dcrc.m_max.x < 0)
  {
    dcrc.m_max.x = -dcrc.m_max.x;
    std::swap(dcrc.m_min.x, dcrc.m_max.x);
  }
  if(dcrc.m_max.y < 0)
  {
    dcrc.m_max.y = -dcrc.m_max.y;
    std::swap(dcrc.m_min.y, dcrc.m_max.y);
  }
  */
  m_pDevice->onSize(dcrc);
  int n = m_pDevice->numViews();
  for(int i=0; i<n; ++i)
  {
    m_pDevice->viewAt(i)->setViewport(rcVp);
  }
  m_pDevice->update();

  return m_pDevice->properties()->getAt(L"RasterImage");
}
