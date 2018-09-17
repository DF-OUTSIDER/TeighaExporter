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



#ifndef _ODVALUE_INCLUDED_
#define _ODVALUE_INCLUDED_

#include "TD_PackPush.h"
#include "StaticRxObject.h"

class OdValueImpl;
class OdValue;
typedef OdSmartPtr<OdValue> OdValuePtr;

/** \details
    <group Other_Classes>
*/
class DBENT_EXPORT OdValue : public OdStaticRxObject<OdRxObject>
{
public:
  ODRX_HEAP_OPERATORS();

  enum DataType
  {
    kUnknown            = 0x00,
    kLong               = 0x01,
    kDouble             = 0x02,
    kString             = 0x04,
    kDate               = 0x08,
    kPoint              = 0x10,
    k3dPoint            = 0x20,
    kObjectId           = 0x40,
    kBuffer             = 0x80,
    kResbuf             = 0x100,
    kGeneral            = 0x200,
    kColor              = 0x400
  };

  enum UnitType
  {
    kUnitless           = 0x00,
    kDistance           = 0x01,
    kAngle              = 0x02,
    kArea               = 0x04,
    kVolume             = 0x08,
    kCurrency           = 0x10,
    kPercentage         = 0x20
  };

  enum ParseOption
  {
    kParseOptionNone        = 0x00,
    kSetDefaultFormat       = 0x01,
    kPreserveMtextFormat    = 0x02,
    kConvertTextToValue     = 0x04,
    kChangeDataType         = 0x08,
    kParseTextForFieldCode  = 0x10
  };

  enum FormatOption
  {
    kFormatOptionNone       = 0x00,
    kForEditing             = 0x01,
    kForExpression          = 0x02,
    kUseMaximumPrecision    = 0x04,
    kIgnoreMtextFormat      = 0x08
  };


public:
  ODRX_DECLARE_MEMBERS(OdValue);

  OdValue(void);
  ~OdValue(void);

  OdValue(const OdValue& value);
  OdValue(const OdString& value);
  OdValue(OdInt32 value);
  OdValue(double value);
  OdValue(const OdInt64& date);
  OdValue(double x, double y);
  OdValue(double x, double y, double z);
  OdValue(const OdDbObjectId& objectId);
  OdValue(const OdResBufPtr& resBuf);
  // OdValue(const OdRxVariantValue& var);

  OdValue(const void* buffer, OdInt32 bufferSize);

  /** \details
  Sets the datatype of this FieldValue object to kUnknown.
  \remarks
  This function always returns true.
  */
  bool reset();
  /** \details
  Returns the datatype of this FieldValue object.
  \remarks
  dataType() returns one of the following:

  <table>
  Name              Value
  kUnknown          0x00
  kLong             0x01
  kDouble           0x02
  kString           0x04
  kDate             0x08
  kPoint            0x10
  k3dPoint          0x20
  kObjectId         0x40
  kBuffer           0x80
  kResbuf           0x100
  </table>
  */
  OdValue::DataType dataType () const;
  /** \details
  Returns true if and only if the datatype of this FieldValue is not kUnknown.
  */
  bool isValid () const;

  /** \details
  Returns this object as the specified type.

  \remarks
  OdInt64 is used for date values.
  */
  operator OdString() const;
  operator OdInt32() const;
  operator double() const;
  operator OdInt64() const;
  operator OdDbObjectId() const;  

  OdValue& operator=(const OdValue& value);
  OdValue& operator=(const OdString& value);
  OdValue& operator=(OdInt32 value);
  OdValue& operator=(double value);
  OdValue& operator=(OdInt64 date);
  OdValue& operator=(const OdDbObjectId& objectId);
  OdValue& operator=(const OdResBufPtr& resBuf);

  // OdValue& operator=    (const OdRxVariantValue& var);

  // bool  get                    (const OdChar*& pszValue) const;

  /** \details
      Returns the value of this FieldValue object.
      \param value [out]  Receives the value.

      \returns
      Returns true if and only if successful.
  */
  bool get(OdString& value) const;
  /** \details
      Returns the value of this FieldValue object.
      \param value [out]  Receives the value.

      \returns
      Returns true if and only if successful.
  */
  bool get(OdInt32& value) const;
  /** \details
      Returns the value of this FieldValue object.
      \param value [out]  Receives the value.

      \returns
      Returns true if and only if successful.
  */
  bool get(double& value) const;
  /** \details
      Returns the value of this FieldValue object.

      \param date [out]  Receives the date value

      \returns
      Returns true if and only if successful.
  */
  bool get(OdInt64& date) const;
  /** \details
      Returns the value of this FieldValue object.

      \param x [out]  Receives the X-coordinate of the point.
      \param y [out]  Receives the Y-coordinate of the point.

      \returns
      Returns true if and only if successful.
  */
  bool get(double& x, double& y) const;
  /** \details
      Returns the value of this FieldValue object.

      \param x [out]  Receives the X-coordinate of the point.
      \param y [out]  Receives the Y-coordinate of the point.
      \param z [out]  Receives the Z-coordinate of the point.

      \returns
      Returns true if and only if successful.
  */
  bool get(double& x, double& y, double& z) const;
  /** \details
      Returns the value of this FieldValue object.

      \param objectId [in]  Object ID.
      
	  \returns
      Returns true if and only if successful.
  */
  bool get(OdDbObjectId& objectId) const;
  /** \details
      Returns the value of this FieldValue object.

      \param resBuf [out]  Receives the  ResBuf list.

      \returns
      Returns true if and only if successful.
  */
  bool get(OdResBufPtr& resBuf) const;

