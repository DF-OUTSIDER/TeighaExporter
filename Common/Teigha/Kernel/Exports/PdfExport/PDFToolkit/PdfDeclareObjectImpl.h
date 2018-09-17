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




#ifndef _PDFDECLAREOBJECTIMPL_INCLUDED_
#define _PDFDECLAREOBJECTIMPL_INCLUDED_ /*!DOM*// 

#define PDF_DECLARE_OBJECT_IMPL_NO_DEF_CONST(className)\
PDFSmartPtr<className> className::createObject(PDFDocument &pDoc, bool isIndirect)\
{                                                                                 \
  className *pAr = NULL;                                                          \
                                                                                  \
  if (isIndirect)                                                                 \
    pAr = new PDFIndirectLinkedObj< className >;                                  \
  else                                                                            \
    pAr = new PDFDirectLinkedObj< className >;\
\
  className##Ptr pObj( pAr );\
  if (pObj.isNull())\
    throw OdError(eOutOfMemory);\
  \
  if (isIndirect)\
    pDoc.AddObject(pObj);\
  else\
    pObj->setDocument(&pDoc);\
  \
  pObj->InitObject();\
  \
  return pObj;\
}

#define PDF_DECLARE_BASE_OBJECT_IMPL_NO_DEF_CONST(className)\
PDFSmartPtr<className> className::createObject()\
{\
  className##Ptr pObj( new className );\
  if (pObj.isNull())\
    throw OdError(eOutOfMemory);\
  \
  return pObj;\
}

#define PDF_DECLARE_OBJECT_NCR_IMPL(className)\
className::className()\
{\
}

#define PDF_DECLARE_OBJECT_IMPL(className)\
PDF_DECLARE_OBJECT_NCR_IMPL(className)\
PDF_DECLARE_OBJECT_IMPL_NO_DEF_CONST(className)

#define PDF_DECLARE_BASE_OBJECT_IMPL(className)\
PDF_DECLARE_OBJECT_NCR_IMPL(className)\
PDF_DECLARE_BASE_OBJECT_IMPL_NO_DEF_CONST(className)

#endif //_PDFDECLAREOBJECTIMPL_INCLUDED_

