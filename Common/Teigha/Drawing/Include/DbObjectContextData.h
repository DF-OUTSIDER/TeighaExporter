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

#ifndef _DBOBJECTCONTEXTDATA_H_INCLUDED_
#define _DBOBJECTCONTEXTDATA_H_INCLUDED_

#include "DbObject.h"
#include "DbHatch.h"
#include "DbMText.h"

class OdDbObjectContext;

/** \details
    This class defines the set of object properties that represent an object in 
	some context.
  
    \remarks
    The context data varies depending on the current context, for example, the current 
	annotation scale. This class is intended for application developers only, 
	not for TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbObjectContextData : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbObjectContextData);
  OdDbObjectContextData();

  /*
  virtual OdResult dwgInContext(OdDbDwgFiler*,OdDbObjectContext* &);
  virtual void dwgOutContext(OdDbDwgFiler*) const;
*/
  virtual OdResult dwgInContextData(OdDbDwgFiler*);
  virtual void dwgOutContextData(OdDbDwgFiler*) const;
  virtual const OdDbObjectContext* context() const;
  virtual bool matchesContext(const OdDbObjectContext&) const;
  virtual void setContext(const OdDbObjectContext&);
  virtual void setContextToNull();

  bool fileToExtensionDictionary() const;
  bool isDefaultContextData() const;
  bool isModified() const;
  void setFileToExtensionDictionary(bool);
  void setModified(bool);
  void setIsDefault(bool b);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbObjectContextData> OdDbObjectContextDataPtr;

/** \details
    This class defines the set of object properties that represent an annotation scale.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbAnnotScaleObjectContextData : public OdDbObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbAnnotScaleObjectContextData);
  OdDbAnnotScaleObjectContextData();

  virtual const OdDbObjectContext* context() const;
  virtual void setContext(const OdDbObjectContext&);
  /*
  virtual OdResult dwgInContext(OdDbDwgFiler*,OdDbObjectContext*&);
  virtual OdResult dwgOutContext(OdDbDwgFiler*) const;
  virtual bool matchesContext(const OdDbObjectContext&) const;
  virtual void setContextToNull();
*/
  virtual OdResult getScale(double&) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbAnnotScaleObjectContextData> OdDbAnnotScaleObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for an MText object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class OdDbMTextObjectContextData;
typedef OdSmartPtr<OdDbMTextObjectContextData> OdDbMTextObjectContextDataPtr;

/** <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbMTextObjectContextData : public OdDbAnnotScaleObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbMTextObjectContextData);
  OdDbMTextObjectContextData();
  /*
  bool zeroWidth(double tol = 1e-10) const;
  double rotation(const OdGeVector3d&) const;
*/
  OdGePoint3d location() const;
  OdGeVector3d direction() const;
  double actualHeight() const;
  double actualWidth() const;
  double definedHeight() const;
  double definedWidth() const;
  OdDbMText::AttachmentPoint attachment() const;
  OdDb::TextHorzMode horizontalMode() const;
  OdDb::TextVertMode verticalMode() const;

  OdDbMText::ColumnType  columnType() const;
  OdInt32 columnCount() const;
  double columnGutter() const;
  double columnHeight(OdInt32) const;
  double columnWidth() const;
  bool columnAutoHeight() const;
  bool columnFlowReversed() const;
  
  void setLocation(const OdGePoint3d&);
  void setDirection(const OdGeVector3d&);
  void setActualHeight(double);
  void setActualWidth(double);
  void setDefinedHeight(double);
  void setDefinedWidth(double);
  void setAttachment(OdDbMText::AttachmentPoint);
  //void setAttachmentMovingLocation(OdDbMText::AttachmentPoint, const OdGeVector3d&, double);
  void setHorizontalMode(OdDb::TextHorzMode);
  void setVerticalMode(OdDb::TextVertMode);
  
  void setColumnType(OdDbMText::ColumnType);
  void setColumnCount(OdInt32);
  void setColumnAutoHeight(bool);
  void setColumnFlowReversed(bool);
  void setColumnGutter(double);
  void setColumnHeight(OdInt32,double);
  void setColumnWidth(double);
