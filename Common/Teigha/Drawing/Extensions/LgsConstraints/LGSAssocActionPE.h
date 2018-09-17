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
#ifndef LGSAssocActionPE_INCLUDED_
#define LGSAssocActionPE_INCLUDED_

#include "LGSConstraintsExport.h"
#include "AssocActionPE.h"
#include "OdConstrainedGeometry.h"
#include "OdConstraintGroupNode.h"
#include "LGSPublicAPI.h"
#include "LGSTransfTyper.h"
#include "OdArray.h"

#include "TD_PackPush.h"

typedef OdArray<OdConstraintGroupNodeId,
    OdMemoryAllocator<OdConstraintGroupNodeId> > OdGroupNodeIdArray;
/** <group Extension_Classes>
*/
class LGSCONSTRAINTS_EXPORT LGSAssocActionPE : public AssocActionPE
{
public:
  ODRX_DECLARE_MEMBERS(LGSAssocActionPE);

  LGSAssocActionPE();
  virtual ~LGSAssocActionPE(){}

  virtual void evaluate(OdDbAssocAction *action);

private:

  struct LGStoDRX {
    LGSObject _lgsId;
    OdConstraintGroupNodeId _drxId;
  };

  LGSContext CreateContext();
  void DestroyContext(LGSContext iContext);

  void CreateLGSModel(OdDbAssoc2dConstraintGroup * ipConstraintGroup);
  void CreateOriginalLGSModel(OdDbAssoc2dConstraintGroup * ipConstraintGroup);

  void ProcessActions(OdDbObjectIdArray iaActionIds);

  void ProcessAssoc2dConstraintGroup(OdDbAssoc2dConstraintGroup *pCnGr);

  LGSObject CreateLGSPoint(OdConstrainedPoint* ipPoint, bool& alreadyExist);
  LGSObject CreateLGSLine(OdConstrainedLine* ipLine);
  LGSObject CreateLGSCircle(OdConstrainedCircle* ipCircle);
  LGSObject CreateLGSEllipse(OdConstrainedEllipse* ipEllipse);
  LGSObject CreateLGSSpline(OdConstrainedSpline* ipSpline, bool original = false);

  LGSObject CreateOriginalLGSPoint(OdConstrainedPoint* ipPoint, bool& alreadyExist);
  LGSObject CreateOriginalLGSLine(OdConstrainedLine* ipLine);
  LGSObject CreateOriginalLGSCircle(OdConstrainedCircle* ipCircle);
  LGSObject CreateOriginalLGSEllipse(OdConstrainedEllipse* ipEllipse);
  LGSObject CreateOriginalLGSSpline(OdConstrainedSpline* ipSpline);

  void GetAllObjectsWithImplicitPoints(OdArray<OdConstrainedGeometry*>& aCurveGeom,
                                       OdArray<OdConstrainedGeometry*>& aPointGeom,
                                       OdGroupNodeIdArray& aNodeId);

  OdConstrainedImplicitPoint* FindImplicitPoint(int pointIndex,
                    OdArray<OdConstrainedImplicitPoint*>& apImplicitPoints);
  LGSObject FindLGSObject(OdConstraintGroupNodeId iId);
  LGSObject CreateLGSRigidSet(OdConstrainedRigidSet* ipRigidSet);

  void CreateLGSRigidSets(OdArray<OdConstrainedRigidSet*> & aRigidSet);
  void CreateLGSConstraints(OdDbAssoc2dConstraintGroup * ipConstraintGroup, bool original);
  void CreateLGSConstraint(OdGeomConstraint* ipConstraint, bool original);

  bool FixAndUpdate(bool removeContextFirst,
                    OdDbAssoc2dConstraintGroup * ipConstraintGroup,
                    OdArray<OdConstrainedGeometry*>& aConstGeom,
                    OdArray<OdConstrainedGeometry*>& aPoint);

  bool CreateContextAndApplyChanges(bool removeContextFirst, OdDbAssoc2dConstraintGroup * ipConstraintGroup);

  bool isContainsIgnoredNotSatisfiedConstraints();

