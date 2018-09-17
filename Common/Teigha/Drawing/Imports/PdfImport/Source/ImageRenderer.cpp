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

#include "ImageRenderer.h"

#include <algorithm>
#include <memory>

#include "core/fpdfapi/page/cpdf_docpagedata.h"
#include "core/fpdfapi/page/cpdf_image.h"
#include "core/fpdfapi/page/cpdf_imageobject.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pageobject.h"
#include "core/fpdfapi/page/cpdf_shadingpattern.h"
#include "core/fpdfapi/page/cpdf_tilingpattern.h"
#include "core/fpdfapi/parser/cpdf_array.h"
#include "core/fpdfapi/parser/cpdf_dictionary.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/render/cpdf_dibsource.h"
#include "core/fpdfapi/render/cpdf_pagerendercache.h"
#include "core/fpdfapi/render/cpdf_rendercontext.h"
#include "core/fpdfapi/render/cpdf_renderstatus.h"
#include "core/fpdfapi/render/cpdf_transferfunc.h"
#include "core/fpdfdoc/cpdf_occontext.h"
#include "core/fxcrt/fx_safe_types.h"
#include "core/fxcrt/maybe_owned.h"
#include "core/fxge/cfx_defaultrenderdevice.h"
#include "core/fxge/cfx_pathdata.h"
#include "core/fxge/dib/cfx_dibitmap.h"
#include "core/fxge/dib/cfx_dibsource.h"
#include "core/fxge/dib/cfx_imagestretcher.h"
#include "core/fxge/dib/cfx_imagetransformer.h"
#include "third_party/base/ptr_util.h"
#include "third_party/base/stl_util.h"

#ifdef _SKIA_SUPPORT_
#include "core/fxge/skia/fx_skia_device.h"
#endif

OdImageRenderer:: OdImageRenderer(ByteString& current_layer_name, bool& is_object_visible)
  :CPDF_ImageRenderer()
  ,m_CurrentLayerName(current_layer_name)
  ,m_IsObjectVisible(is_object_visible)
{

}

OdImageRenderer::~OdImageRenderer() 
{

}

bool OdImageRenderer::Start(CPDF_RenderStatus* pStatus, CPDF_ImageObject* pImageObject, const CFX_Matrix* pObj2Device,
                               bool bStdCS, int blendType) 
{
  ASSERT(pImageObject);
  m_pRenderStatus = pStatus;
  m_bStdCS = bStdCS;
  m_pImageObject = pImageObject;
  m_BlendType = blendType;
  m_pObj2Device = pObj2Device;
  CPDF_Dictionary* pOC = m_pImageObject->GetImage()->GetOC();

  if (pOC && m_pRenderStatus->GetRenderOptions()->GetOCContext())
  {
    m_IsObjectVisible = m_pRenderStatus->GetRenderOptions()->GetOCContext()->CheckOCGVisible(pOC);
    m_CurrentLayerName = ByteString::FromUnicode(pOC->GetUnicodeTextFor("Name"));
  }

  m_ImageMatrix = m_pImageObject->matrix();
  m_ImageMatrix.Concat(*pObj2Device);
  if (StartLoadDIBSource())
    return true;
  return StartRenderDIBSource();
}

bool OdImageRenderer::Start(CPDF_RenderStatus* pStatus, const RetainPtr<CFX_DIBSource>& pDIBSource, FX_ARGB bitmap_argb,
                               int bitmap_alpha, const CFX_Matrix* pImage2Device, uint32_t flags, bool bStdCS, int blendType) 
{
  m_pRenderStatus = pStatus;
  m_pDIBSource = pDIBSource;
  m_FillArgb = bitmap_argb;
  m_BitmapAlpha = bitmap_alpha;
  m_ImageMatrix = *pImage2Device;
  m_Flags = flags;
  m_bStdCS = bStdCS;
  m_BlendType = blendType;
  return StartDIBSource();
}

