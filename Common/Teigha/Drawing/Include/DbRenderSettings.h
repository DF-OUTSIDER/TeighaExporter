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

#ifndef _DbRenderSettings_h_Included_
#define _DbRenderSettings_h_Included_

#include "Gi/GiEnvironment.h"

/** \details
    This class is a container class for the properties relevent to generic high-fidelity renderers.
    Corresponding C++ library: SCENEOE

    \remarks
    Database-resident OdDbRenderSettings objects are stored in the NamedObjectsDictionary
    under ACAD_RENDER_SETTINGS. 
    
    In addition, the active RenderSettings are stored in the NamedObjectsDictionary 
    under ACAD_RENDER_ACTIVE_SETTINGS. 

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbRenderSettings : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbRenderSettings);

  OdDbRenderSettings();
  virtual ~OdDbRenderSettings();

  /** \details
    Returns the OdGiDrawable for this RenderSettings object.
  */
  virtual OdGiDrawable* drawable();

  /** \details
    Sets the name of this RenderSettings object.
    \param name [in]  Name.
    \returns
    Returns eOk if successful, or an appropriate error code if not.    
    \remarks
    No check is made for RenderSettings objects with duplicate names.
  */
  OdResult setName(const OdString& name);

  /** \details
    Returns the name of this RenderSettings object.
  */
  OdString name() const;

  /** \details
    Sets the description of this RenderSettings object.
    \param description [in]  Description.
  */
  void setDescription(const OdString& description);

  /** \details
    Returns the description of this RenderSettings object.
  */
  OdString description() const;

  /** \details
    Sets the DisplayIndex of this RenderSettings object.
    \param displayIndex [in]  DisplayIndex.
    \remarks
    The DisplayIndex specfies the display order in the UI of the entries in the ACAD_RENDER_SETTINGS dictionary.
    
    DisplayIndices need not be positive or consecutive, and are not intended for use as ID numbers.
  */
  void setDisplayIndex(int displayIndex);

  /** \details
    Returns the DisplayIndex of this RenderSettings object.
    \remarks
    The DisplayIndex specfies the display order in the UI of the entries in the ACAD_RENDER_SETTINGS dictionary.
    
    DisplayIndices need not be positive or consecutive, and are not intended for use as ID numbers.
  */
  int displayIndex() const;

  
  /** \details
    Controls the rendering of materials on a per-object basis for this RenderSettings object.
    \param materialsEnabled [in]  Controls MaterialsEnabled.
    \remarks
    If true, materials are rendered on a per-object basis. If false, all objects
    are rendered with the global material.
    
    The default value for MaterialsEnabled == true.  
  */
  void setMaterialsEnabled(bool materialsEnabled);

  /** \details
    Returns true if and only if materials are rendered on a per-object basis for this RenderSettings object.
    \remarks
    If true, materials are rendered on a per-object basis. If false, all objects
    are rendered with the global material.
    
    The default value for MaterialsEnabled == true.  
  */
  bool materialsEnabled() const;

  /** \details
    Controls the sampling of image textures for this RenderSettings object.
    \param textureSampling [in]  Controls TextureSampling.
  */
  void setTextureSampling(bool textureSampling);

  /** \details
    Returns true if and only if texture sampling is enabled for this RenderSettings object.
  */
  bool textureSampling() const;

  /** \details
    Controls the rendering of back-facing faces for this RenderSettings object.
    \param backFacesEnabled [in]  Controls BackFacesEnabled.
    \remarks
    Back-facing faces are rendered if and only if BackFacesEanbled == true;
    
    \remarks
    The default value for BackFacesEnabled == true;
  */
  void setBackFacesEnabled(bool backFacesEnabled);

  /** \details
    Returns true if and only if back-facing faces are rendered for this RenderSettings object.
  */
  bool backFacesEnabled() const;

  /** \details
    Controls the casting of shadows for this RenderSettings object.
    \param shadowsEnabled [in]  Controls ShadowsEnabled.
    \remarks
    Shadows are cast if and only if ShadowsEnabled == true;
    
    \remarks
    The default value for ShadowsEnabled == true;
  */
  void setShadowsEnabled(bool shadowsEnabled);

  /** \details
    Returns true if and only if shadows are cast for this RenderSettings object.
  */
  bool shadowsEnabled() const;


  /** \details
    Sets the full filename of the preview image for this RenderSettings object.
    \param filename [in]  Filename.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    If the PreviewImageFileName is blank, no preview image is associated with this RenderSettings object.
    The default value for PreviewImageFileName is blank.
  */
  OdResult setPreviewImageFileName(const OdString& filename);

  /** \details
    Returns the full filename of the preview image for this RenderSettings object.
    \remarks

    If the PreviewImageFileName is blank, no preview image is associated with this RenderSettings object.
    
  */
  OdString previewImageFileName() const;

  /** \details
    Controls the rendering of the diagnostic background for this RenderSettings object.
    \param diagnosticBackgroundEnabled [in]  Controls DiagnosticBackgroundEnabled.
    \remarks
    The diagnostic (checker) background overrides the background for this RenderSettings object.
    
    The default value of DiagnosticBackgroundEnabled == false.
  */
  void setDiagnosticBackgroundEnabled(bool diagnosticBackgroundEnabled);

  /** \details
    Return true if and only if the diagnostic background is enabled for this RenderSettings object.
  */
  bool diagnosticBackgroundEnabled() const;

  /** \details
    Sets flag which is identifies does this render settings are predefined or not.
    \param bSet [in]  Set flag.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setIsPredefined(bool bSet);

  /** \details
    Returns flag which is identifies does this render settings are predefined or not.
  */
  bool isPredefined() const;

  // OdDbObject functions

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdGiDrawable functions

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits)const;

  virtual bool operator==(const OdDbRenderSettings& settings);

  static OdSmartPtr<OdDbRenderSettings> castByClassName(OdRxObject* obj) {
    return ::odrxCastByClassName<OdDbRenderSettings>(obj, "AcDbRenderSettings");
  }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbRenderSettings object pointers.
