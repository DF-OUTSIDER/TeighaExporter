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

#include "OdaCommon.h"
#include "DbEvalVariant.h"
#include "RxObjectImpl.h"
#include "Ge/GePoint3d.h"

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbEvalVariant, OdResBuf, L"AcDbEvalVariant", RXIMPL_CONSTR);

OdDbEvalVariant::OdDbEvalVariant():OdResBuf(), m_type(kDwgNull)
{
}

OdDbEvalVariant::~OdDbEvalVariant()
{
}

OdDbEvalVariantPtr OdDbEvalVariant::init(const OdDbEvalVariant& other)
{
  switch ( other.getType() )
  {
    case kDwgReal:
    {
       double val;
       other.getValue(val);
       return OdDbEvalVariant::init(val);
    }
    case kDwgInt16:
    {
      short val;
      other.getValue(val);
      return OdDbEvalVariant::init(val);
    }
    case kDwgInt32:
    {
      int val;
      other.getValue(val);
      return OdDbEvalVariant::init(val);
    }
    case kDwgText:
    {
      OdString val;
      other.getValue(val);
      OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
      res->setValue(kDxfText, val);
      return res;
    }
    case kDwgSoftOwnershipId:
    {
      OdDbObjectId val;
      other.getValue(val);
      return OdDbEvalVariant::init(val);
    }
    case kDwg3Real:
    {
      OdGePoint3d val;
      other.getValue(val);
      return OdDbEvalVariant::init(val);
    }
    case kDwg2Real:
    {
      OdGePoint2d val;
      other.getValue(val);
      return OdDbEvalVariant::init(val);
    }
  }

  return OdDbEvalVariant::createObject();
}
OdDbEvalVariantPtr OdDbEvalVariant::init(const OdDbEvalVariant* pOther)
{
  return OdDbEvalVariant::init(*pOther);
}

OdDbEvalVariantPtr OdDbEvalVariant::init(double dVal)
{
  OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
  res->setValue(kDxfReal, dVal);

  return res;
}

OdDbEvalVariantPtr OdDbEvalVariant::init(short iVal)
{
  OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
  res->setValue(kDxfInt16, iVal);

  return res;
}

OdDbEvalVariantPtr OdDbEvalVariant::init(int lVal)
{
  OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
  res->setValue(kDxfInt32, lVal);

  return res;
}

OdDbEvalVariantPtr OdDbEvalVariant::init(const OdDbObjectId& id)
{
  OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
  res->setValue(kDxfSoftPointerId, id);

  return res;
}

OdDbEvalVariantPtr OdDbEvalVariant::init(const OdGePoint2d& pt)
{
  OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
  res->setValue(kRtPoint2d, pt);

  return res;
}

OdDbEvalVariantPtr OdDbEvalVariant::init(const OdGePoint3d& pt)
{
  OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
  res->setValue(kRtPoint3d, pt);

  return res;
}

OdDbEvalVariantPtr OdDbEvalVariant::init(const OdString& str)
{
  OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
  //res->setValue(kRtString, str);
  res->setValue(kDxfText, str);

  return res;
}

OdDbEvalVariantPtr OdDbEvalVariant::init(const OdResBuf& rb)
{
  int gCode = rb.restype();
  switch (OdDxfCode::_getType(gCode))
  {
  case OdDxfCode::Name:
  case OdDxfCode::String:
  case OdDxfCode::Handle:
  case OdDxfCode::LayerName:
  {
    OdDbEvalVariantPtr res = OdDbEvalVariant::createObject();
    res->setValue(kDxfText, rb.getString());
    return res;
  }
  case OdDxfCode::Integer8:
    return init(rb.getInt8());
  case OdDxfCode::Integer16:
    return init((short)rb.getInt16());
  case OdDxfCode::Integer32:
    return init((int)rb.getInt32());
  case OdDxfCode::Double:
  case OdDxfCode::Angle:
    return init(rb.getDouble());
  case OdDxfCode::Point:
    return init(rb.getPoint3d());
  }
  return OdDbEvalVariant::createObject();
}

