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
#ifndef DbImpAssocRotatedDimActionBody_INCLUDED_
#define DbImpAssocRotatedDimActionBody_INCLUDED_ /*!DOM*/

#include "DbAssocGlobal.h"
#include "DbImpAssocActionBody.h"
#include "DbAssocRotatedDimActionBody.h"
#include "DbAssocMLeaderActionBody.h"
#include "DbSmartCenterActionBody.h"
#include "DbAssocGeomDependency.h"
#include "DbRotatedDimension.h"
#include "DbAlignedDimension.h"
#include "DbBlockReference.h"
#include "DbAssocActionParam.h"
#include "DbImpSmartCenterActionBody.h"

#include "TD_PackPush.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocAnnotationActionBody : public OdDbImpAssocActionBody
{
public:
  /** \details
  Constructor creates an instance of this class.
  */
  OdDbImpAssocAnnotationActionBody();

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocAnnotationActionBody();

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
  ////////////////////////////////////////////////////////////////////// From Arx
  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(OdDbObjectId parentActionId);

  void evaluateOverrideAssocDim(OdDbObjectId parentActionId, OdGePoint3d* ptPoint, OdGePoint3d* ptDimPoint, const int pointNum);
  /** \details
  Gets the object ID of the associated array which this action controlls.
  */
  virtual OdDbObjectId getArrayEntity(OdDbObjectId parentActionId) const;

  OdDbObjectId getViewPortIdFromDependency(OdDbObjectId parentActionId) const;

  void setAssocDependencyID(OdDbObjectId objID);
  void setActionParamID1(OdDbObjectId objID);
  void setActionParamID2(OdDbObjectId objID);
  OdDbObjectId actionParamID1() const { return m_ActionParamID1; }
  OdDbObjectId actionParamID2() const { return m_ActionParamID2; }
protected:
  //virtual OdResult setAndGetEdgeRefs(OdDbObjectId assocActionParam, OdGeCurve3d* geometry, OdGePoint3d ptPoint, OdDbObjectId parentActionId, OdDbEdgeRef& edgeRef);
  virtual OdResult resetEdgeRefs(OdDbAssocEdgeActionParamPtr pEdgeParam, OdGeCurve3d* geometry, /*OdGePoint3d ptPoint,*/ OdDbObjectId parentActionId, OdDbEdgeRef& edgeRef);
  virtual OdResult getGeometryAndSidePointForDimension(OdGeCurve3d* &geometry, OdGePoint3d & refLineSegment, int seg, OdDbAssocActionPtr pAction);
  virtual OdResult setDataToActionParam(OdGeCurve3d* geometry, OdDbObjectId snapPointParamId, OdGePoint3d& ptPoint, OdGePoint3d ptDimPoint,
    OdDbObjectId parentActionId/*, OdDbBlockReferencePtr pBlockRef, OdDbViewportPtr  pViewport*/);
  static OdDbObjectId createOdDbAssocOSnapPointRefActionParam( const OdDbObjectId actionId, const OdDbObjectId source, OdGePoint3d ptPoint1, const OdDbAssocGeomDependencyPtr pGeomDependency1, 
    const OdDbAssocGeomDependencyPtr pGeomDependency2, OdDbObjectId& anchorId, const PointPositionOnBlockLine isStartPoint, int index);
  static void CreateGeomDependencies(const OdDbFullSubentPath entPath, const OdDbObjectId actionId, const OdDbObjectId source, 
    OdDbAssocGeomDependencyPtr& pGeomDependency1, OdDbAssocGeomDependencyPtr& pGeomDependency2, PointPositionOnBlockLine& isStartPoint);
  static OdDbObjectId getAssocDependencyForAction(const OdDbObjectId parentActionId);
  static OdDbObjectId GetLineIDByPoint(OdDbBlockReferencePtr bRef, OdGePoint3d ptPoint, PointPositionOnBlockLine&  isStartPoint);
  /*!DOM*/
  friend class OdDbImpAssocAction;
  static OdDbObjectId getIdForFirstWriteDependency(OdDbObjectIdArray depIDs);
  //enum AssocActionBodyTypes
  //{
  //  kUnDefined = 0,
  //  kMleader,
  //  kRotatedDim
  //};

  //AssocActionBodyTypes m_ActionBodyType;
//private:
protected:	
	OdDbObjectId m_ActionParamID1;
	OdDbObjectId m_ActionParamID2;
	OdDbObjectId m_ActionParamID3;
	OdUInt16 m_undefined2;
  OdDbObjectId m_assocDependencyID;
  OdUInt16 m_undefinedAnnotationActionBody;

  OdDbImpAssocParamBasedActionBody *m_paramBasedActionBody;
};

//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents the custom implementation of the action body based on AnnotationActionBody.

Library: Source code provided.
<group Contraint_Classes>
*/

class DBCONSTRAINTS_EXPORT OdDbImpAssocRotatedDimActionBody : public OdDbImpAssocAnnotationActionBody
{
public:
  /** \details
  Constructor creates an instance of this class.
  */
  OdDbImpAssocRotatedDimActionBody();

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocRotatedDimActionBody();

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
  /** \details
  Returns a pointer to the array of parameters for writing.
  */
  //const OdDbSmartCenterMarkParametersPtr parameters() const;
  //OdDbSmartCenterMarkParametersPtr& parameters();

  virtual void composeForLoad(OdDbObject *pObject, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(OdDbObjectId parentActionId);

  /** \details
  Creates associative rotated dimensin: action, action body, dependencies etc.
  */
  static OdResult createInstance(
	  const OdDbFullSubentPathArray entPathArray,
	  OdGePoint3dArray ptsSelected,
	  OdDbObjectId& actionBodyId, OdString paramName = OD_T("Rotated"));

  virtual OdStringArray compareWith(OdDbImpAssocActionBody *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;
};

#include "TD_PackPop.h"

#endif

