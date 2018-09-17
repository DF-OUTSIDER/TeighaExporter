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

#ifndef __ODGISUBENTITYTRAITSDATA_H__
#define __ODGISUBENTITYTRAITSDATA_H__

#include "TD_PackPush.h"

#include "Gi/GiMaterial.h"

struct OdGiDgLinetypeModifiers;

/** \details
    Wraps traits changes flags into importable namespace.
    <group OdGi_Classes> 
*/
struct OdGiSubEntityTraitsChangedFlags
{
  enum ChangedTraits
  {
    kFirstChangedFlag      = 1,
    kColorChanged          = kFirstChangedFlag <<  0,
    kLayerChanged          = kFirstChangedFlag <<  1,
    kLineTypeChanged       = kFirstChangedFlag <<  2,
    kFillTypeChanged       = kFirstChangedFlag <<  3,
    kLineWeightChanged     = kFirstChangedFlag <<  4,
    kLineTypeScaleChanged  = kFirstChangedFlag <<  5,
    kThicknessChanged      = kFirstChangedFlag <<  6,
    kPlotStyleChanged      = kFirstChangedFlag <<  7,
    kMaterialChanged       = kFirstChangedFlag <<  8,
    kMapperChanged         = kFirstChangedFlag <<  9,
    kVisualStyleChanged    = kFirstChangedFlag << 10,
    kTransparencyChanged   = kFirstChangedFlag << 11,
    kDrawFlagsChanged      = kFirstChangedFlag << 12,
    kSelectionGeomChanged  = kFirstChangedFlag << 13,
    kShadowFlagsChanged    = kFirstChangedFlag << 14,
    kSectionableChanged    = kFirstChangedFlag << 15,
    kSelectionFlagsChanged = kFirstChangedFlag << 16,
    kSecColorChanged       = kFirstChangedFlag << 17,
    kLSModifiersChanged    = kFirstChangedFlag << 18,
    kFillChanged           = kFirstChangedFlag << 19,
    kAwareFlagChanged      = kFirstChangedFlag << 20,
    kAllChanged            = 0x1FFFFF, // bin: 111111111111111111111
    kSomeChanged           = kAllChanged
  };
};

/** \details

    <group OdGi_Classes> 
*/
class OdGiSubEntityTraitsData
{
  // Members
  //
  OdUInt16                         m_flags;
  OdUInt16                         m_selFlags;
  OdCmEntityColor                  m_cmColor;
  OdDbStub*                        m_layerId;
  OdDbStub*                        m_lineTypeId;
  OdGiFillType                     m_fillType;
  OdDb::LineWeight                 m_lineWeight;
  double                           m_lineTypeScale;
  double                           m_thickness;
  OdDb::PlotStyleNameType          m_plotStyleNameType;
  OdDbStub*                        m_plotStyleNameId;
  OdDbStub*                        m_materialId;
  const OdGiMapper*                m_mapper;
  OdDbStub*                        m_visualStyleId;
  OdCmTransparency                 m_cmTransparency;
  OdUInt32                         m_drawFlags;
  OdGiSubEntityTraits::ShadowFlags m_shadowFlags;
  OdCmEntityColor                  m_cmSecondaryColor;
  const OdGiDgLinetypeModifiers*   m_lsModifiers;
  const OdGiFill*                  m_fill;

  enum
  {
    kSelectionGeom    = 0x100,
    kSectionable      = 0x200
  };
public:
  enum
  {
    kLayerOff         = 1,
    kLayerFrozen      = 2
  };
  // Geometry visibility flags returned by geomVisibilityFlags method.
  // This is more easy-to-use representation than analysis of selectionGeom and selectionFlags return values.
  enum
  {
    kGeomDisplayNormally    = (1 << 0), // Display unhighlighted geometry
    kGeomSelectNormally     = (1 << 1), // Select unhighlighted geometry 
    kGeomDisplayHighlighted = (1 << 2), // Display highlighted geometry
    kGeomSelectHighlighted  = (1 << 3), // Select highlighted geometry
    kGeomAllVisibilityFlags = 0x0F
  };

