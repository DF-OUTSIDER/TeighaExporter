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




#ifndef _ODRXITERATOR_H_
#define _ODRXITERATOR_H_

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
  <group OdRx_Classes> 

  This class is the abstract base class for OdDbDictionaryIterator and OdRxDictionaryIterator objects.

  \sa
  TD_Db
  
   \sa
   <link iterator_overview.html, Overview of Classes that Implement an Iterator>
*/
class FIRSTDLL_EXPORT OdRxIterator : public OdRxObject
{
public:

  OdRxIterator() {}

  ODRX_DECLARE_MEMBERS(OdRxIterator);
  
  /** \details
    Returns true if and only if the traversal by this iterator object is completed.
  */
    virtual bool done() const = 0;

  /** \details
    Sets the iterator object to the next element of the container.

    \remarks
    Returns true if and only if not at the end of the list.
  */
  virtual bool next() = 0;

  /** \details
    Returns the non-typified smart pointer to the object on which the iterator object refers. 
  */
  virtual OdRxObjectPtr object() const = 0;
};

/** \details
  The typified smart pointer for the OdRxIterator object.
*/
typedef OdSmartPtr<OdRxIterator> OdRxIteratorPtr;

#include "TD_PackPop.h"

#endif // _ODRXITERATOR_H_

