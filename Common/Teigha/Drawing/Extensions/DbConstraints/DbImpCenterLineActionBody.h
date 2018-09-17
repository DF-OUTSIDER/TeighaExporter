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
#ifndef DbImpCenterLineActionBody_INCLUDED_
#define DbImpCenterLineActionBody_INCLUDED_ /*!DOM*/

#include "DbImpSmartCenterActionBody.h"
#include "DbCenterLineActionBody.h"
#include "DbSmartCenterActionBody.h"

#include "TD_PackPush.h"
/** \details
This class represents the custom implementation of the CenterLine and CentwerLine of the action body.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpCenterLineActionBody : public OdDbImpSmartCenterActionBody
{
public:
	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbImpCenterLineActionBody();

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbImpCenterLineActionBody();

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

  //const OdDbSmartCenterLineParametersPtr parameters() const;
  //OdDbSmartCenterLineParametersPtr& parameters();
  
  virtual void composeForLoad(OdDbObject *pObject, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
	//////////////////////////////////////////////////////////////////////////////////// From Arx
	//OdDbCenterLineObjectWrapper(OdDbCenterLineObjectWrapper const &);
	//OdDbCenterLineObjectWrapper(OdDbObjectId);
	//OdDbCenterLineObjectWrapper &  operator=(OdDbCenterLineObjectWrapper const &);
	//void __autoclassinit2(unsigned __int64);

	//OdGeVector3d cachedCenterLineDirection(void)const;
	//double cachedCenterLineLength(void)const;
	//int edgeRefDirEnding(int);
  enum OdResult edgeRefDirEndingArray(OdDbAssocActionPtr pAction, OdArray<int>  &);// , OdArrayMemCopyReallocator<int> > &);
	double endExtension(OdDbAssocActionPtr pAction)const;
	double endOvershoot(OdDbAssocActionPtr pAction)const;
	enum OdResult getCenterLineGeometry(OdGeLineSeg3d &, bool, OdDbObjectId parentActionId);
	//enum OdResult getCenterPoint(OdGePoint3d &);
	//enum OdResult setCachedCenterLineDirection(OdGeVector3d const &);
	//enum OdResult setCachedCenterLineLength(double);
	//enum OdResult setEdgeRefDirEnding(int, int);
	enum OdResult setEdgeRefDirEndingArray(OdDbAssocActionPtr pAction, OdArray<int>  &);
	enum OdResult setEndExtension(double, OdDbAssocActionPtr pAction);
	enum OdResult setEndOvershoot(double, OdDbAssocActionPtr pAction);
	enum OdResult setStartExtension(double, OdDbAssocActionPtr pAction);
	enum OdResult setStartOvershoot(double, OdDbAssocActionPtr pAction);
	double startExtension(OdDbAssocActionPtr pAction)const;
	double startOvershoot(OdDbAssocActionPtr pAction)const;
	//static enum SmartCenterType wrapperCenterType(void);
  ////////////////////////////////////////////////////////////////////// from Arx for parent classes
  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(OdDbObjectId parentActionId);
  /** \details
  Creates associative centerLine: action, action body, dependencies etc.
  */
  static OdResult createInstance(
    const OdDbFullSubentPathArray entPathArray,
    OdArray<int> inputArr,
    OdDbObjectId& centerLineId,
    OdDbObjectId& actionBodyId);

  static OdResult createInstance(
    const OdDbFullSubentPathArray entPathArray,
    //OdGePoint3dArray ptsSelected,
    OdDbObjectId& centerLineId,
    OdDbObjectId& actionBodyId);

  static OdResult setParametersAndCreateCL(
    OdArray<int> inputArr,
    OdDbDatabasePtr pDb,
    OdDbAssocActionPtr pAction,
    OdDbCenterLineActionBodyPtr pActionBody,
    OdDbAssocDependencyPtr pDependency,
    OdDbBlockTableRecordPtr pDestinationBTR,
    OdDbLinetypeTableRecordPtr centerLTypetableRec,
    OdDbBlockReferencePtr pCenterLineBlock,
    OdDbObjectId& centerLineId
    );

  static OdResult getPtrsForTree(
    const OdDbFullSubentPathArray entPathArray,
    OdDbDatabasePtr& pDb,
    OdDbAssocActionPtr& pAction,
    OdDbCenterLineActionBodyPtr& pActionBody,
    OdDbAssocDependencyPtr& pDependency,
    OdArray<OdDbAssocGeomDependencyPtr>& pGeomDependency,
    OdDbBlockTableRecordPtr& pDestinationBTR,
    OdDbLinetypeTableRecordPtr& centerLTypetableRec,
    OdDbBlockReferencePtr& pCenterLineBlock,
    OdArray<OdGeLineSeg3d>& segLineArr,
    OdDbObjectId& actionBodyId
    );
//protected:
//  OdDbSmartCenterLineParametersPtr m_pParams;
};

#include "TD_PackPop.h"
#endif
