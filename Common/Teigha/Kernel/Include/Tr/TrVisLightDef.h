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
// GLES2 device light definition

#ifndef ODTRVISLIGHTDEF
#define ODTRVISLIGHTDEF

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "Ge/GeVector3d.h"
#include "Ge/GePoint3d.h"
#include "Gs/GsFiler.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVisLightDef
{
  // Type of light
  enum LightType
  {
    kDistant = 0,
    kPoint,
    kSpot
  } m_type;
  // Default light marker
  bool m_bDefLight;
  // Light position
  OdGePoint3d m_position;
  // Light direction vector
  OdGeVector3d m_direction;
  // Light color
  float m_color[3];
  // Attenuations
  float m_constantAttenuation;
  float m_linearAttenuation;
  float m_quadraticAttenuation;
  // Spot light parameters
  float m_spotCutoff;
  float m_spotExponent;
  // Factor for specular highlighting
  float m_specularFactor;

  bool eq(const OdTrVisLightDef &dl2) const;

  void save(OdGsFiler *pFiler) const;
  void load(OdGsFiler *pFiler);
};

inline bool OdTrVisLightDef::eq(const OdTrVisLightDef &dl2) const
{
  return (m_type == dl2.m_type) && (m_bDefLight == dl2.m_bDefLight) &&
         m_position.isEqualTo(dl2.m_position) && m_direction.isEqualTo(dl2.m_direction) &&
         OdTrVisFPEqual(m_color[0], dl2.m_color[0]) && OdTrVisFPEqual(m_color[1], dl2.m_color[1]) &&
         OdTrVisFPEqual(m_color[2], dl2.m_color[2]) && OdTrVisFPEqual(m_constantAttenuation, dl2.m_constantAttenuation) &&
         OdTrVisFPEqual(m_linearAttenuation, dl2.m_linearAttenuation) && OdTrVisFPEqual(m_quadraticAttenuation, dl2.m_quadraticAttenuation) &&
         OdTrVisFPEqual(m_spotCutoff, dl2.m_spotCutoff) && OdTrVisFPEqual(m_spotExponent, dl2.m_spotExponent) &&
         OdTrVisFPEqual(m_specularFactor, dl2.m_specularFactor);
}


inline void OdTrVisLightDef::save(OdGsFiler *pFiler) const {
  pFiler->wrUInt8   ((OdUInt8)m_type        );
  pFiler->wrBool    (m_bDefLight            );
  pFiler->wrPoint3d (m_position             );
  pFiler->wrVector3d(m_direction            );
  pFiler->wrFloat   (m_color[0]             );
  pFiler->wrFloat   (m_color[1]             );
  pFiler->wrFloat   (m_color[2]             );
  pFiler->wrFloat   (m_constantAttenuation  );
  pFiler->wrFloat   (m_linearAttenuation    );
  pFiler->wrFloat   (m_quadraticAttenuation );
  pFiler->wrFloat   (m_spotCutoff           );
  pFiler->wrFloat   (m_spotExponent         );
  pFiler->wrFloat   (m_specularFactor       );
}

inline void OdTrVisLightDef::load(OdGsFiler *pFiler) {
  m_type = (LightType)pFiler->rdUInt8();
  m_bDefLight = pFiler->rdBool();
  pFiler->rdPoint3d(m_position);
  pFiler->rdVector3d(m_direction);
  m_color[0]             = pFiler->rdFloat();
  m_color[1]             = pFiler->rdFloat();
  m_color[2]             = pFiler->rdFloat();
  m_constantAttenuation  = pFiler->rdFloat();
  m_linearAttenuation    = pFiler->rdFloat();
  m_quadraticAttenuation = pFiler->rdFloat();
  m_spotCutoff           = pFiler->rdFloat();
  m_spotExponent         = pFiler->rdFloat();
  m_specularFactor       = pFiler->rdFloat();
}


#include "TD_PackPop.h"

#endif // ODTRVISLIGHTDEF