/*  
  void  chgdirXattach();
  void  chgYattach();
  void  getCorners(OdGePoint3dArray&, const OdGeVector3d&, bool, double) const;
  void  setDirFromAngle(double, const OdGeVector3d&);
  void  xlateLocation(const OdGeVector3d&);
*/
  static OdDbMTextObjectContextDataPtr getMTextAnnoScaleDefaultContextData(const OdDbMText*);
  virtual void copyFrom(const OdRxObject* pSource);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
    This class defines a set of annotation-specific properties for a Text object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbTextObjectContextData : public OdDbAnnotScaleObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbTextObjectContextData);
  OdDbTextObjectContextData();
  virtual OdGePoint2d alignmentPointInEcs() const;
  //virtual void audit(OdDbAuditInfo *,OdDbText *,OdInt32 &,bool &);
  virtual OdDb::TextHorzMode horizontalMode() const;
  virtual OdGePoint2d positionInEcs() const;
  virtual double rotation() const;
  virtual void setAlignmentPointInEcs(const OdGePoint2d&);
  virtual void setHorizontalMode(OdDb::TextHorzMode);
  virtual void setPositionInEcs(const OdGePoint2d&);
  virtual void setRotation(double);

  //static OdDbTextObjectContextData* getTextAnnoScaleDefaultContextData(const OdDbText*);

  virtual void copyFrom(const OdRxObject *);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbTextObjectContextData> OdDbTextObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for an MText Attribute object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbMTextAttributeObjectContextData : public OdDbTextObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbMTextAttributeObjectContextData);
  OdDbMTextAttributeObjectContextData();
  virtual void setContext(const OdDbObjectContext&) ODRX_OVERRIDE;
  virtual OdDbMTextObjectContextDataPtr getMTextObjectContextData() const;
  virtual void setMTextObjectContextData(OdDbMTextObjectContextData*);
  virtual void copyFrom(const OdRxObject *);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbMTextAttributeObjectContextData> OdDbMTextAttributeObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Block Reference object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbBlkRefObjectContextData : public OdDbAnnotScaleObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBlkRefObjectContextData);
  OdDbBlkRefObjectContextData();

  virtual OdGePoint3d positionInEcs() const;
  virtual double rotation() const;
  virtual OdGeScale3d  scaleFactors() const;
  virtual void setPositionInEcs(const OdGePoint3d&);
  virtual void setRotation(double);
  virtual void setScaleFactors(const OdGeScale3d&);
  //static OdDbBlkRefObjectContextData* getBlkAnnoScaleDefaultContextData(const OdDbBlockReference *);

  //virtual void audit(OdDbAuditInfo *,OdDbBlockReference *,OdInt32 &,bool &);
  virtual void copyFrom(const OdRxObject *);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbBlkRefObjectContextData> OdDbBlkRefObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Dimension object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbDimensionObjectContextData : public OdDbAnnotScaleObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDimensionObjectContextData);
  OdDbDimensionObjectContextData();

  virtual OdDbObjectId blockIndex() const;
  virtual void setBlockIndex(OdDbObjectId);
  void setNDBRDimBlock(OdDbBlockTableRecord*);
  OdDbBlockTableRecordPtr getNDBRDimBlock()const;

  enum OverrideCode
  {
    eDimtofl  = 1,
    eDimsoxd   = 2,
    eDimatfit = 4,
    eDimtix   = 8,
    eDimtmove = 16
  };
  virtual bool  hasOverride(OverrideCode) const;
  virtual void setOverride(OverrideCode,bool);

  // dimvar overrides
  virtual OdInt16  dimatfit() const;
  virtual bool  dimsoxd() const;
  virtual bool  dimtix() const;
  virtual OdInt16  dimtmove() const;
  virtual bool  dimtofl() const;

  virtual void setDimatfit(OdInt16);
  virtual void setDimsoxd(bool);
  virtual void setDimtix(bool);
  virtual void setDimtmove(OdInt16);
  virtual void setDimtofl(bool);

  virtual bool  defTextLocation() const;
  virtual OdGePoint2d  textLocation() const;
  virtual double  textRotation() const;
  virtual bool  getArrowFirstIsFlipped() const;
  virtual bool  getArrowSecondIsFlipped() const;

  virtual void setDefTextLocation(bool);
  virtual void setTextLocation(const OdGePoint2d& /*,bool */);
  virtual void setTextRotation(double);
  virtual void  setArrowFirstIsFlipped(bool);
  virtual void  setArrowSecondIsFlipped(bool);
   
   //  virtual double calcZeroDimScale(OdDbDatabase *);
   //  OdResult createOrClearBlock(OdDbDatabase *);
   //  virtual void addEntity(OdDbEntity *);
   //   OdResult getEntityList(OdRxObjectPtrArray&, bool&) const;
   //   virtual OdGePoint3d  getFlipArrowPickPoint() const;
   //   bool  isBlockSpace(OdDbObjectId,OdDbDatabase *) const;
   //   virtual bool  isFlipArrow() const;
   //   virtual bool  isOwnDimBlock() const;
   //   virtual bool  recomputeDimBlock() const;
   //   void  releaseLayoutGeometry();
   //   virtual void  setFlipArrow(bool);
   //   virtual void  setFlipArrowPickPoint(OdGePoint3d &);
   //   virtual OdResult setOwnDimBlock(bool);
   //   virtual OdResult setRecomputeDimBlock(bool);
   //   void  setToBeRemoved(bool);
   //   OdResult stuffNewEntitiesInDatabase(bool,const OdGeMatrix3d&,wchar_t *);
   //  OdResult geomExtents(OdGeExtents3d&, const BlockRefData&) const;
   //  virtual OdRxObjectPtrArray const *  geometry() const;
