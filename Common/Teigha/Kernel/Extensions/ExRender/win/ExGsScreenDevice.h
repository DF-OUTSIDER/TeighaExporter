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

#ifndef ODEXSCREENDEVICE_INCLUDED
#define ODEXSCREENDEVICE_INCLUDED

#include "TD_PackPush.h"

#include "RxObjectImpl.h"
#include "Gs/Gs.h"

#include "ExGsWin32Device.h"
/** <group ExRender_Windows_Classes> 
*/
template <class T>
class ExGsScreenDevice : public T
{
  bool m_bMustReleaseDc;
public:
  ExGsScreenDevice()
    : m_bMustReleaseDc(false)
  {
  }
  virtual ~ExGsScreenDevice()
  {
    deleteContext();
  }
  /** \details
    Creates an instance of this Device object.
  */
  static OdGsDevicePtr createObject()
  {
    return OdRxObjectImpl<ExGsScreenDevice<T>, OdGsDevice>::createObject();
  }
protected:

  /** \details
    Creates a context for this Device object.
  */
  void createContext()
  {
    if(T::m_hWnd && !T::m_hDC)
    {
      T::m_hDC = ::GetDC(T::m_hWnd);
      m_bMustReleaseDc = true;
    }

    if(!T::m_hDC)
      throw OdError(eNotInitializedYet);

    T::m_nColorDepth = T::getDeviceColorDepth();
    T::createContext();
  }

  /** \details
    Deletes the context for this Device object.
  */
  void deleteContext()
  {
    T::deleteContext();
  	if(m_bMustReleaseDc)
	  {
      ::ReleaseDC(T::m_hWnd, T::m_hDC);
		  T::m_hDC = NULL;
  	}
  }

  /** \details
    Creates a logical palette for this Device object.
  */
   void createPalette()
  {
    ExGsWin32Device::createPalette(&T::m_logPalette);
    if (T::m_pPal)
    {
     	T::m_pOldPal = SelectPalette(T::m_hDC, T::m_pPal, FALSE);
    	RealizePalette(T::m_hDC);
    }
  }

  /** \details
    Returns true if and only if the vectorization is to be rendered to the screen.
    \note
    The default implementation of this function always returns true.
  */
  bool renderToScreen() const
  {
    return true;
  }
};

#include "TD_PackPop.h"

#endif //ODEXSCREENDEVICE_INCLUDED