OdDbEvalVariant& OdDbEvalVariant::operator=(const OdResBuf& rb)
{
  switch (OdDxfCode::_getType(rb.restype()))
  {
  case OdDxfCode::Name:
  case OdDxfCode::String:
  case OdDxfCode::Handle:
  case OdDxfCode::LayerName:
    setValue(kDxfText, rb.getString());
  break;
  case OdDxfCode::Integer8:
    setValue(kDxfInt16, (short)rb.getInt8());
  break;
  case OdDxfCode::Integer16:
    setValue(kDxfInt16, (short)rb.getInt16());
  break;
  case OdDxfCode::Integer32:
    setValue(kDxfInt32, (int)rb.getInt32());
  break;
  case OdDxfCode::Double:
  case OdDxfCode::Angle:
    setValue(kDxfReal, rb.getDouble());
  break;
  case OdDxfCode::Point:
    setValue(kRtPoint3d, rb.getPoint3d());
  break;
  }

  return *this;
}

OdDbEvalVariant& OdDbEvalVariant::operator=(double      dVal)
{
  setValue(kDxfReal, dVal);
  return *this;
}

OdDbEvalVariant& OdDbEvalVariant::operator=(short       iVal)
{
  setValue(kDxfInt16, iVal);
  return *this;
}

OdDbEvalVariant& OdDbEvalVariant::operator=(int        lVal)
{
  setValue(kDxfInt32, lVal);
  return *this;
}


//OdDbEvalVariant& OdDbEvalVariant::operator=(const OdDbEvalVariant& other)
//{
//  switch ( other.getType() )
//  {
//    case kDwgReal:
//    {
//       double val;
//       other.getValue(val);
//       return operator=(val);
//    }
//    case kDwgInt16:
//    {
//      short val;
//      other.getValue(val);
//      return operator=(val);
//    }
//    case kDwgInt32:
//    {
//      int val;
//      other.getValue(val);
//      return operator=(val);
//    }
//    case kDwgText:
//    {
//      OdString val;
//      other.getValue(val);
//      setValue(kDxfText, val);
//      return *this;
//    }
//    case kDwgSoftOwnershipId:
//    {
//      OdDbObjectId val;
//      other.getValue(val);
//      return operator=(val);
//    }
//    case kDwg3Real:
//    {
//      OdGePoint3d val;
//      other.getValue(val);
//      return operator=(val);
//    }
//    case kDwg2Real:
//    {
//      OdGePoint2d val;
//      other.getValue(val);
//      return operator=(val);
//    }
//  }
//  return *this;
//}

OdDbEvalVariant& OdDbEvalVariant::operator=(const OdDbEvalVariant& other)
{
  OdDbEvalVariantPtr optherPtr = &other;
  OdDbEvalVariantPtr updatedPtr = this;

  while (!optherPtr.isNull())
  {
    switch (optherPtr->getType())
    {
      case kDwgReal:
      {
        double val;
        optherPtr->getValue(val);
        updatedPtr->setValue(kDxfReal, val);
        break;
      }
      case kDwgInt16:
      {
        short val;
        optherPtr->getValue(val);
        updatedPtr->setValue(kDxfInt16, val);
        break;
      }
      case kDwgInt32:
      {
        int val;
        optherPtr->getValue(val);
        updatedPtr->setValue(kDxfInt32, val);
        break;
      }
      case kDwgText:
      {
        OdString val;
        optherPtr->getValue(val);
        updatedPtr->setValue(kDxfText, val);
        return *this;
      }
      case kDwgSoftOwnershipId:
      {
        OdDbObjectId val;
        optherPtr->getValue(val);
        updatedPtr->setValue(kDxfSoftPointerId, val);
        break;
      }
      case kDwg3Real:
      {
        OdGePoint3d val;
        optherPtr->getValue(val);
        updatedPtr->setValue(kRtPoint3d, val);
        break;
      }
      case kDwg2Real:
      {
        OdGePoint2d val;
        optherPtr->getValue(val);
        updatedPtr->setValue(kRtPoint2d, val);
        break;
      }
    }

    optherPtr = optherPtr->next();

    if (!optherPtr.isNull())
    {
      OdDbEvalVariantPtr newUpdatedPtr = OdDbEvalVariant::init(*optherPtr);
      updatedPtr->setNext(newUpdatedPtr);
      updatedPtr = newUpdatedPtr;
    }
  }

  return *this;
}

