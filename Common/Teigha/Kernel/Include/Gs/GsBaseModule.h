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

#ifndef ODGSBASEMODULE_INC
#define ODGSBASEMODULE_INC

#include "TD_PackPush.h"
#include "Gs/GsExport.h"
#include "Gs/Gs.h"

class OdGsViewImpl;
class OdGsBaseVectorizeDevice;

/** \details
    This class provides management of bitmapped GUI display windows.

    Corresponding C++ library: Gs 
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseModule : public OdGsModule
{
  OdRxObjectPtrArray  m_reactors;
protected:
  /** \details
    Notifies connected reactors that the specified OdGsView object was created.
    
    \param pView [in]  Pointer to the OdGsView object.
    \remarks
    This function is called after the operation.
  */
  void fire_viewWasCreated(OdGsView* pView);

  /** \details
    Notifies connected reactors that the specified OdGsModule object is about to be unloaded.
    
    \param pModule [in]  Pointer to the GsModule object.
    \remarks
    This function is called before the operation.
  */
  void fire_gsToBeUnloaded(OdGsModule* pModule);

  /** \details
    Creates a new OdGsBaseVectorizeDevice object, and associates it with this BaseModule object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  virtual OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject() = 0;

  /** \details
    Creates a new OdGsViewImpl object, and associates it with this BaseModule object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  virtual OdSmartPtr<OdGsViewImpl> createViewObject() = 0;

  /** \details
    Creates a new OdGsBaseVectorizeDevice object, and associates it with this BaseModule object.
    \returns
    Returns a SmartPointer to the newly created object.
    \remarks
    This default implementation simply returns NULL which means that the option is not supported.
  */
  virtual OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject();

  /** \details
    Creates a new OdGsBaseVectorizeView object, and associates it with this BaseModule object.
    \returns
    Returns a SmartPointer to the newly created object.
    \remarks
    This default implementation simply returns NULL which means that the option is not supported.
  */
  virtual OdSmartPtr<OdGsViewImpl> createBitmapViewObject();

  OdGsBaseModule();
  
  /** \details
    Notification function called whenever this BaseModule object is about to be released.
  */
  void onFinalRelease();
public:
  ODRX_DECLARE_MEMBERS(OdGsBaseModule);

  /** \details
    Notifies connected reactors that the specified View object is about to be created.
    
    \param pView [in]  Pointer to the OdGsView object.
    \remarks
    This function is called before the operation.
  */
  void fire_viewToBeDestroyed(OdGsView* pView);

  /** \details
    Creates a new OdGsDevice object, and associates it with this BaseModule object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  OdGsDevicePtr createDevice();
  
  /** \details
    Creates a new OdGsBaseVectorizeDevice object, and associates it with this BaseModule object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  OdGsDevicePtr createBitmapDevice();

  /** \details
    Creates a new OdGsViewImpl object, and associates it with this BaseModule object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  OdSmartPtr<OdGsViewImpl> createView();

  /** \details
    Creates a new OdGsBaseVectorizeView object, and associates it with this BaseModule object.
    \returns
    Returns a SmartPointer to the newly created object.
  */
  OdSmartPtr<OdGsViewImpl> createBitmapView();

  /** \details
    Adds the specified reactor to this object's reactor list.
    \param pReactor [in]  Pointer to the reactor object.
  */
  void addReactor(OdGsReactor* pReactor);

  /** \details
    Removes the specified reactor from this object's reactor list.
    \param pReactor [in]  Pointer to the reactor object.
  */
  void removeReactor(OdGsReactor* pReactor);

  /** \details
    Notification function called at the start of the client application.
    \remarks
    The user override of this function should register any custom objects defined in the 
    custom application, using the OdRxObject::rxInit function.  It should also register
    custom commands defined in the module.
  */
  void initApp();

  /** \details
    Notification function called at the end of the client application.
    \remarks
    The user override of this function should unregister any custom objects defined in the
    custom application, using the OdRxObject::rxUninit function.  It should also
    remove any custom commands that were registered in the initApp function.
  */
  void uninitApp();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsBaseModule object pointers.
*/
typedef OdSmartPtr<OdGsBaseModule> OdGsBaseModulePtr;

/** \details
    This function must be called before using any GS implementation classes.
    \remarks
    OdGsBaseModule::initApp() calls this function, so there is no need to call it explicitly
    when using GS classes within an OdRx Module derived from OdGsBaseModule.

    Corresponding C++ library: Gs
*/
GS_TOOLKIT_EXPORT void odgsInitialize();

/** \details
    This function must be called after using any GS implementation classes.
    \remarks
    OdGsBaseModule::uninitApp() calls this function, so there is no need to call it explicitly
    when using OdGs classes within an OdRx Module derived from OdGsBaseModule.

    Corresponding C++ library: Gs
*/
GS_TOOLKIT_EXPORT void odgsUninitialize();

#include "TD_PackPop.h"

#endif // ODGSBASEMODULE_INC
