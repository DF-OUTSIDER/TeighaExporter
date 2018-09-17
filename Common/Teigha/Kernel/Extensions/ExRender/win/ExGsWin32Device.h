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

#ifndef ODEXGSWIN32DEVICE_INCLUDED
#define ODEXGSWIN32DEVICE_INCLUDED

#include "TD_PackPush.h"

#include "../CommonDeviceProps.h"
#include "OdPlatform.h"

typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;

/** \details
  This class contains methods and properties common to Win32 Vectorizer devices.
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
class ExGsWin32Device : public OdCommonDeviceProps
{
protected:
  virtual void HDCWasChanged() {}
public:
  ExGsWin32Device();
  /** \details
    Returns the color depth for this Device object.
  */
  virtual int getDeviceColorDepth() const;
  virtual ~ExGsWin32Device();
  
  /** \details
    Sets the device context for this Device object.
    \param hDC [in]  Handle to the device context.
  */
  HDC setHDC(HDC hDC) { HDC retDC = m_hDC; m_hDC = hDC; HDCWasChanged(); return retDC; }

  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExGsWin32Device);

  /** \details
    Sets the device context for this Device object.
    \param hDC [in]  Handle to the device context.
  */
  void setHDC(OdIntPtr hDC) { m_hDC = ((HDC)hDC); HDCWasChanged(); }
  /** \details
    Returns the handle to the device context for this Device object.
  */
  OdIntPtr getHDC() const { return (OdIntPtr)m_hDC; }
  
  /** \details
    Sets the handle to the window for this Device object.
    \param hWND [in]  Handle to the window.
  */
  void setHWND(OdIntPtr hWND) { m_hWnd = ((HWND)hWND); }
  /** \details
    Returns the handle to the window for this Device object.
  */
  OdIntPtr getHWND() const { return (OdIntPtr)m_hWnd; }
  
  /** \details
    Returns the resolution of the device context of this Device object.
  */
  long DPI()
  {
    long res = 1;
    if (m_nDPIOverride > 0) // #9633 (1)
    {
      res = m_nDPIOverride;
    }
    else
    {
      if(m_hDC)
      {
        res = odmin(::GetDeviceCaps(m_hDC, LOGPIXELSX), ::GetDeviceCaps(m_hDC, LOGPIXELSY));
      }
      else if(m_hWnd)
      {
        HDC hDC = ::GetDC(m_hWnd);
        res = odmin(::GetDeviceCaps(hDC, LOGPIXELSX), ::GetDeviceCaps(hDC, LOGPIXELSY));
        ::ReleaseDC(m_hWnd, hDC);
      }
    }
    return res;
  }
  /** \details
    Sets the resolution of the device context of this Device object.
    \note
    The default implemenation of this function does nothing but return.
    \param DPI [in]  Dots per inch.
  */
  void setDPI(long DPI);

protected:
  HDC       m_hDC;
  HWND      m_hWnd;  
  HPALETTE  m_pPal;
  HPALETTE  m_pOldPal;
  int       m_nColorDepth;
  bool      m_bColorIndexMode;
  long      m_nDPIOverride;

protected:
  /** \details
    Returns true if and only if the vectorization is to be rendered to the screen.
    \note
    The default implementation of this function always returns true.
  */
  virtual bool renderToScreen() const { return true; }
  /** \details
    Creates a device context for this Device object.
  */
  virtual void createContext() = 0;
  /** \details
    Deletes the device context for this Device object.
  */
  virtual void deleteContext();
  /** \details
    Creates a logical palette for this Device object.
    \param logicalPalette [out]  Receives a pointer to the logical palette.
  */
  virtual void createPalette(ODGSPALETTE *logicalPalette);
};

#include "TD_PackPop.h"

#endif // ODEXGSWIN32DEVICE_INCLUDED
