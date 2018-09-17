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



#ifndef OD_AVERENDER_H
#define OD_AVERENDER_H

#include "TD_PackPush.h"

#include "Ve/AveDefs.h"
#include "DbObjectId.h"

/** \details
    This class represents AVE preferences.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAvePref
{
public:
  OdAvePref();

  OdAve::RenderType m_RenderType;             // Description: Render type.
  OdAve::RenderDestination m_Destination;     // Description: Render destination.
  OdString m_Scene;                           // Description: Scene name.
  OdInt16 m_nSubSampling;                     // Description: Sub-Sampling (1 = Best, 8 = Fastest)
  double m_dLightIconScale;                   // Description: Light icon scale.
  double m_dSmoothingAngle;                   // Description: Smoothing angle (delta from coincident).
  double m_dPreviewObjectSize;                // Description: Preview object size.
  bool m_bSmoothShade;                        // Description: True to apply smooth shading.
  bool m_bShadows;                            // Description: True to render shadows.
  bool m_bApplyMaterials;                     // Description: True to apply materials.
  bool m_bCropWindow;                         // Description: True to crop window.
  bool m_bRenderCache;                        // Description: True to use render cache.
  bool m_bQueryForSelections;                 // Description: True to query for selections.
  bool m_bSkipRenderDlg;                      // Description: True to skip render dialog.
  double   m_dLightIntensity;                 // Description: Ambient light intensity
  OdUInt32 m_nLightColor;                     // Description: Ambient light color.
  OdString m_NorthUcsName;                    // Description: North UCS name.
  double   m_dNorthAngle;                     // Description: North angle.
  double   m_dAmbientFactor;                  // Description: Default ambient factor.       
  double   m_dDiffuseFactor;                  // Description: Default diffuse factor.
  double   m_dSpecularFactor;                 // Description: Default specular factor.
  double   m_dRoughness;                      // Description: Default roughness.
  OdUInt32 m_nDiffuseColor;                   // Description: Default diffuse.
  bool     m_bCurrentDiffuseColor;            // Description: True to use CECOLOR as diffuse color.
};

/** \details
    This class represents the AVE Photo options.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveFullOpt
{
public:
  OdAveFullOpt();

  OdAve::RenderQuality m_RenderQuality;        // Description: Render quality. 
  bool m_bDiscardBackFaces;                    // Description: True to discard back faces.
  bool m_bNegativeNormal;                      // Description: True if back faces have negative normals.
};

/** \details
    This class represents AVE scanline ("Render") options.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveScanlOpt
{
public:
  OdAveScanlOpt();

  OdAve::AntiAliasing m_AntiAliasing;                 // Description: Anti=Aliasing.
  OdAve::TextureMapSampling m_TextureMapSampling;     // Description: Texture Map Sampling.
  double m_dShadowDepthMinBias;                       // Description: Shadow depth minimum bias.
  double m_dShadowDepthMaxBias;                       // Description: Shadow depth maximum bias.
  double m_dAdaptSamplThreshold;                      // Description: Adaptive sampling threshold.
  bool m_bAdaptSampl;                                 // Description: True to use adaptive sampling.
  bool m_bDiscardBackFaces;                           // Description: True to discard back faces.
  bool m_bNegativeNormal;                             // Description: True if back faces have negative normals.
};
/** \details
    This class represents AVE raytrace options.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveRaytrOpt : public OdAveScanlOpt
{
public:
  OdAveRaytrOpt();

  OdInt16 m_nRayTreeMaxDepth;                         //  Ray tree depth.
  double m_dRayTreeCutoffThreshold;                   //  Ray tree cutoff threshold.
};
/** \details
    This class represents AVE render file options.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveRFileOpt
{
public:
  OdAveRFileOpt();

  OdInt16 m_nFileType;              // Description: File type: BMP=1, PCX=2, PostScript=3, TGA=4, TIFF=5
  OdInt16 m_nX;                     // Description: Image width in pixels.
  OdInt16 m_nY;                     // Description: Image height in pixels.
  OdInt16 m_nAspectRatio;           // Description: Width/height pixel size.
  OdInt16 m_nColorBits1;            // Description: Color Bits 1.
  OdInt16 m_nColorBits2;            // Description: Color Bits 2.
  bool m_bGrayscale;                // Description: True if grayscale.
  bool m_bCompressed;               // Description: True if compresssed.
  OdInt16 m_nOrientation;           // Description: True if Bottom to top
  OdInt16 m_nInterlace;             // Description: Interlace
  OdInt16 m_nPostScriptImgSize;     // Description: Image width in pixels for Postscript.
};

/** \details
    This class represents AVE fog options.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveFogOpt{
public:
  OdAveFogOpt();

  bool m_bFogEnabled;                 // Description:  True if fog is enabled.
  bool m_bFogBackground;              // Description:  True to apply fog to background.
  double m_dNearDistance;             // Description:  Where fog begins as percentage of distance to front clip plane.
  double m_dFarDistance;              // Description:  Where fog ends as percentage of distance to front clip plane.
  double m_dNearPercentage;           // Description:  Fog percentage at near distance.
  double m_dFarPercentage;            // Description:  Fog percentage at far distance.
  OdUInt32 m_Color;                   // Description:  Color of fog.
};

/** \details
    This class represents AVE Background Options.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveBGOpt
{
public:
  OdAveBGOpt();
  ~OdAveBGOpt();

  OdAve::BackgroundType m_BgType; // Description: Background type.
  OdAveBitmap* m_pImage;          // Description: Pointer to background image.
  bool m_bDefaultBackground;      // Description: True to use the default (editor) background.
  OdUInt32 m_SolidColor;          // Description: Solid background color.
  OdUInt32 m_GrColor1;            // Description: Gradient top color.
  OdUInt32 m_GrColor2;            // Description: Gradient middle color.
  OdUInt32 m_GrColor3;            // Description: Gradient bottom color.
  double m_dGrHorizon;            // Description: Position of gradient horizon (0 = Bottom, 1 = Top).
  double m_dGrHeight;             // Description: Gradient height (fraction of view).
  double m_dGrRotation;           // Description: Gradient rotation.
  bool m_bEnvBackground;          // Description: True to use envirionment background image. 
  OdString m_EnvImageName;        // Description: Environment image filename.
};

/** \details
    This class represents the AVE Render Options.
    
    Corresponding C++ library: TD_Ave

    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveRenderOption
{
public:
  /** \details
    Populates this RenderOption object with the render options from the
    specified OdDbDatabase. 
    \param pDb [in]  Pointer to the database.
    \returns
    Returns true if and only if there were render options in the specified database.
  */
  bool getRenderOption(OdDbDatabase* pDb);
  /** \details
    Writes the render options from this RenderOption object 
    to the specified OdDbDatabase. 
    \param pDb [in]  Pointer to the database.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  OdResult writeRenderOption(OdDbDatabase* pDb);

  OdAvePref     m_Pref;        // Description: Render preferences.
  OdAveFullOpt  m_FullOpt;     // Description: Photo options.
  OdAveScanlOpt m_ScanlOpt;    // Description: Scanline options.
  OdAveRaytrOpt m_RaytrOpt;    // Description: Raytrace options.
  OdAveRFileOpt m_RFileOpt;    // Description: Render file options.
  OdAveFogOpt   m_FogOpt;      // Description: Fog options.
  OdAveBGOpt    m_BGOpt;       // Description: Background options.
  
private:
  OdDbObjectId  m_ids[OdAve::kRndOptAmount];
};


#include "TD_PackPop.h"

#endif // OD_AVERENDER_H
