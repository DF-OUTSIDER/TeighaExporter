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

#ifndef _ODDBABSTRACTVIEWPORTDATAFORDBVPTABLERECORD_INCLUDED
#define _ODDBABSTRACTVIEWPORTDATAFORDBVPTABLERECORD_INCLUDED

#include "DbAbstractViewportDataForAbstractViewTabRec.h"

#include "TD_PackPush.h"

/** \details
    Corresponding C++ library: TD_Db
    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbAbstractViewportDataForDbVpTabRec : public OdDbAbstractViewportDataForAbstractViewTabRec
{
public:
  ODRX_DECLARE_MEMBERS( OdDbAbstractViewportDataForDbVpTabRec );

  OdGePoint2d lowerLeftCorner(const OdRxObject* pVpFrom) const;
  OdGePoint2d upperRightCorner(const OdRxObject* pVpFrom) const;
  void setViewport(OdRxObject* pVpTo, const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight) const;
  bool hasViewport(const OdRxObject* pVpFrom) const;

  bool viewExtents(const OdRxObject* pVp, OdGeBoundBlock3d& extents) const;

  bool hasUcs(const OdRxObject* pVpFrom) const;
  void getUcs(const OdRxObject* pVpFrom, OdGePoint3d& origin, OdGeVector3d& xAxis, OdGeVector3d& yAxis) const;
  OdDb::OrthographicView orthoUcs(const OdRxObject* pVpFrom) const;
  OdDbStub* ucsName(const OdRxObject* pVpFrom) const;
  double elevation(const OdRxObject* pVpFrom) const;

  bool isUcsSavedWithViewport(const OdRxObject* pVpFrom) const;
  void setUcsPerViewport( OdRxObject* pVpTo, bool ucsvp ) const;

  bool isUcsFollowModeOn(const OdRxObject* pVpFrom) const;
  void setUcsFollowModeOn(OdRxObject* pVpTo, bool bOn) const;

  OdUInt16 circleSides(const OdRxObject* pVpFrom) const;
  void setCircleSides(OdRxObject* pVpTo, OdUInt16) const;

  bool isGridOn(const OdRxObject* pVpFrom) const;
  void setGridOn(OdRxObject* pVpTo, bool bOn) const;

  OdGeVector2d gridIncrement(const OdRxObject* pVpFrom) const;
  void setGridIncrement(OdRxObject* pVpTo, const OdGeVector2d&) const;

  bool isGridBoundToLimits(const OdRxObject* pVpFrom) const;
  void setGridBoundToLimits(OdRxObject* pVpTo, bool gridDispFlag) const;

  bool isGridAdaptive(const OdRxObject* pVpFrom) const;
  void setGridAdaptive(OdRxObject* pVpTo, bool gridDispFlag) const;

  bool isGridSubdivisionRestricted(const OdRxObject* pVpFrom) const;
  void setGridSubdivisionRestricted(OdRxObject* pVpTo, bool gridDispFlag) const;

  bool isGridFollow(const OdRxObject* pVpFrom) const;
  void setGridFollow(OdRxObject* pVpTo, bool gridDispFlag) const;

  OdInt16 gridMajor(const OdRxObject* pVpFrom) const;
  void setGridMajor(OdRxObject* pVpTo, OdInt16 nMajor) const;

  bool isUcsIconVisible(const OdRxObject* pVpFrom) const;
  void setUcsIconVisible(OdRxObject* pVpTo, bool bVisible) const;

  bool isUcsIconAtOrigin(const OdRxObject* pVpFrom) const;
  void setUcsIconAtOrigin(OdRxObject* pVpTo, bool bAtOrigin) const;

  bool isSnapOn(const OdRxObject* pVpFrom) const;
  void setSnapOn(OdRxObject* pVpTo, bool bOn) const;

  bool isSnapIsometric(const OdRxObject* pVpFrom) const;
  void setSnapIsometric(OdRxObject* pVpTo, bool bIsometric) const;

  double snapAngle(const OdRxObject* pVpFrom) const;
  void setSnapAngle(OdRxObject* pVpTo, double) const;

  OdGePoint2d snapBase(const OdRxObject* pVpFrom) const;
  void setSnapBase(OdRxObject* pVpTo, const OdGePoint2d&) const; 

  OdGeVector2d snapIncrement(const OdRxObject* pVpFrom) const;
  void setSnapIncrement(OdRxObject* pVpTo, const OdGeVector2d&) const;

  OdUInt16 snapIsoPair(const OdRxObject* pVpFrom) const;
  void setSnapIsoPair(OdRxObject* pVpTo, OdUInt16) const;

  OdGsView* gsView(const OdRxObject* pViewport) const;
  void setGsView(OdRxObject* pViewport, OdGsView* pGsView) const;

  OdRxObjectPtr plotDataObject(const OdRxObject* pViewport, bool bOpenForWrite = false) const;
};

#include "TD_PackPop.h"

#endif // _ODDBABSTRACTVIEWPORTDATAFORDBVPTABLERECORD_INCLUDED

