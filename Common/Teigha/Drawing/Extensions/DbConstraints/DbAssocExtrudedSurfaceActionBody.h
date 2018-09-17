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

#ifndef DbAssocExtrudedSurfaceActionBody_INCLUDED_
#define DbAssocExtrudedSurfaceActionBody_INCLUDED_ /*!DOM*/
#include "DbAssocSurfaceActionBody.h"
#include "TD_PackPush.h"

const OdConstString kOffsetDistanceParamName = OD_T("OffsetDistance");
const OdConstString kContinuityParamName = OD_T("Continuity");
const OdConstString kBulgeParamName = OD_T("Bulge");
const OdConstString kRevolveAngleParamName = OD_T("RevolveAngle");
const OdConstString kRotationAngleParamName = OD_T("RotationAngle");
const OdConstString kScaleFactorParamName = OD_T("ScaleFactor");
const OdConstString kTwistAngleParamName = OD_T("TwistAngle");
const OdConstString kExtrusionHeightParamName = OD_T("ExtrusionHeight");
const OdConstString kExtendDistanceParamName = OD_T("ExtendDistance");
const OdConstString kFilletRadiusParamName = OD_T("FilletRadius");
const OdConstString kChamferDistanceParamName = OD_T("ChamferDistance");
const OdConstString kExtrusionTaperAngle = OD_T("ExtrusionTaperAngle");
const OdConstString kFilletInputFace1ParamName = OD_T("InputFace1");
const OdConstString kFilletInputFace2ParamName = OD_T("InputFace2");
const OdConstString kBlendStartProfileParamName = OD_T("BlendStartProfile");
const OdConstString kBlendEndProfileParamName = OD_T("BlendEndProfile");
const OdConstString kExtendEdgesParamName = OD_T("ExtendEdges");
const OdConstString kExtrudeProfileParamName = OD_T("ExtrudeProfile");
const OdConstString kLoftCrossSectionName = OD_T("CrossSection");
const OdConstString kLoftGuideCurveName = OD_T("GuideCurve");
const OdConstString kLoftPathCurveName = OD_T("PathCurve");
const OdConstString kRevolveProfileParamName = OD_T("RevolveProfile");
const OdConstString kRevolvePathParamName = OD_T("RevolvePath");
const OdConstString kSweepProfileParamName = OD_T("SweepProfile");
const OdConstString kSweepPathParamName = OD_T("SweepPath");
const OdConstString kOffsetBodyParamName = OD_T("OffsetBody");
const OdConstString kPatchProfileParamName = OD_T("PatchProfile");
const OdConstString kPatchConstraintParamName = OD_T("PatchConstraint");
const OdConstString kTrimToolFaceParamName = OD_T("TrimmingFace");
const OdConstString kTrimToolPathParamName = OD_T("TrimmingPath");
const OdConstString kEdgeFilletChamferParamName = OD_T("FilletChamferEdge");
/////////////////////////////////////////////////
/** \details
This class represents the base class OdDbAssocExtrudedSurfaceActionBody for the action bodies.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocExtrudedSurfaceActionBody : public OdDbAssocPathBasedSurfaceActionBody
{
public:
	ODRX_DECLARE_MEMBERS(OdDbAssocExtrudedSurfaceActionBody);

	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbAssocExtrudedSurfaceActionBody(bool createImpObj = true);

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbAssocExtrudedSurfaceActionBody();
};

typedef OdSmartPtr<OdDbAssocExtrudedSurfaceActionBody> OdDbAssocExtrudedSurfaceActionBodyPtr;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TD_PackPop.h"

#endif