  // bool get(OdRxVariantValue& var) const;

  /** \details
      Returns the value of this FieldValue object.
      
	  \param pBuf [out]  Receives a pointer to the buffer.
      \param bufferSize [out]  Receives the buffer size.
      
	  \returns
      Returns true if and only if successful.
  */
  bool get(void*& pBuf, OdInt32& bufferSize) const;
  /** \details
      Sets the value for this FieldValue object.
	  
      \param value [in]  Value.
      
	  \returns
      Returns true if and only if successful.
  */
  bool set(const OdValue& value);
  /** \details
      Sets the value for this FieldValue object.
      
	  \param value [in]  Value.

      \returns
      Returns true if and only if successful.
  */
  bool set(const OdString& value);
  /** \details
      Sets the value for this FieldValue object.
      
	  \param value [in]  Value.
      
	  \returns
      Returns true if and only if successful.
  */
  bool set(OdInt32 value);
  /** \details
      Sets the value for this FieldValue object.
      
	  \param value [in]  Value.
      
	  \returns
      Returns true if and only if successful.
  */
  bool set(double value);
  /** \details
      Sets the value for this FieldValue object.

      \param date [in]  Date value

      \returns
      Returns true if and only if successful.
  */
  bool set(const OdInt64& date);
  /** \details
      Sets the value for this FieldValue object.

      \param x [in]  X-coordinate of the point.
      \param y [in]  Y-coordinate of the point.

      \returns
      Returns true if and only if successful.
  */
  bool set(double x, double y);
  /** \details
      Sets the value for this FieldValue object.

      \param x [in]  X-coordinate of the point.
      \param y [in]  Y-coordinate of the point.
      \param z [in]  Z-coordinate of the point.

      \returns
      Returns true if and only if successful.
  */
  bool set(double x, double y, double z);
  /** \details
      Sets the value for this FieldValue object.

      \param objectId [in]  Object ID.
	  
      \returns
      Returns true if and only if successful.
  */
  bool set(const OdDbObjectId& objectId);
  /** \details
      Sets the value for this FieldValue object.

      \param resBuf [in]  ResBuf list.

      \returns
      Returns true if and only if successful.
  */
  bool set(const OdResBufPtr& resBuf);

  // bool  set                    (const OdRxVariantValue& var);

  /** \details
      Sets the value for this FieldValue object.

      \param buffer [in]  Buffer.
      \param bufferSize [in]  Buffer size.

      \returns
      Returns true if and only if successful.
  */
  bool set(const void* buffer, OdInt32 bufferSize);

//     bool  reset                   (void);
  /** \details
      Sets the datatype of this FieldValue object to kUnknown.
      
	  \remarks
      This function always returns true.
  */
  bool  reset  (OdValue::DataType nDataType);
//     bool  resetValue              (void);

  OdValue::UnitType unitType (void) const;
  void  setUnitType (OdValue::UnitType nUnitType);
  OdString   getFormat (void) const;
  void  setFormat (const OdString& pszFormat);

//     bool  parse                   (const OdString& pszText, 
//                                             OdValue::DataType nDataType, 
//                                             OdValue::UnitType nUnitType, 
//                                             const ACHAR* pszFormat, 
//                                             OdValue::ParseOption nOption, 
//                                             const AcDbObjectId* pTextStyleId);
// 

  OdString  format(OdDbDatabase* pDb = 0) const;
  OdString format(OdValue::FormatOption nOption, OdDbDatabase* pDb = 0);
  OdString format(const OdString& pszFormat, OdValue::FormatOption nOption, OdDbDatabase* pDb = 0);
  bool format(const OdString& pszFormat, OdString& pszValue, OdDbDatabase* pDb = 0) const;
 
  bool  convertTo               (OdValue::DataType nDataType, 
                                          OdValue::UnitType nUnitType);
  bool  convertTo               (OdValue::DataType nDataType, 
                                          OdValue::UnitType nUnitType, 
                                          bool bResetIfIncompatible);

  virtual OdResult dwgInFields (
    OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields (
    OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

protected:
  OdValue(OdValueImpl* pValImpl);

  friend class OdDbSystemInternals;
  OdValueImpl* m_pImpl;
};

#include "TD_PackPop.h"

#endif // _ODVALUE_INCLUDED_
