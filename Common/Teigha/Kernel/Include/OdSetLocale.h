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



#ifndef _OD_SETLOCALE_H_
#define _OD_SETLOCALE_H_

#include "TD_PackPush.h"


#if (defined(_WIN32) || defined(WIN64)) && !defined(_WIN32_WCE)
  #define TD_USE_LOCALE
#endif


#ifdef TD_USE_LOCALE

#include <locale.h>
#include "OdString.h"
#include "OdMutex.h"

/** \details
    This class sets and restores the current locale of the of Teigha context.
    
    \remarks
    The locale is restored OdSetLocale objects go out of scope.
    
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdSetLocale
{
public:
  /** \param category [in]  Category affected.
    \param locale [in]  Name of locale.
    \param ansiCp [in]  ANSI code page.
  */
  OdSetLocale(int category, const OdString& locale)
    : _autoLock(_mutex)
  {
    set(category, locale);
  }
  OdSetLocale(int category, int ansiCp)
    : _autoLock(_mutex)
  {
    if (ansiCp)
    {
      OdString loc;
      loc.format(OD_T(".%d"),ansiCp);
      set(category, loc.c_str());
    }
  }
  ~OdSetLocale()
  {
    reset();
  }
  void set(int category, const OdString& loc)
  {
    ODA_ASSERT(_origLocale.isEmpty());
    _category = category;
    
    OdString origLocale = ::_wsetlocale(_category, 0);
    if (origLocale.iCompare(loc) != 0 && ::_wsetlocale(_category, loc) != 0)
    {
      _origLocale = origLocale;
    }
  }
  void reset()
  {
    if (!_origLocale.isEmpty())
    {
      _wsetlocale(_category, _origLocale);
#ifdef _DEBUG
      OdString loc = _wsetlocale(_category, 0);
      ODA_ASSERT(!loc.iCompare(_origLocale));
#endif
      _origLocale.empty();
    }
  }

private:  
  FIRSTDLL_EXPORT_STATIC static OdMutex  _mutex;

  OdMutexAutoLock _autoLock;
  OdString        _origLocale;
  int             _category;
};

#define SET_LOCALE(category, locale) OdSetLocale loc(category, locale);
#define RESET_LOCALE() loc.reset();
#else  // TD_USE_LOCALE

#define SET_LOCALE(category, locale)
#define RESET_LOCALE()

#endif // TD_USE_LOCALE


#include "TD_PackPop.h"

#endif // _OD_SETLOCALE_H_