OdDbEvalVariant& OdDbEvalVariant::operator=(const OdDbObjectId& id)
{
  setValue(kDxfSoftPointerId, id);
  return *this;
}

OdDbEvalVariant& OdDbEvalVariant::operator=(const OdGePoint2d&  pt)
{
  setValue(kRtPoint2d, pt);
  return *this;
}

OdDbEvalVariant& OdDbEvalVariant::operator=(const OdGePoint3d&  pt)
{
  setValue(kRtPoint3d, pt);
  return *this;
}

OdDbEvalVariant& OdDbEvalVariant::operator=(const OdString&  str)
{
  setValue(kRtString, str);
  return *this;
}

void OdDbEvalVariant::clear()
{
  
}

void OdDbEvalVariant::copyFrom(const OdRxObject* pOther)
{
  if ( !pOther->isKindOf(desc()) )
    return;

  OdResBuf::copyFrom(pOther);
  OdDbEvalVariantPtr pVar = OdDbEvalVariant::cast(pOther);
  m_type = pVar->m_type;
}


bool OdDbEvalVariant::operator <  ( const OdDbEvalVariant& val ) const
{
  if ( getType() != val.getType() )
    return false;

  return !operator==(val) || !operator>(val);
}

bool OdDbEvalVariant::operator >  ( const OdDbEvalVariant& val ) const
{
  if ( getType() != val.getType() )
    return false;

  switch ( getType() )
  {
    case kDwgReal:
      double dVal1;
      double dVal2;
      getValue(dVal1);
      val.getValue(dVal2);
      return dVal1 > dVal2;
    case kDwgInt16:
      short iVal1;
      short iVal2;
      getValue(iVal1);
      val.getValue(iVal2);
      return iVal1 > iVal2;
    case kDwgInt32:
      int IVal1;
      int IVal2;
      getValue(IVal1);
      val.getValue(IVal2);
      return IVal1 > IVal2;
    case kDwgText:
      OdString strVal1;
      OdString strVal2;
      getValue(strVal1);
      val.getValue(strVal2);
      return strVal1 > strVal2;
    break;
  }

  return false;
}

bool OdDbEvalVariant::operator <= ( const OdDbEvalVariant& val ) const
{
  if ( getType() != val.getType() )
    return false;

  return operator==(val) || !operator>(val);
}

bool OdDbEvalVariant::operator >= ( const OdDbEvalVariant& val ) const
{
  if ( getType() != val.getType() )
    return false;

  return operator==(val) || operator>(val);
}

bool OdDbEvalVariant::operator == ( const OdDbEvalVariant& val ) const
{
  if ( getType() != val.getType() )
    return false;

  switch ( getType() )
  {
    case kDwgReal:
      double dVal1;
      double dVal2;
      getValue(dVal1);
      val.getValue(dVal2);
      return fabs(dVal1 - dVal2) < 1.0e-10;
    case kDwgInt16:
      short iVal1;
      short iVal2;
      getValue(iVal1);
      val.getValue(iVal2);
      return iVal1 == iVal2;
    case kDwgInt32:
      int IVal1;
      int IVal2;
      getValue(IVal1);
      val.getValue(IVal2);
      return IVal1 == IVal2;
    case kDwgText:
      OdString strVal1;
      OdString strVal2;
      getValue(strVal1);
      val.getValue(strVal2);
      return strVal1 == strVal2;
    break;
  }

  return false;
}

