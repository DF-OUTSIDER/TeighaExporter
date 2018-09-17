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

#ifndef _ODDBABSTRACTVIEWPORTDATAFORABSTRACTVIEWTABLEREC_INCLUDED
#define _ODDBABSTRACTVIEWPORTDATAFORABSTRACTVIEWTABLEREC_INCLUDED

#include "TD_PackPush.h"
#include "DbAbstractViewportData.h"

/** \details
    This class is the Protocol Extension class for OdDbViewportTableRecord objects.

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbAbstractViewportDataForAbstractViewTabRec : public OdDbAbstractViewportData
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAbstractViewportDataForAbstractViewTabRec);

  void setView(
      OdRxObject* pViewport,
      const OdGePoint3d& target,
      const OdGeVector3d& direction,
      const OdGeVector3d& upVector,
      double fieldWidth,
      double fieldHeight,
      bool isPerspective,
      const OdGeVector2d& viewOffset = OdGeVector2d::kIdentity) const;

  OdGePoint3d  target(const OdRxObject* pViewport) const;
  OdGeVector3d direction(const OdRxObject* pViewport) const;
  OdGeVector3d upVector(const OdRxObject* pViewport) const;

  double fieldWidth(const OdRxObject* pViewport) const;
  double fieldHeight(const OdRxObject* pViewport) const;

  OdGeVector2d viewOffset(const OdRxObject* pViewport) const;

  double viewTwist(const OdRxObject* pViewport) const;

  bool isPerspective(const OdRxObject* pViewport) const;

  void setLensLength(OdRxObject* pViewport, double lensLength) const;
  double lensLength(const OdRxObject* pViewport) const;

  bool isFrontClipOn(const OdRxObject* pViewport) const;
  void setFrontClipOn(OdRxObject* pViewport, bool frontClip) const;

  bool isBackClipOn(const OdRxObject* pViewport) const;
  void setBackClipOn(OdRxObject* pViewport, bool backClip) const;

  bool isFrontClipAtEyeOn(const OdRxObject* pViewport) const;
  void setFrontClipAtEyeOn(OdRxObject* pViewport, bool frontClipAtEye) const;

  double frontClipDistance(const OdRxObject* pViewport) const;
  void setFrontClipDistance(OdRxObject* pViewport, double frontClipDistance) const;

  double backClipDistance(const OdRxObject* pViewport) const;
  void setBackClipDistance(OdRxObject* pViewport, double backClipDistance) const;

  void setRenderMode(OdRxObject* pViewport, OdDb::RenderMode renderMode) const;
  OdDb::RenderMode renderMode(const OdRxObject* pViewport) const;

  void setVisualStyle(OdRxObject* pViewport, OdDbStub* visualStyleId) const;
  OdDbStub* visualStyle(const OdRxObject* pViewport) const;

  void setBackground(OdRxObject* pViewport, OdDbStub* backgroundId) const;
  OdDbStub* background(const OdRxObject* pViewport) const;

  bool hasUcs(const OdRxObject* pViewport) const;
  void getUcs(const OdRxObject* pViewport, OdGePoint3d& origin, OdGeVector3d& xAxis, OdGeVector3d& yAxis) const;
  OdDb::OrthographicView orthoUcs(const OdRxObject* pViewport, const OdDbBaseDatabase* pDb = 0) const;
  double elevation(const OdRxObject* pViewport) const;
  void setUcs(OdRxObject* pViewport, const OdGePoint3d& origin, const OdGeVector3d& xAxis, const OdGeVector3d& yAxis) const;
  bool setUcs(OdRxObject* pViewport, OdDb::OrthographicView orthoUcs, const OdDbBaseDatabase* pDb = 0) const;
  OdDbStub* ucsName(const OdRxObject* pViewport) const;
  bool setUcs(OdRxObject* pViewport, OdDbStub* ucsId) const;
  void setElevation(OdRxObject* pViewport, double elevation) const;

  TD_USING(OdDbAbstractViewportData::setView);
  TD_USING(OdDbAbstractViewportData::setUcs);

  bool isDefaultLightingOn(const OdRxObject* pViewport) const;
  void setDefaultLightingOn(OdRxObject* pViewport, bool isOn) const;

  OdGiViewportTraits::DefaultLightingType defaultLightingType(const OdRxObject* pViewport) const;
  void setDefaultLightingType(OdRxObject* pViewport, OdGiViewportTraits::DefaultLightingType lightingType) const;

  double brightness(const OdRxObject* pViewport) const;
  void setBrightness(OdRxObject* pViewport, double brightness) const;

  double contrast(const OdRxObject* pViewport) const;
  void setContrast(OdRxObject* pViewport, double contrast) const;

  OdCmColor ambientLightColor(const OdRxObject* pViewport) const;
  void setAmbientLightColor(OdRxObject* pViewport, const OdCmColor& color) const;

  OdDbStub *sunId(const OdRxObject* pViewport) const;
  OdDbStub *setSun(OdRxObject* pViewport, OdRxObject* pSun) const;

  void toneOperatorParameters(const OdRxObject* pViewport, OdGiToneOperatorParameters &params) const;
  void setToneOperatorParameters(OdRxObject* pViewport, const OdGiToneOperatorParameters &params) const;
};

#include "TD_PackPop.h"

#endif // _ODDBABSTRACTVIEWPORTDATAFORABSTRACTVIEWTABLEREC_INCLUDED

