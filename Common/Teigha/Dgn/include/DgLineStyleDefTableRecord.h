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
#ifndef __DG_LINESTYLEDEFTABLERECORD_H__
#define __DG_LINESTYLEDEFTABLERECORD_H__


#include "TD_PackPush.h"

#include "SharedPtr.h"
#include "OdBinaryData.h"
#include "UInt64Array.h"
#include "DgLineStyleTableRecord.h"

class OdDgLineStyleResource;
class OdGiLinetypeDash;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLineStyleResource object pointers.
*/
typedef OdSmartPtr<OdDgLineStyleResource> OdDgLineStyleResourcePtr;

/** \details
    This class represents abstract LineStyle resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineStyleResource : public OdRxObject
{
public:

  enum OdLsResourceType
  {
    kLsUnknownRes       = -1,
    kLsInternalRes      = 0,
    kLsPointSymbolRes   = 1,
    kLsPointSymbolResV7 = 2,
    kLsCompoundRes      = 3,
    kLsLinePointRes     = 4,
    kLsLineCodeRes      = 5,
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgLineStyleResource);

  OdDgLineStyleResource() {}
  virtual ~OdDgLineStyleResource() {}

  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize ) = 0;
  virtual void dgnOutFields( OdDgFiler* pFiler ) const = 0;

  virtual OdUInt16 getType() const = 0;
  virtual OdUInt16 getRawDataSize() const = 0;

  static OdDgLineStyleResourcePtr createObject(OdUInt16 type);
  static OdDgLineStyleResourcePtr createObject(OdUInt32 rscid);
};

/** \details
    This class represents PointSymbol line style resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgPointSymbolResource : public OdDgLineStyleResource
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPointSymbolResource);

  OdDgPointSymbolResource();
  virtual ~OdDgPointSymbolResource();

  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize );
  virtual void dgnOutFields( OdDgFiler* pFiler ) const;

  virtual OdUInt16 getType() const;
  virtual OdUInt16 getRawDataSize() const;

  OdString         getDescription() const;
  void             setDescription( const OdString& strDescription );

  OdGePoint3d      getSymbolOffset()  const;
  void             setSymbolOffset( OdGePoint3d ptOffset );

  double           getSymbolScale() const;
  void             setSymbolScale( double dScale );

  OdUInt64         getDependedCellHeaderHandle() const;
  void             setDependedCellHeaderHandle( OdUInt64 idHandle  );

  OdGeExtents3d    getSymbolExtents() const;
  void             setSymbolExtents( const OdGeExtents3d& extSymbol );

  void             getSymbolBuffer( OdBinaryData& symbolData ) const;
  void             setSymbolBuffer( const OdBinaryData& symbolData );

  bool             getSymbol3DFlag() const;
  void             setSymbol3DFlag( bool bSet );

  OdDgElementId    getCell( OdDgDatabase* pDb, OdUInt32 symbolEntryId, bool bV7Symbol );

private:

  OdInt8       m_description[128];
  OdUInt64     m_reserved1;
  OdUInt32     m_auxType;
  OdUInt32     m_reserved2;
  OdGePoint3d  m_rangeLow;
  OdGePoint3d  m_rangeHigh;
  OdGePoint3d  m_offset;
  double       m_scale;
  double       m_reserved3[12];
  OdUInt32     m_symbolFlags;
  OdBinaryData m_symbolBuffer;

  OdUInt64     m_uCellHeaderId;

  OdGiLinetypeDash* m_pShapeDash;
  friend class OdDgLineStyleShaper;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgPointSymbolResource object pointers.
*/
typedef OdSmartPtr<OdDgPointSymbolResource> OdDgPointSymbolResourcePtr;


