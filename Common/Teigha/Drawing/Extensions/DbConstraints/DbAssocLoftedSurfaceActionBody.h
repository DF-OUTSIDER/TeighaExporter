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

#ifndef OdDbAssocLoftedSurfaceActionBody_INCLUDED_
#define OdDbAssocLoftedSurfaceActionBody_INCLUDED_ /*!DOM*/
#include "DbAssocSurfaceActionBody.h"
#include "DbLoftOptions.h"
#include "TD_PackPush.h"
#include "DbAssocSurfacesParameters.h"
#include "DbImpAssocSurfacesParameters.h"
/** \details
This class represents the base class OdDbAssocLoftedSurfaceActionBody for the action bodies.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocLoftedSurfaceActionBody : public OdDbAssocPathBasedSurfaceActionBody
{
public:
	ODRX_DECLARE_MEMBERS(OdDbAssocLoftedSurfaceActionBody);

	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbAssocLoftedSurfaceActionBody(bool createImpObj = true);

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbAssocLoftedSurfaceActionBody();
	const OdDbAssocLoftParametersPtr parameters() const;
  OdDbAssocLoftParametersPtr& parameters();
	//virtual void modified(const OdDbObject* pObject);
	//virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
	/** \details
	Creates associative AssocRotatedDim: action, action body, dependencies etc.
	*/
	static OdResult createInstance(
		const OdDbFullSubentPathArray crossSectionsArray,
		const OdDbFullSubentPathArray guideCurves,// TODO
		const OdDbFullSubentPath pathCurve,// TODO
		const OdDbLoftOptions& loftOptions,// TODO
		//const OdArray<int>& continuityArray,
		//const OdArray<double>& bulgeArray,
		//bool bEnabled,
		OdDbObjectId& actionBodyId);

};

typedef OdSmartPtr<OdDbAssocLoftedSurfaceActionBody> OdDbAssocLoftedSurfaceActionBodyPtr;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TD_PackPop.h"

#endif

