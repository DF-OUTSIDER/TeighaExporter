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

/************************************************************************/
/* DbConstraints.cpp: Defines the entry points for the DLL           */
/************************************************************************/
#include "OdaCommon.h"
#include "RxSysRegistry.h"

#include "RxDynamicModule.h"
#include "StaticRxObject.h"

#include "DbEvalVariant.h"
#include "AssocActionPE.h"

#include "DbAssocPersSubentId.h"
#include "DbAssocEdgePersSubentId.h"
#include "DbAssocGeomDependency.h"
#include "DbBlockReference.h"
#include "OdConstraintGroupNode.h"
#include "OdConstrainedGeometry.h"
#include "OdImplicitConstr.h"
#include "OdExplicitConstr.h"
#include "DbAssocNetwork.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocVariable.h"
#include "DbAssocValueDependency.h"
#include "DbAssocValueProviderPE.h"
#include "DbAssocActionBody.h"
#include "DbAssocActionParam.h"
#include "DbAssocArrayActionBody.h"
#include "DbAssocArrayModifyActionBody.h"
#include "DbAssocArrayCommonParameters.h"
#include "DbAssocArrayPathParameters.h"
#include "DbAssocArrayPolarParameters.h"
#include "DbAssocArrayRectangularParameters.h"
#include "DbAssocArrayItem.h"
#include "DbAssocArrayParameters.h"
#include "DbAssocParamBasedActionBody.h"
#include "DbAssocDimDependencyBody.h"
#include "DbBlockParameterDependencyBody.h"
#include "DbCenterMarkActionBody.h"
#include "DbCenterLineActionBody.h"
#include "DbViewport.h"
#include "OdDbAssocDependencyPE.h"
#include "OdDbAssocLinePersSubentIdPE.h"
#include "OdDbAssocArcPersSubentIdPE.h"
#include "OdDbAssocCirclePersSubentIdPE.h"
#include "OdDbAssocEllArcPersSubentIdPE.h"
#include "OdDbAssocPointPersSubentIdPE.h"
#include "OdDbAssocRayPersSubentIdPE.h"
#include "OdDbAssocSplinePersSubentIdPE.h"
#include "OdDbAssocPolylinePersSubentIdPE.h"
#include "OdDbAssoc3dSolidPersSubentIdPE.h"
#include "DbAssocNetworkDeepCloneAux.h"
#include "OdDbAssocXLinePersSubentIdPE.h"
#include "OdDbAssocBlockReferncePersSubentIdPE.h"
#include "OdDbAssocViewPortPersSubentIdPE.h"
#include "DbAssocRotatedDimActionBody.h"
#include "DbAssocAlignedDimActionBody.h"
#include "DbAssocMLeaderActionBody.h"
#include "DbAssoc3PointAngularDimActionBody.h"
#include "DbAssocOrdinateDimActionBody.h"
#include "DbAssocExtrudedSurfaceActionBody.h"
#include "DbAssocLoftedSurfaceActionBody.h"
#include "DbAssocRevolvedSurfaceActionBody.h"
#include "DbAssocSweptSurfaceActionBody.h"
#include "DbAssocPlaneSurfaceActionBody.h"
#include "DbAssocNetworkSurfaceActionBody.h"
#include "DbAssocPatchSurfaceActionBody.h"
#include "DbAssocBlendSurfaceActionBody.h"
#include "DbAssocEdgeFilletActionBody.h"
#include "DbAssocRestoreEntityStateActionBody.h"
#include "DbAssocTrimSurfaceActionBody.h"
#include "DbAssocExtendSurfaceActionBody.h"
#include "DbAssocFilletSurfaceActionBody.h"
#include "DbAssocOffsetSurfaceActionBody.h"
#include "DbAssocSurfacesParameters.h"
#include "OdDbAssocSurfacePersSubentIdPE.h"
#include "DbExtrudedSurface.h"
#include "DbPlaneSurface.h"
#include "DbSweptSurface.h"
#include "DbLoftedSurface.h"
#include "DbRevolvedSurface.h"

