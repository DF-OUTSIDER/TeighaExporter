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
#ifndef __DG_COLORTABLE_H__
#define __DG_COLORTABLE_H__


#include "TD_PackPush.h"

#include "DgElement.h"
#include "DgCmColor.h"

class OdCmEntityColor;

/** \details
    1. The ordinary color table of DGN files. Each database (file) could have its color table of 256 colors (otherwise, the default color table is available).
        This table keeps all color for the indices [0...255].

    2. Also, the extended color table could be stored among xAttributes of the file (see the static methods below).
        This table could keep an unlimited number of colors linked by indices of { indices > 255 }.

    3. Please, note that a set of values is predefined (kColorByCell, kByLevel, etc). See OdDgGraphicsElement.h for details. 
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgColorTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgColorTable);
public:

  //uses this table only to search for the color;
  //returns zero if fails (+the method to check indices)
  ODCOLORREF lookupRGB(OdUInt32 colorIndex);
  bool isCorrectIndex( OdUInt32 colorIndex );

  //use the color tables of the given database to search for the color (either ordinary or extended ones);
  //returns zero if fails (+the method to check indices)
  static OdDgCmEntityColor lookupTrueColor(OdDgDatabase* pDb, OdUInt32 colorIndex);
  static ODCOLORREF lookupRGB(OdDgDatabase* pDb, OdUInt32 colorIndex);
  static bool isCorrectIndex(  OdDgDatabase* pDb, OdUInt32 colorIndex );

  //it tries to find the given RGB color within existing color tables (or add a new color to the extended color table)
  static OdUInt32 getColorIndexByRGB(  OdDgDatabase* pDb, ODCOLORREF rgbColor ) ;

  const ODCOLORREF* palette();
  void setPalette(OdArray<ODCOLORREF> colors);

  static const ODCOLORREF* currentPalette(OdDgDatabase* pDb);
  static const ODCOLORREF* defaultPalette();

  // If background is white MicroStation draws elements black instead of white.
  //  To do the same if background color is defined for this palette as white (element with index #254) 
  //  then all elements in palette with white will be tuned to black.
  static bool correctPaletteForWhiteBackground(ODCOLORREF* palette);
};

typedef OdSmartPtr<OdDgColorTable> OdDgColorTablePtr;


#include "TD_PackPop.h"

#endif // __DG_COLORTABLE_H__
