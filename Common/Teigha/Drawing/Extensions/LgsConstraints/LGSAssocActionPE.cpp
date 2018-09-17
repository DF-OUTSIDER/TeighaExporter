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
#include "OdaCommon.h"

#include <iostream>

#include "DbAssocNetwork.h"
#include "DbAssoc2dConstraintGroup.h"
#include "OdImplicitConstr.h"
#include "OdExplicitConstr.h"
#include "LGSAssocActionPE.h"
#include "LGSCommands.h"
#include "DbCommandContext.h"
#include "DbUserIO.h"
#include "LGSEmbedPE.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(LGSAssocActionPE, AssocActionPE, OD_T("LGSAssocActionPE"));

//!nb
static char saBuffer[25];

void LGSTrace(const OdString & iMsg)
{
#ifdef _DEBUG
  if (gpCmdCtx)
    gpCmdCtx->userIO()->putString(iMsg);
  std::cout << (const char*)iMsg << std::endl;
#endif
}

LGSAssocActionPE::LGSAssocActionPE()
{
}

LGSContext LGSAssocActionPE::CreateContext()
{
  OdSmartPtr<LGSEmbedPE> pPE = OdDbAssocNetwork::desc()->getX(LGSEmbedPE::desc());
  if (pPE.isNull()) {
    LGSTrace("--! Can't get LGSEmbedPE for CreateContext");
    throw OdError("Can't get LGSEmbedPE for CreateContext");
  }
  return pPE->createContext();
  //return LGSCreateContext(LGS_DEFAULT_LINEAR_TOLERANCE, LGS_DEFAULT_ANGULAR_TOLERANCE);
}

void LGSAssocActionPE::DestroyContext(LGSContext iContext)
{
  OdSmartPtr<LGSEmbedPE> pPE = OdDbAssocNetwork::desc()->getX(LGSEmbedPE::desc());
  if (pPE.isNull()) {
    LGSTrace("--! Can't get LGSEmbedPE for DestroyContext");
    throw OdError("Can't get LGSEmbedPE for DestroyContext");
  }
  pPE->destroyContext(iContext);
  //LGSRemoveContext(iContext);
}

void LGSAssocActionPE::evaluate(OdDbAssocAction *action)
{
  if ( action->isKindOf(OdDbAssoc2dConstraintGroup::desc()) )
  {
    ProcessAssoc2dConstraintGroup(OdDbAssoc2dConstraintGroup::cast(action));
    return;
  }

  OdDbAssocNetworkPtr pGlobalNet = OdDbAssocNetwork::cast(action);
  if (pGlobalNet.isNull())
    return;
  ProcessActions(pGlobalNet->getActions());
}

void LGSAssocActionPE::ProcessActions(OdDbObjectIdArray iaActionIds)
{
  for (unsigned int i = 0; i < iaActionIds.length(); i++) {
    OdDbObjectId id = iaActionIds[i];
    OdDbObjectPtr pObj = id.openObject(OdDb::kForWrite);
    if (OdDbAssocNetwork * pNet = OdDbAssocNetwork::cast(pObj)) {
      ProcessActions(pNet->getActions());
      pNet->setStatus(kIsUpToDateAssocStatus, false, true);
    } else if (OdDbAssoc2dConstraintGroup * pConstraintGroup = OdDbAssoc2dConstraintGroup::cast(pObj)) {
      ProcessAssoc2dConstraintGroup(pConstraintGroup);
      pConstraintGroup->setStatus(kIsUpToDateAssocStatus, false, true);
    }
  }
}

void LGSAssocActionPE::ProcessAssoc2dConstraintGroup(OdDbAssoc2dConstraintGroup *pCnGr)
{
  OdArray<OdConstrainedGeometry*> aConstGeom;
  getAllModifiedCurves(pCnGr, aConstGeom);

  OdArray<OdConstrainedGeometry*> aPoint;
  getAllModifiedGeomPoints(pCnGr, aPoint);

  LGSResult modRes;
  LGSTransfTyper commonTf;
  unsigned int i, size = aConstGeom.length();
  if ( size != 0 )
  {
    OdArray<LGSTransfTyper> aTransforTyper;
    aTransforTyper.resize(size);
    LGSTransfTyper* ptrTf = aTransforTyper.asArrayPtr();
    for ( i = 0; i < size; i++ )
      *ptrTf++ = getTransformation(aConstGeom[i]);

    commonTf = getCommonTransformation(aConstGeom, aTransforTyper);

    if ( commonTf.isSimpleTypeTransformation() )
    {
      size = aPoint.length();
      for ( i = 0; i < size; i++ )
        if ( !isFitTransformation(aPoint[i], commonTf) )
        {
          commonTf.setType(commonTf.TransTypeComposite);
          break;
        }
    }
  }
  else
  {
    size = aPoint.length();
    if ( size == 0 )
    {
      if ( CreateContextAndApplyChanges(false, pCnGr) )
        return;
      else
        ODA_FAIL();//set not resolved flag
      return;
    }

    OdArray<LGSTransfTyper> aTransforTyper;
    aTransforTyper.resize(size);
    LGSTransfTyper* ptrTf = aTransforTyper.asArrayPtr();
    for ( i = 0; i < size; i++ )
      *ptrTf++ = getTransformation(aPoint[i]);

    commonTf = getCommonTransformation(aPoint, aTransforTyper);
  }

  if (commonTf.isSimpleTypeTransformation())
    LGSTrace("--> Common transformation is simple");
  else
    LGSTrace("--> Common transformation is complex");

  if ( commonTf.isSimpleTypeTransformation() )//original geom set
  {
    _ctx = CreateContext();
    CreateOriginalLGSModel(pCnGr);

    OdGroupNodeIdArray aNodeId;
    GetAllObjectsWithImplicitPoints(aConstGeom, aPoint, aNodeId);
    size = aNodeId.length();
    LGSGeoObject* lgsMoveRotObj = new LGSGeoObject[size];
    for ( i = 0; i < size; i++ )
      lgsMoveRotObj[i] = FindLGSObject(aNodeId[i]);

    if ( commonTf.getType() == commonTf.TransTypeMove )
    {
      LGSTrace("--> Common transformation type is Move");

      OdGeVector3d moveVec;
      commonTf.getMoveParam(moveVec);
      modRes = LGSMoveObjects(_ctx, size, lgsMoveRotObj, moveVec.x, moveVec.y);

      LGSTrace(OdString("--> LGSMoveObjects returns ") + itoa(modRes, saBuffer, 10));

      if ( modRes == LGS_EVALUATION_PERIOD_EXPIRED )
        throwExpiredEvalException(true);

      if ( modRes != LGS_ERROR )
      {
        UpdateFromLGS(pCnGr);
        DestroyContext(_ctx);
        _aIds.clear();
      }
      else if ( !CreateContextAndApplyChanges(true, pCnGr) )
        ODA_FAIL();
    }
    else
    {
      LGSTrace("--> Common transformation type is Rotate");

      OdGePoint3d rotCenter;
      double ang;
      commonTf.getRotateParam(rotCenter, ang);

      modRes = LGSRotateObjects(_ctx, size, lgsMoveRotObj, rotCenter.x, rotCenter.y, ang);

      LGSTrace(OdString("--> LGSRotateObjects returns ") + itoa(modRes, saBuffer, 10));

      if ( modRes != LGS_ERROR  )
      {
        UpdateFromLGS(pCnGr);
        DestroyContext(_ctx);
        _aIds.clear();
      }
      else if ( !CreateContextAndApplyChanges(true, pCnGr) )
        ODA_FAIL();
    }
    delete[] lgsMoveRotObj;

  }
  else if ( aConstGeom.length() == 1 && aPoint.length() == 0 ) // object specific transform
  {
    LGSTrace("--> Object specific transformation");

    bool needFixUpdateApply = true;
    if ( aConstGeom[0]->isKindOf(OdConstrainedArc::desc()) ) // arc
    {
      OdConstrainedArc* pArc = OdConstrainedArc::cast(aConstGeom[0]);

      if ( MoveCircleArcVertex(pCnGr, pArc) )
        needFixUpdateApply = false;
    }
    else if ( aConstGeom[0]->isKindOf(OdConstrainedBoundedEllipse::desc()) ) // elliptic arc
    {
      OdConstrainedBoundedEllipse* pArc = OdConstrainedBoundedEllipse::cast(aConstGeom[0]);
      if ( MoveEllipseArcVertex(pCnGr, pArc) )
        needFixUpdateApply = false;
    }
    else if ( aConstGeom[0]->isKindOf(OdConstrainedBoundedLine::desc()) ) // line
    {
      OdConstrainedBoundedLine* pLine = OdConstrainedBoundedLine::cast(aConstGeom[0]);
      ODA_ASSERT(!pLine->isRay());

      if ( MoveBoundedLineVertex(pCnGr, pLine) )
        needFixUpdateApply = false;
    }
    else if ( aConstGeom[0]->isKindOf(OdConstrainedSpline::desc()) ) // spline
    {
      OdConstrainedSpline* pSpline = OdConstrainedSpline::cast(aConstGeom[0]);
      if ( MoveSplineVertex(pCnGr, pSpline) )
        needFixUpdateApply = false;
    }
    else if ( aConstGeom[0]->isKindOf(OdConstrainedCircle::desc()) ) // circle
    {
      OdConstrainedCircle* pCircle = OdConstrainedCircle::cast(aConstGeom[0]);

      if ( MoveCircleCenter(pCnGr, pCircle) )
        needFixUpdateApply = false;
    }
    else
      ODA_FAIL();

    if ( needFixUpdateApply )
    {
      if ( FixAndUpdate(false, pCnGr, aConstGeom, aPoint) )
        return;
      else if ( CreateContextAndApplyChanges(false, pCnGr) )
        return;
      else
        ODA_FAIL();//set not resolved flag
    }
  }
  else//fix and update
  {
    if ( FixAndUpdate(false, pCnGr, aConstGeom, aPoint) )
      return;
    else if ( CreateContextAndApplyChanges(false, pCnGr) )
      return;
    else
      ODA_FAIL();//set not resolved flag
  }
}

bool LGSAssocActionPE::MoveBoundedLineVertex(OdDbAssoc2dConstraintGroup * ipConstraintGroup,
                                             OdConstrainedBoundedLine* pLine)
{
  LGSTrace("--> MoveBoundedLineVertex is called");

  OdArray<OdConstrainedImplicitPoint*> aPoints;
  LGSGeoObject moveObjectId = LGS_NULL;
  LGSGeoObject fix1ObjectId = LGS_NULL;

  OdGeVector3d moveVec;

  pLine->getConstrainedImplicitPoints(aPoints);

  OdGeLineSeg3d line = pLine->getOdGeLineSeg3d();
  OdGeLineSeg3d origLine = pLine->getOriginalOdGeLineSeg3d();

  OdConstrainedImplicitPoint::ImplicitPointType moveType, fixType;

  if ( line.startPoint() == origLine.startPoint() &&
       line.endPoint() != origLine.endPoint() )
  {
    moveType = OdConstrainedImplicitPoint::kEndImplicit;
    fixType = OdConstrainedImplicitPoint::kStartImplicit;

    moveVec = line.endPoint() - origLine.endPoint();
  }
  else if ( line.startPoint() != origLine.startPoint() &&
            line.endPoint() == origLine.endPoint() )
  {
    moveType = OdConstrainedImplicitPoint::kStartImplicit;
    fixType = OdConstrainedImplicitPoint::kEndImplicit;

    moveVec = line.startPoint() - origLine.startPoint();
  }
  else
    return false;

  _ctx = CreateContext();
  CreateOriginalLGSModel(ipConstraintGroup);
  unsigned int i, size = aPoints.length();
  for ( i = 0; i < size; i++ )
    if ( aPoints[i]->pointType() == moveType )
    {
      moveObjectId = FindLGSObject(aPoints[i]->nodeId());
    }
    else if ( aPoints[i]->pointType() == fixType )
    {
      fix1ObjectId = FindLGSObject(aPoints[i]->nodeId());
    }

  if ( fix1ObjectId != LGS_NULL && moveObjectId != LGS_NULL )
  {
    LGSCreateFixation(_ctx, fix1ObjectId);
    LGSGeoObject lgsMoveObj[1] = { moveObjectId };
    LGSResult modRes = LGSMoveObjects(_ctx, 1, lgsMoveObj, moveVec.x, moveVec.y);

    LGSTrace(OdString("--> LGSMoveObjects returns ") + itoa(modRes, saBuffer, 10));

    if ( modRes == LGS_EVALUATION_PERIOD_EXPIRED )
      throwExpiredEvalException(true);

    if ( modRes != LGS_ERROR  )
    {
      UpdateFromLGS(ipConstraintGroup);
      DestroyContext(_ctx);
      _aIds.clear();
      return true;
    }
  }

  DestroyContext(_ctx);
  _aIds.clear();
  return false;
}

