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


#ifndef _OD_TF_Included_
#define _OD_TF_Included_

#include "DbHandle.h"
#include "DbObjectId.h"
#include "DbDatabase.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeScale3d.h"

#include "TD_PackPush.h"

#ifdef TF_DLL_EXPORTS
#define TFDLL_EXPORT           OD_TOOLKIT_EXPORT
#define TFDLL_EXPORT_STATIC    OD_STATIC_EXPORT
#else
#define TFDLL_EXPORT           OD_TOOLKIT_IMPORT
#define TFDLL_EXPORT_STATIC    OD_STATIC_IMPORT
#endif

struct OdTfFiler
{
  virtual OdRxObject* database() const = 0;

  virtual void seek(OdInt64 offset, OdDb::FilerSeekType seekType) = 0;

  /** Description:
    Returns the current I/O pointer position of this Filer object.
  */
  virtual OdUInt64 tell() const = 0;

  /** Description:
    Returns the bool *value* of the current item of this Filer object.
  */
  virtual bool rdBool() = 0;

  /** Description:
    Returns the *string* *value* of the current item of this Filer object.
    Arguments:
  */
  virtual OdString rdString() = 0;
  
  /** Description:
    Returns the specified number of *bytes* from this Filer object.
    Arguments:
    buffer (O) Receives the bytes.
    numBytes (I) Number of *bytes*.
  */
  virtual void rdBytes(
    void* buffer, 
    OdUInt32 numBytes) = 0;

  /** Description:
    Returns the Int8 *value* of the current item of this Filer object.
  */
  virtual OdInt8 rdInt8() = 0;
  
  /** Description:
    Returns the UInt8 *value* of the current item of this Filer object.
  */
  virtual OdUInt8 rdUInt8() = 0;

  /** Description:
    Returns the Int16 *value* of the current item of this Filer object.
  */
  virtual OdInt16 rdInt16() = 0;

  /** Description:
    Returns the Int32 *value* of the current item of this Filer object.
  */
  virtual OdInt32 rdInt32() = 0;

  /** Description:
    Returns the Int64 *value* of the current item of this Filer object.
  */
  virtual OdInt64 rdInt64() = 0;

  /** Description:
    Returns the double *value* of the current item of this Filer object.
  */
  virtual double rdDouble() = 0;

  /** Description:
    Returns the *handle* *value* of the current item of this Filer object.
  */
  virtual OdDbHandle rdDbHandle() = 0;

  /** Description:
    Returns the SoftOwnershipId *value* of the current item of this Filer object.
  */
  virtual OdDbStub* rdSoftOwnershipId() = 0;

  /** Description:
    Returns the HardOwnershipId *value* of the current item of this Filer object.
  */
  virtual OdDbStub* rdHardOwnershipId() = 0;

  /** Description:
    Returns the HardPointerId *value* of the current item of this Filer object.
  */
  virtual OdDbStub* rdHardPointerId() = 0;

  /** Description:
    Returns the SoftPointerId *value* of the current item of this Filer object.
  */
  virtual OdDbStub* rdSoftPointerId() = 0;

  /** Description:
    Returns the 2D *point* *value* of the current item of this Filer object.
  */
  virtual OdGePoint2d rdPoint2d() = 0;
    
  /** Description:
    Returns the 3D *point* *value* of the current item of this Filer object.
  */
  virtual OdGePoint3d rdPoint3d() = 0;

  /** Description:
    Returns the 2D *vector* *value* of the current item of this Filer object.
  */
  virtual OdGeVector2d rdVector2d() = 0;

  /** Description:
    Returns the 3D vector *value* of the current item of this Filer object.
  */
  virtual OdGeVector3d rdVector3d() = 0;
  
  /** Description:
    Returns the *scale* *value* of the current item of this Filer object.
  */
  virtual OdGeScale3d rdScale3d() = 0;

  /** Description:
    Writes the specified bool *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrBool(
    bool value) = 0;

  /** Description:
    Writes the specified *string* *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrString(
    const OdString &value) = 0;
    
  /** Description:
    Writes the specified number of *bytes* to this Filer object.
    Arguments:
    buffer (I) array of *bytes*.
    numBytes (I) Number of *bytes*.
  */
  virtual void wrBytes(
    const void* buffer, 
    OdUInt32 numBytes) = 0;

