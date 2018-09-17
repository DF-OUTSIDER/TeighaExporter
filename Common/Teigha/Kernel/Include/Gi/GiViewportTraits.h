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

#ifndef __OD_GI_VIEWPORT_TRAITS__
#define __OD_GI_VIEWPORT_TRAITS__

#include "TD_PackPush.h"

#include "Gi.h"
#include "CmColorBase.h"
#include "StaticRxObject.h"

class OdGiShadowParameters;

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiToneOperatorParameters : public OdRxObject
{
public:
  enum ExteriorDaylightMode
  {
    kDaylightOff  = 0,
    kDaylightOn   = 1,
    kDaylightAuto = 2
  };
public:
  ODRX_DECLARE_MEMBERS(OdGiToneOperatorParameters);

  OdGiToneOperatorParameters();
  // GCC < 4.4 uses generated copy constructor which is references private OdRxObject copy constructor
  OdGiToneOperatorParameters(const OdGiToneOperatorParameters &params);
  // User-defined destructor moved to header.
  // MSVC compiler bug: http://connect.microsoft.com/VisualStudio/feedback/details/680927/dereferencing-of-incomplete-type-not-diagnosed-fails-to-synthesise-constructor-and-destructor
  virtual ~OdGiToneOperatorParameters() { }

  bool operator ==(const OdGiToneOperatorParameters &params) const;
  bool operator !=(const OdGiToneOperatorParameters &params) const;
  OdGiToneOperatorParameters &operator =(const OdGiToneOperatorParameters &params);

  virtual void setIsActive(bool bActive);
  virtual bool isActive() const;

  virtual void setChromaticAdaptation(bool bEnable);
  virtual bool chromaticAdaptation() const;

  virtual void setColorDifferentiation(bool bEnable);
  virtual bool colorDifferentiation() const;

  virtual void setWhiteColor(const OdCmEntityColor &color);
  virtual const OdCmEntityColor &whiteColor() const;

  virtual void setProcessBackground(bool bProcessBg);
  virtual bool processBackground() const;

  virtual bool setBrightness(double fBrightness);
  virtual double brightness() const;

  virtual bool setContrast(double fContrast);
  virtual double contrast() const;

  virtual bool setMidTones(double fMidTones);
  virtual double midTones() const;

  virtual bool setExteriorDaylight(ExteriorDaylightMode mode);
  virtual ExteriorDaylightMode exteriorDaylight() const;
private:
  bool m_bIsActive;
  bool m_bChromaticAdaptation;
  bool m_bColorDifferentiation;
  OdCmEntityColor m_clrWhiteColor;
  bool m_bProcessBackground;
  double m_fBrightness;
  double m_fContrast;
  double m_fMidTones;
  ExteriorDaylightMode m_eExteriorDaylight;
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiToneOperatorParametersStatic : public OdStaticRxObject<OdGiToneOperatorParameters>
{
  public:
    OdGiToneOperatorParametersStatic() : OdStaticRxObject<OdGiToneOperatorParameters>() { }
    bool operator ==(const OdGiToneOperatorParameters &params) const { return params == *this; }
    bool operator !=(const OdGiToneOperatorParameters &params) const { return params != *this; }
    OdGiToneOperatorParameters &operator =(const OdGiToneOperatorParameters &params) { return (static_cast<OdGiToneOperatorParameters&>(*this) = params); }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiToneOperatorParameters object pointers.
*/
typedef OdSmartPtr<OdGiToneOperatorParameters> OdGiToneOperatorParametersPtr;

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiPhotographicExposureParameters : public OdGiToneOperatorParameters
{
public:
  ODRX_DECLARE_MEMBERS(OdGiPhotographicExposureParameters);

  OdGiPhotographicExposureParameters();
  OdGiPhotographicExposureParameters(const OdGiPhotographicExposureParameters &params);
  // WinCE failed to build w/o user-defined destructor.
  virtual ~OdGiPhotographicExposureParameters() { }

  bool operator ==(const OdGiPhotographicExposureParameters &params) const;
  bool operator !=(const OdGiPhotographicExposureParameters &params) const;
  OdGiPhotographicExposureParameters &operator =(const OdGiPhotographicExposureParameters &params);

  virtual bool setExposure(double fExposure);
  virtual double exposure() const;

  virtual bool setWhitePoint(double fWhitePoint);
  virtual double whitePoint() const;

  virtual bool setBrightness(double fBrightness);

  OdCmEntityColor whitePointToColor() const;

  static double convertExposureToBrightness(double fExposure);
  static double convertBrightnessToExposure(double fBrightness);
private:
  double m_fExposure;
  double m_fWhitePoint;
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiPhotographicExposureParametersStatic : public OdStaticRxObject<OdGiPhotographicExposureParameters>
{
public:
  OdGiPhotographicExposureParametersStatic() : OdStaticRxObject<OdGiPhotographicExposureParameters>() { }
  bool operator ==(const OdGiPhotographicExposureParameters &params) const { return params == *this; }
  bool operator !=(const OdGiPhotographicExposureParameters &params) const { return params != *this; }
  OdGiPhotographicExposureParameters &operator =(const OdGiPhotographicExposureParameters &params) { return (static_cast<OdGiPhotographicExposureParameters&>(*this) = params); }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiPhotographicExposureParameters object pointers.
*/
typedef OdSmartPtr<OdGiPhotographicExposureParameters> OdGiPhotographicExposureParametersPtr;

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiViewportTraits : public OdGiSubEntityTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiViewportTraits);

  enum DefaultLightingType 
  {
    kOneDistantLight  = 0,
    kTwoDistantLights = 1,
    kBackLighting     = 2,
    kUserDefinedLight = 3
  };

  virtual bool isDefaultLightingOn() const = 0;
  virtual void setDefaultLightingOn(bool b) = 0;

  virtual DefaultLightingType defaultLightingType() const = 0;
  virtual void setDefaultLightingType(DefaultLightingType) = 0;

  virtual OdGeVector3d userDefinedLightDirection() const = 0;
  virtual void setUserDefinedLightDirection(const OdGeVector3d& lightDirection) = 0;

  virtual double defaultLightingIntensity() const = 0;
  virtual void setDefaultLightingIntensity(double dIntensity) = 0;

  virtual OdCmEntityColor defaultLightingColor() const = 0;
  virtual void setDefaultLightingColor(const OdCmEntityColor& color) = 0;

  virtual void defaultLightingShadowParameters(OdGiShadowParameters &params) const = 0;
  virtual void setDefaultLightingShadowParameters(const OdGiShadowParameters &params) = 0;

  virtual void setAmbientLightColor(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor ambientLightColor() const = 0;

  virtual void setContrast(double contrast) = 0;
  virtual double contrast() const = 0;

  virtual void setBrightness(double brightness) = 0;
  virtual double brightness() const = 0;

  virtual void setBackground(OdDbStub* bg) = 0;
  virtual OdDbStub* background() const = 0;

  virtual void setRenderEnvironment(OdDbStub* re) = 0;
  virtual OdDbStub* renderEnvironment () const = 0;

  virtual void setRenderSettings(OdDbStub* rs) = 0;
  virtual OdDbStub* renderSettings() const = 0;

  virtual void setToneOperatorParameters(const OdGiToneOperatorParameters &params) = 0;
  virtual void toneOperatorParameters(OdGiToneOperatorParameters &params) = 0;
};

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
typedef OdSmartPtr<OdGiViewportTraits> OdGiViewportTraitsPtr;

#include "TD_PackPop.h"

#endif // __OD_GI_VIEWPORT_TRAITS__
