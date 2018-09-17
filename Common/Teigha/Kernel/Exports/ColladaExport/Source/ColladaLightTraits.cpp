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
#include "ColladaLightTraits.h"


namespace TD_COLLADA_EXPORT
{
  //-------------------------------------------------------------------------------------------------------------------
  void ColladaDistantLightTraits::setData(OdGiDistantLightTraitsData& data)
  { 
    m_pData = &data; 
  }

  double ColladaDistantLightTraits::intensity() const
  {
    return m_pData->intensity();  
  }

  OdGiShadowParameters ColladaDistantLightTraits::shadowParameters() const
  {
    return m_pData->shadowParameters();  
  }

  OdCmEntityColor ColladaDistantLightTraits::lightColor() const
  {
    return m_pData->color();
  }

  void ColladaDistantLightTraits::setShadowParameters( const OdGiShadowParameters& params )
  {
    m_pData->setShadowParameters(params);  
  }

  void ColladaDistantLightTraits::setIntensity( double inten )
  {
    m_pData->setIntensity(inten);  
  }

  void ColladaDistantLightTraits::setLightColor( const OdCmEntityColor& color )
  {
    m_pData->setColor(color);  
  }

  void ColladaDistantLightTraits::setOn(bool on)
  {
    m_pData->setOn(on);
  }

  bool ColladaDistantLightTraits::isOn() const
  {
    return m_pData->isOn();
  }

  OdGeVector3d ColladaDistantLightTraits::lightDirection() const
  {
    return m_pData->direction();
  }

  bool ColladaDistantLightTraits::isSunlight() const
  {
    return m_pData->isSunlight();
  }

  void ColladaDistantLightTraits::setLightDirection( const OdGeVector3d& vec )
  {
    m_pData->setDirection(vec);
  }

  void ColladaDistantLightTraits::setIsSunlight( bool isSunlight )
  {
    m_pData->setSunlight(isSunlight);
  }

  void ColladaDistantLightTraits::setPhysicalIntensity(double fIntensity)
  {
    m_pData->setPhysicalIntensity(fIntensity);
  }

  double ColladaDistantLightTraits::physicalIntensity() const
  {
    return m_pData->physicalIntensity();
  }

  void ColladaDistantLightTraits::setLampColor(const OdGiColorRGB& color)
  {
    m_pData->setLampColor(color);
  }

  OdGiColorRGB ColladaDistantLightTraits::lampColor() const
  {
    return m_pData->lampColor();
  }

  void ColladaDistantLightTraits::setSkyParameters(const OdGiSkyParameters &params)
  {
    m_pData->setSkyParameters(params);
  }

  void ColladaDistantLightTraits::skyParameters(OdGiSkyParameters &params) const
  {
    m_pData->skyParameters(params);
  }

  //-------------------------------------------------------------------------------------------------------------------
  void ColladaPointLightTraits::setData(OdGiPointLightTraitsData& data)
  { 
    m_pData = &data; 
  }

  double ColladaPointLightTraits::intensity() const
  {
    return m_pData->intensity();  
  }

  OdGiShadowParameters ColladaPointLightTraits::shadowParameters() const
  {
    return m_pData->shadowParameters();  
  }

  OdCmEntityColor ColladaPointLightTraits::lightColor() const
  {
    return m_pData->color();
  }

  void ColladaPointLightTraits::setShadowParameters( const OdGiShadowParameters& params )
  {
    m_pData->setShadowParameters(params);  
  }

  void ColladaPointLightTraits::setIntensity( double inten )
  {
    m_pData->setIntensity(inten);  
  }

  void ColladaPointLightTraits::setLightColor( const OdCmEntityColor& color )
  {
    m_pData->setColor(color);  
  }

  void ColladaPointLightTraits::setOn(bool on)
  {
    m_pData->setOn(on);
  }

  bool ColladaPointLightTraits::isOn() const
  {
    return m_pData->isOn();
  }

  OdGePoint3d ColladaPointLightTraits::position() const
  {
    return m_pData->position();
  }

  OdGiLightAttenuation ColladaPointLightTraits::lightAttenuation() const
  {
    return m_pData->attenuation();
  }

  void ColladaPointLightTraits::setAttenuation( const OdGiLightAttenuation& atten )
  {
    m_pData->setAttenuation(atten);
  }