//   bool  toBeRemoved() const;
//   OdResult updateContentsFromField(bool);
//   static OdDbDimensionObjectContextData * getDimensionAnnoScaleDefaultContextData(const OdDbDimension*);
//   static OdDbDimensionObjectContextData * getDimensionCurrentContextData(const OdDbDimension*);
//  
  virtual void copyFrom(const OdRxObject *);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  void transformBy(const OdGeMatrix3d &mx);
};

typedef OdSmartPtr<OdDbDimensionObjectContextData> OdDbDimensionObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for an Aligned Dimension object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbAlignedDimensionObjectContextData : public OdDbDimensionObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbAlignedDimensionObjectContextData);
  OdDbAlignedDimensionObjectContextData();

  OdGePoint3d dimLinePoint() const;
  void setDimLinePoint(const OdGePoint3d&);

  //static OdDbAlignedDimensionObjectContextData * getAlignedDimensionAnnoScaleDefaultContextData(const OdDbDimension*);

  virtual void copyFrom(const OdRxObject* pSource);

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbAlignedDimensionObjectContextData> OdDbAlignedDimensionObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for an Angular Dimension object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbAngularDimensionObjectContextData : public OdDbDimensionObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbAngularDimensionObjectContextData);
  OdDbAngularDimensionObjectContextData();

  OdGePoint3d arcPoint() const;
  void setArcPoint(const OdGePoint3d &);
  //OdResult update2Line(const OdGePoint3d *,const OdGePoint3d*,double,const OdGeVector3d&);
  //OdResult update3Point(const OdGePoint3d *,const OdGePoint3d*,double,const OdGeVector3d&);
  //OdResult updateArc(const OdGePoint3d *,const OdGePoint3d *,double,const OdGeVector3d&);

  //static OdDbAngularDimensionObjectContextData * getAngularDimensionAnnoScaleDefaultContextData(const OdDbDimension*);

  virtual void copyFrom(const OdRxObject* pSource);

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbAngularDimensionObjectContextData> OdDbAngularDimensionObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Diametric Dimension object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbDiametricDimensionObjectContextData : public OdDbDimensionObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDiametricDimensionObjectContextData);
  OdDbDiametricDimensionObjectContextData();

  OdGePoint3d chordPoint() const;
  OdGePoint3d farChordPoint() const;
  void setChordPoint(const OdGePoint3d&);
  void setFarChordPoint(const OdGePoint3d&);

  //static OdDbDiametricDimensionObjectContextData* getDiametricDimensionAnnoScaleDefaultContextData(const OdDbDimension*);

  virtual void copyFrom(const OdRxObject* pSource);

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbDiametricDimensionObjectContextData> OdDbDiametricDimensionObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for an Ordinate Dimension object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbOrdinateDimensionObjectContextData : public OdDbDimensionObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbOrdinateDimensionObjectContextData);
  OdDbOrdinateDimensionObjectContextData();

  OdGePoint3d leaderEndPoint() const;
  OdGePoint3d origin() const;
  void setLeaderEndPoint(const OdGePoint3d&);
  void setOrigin(const OdGePoint3d&);