  bool MoveBoundedLineVertex(OdDbAssoc2dConstraintGroup * ipConstraintGroup, OdConstrainedBoundedLine* pLine);
  bool MoveCircleArcVertex(OdDbAssoc2dConstraintGroup * ipConstraintGroup, OdConstrainedArc* pCircleArc);
  bool MoveEllipseArcVertex(OdDbAssoc2dConstraintGroup * ipConstraintGroup, OdConstrainedBoundedEllipse* pArc);
  bool MoveSplineVertex(OdDbAssoc2dConstraintGroup * ipConstraintGroup, OdConstrainedSpline* pSpline);

  void UpdateFromLGS(OdDbAssoc2dConstraintGroup * ipConstraintGroup);
  bool UpdateDRXPoint(OdConstrainedPoint* ipPoint, LGSPoint iPt);
  bool UpdateDRXLine(OdConstrainedLine* ipLine, LGSLine iLn);
  bool UpdateDRXCircle(OdConstrainedCircle* ipCircle, LGSCircle iCi);
  bool UpdateDRXEllipse(OdConstrainedEllipse* ipEllipse, LGSEllipse iEl);
  bool UpdateDRXRigidSet(OdConstrainedRigidSet* ipRigidSet, LGSRigidSet iRs);


  void crossPoint(const OdGePoint3d& srcPt, const OdGeVector3d& srcVec,
                  const OdGePoint3d& dstPt, const OdGeVector3d& dstVec,
                  OdGePoint3d& xPoint);

  double oxAngle(const OdGeVector3d& vec);

  void getAllModifiedGeomPoints(const OdDbAssoc2dConstraintGroup* ipConstraintGroup,
                                OdArray<OdConstrainedGeometry*>& aConstGeom);

  void getAllModifiedCurves(const OdDbAssoc2dConstraintGroup* ipConstraintGroup,
                            OdArray<OdConstrainedGeometry*>& aConstGeom);

  LGSTransfTyper getTransformation(const OdGePoint3d& srcPtS, const OdGePoint3d& srcPtE,
                                       const OdGePoint3d& dstPtS, const OdGePoint3d& dstPtE);

  LGSTransfTyper getTransformation(const OdConstrainedGeometry* pGeom);
  LGSTransfTyper getTransformation(const OdConstrainedCurve* pCurve);
  LGSTransfTyper getTransformation(const OdConstrainedPoint* pPoint);
  LGSTransfTyper getTransformation(const OdConstrainedLine* pLine);
  LGSTransfTyper getTransformation(const OdConstrainedBoundedLine* pLine);
  LGSTransfTyper getTransformation(const OdConstrainedCircle* pCircle);
  LGSTransfTyper getTransformation(const OdConstrainedArc* pCircleArc);
  LGSTransfTyper getTransformation(const OdConstrainedEllipse* pEllipse);
  LGSTransfTyper getTransformation(const OdConstrainedBoundedEllipse* pEllipseArc);
  LGSTransfTyper getTransformation(const OdConstrainedSpline* pSpline);

  bool isFitTransformation(const OdConstrainedGeometry* pGeom, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedCurve* pCurve, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedPoint* pPoint, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedLine* pLine, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedBoundedLine* pLine, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedCircle* pCircle, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedArc* pCircleArc, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedEllipse* pEllipse, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedBoundedEllipse* pEllipseArc, LGSTransfTyper& tf);
  bool isFitTransformation(const OdConstrainedSpline* pSpline, LGSTransfTyper& tf);

  LGSTransfTyper getCommonTransformation(const OdArray<OdConstrainedGeometry*>& aConstGeom,
                                           const OdArray<LGSTransfTyper>& aTransforTyper);

  LGSObject findCommonLine(const OdConstrainedPoint* pPointF, const OdConstrainedPoint* pPointS);

  void getStartEndPoints(OdConstrainedCurve* pCurve, OdGePoint3d &start, OdGePoint3d &end, bool &haveStart, bool &haveEnd);
  bool getPoint(OdConstraintGroupNodeId id, OdGePoint3d &pt);

  void throwExpiredEvalException(bool removeContext);
  LGSContext _ctx;
  OdArray<LGStoDRX> _aIds;
};

#include "TD_PackPop.h"

#endif
