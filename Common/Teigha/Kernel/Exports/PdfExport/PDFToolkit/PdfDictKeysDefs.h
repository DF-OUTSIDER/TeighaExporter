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



#define _PDF_DICT_KEYS_STANDARD_INT32_(KEYNAME) \
  \
  void set##KEYNAME(OdInt32 nVal) \
  { \
    AddInt32((#KEYNAME), nVal); \
  } \
  \
  \
  OdInt32 get##KEYNAME() const \
  { \
    PDFIntegerPtr pN = Find( (#KEYNAME) );\
    if (pN.isNull())\
      PDF_EXCEPTION(eKeyNotFound);\
    return pN->Int32(); \
  }

#define _PDF_DICT_KEYS_STANDARD_BOOL_(KEYNAME) \
  \
  void set##KEYNAME(bool bVal) \
  { \
    AddBool((#KEYNAME), bVal); \
  } \
  \
  \
  bool get##KEYNAME() const \
  { \
    PDFBooleanPtr pB = Find( (#KEYNAME) );\
    if (pB.isNull())\
      PDF_EXCEPTION(eKeyNotFound);\
    return pB->Bool(); \
  }

#define _PDF_DICT_KEYS_STANDARD_A_(KEYNAME, KEYTYPE) \
  \
  void set##KEYNAME(KEYTYPE##Ptr pVal) \
  { \
    AddItem((#KEYNAME), pVal); \
  } \
  \
  \
  KEYTYPE##Ptr get##KEYNAME() const \
  { \
    KEYTYPE##Ptr pI = Find( (#KEYNAME) ); \
    if (pI.isNull()) \
      PDF_EXCEPTION(eKeyNotFound);\
    return pI;\
  }

#define _PDF_DICT_KEYS_STANDARD_BF_(KEYNAME, KEYTYPE) \
  \
  void set##KEYNAME(KEYTYPE##Ptr pVal) \
  { \
    AddItem((#KEYNAME), pVal); \
  } \
  \
  \
  KEYTYPE##Ptr get##KEYNAME() \
  { \
    KEYTYPE##Ptr pI = Find( (#KEYNAME) ); \
    if (pI.isNull()) \
    { \
      pI = KEYTYPE::createObject(*document(), false); \
      AddItemNoSearch(#KEYNAME, pI); \
    } \
    return pI;\
  }

#define _PDF_DICT_KEYS_BF_(KEYNAME, KEYTYPE) _PDF_DICT_KEYS_STANDARD_BF_(KEYNAME, KEYTYPE)
#define _PDF_DICT_KEYS_A_(KEYNAME, KEYTYPE) _PDF_DICT_KEYS_STANDARD_A_(KEYNAME, KEYTYPE)
#define _PDF_DICT_KEYS_BOOL_(KEYNAME) _PDF_DICT_KEYS_STANDARD_BOOL_(KEYNAME)
#define _PDF_DICT_KEYS_INT32_(KEYNAME) _PDF_DICT_KEYS_STANDARD_INT32_(KEYNAME)
