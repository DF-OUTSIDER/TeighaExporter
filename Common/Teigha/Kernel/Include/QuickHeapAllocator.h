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

#ifndef _OdQuickHeap_h_Included_
#define _OdQuickHeap_h_Included_

#include "RootExport.h"
#include <stdlib.h> // size_t

/** \details
  <group !!RECORDS_tkernel_apiref>
*/
      struct FIRSTDLL_EXPORT OdQuickHeap
{
  static void* Alloc( size_t );
  static void* Realloc( void*,size_t );
  static void Free( void* );
};

#endif // _OdQuickHeap_h_Included_
