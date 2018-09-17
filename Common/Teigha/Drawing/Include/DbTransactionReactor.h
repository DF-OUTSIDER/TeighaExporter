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




#ifndef _ODDBTRANSACTIONREACTOR_INCLUDED_
#define _ODDBTRANSACTIONREACTOR_INCLUDED_

#include "TD_PackPush.h"

class OdDbDatabase;

#include "RxObject.h"


/** \details
    This class is the base class for custom classes that receive notification
    of OdDbTransaction events.

    \remarks
    The default implementations of all methods in this class do nothing but return.

    \sa
    TD_Db
        
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbTransactionReactor : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbTransactionReactor);
  
  OdDbTransactionReactor() {}
  /** \details
    Notification function called to indicate a new transaction is about to start.
    
    \param pDb [in]  Pointer to the database sending the notification.
    
    \sa
    * transactionAborted
    * transactionAboutToAbort
    * transactionAboutToEnd
    * transactionEnded
    * transactionStarted
  */
  virtual void transactionAboutToStart(
    OdDbDatabase* pDb);
  
  /** \details
    Notification function called to indicate a new transaction has started.
    
    \param pDb [in]  Pointer to the database sending the notification.
    
    \sa
    * transactionAborted
    * transactionAboutToAbort
    * transactionAboutToEnd
    * transactionAboutToStart
    * transactionEnded
  */
  virtual void transactionStarted(
    OdDbDatabase* pDb);

  /** \details
    Notification function called to indicate a transaction is about to end.
    
    \param pDb [in]  Pointer to the database sending the notification.
    
    \sa
    * transactionAborted
    * transactionAboutToAbort
    * transactionAboutToStart
    * transactionEnded
    * transactionStarted
  */
  virtual void transactionAboutToEnd(
    OdDbDatabase* pDb);

  /** \details
    Notification function called to indicate a transaction has ended.
    
    \param pDb [in]  Pointer to the database sending the notification.
    
    \sa
    * transactionAborted
    * transactionAboutToAbort
    * transactionAboutToEnd
    * transactionAboutToStart
    * transactionStarted
  */
  virtual void transactionEnded(
    OdDbDatabase* pDb);

  /** \details
    Notification function called to indicate a transaction is about to be terminated.
    
    \param pDb [in]  Pointer to the database sending the notification.
    
    \sa
    * transactionAborted
    * transactionAboutToEnd
    * transactionAboutToStart
    * transactionEnded
    * transactionStarted
  */
  virtual void transactionAboutToAbort(
    OdDbDatabase* pDb);

  /** \details
    Notification function called to indicate a transaction has been terminated.
    
    \param pDb [in]  Pointer to the database sending the notification.
    
    \sa
    * transactionAboutToAbort
    * transactionAboutToEnd
    * transactionAboutToStart
    * transactionEnded
    * transactionStarted
  */
  virtual void transactionAborted(
    OdDbDatabase* pDb);

  /** \details
    Notification function called to indicate the outermost transaction has ended.
    
    \param pDb [in]  Pointer to the database sending the notification.
  */
  virtual void endCalledOnOutermostTransaction(
    OdDbDatabase* pDb);

  /** \details
    Notification function called when the transaction resident object is involved in a swapIdWith() operation.
    
    \param pTransObj [in]  Pointer to the first transaction resident object.
    \param pOtherTransObj [in]  Pointer to the second transaction resident object.
    \param pDb [in]  Pointer to the database sending the notification.

    \remarks
    According to database transactions implementation the both objects are transaction-resident or not.
    So this function will be called only once in OdDbObject::swapIdWith() method.
  */
  virtual void objectIdSwapped(
    const OdDbObject* pTransObj,
    const OdDbObject* pOtherTransObj,
    OdDbDatabase* pDb);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbTransactionReactor object pointers.
*/
typedef OdSmartPtr<OdDbTransactionReactor> OdDbTransactionReactorPtr;

#include "TD_PackPop.h"

#endif // _ODDBTRANSACTIONREACTOR_INCLUDED_
