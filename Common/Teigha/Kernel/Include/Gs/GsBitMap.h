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




#ifndef _OD_GSBITMAP_H_
#define _OD_GSBITMAP_H_

#include "RxObject.h"
#include "Gs/Gs.h"
#include "OdStreamBuf.h"
#include "SmartPtr.h"


/** \details

    <group OdGs_Classes> 
*/
class OdGsPalette : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGsPalette);
  OdGsPalette(){}

  virtual OdUInt32 numColors() const = 0;
  virtual void setNumColors(OdUInt32 nColors) = 0;
  virtual void setColorAt(OdUInt32 nIndex, OdUInt8 blue, OdUInt8 green, OdUInt8 red, OdUInt8 alpha = 0) = 0;
  virtual void colorAt(OdUInt32 nIndex, OdUInt8& blue, OdUInt8& green, OdUInt8& red, OdUInt8* pAlpha = 0) const = 0;
  virtual void setColors(OdUInt32 nColors, const ODCOLORREF* pColors);
  virtual void getColors(OdUInt32 nColors, ODCOLORREF* pColors) const;
};

typedef OdSmartPtr<OdGsPalette> OdGsPalettePtr;


/** \details

    <group OdGs_Classes> 
*/
class OdGsBitMap : public OdRxObject
{
protected:
  OdGsBitMap() {}
public:
  ODRX_DECLARE_MEMBERS(OdGsBitMap);

  virtual OdUInt32 width() const = 0;
  virtual OdUInt32 height() const = 0;
  virtual OdUInt8 bitPerPixel() const = 0;
  virtual void create(OdUInt32 width, OdUInt32 height, OdUInt8 bitCount) = 0;

  virtual OdUInt32 bitDataSize() const;
  virtual OdUInt32 bytePerLine() const;
  virtual const OdUInt8* bits() const = 0;
  virtual void setBits(const OdUInt8* pData, OdUInt32 nSize) = 0;

  virtual OdGsPalette* palette() = 0;
  virtual const OdGsPalette* palette() const = 0;
};

typedef OdSmartPtr<OdGsBitMap> OdGsBitMapPtr;

#endif //_OD_GSBITMAP_H_
