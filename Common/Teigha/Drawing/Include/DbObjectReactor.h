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




#ifndef _DB_OBJECT_REACTOR_INCLUDED_
#define _DB_OBJECT_REACTOR_INCLUDED_

#include "TD_PackPush.h"

#include "RxObject.h"
#include "OdArrayPreDef.h"

/** \details
    This class is the base class for custom classes that receive notification
    of OdDbObject events.

    \remarks
    The default implementations of all methods in this class do nothing but return.

    \sa
    TD_Db
        
    <group OdDb_Classes>
*/
class ODRX_ABSTRACT TOOLKIT_EXPORT OdDbObjectReactor : public OdRxObject
{
protected:
  OdDbObjectReactor() {}
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectReactor);
  
  /** \details
    Not called.
  */
    virtual void cancelled(
    const OdDbObject* pObject);

  /** \details
    Notification function called whenever the notifying object has had its clone() member function called. 
    
    \param pObject [in]  Pointer to the notifying object.
    \param pNewObject [in]  Pointer to the object resulting from the copy.

    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void copied(
    const OdDbObject* pObject, 
    const OdDbObject* pNewObject);

  /** \details
    Notification function called whenever an object has been erased or unerased.

    \param pObject [in]  Pointer to the object that was erased/unerased.
    \param erasing [in]  True if and only if this object is being erased.
    
    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void erased(
    const OdDbObject* pObject, 
    bool erasing = true);

  /** \details
    Notification function called just before an object is deleted from memory. 
    
    \param pObject [in]  Pointer to the object that is being deleted.

    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void goodbye(
    const OdDbObject* pObject);

  /** \details
    Notification function called whenever an object is opened for modify OdDb::kForWrite.

    \param pObject [in]  Pointer to the object that is being opened.

    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void openedForModify(
    const OdDbObject* pObject);

  /** \details
      Notification function called whenever an object is opened OdDb::kForWrite, a function
      has been called that could modify the contents of this object, and this object is now being
      closed.

      \param pObject [in]  Pointer to the object that is being closed after being modified.

      \remarks
      The default implementation of this function does nothing.  This function can be
      overridden in custom classes.
  */
  virtual void modified(
    const OdDbObject* pObject);

  /** \details
    Notification function called whenever an object derived from OdDbEntity is modified.
    
    \param pObject [in]  Pointer to the modified object.
    
    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
    
    This function is called whenever the following has occurred.
    
    1)  The calling object is opened OdDb::kForWrite.
    
    2)  One of its member functions either
    
        a Calls its assertWriteEnabled with recordModified == true.
        
        b Calls its recordGraphicsModified(true).
    
    3) The calling object is being closed.
    
    This function is called whenever the object as been modified. It therefore
    indicates only that the graphics for it may have changed.
            
  */
  virtual void modifiedGraphics(
    const OdDbObject* pObject);

  /** \details
    Notification function called whenever a method of the
    specified subobject has called assertWriteEnabled()
    
    \param pObject [in]  Pointer to the notifying object.
    \param pSubObj [in]  A pointer to the subobject owned by pObject.
    
    \remarks
    Notification is made upon pObject->close() or pObject->cancel(). 
 
    \remarks
    This function is called only for the modification of the following:
       
    Vertices of OdDb2dPolylines, OdDb3dPolylines, OdDbPolygonMeshs, and OdDbPolyFaceMeshes
    OdDbFaceRecords of OdDbPolyFaceMeshs 
    OdDbAttributes owned by OdDbBlockReferences, classes derived from OdDbBlockReference, and OdDbMInsertBlocks
  */
  virtual void subObjModified(
    const OdDbObject* pObject, 
    const OdDbObject* pSubObj);
  /** \details
    Notification function called whenever the notifying
    object is in the midst an Undo operation that
    is undoing modifications.
    
    \param pObject [in]  Pointer to the notifying object.
    
    \remarks
    The notifying object is open OdDb::kForRead.
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void modifyUndone(
    const OdDbObject* pObject);

  /** \details
    Notification function called whenever the XData has been written
    to the notifying object.
    
    \param pObject [in]  Pointer to the notifying object.
    
    \remarks
    Notification is made upon pObject->close() or pObject->cancel(). 

    The notifying object is open OdDb::kForRead.
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void modifiedXData(
    const OdDbObject* pObject);

  /** \details
    Notification function called whenever the Undo process
    processes the appending of the notifying object to the database.
    
    \param pObject [in]  Pointer to the notifying object.
    
    \remarks
    The notifying object is marked as erased. It is not removed the database, and can be unerased,
    even 'before' it was created. 
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void unappended(
    const OdDbObject* pObject);

  /** \details
    Notification function called whenever a Redo process
    processes the reappending of the notifying object to the database.
    
    \param pObject [in]  Pointer to the notifying object.
    
    \remarks
    
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void reappended(
    const OdDbObject* pObject);

  /** \details
    Notification function called immediately before an object is closed.

    \param objectId [in]  Object ID of the object that is being closed.

    \remarks
    The default implementation of this function does nothing.  This function can be
    overridden in custom classes.
  */
  virtual void objectClosed(
    const OdDbObjectId& objectId);

  /** \details
    Returns the first transient reactor of the specified class
    that is attached to the specified object.

    \param pObject [in]  Pointer to the notifying object.
    \param pKeyClass [in]  Pointer to the class desciption.
  */
  static OdDbObjectReactor* findReactor(
    const OdDbObject* pObject, 
    const OdRxClass* pKeyClass);
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbObjectReactor object pointers.
*/
typedef OdSmartPtr<OdDbObjectReactor> OdDbObjectReactorPtr;
/** \details
    This template class is a specialization of the OdArray class for OdDbObjectReactor object SmartPointers.
*/
typedef OdArray<OdDbObjectReactorPtr> OdDbObjectReactorArray;

#include "TD_PackPop.h"

#endif // _DB_OBJECT_REACTOR_INCLUDED_
