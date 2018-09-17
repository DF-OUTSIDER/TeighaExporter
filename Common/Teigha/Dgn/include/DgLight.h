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
#ifndef __DG_LIGHT_H__
#define __DG_LIGHT_H__

#include "TD_PackPush.h"

#include "DgExport.h"
#include "DgMacros.h"
#include "RxObject.h"
#include "DgCellHeader.h"
#include "DgCommonStructs.h"
#include "Gi/GiDrawable.h"

class OdDgCellHeader3d;

//=======================================================================================================
//                                         OdDgLight classes
//=======================================================================================================

struct OdDgLightParam
{
  OdString m_strParamName;
  OdString m_strParamValue;
};

/** \details
This class declares the methods for managing all types of lights.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLight
{
public:

  enum Type
  {
    kDistant  = 2,
    kPoint    = 3,
    kSpot     = 4,
    kArea     = 5,
    kOpenSky  = 6,
    kLastType = kOpenSky
  };

  virtual OdDgLight::Type getLightType() const = 0;

  virtual OdString getLightName() const = 0;
  virtual void     setLightName( const OdString& ppName ) = 0;

  virtual bool     getOnFlag() const = 0;
  virtual void     setOnFlag( bool bOn ) = 0;

  virtual void     setDefaultValues( bool bSetDefaultGeometry = true, double dCellScaleFactor = 1 ) = 0;

  virtual OdUInt32 getExtendedParamCount() const = 0;
  virtual OdDgLightParam getExtendedParam( OdUInt32 uIndex ) const = 0;
  virtual OdDgLightParam getExtendedParam( const OdString& strParamName ) const = 0;
  virtual void     setExtendedParam( OdUInt32 uIndex, const OdDgLightParam& param ) = 0;
  virtual void     setExtendedParam( const OdString& strParamName, const OdDgLightParam& param ) = 0;
  virtual void     addExtendedParam( const OdDgLightParam& param ) = 0;
  virtual void     deleteExtendedParam( OdUInt32 uIndex ) = 0;
  virtual void     deleteExtendedParam( const OdString& strParamName ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLightDistant object pointers.
*/
typedef OdSmartPtr<OdDgLight> OdDgLightPtr;

/** \details
This class declares the methods for managing LT_Distant type of lights.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLightDistant : public OdDgCellHeader3d, public OdDgLight
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLightDistant);

public:

  virtual DrawableType drawableType() const;

  virtual OdDgLight::Type getLightType() const;

  OdString getName() const;
  void     setName( const OdString& ppName );

  OdString getLightName() const;
  void setLightName( const OdString& ppName );

  bool getOnFlag() const;
  void setOnFlag( bool bOn );

  bool getSunLightFlag() const;
  void setSunLightFlag( bool bSet );

  double getIntensity() const;
  void setIntensity( double dIntensity );

  double getBrightness() const;
  void setBrightness( double dBrightness );

  bool getShadowOnFlag() const;
  void setShadowOnFlag( bool bShadowOn );

  OdUInt32 getShadowResolution() const;
  void setShadowResolution( OdUInt32 uShadowResolution );

  double getColorRed() const;
  void setColorRed( double val );

  double getColorGreen() const;
  void setColorGreen( double val );

  double getColorBlue() const;
  void setColorBlue( double val );

  OdUInt32 getExtendedParamCount() const;
  OdDgLightParam getExtendedParam( OdUInt32 uIndex ) const;
  OdDgLightParam getExtendedParam( const OdString& strParamName ) const;
  void     setExtendedParam( OdUInt32 uIndex, const OdDgLightParam& param );
  void     setExtendedParam( const OdString& strParamName, const OdDgLightParam& param );
  void     addExtendedParam( const OdDgLightParam& param );
  void     deleteExtendedParam( OdUInt32 uIndex );
  void     deleteExtendedParam( const OdString& strParamName );

  void     setDefaultValues( bool bSetDefaultGeometry = true, double dCellScaleFactor = 1 );
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLightDistant object pointers.
*/
typedef OdSmartPtr<OdDgLightDistant> OdDgLightDistantPtr;

