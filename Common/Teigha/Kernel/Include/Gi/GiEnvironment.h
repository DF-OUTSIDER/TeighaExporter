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

#ifndef _GIENVIRONMENT_H_INCLUDED_
#define _GIENVIRONMENT_H_INCLUDED_

#include "Gi.h"
#include "GiMaterial.h"

class OdGiSkyParameters;

/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiRenderEnvironmentTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiRenderEnvironmentTraits);

  virtual void setEnable(bool bEnable) = 0;
  virtual bool enable() const = 0;
  virtual void setIsBackground(bool bEnable) = 0;
  virtual bool isBackground() const = 0;
  virtual void setFogColor(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor fogColor() const = 0;
  virtual void setNearDistance(double nearDist) = 0;
  virtual double nearDistance() const = 0;
  virtual void setFarDistance(double farDist) = 0;
  virtual double farDistance() const = 0;
  virtual void setNearPercentage(double nearPct) = 0;
  virtual double nearPercentage() const = 0;
  virtual void setFarPercentage(double farPct) = 0;
  virtual double farPercentage() const = 0;
  virtual void setEnvironmentMap(const OdGiMaterialTexture* m) = 0;
  virtual OdGiMaterialTexture* environmentMap() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiRenderEnvironmentTraits object pointers.
*/
typedef OdSmartPtr<OdGiRenderEnvironmentTraits> OdGiRenderEnvironmentTraitsPtr;


/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiRenderSettingsTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiRenderSettingsTraits);

  virtual void setMaterialEnabled(bool enabled) = 0;
  virtual bool materialEnabled() const = 0;
  virtual void setTextureSampling(bool enabled) = 0;
  virtual bool textureSampling() const = 0;
  virtual void setBackFacesEnabled(bool enabled) = 0;
  virtual bool backFacesEnabled() const = 0;
  virtual void setShadowsEnabled(bool enabled) = 0;
  virtual bool shadowsEnabled() const = 0;
  virtual void setDiagnosticBackgroundEnabled(bool enabled) = 0;
  virtual bool diagnosticBackgroundEnabled() const = 0;
  virtual void setModelScaleFactor(double scaleFactor) = 0;
  virtual double modelScaleFactor() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiRenderSettingsTraits object pointers.
*/
typedef OdSmartPtr<OdGiRenderSettingsTraits> OdGiRenderSettingsTraitsPtr;


typedef enum 
{
  krBox         = 0,
  krTriangle    = 1,
  krGauss       = 2,
  krMitchell    = 3,
  krLanczos     = 4
} 
OdGiMrFilter;

typedef enum 
{
  krSimple      = 0,
  krSorted      = 1,
  krSegments    = 2
} 
OdGiMrShadowMode;

typedef enum 
{
  krOff         = 0,
  krGrid        = 1,
  krPhoton      = 2,
  krSamples     = 3,
  krBSP         = 4
} 
OdGiMrDiagnosticMode;

typedef enum 
{
  krObject      = 0,
  krWorld       = 1,
  krCamera      = 2
}
OdGiMrDiagnosticGridMode;

typedef enum 
{
  krDensity     = 0,
  krIrradiance  = 1
} 
OdGiMrDiagnosticPhotonMode;

typedef enum 
{
  krDepth       = 0,
  krSize        = 1
}
OdGiMrDiagnosticBSPMode;

typedef enum 
{
  krHilbert       = 0,
  krSpiral        = 1,
  krLeftToRight   = 2,
  krRightToLeft   = 3,
  krTopToBottom   = 4,
  krBottomToTop   = 5
}
OdGiMrTileOrder;

typedef enum
{
  krAutomatic     = 0,
  krLogarithmic   = 1
}
OdGiMrExposureType;

typedef enum
{
  krFinalGatherOff   = 0,
  krFinalGatherOn    = 1,
  krFinalGatherAuto  = 2
}
OdGiMrFinalGatheringMode;

typedef enum
{
  krExportMIOff        = 0,
  krExportMIWithRender = 1,
  krExportMIOnly       = 2
}
OdGiMrExportMIMode;

