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




#if !defined(_ODVARIANT_H_INCLUDED_)
#define _ODVARIANT_H_INCLUDED_

#include "TD_PackPush.h"
#include "OdPlatformSettings.h"
#include "RxObject.h"

#include "StringArray.h"
#include "BoolArray.h"
#include "Int8Array.h"
#include "Int16Array.h"
#include "Int32Array.h"
#include "Int64Array.h"
#include "UInt8Array.h"
#include "UInt16Array.h"
#include "UInt32Array.h"
#include "UInt64Array.h"
#include "DoubleArray.h"
#include "OdString.h"
#include "OdAnsiString.h"

/** \details
    This class implements OdError objects with Result codes of eInvalidVariantType.
    
    Corresponding C++ library: TD_Root
    <group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdError_InvalidVariantType : public OdError
{
public:
  OdError_InvalidVariantType();
};

const int nOdVariantDataSize = 8;

/** \details
    This class implements Variant objects within Teigha.
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdVariant
{
protected:
  int m_type;
  union   // Union is required to have proper data alignment which is important
  {       // for some platforms
    OdUInt8 uint8[nOdVariantDataSize];
    double  dbl;
    void* ptr;
  } m_uData;
  void* data() const {return (void*)m_uData.uint8;}

  virtual void setVarType(int newType, int& type, void* data);
public:
  struct TypeFactory
  {
    virtual void construct(void* pData) const = 0;
    virtual void destroy(void* pData) const = 0;
  };
  static const TypeFactory* typeFactory(int type);
public:
  typedef enum
  {
    kVoid         = 0x0000,
    kString       = 0x0001,
    kBool         = 0x0002,
    kInt8         = 0x0003,
    kUInt8        = 0x0003,
    kInt16        = 0x0004,
    kUInt16       = 0x0004,
    kInt32        = 0x0005,
    kUInt32       = 0x0005,
    kInt64        = 0x0006,
    kUInt64       = 0x0006,
    kDouble       = 0x0007,
    kWString      = 0x0008,
    kAnsiString   = 0x0008,
    kRxObjectPtr  = 0x0009,
    kNextType     = 0x000A,
    kByRef        = 0x0040,
    kArray        = 0x0080
  } Type;

  OdVariant();
  OdVariant(const OdVariant& val);
  OdVariant& operator =(const OdVariant& val);
  virtual ~OdVariant();

  /** \details
    Returns the type of this Variant object, including ByRef and Array flags.
  */
  int  varType() const { return m_type; }
  /** \details
    Returns the type of this Variant object, excluding ByRef and Array flags.
  */
  Type type()    const { return Type(m_type & (0x3F)); }
  /** \details
    Returns true if and only if this Variant object is an Array.
  */
  bool isArray() const { return (m_type & kArray)!=0; }
  /** \details
    Returns true if and only if this Variant object is ByRef (read/write).
  */
  bool isByRef() const { return (m_type & kByRef)!=0; }

  OdVariant(const OdString& val);
  OdVariant(const OdAnsiString& val);
  OdVariant(const OdRxObjectPtr& val);
  OdVariant(bool val);
  OdVariant(OdInt8 val);
  OdVariant(OdUInt8 val);
  OdVariant(OdInt16 val);
  OdVariant(OdUInt16 val);
  OdVariant(OdInt32 val);
  OdVariant(OdUInt32 val);
  OdVariant(OdInt64 val);
  OdVariant(OdUInt64 val);
  OdVariant(double val);
#ifdef OD_INTPTR_UNIQUE
  OdVariant(OdIntPtr val);
