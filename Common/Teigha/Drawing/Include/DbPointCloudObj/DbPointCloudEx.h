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

#ifndef _OD_DB_POINTCLOUDEX_
#define _OD_DB_POINTCLOUDEX_

#include "DbPointCloudObjExports.h"
#include "DbPointCloudCropping.h"
#include "DbPointCloudIntensityStyle.h"
#include "TD_PackPush.h"

#include "DbEntity.h"

/** \details
This class represents PointCloudEx entities in an OdDbDatabase instance.

Corresponding C++ library: AcDbPointCloudObj

<group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudEx : public OdDbEntity
{
public:

  ODDB_DECLARE_MEMBERS( OdDbPointCloudEx );

  /** \details
    Default constructor.
  */
  OdDbPointCloudEx();

  /** \details
    Destructor.
  */
  ~OdDbPointCloudEx() {}

  /** \details
  Returns the Object ID of the OdDbPointCloudDefEx object associated with this point cloud ex entity.
  */
  OdDbObjectId pointCloudDefExId() const;

  /** \details
  Sets the Object ID of the OdDbPointCloudDefEx object associated with this point cloud ex entity.

  \param pointCloudDefExId [in]  Object ID of the point cloud ex to be associated with this point cloud ex entity.
  */
  void setPointCloudDefExId( const OdDbObjectId pointCloudDefExId );  

  /** \details
  Returns the Object ID of the OdDbPointCloudDefReactorEx object associated with this point cloud ex entity.

  \remarks
  Each OdDbPointCloudEx object must have an OdDbPointCloudDefReactorEx associated with the OdDbPointCloudDefEx object it is using.

  */
  OdDbObjectId reactorId() const;

  /** \details
  Sets the Object ID of the OdDbPointCloudDefReactorEx object associated with this point cloud entity.

  \param pointCloudDefReactorExId [in]  Object ID of a reactor for this point cloud ex entity.

  \remarks
  Each OdDbPointCloudEx object must have an OdDbPointCloudDefReactorEx associated with the OdDbPointCloudDefEx object it is using.
  */
  void setReactorId( const OdDbObjectId pointCloudDefReactorExId );

  /** \details
  Returns true if the point cloud entity ex is locked or false otherwise.
  */
  bool locked() const;

  /** \details
  Sets the point cloud entity ex to be locked.
  */
  void setLocked( bool bLocked );

  /** \details
  
  */
  double scale() const;

  /** \details
  
  */
  void setScale( double scale );

  /** \details

  */
  double rotation() const;

  /** \details

  */
  void setRotation( double newRotation );

  /** \details

  */
  OdGePoint3d location() const;

  /** \details

  */
  void setLocation( OdGePoint3d newLocation );

  /** \details

  */
  void getNativeCloudExtent( OdGeExtents3d extents ) const;

  /** \details
  NOT IMPLEMENTED
  */
  unsigned int getVisiblePointCount( OdGeExtents3d extents );

  /** \details
  NOT IMPLEMENTED
  */
  unsigned int getLoadedVisiblePointCount( OdGeExtents3d extents );

  /** \details
  NOT IMPLEMENTED
  */
  unsigned int getDisplayedVisiblePointCount( OdGeExtents3d extents );

  /** \details

  */
  bool getActiveFileName( OdString& fileName ) const;

  /** \details

  */
  bool setActiveFileName( const OdChar* strActiveFileName );

  /** \details

  */
  OdString getPointCloudName() const;

  /** \details

  */
  OdResult setPointCloudName( const OdString& name );

  //

  /** \details

  */
  OdDbPointCloudCropping* getConstPointCloudCropping( int index );

  /** \details

  */
  OdDbPointCloudCropping const* getConstPointCloudCropping( int index ) const;

  /** \details

  */
  void addCroppingBoundary( OdDbPointCloudCropping const &boundary );

  /** \details
  
  */
  OdResult clearCropping();

  /** \details

  */
  OdResult removeLastCropping();

  /** \details
  NOT IMPLEMENTED
  */
  bool getCroppingInvert() const;

  /** \details
  NOT IMPLEMENTED
  */
  OdResult setCroppingInvert( bool bInvert );

  /** \details
  NOT IMPLEMENTED
  */
  OdResult setInCreatingCroppingMode( bool bCreating );

  /** \details
  Returns true only if this point cloud ex entity is displayed as clipped.
  */
  bool showCropping() const;

  /** \details
  Set whether this point cloud ex entity is displayed as clipped.

  \param cropping [in]  Value for option.

  \returns
  Returns eOk.
  */
  OdResult setShowCropping( bool bShowCropping );

  //

  enum StylizationType
  {
    kTrueColor = 1,
    kSingleColor,
    kNormalRamp,
    kHeightRamp,
    kIntensityRamp,
    kClassificationRamp
  };

  /** \details

  */
  StylizationType stylizationType() const;

  /** \details

  */
  OdResult setStylizationType( StylizationType type );

  //

  /** \details

  */
  OdString getCurrentColorScheme() const;

  /** \details

  */
  OdResult setCurrentColorScheme( const OdString &guid );

  /** \details
  test 
  */
  OdString getColorSchemeForStylization( StylizationType type ) const;

  /** \details
  test
  */
  OdResult setColorSchemeForStylization( const OdString &guid, StylizationType type );

  //

  enum DisplayOptionOutOfRange
  {
    kUseMinMaxColors = 0,
    kUseRGBScanColors,
    kHidePoints
  };

  /** \details

  */
  bool showIntensityAsGradient() const;

  /** \details

  */
  void setShowIntensityAsGradient( bool bShowAsGradient );

  /** \details

  */
  OdInt32 minIntensity() const;

  /** \details

  */
  OdInt32 maxIntensity() const;

  /** \details

  */
  OdResult setMinMaxIntensity( OdInt32 min, OdInt32 max );

  /** \details

  */
  DisplayOptionOutOfRange intensityOutOfRangeBehavior() const;

  /** \details

  */
  void setIntensityOutOfRangeBehavior( DisplayOptionOutOfRange displayOption );

  /** \details

  */
  bool showElevationAsGradient() const;

  /** \details

  */
  void setShowElevationAsGradient( bool bShowAsGradient );

  /** \details

  */
  double minElevation() const;

  /** \details

  */
  double maxElevation() const;

  /** \details

  */
  OdResult setMinMaxElevation( double min, double max );

  /** \details

  */
  bool elevationApplyToFixedRange() const;

  /** \details

  */
  void setElevationApplyToFixedRange( bool bFixedRange );

  /** \details

  */
  DisplayOptionOutOfRange elevationOutOfRangeBehavior() const;

  /** \details

  */
  void setElevationOutOfRangeBehavior( DisplayOptionOutOfRange displayOption );



  OdResult subTransformBy( const OdGeMatrix3d& xform );
  bool subWorldDraw( OdGiWorldDraw* pWd ) const;
  void subViewportDraw( OdGiViewportDraw* pVd ) const;

  // File IO
  virtual OdResult  dwgInFields( OdDbDwgFiler * );
  virtual void  dwgOutFields( OdDbDwgFiler * ) const;
  virtual OdResult  dxfInFields( OdDbDxfFiler * );
  virtual void  dxfOutFields( OdDbDxfFiler * ) const;

};

typedef OdSmartPtr<OdDbPointCloudEx> OdDbPointCloudExPtr;

#include "TD_PackPop.h"


#endif //_OD_DB_POINTCLOUDEX_