*/
typedef OdSmartPtr<OdDbRenderSettings> OdDbRenderSettingsPtr;


/** \details
    This class is a container class for the properties relevent to mental ray renderers.

    \remarks
    Database-resident OdDbMentalRayRenderSettings objects are stored in the NamedObjectsDictionary
    under ACAD_RENDER_SETTINGS. 
    
    In addition, the active RenderSettings are stored in the NamedObjectsDictionary 
    under ACAD_RENDER_ACTIVE_SETTINGS. 

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbMentalRayRenderSettings : public OdDbRenderSettings
{
public:
  /** \details
    Shadow sampling multiplier for area lights.
  */
  enum ShadowSamplingMultiplier
  {
    kSamplingMultiplierZero         = 0, // 0
    kSamplingMultiplierOneEighth    = 1, // 1/8
    kSamplingMultiplierOneFourth    = 2, // 1/4
    kSamplingMultiplierOneHalf      = 3, // 1/2
    kSamplingMultiplierOne          = 4, // 1
    kSamplingMultiplierTwo          = 5  // 2
  };
public:
  ODDB_DECLARE_MEMBERS(OdDbMentalRayRenderSettings);

  OdDbMentalRayRenderSettings();

  virtual ~OdDbMentalRayRenderSettings();

  /** \details
    Controls the minimum and maximum sampling rates when rendering a pixel for this MentalRayRenderSettings object.
    \param min [in]  Minimum sampling rate. [-3 .. 5]
    \param max [in]  Maximum sampling rate. [-3 .. 5]
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    
    * Negative values enable subsampling.
    * -3 specifies one sample per 64 pixels
    * 5 specfies one sample per 1028 pixels
    * The default value of min == -1
    * The default value of max == 0
    * Min <= Max
  */
  OdResult setSampling(int min, int max);

  /** \details
    Returns the minimum and maximum sampling rates when rendering a pixel for this MentalRayRenderSettings object.
    \param min [out]  Receives the minimum sampling rate. [-3 .. 5]
    \param max [out]  Receives the maximum sampling rate. [-3 .. 5]
    \remarks
    * Negative values enable subsampling.
    * -3 specifies one sample per 64 pixels
    * 5 specfies one sample per 1028 pixels
    * The default value of min == -1
    * The default value of max == 0
  */
  void sampling(int& min, int& max) const;

  /** \details
    Sets the sampling filter parameters for this MentalRayRenderSettings object.
    \param filter [in]  Filter method.
    \param width [in]  Width of the filter area in pixels. [0.0 .. 8.0]
    \param height [in]  Height of the filter area in pixels. [0.0 .. 8.0]
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    The defaults are as follows:
    
    <table>
    Argument     Default
    filter     kBox
    width      1.0
    height     1.0
    </table>
  */
  OdResult setSamplingFilter(OdGiMrFilter filter, double width,
    double height);

  /** \details
    Returns the sampling filter parameters for this MentalRayRenderSettings object.
    \param filter [out]  Receives the filter method.
    \param width [out]  Receives the width of the filter area in pixels.
    \param height [out]  Receives the height of the filter area in pixels.
  */
  void SamplingFilter(OdGiMrFilter& filter, double& width, double& height)
    const;

  /** \details
    Sets the SamplingContrastColor for this MentalRayRenderSettings object.
    \param red [in]  Red color channel threshold. [0.0 .. 1.0]
    \param green [in]  Green channel threshold. [0.0 .. 1.0]
    \param blue [in]  Blue channel threshold. [0.0 .. 1.0]
    \param alpha [in]  Alpha channel threshold. [0.0 .. 1.0]
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Additional color camples will be taken if the difference between
    a sample and its surrounding samples exceeds the SamplingContrastColor.

    The default value for each color channel is 1.0.
    
  */
  OdResult setSamplingContrastColor(float red, float green, float blue, float alpha);

  /** \details
    Returns the SamplingContrastColor for this MentalRayRenderSettings object.
    \param red [out]  Receives the red color channel threshold.
    \param green [out]  Receives the green channel threshold.
    \param blue [out]  Receives the blue channel threshold. 
    \param alpha [out]  Receives the alpha channel threshold.
  */
  void samplingContrastColor(float& red, float& green, float& blue, float& alpha) const;

  /** \details
    Sets the method of computing ray-traced shadows for this MentalRayRenderSettings object.
    \remarks
    The default value for ShadowMode == krSimple.
  */
  void setShadowMode(OdGiMrShadowMode shadowMode);

  /** \details
    Returns the method of computing ray-traced shadows for this MentalRayRenderSettings object.
  */
  OdGiMrShadowMode shadowMode() const;

  /** \details
    Controls the generation of shadow maps for this MentalRayRenderSettings object.
    \param shadowMapsEnabled [in]  Controls shadow maps.
    \remarks
    The default value of ShadowMapsEnabled == true;
  */
  void setShadowMapsEnabled(bool shadowMapsEnabled);

  /** \details
    Returns true if and only if shadow maps are generated for this MentalRayRenderSettings object.
  */
  bool shadowMapsEnabled() const;

  /** \details
    Controls ray tracing for this MentalRayRenderSettings object.
    \param rayTracingEnabled [in]  Controls ray tracing.
    \remarks
    The default value of RayTracing == true;
  */
  void setRayTracingEnabled(bool rayTracingEnabled);

  /** \details
    Returns true if and only if ray tracing is enabled for this MentalRayRenderSettings object.
  */
  bool rayTracingEnabled() const;

  /** \details
    Sets the maximum ray trace depths for this MentalRayRenderSettings object.
    
    \param reflection [in]  Maximum number of ray reflections. 
    \param refraction [in]  Maximum number of ray refractions.
    \param sum [in]  Maximum number of ray reflections and refractions.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
\remarks
    The defaults are as follows:
    
    <table>
    Argument      Default
    reflection    2
    refraction    2
    sum           4
    </table>
  */
  OdResult setRayTraceDepth(int reflection, int refraction,
    int sum);

  /** \details
    Returns the maximum ray trace depths for this MentalRayRenderSettings object.
    
    \param reflection [out]  Receives the maximum number of ray reflections. 
    \param refraction [out]  Receives the maximum number of ray refractions.
    \param sum [out]  Receives the maximum number of ray reflections and refractions.
  */
  void rayTraceDepth(int& reflection, int& refraction, int& sum) const;

  /** \details
    Controls global illumination using photon mapping for this MentalRayRenderSettings object.
    \param globalIlluminationEnabled [in]  Controls GlobalIllumination.
    
    \remarks
    Global illumination permits indirect lighting effects such as color-bleeding.
    
    The default value of GlobalIllumination is false.
  */
  void setGlobalIlluminationEnabled(bool globalIlluminationEnabled);

  /** \details
    Returns true if and only if global illumination using photon mapping is 
    enabled for this MentalRayRenderSettings object.
  */
  bool globalIlluminationEnabled() const;

  /** \details
    Sets the number of photons used for computing global illumination
    near a sample point for this MentalRayRenderSettings object.
    \param numPhotons [in]  Number of photons.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
\remarks
    The default value for GISampleCount == 500
    \sa
    setGISampleRadius
    setGISampleRadiusEnabled
  */
  OdResult setGISampleCount(int numPhotons);

  /** \details
    Returns the number of photons used for computing global illumination
    near a sample point for this MentalRayRenderSettings object.
  */
  int giSampleCount() const;

  /** \details
    Controls the use of the GISampleRadius for computing global illumination 
    for this MentalRayRenderSettings object.
    \param giSampleRadiusEnabled [in]  Controls GISampleRadiusEnabled.
    \remarks
    If GISampleRadiusEnabled == false, a default, the model space extents
    are used to computer a default radius.
    
    The default value of GISampleRadiusEnabled == false.
  */
  void setGISampleRadiusEnabled(bool giSampleRadiusEnabled);

  /** \details
    Returns true if and only if the GISampleRadius is used for computing 
    global illumination for this MentalRayRenderSettings object.
  */
  bool giSampleRadiusEnabled() const;

  /** \details
    Sets the radius for computing the photon count for global illumination for this 
    MentalRayRenderSettings object.
    
    \param radius [in]  Sample radius.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    The default value for GISampleRadius == 1.0.
    
    \sa
    setGISampleCount
    setGISampleRadiusEnabled
  */
  OdResult setGISampleRadius(double radius);

  /** \details
    Returns the radius for computing the photon count for global illumination for this 
    MentalRayRenderSettings object.
  */
  double giSampleRadius() const;

  /** \details
    Sets the average the number of photons shot per light for this 
    MentalRayRenderSettings object.
    \param numPhotons [in]  Number of photons.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    The default value for GIPhotonsPerLight == 10,000.
  */
  OdResult setGIPhotonsPerLight(int numPhotons);

  /** \details
    Returns the average the number of photons shot per light for this 
    MentalRayRenderSettings object.
  */
  int giPhotonsPerLight() const;

  /** \details
    Sets the maximum photon trace depths for this MentalRayRenderSettings object.
    
    \param reflection [in]  Maximum number of photon reflections. 
    \param refraction [in]  Maximum number of photon refractions.
    \param sum [in]  Maximum number of photon reflections and refractions.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
\remarks
    The defaults are as follows:
    
    <table>
    Argument      Default
    reflection    5
    refraction    5
    sum           5
    </table>
  */
  OdResult setPhotonTraceDepth(int reflection, int refraction,
    int sum);

  /** \details
    Returns the maximum photon trace depths for this MentalRayRenderSettings object.
    
    \param reflection [out]  Receives the maximum number of photon reflections. 
    \param refraction [out]  Receives the maximum number of photon refractions.
    \param sum [out]  Receives the maximum number of photon reflections and refractions.
  */
  void photonTraceDepth(int& reflection, int& refraction, int& sum) const;

  /** \details
    Controls the application of final gathering for this MentalRayRenderSettings object.
    \param finalGatheringEnabled [in]  Controls final gathering.
    \remarks
    Final gathering permits indirect lighting effects such as color-bleeding.
  */
  void setFinalGatheringEnabled(bool finalGatheringEnabled);

  /** \details
    Returns true if and only if final gathering is enabled for this MentalRayRenderSettings object.
  */
  bool finalGatheringEnabled() const;

  /** \details
    Sets the final gathering ray count for this MentalRayRenderSettings object.
    \param fgRayCount [in]  Final gathering ray count.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
\remarks
    In order to compute indirect illumination of a scent,
    FGRayCount rays are shot into the scene for each final gathering point.
     
    The default value for FGRayCount = 1000.
  */
  OdResult setFGRayCount(int fgRayCount);

  /** \details
    Returns the final gathering ray count for this MentalRayRenderSettings object.
    
    \remarks
    In order to compute indirect illumination of a scent,
    FGRayCount rays are shot into the scene for each final gathering point.
  */
  int fgRayCount() const;

  /** \details
    Controls the Final Gathering Sample Radius State for this MentalRayRenderSettings object.
    
    \param useMin [in]  True to use user-supplied minimum radius, false for default radius.
    \param useMax [in]  True to use user-supplied maximum radius, false for default radius.
    \param inPixels [in]  True if user-supplied radii are in pixels, false for drawing units.
    \remarks
    The default values for all parameters == false.
  */
  void setFGRadiusState(bool useMin, bool useMax, bool inPixels);

  /** \details
    Returns the the Final Gathering Sample Radius State for this MentalRayRenderSettings object.
    
    \param useMin [out]  Receives true if the user-supplied minimum radius is used, false for the default,
    \param useMax [out]  Receives true if the user-supplied maximum radius is used, false for the default,
    \param inPixels [out]  True if user-supplied radii are in pixels, false for drawing units.
  */
  void fgSampleRadiusState(bool& useMin, bool& useMax, bool& inPixels) const;

  /** \details
    Sets the Final Gathering Sample Radii for this MentalRayRenderSettings object.
    \param minRadius [in]  Minimum radius.
    \param maxRadius [in]  Maximum radius.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Default values are are 0.1 for drawing units, and 0.5 for pixels.
    
    minRadius <= maxRadius 
  */
  OdResult setFGSampleRadius(double minRadius, double maxRadius);

  /** \details
    Returns the Final Gathering Sample Radii for this MentalRayRenderSettings object.
    \param minRadius [out]  Receives the minimum radius.
    \param maxRadius [in]  Receives the maximum radius.
  */
  void fgSampleRadius(double& minRadius, double& maxRadius) const;

  /** \details
    Sets the magnitude of indirect illumination for this MentalRayRenderSettings object.
    \param energyMultiplier [in]  Energy multiplier.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remark
    The default value for EnergyMultiplier is 1.0.
    
    EnergyMultiplier serves as a volume control for the photon emmision of every light.
  */
  OdResult setEnergyMultiplier(float energyMultiplier);

  /** \details
    Returns the magnitude of indirect illumination for this MentalRayRenderSettings object.
    \remarks
    EnergyMultiplier serves as a volume control for the photon emmision of every light.
  */
  float energyMultiplier() const;

  /** \details
    Sets the diagnostic image rendering mode for this MentalRayRenderSettings object.
    \param diagnosticMode [in]  Diagnostic mode.
  */
  void setDiagnosticMode(OdGiMrDiagnosticMode diagnosticMode);

  /** \details
    Returns the diagnostic image rendering mode for this MentalRayRenderSettings object.
    \param diagnosticMode [in]  Diagnostic mode.
  */
  OdGiMrDiagnosticMode diagnosticMode() const;

  /** \details
    Sets the Diagnostic Grid for this MentalRayRenderSettings object.
    \param diagnosticGridMode [in]  Coordinate system for the grid.
    \param gridSpace [in]  Distance between grid lines.
    
    \remarks
    The default mode is krObject, the default grid space is 10.0.
  */
  OdResult setDiagnosticGridMode(
    OdGiMrDiagnosticGridMode diagnosticGridMode, float gridSpace);

  /** \details
    Returns the Diagnostic Grid for this MentalRayRenderSettings object.
    \param diagnosticGridMode [out]  Receives the coordinate system for the grid.
    \param gridSpace [out]  Receives the distance between grid lines.
  */
  void diagnosticGridMode(
    OdGiMrDiagnosticGridMode& diagnosticGridMode, float& gridSpace) const;

  /** \details
    Sets the Diagnostic Photon Mode for this MentalRayRenderSettings object.
    \param diagnosticPhotonMode [in]  Diagnostic Photon Mode.
    \remarks
    The default value for DiagnosticPhotonMode == krDensity.
  */
  void setDiagnosticPhotonMode(
    OdGiMrDiagnosticPhotonMode diagnosticPhotonMode);

  /** \details
    Returns the Diagnostic Photon Mode for this MentalRayRenderSettings object.
  */
  OdGiMrDiagnosticPhotonMode diagnosticPhotonMode() const;

  /** \details
    Controls the Diagnostic Samples Mode for this MentalRayRenderSettings object.
    \param diagnosticSamplesMode [in]  Diagnostic Samples Mode
    \remarks
    The default value for DiagnosticSamplesMode == false.
  */
  void setDiagnosticSamplesMode(bool diagnosticSamplesMode);

  /** \details
    Returns the Diagnostic Samples Mode for this MentalRayRenderSettings object.
    \remarks
    The default value for DiagnosticSamplesMode == false.
  */
  bool diagnosticSamplesMode() const;

  /** \details
    Sets the Diagnostic BSP Mode for this MentalRayRenderSettings object.
    \param diagnosticBSPMode [in]  Diagnostic BSP Mode.
    \remarks
    The default value for DiagnosticBSPMode is krDepth.
  */
  void setDiagnosticBSPMode(OdGiMrDiagnosticBSPMode diagnosticBSPMode);

  /** \details
    Returns the Diagnostic BSP Mode for this MentalRayRenderSettings object.
    \param diagnosticBSPMode [in]  Diagnostic BSP Mode.
    \remarks
    The default value for DiagnosticBSPMode is krDepth.
  */
  OdGiMrDiagnosticBSPMode diagnosticBSPMode() const;

  /** \details
    Controls the creation of an MI file after rendering for this MentalRayRenderSettings object.
    \param exportMIEnabled [in]  Export MI Enabled.
    \remarks
    The default value for ExportMIEnabled == false.
    \sa
    setExportMIFileName
  */
  void setExportMIEnabled(bool exportMIEnabled);

  /** \details
    Returns true if and only if an MI file is created after rendering for this MentalRayRenderSettings object.
  */
  bool exportMIEnabled() const;

  /** \details
    Sets the full filename of the exported MI file for this MentalRayRenderSettings object.
    \param filename [in]  Filename.

    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    If the ExportMIFileName is blank, no MI file is associated with this MentalRayRenderSettings object.
    
    The default value for ExportMIFileName is blank.
  */
  OdResult setExportMIFileName(const OdString& filename);

  /** \details
    Returns the full filename of the exported MI file for this MentalRayRenderSettings object.
  */
  OdString exportMIFileName() const;

  /** \details
    Sets the image tile size used in rendering for this MentalRaySettings object.
    \param tileSize [in]  Tile size. [4..512]
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    Larger values decrease rendering time and update frequency during rendering.
    
    The default value for TileSize is 32.
  */
  OdResult setTileSize(int tileSize);

  /** \details
    Returns the image tile size used in rendering for this MentalRaySettings object.
  */
  int tileSize() const;

  /** \details
    Sets the tile order used in tile rendering for this MentalRaySettings object.
    \param tileOrder [in]  Tile order.
    \remarks
    The default value for TileOrder is krHilbert.
  */
  void setTileOrder(OdGiMrTileOrder tileOrder);

  /** \details
    Returns the tile order used in tile rendering for this MentalRaySettings object.
  */
  OdGiMrTileOrder tileOrder() const;

 
  /** \details
    Sets the maximum memory allocated for rendering for this MentalRaySettings object.
    \param memoryLimit [in]  Memory limit (MB) [>= 128]
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    \remarks
    The renderer will decrease performance so as to stay below the memory limit.
    
    The default value for MemoryLimit is 1048.
  */
  OdResult setMemoryLimit(int memoryLimit);

  /** \details
    Returns the maximum memory allocated for rendering for this MentalRaySettings object.
  */
  int memoryLimit() const;

  /** \details
    Sets the final gathering mode for this MentalRaySettings object.
    \param mode [in]  Final gathering mode.
    \remarks
    The default value for final gathering mode is krFinalGatherAuto.
  */
  OdResult setFinalGatheringMode(OdGiMrFinalGatheringMode mode);

  /** \details
    Returns the final gathering mode for this MentalRaySettings object.
  */
  OdGiMrFinalGatheringMode finalGatheringMode() const;

  /** \details
    Sets the light luminance scale factor for this MentalRaySettings object.
    \param fLuminance [in]  Luminance scale factor.
    \remarks
    The default value for light luminance scale factor is 1500.0.
  */
  OdResult setLightLuminanceScale(double fLuminance);

  /** \details
    Returns the light luminance scale factor for this MentalRaySettings object.
  */
  double lightLuminanceScale() const;

  /** \details
    Sets the MI mode for this MentalRaySettings object.
    \param eExportMIMode [in]  MI mode.
    \remarks
    The default value for MI mode is krExportMIOff.
  */
  OdResult setExportMIMode(OdGiMrExportMIMode eExportMIMode);

  /** \details
    Returns the MI mode for this MentalRaySettings object.
  */
  OdGiMrExportMIMode exportMIMode() const;

  /** \details
    Sets the shadow sampling multiplier for this MentalRaySettings object.
    \param multiplier [in]  Shadow sampling multiplier.
    \remarks
    The default value for shadow sampling multiplier is kSamplingMultiplierOne.
  */
  OdResult setShadowSamplingMultiplier(ShadowSamplingMultiplier multiplier);

  /** \details
    Returns the shadow sampling multiplier for this MentalRaySettings object.
  */
  ShadowSamplingMultiplier shadowSamplingMultiplier() const;

  // OdDbObject functions
  
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdGiDrawable functions
  
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual bool operator==(const OdDbMentalRayRenderSettings& settings);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbMentalRayRenderSettings object pointers.
*/
typedef OdSmartPtr<OdDbMentalRayRenderSettings> OdDbMentalRayRenderSettingsPtr;


