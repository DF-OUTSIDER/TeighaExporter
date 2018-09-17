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


#ifndef _OdDbSection_h_Included_
#define _OdDbSection_h_Included_

#include "DbEntity.h"
#include "SmartPtr.h"
#include "DbHatch.h"
#include "TD_PackPush.h"
#include "ModelerGeometry.h"

class OdDbSectionSettings;
typedef OdSmartPtr<OdDbSectionSettings> OdDbSectionSettingsPtr;

class OdDbSection;
typedef OdSmartPtr<OdDbSection> OdDbSectionPtr;

/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSection : public OdDbEntity
{
public:
  enum State
  {
    kPlane              = 0x01,
    kBoundary           = 0x02,
    kVolume             = 0x04
  };

  enum SubItem
  {
    kNone               = 0x00,
    kSectionLine        = 0x01,
    kSectionLineTop     = 0x02,
    kSectionLineBottom  = 0x04,
    kBackLine           = 0x08,
    kBackLineTop        = 0x10,
    kBackLineBottom     = 0x20,
    kVerticalLineTop    = 0x40,
    kVerticalLineBottom = 0x80
  };

  enum Height
  {
    kHeightAboveSectionLine = 0x1,
    kHeightBelowSectionLine = 0x2
  };

public:
  ODDB_DECLARE_MEMBERS(OdDbSection);

  OdDbSection();
  static OdDbSectionPtr createObject(const OdGePoint3dArray& pts,
    const OdGeVector3d& verticalDir);
  static OdDbSectionPtr createObject(const OdGePoint3dArray& pts,
    const OdGeVector3d& verticalDir, const OdGeVector3d& vecViewingDir);

  OdDbSection::State state() const;
  void setState(OdDbSection::State nState);

  const OdString getName() const;
  void getName(OdString& sName) const;
  void setName(const OdString& pszName);

  OdGeVector3d viewingDirection () const;
  OdResult setViewingDirection(const OdGeVector3d& dir);

  OdGeVector3d verticalDirection() const;
  OdResult setVerticalDirection(const OdGeVector3d& dir);

  OdGeVector3d normal() const;
  OdResult plane(OdGeVector3d& uAxis, OdGeVector3d& vAxis) const;

  int indicatorTransparency() const;
  void setIndicatorTransparency(int nTransparency);

  OdCmColor indicatorFillColor  () const;
  void setIndicatorFillColor(const OdCmColor& color);

  int numVertices() const;
  OdResult getVertex(int nIndex, 
    OdGePoint3d& pt) const;
  OdResult setVertex(int nIndex, 
    const OdGePoint3d& pt);
  OdResult addVertex(int nInsertAt, 
    const OdGePoint3d& pt);
  OdResult removeVertex(int nIndex);
  void  getVertices(OdGePoint3dArray& pts) const;
  OdResult setVertices(const OdGePoint3dArray& pts);

  double height(OdDbSection::Height nHeightType) const;
  OdResult setHeight(OdDbSection::Height nHeightType, double fHeight);

  //bool  hitTest(const OdGePoint3d& ptHit, int* pSegmentIndex, OdGePoint3d* ptOnSegment, OdDbSection::SubItem* pSubItem) const;
  OdResult createJog   (const OdGePoint3d& ptOnSection);
  bool hasJogs() const;

  OdDbObjectId getSettings() const;
  OdDbSectionSettingsPtr getSettings(OdDb::OpenMode mode) const;

  bool isLiveSectionEnabled() const;
  void enableLiveSection(bool bEnable);

  bool isSlice() const;
  OdResult setIsSlice( bool bIsSlice );

  double thicknesDepth() const;
  OdResult setThicknesDepth( double depth );

  double sectionPlaneOffset() const;
  OdResult setSectionPlaneOffset( double offset );

public:
  // Base class overrides
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;

  virtual void subViewportDraw(OdGiViewportDraw* pVd) const;

  virtual OdResult subGetClassID(void* pClsid) const;
  
  virtual void subClose();
  
  virtual OdResult subTransformBy(const OdGeMatrix3d& xform);

  virtual OdResult subGetTransformedCopy(const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;

  //virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  OdResult generateSectionGeometry (OdDbEntity*             pEnt,
                                    OdArray<OdDbEntityPtr>& intBoundaryEnts,
                                    OdArray<OdDbEntityPtr>& intFillEnts,
                                    OdArray<OdDbEntityPtr>& backgroundEnts,
                                    OdArray<OdDbEntityPtr>& foregroundEnts,
                                    OdArray<OdDbEntityPtr>& curveTangencyEnts) const;

  // INTERNAL USE ONLY
  OdResult generateSectionGeometry (OdDbEntity*             pEnt,
                                    OdArray<OdDbEntityPtr>& intBoundaryEnts,
                                    OdArray<OdDbEntityPtr>& intFillEnts,
                                    OdArray<OdDbEntityPtr>& backgroundEnts,
                                    OdArray<OdDbEntityPtr>& foregroundEnts,
                                    OdArray<OdDbEntityPtr>& curveTangencyEnts,
                                    bool                    bIsSetProperties) const;


  double topPlane() const;
  OdResult  setTopPlane(double);

  double bottomPlane() const;
  OdResult setBottomPlane(double);
  
  double elevation() const;
  OdResult setElevation( double);
};


