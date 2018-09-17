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
#ifndef __DG_VIEW_H__
#define __DG_VIEW_H__


#include "TD_PackPush.h"

#include "DgElement.h"
#include "DgModel.h"
#include "Ge/GePoint3d.h"
#include "Gs/GsDefs.h"
#include "Ge/GeExtents2d.h"
#include "AbstractViewPE.h"
#include "Ge/GeBoundBlock3d.h"

class OdDgElementIterator;
typedef OdSmartPtr<OdDgElementIterator> OdDgElementIteratorPtr;

/** \details
This interface offers the operations available for views in camera mode

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgViewAsPerspectiveCamera
{
public:
  //eye point
  virtual void getCameraPosition( OdGePoint3d& position ) const = 0;
  virtual void setCameraPosition( const OdGePoint3d& position ) = 0;

  //rotation
  virtual void getCameraRotation( OdGeMatrix3d& rotation ) const = 0;
  virtual void setCameraRotation( const OdGeMatrix3d& rotation ) = 0;

  //focal length
  virtual double getCameraFocalLength() const = 0;
  virtual void setCameraFocalLength( double length ) = 0;

  //this defines the visible rectangle at the focal plane
  virtual void getCameraVisibleRectangle( OdGeExtents2d& extent ) const = 0;
  virtual void setCameraVisibleRectangle( const OdGeExtents2d& extent ) = 0;

  //distance from the camera to its front (near) clipping plane
  virtual double getCameraFrontClippingDistance() const = 0;
  virtual void setCameraFrontClippingDistance( double distance ) = 0;

  //distance from the camera to its back (far) clipping plane
  virtual double getCameraBackClippingDistance() const = 0;
  virtual void setCameraBackClippingDistance( double distance ) = 0;

  //camera lens angle
  virtual double getCameraLensAngle() const = 0;
  virtual void setCameraLensAngle( double dAngle ) = 0;
};

/** \details
This interface offers the operations available for views in orthography

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgViewAsOrthography
{
public:
  //visible box (view coordinates)
  virtual void getOrthographyVisibleBox( OdGeExtents3d& box ) const = 0;
  virtual void setOrthographyVisibleBox( const OdGeExtents3d& box ) = 0;

  //rotation
  virtual void getOrthographyRotation( OdGeMatrix3d& rotation ) const = 0;
  virtual void setOrthographyRotation( const OdGeMatrix3d& rotation ) = 0;

  //eye point
  virtual OdGePoint3d getOrthographyPosition() const = 0;
  virtual void setOrthographyPosition( const OdGePoint3d& position ) = 0;

  //distance from the camera to its front (near) clipping plane
  virtual double getOrthographyFrontClippingDistance() const = 0;
  virtual void setOrthographyFrontClippingDistance( double distance ) = 0;

  //distance from the camera to its back (far) clipping plane
  virtual double getOrthographyBackClippingDistance() const = 0;
  virtual void setOrthographyBackClippingDistance( double distance ) = 0;
};

/** \details
1. There are 2 alternatives: parallel views and perspective views (virtual camera).
2. The methods related to camera are available only when camera is on (getUseCameraFlag() == true).
3. This class does not care about translation between parameters if the modes are switched.

Corresponding C++ library: TG_Db

      <group OdDg_Classes>
*/
class TG_EXPORT OdDgView : 
  public OdDgElement,
  public OdDgViewAsPerspectiveCamera,
  public OdDgViewAsOrthography
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgView);
public:

  enum OdDgDisplayMode
  {
    kWireFrame        = 0,
    kWireMesh         = 2,
    kHiddenLine       = 3,
    kFilledHiddenLine = 4,
    kConstantShade    = 5,
    kSmoothShade      = 6,
    kPhongShade       = 7
  };

