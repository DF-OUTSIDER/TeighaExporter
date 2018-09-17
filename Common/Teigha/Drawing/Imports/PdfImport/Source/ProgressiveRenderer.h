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

#ifndef ODPROGRESSIVERENDERER_H_
#define ODPROGRESSIVERENDERER_H_

#include <memory>

#include "core/fpdfapi/page/cpdf_pageobjectlist.h"
#include "core/fpdfapi/render/cpdf_rendercontext.h"
#include "core/fpdfapi/render/cpdf_progressiverenderer.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/fx_system.h"
#include "core/fxcrt/bytestring.h"

class CPDF_RenderOptions;
class CPDF_RenderStatus;
class CFX_RenderDevice;
class IFX_PauseIndicator;

class OdProgressiveRenderer : public CPDF_ProgressiveRenderer 
{
 public:

  OdProgressiveRenderer(CPDF_RenderContext* pContext, CFX_RenderDevice* pDevice, const CPDF_RenderOptions* pOptions, 
                        ByteString& current_layer_name, bool& is_object_visible);
  ~OdProgressiveRenderer();

  void Continue(IFX_PauseIndicator* pPause);

 private:
   ByteString& m_CurrentLayerName;
   bool&       m_IsObjectVisible;
};

#endif  // ODPROGRESSIVERENDERER_H_
