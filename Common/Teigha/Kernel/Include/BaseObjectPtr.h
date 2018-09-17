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

#ifndef _ODABASEOBJECTPOINTER_INCLUDED_
#define _ODABASEOBJECTPOINTER_INCLUDED_

#include "TD_PackPush.h"

class OdRxObject;

/** \details
    <group Other_Classes>

    This class is the base class for typified smart pointers.

    \sa
    TD_Db
    
    \remarks
    Smart pointers relieve the programmer of having to determine when objects are no longer needed, or having to delete
    them at that time.

    Each object referenced by a SmartPointer (henceforth 'referenced object') 
    maintains a reference count; i.e., how many SmartPointers are referencing it.
    When the reference count reaches zero, the referenced object is deleted.

    \sa
    <link smart_pointers_overview.html, Overview of Smart Pointer Classes> 
*/
class FIRSTDLL_EXPORT OdBaseObjectPtr
{
protected:
  OdRxObject* m_pObject;
  
  OdBaseObjectPtr() : m_pObject(0) {}
  explicit OdBaseObjectPtr(const OdRxObject *pSource) : m_pObject(const_cast<OdRxObject*>(pSource)) {}
public:
  /** \details
    Returns the raw pointer to the referenced object. 
    
    \remarks
    This class maintains the reference to the referenced object. 
    The reference counter of the referenced object is not changed.
  */
  OdRxObject* get() const { return m_pObject; }
  
  /** \details
     Returns true if and only if the smart pointer stores a NULL.
  */
  bool isNull() const { return m_pObject == 0; }
};

#include "TD_PackPop.h"

#endif // _ODABASEOBJECTPOINTER_INCLUDED_
