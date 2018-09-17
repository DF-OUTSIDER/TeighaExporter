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

#ifndef __OD_GI_LIGHT_TRAITS_DATA__
#define __OD_GI_LIGHT_TRAITS_DATA__

#include "TD_PackPush.h"

#include "Gi/GiLightTraits.h"

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiLightTraitsData
{
  bool                       m_bOn;
  double                     m_dIntensity;
  OdCmEntityColor            m_color;
  OdGiShadowParameters       m_shadowParameters;

public:
  enum LightType
  {
    kInvalidLight = -1,
    kPointLight   = OdGiDrawable::kPointLight,
    kSpotLight    = OdGiDrawable::kSpotLight,
    kDistantLight = OdGiDrawable::kDistantLight,
    kWebLight     = OdGiDrawable::kWebLight
  };

  static LightType drawableLightType(const OdGiDrawable *pDrawable);
protected:
  LightType m_type;

protected:
  OdGiLightTraitsData()
    : m_bOn(false)
    , m_dIntensity(0.0)
    , m_type(kInvalidLight)
  {}

public:
  bool isOn() const { return m_bOn; }
  void setOn(bool value) { m_bOn = value; }

  double intensity() const { return m_dIntensity; }
  void setIntensity(double dIntensity) { m_dIntensity = dIntensity; }

  OdCmEntityColor color() const { return m_color; }
  void setColor(const OdCmEntityColor& color) { m_color = color; }

  OdGiShadowParameters shadowParameters() const { return m_shadowParameters; }
  void setShadowParameters(const OdGiShadowParameters& params) { m_shadowParameters = params; }

  LightType type() const { return m_type; }

  void save(OdGsFiler * pFiler) const;
  void load(OdGsFiler * pFiler);

  static void deleteLightTraitsData(OdGiLightTraitsData* pLightTraits);
};

inline OdGiLightTraitsData::LightType OdGiLightTraitsData::drawableLightType(const OdGiDrawable *pDrawable)
{
  const LightType lightType = (pDrawable) ? (LightType)pDrawable->drawableType() : kInvalidLight;
  switch (lightType)
  {
    case kPointLight:
    case kSpotLight:
    case kDistantLight:
    case kWebLight:
      return lightType;
    default:
      return kInvalidLight;
  }
}

inline void OdGiLightTraitsData::save(OdGsFiler *pFiler) const {
  pFiler->wrBool(isOn());
  pFiler->wrDouble(intensity());
  pFiler->wrUInt32(color().color());
  m_shadowParameters.save(pFiler);
}

