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

#ifndef DbSmartCenterActionBodyBase_INCLUDED_
#define DbSmartCenterActionBodyBase_INCLUDED_ /*!DOM*/

#include "DbAssocActionBody.h"
#include "DbGeomRef.h"
#include "TD_PackPush.h"

/** \details
This class represents the base class dDbSmartCenterActionBodyBase for the smart center action bodies.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT ODRX_ABSTRACT OdDbSmartCenterActionBodyBase : public OdDbAssocActionBody
{
public:
	ODRX_DECLARE_MEMBERS(OdDbSmartCenterActionBodyBase);

	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbSmartCenterActionBodyBase();
	/////////////////////////////////////////////////////////////////////From Arx  
	// is abstract via evaluateOverride()

	//AcDbSmartCenterActionBodyBase(void);
	//virtual ~AcDbSmartCenterActionBodyBase(void);
	//void __autoclassinit2(unsigned __int64);
	//static OdDbSmartCenterActionBodyBase *  cast(AcRxObject const *);
	//static OdRxClass *  desc(void);

	//virtual enum OdResult dwgInFields(OdDbDwgFiler *);

	////virtual enum OdResult dwgOutFields(OdDbDwgFiler *)const;
	//virtual void dwgOutFields(OdDbDwgFiler *) const;

	//virtual enum OdResult dxfInFields(OdDbDxfFiler *);
	//
	////virtual enum OdResult dxfOutFields(OdDbDxfFiler *)const;
	//virtual void dxfOutFields(OdDbDxfFiler *)const;

	//virtual void evaluateOverride(void);
	//bool getBoolParam(OdString const &, bool)const;
	//double getDoubleParam(OdString const &)const;
	//
	//OdDbEdgeRef getInputEdge(int)const;
	//OdDbObjectId getObjectIdParam(OdString const &)const;
	//static wchar_t const *  inputEdgeParamName(void);
	//static void rxInit(void(*)(OdRxClass const *, wchar_t *  &, int));
	//enum OdResult setBoolParam(OdString const &, bool);
	//enum OdResult setDoubleParam(OdString const &, double);
	//enum OdResult setInputEdge(int, OdDbEdgeRef const &);
	//enum OdResult setObjectIdParam(OdString const &, OdDbObjectId);

};

/** \details
The typified smart pointer. This template class is specialization of the OdSmartPtr class
for the OdDbSmartCenterActionBodyBase class.

\sa
<link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSmartCenterActionBodyBase> OdDbSmartCenterActionBodyBasePtr;

#include "TD_PackPop.h"

#endif
