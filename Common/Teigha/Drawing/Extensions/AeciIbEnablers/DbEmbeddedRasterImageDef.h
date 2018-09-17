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


#ifndef _AECI_DB_EMBEDDEDRASTERIMAGEDEF_H_
#define _AECI_DB_EMBEDDEDRASTERIMAGEDEF_H_

#include "TD_PackPush.h"

#include "AeciIbExport.h"
#include "DbRasterImageDef.h"
#include "RxObjectImpl.h"

/** \details
    This class defines Embedded Raster Image Definition objects in an OdDbDatabase instance.
    
    \sa
    TD_AeciIb

    \remarks

    <group AeciIb_Classes>
*/
class AECIIB_EXPORT AECIDbEmbeddedRasterImageDef : public OdDbRasterImageDef
{
public:
  ODDB_DECLARE_MEMBERS(AECIDbEmbeddedRasterImageDef);

  AECIDbEmbeddedRasterImageDef();

  OdString getEmbeddedObjectName() const;

  void setEmbeddedObjectName(
    const OdString& objName);

  OdStreamBufPtr getRasterImage() const;

  virtual OdResult load(
    bool modifyDatabase/* = true*/);

  virtual bool isLoaded() const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;

private:
  static const OdInt32 kCurrentClassVersion;

  OdString  m_objectName;
  bool      m_isLoaded;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for AECIDbEmbeddedRasterImageDef object pointers.
*/
typedef OdSmartPtr<AECIDbEmbeddedRasterImageDef> AECIDbEmbeddedRasterImageDefPtr;

#include "TD_PackPop.h"

#endif // _AECI_DB_EMBEDDEDRASTERIMAGEDEF_H_

