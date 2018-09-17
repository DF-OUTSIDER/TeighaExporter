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

#ifndef ODDB_DBSHMODELERHISTORY_H
#define ODDB_DBSHMODELERHISTORY_H

#include "TD_PackPush.h"

#include <DbObject.h>
#include "ModelerHistory.h"
#include "../Entities/Db3dSolidImpl.h"

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class OdDbShModelerHistoryImpl : public OdDbShModelerHistory
{
protected:
  OdDbShModelerHistoryImpl();
  virtual ~OdDbShModelerHistoryImpl();

  inline void historyObject(OdDb3dSolidImpl* p3dSolidImpl);
public:
  ODDB_DECLARE_MEMBERS(OdDbShModelerHistoryImpl);

  virtual OdDbObjectId createShHistory( OdDb3dSolid const* pSolid, OdDbObjectPtr& pHistObj );
  virtual void getHistoryVariables( const OdDbObjectPtr& pHistObj, bool& bShowHistory, bool& bRecordHistory );
  virtual void setShowHistory( OdDbObjectPtr& pHistObj, bool bShowHistory );
  virtual void setRecordHistory( OdDbObjectPtr& pHistObj, bool bRecordHistory );
  virtual void clearHistory( OdDbObjectPtr& pHistObj );

  virtual OdResult createBox( OdDb3dSolid* pSolid, double dXLen, double dYLen, double dZLen/*, BODY*&*/);
  //virtual OdResult createBrep(OdCmColor const &, OdDbObjectId,void *)
  virtual OdResult createFrustum( OdDb3dSolid* pSolid, double dHeight, double dXRadius, double dYRadius, double dTopXRadius/*, BODY*&*/);
  virtual OdResult createSphere( OdDb3dSolid* pSolid, double dRadius/*, BODY * &*/);
  virtual OdResult createTorus( OdDb3dSolid* pSolid, double dMajorRadius, double dMinorRadius/*, BODY * &*/);
  virtual OdResult createWedge( OdDb3dSolid* pSolid, double dXLen, double dYLen, double dZLen/*, BODY * &*/);
  virtual OdResult createPyramid( OdDb3dSolid* pSolid, double dHeight, int iSides, double dRadius, double dTopRadius/*, BODY * &*/);

  virtual OdResult setSubentColor( OdDb3dSolid* pSolid, const OdDbSubentId &subentId, const OdCmColor &color );
  virtual OdResult setSubentMaterial( OdDb3dSolid* pSolid, const OdDbSubentId &subentId, OdDbObjectId matId );

  virtual OdResult createLoftedSolid( OdDb3dSolid* pSolid, OdDbEntityPtrArray& crossSections, OdDbEntityPtrArray& guideCurves, OdDbEntity* pPathCurve, OdDbLoftOptions& loftOpt );
  virtual OdResult createRevolvedSolid( OdDb3dSolid* pSolid, OdDbEntity* pRevolveCurve, const OdGePoint3d& axisPoint, const OdGeVector3d& axisDir, double dAngleOfRevolution, double dStartAngle, OdDbRevolveOptions& revolveOpt );
  virtual OdResult createSweptSolid( OdDb3dSolid* pSolid, OdDbEntity* pSweepCurve, OdDbEntity* pPathCurve, const OdGeVector3d& directioVector, OdDbSweepOptions& sweepOpt );

  virtual OdResult booleanOper( OdDb3dSolid* pSolid, OdDb3dSolid* pSecondEll,OdDb::BoolOperType operation);

  virtual OdResult postInDatabase( OdDbObjectId objSolidId );
  virtual OdResult subTransformBy( OdDb3dSolid* pSolid, const OdGeMatrix3d& xform );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbShHistory object pointers.
*/
//typedef OdSmartPtr<OdDbShModelerHistoryImpl>OdDbShModelerHistoryImplPtr;

#include "TD_PackPop.h"

#endif

