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
#ifndef DbImpCenterMarkActionBody_INCLUDED_
#define DbImpCenterMarkActionBody_INCLUDED_ /*!DOM*/

#include "DbImpSmartCenterActionBody.h"
#include "DbCenterMarkActionBody.h"
#include "DbDatabase.h"

#include "TD_PackPush.h"
/** \details
This class represents the custom implementation of the CenterMark and CentwerLine of the action body.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpCenterMarkActionBody : public OdDbImpSmartCenterActionBody
{
public:
  //static OdDbImpCenterMarkActionBody* getImpl(const OdDbCenterMarkActionBody *pObj)
  //{
  //  return (OdDbImpCenterMarkActionBody*)OdDbSystemInternals::getImpl(pObj);
  //}

  /** \details
	Constructor creates an instance of this class.
	*/
	OdDbImpCenterMarkActionBody();

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbImpCenterMarkActionBody();

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
	////////////////////////////////////////////////////////////////////////////////////
	/*OdDbCenterMarkObjectWrapper(OdDbCenterMarkObjectWrapper const & ) ;
	OdDbCenterMarkObjectWrapper(OdDbObjectId) ;
	OdDbCenterMarkObjectWrapper &  operator=(OdDbCenterMarkObjectWrapper const & ) ;
	void __autoclassinit2(unsigned __int64) ;*/
	//double crossHairGap(void)const ;
	OdString crossHairGapExpression(OdDbAssocActionPtr pAction)const ;
	//double crossHairLength(void)const ;
	OdString crossHairLengthExpression(OdDbAssocActionPtr pAction)const ;
	//enum OdResult getAllExtensionPoints(OdArray<OdGePoint3d,OdArrayMemCopyReallocator<OdGePoint3d> > & ) ;
	//enum OdResult getAllExtensionPoints(OdGePoint3d & ,OdGePoint3d & ,OdGePoint3d & ,OdGePoint3d & ) ;
	//enum OdResult getAllOvershootPoints(OdArray<OdGePoint3d,OdArrayMemCopyReallocator<OdGePoint3d> > & ) ;
	//enum OdResult getAllOvershootPoints(OdGePoint3d & ,OdGePoint3d & ,OdGePoint3d & ,OdGePoint3d & ) ;
	//enum OdResult getCenterPoint(OdGePoint3d & ) ;
	//enum OdResult getCrossLengthAndGap(double & ,double & )const ;
	//double getExtensionLineLength(enum AcCenterMarkSubEntType)const ;
  OdDb::Visibility getExtensionLinesVisibility(OdDbAssocActionPtr pAction)const ;
	static OdResult getRefCircle(OdGeCircArc3d & , const OdDbObjectId &parentActionId);
	//double getRefCircleRadius(void)const ;
	//bool getSubEntVisibility(enum AcCenterMarkSubEntType)const ;
	//OdGeVector3d getSubentDirection(enum AcCenterMarkSubEntType)const ;
	//OdGeLineSeg3d getSubentLineGeometry(enum AcCenterMarkSubEntType,bool)const ;
	double horizontalEndExtension(const OdDbAssocActionPtr pAction)const ;
	//OdGePoint3d horizontalEndExtensionPoint(void) ;
	double horizontalEndOvershoot(OdDbAssocActionPtr pAction)const ;
	//OdGePoint3d horizontalEndOvershootPoint(void) ;
	double horizontalStartExtension(const OdDbAssocActionPtr pAction)const ;
	//OdGePoint3d horizontalStartExtensionPoint(void) ;
	double horizontalStartOvershoot(OdDbAssocActionPtr pAction)const ;
	//OdGePoint3d horizontalStartOvershootPoint(void) ;
	//bool isCenterCrossTooLarge(enum AcCenterMarkSubEntType)const ;
	double rotation(const OdDbObjectId &parentActionId)const ;
	//enum OdResult setAllOvershoots(double) ;
	//enum OdResult setCachedCircleRadius(double) ;
	enum OdResult setCrossHairGapExpression(OdString const &, OdDbAssocActionPtr pAction) ;
	enum OdResult setCrossHairLengthExpression(OdString const &, OdDbAssocActionPtr pAction) ;
	enum OdResult setExtensionLinesVisibility(OdDb::Visibility, OdDbAssocActionPtr pAction) ;
	enum OdResult setHorizontalEndExtension(double, OdDbAssocActionPtr pAction) ;
	enum OdResult setHorizontalEndOvershoot(double, OdDbAssocActionPtr pAction);
	enum OdResult setHorizontalStartExtension(double, OdDbAssocActionPtr pAction) ;
	enum OdResult setHorizontalStartOvershoot(double , OdDbAssocActionPtr pAction);
	enum OdResult setRotation(double, const OdDbObjectId parentActionId) ;
	//enum OdResult setSubEntVisibility(enum AcCenterMarkSubEntType,bool) ;
	enum OdResult setVerticalEndExtension(double, OdDbAssocActionPtr pActio) ;
	enum OdResult setVerticalEndOvershoot(double, OdDbAssocActionPtr pAction) ;
	enum OdResult setVerticalStartExtension(double, OdDbAssocActionPtr pAction) ;
	enum OdResult setVerticalStartOvershoot(double, OdDbAssocActionPtr pAction) ;
	//void updateCachedCircleRadius(void) ;
	double verticalEndExtension(const OdDbAssocActionPtr pAction)const ;
	//OdGePoint3d verticalEndExtensionPoint(void) ;
	double verticalEndOvershoot(OdDbAssocActionPtr pAction)const ;
	//OdGePoint3d verticalEndOvershootPoint(void) ;
	double verticalStartExtension(const OdDbAssocActionPtr pAction)const ;
	//OdGePoint3d verticalStartExtensionPoint(void) ;
	double verticalStartOvershoot(OdDbAssocActionPtr pAction) const ;
	//OdGePoint3d verticalStartOvershootPoint(void) ;
	//static enum SmartCenterType wrapperCenterType(void);
  ////////////////////////////////////////////////////////////////////// from Arx for parent classes
  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(OdDbObjectId parentActionId);

  /** \details
  Creates associative Center Mark: action, action body, dependencies etc.
  */
  static OdResult createInstance(
    const OdDbFullSubentPath subentPath,
    OdDbObjectId& centerMarkId,
    OdDbObjectId& actionBodyId);
  protected:
    /*!DOM*/
    //OdDbSmartCenterMarkParametersPtr m_pParams;
};

#include "TD_PackPop.h"

#endif
