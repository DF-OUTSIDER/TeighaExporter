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

#ifndef __ODGI_DGLINETYPE_H__
#define __ODGI_DGLINETYPE_H__

#include "Gi/GiNonEntityTraits.h"
#include "Gi/GiLinetype.h"
#include "Gi/GiExport.h"

#include "TD_PackPush.h"

/** \details

    <group OdGi_Classes>
*/
struct OdGiDgLinetypeDash
{
  struct DashInfo
  {
    enum StrokeWidthMode
    {
      kLsNoWidth    = 0,
      kLsWidthLeft  = 1,
      kLsWidthRight = 2,
      kLsWidthFull  = 3
    };
    enum StrokeCapsType
    {
      kLsCapsClosed   = 0,
      kLsCapsOpen     = 1,
      kLsCapsExtended = 2,
      kLsCapsHexagon  = 3,
      kLsCapsOctagon  = 4,
      kLsCapsDecagon  = 5,
      kLsCapsArc      = 30
    };
    enum Flags
    {
      kFlagDash                    = (1 << 0),
      kFlagPassCorner              = (1 << 1),
      kFlagCanBeScaled             = (1 << 2),
      kFlagInvertStrokeInFirstCode = (1 << 3),
      kFlagInvertStrokeInLastCode  = (1 << 4),
      kFlagIncreasingTaper         = (1 << 5),
      kFlagDecreasingTaper         = (1 << 6),
      kFlagBaseStrokeDash          = (1 << 7)
    };

    OdUInt32        m_uFlags;
    double          m_dLength;
    double          m_dStartWidth;
    double          m_dEndWidth;
    StrokeWidthMode m_lsWidthMode;
    StrokeCapsType  m_lsCapsType;

    DashInfo()
      : m_uFlags(0)
      , m_dLength(0.0)
      , m_dStartWidth(0.0)
      , m_dEndWidth(0.0)
      , m_lsWidthMode(kLsNoWidth)
      , m_lsCapsType(kLsCapsClosed)
    { }

    double getLength() const { return m_dLength; }
    void setLength(double dLength) { m_dLength = dLength; }

    double getStartWidth() const { return m_dStartWidth; }
    void setStartWidth(double dWidth) { m_dStartWidth = dWidth; }

    double getEndWidth() const { return m_dEndWidth; }
    void setEndWidth(double dWidth) { m_dEndWidth = dWidth; }

    bool getDashFlag() const { return GETBIT(m_uFlags, kFlagDash); }
    void setDashFlag(bool bSet) { SETBIT(m_uFlags, kFlagDash, bSet); }

    bool getByPassCornerFlag() const { return GETBIT(m_uFlags, kFlagPassCorner); }
    void setByPassCornerFlag(bool bSet) { SETBIT(m_uFlags, kFlagPassCorner, bSet); }

    bool getCanBeScaledFlag() const { return GETBIT(m_uFlags, kFlagCanBeScaled); }
    void setCanBeScaledFlag(bool bSet) { SETBIT(m_uFlags, kFlagCanBeScaled, bSet); }

    bool getInvertStrokeInFirstCodeFlag() const { return GETBIT(m_uFlags, kFlagInvertStrokeInFirstCode); }
    void setInvertStrokeInFirstCodeFlag(bool bSet) { SETBIT(m_uFlags, kFlagInvertStrokeInFirstCode, bSet); }

    bool getInvertStrokeInLastCodeFlag() const { return GETBIT(m_uFlags, kFlagInvertStrokeInLastCode); }
    void setInvertStrokeInLastCodeFlag(bool bSet) { SETBIT(m_uFlags, kFlagInvertStrokeInLastCode, bSet); }

    StrokeWidthMode getWidthMode() const { return m_lsWidthMode; }
    void setWidthMode(StrokeWidthMode iMode) { m_lsWidthMode = iMode; }

    bool getIncreasingTaperFlag() const { return GETBIT(m_uFlags, kFlagIncreasingTaper); }
    void setIncreasingTaperFlag(bool bSet) { SETBIT(m_uFlags, kFlagIncreasingTaper, bSet); }

    bool getDecreasingTaperFlag() const { return GETBIT(m_uFlags, kFlagDecreasingTaper); }
    void setDecreasingTaperFlag(bool bSet) { SETBIT(m_uFlags, kFlagDecreasingTaper, bSet); }