void OdImageRenderer::CalculateDrawImage( CFX_DefaultRenderDevice* pBitmapDevice1, CFX_DefaultRenderDevice* pBitmapDevice2,
    const RetainPtr<CFX_DIBSource>& pDIBSource, CFX_Matrix* pNewMatrix, const FX_RECT& rect) const 
{
  CPDF_RenderStatus bitmap_render;
  bitmap_render.Initialize(m_pRenderStatus->GetContext(), pBitmapDevice2,
                           nullptr, nullptr, nullptr, nullptr, nullptr, 0,
                           m_pRenderStatus->GetDropObjects(), nullptr, true);
  OdImageRenderer image_render(m_CurrentLayerName, m_IsObjectVisible);
  if (image_render.Start(&bitmap_render, pDIBSource, 0xffffffff, 255,
                         pNewMatrix, m_Flags, true, FXDIB_BLEND_NORMAL)) 
  {
    image_render.Continue(nullptr);
  }
  if (m_Loader.m_MatteColor == 0xffffffff)
    return;
  int matte_r = FXARGB_R(m_Loader.m_MatteColor);
  int matte_g = FXARGB_G(m_Loader.m_MatteColor);
  int matte_b = FXARGB_B(m_Loader.m_MatteColor);
  for (int row = 0; row < rect.Height(); row++)
  {
    uint8_t* dest_scan =
        const_cast<uint8_t*>(pBitmapDevice1->GetBitmap()->GetScanline(row));
    const uint8_t* mask_scan = pBitmapDevice2->GetBitmap()->GetScanline(row);
    for (int col = 0; col < rect.Width(); col++) 
    {
      int alpha = *mask_scan++;
      if (!alpha) 
      {
        dest_scan += 4;
        continue;
      }
      int orig = (*dest_scan - matte_b) * 255 / alpha + matte_b;
      *dest_scan++ = pdfium::clamp(orig, 0, 255);
      orig = (*dest_scan - matte_g) * 255 / alpha + matte_g;
      *dest_scan++ = pdfium::clamp(orig, 0, 255);
      orig = (*dest_scan - matte_r) * 255 / alpha + matte_r;
      *dest_scan++ = pdfium::clamp(orig, 0, 255);
      dest_scan++;
    }
  }
}

bool OdImageRenderer::DrawMaskedImage() 
{
  if (NotDrawing())
  {
    m_Result = false;
    return false;
  }

  FX_RECT rect = GetDrawRect();
  if (rect.IsEmpty())
    return false;

  CFX_Matrix new_matrix = GetDrawMatrix(rect);
  CFX_DefaultRenderDevice bitmap_device1;
  if (!bitmap_device1.Create(rect.Width(), rect.Height(), FXDIB_Rgb32, nullptr))
    return true;

#if defined _SKIA_SUPPORT_
  bitmap_device1.Clear(0xffffff);
#else
  bitmap_device1.GetBitmap()->Clear(0xffffff);
#endif
  CPDF_RenderStatus bitmap_render;
  bitmap_render.Initialize(m_pRenderStatus->GetContext(), &bitmap_device1,
                           nullptr, nullptr, nullptr, nullptr, nullptr, 0,
                           m_pRenderStatus->GetDropObjects(), nullptr, true);
  OdImageRenderer image_render(m_CurrentLayerName, m_IsObjectVisible);
  if (image_render.Start(&bitmap_render, m_pDIBSource, 0, 255, &new_matrix,
                         m_Flags, true, FXDIB_BLEND_NORMAL)) 
  {
    image_render.Continue(nullptr);
  }
  CFX_DefaultRenderDevice bitmap_device2;
  if (!bitmap_device2.Create(rect.Width(), rect.Height(), FXDIB_8bppRgb,
                             nullptr))
    return true;

#if defined _SKIA_SUPPORT_
  bitmap_device2.Clear(0);
#else
  bitmap_device2.GetBitmap()->Clear(0);
#endif
  CalculateDrawImage(&bitmap_device1, &bitmap_device2, m_Loader.m_pMask,
                     &new_matrix, rect);
#ifdef _SKIA_SUPPORT_
  m_pRenderStatus->GetRenderDevice()->SetBitsWithMask(
      bitmap_device1.GetBitmap(), bitmap_device2.GetBitmap(), rect.left,
      rect.top, m_BitmapAlpha, m_BlendType);
#else
  bitmap_device2.GetBitmap()->ConvertFormat(FXDIB_8bppMask);
  bitmap_device1.GetBitmap()->MultiplyAlpha(bitmap_device2.GetBitmap());
  if (m_BitmapAlpha < 255)
    bitmap_device1.GetBitmap()->MultiplyAlpha(m_BitmapAlpha);
  m_pRenderStatus->GetRenderDevice()->SetDIBitsWithBlend(
      bitmap_device1.GetBitmap(), rect.left, rect.top, m_BlendType);
#endif  //  _SKIA_SUPPORT_
  return false;
}

