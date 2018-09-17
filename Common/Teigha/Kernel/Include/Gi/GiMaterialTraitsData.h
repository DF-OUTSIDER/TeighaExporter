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

#ifndef _ODGIMATERIALTRAITSDATA_INCLUDED_
#define _ODGIMATERIALTRAITSDATA_INCLUDED_

#include "Gi/GiMaterial.h"

class OdDbStub;

#include "TD_PackPush.h"

/** \details

    <group !!RECORDS_TD_APIRef>
*/
struct OdGiMaterialTraitsData
{
protected:
  OdGiMaterialColor                          m_ambientColor;
  OdGiMaterialColor                          m_diffuseColor;
  OdGiMaterialMap                            m_diffuseMap;
  OdGiMaterialColor                          m_specularColor;
  OdGiMaterialMap                            m_specularMap;
  double                                     m_glossFactor;
  OdGiMaterialMap                            m_reflectionMap;
  double                                     m_opacity;
  OdGiMaterialMap                            m_opacityMap;
  OdGiMaterialMap                            m_bumpMap;
  double                                     m_refractionIndex;
  OdGiMaterialMap                            m_refractionMap;
  double                                     m_translucenceVal;
  double                                     m_selfIllumVal;
  double                                     m_reflectivityVal;
  OdGiMaterialTraits::IlluminationModel      m_illumModel;
  OdGiMaterialTraits::ChannelFlags           m_channelFlags;
  OdGiMaterialTraits::Mode                   m_mode;
  // Extended material properties
  double                                     m_bleedScale;
  double                                     m_indirectBumpScale;
  double                                     m_reflectScale;
  double                                     m_transmitScale;
  bool                                       m_twoSided;
  OdGiMaterialTraits::LuminanceMode          m_lumiMode;
  double                                     m_lumiVal;
  OdGiMaterialMap                            m_normalMap;
  OdGiMaterialTraits::NormalMapMethod        m_normalMethod;
  double                                     m_normalStrength;
  OdGiMaterialTraits::GlobalIlluminationMode m_globIllum;
  OdGiMaterialTraits::FinalGatherMode        m_fgMode;
  // BIM extensions
  OdGiMaterialColor                          m_emissionColor;
  OdGiMaterialMap                            m_emissionMap;
  OdGiMaterialColor                          m_tintColor;
  OdGiMaterialColor                          m_shadingAmbientColor;
  OdGiMaterialColor                          m_shadingDiffuseColor;
  OdGiMaterialColor                          m_shadingSpeclrColor;
  double                                     m_shadingOpacity;
  OdGiMaterialTraits::DiffuseColorMode       m_diffuseColorMode;

public:
  OdGiMaterialTraitsData()
    : m_ambientColor()
    , m_diffuseColor()
    , m_diffuseMap()
    , m_specularColor()
    , m_specularMap()
    , m_glossFactor(0.0)
    , m_reflectionMap()
    , m_opacity(1.0)
    , m_opacityMap()
    , m_bumpMap()
    , m_refractionIndex(0.0)
    , m_refractionMap()
    , m_translucenceVal(0.0)
    , m_selfIllumVal(0.0)
    , m_reflectivityVal(0.0)
    , m_illumModel(OdGiMaterialTraits::kBlinnShader)
    , m_channelFlags(OdGiMaterialTraits::kUseDiffuse)
    , m_mode(OdGiMaterialTraits::kRealistic)
    // Extended material properties
    , m_bleedScale(100.0)
    , m_indirectBumpScale(100.0)
    , m_reflectScale(100.0)
    , m_transmitScale(100.0)
    , m_twoSided(true)
    , m_lumiMode(OdGiMaterialTraits::kSelfIllumination)
    , m_lumiVal(0.0)
    , m_normalMap()
    , m_normalMethod(OdGiMaterialTraits::kTangentSpace)
    , m_normalStrength(1.0)
    , m_globIllum(OdGiMaterialTraits::kGlobalIlluminationCastAndReceive)
    , m_fgMode(OdGiMaterialTraits::kFinalGatherCastAndReceive)
    // BIM extensions
    , m_emissionColor(OdCmEntityColor(0, 0, 0))
    , m_emissionMap()
    , m_tintColor(OdCmEntityColor(255, 255, 255))
    , m_shadingAmbientColor()
    , m_shadingDiffuseColor()
    , m_shadingSpeclrColor()
    , m_shadingOpacity(1.0)
    , m_diffuseColorMode(OdGiMaterialTraits::kDiffuseRealisticColor)
  {
  }
  void operator =(const OdGiMaterialTraitsData& other)
  {
    m_ambientColor        = other.m_ambientColor;
    m_diffuseColor        = other.m_diffuseColor;
    m_diffuseMap          = other.m_diffuseMap;
    m_specularColor       = other.m_specularColor;
    m_specularMap         = other.m_specularMap;
    m_glossFactor         = other.m_glossFactor;
    m_reflectionMap       = other.m_reflectionMap;
    m_opacity             = other.m_opacity;
    m_opacityMap          = other.m_opacityMap;
    m_bumpMap             = other.m_bumpMap;
    m_refractionIndex     = other.m_refractionIndex;
    m_refractionMap       = other.m_refractionMap;
    m_translucenceVal     = other.m_translucenceVal;
    m_selfIllumVal        = other.m_selfIllumVal;
    m_reflectivityVal     = other.m_reflectivityVal;
    m_illumModel          = other.m_illumModel;
    m_channelFlags        = other.m_channelFlags;
    m_mode                = other.m_mode;
    // Extended material properties
    m_bleedScale          = other.m_bleedScale;
    m_indirectBumpScale   = other.m_indirectBumpScale;
    m_reflectScale        = other.m_reflectScale;
    m_transmitScale       = other.m_transmitScale;
    m_twoSided            = other.m_twoSided;
    m_lumiMode            = other.m_lumiMode;
    m_lumiVal             = other.m_lumiVal;
    m_normalMap           = other.m_normalMap;
    m_normalMethod        = other.m_normalMethod;
    m_normalStrength      = other.m_normalStrength;
    m_globIllum           = other.m_globIllum;
    m_fgMode              = other.m_fgMode;
    // BIM extensions
    m_emissionColor       = other.m_emissionColor;
    m_emissionMap         = other.m_emissionMap;
    m_tintColor           = other.m_tintColor;
    m_shadingAmbientColor = other.m_shadingAmbientColor;
    m_shadingDiffuseColor = other.m_shadingDiffuseColor;
    m_shadingSpeclrColor  = other.m_shadingSpeclrColor;
    m_shadingOpacity      = other.m_shadingOpacity;
    m_diffuseColorMode    = other.m_diffuseColorMode;
  }
  bool operator ==(const OdGiMaterialTraitsData& other) const
  {
    if (m_ambientColor        != other.m_ambientColor)        return false;
    if (m_diffuseColor        != other.m_diffuseColor)        return false;
    if (m_diffuseMap          != other.m_diffuseMap)          return false;
    if (m_specularColor       != other.m_specularColor)       return false;
    if (m_specularMap         != other.m_specularMap)         return false;
    if (m_glossFactor         != other.m_glossFactor)         return false;
    if (m_reflectionMap       != other.m_reflectionMap)       return false;
    if (m_opacity             != other.m_opacity)             return false;
    if (m_opacityMap          != other.m_opacityMap)          return false;
    if (m_bumpMap             != other.m_bumpMap)             return false;
    if (m_refractionIndex     != other.m_refractionIndex)     return false;
    if (m_refractionMap       != other.m_refractionMap)       return false;
    if (m_translucenceVal     != other.m_translucenceVal)     return false;
    if (m_selfIllumVal        != other.m_selfIllumVal)        return false;
    if (m_reflectivityVal     != other.m_reflectivityVal)     return false;
    if (m_illumModel          != other.m_illumModel)          return false;
    if (m_channelFlags        != other.m_channelFlags)        return false;
    if (m_mode                != other.m_mode)                return false;
    // Extended material properties
    if (m_bleedScale          != other.m_bleedScale)          return false;
    if (m_indirectBumpScale   != other.m_indirectBumpScale)   return false;
    if (m_reflectScale        != other.m_reflectScale)        return false;
    if (m_transmitScale       != other.m_transmitScale)       return false;
    if (m_twoSided            != other.m_twoSided)            return false;
    if (m_lumiMode            != other.m_lumiMode)            return false;
    if (m_lumiVal             != other.m_lumiVal)             return false;
    if (m_normalMap           != other.m_normalMap)           return false;
    if (m_normalMethod        != other.m_normalMethod)        return false;
    if (m_normalStrength      != other.m_normalStrength)      return false;
    if (m_globIllum           != other.m_globIllum)           return false;
    if (m_fgMode              != other.m_fgMode)              return false;
    // BIM extensions
    if (m_emissionColor       != other.m_emissionColor)       return false;
    if (m_emissionMap         != other.m_emissionMap)         return false;
    if (m_tintColor           != other.m_tintColor)           return false;
    if (m_shadingAmbientColor != other.m_shadingAmbientColor) return false;
    if (m_shadingDiffuseColor != other.m_shadingDiffuseColor) return false;
    if (m_shadingSpeclrColor  != other.m_shadingSpeclrColor)  return false;
    if (m_shadingOpacity      != other.m_shadingOpacity)      return false;
    if (m_diffuseColorMode    != other.m_diffuseColorMode)    return false;
    return true;
  }
  bool operator !=(const OdGiMaterialTraitsData& other) const
  {
    if (m_ambientColor        != other.m_ambientColor)        return true;
    if (m_diffuseColor        != other.m_diffuseColor)        return true;
    if (m_diffuseMap          != other.m_diffuseMap)          return true;
    if (m_specularColor       != other.m_specularColor)       return true;
    if (m_specularMap         != other.m_specularMap)         return true;
    if (m_glossFactor         != other.m_glossFactor)         return true;
    if (m_reflectionMap       != other.m_reflectionMap)       return true;
    if (m_opacity             != other.m_opacity)             return true;
    if (m_opacityMap          != other.m_opacityMap)          return true;
    if (m_bumpMap             != other.m_bumpMap)             return true;
    if (m_refractionIndex     != other.m_refractionIndex)     return true;
    if (m_refractionMap       != other.m_refractionMap)       return true;
    if (m_translucenceVal     != other.m_translucenceVal)     return true;
    if (m_selfIllumVal        != other.m_selfIllumVal)        return true;
    if (m_reflectivityVal     != other.m_reflectivityVal)     return true;
    if (m_illumModel          != other.m_illumModel)          return true;
    if (m_channelFlags        != other.m_channelFlags)        return true;
    if (m_mode                != other.m_mode)                return true;    
    // Extended material properties
    if (m_bleedScale          != other.m_bleedScale)          return true;
    if (m_indirectBumpScale   != other.m_indirectBumpScale)   return true;
    if (m_reflectScale        != other.m_reflectScale)        return true;
    if (m_transmitScale       != other.m_transmitScale)       return true;
    if (m_twoSided            != other.m_twoSided)            return true;
    if (m_lumiMode            != other.m_lumiMode)            return true;
    if (m_lumiVal             != other.m_lumiVal)             return true;
    if (m_normalMap           != other.m_normalMap)           return true;
    if (m_normalMethod        != other.m_normalMethod)        return true;
    if (m_normalStrength      != other.m_normalStrength)      return true;
    if (m_globIllum           != other.m_globIllum)           return true;
    if (m_fgMode              != other.m_fgMode)              return true;
    // BIM extensions
    if (m_emissionColor       != other.m_emissionColor)       return true;
    if (m_emissionMap         != other.m_emissionMap)         return true;
    if (m_tintColor           != other.m_tintColor)           return true;
    if (m_shadingAmbientColor != other.m_shadingAmbientColor) return true;
    if (m_shadingDiffuseColor != other.m_shadingDiffuseColor) return true;
    if (m_shadingSpeclrColor  != other.m_shadingSpeclrColor)  return true;
    if (m_shadingOpacity      != other.m_shadingOpacity)      return true;
    if (m_diffuseColorMode    != other.m_diffuseColorMode)    return true;
    return false;
  }

