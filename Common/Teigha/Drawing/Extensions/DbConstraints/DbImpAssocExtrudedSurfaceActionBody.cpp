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

#include "OdaCommon.h"
#include "NextCodeDefs.h"
#include "DbAssocArrayModifyActionBody.h"
#include "DbBlockTableRecord.h"
#include "DbAssocDependency.h"
#include "DbObjectId.h"
#include "DbPolyline.h"
#include "DbExtrudedSurface.h"
#include "DbImpAssocExtrudedSurfaceActionBody.h"

//////////////////////////////////////////////////////////////////////////

OdDbImpAssocExtrudedSurfaceActionBody::OdDbImpAssocExtrudedSurfaceActionBody()
{
	m_unknownInt32 = 0;
}

OdDbImpAssocExtrudedSurfaceActionBody::~OdDbImpAssocExtrudedSurfaceActionBody()
{
}

OdResult OdDbImpAssocExtrudedSurfaceActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
	OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dwgInFields(pFiler);

	if (res != eOk)
		return res;
	
	m_unknownInt32 = pFiler->rdInt32();  // 0
	
	return res;
}

void OdDbImpAssocExtrudedSurfaceActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
	OdDbImpAssocPathBasedSurfaceActionBody::dwgOutFields(pFiler);
	pFiler->wrInt32(m_unknownInt32);
}

OdResult OdDbImpAssocExtrudedSurfaceActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
	OdResult res = OdDbImpAssocPathBasedSurfaceActionBody::dxfInFields(pFiler);

	if (res != eOk)
		return res;

	if (!pFiler->atSubclassData(OdDbAssocExtrudedSurfaceActionBody::desc()->name()))
	{
		ODA_FAIL_ONCE();
		return eMakeMeProxy;
	}

	NEXT_CODE(90)
	m_unknownInt32 = pFiler->rdUInt32();

	return eOk;
}

void OdDbImpAssocExtrudedSurfaceActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
	OdDbImpAssocPathBasedSurfaceActionBody::dxfOutFields(pFiler);

	pFiler->wrSubclassMarker(OdDbAssocExtrudedSurfaceActionBody::desc()->name());
	pFiler->wrUInt32(90, m_unknownInt32);
}

//const OdDbSmartCenterMarkParametersPtr OdDbImpAssocExtrudedSurfaceActionBody::parameters() const
//{
//  return m_pParams;
//}
//
//OdDbSmartCenterMarkParametersPtr& OdDbImpAssocExtrudedSurfaceActionBody::parameters()
//{
//  return m_pParams;
//}
//
void OdDbImpAssocExtrudedSurfaceActionBody::evaluateOverride(OdDbObjectId parentActionId)
{
OdDbAssocActionPtr pAction = OdDbAssocAction::cast(parentActionId.openObject(OdDb::kForWrite));
	OdDbObjectIdArray snapPointParams = pAction->ownedParams();

	OdDbAssocDependencyPtr pSurfaceAssocDep = m_surfaceObjectDepId.openObject(OdDb::kForWrite);

	if (pSurfaceAssocDep.isNull())
		return;

  OdDbEntityPtr pEntRef = checkAccompanying(parentActionId, pAction);

  if (pEntRef.isNull())
    return;

  OdDbExtrudedSurfacePtr pSurface = OdDbExtrudedSurface::cast(pEntRef);
	OdDbEntityPtr pSurfaceAsEnt = OdDbEntity::cast(pSurfaceAssocDep->dependentOnObject().openObject(OdDb::kForWrite));
	ODA_ASSERT(!pSurface.isNull());

	OdGeVector3d seepVector = pSurface->getSweepVec();
	OdDbSweepOptions sweepOptsTemp;
	pSurface->getSweepOptions(sweepOptsTemp);
  //taperAngle() TODO

	OdDbObjectIdArray objDependentIDs;
	pAction->getDependencies(true, true, objDependentIDs); //getDependentObjects(true, true, objDependentIDs);

	ODA_ASSERT(objDependentIDs.length() > 1);
	OdDbEntityPtr pEntTemp;
	OdDbEntityPtr pEntToExtrude;

	for (OdUInt32 i = 0; i < objDependentIDs.length(); i++)
	{
		OdDbAssocDependencyPtr pDep = objDependentIDs.getAt(i).openObject(OdDb::kForRead);
		pEntTemp = OdDbEntity::cast(pDep->dependentOnObject().openObject());

		if (pEntTemp != pSurfaceAsEnt)
		{
			pEntToExtrude = pEntTemp;
		}
	}

	OdResult esTemp = eNotInitializedYet;

	if (!pEntToExtrude.isNull())
	{
		OdDbExtrudedSurfacePtr pExtrudedSurfTemp = OdDbExtrudedSurface::createObject();

    pExtrudedSurfTemp->setUIsolineDensity(pSurface->uIsolineDensity());
    pExtrudedSurfTemp->setVIsolineDensity(pSurface->vIsolineDensity());

    OdString expression , evaluatorId;
    double haightOpt = height( pAction, expression /*= dummyString()*/, evaluatorId/* = dummyString()*/);
    double taperA  = taperAngle(pAction, expression /*= dummyString()*/, evaluatorId/* = dummyString()*/);
    seepVector.setLength(haightOpt);
    sweepOptsTemp.setDraftAngle(taperA);

    esTemp = pExtrudedSurfTemp->createExtrudedSurface(pEntToExtrude, seepVector, sweepOptsTemp);// , 0, true);

		if (esTemp == eOk)
		{
			pSurface->handOverTo(pExtrudedSurfTemp);
		}
	}
	//delete[] objIdForLoft;
}

OdResult OdDbImpAssocExtrudedSurfaceActionBody::setTaperAngle(OdDbAssocActionPtr pAction, OdDouble dTaperAngle, const OdString& expression,
  const OdString& evaluatorId )
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(dTaperAngle);
  OdString emptyStr("");
  return pAction->setValueParam(kExtrusionTaperAngle, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

OdDouble OdDbImpAssocExtrudedSurfaceActionBody::taperAngle(const OdDbAssocActionPtr pAction, OdString& expression/* = dummyString()*/, OdString& evaluatorId /*= dummyString()*/) const
{
  OdDouble retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kExtrusionTaperAngle, *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return -1;
  }

  return value->getDouble();
}

OdResult OdDbImpAssocExtrudedSurfaceActionBody::setHeight(OdDbAssocActionPtr pAction, OdDouble dHeight, const OdString& expression, const OdString& evaluatorId )
{
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init(dHeight);
  OdString emptyStr("");
  return pAction->setValueParam(kExtrusionHeightParamName, *value.get(), expression, evaluatorId, emptyStr, false, 0);
}

OdDouble OdDbImpAssocExtrudedSurfaceActionBody::height(const OdDbAssocActionPtr pAction, OdString& expression /*= dummyString()*/, OdString& evaluatorId/* = dummyString()*/) const
{
  OdDouble retD = -1;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(retD);

  if (pAction->getValueParam(kExtrusionHeightParamName, *value.get(), expression, evaluatorId, 0) != eOk)
  {
    return -1;
  }

  return value->getDouble();
}