/** \details
    This class represents ComponentInfo of Compound line style resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCompoundLineStyleComponentInfo
{
public:
  OdDgCompoundLineStyleComponentInfo();
  ~OdDgCompoundLineStyleComponentInfo();

  void dgnInFields( OdDgFiler* pFiler );
  void dgnOutFields( OdDgFiler* pFiler ) const;

public:

  OdDgLineStyleResource::OdLsResourceType getComponentType() const;
  void                setComponentType( OdDgLineStyleResource::OdLsResourceType uType );

  OdUInt64            getComponentHandleId() const;
  void                setComponentHandleId( OdUInt64 uComponentId );

  OdUInt32            getComponentEntryId() const;
  void                setComponentEntryId( OdUInt32 uEntryId );

  double              getComponentOffset() const;
  void                setComponentOffset( double dYOffset );

private:

  OdUInt32    m_rscId;
  OdUInt32    m_id;
  OdUInt32    m_reserved1[4];
  double      m_offset;
  OdUInt64    m_reserved2[3];

  OdUInt64    m_uComponentId;
};

/** \details
    This class represents Compound line style resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgCompoundLineStyleResource : public OdDgLineStyleResource
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCompoundLineStyleResource);

  OdDgCompoundLineStyleResource();
  ~OdDgCompoundLineStyleResource();

  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize );
  virtual void dgnOutFields( OdDgFiler* pFiler ) const;

  virtual OdUInt16 getType() const;
  virtual OdUInt16 getRawDataSize() const;

  OdString         getDescription() const;
  void             setDescription( const OdString& strDescription );

  OdUInt32         getComponentCount() const;
  bool             getComponent( OdUInt32 uIndex, OdDgCompoundLineStyleComponentInfo& component ) const;
  bool             setComponent( OdUInt32 uIndex, const OdDgCompoundLineStyleComponentInfo& component );
  void             addComponent( const OdDgCompoundLineStyleComponentInfo& component );
  bool             deleteComponent( OdUInt32 uIndex );

private:

  OdInt8      m_description[128];
  OdUInt64    m_reserved1;
  OdUInt32    m_auxType;
  OdUInt32    m_reserved2[8];
  OdArray<OdDgCompoundLineStyleComponentInfo> m_components;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgCompoundLineStyleResource object pointers.
*/
typedef OdSmartPtr<OdDgCompoundLineStyleResource> OdDgCompoundLineStyleResourcePtr;