    bool getBaseStrokeDashFlag() const { return GETBIT(m_uFlags, kFlagBaseStrokeDash); }
    void setBaseStrokeDashFlag(bool bSet) { SETBIT(m_uFlags, kFlagBaseStrokeDash, bSet); }

    bool isDashOrBaseStrokeDash() const { return GETBIT(m_uFlags, kFlagDash | kFlagBaseStrokeDash); }

    StrokeCapsType getCapsType() const { return m_lsCapsType; }
    void setCapsType(StrokeCapsType iType) { m_lsCapsType = iType; }
  } m_dash;
  struct ShapeInfo
  {
    enum SymbolPosOnStroke
    {
      kLsNoSymbol         = 0,
      kLsAtOriginOfStroke = 1,
      kLsAtEndOfStroke    = 2,
      kLsAtCenterOfStroke = 3
    };
    enum Flags
    {
      kFlagSymbolAtElementOrigin        = (1 << 0),
      kFlagSymbolAtElementEnd           = (1 << 1),
      kFlagSymbolAtEachVertex           = (1 << 2),
      kFlagMirrorSymbolForReversedLines = (1 << 3),
      kFlagAbsoluteRotationAngle        = (1 << 4),
      kFlagDoNotScaleElement            = (1 << 5),
      kFlagDoNotClipElement             = (1 << 6),
      kFlagNoPartialStrokes             = (1 << 7),
      kFlagPartialOriginBeyondEnd       = (1 << 8),
      kFlagUseSymbolColor               = (1 << 9),
      kFlagUseSymbolWeight              = (1 << 10)
    };

    OdUInt32                 m_uFlags;
    OdDbStub                *m_pSymbol;
    SymbolPosOnStroke        m_lsPos;
    OdGiLinetypeDash::offset m_offset;
    double                   m_dRotation;
    double                   m_dScale;

    ShapeInfo()
      : m_uFlags(0)
      , m_pSymbol(NULL)
      , m_lsPos(kLsNoSymbol)
      , m_dRotation(0.0)
      , m_dScale(0.0)
    {
      m_offset.x = m_offset.y = 0.0;
    }

    OdDbStub *getSymbolId() const { return m_pSymbol; }
    void setSymbolId(OdDbStub *pStub) { m_pSymbol = pStub; }

    SymbolPosOnStroke getSymbolPosOnStroke() const { return m_lsPos; }
    void setSymbolPosOnStroke(SymbolPosOnStroke iMode) { m_lsPos = iMode; }

    bool getSymbolAtElementOriginFlag() const { return GETBIT(m_uFlags, kFlagSymbolAtElementOrigin); }
    void setSymbolAtElementOriginFlag(bool bSet) { SETBIT(m_uFlags, kFlagSymbolAtElementOrigin, bSet); }

    bool getSymbolAtElementEndFlag() const { return GETBIT(m_uFlags, kFlagSymbolAtElementEnd); }
    void setSymbolAtElementEndFlag(bool bSet) { SETBIT(m_uFlags, kFlagSymbolAtElementEnd, bSet); }

    bool getSymbolAtEachVertexFlag() const { return GETBIT(m_uFlags, kFlagSymbolAtEachVertex); }
    void setSymbolAtEachVertexFlag(bool bSet) { SETBIT(m_uFlags, kFlagSymbolAtEachVertex, bSet); }

    bool getMirrorSymbolForReversedLinesFlag() const { return GETBIT(m_uFlags, kFlagMirrorSymbolForReversedLines); }
    void setMirrorSymbolForReversedLinesFlag(bool bSet) { SETBIT(m_uFlags, kFlagMirrorSymbolForReversedLines, bSet); }

    bool getAbsoluteRotationAngleFlag() const { return GETBIT(m_uFlags, kFlagAbsoluteRotationAngle); }
    void setAbsoluteRotationAngleFlag(bool bSet) { SETBIT(m_uFlags, kFlagAbsoluteRotationAngle, bSet); }

    bool getDoNotScaleElementFlag() const { return GETBIT(m_uFlags, kFlagDoNotScaleElement); }
    void setDoNotScaleElementFlag(bool bSet) { SETBIT(m_uFlags, kFlagDoNotScaleElement, bSet); }

