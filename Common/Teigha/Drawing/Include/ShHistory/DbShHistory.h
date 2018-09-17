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

#ifndef ODDB_DBSHHISTORY_H
#define ODDB_DBSHHISTORY_H

#include "TD_PackPush.h"

#include <DbObject.h>
#include "TD_3DSolidHistoryDef.h"
#include "DbLoftOptions.h"
#include "DbRevolveOptions.h"
#include "DbSweepOptions.h"

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class DB3DSOLIDHISTORY_EXPORT OdDbShHistory : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbShHistory);

  //OdDbShHistory(OdDbShHistory const &)
  OdDbShHistory();
  virtual ~OdDbShHistory();

  virtual OdResult dwgInFields( OdDbDwgFiler* pFiler );  
  virtual void dwgOutFields( OdDbDwgFiler* pFiler ) const;  
  virtual OdResult dxfInFields( OdDbDxfFiler* pFiler );  
  virtual void dxfOutFields( OdDbDxfFiler* pFiler ) const;

  virtual OdResult setShowHistory( bool bShowhistory );
  virtual bool showHistory() const;
  virtual int numNodes() const;

  virtual OdResult setRecordHistory( bool bRecordHistory );
  virtual bool recordHistory() const;

  virtual void clearHistory();
  virtual OdResult purgeGraph();
  virtual OdResult getGraph( OdDbObjectId & );

  virtual OdResult createBox( double dXLen, double dYLen, double dZLen/*, BODY*&*/);
  //virtual OdResult createBrep(OdCmColor const &, OdDbObjectId,void *)
  virtual OdResult createFrustum( double dHeight, double dXRadius, double dYRadius, double dTopXRadius/*, BODY*&*/);
  virtual OdResult createSphere( double dRadius/*, BODY * &*/);
  virtual OdResult createTorus( double dMajorRadius, double dMinorRadius/*, BODY * &*/);
  virtual OdResult createWedge( double dXLen, double dYLen, double dZLen/*, BODY * &*/);
  virtual OdResult createPyramid( double dHeight, int iSides, double dRadius, double dTopRadius/*, BODY * &*/);

  virtual OdResult setSubentColor( const OdDbSubentId &subentId, const OdCmColor &color );
  virtual OdResult setSubentMaterial( const OdDbSubentId &subentId, OdDbObjectId matId );

  virtual OdResult createLoftedSolid(OdDbEntityPtrArray& crossSections, OdDbEntityPtrArray& guideCurves, OdDbEntity* pPathCurve, OdDbLoftOptions& loftOpt/*, BODY*&*/);
  virtual OdResult createRevolvedSolid(OdDbEntity* pRevolveCurve, const OdGePoint3d& axisPoint, const OdGeVector3d& axisDir, double dAngleOfRevolution, double dStartAngle, OdDbRevolveOptions& revolveOpt/*, BODY*&*/);
  virtual OdResult createSweptSolid(OdDbEntity* pSweepCurve, OdDbEntity* pPathCurve, const OdGeVector3d& directioVector, OdDbSweepOptions& sweepOpt/*, BODY * &*/);
  //OdDbShBrep
  virtual OdResult booleanOper( OdDbShHistory* pSecondEll, OdDb::BoolOperType operation );

  virtual OdResult postInDatabase(OdDbObjectId& objId, OdDbDatabase* pDatabase);
  virtual OdResult subTransformBy( const OdGeMatrix3d& xform );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbShHistory object pointers.
*/
typedef OdSmartPtr<OdDbShHistory> OdDbShHistoryPtr;

#include "TD_PackPop.h"

#endif

