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

#ifndef _RxProtocolReactor_h_Included_
#define _RxProtocolReactor_h_Included_

#include "RxObject.h"
#include "RxDictionary.h"

class OdRxProtocolReactor;
class OdRxProtocolReactorIterator;
class OdRxProtocolReactorList;
class OdRxProtocolReactorListIterator;
class OdRxProtocolReactorManager;
class OdRxProtocolReactorManagerFactory;


/** \details
    This class is the base class for all protocol reactors.

    \remarks
    All protocol reactor classes must derive from this base class.

    Corresponding C++ library: TD_Root
    <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxProtocolReactor : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxProtocolReactor);
};


/** \details
    This class implements Iterators for OdRxProtocolReactor objects in OdRxProtocolReactorList instances.

    \remarks
    Instances are returned by OdRxProtocolReactorList::newIterator().

    Corresponding C++ library: TD_Root
    <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxProtocolReactorIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxProtocolReactorIterator);

  /** \details
    Returns the reactor class type returned by this Iterator object.
  */
  virtual OdRxClass* reactorClass () const = 0;


  /** \details
    Sets this Iterator object to reference the Reactor object that it would normally return first.

    \remarks
    Allows multiple traversals of the Iterator list.
  */
    virtual void start() = 0; 

  /** \details
    Sets this Iterator object to reference the Reactor object following the current Reactor object.
    
    \returns
    Returns true if and only if not at the end of the list.
  */
    virtual bool next () = 0;

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done () const = 0;

  /** \details
    Returns the Reactor object referenced by this Iterator object.
  */
  virtual OdRxProtocolReactor* object () const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxProtocolReactorManager object pointers.
*/
typedef OdSmartPtr<OdRxProtocolReactorIterator> OdRxProtocolReactorIteratorPtr;


/** \details
  This class is a collection of OdRxProtocolReactor objects.  
  Corresponding C++ library: TD_Root
  <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxProtocolReactorList : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxProtocolReactorList);

  /** \details
    Returns the reactor class type returned the Iterator object.
  */
  virtual OdRxClass* reactorClass() const = 0;

  /** \details
    Adds the specified reactor to this object's reactor list.

    \param pReactor [in]  Pointer to the reactor object.
  */
  virtual bool addReactor(OdRxProtocolReactor* pReactor) = 0;

  /** \details
    Removes the specified reactor from this object's reactor list.

    \param pReactor [in]  Pointer to the reactor object.
  */
  virtual void removeReactor(OdRxProtocolReactor* pReactor) = 0;

  
  /** \details
    Creates an Iterator object that provides access to the objects in this List object.
    
    \returns
    Returns a SmartPointer to the Iterator object.
  */
  virtual OdRxProtocolReactorIteratorPtr newIterator () const = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdRxProtocolReactorList object pointers.
*/
typedef OdSmartPtr<OdRxProtocolReactorList> OdRxProtocolReactorListPtr;


/** \details
    This class implements iterators for OdRxProtocolReactorList objects in OdRxProtocolReactorManager instances.
    \remarks
    Instances are returned by OdRxProtocolReactorManager::newIterator().

    Corresponding C++ library: TD_Root
    <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxProtocolReactorListIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxProtocolReactorListIterator);

  /** \details
    Sets this Iterator object to reference the Reactor object that it would normally return first.

    \remarks
    Allows multiple traversals of the Iterator list.
  */
    virtual void start() = 0; 

  /** \details
    Sets this Iterator object to reference the Reactor object following the current Reactor object.
    
    \returns
    Returns true if and only if not at the end of the list.
  */
    virtual bool next () = 0;

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done () const = 0;

  /** \details
    Returns the OdRxProtocolReactorList object referenced by this Iterator object.
  */
  virtual OdRxProtocolReactorList* object () const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxProtocolReactorListIterator object pointers.
*/
typedef OdSmartPtr<OdRxProtocolReactorListIterator> OdRxProtocolReactorListIteratorPtr;


/** \details
  This class is a container class for OdRxProtocolReactorList objects.
  Corresponding C++ library: TD_Root
  <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxProtocolReactorManager: public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxProtocolReactorManager);

  
  /** \details
    Returns a pointer for the ProtocolReactorList for the specified reactor class.
    
    \param pReactorClass [in]  Reactor class.
    \remarks
    The ProtocolReactorList is created as necessary.
  */
  virtual OdRxProtocolReactorList* createReactorList (
    OdRxClass* pReactorClass) = 0;


  /** \details
    Creates an Iterator object that provides access to the OdRxProtocolReactorList objects in this Manager object.
    
    \returns
    Returns a SmartPointer to the Iterator object.
  */
  virtual OdRxProtocolReactorListIteratorPtr newIterator () const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxProtocolReactorManager object pointers.
*/
typedef OdSmartPtr<OdRxProtocolReactorManager> OdRxProtocolReactorManagerPtr;


/** \details
    This factory class obtains the single OdRxProtocolReactorManager instance associated
    with a specified OdRxClass.
     
    Corresponding C++ library: TD_Root
    <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxProtocolReactorManagerFactory : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxProtocolReactorManagerFactory);

  /** \details
    Returns the single OdRxProtocolReactorManager instance associated
    with the specified OdRxClass.
    
    \param pRxClass [in]  Pointer to the RcClass object.
    \remarks
    The OdRxProtocolReactorManager is created as necessary.
  */
    virtual OdRxProtocolReactorManager* createReactorManager (
    OdRxClass* pRxClass) const = 0;
};

/** \details
  Returns the global OdRxProtocolReactorManagerFactory instance. 
*/
#define odrxProtocolReactors \
  OdRxProtocolReactorManagerFactory::cast(odrxServiceDictionary()-> \
  at(ODRX_PROTOCOL_REACTOR_MANAGER))

/** \details
  Returns the OdRxProtocolReactorManager associated with the 
  specified odrxClass.
*/
#define ODRX_PROTOCOL_REACTOR_MANAGER_AT(odrxClass) \
  odrxProtocolReactors->createReactorManager(odrxClass)

/** \details
  Returns the OdRxProtocolReactorList associated with the 
  specified odrxClass.  The returned list contains zero or more 
  reactorClass instances.
*/
#define ODRX_PROTOCOL_REACTOR_LIST_AT(odrxClass, reactorClass) \
  ODRX_PROTOCOL_REACTOR_MANAGER_AT(odrxClass)->createReactorList(reactorClass)

#endif // _RxProtocolReactor_h_Included_
