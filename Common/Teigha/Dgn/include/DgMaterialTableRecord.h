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
#ifndef __DG_MATERIALTABLERECORD_H__
#define __DG_MATERIALTABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"
#include "DgModel.h"
#include "Gi/GiMaterial.h"

class OdDgMaterialPattern;
class OdDgMaterialLayerPattern;

/** \details
This class represents Dgn texture map.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
struct OdDoubleColor
{
  double dRed;
  double dGreen;
  double dBlue;
};

//----------------------------------------------------------

/** \details
This class represents Dgn texture map.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMaterialParam
{
public:
  OdDgMaterialParam();
  OdDgMaterialParam( const OdString& strName, const OdString& strValue, bool bComplex = false );

  void     set( const OdString& strName, const OdString& strValue, bool bComplex = false );

  OdString getParamName() const;
  void     setParamName( const OdString& strName );

  OdString getParamValue() const;
  void     setParamValue( const OdString& strValue );

  bool     getIsComplexParam() const;
  void     setIsComplexParam( bool bSet );

  OdUInt32          getChildCount() const;
  OdDgMaterialParam getChild( OdUInt32 uIndex ) const;
  void              setChild( OdUInt32 uIndex, const OdDgMaterialParam& childParam );
  void              addChild( const OdDgMaterialParam& childParam );
  void              deleteChild( OdUInt32 uIndex );
  void              deleteAllChilds();

private:
  OdString m_strName;
  OdString m_strValue;
  OdArray<OdDgMaterialParam> m_arrChildParams;
  bool     m_bComplexParam;
};

//----------------------------------------------------------

/** \details
This class represents Dgn texture map.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMaterialMap
{

public:

  enum MappingType
  {
    kParametric       = 0,
    kElevationDrape   = 1,
    kPlanar           = 2,
    kDirectionalDrape = 3,
    kCubic            = 4,
    kSpherical        = 5,
    kCylindrical      = 6
  };

  enum ScaleMode
  {
    kSurfaceMode      = 0,
    kMasterUnitMode   = 1,
    kSubUnitMode      = 2,
    kMetersMode       = 3,
    kMillimetrersMode = 4,
    kFeetMode         = 5,
    kInchesMode       = 6
  };

public:

  OdDgMaterialMap();
  ~OdDgMaterialMap();

  void getGiMaterialMap( OdGiMaterialMap& matMap ) const;
  void setGiMaterialMap( const OdGiMaterialMap& matMap );

  MappingType getMappingType() const;
  void        setMappingType( MappingType uType );

  ScaleMode   getTextureScaleMode() const;
  void        setTextureScaleMode( ScaleMode uMode );

  bool        getCappedCylinderFlag() const;
  void        setCappedCylinderFlag( bool bSet );

  bool        getTransparentBgFlag() const;
  void        setTransparentBgFlag( bool bSet );

  bool        getInvertBumpMapFlag() const;
  void        setInvertBumpMapFlag( bool bSet );

  OdGeVector3d getDirectionForDirectionDrapeMap() const;
  void         setDirectionForDirectionDrapeMap( OdGeVector3d vrDirection );

  double      getRotationAngleForDirectionDrapeMap() const;
  void        setRotationAngleForDirectionDrapeMap( double dAngle );

private:

  MappingType     m_uMappingType;
  ScaleMode       m_uScaleMode;
  bool            m_bCapped;
  bool            m_bTransparentBg;
  bool            m_bInvertBump;
  double          m_dDirectionDrapeAngle;
  OdGeVector3d    m_vrDirectionDrapeDir;
  OdGiMaterialMap m_materialMap;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgMaterialLayerPattern object pointers.
*/
typedef OdSmartPtr<OdDgMaterialLayerPattern> OdDgMaterialLayerPatternPtr;

