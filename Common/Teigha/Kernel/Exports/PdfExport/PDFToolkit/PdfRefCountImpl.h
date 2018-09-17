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




#if !defined(_PDFREFCOUNTIMPL_INCLUDED_)
#define _PDFREFCOUNTIMPL_INCLUDED_

#include "OdError.h"

namespace TD_PDF
{

template<class T>
class PDFEXPORT_TOOLKIT PDFRefCountImpl : public T
{
  /** \details
      Allows to create PDFRefCountImpl<T> instances inside this class only.
  */
  OdUInt32 m_nRefCounter;

  /** \details
      Assignment operator prohibited.
  */
  PDFRefCountImpl& operator = (const PDFRefCountImpl&) { throw OdError(eNotApplicable); return *this; }
  PDFRefCountImpl(const PDFRefCountImpl&){throw OdError(eNotApplicable);}

protected:
  /** \details
      Constructor.  Sets the reference count to 0.
  */
  PDFRefCountImpl() : m_nRefCounter(0) {}
public:

  /** \details
      Increments the reference count.
  */
  void addRef()
  {
    ++m_nRefCounter;
  }

  /** \details
      Decrements the reference count, and deletes this object if the reference count
      becomes zero.
  */
  void release()
  {
		ODA_ASSERT((m_nRefCounter > 0));
		if (!(--m_nRefCounter))
		{
      delete this;
		}
  }

  long numRefs() const { return m_nRefCounter; }
};

};

#endif // !defined(_PDFREFCOUNTIMPL_INCLUDED_)