/** \details
    This class is a container class for the properties relevent to rapid RT renderers.

    \remarks
    Database-resident OdDbRapidRTRenderSettings objects are stored in the NamedObjectsDictionary
    under ACAD_RENDER_RAPIDRT_SETTINGS.

    In addition, the active RenderSettings are stored in the NamedObjectsDictionary
    under ACAD_RENDER_ACTIVE_RAPIDRT_SETTINGS.

    \sa
    TD_Db
    <group OdDb_Classes>
*/
class SCENEOE_EXPORT OdDbRapidRTRenderSettings : public OdDbRenderSettings
{
public:
  /** \details
    Rendring quit condition.
  */
  enum RenderTarget
  {
    krLevel = 0, // Quit by level
    krTime,      // Quit by time
    krInfinite   // Quit never
  };
public:
  ODDB_DECLARE_MEMBERS(OdDbRapidRTRenderSettings);

  OdDbRapidRTRenderSettings();

  virtual ~OdDbRapidRTRenderSettings();

  /** \details
    Sets render target.
    \param target [in]  New render target value.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setRenderTarget(RenderTarget target);

  /** \details
    Returns currently set render target.
  */
  RenderTarget renderTarget() const;

  /** \details
    Sets render level.
    \param level [in]  New render level value.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setRenderLevel(int level);

  /** \details
    Returns currently set render level.
  */
  int renderLevel() const;

