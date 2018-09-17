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




#ifndef _PDFSMARTPOINTER_INCLUDED_
#define _PDFSMARTPOINTER_INCLUDED_

namespace TD_PDF
{

class PDFBaseObject;

class PDFBaseObjectPtr
{
protected:
  PDFBaseObject *m_pObject;
  PDFBaseObjectPtr() : m_pObject(0) {}

public:
  PDFBaseObject* get() const {return m_pObject;}
  bool isNull() const { return m_pObject == 0; }
};

template <class T> class PDFSmartPtr : public PDFBaseObjectPtr
{
  void internalAddRef() { if(m_pObject) { ((T*)m_pObject)->addRef(); } }
  
  void assign(
    const T* pObject)
  {
    release();
    m_pObject = (PDFBaseObject*)pObject;
    internalAddRef();
  }
  
  void internalQueryX(
    const PDFBaseObject* pObject)
  {
    if(pObject)
    {
      m_pObject = (PDFBaseObject*)pObject;
      m_pObject->addRef();
    }
  }
  
  void assign(const PDFBaseObject* pObject)
  {
    release();
    internalQueryX(pObject);
  }
  
  bool operator !() const { ODA_FAIL(); return false; }
  
  operator bool() const { ODA_FAIL(); return false; }

  operator bool() { ODA_FAIL(); return false; }
  
public:
  PDFSmartPtr() { }
  
  PDFSmartPtr(
    const T* pSource) {m_pObject = (PDFBaseObject*)pSource; internalAddRef(); }
  
  PDFSmartPtr(
    const PDFBaseObject* pSource) { internalQueryX(pSource); }
  
  PDFSmartPtr(
    const PDFSmartPtr& pSource) {m_pObject = (PDFBaseObject*)pSource.get(); internalAddRef(); }

  PDFSmartPtr(
    const PDFBaseObjectPtr& pSource) { internalQueryX(pSource.get()); }

 void attach(
    const T* pObject) { release(); m_pObject = (PDFBaseObject*)pObject; }
  void attach(
    PDFBaseObject* pObject)
  {
    release();
    internalQueryX(pObject);
    if(pObject)
      pObject->release();
  }
  
  ~PDFSmartPtr() { release(); }
  
  void release()
  {
    if (m_pObject)
    {
      ((T*)m_pObject)->release();
      m_pObject = NULL;
    }
  }
  
  T* detach()
  {
    T* res = (T*)m_pObject;
    m_pObject = NULL;
    return res;
  }
  
  PDFSmartPtr& operator = (
    const PDFSmartPtr& pObject)
  { assign(pObject); return *this; }
    
  PDFSmartPtr& operator = (
    const PDFBaseObjectPtr& pObject)
  { assign(pObject.get()); return *this; }
  
  PDFSmartPtr& operator = (
    const T* pObject)
  { assign(pObject); return *this; }
  
  const T* get() const { return (T*)m_pObject; }
  
  T* get() { return (T*)m_pObject; }
  
  T* operator ->() { return (T*)m_pObject; }
  
  const T* operator ->() const { return (T*)m_pObject; }
  
#ifdef ODA_GCC_2_95
  /** \remarks
    This non-standard form is used to eliminate a large number of compiler
    warnings produced by GCC 2.95.X (GCC 3.X no longer produces these warnings).
  */
  operator T*() const { return (T*)m_pObject; }
  
#else
  operator T*() { return (T*)m_pObject; }
  
  operator const T*() const { return (T*)m_pObject; }

#endif
    
  bool operator==(
    const void* pObject) const { return (m_pObject==pObject); }

  bool operator==(
    const PDFSmartPtr& pObject) const { return operator==((void*)pObject.get()); }
  
  bool operator!=(
    const void* pObject) const { return (m_pObject!=pObject); }

  bool operator!=(
    const PDFSmartPtr& pObject) const { return operator!=((void*)pObject.get()); }

  bool Export(PDFIStream *pStream, const PDFVersion &ver) const
  {
    if (!m_pObject)
      return false;

    T* pT = (T*)m_pObject;
    if (pT->isIndirect())
    {
      pT->ExportLikeRef(pStream, ver);
    }
    else
    {
      return pT->Export(pStream, ver);
    }
    return true;
  }
};

};

#endif //_PDFSMARTPOINTER_INCLUDED_