    bool getDoNotClipElementFlag() const { return GETBIT(m_uFlags, kFlagDoNotClipElement); }
    void setDoNotClipElementFlag(bool bSet) { SETBIT(m_uFlags, kFlagDoNotClipElement, bSet); }

    bool getNoPartialStrokesFlag() const { return GETBIT(m_uFlags, kFlagNoPartialStrokes); }
    void setNoPartialStrokesFlag(bool bSet) { SETBIT(m_uFlags, kFlagNoPartialStrokes, bSet); }

    bool getPartialOriginBeyondEndFlag() const { return GETBIT(m_uFlags, kFlagPartialOriginBeyondEnd); }
    void setPartialOriginBeyondEndFlag(bool bSet) { SETBIT(m_uFlags, kFlagPartialOriginBeyondEnd, bSet); }

    bool getUseSymbolColorFlag() const { return GETBIT(m_uFlags, kFlagUseSymbolColor); }
    void setUseSymbolColorFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseSymbolColor, bSet); }

    bool getUseSymbolWeightFlag() const { return GETBIT(m_uFlags, kFlagUseSymbolWeight); }
    void setUseSymbolWeightFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseSymbolWeight, bSet); }

    void getOffset(double &x, double &y) const { x = m_offset.x; y = m_offset.y; }
    void setOffset(double x, double y) { m_offset.x = x; m_offset.y = y; }

    double getRotationAngle() const { return m_dRotation; }
    void setRotationAngle(double dRotAngle) { m_dRotation = dRotAngle; }

    double getSymbolScale() const { return m_dScale; }
    void setSymbolScale(double dScale) { m_dScale = dScale; }
  } m_shape;

  OdGiDgLinetypeDash()
    : m_dash()
    , m_shape()
  {
  }
};

typedef OdArray<OdGiDgLinetypeDash> OdGiDgLinetypeDashArray;

/** \details

    <group OdGi_Classes>
*/
struct OdGiDgLinetypeItem
{
  enum Flags
  {
    kFlagAutoPhase              = (1 << 0),
    kFlagUseIterationLimit      = (1 << 1),
    kFlagSingleSegmentMode      = (1 << 2),
    kFlagCenterStretchPhaseMode = (1 << 3),
    kFlagStandardLinetype       = (1 << 4),
    kFlagComputeStandardScale   = (1 << 5)
  };

  OdUInt32                m_uFlags;
  double                  m_dLength;
  double                  m_dPhase;
  OdUInt32                m_nMaxIterations;
  double                  m_dOffset;
  OdGiDgLinetypeDashArray m_dashes;

  OdGiDgLinetypeItem()
    : m_uFlags(0)
    , m_dLength(0.0)
    , m_dPhase(0.0)
    , m_nMaxIterations(1)
    , m_dOffset(0.0)
  {
  }

  double getLength() const { return m_dLength; }
  void setLength(double dLen) { m_dLength = dLen; }

  double getPhase() const { return m_dPhase; }
  void setPhase(double dPhase) { m_dPhase = dPhase; }

  bool getAutoPhaseFlag() const { return GETBIT(m_uFlags, kFlagAutoPhase); }
  void setAutoPhaseFlag(bool bSet) { SETBIT(m_uFlags, kFlagAutoPhase, bSet); }