  void ambient(OdGiMaterialColor& ambientColor) const
  {
    ambientColor = m_ambientColor;
  }
  void diffuse(OdGiMaterialColor& diffuseColor, OdGiMaterialMap& diffuseMap) const
  {
    diffuseColor = m_diffuseColor;
    diffuseMap = m_diffuseMap;
  }
  void specular(OdGiMaterialColor& specularColor, OdGiMaterialMap& specularMap, double& glossFactor) const
  {
    specularColor = m_specularColor;
    specularMap = m_specularMap;
    glossFactor = m_glossFactor;
  }
  void reflection(OdGiMaterialMap& reflectionMap) const
  {
    reflectionMap = m_reflectionMap;
  }
  void opacity(double& opacity, OdGiMaterialMap& opacityMap) const
  {
    opacity = m_opacity;
    opacityMap = m_opacityMap;
  }
  void bump(OdGiMaterialMap& bumpMap) const
  {
    bumpMap = m_bumpMap;
  }
  void refraction(double& refractionIndex, OdGiMaterialMap& refractionMap) const
  {
    refractionIndex = m_refractionIndex;
    refractionMap = m_refractionMap;
  }
  double translucence() const
  {
    return m_translucenceVal;
  }
  double selfIllumination() const
  {
    return m_selfIllumVal;
  }
  double reflectivity() const
  {
    return m_reflectivityVal;
  }
  OdGiMaterialTraits::IlluminationModel illuminationModel() const
  {
    return m_illumModel;
  }
  OdGiMaterialTraits::ChannelFlags channelFlags() const
  {
    return m_channelFlags;
  }
  OdGiMaterialTraits::Mode mode() const
  {
    return m_mode;
  }

