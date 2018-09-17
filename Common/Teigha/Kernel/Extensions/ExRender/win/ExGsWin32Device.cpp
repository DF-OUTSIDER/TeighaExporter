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
#include "OdString.h" // else undefined in rxvariantvalue.h (?)
#include "ExGsWin32Device.h"

//Implementation of class ExGsWin32Device
// AE 11.02.2003 - Begin
//*******************************************************************************/
//  Constructor()
//*******************************************************************************/
ExGsWin32Device::ExGsWin32Device()
    : m_hWnd(NULL)
    , m_pOldPal(NULL)
    , m_pPal(NULL)
    , m_hDC(NULL)
    , m_nColorDepth(24)
    , m_bColorIndexMode(false)
    , m_nDPIOverride(0)
{
}

ODRX_DECLARE_PROPERTY(WindowHDC)
ODRX_DECLARE_PROPERTY(WindowHWND)
ODRX_DECLARE_PROPERTY(DPI)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsWin32Device);
  ODRX_INHERIT_PROPERTIES(OdCommonDeviceProps)
  ODRX_GENERATE_PROPERTY(WindowHDC)
  ODRX_GENERATE_PROPERTY(WindowHWND)
  ODRX_GENERATE_PROPERTY(DPI)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsWin32Device);

ODRX_DEFINE_PROPERTY_METHODS(WindowHDC,               ExGsWin32Device,  getHDC,                   setHDC,  getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS(WindowHWND,              ExGsWin32Device, getHWND,                   setHWND, getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS(DPI,                     ExGsWin32Device,     DPI,                   setDPI,  getInt32);

//*******************************************************************************/
//  Destructor()
//*******************************************************************************/
ExGsWin32Device::~ExGsWin32Device()
{
  deleteContext();
}


//*******************************************************************************/
//  deleteContext()
//*******************************************************************************/
void ExGsWin32Device::deleteContext()
{
  if (m_pOldPal)
        {
                // Select in old palette.
    SelectPalette(m_hDC, m_pOldPal, 0);
                m_pOldPal = NULL ;
        }

        if (m_pPal)
        {
                DeleteObject(m_pPal);
                m_pPal = NULL ;
        }
}

//*******************************************************************************/
//  getDeviceColorDepth()
//*******************************************************************************/
int ExGsWin32Device::getDeviceColorDepth() const
{
  return ::GetDeviceCaps(m_hDC, BITSPIXEL);
}


//*******************************************************************************/
//  createPalette()
//*******************************************************************************/
void ExGsWin32Device::createPalette(ODGSPALETTE *logPalette)
{
  OdUInt32 i;
  if (!m_bColorIndexMode)
  {
    return;
  }

  int nLogPaletteSz = (1 << m_nColorDepth);
  int iPalSize = sizeof(LOGPALETTE) + nLogPaletteSz * sizeof(PALETTEENTRY);

  OdUInt8Array palBuf;
  palBuf.resize(iPalSize);
  LOGPALETTE* pPal = (LOGPALETTE*)palBuf.asArrayPtr();
  if(!pPal)
  {
    ODA_TRACE0("Out of memory for logpal");
    return;
  }
  pPal->palVersion = 0x300; // Windows 3.0
  pPal->palNumEntries = (WORD)nLogPaletteSz; // table size

  for(i = 0; i < odmin(logPalette->size(), (OdUInt32)nLogPaletteSz); ++i)
  {
    pPal->palPalEntry[i].peRed    = ODGETRED(logPalette->getAt(i));
    pPal->palPalEntry[i].peGreen  = ODGETGREEN(logPalette->getAt(i));
    pPal->palPalEntry[i].peBlue   = ODGETBLUE(logPalette->getAt(i));
    pPal->palPalEntry[i].peFlags  = ODGETALPHA(logPalette->getAt(i));
  }

  if (m_pPal)
  {
    DeleteObject(m_pPal);
  }
  m_pPal = ::CreatePalette(pPal);
}
// AE - End

void ExGsWin32Device::setDPI(long dpi)
{
  // #9633 (1)
  m_nDPIOverride = dpi;
}
