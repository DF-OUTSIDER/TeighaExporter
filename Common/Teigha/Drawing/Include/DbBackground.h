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

#ifndef _ODDBBACKGROUND_INCLUDED_
#define _ODDBBACKGROUND_INCLUDED_

#include "TD_PackPush.h"

#include "DbObject.h"

#define ODDB_BACKGROUND_DICTIONARY ACAD_BACKGROUND

/** \details
    This class is a base class for all background objects.
    Corresponding C++ library: SCENEOE

    \remarks
    Database-resident background objects are stored in the NamedObjectsDictionary
    under ACAD_BACKGROUND.
    
    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbBackground : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBackground);

  OdDbBackground();
  virtual ~OdDbBackground();

  /** \details
    Returns the OdGiDrawable for this Background object.
  */
  virtual OdGiDrawable* drawable();
};

typedef OdSmartPtr<OdDbBackground> OdDbBackgroundPtr;

/** \details
    This class is for a solid background.
    Corresponding C++ library: SCENEOE

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbSolidBackground : public OdDbBackground
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSolidBackground);

  OdDbSolidBackground();
  virtual ~OdDbSolidBackground();

  void setColorSolid(const OdCmEntityColor& color);
  OdCmEntityColor colorSolid() const;

  // OdDbObject overridden functions

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdRxObject overridden functions

  //virtual void copyFrom(const OdRxObject* pSource);

  // OdGiDrawable overridden functions

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  virtual OdGiDrawable::DrawableType drawableType() const;
};

typedef OdSmartPtr<OdDbSolidBackground> OdDbSolidBackgroundPtr;

/** \details
    This class is for a gradient background.
    Corresponding C++ library: SCENEOE

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbGradientBackground : public OdDbBackground
{
public:
  ODDB_DECLARE_MEMBERS(OdDbGradientBackground);

  OdDbGradientBackground();
  virtual ~OdDbGradientBackground();

  void setColorTop(const OdCmEntityColor& color);
  OdCmEntityColor colorTop() const;
  void setColorMiddle(const OdCmEntityColor& color);
  OdCmEntityColor colorMiddle() const;
  void setColorBottom(const OdCmEntityColor& color);
  OdCmEntityColor colorBottom() const;
  void setHorizon(double horizon);
  double horizon() const;
  void setHeight(double height);
  double height() const ;
  void setRotation(double rotation);
  double rotation() const;

  // OdDbObject overridden functions

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdRxObject overridden functions

  //virtual void copyFrom(const OdRxObject* pSource);

  // OdGiDrawable overridden functions

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  virtual OdGiDrawable::DrawableType drawableType() const;
};

typedef OdSmartPtr<OdDbGradientBackground> OdDbGradientBackgroundPtr;

/** \details
    This class is for an image background.
    Corresponding C++ library: SCENEOE

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbImageBackground : public OdDbBackground
{
public:

  ODDB_DECLARE_MEMBERS(OdDbImageBackground);

  OdDbImageBackground();
  virtual ~OdDbImageBackground();

  void setImageFilename(const OdString& filename);
  const OdString& imageFilename() const;
  void setFitToScreen(bool bFitToScreen);
  bool fitToScreen() const;
  void setMaintainAspectRatio(bool bMaintainAspectRatio);
  bool maintainAspectRatio() const;
  void setUseTiling(bool bUseTiling);
  bool useTiling() const;
  void setXOffset(double xOffset);
  double xOffset() const;
  void setYOffset(double yOffset);
  double yOffset() const;
  void setXScale(double xScale);
  double xScale() const;
  void setYScale(double yScale);
  double yScale() const;

  // OdDbObject overridden functions

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdRxObject overridden functions

  //virtual void copyFrom(const OdRxObject* pSource);

  // OdGiDrawable overridden functions

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  virtual OdGiDrawable::DrawableType drawableType() const;
};

typedef OdSmartPtr<OdDbImageBackground> OdDbImageBackgroundPtr;

/** \details
    This class implements a ground plane for a background object in a viewport.
    Corresponding C++ library: SCENEOE

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbGroundPlaneBackground : public OdDbBackground
{
public:
  ODDB_DECLARE_MEMBERS(OdDbGroundPlaneBackground);

  OdDbGroundPlaneBackground();
  virtual ~OdDbGroundPlaneBackground();

  void setColorSkyZenith(const OdCmEntityColor& color);
  OdCmEntityColor colorSkyZenith() const;
  void setColorSkyHorizon(const OdCmEntityColor& color);
  OdCmEntityColor colorSkyHorizon() const;
  void setColorUndergroundHorizon(const OdCmEntityColor& color);
  OdCmEntityColor colorUndergroundHorizon() const;
  void setColorUndergroundAzimuth(const OdCmEntityColor& color);
  OdCmEntityColor colorUndergroundAzimuth() const;
  void setColorGroundPlaneNear(const OdCmEntityColor& color);
  OdCmEntityColor colorGroundPlaneNear() const;
  void setColorGroundPlaneFar(const OdCmEntityColor& color);
  OdCmEntityColor colorGroundPlaneFar() const;

  // OdDbObject overridden functions

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdRxObject overridden functions

  //virtual void copyFrom(const OdRxObject* pSource);

  // OdGiDrawable overridden functions

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  virtual OdGiDrawable::DrawableType drawableType() const;
};

typedef OdSmartPtr<OdDbGroundPlaneBackground> OdDbGroundPlaneBackgroundPtr;

/** \details
    This class implements a sky for a background object in a viewport.
    Corresponding C++ library: SCENEOE

    \sa
    TD_Db
    <group OdDb_Classes> 
*/
class SCENEOE_EXPORT OdDbSkyBackground : public OdDbBackground
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSkyBackground);

  OdDbSkyBackground();
  virtual ~OdDbSkyBackground();

  void setSunId(const OdDbObjectId& sunId);
  OdDbObjectId sunId() const;

  // OdDbObject overridden functions

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdRxObject overridden functions

  //virtual void copyFrom(const OdRxObject* pSource);

  // OdGiDrawable overridden functions

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  virtual OdGiDrawable::DrawableType drawableType() const;
};