bool LGSAssocActionPE::MoveCircleArcVertex(OdDbAssoc2dConstraintGroup * ipConstraintGroup,
                                           OdConstrainedArc* pCircleArc)
{
  LGSTrace("--> MoveCircleArcVertex is called");

  OdArray<OdConstrainedImplicitPoint*> aPoints;
  LGSGeoObject moveObjectId = LGS_NULL;
  LGSGeoObject fix1ObjectId = LGS_NULL;
  LGSGeoObject fix2ObjectId = LGS_NULL;

  bool addFixMidPoint = false;

  OdGeVector3d moveVec;

  pCircleArc->getConstrainedImplicitPoints(aPoints);

  OdGeCircArc3d arc = pCircleArc->getOdGeCircArc3d();
  OdGeCircArc3d origArc = pCircleArc->getOriginalOdGeCircArc3d();

  OdConstrainedImplicitPoint::ImplicitPointType moveType, fixType1, fixType2;

  OdGePoint3d sP = arc.startPoint();
  OdGePoint3d osP = origArc.startPoint();

  OdGePoint3d eP = arc.endPoint();
  OdGePoint3d oeP = origArc.endPoint();

  bool startPointsEq = arc.startPoint() == origArc.startPoint();
  bool endPointsEq = arc.endPoint() == origArc.endPoint();

  if ( startPointsEq  && !endPointsEq )
  {
    moveType = OdConstrainedImplicitPoint::kEndImplicit;
    fixType1 = OdConstrainedImplicitPoint::kStartImplicit;
    fixType2 = OdConstrainedImplicitPoint::kDefineImplicit;
    addFixMidPoint = true;

    moveVec = arc.endPoint() - origArc.endPoint();
  }
  else if ( !startPointsEq  && endPointsEq )
  {
    moveType = OdConstrainedImplicitPoint::kStartImplicit;
    fixType1 = OdConstrainedImplicitPoint::kEndImplicit;
    fixType2 = OdConstrainedImplicitPoint::kDefineImplicit;
    addFixMidPoint = true;

    moveVec = arc.startPoint() - origArc.startPoint();
  }
  else if ( startPointsEq  && endPointsEq )
  {
    moveType = OdConstrainedImplicitPoint::kMidImplicit;
    fixType1 = OdConstrainedImplicitPoint::kStartImplicit;
    fixType2 = OdConstrainedImplicitPoint::kEndImplicit;

    OdGeInterval interval;
    origArc.getInterval(interval);
    OdGePoint3d origMidpoint = origArc.evalPoint((interval.lowerBound() + interval.upperBound())/2.0);

    arc.getInterval(interval);
    OdGePoint3d midpoint = origArc.evalPoint((interval.lowerBound() + interval.upperBound())/2.0);

    addFixMidPoint = false;

    moveVec = midpoint - origMidpoint;
  }
  else
    return false;

  _ctx = CreateContext();
  CreateOriginalLGSModel(ipConstraintGroup);
  unsigned int i, size = aPoints.length();
  for ( i = 0; i < size; i++ )
    if ( aPoints[i]->pointType() == moveType )
    {
      moveObjectId = FindLGSObject(aPoints[i]->nodeId());
    }
    else if ( aPoints[i]->pointType() == fixType1 )
    {
      fix1ObjectId = FindLGSObject(aPoints[i]->nodeId());
    }
    else if ( aPoints[i]->pointType() == fixType2 )
    {
      fix2ObjectId = FindLGSObject(aPoints[i]->nodeId());
    }

  if ( fix1ObjectId != LGS_NULL )
    LGSCreateFixation(_ctx, fix1ObjectId);

  if ( fix2ObjectId != LGS_NULL )
    LGSCreateFixation(_ctx, fix2ObjectId);

  if ( addFixMidPoint )
  {
    OdGeInterval interval;
    origArc.getInterval(interval);
    OdGePoint3d midpoint = origArc.evalPoint((interval.lowerBound() + interval.upperBound())/2.0);

    LGSReal coords[3] = { midpoint.x, midpoint.y, midpoint.z };
    LGSPoint pointId = LGSCreatePoint(_ctx, coords);
    if ( pointId != LGS_NULL )
    {
      LGSObject lgsarc = FindLGSObject(pCircleArc->nodeId());
      if ( lgsarc != LGS_NULL )
      {
        LGSConstraint id = LGSCreateIncidence(_ctx, lgsarc, pointId);
        id = LGSCreateFixation(_ctx, pointId);
      }
    }
  }

  if ( moveObjectId != LGS_NULL )
  {
    LGSGeoObject lgsMoveObj[1] = { moveObjectId };
    LGSResult modRes = LGSMoveObjects(_ctx, 1, lgsMoveObj, moveVec.x, moveVec.y);

    LGSTrace(OdString("--> LGSMoveObjects returns ") + itoa(modRes, saBuffer, 10));

    if ( modRes == LGS_EVALUATION_PERIOD_EXPIRED )
      throwExpiredEvalException(true);

    if ( modRes == LGS_ERROR  )
    {
      UpdateFromLGS(ipConstraintGroup);
      DestroyContext(_ctx);
      _aIds.clear();
      return true;
    }
  }

  DestroyContext(_ctx);
  _aIds.clear();
  return false;
}

bool LGSAssocActionPE::MoveEllipseArcVertex(OdDbAssoc2dConstraintGroup * ipConstraintGroup,
                                            OdConstrainedBoundedEllipse* pArc)
{
  LGSTrace("--> MoveEllipseArcVertex is called");

  OdArray<OdConstrainedImplicitPoint*> aPoints;
  LGSGeoObject moveObjectId = LGS_NULL;
  LGSGeoObject fix1ObjectId = LGS_NULL;
  LGSGeoObject fix2ObjectId = LGS_NULL;

  bool addFixMidPoint = false;

  OdGeVector3d moveVec;

  pArc->getConstrainedImplicitPoints(aPoints);

  OdGeEllipArc3d arc = pArc->getOdGeEllipArc3d();
  OdGeEllipArc3d origArc = pArc->getOriginalOdGeEllipArc3d();

  OdConstrainedImplicitPoint::ImplicitPointType moveType, fixType1, fixType2;

  OdGePoint3d sP = arc.startPoint();
  OdGePoint3d osP = origArc.startPoint();

  OdGePoint3d eP = arc.endPoint();
  OdGePoint3d oeP = origArc.endPoint();

  bool startPointsEq = arc.startPoint() == origArc.startPoint();
  bool endPointsEq = arc.endPoint() == origArc.endPoint();

  if ( startPointsEq  && !endPointsEq )
  {
    moveType = OdConstrainedImplicitPoint::kEndImplicit;
    fixType1 = OdConstrainedImplicitPoint::kStartImplicit;
    fixType2 = OdConstrainedImplicitPoint::kDefineImplicit;
    addFixMidPoint = true;

    moveVec = arc.endPoint() - origArc.endPoint();
  }
  else if ( !startPointsEq  && endPointsEq )
  {
    moveType = OdConstrainedImplicitPoint::kStartImplicit;
    fixType1 = OdConstrainedImplicitPoint::kEndImplicit;
    fixType2 = OdConstrainedImplicitPoint::kDefineImplicit;
    addFixMidPoint = true;

    moveVec = arc.startPoint() - origArc.startPoint();
  }
  else if ( startPointsEq  && endPointsEq )
  {
    moveType = OdConstrainedImplicitPoint::kMidImplicit;
    fixType1 = OdConstrainedImplicitPoint::kStartImplicit;
    fixType2 = OdConstrainedImplicitPoint::kEndImplicit;

    OdGeInterval interval;
    origArc.getInterval(interval);
    OdGePoint3d origMidpoint = origArc.evalPoint((interval.lowerBound() + interval.upperBound())/2.0);

    arc.getInterval(interval);
    OdGePoint3d midpoint = origArc.evalPoint((interval.lowerBound() + interval.upperBound())/2.0);

    addFixMidPoint = false;

    moveVec = midpoint - origMidpoint;
  }
  else
    return false;

  _ctx = CreateContext();
  CreateOriginalLGSModel(ipConstraintGroup);
  unsigned int i, size = aPoints.length();
  for ( i = 0; i < size; i++ )
    if ( aPoints[i]->pointType() == moveType )
    {
      moveObjectId = FindLGSObject(aPoints[i]->nodeId());
    }
    else if ( aPoints[i]->pointType() == fixType1 )
    {
      fix1ObjectId = FindLGSObject(aPoints[i]->nodeId());
    }
    else if ( aPoints[i]->pointType() == fixType2 )
    {
      fix2ObjectId = FindLGSObject(aPoints[i]->nodeId());
    }

  if ( fix1ObjectId != LGS_NULL )
    LGSCreateFixation(_ctx, fix1ObjectId);

  if ( fix2ObjectId != LGS_NULL )
    LGSCreateFixation(_ctx, fix2ObjectId);

  if ( addFixMidPoint )
  {
    OdGeInterval interval;
    origArc.getInterval(interval);
    OdGePoint3d midpoint = origArc.evalPoint((interval.lowerBound() + interval.upperBound())/2.0);

    LGSReal coords[3] = { midpoint.x, midpoint.y, midpoint.z };
    LGSPoint pointId = LGSCreatePoint(_ctx, coords);
    if ( pointId != LGS_NULL )
    {
      LGSObject lgsarc = FindLGSObject(pArc->nodeId());
      if ( lgsarc != LGS_NULL )
      {
        LGSConstraint id = LGSCreateIncidence(_ctx, lgsarc, pointId);
        id = LGSCreateFixation(_ctx, pointId);
      }
    }
  }

  if ( moveObjectId != LGS_NULL )
  {
    LGSGeoObject lgsMoveObj[1] = { moveObjectId };
    LGSResult modRes = LGSMoveObjects(_ctx, 1, lgsMoveObj, moveVec.x, moveVec.y);

    LGSTrace(OdString("--> LGSMoveObjects returns ") + itoa(modRes, saBuffer, 10));

    if ( modRes == LGS_EVALUATION_PERIOD_EXPIRED )
      throwExpiredEvalException(true);

    if ( modRes != LGS_ERROR  )
    {
      UpdateFromLGS(ipConstraintGroup);
      DestroyContext(_ctx);
      _aIds.clear();
      return true;
    }
  }

  DestroyContext(_ctx);
  _aIds.clear();
  return false;
}

