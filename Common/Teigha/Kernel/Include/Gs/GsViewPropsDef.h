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
#ifndef __OD_GS_VIEW_PROPS_DEF__
#define __OD_GS_VIEW_PROPS_DEF__

enum
{
  kVpID                   = 0x00000001,
  kVpRegenType            = 0x00000002,
  kVpRenderMode           = 0x00000004,
  kVpWorldToEye           = 0x00000008,
  kVpPerspective          = 0x00000010,
  kVpResolution           = 0x00000020,
  kVpMaxDevForCircle      = 0x00000040,
  kVpMaxDevForCurve       = 0x00000080,
  kVpMaxDevForBoundary    = 0x00000100,
  kVpMaxDevForIsoline     = 0x00000200,
  kVpMaxDevForFacet       = 0x00000400,
  kVpCamLocation          = 0x00000800,
  kVpCamTarget            = 0x00001000,
  kVpCamUpVector          = 0x00002000,
  kVpCamViewDir           = 0x00004000,
  kVpViewport             = 0x00008000,
  kVpFrontBack            = 0x00010000,
  kVpFrozenLayers         = 0x00020000,
  kVpLtypeScaleMult       = 0x00040000,
  kEntityRegenDraw        = 0x00080000,
  kVpDisableLayoutCache   = 0x00100000,
  kVpModelTf              = 0x00200000,
  kVpExcludeFromExtMinMax = 0x00400000,
  kSectionable            = 0x00800000,
  kVpRenderModule         = 0x01000000,
  kVpAnnoScale            = 0x02000000,
  kVpFilterFunction       = 0x04000000,
  kVpVisualStyle          = 0x08000000,
  kVpHighlightingGeom     = 0x10000000,
  kVpLastPropBit          = kVpHighlightingGeom,
  kVpAllProps             = 0x1FFFFFFF,
  kVpForceEraseMf         = 0x80000000
};

inline bool isModelTfDependent(OdUInt32 flags)
{
    return GETBIT(flags, kVpModelTf);
}

#endif // __OD_GS_VIEW_PROPS_DEF__
