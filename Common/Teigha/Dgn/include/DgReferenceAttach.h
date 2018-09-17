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

#ifndef __DG_REFERENCE_ATTACH_H__
#define __DG_REFERENCE_ATTACH_H__

#include "TD_PackPush.h"

#include "DgElement.h"
#include "DgElementIterator.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeMatrix3d.h"
#include "DgTable.h"
#include "DgModel.h"
#include "DgView.h"
#include "DgApplicationData.h"

#include "Gi/GiAbstractClipBoundary.h"


/*
namespace OdDg
{
  // Values for units options to define the linear units for DGN to use when opening .dwg files.
  enum DesignCenterUnits 
  {
      kSeedFileMasterUnits    // Specifies the master units in the .dgn seed file.
    , kSeedFileSubUnits       // Specifies the sub units in the .dgn seed file.
    , kDesignCenterUnits      // Specifies the units used in the .dwg file being opened.
    , kMicroinches 
    , kMils 
Inches 
Feet 
Yards 
Miles 
Micrometers 
Millimeters 
Centimeters 
Meters 
Kilometers 
  };
}
*/


/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgReferenceAttachmentViewFlags
{
  public:

    OdDgReferenceAttachmentViewFlags();
    OdDgReferenceAttachmentViewFlags( OdUInt32 uFlag1, OdUInt32 uFlag2 );

    OdUInt32 getFlags1() const;
    void     setFlags1( OdUInt32 uFlag );
    OdUInt32 getFlags2() const;
    void     setFlags2( OdUInt32 uFlag );

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
    bool getShowFillsFlag() const;
    void setShowFillsFlag( bool );
    bool getRasterTextFlag() const;
    void setRasterTextFlag( bool );
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
    bool getDepthCueFlag() const;
    void setDepthCueFlag( bool );
    bool getInhibitDynamicsFlag() const;
    void setInhibitDynamicsFlag( bool );
    bool getShowShadowsFlag() const;
    void setShowShadowsFlag( bool );
    bool getShowTextureMapsFlag() const;
    void setShowTextureMapsFlag( bool );
    bool getShowHazeFlag() const;
    void setShowHazeFlag( bool );
    bool getTransparencyFlag() const;
    void setTransparencyFlag( bool );
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

    OdDgView::OdDgDisplayMode getDisplayMode() const;
    void setDisplayMode( OdDgView::OdDgDisplayMode uMode );

  private:

    OdUInt32 m_uFlags1;
    OdUInt32 m_uFlags2;
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgReferenceAttachmentHeader : public OdDgElement
{
public:

  enum OdDgNewLevelDisplyMode
  {
    kConfigVariable    = 0,
    kAlways            = 1,
    kNever             = 2
  };

  enum OdDgLineStyleScaleMode
  {
    kLSMaster          = 0,
    kLSNone            = 1,
    kLSMasterReference = 2,
    kLSReference       = 3
  };

  enum OdDgReferenceForeignUnits
  {
    kUnitsUndefined     = 0,
    kSeedFileMasterUnits = 1,
    kSeedFileSubUnits    = 2,
    kDesignCenterUnits  = 3,
    kUnitsMicrometers   = 4,
    kUnitsMillimeters   = 5,
    kUnitsCentimeters   = 6,
    kUnitsMeters        = 7,
    kUnitsKilometers    = 8,
    kUnitsMicroinches   = 9,
    kUnitsMils          = 10,
    kUnitsInches        = 11,
    kUnitsFeet          = 12,
    kUnitsYards         = 13,
    kUnitsMiles         = 14
  };

  enum OdDgXRefPaletteModificationMode
  {
    kAdjusmentFactor         = 0,
    kFixedValueAndSaturation = 1
  };

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgReferenceAttachmentHeader );
public:

  //The short name should be equal to the shoft part of the full name.
  //It is user's responsibility to keep these fields correlate.
  OdString getFileName() const;
  void setFileName( const OdString& filename );

  //Normally, it is a full name with the short part equal to the one given by getFileName().
  //It is user's responsibility to keep these fields correlate.
  //Note: the short name is preferred (with the current path of the host database) during referenced file search,
  //even if there is any file by the full name
  OdString getFullFileName() const;
  void setFullFileName( const OdString& filename );

  //Note: to refer to a default model of a referred database, keep this field empty
  OdString getModelName() const;
  void setModelName( const OdString& name );

  // The logical name
  OdString getLogicalName() const;
  void setLogicalName( const OdString& name );

  // The description (if present)
  OdString getDescription() const;
  void setDescription( const OdString& description );

  // The revision name
  OdString getRevisionName() const;
  void setRevisionName( const OdString& name );

  // The Extraction model name (if present. It has higher priority than model and it contained
  //                            into same database as a model with reference attachment. )
  OdString getExtractionModelName() const;
  void     setExtractionModelName( const OdString& name );

  // The Named Group (if present)
  OdString getNamedGroupName() const;
  void     setNamedGroupName( const OdString& name );

  OdUInt32 getFileNumber() const;
  void setFileNumber( OdUInt32 val );

  OdUInt32 getPriority() const;
  void setPriority( OdUInt32 val );

  OdUInt16 getBaseNestDepth() const;
  void setBaseNestDepth( OdUInt16 val );

  OdUInt16 getNestDepth() const;
  void setNestDepth( OdUInt16 val );

  //Note: this point is managed as UORs, because it is an offset of zero point within the referenced file
  //and this reference element knows nothing about storage/master/sub units of that file
  OdGePoint3d getReferenceOrigin() const;
  void setReferenceOrigin( const OdGePoint3d& origin );

  //it is an offset of the referenced file (relative to its zero point) within the current file
  OdGePoint3d getMasterOrigin() const;
  void setMasterOrigin( const OdGePoint3d& origin );

  //this matrix is for rotation only
  OdGeMatrix3d getTransformation() const;
  void setTransformation( const OdGeMatrix3d& rotation );

  //This is low-level scale factor: scale factor from working units of a referenced model to working units of the current model depends on
  //the different flags. It is recommended to use getEntireScale().
  double getScale() const;
  void setScale( double scale );

  //It calculates the scale factor with all flags taken into consideration (see getScale()).
  //Literally, it is the ratio: 'working units of a referenced model' / 'working units of the current model'.
  //It could generate eNotApplicable exception if no current or referenced models can be found.
  double getEntireScale() const;

  double getZFront() const;
  void setZFront( double val );

  double getZBack() const;
  void setZBack( double val );

  OdGePoint3d getCameraPosition() const;
  void setCameraPosition( const OdGePoint3d& pos );

  double getCameraFocalLength() const;
  void setCameraFocalLength( double val );

  virtual void copyFieldReferencesForWBlockClone( OdDgFiler* pFiler ) const;

  //
  //clip points
  //

  OdUInt32 getClipPointsCount() const;
  void setClipPointsCount( OdUInt32 count );

  OdGePoint2d getClipPoint( OdUInt32 index ) const;
  void setClipPoint( OdUInt32 index, const OdGePoint2d& point );

  //flags

  virtual bool getCoincidentFlag() const;
  virtual void setCoincidentFlag(bool val);
  virtual bool getSnapLockFlag() const;
  virtual void setSnapLockFlag(bool val);
  virtual bool getLocateLockFlag() const;
  virtual void setLocateLockFlag(bool val);
  virtual bool getCompletePathInV7Flag() const;
  virtual void setCompletePathInV7Flag(bool val);
  virtual bool getAnonymousFlag() const;
  virtual void setAnonymousFlag(bool val);
  virtual bool getInactiveFlag() const;
  virtual void setInactiveFlag(bool val);
  virtual bool getMissingFileFlag() const;
  virtual void setMissingFileFlag(bool val);

  virtual OdUInt32 getLevelOverride() const;
  virtual void setLevelOverride(OdUInt32 val);

  virtual bool getDontDetachOnAllFlag() const;
  virtual void setDontDetachOnAllFlag(bool val);
  virtual bool getMetadataOnlyFlag() const;
  virtual void setMetadataOnlyFlag(bool val);

  virtual bool getDisplayFlag() const;
  virtual void setDisplayFlag(bool val);
  virtual bool getLineStyleScaleFlag() const;
  virtual void setLineStyleScaleFlag(bool val);
  virtual bool getHiddenLineFlag() const;
  virtual void setHiddenLineFlag(bool val);
  virtual bool getDisplayHiddenLinesFlag() const;
  virtual void setDisplayHiddenLinesFlag(bool val);
  virtual bool getRotateClippingFlag() const;
  virtual void setRotateClippingFlag(bool val);
  virtual bool getExtendedRefFlag() const;
  virtual void setExtendedRefFlag(bool val);
  virtual bool getClipBackFlag() const;
  virtual void setClipBackFlag(bool val);
  virtual bool getClipFrontFlag() const;
  virtual void setClipFrontFlag(bool val);
  virtual bool getCameraOnFlag() const;
  virtual void setCameraOnFlag(bool val);
  virtual bool getTrueScaleFlag() const;
  virtual void setTrueScaleFlag(bool val);
  virtual bool getDisplayBoundaryFlag() const;
  virtual void setDisplayBoundaryFlag(bool val);
  virtual bool getLibraryRefFlag() const;
  virtual void setLibraryRefFlag(bool val);
  virtual bool getDisplayRasterRefsFlag() const;
  virtual void setDisplayRasterRefsFlag(bool val);
  virtual bool getUseAlternateFileFlag() const;
  virtual void setUseAlternateFileFlag(bool val);
  virtual bool getUseLightsFlag() const;
  virtual void setUseLightsFlag(bool val);
  virtual bool getDoNotDisplayAsNestedFlag() const;
  virtual void setDoNotDisplayAsNestedFlag(bool val);
  virtual OdUInt32 getColorTableUsage() const;
  virtual void setColorTableUsage(OdUInt32 val);
  virtual bool getViewportFlag() const;
  virtual void setViewportFlag(bool val);
  virtual bool getScaleByStorageUnitsFlag() const;
  virtual void setScaleByStorageUnitsFlag(bool val);
  virtual bool getPrintColorAdjustmentFlag() const;
  virtual void setPrintColorAdjustmentFlag(bool val);
  virtual bool getUseViewFlagsFlag() const;
  virtual void setUseViewFlagsFlag(bool val);

  virtual bool getUseAnnotationScaleFlag() const;
  virtual void setUseAnnotationScaleFlag(bool val);
  virtual OdDgNewLevelDisplyMode getNewLevelDisplayMode() const;
  virtual void                   setNewLevelDisplayMode( const OdDgNewLevelDisplyMode& uMode);

  OdDgReferenceAttachmentViewFlags getViewFlags( OdUInt32 uViewIndex ) const;
  void setViewFlags( OdUInt32 uViewIndex, const OdDgReferenceAttachmentViewFlags& viewFlags );

  //returns a linked database if the short & full names allow to find any
  //Note:
  //1. Look at descriptions for getFileName() & getFullFileName() for comments about how the file search works
  //2. It can be either TG database, TD database or a database of other types
  //3. All databases ordered by references are loaded and stored into the internal cash of the host database
  //   since they are ordered for the first time
  //4. All returned databases are opened for read only
  //5. Returns zero if no database is found
  OdRxObjectPtr getReferencedDatabase();

  //returns a linked model if the short & full names of database and the name of model allow to find any
  //Note:
  //1. Look at the comments for getReferenceAttachment()
  //2. It works for DGN databases only
  //3. All returned models are opened for read only
  //4. Returns zero if no model (or no database) are found
  OdDgModelPtr getReferencedModel();

  //support for the complexity
  void add( OdDgElementPtr pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  //this level table is stored locally within the current host database
  //and allows to override level tables of referred databases
  OdDgLevelTablePtr getLevelTable( OdDg::OpenMode openMode = OdDg::kForRead ) const;

  //overrides for OdGiDrawable
  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  void subViewportDraw(OdGiViewportDraw *pVd) const;

  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual OdRxObjectPtr clone() const;

  // Defines the units for DGN to use when .dwf or .dxf files being opened has its Design Center Units set to “Unspecified.”
  //  ( It can happen for .dwg files created prior to release 2000 )
  OdDgReferenceForeignUnits getForeignUnits() const;
  void                      setForeighUnits( OdDgReferenceForeignUnits iUnits );
  static double             getForeignUnitsScale( OdDgReferenceAttachmentHeader::OdDgReferenceForeignUnits iUnits );

  OdDgLineStyleScaleMode getLineStyleScaleMode() const;
  void  setLineStyleScaleMode( OdDgLineStyleScaleMode iMode );

  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  virtual OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents ) const;

  virtual OdUInt32 getLevelEntryId() const; // If level entry id of reference attachment is 0, then level property is not defined.
  virtual void setLevelEntryId(OdUInt32 index);

  virtual OdDgElementId getLevelId() const;
  virtual void setLevelId(const OdDgElementId& levelId, bool doSubents = true);

  virtual void bindNamedView( OdUInt64 uIdView );

  // Reference color palette adjustment

  virtual OdCmTransparency getTransparency() const;
  virtual void             setTransparency( const OdCmTransparency& cmTransparency );
  virtual OdDgXRefPaletteModificationMode getPaletteModificationMode() const;
  virtual void setPaletteModificationMode( const OdDgXRefPaletteModificationMode& uMode );
  virtual double getPaletteValue() const;
  virtual void   setPaletteValue( double dValue );
  virtual double getPaletteSaturation() const;
  virtual void   setPaletteSaturation( double dSaturation );
  virtual double getPaletteHue() const;
  virtual void   setPaletteHue( double dHue );
  virtual bool   getHueFixForAllColorsFlag() const;
  virtual void   setHueFixForAllColorsFlag( bool bSet );
  virtual bool   getPrintAdjustedColorsFlag() const;
  virtual void   setPrintAdjustedColorsFlag( bool bSet );
};