bool LGSAssocActionPE::MoveSplineVertex(OdDbAssoc2dConstraintGroup * ipConstraintGroup,
                                        OdConstrainedSpline* pSpline)
{
  LGSTrace("--> MoveSplineVertex is called");

  OdGeNurbCurve3d nurb = pSpline->nurbSpline();
  OdGeNurbCurve3d origNurb = pSpline->originalNurbSpline();

  int i, size = nurb.numControlPoints();

  OdGePoint3d nurbPt, originalNurbPt;

  OdIntArray aInt;
  for ( i = 0; i < size; i++ )
  {
    if ( nurb.controlPointAt(i) != origNurb.controlPointAt(i) )
      aInt.push_back(i);
  }

  if ( aInt.length() == 0 )
    return false;

  size = aInt.length();
  OdGeVector3d moveVec = nurb.controlPointAt(aInt[0]) - origNurb.controlPointAt(aInt[0]);
  OdGeVector3d testVec;
  for ( i = 1; i < size; i++ )
  {
    testVec = nurb.controlPointAt(aInt[i]) - origNurb.controlPointAt(aInt[i]);
    if ( moveVec != testVec )
      return false;
  }

  _ctx = CreateContext();
  CreateOriginalLGSModel(ipConstraintGroup);

  LGSGeoObject *lgsMoveObj = new LGSGeoObject[size];

  OdArray<OdConstrainedImplicitPoint*> apImplicitPoints;
  pSpline->getConstrainedImplicitPoints(apImplicitPoints);

  OdConstrainedImplicitPoint* pPoint;
  for ( i = 0; i < size; i++ )
  {
    pPoint = FindImplicitPoint(aInt[i], apImplicitPoints);
    if ( pPoint == NULL )
    {
      DestroyContext(_ctx);
      _aIds.clear();
      return false;
    }

    lgsMoveObj[i] = FindLGSObject(pPoint->nodeId());
  }

  bool res = false;
  LGSResult modRes = LGSMoveObjects(_ctx, size, lgsMoveObj, moveVec.x, moveVec.y);

  LGSTrace(OdString("--> LGSMoveObjects returns ") + itoa(modRes, saBuffer, 10));

  if ( modRes == LGS_EVALUATION_PERIOD_EXPIRED )
    throwExpiredEvalException(true);

  if ( modRes != LGS_ERROR  )
  {
    UpdateFromLGS(ipConstraintGroup);
    res = true;
  }

  DestroyContext(_ctx);
  _aIds.clear();
  delete[] lgsMoveObj;

  return res;
}

bool LGSAssocActionPE::FixAndUpdate(bool removeContextFirst,
                                    OdDbAssoc2dConstraintGroup * ipConstraintGroup,
                                    OdArray<OdConstrainedGeometry*>& aConstGeom,
                                    OdArray<OdConstrainedGeometry*>& aPoint)
{
  LGSTrace("--> FixAndUpdate is called");

  if ( removeContextFirst )
  {
    DestroyContext(_ctx);
    _aIds.clear();
  }

  OdGroupNodeIdArray aNodeId;
  GetAllObjectsWithImplicitPoints(aConstGeom, aPoint, aNodeId);
  unsigned int i, size = aNodeId.length();

  _ctx = CreateContext();
  CreateLGSModel(ipConstraintGroup);

  LGSGeoObject lgsObj;
  for ( i = 0; i < size; i++ )
  {
    lgsObj = FindLGSObject(aNodeId[i]);
    if ( lgsObj != LGS_NULL )
      LGSCreateFixation(_ctx, lgsObj);
  }

  LGSResult modRes = LGSApplyChanges(_ctx);

  LGSTrace(OdString("--> LGSApplyChanges returns ") + itoa(modRes, saBuffer, 10));

  if ( modRes == LGS_EVALUATION_PERIOD_EXPIRED )
    throwExpiredEvalException(true);

  if ( modRes != LGS_ERROR )
  {
    if( isContainsIgnoredNotSatisfiedConstraints() )
    {
      DestroyContext(_ctx);
      _aIds.clear();
      return false;
    }
    UpdateFromLGS(ipConstraintGroup);
    DestroyContext(_ctx);
    _aIds.clear();
    return true;
  }

   DestroyContext(_ctx);
   _aIds.clear();
  return false;
}

bool LGSAssocActionPE::CreateContextAndApplyChanges(bool removeContextFirst, OdDbAssoc2dConstraintGroup * ipConstraintGroup)
{
  LGSTrace("--> CreateContextAndApplyChanges is called");

  if ( removeContextFirst )
  {
    DestroyContext(_ctx);
    _aIds.clear();
  }

  _ctx = CreateContext();
  CreateLGSModel(ipConstraintGroup);

  LGSResult modRes = LGSApplyChanges(_ctx);

  LGSTrace(OdString("--> LGSApplyChanges returns ") + itoa(modRes, saBuffer, 10));

  if ( modRes == LGS_EVALUATION_PERIOD_EXPIRED )
    throwExpiredEvalException(true);

  if ( modRes != LGS_ERROR )
  {
    UpdateFromLGS(ipConstraintGroup);
    DestroyContext(_ctx);
    _aIds.clear();
    return true;
  }

  DestroyContext(_ctx);
  _aIds.clear();
  return false;
}

bool LGSAssocActionPE::isContainsIgnoredNotSatisfiedConstraints()
{
  LGSUInt satQty = LGSGetContextObjectsCount (_ctx, LGS_TYPE_CONSTRAINT, LGS_STATE_SATISFIED);
  LGSUInt constraintQty = LGSGetContextObjectsCount (_ctx, LGS_TYPE_CONSTRAINT, LGS_STATE_ANY);

  return constraintQty != satQty;
}

void LGSAssocActionPE::CreateOriginalLGSModel(OdDbAssoc2dConstraintGroup * ipConstraintGroup)
{
  LGSTrace("--> CreateOriginalLGSModel is called");

  if (ipConstraintGroup == NULL)
    return;
  //create geometries
  OdArray<OdConstrainedGeometry*> aConstGeom;
  OdArray<OdConstrainedRigidSet*> aRigidSet;

  ipConstraintGroup->getConstrainedGeometries(aConstGeom);
  bool alreadyExistObj;
  unsigned int i;
  for (i = 0; i < aConstGeom.length(); i++) {
    alreadyExistObj = false;
    LGSAssocActionPE::LGStoDRX ids;
    OdConstrainedGeometry* pGeom = aConstGeom[i];
    ids._drxId = pGeom->nodeId();
    if (OdConstrainedLine * pLine = OdConstrainedLine::cast(pGeom))
      ids._lgsId = CreateOriginalLGSLine(pLine);
    else if (OdConstrainedCircle* pCircle = OdConstrainedCircle::cast(pGeom))
      ids._lgsId = CreateOriginalLGSCircle(pCircle);
    else if (OdConstrainedEllipse * pEllipse = OdConstrainedEllipse::cast(pGeom))
      ids._lgsId = CreateOriginalLGSEllipse(pEllipse);
    else if ( OdConstrainedSpline * pSpline = OdConstrainedSpline::cast(pGeom) )
      ids._lgsId = CreateOriginalLGSSpline(pSpline);
    else  if (OdConstrainedPoint* pPoint = OdConstrainedPoint::cast(pGeom))
      ids._lgsId = CreateOriginalLGSPoint(pPoint, alreadyExistObj);
    else if ( OdConstrainedRigidSet * pRigidSet = OdConstrainedRigidSet::cast(pGeom) )
    {
      aRigidSet.push_back(pRigidSet);
      continue;
    }
    else
      continue;

    if ( !alreadyExistObj && ids._lgsId != LGS_NULL)
      _aIds.append(ids);
  }

  CreateLGSRigidSets(aRigidSet);

  CreateLGSConstraints(ipConstraintGroup, true);
}

void LGSAssocActionPE::CreateLGSModel(OdDbAssoc2dConstraintGroup * ipConstraintGroup)
{
  LGSTrace("--> CreateLGSModel is called");

  if (ipConstraintGroup == NULL)
    return;
  //create geometries
  OdArray<OdConstrainedGeometry*> aConstGeom;
  OdArray<OdConstrainedRigidSet*> aRigidSet;

  ipConstraintGroup->getConstrainedGeometries(aConstGeom);
  bool alreadyExistObj;
  unsigned int i;
  for (i = 0; i < aConstGeom.length(); i++) {
    alreadyExistObj = false;
    LGStoDRX ids;
    OdConstrainedGeometry* pGeom = aConstGeom[i];
    ids._drxId = pGeom->nodeId();
    if (OdConstrainedLine * pLine = OdConstrainedLine::cast(pGeom))
      if ( pGeom->isReadOnly() )
        ids._lgsId = CreateOriginalLGSLine(pLine);
      else
        ids._lgsId = CreateLGSLine(pLine);
    else if (OdConstrainedCircle* pCircle = OdConstrainedCircle::cast(pGeom))
      if ( pGeom->isReadOnly() )
        ids._lgsId = CreateOriginalLGSCircle(pCircle);
      else
        ids._lgsId = CreateLGSCircle(pCircle);
    else if (OdConstrainedEllipse * pEllipse = OdConstrainedEllipse::cast(pGeom))
      if ( pGeom->isReadOnly() )
        ids._lgsId = CreateOriginalLGSEllipse(pEllipse);
      else
        ids._lgsId = CreateLGSEllipse(pEllipse);
    else  if (OdConstrainedPoint* pPoint = OdConstrainedPoint::cast(pGeom))
      if ( pGeom->isReadOnly() )
        ids._lgsId = CreateOriginalLGSPoint(pPoint, alreadyExistObj);
      else
        ids._lgsId = CreateLGSPoint(pPoint, alreadyExistObj);
    else  if (OdConstrainedSpline* pSpline = OdConstrainedSpline::cast(pGeom))
      if ( pGeom->isReadOnly() )
        ids._lgsId = CreateOriginalLGSSpline(pSpline);
      else
        ids._lgsId = CreateLGSSpline(pSpline);
    else if ( OdConstrainedRigidSet * pRigidSet = OdConstrainedRigidSet::cast(pGeom) )
    {
      aRigidSet.push_back(pRigidSet);
      continue;
    }
    else
      continue;

    if ( !alreadyExistObj && ids._lgsId != LGS_NULL )
      _aIds.append(ids);
  }

  CreateLGSRigidSets(aRigidSet);

  CreateLGSConstraints(ipConstraintGroup, false);
}

void LGSAssocActionPE::CreateLGSConstraints(OdDbAssoc2dConstraintGroup * ipConstraintGroup, bool original)
{
  unsigned int i;

  OdArray<OdGeomConstraint*> apConstraints;
  ipConstraintGroup->getConstraints(apConstraints);
  for (i = 0; i < apConstraints.length(); i++) {
    CreateLGSConstraint(apConstraints[i], original);
  }
}

void LGSAssocActionPE::CreateLGSRigidSets(OdArray<OdConstrainedRigidSet*> &aRigidSet)
{
  OdConstrainedRigidSet* pRigidSet;
  for ( unsigned int i = 0; i < aRigidSet.length(); i++ )
  {
    pRigidSet = aRigidSet[i];
    LGSAssocActionPE::LGStoDRX ids;
    ids._drxId = pRigidSet->nodeId();
    ids._lgsId = CreateLGSRigidSet(pRigidSet);
    if (ids._lgsId != LGS_NULL)
      _aIds.append(ids);
  }
}
LGSObject LGSAssocActionPE::CreateLGSRigidSet(OdConstrainedRigidSet* ipRigidSet)
{
  int geomQty = ipRigidSet->numOfConstrainedGeoms();
  LGSGeoObject *aGeomObj = new LGSGeoObject[geomQty];
  for ( int i = 0; i < geomQty; i++ )
    aGeomObj[i] = FindLGSObject(ipRigidSet->getConstrainedGeomAt(i)->nodeId());

  LGSObject rs = LGSCreateRigidSet(_ctx, geomQty, aGeomObj);
  delete[] aGeomObj;
  return rs;
}

OdConstrainedImplicitPoint* LGSAssocActionPE::FindImplicitPoint(int pointIndex,
                          OdArray<OdConstrainedImplicitPoint*>& apImplicitPoints)
{
  int size = apImplicitPoints.size();
  for ( int i = 0; i < size; i++ )
  {
    if ( apImplicitPoints[i]->pointIndex() == pointIndex )
      return apImplicitPoints[i];
  }

  return NULL;
}