  bool getUseIterationLimitFlag() const { return GETBIT(m_uFlags, kFlagUseIterationLimit); }
  void setUseIterationLimitFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseIterationLimit, bSet); }

  bool getSingleSegmentModeFlag() const { return GETBIT(m_uFlags, kFlagSingleSegmentMode); }
  void setSingleSegmentModeFlag(bool bSet) { SETBIT(m_uFlags, kFlagSingleSegmentMode, bSet); }

  bool getCenterStretchPhaseModeFlag() const { return GETBIT(m_uFlags, kFlagCenterStretchPhaseMode); }
  void setCenterStretchPhaseModeFlag(bool bSet) { SETBIT(m_uFlags, kFlagCenterStretchPhaseMode, bSet); }

  bool getStandardLinetypeFlag() const { return GETBIT(m_uFlags, kFlagStandardLinetype); }
  void setStandardLinetypeFlag(bool bSet) { SETBIT(m_uFlags, kFlagStandardLinetype, bSet); }

  bool getComputeStandardScaleFlag() const { return GETBIT(m_uFlags, kFlagComputeStandardScale); }
  void setComputeStandardScaleFlag(bool bSet) { SETBIT(m_uFlags, kFlagComputeStandardScale, bSet); }

  OdUInt32 getMaxIterations() const { return m_nMaxIterations; }
  void setMaxIterations(OdUInt32 iMaxNum) { m_nMaxIterations = iMaxNum; }

  double getYOffset() const { return m_dOffset; }
  void setYOffset(double dYOffset) { m_dOffset = dYOffset; }

  OdUInt32 numDashes() const { return m_dashes.size(); }
  void setNumDashes(OdUInt32 count) { m_dashes.resize(count); }

  void dashAt(OdUInt32 index, OdGiDgLinetypeDash& dash) const { dash = m_dashes.at(index); }
  const OdGiDgLinetypeDash& dashAt(OdUInt32 index) const { return m_dashes.at(index); }

  // void setDashAt(OdUInt32 /*index*/, double /*value*/) { throw OdError(eNotImplementedYet); }
  void setDashAt(OdUInt32 index, const OdGiDgLinetypeDash& dash) { m_dashes[index] = dash; }

  void dashes(OdGiDgLinetypeDashArray& dashes) const { dashes = m_dashes; }
  void setDashes(const OdGiDgLinetypeDashArray& dashes) { m_dashes = dashes; }
};

typedef OdArray<OdGiDgLinetypeItem> OdGiDgLinetypeItemArray;

/** \details

    <group OdGi_Classes> 
*/
class OdGiDgLinetype
{
  enum
  {
    kByBlock            = 1,
    kByLayer            = 2
  };

  OdUInt32                m_uFlags;
  OdGiDgLinetypeItemArray m_items;
  double                  m_dPatLength;
public:
  OdGiDgLinetype()
    : m_uFlags(0)
    , m_dPatLength(0.0)
  {
  }

  bool isContinuous() const { return m_items.empty(); }

  bool isByBlock() const { return GETBIT(m_uFlags, kByBlock); }
  void setByBlock(bool value) { SETBIT(m_uFlags, kByBlock, value); }

  bool isByLayer() const { return GETBIT(m_uFlags, kByLayer); }
  void setByLayer(bool value) { SETBIT(m_uFlags, kByLayer, value); }

  double patternLength() const { return m_dPatLength; }
  void setPatternLength(double dPatLen) { m_dPatLength = dPatLen; }

  OdUInt32 numItems() const { return m_items.size(); }
  void setNumItems(OdUInt32 count) { m_items.resize(count); }

  void itemAt(OdUInt32 index, OdGiDgLinetypeItem& item) const { item = m_items.at(index); }
  const OdGiDgLinetypeItem& itemAt(OdUInt32 index) const { return m_items.at(index); }

  void setItemAt(OdUInt32 index, const OdGiDgLinetypeItem& item) { m_items[index] = item; }

  void items(OdGiDgLinetypeItemArray& items) const { items = m_items; }
  void setItems(const OdGiDgLinetypeItemArray& items) { m_items = items; }
};

/** \details

    <group OdGi_Classes>
*/
struct OdGiDgLinetypeModifiers
{
  enum Flags
  {
    kFlagUseDashScale              = (1 << 0),
    kFlagUseGapScale               = (1 << 1),
    kFlagUseWidth                  = (1 << 2),
    kFlagUseEndWidth               = (1 << 3),
    kFlagUseShift                  = (1 << 4),
    kFlagUseFractionShift          = (1 << 5),
    kFlagUseCenteredShift          = (1 << 6),
    kFlagTrueWidth                 = (1 << 7),
    kFlagOverrideBreakAtCorners    = (1 << 8),
    kFlagOverrideRunThroughCorners = (1 << 9),

    kFlagsWidthMask                = (kFlagUseWidth | kFlagUseEndWidth),
    kFlagsShiftMask                = (kFlagUseShift | kFlagUseFractionShift | kFlagUseCenteredShift),
    kFlagsCornersMask              = (kFlagOverrideBreakAtCorners | kFlagOverrideRunThroughCorners)
  };

  enum WidthMode
  {
    kLsNoWidth = 0,
    kLsWidthConstant,
    kLsWidthTapered
  };
  enum ShiftMode
  {
    kLsNoShift = 0,
    kLsShiftDistance,
    kLsShiftFraction,
    kLsShiftCentered
  };
  enum CornersMode
  {
    kLsCornersFromLS = 0,
    kLsBreakAtCorners,
    kLsRunThroughCorners
  };

