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


#ifndef _ODRXMODULESOFTREFERENCE_H_
#define _ODRXMODULESOFTREFERENCE_H_

#include "DynamicLinker.h"
#include "RxDLinkerReactor.h"
#include "StaticRxObject.h"

#include "TD_PackPush.h"


/** \details
  <group OdRx_Classes>

  This template class represents client's atomic portion of code to execute via asynchronous call.

  Corresponding C++ library: TD_Db

  \sa
  OdRxModule, OdRxDLinkerReactor classes
*/
template <class TModule = OdRxModule>
class OdRxModuleSoftReference : protected OdStaticRxObject<OdRxDLinkerReactor> {
  TModule* m_pModule;
protected:
  /*!DOM*/
  void rxAppWillBeUnloaded( OdRxModule* pModule ) {
    if( pModule==m_pModule ) {
      clear();
    }
  }
public:
  /** \details
    Builds an instance of this object.
  */
  OdRxModuleSoftReference() : m_pModule(0) {}

  /** \details
    Destroy an instance of this object.
  */
  ~OdRxModuleSoftReference() { clear(); }

  /** \details
    Returns a smart pointer to the rx-module instance with the specified name.

    \param appName [in]  Name as a non-empty string value.
  */
  OdSmartPtr<TModule> getModule( const OdString& appName ) {
    OdSmartPtr<TModule> pRes = m_pModule;
    if( pRes.isNull() ) {
      pRes = ::odrxDynamicLinker()->loadApp( appName );
      ::odrxDynamicLinker()->addReactor( this );
      m_pModule = pRes.get();
    }
    return pRes;
  }

  /** \details
    Returns a raw pointer to the rx-module instance.
  */
  TModule* getRef() {
    return m_pModule;
  }

  /** \details
    Unload this module.
  */
  void clear() {
    if( m_pModule ) {
      m_pModule = 0;
      ::odrxDynamicLinker()->removeReactor( this );
    }
  }
};

#include "TD_PackPop.h"

#endif // _ODRXMODULESOFTREFERENCE_H_

