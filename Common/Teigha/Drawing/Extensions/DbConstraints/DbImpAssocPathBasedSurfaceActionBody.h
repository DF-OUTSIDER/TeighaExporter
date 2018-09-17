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
#ifndef DbImpAssocPathBasedSurfaceActionBody_INCLUDED_
#define DbImpAssocPathBasedSurfaceActionBody_INCLUDED_ /*!DOM*/

#include "DbAssocGlobal.h"
#include "DbImpAssocActionBody.h"
#include "DbAssocExtrudedSurfaceActionBody.h"
#include "DbSmartCenterActionBody.h"
#include "DbAssocGeomDependency.h"
#include "DbBlockReference.h"
#include "DbAssocActionParam.h"
#include "DbAssocLoftedSurfaceActionBody.h"
#include "DbBlendOptions.h"

#include "TD_PackPush.h"

//////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents the custom implementation of the action body based on AnnotationActionBody.

Library: Source code provided.
<group Contraint_Classes>
*/

extern const OdConstString kOffsetDistanceParamName;
extern const OdConstString kContinuityParamName;
extern const OdConstString kBulgeParamName;
extern const OdConstString kRevolveAngleParamName;
extern const OdConstString kRotationAngleParamName;
extern const OdConstString kScaleFactorParamName;
extern const OdConstString kTwistAngleParamName;
extern const OdConstString kExtrusionHeightParamName;
extern const OdConstString kExtendDistanceParamName;
extern const OdConstString kFilletRadiusParamName;
extern const OdConstString kChamferDistanceParamName;
extern const OdConstString kExtrusionTaperAngle;
extern const OdConstString kFilletInputFace1ParamName;
extern const OdConstString kFilletInputFace2ParamName;
extern const OdConstString kBlendStartProfileParamName;
extern const OdConstString kBlendEndProfileParamName;
extern const OdConstString kExtendEdgesParamName;
extern const OdConstString kExtrudeProfileParamName;
extern const OdConstString kLoftCrossSectionName;
extern const OdConstString kLoftGuideCurveName;
extern const OdConstString kLoftPathCurveName;
extern const OdConstString kRevolveProfileParamName;
extern const OdConstString kRevolvePathParamName;
extern const OdConstString kSweepProfileParamName;
extern const OdConstString kSweepPathParamName;
extern const OdConstString kOffsetBodyParamName;
extern const OdConstString kPatchProfileParamName;
extern const OdConstString kPatchConstraintParamName;
extern const OdConstString kTrimToolFaceParamName;
extern const OdConstString kTrimToolPathParamName;
extern const OdConstString kEdgeFilletChamferParamName;

class DBCONSTRAINTS_EXPORT OdDbImpAssocSurfaceActionBody : public OdDbImpAssocParamBasedActionBody//OdDbImpAssocActionBody// OdDbImpAssocAnnotationActionBody
{
public:
  // DESCRIPTION: defines the names of parameters in surface action bodies
  //
  //////////////////////////////////////////////////////////////////////////////
#pragma once
  /** \details
  Constructor creates an instance of this class.
  */
  OdDbImpAssocSurfaceActionBody();

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocSurfaceActionBody();

  /** \details
  Reads the .dwg file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
  Writes the .dwg file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
  Reads the .dxf file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
  Writes the .dxf file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  virtual void evaluateOverride(OdDbObjectId parentActionId) {};
  OdDbObjectId surfaceObjectDepId() const { return m_surfaceObjectDepId; };
  virtual OdStringArray compareWith(OdDbImpAssocActionBody *paramIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;
protected:
  OdUInt32 m_udef1;
  OdUInt32 m_udef2;
  OdDbObjectId  m_surfaceObjectDepId;
  bool m_boolUndef1;
  bool m_boolUndef2;
  OdUInt16 m_udefInt16_1;
  OdUInt16 m_udefInt16_2;
  //OdDbImpAssocParamBasedActionBody *m_paramBasedActionBody;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents the custom implementation of the action body based on AnnotationActionBody.

Library: Source code provided.
<group Contraint_Classes>
*/

class DBCONSTRAINTS_EXPORT OdDbImpAssocPathBasedSurfaceActionBody : public OdDbImpAssocSurfaceActionBody// OdDbImpAssocAnnotationActionBody
{
public:
  /** \details
  Constructor creates an instance of this class.
  */
  OdDbImpAssocPathBasedSurfaceActionBody() { m_udefPathBased = 0; };

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocPathBasedSurfaceActionBody() {};

  /** \details
  Reads the .dwg file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
  Writes the .dwg file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
  Reads the .dxf file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
  Writes the .dxf file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  virtual void evaluateOverride(OdDbObjectId parentActionId) {};
  static OdDbObjectId createOdDdAssocPathActionParam(const OdDbObjectId actionId, const OdDbObjectId assocOb, const OdDbAssocDependencyPtr pDependency, OdString paramName, int paramIndex = 0);
  virtual OdStringArray compareWith(OdDbImpAssocActionBody *paramIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;

protected:
  OdUInt32 m_udefPathBased;
};

#include "TD_PackPop.h"
#endif