  /** Description:
    Writes the specified Int8 *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrInt8(
    OdInt8 value) = 0;
    
  /** Description:
    Writes the specified UInt8 *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrUInt8(
    OdUInt8 value) = 0;

  /** Description:
    Writes the specified Int16 *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrInt16(
    OdInt16 value) = 0;
    
  /** Description:
    Writes the specified Int32 *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrInt32(
    OdInt32 value) = 0;

  /** Description:
    Writes the specified Int64 *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrInt64(
    OdInt64 value) = 0;

   
  /** Description:
    Writes the specified double *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrDouble(double value) = 0;

  /** Description:
    Writes the specified *handle* *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrDbHandle(
    const OdDbHandle& value) = 0;

  /** Description:
    Writes the specified SoftOwnershipId *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrSoftOwnershipId(
    OdDbStub* value) = 0;
    
  /** Description:
    Writes the specified HardOwnershipId *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrHardOwnershipId(
    OdDbStub* value) = 0;

  /** Description:
    Writes the specified SoftPointerId *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrSoftPointerId(
    OdDbStub* value) = 0;

  /** Description:
    Writes the specified HardPointerId *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrHardPointerId(
    OdDbStub* value) = 0;

  /** Description:
    Writes the specified 2D *point* *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrPoint2d(
    const OdGePoint2d& value) = 0;

  /** Description:
    Writes the specified 3D *point* *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrPoint3d(
    const OdGePoint3d& value) = 0;
  
  /** Description:
    Writes the specified 2D *vector* *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrVector2d(
    const OdGeVector2d& value) = 0;
  
  /** Description:
    Writes the specified 3D *vector* *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrVector3d(
    const OdGeVector3d& value) = 0;
  
  /** Description:
    Writes the specified 3D *scale* *value* to this Filer object.
    Arguments:
    value (I) Value.
  */  
  virtual void wrScale3d(
    const OdGeScale3d& value) = 0;

  virtual void wrSubobject(int id, const char* name) = 0;
  virtual const char* rdSubobject(int& id) = 0;
  virtual void wrDateTime(OdTimeStamp) = 0;
  virtual void rdDateTime(OdTimeStamp&) = 0;
};


struct TFDLL_EXPORT OdTfObjectPE : OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdTfObjectPE);
  virtual int schemaVersion(OdRxObject*) = 0;
  virtual OdString schema(OdRxObject*) = 0;
  virtual bool in(OdRxObject*, OdTfFiler*) = 0;
  virtual void out(OdRxObject*, OdTfFiler*) = 0;
};

typedef OdSmartPtr<OdTfObjectPE> OdTfObjectPEPtr;

namespace OdTf
{
  enum TagsEnum {
    Element = 1,
    Condition = 2,
    Array = 4,
    version = 8,
    EndClass = 16,
    Mark = 32,

    EndTagMask = 64
  };

  enum ClassVersionEnum
  {
    originalVersion = 0,
    addInVersion = 1,
    deletedInVersion = 2
  };

  enum ElementAttributesEnum {
    isCacheValue = 1,
    isBreak = 2,
    isTableName = 4,
    isContextData = 8,
    isTableQueue = 16,
    isTableCache = 32,
    TableQueue = 64,
    isSum = 128
  };

  enum ConditionAttributesEnum {
    isMarkCondition = 1,
    isOwner = 2,
    dwgVer = 4
  };

  enum ArrayAttributesEnum {
    breakValue = 1,
    increaseOn = 2,
    decreaseOn = 3,
  };

  enum ConditionOperatorsEnum {
    kTrue = 1,
    kFalse = 2,
    kNot = 4,
    kAnd = 8,
    kLessThan = 16,
    kGreaterThan = 32
  };

  enum HexBinaryAttributesEnum {
    increaseIn = 1,
    reduceIn = 2,
    limitLen = 4,
    countBytes = 8,
    mul = 16,
    value = 32,
    cacheValue = 64
  };

  enum TypesEnum
  {
    Subclass,
    Bool,
    Byte,
    Short,
    Int,
    UnsignedByte,
    UnsignedShort,
    UnsignedInt,
    Long,
    Double,
    HexBinary,
    String,
    Handle,
    SoftOwnershipId,
    HardOwnershipId,
    SoftPointerId,
    HardPointerId,
    Point2d,
    Point3d,
    Vector2d,
    Vector3d,
    Scale3d,
    DateTime,
    DataEnd,
    Table,
    Empty,
    DoubleWithDefault
  };
}


#include "TD_PackPop.h"

#endif // _OD_TF_Included_