#include "DbLine.h"
#include "DbArc.h"
#include "DbCircle.h"
#include "DbEllipse.h"
#include "DbPoint.h"
#include "DbRay.h"
#include "DbSpline.h"
#include "DbPolyline.h"
#include "DbXline.h"
#include "DbGeomRef.h"
#include "DynamicBlocks/DbConstraintsInterfacePE.h"

class DbConstraints_Module : public OdRxModule
{
  public:
	void initApp()
  {
    ::odrxSafeLoadApp(OdDbEntitiesAppName)->addRef();
    ///////////////////////////////
    OdDbEvalVariant::rxInit();
    OdDbGeomRef::rxInit();
    OdDbSubentRef::rxInit();
    OdDbFaceRef::rxInit();
    OdDbEdgeRef::rxInit();
    OdDbVertexRef::rxInit();
    OdDbPathRef::rxInit();
    OdDbVectorRef::rxInit();
    OdDbCoordSystemRef::rxInit();
    AssocActionPE::rxInit();
    ///////////////////////////////

    OdDbAssocPersSubentId::rxInit();
    OdDbAssocSingleEdgePersSubentId::rxInit();
    OdDbAssocSimplePersSubentId::rxInit();
    OdDbAssocEdgePersSubentId::rxInit();
    OdDbAssocIndexPersSubentId::rxInit();
    OdDbAssocAsmBasedEntityPersSubentId::rxInit();

    OdDbAssocDependency::rxInit();
    OdDbAssocGeomDependency::rxInit();

    OdConstraintGroupNode::rxInit();
    OdHelpParameter::rxInit();
    OdConstrainedGeometry::rxInit();
    OdConstrainedPoint::rxInit();
    OdConstrainedImplicitPoint::rxInit();
    OdConstrainedCurve::rxInit();
    OdConstrainedCircle::rxInit();
    OdConstrainedArc::rxInit();
    OdConstrainedEllipse::rxInit();
    OdConstrainedBoundedEllipse::rxInit();
    OdConstrainedLine::rxInit();
    OdConstrainedBoundedLine::rxInit();
    OdConstrainedConstructionLine::rxInit();
    OdConstrained2PointsConstructionLine::rxInit();
    OdConstrainedDatumLine::rxInit();
    OdConstrainedSpline::rxInit();
    OdConstrainedRigidSet::rxInit();

    OdGeomConstraint::rxInit();
    OdPerpendicularConstraint::rxInit();
    OdNormalConstraint::rxInit();
    OdPointCurveConstraint::rxInit();
    OdColinearConstraint::rxInit();
    OdPointCoincidenceConstraint::rxInit();
    OdConcentricConstraint::rxInit();
    OdCenterPointConstraint::rxInit();
    OdTangentConstraint::rxInit();
    OdEqualRadiusConstraint::rxInit();
    OdEqualDistanceConstraint::rxInit();
    OdEqualLengthConstraint::rxInit();
    OdParallelConstraint::rxInit();
    OdHorizontalConstraint::rxInit();
    OdVerticalConstraint::rxInit();
    OdEqualCurvatureConstraint::rxInit();
    OdSymmetricConstraint::rxInit();
    OdMidPointConstraint::rxInit();
    OdFixedConstraint::rxInit();
    OdCompositeConstraint::rxInit();
    OdG2SmoothConstraint::rxInit();
#if 0 // TODO
    OdEqualHelpParameterConstraint::rxInit();
#endif

    OdDbAssocAction::rxInit();
    OdDbAssocNetwork::rxInit();
    OdDbAssoc2dConstraintGroup::rxInit();

    //
    OdDbAssocValueProviderPE::rxInit();
    OdDbAssocVariable::rxInit();
    OdDbAssocVariableValueProviderPE::rxInit();
    OdDbAssocValueDependency::rxInit();
    OdExplicitConstraint::rxInit();
    OdDistanceConstraint::rxInit();
    OdAngleConstraint::rxInit();
    Od3PointAngleConstraint::rxInit();
    OdRadiusDiameterConstraint::rxInit();
    OdDbAssocActionBody::rxInit();
    OdDbAssocDependencyBody::rxInit();
    OdDbAssocDimDependencyBodyBase::rxInit();
    OdDbBlockParameterDependencyBody::rxInit();
    OdDbAssocDimDependencyBody::rxInit();
    OdDbAssocActionParam::rxInit();
    OdDbAssocAsmBodyActionParam::rxInit();
    OdDbAssocCompoundActionParam::rxInit();
	OdDbAssocPathActionParam::rxInit();
    OdDbAssocPointRefActionParam::rxInit();
    OdDbAssocOsnapPointRefActionParam::rxInit();
    OdDbAssocEdgeActionParam::rxInit();
    OdDbAssocFaceActionParam::rxInit();
	OdDbAssocTrimmingBodyActionParam::rxInit();
    OdDbAssocObjectActionParam::rxInit();
    OdDbAssocVertexActionParam::rxInit();
    OdDbAssocParamBasedActionBody::rxInit();
    OdDbAssocArrayActionBody::rxInit();
	OdDbSmartCenterActionBodyBase::rxInit();
	OdDbSmartCenterActionBody::rxInit();
	OdDbCenterMarkActionBody::rxInit();
	OdDbCenterLineActionBody::rxInit();
  OdDbAssocAnnotationActionBody::rxInit();
  OdDbAssocRotatedDimActionBody::rxInit();
  OdDbAssocAlignedDimActionBody::rxInit();
  OdDbAssocMLeaderActionBody::rxInit();
  OdDbAssoc3PointAngularDimActionBody::rxInit();
  OdDbAssocOrdinateDimActionBody::rxInit();
  OdDbAssocSurfaceActionBody::rxInit();
  OdDbAssocPathBasedSurfaceActionBody::rxInit();
  OdDbAssocTrimSurfaceActionBody::rxInit();
  OdDbAssocExtrudedSurfaceActionBody::rxInit();
  OdDbAssocExtendSurfaceActionBody::rxInit();
  OdDbAssocLoftedSurfaceActionBody::rxInit();
  OdDbAssocRevolvedSurfaceActionBody::rxInit();
  OdDbAssocSweptSurfaceActionBody::rxInit();
	OdDbAssocPlaneSurfaceActionBody::rxInit();
	OdDbAssocNetworkSurfaceActionBody::rxInit();
	OdDbAssocPatchSurfaceActionBody::rxInit();
	OdDbAssocBlendSurfaceActionBody::rxInit();
	OdDbAssocOffsetSurfaceActionBody::rxInit();
	OdDbAssocFilletSurfaceActionBody::rxInit();
    OdDbAssocArrayModifyActionBody::rxInit();
    OdDbAssocArrayParameters::rxInit();
    OdDbAssocArrayCommonParameters::rxInit();
    OdDbAssocArrayPathParameters::rxInit();
    OdDbAssocArrayPolarParameters::rxInit();
    OdDbAssocArrayRectangularParameters::rxInit();
    OdDbAssocArrayModifyParameters::rxInit();
    OdDbAssocArrayItem::rxInit();
    OdDbSmartCenterCommonParameters::rxInit();
    OdDbSmartCenterMarkParameters::rxInit();
    OdDbSmartCenterLineParameters::rxInit();
    OdDbAssocLoftParameters::rxInit();
	OdDbAssocRestoreEntityStateActionBody::rxInit();

    OdDbAssocDependencyPE::rxInit();

    OdDbAssocVariable::desc()->addX(OdDbAssocValueProviderPE::desc(), &_assocVariableValueProviderPE);

    //add OdDbAssocPersSubentIdPE
    OdDbAssocPersSubentIdPE::rxInit();
    OdDbAssocCurvePersSubentIdPE::rxInit();
    OdDbAssocLinePersSubentIdPE::rxInit();
    OdDbAssocArcPersSubentIdPE::rxInit();
    OdDbAssocCirclePersSubentIdPE::rxInit();
    OdDbAssocEllArcPersSubentIdPE::rxInit();

    OdDbAssocPointBasedPersSubentIdPE::rxInit();
    OdDbAssocRayPersSubentIdPE::rxInit();
    OdDbAssocSplinePersSubentIdPE::rxInit();
    OdDbAssocPolylinePersSubentIdPE::rxInit();
    OdDbAssoc3dSolidPersSubentIdPE::rxInit();
    OdDbAssocBlockReferncePersSubentIdPE::rxInit();
    OdDbAssocViewPortPersSubentIdPE::rxInit();
    OdDbAssocXLinePersSubentIdPE::rxInit();
    OdDbAssocSurfacePersSubentIdPE::rxInit();

    OdDbLine::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocLinePersSubentIdPE);
    OdDbArc::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocArcPersSubentIdPE);
    OdDbCircle::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocCirclePersSubentIdPE);
    OdDbEllipse::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocEllArcPersSubentIdPE);
    OdDbPoint::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocPointPersSubentIdPE);
    OdDbRay::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocRayPersSubentIdPE);
    OdDbSpline::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocSplinePersSubentIdPE);
    OdDbPolyline::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocPolylinePersSubentIdPE);
    OdDbBlockReference::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocBlockReferncePersSubentIdPE);
    OdDbViewport::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocAssocViewPortPersSubentIdPE);
    OdDbXline::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocXLinePersSubentIdPE);
    OdDbAssocNetworkDeepCloneAux::createAssocNetworkCloneReactors();
    OdDbAssocVariable::desc()->addX(OdDbConstraintsInterfacePE::desc(), &constraintsInterfacePE);
    OdDb3dSolid::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assoc3DSolidPersSubentIdPE);
    OdDbSurface::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocSurfacePersSubentIdPE);
    OdDbExtrudedSurface::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocSurfacePersSubentIdPE);
    OdDbPlaneSurface::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocSurfacePersSubentIdPE);
    OdDbSweptSurface::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocSurfacePersSubentIdPE);
    OdDbLoftedSurface::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocSurfacePersSubentIdPE);
    OdDbRevolvedSurface::desc()->addX(OdDbAssocPersSubentIdPE::desc(), &_assocSurfacePersSubentIdPE);
    ::odrxSafeLoadApp(OdDbEntitiesAppName)->release();
  };

  void uninitApp()
  {
    OdDbAssocVariable::desc()->delX(OdDbConstraintsInterfacePE::desc());

    OdDbAssocNetworkDeepCloneAux::deleteAssocNetworkCloneReactors();
    OdDbAssocVariable::desc()->delX(OdDbAssocValueProviderPE::desc());

    OdDbLine::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbArc::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbCircle::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbEllipse::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbPoint::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbRay::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbSpline::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbPolyline::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDb3dSolid::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbSurface::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbPlaneSurface::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbExtrudedSurface::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbSweptSurface::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbLoftedSurface::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbRevolvedSurface::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbBlockReference::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbXline::desc()->delX(OdDbAssocPersSubentIdPE::desc());
    OdDbViewport::desc()->delX(OdDbAssocPersSubentIdPE::desc());

    //remove OdDbAssocPersSubentIdPE
    OdDbAssocLinePersSubentIdPE::rxUninit();
    OdDbAssocArcPersSubentIdPE::rxUninit();
    OdDbAssocCirclePersSubentIdPE::rxUninit();
    OdDbAssocEllArcPersSubentIdPE::rxUninit();
    OdDbAssocRayPersSubentIdPE::rxUninit();
    OdDbAssocSplinePersSubentIdPE::rxUninit();
    OdDbAssoc3dSolidPersSubentIdPE::rxUninit();
    OdDbAssocSurfacePersSubentIdPE::rxUninit();
    OdDbAssocPolylinePersSubentIdPE::rxUninit();
    OdDbAssocBlockReferncePersSubentIdPE::rxUninit();
    OdDbAssocViewPortPersSubentIdPE::rxUninit();
    OdDbAssocPointBasedPersSubentIdPE::rxUninit();
    OdDbAssocXLinePersSubentIdPE::rxUninit();
    OdDbAssocCurvePersSubentIdPE::rxUninit();
    OdDbAssocPersSubentIdPE::rxUninit();
    

    ///////////////////////////////
    AssocActionPE::rxUninit();
    OdDbCoordSystemRef::rxUninit();
    OdDbVectorRef::rxUninit();
    OdDbPathRef::rxUninit();
    OdDbVertexRef::rxUninit();
    OdDbEdgeRef::rxUninit();
    OdDbFaceRef::rxUninit();
    OdDbSubentRef::rxUninit();
    OdDbGeomRef::rxUninit();
    OdDbEvalVariant::rxUninit();
    ///////////////////////////////

    OdDbAssocArrayItem::rxUninit();
	OdDbAssocRestoreEntityStateActionBody::rxUninit();
    OdDbAssocArrayModifyParameters::rxUninit();
    OdDbAssocArrayRectangularParameters::rxUninit();
    OdDbAssocArrayPolarParameters::rxUninit();
    OdDbAssocArrayPathParameters::rxUninit();
    OdDbAssocArrayCommonParameters::rxUninit();
    OdDbSmartCenterLineParameters::rxUninit();
    OdDbSmartCenterMarkParameters::rxUninit();
    OdDbAssocLoftParameters::rxUninit();
    OdDbSmartCenterCommonParameters::rxUninit();
    OdDbAssocArrayParameters::rxUninit();
    OdDbAssocArrayModifyActionBody::rxUninit();
    OdDbAssocArrayActionBody::rxUninit();
	OdDbAssocParamBasedActionBody::rxUninit();
	OdDbCenterMarkActionBody::rxUninit();
	OdDbCenterLineActionBody::rxUninit();
	OdDbSmartCenterActionBody::rxUninit();
	OdDbSmartCenterActionBodyBase::rxUninit();
  OdDbAssocRotatedDimActionBody::rxUninit();
  OdDbAssocAlignedDimActionBody::rxUninit();
  OdDbAssocMLeaderActionBody::rxUninit();
  OdDbAssoc3PointAngularDimActionBody::rxUninit();
  OdDbAssocOrdinateDimActionBody::rxUninit();
  OdDbAssocAnnotationActionBody::rxUninit();
  OdDbAssocFilletSurfaceActionBody::rxUninit();
  OdDbAssocOffsetSurfaceActionBody::rxUninit();
  OdDbAssocBlendSurfaceActionBody::rxUninit();
  OdDbAssocPatchSurfaceActionBody::rxUninit();
  OdDbAssocLoftedSurfaceActionBody::rxUninit();
  OdDbAssocRevolvedSurfaceActionBody::rxUninit();
  OdDbAssocSweptSurfaceActionBody::rxUninit();
  OdDbAssocNetworkSurfaceActionBody::rxUninit();
  OdDbAssocPlaneSurfaceActionBody::rxUninit();
  OdDbAssocExtendSurfaceActionBody::rxUninit();
  OdDbAssocExtrudedSurfaceActionBody::rxUninit();
  OdDbAssocTrimSurfaceActionBody::rxUninit();
  OdDbAssocPathBasedSurfaceActionBody::rxUninit();
  OdDbAssocSurfaceActionBody::rxUninit();
    OdDbAssocVertexActionParam::rxUninit();
    OdDbAssocObjectActionParam::rxUninit();
	OdDbAssocTrimmingBodyActionParam::rxUninit();
    OdDbAssocFaceActionParam::rxUninit();
    OdDbAssocEdgeActionParam::rxUninit();
    OdDbAssocOsnapPointRefActionParam::rxUninit();
    OdDbAssocPointRefActionParam::rxUninit();
	OdDbAssocPathActionParam::rxUninit();
    OdDbAssocCompoundActionParam::rxUninit();
    OdDbAssocAsmBodyActionParam::rxUninit();
    OdDbAssocActionParam::rxUninit();
    OdDbAssoc2dConstraintGroup::rxUninit();
    OdDbAssocNetwork::rxUninit();
    OdDbAssocAction::rxUninit();

#if 0 // TODO
    OdEqualHelpParameterConstraint::rxUninit();
#endif
    OdG2SmoothConstraint::rxUninit();
    OdCompositeConstraint::rxUninit();
    OdPerpendicularConstraint::rxUninit();
    OdNormalConstraint::rxUninit();
    OdPointCurveConstraint::rxUninit();
    OdColinearConstraint::rxUninit();
    OdPointCoincidenceConstraint::rxUninit();
    OdCenterPointConstraint::rxUninit();
    OdConcentricConstraint::rxUninit();
    OdTangentConstraint::rxUninit();
    OdEqualRadiusConstraint::rxUninit();
    OdEqualDistanceConstraint::rxUninit();
    OdEqualLengthConstraint::rxUninit();
    OdHorizontalConstraint::rxUninit();
    OdVerticalConstraint::rxUninit();
    OdParallelConstraint::rxUninit();
    OdEqualCurvatureConstraint::rxUninit();
    OdSymmetricConstraint::rxUninit();
    OdMidPointConstraint::rxUninit();
    OdFixedConstraint::rxUninit();
    OdGeomConstraint::rxUninit();

    OdConstrainedRigidSet::rxUninit();
    OdConstrainedSpline::rxUninit();
    OdConstrainedDatumLine::rxUninit();
    OdConstrained2PointsConstructionLine::rxUninit();
    OdConstrainedConstructionLine::rxUninit();
    OdConstrainedBoundedLine::rxUninit();
    OdConstrainedLine::rxUninit();
    OdConstrainedBoundedEllipse::rxUninit();
    OdConstrainedEllipse::rxUninit();
    OdConstrainedArc::rxUninit();
    OdConstrainedCircle::rxUninit();
    OdConstrainedCurve::rxUninit();
    OdConstrainedImplicitPoint::rxUninit();
    OdConstrainedPoint::rxUninit();
    OdConstrainedGeometry::rxUninit();
    OdHelpParameter::rxUninit();
    OdConstraintGroupNode::rxUninit();

    OdDbAssocGeomDependency::rxUninit();
    OdDbAssocDependency::rxUninit();

    OdDbAssocIndexPersSubentId::rxUninit();
    OdDbAssocEdgePersSubentId::rxUninit();
    OdDbAssocSimplePersSubentId::rxUninit();
    OdDbAssocSingleEdgePersSubentId::rxUninit();
    OdDbAssocPersSubentId::rxUninit();
    OdDbAssocAsmBasedEntityPersSubentId::rxUninit();

    //
    OdDbAssocValueProviderPE::rxUninit();
    OdDbAssocVariable::rxUninit();
    OdDbAssocVariableValueProviderPE::rxUninit();
    OdDbAssocValueDependency::rxUninit();
    OdExplicitConstraint::rxUninit();
    OdDistanceConstraint::rxUninit();
    OdAngleConstraint::rxUninit();
    Od3PointAngleConstraint::rxUninit();
    OdRadiusDiameterConstraint::rxUninit();

    OdDbAssocActionBody::rxUninit();

    OdDbAssocDimDependencyBody::rxUninit();
    OdDbBlockParameterDependencyBody::rxUninit();
    OdDbAssocDimDependencyBodyBase::rxUninit();
    OdDbAssocDependencyBody::rxUninit();
    OdDbAssocDependencyPE::rxUninit();
  };
