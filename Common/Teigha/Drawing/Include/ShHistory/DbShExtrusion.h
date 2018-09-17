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

#ifndef ODDB_DBSHEXTRUSION_H
#define ODDB_DBSHEXTRUSION_H

#include "TD_PackPush.h"

#include "DbShPrimitive.h"
#include "DbSweepOptions.h"

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class DB3DSOLIDHISTORY_EXPORT OdDbShExtrusion : public OdDbShPrimitive
{
public:
  ODDB_DECLARE_MEMBERS(OdDbShExtrusion);
  OdDbShExtrusion();

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);  
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;  
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);  
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  OdGeVector3d direction() const;
  OdResult setDirection(OdGeVector3d const & vecDir);
  OdResult getSweepOptions(OdDbSweepOptions& sweepOptions) const;
  OdResult setSweepOptions(OdDbSweepOptions const &);
  OdResult setExtrude(OdGeVector3d const & vec, OdDbSweepOptions const & opt);
  double height() const;
  OdResult setHeight(double dHeight);

  OdDbEntityPtr sweepEntity() const;
  OdDbEntityPtr pathEntity() const;
  OdResult setPathEntity(OdDbEntity* pPathEnt);
  OdResult setSweepEntity(OdDbEntity* pSweepEnt);
  
  /*OdResult setTaperAngle(double dtaperAng);
  double taperAngle();*/
  OdResult getPathLength(double& dLen)const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbShExtrusion object pointers.
*/
typedef OdSmartPtr<OdDbShExtrusion> OdDbShExtrusionPtr;

#include "TD_PackPop.h"

#endif

