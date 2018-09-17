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


#ifndef __OD_DB_LIGHT__
#define __OD_DB_LIGHT__

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "Gi/GiLightTraits.h"

/** \details
    Corresponding C++ library: SCENEOE

    <group OdDb_Classes>
*/
class SCENEOE_EXPORT OdDbLight : public OdDbEntity
{
public:
  ODDB_DECLARE_MEMBERS(OdDbLight);

  OdDbLight();

  enum PhysicalIntensityMethod
  {
    kPeakIntensity = 0,
    kFlux,
    kIlluminance
  };

  enum LampColorType
  {
    kKelvin = 0,
    kPreset
  };

  enum LampColorPreset
  {
    kD65White = 0,
    kFluorescent,
    kCoolWhite,
    kWhiteFluorescent,
    kDaylightFluorescent,
    kIncandescent,
    kXenon,
    kHalogen,
    kQuartz,
    kMetalHalide,
    kMercury,
    kPhosphorMercury,
    kHighPressureSodium,
    kLowPressureSodium,
    kCustom
  };

  enum GlyphDisplayType
  {
    kGlyphDisplayAuto = 0,
    kGlyphDisplayOn,
    kGlyphDisplayOff
  };

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  virtual OdResult subErase(bool erasing);
  virtual void subHighlight(bool bDoIt = true, const OdDbFullSubentPath* pSubId = 0, bool highlightAll = false) const;

  virtual DrawableType drawableType() const;
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;

  OdString name() const;
  void setName(OdString name);

  OdGiDrawable::DrawableType lightType() const;
  void setLightType(OdGiDrawable::DrawableType type);

  bool isOn() const;
  void setOn(bool value);

  bool isPlottable() const;
  void setIsPlottable(bool plot);

  OdCmColor lightColor() const;
  void setLightColor(const OdCmColor& color);

  double intensity() const;
  void setIntensity(double intensity);

  void setShadowParameters(const OdGiShadowParameters& params);
  const OdGiShadowParameters &shadowParameters() const;

  void setLightAttenuation(const OdGiLightAttenuation& params);
  const OdGiLightAttenuation &lightAttenuation() const;

  void setPosition(const OdGePoint3d& pos);
  OdGePoint3d position() const;

  OdGePoint3d targetLocation() const;
  void setTargetLocation(const OdGePoint3d& newVal);

  void setHotspotAndFalloff(double hotspot, double falloff);
  double hotspotAngle() const;
  double falloffAngle() const;

  OdGeVector3d lightDirection() const;
  void setLightDirection(const OdGeVector3d& lightvec);

  // Photometric properties

  PhysicalIntensityMethod physicalIntensityMethod() const;
  void setPhysicalIntensityMethod(PhysicalIntensityMethod physIntMethod);

  double physicalIntensity() const;
  void setPhysicalIntensity(double physIntensity);

  double illuminanceDistance() const;
  void setIlluminanceDistance(double illumDistance);

  LampColorType lampColorType() const;
  void setLampColorType(LampColorType colorType);

  double lampColorTemp() const;
  void setLampColorTemp(double colorTemp);

  LampColorPreset lampColorPreset() const;
  void setLampColorPreset(LampColorPreset colorPreset);

  OdGiColorRGB lampColorRGB() const;
  void setLampColorRGB(const OdGiColorRGB &color);
  OdCmColor resultingColor() const;

  OdString webFile() const;
  void setWebFile(const OdString &webFile);

  OdGeVector3d webRotation() const;
  void setWebRotation(const OdGeVector3d &rotation);

  bool hasTarget() const;
  void setHasTarget(bool bTarget);

  GlyphDisplayType glyphDisplay() const;
  void setGlyphDisplay(GlyphDisplayType type);
};

typedef OdSmartPtr<OdDbLight> OdDbLightPtr;

#include "TD_PackPop.h"

#endif // __OD_DB_LIGHT__
