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

#ifndef _OD_DBAPPENTITYGRIPS_H_
#define _OD_DBAPPENTITYGRIPS_H_

#include "RxProtocolReactor.h"
#include "DbGrip.h"
#include "DbEntity.h"

/** \details
  Corresponding C++ library:  TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbAppEntityGrips : public OdRxProtocolReactor
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAppEntityGrips);

  virtual OdResult getGripPoints(OdDbGripDataPtrArray& grips, const double curViewUnitSize, 
    const int gripSize, const OdGeVector3d& curViewDir, const int bitflags, OdDbEntity* pEntity) = 0;

  virtual OdResult moveGripPointsAt(const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, 
    const int bitflags, OdDbEntity* pEntity) = 0;

  virtual void gripStatus(const OdDb::GripStat status, OdDbEntity* pEntity) = 0;
};

/** \details
  Corresponding C++ library:  TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbBlockInsertionPoints : public OdRxProtocolReactor
{
public:
  ODRX_DECLARE_MEMBERS(OdDbBlockInsertionPoints);
  virtual OdResult getInsertionPoints(const OdDbBlockTableRecord* pBlock, const OdDbBlockReference*   pBlkRef,
    OdGePoint3dArray& insPts, OdGeVector3dArray& alignmentDirections) = 0;
};

#endif