LGSObject LGSAssocActionPE::FindLGSObject(OdConstraintGroupNodeId iId)
{
  int len = _aIds.length();
  if ( len == 0 )
    return LGS_NULL;

  int idx = 0;

  while (_aIds[idx]._drxId != iId) {
    idx++;
    if (idx >= len)
      return LGS_NULL;
  }
  return _aIds[idx]._lgsId;
}

LGSObject LGSAssocActionPE::CreateLGSPoint(OdConstrainedPoint* ipPoint, bool& alreadyExist)
{
  if (ipPoint == NULL)
    return LGS_NULL;

  LGSObject pt = FindLGSObject(ipPoint->nodeId());
  if ( pt != LGS_NULL )
  {
    alreadyExist = true;
    return pt;
  }
  else
    alreadyExist = false;

  OdGePoint3d point = ipPoint->point();
  LGSReal coords[3] = { point.x, point.y, point.z };
  pt = LGSCreatePoint(_ctx, coords);
  return pt;
}

LGSObject LGSAssocActionPE::CreateLGSLine(OdConstrainedLine* ipLine)
{
  if (ipLine == NULL)
    return LGS_NULL;
  OdGePoint3d startPoint = ipLine->pointOnLine();
  LGSReal startCoords[3] = { startPoint.x, startPoint.y, startPoint.z };
  OdGeVector3d dir = ipLine->direction();
  dir.normalize();
  LGSReal dirCoords[3] = {dir.x, dir.y, dir.z};
  return LGSCreateLine(_ctx, startCoords, dirCoords);
}

LGSObject LGSAssocActionPE::CreateLGSCircle(OdConstrainedCircle* ipCircle)
{
  if (ipCircle == NULL)
    return LGS_NULL;
  OdGePoint3d circleCenter = ipCircle->centerPoint();
  LGSReal centerCoords[3] = {circleCenter.x, circleCenter.y, circleCenter.z};
  LGSReal radius = ipCircle->radius();
  return LGSCreateCircle(_ctx, centerCoords, radius);
}

LGSObject LGSAssocActionPE::CreateLGSEllipse(OdConstrainedEllipse* ipEllipse)
{
  if (ipEllipse == NULL)
    return LGS_NULL;
  OdGePoint3d center = ipEllipse->centerPoint();
  LGSReal centerCoords[3] = {center.x, center.y, center.z};
  OdGeVector3d dir = ipEllipse->direction();
  dir.normalize();
  LGSReal dirCoords[3] = {dir.x, dir.y, dir.z};
  LGSReal majorRadius = ipEllipse->majorRadius();
  LGSReal minorRadius = ipEllipse->minorRadius();
  return LGSCreateEllipse(_ctx, centerCoords, dirCoords, majorRadius, minorRadius);
}

LGSObject LGSAssocActionPE::CreateLGSSpline(OdConstrainedSpline* ipSpline, bool original)
{
  if (ipSpline == NULL)
    return LGS_NULL;

  OdArray<OdConstrainedImplicitPoint*> apImplicitPoints;
  ipSpline->getConstrainedImplicitPoints(apImplicitPoints);

  OdGeNurbCurve3d nurbCurve = original ? ipSpline->originalNurbSpline() : ipSpline->nurbSpline();

  bool alreadyExistObj;
  int i, ptQty = ipSpline->numOfDefinePoints();
  int allPtQty = ptQty + (nurbCurve.isClosed() ? 1 : 0);
  LGSGeoObject *iaPoints = new LGSGeoObject[allPtQty];
  OdConstrainedImplicitPoint* pPoint;
  for ( i = 0; i < ptQty; i++ )
  {
    alreadyExistObj = false;
    pPoint = FindImplicitPoint(i, apImplicitPoints);
    if ( pPoint == NULL )
      return LGS_NULL;

    iaPoints[i] = original ? CreateOriginalLGSPoint(pPoint, alreadyExistObj) : CreateLGSPoint(pPoint, alreadyExistObj);

    LGStoDRX ids;
    ids._drxId = pPoint->nodeId();
    ids._lgsId = iaPoints[i];
    if ( !alreadyExistObj && ids._lgsId != LGS_NULL )
      _aIds.append(ids);
  }

  if ( allPtQty > ptQty )
    iaPoints[ptQty] = iaPoints[0];

  int numWeights = nurbCurve.numWeights();
  int numKnots = nurbCurve.numKnots();

  LGSReal *iaKnots = NULL;
  if ( numKnots > 0 )
  {
    if ( numKnots > 0 )
    {
      iaKnots = new LGSReal[numKnots];
      for ( i = 0; i < numKnots; i++ )
        iaKnots[i] = nurbCurve.knotAt(i);
    }
  }

  LGSReal *iaWeights = NULL;
  if ( numWeights == 0 )
  {
    iaWeights = new LGSReal[ptQty];
    for ( i = 0; i < ptQty; i++ )
      iaWeights[i] = 1.0;
  }
  else
  {
    iaWeights = new LGSReal[numWeights];
    for ( i = 0; i < numWeights; i++ )
      iaWeights[i] = nurbCurve.weightAt(i);
  }
  LGSObject res =  LGSCreateNURBS(_ctx, ptQty,
                                        iaPoints,
                                        nurbCurve.degree(),
                                        0/*nurbCurve.isClosed()*/,
                                        iaKnots,
                                        iaWeights);

  delete [] iaPoints;
  delete [] iaKnots;
  delete [] iaWeights;

  return res;
}

LGSObject LGSAssocActionPE::CreateOriginalLGSPoint(OdConstrainedPoint* ipPoint, bool& alreadyExist)
{
  if (ipPoint == NULL)
    return LGS_NULL;

  LGSObject pt = FindLGSObject(ipPoint->nodeId());
  if ( pt != LGS_NULL )
  {
    alreadyExist = true;
    return pt;
  }
  else
    alreadyExist = false;

  OdGePoint3d point = ipPoint->getOriginalPoint();
  LGSReal coords[3] = { point.x, point.y, point.z };
  pt = LGSCreatePoint(_ctx, coords);
  return pt;
}

LGSObject LGSAssocActionPE::CreateOriginalLGSLine(OdConstrainedLine* ipLine)
{
  if (ipLine == NULL)
    return LGS_NULL;

  OdGeLine3d line = ipLine->getOriginalOdGeLine3d();
  OdGePoint3d startPoint = line.evalPoint(0.0);
  LGSReal startCoords[3] = { startPoint.x, startPoint.y, startPoint.z };
  OdGeVector3d dir = line.direction();
  dir.normalize();
  LGSReal dirCoords[3] = {dir.x, dir.y, dir.z};
  return LGSCreateLine(_ctx, startCoords, dirCoords);
}

LGSObject LGSAssocActionPE::CreateOriginalLGSCircle(OdConstrainedCircle* ipCircle)
{
  if (ipCircle == NULL)
    return LGS_NULL;

  OdGeCircArc3d circle = ipCircle->getOriginalOdGeCircArc3d();
  OdGePoint3d circleCenter = circle.center();
  LGSReal centerCoords[3] = {circleCenter.x, circleCenter.y, circleCenter.z};
  LGSReal radius = circle.radius();
  return LGSCreateCircle(_ctx, centerCoords, radius);
}

LGSObject LGSAssocActionPE::CreateOriginalLGSEllipse(OdConstrainedEllipse* ipEllipse)
{
  if (ipEllipse == NULL)
    return LGS_NULL;

  OdGeEllipArc3d ellipse = ipEllipse->getOriginalOdGeEllipArc3d();

  OdGePoint3d center = ellipse.center();
  LGSReal centerCoords[3] = {center.x, center.y, center.z};
  OdGeVector3d dir = ellipse.majorAxis();
  dir.normalize();
  LGSReal dirCoords[3] = {dir.x, dir.y, dir.z};
  LGSReal majorRadius = ellipse.majorRadius();
  LGSReal minorRadius = ellipse.minorRadius();
  return LGSCreateEllipse(_ctx, centerCoords, dirCoords, majorRadius, minorRadius);
}

LGSObject LGSAssocActionPE::CreateOriginalLGSSpline(OdConstrainedSpline* ipSpline)
{
  return CreateLGSSpline(ipSpline, true);
}/*CreateOriginalLGSSpline*/

void LGSAssocActionPE::GetAllObjectsWithImplicitPoints(OdArray<OdConstrainedGeometry*>& aCurveGeom,
                                                       OdArray<OdConstrainedGeometry*>& aPointGeom,
                                                       OdGroupNodeIdArray& aNodeId)
{
  OdConstrainedCurve* curve;
  OdArray<OdConstrainedImplicitPoint*> aPoints;
  unsigned int i, j, sizePoints, size = aCurveGeom.length();
  for ( i = 0; i < size; i++ )
  {
    curve = OdConstrainedCurve::cast(aCurveGeom[i]);
    aNodeId.push_back(curve->nodeId());
    aPoints.clear();
    curve->getConstrainedImplicitPoints(aPoints);
    sizePoints = aPoints.length();
    for ( j = 0; j < sizePoints; j++ )
      aNodeId.push_back(aPoints[j]->nodeId());
  }

  size = aPointGeom.length();
  for ( i = 0; i < size; i++ )
    aNodeId.push_back(aPointGeom[i]->nodeId());
}

