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
#ifndef __DG_ELEMENTITERATOR_H__
#define __DG_ELEMENTITERATOR_H__

#include "DgExport.h"
#include "RxObject.h"
#include "DgElementId.h"

/** \details
    This class defines bidirectional Iterator objects that traverse elements contained in complex elements.

    This class cannot be directly instantiated, but must be instantiated with the
    iterator creation methods of the class to be iterated through.
  
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgElementIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgElementIterator);

  OdDgElementIterator() {}

  /** \details
    Sets this Iterator element to reference the entity that it would normally return first.
     
    \remarks
    Allows multiple traversals of the iterator list.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipErased [in]  If and only if true, erased records are skipped.
  */
    virtual void start(
    bool atBeginning = true, 
    bool skipErased = true) = 0;

  /** \details
    Returns true if and only if the traversal by this Iterator element is complete.
  */
    virtual bool done() const = 0;

  /** \details
    Returns the Element ID of the entity currently referenced by this Iterator element.
  */
  virtual OdDgElementId item() const = 0;

  /** \details
    Steps this Iterator element.

    \param forward [in]  True to step forward, false to step backward.
    \param skipErased [in]  If and only if true, erased records are skipped.
  */
  virtual void step(
    bool forward = true, 
    bool skipErased = true) = 0;

  /** \details
    Positions this Iterator element at the specified record.
    \param objectId [in]  Element ID of the entity.
  */
    virtual bool seek(
    const OdDgElementId& objectId) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDgElementIterator element pointers.
*/
typedef OdSmartPtr<OdDgElementIterator> OdDgElementIteratorPtr;

#endif //__DG_ELEMENTITERATOR_H__

