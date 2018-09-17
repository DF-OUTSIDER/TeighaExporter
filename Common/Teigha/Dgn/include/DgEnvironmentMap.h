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
#ifndef __DG_ENVIRONMENTMAP_H__
#define __DG_ENVIRONMENTMAP_H__


#include "TD_PackPush.h"
#include "DgElement.h"

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvMapBackground
{
public:

  enum OdDgEnvMapBackgroungMode
  {
    kNone  = 0,
    kColor = 1,
    kImage = 2
  };

public:

  OdDgEnvMapBackground();

  OdDgEnvMapBackgroungMode getMode() const;
  void setMode( const OdDgEnvMapBackgroungMode& uMode );

  ODCOLORREF getColor() const;
  void  setColor( ODCOLORREF clrColor );

  OdString getImageFileName() const;
  void     setImageFileName( const OdString& strFileName );

private:

  OdDgEnvMapBackgroungMode m_uMode;
  ODCOLORREF               m_clrColor;
  OdString                 m_strFileName;

};

//------------------------------------------------------------------

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvMapFog
{
public:

  enum OdDgEnvMapFogMode
  {
    kExponential  = 0,
    kLinear       = 1
  };

public:

  OdDgEnvMapFog();

  OdDgEnvMapFogMode getMode() const;
  void setMode( const OdDgEnvMapFogMode& uMode );

  ODCOLORREF getColor() const;
  void  setColor( ODCOLORREF clrColor );

  double getBrightness() const;
  void  setBrightness( double dBrightness );

  double getFogStart() const;
  void  setFogStart( double dStart );

  double getFogEnd() const;
  void  setFogEnd( double dEnd );

  double getFogDensity() const;
  void  setFogDensity( double dDensity );

  bool  getUseEnvironmentColorForFogFlag() const;
  void  setUseEnvironmentColorForFogFlag( bool bSet );

private:

  OdDgEnvMapFogMode m_uMode;
  ODCOLORREF        m_clrColor;
  double            m_dBrightness;
  double            m_dStart;
  double            m_dEnd;
  double            m_dDensity;
  bool              m_bUseEnvColorForFog;

};

//------------------------------------------------------------------

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvMapSkyLight
{
public:

  OdDgEnvMapSkyLight();

  double getSolarSize() const;
  void   setSolarSize( double dSolarSize );

private:

  double m_dSolarSize;
};

//------------------------------------------------------------------

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvMapImage
{
public:
  enum OdDgEnvMapProjectionType
  {
    kPlanar           = 2,
    kCylindrical      = 3,
    kSpherical        = 4,
    kCubic            = 5,
    kFront            = 6,
    kLightProbe       = 8  // It's projection of light probe level section
  };

  enum OdDgEnvMapFilterType
  {
    kNearest  = 0,
    kBilinear = 1,
    kBicubic  = 2
  };

public:

  OdDgEnvMapImage();

  OdDgEnvMapProjectionType getProjectionType() const;
  void                     setProjectionType( const OdDgEnvMapProjectionType& uType );

  bool  getMirrorFlag() const;
  void  setMirrorFlag( bool bSet );

  bool  getAutoCorrectionGammaFlag() const;
  void  setAutoCorrectionGammaFlag( bool bSet );

  bool  getUseAntialiasingFlag() const;
  void  setUseAntialiasingFlag( bool bSet );

  OdDgEnvMapFilterType getFilterType() const;
  void                 setFilterType( const OdDgEnvMapFilterType& uType );

  double getGamma() const;
  void   setGamma( double dGamma );

  double getAntialiasingStrength() const;
  void   setAntialiasingStrength( double dValue );

  OdString getImageFileName() const;
  void     setImageFileName( const OdString& strFileName );

  OdGePoint3d getOrigin() const;
  void        setOrigin( const OdGePoint3d& ptOrigin );

  OdGeVector3d getRotation() const;
  void         setRotation( const OdGeVector3d& vrRotation );

  OdGeVector3d getScale() const;
  void         setScale( const OdGeVector3d& vrScale );

private:

  OdDgEnvMapProjectionType m_uProjectionType;
  OdDgEnvMapFilterType     m_uFilterType;
  bool                     m_bMirror;
  bool                     m_bAutoGamma;
  bool                     m_bUseAntialiasing;
  double                   m_dGamma;
  double                   m_dAntialiasing;
  OdString                 m_strFileName;
  OdGePoint3d              m_ptOrigin;
  OdGeVector3d             m_vrRotation;
  OdGeVector3d             m_vrScale;

};