/*  static OdDbOrdinateDimensionObjectContextData * getOrdinateDimensionAnnoScaleDefaultContextData(const OdDbDimension*);
*/
  virtual void copyFrom(const OdRxObject* pSource);

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbOrdinateDimensionObjectContextData> OdDbOrdinateDimensionObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Radial Dimension object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbRadialDimensionObjectContextData : public OdDbDimensionObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbRadialDimensionObjectContextData);
  OdDbRadialDimensionObjectContextData();

  OdGePoint3d  chordPoint() const;
  void setChordPoint(const OdGePoint3d&);

  //static OdDbRadialDimensionObjectContextData * getRadialDimensionAnnoScaleDefaultContextData(const OdDbDimension*);

  virtual void copyFrom(const OdRxObject* pSource);

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbRadialDimensionObjectContextData> OdDbRadialDimensionObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Large Radial Dimension object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbRadialDimensionLargeObjectContextData : public OdDbRadialDimensionObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbRadialDimensionLargeObjectContextData);
  OdDbRadialDimensionLargeObjectContextData();

  OdGePoint3d jogPoint() const;
  OdGePoint3d overrideCenter() const;
  void setJogPoint(const OdGePoint3d&);
  void setOverrideCenter(const OdGePoint3d&);

  //static OdDbRadialDimensionLargeObjectContextData * getRadialDimensionLargeAnnoScaleDefaultContextData(const OdDbDimension*);

  virtual void copyFrom(const OdRxObject* pSource);

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbRadialDimensionLargeObjectContextData> OdDbRadialDimensionLargeObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Feature Control Frame object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbFcfObjectContextData : public OdDbAnnotScaleObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbFcfObjectContextData);
  OdDbFcfObjectContextData();

  virtual OdGeVector3d horizontal() const;
  virtual OdGePoint3d location() const;
  virtual void setHorizontal(const OdGeVector3d&);
  virtual void setLocation(const OdGePoint3d&);

  //static OdDbFcfObjectContextData * getFcfAnnoScaleDefaultContextData(const OdDbFcf*);

  void transformBy(const OdGeMatrix3d&);
  virtual void copyFrom(OdRxObject const *);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbFcfObjectContextData> OdDbFcfObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Hatch Scale object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbHatchScaleContextData : public OdDbAnnotScaleObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbHatchScaleContextData);
  OdDbHatchScaleContextData();

//   virtual OdDbObjectContext *  context() const;
//   virtual OdResult  setContext(const OdDbObjectContext&);

  void appendLoop(OdInt32 loopType, const EdgeArray& edgePtrs);
  void getLoopAt(const OdDbHatch*, OdInt32 i, OdInt32 &loopType, EdgeArray& edgePtrs) const;
  void insertLoopAt(OdInt32 i, OdInt32 loopType, const EdgeArray& edgePtrs);
  OdInt32 loopTypeAt(OdInt32 i) const;
  OdInt32 numLoops() const;
  void removeLoopAt(OdInt32 i);
  void setLoopAt(OdInt32 i, OdInt32 loopType, const EdgeArray& edgePtrs);
  void setLoopTypeAt(OdInt32 i, OdInt32 loopType);
  void setNumLoops(OdInt32 i);

  const OdHatchPattern& hatchPattern() const;
  void  setHatchPattern(const OdHatchPattern&);
  OdGeVector3d patternBaseAngle( ) const;
  double  patternScale() const;
  void  setPatternBaseAngle(const OdGeVector3d&);
  void  setPatternScale(double);
//   void  setSupportsContext(OdDbHatch *);

//   void  transformBy(const OdGeMatrix3d&);
//   void  transformBy(OdInt32,const OdGeMatrix2d&,OdInt32);
//   void  transformBy(OdInt32,const OdGeMatrix3d&,const OdGeMatrix3d&,const OdGeMatrix3d&,OdGeVector3d,double);

