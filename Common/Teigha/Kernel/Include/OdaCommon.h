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

#ifndef _ODA_COMMON_INCLUDED_
#define _ODA_COMMON_INCLUDED_

#ifdef _MSC_VER
#pragma warning(disable: 4710)  // 'function' : function not inlined
#pragma warning(disable: 4100)  // unreferenced formal parameter
#endif

#if defined(_MSC_VER) && !defined(_WIN32_WINNT) && !defined(_WINRT)
#define _WIN32_WINNT 0x0500
#endif

#if defined(_WINRT) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0602
#endif

#include "TDVersion.h"
#include "RootExport.h"
#include "DbRootExport.h"

#include  "DebugStuff.h"
#include  "OdaDefs.h"
#include  "OdPlatform.h"

#include "OdModuleNames.h"

/* {NoAutoLink} */
#define OdaPI 3.14159265358979323846
/* {NoAutoLink} */
#define OdaPI2 (OdaPI / 2.0)
/* {NoAutoLink} */
#define OdaPI4 (OdaPI / 4.0)
/* {NoAutoLink} */
#define Oda2PI (OdaPI+OdaPI)

/** \details
  Converts a value in radians to a value in degrees.

  \sa
  OdaToRadian() pseudo-function
*/
#define OdaToDegree(rad) ((rad)/OdaPI*180.0)

/** \details
  Converts a value in degrees to a value in radians.

  \sa
  OdaToDegree() pseudo-function
*/
#define OdaToRadian(deg) ((deg)*OdaPI/180.0)

// SSL:
// from http://www.ex.ac.uk/cimt/dictunit/dictunit.htm
//
// Even as late as the middle of the 20th century there were some differences in UK
// and US measures which were nominally the same. The UK inch measured 2.53998 cm while
// the US inch was 2.540005 cm. Both were standardised at 2.54 cm in July 1959.

/* {NoAutoLink} */
const double kMmPerInch = 25.4;

#ifdef EMCC
#include <iostream>
#endif

#endif