//------------------------------------------------------------------

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvMapImageCube
{
public:

  OdDgEnvMapImageCube();

  bool  getAutoCorrectionGammaFlag() const;
  void  setAutoCorrectionGammaFlag( bool bSet );

  bool  getUseAntialiasingFlag() const;
  void  setUseAntialiasingFlag( bool bSet );

  double getRotation() const;
  void   setRotation( double dRotation );

  double getGamma() const;
  void   setGamma( double dGamma );

  double getAntialiasingStrength() const;
  void   setAntialiasingStrength( double dValue );

  OdDgEnvMapImage::OdDgEnvMapFilterType getFilterType() const;
  void                 setFilterType( const OdDgEnvMapImage::OdDgEnvMapFilterType& uType );

  OdString getFrontFileName() const;
  void     setFrontFileName( const OdString& strFileName );

  OdString getBackFileName() const;
  void     setBackFileName( const OdString& strFileName );

  OdString getTopFileName() const;
  void     setTopFileName( const OdString& strFileName );

  OdString getBottomFileName() const;
  void     setBottomFileName( const OdString& strFileName );

  OdString getLeftFileName() const;
  void     setLeftFileName( const OdString& strFileName );

  OdString getRightFileName() const;
  void     setRightFileName( const OdString& strFileName );

private:

  OdDgEnvMapImage::OdDgEnvMapFilterType     m_uFilterType;
  bool                     m_bAutoGamma;
  bool                     m_bUseAntialiasing;
  double                   m_dGamma;
  double                   m_dAntialiasing;
  OdString                 m_strFrontFileName;
  OdString                 m_strBackFileName;
  OdString                 m_strTopFileName;
  OdString                 m_strBottomFileName;
  OdString                 m_strLeftFileName;
  OdString                 m_strRightFileName;
  double                   m_dRotation;
};

//------------------------------------------------------------------

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvMapGradient
{
public:

  enum OdDgEnvMapGradientMode
  {
    k4Colors = 0,
    k2Colors = 1
  };

public:

  OdDgEnvMapGradient();

  double getGroundExponent() const;
  void   setGroundExponent( double dExponent );

  double getSkyExponent() const;
  void   setSkyExponent( double dExponent );

  ODCOLORREF getZenithColor() const;
  void       setZenithColor( ODCOLORREF clrColor );

  ODCOLORREF getNadirColor() const;
  void       setNadirColor( ODCOLORREF clrColor );

  ODCOLORREF getSkyColor() const;
  void       setSkyColor( ODCOLORREF clrColor );

  ODCOLORREF getGroundColor() const;
  void       setGroundColor( ODCOLORREF clrColor );

  OdDgEnvMapGradientMode getGradientMode() const;
  void setGradientMode( OdDgEnvMapGradientMode uMode );

private:

  double m_dSkyExponent;
  double m_dGroundExponent;
  OdDgEnvMapGradientMode m_uMode;
  ODCOLORREF m_clrZenith;
  ODCOLORREF m_clrNadir;
  ODCOLORREF m_clrSky;
  ODCOLORREF m_clrGround;

};