/** \details
This class declares the methods for managing LT_Point type of lights.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLightPoint : public OdDgCellHeader3d, public OdDgLight
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLightPoint);

  virtual DrawableType drawableType() const;
  virtual OdDgLight::Type getLightType() const;

  OdString getName() const;
  void     setName( const OdString& ppName );

  OdString getLightName() const;
  void setLightName( const OdString& ppName );

  bool getOnFlag() const;
  void setOnFlag( bool bOn );

  double getIntensity() const;
  void setIntensity( double dIntensity );

  double getBrightness() const;
  void setBrightness( double dBrightness );

  bool getShadowOnFlag() const;
  void setShadowOnFlag( bool bShadowOn );

  OdUInt32 getShadowResolution() const;
  void setShadowResolution( OdUInt32 uShadowResolution );

  double getColorRed() const;
  void setColorRed( double val );

  double getColorGreen() const;
  void setColorGreen( double val );

  double getColorBlue() const;
  void setColorBlue( double val );

  bool getAttenuateOnFlag() const;
  void setAttenuateOnFlag( bool bAttenuateOn );

  double getAttenuationDistance() const;
  void setAttenuationDistance( double dAttenuationDistance );

  bool getIESDataOnFlag() const;
  void setIESDataOnFlag( bool bIESDataOn );

  double getIESRotation() const;
  void setIESRotation( double dIESRotation );

  OdString getIESFilename() const;
  void setIESFilename( const OdString& ppIESFilename );

  OdUInt32 getExtendedParamCount() const;
  OdDgLightParam getExtendedParam( OdUInt32 uIndex ) const;
  OdDgLightParam getExtendedParam( const OdString& strParamName ) const;
  void     setExtendedParam( OdUInt32 uIndex, const OdDgLightParam& param );
  void     setExtendedParam( const OdString& strParamName, const OdDgLightParam& param );
  void     addExtendedParam( const OdDgLightParam& param );
  void     deleteExtendedParam( OdUInt32 uIndex );
  void     deleteExtendedParam( const OdString& strParamName );

  void   setDefaultValues( bool bSetDefaultGeometry = true, double dCellScaleFactor = 1 );
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLightPoint object pointers.
*/
typedef OdSmartPtr<OdDgLightPoint> OdDgLightPointPtr;

/** \details
This class declares the methods for managing LT_Spot type of lights.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLightSpot : public OdDgCellHeader3d, public OdDgLight
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLightSpot);

  virtual DrawableType drawableType() const;
  virtual OdDgLight::Type getLightType() const;

  OdString getName() const;
  void     setName( const OdString& ppName );

  OdString getLightName() const;
  void setLightName( const OdString& ppName );

  bool getOnFlag() const;
  void setOnFlag( bool bOn );

  double getIntensity() const;
  void setIntensity( double dIntensity );

  double getBrightness() const;
  void setBrightness( double dBrightness );

  bool getShadowOnFlag() const;
  void setShadowOnFlag( bool bShadowOn );

  OdUInt32 getShadowResolution() const;
  void setShadowResolution( OdUInt32 uShadowResolution );

  double getColorRed() const;
  void setColorRed( double val );

  double getColorGreen() const;
  void setColorGreen( double val );

  double getColorBlue() const;
  void setColorBlue( double val );

  bool getAttenuateOnFlag() const;
  void setAttenuateOnFlag( bool bAttenuateOn );

  double getAttenuationDistance() const;
  void setAttenuationDistance( double dAttenuationDistance );

  bool getIESDataOnFlag() const;
  void setIESDataOnFlag( bool bIESDataOn );

  double getIESRotation() const;
  void setIESRotation( double dIESRotation );

  OdString getIESFilename() const;
  void setIESFilename( const OdString& ppIESFilename );

  double getConeAngle() const;
  void setConeAngle( double dConeAngle );

  double getDeltaAngle() const;
  void setDeltaAngle( double dDeltaAngle );

  OdUInt32 getExtendedParamCount() const;
  OdDgLightParam getExtendedParam( OdUInt32 uIndex ) const;
  OdDgLightParam getExtendedParam( const OdString& strParamName ) const;
  void     setExtendedParam( OdUInt32 uIndex, const OdDgLightParam& param );
  void     setExtendedParam( const OdString& strParamName, const OdDgLightParam& param );
  void     addExtendedParam( const OdDgLightParam& param );
  void     deleteExtendedParam( OdUInt32 uIndex );
  void     deleteExtendedParam( const OdString& strParamName );

  void   setDefaultValues( bool bSetDefaultGeometry = true, double dCellScaleFactor = 1 );
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLightSpot object pointers.
*/
typedef OdSmartPtr<OdDgLightSpot> OdDgLightSpotPtr;

