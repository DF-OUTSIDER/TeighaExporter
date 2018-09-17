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


#ifndef _OD_RXMODULE_H_
#define _OD_RXMODULE_H_

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
  <group OdRx_Classes>

  This class provides the base implementation of custom Teigha applications (TX modules).

  Corresponding C++ library: TD_Db

  \sa
  <link CustomApplications.html, Creating Custom Applications>

  OdRxStaticModule, OdRxPseudoStaticModule, OdRxMacModule, OdRxUnixModule, OdRxWin32Module classes
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxModule : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxModule);

  /** \details
    Declares the method returning the system data for the rx-module instance.

    \remarks
    It is the pure virtual method that does not have an implementation. You must redefine it in the derived class.
  */
  virtual void* sysData() = 0;

  /** \details
    Declares the method deleting (unloading) the rx-module.

    \remarks
    It is the pure virtual method that does not have an implementation. You must redefine it in the derived class.
  */
  virtual void deleteModule() = 0;

  /** \details
    Declares the notification method being called at the start of the client application.
      
    \remarks
    It is the pure virtual method that does not have an implementation. You must redefine it in the 
    derived class. Overrides of this module should register any custom objects or commands defined
    in the module.

    \sa
    <link CustomApplications.html, Creating Custom Applications>
  */
  virtual void initApp() = 0;

  /** \details
    Declares the notification method being called at the end of the client application.

    \remarks
    It is the pure virtual method that does not have an implementation. You must redefine it in the 
    derived class. Overrides of this module should unregister any custom objects or commands defined
    in the module.

    \sa
    <link CustomApplications.html, Creating Custom Applications>
  */
  virtual void uninitApp() = 0;

  /** \details
    Declares the method returning the module name for the rx-module instance.

    \remarks
    It is the pure virtual method that does not have an implementation. You must redefine it in the derived class.
  */
  virtual OdString moduleName() const = 0;
};

/** \details
  The typified smart pointer for the rx-module object (TX module). This  
  template class is specialization of the OdSmartPtr class for the OdRxModule class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdRxModule> OdRxModulePtr;


#include "TD_PackPop.h"

#endif // _OD_RXMODULE_H_