//   OdResult  updateHatchBoundary(const OdDbHatch*,const OdArray<OdHatchBoundaryLoop>&);
//   void  verify(const OdArray<OdHatchBoundaryLoop>&) const;
  void getHatchLineDataAt( const OdDbHatch*, OdInt32 n, OdGePoint2d& startPt, OdGePoint2d& endPt) const;
  void getHatchLinesData( const OdDbHatch*, OdGePoint2dArray& startPts, OdGePoint2dArray& endPts) const;
  void setHatchLineDataAt( OdInt32 n, const OdGePoint2d& startPt, const OdGePoint2d& endPt);
  void setHatchLinesData( const OdGePoint2dArray& startPts, const OdGePoint2dArray& endPts);
  bool isValidHatchLines(const OdDbHatch*) const;
  void setEvaluateHatch(bool/*,bool*/);
  OdInt32 numHatchLines(const OdDbHatch*) const;
  void  transformBy(const OdGeMatrix2d&);

  virtual void copyFrom(OdRxObject const *);
  
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbHatchScaleContextData> OdDbHatchScaleContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Hatch View object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbHatchViewContextData : public OdDbHatchScaleContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbHatchViewContextData);
  OdDbHatchViewContextData();
/*
  OdResult appendHatchLine(const OdGePoint2d&,const OdGePoint2d&);
  OdResult compressHatch();
  virtual OdDbObjectContext * context() const;
  virtual OdResult dwgInContext(OdDbDwgFiler *,OdDbObjectContext * &);
  virtual OdResult dwgOutContext(OdDbDwgFiler *) const;
  OdResult freeUncompressedHatch();
  OdResult initCompression();
  virtual bool matchesContext(const OdDbObjectContext&) const;
  OdResult rotateHatchPattern(const OdDbHatch*,double);
  virtual OdResult setContext(const OdDbObjectContext&);
  OdResult setGrowLength(OdInt32);
  OdResult setLogicalLength(OdInt32);
  OdResult setPhysicalLength(OdInt32);
  OdResult traceHatchBoundary(const OdDbHatch*) const;
  bool viewChanged(const OdDbHatch*);
*/
  virtual void copyFrom(OdRxObject const *);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  OdDbObjectId viewportId() const;
  void setViewportId(OdDbObjectId id);
  OdGeVector3d viewTarget() const;
  void setViewTarget(const OdGeVector3d& v);
  double viewRotation() const;
  void setViewRotation(double a);
};

typedef OdSmartPtr<OdDbHatchViewContextData> OdDbHatchViewContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for a Leader object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbLeaderObjectContextData : public OdDbAnnotScaleObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbLeaderObjectContextData);
  OdDbLeaderObjectContextData();

  virtual bool addLast(const OdGePoint3d&);
  virtual OdGeVector3d annotationOffset() const;
  virtual OdGeVector3d blockInsertOffset() const;
  virtual void clearFurtherVertices();
  virtual OdResult endPoint(OdGePoint3d &) const;
  virtual const OdGePoint3dArray& furtherVertices() const;
  virtual OdGePoint3d getVertex(OdInt32) const;
  //virtual bool hasHookLine(const OdGePoint3d&) const;
  virtual bool hookLineDirSameAsHorizon() const;
  virtual OdGeVector3d horizontal() const;
  virtual OdInt32 numFurtherVertices() const;
  virtual void removeLastVertex();
  virtual OdResult setAnnotationOffset(const OdGeVector3d&);
  virtual OdResult setBlockInsertOffset(const OdGeVector3d&);
  virtual OdResult setFurtherVertices(const OdGePoint3dArray&);
  virtual OdResult setHookLineDirSameAsHorizon(bool);
  virtual OdResult setHorizontal(const OdGeVector3d&);
  virtual void setVertex(OdInt32,const OdGePoint3d&);
  //virtual OdResult transformBy(const OdGeMatrix3d&);

  //static OdDbLeaderObjectContextData * getLeaderAnnoScaleDefaultContextData(OdDbLeader const *);

  virtual void copyFrom(OdRxObject const *);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbLeaderObjectContextData> OdDbLeaderObjectContextDataPtr;

/** \details
    This class defines a set of annotation-specific properties for an MLeader object.
  
    \remarks
    This class is intended for application developers only, not TX developers.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbMLeaderObjectContextData : public OdDbAnnotScaleObjectContextData
{
public:
  ODDB_DECLARE_MEMBERS(OdDbMLeaderObjectContextData);
  OdDbMLeaderObjectContextData();

  //static OdDbMLeaderObjectContextData * getMLeaderAnnoScaleDefaultContextData(OdDbMLeader const *);
  //virtual OdResult updateContentScale(OdDbMLeaderObjectContextData* pData);

  virtual void copyFrom(OdRxObject const *);
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbMLeaderObjectContextData> OdDbMLeaderObjectContextDataPtr;

#endif //_DBOBJECTCONTEXTDATA_H_INCLUDED_