public:
  //any view should refer to the model it displays
  OdDgElementId getModelId() const;
  void setModelId( const OdDgElementId& id );

  //any view is either named or owned to some view group (8 views per 1 group)
  bool isNamed() const;
  OdString getName() const;
  void setName(const OdString& name);

  //for unnamed view, that index stands for its index within hosting view group
  OdUInt16 getIndex() const;
  void setIndex(OdUInt16 val);

  //physical rectangle at the screen
  OdGsDCRect getViewRectangle() const;
  void setViewRectangle( const OdGsDCRect& rectangle );

  //any view is either in orthography mode (methods of OdDgViewAsOrthography are available) or camera mode (OdDgViewAsPerspectiveCamera)
  bool getUseCameraFlag() const;
  void setUseCameraFlag( bool value );

  //binary flags about how to display some specific things
  bool getVisibleFlag() const;
  void setVisibleFlag( bool );
  bool getFastCurveFlag() const;
  void setFastCurveFlag( bool );
  bool getHideTextsFlag() const;
  void setHideTextsFlag( bool );
  bool getHighQualityFontsFlag() const;
  void setHighQualityFontsFlag( bool );
  bool getShowLineWeightsFlag() const;
  void setShowLineWeightsFlag( bool );
  bool getShowPatternsFlag() const;
  void setShowPatternsFlag( bool );
  bool getShowTextNodesFlag() const;
  void setShowTextNodesFlag( bool );
  bool getShowDataFieldsFlag() const;
  void setShowDataFieldsFlag( bool );
  bool getShowGridFlag() const;
  void setShowGridFlag( bool );
  bool getShowLevelSymbologyFlag() const;
  void setShowLevelSymbologyFlag( bool );
  bool getShowPointsFlag() const;
  void setShowPointsFlag( bool );
  bool getShowConstructionFlag() const;
  void setShowConstructionFlag( bool );
  bool getShowDimensionsFlag() const;
  void setShowDimensionsFlag( bool );
  bool getFastCellsFlag() const;
  void setFastCellsFlag( bool );
  bool getDefinedFlag() const;
  void setDefinedFlag( bool );
  bool getShowFillsFlag() const;
  void setShowFillsFlag( bool );
  bool getShowRasterTextFlag() const;
  void setShowRasterTextFlag( bool );
  bool getShowAxisTriadFlag() const;
  void setShowAxisTriadFlag( bool );
  bool getOrientationDisplayFlag() const;
  void setOrientationDisplayFlag( bool );
  bool getViewRenderedFlag() const;
  void setViewRenderedFlag( bool );
  bool getShowBackgroundFlag() const;
  void setShowBackgroundFlag( bool );
  bool getShowBoundaryFlag() const;
  void setShowBoundaryFlag( bool );
  bool getFastBoundaryClipFlag() const;
  void setFastBoundaryClipFlag( bool );
  bool getUseDepthCueFlag() const;
  void setUseDepthCueFlag( bool );
  bool getInhibitDynamicsFlag() const;
  void setInhibitDynamicsFlag( bool );
  bool getShowShadowsFlag() const;
  void setShowShadowsFlag( bool );
  bool getShowTextureMapsFlag() const;
  void setShowTextureMapsFlag( bool );
  bool getShowHazeFlag() const;
  void setShowHazeFlag( bool );
  bool getUseTransparencyFlag() const;
  void setUseTransparencyFlag( bool );
  bool getIgnoreLineStylesFlag() const;
  void setIgnoreLineStylesFlag( bool );
  bool getAcceleratedFlag() const;
  void setAcceleratedFlag( bool );
  bool getPatternDynamicsFlag() const;
  void setPatternDynamicsFlag( bool );
  bool getHiddenLineFlag() const;
  void setHiddenLineFlag( bool );
  bool getShowTagsFlag() const;
  void setShowTagsFlag( bool );
  bool getDisplayEdgesFlag() const;
  void setDisplayEdgesFlag( bool );
  bool getDisplayHiddenEdgesFlag() const;
  void setDisplayHiddenEdgesFlag( bool );
  bool getOverrideBackgroundFlag() const;
  void setOverrideBackgroundFlag( bool );
  bool getShowClipFrontFlag() const;
  void setShowClipFrontFlag( bool );
  bool getShowClipBackFlag() const;
  void setShowClipBackFlag( bool );
  bool getShowClipVolumeFlag() const;
  void setShowClipVolumeFlag( bool );
  bool getUseDisplaySetFlag() const;
  void setUseDisplaySetFlag( bool );

  OdDgDisplayMode getDisplayMode() const;
  void setDisplayMode( OdDgDisplayMode uMode );

  //implementation of OdDgViewAsPerspectiveCamera
  void getCameraPosition( OdGePoint3d& position ) const;
  void setCameraPosition( const OdGePoint3d& position );
  void getCameraRotation( OdGeMatrix3d& rotation ) const;
  void setCameraRotation( const OdGeMatrix3d& rotation );
  double getCameraFocalLength() const;
  void setCameraFocalLength( double length );
  void getCameraVisibleRectangle( OdGeExtents2d& extent ) const;
  void setCameraVisibleRectangle( const OdGeExtents2d& extent );
  double getCameraFrontClippingDistance() const;
  void setCameraFrontClippingDistance( double distance );
  double getCameraBackClippingDistance() const;
  void setCameraBackClippingDistance( double distance );
  double getCameraLensAngle() const;
  void setCameraLensAngle( double dAngle );

  //implementation of OdDgViewAsOrthography
  void getOrthographyVisibleBox( OdGeExtents3d& box ) const;
  void setOrthographyVisibleBox( const OdGeExtents3d& box );
  void getOrthographyRotation( OdGeMatrix3d& rotation ) const;
  void setOrthographyRotation( const OdGeMatrix3d& rotation );
  double getOrthographyFrontClippingDistance() const;
  void setOrthographyFrontClippingDistance( double distance );
  double getOrthographyBackClippingDistance() const;
  void setOrthographyBackClippingDistance( double distance );
  OdGePoint3d getOrthographyPosition() const;
  void setOrthographyPosition( const OdGePoint3d& position );

  //to support its complexity;
  //indeed, the only sub-element is level mask (to mask some levels by this view)
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  bool getNamedFlag() const;
  void setNamedFlag( bool );

  OdDgElementId getVolumeClipElementId() const;
  void setVolumeClipElementId( const OdDgElementId& idClipElement );

  OdDgElementId getVolumeClipMaskElementId() const;
  void setVolumeClipMaskElementId( const OdDgElementId& idClipElement );

  bool isLevelVisible( const OdDgElementId& idLevel ) const;
};

