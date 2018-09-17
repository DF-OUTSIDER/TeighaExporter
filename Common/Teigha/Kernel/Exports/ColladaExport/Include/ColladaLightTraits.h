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

#ifndef _COLLADA_LIGHTTRAITS_INCLUDED_
#define _COLLADA_LIGHTTRAITS_INCLUDED_
#pragma once

#include "Gi/GiDrawable.h"
#include "DbBaseDatabase.h"
#include "Ge/GePoint3d.h"
#include "SmartPtr.h"
#include "Gi/GiLightTraitsData.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_COLLADA_EXPORT
{
  /** \details
    This class implements the distant light traits for Collada.
  */
  class ColladaDistantLightTraits : public OdGiDistantLightTraits
  {
    OdGiDistantLightTraitsData* m_pData;
  public:
    /** \details
       Returns the distant light intensity.
    */
    virtual double intensity() const;

    /** \details
       Sets the distant light intensity.
    */
    virtual void setIntensity( double inten );

    /** \details
       Returns the shadow parameters.
    */
    virtual OdGiShadowParameters shadowParameters() const;

    /** \details
       Sets the shadow parameters.
    */
    virtual void setShadowParameters( const OdGiShadowParameters& params );

    /** \details
       Returns the distant light color.
    */
    virtual OdCmEntityColor lightColor() const;

    /** \details
       Sets the distant light color.
    */
    virtual void setLightColor( const OdCmEntityColor& color );

    /** \details
       Checks whether the distant light is on or off.
    */
    virtual bool isOn() const;
    
    /** \details
       Switches the distant light to the on or off.
    */
    virtual void setOn(bool on);

    /** \details
       Returns the distant light direction.
    */
    virtual OdGeVector3d lightDirection() const;

    /** \details
       Sets the distant light direction.
    */
    virtual void setLightDirection( const OdGeVector3d& vec );

    /** \details
       Checks whether the sun light is on.
    */
    virtual bool isSunlight() const;

    /** \details
       Switches the sun light to the on or off.
    */
    virtual void setIsSunlight( bool isSunlight );

    virtual void setPhysicalIntensity(double fIntensity);
    virtual double physicalIntensity() const;

    virtual void setLampColor(const OdGiColorRGB& color);
    virtual OdGiColorRGB lampColor() const;

    virtual void setSkyParameters(const OdGiSkyParameters &params);
    virtual void skyParameters(OdGiSkyParameters &params) const;

    void setData(OdGiDistantLightTraitsData& data);
  };


  //-------------------------------------------------------------------------------------------------------------------

  /** \details
    This class implements the point light traits for Collada.
  */
  class ColladaPointLightTraits : public OdGiPointLightTraits
  {
    OdGiPointLightTraitsData* m_pData;
  public:
    /** \details
       Returns the point light intensity.
    */
    virtual double intensity() const;

    /** \details
       Sets the point light intensity.
    */
    virtual void setIntensity( double inten );

    /** \details
       Returns the shadow parameters.
    */
    virtual OdGiShadowParameters shadowParameters() const;

    /** \details
       Sets the shadow parameters.
    */
    virtual void setShadowParameters( const OdGiShadowParameters& params );

    /** \details
       Returns the point light color.
    */
    virtual OdCmEntityColor lightColor() const;

    /** \details
       Sets the point light color.
    */
    virtual void setLightColor( const OdCmEntityColor& color );

    /** \details
       Checks whether the point light is on or off.
    */
    virtual bool isOn() const;

    /** \details
       Switches the point light to the on or off.
    */
    virtual void setOn(bool on);

    /** \details
       Returns the point light position.
    */
    virtual OdGePoint3d position() const;

    /** \details
       Sets the point light position.
    */
    virtual void setPosition( const OdGePoint3d& pos );

    /** \details
       Returns the point light attenuation.
    */
    virtual OdGiLightAttenuation lightAttenuation() const;

    /** \details
       Sets the point light attenuation.
    */
    virtual void setAttenuation( const OdGiLightAttenuation& atten );

    virtual void setPhysicalIntensity(double fIntensity);
    virtual double physicalIntensity() const;

    virtual void setLampColor(const OdGiColorRGB& color);
    virtual OdGiColorRGB lampColor() const;

    virtual void setHasTarget(bool bTarget);
    virtual bool hasTarget() const;

    virtual void setTargetLocation(const OdGePoint3d &loc);
    virtual OdGePoint3d targetLocation() const;

    virtual void setHemisphericalDistribution(bool bHemisphere);
    virtual bool hemisphericalDistribution() const;

    void setData(OdGiPointLightTraitsData& data);
  };

  //-------------------------------------------------------------------------------------------------------------------

  /** \details
    This class implements the spot light traits for Collada.
  */
  class ColladaSpotLightTraits : public OdGiSpotLightTraits
  {
    OdGiSpotLightTraitsData* m_pData;
  public:
    /** \details
       Returns the spot light intensity.
    */
    virtual double intensity() const;

    /** \details
       Sets the spot light intensity.
    */
    virtual void setIntensity( double inten );

    /** \details
       Returns the shadow parameters.
    */
    virtual OdGiShadowParameters shadowParameters() const;

    /** \details
       Sets the shadow parameters.
    */
    virtual void setShadowParameters( const OdGiShadowParameters& params );

    /** \details
       Returns the spot light color.
    */
    virtual OdCmEntityColor lightColor() const;

    /** \details
       Sets the spot light color.
    */
    virtual void setLightColor( const OdCmEntityColor& color );

    /** \details
       Checks whether the spot light is on or off.
    */
    virtual bool isOn() const;

    /** \details
       Switches the spot light to the on or off.
    */
    virtual void setOn(bool on);

    /** \details
       Returns the spot light target point.
    */
    virtual OdGePoint3d targetLocation() const;

    /** \details
       Sets the spot light target point.
    */
    virtual void setTargetLocation( const OdGePoint3d& loc );

    /** \details
       Returns the spot light position.
    */
    virtual OdGePoint3d position() const;

    /** \details
       Sets the spot light position.
    */
    virtual void setPosition( const OdGePoint3d& pos );

    /** \details
       Returns the spot light attenuation.
    */
    virtual void setAttenuation( const OdGiLightAttenuation& atten );

    /** \details
       Sets the spot light attenuation.
    */
    virtual OdGiLightAttenuation lightAttenuation() const;

    virtual double falloff() const;

    virtual bool setHotspotAndFalloff( double hotspot, double falloff );

    /** \details
       Returns the hotspot.
    */
    virtual double hotspot() const;

    virtual void setPhysicalIntensity(double fIntensity);
    virtual double physicalIntensity() const;

    virtual void setLampColor(const OdGiColorRGB& color);
    virtual OdGiColorRGB lampColor() const;

    void setData(OdGiSpotLightTraitsData& data);
  };
};

#endif // _COLLADA_LIGHTTRAITS_INCLUDED_