void LGSAssocActionPE::CreateLGSConstraint(OdGeomConstraint* ipConstraint, bool original)
{
  if (ipConstraint == NULL)
    return;
  OdArray<OdConstrainedGeometry*> apConsGeoms;
  ipConstraint->getConnectedGeometries(apConsGeoms);
  LGSObject args[4];
  int i;
  int len = __min(apConsGeoms.length(), 4);
  for (i = 0; i < len; i++)
    args[i] = FindLGSObject(apConsGeoms[i]->nodeId());
  for (; i < 4; i++)
    args[i] = LGS_NULL;

  LGSConstraint LGSCnstr = LGS_NULL;
  bool bLGSCnstrNull = false;

  if (OdPointCoincidenceConstraint * pCoincidence = OdPointCoincidenceConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateIncidence(_ctx, args[0], args[1]);
  } else if (OdPointCurveConstraint * pPointCurve = OdPointCurveConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateIncidence(_ctx, args[0], args[1]);
  } else if (OdColinearConstraint * pColinear = OdColinearConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateIncidence(_ctx, args[0], args[1]);
  } else if (OdConcentricConstraint * pConcentric = OdConcentricConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateConcentric(_ctx, args[0], args[1]);
  } else if (OdPerpendicularConstraint * pPerpendicular = OdPerpendicularConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreatePerpendicularity(_ctx, args[0], args[1]);
  } else if (OdHorizontalConstraint * pHorizontal = OdHorizontalConstraint::cast(ipConstraint)) {
    if ( len == 1 )
      LGSCnstr = LGSCreateHorizontality(_ctx, args[0]);
    else //2 points
    {
      LGSObject line = findCommonLine(OdConstrainedPoint::cast(apConsGeoms[0]), OdConstrainedPoint::cast(apConsGeoms[1]));
      LGSCnstr = LGSCreateHorizontality(_ctx, line);
    }
  } else if (OdVerticalConstraint * pVertical = OdVerticalConstraint::cast(ipConstraint)) {
    if ( len == 1 )
      LGSCnstr = LGSCreateVerticality(_ctx, args[0]);
    else//2 points
    {
      LGSObject line = findCommonLine(OdConstrainedPoint::cast(apConsGeoms[0]), OdConstrainedPoint::cast(apConsGeoms[1]));
      LGSCnstr = LGSCreateVerticality(_ctx, line);
    }
  } else if (OdParallelConstraint * pParallel = OdParallelConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateParallelity(_ctx, args[0], args[1]);
  } else if (OdTangentConstraint * pTangent = OdTangentConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateTangency(_ctx, args[0], args[1]);
  } else if(OdEqualCurvatureConstraint * pEqualCur = OdEqualCurvatureConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateEqualCurvature(_ctx, args[0], args[1]);
  }else if (OdEqualRadiusConstraint * pEqualRadius = OdEqualRadiusConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateEqualRadius(_ctx, args[0], args[1]);
  } else if (OdEqualDistanceConstraint * pEqualDistance = OdEqualDistanceConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateEqualDistance(_ctx, args[0], args[1], args[2], args[3]);
  } else if (OdEqualLengthConstraint * pEqualLength = OdEqualLengthConstraint::cast(ipConstraint)) {
    ODA_ASSERT(len == 2);
    LGSObject aLGSPoints[2][2] = {{LGS_NULL, LGS_NULL},{LGS_NULL, LGS_NULL}};
    for (int t = 0; t < 2; t++) {
      OdConstrainedLine * pLn = OdConstrainedLine::cast(apConsGeoms[t]);
      ODA_ASSERT(pLn);
      OdArray<OdConstrainedImplicitPoint*> apImplicitPoints;
      pLn->getConstrainedImplicitPoints(apImplicitPoints);
      unsigned int i = 0;
      while ((aLGSPoints[t][0] == LGS_NULL || aLGSPoints[t][1] == LGS_NULL) && i < apImplicitPoints.length()) {
        OdConstrainedImplicitPoint * pPt = apImplicitPoints[i++];
        switch (pPt->pointType()) {
          case OdConstrainedImplicitPoint::kStartImplicit:
            aLGSPoints[t][0] = FindLGSObject(pPt->nodeId());
            break;
          case OdConstrainedImplicitPoint::kEndImplicit:
            aLGSPoints[t][1] = FindLGSObject(pPt->nodeId());
            break;
        }
      }
      ODA_ASSERT(aLGSPoints[t][0] && aLGSPoints[t][1]);
    }
    LGSCnstr = LGSCreateEqualDistance(_ctx, aLGSPoints[0][0], aLGSPoints[0][1],
                                                        aLGSPoints[1][0], aLGSPoints[1][1]);
  } else if (OdSymmetricConstraint * pSymmetric = OdSymmetricConstraint::cast(ipConstraint)) {
    ODA_ASSERT(len == 3);
    LGSObject ln, arg1, arg2;
    OdConstrainedLine * pLine;
    if (pLine = OdConstrainedLine::cast(apConsGeoms[2])) {
      ln = args[2];
      arg1 = args[0];
      arg2 = args[1];
    } else if (pLine = OdConstrainedLine::cast(apConsGeoms[1])) {
      ln = args[1];
      arg1 = args[0];
      arg2 = args[2];
    } else if (pLine = OdConstrainedLine::cast(apConsGeoms[0])) {
      ln = args[0];
      arg1 = args[1];
      arg2 = args[2];
    } else {
      ODA_ASSERT(false); //error!
    }
    LGSCnstr = LGSCreateSymmetric(_ctx, ln, arg1, arg2);
  } else if (OdMidPointConstraint * pMidPoint = OdMidPointConstraint::cast(ipConstraint)) {
    ODA_ASSERT(len == 2);
    OdConstrainedPoint *pPt = OdConstrainedPoint::cast(apConsGeoms[0]);
    OdConstrainedPoint *pPtS, *pPtE, *pPtC, *pPtMid;

    OdConstrainedCurve *pCurve = OdConstrainedCurve::cast(apConsGeoms[1]);
    ODA_ASSERT(pPt && pCurve);
    LGSObject pt = args[0], start = LGS_NULL, end = LGS_NULL, center = LGS_NULL;
    OdArray<OdConstrainedImplicitPoint*> apImplicitPoints;
    pCurve->getConstrainedImplicitPoints(apImplicitPoints);
    unsigned int i = 0;
    while ( i < apImplicitPoints.length() ) {
      OdConstrainedImplicitPoint * pPt = apImplicitPoints[i++];
      switch (pPt->pointType()) {
        case OdConstrainedImplicitPoint::kStartImplicit:
          start = FindLGSObject(pPt->nodeId());
          pPtS = pPt;
          break;
        case OdConstrainedImplicitPoint::kEndImplicit:
          end = FindLGSObject(pPt->nodeId());
          pPtE = pPt;
          break;
        case OdConstrainedImplicitPoint::kCenterImplicit:
          center = FindLGSObject(pPt->nodeId());
          pPtC = pPt;
          break;
        case OdConstrainedImplicitPoint::kMidImplicit:
          pPtMid = pPt;
          break;
      }
    }
    if ( pCurve->isKindOf(OdConstrainedArc::desc()) )
    {
      OdGePoint3d geS = original ? pPtS->getOriginalPoint() : pPtS->point();
      OdGePoint3d geE = original ? pPtE->getOriginalPoint() : pPtE->point();
      OdGePoint3d geM = original ? pPtMid->getOriginalPoint() : pPtMid->point();
      OdGePoint3d geC = original ? pPtC->getOriginalPoint() : pPtC->point();

      LGSConstraint hdCS = LGSCreateHorizontalDistance(_ctx, geC.x - geS.x, start, center);
      LGSVariable hVarCS = LGSCreateVariable(_ctx, geC.x - geS.x);
      LGSSetConstraintParameter(_ctx, hdCS, hVarCS);

      LGSConstraint vdCS = LGSCreateVerticalDistance(_ctx, geC.y - geS.y, start, center);
      LGSVariable vVarCS = LGSCreateVariable(_ctx, geC.y - geS.y);
      LGSSetConstraintParameter(_ctx, vdCS, vVarCS);

      LGSConstraint hdCM = LGSCreateHorizontalDistance(_ctx, geC.x - geM.x, pt, center);
      LGSVariable hVarCM = LGSCreateVariable(_ctx, geC.x - geM.x);
      LGSSetConstraintParameter(_ctx, hdCM, hVarCM);

      LGSConstraint vdCM = LGSCreateVerticalDistance(_ctx, geC.y - geM.y, pt, center);
      LGSVariable vVarCM = LGSCreateVariable(_ctx, geC.y - geM.y);
      LGSSetConstraintParameter(_ctx, vdCM, vVarCM);

      LGSConstraint hdCE = LGSCreateHorizontalDistance(_ctx, geC.x - geE.x, end, center);
      LGSVariable hVarCE = LGSCreateVariable(_ctx, geC.x - geE.x);
      LGSSetConstraintParameter(_ctx, hdCE, hVarCE);

      LGSConstraint vdCE = LGSCreateVerticalDistance(_ctx, geC.y - geE.y, end, center);
      LGSVariable vVarCE = LGSCreateVariable(_ctx, geC.y - geE.y);
      LGSSetConstraintParameter(_ctx, vdCE, vVarCE);

      LGSVariable vect_var[6] = {hVarCS, vVarCS, hVarCM, vVarCM, hVarCE, vVarCE};

      LGSCreateWhiteBoxEquation(_ctx, 6, vect_var, "x0*x3-x2*x1-x2*x5+x3*x4");

      //original var
      OdGePoint3d geSOrig = pPtS->getOriginalPoint();
      OdGePoint3d geMOrig = pPtMid->getOriginalPoint();
      OdGePoint3d geCOrig = pPtC->getOriginalPoint();

      LGSVariable hVarCS_Orig = LGSCreateVariable(_ctx, geCOrig.x - geSOrig.x);
      LGSVariable vVarCS_Orig = LGSCreateVariable(_ctx, geCOrig.y - geSOrig.y);
      LGSVariable hVarCM_Orig = LGSCreateVariable(_ctx, geCOrig.x - geMOrig.x);
      LGSVariable vVarCM_Orig = LGSCreateVariable(_ctx, geCOrig.y - geMOrig.y);

      LGSVariable vect_var_ineq[4] = {hVarCS_Orig, vVarCS_Orig, hVarCM_Orig, vVarCM_Orig};
      if ( (geCOrig.x - geSOrig.x)*(geCOrig.y - geMOrig.y) - (geCOrig.x - geMOrig.x)*(geCOrig.y - geSOrig.y) >= 0.0 )
        LGSCreateWhiteBoxInequality(_ctx, 4, vect_var_ineq, "x0*x3-x2*x1");
      else
        LGSCreateWhiteBoxInequality(_ctx, 4, vect_var_ineq, "x2*x1-x0*x3");

      LGSCnstr = LGS_NULL;
      bLGSCnstrNull = true;
    }
    else
      LGSCnstr = LGSCreateMidPoint(_ctx, pt, start, end);
  } else if (OdFixedConstraint * pFixed = OdFixedConstraint::cast(ipConstraint)) {
    LGSCnstr = LGSCreateFixation(_ctx, args[0]);
  } else if (OdExplicitConstraint * pDim = OdExplicitConstraint::cast(ipConstraint)) {
    double val;
    pDim->getMeasuredValue(val);
    if (OdDistanceConstraint * pDistance = OdDistanceConstraint::cast(pDim)) {
      switch (pDistance->directionType()) {
        case OdDistanceConstraint::kNotDirected:
          LGSCnstr = LGSCreateDistance(_ctx, val, args[0], args[1]);
          break;
        case OdDistanceConstraint::kFixedDirection:
          {
            OdGeVector3d dir = pDistance->direction();
            dir.normalize();
            OdConstrainedPoint *startPt = OdConstrainedPoint::cast(apConsGeoms[0]);
            OdConstrainedPoint *endPt = OdConstrainedPoint::cast(apConsGeoms[1]);
            OdGePoint3d startPtCoord = original ? startPt->getOriginalPoint() : startPt->point();
            OdGePoint3d endPtCoord = original ? endPt->getOriginalPoint() : endPt->point();

            if ( fabs(dir.x) > OdGeContext::gTol.equalPoint() && fabs(dir.y) < OdGeContext::gTol.equalPoint() )
            {
              double sign = endPtCoord.x - startPtCoord.x > 0 ? 1.0 : -1.0;
              LGSCnstr = LGSCreateHorizontalDistance(_ctx, sign*val, args[0], args[1]);
            }
            else if (dir.x == 0 && dir.y != 0)
            {
              double sign = endPtCoord.y - startPtCoord.y > 0 ? 1.0 : -1.0;
              LGSCnstr = LGSCreateVerticalDistance(_ctx, sign*val, args[0], args[1]);
            }
            else
            {
              LGSReal LGSPoint[3] = { startPtCoord.x, startPtCoord.y, startPtCoord.z };
              LGSReal LGSDir[3] = { dir.y, -dir.x, dir.z };
              LGSLine lineId = LGSCreateLine(_ctx, LGSPoint, LGSDir);
              LGSCreateIncidence(_ctx, args[0], lineId);
              LGSCnstr = LGSCreateDistance(_ctx, val, lineId, args[1]);
              double angOx = acos(LGSDir[0]);
              if ( LGSDir[1] < 0.0 )
                angOx = Oda2PI - angOx;

              LGSCreateAngleOX(_ctx, angOx, lineId);
            }
          }
          break;
        default:
          ODA_ASSERT(false); //other direction types are not supported by LGS!!!
      }
    } else if (Od3PointAngleConstraint * pAngle = Od3PointAngleConstraint::cast(pDim)) {
      OdConstrainedPoint *centerPt = OdConstrainedPoint::cast(apConsGeoms[0]);
      OdConstrainedPoint *rightPt = OdConstrainedPoint::cast(apConsGeoms[1]);
      OdConstrainedPoint *leftPt = OdConstrainedPoint::cast(apConsGeoms[2]);

      OdGePoint3d centerPtCoord = original ? centerPt->getOriginalPoint() : centerPt->point();
      OdGePoint3d rightPtCoord = original ? rightPt->getOriginalPoint(): rightPt->point();
      OdGePoint3d leftPtCoord = original ? leftPt->getOriginalPoint(): leftPt->point();

      LGSReal LGSCenter[3] = { centerPtCoord.x, centerPtCoord.y, centerPtCoord.z };
      LGSReal LGSRight[3] = { rightPtCoord.x, rightPtCoord.y, rightPtCoord.z };
      LGSReal LGSLeft[3] = { leftPtCoord.x, leftPtCoord.y, leftPtCoord.z };

      LGSReal LGSDirRight[3] = { LGSRight[0] - LGSCenter[0], LGSRight[1] - LGSCenter[1], LGSRight[2] - LGSCenter[2] };
      LGSReal LGSDirLeft[3] = { LGSLeft[0] - LGSCenter[0], LGSLeft[1] - LGSCenter[1], LGSLeft[2] - LGSCenter[2] };

      LGSLine lineRightId = LGSCreateLine(_ctx, LGSCenter, LGSDirRight);
      LGSLine lineLeftId = LGSCreateLine(_ctx, LGSCenter, LGSDirLeft);

      LGSCreateIncidence(_ctx, args[0], lineRightId);
      LGSCreateIncidence(_ctx, args[0], lineLeftId);
      LGSCreateIncidence(_ctx, args[1], lineRightId);
      LGSCreateIncidence(_ctx, args[2], lineLeftId);

      val *= pAngle->angleMultiplier();
      switch ( pAngle->sectorType() )
      {
        case OdAngleConstraint::kParallelAntiClockwise:
        break;
        case OdAngleConstraint::kAntiParallelClockwise:
          val = OdaPI - val;
        break;
        case OdAngleConstraint::kParallelClockwise:
          val = Oda2PI - val;
        break;
        case OdAngleConstraint::kAntiParallelAntiClockwise:
          val = val + OdaPI;
        break;
      }

      LGSCnstr = LGSCreateAngle(_ctx, val, lineRightId, lineLeftId);

      LGSReal det = LGSDirRight[0] * LGSDirLeft[1] - LGSDirRight[1] * LGSDirLeft[0];
      LGSReal normRight = sqrt(LGSDirRight[0] * LGSDirRight[0] + LGSDirRight[1] * LGSDirRight[1]);
      LGSReal normLeft = sqrt(LGSDirLeft[0] * LGSDirLeft[0] + LGSDirLeft[1] * LGSDirLeft[1]);
      LGSReal valDist1 = ((normRight > LGS_DEFAULT_ANGULAR_TOLERANCE) ? (det / normRight) : 0.0);
      LGSReal valDist2 =-((normLeft > LGS_DEFAULT_ANGULAR_TOLERANCE) ? (det / normLeft) : 0.0);

      LGSConstraint LGSCnstrDist1 = LGSCreateDistance(_ctx, valDist1, lineRightId, args[2]);
      LGSVariable LGSVarDist1 = LGSCreateVariable(_ctx, valDist1);
      LGSSetConstraintParameter(_ctx, LGSCnstrDist1, LGSVarDist1);

      LGSConstraint LGSCnstrDist2 = LGSCreateDistance(_ctx, valDist2, lineLeftId, args[1]);
      LGSVariable LGSVarDist2 = LGSCreateVariable(_ctx, valDist2);
      LGSSetConstraintParameter(_ctx, LGSCnstrDist2, LGSVarDist2);

      LGSVariable vect_var1[] = {LGSVarDist1};
      LGSVariable vect_var2[] = {LGSVarDist2};

      LGSCreateWhiteBoxInequality(_ctx, 1, ((det >= 0.0) ? vect_var1 : vect_var2), "x0");
      LGSCreateWhiteBoxInequality(_ctx, 1, ((det >= 0.0) ? vect_var2 : vect_var1), "-x0");
    } else if (OdAngleConstraint * pAngle = OdAngleConstraint::cast(pDim)) {
      val *= pAngle->angleMultiplier();
      switch ( pAngle->sectorType() )
      {
        case OdAngleConstraint::kParallelAntiClockwise:
        break;
        case OdAngleConstraint::kAntiParallelClockwise:
          val = OdaPI - val;
        break;
        case OdAngleConstraint::kParallelClockwise:
          val = Oda2PI - val;
        break;
        case OdAngleConstraint::kAntiParallelAntiClockwise:
          val = val + OdaPI;
        break;
      }
      LGSCnstr = LGSCreateAngle(_ctx, val, args[0], args[1]);
    } else if (OdRadiusDiameterConstraint * pRadius = OdRadiusDiameterConstraint::cast(pDim)) {
      switch (pRadius->constrType()) {
        case OdRadiusDiameterConstraint::kCircleRadius:
          LGSCnstr = LGSCreateRadius(_ctx, val, args[0]);
          break;
        case OdRadiusDiameterConstraint::kCircleDiameter:
          LGSCnstr = LGSCreateRadius(_ctx, val/2, args[0]);
          break;
        case OdRadiusDiameterConstraint::kMajorRadius:
          LGSCnstr = LGSCreateMajorRadius(_ctx, val, args[0]);
          break;
        case OdRadiusDiameterConstraint::kMinorRadius:
          LGSCnstr = LGSCreateMinorRadius(_ctx, val, args[0]);
          break;
      }
    }
  }
  else
    return;

  //add help parameters
  if ( len == 0 )
    return;

  OdHelpParameter *pHelpParameter;
  for (i = 0; i < len; i++)
  {
    pHelpParameter = ipConstraint->getConnectedHelpParameterFor(apConsGeoms[i]);
    if ( pHelpParameter == NULL )
      continue;

    LGSReal val = pHelpParameter->getValue();
    LGSModifyHelpParameter(_ctx,
                           LGSCnstr,
                           i,
                           LGS_FINAL_HELP_PARAM,
                           val);
  }

  if ( ipConstraint->isKindOf(OdTangentConstraint::desc()) )
  {
    for ( i = 0; i < len; i++)
    {
      if ( LGSGetObjectType(_ctx, args[i]) == LGS_GEOOBJECT_NURBS )
      {
        LGSUInt attr_val[ 1] = { LGS_ALIGN_KEEP };
        LGSSetObjectAttribute(_ctx, LGSCnstr, LGS_ATTR_ALIGNMENT, attr_val);
        return;
      }
    }
  }
  if ( ipConstraint->isKindOf(OdEqualCurvatureConstraint::desc()) )
  {
    LGSUInt attr_val[ 1] = { LGS_ALIGN_KEEP };
    LGSSetObjectAttribute(_ctx, LGSCnstr, LGS_ATTR_ALIGNMENT, attr_val);
  }
}