/** \details
    This class represents abstract LineStyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMaterialLayerPattern : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdDgMaterialLayerPattern);

public:

  enum OdDgMaterialLayerPatternType
  {
    kLayerUnknown      = 0,
    kLayerNormal       = 1,
    kLayerAdd          = 2,
    kLayerSubtract     = 3,
    kLayerAlpha        = 4,
    kLayerGradient     = 5,
    kLayerImage        = 6,
    kLayerProcedure    = 7,
    kLayerGamma        = 8,
    kLayerTint         = 9,
    kLayerGroupStart   = 10,
    kLayerGroupEnd     = 11,
    kLayerAlphaBgStart = 12,
    kLayerAlphaBgEnd   = 13
  };

public:

  static OdDgMaterialLayerPatternPtr createObject( const OdDgMaterialLayerPatternPtr pObj );

  virtual void copyData( const OdDgMaterialLayerPatternPtr pObj );

  OdString         getLayerTypeStr() const;
  void             setLayerTypeStr( OdString strType );

  OdDgMaterialLayerPatternType getLayerType() const;

  OdDgMaterialMap::MappingType getMappingType() const;
  void setMappingType( OdDgMaterialMap::MappingType uType );

  OdDgMaterialMap::ScaleMode getScaleMode() const;
  void setScaleMode( OdDgMaterialMap::ScaleMode uMode );

  double           getPatternAngle() const;
  void             setPatternAngle( double dAngle );

  OdGeScale3d      getPatternScale() const;
  void             setPatternScale( OdGeScale3d scale );

  OdGePoint2d      getPatternOffset() const;
  void             setPatternOffset( OdGePoint2d ptOffset );

  double           getBlend() const;
  void             setBlend( double dBlend );

  OdUInt32         getLayerFlags() const;
  void             setLayerFlags( OdUInt32 uFlags );

  OdUInt32         getDataFlags() const;
  void             setDataFlags( OdUInt32 uFlags );

  double           getColorGamma() const;
  void             setColorGamma( double dColorGamma );
  OdDoubleColor    getBrightnessColor() const;
  void             setBrightnessColor( OdDoubleColor dblColor );

  OdUInt32         getUserDataCount() const;
  void             addUserData( OdString& strData );
  bool             removeUserData( OdUInt32 uIndex );
  OdString         getUserData( OdUInt32 uIndex ) const;
  bool             setUserData( OdUInt32 uIndex, OdString& strData );

  bool             getVisibleLayerFlag() const;
  void             setVisibleLayerFlag( bool bSet );

  bool             getTransparentLayerBackgroundFlag() const;
  void             setTransparentLayerBackgroundFlag( bool bSet );

  bool             getFlipYFlag() const;
  void             setFlipYFlag( bool bSet );

  bool             getFlipXFlag() const;
  void             setFlipXFlag( bool bSet );

  bool             getLockScaleFlag() const;
  void             setLockScaleFlag( bool bSet );

  bool             getNoRepeatXFlag() const;
  void             setNoRepeatXFlag( bool bSet );

  bool             getNoRepeatYFlag() const;
  void             setNoRepeatYFlag( bool bSet );

  bool             getMirrorXFlag() const;
  void             setMirrorXFlag( bool bSet );

  bool             getMirrorYFlag() const;
  void             setMirrorYFlag( bool bSet );

  virtual OdUInt32 getExtendedParamCount() const;
  virtual OdDgMaterialParam getExtendedParam( OdUInt32 uIndex ) const;
  virtual OdDgMaterialParam getExtendedParam( const OdString& strParamName ) const;
  virtual void     setExtendedParam( OdUInt32 uIndex, const OdDgMaterialParam& param );
  virtual void     setExtendedParam( const OdString& strParamName, const OdDgMaterialParam& param );
  virtual void     addExtendedParam( const OdDgMaterialParam& param );
  virtual void     deleteExtendedParam( OdUInt32 uIndex );
  virtual void     deleteExtendedParam( const OdString& strParamName );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgMaterialPattern object pointers.
*/
typedef OdSmartPtr<OdDgMaterialPattern> OdDgMaterialPatternPtr;

