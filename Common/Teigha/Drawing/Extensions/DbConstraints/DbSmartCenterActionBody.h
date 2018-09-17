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

#ifndef DbSmartCenterActionBody_INCLUDED_
#define DbSmartCenterActionBody_INCLUDED_ /*!DOM*/

#include "DbSmartCenterActionBodyBase.h"
#include "DbLayout.h"
#include "TD_PackPush.h"
#include "DbSmartCenterObjectsParameters.h"

class OdDbBlockReference;

/** \details
Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT ODRX_ABSTRACT OdDbSmartCenterActionBody : public OdDbSmartCenterActionBodyBase
{
public:
	ODRX_DECLARE_MEMBERS(OdDbSmartCenterActionBody);

	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbSmartCenterActionBody();

  /** \details
  Returns a pointer to the array of parameters for writing.
  */
  const OdDbSmartCenterCommonParametersPtr parameters() const;

  /** \details
  Returns a pointer to the array of parameters for writing.
  */
  OdDbSmartCenterCommonParametersPtr& parameters();

	//////////////////////////////////////////////////////////////////////////// fromArx
	////AcDbSmartCenterActionBody(void);
	////virtual ~AcDbSmartCenterActionBody(void);
	////void __autoclassinit2(unsigned __int64);
	//enum OdResult addDependencyToStyle(OdDbObjectId);
	//enum OdResult addEntitiesIntoCenterAnonymousBTR(OdDbDatabase *, OdDbObjectId);
  
  /** \details
  This is the notification call of the corresponding addMoreObjectsToDeepClone() method
  of the parent class that owns this action body.
  */
	virtual enum OdResult addMoreObjectsToDeepCloneOverride(OdDbIdMapping& idMap, OdDbObjectIdArray& additionalObjectsToClone  )const;
  virtual OdResult postProcessAfterDeepCloneOverride(OdDbIdMapping &);

	//bool associationToBeBroken(void)const;
	//double autoMatchExtension(void)const;
	//enum OdResult breakAssociativity(void);
	//bool cacheAssociateStatus(void);
	//void cacheViewportScale(void);
	//double cachedViewportScale(void)const;
	////static OdDbSmartCenterActionBody *  cast(OdRxObject const *);
	//OdDbObjectId centerDepId(void)const;
	//OdDbObjectId centerObjectId(void)const;
	//enum OdResult createCenterEntity(OdDbDatabase *);
	//virtual bool doesActionMatchCurrentGeometry(void)const;
	//bool doesActionMatchCurrentSubEntLineWeight(void)const;
	//bool doesActionMatchCurrentSubEntLinetype(void)const;
	//bool doesActionMatchCurrentSubEntLinetypeScale(void)const;
	//bool doesActionNeedUpdate(void)const;
	//enum OdResult eraseCenterEntity(void);

  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(void);

	////enum OdResult getAssociatedEntitiesIds(OdArray<OdDbObjectId, OdArrayMemCopyReallocator<OdDbObjectId> > &);
	//bool getCachedAssociateStatus(void)const;
	////enum OdResult getCenterEntitesIds(OdArray<OdDbObjectId, OdArrayMemCopyReallocator<OdDbObjectId> > &)const;
	////enum OdResult getCenterObjGeometry(OdArray<OdGeCurve3d *, OdArrayMemCopyReallocator<OdGeCurve3d * > > &)const;
	////virtual enum OdResult getEdgeRefs(OdArray<OdDbEdgeRef, OdArrayObjectCopyReallocator<OdDbEdgeRef> > &)const;
	////virtual OdDbSmartCenterLayout *  getLayout(void)const;
	//virtual OdDbLayoutPtr * getLayout(void)const;
	//OdDbObjectId getStyleId(void)const;
	//virtual bool handleBreakingAssociation(void);
	//bool isAssociative(void)const;
	//bool isAutoMatchExtension(double)const;
	//bool isCenterObjectErased(void)const;
	//bool isDragging(void);
	//bool isExplicitlyDeassociatived(void)const;
  /** \details
  This is the notification call of the corresponding postProcessAfterDeepClone() method
  of the parent class that owns this action body.
  */
	//virtual enum OdResult resetExtensions(void);
	//virtual enum OdResult resetOvershoots(void);
	//virtual bool rotationToBeChecked(void)const;
	//static void rxInit(void(*)(OdRxClass const *, wchar_t *  &, int));
	//void setAssociationToBeBroken(bool);
	//enum OdResult setCachedAssociateStatus(bool);
	//enum OdResult setCachedViewportScale(double);
	//enum OdResult setCenterDepId(OdDbObjectId);
	//enum OdResult setIsExplicitlyDeassociatived(bool);
	//enum OdResult setStyleDepId(OdDbObjectId);
	//enum OdResult setSubentLineType(OdString);
	//enum OdResult setSubentLineWeight(int);
	//enum OdResult setSubentLinetypeScale(double);
	//OdDbObjectId styleDepId(void)const;
	//OdString subentLineType(void)const;
	//int subentLineWeight(void)const;
	//double subentLinetypeScale(void)const;
  enum OdResult setGeometryParam(double newOvershoot, const OdString& paramName, bool assertEnable = true);
  enum OdResult getGeometryParam(double& newOvershoot, const OdString& paramName) const;
  enum OdResult setStringGeometryParam(const OdString newValue, const OdString& paramName);
  enum OdResult getStringGeometryParam(OdString& newString, const OdString& paramName) const;	
  static OdDbObjectId getSmartCenterActionBody(const OdDbBlockReference* pBlkRef);

protected:
  virtual OdResult addSubentitySpecialSettings(OdDbEntityPtr ent, const OdDbSubentId subentId, OdUInt32 idx);

  friend class OdDbImpSmartCenterActionBody;
  friend class OdDbImpCenterLineActionBody;
  friend class OdDbImpCenterMarkActionBody;
};

/** \details
The typified smart pointer. This template class is specialization of the OdSmartPtr class
for the OdDbSmartCenterActionBodyBase class.

\sa	
<link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSmartCenterActionBody> OdDbSmartCenterActionBodyPtr;

#include "TD_PackPop.h"

#endif

