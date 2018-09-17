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

#ifndef __ODGISUBENTITYTRAITSDATASAVER_H__
#define __ODGISUBENTITYTRAITSDATASAVER_H__

#include "TD_PackPush.h"

#include "Gi/GiSubEntityTraitsData.h"
#include "Gi/GiDgLinetype.h"
#include "Gi/GiFill.h"

/** \details

    <group OdGi_Classes> 
*/
struct OdGiSubEntityTraitsDataSaver : public OdGiSubEntityTraitsData
{
  ODRX_HEAP_OPERATORS();

  OdGiSubEntityTraitsDataSaver() : OdGiSubEntityTraitsData() { }
  OdGiSubEntityTraitsDataSaver(const OdGiSubEntityTraitsData &from) : OdGiSubEntityTraitsData(from) { allocPtrs(); }
  OdGiSubEntityTraitsDataSaver(const OdGiSubEntityTraitsDataSaver &from) : OdGiSubEntityTraitsData(from) { allocPtrs(); }

  ~OdGiSubEntityTraitsDataSaver() { deletePtrs(); }

  void setMapper(const OdGiMapper* pMapper)
  {
    if (mapper()) delete mapper();
    if (pMapper) OdGiSubEntityTraitsData::setMapper(new OdGiMapper(*pMapper));
    else OdGiSubEntityTraitsData::setMapper(NULL);
  }
  void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod)
  {
    if (lineStyleModifiers()) delete lineStyleModifiers();
    if (pLSMod) OdGiSubEntityTraitsData::setLineStyleModifiers(new OdGiDgLinetypeModifiers(*pLSMod));
    else OdGiSubEntityTraitsData::setLineStyleModifiers(NULL);
  }
  void setFill(const OdGiFill* pFill)
  {
    if (fill()) (OdGiFillPtr(fill(), kOdRxObjAttach));
    if (pFill) OdGiSubEntityTraitsData::setFill(static_cast<OdGiFill*>(pFill->clone().detach()));
    else OdGiSubEntityTraitsData::setFill(NULL);
  }

  OdGiSubEntityTraitsData &operator =(const OdGiSubEntityTraitsData &data)
  {
    deletePtrs();
    static_cast<OdGiSubEntityTraitsData&>(*this) = data;
    allocPtrs();
    return *this;
  }

protected:
  void deletePtrs()
  {
    if (mapper()) delete mapper();
    if (lineStyleModifiers()) delete lineStyleModifiers();
    if (fill()) (OdGiFillPtr(fill(), kOdRxObjAttach));
  }
  void allocPtrs()
  {
    if (mapper()) OdGiSubEntityTraitsData::setMapper(new OdGiMapper(*mapper()));
    if (lineStyleModifiers()) OdGiSubEntityTraitsData::setLineStyleModifiers(new OdGiDgLinetypeModifiers(*lineStyleModifiers()));
    if (fill()) OdGiSubEntityTraitsData::setFill(static_cast<OdGiFill*>(fill()->clone().detach()));
  }
};

#include "TD_PackPop.h"

#endif // __ODGISUBENTITYTRAITSDATASAVER_H__
