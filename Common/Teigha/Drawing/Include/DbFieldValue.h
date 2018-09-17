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


#ifndef OD_DBFIELDVALUE_H
#define OD_DBFIELDVALUE_H

#include "TD_PackPush.h"
#include "OdValue.h"

class OdFieldValueImpl;
class OdDbObjectId;
class OdResBuf;
class OdDbDwgFiler;
class OdDbDxfFiler;

/** \details
    This class represents Field Values for OdField objects in an OdDbDatabase instance.
    
    \remarks
    Field objects store both the Field expression and their evaluated values.
    
    Fields can be evaluated by an evaluator to any one of the following data types: OdInt32,         
    Double, String, Date, Point, 3dPoint, ObjectId, Buffer, and Resbuf.       

    <group Other_Classes>
*/
class DBENT_EXPORT OdFieldValue : public OdValue 
{
public:
  ODRX_DECLARE_MEMBERS(OdFieldValue);
  
  OdFieldValue();
  /** \param value [in]  Value.
    \param date [in]  Date value
    \param x [in]  X-coordinate of the point.
    \param y [in]  Y-coordinate of the point.
    \param z [in]  Z-coordinate of the point.
    \param buffer [in]  Buffer.
    \param bufferSize [in]  Buffer size.
    \param resBuf [in]  ResBuf list.
    \param objectId [in]  Object ID.
  */
  OdFieldValue(const OdFieldValue& value);
  OdFieldValue(const OdString& value);
  OdFieldValue(OdInt32 value);
  OdFieldValue(double value);
  OdFieldValue(const OdInt64& date);
  OdFieldValue(double x, double y);
  OdFieldValue(double x, double y, double z);
  OdFieldValue(const OdDbObjectId& objectId);
  OdFieldValue(const OdResBuf& resBuf);
  // OdFieldValue(const OdRxVariantValue& var);
  OdFieldValue(const void* buffer, OdInt32 bufferSize);
  OdFieldValue(const OdGePoint3d& p);
  OdFieldValue(const OdGeVector3d& p);
  using OdValue::get;
  bool get(OdGePoint3d& p) const;
  bool get(OdGeVector3d& p) const;
  using OdValue::set;
  bool set(const OdGePoint3d& p);
  bool set(const OdGeVector3d& p);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdFieldValue object pointers.
*/
typedef OdSmartPtr<OdFieldValue> OdFieldValuePtr;

#include "TD_PackPop.h"

#endif // OD_DBFIELDVALUE_H