bool OdDbEvalVariant::operator != ( const OdDbEvalVariant& val ) const
{
  if ( getType() != val.getType() )
    return false;

  return !operator==(val);
}

void OdDbEvalVariant::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrInt16((OdInt16)restype());
  switch (getType())
  {
  case kDwgReal:
    {
      double dVal;
      getValue(dVal);
      pFiler->wrDouble(dVal);
    }
    break;
  case kDwgInt32:
    {
      int iVal;
      getValue(iVal);
      pFiler->wrInt32(iVal);
    }
    break;
  case kDwgText:
    {
      OdString sVal;
      getValue(sVal);
      pFiler->wrString(sVal);
    }
    break;
  case kDwgInt16:
  {
    short value;
    getValue(value);
    pFiler->wrInt16(value);
    break;
  }
  case kDwgSoftPointerId:
  {
    OdDbObjectId id;
    getValue(id, pFiler->database());
    pFiler->wrSoftPointerId(id);
    break;
  }
  default:
    ODA_FAIL_ONCE();
    throw OdError(eNotApplicable);
  }
}

OdResult OdDbEvalVariant::dwgInFields(OdDbDwgFiler* pFiler)
{
  int type = pFiler->rdInt16();
  switch (type)
  {
  case kDxfInt32: // kDwgInt32
    setValue(type, (int)pFiler->rdInt32());
    break;
  case kDxfReal: // kDwgReal
    setValue(type, pFiler->rdDouble());
    break;
  case kDxfText: // kDwgText:
    setValue(type, pFiler->rdString());
    break;
  case kDxfInt16:
    setValue(type, pFiler->rdInt16());
    break;
  case kDxfSoftPointerId:
    setValue(type, pFiler->rdSoftPointerId());
    break;

  default:
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  return eOk;
}

void OdDbEvalVariant::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  switch (getType())
  {
  case kDwgReal: // kDxfReal:
    {
      double dVal;
      getValue(dVal);
      pFiler->wrDouble(restype(), dVal);
    }
    break;
  case kDwgInt32: // kDxfInt32
    {
      int iVal;
      getValue(iVal);
      pFiler->wrInt32(restype(), iVal);
    }
    break;
  case kDwgText: // kDxfText
    {
      OdString sVal;
      getValue(sVal);
      pFiler->wrString(restype(), sVal);
    }
    break;
  case kDwgInt16: // kDxfInt16:
  {
    short value;
    getValue(value);
    pFiler->wrInt16(restype(), value);
    break;
  }
  case kDwgSoftPointerId:
  {
    OdDbObjectId id;
    getValue(id, pFiler->database());
    pFiler->wrObjectId(restype(), id);
    break;
  }
  default:
    ODA_FAIL_ONCE();
    throw OdError(eNotApplicable);
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{ \
  ODA_FAIL(); \
  return eMakeMeProxy; \
}

OdResult OdDbEvalVariant::dxfInFields(OdDbDxfFiler* pFiler)
{
  int nextItemCode = pFiler->nextItem();
  switch (nextItemCode)
  {
  case kDxfInt32:
    setValue(nextItemCode, (int)pFiler->rdInt32());
    break;
  case kDxfReal:
    setValue(nextItemCode, pFiler->rdDouble());
    break;
  case kDxfText:
    setValue(nextItemCode, pFiler->rdString());
    break;
  case kDxfInt16:
    setValue(nextItemCode, pFiler->rdInt16());
    break;
  case kDxfSoftPointerId:
  {
    setValue(nextItemCode, pFiler->rdObjectId());
    break;
  }
  default:
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  return eOk;
}

#undef NEXT_CODE

DwgDataType OdDbEvalVariant::getType() const
{
  return m_type;
}


OdResult OdDbEvalVariant::setValue(int /*AcDb::DxfCode*/ groupcode, const double value)
{
  setRestype(groupcode);
  setDouble(value);

  m_type = kDwgReal;
  return eOk;
}

OdResult OdDbEvalVariant::setValue(int /*AcDb::DxfCode*/ groupcode, const short value)
{
  setRestype(groupcode);
  setInt16(value);

  m_type = kDwgInt16;

  return eOk;
}

OdResult OdDbEvalVariant::setValue(int /*AcDb::DxfCode*/ groupcode, const int value)
{
  setRestype(groupcode);
  setInt32(value);

  m_type = kDwgInt32;

  return eOk;
}

OdResult OdDbEvalVariant::setValue(int /*AcDb::DxfCode*/ groupcode, const OdString& value)
{
  setRestype(groupcode);
  setString(value);

  m_type = kDwgText;

  return eOk;
}

OdResult OdDbEvalVariant::setValue(int /*AcDb::DxfCode*/ groupcode, const OdDbObjectId value)
{
  setRestype(groupcode);
  setObjectId(value);
  switch (groupcode)
  {
  case kDxfSoftPointerId:
    m_type = kDwgSoftPointerId;
    break;
  case kDxfHardPointerId:
    m_type = kDwgHardPointerId;
    break;
  case kDxfSoftOwnershipId:
    m_type = kDwgSoftOwnershipId;
    break;
  case kDxfHardOwnershipId:
    m_type = kDwgHardOwnershipId;
    break;
  default:
    ODA_ASSERT_ONCE(!"Unknown code");
    return eNotApplicable;
  }
  return eOk;
}

OdResult OdDbEvalVariant::setValue(int /*AcDb::DxfCode*/ groupcode, const OdGePoint3d& value)
{
  setRestype(groupcode);
  setPoint3d(value);

  m_type = kDwg3Real;

  return eOk;
}

OdResult OdDbEvalVariant::setValue(int /*AcDb::DxfCode*/ groupcode, const OdGePoint2d& value)
{
  setRestype(groupcode);
  setPoint2d(value);

  m_type = kDwg2Real;
  return eOk;
}

OdResult OdDbEvalVariant::getValue(double& value) const
{
  if ( getType() != kDwgReal )
    return eWrongObjectType;

  value = getDouble();
  return eOk;
}

OdResult OdDbEvalVariant::getValue(short& value) const
{
  if ( getType() != kDwgInt16 )
    return eWrongObjectType;

  value = getInt16();
  return eOk;
}

OdResult OdDbEvalVariant::getValue(int& value) const
{
   if ( getType() != kDwgInt32 )
    return eWrongObjectType;

  value = getInt32();
  return eOk;
}

OdResult OdDbEvalVariant::getValue(OdString& value) const
{
  if ( getType() != kDwgText )
    return eWrongObjectType;

  value = getString();
  return eOk;
}

OdResult OdDbEvalVariant::getValue(OdDbObjectId& value, OdDbDatabase *pDb) const
{
  switch (getType())
  {
  case kDwgSoftPointerId:
  case kDwgHardPointerId:
  case kDwgSoftOwnershipId:
  case kDwgHardOwnershipId:
    break;
  default:
    return eWrongObjectType;
  }
  value = getObjectId(pDb);
  return eOk;
}

OdResult OdDbEvalVariant::getValue(OdGePoint3d& value) const
{
  if ( getType() != kDwg3Real )
    return eWrongObjectType;

  OdGePoint3d val = getPoint3d();
  value.set(val.x, val.y, val.z);
  return eOk;
}

OdResult OdDbEvalVariant::getValue(OdGePoint2d& value) const
{
  if ( getType() != kDwg2Real )
    return eWrongObjectType;

  OdGePoint2d val = getPoint2d();
  value.set(val.x, val.y);
  return eOk;
}