/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiMentalRayRenderSettingsTraits : public OdGiRenderSettingsTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiMentalRayRenderSettingsTraits);

  virtual void setSampling(int min, int max) = 0;
  virtual void sampling(int& min, int& max) const = 0;

  virtual void setSamplingFilter(OdGiMrFilter filter, double width, double height) = 0;
  virtual void SamplingFilter(OdGiMrFilter& filter, double& width, double& height) const = 0;

  virtual void setSamplingContrastColor(float r, float g, float b, float a) = 0;
  virtual void samplingContrastColor(float& r, float& g, float& b, float& a) const = 0;

  virtual void setShadowMode(OdGiMrShadowMode mode) = 0;
  virtual OdGiMrShadowMode shadowMode() const = 0;

  virtual void setShadowMapEnabled(bool enabled) = 0;
  virtual bool shadowMapEnabled() const = 0;

  virtual void setRayTraceEnabled(bool enabled) = 0;
  virtual bool rayTraceEnabled() const = 0;

  virtual void setRayTraceDepth(int reflection, int refraction, int sum) = 0;
  virtual void rayTraceDepth(int& reflection, int& refraction, int& sum) const = 0;

  virtual void setGlobalIlluminationEnabled(bool enabled) = 0;
  virtual bool globalIlluminationEnabled() const = 0;

  virtual void setGISampleCount(int num) = 0;
  virtual int giSampleCount() const = 0;

  virtual void setGISampleRadiusEnabled(bool enabled) = 0;
  virtual bool giSampleRadiusEnabled() const = 0;

  virtual void setGISampleRadius(double radius) = 0;
  virtual double giSampleRadius() const = 0;

  virtual void setGIPhotonsPerLight(int num) = 0;
  virtual int giPhotonsPerLight() const = 0;

  virtual void setPhotonTraceDepth(int reflection, int refraction, int sum) = 0;
  virtual void photonTraceDepth(int& reflection, int& refraction, int& sum) const = 0;

  virtual void setFinalGatheringEnabled(bool enabled) = 0;
  virtual bool finalGatheringEnabled() const = 0;

  virtual void setFGRayCount(int num) = 0;
  virtual int fgRayCount() const = 0;

  virtual void setFGRadiusState(bool bMin, bool bMax, bool bPixels) = 0;
  virtual void fgSampleRadiusState(bool& bMin, bool& bMax, bool& bPixels) = 0;

  virtual void setFGSampleRadius(double min, double max) = 0;
  virtual void fgSampleRadius(double& min, double& max) const = 0;

  virtual void setLightLuminanceScale(double luminance) = 0;
  virtual double lightLuminanceScale() const = 0;

  virtual void setDiagnosticMode(OdGiMrDiagnosticMode mode) = 0;
  virtual OdGiMrDiagnosticMode diagnosticMode() const = 0;

  virtual void setDiagnosticGridMode(OdGiMrDiagnosticGridMode mode, float fSize) = 0;
  virtual void diagnosticGridMode(OdGiMrDiagnosticGridMode& mode, float& fSize) const = 0;

  virtual void setDiagnosticPhotonMode(OdGiMrDiagnosticPhotonMode mode) = 0;
  virtual OdGiMrDiagnosticPhotonMode diagnosticPhotonMode() const = 0;

  virtual void setDiagnosticBSPMode(OdGiMrDiagnosticBSPMode mode) = 0;
  virtual OdGiMrDiagnosticBSPMode diagnosticBSPMode() const = 0;

  virtual void setExportMIEnabled(bool enabled) = 0;
  virtual bool exportMIEnabled() const = 0;

  virtual void setExportMIFileName(const OdString& miName) = 0;
  virtual OdString exportMIFileName() const = 0;

  virtual void setTileSize(int size) = 0;
  virtual int tileSize() const = 0;

  virtual void setTileOrder(OdGiMrTileOrder order) = 0;
  virtual OdGiMrTileOrder tileOrder() const = 0;

  virtual void setMemoryLimit(int limit) = 0;
  virtual int memoryLimit() const = 0;

  virtual void setEnergyMultiplier(float fScale) = 0;
  virtual float energyMultiplier() const = 0;

  virtual void setProgressMonitor(void* pMonitor) = 0;
  virtual const void* progressMonitor() const = 0;

  virtual void setExposureType(OdGiMrExposureType type) = 0;
  virtual OdGiMrExposureType exposureType() const = 0;

  virtual void setFinalGatheringMode(OdGiMrFinalGatheringMode mode) = 0;
  virtual OdGiMrFinalGatheringMode finalGatheringMode() const = 0;

  virtual void setShadowSamplingMultiplier(double multiplier) = 0;
  virtual double shadowSamplingMultiplier() const = 0;

  virtual void setExportMIMode(OdGiMrExportMIMode mode) = 0;
  virtual OdGiMrExportMIMode exportMIMode() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMentalRayRenderSettingsTraits object pointers.
*/
typedef OdSmartPtr<OdGiMentalRayRenderSettingsTraits> OdGiMentalRayRenderSettingsTraitsPtr;


typedef enum
{
  krEQuitByRenderLevel = 0,
  krEQuitByRenderTime  = 1
}
OdGiQuitCondition;

typedef enum
{
  krESimplistic = 0,
  krEBasic      = 1,
  krEAdvanced   = 2
}
OdGiLightingMode;

typedef enum
{
  krEBox      = 0,
  krETriangle = 1,
  krEGaussian = 2,
  krELanczos  = 3,
  krEMitchell = 4
}
OdGiFilterType;