  void setAmbient(const OdGiMaterialColor& ambientColor)
  {
    m_ambientColor = ambientColor;
  }
  void setDiffuse(const OdGiMaterialColor& diffuseColor, const OdGiMaterialMap& diffuseMap)
  {
    m_diffuseColor = diffuseColor;
    m_diffuseMap = diffuseMap;
  }
  void setSpecular(const OdGiMaterialColor& specularColor, const OdGiMaterialMap& specularMap, double glossFactor)
  {
    m_specularColor = specularColor;
    m_specularMap = specularMap;
    m_glossFactor = glossFactor;
  }
  void setReflection(const OdGiMaterialMap& reflectionMap)
  {
    m_reflectionMap = reflectionMap;
  }
  void setOpacity(double opacity, const OdGiMaterialMap& opacityMap)
  {
    m_opacity = opacity;
    m_opacityMap = opacityMap;
  }
  void setBump(const OdGiMaterialMap& bumpMap)
  {
    m_bumpMap = bumpMap;
  }
  void setRefraction(double refractionIndex, const OdGiMaterialMap& refractionMap)
  {
    m_refractionIndex = refractionIndex;
    m_refractionMap = refractionMap;
  }
  void setTranslucence(double value)
  {
    m_translucenceVal = value;
  }
  void setSelfIllumination(double value)
  {
    m_selfIllumVal = value;
  }
  void setReflectivity(double value)
  {
    m_reflectivityVal = value;
  }
  void setIlluminationModel(OdGiMaterialTraits::IlluminationModel model)
  {
    m_illumModel = model;
  }
  void setChannelFlags(OdGiMaterialTraits::ChannelFlags flags)
  {
    m_channelFlags = flags;
  }
  void setMode(OdGiMaterialTraits::Mode value)
  {
    m_mode = value;
  }

