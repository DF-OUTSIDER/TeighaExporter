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

#ifndef __OD_GI_VIEWPORT_TRAITS_DATA__
#define __OD_GI_VIEWPORT_TRAITS_DATA__

#include "TD_PackPush.h"

#include "Ge/GeVector3d.h"
#include "Gi/GiViewportTraits.h"
#include "Gi/GiLightTraits.h"

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiViewportTraitsData
{
  OdDbStub*                               m_backgroundId;
  bool                                    m_bDefLightOn;
  OdGiViewportTraits::DefaultLightingType m_lightType;
  OdGeVector3d                            m_userDefLightDir;
  double                                  m_defLightIntensity;
  OdCmEntityColor                         m_defLightColor;
  OdGiShadowParameters                    m_defShadowParams;
  OdCmEntityColor                         m_colorAmbient;
  double                                  m_dBrightness;
  double                                  m_dContrast;
  OdDbStub*                               m_renderEnvId;
  OdDbStub*                               m_renderSettings;
  OdDbStub*                               m_visualStyle;
  OdGiToneOperatorParametersPtr           m_pToneOperators;

public:

  OdGiViewportTraitsData()
    : m_backgroundId(NULL)
    , m_bDefLightOn(false)
    , m_lightType(OdGiViewportTraits::kOneDistantLight)
    , m_userDefLightDir(OdGeVector3d::kZAxis)
    , m_defLightIntensity(0.5)
    , m_defLightColor(255, 255, 255)
    , m_defShadowParams()
    , m_colorAmbient()
    , m_dBrightness(50.0)
    , m_dContrast(50.0)
    , m_renderEnvId(NULL)
    , m_renderSettings(NULL)
    , m_visualStyle(NULL)
  {
    m_defShadowParams.setShadowsOn(false);
    m_pToneOperators = OdGiToneOperatorParameters::createObject();
  }

  OdGiDrawable::DrawableType type() const { return OdGiDrawable::kViewport; }

  void setBackground(OdDbStub* backgroundId)
  {
    m_backgroundId = backgroundId;
  }
  OdDbStub* background() const
  {
    return m_backgroundId;
  }

  void setDefaultLightingOn(bool on)
  {
    m_bDefLightOn = on;
  }
  bool isDefaultLightingOn() const
  {
    return m_bDefLightOn;
  }

  void setDefaultLightingType(OdGiViewportTraits::DefaultLightingType typ)
  {
    m_lightType = typ;
  }
  OdGiViewportTraits::DefaultLightingType defaultLightingType() const
  {
    return m_lightType;
  }

  OdGeVector3d userDefinedLightDirection() const
  {
    return m_userDefLightDir;
  }
  void setUserDefinedLightDirection(const OdGeVector3d& lightDirection)
  {
    m_userDefLightDir = lightDirection;
  }

  double defaultLightingIntensity() const
  {
    return m_defLightIntensity;
  }
  void setDefaultLightingIntensity(double dIntensity)
  {
    m_defLightIntensity = dIntensity;
  }

  const OdCmEntityColor &defaultLightingColor() const
  {
    return m_defLightColor;
  }
  void setDefaultLightingColor(const OdCmEntityColor& color)
  {
    m_defLightColor = color;
  }

  const OdGiShadowParameters &defaultLightingShadowParameters() const
  {
    return m_defShadowParams;
  }
  void setDefaultLightingShadowParameters(const OdGiShadowParameters &params)
  {
    m_defShadowParams = params;
  }

  void setAmbientLightColor(const OdCmEntityColor& clr)
  {
    m_colorAmbient = clr;
  }
  const OdCmEntityColor &ambientLightColor() const
  {
    return m_colorAmbient;
  }

  void setBrightness(double brightness)
  {
    m_dBrightness = brightness;
  }
  double brightness() const
  {
    return m_dBrightness;
  }

  void setContrast(double contrast)
  {
    m_dContrast = contrast;
  }
  double contrast() const
  {
    return m_dContrast;
  }

  void setRenderEnvironment(OdDbStub* renderEnvId)
  {
    m_renderEnvId = renderEnvId;
  }
  OdDbStub* renderEnvironment() const
  {
    return m_renderEnvId;
  }

  void setRenderSettings(OdDbStub* renderSettingsId)
  {
    m_renderSettings = renderSettingsId;
  }
  OdDbStub* renderSettings() const
  {
    return m_renderSettings;
  }

  void setVisualStyle(OdDbStub* visualStyleId)
  {
    m_visualStyle = visualStyleId;
  }
  OdDbStub* visualStyle() const
  {
    return m_visualStyle;
  }

  void setToneOperatorParameters(const OdGiToneOperatorParameters &params)
  {
    if (params.isKindOf(OdGiPhotographicExposureParameters::desc()))
    {
      if (!m_pToneOperators->isKindOf(OdGiPhotographicExposureParameters::desc()))
        m_pToneOperators = OdGiPhotographicExposureParameters::createObject();
      *static_cast<OdGiPhotographicExposureParameters*>(m_pToneOperators.get()) = static_cast<const OdGiPhotographicExposureParameters&>(params);
    }
    else
    {
      if (m_pToneOperators->isKindOf(OdGiPhotographicExposureParameters::desc()))
        m_pToneOperators = OdGiToneOperatorParameters::createObject();
      *m_pToneOperators = params;
    }
  }
  void toneOperatorParameters(OdGiToneOperatorParameters &params) const
  {
    if (m_pToneOperators->isKindOf(OdGiPhotographicExposureParameters::desc()) &&
        params.isKindOf(OdGiPhotographicExposureParameters::desc()))
      static_cast<OdGiPhotographicExposureParameters&>(params) = *static_cast<const OdGiPhotographicExposureParameters*>(m_pToneOperators.get());
    else
      params = *m_pToneOperators;
  }
  const OdGiToneOperatorParameters &toneOperatorParameters() const
  {
    return *m_pToneOperators;
  }

  static bool compareToneOps(const OdGiToneOperatorParametersPtr &op1, const OdGiToneOperatorParametersPtr &op2)
  {
    const bool bPhoto1 = op1->isKindOf(OdGiPhotographicExposureParameters::desc());
    const bool bPhoto2 = op2->isKindOf(OdGiPhotographicExposureParameters::desc());
    if (bPhoto1 && bPhoto2)
      return *static_cast<const OdGiPhotographicExposureParameters*>(op1.get()) == *static_cast<const OdGiPhotographicExposureParameters*>(op2.get());
    else if (!bPhoto1 && !bPhoto2)
      return *op1 == *op2;
    return false;
  }

  bool operator ==(const OdGiViewportTraitsData &data2) const
  {
    return ((m_backgroundId == data2.m_backgroundId) &&
            (m_bDefLightOn == data2.m_bDefLightOn) &&
            (m_lightType == data2.m_lightType) &&
            m_userDefLightDir.isEqualTo(data2.m_userDefLightDir) &&
            (m_colorAmbient == data2.m_colorAmbient) &&
            OdEqual(m_dBrightness, data2.m_dBrightness) &&
            OdEqual(m_dContrast, data2.m_dContrast) &&
            (m_renderEnvId == data2.m_renderEnvId) &&
            (m_renderSettings == data2.m_renderSettings) &&
            (m_visualStyle == data2.m_visualStyle) &&
            compareToneOps(m_pToneOperators, data2.m_pToneOperators));
  }
  bool operator !=(const OdGiViewportTraitsData &data2) const
  {
    return !(*this == data2);
  }
};

#include "TD_PackPop.h"

#endif // __OD_GI_VIEWPORT_TRAITS_DATA__
