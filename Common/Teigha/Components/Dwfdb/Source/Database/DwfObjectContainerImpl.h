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

#ifndef __DWF_OBJECTCONTAINERIMPL_H__
#define __DWF_OBJECTCONTAINERIMPL_H__

#include "DwfObjectContainer.h"

//----------------------------------------------------------
//
// OdDwfObjectIteratorImpl
// Implementation of abstract iterator (OdDwfObjectIterator) for IDs container
// (used in the external interfaces).
//

class OdDwfObjectIteratorImpl : public OdRxObjectImpl< DwfBaseIterator<OdDwfObjectId,  DwfIdContainerType, OdDwfObjectIterator> >
{
public:
  // Static constructor
  static OdRxObjectPtr createObject(const DwfIdContainer* pContainer)
  {
    if ( !pContainer )
    {
      ODA_FAIL();
      throw OdError(eNullObjectPointer);
    }
    return OdRxObjectPtr(new OdDwfObjectIteratorImpl(pContainer), kOdRxObjAttach);
  }
  // Virtual constructor
  virtual OdRxObjectPtr clone() const
  {
    OdSmartPtr<OdDwfObjectIteratorImpl> it(new OdDwfObjectIteratorImpl(m_pContainer), kOdRxObjAttach);
    it->m_iter = m_iter;
    return OdRxObjectPtr(it);
  }

protected:
  OdDwfObjectIteratorImpl(const DwfIdContainer* pContainer)
  {
    m_pContainer = pContainer;
    start();
  }
};

//template <> struct c_TfVariantTo<OdDwfObjectContainer> {
//  OdDwfObjectContainer operator()(const OdTfVariant& value) {
//    const OdTfVariantArray& arr(value.getVariantArray());
//    OdDwfObjectContainer result;
//    result.reserve(arr.size());
//    for (OdTfVariantArray::const_iterator it = arr.begin(); it != arr.end(); ++it) {
//      result.push_back(it->getDbStubPtr());
//    }
//    return result;
//  }
//};
//
//template <> struct c_ToTfVariant<OdDwfObjectContainer> {
//  OdTfVariant operator()(const OdDwfObjectContainer& container) { 
//    OdArray<OdTfVariant> Ids;
//    std::transform(container.begin(), container.end(), std::back_inserter(Ids), 
//      c_ToTfVariant<OdDbStub*>());
//    OdTfVariant value(OdTfVariant::kVariantArray(OdTfVariant::kDbStubPtr), Ids);
//    return value;
//  }
//};

#endif //__DWF_OBJECTCONTAINERIMPL_H__
