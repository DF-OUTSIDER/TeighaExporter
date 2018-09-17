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

#ifndef _PDF_INPORT_FUNCTION_INCLUDED_
#define _PDF_INPORT_FUNCTION_INCLUDED_

#include "TD_PackPush.h"
#include "core/fxcrt/fx_coordinates.h"
#include "public/fpdfview.h"

class CPDF_PageRenderContext;
class IFSDK_PAUSE_Adapter;

void RenderPage_Retail(CPDF_PageRenderContext* pContext,
  FPDF_PAGE page,
  int start_x,
  int start_y,
  int size_x,
  int size_y,
  int rotate,
  int flags,
  bool bNeedToRestore,
  IFSDK_PAUSE_Adapter* pause, ByteString& current_layer_name, bool& is_object_visible);

#include "TD_PackPop.h"

#endif // _PDF_INPORT_FUNCTION_INCLUDED_