bool LGSAssocActionPE::UpdateDRXPoint(OdConstrainedPoint* ipPoint, LGSPoint iPt)
{
  if (ipPoint == NULL)
    return false;

  if ( ipPoint->isKindOf(OdConstrainedImplicitPoint::desc()) )
  {
    OdConstrainedImplicitPoint *pImplPt = OdConstrainedImplicitPoint::cast(ipPoint);
    if ( pImplPt->pointType() != OdConstrainedImplicitPoint::kDefineImplicit )
      return true;
  }

  // update point position
  OdGePoint3d position;
  LGSGetObjectParamValue(_ctx, iPt, LGS_LOCATION_X, &position.x);
  LGSGetObjectParamValue(_ctx, iPt, LGS_LOCATION_Y, &position.y);
  position.z = 0.0;
  ipPoint->setPoint(position);

  return true;
}

bool LGSAssocActionPE::UpdateDRXLine(OdConstrainedLine* ipLine, LGSLine iLn)
{
  if (ipLine == NULL)
    return false;

  OdGeVector3d dir;
  LGSGetObjectParamValue(_ctx, iLn, LGS_DIRECTION_X, &dir.x);
  LGSGetObjectParamValue(_ctx, iLn, LGS_DIRECTION_Y, &dir.y);
  dir.z = 0.0;

  if ( ipLine->isKindOf(OdConstrainedBoundedLine::desc()) )
  {
    OdConstrainedBoundedLine* pBoundedLine = OdConstrainedBoundedLine::cast(ipLine);

    bool haveStart = false, haveEnd = false;
    OdGePoint3d start;
    OdGePoint3d end;
    getStartEndPoints(ipLine, start, end, haveStart, haveEnd);

    if ( !haveStart )
      return false;

    if ( !pBoundedLine->isRay() && !haveEnd )
      return false;

    if ( pBoundedLine->isRay() )
    {
      ipLine->set(start, dir);
      return true;
    }

    pBoundedLine->set(start, end);

    return true;
  }
  else
  {
    OdGePoint3d startPoint;
    LGSGetObjectParamValue(_ctx, iLn, LGS_LOCATION_X, &startPoint.x);
    LGSGetObjectParamValue(_ctx, iLn, LGS_LOCATION_Y, &startPoint.y);
    startPoint.z = 0.0;

    ipLine->set(startPoint, dir);
    return true;
  }
}

bool LGSAssocActionPE::UpdateDRXCircle(OdConstrainedCircle* ipCircle, LGSCircle iCi)
{
  if (ipCircle == NULL)
    return false;

  OdGePoint3d center;
  LGSGetObjectParamValue(_ctx, iCi, LGS_LOCATION_X, &center.x);
  LGSGetObjectParamValue(_ctx, iCi, LGS_LOCATION_Y, &center.y);
  center.z = 0.0;

  LGSReal radius;
  LGSGetObjectParamValue(_ctx, iCi, LGS_RADIUS, &radius);

  if ( ipCircle->isKindOf(OdConstrainedArc::desc()) )
  {
    OdConstrainedArc* pArc = OdConstrainedArc::cast(ipCircle);

    bool haveStart = false, haveEnd = false;
    OdGePoint3d start;
    OdGePoint3d end;

    getStartEndPoints(pArc, start, end, haveStart, haveEnd);
    if ( !haveStart || !haveEnd )
      return false;

    pArc->set(center, radius, start, end);
    return true;
  }
  else
  {
    ipCircle->set(center, radius);
    return true;
  }
}

bool LGSAssocActionPE::UpdateDRXEllipse(OdConstrainedEllipse* ipEllipse, LGSEllipse iEl)
{
  if (ipEllipse == NULL)
    return false;
  OdGePoint3d center;
  OdGeVector3d dir;
  LGSReal majorRadius;
  LGSReal minorRadius;

  LGSGetObjectParamValue(_ctx, iEl, LGS_LOCATION_X, &center.x);
  LGSGetObjectParamValue(_ctx, iEl, LGS_LOCATION_Y, &center.y);

  LGSGetObjectParamValue(_ctx, iEl, LGS_DIRECTION_X, &dir.x);
  LGSGetObjectParamValue(_ctx, iEl, LGS_DIRECTION_Y, &dir.y);

  LGSGetObjectParamValue(_ctx, iEl, LGS_MAJOR_RADIUS, &majorRadius);

  LGSGetObjectParamValue(_ctx, iEl, LGS_MINOR_RADIUS, &minorRadius);

  center.z = 0.0;
  dir.z = 0.0;

  if ( ipEllipse->isKindOf(OdConstrainedBoundedEllipse::desc()) )
  {
    OdConstrainedBoundedEllipse* pArc = OdConstrainedBoundedEllipse::cast(ipEllipse);

    bool haveStart = false, haveEnd = false;
    OdGePoint3d start;
    OdGePoint3d end;

    getStartEndPoints(pArc, start, end, haveStart, haveEnd);
    if ( !haveStart || !haveEnd )
      return false;

    if ( majorRadius > minorRadius )
    {
      pArc->set(center, dir, majorRadius, minorRadius, start, end);
    }
    else
    {
      dir = dir.perpVector();
      dir.normalize();
      pArc->set(center, dir, minorRadius, majorRadius, start, end);
    }
    return true;
  }
  else
  {
    if ( majorRadius > minorRadius )
    {
      ipEllipse->set(center, dir, majorRadius, minorRadius);
    }
    else
    {
      dir = dir.perpVector();
      dir.normalize();
      ipEllipse->set(center, dir, minorRadius, majorRadius);
    }
  }
  return true;
}

