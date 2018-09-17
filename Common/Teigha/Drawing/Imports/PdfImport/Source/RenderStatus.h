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

#ifndef ODRENDERSTATUS_H_
#define ODRENDERSTATUS_H_

#include <memory>
#include <vector>

#include "core/fpdfapi/page/cpdf_clippath.h"
#include "core/fpdfapi/page/cpdf_graphicstates.h"
#include "core/fpdfapi/render/cpdf_renderoptions.h"
#include "core/fxcrt/unowned_ptr.h"
#include "core/fxge/cfx_renderdevice.h"
#include "core/fpdfapi/render/cpdf_renderstatus.h"

class OdRenderStatus : public CPDF_RenderStatus 
{
 public:
   OdRenderStatus(ByteString& current_layer_name, bool& is_object_visible);
  ~OdRenderStatus();

  void RenderSingleObject(CPDF_PageObject* pObj, const CFX_Matrix* pObj2Device);
  bool ContinueSingleObject(CPDF_PageObject* pObj, const CFX_Matrix* pObj2Device, IFX_PauseIndicator* pPause);
  void DrawTilingPattern(CPDF_TilingPattern* pPattern, CPDF_PageObject* pPageObj, const CFX_Matrix* pObj2Device, bool bStroke);
  virtual void DrawShading(CPDF_ShadingPattern* pPattern, CFX_Matrix* pMatrix, FX_RECT& clip_rect, int alpha, bool bAlphaMode);

 protected:
  bool ProcessTransparency(CPDF_PageObject* PageObj, const CFX_Matrix* pObj2Device);
  void ProcessObjectNoClip(CPDF_PageObject* PageObj, const CFX_Matrix* pObj2Device);
  void DrawObjWithBackground(CPDF_PageObject* pObj, const CFX_Matrix* pObj2Device);
  bool DrawObjWithBlend(CPDF_PageObject* pObj, const CFX_Matrix* pObj2Device);
  bool ProcessImage(CPDF_ImageObject* pImageObj, const CFX_Matrix* pObj2Device);
  bool ProcessType3Text(CPDF_TextObject* textobj, const CFX_Matrix* pObj2Device);
  bool ProcessText(CPDF_TextObject* textobj, const CFX_Matrix* pObj2Device, CFX_PathData* pClippingPath);
  bool ProcessForm(const CPDF_FormObject* pFormObj, const CFX_Matrix* pObj2Device);
  RetainPtr<CFX_DIBitmap> LoadSMask(CPDF_Dictionary* pSMaskDict, FX_RECT* pClipRect, const CFX_Matrix* pMatrix);

  ByteString& m_CurrentLayerName;
  bool&       m_IsObjectVisible;
};

#endif  // ODRENDERSTATUS_H_