/** \details
    This class represents abstract LineStyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMaterialPattern : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgMaterialPattern);
public:

  enum OdDgMaterialPatternType
  {
    kUnknownPattern              = 0,
    kDiffusePattern              = 1,
    kBumpMapPattern              = 2,
// Only for materials, that used into Microstation V8i SS3
    kSpecularPattern             = 3,
    kReflectPattern              = 4,
    kOpacityPattern              = 5,
    kDiffuseMapPattern           = 6,
    kFinishPattern               = 7,
    kTranslucencyPattern         = 8,
    kGlowPattern                 = 9,
    kClearcoatPattern            = 10,
    kAnisotropyPattern           = 11,
    kSpecularColorPattern        = 12,
    kTransparentColorPattern     = 13,
    kSubSurfaceColorPattern      = 14,
    kDisplacementPattern         = 15,
    kNormalPattern               = 16,
    kFurLengthPattern            = 17,
    kFurDensityPattern           = 18,
    kFurJitterPattern            = 19,
    kFurFlexPattern              = 20,
    kFurClimpsPattern            = 21,
    kGlowColorPattern            = 26,
    kReflectColorPattern         = 27,
    kRefractionRoughnessPattern  = 28,
    kFrenselPattern              = 29,
    kGeometryPattern             = 30
  };

public:

  static OdDgMaterialPatternPtr createObject( OdDgMaterialPatternType uType);
  static OdDgMaterialPatternPtr createObject( const OdDgMaterialPatternPtr pObj );

  virtual void copyData( const OdDgMaterialPatternPtr pObj );

  OdDgMaterialPatternType          getType() const;
  void          setType( OdDgMaterialPatternType uType );

  OdUInt32      getFlags() const;
  void          setFlags( OdUInt32 uFlags );

  OdDgMaterialMap::MappingType   getMappingType() const;
  void          setMappingType( OdDgMaterialMap::MappingType uMappingType );

  OdDgMaterialMap::MappingType   getImageTextureMappingType() const;
  void          setImageTextureMappingType( OdDgMaterialMap::MappingType uMappingType );

  OdDgMaterialMap::ScaleMode     getScaleMode() const;
  void          setScaleMode( OdDgMaterialMap::ScaleMode uScaleMode );

  bool          getUsePattern() const;
  void          setUsePattern( bool bUsePattern );

  double        getPatternAngle() const;
  void          setPatternAngle( double dPatternAngle );

  OdGeScale3d   getPatternScale() const;
  void          setPatternScale( OdGeScale3d scale );

  OdGePoint2d   getPatternOffset() const;
  void          setPatternOffset( OdGePoint2d ptOffset );

  bool          getUseMapLink() const;
  void          setUseMapLink( bool bUseLink );

  OdUInt32      getPatternFlags() const;
  void          setPatternFlags( OdUInt32 uFlags );

  OdUInt32      getBumpFlags() const;
  void          setBumpFlags( OdUInt32 uFlags );

  double        getBumpScale() const;
  void          setBumpScale( double dScale );

  double        getPatternWeight() const;
  void          setPatternWeight( double dWeight );

  OdGeVector3d  getProjectionAngles() const;
  void          setProjectionAngles( OdGeVector3d vrAngles );

  OdGeScale3d   getProjectionScale() const;
  void          setProjectionScale( OdGeScale3d projScale );

  OdGePoint3d   getUFlip() const;
  void          setUFlip( OdGePoint3d ptUFlip );

  OdString      getFileName() const;
  void          setFileName( OdString strFileName );

  OdUInt32      getUserDataCount() const;
  void          addUserData( OdString& strData );
  bool          removeUserData( OdUInt32 uIndex );
  OdString      getUserData( OdUInt32 uIndex ) const;
  bool          setUserData( OdUInt32 uIndex, OdString& strData );

  OdUInt32      getPatternLayerCount() const;
  void          addPatternLayer( OdDgMaterialLayerPatternPtr pLayerData );
  bool          removePatternLayer( OdUInt32 uIndex );
  OdDgMaterialLayerPatternPtr getPatternLayer( OdUInt32 uIndex ) const;

  bool          isImageTexture() const;
  bool          isProceduralTexture() const;

  bool          getFlipXFlag() const;
  void          setFlipXFlag( bool bSet );

  bool          getFlipYFlag() const;
  void          setFlipYFlag( bool bSet );

  bool          getLockScaleFlag() const;
  void          setLockScaleFlag( bool bSet );

  bool          getCappedFlag() const;
  void          setCappedFlag( bool bSet );

  bool          getTransparentBackgroundFlag() const;
  void          setTransparentBackgroundFlag( bool bSet );

  bool          getInvertBumpFlag() const;
  void          setInvertBumpFlag( bool bSet );

  virtual OdUInt32 getExtendedParamCount() const;
  virtual OdDgMaterialParam getExtendedParam( OdUInt32 uIndex ) const;
  virtual OdDgMaterialParam getExtendedParam( const OdString& strParamName ) const;
  virtual void     setExtendedParam( OdUInt32 uIndex, const OdDgMaterialParam& param );
  virtual void     setExtendedParam( const OdString& strParamName, const OdDgMaterialParam& param );
  virtual void     addExtendedParam( const OdDgMaterialParam& param );
  virtual void     deleteExtendedParam( OdUInt32 uIndex );
  virtual void     deleteExtendedParam( const OdString& strParamName );
};

/** \details
    This class represents Material records in the OdDgMaterialTable in an OdDgDatabase instance.
 
    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMaterialTableRecord : public OdDgTableRecord
{
public:
  
  enum SpecularColorType
  {
    kPlastic   = 0, // Always white
    kMetallic  = 1, // Same as diffuse color
    kByElement = 2, // Same as element color
    kCustom    = 3  // Custom specular color
  };

  enum GlossFactorType
  {
    kGlossFactorSameAsSpacular = 0, // Gloss factor same as specular factor
    kGlossFactorOff            = 1, // Gloss factor = 1
    kGlossFactorCustom         = 2  // Custom gloss factor
  };

public:

 ODDG_DECLARE_MEMBERS_INTERNAL(OdDgMaterialTableRecord);

 OdUInt32         subSetAttributes( OdGiDrawableTraits* pTraits ) const;

 virtual OdString getPaletteName() const;
 virtual void     setPaletteName( OdString& strName );

 // Ambient color based on diffuse color.
 virtual void     getAmbient( OdGiMaterialColor& ambientColor ) const;
 virtual void     setAmbientFactor( double dFactor );

 virtual void     getDiffuse( OdGiMaterialColor& diffuseColor, OdDgMaterialMap& diffuseMap, bool asGenericTexture = false ) const;
 virtual void     setDiffuse( const OdGiMaterialColor& diffuseColor, const OdDgMaterialMap& diffuseMap );

 virtual void     getSpecular( OdGiMaterialColor& specularColor, double& dGlossFactor ) const;
 virtual void     setSpecular( const OdGiMaterialColor& specularColor, SpecularColorType iSpecularType, 
                               double dGlossFactor, GlossFactorType iGlossFactorType );

 virtual void     getReflection( OdGiMaterialMap& reflectionMap ) const;
 virtual void     setReflectionFactor( double dFactor, bool bSameAsSpecular );

 virtual double   getOpacity() const;
 virtual void     setOpacity( double dOpacity );

 virtual void     getBump( OdDgMaterialMap& bumpMap, bool asGenericTexture = false ) const;
 virtual void     setBump( const OdDgMaterialMap& bumpMap, bool bLinkToDiffuseMap );

 virtual double   getRefraction() const;
 virtual void     setRefraction( double dRefraction );

 virtual double   getTranslucence() const;
 virtual void     setTranslucence( double dTranslucence );

 virtual double   getThickness() const;
 virtual void     setThickness( double dThickness );

 virtual double   getShininess() const;
 virtual void     setShininess( double dShininess );

 virtual bool     getShadowCastFlag() const;
 virtual void     setShadowCastFlag( bool bSet );

 virtual OdUInt32 getVersion() const;
 virtual void     setVersion( OdUInt32 uVersion );

 virtual bool     getGlobalIlluminationFlag() const;
 virtual void     setGlobalIlluminationFlag( bool bSet );

 virtual bool     prepareTextureMapper( OdDgModelPtr pObjectModel, OdGePoint3d ptOrigin, 
                                        OdGePoint3d ptParamOrigin, OdGeVector3d vrXAxis, 
                                        OdGeVector3d vrYAxis ,OdGeVector3d vrZAxis, OdGiMapper& texMapper ) const;
 virtual bool     hasTexture() const;

 virtual bool     getUseDiffuseTextureFlag() const;
 virtual bool     getUseBumpMapTextureFlag() const;
 virtual void     setUseDiffuseTextureFlag( bool bSet );
 virtual void     setUseBumpMapTextureFlag( bool bSet );

 virtual OdUInt32 getExtendedParamCount() const;
 virtual OdDgMaterialParam getExtendedParam( OdUInt32 uIndex ) const;
 virtual OdDgMaterialParam getExtendedParam( const OdString& strParamName ) const;
 virtual void     setExtendedParam( OdUInt32 uIndex, const OdDgMaterialParam& param );
 virtual void     setExtendedParam( const OdString& strParamName, const OdDgMaterialParam& param );
 virtual void     addExtendedParam( const OdDgMaterialParam& param );
 virtual void     deleteExtendedParam( OdUInt32 uIndex );
 virtual void     deleteExtendedParam( const OdString& strParamName );

 virtual OdUInt32 getMaterialPatternCount() const;
 virtual OdDgMaterialPatternPtr getMaterialPattern( OdUInt32 uIndex ) const;
 virtual OdDgMaterialPatternPtr getMaterialPatternByType( OdDgMaterialPattern::OdDgMaterialPatternType uMapType ) const;
 virtual void     addMaterialPattern( const OdDgMaterialPatternPtr pMap );
 virtual void     deleteMaterialPattern( OdUInt32 uIndex );
 virtual void     deleteMaterialPatternByType( OdDgMaterialPattern::OdDgMaterialPatternType uMapType );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgMaterialTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgMaterialTableRecord> OdDgMaterialTableRecordPtr;


#include "TD_PackPop.h"

#endif // __DG_MATERIALTABLERECORD_H__