  /** \details
    Sets render time.
    \param time [in]  New render time value.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setRenderTime(int time);

  /** \details
    Returns currently set render time.
  */
  int renderTime() const;

  /** \details
    Sets render lighting model.
    \param mode [in]  New render lighting model value.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setLightingModel(OdGiLightingMode mode);

  /** \details
    Returns currently set render lighting model.
  */
  OdGiLightingMode lightingModel() const;

  /** \details
    Sets render filter type.
    \param type [in]  New render filter type value.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setFilterType(OdGiFilterType type);

  /** \details
    Returns currently set render filter type.
  */
  OdGiFilterType filterType() const;

  /** \details
    Sets render filter width.
    \param width [in]  New render filter width value.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setFilterWidth(float width);

  /** \details
    Returns currently set render filter width.
  */
  float filterWidth() const;

  /** \details
    Sets render filter height.
    \param height [in]  New render filter height value.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setFilterHeight(float height);

  /** \details
    Returns currently set render filter height.
  */
  float filterHeight() const;

  /** \details
    Sets render filter width and height.
    \param width [in]  New render filter width value.
    \param height [in]  New render filter height value.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult setFilterDimensions(float width, float height)
  {
    OdResult result = setFilterWidth(width);
    if (result == eOk)
      result = setFilterHeight(height);
    return result;
  }

  /** \details
    Returns currently set render filter width and height.
    \param width [out]  Render filter width value.
    \param height [out]  Render filter height value.
  */
  void filterDimensions(float &width, float &height) const
  {
    width = filterWidth();
    height = filterHeight();
  }