#endif

  /** \details
    Returns a reference to the value of this Variant object as an OdString object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdString& getString() const;

  /** \details
  Returns a reference to the value of this Variant object as an OdAnsiString object.
  \remarks
  No check is made to determine that this Variant object is of the specified type.
  */
  const OdAnsiString& getAnsiString() const;

  /** \details
    Returns a reference to the value of this Variant object as an OdRxObjectPtr object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdRxObjectPtr& getRxObjectPtr() const;

  /** \details
    Returns the value of this Variant object as a bool object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  bool getBool()   const;

  /** \details
    Returns the value of this Variant object as an OdInt8 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt8 getInt8()   const;

  /** \details
    Returns the value of this Variant object as an OdUInt8 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt8 getUInt8()  const;

  /** \details
    Returns the value of this Variant object as an OdInt16 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt16 getInt16()  const;

  /** \details
    Returns the value of this Variant object as an OdUInt16 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt16 getUInt16() const;

  /** \details
    Returns the value of this Variant object as an OdInt32 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt32 getInt32()  const;

  /** \details
    Returns the value of this Variant object as an OdUInt32 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt32 getUInt32() const;

  /** \details
    Returns the value of this Variant object as an OdInt64 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt64 getInt64()  const;

  /** \details
    Returns the value of this Variant object as an OdUInt64 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt64 getUInt64() const;

  /** \details
    Returns the value of this Variant object as an OdIntPtr object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdIntPtr getIntPtr() const;

  /** \details
    Returns the value of this Variant object as an double object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  double getDouble() const;


  /** \details
    Returns the value of this Variant object as a pointer to an OdString object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdString* getStringPtr() const;

  /** \details
  Returns the value of this Variant object as a pointer to an OdAnsiString object.
  \remarks
  No check is made to determine that this Variant object is of the specified type.
  */
  OdAnsiString* getAnsiStringPtr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdRxObjectPtr object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdRxObjectPtr* getRxObjectPtrPtr() const;

  /** \details
    Returns the value of this Variant object as a pointer to a bool object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  bool* getBoolPtr()   const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdInt8 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt8* getInt8Ptr()   const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdUInt8 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt8* getUInt8Ptr()  const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdInt16 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt16* getInt16Ptr()  const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdUInt16 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt16* getUInt16Ptr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdInt32 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt32* getInt32Ptr()  const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdUInt32 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt32* getUInt32Ptr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdInt64 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt64* getInt64Ptr()  const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdUInt64 object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt64* getUInt64Ptr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an double object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  double* getDoublePtr() const;


  /** \details
    Returns a reference to the value of this Variant object as an OdStringArray object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdStringArray& getStringArray() const;

  /** \details
  Returns a reference to the value of this Variant object as an OdAnsiStringArray object.
  \remarks
  No check is made to determine that this Variant object is of the specified type.
  */
  const OdAnsiStringArray& getAnsiStringArray() const;

  /** \details
    Returns a reference to the value of this Variant object as an OdRxObjectPtrArray object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdRxObjectPtrArray& getRxObjectPtrArray() const;

  /** \details
    Returns a reference to the value of this Variant object as an OdBoolArray object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdBoolArray& getBoolArray  () const;

  /** \details
    Returns a reference to the value of this Variant object as an OdInt8Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdInt8Array& getInt8Array  () const;

  /** \details
    Returns a reference to the value of this Variant object as an OdUInt8Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdUInt8Array& getUInt8Array () const;

  /** \details
    Returns a reference to the value of this Variant object as an OdInt16Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdInt16Array& getInt16Array () const;

  /** \details
    Returns a reference to the value of this Variant object as an OdUInt16Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdUInt16Array& getUInt16Array() const;

  /** \details
    Returns a reference to the value of this Variant object as an OdInt32Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdInt32Array& getInt32Array () const;

  /** \details
    Returns a reference to the value of this Variant object as an OdUInt32Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdUInt32Array& getUInt32Array() const;

  /** \details
    Returns a reference to the value of this Variant object as an OdInt64Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdInt64Array& getInt64Array () const;

  /** \details
    Returns a reference to the value of this Variant object as an OdUInt64Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdUInt64Array& getUInt64Array() const;

  /** \details
    Returns a reference to the value of this Variant object as an OdDoubleArray object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  const OdDoubleArray& getDoubleArray() const;

  /** \details
    Sets the type of this Variant object to OdStringArray.
  */
  OdStringArray& asStringArray();

  /** \details
  Sets the type of this Variant object to OdAnsiStringArray.

  \returns
  Returns a reference to this Variant object.
  */
  OdAnsiStringArray& asAnsiStringArray();

  /** \details
    Sets the type of this Variant object to OdRxObjectPtrArray.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdRxObjectPtrArray& asRxObjectPtrArray();
  
  /** \details
    Sets the type of this Variant object to OdBoolArray.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdBoolArray&   asBoolArray  ();
  
  /** \details
    Sets the type of this Variant object to OdInt8Array.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdInt8Array&   asInt8Array  ();
  
  /** \details
    Sets the type of this Variant object to OdUInt8Array.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdUInt8Array&  asUInt8Array ();
  
  /** \details
    Sets the type of this Variant object to OdInt16Array.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdInt16Array&  asInt16Array ();
  
  /** \details
    Sets the type of this Variant object to OdUInt16Array.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdUInt16Array& asUInt16Array();
  
  /** \details
    Sets the type of this Variant object to OdInt32Array.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdInt32Array&  asInt32Array ();
  
  /** \details
    Sets the type of this Variant object to OdUInt32Array.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdUInt32Array& asUInt32Array();
  
  /** \details
    Sets the type of this Variant object to OdInt64Array.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdInt64Array&  asInt64Array ();
  
  /** \details
    Sets the type of this Variant object to OdUInt64Array.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdUInt64Array& asUInt64Array();
  
  /** \details
    Sets the type of this Variant object to OdDoubleArray.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdDoubleArray& asDoubleArray();

  /** \details
    Returns the value of this Variant object as a pointer to an OdStringArray object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdStringArray* getStringArrayPtr() const;

  /** \details
  Returns the value of this Variant object as a pointer to an OdAnsiStringArray object.
  \remarks
  No check is made to determine that this Variant object is of the specified type.
  */
  OdAnsiStringArray* getAnsiStringArrayPtr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdRxObjectPtrArray object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdRxObjectPtrArray* getRxObjectPtrArrayPtr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdBoolArray object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdBoolArray* getBoolArrayPtr  () const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdInt8Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt8Array* getInt8ArrayPtr  () const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdUInt8Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt8Array* getUInt8ArrayPtr () const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdInt16Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt16Array* getInt16ArrayPtr () const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdUInt16Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt16Array* getUInt16ArrayPtr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdInt32Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt32Array* getInt32ArrayPtr () const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdUInt32Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt32Array* getUInt32ArrayPtr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdInt64Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdInt64Array* getInt64ArrayPtr () const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdUInt64Array object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdUInt64Array* getUInt64ArrayPtr() const;

  /** \details
    Returns the value of this Variant object as a pointer to an OdDoubleArray object.
    \remarks
    No check is made to determine that this Variant object is of the specified type.
  */
  OdDoubleArray* getDoubleArrayPtr() const;

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
  */
  OdVariant& setString(const OdString& val);

  /** \details
  Sets the value of this Variant object.

  \param val [in]  Value.

  \returns
  Returns a reference to this Variant object.
  */
  OdVariant& setAnsiString(const OdAnsiString& val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setRxObjectPtr( const OdRxObjectPtr& val );

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setBool  (bool val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt8  (OdInt8 val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt8 (OdUInt8 val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt16 (OdInt16 val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt16(OdUInt16 val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt32 (OdInt32 val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt32(OdUInt32 val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt64 (OdInt64 val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt64(OdUInt64 val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setIntPtr (OdIntPtr val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setDouble(double val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setStringPtr(OdString* val);

  /** \details
  Sets the value of this Variant object.

  \param val [in]  Value.

  \returns
  Returns a reference to this Variant object.
  */
  OdVariant& setAnsiStringPtr(OdAnsiString* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setRxObjectPtrPtr(OdRxObjectPtr* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setBoolPtr  (bool* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt8Ptr  (OdInt8* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt8Ptr (OdUInt8* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt16Ptr (OdInt16* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt16Ptr(OdUInt16* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt32Ptr (OdInt32* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt32Ptr(OdUInt32* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt64Ptr (OdInt64* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt64Ptr(OdUInt64* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setDoublePtr(double* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setStringArray(const OdStringArray& val);

  /** \details
  Sets the value of this Variant object.

  \param val [in]  Value.

  \returns
  Returns a reference to this Variant object.
  */
  OdVariant& setAnsiStringArray(const OdAnsiStringArray& val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setRxObjectPtrArray(const OdRxObjectPtrArray& val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setBoolArray  (const OdBoolArray&   val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt8Array  (const OdInt8Array&   val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt8Array (const OdUInt8Array&  val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt16Array (const OdInt16Array&  val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt16Array(const OdUInt16Array& val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt32Array (const OdInt32Array&  val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt32Array(const OdUInt32Array& val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt64Array (const OdInt64Array&  val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt64Array(const OdUInt64Array& val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setDoubleArray(const OdDoubleArray& val);


  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setStringArrayPtr(OdStringArray* val);

  /** \details
  Sets the value of this Variant object.

  \param val [in]  Value.

  \returns
  Returns a reference to this Variant object.
  */
  OdVariant& setAnsiStringArrayPtr(OdAnsiStringArray* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setRxObjectPtrArrayPtr(OdRxObjectPtrArray* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setBoolArrayPtr  (OdBoolArray*   val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt8ArrayPtr  (OdInt8Array*   val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt8ArrayPtr (OdUInt8Array*  val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt16ArrayPtr (OdInt16Array*  val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt16ArrayPtr(OdUInt16Array* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt32ArrayPtr (OdInt32Array*  val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt32ArrayPtr(OdUInt32Array* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setInt64ArrayPtr (OdInt64Array*  val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setUInt64ArrayPtr(OdUInt64Array* val);

  /** \details
    Sets the value of this Variant object.
    
    \param val [in]  Value.
    
    \returns
    Returns a reference to this Variant object.
  */
  OdVariant& setDoubleArrayPtr(OdDoubleArray* val);
};



#include "TD_PackPop.h"

#endif //_ODVARIANT_H_INCLUDED_

