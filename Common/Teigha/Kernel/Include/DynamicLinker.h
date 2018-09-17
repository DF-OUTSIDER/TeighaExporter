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




// DynamicLinker.h: interface for the OdaDynamicLinkerI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ODADYNAMICLINKERI_H_INCLUDED_)
#define _ODADYNAMICLINKERI_H_INCLUDED_

class OdRxDictionary;
class OdRxDLinkerReactor;

#include "RxDictionary.h"
#include "RxModule.h"

class OdRxModule;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxModule pointers.
*/
typedef OdSmartPtr<OdRxModule> OdRxModulePtr;

class OdRxSystemServices;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxSystemServices pointers.
*/
typedef OdSmartPtr<OdRxSystemServices> OdRxSystemServicesPtr;

#include "TD_PackPush.h"

/** \details
    This class implements Dynamic Linker services for Teigha Xtension(TX) applications:
    
    The following services are implemented:
    * Loading applications by specified by application name.
    * Loading and unloading modules specified by filename.
    * Addition and removal of reactors from the dynamic linker reactor chain.
    
    \remarks
    There is exactly one OdRxDynamicLinker class object. It is gloval to Teigha,
    and thus is non-instantiable for Teigha applications.
    
    A pointer to the OdRxDynamicLinker object will be returned by the
    global odrxDynamicLinker() function.

    <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxDynamicLinker : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxDynamicLinker);

  /** \details
    Adds the specified reactor to the dynamic linker reactor chain.
    \param pReactor [in]  Pointer to the reactor.
  */
  virtual void addReactor (
    OdRxDLinkerReactor* pReactor) = 0;
  /** \details
    Removes the specified reactor to the dynamic linker reactor chain.
    \param pReactor [in]  Pointer to the reactor.
  */
  virtual void removeReactor (
    OdRxDLinkerReactor* pReactor) = 0;

  /** \details
    Loads the specified abstract (platform-independent) module.
    
    \returns
    Returns a SmartPointer to the abstract module object.
    
    \param moduleFileName [in]  Module filename to load.
    \param silent [in]  If true, no load status message will be printed.
  */
  virtual OdRxModulePtr loadModule(
    const OdString& moduleFileName, bool silent = true) = 0;

  /** \details
    Unoads the specified abstract (platform-independent) module.
    
    \param moduleFileName [in]  Module filename to unload.
  */
  virtual bool unloadModule(
    const OdString& moduleFileName) = 0;


  /** \details
    Unoads all unreferenced modules.
  */
  virtual bool unloadUnreferenced() = 0;

  /** \details
    Maps the specfied application name to the a module filename,
    and loads that module.

    \param applicationName [in]  TX application name.
    \param silent [in]  If and only if true, no load status message will be printed.
  */
  virtual OdRxModulePtr loadApp(
    const OdString& applicationName, 
    bool silent = true) = 0;

  /** \details
    Returns a SmartPointer to the OdDbSystemServices instance that is used for file creation and access.
    
    \sa
    odInitialize
  */
  virtual OdRxSystemServicesPtr sysServices() const = 0;

  /** \details
    Returns a SmartPointer to the OdRxDictionary instance that created by OdInitialize.
  */
  virtual OdRxDictionaryPtr sysRegistry() const = 0;

  /*!DOM*/  
  /** \details
    Special internal method - used for non-standard module loading only
  */
  virtual OdRxModule* loadModuleObj(
    OdRxModule* pModuleObj, bool bSilent = true) = 0;

  /** \details
    Returns the count of loaded modules.
  */
  virtual int getModuleCount() const = 0;

  /** \details
    Returns the specified abstract (platform-independent) module.
  */
  virtual OdRxModulePtr getModule(
    int index,
    OdString& key) const = 0;

  /** \details
    Returns the specified abstract (platform-independent) module.
  */
  virtual OdRxModulePtr getModule(
    const OdString& key) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxDynamicLinker pointers.
*/
typedef OdSmartPtr<OdRxDynamicLinker> OdRxDynamicLinkerPtr;


/** \details
    Returns the Teigha global dynamic linker.
*/
FIRSTDLL_EXPORT OdRxDynamicLinker* odrxDynamicLinker();

/** \details
    This function creates a new instance of the class specified by the argument as a string
    and returns a /typified smart pointer/ to it. Always returns non-null pointer (or throws exception).

    \param sClassName [in]  Custom class name as a string.
    \param sAppName [in,optional] RX application name to load if class is not registred.
*/
template <class TClass>
OdSmartPtr<TClass> odrxSafeCreateObject(const OdString& sClassName, const OdString& sAppName = OdString::kEmpty) {
  OdRxDictionaryPtr clsdict = odrxClassDictionary();
  OdRxClass* pClass = (OdRxClass*)clsdict->getAt(sClassName).get();
  if(pClass)
    return (TClass*)pClass->create().get();
  if(!sAppName.isEmpty()) {
    odrxDynamicLinker()->loadApp(sAppName, false);
    pClass = (OdRxClass*)clsdict->getAt(sClassName).get();
    if(pClass)
      return (TClass*)pClass->create().get();
  }
  throw OdError(sClassName +" class was not found");
}


inline OdRxModulePtr odrxLoadApp(const OdString& applicationName) { return odrxDynamicLinker()->loadApp(applicationName); }

inline OdRxModulePtr odrxSafeLoadApp(const OdString& applicationName) { return odrxDynamicLinker()->loadApp(applicationName, false); }

#include "TD_PackPop.h"

#endif // !defined(_ODADYNAMICLINKERI_H_INCLUDED_)

