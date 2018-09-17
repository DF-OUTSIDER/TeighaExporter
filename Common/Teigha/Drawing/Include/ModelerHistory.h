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


#ifndef _OD_MODELERHISTORY_INCLUDED_
#define _OD_MODELERHISTORY_INCLUDED_

#define STL_USING_MAP
#include "OdaSTL.h"
#include "RxObject.h"
#include "ModelerDefs.h"
#include "Db3dSolid.h"
#include "DbObject.h"

#include "TD_PackPush.h"


/** \details

  Corresponding C++ library: TD_Db
  <group OdDbSh_Classes> 
*/
class DBENT_EXPORT OdDbShModelerHistory : public OdRxObject
{
protected:
  OdDbShModelerHistory() {}
public:  
  ODRX_DECLARE_MEMBERS(OdDbShModelerHistory);

  virtual OdDbObjectId createShHistory( OdDb3dSolid const* pSolid, OdDbObjectPtr& pHistObj ) = 0;
  virtual void getHistoryVariables( const OdDbObjectPtr& pHistObj, bool& bShowHistory, bool& bRecordHistory ) = 0;
  virtual void setShowHistory( OdDbObjectPtr& pHistObj, bool bShowHistory ) = 0;
  virtual void setRecordHistory( OdDbObjectPtr& pHistObj, bool bRecordHistory ) = 0;
  virtual void clearHistory( OdDbObjectPtr& pHistObj ) = 0;

  virtual OdResult createBox( OdDb3dSolid* pSolid, double dXLen, double dYLen, double dZLen ) = 0;
  virtual OdResult createFrustum( OdDb3dSolid* pSolid, double dHeight, double dXRadius, double dYRadius, double dTopXRadius ) = 0;
  virtual OdResult createWedge( OdDb3dSolid* pSolid, double dXLen, double dYLen, double dZLen ) = 0;
  virtual OdResult createPyramid( OdDb3dSolid* pSolid, double dHeight, int iSides, double dRadius, double dTopRadius ) = 0;
  virtual OdResult createSphere( OdDb3dSolid* pSolid, double dRadius ) = 0;
  virtual OdResult createTorus( OdDb3dSolid* pSolid, double dMajorRadius, double dMinorRadius ) = 0;

  virtual OdResult setSubentColor( OdDb3dSolid* pSolid, const OdDbSubentId &subentId, const OdCmColor &color ) = 0;
  virtual OdResult setSubentMaterial( OdDb3dSolid* pSolid, const OdDbSubentId &subentId, OdDbObjectId matId ) = 0;

  virtual OdResult createLoftedSolid( OdDb3dSolid* pSolid, OdDbEntityPtrArray& crossSections, OdDbEntityPtrArray& guideCurves, OdDbEntity* pPathCurve, OdDbLoftOptions& loftOpt ) = 0;
  virtual OdResult createRevolvedSolid( OdDb3dSolid* pSolid, OdDbEntity* pRevolveCurve, const OdGePoint3d& axisPoint, const OdGeVector3d& axisDir, double dAngleOfRevolution, double dStartAngle, OdDbRevolveOptions& revolveOpt ) = 0;
  virtual OdResult createSweptSolid( OdDb3dSolid* pSolid, OdDbEntity* pSweepCurve, OdDbEntity* pPathCurve, const OdGeVector3d& directioVector, OdDbSweepOptions& sweepOpt ) = 0;

  virtual OdResult booleanOper( OdDb3dSolid* pSolid, OdDb3dSolid* pSecondEll,OdDb::BoolOperType operation) = 0;

  virtual OdResult postInDatabase( OdDbObjectId objSolidId ) = 0;
  virtual OdResult subTransformBy( OdDb3dSolid* pSolid, const OdGeMatrix3d& xform ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdModelerHistory object pointers.
*/
typedef OdSmartPtr<OdDbShModelerHistory> OdDbShModelerHistoryPtr;

#include "TD_PackPop.h"

#endif // _OD_MODELERHISTORY_INCLUDED_