typedef OdSmartPtr< OdDgReferenceAttachmentHeader > OdDgReferenceAttachmentHeaderPtr;

/** \details
   
    Corresponding C++ library: DG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgReferenceAttachClipBoundaryPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS( OdDgReferenceAttachClipBoundaryPE );

  long m_references;
public:

  OdDgReferenceAttachClipBoundaryPE(){};

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  static bool extractBoundary( OdDgReferenceAttachmentHeaderPtr reference, OdGiClipBoundary& clipBoundary, double deviation,
    bool bViewIndependedClipping, OdGiWorldDraw* pWd, bool& bClipIsInverce, bool bSetDeviation = false, const OdGeMatrix3d& matXRefTransform = OdGeMatrix3d::kIdentity,
    bool bAllowInverceClip = false );

  static bool extractBoundary( const OdDgReferenceAttachmentHeaderPtr& reference, OdGiClipBoundary& clipBoundary, 
                               OdGiExtendedClipBoundary& extClipBoundary, double deviation, bool bViewIndependedClipping,
                               OdGiWorldDraw* pWd, bool bSetDeviation = false, const OdGeMatrix3d& matXRefTransform = OdGeMatrix3d::kIdentity );

private:

  static bool tryToFindRotationMatrix( OdDgReferenceAttachmentHeaderPtr reference, OdGeMatrix3d& rotation );
  static bool tryToFindLinkedGraphicsElement( OdDgReferenceAttachmentHeaderPtr reference, OdDgGraphicsElementPtr& foundElement, 
                                              OdGeMatrix3d& matAdditionalTransform );
  static bool getFarElement( const OdDgElementIdPathPtr& pPath, OdDgGraphicsElementPtr& foundElement, OdGeMatrix3d& matAdditionalTransform );
};

typedef OdSmartPtr<OdDgReferenceAttachClipBoundaryPE> OdDgReferenceAttachClipBoundaryPEPtr;

/** \details
This class represent the type 108 'Reference Override Element'.

It is used to override level tables of xRefs, nested overriddens are allowed. These objects can be followed by 'level mask'
elements that operate on the same xRefs.

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgReferenceOverride : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgReferenceOverride );
public:

  //display
  bool getDisplayFlag() const;
  void setDisplayFlag( bool value, bool setOverrideFlag = true );

  bool getDisplayOverrideFlag() const;
  void setDisplayOverrideFlag( bool value );

  //snap
  bool getSnapFlag() const;
  void setSnapFlag( bool value, bool setOverrideFlag = true );

  bool getSnapOverrideFlag() const;
  void setSnapOverrideFlag( bool value );

  //locate
  bool getLocateFlag() const;
  void setLocateFlag( bool value, bool setOverrideFlag = true );

  bool getLocateOverrideFlag() const;
  void setLocateOverrideFlag( bool value );

  //These IDs define the nested xRef all overrides are applied to.
  //They can not be resolved to OdDgElementId's here, because are actual for the databases referred from the current file.
  //Note : the longest path is 40.
  void getPathOfXRef( OdUInt64Array& path ) const;
  void setPathOfXRef( const OdUInt64Array& path );

  virtual OdRxObjectPtr clone() const;

  //only LevelMask elements are allowed to be here
  void add( OdDgElement* pElement );
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;
};

typedef OdSmartPtr< OdDgReferenceOverride > OdDgReferenceOverridePtr;

/** \details
This class represent the type 66 subtype 33 'Reference Order Element'.

It is used to hold xRefs order into drawing. Its priority higher then order of
xRefs in file, but lower then priority of xRef element.

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgReferenceOrder : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgReferenceOrder );

public:

  virtual OdUInt32 getFlags() const;
  virtual void     setFlags( OdUInt32 uFlags );

  virtual OdUInt32 getNumberOfReferences() const;
  virtual OdUInt64 getReferenceId( OdUInt32 uPosition );
  virtual void     setReferenceId( OdUInt32 uPosition, OdUInt64 uId );
  virtual void     addReferenceId( OdUInt64 uId, OdInt32 iInsertBefore = -1 );
  virtual void     deleteReferenceId( OdUInt32 uPosition );
};

typedef OdSmartPtr< OdDgReferenceOrder > OdDgReferenceOrderPtr;

#include "TD_PackPop.h"

#endif // __DG_REFERENCE_ATTACH_H__
