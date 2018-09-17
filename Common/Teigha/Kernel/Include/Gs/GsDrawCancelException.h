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
// GsDrawCancelException.h : OdGsDrawCancelException class

// Copyright © Menhirs NV. All rights reserved.

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef H_GS_DRAW_CANCEL_EXCEPTION
#define H_GS_DRAW_CANCEL_EXCEPTION

#include "Gs/GsExport.h"

/** \details
  Exception thrown when draw operation is cancelled.
  Bricscad throws it in progress bar meter and catches in preview image draw function;
  OdGsBaseVectorizeView::doDraw() is an intermediate catcher which throws it further.
        
  Corresponding C++ library: TD_Gs
  <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsDrawCancelException
{
};

#endif // H_GS_DRAW_CANCEL_EXCEPTION
