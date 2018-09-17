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

#ifndef ODIMAGERENDERER_H_
#define ODIMAGERENDERER_H_

#include <memory>

#include "core/fpdfapi/render/cpdf_imagerenderer.h"
#include "core/fpdfapi/render/cpdf_imageloader.h"
#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/unowned_ptr.h"
#include "core/fxge/dib/cfx_imagerenderer.h"
#include "core/fxcrt/bytestring.h"

class CPDF_ImageObject;
class CPDF_RenderStatus;

class OdImageRenderer : public CPDF_ImageRenderer 
{
 public:
   OdImageRenderer(ByteString& current_layer_name, bool& is_object_visible);
  virtual ~OdImageRenderer();

  virtual bool Start(CPDF_RenderStatus* pStatus, CPDF_ImageObject* pImageObject, const CFX_Matrix* pObj2Device,
             bool bStdCS, int blendType);

  virtual bool Start(CPDF_RenderStatus* pStatus, const RetainPtr<CFX_DIBSource>& pDIBSource, FX_ARGB bitmap_argb,
             int bitmap_alpha, const CFX_Matrix* pImage2Device, uint32_t flags, bool bStdCS, int blendType);

 protected:
  virtual bool DrawMaskedImage();
  virtual void CalculateDrawImage(CFX_DefaultRenderDevice* bitmap_device1, CFX_DefaultRenderDevice* bitmap_device2,
                          const RetainPtr<CFX_DIBSource>& pDIBSource, CFX_Matrix* pNewMatrix, const FX_RECT& rect) const;

  ByteString& m_CurrentLayerName;
  bool&       m_IsObjectVisible;
};

#endif  // ODIMAGERENDERER_H_
