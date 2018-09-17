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
// GLES2 device viewport definition

#ifndef ODTRVISVIEWPORTDEF
#define ODTRVISVIEWPORTDEF

#include "TD_PackPush.h"

#include "Ge/GeMatrix3d.h"
#include "IntArray.h"
#include "Ge/GePoint2dArray.h"
#include "UInt8Array.h"

#include "TrVisRenderMode.h"
#include "TrVisOverlayDef.h"

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportDef
{
  // Flags
  enum ViewportDefFlags
  {
    kPSOverallViewport = (1 << 0), // Overall PaperSpace viewport
    kPSModelViewport   = (1 << 1), // ModelSpace viewport in PaperSpace
    kPSHelperViewport  = (1 << 2)  // Helper PaperSpace viewport
  };

  // applyChanges parts
  enum KindOfModification
  {
    // Viewport visibility flag
    kViewportModVisibility  = (1 << 0),
    // Transform matrices, camera positioning
    kViewportModOrientation = (1 << 1),
    // Persistent viewport parameters, flags and etc.
    kViewportModPersistent  = (1 << 2),
    // Contextual viewport parameters (lineweight style, fading, etc.)
    kViewportModContextual  = (1 << 3),
    // Viewport rectangle in screen coordinates
    kViewportModRect        = (1 << 4),
    // Viewport Nrc clipping boundary in screen coordinates
    kViewportModNrcClip     = (1 << 5),
    // Array of lineweights
    kViewportModLineweights = (1 << 6),
    // Base shader program
    kViewportModShader      = (1 << 7),
    // Viewport border in screen coordinates
    kViewportModBorder      = (1 << 8),

    kViewportModAll = kViewportModVisibility | kViewportModOrientation | kViewportModPersistent | kViewportModContextual |
                      kViewportModRect | kViewportModNrcClip | kViewportModLineweights | kViewportModShader | kViewportModBorder
  };

  // Mark viewport as visible (kViewportModVisibility)
  bool m_bVisible;
  // Orientation
  OdTrVisViewParamsDef m_viewParams;
  // Persistent viewport parameters (kViewportModPersistent)
  OdUInt16 m_vptFlags;
  OdTrVisViewportId m_overallViewportId;
  // Contextual viewport parameters (kViewportModContextual)
  ODCOLORREF m_lineStyleConfig;
  ODCOLORREF m_fadingConfig;
  // Viewport positioning (rect clip), as 0-1 range (kViewportModRect)
  OdGsDCRectDouble m_screenRect;
  // Viewport clip region (as triangle strips) (kViewportModNrcClip)
  OdIntArray m_nrcCounts;
  OdFloatArray m_nrcPoints;
  // Lineweights (kViewportModLineweights)
  OdUInt8Array m_lineweightsEnum;
  double m_lineweightsCoef;
  // Render mode (kViewportModShader)
  OdTrVisProgramId m_rmShader;
  OdTrVisVisualStyleId m_visualStyle;
  OdTrVisRenderMode m_renderMode;
  ODCOLORREF m_faceFillColor;
  // Viewport border (kViewportModBorder)
  bool m_bBorder;
  ODCOLORREF m_borderColor;
  int m_borderWeight;
  OdIntArray m_borderCounts;
  OdFloatArray m_borderPoints;

  // Reset viewport flags
  void resetViewportFlags(OdUInt16 vptFlags = 0) { m_vptFlags = vptFlags; }

  // PSOverallViewport flag
  bool isPSOverallViewport() const { return GETBIT(m_vptFlags, kPSOverallViewport); }
  void setPSOverallViewport(bool bSet) { SETBIT(m_vptFlags, kPSOverallViewport, bSet); }

  // PSModelViewport flag
  bool isPSModelViewport() const { return GETBIT(m_vptFlags, kPSModelViewport); }
  void setPSModelViewport(bool bSet) { SETBIT(m_vptFlags, kPSModelViewport, bSet); }

  // PSHelperViewport flag
  bool isPSHelperViewport() const { return GETBIT(m_vptFlags, kPSHelperViewport); }
  void setPSHelperViewport(bool bSet) { SETBIT(m_vptFlags, kPSHelperViewport, bSet); }

  // Setup default values
  void setDefault()
  {
    m_bVisible          = false;
    m_viewParams        .setDefault();
    m_vptFlags          = 0;
    m_overallViewportId = 0;
    m_lineStyleConfig   = ODRGBA(1, 2, 2, 1);
    m_fadingConfig      = ODRGBA(0, 0, 0, 0);
    m_screenRect.m_min  = OdGePoint2d(0.0, 1.0);
    m_screenRect.m_max  = OdGePoint2d(1.0, 0.0);
    m_lineweightsCoef   = 1.0;
    m_rmShader          = kTrVisNegativeId;
    m_visualStyle       = kTrVisNegativeId;
    m_renderMode        .reset();
    m_faceFillColor     = 0;
    m_bBorder           = false;
    m_borderColor       = 0;
    m_borderWeight      = 1;
  }

  // Apply partial changes in OdTrVisViewportDef
  static void viewportDefApplyChanges(OdTrVisViewportDef &applyTo, const OdTrVisViewportDef &applyFrom, OdUInt32 nChanges);
  void applyChanges(const OdTrVisViewportDef &applyFrom, OdUInt32 nChanges)
  {
    viewportDefApplyChanges(*this, applyFrom, nChanges);
  }
};

inline
void OdTrVisViewportDef::viewportDefApplyChanges(OdTrVisViewportDef &applyTo, const OdTrVisViewportDef &applyFrom, OdUInt32 nChanges)
{
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModVisibility))
  {
    applyTo.m_bVisible          = applyFrom.m_bVisible;
  }
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModOrientation))
  {
    applyTo.m_viewParams        = applyFrom.m_viewParams;
  }
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModPersistent))
  {
    applyTo.m_vptFlags          = applyFrom.m_vptFlags;
    applyTo.m_overallViewportId = applyFrom.m_overallViewportId;
  }
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModContextual))
  {
    applyTo.m_lineStyleConfig   = applyFrom.m_lineStyleConfig;
    applyTo.m_fadingConfig      = applyFrom.m_fadingConfig;
  }
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModRect))
  {
    applyTo.m_screenRect        = applyFrom.m_screenRect;
  }
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModNrcClip))
  {
    applyTo.m_nrcCounts         = applyFrom.m_nrcCounts;
    applyTo.m_nrcPoints         = applyFrom.m_nrcPoints;
  }
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModLineweights))
  {
    applyTo.m_lineweightsEnum   = applyFrom.m_lineweightsEnum;
    applyTo.m_lineweightsCoef   = applyFrom.m_lineweightsCoef;
  }
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModShader))
  {
    applyTo.m_rmShader          = applyFrom.m_rmShader;
    applyTo.m_visualStyle       = applyFrom.m_visualStyle;
    applyTo.m_renderMode        = applyFrom.m_renderMode;
    applyTo.m_faceFillColor     = applyFrom.m_faceFillColor;
  }
  if (GETBIT(nChanges, OdTrVisViewportDef::kViewportModBorder))
  {
    applyTo.m_bBorder           = applyFrom.m_bBorder;
    applyTo.m_borderColor       = applyFrom.m_borderColor;
    applyTo.m_borderWeight      = applyFrom.m_borderWeight;
    applyTo.m_borderCounts      = applyFrom.m_borderCounts;
    applyTo.m_borderPoints      = applyFrom.m_borderPoints;
  }
}

#include "TD_PackPop.h"

#endif // ODTRVISVIEWPORTDEF