/** \details
    This class represents StrokeData of LineCode line style resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineCodeResourceStrokeData
{
public:

  enum OdLsStrokeWidthMode
  {
    kLsNoWidth    = 0,
    kLsWidthLeft  = 1,
    kLsWidthRight = 2,
    kLsWidthFull  = 3
  };

  enum OdLsStrokeCapsType
  {
    kLsCapsClosed   = 0,
    kLsCapsOpen     = 1,
    kLsCapsExtended = 2,
    kLsCapsHexagon  = 3,
    kLsCapsOctagon  = 4,
    kLsCapsDecagon  = 5,
    kLsCapsArc      = 30
  };

public:
  OdDgLineCodeResourceStrokeData();
  ~OdDgLineCodeResourceStrokeData();

  void dgnInFields( OdDgFiler* pFiler );
  void dgnOutFields( OdDgFiler* pFiler ) const;

  double getLength() const;
  void   setLength( double dLength );

  double getStartWidth() const;
  void   setStartWidth(  double dWidth  );

  double getEndWidth() const;
  void   setEndWidth(  double dWidth  );

  bool   getDashFlag() const;
  void   setDashFlag( bool bSet );

  bool   getByPassCornerFlag() const;
  void   setByPassCornerFlag( bool bSet );

  bool   getCanBeScaledFlag() const;
  void   setCanBeScaledFlag( bool bSet );

  bool   getInvertStrokeInFirstCodeFlag() const;
  void   setInvertStrokeInFirstCodeFlag( bool bSet );

  bool   getInvertStrokeInLastCodeFlag() const;
  void   setInvertStrokeInLastCodeFlag( bool bSet );

  OdLsStrokeWidthMode getWidthMode() const;
  void   setWidthMode( OdLsStrokeWidthMode iMode );

  bool   getIncreasingTaperFlag() const;
  void   setIncreasingTaperFlag( bool bSet );

  bool   getDecreasingTaperFlag() const;
  void   setDecreasingTaperFlag( bool bSet );

  OdLsStrokeCapsType getCapsType() const;
  void   setCapsType( OdLsStrokeCapsType iType );

private:

  double      m_length;
  double      m_width;
  double      m_endWidth;
  OdUInt32    m_reserved1[5];
  OdUInt8     m_strokeMode;
  OdUInt8     m_widthMode;
  OdUInt8     m_capMode;
  OdUInt8     m_reserved2;
};

/** \details
    This class represents LineCode line style resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineCodeResource : public OdDgLineStyleResource
{
public:
  ODRX_DECLARE_MEMBERS(OdDgLineCodeResource);

  OdDgLineCodeResource();
  virtual ~OdDgLineCodeResource();

  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize );
  virtual void dgnOutFields( OdDgFiler* pFiler ) const;

  virtual OdUInt16 getType() const;
  virtual OdUInt16 getRawDataSize() const;

  OdString         getDescription() const;
  void             setDescription( const OdString& strDescription );

  double           getPhase() const;
  void             setPhase( double dPhase );

  bool             getAutoPhaseFlag() const;
  void             setAutoPhaseFlag( bool bSet );

  bool             getUseIterationLimitFlag() const;
  void             setUseIterationLimitFlag( bool bSet );

  bool             getSingleSegmentModeFlag() const;
  void             setSingleSegmentModeFlag( bool bSet );

  bool             getCenterStretchPhaseModeFlag() const;
  void             setCenterStretchPhaseModeFlag( bool bSet );

  OdUInt32         getMaxIterations() const;
  void             setMaxIterations( OdUInt32 iMaxNum );

  OdUInt32         getStrokeCount() const;
  bool             getStroke( OdUInt32 uIndex, OdDgLineCodeResourceStrokeData& strokeData ) const;
  bool             setStroke( OdUInt32 uIndex, const OdDgLineCodeResourceStrokeData& strokeData );
  void             addStroke( const OdDgLineCodeResourceStrokeData& strokeData );
  bool             deleteStroke( OdUInt32 uIndex );

private:

  OdInt8      m_description[128];
  OdUInt64    m_reserved1;
  OdUInt32    m_auxType;
  OdUInt32    m_reserved2;
  double      m_phase;
  double      m_reserved3[5];
  OdUInt32    m_options;
  OdUInt32    m_maxIterate;
  OdUInt32    m_reserved4;
  OdArray<OdDgLineCodeResourceStrokeData> m_strokes;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLineCodeResource object pointers.
*/
typedef OdSmartPtr<OdDgLineCodeResource> OdDgLineCodeResourcePtr;