  // Constructors/destructors
  //
  OdGiSubEntityTraitsData()
    : m_flags(0)
    , m_selFlags(0)
    , m_cmColor(OdCmEntityColor::kForeground)
    , m_layerId(NULL)
    , m_lineTypeId(NULL)
    , m_fillType(kOdGiFillNever)
    , m_lineWeight(OdDb::kLnWt000)
    , m_lineTypeScale(1.0)
    , m_thickness(0.0)
    , m_plotStyleNameType(OdDb::kPlotStyleNameIsDictDefault)
    , m_plotStyleNameId(NULL)
    , m_materialId(NULL)
    , m_mapper(NULL)
    , m_visualStyleId(NULL)
    , m_cmTransparency(OdUInt8(0xFF))
    , m_drawFlags(0)
    , m_shadowFlags(OdGiSubEntityTraits::kShadowsCastAndReceive)
    , m_cmSecondaryColor(OdCmEntityColor::kForeground)
    , m_lsModifiers(NULL)
    , m_fill(NULL)
  {
  }

  // Accessors / transformers
  //
  OdUInt16 flags() const;
  OdUInt16 selFlags() const;
  OdUInt16 geomVisibilityFlags() const;
  OdUInt16 geomVisibilityFlags(bool bHighlighted) const;
  bool isLayerFrozen() const;
  bool isLayerOff() const;
  bool isLayerVisible() const;
  bool visibility() const { return isLayerVisible(); }
  const OdCmEntityColor& trueColor() const;
  OdUInt16 color() const;
  OdDbStub* layer() const;
  OdDbStub* lineType() const;
  OdGiFillType fillType() const;
  OdDb::LineWeight lineWeight() const;
  double lineTypeScale() const;
  double thickness() const;
  OdDb::PlotStyleNameType plotStyleNameType() const;
  OdDbStub* plotStyleNameId() const;
  OdDbStub* material() const;
  const OdGiMapper* mapper() const;
  OdDbStub* visualStyle() const;
  const OdCmTransparency& transparency() const;
  OdUInt32 drawFlags() const;
  bool selectionGeom() const;
  OdGiSubEntityTraits::ShadowFlags shadowFlags() const;
  bool sectionable() const;
  OdGiSubEntityTraits::SelectionFlags selectionFlags() const;
  const OdCmEntityColor& secondaryTrueColor() const;
  const OdGiDgLinetypeModifiers* lineStyleModifiers() const;
  const OdGiFill* fill() const;

  void setFlags(OdUInt16 flags);
  void setSelFlags(OdUInt16 selFlags);
  void setTrueColor(const OdCmEntityColor& trueColor);
  void setColor(OdUInt16 colorIndex);
  void setLayer(OdDbStub* layerId);
  void setLineType(OdDbStub* lineTypeId);
  void setFillType(OdGiFillType fillType);
  void setLineWeight(OdDb::LineWeight lineWeight);
  void setLineTypeScale(double lineTypeScale);
  void setThickness(double thickness);
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId);
  void setMaterial(OdDbStub* materialId);
  void setMapper(const OdGiMapper* pMapper);
  void setVisualStyle(OdDbStub* visualStyleId);
  void setTransparency(const OdCmTransparency& transparency);
  void setDrawFlags(OdUInt32 drawFlags);
  void setSelectionGeom(bool bSelectionFlag);
  void setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags);
  void setSectionable(bool bSectionableFlag);
  void setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags);
  void setSecondaryTrueColor(const OdCmEntityColor& trueColor);
  void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod);
  void setFill(const OdGiFill* pFill);
};