  // Extended material properties

  void setColorBleedScale(double scale)
  {
    m_bleedScale = scale;
  }
  double colorBleedScale() const
  {
    return m_bleedScale;
  }

  void setIndirectBumpScale(double scale)
  {
    m_indirectBumpScale = scale;
  }
  double indirectBumpScale() const
  {
    return m_indirectBumpScale;
  }

  void setReflectanceScale(double scale)
  {
    m_reflectScale = scale;
  }
  double reflectanceScale() const
  {
    return m_reflectScale;
  }

  void setTransmittanceScale(double scale)
  {
    m_transmitScale = scale;
  }
  double transmittanceScale() const
  {
    return m_transmitScale;
  }

  void setTwoSided(bool flag)
  {
    m_twoSided = flag;
  }
  bool twoSided() const
  {
    return m_twoSided;
  }

  void setLuminanceMode(OdGiMaterialTraits::LuminanceMode mode)
  {
    m_lumiMode = mode;
  }
  OdGiMaterialTraits::LuminanceMode luminanceMode() const
  {
    return m_lumiMode;
  }

  void setLuminance(double value)
  {
    m_lumiVal = value;
  }
  double luminance() const
  {
    return m_lumiVal;
  }

  void setNormalMap(const OdGiMaterialMap &normalMap, OdGiMaterialTraits::NormalMapMethod method, double strength)
  {
    m_normalMap = normalMap;
    m_normalMethod = method;
    m_normalStrength = strength;
  }
  void normalMap(OdGiMaterialMap &normalMap, OdGiMaterialTraits::NormalMapMethod &method, double &strength) const
  {
    normalMap = m_normalMap;
    method = m_normalMethod;
    strength = m_normalStrength;
  }