/** \details
    This class represents SymInfo of LinePoint line style resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinePointResourceSymInfo
{
public:

  enum OdLsSymbolPosOnStroke
  {
    kLsNoSymbol         = 0,
    kLsAtOriginOfStroke = 1,
    kLsAtEndOfStroke    = 2,
    kLsAtCenterOfStroke = 3
  };

public:
  OdDgLinePointResourceSymInfo();
  ~OdDgLinePointResourceSymInfo();

  void dgnInFields( OdDgFiler* pFiler );
  void dgnOutFields( OdDgFiler* pFiler ) const;

  OdDgLineStyleResource::OdLsResourceType getSymbolType() const;
  void                setSymbolType( OdDgLineStyleResource::OdLsResourceType iType );

  OdUInt64            getPointSymbolHandleId() const;
  void                setPointSymbolHandleId( OdUInt64 idPtSymbol );

  OdUInt32            getPointSymbolEntryId() const;
  void                setPointSymbolEntryId( OdUInt32 idPtSymbol );

  OdUInt16            getSymbolStrokeNo() const;
  void                setSymbolStrokeNo( OdUInt16 uStrokeNo );

  OdLsSymbolPosOnStroke getSymbolPosOnStroke() const;
  void                setSymbolPosOnStroke( OdLsSymbolPosOnStroke iMode );

  bool                getSymbolAtElementOriginFlag() const;
  void                setSymbolAtElementOriginFlag( bool bSet );
  bool                getSymbolAtElementEndFlag() const;
  void                setSymbolAtElementEndFlag( bool bSet );
  bool                getSymbolAtEachVertexFlag() const;
  void                setSymbolAtEachVertexFlag( bool bSet );
  bool                getMirrorSymbolForReversedLinesFlag() const;
  void                setMirrorSymbolForReversedLinesFlag( bool bSet );
  bool                getAbsoluteRotationAngleFlag() const;
  void                setAbsoluteRotationAngleFlag( bool bSet );
  bool                getDoNotScaleElementFlag() const;
  void                setDoNotScaleElementFlag( bool bSet );
  bool                getDoNotClipElementFlag() const;
  void                setDoNotClipElementFlag( bool bSet );
  bool                getNoPartialStrokesFlag() const;
  void                setNoPartialStrokesFlag( bool bSet );
  bool                getPartialOriginBeyondEndFlag() const;
  void                setPartialOriginBeyondEndFlag( bool bSet );
  bool                getUseSymbolColorFlag() const;
  void                setUseSymbolColorFlag( bool bSet );
  bool                getUseSymbolWeightFlag() const;
  void                setUseSymbolWeightFlag( bool bSet );

  OdGePoint2d         getOffset() const;
  void                setOffset( OdGePoint2d ptOffset );

  double              getRotationAngle() const;
  void                setRotationAngle( double dRotAngle );

private:

  OdUInt32  m_symType;
  OdUInt32  m_symID;
  OdUInt16  m_strokeNo;
  OdUInt16  m_mod1;
  OdUInt32  m_reserved1;
  double    m_xOffset;
  double    m_yOffset;
  double    m_reserved2[3];
  double    m_zAngle;
  double    m_reserved3[3];

  OdUInt64  m_uPtSymbolId;
};

/** \details
    This class represents LinePoint line style resource.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinePointResource : public OdDgLineStyleResource
{
public:
  ODRX_DECLARE_MEMBERS(OdDgLinePointResource);

  OdDgLinePointResource();
  virtual ~OdDgLinePointResource();

  virtual void dgnInFields( OdDgFiler* pFiler, OdUInt32 dataSize );
  virtual void dgnOutFields( OdDgFiler* pFiler ) const;

  virtual OdUInt16 getType() const;
  virtual OdUInt16 getRawDataSize() const;

  OdString         getDescription() const;
  void             setDescription( const OdString& strDescription );

  OdDgLineStyleResource::OdLsResourceType getBasePatternType() const;
  void             setBasePatternType( OdDgLineStyleResource::OdLsResourceType iType );

  OdUInt64         getBasePatternHandleId() const;
  void             setBasePatternHandleId( OdUInt64 idBasePattern );

  OdUInt32         getBasePatternEntryId() const;
  void             setBasePatternEntryId( OdUInt32 idEntry );

  OdUInt32         getSymbolCount() const;
  bool             getSymbol( OdUInt32 uIndex, OdDgLinePointResourceSymInfo& symbolInfo ) const;
  bool             setSymbol( OdUInt32 uIndex, const OdDgLinePointResourceSymInfo& symbolInfo );
  void             addSymbol( const OdDgLinePointResourceSymInfo& symbolInfo );
  bool             deleteSymbol( OdUInt32 uIndex );

private:

  OdInt8      m_description[128];
  OdUInt64    m_reserved1;
  OdUInt32    m_auxType;
  OdUInt32    m_reserved2;
  double      m_reserved3;
  OdUInt32    m_lcType;
  OdUInt32    m_lcID;
  OdUInt32    m_reserved4;
  OdArray<OdDgLinePointResourceSymInfo> m_symbols;

  OdUInt64    m_uBaseId;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLinePointResource object pointers.
*/
typedef OdSmartPtr<OdDgLinePointResource> OdDgLinePointResourcePtr;


class OdDgLineStyleDefTableRecord;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgLineStyleDefTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgLineStyleDefTableRecord> OdDgLineStyleDefTableRecordPtr;

/** \details
    This class represents LineStyle records in the OdDgLineStyleDefTableRecord in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineStyleDefTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgLineStyleDefTableRecord);
public:

  static OdDgLineStyleDefTableRecordPtr createObject( OdDg::LineStyleType type );

  OdDg::LineStyleType getType() const;
  void setType(OdDg::LineStyleType val);

  OdDgLineStyleResourcePtr getResource();
};



#include "TD_PackPop.h"

#endif // __DG_LINESTYLEDEFTABLERECORD_H__