  void ColladaPointLightTraits::setPosition( const OdGePoint3d& pos )
  {
    m_pData->setPosition(pos);
  }

  void ColladaPointLightTraits::setPhysicalIntensity(double fIntensity)
  {
    m_pData->setPhysicalIntensity(fIntensity);
  }

  double ColladaPointLightTraits::physicalIntensity() const
  {
    return m_pData->physicalIntensity();
  }

  void ColladaPointLightTraits::setLampColor(const OdGiColorRGB& color)
  {
    m_pData->setLampColor(color);
  }

  OdGiColorRGB ColladaPointLightTraits::lampColor() const
  {
    return m_pData->lampColor();
  }

  void ColladaPointLightTraits::setHasTarget(bool bTarget)
  {
    m_pData->setHasTarget(bTarget);
  }

  bool ColladaPointLightTraits::hasTarget() const
  {
    return m_pData->hasTarget();
  }

  void ColladaPointLightTraits::setTargetLocation(const OdGePoint3d &loc)
  {
    m_pData->setTargetLocation(loc);
  }

  OdGePoint3d ColladaPointLightTraits::targetLocation() const
  {
    return m_pData->targetLocation();
  }

  void ColladaPointLightTraits::setHemisphericalDistribution(bool bHemisphere)
  {
    m_pData->setHemisphericalDistribution(bHemisphere);
  }
  
  bool ColladaPointLightTraits::hemisphericalDistribution() const
  {
    return m_pData->hemisphericalDistribution();
  }

  //-------------------------------------------------------------------------------------------------------------------
  void ColladaSpotLightTraits::setData(OdGiSpotLightTraitsData& data)
  { 
    m_pData = &data; 
  }

  double ColladaSpotLightTraits::intensity() const
  {
    return m_pData->intensity();  
  }

  OdGiShadowParameters ColladaSpotLightTraits::shadowParameters() const
  {
    return m_pData->shadowParameters();  
  }

  OdCmEntityColor ColladaSpotLightTraits::lightColor() const
  {
    return m_pData->color();
  }

  void ColladaSpotLightTraits::setShadowParameters( const OdGiShadowParameters& params )
  {
    m_pData->setShadowParameters(params);  
  }

  void ColladaSpotLightTraits::setIntensity( double inten )
  {
    m_pData->setIntensity(inten);  
  }

  void ColladaSpotLightTraits::setLightColor( const OdCmEntityColor& color )
  {
    m_pData->setColor(color);  
  }

  void ColladaSpotLightTraits::setOn(bool on)
  {
    m_pData->setOn(on);
  }

  bool ColladaSpotLightTraits::isOn() const
  {
    return m_pData->isOn();
  }

  OdGePoint3d ColladaSpotLightTraits::targetLocation() const
  {
    return m_pData->target();
  }

  OdGePoint3d ColladaSpotLightTraits::position() const
  {
    return m_pData->position();
  }

  OdGiLightAttenuation ColladaSpotLightTraits::lightAttenuation() const
  {
    return m_pData->attenuation();
  }

  void ColladaSpotLightTraits::setTargetLocation( const OdGePoint3d& loc )
  {
    m_pData->setTarget(loc);
  }

  double ColladaSpotLightTraits::falloff() const
  {
    return m_pData->falloff();
  }

  bool ColladaSpotLightTraits::setHotspotAndFalloff( double hotspot, double falloff )
  {
    return m_pData->setHotspotAndFalloff(hotspot, falloff);
  }

  double ColladaSpotLightTraits::hotspot() const
  {
    return m_pData->hotspot();
  }

  void ColladaSpotLightTraits::setAttenuation( const OdGiLightAttenuation& atten )
  {
    return m_pData->setAttenuation(atten);
  }

  void ColladaSpotLightTraits::setPosition( const OdGePoint3d& pos )
  {
    return m_pData->setPosition(pos);
  }

  void ColladaSpotLightTraits::setPhysicalIntensity(double fIntensity)
  {
    m_pData->setPhysicalIntensity(fIntensity);
  }

  double ColladaSpotLightTraits::physicalIntensity() const
  {
    return m_pData->physicalIntensity();
  }

  void ColladaSpotLightTraits::setLampColor(const OdGiColorRGB& color)
  {
    m_pData->setLampColor(color);
  }

  OdGiColorRGB ColladaSpotLightTraits::lampColor() const
  {
    return m_pData->lampColor();
  }
};
