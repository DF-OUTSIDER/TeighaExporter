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

#ifndef DbAssocNetworkLongTransactionReactor_INCLUDED_
#define DbAssocNetworkLongTransactionReactor_INCLUDED_ /*!DOM*/

#include "ApLongTransactions.h"
#include "DbObjectReactor.h"
#include "StaticRxObject.h"

#define STL_USING_MAP
#include "OdaSTL.h"

/** \details
  This class implements the object modification reactor.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocNetworkObjectModificationReactor : public OdDbObjectReactor
{
public:

  ODRX_DECLARE_MEMBERS(OdDbAssocNetworkObjectModificationReactor);

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdDbAssocNetworkObjectModificationReactor();

  /** \details
    Adds the reference to this reactor.
  */
  virtual void addRef() {};

  /** \details
    Releases this reactor.
  */
  virtual void release() {};

  /** \details
    Notifies the reactor when the object is erased.
  */
  void erased(const OdDbObject* pObject, bool erasing);

  /** \details
    Notifies the reactor when the object is modified.
  */
  void modified(const OdDbObject* pObject);

  /** \details
    Appends an object to the workset.
  */
  void appendToWorksetObject(OdDbObjectId objId);

  /** \details
    Gets the modified objects of the workset.
  */
  void getModifiedWorksetObjects(OdDbObjectIdArray & aResArray);

  /** \details
    Checks whether the workset is empty.
  */
  bool isEmpty();

private:
  /*!DOM*/
  OdDbObjectIdArray m_aWorksetObjReactorAttachedTo;
  /*!DOM*/
  OdDbObjectIdArray m_aModifiedWorksetObj;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocNetworkObjectModificationReactor class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocNetworkObjectModificationReactor> OdDbAssocNetworkObjectModificationReactorPtr;


/** \details
  This class implements the transaction reactor.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocNetworkLongTransactionReactor: public OdStaticRxObject<OdApLongTransactionReactor>
{
public:

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocNetworkLongTransactionReactor();

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdDbAssocNetworkLongTransactionReactor();

  /** \details
    Adds the reference to this reactor.
  */
  void addRefReactor();

  /** \details
    Releases this reactor.
  */
  void releaseReactor();

  // OdApLongTransactionReactor methods

  /** \details
    Begins the checking out.
  */
  void beginCheckOut(OdDbLongTransaction& iTransaction, OdDbObjectIdArray& iOriginList);

  /** \details
    Ends the checking out.
  */
  void endCheckOut(OdDbLongTransaction& iTransaction);

  /** \details
    Begins the checking in.
  */
  void beginCheckIn(OdDbLongTransaction& iTransaction);

  /** \details
    Ends the checking in.
  */
  void endCheckIn(OdDbLongTransaction& iTransaction);

  /** \details
    Aborts the transaction.
  */
  void abortLongTransaction(OdDbLongTransaction& iTransaction);

private:
  /*!DOM*/
  void showDimensions(OdDbObjectId blockId, bool show);

  /*!DOM*/
  OdDbAssocNetworkObjectModificationReactorPtr m_pModificationReactor;

  /*!DOM*/
  OdDbObjectIdArray m_aOriginEntities;

  /*!DOM*/
  OdDbObjectIdArray m_aModifiedWorksetEntities;

  /*!DOM*/
  std::map<OdDbObjectId, OdDbObjectId> m_WorksetObjectToOriginalMap;

  /*!DOM*/
  typedef std::map<OdDbObjectId, OdDbObjectIdArray> OdDbAssocOriginalObjectReactorsMap;
  OdDbAssocOriginalObjectReactorsMap m_OriginalObjectReactorsMap;

  /*!DOM*/
  int m_Counter;
};

#endif
