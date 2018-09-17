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


#ifndef __ODDBMATERIAL_H__
#define __ODDBMATERIAL_H__

#include "TD_PackPush.h"

#include "Gi/GiMaterial.h"
#include "DbObject.h"

/** \details
  This class represents Material properties for shaded entities.

  <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbMaterial : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbMaterial);

  OdDbMaterial();
  
  /** \details
    Sets the name of this Material object.
    
    \param name [in]  Name.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult setName(
    const OdString& name);
  /** \details
    Returns the name of this Material object.
  */
  virtual OdString name() const;


  /** \details
    Sets the description of this Material object.
    
    \param description [in]  Description.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual void setDescription(
    const OdString& description);
  /** \details
    Returns the description of this Material object.
  */
  virtual OdString description() const;


  /** \details
    Sets the ambient (shadow) component of this Material object.
    
    \param ambientColor [in]  Ambient color.

  */
  virtual void setAmbient(
    const OdGiMaterialColor& ambientColor);
    
  /** \details
    Returns the ambient (shadow) component of this Material object.
    
    \param ambientColor [out]  Receives the ambient color.

  */
  virtual void ambient(
    OdGiMaterialColor& ambientColor) const;

  /** \details
    Sets the diffuse (main) component of this Material object.
    
    \param diffuseColor [in]  Diffuse color.
    \param diffuseMap [in]  Diffuse map.
  */
  virtual void setDiffuse(
    const OdGiMaterialColor& diffuseColor, 
    const OdGiMaterialMap& diffuseMap);
    
  /** \details
    Returns the diffuse (main) component of this Material object.
    
    \param diffuseColor [out]  Receives the diffuse color.
    \param diffuseMap [out]  Receives the diffuse map.

  */
  virtual void diffuse(
    OdGiMaterialColor& diffuseColor, 
    OdGiMaterialMap& diffuseMap) const;


  /** \details
    Sets the specular (reflection) component of this Material object.
    
    \param specularColor [in]  Specular color.
    \param specularMap [in]  Specular map.
    \param glossFactor [in]  Gloss factor.
    
    \remarks
    The specular component is viewpoint dependent, and most apparent when the entity is highlighted.
  */
  virtual void setSpecular(
    const OdGiMaterialColor& specularColor, 
    const OdGiMaterialMap& specularMap, 
    double glossFactor);

  /** \details
    Returns the specular (reflection) component of this Material object.
    
    \param specularColor [out]  Receives the specular color.
    \param specularMap [out]  Receives the specular map.
    \param glossFactor [out]  Receives the gloss factor.
    
    \remarks
    The specular component is viewpoint dependent, and most apparent when the entity is highlighted.
  */
  virtual void specular(
    OdGiMaterialColor& specularColor, 
    OdGiMaterialMap& specularMap, 
    double& glossFactor) const;

  /** \details
    Sets the reflection component of this Material object.
    
    \param reflectionMap [in]  Reflection map.
    
    \remarks
    The reflection component creates a mirror finish on the entity.
  */
  virtual void setReflection(const OdGiMaterialMap& reflectionMap);

  /** \details
    Returns the reflection component of this Material object.
    
    \param reflectionMap [out]  Receives the reflection map.
    
    \remarks
    The reflection component creates a mirror finish on the entity .
  */
  virtual void reflection(OdGiMaterialMap& reflectionMap) const;


  /** \details
    Sets the opacity component of this Material object.
    
    \param opacityPercentage [in]  Opacity percentage.
    \param opacityMap [in]  Opacity map.
  */
  virtual void setOpacity(
    double opacityPercentage, 
    const OdGiMaterialMap& opacityMap);

  /** \details
    Returns the opacity component of this Material object.
    
    \param opacityPercentage [out]  Receives the opacity percentage.
    \param opacityMap [out]  Receives the opacity map.
  */
  virtual void opacity(
    double& opacityPercentage, 
    OdGiMaterialMap& opacityMap) const;

  /** \details
    Sets the bump component of this Material object.
    
    \param bumpMap [in]  Bump map.
  */
  virtual void setBump(const OdGiMaterialMap& bumpMap);

  /** \details
    Returns the bump component of this Material object.
    
    \param bumpMap [out]  Receives the bump map.
  */
  virtual void bump(OdGiMaterialMap& map) const;

  /** \details
    Sets the refraction component of this Material object.
    
    \param refractionIndex [in]  Refraction index.
    \param refractionMap [in]  Refraction map.
  */
  virtual void setRefraction(
    double refractionIndex, 
    const OdGiMaterialMap& refractionMap);

  /** \details
    Returns the refraction component of this Material object.
    
    \param refractionIndex [out]  Receives the refraction index.
    \param refractionMap [out]  Receives the refraction map.
  */
  virtual void refraction(
    double& refractionIndex,
    OdGiMaterialMap& refractionMap) const;

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
    
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
    
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
    
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual OdGiDrawable* drawable();

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;