inline OdUInt16 OdGiSubEntityTraitsData::flags() const
{
  return m_flags;
}
inline OdUInt16 OdGiSubEntityTraitsData::selFlags() const
{
  return m_selFlags;
}
inline OdUInt16 OdGiSubEntityTraitsData::geomVisibilityFlags() const
{
  OdUInt16 visFlags = kGeomAllVisibilityFlags;
  if (GETBIT(m_selFlags, kSelectionGeom | OdGiSubEntityTraits::kSelectionIgnore | OdGiSubEntityTraits::kHighlightingGeometry))
  {
    if (GETBIT(m_selFlags, kSelectionGeom)) visFlags &= ~(kGeomDisplayNormally | kGeomDisplayHighlighted);
    if (GETBIT(m_selFlags, OdGiSubEntityTraits::kSelectionIgnore)) visFlags &= ~(kGeomSelectNormally | kGeomSelectHighlighted);
    if (GETBIT(m_selFlags, OdGiSubEntityTraits::kHighlightingGeometry)) visFlags &= ~kGeomDisplayNormally;
  }
  return visFlags;
}
inline OdUInt16 OdGiSubEntityTraitsData::geomVisibilityFlags(bool bHighlighted) const
{ // Returns only first two flags depending onto highlighting state
  const OdUInt16 visFlags = geomVisibilityFlags();
  return (!bHighlighted) ? visFlags : (visFlags >> 2);
}
inline bool OdGiSubEntityTraitsData::isLayerVisible() const
{
  return (m_flags & (kLayerOff|kLayerFrozen))==0;
}
inline bool OdGiSubEntityTraitsData::isLayerFrozen() const
{
  return GETBIT(m_flags, kLayerFrozen);
}
inline bool OdGiSubEntityTraitsData::isLayerOff() const
{
  return GETBIT(m_flags, kLayerOff);
}
inline OdUInt16 OdGiSubEntityTraitsData::color() const
{
  return m_cmColor.colorIndex();
}
inline const OdCmEntityColor& OdGiSubEntityTraitsData::trueColor() const
{
  return m_cmColor;
}
inline OdDbStub* OdGiSubEntityTraitsData::layer() const
{
  return m_layerId;
}
inline OdDbStub* OdGiSubEntityTraitsData::lineType() const
{
  return m_lineTypeId;
}
inline OdGiFillType OdGiSubEntityTraitsData::fillType() const
{
  return m_fillType;
}
inline OdDb::LineWeight OdGiSubEntityTraitsData::lineWeight() const
{
  return m_lineWeight;
}
inline double OdGiSubEntityTraitsData::lineTypeScale() const
{
  return m_lineTypeScale;
}
inline double OdGiSubEntityTraitsData::thickness() const
{
  return m_thickness;
}
inline OdDb::PlotStyleNameType OdGiSubEntityTraitsData::plotStyleNameType() const
{
  return m_plotStyleNameType;
}
inline OdDbStub* OdGiSubEntityTraitsData::plotStyleNameId() const
{
  return m_plotStyleNameId;
}
inline OdDbStub* OdGiSubEntityTraitsData::material() const
{
  return m_materialId;
}
inline const OdGiMapper* OdGiSubEntityTraitsData::mapper() const
{
  return m_mapper;
}
inline OdDbStub* OdGiSubEntityTraitsData::visualStyle() const
{
  return m_visualStyleId;
}
inline const OdCmTransparency& OdGiSubEntityTraitsData::transparency() const
{
  return m_cmTransparency;
}
inline OdUInt32 OdGiSubEntityTraitsData::drawFlags() const
{
  return m_drawFlags;
}
inline bool OdGiSubEntityTraitsData::selectionGeom() const
{
  return GETBIT(m_selFlags, kSelectionGeom);
}
inline OdGiSubEntityTraits::ShadowFlags OdGiSubEntityTraitsData::shadowFlags() const
{
  return m_shadowFlags;
}
inline bool OdGiSubEntityTraitsData::sectionable() const
{
  return GETBIT(m_selFlags, kSectionable);
}
inline OdGiSubEntityTraits::SelectionFlags OdGiSubEntityTraitsData::selectionFlags() const
{
  return (OdGiSubEntityTraits::SelectionFlags)(m_selFlags & 0xFF);
}
inline const OdCmEntityColor& OdGiSubEntityTraitsData::secondaryTrueColor() const
{
  return m_cmSecondaryColor;
}
inline const OdGiDgLinetypeModifiers* OdGiSubEntityTraitsData::lineStyleModifiers() const
{
  return m_lsModifiers;
}
inline const OdGiFill* OdGiSubEntityTraitsData::fill() const
{
  return m_fill;
}