  // OdDbObject functions

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdGiDrawable functions

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual bool operator==(const OdDbRapidRTRenderSettings& settings);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbRapidRTRenderSettings object pointers.
*/
typedef OdSmartPtr<OdDbRapidRTRenderSettings> OdDbRapidRTRenderSettingsPtr;


/** \details

    \remarks
    Database-resident OdDbRenderEnvironment objects are stored in the NamedObjectsDictionary
    under ACAD_RENDER_ENVIRONMENT. 
    
    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbRenderEnvironment : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbRenderEnvironment);

  OdDbRenderEnvironment();
  virtual ~OdDbRenderEnvironment();

  void setFogEnabled(bool bEnable);

  bool fogEnabled() const;

  void setFogBackgroundEnabled(bool bEnable);

  bool fogBackgroundEnabled() const;

  void setFogColor(const OdCmEntityColor& color);

  OdCmEntityColor fogColor() const;

  OdResult setFogDensity(double dNear, double dFar);

  void fogDensity(double& dNear, double& dFar) const;

  OdResult setDistances(double dNear, double dFar);

  void distances(double& dNear, double& dFar) const;

  void setEnvironmentImageEnabled(bool bEnabled);

  bool environmentImageEnabled() const;

  OdResult setEnvironmentImageFileName(const OdString& sFileName);

  OdString environmentImageFileName() const;

  // OdDbObject functions

  OdGiDrawable* drawable();
  OdResult dwgInFields(OdDbDwgFiler* pFiler);
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdGiDrawable functions

  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits)const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbRenderEnvironment object pointers.
*/
typedef OdSmartPtr<OdDbRenderEnvironment> OdDbRenderEnvironmentPtr;


/** \details
    This class is a container for all global rendering properties. 

    \remarks
    Database-resident OdDbRenderGlobal objects are stored in the NamedObjectsDictionary
    under ACAD_RENDER_GLOBAL.
    
    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbRenderGlobal : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbRenderGlobal);

  enum Procedure
  {
    krView = 0,
    krCrop,
    krSelected
  };

  enum Destination
  {
    krWindow = 0,
    krViewport
  };

  OdDbRenderGlobal();
  virtual ~OdDbRenderGlobal();

  void setProcedureAndDestination(Procedure nProcedure, Destination nDestination);

  void procedureAndDestination(Procedure& nProcedure, Destination& nDestination) const;

  OdResult setProcedure(Procedure nProcedure);

  Procedure procedure();

  OdResult setDestination(Destination nDestination);

  Destination destination();

  void setSaveEnabled(bool bEnabled);

  bool saveEnabled() const;

  OdResult setSaveFileName(const OdString& sFileName);

  OdString saveFileName() const;

  OdResult setDimensions(int w, int h);

  void dimensions(int& w, int& h) const;

  void setPredefinedPresetsFirst(bool bPredefinedPresetsFirst);

  bool predefinedPresetsFirst() const;

  void setHighInfoLevel(bool bHighInfoLevel);

  bool highInfoLevel() const;

  OdResult setExposureType(OdGiMrExposureType type);

  OdGiMrExposureType exposureType() const;

  OdString dimensionName() const;

  // OdDbObject functions

  OdResult dwgInFields(OdDbDwgFiler* pFiler);
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbRenderGlobal object pointers.
*/
typedef OdSmartPtr<OdDbRenderGlobal> OdDbRenderGlobalPtr;


/** \details

    \remarks
    Database-resident OdDbRenderEntry objects are stored in the NamedObjectsDictionary
    under ACAD_RENDER_ENTRIES.
    
    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbRenderEntry : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbRenderEntry);

  OdDbRenderEntry();
  virtual ~OdDbRenderEntry();

  OdResult setImageFileName(const OdString &fileName);
  OdString imageFileName() const;

  void setDisplayIndex(int nDisplay);
  int displayIndex() const;

  OdResult setPresetName(const OdString &name);
  OdString presetName() const;

  OdResult setViewName(const OdString &name);
  OdString viewName() const;

  OdResult setDimensions(int w, int h);
  void dimensions(int &w, int &h) const;
  int dimensionW() const { int w, h; dimensions(w, h); return w; }
  int dimensionH() const { int w, h; dimensions(w, h); return h; }

  OdResult setStartTime(const OdDbDate &date);
  const OdDbDate &startTime() const;

  OdResult setEndTime(const OdDbDate &date);
  const OdDbDate &endTime() const;

  void setRenderTime(float fTime);
  float renderTime() const;

  void setMemoryAmount(int nAmount);
  int memoryAmount() const;

  void setMaterialCount(int nCount);
  int materialCount() const;

  void setLightCount(int nCount);
  int lightCount() const;

  void setTriangleCount(int nCount);
  int triangleCount() const;

  // OdDbObject functions

  OdResult dwgInFields(OdDbDwgFiler* pFiler);
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbRenderEntry object pointers.
*/
typedef OdSmartPtr<OdDbRenderEntry> OdDbRenderEntryPtr;


TOOLKIT_EXPORT OdDbObjectId oddbGetRenderSettingsDictionaryId(OdDbDatabase* pDb, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbDictionaryPtr oddbGetRenderSettingsDictionary(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbObjectId oddbGetRenderPlotSettingsDictionaryId(OdDbDatabase* pDb, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbDictionaryPtr oddbGetRenderPlotSettingsDictionary(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbObjectId oddbGetRenderGlobalObjectId(OdDbDatabase* pDb, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbRenderGlobalPtr oddbGetRenderGlobalObject(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbObjectId oddbGetRenderEnvironmentObjectId(OdDbDatabase* pDb, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbRenderEnvironmentPtr oddbGetRenderEnvironmentObject(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbObjectId oddbGetActiveRenderSettingsObjectId(OdDbDatabase* pDb, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbRenderSettingsPtr oddbGetActiveRenderSettingsObject(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbObjectId oddbGetRenderEntriesDictionaryId(OdDbDatabase* pDb, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbDictionaryPtr oddbGetRenderEntriesDictionary(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbObjectId oddbGetRenderRapidRTSettingsDictionaryId(OdDbDatabase* pDb, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbDictionaryPtr oddbGetRenderRapidRTSettingsDictionary(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbObjectId oddbGetActiveRenderRapidRTSettingsObjectId(OdDbDatabase* pDb, bool createIfNotFound = false);
TOOLKIT_EXPORT OdDbRenderSettingsPtr oddbGetActiveRenderRapidRTSettingsObject(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);

#endif
