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




#ifndef _PDFDECLAREOBJECT_INCLUDED_
#define _PDFDECLAREOBJECT_INCLUDED_ /*!DOM*// 

#define DECLARE_PDF_ABSTRACT_RTTI() \
  public:\
  virtual bool isKindOf (PDFTypeId objType) const = 0;\
  virtual PDFTypeId type() const = 0;\
  private:

#define DEFINE_PDF_RTTI(Class,Parent,TYPE) \
  public:\
  virtual bool isKindOf(PDFTypeId objType) const\
  {\
    if ( objType == TYPE ) return true;\
    return Parent::isKindOf( objType );\
  }\
  virtual PDFTypeId type() const { return TYPE; }\
  private:

#define PDF_DECLARE_NCR_OBJECT_BASE(Class,Parent,TYPE)\
  DEFINE_PDF_RTTI(Class,Parent,TYPE)\
private:\
  Class(const Class&);\
  Class& operator = (const Class&);\
protected:\
  Class();\
private:

#define PDF_DECLARE_NCR_OBJECT(Class,Parent,TYPE)\
  PDF_DECLARE_NCR_OBJECT_BASE(Class,Parent,TYPE)\
  private:\
  static PDFSmartPtr<Class> createObject(PDFDocument &pDoc, bool isIndirect = false);

#define PDF_DECLARE_OBJECT(Class,Parent,TYPE)\
  PDF_DECLARE_NCR_OBJECT_BASE(Class,Parent,TYPE)\
public:\
  static PDFSmartPtr<Class> createObject(PDFDocument &pDoc, bool isIndirect = false);\
private:

#define PDF_DECLARE_BASE_OBJECT(Class,Parent,TYPE)\
  PDF_DECLARE_NCR_OBJECT_BASE(Class,Parent,TYPE)\
public:\
  static PDFSmartPtr<Class> createObject();\
private:

#endif //_PDFDECLAREOBJECT_INCLUDED_

