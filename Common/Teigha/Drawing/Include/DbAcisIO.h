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

#ifndef _ODDB_ACISIO_INCLUDED_
#define _ODDB_ACISIO_INCLUDED_

#include "ModelerGeometry.h"

class OdDbDwgFiler;

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDbAcisIO
{
  DBENT_EXPORT OdResult readAcisData(OdDbDwgFiler* pFiler, OdModelerGeometryPtr& pAcisData, bool bStandardSaveFlag = true, OdDbAuditInfo* pAuditInfo = NULL, bool bEnableAcisAudit = false);
  //flag bCheckForBody in writeAcisData if disabled - disables checking if body present or not. If no body - sat is empty. But in case of edges cache this is not valid situation
  //because there is not body. So this flag should be set in false.
  DBENT_EXPORT bool writeAcisData(OdDbDwgFiler* pFiler, OdModelerGeometry* pAcisData, bool bStandardSaveFlag = true, bool bEnableAcisAudit = false, bool bCheckForBody = true);
};

#endif // _ODDB_ACISIO_INCLUDED_
