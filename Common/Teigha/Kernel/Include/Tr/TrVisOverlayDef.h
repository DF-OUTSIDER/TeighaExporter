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
// GLES2 device overlay buffer definition

#ifndef ODTRVISOVERLAYDEF
#define ODTRVISOVERLAYDEF

#include "TD_PackPush.h"

#include "Ge/GeMatrix3d.h"

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisOverlayDef
{
  // Flags
  enum OverlayDefFlags
  {
    kOBEnabled      = (1 << 0), // Overlay buffer visible
    kOBDepthBuffer  = (1 << 1), // Depth buffer enabled for this overlay
    kOBMergeDepth   = (1 << 2), // Merge depth buffer with primary depth buffer
    kOBDirectRender = (1 << 3), // Render directly on screen
    kOBHighlight    = (1 << 4), // Render geometry as highlighted
    kOBContrast     = (1 << 5)  // Render geometry with contrast style applied
  };

  // Set of overlay buffer flags
  OdUInt16 m_overlayFlags;
  // Overlay rendering order position
  OdInt16  m_renderOrder;

  // Reset overlay flags
  void resetOverlayFlags(OdUInt16 overlayFlags = 0) { m_overlayFlags = overlayFlags; }

  // Overlay "enabled" flag
  bool isOverlayEnabled() const { return GETBIT(m_overlayFlags, kOBEnabled); }
  void setOverlayEnabled(bool bSet) { SETBIT(m_overlayFlags, kOBEnabled, bSet); }

  // Overlay "depth buffer" flag
  bool isOverlayDepthBufferEnabled() const { return GETBIT(m_overlayFlags, kOBDepthBuffer); }
  void setOverlayDepthBufferEnabled(bool bSet) { SETBIT(m_overlayFlags, kOBDepthBuffer, bSet); }

  // Overlay "merge depth" flag
  bool isOverlayDepthBufferMergeEnabled() const { return GETBIT(m_overlayFlags, kOBMergeDepth); }
  void setOverlayDepthBufferMergeEnabled(bool bSet) { SETBIT(m_overlayFlags, kOBMergeDepth, bSet); }

  // Overlay "direct render" flag
  bool isOverlayDirectRenderEnabled() const { return GETBIT(m_overlayFlags, kOBDirectRender); }
  void setOverlayDirectRenderEnabled(bool bSet) { SETBIT(m_overlayFlags, kOBDirectRender, bSet); }

  // Overlay "highlight" flag
  bool isOverlayHighlightingEnabled() const { return GETBIT(m_overlayFlags, kOBHighlight); }
  void setOverlayHighlightingEnabled(bool bSet) { SETBIT(m_overlayFlags, kOBHighlight, bSet); }

  // Overlay "contrast" flag
  bool isOverlayContrastStyleEnabled() const { return GETBIT(m_overlayFlags, kOBContrast); }
  void setOverlayContrastStyleEnabled(bool bSet) { SETBIT(m_overlayFlags, kOBContrast, bSet); }

  // Setup default values
  void setDefault()
  {
    m_overlayFlags = 0;
    m_renderOrder  = 0;
  }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewClipped
{
  bool m_bClipped;
  double m_clipDist;
  OdTrVisViewClipped() : m_bClipped(false), m_clipDist(0.) { }
  OdTrVisViewClipped(bool bClipped, double clipDist) : m_bClipped(false), m_clipDist(0.) { set(bClipped, clipDist); }
  void set(bool bClipped, double clipDist) { m_bClipped = bClipped; m_clipDist = (bClipped) ? clipDist : 0.; }
  void reset() { m_bClipped = false; m_clipDist = 0.; }
  bool enabled() const { return m_bClipped; }
  double clipDistance() const { return m_clipDist; }
  enum ApplyType { kGreater = 0, kLess = 1 };
  void apply(double &dVal, ApplyType appType) const
  {
    if (m_bClipped && (((appType == kGreater) && (m_clipDist > dVal)) || ((appType == kLess) && (m_clipDist < dVal))))
      dVal = m_clipDist;
  }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewParamsDef
{
  // Orientation properties related to Gs only
  OdGeMatrix3d m_screenMatrix;
  OdGeMatrix3d m_viewingMatrix;
  OdGeMatrix3d m_projectionMatrix;
  OdGeMatrix3d m_correctionMatrix;
  OdGeMatrix3d m_outputMatrix;
  OdGeMatrix3d m_metafileMatrix;
  // Orientation properties related to Gs and Xml together
  OdGePoint3d  m_viewPosition;
  OdGeVector3d m_viewBasis[3];
  bool m_bPerspectiveProjection;
  // Orientation properties related to Xml only
  double m_fieldWidth;
  double m_fieldHeight;
  double m_lensLength;
  OdGePoint3d m_viewTarget;
  OdTrVisViewClipped m_frontClip;
  OdTrVisViewClipped m_backClip;

  // Setup default values
  void setDefault()
  {
    m_screenMatrix           .setToIdentity();
    m_viewingMatrix          .setToIdentity();
    m_projectionMatrix       .setToIdentity();
    m_correctionMatrix       .setToIdentity();
    m_outputMatrix           .setToIdentity();
    m_metafileMatrix         .setToIdentity();
    m_viewBasis[0]           = OdGeVector3d::kXAxis;
    m_viewBasis[1]           = OdGeVector3d::kYAxis;
    m_viewBasis[2]           = OdGeVector3d::kZAxis;
    m_bPerspectiveProjection = false;
    m_fieldWidth             = 1.0;
    m_fieldHeight            = 1.0;
    m_lensLength             = 51.0;
    m_viewTarget             = OdGePoint3d::kOrigin + OdGeVector3d::kZAxis;
    m_frontClip              .reset();
    m_backClip               .reset();
  }

  // Check does view parameters cumulative
  bool isCumulative(const OdTrVisViewParamsDef &secParams) const
  { // View parameters cumulative in case if view orientation similar. View projection can differ.
    return m_viewPosition.isEqualTo(secParams.m_viewPosition) &&
           m_viewBasis[1].isEqualTo(secParams.m_viewBasis[1]) &&
           //(m_bPerspectiveProjection == secParams.m_bPerspectiveProjection) &&
           //OdEqual(m_fieldWidth, secParams.m_fieldWidth) &&
           //OdEqual(m_fieldHeight, secParams.m_fieldHeight) &&
           //OdEqual(m_lensLength, secParams.m_lensLength) &&
           m_viewTarget.isEqualTo(secParams.m_viewTarget);
  }
};

#include "TD_PackPop.h"

#endif // ODTRVISOVERLAYDEF
