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

#ifndef _ODASMARTPOINTER_INCLUDED_
#define _ODASMARTPOINTER_INCLUDED_

#include "TD_PackPush.h"
#include "Error_NotThatKindOfClass.h"
#include "BaseObjectPtr.h"

/** \details
    <group Other_Classes>

    This template class provides the typified implementation of smart pointers for instances of classes derived from the OdRxObject class.

    \remarks
    SmartPointers relieve the programmer of having to determine when objects are no longer needed, or having to delete them
    them at that time.

    Each object referenced by a SmartPointer (henceforth 'referenced object') 
    maintains a reference count; i.e., how many SmartPointers are referencing it.
    When the reference count reaches zero, the referenced object is deleted.

    \sa
    TD_Db

    \sa
    <link smart_pointers_example_refexchange.html, Example of Assigning Smart Pointers>

    <link smart_pointers_example_typified.html, Example of a Typified Smart Pointer>

    \sa
    <link smart_pointers_overview.html, Overview of Smart Pointer Classes> 

    The OdRxObjectPtr class
*/
template <class T> class OdSmartPtr : public OdBaseObjectPtr
{
protected:
  /** \details
    Increments the reference counter of the referenced object.
  */
  void internalAddRef()
  {
    if(m_pObject)
      m_pObject->addRef();
  }
  
  /** \details
    Assigns the pointer of specified object to the smart pointer.  
      
    \remarks
    The reference counter of the specified object is incremented.

    If this SmartPointer is currently referencing another object, that object 
    is released prior to the assignment.  

    \param pObject [in]  Pointer to the specified object to be assigned.
  */
  void assign(const T* pObject)
  {
    if (m_pObject != pObject) // Prevent deleting object if nRefs=1
    {
      release();
      m_pObject = const_cast<T*>(pObject);
      internalAddRef();
    }
  }
  
  /** \details
    Performs a "safe" assignment of the specified object to this SmartPointer object. 
      
    \param pObject [in]  Pointer to the object to be assigned.

    Throws:
    eNotThatKindOfClass if not successful. 
  */
  void internalQueryX(const OdRxObject* pObject)
  {
    if(pObject)
    {
      OdRxObject* pX = pObject->queryX(T::desc());
      if(pX)
        m_pObject = pX;
      else
        throw OdError_NotThatKindOfClass(pObject->isA(), T::desc());
    }
  }
  
  void assign(const OdRxObject* pObject)
  {
    release();
    internalQueryX(pObject);
  }
  
  // Note: Using of SmartPtr<T> as bool expression produce ambiguous call with some compilers.
  // Use isNull() method instead.

  /** \details
    Declared private to prevent use.
    \remarks
    Use of SmartPtr<T> as a bool expression produces ambiguous calls with some compilers. Use isNull() instead. 
  */
  bool operator !() const { ODA_FAIL(); return false; }
  
  /** \details
    Declared private to prevent use.
    \remarks
    Use of SmartPtr<T> as a bool expression produces ambiguous calls with some compilers. Use isNull() instead. 
  */
  operator bool() const { ODA_FAIL(); return false; }

  /** \details
    Declared private to prevent use.
    \remarks
    Use of SmartPtr<T> as a bool expression produces ambiguous calls with some compilers. Use isNull() instead. 
  */
  operator bool() { ODA_FAIL(); return false; }
  
public:
  /** \details
    Constructors for the typified smart pointer.

    \param pObject [in]  Pointer to the object to be assigned to the new SmartPointer object.

    \remarks
    If pObject is specified, the smart pointer is set to the address of the specified object. If pObject is not specified, the smart pointer is set to a NULL.
    
    \remarks
    If OdRxObjMod or const OdBaseObjectPtr& are specified, the reference counter of the specified object 
    is not incremented. 
  */
  OdSmartPtr()
    : OdBaseObjectPtr()
  {
  }

  OdSmartPtr(const T* pObject, OdRxObjMod)
    : OdBaseObjectPtr(pObject)
  {
  }
  
  OdSmartPtr(const T* pObject)
    : OdBaseObjectPtr(pObject)
  {
    internalAddRef();
  }
  
  OdSmartPtr(const OdRxObject* pObject)
    : OdBaseObjectPtr()
  {
    internalQueryX(pObject);
  }

  OdSmartPtr(OdRxObject* pObject, OdRxObjMod)
    : OdBaseObjectPtr()
  {
    internalQueryX(pObject);
    if(pObject)
      pObject->release();
  }
  
  OdSmartPtr(const OdSmartPtr& pObject)
    : OdBaseObjectPtr(pObject.get())
  {
    internalAddRef();
  }

  OdSmartPtr(const OdRxObjectPtr& pObject)
    : OdBaseObjectPtr()
  {
    internalQueryX(pObject.get());
  }

  OdSmartPtr(const OdBaseObjectPtr& pObject)
    : OdBaseObjectPtr()
  {
    internalQueryX(pObject.get());
  }

  /** \details
    Assigns the pointer of the specifed object to the smart pointer without incrementing its reference counter.  
      
    \param pObject [in]  Pointer to the specifed object to be assigned.

    \remarks
    The reference counter of the specified object is not incremented.

    If the smart pointer is currently referencing to another object, that object 
    is released prior to the assignment.

    \sa
    <link smart_pointers_example_typified.html, Example of a Typified Smart Pointer>
  */
  void attach(const T* pObject)
  {
    release();
    m_pObject = const_cast<T*>(pObject);
  }
  
  /** \details
    Assigns the pointer of the specifed object to the smart pointer without incrementing its reference counter.  
      
    \param pObject [in]  Pointer to the specifed object to be assigned.

    \remarks
    The reference counter of the specified object is not incremented.

    If the smart pointer is currently referencing to another object, that object 
    is released prior to the assignment.

    \sa
    <link smart_pointers_example_typified.html, Example of a Typified Smart Pointer>
  */
  void attach(OdRxObject* pObject)
  {
    release();
    internalQueryX(pObject);
    if(pObject)
      pObject->release();
  }
  
  /** \details
    Destructor for the typified smart pointer.
 
    \remarks
    Decrements the reference counter of the referenced object. When the reference count reaches zero, the referenced object is deleted.
  */
  ~OdSmartPtr()
  {
    release();
  }
  
  /** \details
    Releases the smart pointer reference to the referenced object. 
    
    \remarks
    Decrements the reference counter of the referenced object.  When the reference count reaches zero, 
    the referenced object is deleted. Smart pointer is set to a NULL.

    \sa
    <link smart_pointers_example_typified.html, Example of a Typified Smart Pointer>
  */
  void release()
  {
    if (m_pObject)
    {
      m_pObject->release();
      m_pObject = 0;
    }
  }
  
  /** \details
    Releases the smart pointer reference to the referenced object without decrementing of the reference counter. 
      
    \returns
    Returns the raw pointer to the referenced object. The reference counter is not modified 
    for the referenced object. Smart pointer is set to a NULL.
 
    \sa
    <link smart_pointers_example_typified.html, Example of a Typified Smart Pointer>
  */
  T* detach()
  {
    T* pRes = static_cast<T*>(m_pObject);
    m_pObject = 0;
    return pRes;
  }
  
  /** \remarks
    The assignment operator increments the reference counter of the referenced object and saves its address in the smart pointer.
     If the smart pointer is currently referencing to another object, that assignment operator releases the referenced object prior 
     to the assignment.  

    \sa
    <link smart_pointers_example_typified.html, Example of a Typified Smart Pointer>
  */
  OdSmartPtr& operator = (const OdSmartPtr& pObject)
  {
    assign(pObject);
    return *this;
  }
    
  OdSmartPtr& operator = (const OdBaseObjectPtr& pObject)
  {
    assign(pObject.get());
    return *this;
  }
  
  OdSmartPtr& operator = (const T* pObject)
  {
    assign(pObject);
    return *this;
  }

  /** \details
    Returns the raw pointer to the referenced object.

    \remarks
    The smart pointer maintains its reference to the referenced object. 
    The reference counter of the referenced object is not changed.
 
    \sa
    <link smart_pointers_example_typified.html, Example of a Typified Smart Pointer>
  */
  const T* get() const
  {
    return static_cast<const T*>(m_pObject);
  }
  
  /** \details
    Returns the raw pointer to the referenced object.

    \remarks
    The smart pointer maintains its reference to the referenced object. 
    The reference counter of the referenced object is not changed.
 
    \sa
    <link smart_pointers_example_typified.html, Example of a Typified Smart Pointer>
  */
  T* get()
  {
    return static_cast<T*>(m_pObject);
  }
  
  /** \details
    Returns the raw pointer to the referenced object.
      
    \remarks
    The reference counter of the referenced object is not changed.
  */
  T* operator ->()
  {
    return static_cast<T*>(m_pObject);
  }
  /** \details
    Returns the raw pointer to the referenced object.
      
    \remarks
    The reference counter of the referenced object is not changed.
  */
  const T* operator ->() const
  {
    return static_cast<const T*>(m_pObject);
  }
  
#ifdef ODA_GCC_2_95
  /** \details
    Returns the raw pointrer to the referenced object.
    
    \remarks
    The class maintains the reference to the referenced object.
    The reference counter of the referenced object is not changed.
  */
  operator T*() const
  {
    return const_cast<T*>(static_cast<const T*>(m_pObject));
  }
  
#else
  /** \details
    Returns the raw pointer the  referenced object.
    
    \remarks
    This SmartPointer maintains its reference to the referenced object.
    
    The reference counter of the referenced object is not changed.
  */
  operator T*()
  {
    return static_cast<T*>(m_pObject);
  }
  
  operator const T*() const
  {
    return static_cast<const T*>(m_pObject);
  }

#endif
    
  bool operator==(const void* pObject) const
  {
    return (m_pObject==pObject);
  }

  bool operator==(const OdSmartPtr& pObject) const
  {
    return operator==((void*)pObject.get());
  }
  
  bool operator!=(const void* pObject) const
  {
    return (m_pObject!=pObject);
  }

  bool operator!=(const OdSmartPtr& pObject) const
  {
    return operator!=((void*)pObject.get());
  }
};

#include "TD_PackPop.h"

#endif // _ODASMARTPOINTER_INCLUDED_
