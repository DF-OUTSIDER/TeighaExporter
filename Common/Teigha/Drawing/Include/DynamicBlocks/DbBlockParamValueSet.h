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

#ifndef _DbBlockParamValueSet_h_Included_
#define _DbBlockParamValueSet_h_Included_

#include "TD_PackPush.h"

#include "Ge/GeDoubleArray.h"
#include "RxObjectImpl.h"
#include "DbEvalGraph.h"
class OdDbBlockParamValueSetImpl;

#ifdef OD_HAVE_MATH_FILE
#include <math.h>
#endif

#ifdef OD_HAVE_COMPLEX_FILE
#include <complex>
#endif

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockParamValueSet : public OdRxObject
{
protected:
  OdDbBlockParamValueSet(void *);
public: 
  ODRX_DECLARE_MEMBERS(OdDbBlockParamValueSet);
  OdDbBlockParamValueSet( const OdDbBlockParamValueSet&);
  OdDbBlockParamValueSet();
  virtual ~OdDbBlockParamValueSet();
  class OdDbBlockParamValueSet & operator=(const OdDbBlockParamValueSet &);
  void assign(const OdDbBlockParamValueSet& vs){ *this = vs; }
  bool angDir()const ;
  bool angular()const ;
  virtual double closestLegalValue(double value , double initial)const ;
  virtual void dwgInFieldsHelper(OdDbDwgFiler *);
  virtual void dwgOutFieldsHelper(OdDbDwgFiler *)const ;
  virtual void dxfInFieldsHelper(OdDbDxfFiler *,short,short,short,short);
  virtual void dxfOutFieldsHelper(OdDbDxfFiler *,short,short,short,short)const ;
  double increment()const ;
  double maximum()const ;
  double minimum()const ;
  void setAngDir(bool);
  void setAngular(bool);
  void setIncrement(double);
  void setIncrement(double,bool);
  void setMaximum(double);
  void setMaximum(double,bool);
  void setMinimum(double);
  void setMinimum(double,bool);
  void setUseIncrement(bool);
  void setUseMaximum(bool);
  void setUseMinimum(bool);
  void setUseValueList(bool);
  void setValueList(OdGeDoubleArray const &);
  void setValueList(OdGeDoubleArray const &,bool);
  OdGeDoubleArray  sortedValueListIncluding(double)const ;
  void updateValue(double);
  bool useIncrement()const ;
  bool useMaximum()const ;
  bool useMinimum()const ;
  bool useValueList()const ;
  virtual bool valueIsLegal(double,double)const ;
  OdGeDoubleArray  valueList()const ;
private:
  OdDbBlockParamValueSetImpl* m_pImpl;
};

typedef OdSmartPtr<OdDbBlockParamValueSet> OdDbBlockParamValueSetPtr;

#include "TD_PackPop.h"

#endif // _DbBlockParamValueSet_h_Included_