//*************************************************************************
// OdDbSectionSettings
//*************************************************************************

/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSectionSettings : public OdDbObject
{
public:
  enum SectionType
  {
    kLiveSection                = 0x1,
    k2dSection                  = 0x2,
    k3dSection                  = 0x4 
  };

  enum Geometry
  {
    kIntersectionBoundary       = 0x1,
    kIntersectionFill           = 0x2,
    kBackgroundGeometry         = 0x4,
    kForegroundGeometry         = 0x8,
    kCurveTangencyLines         = 0x10 
  };

  enum Generation
  {
    kSourceAllObjects           = 0x1,
    kSourceSelectedObjects      = 0x2,

    kDestinationNewBlock        = 0x10,
    kDestinationReplaceBlock    = 0x20,
    kDestinationFile            = 0x40
  };

public:
  ODDB_DECLARE_MEMBERS(OdDbSectionSettings);

  OdDbSectionSettings();

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;

  void reset();
  void reset(OdDbSectionSettings::SectionType nSecType);

  OdDbSectionSettings::SectionType currentSectionType() const;
  void setCurrentSectionType(OdDbSectionSettings::SectionType nSecType);

  OdDbSectionSettings::Generation generationOptions(OdDbSectionSettings::SectionType nSecType) const;
  void setGenerationOptions(OdDbSectionSettings::SectionType nSecType,
    OdDbSectionSettings::Generation nOptions);

  void getSourceObjects(OdDbSectionSettings::SectionType nSecType,
    OdDbObjectIdArray& ids) const;
  void setSourceObjects(OdDbSectionSettings::SectionType nSecType,
    const OdDbObjectIdArray& ids);

  OdDbObjectId destinationBlock(OdDbSectionSettings::SectionType nSecType) const;
  void setDestinationBlock(OdDbSectionSettings::SectionType nSecType,
    const OdDbObjectId& id);

  const OdString destinationFile(OdDbSectionSettings::SectionType nSecType) const;
  void setDestinationFile(OdDbSectionSettings::SectionType nSecType,
    const OdString& pszFileName);

  bool  visibility(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setVisibility(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    bool bVisible);
  OdCmColor color(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setColor(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, const OdCmColor& color);
  OdString layer(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setLayer(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    const OdString& pszLayer);
  OdString linetype(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setLinetype (OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    const OdString& pszLinetype);
  double linetypeScale(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setLinetypeScale(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    double fScale);
  OdString plotStyleName(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setPlotStyleName(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    const OdString& pszPlotStyleName);
  OdDb::LineWeight lineWeight(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setLineWeight(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    OdDb::LineWeight nLineWeight);
  int   faceTransparency(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setFaceTransparency(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    int nTransparency);
  int   edgeTransparency(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setEdgeTransparency(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    int nTransparency);
  bool  hatchVisibility(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setHatchVisibility(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    bool bVisible);
  void getHatchPattern(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    OdDbHatch::HatchPatternType& nPatternType, 
    OdString& pszPatternName) const;
  void setHatchPattern(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    OdDbHatch::HatchPatternType nPatternType, 
    const OdString& pszPatternName);
  double hatchAngle(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setHatchAngle(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    double fAngle);
  double hatchSpacing(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setHatchSpacing(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    double fSpacing);
  double hatchScale(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setHatchScale(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    double fScale);
  bool  hiddenLine(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setHiddenLine(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    bool bHiddenLine);
  bool  divisionLines(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry) const;
  void setDivisionLines(OdDbSectionSettings::SectionType nSecType, 
    OdDbSectionSettings::Geometry nGeometry, 
    bool bShow);
};


#include "TD_PackPop.h"

#endif //_OdDbSection_h_Included_
