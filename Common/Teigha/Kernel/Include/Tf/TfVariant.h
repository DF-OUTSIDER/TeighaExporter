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

#ifndef __TF_VARIANT_H__
#define __TF_VARIANT_H__

#include "RxVariant.h"
#include "Tf/TfExport.h"
#include "OdGUID.h"

class OdDbStub;

class OdTfVariant;
typedef OdArray<OdTfVariant> OdTfVariantArray;
typedef OdArray<OdTfVariant> OdTfVariantTuple;

/** \details
    Corresponding C++ library: Od_Tf
    <group OdTf_Classes>
*/
class TFCORE_EXPORT OdTfVariant : public OdVariant {
public:
  static const OdVariant::TypeFactory* typeFactory(int type);
  typedef enum {
    kGUID = OdVariant::kNextType,
    kEmpty,
    kDEPRECARED0,
    kDbStubPtr,
    kRxObjectWeakPtr,
    kTuple,
    kNextType
  } Type;

  static Type kVariantArray(OdUInt16, OdUInt32);
  static Type kVariantArray(OdUInt16, OdTfVariant::Type);
  static Type kVariantArray(OdUInt32);
  static Type kVariantArray(OdTfVariant::Type);

  static Type getItemType(OdTfVariant::Type type);
  static OdUInt16 getDimsCount(OdTfVariant::Type type);
  static OdTfVariant empty();

  bool isEmpty() const;
  bool isVariantArray() const;

  OdTfVariant();
  OdTfVariant(const OdVariant& val);
  OdTfVariant(const OdTfVariant& val);
  OdTfVariant(const OdGUID& val);
  OdTfVariant(const OdTfVariantTuple&);
  OdTfVariant(OdTfVariant::Type type, const OdTfVariantArray& val);
  OdTfVariant(OdDbStub* const);

  OdTfVariant& operator =(const OdTfVariant& val);

  virtual ~OdTfVariant();
  const OdGUID& getGUID() const;
  const OdTfVariantArray& getVariantArray() const;
  OdDbStub* getDbStubPtr() const;
  OdRxObject* getRxObjectWeakPtr() const;
  const OdTfVariantTuple& getVariantTuple() const;

  OdTfVariantArray& asVariantArray();
  OdTfVariantTuple& asVariantTuple();

  OdTfVariant& setGUID(const OdGUID&);
  OdTfVariant& setVariantArray(OdTfVariant::Type type, const OdTfVariantArray&);
  OdTfVariant& setVariantTuple(const OdTfVariantTuple&);
  OdTfVariant& setDbStubPtr(OdDbStub* const);
  OdTfVariant& setRxObjectWeakPtr(OdRxObject* const);

protected:
  void setVarType(int newType, int& type, void* data);
  void makeEmpty();
};

inline bool operator==(OdTfVariant::Type lhs, OdVariant::Type rhs) {
  return lhs == static_cast<OdTfVariant::Type>(rhs);
}

inline bool operator!=(OdTfVariant::Type lhs, OdVariant::Type rhs) {
  return !(lhs == rhs);
}

inline bool operator==(OdVariant::Type lhs, OdTfVariant::Type rhs) {
  return rhs == lhs;
}

inline bool operator!=(OdVariant::Type lhs, OdTfVariant::Type rhs) {
  return rhs != lhs;
}

#endif