  void setGlobalIllumination(OdGiMaterialTraits::GlobalIlluminationMode mode)
  {
    m_globIllum = mode;
  }
  OdGiMaterialTraits::GlobalIlluminationMode globalIllumination() const
  {
    return m_globIllum;
  }

  void setFinalGather(OdGiMaterialTraits::FinalGatherMode mode)
  {
    m_fgMode = mode;
  }
  OdGiMaterialTraits::FinalGatherMode finalGather() const
  {
    return m_fgMode;
  }

  // BIM extensions

  void setEmission(const OdGiMaterialColor& emissionColor, const OdGiMaterialMap& emissionMap)
  {
    m_emissionColor = emissionColor;
    m_emissionMap = emissionMap;
  }
  void emission(OdGiMaterialColor& emissionColor, OdGiMaterialMap& emissionMap) const
  {
    emissionMap = m_emissionMap;
    emissionColor = m_emissionColor;
  }

  void setTint(const OdGiMaterialColor& tintColor)
  {
    m_tintColor = tintColor;
  }
  void tint(OdGiMaterialColor& tintColor) const
  {
    tintColor = m_tintColor;
  }

  void setShadingAmbient(const OdGiMaterialColor& ambientColor)
  {
    m_shadingAmbientColor = ambientColor;
  }
  void shadingAmbient(OdGiMaterialColor& ambientColor) const
  {
    ambientColor = m_shadingAmbientColor;
  }

  void setShadingDiffuse(const OdGiMaterialColor& diffuseColor)
  {
    m_shadingDiffuseColor = diffuseColor;
  }
  void shadingDiffuse(OdGiMaterialColor& diffuseColor) const
  {
    diffuseColor = m_shadingDiffuseColor;
  }

  void setShadingSpecular(const OdGiMaterialColor& specularColor)
  {
    m_shadingSpeclrColor = specularColor;
  }
  void shadingSpecular(OdGiMaterialColor& specularColor) const
  {
    specularColor = m_shadingSpeclrColor;
  }

  void setShadingOpacity(double opacityPercentage)
  {
    m_shadingOpacity = opacityPercentage;
  }
  void shadingOpacity(double &opacityPercentage) const
  {
    opacityPercentage = m_shadingOpacity;
  }

  void setDiffuseColorMode(OdGiMaterialTraits::DiffuseColorMode diffuseColorMode)
  {
    m_diffuseColorMode = diffuseColorMode;
  }
  OdGiMaterialTraits::DiffuseColorMode diffuseColorMode() const
  {
    return m_diffuseColorMode;
  }
};

