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

#ifndef DbImpSmartCenterActionBody_INCLUDED_
#define DbImpSmartCenterActionBody_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbObject.h"
#include "DbObjectIterator.h"
#include "RxObjectImpl.h"
#include "SharedPtr.h"
#include "DbEvalVariant.h"
#include "DbImpAssocActionBody.h"
#include "DbBlockTableRecord.h"
#include "DbLine.h"
#include "DbAssocGeomDependency.h"
#include "DbSmartCenterActionBody.h"

#include "TD_PackPush.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpSmartCenterActionBodyBase : public OdDbImpAssocActionBody
{
public:
	/** \details
	Constructor creates an instance of this class.
	*/
		OdDbImpSmartCenterActionBodyBase();

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbImpSmartCenterActionBodyBase();

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
protected:
	/*!DOM*/
	friend class OdDbImpAssocAction;
	/*!DOM*/
	//OdDbObjectIdArray m_arrParams;
	/*!DOM*/
	//OdArray<OdDbImpValueParamPtr> m_arrValueParams;
};

//////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents the custom implementation of the associated parameter of the action body.
	
Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpSmartCenterActionBody : public OdDbImpSmartCenterActionBodyBase
{
public:
	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbImpSmartCenterActionBody();

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbImpSmartCenterActionBody();

  /** \details
  Returns a pointer to the array of parameters for writing.
  */
  const OdDbSmartCenterCommonParametersPtr parameters() const;

  /** \details
  Returns a pointer to the array of parameters for writing.
  */
  OdDbSmartCenterCommonParametersPtr& parameters();

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

  /** \details
  Gets the object ID of the associated array which this action controlls.
  */
  //virtual OdDbObjectId getGeomDependEntity(OdDbObjectId parentActionId) const;
  //enum OdResult addDependencyToStyle(OdDbObjectId);
  //enum OdResult addEntitiesIntoCenterAnonymousBTR(OdDbDatabase *, OdDbObjectId);
  OdResult addMoreObjectsToDeepCloneOverride(const OdDbSmartCenterActionBody *pBody,
    OdDbIdMapping& idMap,
    OdDbObjectIdArray& additionalObjectsToClone) const;

  OdResult postProcessAfterDeepCloneOverride(const OdDbSmartCenterActionBody *pBody, OdDbIdMapping& idMap);
  static OdDbViewportPtr getViewPortFromCompoundObject(OdDbAssocGeomDependencyPtr pGeomDep);
  //bool associationToBeBroken(void)const;
  //double autoMatchExtension(void)const;
  //enum OdResult breakAssociativity(void);
  //bool cacheAssociateStatus(void);
  //void cacheViewportScale(void);
  //double cachedViewportScale(void)const;
  //static OdDbSmartCenterActionBody *  cast(AcRxObject const *);
  //OdDbObjectId centerDepId(void)const;
  //OdDbObjectId centerObjectId(void)const;
  //enum OdResult createCenterEntity(OdDbDatabase *);
  //static AcRxClass *  desc(void);
  //virtual bool doesActionMatchCurrentGeometry(void)const;
  //bool doesActionMatchCurrentSubEntLineWeight(void)const;
  //bool doesActionMatchCurrentSubEntLinetype(void)const;
  //bool doesActionMatchCurrentSubEntLinetypeScale(void)const;
  //bool doesActionNeedUpdate(void)const;
  //enum OdResult eraseCenterEntity(void);
  //virtual void evaluateOverride(void);
  //enum OdResult getAssociatedEntitiesIds(AcArray<OdDbObjectId, AcArrayMemCopyReallocator<OdDbObjectId> > &);
  //bool getCachedAssociateStatus(void)const;
  //enum OdResult getCenterEntitesIds(AcArray<OdDbObjectId, AcArrayMemCopyReallocator<OdDbObjectId> > &)const;
  //enum OdResult getCenterObjGeometry(AcArray<AcGeCurve3d *, AcArrayMemCopyReallocator<AcGeCurve3d * > > &)const;
  //virtual enum OdResult getEdgeRefs(AcArray<OdDbEdgeRef, AcArrayObjectCopyReallocator<OdDbEdgeRef> > &)const;
  //virtual OdDbSmartCenterLayout *  getLayout(void)const;
  //OdDbObjectId getStyleId(void)const;
  //virtual bool handleBreakingAssociation(void);
  //virtual AcRxClass *  isA(void)const;
  //bool isAssociative(void)const;
  //bool isAutoMatchExtension(double)const;
  //bool isCenterObjectErased(void)const;
  //bool isDragging(void);
  //bool isExplicitlyDeassociatived(void)const;
  //virtual enum OdResult resetExtensions(void);
  //virtual enum OdResult resetOvershoots(void);
  //virtual bool rotationToBeChecked(void)const;
  //static void rxInit(void(*)(AcRxClass const *, wchar_t *  &, int));
  //static void rxInit(void);
  //void setAssociationToBeBroken(bool);
  //enum OdResult setCachedAssociateStatus(bool);
  //enum OdResult setCachedViewportScale(double);
  //enum OdResult setCenterDepId(OdDbObjectId);
  //enum OdResult setIsExplicitlyDeassociatived(bool);
  //enum OdResult setStyleDepId(OdDbObjectId);
  enum OdResult setSubentLineType(OdString, OdDbAssocActionPtr pAction);
  static enum OdResult setSubentLineWeight(double, OdDbAssocActionPtr pAction);
  enum OdResult setSubentLinetypeScale(double, OdDbAssocActionPtr pAction);
  //OdDbObjectId styleDepId(void)const;
  OdString subentLineType(OdDbAssocActionPtr pAction)const;
  OdInt32 subentLineWeight(OdDbAssocActionPtr pAction)const;
  double subentLinetypeScale(OdDbAssocActionPtr pAction)const;
protected:
  virtual OdResult addSubentitySpecialSettings(OdDbEntityPtr ent, const OdDbSubentId subentId, OdUInt32 idx, OdDbSmartCenterActionBodyPtr pActionBody);
  OdDbSmartCenterCommonParametersPtr m_pParams;
  static OdDbLinetypeTableRecordPtr getCenterLType(OdDbDatabase* pDb, OdString newType = OdString::kEmpty);
  static void setAddedEntityPropertiesByBlock(OdDbEntityPtr pLine, OdDbAssocActionPtr pAction);// const double newScale, const OdDb::LineWeight newWeight, const OdCmColor newColor);
	static OdDbObjectId getCenterLayerId(OdDbDatabase* pDb);
  OdDbImpAssocParamBasedActionBody* m_pParamActionBody;
	/*!DOM*/
	friend class OdDbImpAssocAction;
  friend class OdDbSmartCenterActionBody;
};

OdDbAssocGeomDependencyPtr  getAssocGeomDependencyPtr(OdDbAssocActionPtr pAction, OdDb::OpenMode mode = OdDb::kForRead, int geomDepNumber = 0);

#include "TD_PackPop.h"
#endif