inline void OdGiSubEntityTraitsData::setFlags(OdUInt16 flags)
{
  m_flags = flags;
}
inline void OdGiSubEntityTraitsData::setSelFlags(OdUInt16 selFlags)
{
  m_selFlags = selFlags;
}
inline void OdGiSubEntityTraitsData::setTrueColor(const OdCmEntityColor& trueColor)
{
  m_cmColor = trueColor;
}
inline void OdGiSubEntityTraitsData::setColor(OdUInt16 colorIndex)
{
  m_cmColor.setColorIndex(colorIndex);
}
inline void OdGiSubEntityTraitsData::setLayer(OdDbStub* layerId)
{
  m_layerId = layerId;
}
inline void OdGiSubEntityTraitsData::setLineType(OdDbStub* lineTypeId)
{
  m_lineTypeId = lineTypeId;
}
inline void OdGiSubEntityTraitsData::setFillType(OdGiFillType fillType)
{
  m_fillType = fillType;
}
inline void OdGiSubEntityTraitsData::setLineWeight(OdDb::LineWeight lineWeight)
{
  m_lineWeight = lineWeight;
}
inline void OdGiSubEntityTraitsData::setLineTypeScale(double lineTypeScale)
{
  m_lineTypeScale = lineTypeScale;
}
inline void OdGiSubEntityTraitsData::setThickness(double thickness)
{
  m_thickness = thickness;
}
inline void OdGiSubEntityTraitsData::setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* plotStyleNameId)
{
  m_plotStyleNameType = plotStyleNameType;
  m_plotStyleNameId = plotStyleNameId;
}
inline void OdGiSubEntityTraitsData::setMaterial(OdDbStub* materialId)
{
  m_materialId = materialId;
}
inline void OdGiSubEntityTraitsData::setMapper(const OdGiMapper* pMapper)
{
  m_mapper = pMapper;
}
inline void OdGiSubEntityTraitsData::setVisualStyle(OdDbStub* visualStyleId)
{
  m_visualStyleId = visualStyleId;
}
inline void OdGiSubEntityTraitsData::setTransparency(const OdCmTransparency &transparency)
{
  m_cmTransparency = transparency;
}
inline void OdGiSubEntityTraitsData::setDrawFlags(OdUInt32 drawFlags)
{
  m_drawFlags = drawFlags;
}
inline void OdGiSubEntityTraitsData::setSelectionGeom(bool bSelectionFlag)
{
  SETBIT(m_selFlags, kSelectionGeom, bSelectionFlag);
}
inline void OdGiSubEntityTraitsData::setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags)
{
  m_shadowFlags = shadowFlags;
}
inline void OdGiSubEntityTraitsData::setSectionable(bool bSectionableFlag)
{
  SETBIT(m_selFlags, kSectionable, bSectionableFlag);
}
inline void OdGiSubEntityTraitsData::setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags)
{
  m_selFlags = (m_selFlags & (~0xFF)) | (OdUInt16)selectionFlags;
}
inline void OdGiSubEntityTraitsData::setSecondaryTrueColor(const OdCmEntityColor& trueColor)
{
  m_cmSecondaryColor = trueColor;
}
inline void OdGiSubEntityTraitsData::setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod)
{
  m_lsModifiers = pLSMod;
}
inline void OdGiSubEntityTraitsData::setFill(const OdGiFill* pFill)
{
  m_fill = pFill;
}

#include "TD_PackPop.h"

#endif // __ODGISUBENTITYTRAITSDATA_H__