/** \details
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTraitsTaker : public OdGiMaterialTraits
{
protected:
  OdGiMaterialTraitsData m_data;
public:
  //ODRX_DECLARE_MEMBERS(OdGiMaterialTraitsTaker);
  ODRX_USING_HEAP_OPERATORS(OdGiMaterialTraits);

  void addRef() { }
  void release() { }

  virtual void ambient(OdGiMaterialColor& ambientColor) const
  {
    m_data.ambient(ambientColor);
  }
  virtual void diffuse(OdGiMaterialColor& diffuseColor, OdGiMaterialMap& diffuseMap) const
  {
    m_data.diffuse(diffuseColor, diffuseMap);
  }
  virtual void specular(OdGiMaterialColor& specularColor, OdGiMaterialMap& specularMap, double& glossFactor) const
  {
    m_data.specular(specularColor, specularMap, glossFactor);
  }
  virtual void reflection(OdGiMaterialMap& reflectionMap) const
  {
    m_data.reflection(reflectionMap);
  }
  virtual void opacity(double& opacityPercentage, OdGiMaterialMap& opacityMap) const
  {
    m_data.opacity(opacityPercentage, opacityMap);
  }
  virtual void bump(OdGiMaterialMap& bumpMap) const
  {
    m_data.bump(bumpMap);
  }
  virtual void refraction(double& refractionIndex, OdGiMaterialMap& refractionMap) const
  {
    m_data.refraction(refractionIndex, refractionMap);
  }
  virtual double translucence() const
  {
    return m_data.translucence();
  }
  virtual double selfIllumination() const
  {
    return m_data.selfIllumination();
  }
  virtual double reflectivity() const
  {
    return m_data.reflectivity();
  }
  virtual IlluminationModel illuminationModel() const
  {
    return m_data.illuminationModel();
  }
  virtual ChannelFlags channelFlags() const
  {
    return m_data.channelFlags();
  }
  virtual Mode mode() const
  {
    return m_data.mode();
  }

  virtual void setAmbient(const OdGiMaterialColor& ambientColor)
  {
    m_data.setAmbient(ambientColor);
  }
  virtual void setDiffuse(const OdGiMaterialColor& diffuseColor, const OdGiMaterialMap& diffuseMap)
  {
    m_data.setDiffuse(diffuseColor, diffuseMap);
  }
  virtual void setSpecular(const OdGiMaterialColor& specularColor, const OdGiMaterialMap& specularMap, double glossFactor)
  {
    m_data.setSpecular(specularColor, specularMap, glossFactor);
  }
  virtual void setReflection(const OdGiMaterialMap& reflectionMap)
  {
    m_data.setReflection(reflectionMap);
  }
  virtual void setOpacity(double opacityPercentage, const OdGiMaterialMap& opacityMap)
  {
    m_data.setOpacity(opacityPercentage, opacityMap);
  }
  virtual void setBump(const OdGiMaterialMap& bumpMap)
  {
    m_data.setBump(bumpMap);
  }
  virtual void setRefraction(double refractionIndex, const OdGiMaterialMap& refractionMap)
  {
    m_data.setRefraction(refractionIndex, refractionMap);
  }
  virtual void setTranslucence(double value)
  {
    m_data.setTranslucence(value);
  }
  virtual void setSelfIllumination(double value)
  {
    m_data.setSelfIllumination(value);
  }
  virtual void setReflectivity(double value)
  {
    m_data.setReflectivity(value);
  }
  virtual void setIlluminationModel(IlluminationModel model)
  {
    m_data.setIlluminationModel(model);
  }
  virtual void setChannelFlags(ChannelFlags flags)
  {
    m_data.setChannelFlags(flags);
  }
  virtual void setMode(Mode value)
  {
    m_data.setMode(value);
  }

  virtual void setColorBleedScale(double scale)
  {
    m_data.setColorBleedScale(scale);
  }
  virtual double colorBleedScale() const
  {
    return m_data.colorBleedScale();
  }

  virtual void setIndirectBumpScale(double scale)
  {
    m_data.setIndirectBumpScale(scale);
  }
  virtual double indirectBumpScale() const
  {
    return m_data.indirectBumpScale();
  }

  virtual void setReflectanceScale(double scale)
  {
    m_data.setReflectanceScale(scale);
  }
  virtual double reflectanceScale() const
  {
    return m_data.reflectanceScale();
  }

  virtual void setTransmittanceScale(double scale)
  {
    m_data.setTransmittanceScale(scale);
  }
  virtual double transmittanceScale() const
  {
    return m_data.transmittanceScale();
  }

  virtual void setTwoSided(bool flag)
  {
    m_data.setTwoSided(flag);
  }
  virtual bool twoSided() const
  {
    return m_data.twoSided();
  }

  virtual void setLuminanceMode(LuminanceMode mode)
  {
    m_data.setLuminanceMode(mode);
  }
  virtual LuminanceMode luminanceMode() const
  {
    return m_data.luminanceMode();
  }

  virtual void setLuminance(double value)
  {
    m_data.setLuminance(value);
  }
  virtual double luminance() const
  {
    return m_data.luminance();
  }

  virtual void setNormalMap(const OdGiMaterialMap &normalMap, NormalMapMethod method, double strength)
  {
    m_data.setNormalMap(normalMap, method, strength);
  }
  virtual void normalMap(OdGiMaterialMap &normalMap, NormalMapMethod &method, double &strength) const
  {
    m_data.normalMap(normalMap, method, strength);
  }

  virtual void setGlobalIllumination(GlobalIlluminationMode mode)
  {
    m_data.setGlobalIllumination(mode);
  }
  virtual GlobalIlluminationMode globalIllumination() const
  {
    return m_data.globalIllumination();
  }

  virtual void setFinalGather(FinalGatherMode mode)
  {
    m_data.setFinalGather(mode);
  }
  virtual FinalGatherMode finalGather() const
  {
    return m_data.finalGather();
  }

  virtual void setEmission(const OdGiMaterialColor& emissionColor, const OdGiMaterialMap& emissionMap)
  {
    m_data.setEmission(emissionColor, emissionMap);
  }
  virtual void emission(OdGiMaterialColor& emissionColor, OdGiMaterialMap& emissionMap) const
  {
    m_data.emission(emissionColor, emissionMap);
  }

  virtual void setTint(const OdGiMaterialColor& tintColor)
  {
    m_data.setTint(tintColor);
  }
  virtual void tint(OdGiMaterialColor& tintColor) const
  {
    m_data.tint(tintColor);
  }

  virtual void setShadingAmbient(const OdGiMaterialColor& ambientColor)
  {
    m_data.setShadingAmbient(ambientColor);
  }
  virtual void shadingAmbient(OdGiMaterialColor& ambientColor) const
  {
    m_data.shadingAmbient(ambientColor);
  }

  virtual void setShadingDiffuse(const OdGiMaterialColor& diffuseColor)
  {
    m_data.setShadingDiffuse(diffuseColor);
  }
  virtual void shadingDiffuse(OdGiMaterialColor& diffuseColor) const
  {
    m_data.shadingDiffuse(diffuseColor);
  }

  virtual void setShadingSpecular(const OdGiMaterialColor& specularColor)
  {
    m_data.setShadingSpecular(specularColor);
  }
  virtual void shadingSpecular(OdGiMaterialColor& specularColor) const
  {
    m_data.shadingSpecular(specularColor);
  }

  virtual void setShadingOpacity(double opacityPercentage)
  {
    m_data.setShadingOpacity(opacityPercentage);
  }
  virtual void shadingOpacity(double &opacityPercentage) const
  {
    m_data.shadingOpacity(opacityPercentage);
  }

  virtual void setDiffuseColorMode(DiffuseColorMode diffuseColorMode)
  {
    m_data.setDiffuseColorMode(diffuseColorMode);
  }
  virtual DiffuseColorMode diffuseColorMode() const
  {
    return m_data.diffuseColorMode();
  }

  void getData(OdGiMaterialTraitsData& data) const { data = m_data; }
  const OdGiMaterialTraitsData &getData() const { return m_data; }

  void setData(const OdGiMaterialTraitsData& data) { m_data = data; }
};

typedef OdSmartPtr<OdGiMaterialTraitsTaker> OdGiMaterialTraitsTakerPtr;

/** \details
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTraits_Stub : public OdGiMaterialTraits
{
public:
  ODRX_USING_HEAP_OPERATORS(OdGiMaterialTraits);

  OdGiMaterialTraits_Stub() { }

  void addRef() { }
  void release() { }

  void ambient(OdGiMaterialColor& /*ambientColor*/) const { }
  void diffuse(OdGiMaterialColor& /*diffuseColor*/, OdGiMaterialMap& /*diffuseMap*/) const { }
  void specular(OdGiMaterialColor& /*specularColor*/, OdGiMaterialMap& /*specularMap*/, double& /*glossFactor*/) const { }
  void reflection(OdGiMaterialMap& /*reflectionMap*/) const { }
  void opacity(double& /*opacityPercentage*/, OdGiMaterialMap& /*opacityMap*/) const { }
  void bump(OdGiMaterialMap& /*bumpMap*/) const { }
  void refraction(double& /*refractionIndex*/, OdGiMaterialMap& /*refractionMap*/) const { }
  double translucence() const { return 0.0; }
  double selfIllumination() const { return 0.0; }
  double reflectivity() const { return 0.0; }
  IlluminationModel illuminationModel() const { return kBlinnShader; }
  ChannelFlags channelFlags() const { return kUseAll; }
  Mode mode() const { return kRealistic; }

  void setAmbient(const OdGiMaterialColor& /*ambientColor*/) { }
  void setDiffuse(const OdGiMaterialColor& /*diffuseColor*/, const OdGiMaterialMap& /*diffuseMap*/) { }
  void setSpecular(const OdGiMaterialColor& /*specularColor*/, const OdGiMaterialMap& /*specularMap*/, double /*glossFactor*/) { }
  void setReflection(const OdGiMaterialMap& /*reflectionMap*/) { }
  void setOpacity(double /*opacityPercentage*/, const OdGiMaterialMap& /*opacityMap*/) { }
  void setBump(const OdGiMaterialMap& /*bumpMap*/) { }
  void setRefraction(double /*refractionIndex*/, const OdGiMaterialMap& /*refractionMap*/) { }
  void setTranslucence(double /*value*/) { }
  void setSelfIllumination(double /*value*/) { }
  void setReflectivity(double /*value*/) { }
  void setIlluminationModel(IlluminationModel /*model*/) { }
  void setChannelFlags(ChannelFlags /*flags*/) { }
  void setMode(Mode /*value*/) { }

  void setColorBleedScale(double /*scale*/) { }
  double colorBleedScale() const { return 0.0; }

  void setIndirectBumpScale(double /*scale*/) { }
  double indirectBumpScale() const { return 0.0; }

  void setReflectanceScale(double /*scale*/) { }
  double reflectanceScale() const { return 0.0; }

  void setTransmittanceScale(double /*scale*/) { }
  double transmittanceScale() const { return 0.0; }

  void setTwoSided(bool /*flag*/) { }
  bool twoSided() const { return true; }

  void setLuminanceMode(LuminanceMode /*mode*/) { }
  LuminanceMode luminanceMode() const { return kSelfIllumination; }

  void setLuminance(double /*value*/) { }
  double luminance() const { return 0.0; }

  void setNormalMap(const OdGiMaterialMap &/*normalMap*/, NormalMapMethod /*method*/, double /*strength*/) { }
  void normalMap(OdGiMaterialMap &/*normalMap*/, NormalMapMethod &/*method*/, double &/*strength*/) const { }

  void setGlobalIllumination(GlobalIlluminationMode /*mode*/) { }
  GlobalIlluminationMode globalIllumination() const { return kGlobalIlluminationCastAndReceive; }

  void setFinalGather(FinalGatherMode /*mode*/) { }
  FinalGatherMode finalGather() const { return kFinalGatherCastAndReceive; }

  void setEmission(const OdGiMaterialColor& /*emissionColor*/, const OdGiMaterialMap& /*emissionMap*/) { }
  void emission(OdGiMaterialColor& /*emissionColor*/, OdGiMaterialMap& /*emissionMap*/) const { }

  void setTint(const OdGiMaterialColor& /*tintColor*/) { }
  void tint(OdGiMaterialColor& /*tintColor*/) const { }

  void setShadingAmbient(const OdGiMaterialColor& /*ambientColor*/) { }
  void shadingAmbient(OdGiMaterialColor& /*ambientColor*/) const { }

  void setShadingDiffuse(const OdGiMaterialColor& /*diffuseColor*/) { }
  void shadingDiffuse(OdGiMaterialColor& /*diffuseColor*/) const { }

  void setShadingSpecular(const OdGiMaterialColor& /*specularColor*/) { }
  void shadingSpecular(OdGiMaterialColor& /*specularColor*/) const { }

  void setShadingOpacity(double /*opacityPercentage*/) { }
  void shadingOpacity(double& /*opacityPercentage*/) const { }

  void setDiffuseColorMode(DiffuseColorMode /*diffuseColorMode*/) { }
  DiffuseColorMode diffuseColorMode() const { return kDiffuseRealisticColor; }
};

typedef OdSmartPtr<OdGiMaterialTraits_Stub> OdGiMaterialTraits_StubPtr;

#include "TD_PackPop.h"

#endif // #ifndef _ODGIMATERIALTRAITSDATA_INCLUDED_
