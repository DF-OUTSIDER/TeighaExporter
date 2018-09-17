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




#ifndef _OD_GEOMETRYFROMPROXY_INCLUDED_
#define _OD_GEOMETRYFROMPROXY_INCLUDED_

#include "TD_PackPush.h"

#include "DbProxyEntity.h"
#include "DbProxyObject.h"
#include "ModelerGeometry.h"
#include "DynamicLinker.h"
#include "RxModule.h"

/** \details
    Returns the SAT file associated with the specified AcAdPart entity.
    \param adPart [in]  Entity pointer.
    \param sat [out]  Receives the SAT file.
    
    \remarks
    Returns true if and only if adPart is an entity of type AcAdPart, and a valid SAT file was
    returned in the sat parameter.
*/
DBENT_EXPORT bool odGetSatFromProxy(const OdDbProxyEntityPtr& adPart, OdAnsiString& sat);

/** \details
    Returns the OdModelerGeometry object associated with the specified AcAdPart entity. 
    \param adPart [in]  Entity pointer.
    \param pModelerGeometry [out]  Receives the OdModelerGeometry object.
    
    \remarks
    Returns true if and only if adPart is an entity of type AcAdPart, and a valid OdModelerGeometry object was
    returned in the pModelerGeometry parameter.
*/
DBENT_EXPORT bool odGetSatFromProxy(const OdDbProxyEntityPtr& adPart, OdModelerGeometryPtr& pModelerGeometry);

DBIO_EXPORT OdDbProxyEntityPtr odEntityToProxy(const OdDbEntity &pEnt, OdDb::DwgVersion dwgVer = OdDb::kDHL_CURRENT, OdDb::MaintReleaseVer nMaintVer = OdDb::kMRelease0);

DBIO_EXPORT OdDbProxyObjectPtr odObjectToProxy(const OdDbObject &pObj, OdDb::DwgVersion dwgVer = OdDb::kDHL_CURRENT, OdDb::MaintReleaseVer nMaintVer = OdDb::kMRelease0);


#include "TD_PackPop.h"

#endif

