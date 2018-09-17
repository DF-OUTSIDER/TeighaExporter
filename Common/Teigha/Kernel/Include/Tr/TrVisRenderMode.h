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
// GLES2 device render mode definition

#ifndef ODTRVISRENDERMODE
#define ODTRVISRENDERMODE

#include "TD_PackPush.h"

#include "TrVisDefs.h"

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisRenderMode
{
  protected:
    enum Flags
    {
      kDepthBuffer = 0,
      kStencilBuffer,
      kPolygonOffset,
      kLighting,
      kFaceNormals,
        kNumFlags
    };
  protected:
    bool m_bFlags[kNumFlags];
  public:
  // Constructors, setters/getters
  OdTrVisRenderMode()
  { reset(); }
  void reset() { for (long i = 0; i < kNumFlags; i++) m_bFlags[i] = false; } /* reset to 2d mode by default */
  bool depthBufferEnabled() const   { return m_bFlags[kDepthBuffer]; }   void setDepthBufferEnabled(bool bSet)   { m_bFlags[kDepthBuffer] = bSet; }
  bool stencilBufferEnabled() const { return m_bFlags[kStencilBuffer]; } void setStencilBufferEnabled(bool bSet) { m_bFlags[kStencilBuffer] = bSet; }
  bool polygonOffsetEnabled() const { return m_bFlags[kPolygonOffset]; } void setPolygonOffsetEnabled(bool bSet) { m_bFlags[kPolygonOffset] = bSet; }
  bool lightingEnabled() const      { return m_bFlags[kLighting]; }      void setLightingEnabled(bool bSet)      { m_bFlags[kLighting] = bSet; }
  bool faceNormalsEnabled() const   { return m_bFlags[kFaceNormals]; }   void setFaceNormalsEnabled(bool bSet)   { m_bFlags[kFaceNormals] = bSet; }
  // Serialization
  OdUInt32 serializeOut() const { OdUInt32 nRez = 0; for (long i = 0; i < kNumFlags; i++) { if (m_bFlags[i]) nRez |= 1 << i; } return nRez; }
  void serializeIn(OdUInt32 nFlags) { for (long i = 0; i < kNumFlags; i++) { m_bFlags[i] = (nFlags | (1 << i)) != 0; } }
  // Control version
  const long controlVersion() const { return kNumFlags; }
  // Gs Render Mode compatibility
  // 2dOptimized mode
  bool is_2d() const
  { return !m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && !m_bFlags[kPolygonOffset] && !m_bFlags[kLighting]; }
  // Wireframe mode
  bool is_3d() const
  { return m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && !m_bFlags[kPolygonOffset] && !m_bFlags[kLighting]; }
  // HiddenLine mode
  bool is_hl() const
  { return m_bFlags[kDepthBuffer] && m_bFlags[kStencilBuffer] && m_bFlags[kPolygonOffset] && !m_bFlags[kLighting]; }
  // Shaded (any) modes
  bool is_sa() const
  { return m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && m_bFlags[kLighting]; }
  // Shaded without edges modes
  bool is_sh() const
  { return m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && !m_bFlags[kPolygonOffset] && m_bFlags[kLighting]; }
  // ShadedWithWireframe modes
  bool is_sw() const
  { return m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && m_bFlags[kPolygonOffset] && m_bFlags[kLighting]; }
  // FlatShaded mode
  bool is_fs() const
  { return m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && !m_bFlags[kPolygonOffset] && m_bFlags[kLighting] && m_bFlags[kFaceNormals]; }
  // GouraudShaded mode
  bool is_gs() const
  { return m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && !m_bFlags[kPolygonOffset] && m_bFlags[kLighting] && !m_bFlags[kFaceNormals]; }
  // FlatShadedWithWireframe mode
  bool is_fw() const
  { return m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && m_bFlags[kPolygonOffset] && m_bFlags[kLighting] && m_bFlags[kFaceNormals]; }
  // GouraudShadedWithWireframe mode
  bool is_gw() const
  { return m_bFlags[kDepthBuffer] && !m_bFlags[kStencilBuffer] && m_bFlags[kPolygonOffset] && m_bFlags[kLighting] && !m_bFlags[kFaceNormals]; }
  // RenderMode from flags
  OdGsView::RenderMode make_rm() const
  {
    if (!m_bFlags[kDepthBuffer]) return OdGsView::k2DOptimized;
    else if (m_bFlags[kStencilBuffer]) return OdGsView::kHiddenLine;
    else if (m_bFlags[kLighting])
    {
      if (m_bFlags[kPolygonOffset])
        return (m_bFlags[kFaceNormals]) ? OdGsView::kFlatShadedWithWireframe : OdGsView::kGouraudShadedWithWireframe;
      else
        return (m_bFlags[kFaceNormals]) ? OdGsView::kFlatShaded : OdGsView::kGouraudShaded;
    }
    else
      return OdGsView::kWireframe;
  }
  // Flags from render mode
  void make_fs(OdGsView::RenderMode rm)
  {
    switch (rm)
    {
      case OdGsView::k2DOptimized:
        m_bFlags[kDepthBuffer] = false; m_bFlags[kStencilBuffer] = false; m_bFlags[kPolygonOffset] = false; m_bFlags[kLighting] = false; m_bFlags[kFaceNormals] = false;
      break;
      case OdGsView::kWireframe:
        m_bFlags[kDepthBuffer] = true;  m_bFlags[kStencilBuffer] = false; m_bFlags[kPolygonOffset] = false; m_bFlags[kLighting] = false; m_bFlags[kFaceNormals] = false;
      break;
      case OdGsView::kHiddenLine:
        m_bFlags[kDepthBuffer] = true;  m_bFlags[kStencilBuffer] = true;  m_bFlags[kPolygonOffset] = true;  m_bFlags[kLighting] = false; m_bFlags[kFaceNormals] = false;
      break;
      case OdGsView::kFlatShaded:
        m_bFlags[kDepthBuffer] = true;  m_bFlags[kStencilBuffer] = false; m_bFlags[kPolygonOffset] = false; m_bFlags[kLighting] = true;  m_bFlags[kFaceNormals] = true;
      break;
      case OdGsView::kGouraudShaded:
        m_bFlags[kDepthBuffer] = true;  m_bFlags[kStencilBuffer] = false; m_bFlags[kPolygonOffset] = false; m_bFlags[kLighting] = true;  m_bFlags[kFaceNormals] = false;
      break;
      case OdGsView::kFlatShadedWithWireframe:
        m_bFlags[kDepthBuffer] = true;  m_bFlags[kStencilBuffer] = false; m_bFlags[kPolygonOffset] = true;  m_bFlags[kLighting] = true;  m_bFlags[kFaceNormals] = true;
      break;
      case OdGsView::kGouraudShadedWithWireframe:
        m_bFlags[kDepthBuffer] = true;  m_bFlags[kStencilBuffer] = false; m_bFlags[kPolygonOffset] = true;  m_bFlags[kLighting] = true;  m_bFlags[kFaceNormals] = false;
      break;
      default: break;
    }
  }
  // Calibrate settings
  void calibrate() { make_fs(make_rm()); }
};

#include "TD_PackPop.h"

#endif // ODTRVISRENDERMODE
