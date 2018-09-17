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

#ifndef ODDGNLS_DBLSXDATA_H
#define ODDGNLS_DBLSXDATA_H

#include "OdaCommon.h"
#include "DbEntity.h"

#include "OdDgnLSExport.h"

#include "TD_PackPush.h"

// Entity DgnLSModifiers XData processing helper functions.

ODDGNLS_EXPORT const OdConstString &oddbDgnLSGetEntityXDataName();

ODDGNLS_EXPORT bool oddbDgnLSCheckEntityXData(const OdDbEntity *pEntity);

ODDGNLS_EXPORT bool oddbDgnLSReadEntityXData(const OdDbEntity *pEntity, OdGiDgLinetypeModifiers &pModifiers, double *pScale = NULL);
ODDGNLS_EXPORT bool oddbDgnLSDrawEntityXData(const OdDbEntity *pEntity, OdGiSubEntityTraits *pTraits);

ODDGNLS_EXPORT bool oddbDgnLSWriteEntityXData(OdDbEntity *pEntity, const OdGiDgLinetypeModifiers &pModifiers, const double &pScale);
ODDGNLS_EXPORT bool oddbDgnLSRemoveEntityXData(OdDbEntity *pEntity);

#include "TD_PackPop.h"

#endif // ODDGNLS_DBLSXDATA_H