/*-------------- New in AC21--------------------*/

  /** \details
    Returns the translucence of this Material object.
  */
  virtual double translucence() const;

  /** \details
    Sets the translucence of this Material object.
    \param translucence [in]  Translucense.
  */
  virtual void setTranslucence(double translucence);

  /** \details
    Returns the self-illumination of this Material object.
  */
  virtual double selfIllumination() const;

  /** \details
    Sets the self-illumination of this material object
    \param selfIllumination [in]  Self-illumination.
  */
  virtual void setSelfIllumination(double selfIllumination);

  /** \details
    Returns the refelectivity of this Material object. 
  */
  virtual double reflectivity() const;

  /** \details
    Sets the reflectivity of this Material object.
    \param reflectivity [in]  Reflectivity.
  */
  virtual void  setReflectivity(double reflectivity);

  /** \details
    Returns the mode of this Material object.
  */
  virtual OdGiMaterialTraits::Mode mode() const;

  /** \details
    Sets the mode of this Material object.
    \param mode [in]  Mode.
  */
  virtual void setMode(OdGiMaterialTraits::Mode mode);

  /** \details
    Returns the channel flags of this material object.
  */
  virtual OdGiMaterialTraits::ChannelFlags channelFlags() const;
  /** \details
    Sets the channel flags for this material object.
    \remarks
    This allows multiple channels to be added to a material 
    and each channel to be enabled or disabled individually.
    \param channelFlags [in]  channel flags.
  */
  virtual void setChannelFlags(OdGiMaterialTraits::ChannelFlags channelFlags);

  /** \details
    Returns the illumination model of this Material object.
    \remarks
    The illumination model determines the type of shader used to render this Material object.
  */
  virtual OdGiMaterialTraits::IlluminationModel illuminationModel() const;

  /** \details
    Sets the illumination model of this Material object.
    \param illuminationModel [in]  Illumination model.
  */
  virtual void setIlluminationModel(OdGiMaterialTraits::IlluminationModel illuminationMode);

  /*---- Advanced material properties in AC21 XRec ----*/

  /** \details
    Returns the color bleed scale of this Material object.
  */
  virtual double colorBleedScale() const;

  /** \details
    Sets the color bleed scale of this Material object.
    \param scale [in]  Color bleed scale.
  */
  virtual void setColorBleedScale(double scale);

  /** \details
    Returns the indirect bump scale of this Material object.
  */
  virtual double indirectBumpScale() const;

  /** \details
    Sets the indirect bump scale of this Material object.
    \param scale [in]  Indirect bump scale.
  */
  virtual void setIndirectBumpScale(double scale);

  /** \details
    Returns the reflectance scale of this Material object.
  */
  virtual double reflectanceScale() const;

  /** \details
    Sets the reflectance scale of this Material object.
    \param scale [in]  Reflectance scale.
  */
  virtual void setReflectanceScale(double scale);

  /** \details
    Returns the transmittance scale of this Material object.
  */
  virtual double transmittanceScale() const;

  /** \details
    Sets the transmittance scale of this Material object.
    \param scale [in]  Transmittance scale.
  */
  virtual void setTransmittanceScale(double scale);

  /** \details
    Returns the two sided mode of this Material object.
  */
  virtual bool twoSided() const;

  /** \details
    Sets the two sided mode of this Material object.
    \param flag [in]  Two sided mode flag.
  */
  virtual void setTwoSided(bool flag);

  /** \details
    Returns the luminance mode of this Material object.
  */
  virtual OdGiMaterialTraits::LuminanceMode luminanceMode() const;

  /** \details
    Sets the luminance mode of this Material object.
    \param mode [in]  Luminance mode.
  */
  virtual void setLuminanceMode(OdGiMaterialTraits::LuminanceMode mode);

  /** \details
    Returns the luminance of this Material object.
  */
  virtual double luminance() const;

  /** \details
    Sets the luminance of this Material object.
    \param value [in]  Luminance value.
  */
  virtual void setLuminance(double value);

  /** \details
    Returns the shininess of this Material object.
    \remarks
    See also specular gloss factor.
  */
  virtual double shininess() const;
  
  /** \details
    Sets the shininess of this Material object.
    \remarks
    See also specular gloss factor.
    \param value [in]  Shinincess value.
  */
  virtual void setShininess(double value);

  /** \details
    Returns the normalMap component of this Material object.
    \param normalMap [out]  Receives the normalMap.
    \param method [out]  Receives normal map method.
    \param strength [out]  Receives normal map strength factor.
  */
  virtual void normalMap(OdGiMaterialMap &normalMap, OdGiMaterialTraits::NormalMapMethod &method, double &strength) const;

  /** \details
    Sets the normalMap component of this Material object.
    
    \param normalMap [in]  Normal map.
    \param method [in]  Normal map method.
    \param strength [in]  Normal map strength factor.
  */
  virtual void setNormalMap(const OdGiMaterialMap &normalMap, OdGiMaterialTraits::NormalMapMethod method, double strength);

  /** \details
    Returns the anonymous material mode of this Material object.
  */
  virtual bool isAnonymous() const;

  /** \details
    Sets the anonymous material mode of this Material object.
    \param flag [in]  Anonymous mode flag.
  */
  virtual void setAnonymous(bool flag);

  /** \details
    Returns the global illumination mode of this Material object.
  */
  virtual OdGiMaterialTraits::GlobalIlluminationMode globalIllumination() const;

  /** \details
    Sets the global illumination mode of this Material object.
    \param mode [in]  Global illumination mode.
  */
  virtual void setGlobalIllumination(OdGiMaterialTraits::GlobalIlluminationMode mode);

  /** \details
    Returns the final gather mode of this Material object.
  */
  virtual OdGiMaterialTraits::FinalGatherMode finalGather() const;

  /** \details
    Sets the final gather mode of this Material object.
    \param mode [in]  Final gather mode.
  */
  virtual void setFinalGather(OdGiMaterialTraits::FinalGatherMode mode);

  virtual OdResult subGetClassID(void* pClsid) const;

  OdResult subErase( bool erasing );
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbMaterial object pointers.
*/
typedef OdSmartPtr<OdDbMaterial> OdDbMaterialPtr;

#include "TD_PackPop.h"

#endif
