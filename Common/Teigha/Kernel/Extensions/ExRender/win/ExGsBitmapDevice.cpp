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


// to satisfy VC2005, else it refuses to compile WinDirectX project
#if !defined(_WIN32_WCE) && (_MSC_VER >= 1400)
  #define POINTER_64 __ptr64
#endif

#include "OdaCommon.h"
#include "Gs/GsBaseVectorizer.h"
#include "ExGsBitmapDevice.h"
#include "StaticRxObject.h"
#include "RxObjectImpl.h"

//Implementation of class GsDIBSection
//*******************************************************************************/
// AE 03.02.2003 - Begin 
GsDIBSection::GsDIBSection()
{
  m_hbmp  = NULL ;
	m_pBits = NULL;
	m_hdc   = NULL ;
}

GsDIBSection::~GsDIBSection()
{
	if (m_hdc)
  {
    destroy();
    DeleteDC(m_hdc);
  }
}


void GsDIBSection::destroy()
{
  if (m_hbmp)
  {
    if (m_hbmOld)
    {
      SelectObject(m_hdc, m_hbmOld);
      m_hbmOld = 0;
    }
    
    DeleteObject(m_hbmp);
    m_hbmp = 0;
  }
}

void GsDIBSection::createDib(int cx, int cy, int bpp) 
{
  if(cx <= 0 || cy <= 0)
    throw OdError(eInvalidInput);

  switch(bpp)
  {
  case  1:
  case  2:
  case  4:
  case  8:
  case 16:
  case 24:
  case 32:
    break;
  default:
    throw OdError(eInvalidInput);
  }

  if(!m_hdc)
    m_hdc = CreateCompatibleDC(NULL);

  if(m_hbmp)
  {
    BITMAP bmp;
    GetObject(m_hbmp, sizeof(bmp), &bmp);
    if(bmp.bmWidth!=cx || bmp.bmHeight!=cy || bmp.bmBitsPixel != bpp)
      destroy();
  }

  if(!m_hbmp)
  {
    BITMAPINFO* bmi = (BITMAPINFO*)::odrxAlloc(sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
    if(!bmi)
      throw OdError(eOutOfMemory);
    
    memset(bmi, 0, sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));

    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = cx;
    bmi->bmiHeader.biHeight = cy;
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = (WORD)bpp;
    bmi->bmiHeader.biCompression = BI_RGB;

    m_hbmp = CreateDIBSection(
        m_hdc,
        bmi,
        bpp < 16 ? DIB_PAL_COLORS : DIB_RGB_COLORS,
        &m_pBits,
        NULL,
        0
      );
    
    odrxFree(bmi);
    
    if(!m_hbmp)
      throw OdError(eExtendedError);

    m_hbmOld = (HBITMAP)SelectObject(m_hdc, m_hbmp);
  }
}

OdUInt32 GsDIBSection::getDibWidth() const 
{
  if(m_hbmp)
  {
    BITMAP bmp;
    GetObject(m_hbmp, sizeof(bmp), &bmp);
    return bmp.bmWidth;
  }
  return 0;
}

OdUInt32 GsDIBSection::getDibHeight() const 
{
  if(m_hbmp)
  {
    BITMAP bmp;
    GetObject(m_hbmp, sizeof(bmp), &bmp);
    return bmp.bmHeight;
  }
  return 0;
}

int GsDIBSection::getBitsPerPixel() const
{
  if(m_hbmp)
  {
    BITMAP bmp;
    GetObject(m_hbmp, sizeof(bmp), &bmp);
    return bmp.bmBitsPixel;
  }
  return 0;
}

OdUInt32 GsDIBSection::scanLineSize() const
{
  if(m_hbmp)
  {
    BITMAP bmp;
    GetObject(m_hbmp, sizeof(bmp), &bmp);
    return OdGiRasterImage::calcBMPScanLineSize(bmp.bmWidth, bmp.bmBitsPixel);
  }
  return 0;
}

void GsDIBSection::scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines) const
{
  OdUInt32 scLSz(scanLineSize());
  memcpy(pBytes, (OdUInt8*)m_pBits + index*scLSz, numLines*scLSz);
}

const OdUInt8* GsDIBSection::scanLines() const
{
  return (OdUInt8*)m_pBits;
}

void GsDIBSection::setPalette(HPALETTE hPal)
{
  m_hPal = hPal;
  ODA_ASSERT(hPal);
  // get the colors from the palette
  int iColors = 0;
  GetObject(hPal, sizeof(iColors), &iColors);
  ODA_ASSERT(iColors > 0);
  OdArray<PALETTEENTRY, OdMemoryAllocator<PALETTEENTRY> > buffer;
  buffer.resize(iColors);
  PALETTEENTRY* pPE = buffer.asArrayPtr();
  GetPaletteEntries(hPal, 0, iColors, pPE);

  // Build a table of RGBQUADS
  m_paletteData.clear();
  m_paletteData.resize(iColors * 4);
  for (int i = 0; i < iColors; i++) 
  {
    int nPaletteDataIndex = i * 4;
    m_paletteData[nPaletteDataIndex + 2] = pPE[i].peRed;
    m_paletteData[nPaletteDataIndex + 1] = pPE[i].peGreen;
    m_paletteData[nPaletteDataIndex]     = pPE[i].peBlue;
    m_paletteData[nPaletteDataIndex + 3] = pPE[i].peFlags;
  }
  ::SetDIBColorTable(m_hdc, 0, iColors, (CONST RGBQUAD *)m_paletteData.getPtr());
}
