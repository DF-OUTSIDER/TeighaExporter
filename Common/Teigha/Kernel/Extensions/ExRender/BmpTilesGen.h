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

#if !defined(_BMPTILESGEN_INCLUDED_)
#define _BMPTILESGEN_INCLUDED_

#include "TD_PackPush.h"

#include "Gs/Gs.h"
#include "Gi/GiRasterImage.h"

/** \details
  This class manages multiple BMP viewports.
  
  Library: Source code provided.
  <group ExRender_Classes> 
*/
class BmpTilesGen
{
  OdGsDevicePtr m_pDevice;
  OdGsDCRect    m_rcOverall;
public:
  BmpTilesGen();
  /** \param pDevice [in]  Pointer to the display device.
    \param rcOverall [in]  Overall Device Coordinate rectangle.
  */
  BmpTilesGen(OdGsDevice* pDevice, const OdGsDCRect& rcOverall);
  /** Initializes this BmpTilesGen object.
    \param pDevice [in]  Pointer to the display device.
    \param rcOverall [in]  Overall Device Coordinate rectangle.
  */
  void init(OdGsDevice* pDevice, const OdGsDCRect& rcOverall);
  /** \details
    Regenerates the specified DeviceCoordinate rectangle.
    \param rcTile [in]  Device Coordinate rectangle.
    \remarks
    Returns a SmartPointer to the RasterImage.
  */
  OdGiRasterImagePtr regenTile(const OdGsDCRect& dcTile);
};

#include "TD_PackPop.h"

#endif //#if !defined(_BMPTILESGEN_INCLUDED_)
