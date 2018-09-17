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

#ifndef ODOPENGLVECTORIZER_DEFINITIONS_INCLUDED
#define ODOPENGLVECTORIZER_DEFINITIONS_INCLUDED

#include "TD_PackPush.h"

#include "Gs/Gs.h"
#include "Ps/PlotStyles.h"

/** \details
  <group ExRender_Classes> 
*/
struct OdOpenGLLinePatt
{
  enum Entry
  {
    kLnPattLinetype = 0,
    kLnPattSel,

    kLnPattAll
  };

  union Data
  {
    OdUInt16 summ;
    OdUInt8 reg[2];
    struct Registers
    {
      OdUInt8 reg_linetype;
      OdUInt8 reg_sel;
    } named;
  } data;

  OdOpenGLLinePatt() { data.summ = 0; }
};

/** \details

  <group ExRender_Classes> 
*/
struct OdOpenGLFillPatt
{
  enum Entry
  {
    kFillPattTransparency = 0,
    kFillPattMaterial,
    kFillPattFillstyle,
    kFillPattSel,

    kFillPattAll
  };

  union Data
  {
    OdUInt32 summ;
    OdUInt8 reg[4];
    struct Registers
    {
      OdUInt8 reg_transparency;
      OdUInt8 reg_material;
      OdUInt8 reg_fillstyle;
      OdUInt8 reg_sel;
    } named;
  } data;

  OdOpenGLFillPatt() { data.summ = 0; }
};

/** \details

  <group ExRender_Classes> 
*/
struct OdOpenGLLwdStyle
{
  bool m_bPointsLwd;
  OdPs::LineEndStyle  m_curLesStyle;
  OdPs::LineJoinStyle m_curLjsStyle;
  bool m_bCurStyleDef;
  bool m_bCurUsesDef;
  OdPs::LineEndStyle  m_defLesStyle;
  OdPs::LineJoinStyle m_defLjsStyle;
  bool m_bDefStyleDef;

  OdOpenGLLwdStyle() : m_bPointsLwd(true), m_curLesStyle(OdPs::kLesRound), m_curLjsStyle(OdPs::kLjsRound), m_bCurStyleDef(true), m_bCurUsesDef(true)
                                         , m_defLesStyle(OdPs::kLesRound), m_defLjsStyle(OdPs::kLjsRound), m_bDefStyleDef(true) { }
};

/** \details

  <group ExRender_Classes>
*/
struct OdOpenGLLwdSetting
{
  enum LwdType
  { kDatabase = 0, kPlotStyle, kPixels, kAbsolute };
  OdUInt8            m_type;
  void setType(LwdType typ) { m_type = (OdUInt8)typ; }
  LwdType type() const      { return (LwdType)m_type; }
  bool isForcedLineweight() const { return (m_type == kPixels) && (m_val.m_pixLineWeight > 0); }
  union {
    double           m_dLineWeight;
    OdDb::LineWeight m_lwLineWeight;
    OdInt32          m_pixLineWeight;
  } m_val;
  OdOpenGLLwdSetting() : m_type(0) { m_val.m_dLineWeight = 0.0; }
  OdOpenGLLwdSetting(LwdType typ) : m_type(typ) { }
  bool operator ==(const OdOpenGLLwdSetting &sec) const
  { if (m_type == sec.m_type)
    { switch (m_type)
      { case kDatabase:  return m_val.m_lwLineWeight == sec.m_val.m_lwLineWeight;
        case kAbsolute:
        case kPlotStyle: return OdEqual(m_val.m_dLineWeight, sec.m_val.m_dLineWeight);
        case kPixels:    return m_val.m_pixLineWeight == sec.m_val.m_pixLineWeight;
    } }
    return false;
  }
  bool operator !=(const OdOpenGLLwdSetting &sec) const
  { if (m_type == sec.m_type)
    { switch (m_type)
      { case kDatabase:  return m_val.m_lwLineWeight != sec.m_val.m_lwLineWeight;
        case kAbsolute:
        case kPlotStyle: return !OdEqual(m_val.m_dLineWeight, sec.m_val.m_dLineWeight);
        case kPixels:    return m_val.m_pixLineWeight != sec.m_val.m_pixLineWeight;
    } }
    return true;
  }
  bool hasLineweight() const
  { switch (m_type)
    { case kDatabase:  return m_val.m_lwLineWeight != OdDb::kLnWt000;
      case kAbsolute:
      case kPlotStyle: return OdNonZero(m_val.m_dLineWeight);
      case kPixels:    return m_val.m_pixLineWeight > 1;
    }
    return false;
  }
  bool isPs() const { return GETBIT(m_type, 1); }
  static bool isPs(LwdType type) { return GETBIT(type, 1); }
};

#define OD_OGL_OPAQUE(color)          ((color) | 0xFF000000)
#define OD_OGL_TRANSPARENT(color)     ((color) & 0x00FFFFFF)
#define OD_OGL_SETALPHA(color, alpha) (OD_OGL_TRANSPARENT(color) | (ODCOLORREF(alpha) << 24))

enum OdOpenGLAlphaTestType
{
  OdOpenGLAlphaTestType_Disable = 0,
  OdOpenGLAlphaTestType_Bitonal,
  OdOpenGLAlphaTestType_0,
  OdOpenGLAlphaTestType_255,
  OdOpenGLAlphaTestType_Blend
};

typedef OdUInt64 OdOpenGLGsMarker;
typedef OdArray<OdOpenGLGsMarker, OdMemoryAllocator<OdOpenGLGsMarker> > OdOpenGLGsMarkerArray;
inline bool isValidMarker(OdOpenGLGsMarker marker)
{ return marker != OdOpenGLGsMarker(kNullSubentIndex); }

#include "TD_PackPop.h"

#endif // ODOPENGLVECTORIZER_DEFINITIONS_INCLUDED
