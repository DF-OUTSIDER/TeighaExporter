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

#ifndef _DWFOBJECTITERATOR_H_
#define _DWFOBJECTITERATOR_H_

#include "DwfObject.h"

class DWFDBEXPORT OdDwfObjectIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDwfObjectIterator);

  OdDwfObjectIterator() {}

  /** Description:
    Sets this Iterator object to reference the *entity* that it would normally return first.
     
    Remarks:
    Allows multiple traversals of the iterator list.

    Arguments:
    atBeginning (I) True to start at the beginning, false to start at the end. 
    skipErased (I) If and only if true, *erased* records are skipped.
  */  
  virtual void start(
    bool atBeginning = true, 
    bool skipErased = true) = 0;

  /** Description:
    Returns true if and only if the traversal by this Iterator object is complete.
  */  
  virtual bool done() const = 0;

  /** Description:
    Returns the Object ID of the *entity* currently referenced by this Iterator object.
  */
  virtual OdDwfObjectId item() const = 0;

  /** Description:
    Opens and returns the *entity* currently referenced by this Iterator object.

    Arguments:
    openMode (I) Mode in which to open the *entity*.
    openErasedEntity (I) If and only if true, *erased* records will be opened or retrieved.

    Remarks:
    Returns a SmartPointer to the opened *entity* if successful, otherwise a null SmartPointer.
  */
  //virtual OdDbElementPtr element(
  //OdDb::OpenMode openMode = OdDb::kForRead, 
  //bool openErasedEntity = false) = 0;

  /** Description:
    Steps this Iterator object.

    Arguments:
    forward (I) True to step *forward*, false to step backward.
    skipErased (I) If and only if true, *erased* records are skipped.
  */
  virtual void step(
    bool forward = true, 
    bool skipErased = true) = 0;

  /** Description:
    Positions this Iterator object at the specified record.
    Arguments:
    objectId (I) Object ID of the *entity*.
    pEntity(I) Pointer to the *entity*.
  */  
  virtual bool seek(const OdDwfObjectId& objectId) = 0;
};

/** Description:
  This template class is a specialization of the OdSmartPtr class for OdDbObjectIterator object pointers.
*/
typedef OdSmartPtr<OdDwfObjectIterator> OdDwfObjectIteratorPtr;

#endif //_DWFOBJECTITERATOR_H_

