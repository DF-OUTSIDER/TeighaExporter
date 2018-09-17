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

#ifndef _ODDBABSTRACTVIEWPORTDATAFORDBVIEWTABLERECORD_INCLUDED
#define _ODDBABSTRACTVIEWPORTDATAFORDBVIEWTABLERECORD_INCLUDED

#include "TD_PackPush.h"

#include "DbAbstractViewportDataForAbstractViewTabRec.h"

/** \details
    This class is the Protocol Extension class for OdDbViewTableRecord objects.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbAbstractViewportDataForDbViewTabRec : public OdDbAbstractViewportDataForAbstractViewTabRec
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAbstractViewportDataForDbViewTabRec);

  // OdAbstractViewPE

  bool hasUcs(const OdRxObject* pViewport) const;

  bool viewExtents(const OdRxObject* pViewport, OdGeBoundBlock3d& extents) const;

  // OdDbAbstractViewportData

  bool isUcsSavedWithViewport(const OdRxObject* pViewport) const;
  void setUcsPerViewport( OdRxObject* pViewport, bool ucsPerViewport) const;

  bool isUcsFollowModeOn(const OdRxObject* pViewport) const;
  void setUcsFollowModeOn(OdRxObject* pViewport, bool ucsFollowMode) const;

  OdUInt16 circleSides(const OdRxObject* pViewport) const;
  void setCircleSides(OdRxObject* pViewport, OdUInt16 circleSides) const;

  bool isGridOn(const OdRxObject* pViewport) const;
  void setGridOn(OdRxObject* pViewport, bool gridOn) const;

  OdGeVector2d gridIncrement(const OdRxObject* pViewport) const;
  void setGridIncrement(OdRxObject* pViewport, const OdGeVector2d& gridIncrement) const;

  bool isGridBoundToLimits(const OdRxObject* pViewport) const;
  void setGridBoundToLimits(OdRxObject* pViewport, bool gridDispFlag) const;

  bool isGridAdaptive(const OdRxObject* pViewport) const;
  void setGridAdaptive(OdRxObject* pViewport, bool gridDispFlag) const;

  bool isGridSubdivisionRestricted(const OdRxObject* pViewport) const;
  void setGridSubdivisionRestricted(OdRxObject* pViewport, bool gridDispFlag) const;

  bool isGridFollow(const OdRxObject* pViewport) const;
  void setGridFollow(OdRxObject* pViewport, bool gridDispFlag) const;

  OdInt16 gridMajor(const OdRxObject* pViewport) const;
  void setGridMajor(OdRxObject* pViewport, OdInt16 nMajor) const;

  bool isUcsIconVisible(const OdRxObject* pViewport) const;
  void setUcsIconVisible(OdRxObject* pViewport, bool iconVisible) const;

  bool isUcsIconAtOrigin(const OdRxObject* pViewport) const;
  void setUcsIconAtOrigin(OdRxObject* pViewport, bool atOrigin) const;

  bool isSnapOn(const OdRxObject* pViewport) const;
  void setSnapOn(OdRxObject* pViewport, bool atOrigin) const;

  bool isSnapIsometric(const OdRxObject* pViewport) const;
  void setSnapIsometric(OdRxObject* pViewport, bool snapIsometric) const;

  double snapAngle(const OdRxObject* pViewport) const;
  void setSnapAngle(OdRxObject* pViewport, double snapAngle) const;

  OdGePoint2d snapBase(const OdRxObject* pViewport) const;
  void setSnapBase(OdRxObject* pViewport, const OdGePoint2d& snapBase) const; 

  OdGeVector2d snapIncrement(const OdRxObject* pViewport) const;
  void setSnapIncrement(OdRxObject* pViewport, const OdGeVector2d& snapIncrement) const;

  OdUInt16 snapIsoPair(const OdRxObject* pViewport) const;
  void setSnapIsoPair(OdRxObject* pViewport, OdUInt16 snapIncrement) const;

  OdGsView* gsView(const OdRxObject* pViewport) const;
  void setGsView(OdRxObject* pViewport, OdGsView* pGsView) const;
};

#include "TD_PackPop.h"

#endif // _ODDBABSTRACTVIEWPORTDATAFORDBVIEWTABLERECORD_INCLUDED
