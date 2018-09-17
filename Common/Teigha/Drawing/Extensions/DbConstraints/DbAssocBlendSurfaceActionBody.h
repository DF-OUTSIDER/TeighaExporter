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

#ifndef OdDbAssocBlendSurfaceActionBody_INCLUDED_
#define OdDbAssocBlendSurfaceActionBody_INCLUDED_ /*!DOM*/
#include "DbAssocSurfaceActionBody.h"
#include "TD_PackPush.h"

/** \details
<group Contraint_Classes>
  
This class represents the base class OdDbAssocBlendSurfaceActionBody for the action bodies.

Library: Source code provided.
*/
class DBCONSTRAINTS_EXPORT OdDbAssocBlendSurfaceActionBody : public OdDbAssocPathBasedSurfaceActionBody
{
public:
	ODRX_DECLARE_MEMBERS(OdDbAssocBlendSurfaceActionBody);

	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbAssocBlendSurfaceActionBody(bool createImpObj = true);

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbAssocBlendSurfaceActionBody();

	//const OdDbSmartCenterMarkParametersPtr parameters() const;
	//OdDbSmartCenterMarkParametersPtr& parameters();
	//virtual void modified(const OdDbObject* pObject);
	//virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
	/** \details
	Creates associative AssocRotatedDim: action, action body, dependencies etc.
	*/
  /** \details
  Creates associative blend: action, action body, dependencies etc.
  */
  static OdResult createInstance(
    const OdDbFullSubentPathArray entPathArray,
    OdGePoint3dArray ptsSelected,
    OdDbObjectId& centerLineId,
    OdDbObjectId& actionBodyId);

	///// <summary><para> 
	///// Get continuity of start profile
	///// </para><para>
  OdInt16  startEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;

	///// <summary><para> 
	///// Set continuity of start profile of the surface
	///// </para><para>
  OdResult  setStartEdgeContinuity(OdDbAssocActionPtr pAction, OdInt16 value, const OdString& expression, const OdString& evaluatorId);

	///// <summary><para> 
	///// Get continuity of end profile
	///// </para><para>
  OdInt16  endEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;
  OdArray<OdInt16>  arrayEdgeContinuity(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;
  OdResult  setArrayEdgeContinuity(const OdDbAssocActionPtr pAction, OdArray<OdInt16>& values, OdString& expression, OdString& evaluatorId);

  ///// <summary><para> 
	///// Set continuity of end profile
	///// </para><para>
  OdResult  setEndEdgeContinuity(OdDbAssocActionPtr pAction, OdInt16 value, const OdString& expression, const OdString& evaluatorId);

	///// <summary><para> 
	///// Get bulge factor of end profil
	///// </para><para>
  double  startEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;
  OdArray<double>  arrayEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;
  OdResult  setArrayEdgeBulge(const OdDbAssocActionPtr pAction, OdArray<double>& values, OdString& expression, OdString& evaluatorId);

	///// <summary><para> 
	///// Set bulge factor
	///// </para><para>
	OdResult  setStartEdgeBulge(OdDbAssocActionPtr pAction, double value, const OdString& expression, const OdString& evaluatorId);

	///// <summary><para> 
	///// Get bulge factor of end profile
	///// </para><para>
  double  endEdgeBulge(const OdDbAssocActionPtr pAction, OdString& expression, OdString& evaluatorId) const;

	///// <summary><para> 
	///// Set bulge factor of end profile
	///// </para><para>
  OdResult  setEndEdgeBulge(OdDbAssocActionPtr pAction, double value, const OdString& expression, const OdString& evaluatorId);
};

typedef OdSmartPtr<OdDbAssocBlendSurfaceActionBody> OdDbAssocBlendSurfaceActionBodyPtr;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TD_PackPop.h"

#endif