bool LGSAssocActionPE::UpdateDRXRigidSet(OdConstrainedRigidSet* ipRigidSet, LGSRigidSet iRs)
{
  if (ipRigidSet == NULL)
    return false;


  LGSTransformation transform;

  if ( LGSGetTransformation(_ctx, iRs, &transform) != LGS_OK )
    return false;

  OdGeMatrix3d matrix = ipRigidSet->transform();

  OdGeMatrix3d rotMatr = OdGeMatrix3d::kIdentity;
  rotMatr.entry[0][0] = transform.rotate[0][0];
  rotMatr.entry[0][1] = transform.rotate[0][1];
  rotMatr.entry[1][0] = transform.rotate[1][0];
  rotMatr.entry[1][1] = transform.rotate[1][1];

  OdGeMatrix3d shiftMatr = OdGeMatrix3d::translation(OdGeVector3d(transform.shift[0], transform.shift[1], 0.0));

  matrix.postMultBy(rotMatr);
  matrix.postMultBy(shiftMatr);

  ipRigidSet->setTransform(matrix);
  return true;
}

void LGSAssocActionPE::UpdateFromLGS(OdDbAssoc2dConstraintGroup * ipConstraintGroup)
{
  if (ipConstraintGroup == NULL)
    return;

  OdConstraintGroupNode* pNode;
  OdConstrainedGeometry* pGeom;
  for (unsigned int i = 0; i < _aIds.length(); i++) {
    pNode = ipConstraintGroup->getGroupNodePtr(_aIds[i]._drxId);

    pGeom = OdConstrainedGeometry::cast(pNode);
    if ( pGeom != NULL )
      pGeom->DoPostEvaluateJob();
    else
      continue;

    if ( UpdateDRXPoint(OdConstrainedPoint::cast(pNode), _aIds[i]._lgsId) )
      continue;

    if ( UpdateDRXLine(OdConstrainedLine::cast(pNode), _aIds[i]._lgsId) )
      continue;

    if ( UpdateDRXCircle(OdConstrainedCircle::cast(pNode), _aIds[i]._lgsId) )
      continue;

    if ( UpdateDRXEllipse(OdConstrainedEllipse::cast(pNode), _aIds[i]._lgsId) )
      continue;

    if ( UpdateDRXRigidSet(OdConstrainedRigidSet::cast(pNode), _aIds[i]._lgsId) )
      continue;
  }
  //ipConstraintGroup->updateDatabaseObjects();
}

void LGSAssocActionPE::crossPoint(const OdGePoint3d& srcPt, const OdGeVector3d& srcVec,
                                  const OdGePoint3d& dstPt, const OdGeVector3d& dstVec,
                                  OdGePoint3d& xPoint)
{
  xPoint = srcPt + srcVec*(dstVec.x*(dstPt.y - srcPt.y) -
                  dstVec.y*(dstPt.x - srcPt.x))/
                  (dstVec.x*srcVec.y - dstVec.y*srcVec.x);
}

double LGSAssocActionPE::oxAngle(const OdGeVector3d& vec)
{
  OdGeVector3d normalVec = vec/vec.length();
  double res = acos(normalVec.x);
  if ( normalVec.y < 0.0 )
    res = Oda2PI - res;

  return res;
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdGePoint3d& srcPtS, const OdGePoint3d& srcPtE,
                                                   const OdGePoint3d& dstPtS, const OdGePoint3d& dstPtE)
{
  OdGeVector3d origVec = srcPtE - srcPtS;
  OdGeVector3d newVec = dstPtE - dstPtS;

  double origLen = origVec.length();
  double len = newVec.length();

  if ( fabs(origLen - len) > OdGeContext::gTol.equalVector() )
  {
    LGSTransfTyper res;
    res.setType(LGSTransfTyper::TransTypeComposite);
    return res;
  }
  else if ( fabs(origLen) < OdGeContext::gTol.equalVector() &&
            fabs(len) < OdGeContext::gTol.equalVector() )
  {
    return LGSTransfTyper(dstPtS - srcPtS);
  }

  if ( origVec.isCodirectionalTo(newVec) )
  {
    return LGSTransfTyper(dstPtS - srcPtS);
  }
  else if ( origVec.isParallelTo(newVec) )
  {
    OdGePoint3d center((dstPtS.x + srcPtS.x)/2.0, (dstPtS.y + srcPtS.y)/2.0, (dstPtS.z + srcPtS.z)/2.0);
    return LGSTransfTyper(center, OdaPI);
  }

  OdGeVector3d startPtVec = dstPtS - srcPtS;
  OdGeVector3d endPtVec = dstPtE - srcPtE;

  OdGePoint3d ptVec1, ptVec2;
  OdGeVector3d vec1, vec2;

  if ( startPtVec.isZeroLength() || endPtVec.isZeroLength() ||
       startPtVec.isParallelTo(endPtVec) )
  {
    ptVec1.set(srcPtS.x, srcPtS.y, srcPtS.z);
    ptVec2.set(dstPtS.x, dstPtS.y, dstPtS.z);

    vec1 = origVec;
    vec2 = newVec;
  }
  else
  {
    ptVec1.set((dstPtS.x + srcPtS.x)/2.0,
               (dstPtS.y + srcPtS.y)/2.0,
               (dstPtS.z + srcPtS.z)/2.0);

    ptVec2.set((dstPtE.x + srcPtE.x)/2.0,
               (dstPtE.y + srcPtE.y)/2.0,
               (dstPtE.z + srcPtE.z)/2.0);

    vec1 = startPtVec.perpVector();
    vec2 = endPtVec.perpVector();
  }

  OdGePoint3d rotCn;
  crossPoint(ptVec1, vec1, ptVec2, vec2, rotCn);

  double rotAng = oxAngle(vec2) - oxAngle(vec1);

  return LGSTransfTyper(rotCn, rotAng);
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedGeometry* pGeom)
{
  if ( pGeom->isKindOf(OdConstrainedCurve::desc()) )
    return getTransformation(OdConstrainedCurve::cast(pGeom));
  else if ( pGeom->isKindOf(OdConstrainedPoint::desc()) )
    return getTransformation(OdConstrainedPoint::cast(pGeom));
  else
    ODA_FAIL_ONCE();

  return LGSTransfTyper();
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedPoint* pPoint)
{
  return getTransformation(pPoint->getOriginalPoint(), pPoint->getOriginalPoint(), pPoint->point(), pPoint->point());
}/*getTransformation*/

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedCurve* pCurve)
{
  if ( pCurve->isKindOf(OdConstrainedBoundedLine::desc()) )
    return getTransformation(OdConstrainedBoundedLine::cast(pCurve));
  else if ( pCurve->isKindOf(OdConstrainedLine::desc()) )
    return getTransformation(OdConstrainedLine::cast(pCurve));
  else if ( pCurve->isKindOf(OdConstrainedArc::desc()) )
    return getTransformation(OdConstrainedArc::cast(pCurve));
  else if ( pCurve->isKindOf(OdConstrainedCircle::desc()) )
    return getTransformation(OdConstrainedCircle::cast(pCurve));
  else if ( pCurve->isKindOf(OdConstrainedBoundedEllipse::desc()) )
    return getTransformation(OdConstrainedBoundedEllipse::cast(pCurve));
  else if ( pCurve->isKindOf(OdConstrainedEllipse::desc()) )
    return getTransformation(OdConstrainedEllipse::cast(pCurve));
  else if ( pCurve->isKindOf(OdConstrainedSpline::desc()) )
    return getTransformation(OdConstrainedSpline::cast(pCurve));
  else
    ODA_FAIL();

  return LGSTransfTyper();
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedLine* pLine)
{
  OdGeLine3d origLine = pLine->getOriginalOdGeLine3d();
  OdGeLine3d line = pLine->getOdGeLine3d();

  OdGeVector3d dir1 = origLine.direction();
  OdGeVector3d dir2 = line.direction();

  dir1.normalize();
  dir2.normalize();
  return getTransformation(origLine.evalPoint(0.0), origLine.evalPoint(0.0) + dir1, line.evalPoint(0.0), line.evalPoint(0.0) + dir2);
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedBoundedLine* pLine)
{
  LGSTransfTyper res = getTransformation(OdConstrainedLine::cast(pLine));

  if ( pLine->isRay() )
    return res;

  OdGeLineSeg3d origLine = pLine->getOriginalOdGeLineSeg3d();
  OdGeLineSeg3d line = pLine->getOdGeLineSeg3d();

  res = getTransformation(origLine.startPoint(), origLine.endPoint(),
                          line.startPoint(), line.endPoint());
  return res;
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedCircle* pCircle)
{
  OdGeCircArc3d origArc = pCircle->getOriginalOdGeCircArc3d();
  OdGeCircArc3d arc = pCircle->getOdGeCircArc3d();

  if ( fabs(origArc.radius() - arc.radius()) > OdGeContext::gTol.equalPoint() )
  {
    LGSTransfTyper res;
    res.setType(LGSTransfTyper::TransTypeComposite);
    return res;
  }

  return getTransformation(origArc.center(), origArc.center(), arc.center(), arc.center());
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedArc* pCircleArc)
{
  OdGeCircArc3d origArc = pCircleArc->getOriginalOdGeCircArc3d();
  OdGeCircArc3d arc = pCircleArc->getOdGeCircArc3d();

  LGSTransfTyper centerTr = getTransformation(OdConstrainedCircle::cast(pCircleArc));
  if ( centerTr.getType() == LGSTransfTyper::TransTypeComposite )
    return centerTr;

  OdGePoint3d sPtOrig = origArc.startPoint();
  OdGePoint3d ePtOrig = origArc.endPoint();
  OdGePoint3d cPtOrig = origArc.center();

  OdGePoint3d sPt = arc.startPoint();
  OdGePoint3d ePt = arc.endPoint();
  OdGePoint3d cPt = arc.center();

  LGSTransfTyper startTr = getTransformation(cPtOrig, sPtOrig, cPt, sPt);

  if ( startTr.isFitTransform(ePtOrig, ePt) )
    return startTr;

  centerTr.setType(LGSTransfTyper::TransTypeComposite);
  return centerTr;
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedEllipse* pEllipse)
{
  OdGeEllipArc3d origEllipse = pEllipse->getOriginalOdGeEllipArc3d();
  OdGeEllipArc3d ellipse = pEllipse->getOdGeEllipArc3d();

  OdGeVector3d majorOrig = origEllipse.majorAxis();
  OdGeVector3d minorOrig = origEllipse.minorAxis();

  OdGeVector3d major = ellipse.majorAxis();
  OdGeVector3d minor = ellipse.minorAxis();

  if ( majorOrig != major || minorOrig != minor )
  {
    LGSTransfTyper res;
    res.setType(LGSTransfTyper::TransTypeComposite);
    return res;
  }
  return getTransformation(origEllipse.center() + majorOrig, origEllipse.center(), ellipse.center() + major, ellipse.center());
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedBoundedEllipse* pEllipseArc)
{
  OdGeEllipArc3d origEllipse = pEllipseArc->getOriginalOdGeEllipArc3d();
  OdGeEllipArc3d ellipse = pEllipseArc->getOdGeEllipArc3d();

  LGSTransfTyper centerTr = getTransformation(OdConstrainedEllipse::cast(pEllipseArc));
  if ( centerTr.getType() == LGSTransfTyper::TransTypeComposite )
    return centerTr;

  OdGePoint3d sPtOrig = origEllipse.startPoint();
  OdGePoint3d ePtOrig = origEllipse.endPoint();

  OdGePoint3d sPt = ellipse.startPoint();
  OdGePoint3d ePt = ellipse.endPoint();

  if ( centerTr.isFitTransform(ePtOrig, ePt) &&
       centerTr.isFitTransform(sPtOrig, sPt))
    return centerTr;

  centerTr.setType(LGSTransfTyper::TransTypeComposite);
  return centerTr;
}

LGSTransfTyper LGSAssocActionPE::getTransformation(const OdConstrainedSpline* pSpline)
{
  OdGeNurbCurve3d nurb = pSpline->nurbSpline();
  OdGeNurbCurve3d origNurb = pSpline->originalNurbSpline();

  LGSTransfTyper tf = getTransformation(origNurb.controlPointAt(0),
                                        origNurb.controlPointAt(1),
                                        nurb.controlPointAt(0),
                                        nurb.controlPointAt(1));

  if ( !isFitTransformation(pSpline, tf) )
    tf.setType(LGSTransfTyper::TransTypeComposite);

  return tf;
}