typedef OdSmartPtr<OdDgView> OdDgViewPtr;

/** \details

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDisplayStyleViewPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgDisplayStyleViewPE);
  long        m_references;     //used by each object separately
public:

  OdDgDisplayStyleViewPE();
  ~OdDgDisplayStyleViewPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  virtual OdDgElementId getDisplayStyle(const OdDgView* obj ) const;
  virtual void          setDisplayStyle(const OdDgView* obj, const OdDgElementId& idDisplayStyle ) const;
};

typedef OdSmartPtr<OdDgDisplayStyleViewPE> OdDgDisplayStyleViewPEPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgViewGroup : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgViewGroup);
public:

  OdString getName() const;
  void setName(const OdString& name);

  OdString getDescription() const;
  void setDescription(const OdString& name);

  OdUInt32 getActiveLevelEntryId() const;
  void     setActiveLevelEntryId( OdUInt32 uLevelEntryId );

  OdUInt32 getActiveColor() const;
  void     setActiveColor( OdUInt32 uColor );

  OdUInt32 getActiveLineStyleId() const;
  void     setActiveLineStyleId( OdUInt32 uLineStyleId );

  OdUInt32 getActiveLineWeight() const;
  void     setActiveLineWeight( OdUInt32 uLineWeight );

  OdUInt32 getDwgDisplayOrder() const;
  void     setDwgDisplayOrder( OdUInt32 uDisplayOrder );

  /** \details
    Adds the specified view to this view group.

    \param pView [in]  Pointer to the view to add.
  */
  void add(OdDgViewPtr pView);

  /** \details
    Returns an Iterator object that can be used to traverse this view group.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted objects are skipped.
  */
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  OdDgElementId getModelId() const;
  void setModelId( const OdDgElementId& id );
};

typedef OdSmartPtr<OdDgViewGroup> OdDgViewGroupPtr;


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgViewGroupTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgViewGroupTable);
public:
  /** \details
    Adds the specified view group to this collection.

    \param pViewGroup [in]  Pointer to the view group to add.
  */
  void add(OdDgViewGroupPtr pViewGroup);

  /** \details
    Returns an Iterator object that can be used to traverse this collection object.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted objects are skipped.
  */
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  virtual OdDgElementId getAt( const OdString& name, bool getErasedRecord = false) const;

  virtual bool has( const OdDgElementId& elementId) const;

  /** \details
    Creates a view group for the specified model.

    \param pModel [in]  Pointer to the model for which a view group is created.
  */
  virtual void createViewGroupForModel( OdDgModelPtr pModel );
};

