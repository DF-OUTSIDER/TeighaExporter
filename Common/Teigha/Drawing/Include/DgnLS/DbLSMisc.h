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

#ifndef ODDGNLS_DBLSMISCELLANEOUS_H
#define ODDGNLS_DBLSMISCELLANEOUS_H

#include "OdaCommon.h"
#include "DbDictionary.h"

#include "OdDgnLSExport.h"

#include "TD_PackPush.h"

// Miscellaneous DgnLS functions. Helpful for importing.

// Dictionary operations

ODDGNLS_EXPORT const OdConstString &oddbDgnLSGetComponentsDictionaryName();
ODDGNLS_EXPORT const OdConstString &oddbDgnLSGetDefinitionKeyName();

ODDGNLS_EXPORT OdDbObjectId oddbDgnLSGetComponentsDictionaryId(OdDbDatabase* pDb, bool createIfNotFound = false);
ODDGNLS_EXPORT OdDbDictionaryPtr oddbDgnLSGetComponentsDictionary(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);

// UID operations

ODDGNLS_EXPORT void oddbDgnLSInitializeImportUID(OdUInt8 *pUID);
ODDGNLS_EXPORT void oddbDgnLSIncrementUID(OdUInt8 *pUID);

#include "TD_PackPop.h"

#endif // ODDGNLS_DBLSMISCELLANEOUS_H