void LGSAssocActionPE::getAllModifiedGeomPoints(const OdDbAssoc2dConstraintGroup* ipConstraintGroup,
                                                OdArray<OdConstrainedGeometry*>& aConstGeom)
{
  OdArray<OdConstrainedGeometry*> aAllGeom;
  ipConstraintGroup->getConstrainedGeometries(aAllGeom);

  OdConstrainedGeometry* testGeom;
  unsigned int i, len = aAllGeom.length();
  for( i = 0; i < len; i++ )
  {
    testGeom = aAllGeom[i];
    if ( testGeom->isModified() &&
         testGeom->isKindOf(OdConstrainedPoint::desc()) &&
         !testGeom->isKindOf(OdConstrainedImplicitPoint::desc()))
      aConstGeom.append(testGeom);
  }
}

void LGSAssocActionPE::getAllModifiedCurves(const OdDbAssoc2dConstraintGroup* ipConstraintGroup,
                                            OdArray<OdConstrainedGeometry*>& aConstGeom)
{
  OdArray<OdConstrainedGeometry*> aAllGeom;
  ipConstraintGroup->getConstrainedGeometries(aAllGeom);

  OdConstrainedGeometry* testGeom;
  unsigned int i, len = aAllGeom.length();
  for( i = 0; i < len; i++ )
  {
    testGeom = aAllGeom[i];
    if ( testGeom->isKindOf(OdConstrainedCurve::desc()) && testGeom->isModified() )
      aConstGeom.append(testGeom);
  }
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedGeometry* pGeom, LGSTransfTyper& tf)
{
  if ( pGeom->isKindOf(OdConstrainedCurve::desc()) )
    return isFitTransformation(OdConstrainedCurve::cast(pGeom), tf);
  else if ( pGeom->isKindOf(OdConstrainedPoint::desc()) )
    return isFitTransformation(OdConstrainedPoint::cast(pGeom), tf);
  else
    ODA_FAIL_ONCE();

  return false;
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedCurve* pCurve, LGSTransfTyper& tf)
{
  if ( pCurve->isKindOf(OdConstrainedBoundedLine::desc()) )
    return isFitTransformation(OdConstrainedBoundedLine::cast(pCurve), tf);
  else if ( pCurve->isKindOf(OdConstrainedLine::desc()) )
    return isFitTransformation(OdConstrainedLine::cast(pCurve), tf);
  else if ( pCurve->isKindOf(OdConstrainedArc::desc()) )
    return isFitTransformation(OdConstrainedArc::cast(pCurve), tf);
  else if ( pCurve->isKindOf(OdConstrainedCircle::desc()) )
    return isFitTransformation(OdConstrainedCircle::cast(pCurve), tf);
  else if ( pCurve->isKindOf(OdConstrainedBoundedEllipse::desc()) )
    return isFitTransformation(OdConstrainedBoundedEllipse::cast(pCurve), tf);
  else if ( pCurve->isKindOf(OdConstrainedEllipse::desc()) )
    return isFitTransformation(OdConstrainedEllipse::cast(pCurve), tf);
  else if ( pCurve->isKindOf(OdConstrainedSpline::desc()) )
    return isFitTransformation(OdConstrainedSpline::cast(pCurve), tf);
  else
    ODA_FAIL();

  return false;
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedPoint* pPoint, LGSTransfTyper& tf)
{
  return tf.isFitTransform(pPoint->getOriginalPoint(), pPoint->point());
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedLine* pLine, LGSTransfTyper& tf)
{
  OdGeLine3d origLine = pLine->getOriginalOdGeLine3d();
  OdGeLine3d line = pLine->getOdGeLine3d();
  return tf.isFitTransform(origLine.evalPoint(0.0), line.evalPoint(0.0));
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedBoundedLine* pLine, LGSTransfTyper& tf)
{
  if ( pLine->isRay() )
  {
    OdGeRay3d origRay = pLine->getOriginalOdGeRay3d();
    OdGeRay3d ray = pLine->getOdGeRay3d();
    return tf.isFitTransform(origRay.evalPoint(0.0), ray.evalPoint(0.0));
  }
  else
  {
    OdGeLineSeg3d origLine = pLine->getOriginalOdGeLineSeg3d();
    OdGeLineSeg3d line = pLine->getOdGeLineSeg3d();
    return tf.isFitTransform(origLine.startPoint(), line.startPoint()) &&
           tf.isFitTransform(origLine.endPoint(), line.endPoint());
  }
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedCircle* pCircle, LGSTransfTyper& tf)
{
  OdGeCircArc3d origCircle = pCircle->getOriginalOdGeCircArc3d();
  OdGeCircArc3d circle = pCircle->getOdGeCircArc3d();

  return tf.isFitTransform(origCircle.center(), circle.center());
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedArc* pCircleArc, LGSTransfTyper& tf)
{
  OdGeCircArc3d origArc = pCircleArc->getOriginalOdGeCircArc3d();
  OdGeCircArc3d arc = pCircleArc->getOdGeCircArc3d();

  return tf.isFitTransform(origArc.center(), arc.center()) &&
         tf.isFitTransform(origArc.startPoint(), arc.startPoint()) &&
         tf.isFitTransform(origArc.endPoint(), arc.endPoint());
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedEllipse* pEllipse, LGSTransfTyper& tf)
{
  OdGeEllipArc3d origEllipse = pEllipse->getOriginalOdGeEllipArc3d();
  OdGeEllipArc3d ellipse = pEllipse->getOdGeEllipArc3d();

  OdGePoint3d origMagorAxisEnd = origEllipse.center() + origEllipse.majorAxis();
  OdGePoint3d magorAxisEnd = ellipse.center() + ellipse.majorAxis();
  return tf.isFitTransform(origEllipse.center(), ellipse.center()) &&
         tf.isFitTransform(origMagorAxisEnd, magorAxisEnd);
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedBoundedEllipse* pEllipseArc, LGSTransfTyper& tf)
{
  OdGeEllipArc3d origEllipse = pEllipseArc->getOriginalOdGeEllipArc3d();
  OdGeEllipArc3d ellipse = pEllipseArc->getOdGeEllipArc3d();

  OdGePoint3d origMagorAxisEnd = origEllipse.center() + origEllipse.majorAxis();
  OdGePoint3d magorAxisEnd = ellipse.center() + ellipse.majorAxis();
  return tf.isFitTransform(origEllipse.center(), ellipse.center()) &&
         tf.isFitTransform(origMagorAxisEnd, magorAxisEnd) &&
         tf.isFitTransform(origEllipse.startPoint(), ellipse.startPoint()) &&
         tf.isFitTransform(origEllipse.endPoint(), ellipse.endPoint());
}

bool LGSAssocActionPE::isFitTransformation(const OdConstrainedSpline* pSpline, LGSTransfTyper& tf)
{
  OdGeNurbCurve3d nurb = pSpline->nurbSpline();
  OdGeNurbCurve3d origNurb = pSpline->originalNurbSpline();

  int size = nurb.numControlPoints();
  for ( int i = 0; i < size; i++ )
  {
    if ( !tf.isFitTransform(origNurb.controlPointAt(i), nurb.controlPointAt(i)) )
      return false;
  }
  return true;
}

LGSTransfTyper LGSAssocActionPE::getCommonTransformation(const OdArray<OdConstrainedGeometry*>& aConstGeom,
                                                         const OdArray<LGSTransfTyper>& aTransforTyper)
{
  LGSTransfTyper res;
  OdConstraintGroupNodeId resGeomId = 0;

  //find first rotate transformation
  unsigned int i, len = aConstGeom.length();

  for ( i = 0; i < len; i++ )
    if ( LGSTransfTyper::TransTypeRotate == aTransforTyper[i].getType() )
    {
      res = aTransforTyper[i];
      resGeomId = aConstGeom[i]->nodeId();
    }

  if ( resGeomId == 0 )
  {
    res = aTransforTyper[0];
    resGeomId = aConstGeom[0]->nodeId();
  }

  OdConstrainedGeometry* testGeom;
  for ( i = 0; i < len; i++ )
  {
    testGeom = aConstGeom[i];
    if ( testGeom->nodeId() == resGeomId )
      continue;

    if ( testGeom->isKindOf(OdConstrainedCurve::desc()) )
    {
      if ( !isFitTransformation(OdConstrainedCurve::cast(testGeom), res) )
      {
        res.setType(LGSTransfTyper::TransTypeComposite);
        return res;
      }
    }
    else if ( testGeom->isKindOf(OdConstrainedPoint::desc()) )
    {
      if ( !isFitTransformation(OdConstrainedPoint::cast(testGeom), res) )
      {
        res.setType(LGSTransfTyper::TransTypeComposite);
        return res;
      }
    }
    else
      ODA_FAIL_ONCE();
  }

  return res;
}

LGSObject LGSAssocActionPE::findCommonLine(const OdConstrainedPoint* pPointF, const OdConstrainedPoint* pPointS)
{
  OdArray<OdConstrainedGeometry*> arrConGeomF;
  pPointF->getConnectedGeometries(arrConGeomF);

  OdArray<OdConstrainedGeometry*> arrConGeomS;
  pPointS->getConnectedGeometries(arrConGeomS);

  unsigned int i, j;
  for ( i = 0; i < arrConGeomF.length(); i++ )
  {
    for ( j = 0; j < arrConGeomS.length(); j++ )
    {
      if ( arrConGeomF[i]->nodeId() == arrConGeomS[j]->nodeId() )
      {
        if ( arrConGeomF[i]->isKindOf(OdConstrainedBoundedLine::desc()) ||
                arrConGeomF[i]->isKindOf(OdConstrained2PointsConstructionLine::desc()) )
          return FindLGSObject(arrConGeomF[i]->nodeId());
      }
    }
  }

  return LGS_NULL;
}

void LGSAssocActionPE::getStartEndPoints(OdConstrainedCurve* pCurve, OdGePoint3d &start, OdGePoint3d &end, bool &haveStart, bool &haveEnd)
{
  OdArray<OdConstrainedImplicitPoint*> arrImplPt;
  pCurve->getConstrainedImplicitPoints(arrImplPt);

  OdConstraintGroupNodeId startPtId = OdConstraintGroupNode::kNullGroupNodeId;
  OdConstraintGroupNodeId endPtId = OdConstraintGroupNode::kNullGroupNodeId;

  unsigned int size = arrImplPt.length();
  for ( unsigned int i = 0; i < size; i++ )
  {
    if ( arrImplPt[i]->pointType() == OdConstrainedImplicitPoint::kStartImplicit )
      startPtId = arrImplPt[i]->nodeId();
    else if ( arrImplPt[i]->pointType() == OdConstrainedImplicitPoint::kEndImplicit )
      endPtId = arrImplPt[i]->nodeId();
  }

  haveStart = getPoint(startPtId, start);
  haveEnd = getPoint(endPtId, end);
}

bool LGSAssocActionPE::getPoint(OdConstraintGroupNodeId id, OdGePoint3d &pt)
{
  if ( id == OdConstraintGroupNode::kNullGroupNodeId )
    return false;

  LGSObject lgsId = FindLGSObject(id);
  if ( lgsId == LGS_NULL )
    return false;

  LGSGetObjectParamValue(_ctx, lgsId, LGS_LOCATION_X, &pt.x);
  LGSGetObjectParamValue(_ctx, lgsId, LGS_LOCATION_Y, &pt.y);
  pt.z = 0.0;

  return true;
}

void LGSAssocActionPE::throwExpiredEvalException(bool removeContext)
{
  if ( removeContext )
  {
    DestroyContext(_ctx);
    _aIds.clear();
  }

  throw OdError("The evaluation period is expired");
}