/** \details
This class declares the methods for managing LT_Area type of lights.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLightArea : public OdDgCellHeader3d, public OdDgLight
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLightArea);

  virtual DrawableType drawableType() const;
  virtual OdDgLight::Type getLightType() const;

  OdString getName() const;
  void     setName( const OdString& ppName );

  OdString getLightName() const;
  void setLightName( const OdString& ppName );

  bool getOnFlag() const;
  void setOnFlag( bool bOn );

  double getIntensity() const;
  void setIntensity( double dIntensity );

  double getBrightness() const;
  void setBrightness( double dBrightness );

  bool getShadowOnFlag() const;
  void setShadowOnFlag( bool bShadowOn );

  OdUInt32 getShadowResolution() const;
  void setShadowResolution( OdUInt32 uShadowResolution );

  double getColorRed() const;
  void setColorRed( double val );

  double getColorGreen() const;
  void setColorGreen( double val );

  double getColorBlue() const;
  void setColorBlue( double val );

  bool getAttenuateOnFlag() const;
  void setAttenuateOnFlag( bool bAttenuateOn );

  double getAttenuationDistance() const;
  void setAttenuationDistance( double dAttenuationDistance );

  bool getIESDataOnFlag() const;
  void setIESDataOnFlag( bool bIESDataOn );

  double getIESRotation() const;
  void setIESRotation( double dIESRotation );

  OdString getIESFilename() const;
  void setIESFilename( const OdString& ppIESFilename );

  OdUInt32 getSampleCount() const;
  void setSampleCount( OdUInt32 uSampleCount );

  OdUInt32 getExtendedParamCount() const;
  OdDgLightParam getExtendedParam( OdUInt32 uIndex ) const;
  OdDgLightParam getExtendedParam( const OdString& strParamName ) const;
  void     setExtendedParam( OdUInt32 uIndex, const OdDgLightParam& param );
  void     setExtendedParam( const OdString& strParamName, const OdDgLightParam& param );
  void     addExtendedParam( const OdDgLightParam& param );
  void     deleteExtendedParam( OdUInt32 uIndex );
  void     deleteExtendedParam( const OdString& strParamName );

  void   setDefaultValues( bool bSetDefaultGeometry = true, double dCellScaleFactor = 1 );
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLightArea object pointers.
*/
typedef OdSmartPtr<OdDgLightArea> OdDgLightAreaPtr;

/** \details
This class declares the methods for managing LT_OpenSky type of lights.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLightOpenSky : public OdDgCellHeader3d, public OdDgLight
{
public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLightOpenSky);

  virtual DrawableType drawableType() const;
  virtual OdDgLight::Type getLightType() const;

  virtual OdString getLightName() const;
  virtual void     setLightName( const OdString& ppName );

  virtual bool     getOnFlag() const;
  virtual void     setOnFlag( bool bOn );

  OdString         getName() const;
  void             setName( const OdString& ppName );

  OdUInt32         getMinSamples() const;
  void             setMinSamples( OdUInt32 uMinSamples );

  OdUInt32         getMaxSamples() const;
  void             setMaxSamples( OdUInt32 uMaxSamples );

  OdUInt32         getExtendedParamCount() const;
  OdDgLightParam   getExtendedParam( OdUInt32 uIndex ) const;
  OdDgLightParam   getExtendedParam( const OdString& strParamName ) const;
  void             setExtendedParam( OdUInt32 uIndex, const OdDgLightParam& param );
  void             setExtendedParam( const OdString& strParamName, const OdDgLightParam& param );
  void             addExtendedParam( const OdDgLightParam& param );
  void             deleteExtendedParam( OdUInt32 uIndex );
  void             deleteExtendedParam( const OdString& strParamName );

  void             setDefaultValues( bool bSetDefaultGeometry = true, double dCellScaleFactor = 1 );
  OdUInt32         subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLightOpenSky object pointers.
*/
typedef OdSmartPtr<OdDgLightOpenSky> OdDgLightOpenSkyPtr;

/** \details
This class declares the methods for view background image.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgImageBackground : public OdDgElement
{
public:

  enum OdBackgroundType
  {
    kImageBackground = OdDgLight::kLastType + 1,
    kLastType = kImageBackground
  };

public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgImageBackground);

  virtual DrawableType drawableType() const;

  void             setImageFilename(const OdString& filename);
  OdString         imageFilename() const;

  bool             getOnFlag() const;
  void             setOnFlag( bool bSet );

  OdUInt32         subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgLightArea object pointers.
*/
typedef OdSmartPtr<OdDgImageBackground> OdDgImageBackgroundPtr;

/** \details
This class declares the methods for default white material.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgWhiteMaterial : public OdDgElement
{
public:

  enum OdBackgroundType
  {
    kWhiteMaterial = OdDgImageBackground::kLastType + 1,
    kLastType = kWhiteMaterial
  };

public:
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgWhiteMaterial);

  OdUInt32         subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgWhiteMaterial object pointers.
*/
typedef OdSmartPtr<OdDgWhiteMaterial> OdDgWhiteMaterialPtr;

#include "TD_PackPop.h"

#endif // __DG_LIGHT_H__