private:
  OdStaticRxObject<OdDbAssocVariableValueProviderPE> _assocVariableValueProviderPE;

  OdStaticRxObject<OdDbAssocLinePersSubentIdPE>    _assocLinePersSubentIdPE;
  OdStaticRxObject<OdDbAssocArcPersSubentIdPE>    _assocArcPersSubentIdPE;
  OdStaticRxObject<OdDbAssocCirclePersSubentIdPE> _assocCirclePersSubentIdPE;
  OdStaticRxObject<OdDbAssocEllArcPersSubentIdPE> _assocEllArcPersSubentIdPE;
  OdStaticRxObject<OdDbAssocPointBasedPersSubentIdPE> _assocPointPersSubentIdPE;
  OdStaticRxObject<OdDbAssocRayPersSubentIdPE>    _assocRayPersSubentIdPE;
  OdStaticRxObject<OdDbAssocSplinePersSubentIdPE> _assocSplinePersSubentIdPE;
  OdStaticRxObject<OdDbAssocPolylinePersSubentIdPE> _assocPolylinePersSubentIdPE;
  OdStaticRxObject<OdDbAssoc3dSolidPersSubentIdPE> _assoc3DSolidPersSubentIdPE;
  OdStaticRxObject<OdDbAssocXLinePersSubentIdPE>  _assocXLinePersSubentIdPE;
  OdStaticRxObject<OdDbAssocBlockReferncePersSubentIdPE> _assocBlockReferncePersSubentIdPE;
  OdStaticRxObject<OdDbAssocViewPortPersSubentIdPE> _assocAssocViewPortPersSubentIdPE;
  OdStaticRxObject<OdDbAssocSurfacePersSubentIdPE> _assocSurfacePersSubentIdPE;

  struct OdDbConstraintsInterfacePEImpl : OdStaticRxObject < OdDbConstraintsInterfacePE >
  {
    static OdDbAssocVariablePtr findVariable(const OdString& name, OdDbObjectId blockId)
    {
      OdDbAssocNetworkPtr nw = OdDbAssocNetwork::getInstanceFromObject(blockId, false).openObject();
      if (nw.isNull())
        return OdDbAssocVariablePtr();
      OdDbObjectIdArray actions = nw->getActions();
      for (unsigned int i = 0; i < actions.size(); ++i)
      {
        OdDbObjectPtr action = actions[i].openObject();
        if (action->isKindOf(OdDbAssocVariable::desc()))
        {
          OdDbAssocVariablePtr va = action;
          if (va->name() == name)
            return va;
        }
      }
      return OdDbAssocVariablePtr();
    }
    virtual OdResult getAssocVariableName(const OdDbObject* pObject, OdString& name) ODRX_OVERRIDE
    {
      OdDbAssocVariablePtr v = OdDbAssocVariable::cast(pObject);
      if (v.isNull())
        return eInvalidInput;
      name = v->name();
      return eOk;
    }
      virtual OdResult getAssocVariableValue(const OdDbObject* pObject, OdDbObjectId blockId, OdResBufPtr& value) ODRX_OVERRIDE
    {
      if (blockId.isNull())
        return eInvalidInput;
      OdString name;
      OdResult res = getAssocVariableName(pObject, name);
      if (res != eOk)
        return res;
      OdDbAssocVariablePtr v = findVariable(name, blockId);
      if (v.isNull())
        return eInvalidInput;
      value = v->value();
      return eOk;
    }
      virtual OdResult setAssocVariableValue(const OdDbObject* pObject, OdDbObjectId blockId, const OdResBuf* value) ODRX_OVERRIDE
    {
      if (blockId.isNull())
        return eInvalidInput;
      OdString name;
      OdResult res = getAssocVariableName(pObject, name);
      if (res != eOk)
        return res;
      OdDbAssocVariablePtr v = findVariable(name, blockId);
      if (v.isNull())
        return eInvalidInput;
      v->setValue(*OdDbEvalVariant::init(*value));
      return eOk;
    }
  } 
  constraintsInterfacePE;

};

ODRX_DEFINE_DYNAMIC_MODULE(DbConstraints_Module);
