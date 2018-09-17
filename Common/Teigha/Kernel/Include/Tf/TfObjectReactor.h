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

#ifndef __TF_OBJECTREACTOR_H__
#define __TF_OBJECTREACTOR_H__

#include "TD_PackPush.h"

#include "RxObject.h"
#include "TfExport.h"

class OdTfObject;
class OdDbStub;

/** \details
    This class is the base class for custom classes that receive notification
    of OdTfObject events.

    \remarks
    The default implementations of all methods in this class do nothing but
   return.

    Corresponding C++ library: Od_Tf

    <group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfObjectReactor : public OdRxObject {
  ODRX_DECLARE_MEMBERS(OdTfObjectReactor);
public:
  /** \details
    Notification function called whenever the notifying element has had its
    clone() member function called.

    \param pElement [in]  Pointer to the notifying element.
    \param pNewElement [in]  Pointer to the element resulting from the copy.

    \remarks
    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void copied(const OdTfObject* pElement,
                      const OdTfObject* pNewElement) = 0;

  /** \details
    Notification function called whenever an element has been erased or
    unerased.

    \param pElement [in]  Pointer to the element that was erased/unerased.
    \param erasing [in]  True if and only if this element is being erased.

    \remarks
    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void erased(const OdTfObject* pElement, bool erasing = true) = 0;

  /** \details
    Notification function called just before an element is deleted from memory.

    \param pElement [in]  Pointer to the element that is being deleted.

    \remarks
    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void goodbye(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever an element is opened for modify
    OdDg::kForWrite.

    \param pElement [in]  Pointer to the element that is being opened.

    \remarks
    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void openedForModify(const OdTfObject* pElement) = 0;

  /** \details
      Notification function called whenever an element is opened
     OdDg::kForWrite, a function
      has been called that could modify the contents of this element, and this
     element is now being
      closed.

      \param pElement [in]  Pointer to the element that is being closed after
     being modified.

      \remarks
      The default implementation of this function does nothing. This function
     can be
      overridden in custom classes.
  */
  virtual void modified(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever an element derived from
    OdDgGraphicsElement is modified.

    \param pElement [in]  Pointer to the modified element.

    \remarks
    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.

    This function is called whenever the following has occurred.

    1)  The calling element is opened OdDg::kForWrite.

    2)  One of its member functions either

        a Calls its assertWriteEnabled with recordModified == true.

        b Calls its recordGraphicsModified(true).

    3) The calling element is being closed.

    This function is called whenever the element as been modified. It
    therefore
    indicates only that the graphics for it may have changed.
  */
  virtual void modifiedGraphics(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever the attribute linkages has been
    written
    to the notifying element.

    \param pElement [in]  Pointer to the notifying element.

    \remarks
    Notification is made upon pElement->close().

    The notifying element is open OdDg::kForRead.

    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void modifiedLinkages(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever a method of the
    specified subelement has called assertWriteEnabled()

    \param pElement [in]  Pointer to the notifying element.
    \param pSubElm [in]  A pointer to the subelement owned by pElement.

    \remarks
    Notification is made upon pElement->close().
  */
  virtual void subElementModified(const OdTfObject* pElement,
                                  const OdTfObject* pSubElm) = 0;

  /** \details
    Notification function called whenever the notifying
    element is in the midst an Undo operation that
    is undoing modifications.

    \param pElement [in]  Pointer to the notifying element.

    \remarks
    The notifying element is open OdDg::kForRead.

    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void modifyUndone(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever the Undo process
    processes the appending of the notifying element to the database.

    \param pElement [in]  Pointer to the notifying element.

    \remarks
    The notifying element is marked as erased. It is not removed the
    database, and can be unerased,
    even 'before' it was created.

    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void unappended(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called whenever a Redo process
    processes the reappending of the notifying element to the database.

    \param pElement [in]  Pointer to the notifying element.

    \remarks

    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void reappended(const OdTfObject* pElement) = 0;

  /** \details
    Notification function called immediately before an element is closed.

    \param elementId [in]  Element ID of the element that is being closed.

    \remarks
    The default implementation of this function does nothing. This function can
    be
    overridden in custom classes.
  */
  virtual void closed(OdDbStub* elementId) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for
   OdTfObjectReactor object pointers.
*/
typedef OdSmartPtr<OdTfObjectReactor> OdTfObjectReactorPtr;

/** \details
    This template class is a specialization of the OdArray class for
   OdTfObjectReactor object SmartPointers.
*/
typedef OdArray<OdTfObjectReactor*> OdTfObjectReactorArray;

/** <group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfPersistentReactor : public OdTfObjectReactor {
  ODRX_DECLARE_MEMBERS(OdTfPersistentReactor);
public:
  /** \details
    Allows a subelement of a complex element to notify its root element that it
    has been changed.

    \param pSubElm [in]  Pointer to the modified subelement.

    \remarks
    Here's how it's supposed to work:

    1)  The subelement class's close() calls its triggers a "modified"
    notification which
        calls its xmitPropagateModify().
    2)  The subelement class calls the its parent's recvPropagateModify() its
    element's pointer.
    3)  The owner's class's recvPropagateModify() sends a "modified"
    notification to the top of its class.

    The default implementation of this function does nothing. This function can
    be overridden in custom classes.
  */
  virtual void recvPropagateModify(const OdTfObject* pSubElm) = 0;

  /** \details
    Allows a subelement of a complex element to notify its root element that it
    has been changed.

    \remarks
    The default implementation of this function inform the database
    in which the owner resides has been modified; triggering reactors
    attached to it. This function can be overridden in custom classes.
  */
  virtual void xmitPropagateModify() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for
   OdTfPersistentReactor object pointers.
*/
typedef OdSmartPtr<OdTfPersistentReactor> OdTfPersistentReactorPtr;

/** \details
    This template class is a specialization of the OdArray class for
   OdTfPersistentReactor object SmartPointers.
*/
typedef OdArray<OdTfPersistentReactor*> OdTfPersistentReactorArray;

#include "TD_PackPop.h"

#endif // __TF_OBJECTREACTOR_H__
