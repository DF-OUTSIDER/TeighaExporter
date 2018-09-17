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

#include "OdaCommon.h"
#include "ProgressiveRenderer.h"
#include "public/fpdfview.h"

#include <memory>
#include <utility>
#include <vector>

#include "core/fpdfapi/cpdf_modulemgr.h"
#include "core/fpdfapi/cpdf_pagerendercontext.h"
#include "core/fpdfapi/page/cpdf_image.h"
#include "core/fpdfapi/page/cpdf_imageobject.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pageobject.h"
#include "core/fpdfapi/page/cpdf_pathobject.h"
#include "core/fpdfapi/parser/cpdf_array.h"
#include "core/fpdfapi/parser/cpdf_dictionary.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/fpdf_parser_decode.h"
#include "core/fpdfapi/render/cpdf_progressiverenderer.h"
#include "core/fpdfapi/render/cpdf_renderoptions.h"
#include "core/fpdfdoc/cpdf_annotlist.h"
#include "core/fpdfdoc/cpdf_nametree.h"
#include "core/fpdfdoc/cpdf_occontext.h"
#include "core/fpdfdoc/cpdf_viewerpreferences.h"
#include "core/fxcrt/fx_memory.h"
#include "core/fxcrt/fx_safe_types.h"
#include "core/fxcrt/fx_stream.h"
#include "core/fxge/cfx_defaultrenderdevice.h"
#include "core/fxge/cfx_gemodule.h"
#include "fpdfsdk/cpdfsdk_formfillenvironment.h"
#include "fpdfsdk/cpdfsdk_pageview.h"
#include "fpdfsdk/fsdk_define.h"
#include "fpdfsdk/fsdk_pauseadapter.h"
#include "fxjs/ijs_runtime.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_ext.h"
#include "public/fpdf_formfill.h"
#include "public/fpdf_progressive.h"
#include "third_party/base/allocator/partition_allocator/partition_alloc.h"
#include "third_party/base/numerics/safe_conversions_impl.h"
#include "third_party/base/ptr_util.h"

void RenderPageImplNew(CPDF_PageRenderContext* pContext, CPDF_Page* pPage, const CFX_Matrix& matrix, const FX_RECT& clipping_rect,
                    int flags, bool bNeedToRestore, IFSDK_PAUSE_Adapter* pause, ByteString& current_layer_name, bool& is_object_visible)
{
  if (!pContext->m_pOptions)
    pContext->m_pOptions = pdfium::MakeUnique<CPDF_RenderOptions>();

  uint32_t option_flags = pContext->m_pOptions->GetFlags();
  if (flags & FPDF_LCD_TEXT)
    option_flags |= RENDER_CLEARTYPE;
  else
    option_flags &= ~RENDER_CLEARTYPE;

  if (flags & FPDF_NO_NATIVETEXT)
    option_flags |= RENDER_NO_NATIVETEXT;
  if (flags & FPDF_RENDER_LIMITEDIMAGECACHE)
    option_flags |= RENDER_LIMITEDIMAGECACHE;
  if (flags & FPDF_RENDER_FORCEHALFTONE)
    option_flags |= RENDER_FORCE_HALFTONE;
#ifndef PDF_ENABLE_XFA
  if (flags & FPDF_RENDER_NO_SMOOTHTEXT)
    option_flags |= RENDER_NOTEXTSMOOTH;
  if (flags & FPDF_RENDER_NO_SMOOTHIMAGE)
    option_flags |= RENDER_NOIMAGESMOOTH;
  if (flags & FPDF_RENDER_NO_SMOOTHPATH)
    option_flags |= RENDER_NOPATHSMOOTH;
#endif  // PDF_ENABLE_XFA
  pContext->m_pOptions->SetFlags(option_flags);

  // Grayscale output
  if (flags & FPDF_GRAYSCALE)
    pContext->m_pOptions->SetColorMode(CPDF_RenderOptions::kGray);

  const CPDF_OCContext::UsageType usage =
      (flags & FPDF_PRINTING) ? CPDF_OCContext::Print : CPDF_OCContext::View;
  pContext->m_pOptions->SetOCContext(
      pdfium::MakeRetain<CPDF_OCContext>(pPage->m_pDocument.Get(), usage));

  pContext->m_pDevice->SaveState();
  pContext->m_pDevice->SetClip_Rect(clipping_rect);
  pContext->m_pContext = pdfium::MakeUnique<CPDF_RenderContext>(pPage);
  pContext->m_pContext->AppendLayer(pPage, &matrix);

  if (flags & FPDF_ANNOT) {
    pContext->m_pAnnots = pdfium::MakeUnique<CPDF_AnnotList>(pPage);
    bool bPrinting = pContext->m_pDevice->GetDeviceClass() != FXDC_DISPLAY;
    pContext->m_pAnnots->DisplayAnnots(pPage, pContext->m_pContext.get(),
                                       bPrinting, &matrix, false, nullptr);
  }

  pContext->m_pRenderer = pdfium::MakeUnique<OdProgressiveRenderer>(
      pContext->m_pContext.get(), pContext->m_pDevice.get(),
      pContext->m_pOptions.get(), current_layer_name, is_object_visible);
  pContext->m_pRenderer->Start(pause);
  if (bNeedToRestore)
    pContext->m_pDevice->RestoreState(false);
}

void RenderPage_Retail(CPDF_PageRenderContext* pContext, FPDF_PAGE page, int start_x, int start_y, int size_x,
  int size_y, int rotate, int flags, bool bNeedToRestore, IFSDK_PAUSE_Adapter* pause, ByteString& current_layer_name, bool& is_object_visible) 
{
  CPDF_Page* pPage = CPDFPageFromFPDFPage(page);
  if (!pPage)
    return;

  RenderPageImplNew(pContext, pPage, pPage->GetDisplayMatrix(
    start_x, start_y, size_x, size_y, rotate),
    FX_RECT(start_x, start_y, start_x + size_x, start_y + size_y),
    flags, bNeedToRestore, pause, current_layer_name, is_object_visible);
}

