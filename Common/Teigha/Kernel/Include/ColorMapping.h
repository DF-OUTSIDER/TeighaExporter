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

#ifndef COLORMAPPING_DEFINED
#define COLORMAPPING_DEFINED

#include "OdPlatform.h"


// Background is dark if  Red < 128 && Green < 128 && Blue < 128
// Background is light if  Red > 127 || Green > 127 || Blue > 127
inline bool odcmIsBackgroundLight(ODCOLORREF backgroung)
{
  return (ODGETRED(backgroung) >= 128 ||
					ODGETGREEN(backgroung) >= 128 ||
					ODGETBLUE(backgroung) >= 128);
}

//DD:EXPORT_ON

// Returns palette for dark background
FIRSTDLL_EXPORT const ODCOLORREF * odcmAcadDarkPalette();

// Returns palette for light background
FIRSTDLL_EXPORT const ODCOLORREF * odcmAcadLightPalette();

// Returns palette for given background
FIRSTDLL_EXPORT const ODCOLORREF * odcmAcadPalette(ODCOLORREF backgr);


// Returns RGB for given color index
// pPalette - palette to perform mapping
FIRSTDLL_EXPORT ODCOLORREF odcmLookupRGB(int index, const ODCOLORREF* pPalette);

// Returns color index for given RGB
// pPalette - palette to perform mapping
FIRSTDLL_EXPORT int odcmLookupACI(ODCOLORREF rgb, const ODCOLORREF* pPalette);

//DD:EXPORT_OFF

#endif	// COLORMAPPING_DEFINED