typedef OdSmartPtr<OdDgViewGroupTable> OdDgViewGroupTablePtr;


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgNamedViewTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgNamedViewTable);
public:

  /** \details
    Adds the specified saved (named) view to this collection.

    \param pView [in]  Pointer to the view to add.
  */
  void add(OdDgViewPtr pView);

  /** \details
    Returns an Iterator object that can be used to traverse this collection object.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted objects are skipped.
  */
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  virtual OdDgElementId getAt( const OdString& name, bool getErasedRecord = false) const;

  virtual bool has( const OdDgElementId& elementId) const;
};

typedef OdSmartPtr<OdDgNamedViewTable> OdDgNamedViewTablePtr;

//-------------------------------------------------------------------------------------------------

class TG_EXPORT OdDgAbstractViewPE : public OdAbstractViewPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAbstractViewPE);

  // Viewport methods:

  virtual OdGePoint2d lowerLeftCorner(const OdRxObject* pViewport) const;
  virtual OdGePoint2d upperRightCorner(const OdRxObject* pViewport) const;
  virtual void setViewport(OdRxObject* pViewport, const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight) const;
  virtual bool hasViewport(const OdRxObject* pViewport) const;

  // View methods:

  virtual OdGePoint3d target(const OdRxObject* pViewport)     const;
  virtual OdGeVector3d direction(const OdRxObject* pViewport) const;
  virtual OdGeVector3d upVector (const OdRxObject* pViewport) const;
  virtual double fieldWidth(const OdRxObject* pViewport)      const;
  virtual double fieldHeight(const OdRxObject* pViewport)     const;
  virtual bool isPerspective(const OdRxObject* pViewport)     const;
  virtual double viewTwist(const OdRxObject* pViewport)       const;

  virtual void setView( OdRxObject* pViewport,
                        const OdGePoint3d& target,
                        const OdGeVector3d& direction,
                        const OdGeVector3d& upVector,
                        double fieldWidth,
                        double fieldHeight,
                        bool isPerspective,
                        const OdGeVector2d& viewOffset = OdGeVector2d::kIdentity) const;

  virtual void setLensLength(OdRxObject* pViewport, double lensLength) const;
  virtual double lensLength(const OdRxObject* pViewport)               const;

  // Front and back clip

  virtual bool isFrontClipOn(const OdRxObject* pViewport)              const;
  virtual void setFrontClipOn(OdRxObject* pViewport, bool frontClip)   const;
  virtual bool isBackClipOn(const OdRxObject* pViewport)               const;
  virtual void setBackClipOn(OdRxObject* pViewport, bool backClip)     const;
  virtual bool isFrontClipAtEyeOn(const OdRxObject* pViewport)         const;
  virtual void setFrontClipAtEyeOn(OdRxObject* pViewport, bool frontClipAtEye) const;
  virtual double frontClipDistance(const OdRxObject* pViewport) const;
  virtual void setFrontClipDistance(OdRxObject* pViewport, double frontClipDistance) const;
  virtual double backClipDistance(const OdRxObject* pViewport) const;
  virtual void setBackClipDistance(OdRxObject* pViewport, double backClipDistance) const;

  // Render mode

  virtual void setRenderMode(OdRxObject* pViewport, OdDb::RenderMode renderMode) const;
  virtual OdDb::RenderMode renderMode(const OdRxObject* pViewport) const;

  // Visual style

  virtual void setVisualStyle(OdRxObject* pViewport, OdDbStub* visualStyleId) const;
  virtual OdDbStub* visualStyle(const OdRxObject* pViewport) const;

  // Background

  virtual void setBackground(OdRxObject* pViewport, OdDbStub* backgroundId) const;
  virtual OdDbStub* background(const OdRxObject* pViewport) const;

  // View extents

  virtual bool viewExtents(const OdRxObject* pViewport, OdGeBoundBlock3d& extents) const;

  // Utility methods

  virtual OdDbStub* compatibleCopyObject(const OdRxObject* pViewport, OdDbStub* pCopyObject) const;
};


#include "TD_PackPop.h"

#endif // __DG_VIEW_H__