typedef OdSmartPtr<OdDbSkyBackground> OdDbSkyBackgroundPtr;

/** \details
    This class implements a image based lighting for a background object in a viewport.
    Corresponding C++ library: SCENEOE

    \sa
    TD_Db
    <group OdDb_Classes>
*/
class SCENEOE_EXPORT OdDbIBLBackground : public OdDbBackground
{
public:
  ODDB_DECLARE_MEMBERS(OdDbIBLBackground);

  OdDbIBLBackground();
  virtual ~OdDbIBLBackground();

  virtual void setEnable(bool bEnable);
  virtual bool enable() const;
  virtual void setIBLImageName(const OdString &filename);
  virtual const OdString &IBLImageName() const;
  virtual void setRotation(double rotation);
  virtual double rotation() const;
  virtual void setDisplayImage(bool bDisplay);
  virtual bool displayImage() const;
  virtual void setSecondaryBackground(const OdDbObjectId &bgId);
  virtual OdDbObjectId secondaryBackground() const;

  virtual void setDataFrom(const OdDbIBLBackground *pBkgnd);

  // OdDbObject overridden functions

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  // OdRxObject overridden functions

  //virtual void copyFrom(const OdRxObject* pSource);

  // OdGiDrawable overridden functions

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
  virtual OdGiDrawable::DrawableType drawableType() const;
};

typedef OdSmartPtr<OdDbIBLBackground> OdDbIBLBackgroundPtr;

SCENEOE_EXPORT OdDbObjectId oddbGetBackgroundDictionaryId(OdDbDatabase* pDb, bool createIfNotFound = false);
SCENEOE_EXPORT OdDbDictionaryPtr oddbGetBackgroundDictionary(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);

#include "TD_PackPop.h"

#endif // _ODDBBACKGROUND_INCLUDED_