/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiRapidRTRenderSettingsTraits : public OdGiRenderSettingsTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiRapidRTRenderSettingsTraits);

  virtual void setQuitCondition(OdGiQuitCondition condition) = 0;
  virtual OdGiQuitCondition quitCondition() const = 0;

  virtual void setDesiredRenderLevel(int level) = 0;
  virtual int desiredRenderLevel() const = 0;

  virtual void setDesiredRenderTime(int time) = 0;
  virtual int desiredRenderTime() const = 0;

  virtual void setLightingMode(OdGiLightingMode mode) = 0;
  virtual OdGiLightingMode lightingMode() const = 0;

  virtual void setFilterType(OdGiFilterType type) = 0;
  virtual OdGiFilterType filterType() const = 0;

  virtual void setFilterWidth(float width) = 0;
  virtual float filterWidth() const = 0;

  virtual void setFilterHeight(float height) = 0;
  virtual float filterHeight() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiRapidRTRenderSettingsTraits object pointers.
*/
typedef OdSmartPtr<OdGiRapidRTRenderSettingsTraits> OdGiRapidRTRenderSettingsTraitsPtr;


/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiSolidBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSolidBackgroundTraits);

  virtual void setColorSolid(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorSolid() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiSolidBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiSolidBackgroundTraits> OdGiSolidBackgroundTraitsPtr;


/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiGradientBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiGradientBackgroundTraits);

  virtual void setColorTop(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorTop() const = 0;

  virtual void setColorMiddle(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorMiddle() const  = 0;

  virtual void setColorBottom(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorBottom() const  = 0;

  virtual void setHorizon(double horizon) = 0;
  virtual double horizon() const = 0;

  virtual void setHeight(double height) = 0;
  virtual double height() const = 0;

  virtual void setRotation(double rotation) = 0;
  virtual double rotation() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiGradientBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiGradientBackgroundTraits> OdGiGradientBackgroundTraitsPtr;


/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiImageBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiImageBackgroundTraits);

  virtual void setImageFilename(const OdString& filename) = 0;
  virtual const OdString& imageFilename() const = 0;

  virtual void setFitToScreen(bool bFitToScreen) = 0;
  virtual bool fitToScreen() const = 0;

  virtual void setMaintainAspectRatio(bool bMaintainAspectRatio) = 0;
  virtual bool maintainAspectRatio() const = 0;

  virtual void setUseTiling(bool bUseTiling) = 0;
  virtual bool useTiling() const = 0;

  virtual void setXOffset(double xOffset) = 0;
  virtual double xOffset() const = 0;

  virtual void setYOffset(double yOffset) = 0;
  virtual double yOffset() const = 0;

  virtual void setXScale(double xScale) = 0;
  virtual double xScale() const = 0;

  virtual void setYScale(double yScale) = 0;
  virtual double yScale() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiImageBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiImageBackgroundTraits> OdGiImageBackgroundTraitsPtr;


/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiGroundPlaneBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiGroundPlaneBackgroundTraits);

  virtual void setColorSkyZenith(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorSkyZenith() const = 0;

  virtual void setColorSkyHorizon(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorSkyHorizon() const = 0;

  virtual void setColorUndergroundHorizon(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorUndergroundHorizon() const = 0;

  virtual void setColorUndergroundAzimuth(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorUndergroundAzimuth() const = 0;

  virtual void setColorGroundPlaneNear(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorGroundPlaneNear() const = 0;

  virtual void setColorGroundPlaneFar(const OdCmEntityColor& color) = 0;
  virtual OdCmEntityColor colorGroundPlaneFar() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiGroundPlaneBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiGroundPlaneBackgroundTraits> OdGiGroundPlaneBackgroundTraitsPtr;


/** \details
    This class allows Gi classes to implement sky backgrounds.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiSkyBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSkyBackgroundTraits);

  virtual void setSkyParameters(const OdGiSkyParameters& params) = 0;
  virtual void skyParameters(OdGiSkyParameters& params) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiSkyBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiSkyBackgroundTraits> OdGiSkyBackgroundTraitsPtr;


/** \details
    This class allows Gi classes to implement image base lighting backgrounds.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdGiIBLBackgroundTraits : public OdGiDrawableTraits
{
public:
  ODRX_DECLARE_MEMBERS(OdGiIBLBackgroundTraits);

  virtual void setEnable(bool bEnable) = 0;
  virtual bool enable() const = 0;

  virtual void setIBLImageName(const OdString& filename) = 0;
  virtual const OdString& IBLImageName() const = 0;

  virtual void setRotation(double rotation) = 0;
  virtual double rotation() const = 0;

  virtual void setDisplayImage(bool bDisplay) = 0;
  virtual bool displayImage() const = 0;

  virtual void setSecondaryBackground(OdDbStub* bgId) = 0;
  virtual OdDbStub* secondaryBackground() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiIBLBackgroundTraits object pointers.
*/
typedef OdSmartPtr<OdGiIBLBackgroundTraits> OdGiIBLBackgroundTraitsPtr;

#endif // _GIENVIRONMENT_H_INCLUDED_
