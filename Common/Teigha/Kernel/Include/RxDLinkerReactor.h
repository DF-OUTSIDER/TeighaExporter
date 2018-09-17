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




// RxDLinkerReactor.h: interface for the OdRxDLinkerReactor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ODRXDLINKERREACTOR_H_INCLUDED_)
#define _ODRXDLINKERREACTOR_H_INCLUDED_

#include "TD_PackPush.h"

/** \details
    This class is the base class for custom classes that receive notification
    of OdRxDLinker events.
    
    \remarks
    Events consist of the loading and unloading of OdRxModules.
    
    \remarks
    The default implementations of all methods in this class do nothing.

    \sa
    TD_Root

    <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxDLinkerReactor : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxDLinkerReactor);

  /** \details
    Notification function called whenever an OdRxModule (TX application) will be loaded.
    \param moduleName [in]  Module name.
    \remarks
    This function is called before the operation. 
  */
  virtual void rxAppWillBeLoaded(const OdString& moduleName);
  /** \details
    Notification function called whenever an OdRxModule (TX application) has been loaded.
    \param pModule [in]  Pointer to the OdRxModule.
    \remarks
    This function is called after the operation. 
  */
  virtual void rxAppLoaded(OdRxModule* pModule);
  /** \details
    Notification function called whenever the loading of an OdRxModule (TX application) has been aborted.
    \param moduleName [in]  Module name.
    \remarks
    This function is after before the operation. 
  */
  virtual void rxAppLoadAborted(const OdString& moduleName);

  /** \details
    Notification function called whenever an OdRxModule (TX application) is about to be unloaded.
    \param pModule [in]  Pointer to the OdRxModule.
    \remarks
    This function is called before the operation. 
  */
  virtual void rxAppWillBeUnloaded(OdRxModule* pModule);
  /** \details
    Notification function called whenever an OdRxModule (TX application) has been unloaded.
    \param moduleName [in]  Module name.
    \remarks
    This function is called after the operation. 
  */
  virtual void rxAppUnloaded(const OdString& moduleName);
  /** \details
    Notification function called whenever the unloading of an OdRxModule (TX application) has been aborted.
    \param moduleName [in]  Module name.
    \remarks
    This function is after before the operation. 
  */
  virtual void rxAppUnloadAborted(OdRxModule* pModule);
};

#include "TD_PackPop.h"

#endif // !defined(_ODRXDLINKERREACTOR_H_INCLUDED_)

