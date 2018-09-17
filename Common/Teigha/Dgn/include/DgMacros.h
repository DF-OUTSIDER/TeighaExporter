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
#ifndef __DG_MACROS_H__
#define __DG_MACROS_H__

#include "RxObject.h"

#define ODDG_DECLARE_MEMBERS(ClassName)          \
public:                                          \
  ODRX_DECLARE_MEMBERS(ClassName)


#define ODDG_DECLARE_MEMBERS_INTERNAL(ClassName)                 \
protected:                                                       \
  ClassName();                                                   \
  ClassName(OdDgElementImpl* pImpl);                             \
private:                                                         \
  static const OdDgElement::ElementTypes m_elementType;          \
  static const OdUInt8 m_elementSubType;                         \
public:                                                          \
  virtual OdDgElement::ElementTypes getElementType() const;      \
  virtual OdUInt8 getElementSubType() const;                     \
public:                                                          \
  ODRX_DECLARE_MEMBERS(ClassName)

//this macro was created when Borland C++ refuses some situations when default OD_TYPENAME (empty for BCB configuration) did not effect
#if defined(__BCPLUSPLUS__) || defined(__BORLANDC__)
#define OD_TYPENAME4 typename
#else
#define OD_TYPENAME4 OD_TYPENAME
#endif

#endif //__DG_MACROS_H__