  OdUInt32 m_uFlags;
  double m_dDashScale;
  double m_dGapScale;
  double m_dWidth;
  double m_dEndWidth;
  double m_dPhase;

  OdGiDgLinetypeModifiers()
    : m_uFlags(0)
    , m_dDashScale(1.0)
    , m_dGapScale(1.0)
    , m_dWidth(0.0)
    , m_dEndWidth(0.0)
    , m_dPhase(0.0)
  {
  }

  bool getDashScaleFlag() const { return GETBIT(m_uFlags, kFlagUseDashScale); }
  void setDashScaleFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseDashScale, bSet); }

  double getDashScale() const { return m_dDashScale; }
  void setDashScale(double dScale) { m_dDashScale = dScale; }

  bool getGapScaleFlag() const { return GETBIT(m_uFlags, kFlagUseGapScale); }
  void setGapScaleFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseGapScale, bSet); }

  double getGapScale() const { return m_dGapScale; }
  void setGapScale(double dScale) { m_dGapScale = dScale; }

  bool getWidthFlag() const { return GETBIT(m_uFlags, kFlagUseWidth); }
  void setWidthFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseWidth, bSet); }

  double getWidth() const { return m_dWidth; }
  void setWidth(double dWidth) { m_dWidth = dWidth; }

  bool getEndWidthFlag() const { return GETBIT(m_uFlags, kFlagUseEndWidth); }
  void setEndWidthFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseEndWidth, bSet); }

  double getEndWidth() const { return m_dEndWidth; }
  void setEndWidth(double dEndWidth) { m_dEndWidth = dEndWidth; }
  
  WidthMode getWidthMode() const;
  void setWidthMode(WidthMode mode);

  bool getShiftFlag() const { return GETBIT(m_uFlags, kFlagUseShift); }
  void setShiftFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseShift, bSet); }

  bool getFractionShiftFlag() const { return GETBIT(m_uFlags, kFlagUseFractionShift); }
  void setFractionShiftFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseFractionShift, bSet); }

  bool getCenteredShiftFlag() const { return GETBIT(m_uFlags, kFlagUseCenteredShift); }
  void setCenteredShiftFlag(bool bSet) { SETBIT(m_uFlags, kFlagUseCenteredShift, bSet); }

  double getPhase() const { return m_dPhase; }
  void setPhase(double dPhase) { m_dPhase = dPhase; }

  ShiftMode getShiftMode() const;
  void setShiftMode(ShiftMode mode);

  bool getTrueWidthFlag() const { return GETBIT(m_uFlags, kFlagTrueWidth); }
  void setTrueWidthFlag(bool bSet) { SETBIT(m_uFlags, kFlagTrueWidth, bSet); }

  bool getBreakAtCornersFlag() const { return GETBIT(m_uFlags, kFlagOverrideBreakAtCorners); }
  void setBreakAtCornersFlag(bool bSet) { SETBIT(m_uFlags, kFlagOverrideBreakAtCorners, bSet); }

  bool getRunThroughCornersFlag() const { return GETBIT(m_uFlags, kFlagOverrideRunThroughCorners); }
  void setRunThroughCornersFlag(bool bSet) { SETBIT(m_uFlags, kFlagOverrideRunThroughCorners, bSet); }

  CornersMode getCornersMode() const;
  void setCornersMode(CornersMode mode);

  bool operator==(const OdGiDgLinetypeModifiers& lsMod) const;
  bool operator!=(const OdGiDgLinetypeModifiers& lsMod) const;
};

inline OdGiDgLinetypeModifiers::WidthMode OdGiDgLinetypeModifiers::getWidthMode() const
{
  switch (m_uFlags & kFlagsWidthMask)
  {
    case kFlagUseWidth:   return kLsWidthConstant;
    case kFlagsWidthMask: return kLsWidthTapered;
    default:              return kLsNoWidth;
  }
}

inline void OdGiDgLinetypeModifiers::setWidthMode(OdGiDgLinetypeModifiers::WidthMode mode)
{
  switch (mode)
  {
    case kLsWidthConstant: SETBIT_1(m_uFlags, kFlagUseWidth); SETBIT_0(m_uFlags, kFlagUseEndWidth); break;
    case kLsWidthTapered:  SETBIT_1(m_uFlags, kFlagsWidthMask); break;
    default: SETBIT_0(m_uFlags, kFlagsWidthMask);
  };
}

