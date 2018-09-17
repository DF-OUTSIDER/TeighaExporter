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




#ifndef _AECI_DB_EMBEDDEDRASTERIMAGE_H_
#define _AECI_DB_EMBEDDEDRASTERIMAGE_H_ /*!DOM*/

#include "TD_PackPush.h"

#include "AeciIbExport.h"
#include "DbRasterImage.h"

/** \details
    This class represents RasterImage entities in an OdDbDatabase instance.

    \sa
    TD_AeciIb
    
    <group AeciIb_Classes>
*/
class AECIIB_EXPORT AECIDbEmbeddedRasterImage : public OdDbRasterImage
{
public:
  ODDB_DECLARE_MEMBERS(AECIDbEmbeddedRasterImage);

  AECIDbEmbeddedRasterImage();

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;

private:
  static const OdInt32 kCurrentClassVersion;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for AECIDbEmbeddedRasterImage object pointers.
*/
typedef OdSmartPtr<AECIDbEmbeddedRasterImage> AECIDbEmbeddedRasterImagePtr;

#include "TD_PackPop.h"

#endif // _AECI_DB_EMBEDDEDRASTERIMAGE_H_