inline void OdGiLightTraitsData::load(OdGsFiler *pFiler) {
  setOn(pFiler->rdBool());
  setIntensity(pFiler->rdDouble());
  { OdCmEntityColor lightColor; lightColor.setColor(pFiler->rdUInt32()); setColor(lightColor); }
  m_shadowParameters.load(pFiler);
}

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiPointLightTraitsData : public OdGiLightTraitsData
{
  OdGePoint3d          m_position;
  OdGiLightAttenuation m_attenuation;
  // Photometric data
  double               m_dPhysIntensity;
  OdGiColorRGB         m_lampColor;
  bool                 m_bHasTarget;
  OdGePoint3d          m_target;
  bool                 m_bHemisphere;

public:
  OdGiPointLightTraitsData()
    : m_dPhysIntensity(1500.0)
    , m_lampColor(1.0, 1.0, 1.0)
    , m_bHasTarget(false)
    , m_bHemisphere(false)
  {
    m_type = kPointLight;
  }

  OdGePoint3d position() const { return m_position; }
  void setPosition(const OdGePoint3d& pos) { m_position = pos; }

  OdGiLightAttenuation attenuation() const { return m_attenuation; }
  void setAttenuation(const OdGiLightAttenuation& atten) { m_attenuation = atten; }

  double physicalIntensity() const { return m_dPhysIntensity; }
  void setPhysicalIntensity(double intensity) { m_dPhysIntensity = intensity; }

  OdGiColorRGB lampColor() const { return m_lampColor; }
  void setLampColor(const OdGiColorRGB& color) { m_lampColor = color; }

  bool hasTarget() const { return m_bHasTarget; }
  void setHasTarget(bool bTarget) { m_bHasTarget = bTarget; }

  OdGePoint3d targetLocation() const { return m_target; }
  void setTargetLocation(const OdGePoint3d &loc) { m_target = loc; }

  void setHemisphericalDistribution(bool bHemisphere) { m_bHemisphere = bHemisphere; }
  bool hemisphericalDistribution() const { return m_bHemisphere; }

  inline void save(OdGsFiler *pFiler) const {
    OdGiLightTraitsData::save(pFiler);
    pFiler->wrPoint3d(position());
    attenuation().save(pFiler);
    pFiler->wrDouble(physicalIntensity());
    pFiler->wrDouble(m_lampColor.red);
    pFiler->wrDouble(m_lampColor.green);
    pFiler->wrDouble(m_lampColor.blue);
    pFiler->wrBool(hasTarget());
    pFiler->wrPoint3d(targetLocation());
    pFiler->wrBool(hemisphericalDistribution());
  }

  inline void load(OdGsFiler *pFiler) {
    OdGiLightTraitsData::load(pFiler);
    pFiler->rdPoint3d(m_position);
    m_attenuation.load(pFiler);
    setPhysicalIntensity(pFiler->rdDouble());
    m_lampColor.red   = pFiler->rdDouble();
    m_lampColor.green = pFiler->rdDouble();
    m_lampColor.blue  = pFiler->rdDouble();
    setHasTarget(pFiler->rdBool());
    pFiler->rdPoint3d(m_target);
    setHemisphericalDistribution(pFiler->rdBool());
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiSpotLightTraitsData : public OdGiLightTraitsData
{
  OdGePoint3d          m_position, m_target;
  OdGiLightAttenuation m_attenuation;
  double               m_dHotspot, m_dFalloff;
  // Photometric data
  double               m_dPhysIntensity;
  OdGiColorRGB         m_lampColor;

public:
  OdGiSpotLightTraitsData()
    : m_dHotspot(0.0)
    , m_dFalloff(0.0)
    , m_dPhysIntensity(1500.0)
    , m_lampColor(1.0, 1.0, 1.0)
  {
    m_type = kSpotLight;
  }

  OdGePoint3d position() const { return m_position; }
  void setPosition(const OdGePoint3d& pos) { m_position = pos; }

  OdGePoint3d target() const { return m_target; }
  void setTarget(const OdGePoint3d& trg) { m_target = trg; }

  OdGiLightAttenuation attenuation() const { return m_attenuation; }
  void setAttenuation(const OdGiLightAttenuation& atten) { m_attenuation = atten; }

  bool setHotspotAndFalloff(double hotSpot, double falloff);
  double hotspot() const { return m_dHotspot; }
  double falloff() const { return m_dFalloff; }

  double physicalIntensity() const { return m_dPhysIntensity; }
  void setPhysicalIntensity(double intensity) { m_dPhysIntensity = intensity; }

  OdGiColorRGB lampColor() const { return m_lampColor; }
  void setLampColor(const OdGiColorRGB& color) { m_lampColor = color; }

  void save(OdGsFiler *pFiler) const {
    OdGiLightTraitsData::save(pFiler);
    pFiler->wrPoint3d(position());
    pFiler->wrPoint3d(target());
    attenuation().save(pFiler);
    pFiler->wrDouble(physicalIntensity());
    pFiler->wrDouble(hotspot());
    pFiler->wrDouble(falloff());
    pFiler->wrDouble(m_lampColor.red);
    pFiler->wrDouble(m_lampColor.green);
    pFiler->wrDouble(m_lampColor.blue);
  }

  void load(OdGsFiler *pFiler) {
    OdGiLightTraitsData::load(pFiler);
    pFiler->rdPoint3d(m_position);
    pFiler->rdPoint3d(m_target);
    m_attenuation.load(pFiler);
    setPhysicalIntensity(pFiler->rdDouble());
    m_dHotspot        = pFiler->rdDouble();
    m_dFalloff        = pFiler->rdDouble();
    m_lampColor.red   = pFiler->rdDouble();
    m_lampColor.green = pFiler->rdDouble();
    m_lampColor.blue  = pFiler->rdDouble();
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
inline bool OdGiSpotLightTraitsData::setHotspotAndFalloff(double hotspot, double falloff)
{ 
  if((hotspot > falloff) || 
     (hotspot < 0.0) || (falloff < 0.0) || 
     (OdaToDegree(hotspot) > 160.0) || (OdaToDegree(falloff) > 160.0))
    return false;
  m_dHotspot = hotspot;
  m_dFalloff = falloff; 
  return true;
}

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiDistantLightTraitsData : public OdGiLightTraitsData
{
  OdGeVector3d m_direction;
  bool         m_bIsSunlight;
  // Photometric data
  double       m_dPhysIntensity;
  OdGiColorRGB m_lampColor;
  //OdGiSkyParameters m_skyParams; // Unused

public:
  OdGiDistantLightTraitsData()
    : m_bIsSunlight(false)
    , m_dPhysIntensity(1500.0)
    , m_lampColor(1.0, 1.0, 1.0)
  {
    m_type = kDistantLight;
  }

  OdGeVector3d direction() const { return m_direction; }
  void setDirection(const OdGeVector3d& dir) { m_direction = dir; }

  bool isSunlight() const { return m_bIsSunlight; }
  void setSunlight(bool val) { m_bIsSunlight = val; }

  double physicalIntensity() const { return m_dPhysIntensity; }
  void setPhysicalIntensity(double intensity) { m_dPhysIntensity = intensity; }

  OdGiColorRGB lampColor() const { return m_lampColor; }
  void setLampColor(const OdGiColorRGB& color) { m_lampColor = color; }

  void skyParameters(OdGiSkyParameters & /*params*/) const { /*params = m_skyParams;*/ }
  void setSkyParameters(const OdGiSkyParameters & /*params*/) { /*m_skyParams = params;*/ }

  void save(OdGsFiler *pFiler) const {
    OdGiLightTraitsData::save(pFiler);
    pFiler->wrVector3d(direction());
    pFiler->wrBool(isSunlight());
    pFiler->wrDouble(physicalIntensity());
    pFiler->wrDouble(m_lampColor.red);
    pFiler->wrDouble(m_lampColor.green);
    pFiler->wrDouble(m_lampColor.blue);
  }

  void load(OdGsFiler *pFiler) {
    OdGiLightTraitsData::load(pFiler);
    pFiler->rdVector3d(m_direction);
    setSunlight(pFiler->rdBool());
    setPhysicalIntensity(pFiler->rdDouble());
    m_lampColor.red   = pFiler->rdDouble();
    m_lampColor.green = pFiler->rdDouble();
    m_lampColor.blue  = pFiler->rdDouble();
  }
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiWebLightTraitsData : public OdGiPointLightTraitsData
{
  OdString                        m_webFile;
  OdGeVector3d                    m_webRotate;
  double                          m_dFlux;
  OdGiWebLightTraits::WebFileType m_webFileType;
  OdGiWebLightTraits::WebSymmetry m_webSymmetry;
  bool                            m_bHorzAng90to270;

public:
  OdGiWebLightTraitsData()
    : OdGiPointLightTraitsData()
    , m_dFlux(0.0)
    , m_webFileType(OdGiWebLightTraits::kTypeA)
    , m_webSymmetry(OdGiWebLightTraits::kNoSymmetry)
    , m_bHorzAng90to270(false)
  {
    m_type = kWebLight;
  }

  const OdString &webFile() const { return m_webFile; }
  void setWebFile(const OdString &fileName) { m_webFile = fileName; }

  const OdGeVector3d& webRotation() const { return m_webRotate; }
  void setWebRotation(const OdGeVector3d& rot) { m_webRotate = rot; }

  double webFlux() const { return m_dFlux; }
  void setWebFlux(double flux) { m_dFlux = flux; }

  OdGiWebLightTraits::WebFileType webFileType() const { return m_webFileType; }
  void setWebFileType(OdGiWebLightTraits::WebFileType type) { m_webFileType = type; }

  OdGiWebLightTraits::WebSymmetry webSymmetry() const { return m_webSymmetry; }
  void setWebSymmetry(OdGiWebLightTraits::WebSymmetry sym) { m_webSymmetry = sym; }

  bool webHorzAng90to270() const { return m_bHorzAng90to270; }
  void setWebHorzAng90to270(bool bHA) { m_bHorzAng90to270 = bHA; }

  inline void save(OdGsFiler *pFiler) const {
    OdGiPointLightTraitsData::save(pFiler);
    pFiler->wrString(webFile());
    pFiler->wrVector3d(webRotation());
    pFiler->wrDouble(webFlux());
    pFiler->wrUInt8((OdUInt8)webFileType());
    pFiler->wrUInt8((OdUInt8)webSymmetry());
    pFiler->wrBool(webHorzAng90to270());
  }

  inline void load(OdGsFiler *pFiler) {
    OdGiPointLightTraitsData::load(pFiler);
    pFiler->rdString(m_webFile);
    pFiler->rdVector3d(m_webRotate);
    setWebFlux(pFiler->rdDouble());
    setWebFileType((OdGiWebLightTraits::WebFileType)pFiler->rdUInt8());
    setWebSymmetry((OdGiWebLightTraits::WebSymmetry)pFiler->rdUInt8());
    setWebHorzAng90to270(pFiler->rdBool());
  }
};

inline void OdGiLightTraitsData::deleteLightTraitsData(OdGiLightTraitsData* pLightTraits)
{
  switch(pLightTraits->type())
  {
  case OdGiLightTraitsData::kDistantLight:
    delete static_cast<OdGiDistantLightTraitsData*>(pLightTraits);
    break;
  case OdGiLightTraitsData::kPointLight:
    delete static_cast<OdGiPointLightTraitsData*>(pLightTraits);
    break;
  case OdGiLightTraitsData::kSpotLight:
    delete static_cast<OdGiSpotLightTraitsData*>(pLightTraits);
    break;
  case OdGiLightTraitsData::kWebLight:
    delete static_cast<OdGiWebLightTraitsData*>(pLightTraits);
    break;
  default:
    {
      ODA_FAIL_ONCE();
    }
    break;
  }
}

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiLightTraitsVpDepData : public OdGiLightTraitsData
{
  OdUInt32    m_nViewportId;
  OdDbStub*   m_pViewportObjectId;
  bool        m_bVpDepOn;
  double      m_dVpDepDimming;

public:
  OdGiLightTraitsVpDepData() : OdGiLightTraitsData(),
    m_nViewportId( 0 ),
    m_pViewportObjectId( NULL ),
    m_bVpDepOn( false ),
    m_dVpDepDimming( 0.0 )
  {
  }

  OdUInt32 viewportId() const { return m_nViewportId; }
  void setViewportId( OdUInt32 id ) { m_nViewportId = id; }

  OdDbStub* viewportObjectId() const { return m_pViewportObjectId; }
  void setViewportObjectId( OdDbStub* id ) { m_pViewportObjectId = id; }

  bool vpDepOn() const { return m_bVpDepOn; }
  void setVpDepOn( bool on ) { m_bVpDepOn = on; }

  double vpDepDimming() const { return m_dVpDepDimming; }
  void setVpDepDimming( double dimming ) { m_dVpDepDimming = dimming; }

  inline void save(OdGsFiler *pFiler) const {
    OdGiLightTraitsData::save(pFiler);
    pFiler->wrBool( vpDepOn() );
    pFiler->wrDouble( vpDepDimming() );
    pFiler->wrUInt32( viewportId() );
    pFiler->wrHandle( viewportObjectId() );
  }

  inline void load(OdGsFiler *pFiler) {
    OdGiLightTraitsData::load(pFiler);
    setVpDepOn( pFiler->rdBool() );
    setVpDepDimming( pFiler->rdDouble() );
    setViewportId( pFiler->rdUInt32() );
    setViewportObjectId( pFiler->rdHandle() );
  }
};

#include "TD_PackPop.h"

#endif // __OD_GI_LIGHT_TRAITS_DATA__
