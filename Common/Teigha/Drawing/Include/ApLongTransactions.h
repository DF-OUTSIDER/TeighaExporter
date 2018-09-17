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


#ifndef   _LONG_TRANSACTION_MANAGER_H_INCLUDED_
#define   _LONG_TRANSACTION_MANAGER_H_INCLUDED_

#include "RxObject.h"
#include "IdArrays.h"
#include "TD_PackPush.h"

class OdDbLongTransaction;
class OdDbDatabase;
class OdDbIdMapping;

/** \details
    This class is used to access callbacks for long transactions.  

    \sa
    TD_Db

	<group OdAp_Classes>
*/
class TOOLKIT_EXPORT OdApLongTransactionReactor: public OdRxObject 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdApLongTransactionReactor);
  OdApLongTransactionReactor();

   /** \details
    Begins the checkout process of the specified long transaction.

	\param originList [in]  List/array of objects (in the same block table record) to check out.
   */
  virtual void beginCheckOut        (OdDbLongTransaction& , OdDbObjectIdArray& originList);
 /** \details
    Completes the checkout process and returns the long transaction.
 */
  virtual void endCheckOut          (OdDbLongTransaction& );
   /** \details
    Begins the checkin process of the specified long transaction.
   */
  virtual void beginCheckIn         (OdDbLongTransaction& );
   /** \details
    Completes the checkin process of the specified long transaction.
   */
  virtual void endCheckIn           (OdDbLongTransaction& );
   /** \details
    Terminates the check out process and returns the aborted long transaction.
   */
  virtual void abortLongTransaction (OdDbLongTransaction& );

  /** \details
    Terminates the check out process, if in progress.
  */
protected:
  OdResult veto();

private:
  bool m_Veto;
  friend class OdApLongTransactionManagerImpl;
};

/** \details
    This class controls long transactions. 

    \remarks
    There is one OdApLongTransactionManager object per application session.  
    
    \sa
    TD_Db

	<group OdAp_Classes> 
*/
class ODRX_ABSTRACT TOOLKIT_EXPORT OdApLongTransactionManager : public OdRxObject 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdApLongTransactionManager);

  /** \details
    Generates a new long transaction and clones objects from the specified list.
    
    \param transID [out]  Receives the object ID of the new long transaction.
	\param objList [in]  List/array of objects (in the same block table record) to check out.
	\param toBlock [in]  Object ID of the block table record to which the objects will be cloned.
	\param errorMap [out]  Receives an empty mapping object.
	\param lockBlkRef [in]  Optional object ID of the last block reference, which during checkout will
	will be placed on a locked layer.
  */
  virtual OdResult checkOut(OdDbObjectId& transId, OdDbObjectIdArray& objList,
    OdDbObjectId toBlock, OdDbIdMapping& errorMap, OdDbObjectId lockBlkRef = OdDbObjectId::kNull) = 0;

  /** \details
    Checks in a long transaction that was previously checked out.
    
    \param transID [in]  Object ID of the long transaction.
	\param errorMap [out]  Receives an empty mapping object.
	\param keepObjs [in]  Boolean value indicating whether to keep workset objects.
  */
  virtual OdResult checkIn(OdDbObjectId transId, OdDbIdMapping& errorMap, bool keepObjs = false) = 0;

  /** \details
    Terminates a long transaction during or in between checkin and checkout.
    
    \param transID [in]  Object ID of the long transaction.
	\param keepObjs [in]  Boolean value indicating whether to keep cloned objects.
  */
  virtual OdResult abortLongTransaction(OdDbObjectId transId, bool keepObjs = false) = 0;

  virtual OdDbObjectId currentLongTransactionFor(const OdDbDatabase* pDb) const = 0;
  /** \details
    Adds the specified reactor to this object's reactor list.
  */
  virtual void addReactor(OdApLongTransactionReactor*) = 0;
  /** \details
    Deletes the specified reactor from this object's reactor list.
  */
  virtual void removeReactor(OdApLongTransactionReactor*) = 0;

  /** \details
    Adds the class of the specified OdRxClass object to the class filter.
  */
  virtual OdResult addClassFilter (OdRxClass* ) = 0;
  /** \details
    Returns True if the class of the specified OdRxClass object is in the class filter.
  */
  virtual bool isFiltered (OdRxClass* ) const = 0;
};

extern TOOLKIT_EXPORT OdApLongTransactionManager* odapLongTransactionManager();

#include "TD_PackPop.h"
#endif  // _LONG_TRANSACTION_MANAGER_H_INCLUDED_
