/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


#ifndef _ODDBDWGCLASSDEF_H_INCLUDED_
#define _ODDBDWGCLASSDEF_H_INCLUDED_



TOOLKIT_EXPORT OdRxClass* oddbDwgClassMapDesc(OdUInt16 nDwgType);


enum {
# define __DUMMY_( a,b,c,d,e,f,g,h,i )
# define CLASSDEF(DWGTYPE,ClassName, BaseClassName,DwgVer,MaintVer,DxfName,AppName,ProxyFlags,CustomFlags)\
  kDwgId_OdDb##ClassName = DWGTYPE,
# include "DwgClassDef.h"
# undef CLASSDEF
# undef CLASSmic
# undef CLASSdim
# undef CLASSext
# undef __DUMMY_
  kOdDbDwgClassMapSize,
  kDwgId_OdDbProxyObject  = kOdDbDwgClassMapSize,
  kDwgId_OdDbProxyEntity,
  kDwgId_OdDbVertex,
  kDwgId_OdDbDimension,
  kOdDbDwgClassMapSize_WithProxy
};

#define DWGMAP_DECLARE_MEMBERS(ClassName)                                                            \
protected:                                                                                           \
  ClassName(OdDbObjectImpl* pImpl);                                                                  \
public:                                                                                              \
                                                                                                     \
  /** Casts the specified pointer to an ClassName SmartPointer. **/                                  \
  static OdSmartPtr<ClassName> cast(const OdRxObject* pObj) {                                        \
    if (pObj)                                                                                        \
      return OdSmartPtr<ClassName>(((ClassName*)pObj->queryX(ClassName::desc())), kOdRxObjAttach);   \
    return (ClassName*)0;                                                                            \
  }                                                                                                  \
                                                                                                     \
  /** Returns the static OdRxClass description object associated with this object. **/               \
  /** This function is for use only when the class type of this object is known.   **/               \
  /** If the class type of this object is unknown, use instead isA().              **/               \
  static OdRxClass* desc() { return ::oddbDwgClassMapDesc(kDwgId_##ClassName); }                     \
                                                                                                     \
  /** Returns the OdRxClass description instance associated with this object.      **/               \
  /** This function is for use only when the class type of this object is unknown. **/               \
  /** If the class type of this object is known, use instead desc().               **/               \
  virtual OdRxClass* isA() const;                                                                    \
                                                                                                     \
  /** Returns the Protocol Extension object for this object.                       **/               \
  /** Return null if there is no Protocol Extension object is found.               **/               \
  virtual OdRxObject* queryX(const OdRxClass* protocolClass) const;                                  \
                                                                                                     \
  /** Creates a new instance of this object type.                                  **/               \
  /** Returns a SmartPointer to the new instance.                                  **/               \
  static OdRxObjectPtr pseudoConstructor();                                                          \
                                                                                                     \
  /** Creates a new instance of this object type.                                  **/               \
  /** Returns a SmartPointer to the new instance.                                  **/               \
  static OdSmartPtr<ClassName> createObject()                                                        \
  { if (desc()) return desc()->create(); throw OdError("OdDb" #ClassName "is not loaded"); }         \
                                                                                                     \
  /** Registers ClassName with Teigha.                                             **/               \
  static void rxInit();                                                                              \
  static void rxInit(AppNameChangeFuncPtr appNameChangeFunc);                                        \
                                                                                                     \
/* Unregisters ClassName with Teigha.                                              **/               \
  static void rxUninit()




#endif // _ODDBDWGCLASSDEF_H_INCLUDED_
