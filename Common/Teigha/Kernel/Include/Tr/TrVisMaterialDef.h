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
// GLES2 device material definition

#ifndef ODTRVISMATERIALDEF
#define ODTRVISMATERIALDEF

#include "TD_PackPush.h"

#include "TrVisTextureDef.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisMaterialDef
{
  // Ambient color components
  float m_ambientColor[3];
  // Ambient color components for shading in non-realistic modes
  float m_shadingAmbientColor[3];
  // Diffuse color components
  float m_diffuseColor[3];
  // Diffuse color components for shading in non-realistic modes
  float m_shadingDiffuseColor[3];
  // Specular color components
  float m_specularColor[3];
  // Specular color components for shading in non-realistic modes
  float m_shadingSpecularColor[3];
  // Emission color components
  float m_emissionColor[3];
  // Emission color components for shading in non-realistic modes
  float m_shadingEmissionColor[3];
  // Factor to blend between material diffuse color and vertex color [0 - full material color, 1 - full vertex color]
  float m_blendFactor;
  // Factor to blend between material diffuse color and vertex color for shading in non-realistic modes
  float m_shadingBlendFactor;
  // Material transparency [0 - completely transparent, 1 - opaque]
  float m_opacityLevel;
  // Material transparency for shading in non-realistic modes
  float m_shadingOpacityLevel;
  // Specular color (shininnes) component
  float m_specularPower;
  // Factor to blend between texture and diffuse color [0 - full diffuse color, 1 - full texture color]
  float m_textureBlend;
  // Texture ID
  OdTrVisTextureId m_textureId;
  // Texture tiling method
  enum WrapType
  {
    kWrapRepeat = 0,
    kWrapClamp,
    kWrapCrop,
    kWrapMirror
  } m_uWrap, m_vWrap;

  // Setup default material defaults
  void setDefault()
  {
    m_ambientColor[0] = m_ambientColor[1] = m_ambientColor[2] = 0.0f;
    m_shadingAmbientColor[0] = m_shadingAmbientColor[1] = m_shadingAmbientColor[2] = 0.0f;
    m_diffuseColor[0] = m_diffuseColor[1] = m_diffuseColor[2] = 0.0f;
    m_shadingDiffuseColor[0] = m_shadingDiffuseColor[1] = m_shadingDiffuseColor[2] = 0.0f;
    m_specularColor[0] = m_specularColor[1] = m_specularColor[2] = 1.0f;
    m_shadingSpecularColor[0] = m_shadingSpecularColor[1] = m_shadingSpecularColor[2] = 0.0f;
    m_emissionColor[0] = m_emissionColor[1] = m_emissionColor[2] = 0.0f;
    m_shadingEmissionColor[0] = m_shadingEmissionColor[1] = m_shadingEmissionColor[2] = 0.0f;
    m_blendFactor = 1.0f;
    m_shadingBlendFactor = 1.0f;
    m_opacityLevel = 1.0f;
    m_shadingOpacityLevel = 1.0f;
    m_specularPower = 50.0f;
    m_textureBlend = 0.0f;
    m_textureId = kTrVisNullId;
    m_uWrap = m_vWrap = kWrapRepeat;
    m_name = OdString::kEmpty;
    m_bDefault = true;
  }
  // Material name
  OdString m_name;
  // Material "Default" flag
  bool m_bDefault;
};

#include "TD_PackPop.h"

#endif // ODTRVISMATERIALDEF