inline OdGiDgLinetypeModifiers::ShiftMode OdGiDgLinetypeModifiers::getShiftMode() const
{
  switch (m_uFlags & kFlagsShiftMask)
  {
    case kFlagUseShift:                         return kLsShiftDistance;
    case kFlagUseShift | kFlagUseFractionShift: return kLsShiftFraction;
    case kFlagUseShift | kFlagUseCenteredShift: return kLsShiftCentered;
    default:                                    return kLsNoShift;
  }
}

inline void OdGiDgLinetypeModifiers::setShiftMode(OdGiDgLinetypeModifiers::ShiftMode mode)
{
  switch (mode)
  {
    case kLsShiftDistance: SETBIT_1(m_uFlags, kFlagUseShift); SETBIT_0(m_uFlags, kFlagUseFractionShift | kFlagUseCenteredShift); break;
    case kLsShiftFraction: SETBIT_1(m_uFlags, kFlagUseShift | kFlagUseFractionShift); SETBIT_0(m_uFlags, kFlagUseCenteredShift); break;
    case kLsShiftCentered: SETBIT_1(m_uFlags, kFlagUseShift | kFlagUseCenteredShift); SETBIT_0(m_uFlags, kFlagUseFractionShift); break;
    default: SETBIT_0(m_uFlags, kFlagsShiftMask);
  }
}

inline OdGiDgLinetypeModifiers::CornersMode OdGiDgLinetypeModifiers::getCornersMode() const
{
  switch (m_uFlags & kFlagsCornersMask)
  {
    case kFlagOverrideBreakAtCorners:    return kLsBreakAtCorners;
    case kFlagOverrideRunThroughCorners: return kLsRunThroughCorners;
    default:                             return kLsCornersFromLS;
  }
}

inline void OdGiDgLinetypeModifiers::setCornersMode(OdGiDgLinetypeModifiers::CornersMode mode)
{
  switch (mode)
  {
    case kLsBreakAtCorners:    SETBIT_1(m_uFlags, kFlagOverrideBreakAtCorners); SETBIT_0(m_uFlags, kFlagOverrideRunThroughCorners); break;
    case kLsRunThroughCorners: SETBIT_0(m_uFlags, kFlagOverrideBreakAtCorners); SETBIT_1(m_uFlags, kFlagOverrideRunThroughCorners); break;
    default:                   SETBIT_0(m_uFlags, kFlagsCornersMask);
  }
}

inline bool OdGiDgLinetypeModifiers::operator ==(const OdGiDgLinetypeModifiers& lsMod) const
{
  return (m_uFlags == lsMod.m_uFlags) &&
         (m_dDashScale == lsMod.m_dDashScale) && (m_dGapScale == lsMod.m_dGapScale) &&
         (m_dWidth == lsMod.m_dWidth) && (m_dEndWidth == lsMod.m_dEndWidth) &&
         (m_dPhase == lsMod.m_dPhase);
}

inline bool OdGiDgLinetypeModifiers::operator !=(const OdGiDgLinetypeModifiers& lsMod) const
{
  return (m_uFlags != lsMod.m_uFlags) ||
         (m_dDashScale != lsMod.m_dDashScale) || (m_dGapScale != lsMod.m_dGapScale) ||
         (m_dWidth != lsMod.m_dWidth) || (m_dEndWidth != lsMod.m_dEndWidth) ||
         (m_dPhase != lsMod.m_dPhase);
}

/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiDgLinetypeTraits : public OdGiDrawableTraits
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiDgLinetypeTraits);

    virtual void items(OdGiDgLinetypeItemArray& items) const = 0;
    virtual void setItems(const OdGiDgLinetypeItemArray& items) const = 0;

    virtual double patternLength() const = 0;
    virtual void setPatternLength(double dPatLen) = 0;

    virtual double scale() const = 0;
    virtual void setScale(double dScale) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiDgLinetypeTraits object pointers.
*/
typedef OdSmartPtr<OdGiDgLinetypeTraits> OdGiDgLinetypeTraitsPtr;

#include "TD_PackPop.h"

#endif // __ODGI_DGLINETYPE_H__