//------------------------------------------------------------------

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvMapLayer
{
public:

  enum OdDgEnvMapLayerType
  {
    kSkyLight   = 0,
    kLightProbe = 1,
    kImage      = 2,
    kImageCube  = 3,
    kGradient   = 4
  };

public:

  OdDgEnvMapLayer();

  double getIntensityExponent() const;
  void   setIntensityExponent( double dExponent );

  bool   getUseLuxIntensityFlag() const;
  void   setUseLuxIntensityFlag( bool bSet );

  double getLuxIntensity() const;
  void   setLuxIntensity( double dIntensity );

  OdDgEnvMapLayerType getLayerType() const;
  void setLayerType( const OdDgEnvMapLayerType& uType );

  OdDgEnvMapSkyLight getSkyLight() const;
  void setSkyLight( const OdDgEnvMapSkyLight& skyLight );

  OdDgEnvMapImage getLightProbe() const;
  void setLightProbe( const OdDgEnvMapImage& lightProbe );

  OdDgEnvMapImage getImage() const;
  void setImage( const OdDgEnvMapImage& image );

  OdDgEnvMapImageCube getImageCube() const;
  void setImageCube( const OdDgEnvMapImageCube& imageCube );

  OdDgEnvMapGradient getGradient() const;
  void setGradient( const OdDgEnvMapGradient& gradient );

private:

  double m_dIntensityExponent;
  double m_dLuxIntensity;
  bool   m_bUseLuxIntensity;
  OdDgEnvMapLayerType m_uType;
  OdDgEnvMapSkyLight  m_skyLight;
  OdDgEnvMapImage     m_lightProbe;
  OdDgEnvMapImage     m_image;
  OdDgEnvMapImageCube m_imageCube;
  OdDgEnvMapGradient  m_gradient;
};

//------------------------------------------------------------------

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvironmentMap
{
public:

  OdDgEnvironmentMap();

  bool   getUntitledFlag() const;
  void   setUntitledFlag( bool bSet );

  bool   getEnableFlag() const;
  void   setEnableFlag( bool bSet );

  bool   getVisCameraFlag() const;
  void   setVisCameraFlag( bool bSet );

  bool   getVisIndirectFlag() const;
  void   setVisIndirectFlag( bool bSet );

  bool   getUseIndirectLightsFlag() const;
  void   setUseIndirectLightsFlag( bool bSet );

  bool   getReflectFlag() const;
  void   setReflectFlag( bool bSet );

  bool   getRefractFlag() const;
  void   setRefractFlag( bool bSet );

  bool   getUseIndirectLayerAsMainFlag() const;
  void   setUseIndirectLayerAsMainFlag( bool bSet );

  bool   getBackgroundEnabledFlag() const;
  void   setBackgroundEnabledFlag( bool bSet );

  bool   getFogEnabledFlag() const;
  void   setFogEnabledFlag( bool bSet );

  OdString getMapName() const;
  void     setMapName( const OdString& strName );

  OdDgEnvMapFog getFog() const;
  void setFog( const OdDgEnvMapFog& fog );

  OdDgEnvMapBackground getBackground() const;
  void setBackground( const OdDgEnvMapBackground& background );

  OdDgEnvMapLayer getMainLayer() const;
  void setMainLayer( const OdDgEnvMapLayer& layer );

  OdDgEnvMapLayer getIndirectLayer() const;
  void setIndirectLayer( const OdDgEnvMapLayer& layer );

private:
  bool m_bUntitled;
  bool m_bEnable;
  bool m_bVisCamera;
  bool m_bVisIndirect;
  bool m_bReflect;
  bool m_bRefract;
  bool m_bUseIndirectLayer;
  bool m_bBackgroundEnabled;
  bool m_bFogEnabled;
  bool m_bUseIndirectLights;
  OdString m_strName;
  OdDgEnvMapFog m_fog;
  OdDgEnvMapBackground m_background;
  OdDgEnvMapLayer m_layerMain;
  OdDgEnvMapLayer m_layerIndirect;
};

/** \details
  Corresponding C++ library: TG_Db
  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvironmentMapTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgEnvironmentMapTable);
public:

  virtual OdUInt32 getMapCount() const;
  virtual OdDgEnvironmentMap getMapAt( OdUInt32 uIndex ) const;
  virtual OdDgEnvironmentMap getMapAt( const OdString& name ) const;
  virtual void setMapAt( OdUInt32 uIndex, const OdDgEnvironmentMap& envMap );
  virtual void addMap(const OdDgEnvironmentMap& envMap);
  virtual void deleteMap( OdUInt32 uIndex );
  virtual void deleteMap( const OdString& name );
  virtual bool has( const OdString& name ) const;

};

typedef OdSmartPtr<OdDgEnvironmentMapTable> OdDgEnvironmentMapTablePtr;


#include "TD_PackPop.h"

#endif // __DG_ENVIRONMENTMAP_H__
