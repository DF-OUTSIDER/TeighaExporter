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
#include "DbAssocActionParam.h"
#include "DbImpAssocActionParam.h"
#include "NextCodeDefs.h"
#include "DbAssocDependency.h"
#include "DbAssocGeomDependency.h"
#include "GeDwgIO.h"
#include "GeDxfIO.h"
#include "DbFiler.h"
#include "Ge/GeKnotVector.h"
#include <typeinfo>

OdDbImpAssocActionParam::OdDbImpAssocActionParam()
  : m_status(kIsUpToDateAssocStatus)
{
}

OdDbImpAssocActionParam::~OdDbImpAssocActionParam()
{
}

OdResult OdDbImpAssocActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdUInt16 ver = pFiler->rdInt16();
  ODA_ASSERT_ONCE(ver == (pFiler->dwgVersion() > OdDb::vAC24 ? 1 : 0));
  if (ver > 1)
    return eMakeMeProxy;

  if (ver && pFiler->rdInt32())
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  m_sName = pFiler->rdString();

  return eOk;
}

void OdDbImpAssocActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdUInt16 ver = (pFiler->dwgVersion() > OdDb::vAC24) ? 1 : 0;
  pFiler->wrInt16(ver);

  if (ver)
    pFiler->wrInt32(0);

  pFiler->wrString(m_sName);
}

OdResult OdDbImpAssocActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  if (!pFiler->atSubclassData(OdDbAssocActionParam::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdInt32 ver = pFiler->rdInt32();
  ODA_ASSERT_ONCE(ver == (pFiler->dwgVersion() > OdDb::vAC24 ? 1 : 0));
  if (ver > 1)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }
  if (ver)
  {
    NEXT_CODE(90)
    if (pFiler->rdInt32())
    {
      ODA_FAIL_ONCE(); // TODO
      return eMakeMeProxy;
    }
  }

  NEXT_CODE(1)
  m_sName = pFiler->rdString();

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrSubclassMarker(OdDbAssocActionParam::desc()->name());

  OdInt32 ver = (pFiler->dwgVersion() > OdDb::vAC24) ? 1 : 0;
  pFiler->wrInt32(90, ver);
  if (ver)
    pFiler->wrInt32(90, 0);
  pFiler->wrString(1, m_sName);
}

OdStringArray OdDbImpAssocActionParam::compareWith(OdDbImpAssocActionParam *paramIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/)  const
{
	OdStringArray retArr;  

	if (m_sName.compare(paramIDToCompare->m_sName) != 0)
		retArr.append(CS_PARAM_NAME);

	return retArr; 
}
//////////////////////////////////////////////////////////////////////////

OdDbImpAssocSingleDependencyActionParam::OdDbImpAssocSingleDependencyActionParam()
{
	m_DependencyId = NULL;
}

OdDbImpAssocSingleDependencyActionParam::~OdDbImpAssocSingleDependencyActionParam()
{
}

OdResult OdDbImpAssocSingleDependencyActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
	OdResult res = eNotImplemented;

	res = OdDbImpAssocActionParam::dwgInFields(pFiler, objectId);
	if (res != eOk)
			return res;

  OdUInt16 ver = pFiler->rdInt16();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  m_DependencyId = pFiler->rdSoftPointerId();
  return eOk;
}

void OdDbImpAssocSingleDependencyActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocActionParam::dwgOutFields(pFiler, objectId);

  OdUInt16 ver = 0;
  pFiler->wrInt16(ver);

  pFiler->wrSoftPointerId(m_DependencyId);
}

OdResult OdDbImpAssocSingleDependencyActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
	OdResult res = eNotImplemented;

	res = OdDbImpAssocActionParam::dxfInFields(pFiler, objectId);
	if (res != eOk)
			return res;

  if (!pFiler->atSubclassData(L"AcDbAssocSingleDependencyActionParam"))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdInt32 ver = pFiler->rdInt32();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(330)
  m_DependencyId = pFiler->rdObjectId();

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocSingleDependencyActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
	OdDbImpAssocActionParam::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker(L"AcDbAssocSingleDependencyActionParam");
  OdInt32 ver = 0;
  pFiler->wrInt32(90, ver);
  pFiler->wrObjectId(330, m_DependencyId);
}

OdStringArray OdDbImpAssocSingleDependencyActionParam::compareWith(OdDbImpAssocActionParam *paramIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/)  const
{
  OdStringArray retArr;
  OdDbImpAssocSingleDependencyActionParam* pComparedParam = static_cast<OdDbImpAssocSingleDependencyActionParam *>(paramIDToCompare);

  ODA_ASSERT(pComparedParam != NULL);

  if (NULL != pComparedParam)
  {
    retArr = OdDbImpAssocActionParam::compareWith(paramIDToCompare, filerVersion /*OdDb::kDHL_CURRENT*/);

    if (typeid(*(m_DependencyId.openObject())) != typeid(*(pComparedParam->m_DependencyId.openObject())))
    {
      retArr.append(CS_DIFFERENT_OBJECTS);
    }
  }

  return retArr;
}

OdDbObjectId OdDbImpAssocSingleDependencyActionParam::dependentOnObjectId() const
{
  OdDbObjectId idRes;
  OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(m_DependencyId.openObject());
  if (!pDep.isNull())
  {
    idRes = pDep->dependentOnObject();
  }
  return idRes;
}

OdResult OdDbImpAssocSingleDependencyActionParam::detachDependencies( OdDbObjectId parentActionId )
{
  OdDbAssocActionPtr pAction = parentActionId.openObject( OdDb::kForWrite );
  pAction->removeDependency( m_DependencyId, true );
  m_DependencyId.setNull();
  return eOk;
}

OdResult OdDbImpAssocSingleDependencyActionParam::setObject(OdDbObjectId actionId,
  const OdDbCompoundObjectId* object,
  bool isReadDependency /*= true*/,
  bool isWriteDependency /*= false*/)
{
  ODA_ASSERT_ONCE(!actionId.isNull());

  if (isReadDependency || isWriteDependency)
  {
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast( actionId.openObject( OdDb::kForWrite ) );

    OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();
    actionId.database()->addOdDbObject( pDependency );
    pDependency->setIsReadDependency( isReadDependency );
    pDependency->setIsWriteDependency( isWriteDependency );
    pAction->addDependency( pDependency->objectId(), true );
    pDependency->attachToObject( object );
    m_DependencyId = pDependency->objectId();
  }
  return eOk;
}

OdResult OdDbImpAssocSingleDependencyActionParam::setObjectOnly(OdDbObjectId actionId,
  const OdDbCompoundObjectId* object,
  bool isReadDependency /*= true*/,
  bool isWriteDependency /*= false*/)
{
  ODA_ASSERT_ONCE(!actionId.isNull());

  if (isReadDependency || isWriteDependency)
  {
    m_DependencyId = object->leafId();
  }

  return eOk;
}


OdResult OdDbImpAssocSingleDependencyActionParam::getObject(OdDbCompoundObjectId* idObject) const
{
  idObject->setEmpty();

  OdResult res;
  OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::cast(m_DependencyId.openObject(OdDb::kForRead));
  if (pDependency.isNull())
  {
    return eOk;
  }
  OdDbCompoundObjectIdPtr cmpId;
  res = pDependency->getDependentOnCompoundObject(cmpId);
  if (eOk != res)
  {
    return res;
  }
  *idObject = *idObject;
  return eOk;
}

//-------------------------------------------------------------------------------------------------------------------
// Replace: OdDbObjectId m_AssocViewRepHatchActionParam -> OdDbObjectIdArray m_ActionParams

OdDbImpAssocCompoundActionParam::OdDbImpAssocCompoundActionParam()
  : m_Unknown1(0)
{
  m_usingInheritor = false;
  m_Unknown1 = 0;
  m_Unknown2 = 0;
  m_UseAdditionalAnchors = 0;
  m_AnchorId1 = NULL;
  m_AnchorId2 = NULL;
  m_Unknown3 = 0;
  m_AnchorId3= NULL;
}

OdDbImpAssocCompoundActionParam::~OdDbImpAssocCompoundActionParam()
{
}

OdResult OdDbImpAssocCompoundActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  if (!m_usingInheritor)
  {
    OdResult res = OdDbImpAssocActionParam::dwgInFields(pFiler, objectId);
    if (res != eOk)
      return res;
  }

  OdUInt16 ver = pFiler->rdInt16();
  ODA_ASSERT_ONCE(!ver);           // TODO
  if (ver)
    return eMakeMeProxy;

  m_Unknown1 = pFiler->rdInt16();  // 0
  ODA_ASSERT_ONCE(!m_Unknown1);    // test

  int nParamsSize = pFiler->rdInt32();
  m_ActionParams.resize(nParamsSize);
  OdDbObjectId * pId = m_ActionParams.asArrayPtr();
  while (nParamsSize--)
  {
    (*pId++) = pFiler->rdHardOwnershipId();
  }

  if (m_usingInheritor)
  {
    m_Unknown2 = pFiler->rdInt16();  // 0
    m_UseAdditionalAnchors = pFiler->rdInt32();  // 0
    m_AnchorId1 = pFiler->rdSoftPointerId();
  }

  if (m_UseAdditionalAnchors != 0)
  {
    m_AnchorId2 = pFiler->rdSoftPointerId();
    m_Unknown3 = pFiler->rdInt32();
    m_AnchorId3 = pFiler->rdSoftPointerId();
  }

  return eOk;
}

void OdDbImpAssocCompoundActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  if (!m_usingInheritor)
  {
    OdDbImpAssocActionParam::dwgOutFields(pFiler, objectId);
  }

  OdUInt16 ver = 0;
  pFiler->wrInt16(ver);

  ODA_ASSERT_ONCE(!m_Unknown1); // test
  pFiler->wrInt16(m_Unknown1);  // 0

  int nParamsSize = m_ActionParams.size();
  pFiler->wrInt32(nParamsSize);
  const OdDbObjectId * pId = m_ActionParams.asArrayPtr();
  while (nParamsSize--)
  {
    pFiler->wrHardOwnershipId(*pId++);
  }

  if (m_usingInheritor)
  {
    pFiler->wrInt16(m_Unknown2);  // 0
    pFiler->wrInt32(m_UseAdditionalAnchors);  // 0
    pFiler->wrSoftPointerId(m_AnchorId1);
  }

  if (m_UseAdditionalAnchors != 0)
  {
    pFiler->wrSoftPointerId(m_AnchorId2);
    pFiler->wrInt32(m_Unknown3);
    pFiler->wrSoftPointerId(m_AnchorId3);
  }
}

OdResult OdDbImpAssocCompoundActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  if (!m_usingInheritor)
  {
    OdResult res = OdDbImpAssocActionParam::dxfInFields(pFiler, objectId);
    if (res != eOk)
      return res;
  }

  if (!pFiler->atSubclassData(OdDbAssocCompoundActionParam::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdUInt32 ver = pFiler->rdUInt32();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_Unknown1 = (OdUInt16) pFiler->rdUInt32(); // 0
  ODA_ASSERT_ONCE(!m_Unknown1); // test

  NEXT_CODE(90)
  int nParamsSize = pFiler->rdInt32();
  m_ActionParams.reserve(nParamsSize);
  OdDbObjectId * pId = m_ActionParams.asArrayPtr();
  while (nParamsSize--)
  {
    NEXT_CODE(360)
    m_ActionParams.append(pFiler->rdObjectId());
  }

  if (m_usingInheritor)
  {
    NEXT_CODE(90)
    m_Unknown2 = pFiler->rdInt32();
    NEXT_CODE(90)
    m_UseAdditionalAnchors = pFiler->rdInt32();
    NEXT_CODE(330)
    m_AnchorId1 = pFiler->rdObjectId();
  }

  if (m_UseAdditionalAnchors != 0)
  {
    ODA_ASSERT_ONCE(!"Unexpected case! TODO");
    NEXT_CODE(330)
    m_AnchorId2 = pFiler->rdObjectId();
    NEXT_CODE(90)
    m_Unknown3 = pFiler->rdInt32();
    NEXT_CODE(330)
    m_AnchorId3 = pFiler->rdObjectId();
  }

  ODA_ASSERT_ONCE(pFiler->atEOF());

  return eOk;
}

void OdDbImpAssocCompoundActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  if (!m_usingInheritor)
  {
    OdDbImpAssocActionParam::dxfOutFields(pFiler, objectId);
  }

  pFiler->wrSubclassMarker(OdDbAssocCompoundActionParam::desc()->name());

  OdUInt32 ver = 0;
  pFiler->wrUInt32(90, ver);

  ODA_ASSERT_ONCE(!m_Unknown1); // test
  pFiler->wrUInt32(90, m_Unknown1);

  int nParamsSize = m_ActionParams.size();
  pFiler->wrUInt32(90, nParamsSize);
  const OdDbObjectId * pId = m_ActionParams.asArrayPtr();
  while (nParamsSize--)
  {
    pFiler->wrObjectId(360, *pId++);
  }

  if (m_usingInheritor)
  {
    pFiler->wrUInt32(90, m_Unknown2);
    pFiler->wrUInt32(90, m_UseAdditionalAnchors);
    pFiler->wrObjectId(330, m_AnchorId1);
  }

  if (m_UseAdditionalAnchors != 0)
  {
    pFiler->wrObjectId(330, m_AnchorId2);
    pFiler->wrUInt32(90, m_Unknown3);
    pFiler->wrObjectId(330, m_AnchorId3);
  }

}

OdStringArray OdDbImpAssocCompoundActionParam::compareWith(OdDbImpAssocActionParam *paramIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/)  const
{
  OdStringArray retArr;
  OdDbImpAssocCompoundActionParam* pComparedParam = static_cast<OdDbImpAssocCompoundActionParam *>(paramIDToCompare);

  ODA_ASSERT(pComparedParam != NULL);

  if (NULL != pComparedParam)
  {
    retArr = OdDbImpAssocActionParam::compareWith(paramIDToCompare, filerVersion /*OdDb::kDHL_CURRENT*/);

    if (m_Unknown1 != pComparedParam->m_Unknown1)
      retArr.append(CS_NOT_CRITICAL);

    int nParamsSize = m_ActionParams.size();

    if (nParamsSize != pComparedParam->m_ActionParams.size())
      retArr.append(CS_DIFFERENT_OBJECTS);
    else
    {
      const OdDbObjectId * pId = m_ActionParams.asArrayPtr();
      const OdDbObjectId * pComparedId = pComparedParam->m_ActionParams.asArrayPtr();

      OdDbObjectId m_surfaceObjectDepId;
      
      while (nParamsSize--)
      {
        if (typeid(*(*pId).openObject()) != typeid(*(*pComparedId).openObject()))
        {
          retArr.append(CS_DIFFERENT_OBJECTS);
          return retArr;
        }

        *pId++;
        *pComparedId++;
      }
    }

    if (m_usingInheritor != pComparedParam->m_usingInheritor)
    {
      retArr.append(CS_DIFFERENT_OBJECTS);
      return retArr;
    }
    else
    {
      if (m_Unknown2 != pComparedParam->m_Unknown2)
        retArr.append(CS_OBJECT_ERROR); // TODO

      if (m_UseAdditionalAnchors != pComparedParam->m_UseAdditionalAnchors)
        retArr.append(CS_OBJECT_ERROR); // TODO

      if (m_AnchorId1 != pComparedParam->m_AnchorId1)
        retArr.append(CS_OBJECT_ERROR); // TODO
    }

    if (m_UseAdditionalAnchors != pComparedParam->m_UseAdditionalAnchors)
    {
      retArr.append(CS_DIFFERENT_OBJECTS);
      return retArr;
    }
    else
    {
      if (m_AnchorId2 != pComparedParam->m_AnchorId2)
        retArr.append(CS_OBJECT_ERROR); // TODO

      if (m_Unknown3 != pComparedParam->m_Unknown3)
        retArr.append(CS_OBJECT_ERROR); // TODO

      if (m_AnchorId3 != pComparedParam->m_AnchorId3)
        retArr.append(CS_OBJECT_ERROR); // TODO
    }
  }

  return retArr;
}


OdResult OdDbImpAssocCompoundActionParam::addChildParam(const OdDbObjectId actionId, const OdDbObjectId thisObjectId, const OdString& paramName, OdRxClass* pParamClass, const OdDbObjectId anchorId, OdDbObjectId& paramId, int& paramIndex)
{
  ODA_ASSERT_ONCE(!paramName.isEmpty() && pParamClass && !actionId.isNull() && !actionId.isNull());

  OdDbObjectIdArray idPars = paramsAtName(paramName);
  if (paramIndex < (int)idPars.size())
  {
    paramId = idPars[paramIndex];
    return eAlreadyInDb;
  }

  OdDbObjectPtr pObj = OdDbObject::cast(pParamClass->create());
  if (pObj.isNull())
    return eBadObjType;

  OdDbAssocActionParamPtr pParam = OdDbAssocActionParam::cast(pObj.get());
  ODA_ASSERT_ONCE(!pParam.isNull());
  if (pParam.isNull())
    return eBadObjType;

  actionId.database()->addOdDbObject(pParam);

  pParam->setOwnerId(thisObjectId);
  pParam->setName(paramName);
  paramId = pParam->objectId();

  m_ActionParams.push_back(paramId);
  m_AnchorId1 = anchorId;

  return eOk;
}

OdDbObjectIdArray OdDbImpAssocCompoundActionParam::paramsAtName(const OdString& paramName) const
{
  ODA_ASSERT_ONCE(!paramName.isEmpty());

  OdDbObjectIdArray idsRes;
  OdUInt32 sz = m_ActionParams.size(),
    idx = 0;
  for (; idx < sz; idx++)
  {
    OdDbObjectPtr pObj = m_ActionParams[idx].openObject();
    ODA_ASSERT_ONCE(!pObj.isNull());
    if (pObj.isNull())
      continue;

    OdDbAssocActionParamPtr pParam = OdDbAssocActionParam::cast(pObj.get());
    ODA_ASSERT_ONCE(!pParam.isNull());
    if (pParam.isNull())
      continue;

    OdString sName = pParam->name();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (paramName != sName)
      continue;

    idsRes.push_back(m_ActionParams[idx]);
  }

  return idsRes;
}

OdDbObjectId OdDbImpAssocCompoundActionParam::paramAtName(const OdString& paramName,
  int index) const // = 0
{
  OdDbObjectIdArray ids = paramsAtName(paramName);
  ODA_ASSERT_ONCE(index >= 0);
  if (index < 0 || index >= (int)ids.size())
    return OdDbObjectId();
  return ids[index];
}

//-------------------------------------------------------------------------------------------------------------------

OdDbImpAssocObjectActionParam::OdDbImpAssocObjectActionParam()
  : m_isBase(true)
{
}

OdDbImpAssocObjectActionParam::~OdDbImpAssocObjectActionParam()
{
}

OdResult OdDbImpAssocObjectActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocSingleDependencyActionParam::dwgInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  OdUInt16 ver = pFiler->rdInt16();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }
  return eOk;
}

void OdDbImpAssocObjectActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocSingleDependencyActionParam::dwgOutFields(pFiler, objectId);

  OdUInt16 ver = 0;
  pFiler->wrInt16(ver);
}

OdResult OdDbImpAssocObjectActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocSingleDependencyActionParam::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocObjectActionParam::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdInt32 ver = pFiler->rdInt32();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocObjectActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocSingleDependencyActionParam::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker(OdDbAssocObjectActionParam::desc()->name());
  OdInt32 ver = 0;
  pFiler->wrInt32(90, ver);
}
/////////////////////////////////////////////////////////////////////////

OdDbImpAssocVertexActionParam::OdDbImpAssocVertexActionParam()
{
}

OdDbImpAssocVertexActionParam::~OdDbImpAssocVertexActionParam()
{
}

OdResult OdDbImpAssocVertexActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocSingleDependencyActionParam::dwgInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  int ver = pFiler->rdInt32();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  OdGePoint3d point = pFiler->rdPoint3d();
  m_vertexRef = OdDbVertexRef(point);
  ODA_ASSERT_ONCE(point == m_vertexRef.point());

  return eOk;
}

void OdDbImpAssocVertexActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocSingleDependencyActionParam::dwgOutFields(pFiler, objectId);

  int ver = 0;
  pFiler->wrInt32(ver);

  pFiler->wrPoint3d(m_vertexRef.point());
}

OdResult OdDbImpAssocVertexActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocSingleDependencyActionParam::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocVertexActionParam::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdInt32 ver = pFiler->rdInt32();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(10)
  OdGePoint3d point;
  pFiler->rdPoint3d(point);
  
  m_vertexRef = OdDbVertexRef(point);
  ODA_ASSERT_ONCE(point == m_vertexRef.point());

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocVertexActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocSingleDependencyActionParam::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker(OdDbAssocVertexActionParam::desc()->name());
  OdInt32 ver = 0;
  pFiler->wrInt32(90, ver);
  pFiler->wrPoint3d(10, m_vertexRef.point());
}

OdResult OdDbImpAssocVertexActionParam::getVertexRef( OdArray<OdDbVertexRef>& vertexRefs ) const
{
  vertexRefs.push_back( m_vertexRef );
  return eOk;
}

OdResult OdDbImpAssocVertexActionParam::getVertexRef( OdDbVertexRef& vertexRef ) const
{
  vertexRef = m_vertexRef;
  return eOk;
}

OdResult OdDbImpAssocVertexActionParam::setVertexRef( const OdDbVertexRef& vertexRef, bool isReadDependency, bool isWriteDependency, int dependencyOrder, OdDbObjectId actionId )
{
  if ( isReadDependency || isWriteDependency )
  {
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast( actionId.openObject( OdDb::kForWrite ) );

    OdDbSubentRefPtr pSubRef = OdDbSubentRef::cast( vertexRef.referencedRef() );

    if ( !pSubRef.isNull() )
    {
      OdDbAssocGeomDependencyPtr pDependency = OdDbAssocGeomDependency::createObject();
      actionId.database()->addOdDbObject( pDependency );
      pDependency->setIsReadDependency( isReadDependency );
      pDependency->setIsWriteDependency( isWriteDependency );
      OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
      pCompoundObjectId->setEmpty();

      pCompoundObjectId->set( vertexRef.entity() );

      pDependency->attachToObject( pCompoundObjectId );
      pDependency->setSubentity( pSubRef->subentId() );

      pAction->addDependency( pDependency->objectId(), true );

      m_DependencyId = pDependency->objectId();
    }
  }

  m_vertexRef = vertexRef;
  return eOk;
}


//////////////////////////////////////////////////////////////////////////

OdDbImpAssocEdgeActionParam::OdDbImpAssocEdgeActionParam()
{
}

OdDbImpAssocEdgeActionParam::~OdDbImpAssocEdgeActionParam()
{
}

OdResult OdDbImpAssocEdgeActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocSingleDependencyActionParam::dwgInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  int ver = pFiler->rdInt32();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  m_UnkownId = pFiler->rdSoftPointerId();
  
  if (!pFiler->rdBool())
  {
    m_edgeRef.reset();
  }
  else
  {
    OdInt32 entId = pFiler->rdInt32();

    OdSharedPtr<OdGeCurve3d> pCurve3d;

    switch (entId)
    {
    case 23: //case OdGe::kLineSeg3d: // to regenerate DbLine in m_dbEntIds 
      pCurve3d = new OdGeLineSeg3d();
      if (OdGeDwgIO::inFields(pFiler, *(OdGeLineSeg3d*) pCurve3d.get()) != eOk)
        return eMakeMeProxy;
      break;
    case 17: //case OdGe::kEllipArc3d: // to regenerate DbEllipse in m_dbEntIds // x3a8
      pCurve3d = new OdGeEllipArc3d();
      if (OdGeDwgIO::inFields(pFiler, *(OdGeEllipArc3d*) pCurve3d.get()) != eOk)
        return eMakeMeProxy;
      break;
    case 11: //case OdGe::kCircArc3d: // to regenerate DbArc in m_dbEntIds // budweiser2013.dwg
      pCurve3d = new OdGeCircArc3d();
      if (OdGeDwgIO::inFields(pFiler, *(OdGeCircArc3d*) pCurve3d.get()) != eOk)
        return eMakeMeProxy;
      break;
    case 42: //case OdGe::kNurbCurve3d: // to regenerate DbSpline in m_dbEntIds // budweiser2013.dwg x12fa8
      // TODO move kSpunSurface and kSegmentChain2d to end of enum // via 
      pCurve3d = new OdGeNurbCurve3d();
      if (OdGeDwgIO::inFields(pFiler, *(OdGeNurbCurve3d*) pCurve3d.get()) != eOk)
        return eMakeMeProxy;
      break;
    case 19: //case OdGe::kLine3d: // to regenerate DbLine in m_dbEntIds 
      pCurve3d = new OdGeLine3d();
      if (OdGeDwgIO::inFields(pFiler, *(OdGeLine3d*) pCurve3d.get()) != eOk)
        return eMakeMeProxy;
      break;
    case 47: 
      pCurve3d = new OdGeCompositeCurve3d();
      if (OdGeDwgIO::inFields(pFiler, *(OdGeCompositeCurve3d*) pCurve3d.get() ) != eOk)
        return eMakeMeProxy;
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      return eNotImplementedYet;
    }
  
    m_edgeRef = OdDbEdgeRef(pCurve3d);
  }
  return eOk;
}

void OdDbImpAssocEdgeActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocSingleDependencyActionParam::dwgOutFields(pFiler, objectId);

  int ver = 0;
  pFiler->wrInt32(ver);

  pFiler->wrSoftPointerId( m_UnkownId );

  bool bHasGeometry = !m_edgeRef.isEmpty();
  pFiler->wrBool( bHasGeometry );
  if (bHasGeometry)
  {
    OdInt32 entId = m_edgeRef.curve()->type();
    pFiler->wrInt32( entId );

    switch (entId)
    {
    case 23: //case OdGe::kLineSeg3d: // to regenerate DbLine in m_dbEntIds 
      OdGeDwgIO::outFields(pFiler, *(OdGeLineSeg3d*) m_edgeRef.curve());
      break;
    case 17: //case OdGe::kEllipArc3d: // to regenerate DbEllipse in m_dbEntIds // x3a8
      OdGeDwgIO::outFields(pFiler, *(OdGeEllipArc3d*) m_edgeRef.curve());
      break;
    case 11: //case OdGe::kCircArc3d: // to regenerate DbArc in m_dbEntIds // budweiser2013.dwg
      OdGeDwgIO::outFields(pFiler, *(OdGeCircArc3d*) m_edgeRef.curve());
      break;
    case 42: //case OdGe::kNurbCurve3d: // to regenerate DbSpline in m_dbEntIds // budweiser2013.dwg x12fa8
      // TODO move kSpunSurface and kSegmentChain2d to end of enum // via 
      OdGeDwgIO::outFields(pFiler, *(OdGeNurbCurve3d*) m_edgeRef.curve());
      break;
    case 19: //case OdGe::kNurbCurve3d: // to regenerate DbSpline in m_dbEntIds // budweiser2013.dwg x12fa8
      OdGeDwgIO::outFields(pFiler, *(OdGeLine3d*) m_edgeRef.curve());
      break;
    case 47:
      OdGeDwgIO::outFields(pFiler, *(OdGeCompositeCurve3d*) m_edgeRef.curve());
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      throw OdError(eNotImplementedYet);
    }
  }
}

OdResult OdDbImpAssocEdgeActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocSingleDependencyActionParam::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocEdgeActionParam::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdInt32 ver = pFiler->rdInt32();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(330)
  OdDbObjectId m_UnkownId = pFiler->rdObjectId();

  NEXT_CODE(290)  
  if (!pFiler->rdBool())
  {
    m_edgeRef.reset();
  }
  else
  {
	  NEXT_CODE(90)
		OdUInt32 entId = pFiler->rdUInt32();
	  OdSharedPtr<OdGeCurve3d> pCurve3d;
	  switch (entId)
	  {
	  case 23: //case OdGe::kLineSeg3d: // to regenerate DbLine in m_dbEntIds 
		  pCurve3d = new OdGeLineSeg3d();
		  if (OdGeDxfIO::inFields(pFiler, *(OdGeLineSeg3d*)pCurve3d.get()) != eOk)
			  return eMakeMeProxy;
		  break;
	  case 17: //case OdGe::kEllipArc3d: // to regenerate DbEllipse in m_dbEntIds // x3a8
		  pCurve3d = new OdGeEllipArc3d();
		  if (OdGeDxfIO::inFields(pFiler, *(OdGeEllipArc3d*)pCurve3d.get()) != eOk)
			  return eMakeMeProxy;
		  break;
	  case 11: //case OdGe::kCircArc3d: // to regenerate DbArc in m_dbEntIds // budweiser2013.dwg
		  pCurve3d = new OdGeCircArc3d();
		  if (OdGeDxfIO::inFields(pFiler, *(OdGeCircArc3d*)pCurve3d.get()) != eOk)
			  return eMakeMeProxy;
		  break;
	  case 42: //case OdGe::kNurbCurve3d: // to regenerate DbSpline in m_dbEntIds // budweiser2013.dwg x12fa8
		// TODO move kSpunSurface and kSegmentChain2d to end of enum // via 
		  pCurve3d = new OdGeNurbCurve3d();
		  if (OdGeDxfIO::inFields(pFiler, *(OdGeNurbCurve3d*)pCurve3d.get()) != eOk)
			  return eMakeMeProxy;
		  break;
	  case 19: //case OdGe::kLine3d: // to regenerate DbLine in m_dbEntIds 
		  pCurve3d = new OdGeLine3d();
		  if (OdGeDxfIO::inFields(pFiler, *(OdGeLine3d*)pCurve3d.get()) != eOk)
			  return eMakeMeProxy;
		  break;
	  case 47:
		  pCurve3d = new OdGeCompositeCurve3d();
		  if (OdGeDxfIO::inFields(pFiler, *(OdGeCompositeCurve3d*)pCurve3d.get()) != eOk)
			  return eMakeMeProxy;
		  break;
	  default:
		  ODA_FAIL_ONCE(); // TODO
		  return eNotImplementedYet;
	  }

	  m_edgeRef = OdDbEdgeRef(pCurve3d);
  }

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
   return eOk;
}

void OdDbImpAssocEdgeActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocSingleDependencyActionParam::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker(OdDbAssocEdgeActionParam::desc()->name());
  OdInt32 ver = 0;
  pFiler->wrInt32(90, ver);
  
  pFiler->wrObjectId( 330, m_UnkownId );

  bool bHasGeometry = !m_edgeRef.isEmpty();
  pFiler->wrBool( 290, bHasGeometry );
  if (bHasGeometry)
  {
    OdInt32 entId = m_edgeRef.curve()->type();
    pFiler->wrUInt32(90, entId);
    switch (entId)
    {
    case 23: //case OdGe::kLineSeg3d: // to regenerate DbLine in m_dbEntIds 
      OdGeDxfIO::outFields(pFiler, *(OdGeLineSeg3d*) m_edgeRef.curve());
      break;
    case 17: //case OdGe::kEllipArc3d: // to regenerate DbEllipse in m_dbEntIds // x3a8
      OdGeDxfIO::outFields(pFiler, *(OdGeEllipArc3d*) m_edgeRef.curve());
      break;
    case 11: //case OdGe::kCircArc3d: // to regenerate DbArc in m_dbEntIds // budweiser2013.dwg
      OdGeDxfIO::outFields(pFiler, *(OdGeCircArc3d*) m_edgeRef.curve());
      break;
    case 42: //case OdGe::kNurbCurve3d: // to regenerate DbSpline in m_dbEntIds // budweiser2013.dwg x12fa8
      // TODO move kSpunSurface and kSegmentChain2d to end of enum // via 
      OdGeDxfIO::outFields(pFiler, *(OdGeNurbCurve3d*) m_edgeRef.curve());
      break;
    case 19: //case OdGe::kLine3d: // to regenerate DbLine in m_dbEntIds 
      OdGeDxfIO::outFields(pFiler, *(OdGeLine3d*) m_edgeRef.curve());
      break;
    case 47:
      OdGeDxfIO::outFields(pFiler, *(OdGeCompositeCurve3d*) m_edgeRef.curve());
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      throw OdError(eNotImplementedYet);
    }
  }
}

OdStringArray CompareTwoCircArc3d(OdGeCircArc3d arc, OdGeCircArc3d arcToCompare)//type 11
{
  OdStringArray retArr;

  if (arc.center() != arcToCompare.center())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (arc.normal() != arcToCompare.normal())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (arc.normal() != arcToCompare.normal())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (arc.normal() != arcToCompare.normal())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (arc.radius() != arcToCompare.radius())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (arc.startAng() != arcToCompare.startAng())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (arc.endAng() != arcToCompare.endAng())
    retArr.append(CS_NOT_CRITICAL);// TODO

  return retArr;
}

OdStringArray CompareTwoLineSeg3d(OdGeLineSeg3d line, OdGeLineSeg3d lineToCompare)//type 23
{
  OdStringArray retArr;

  if (line.startPoint() != lineToCompare.startPoint())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (line.endPoint() - line.startPoint() != lineToCompare.endPoint() - lineToCompare.startPoint())
    retArr.append(CS_NOT_CRITICAL);// TODO

  return retArr;
}

OdStringArray CompareTwoEllipArc3d(OdGeEllipArc3d ellArc, OdGeEllipArc3d ellArcToCompare)// type 17
{
  OdStringArray retArr;

  if (ellArc.center() != ellArcToCompare.center())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (ellArc.majorAxis() != ellArcToCompare.majorAxis())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (ellArc.minorAxis() != ellArcToCompare.minorAxis())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (ellArc.majorRadius() != ellArcToCompare.majorRadius())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (ellArc.minorRadius() != ellArcToCompare.minorRadius())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (ellArc.startAng() != ellArcToCompare.startAng())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (ellArc.endAng() != ellArcToCompare.endAng())
    retArr.append(CS_NOT_CRITICAL);// TODO

  return retArr;
}

OdStringArray  CompareTwoNurbCurve3d(OdGeNurbCurve3d spline, OdGeNurbCurve3d splineToCompare) // type 42
{
  OdStringArray retArr;

  int degree = 0;
  bool rational = false, periodic = false;
  OdGeKnotVector knots;
  OdGeDoubleArray weights;
  OdGePoint3dArray controlPoints;
  double tolKnot = knots.tolerance();

  spline.getDefinitionData(degree, rational, periodic, knots, controlPoints, weights);

  int degree1 = 0;
  bool rational1 = false, periodic1 = false;
  OdGeKnotVector knots1;
  OdGeDoubleArray weights1;
  OdGePoint3dArray controlPoints1;
  double tolKnot1 = knots1.tolerance();

  splineToCompare.getDefinitionData(degree1, rational1, periodic1, knots1, controlPoints1, weights1);

  OdInt32 growSize = 8; // TODO // = knots.growLength();
                        //ODA_ASSERT_ONCE(growSize == 8); // test
                        //growSize = 8;

  if (spline.evalMode() != splineToCompare.evalMode())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (degree != degree1)
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (tolKnot != tolKnot1)
    retArr.append(CS_NOT_CRITICAL);// TODO

  OdInt32 countKnots = knots.length();
  OdInt32 countKnots1 = knots1.length();

  if (countKnots != countKnots1)
    retArr.append(CS_DIFFERENT_PARAM_CRITICAL);// TODO
  else
  {
    OdInt32 idx = 0;
    for (; idx < countKnots; idx++)
    {
      double knot = knots[idx];
      double knot1 = knots1[idx];

      if (knot != knot1)
        retArr.append(CS_NOT_CRITICAL);// TODO
    }

    OdInt32 countWeights = weights.size();
    OdInt32 countWeights1 = weights1.size();

    if (countWeights != countWeights1)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);// TODO
    else
    {
      for (idx = 0; idx < countWeights; idx++)
      {
        double weight = weights[idx]; // 1.000000 1.493825 ... 1.993101 1.000000
        double weight1 = weights1[idx]; // 1.000000 1.493825 ... 1.993101 1.000000

        if (weight != weight1)
          retArr.append(CS_NOT_CRITICAL);// TODO
      }

      OdInt32 countPoints = controlPoints.size();
      OdInt32 countPoints1 = controlPoints1.size();

      if (countPoints != countPoints1)
        retArr.append(CS_DIFFERENT_PARAM_CRITICAL);// TODO
      else
      {
        for (idx = 0; idx < countPoints; idx++)
        {
          OdGePoint3d pt = controlPoints[idx];
          OdGePoint3d pt1 = controlPoints1[idx];
  
          if (pt != pt1)
            retArr.append(CS_NOT_CRITICAL);// TODO
        }
      }
    }
  }

  return retArr;
}

OdStringArray  CompareTwoLine3d(OdGeLine3d line, OdGeLine3d lineToCompare)//type 19
{
  OdStringArray retArr;

  if (line.pointOnLine() != lineToCompare.pointOnLine())
    retArr.append(CS_NOT_CRITICAL);// TODO

  if (line.direction() != lineToCompare.direction())
    retArr.append(CS_NOT_CRITICAL);// TODO

  return retArr;
}

OdStringArray OdDbImpAssocEdgeActionParam::compareWith(OdDbImpAssocActionParam *paramIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/)  const
{
  OdStringArray retArr;
  OdDbImpAssocEdgeActionParam* pComparedParam = static_cast<OdDbImpAssocEdgeActionParam *>(paramIDToCompare);
  ODA_ASSERT(pComparedParam != NULL);

  if (NULL != pComparedParam)
  {
    retArr = OdDbImpAssocSingleDependencyActionParam::compareWith(paramIDToCompare, filerVersion /*OdDb::kDHL_CURRENT*/);

    if (m_UnkownId.isNull() != pComparedParam->m_UnkownId.isNull())
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
    else if (!m_UnkownId.isNull())
    {
      if (typeid(*(m_UnkownId.openObject())) != typeid(*(pComparedParam->m_UnkownId.openObject())))
      {
        retArr.append(CS_DIFFERENT_OBJECTS);
        //return retArr;
      }
    }

    if (m_edgeRef.isEmpty() != pComparedParam->m_edgeRef.isEmpty())
      retArr.append(CS_NOT_CRITICAL);// TODO
    else
    {
      if (!m_edgeRef.isEmpty())
      {
        if (m_edgeRef.curve()->type() != pComparedParam->m_edgeRef.curve()->type())
          retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

        switch (m_edgeRef.curve()->type())
        {
          case 23: //case OdGe::kLineSeg3d:  
          {
            OdStringArray retCompared = CompareTwoLineSeg3d(*(OdGeLineSeg3d*)m_edgeRef.curve(), *(OdGeLineSeg3d*)pComparedParam->m_edgeRef.curve());

            retArr.append(retCompared);
          }
            break;
          case 17: //case OdGe::kEllipArc3d:
          {
            OdStringArray retCompared = CompareTwoEllipArc3d(*(OdGeEllipArc3d*)m_edgeRef.curve(), *(OdGeEllipArc3d*)pComparedParam->m_edgeRef.curve());

            retArr.append(retCompared);
          }
            break;
          case 11: //case OdGe::kCircArc3d: 
          {
            OdStringArray retCompared = CompareTwoCircArc3d(*(OdGeCircArc3d*)m_edgeRef.curve(), *(OdGeCircArc3d*)pComparedParam->m_edgeRef.curve());
       
            retArr.append(retCompared);
          }
            break;
          case 42: //case OdGe::kNurbCurve3d:
          {
            OdStringArray retCompared = CompareTwoNurbCurve3d(*(OdGeNurbCurve3d*)m_edgeRef.curve(), *(OdGeNurbCurve3d*)pComparedParam->m_edgeRef.curve());

            retArr.append(retCompared);
          }
          break;
          case 19: //case OdGe::kLine3d: 
          {
            OdStringArray retCompared = CompareTwoLine3d(*(OdGeLine3d*)m_edgeRef.curve(), *(OdGeLine3d*)pComparedParam->m_edgeRef.curve());

            retArr.append(retCompared);
          }
          break;
          case 47:
          {
            OdGeCompositeCurve3d curve = *(OdGeCompositeCurve3d*)m_edgeRef.curve();
            OdGeCompositeCurve3d curveToCompare = *(OdGeCompositeCurve3d*)pComparedParam->m_edgeRef.curve();

            OdGeCurve3dPtrArray curvelist;
            curve.getCurveList(curvelist);

            OdGeCurve3dPtrArray curvelistToCompare;
            curveToCompare.getCurveList(curvelistToCompare);

            if (curvelist.size() != curvelistToCompare.size())
              retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
            else
            {
              for (unsigned int i = 0; i < curvelist.size(); i++)
              {
                if (curvelist[i]->type() != curvelistToCompare[i]->type())
                  retArr.append(CS_NOT_CRITICAL);// TODO

                switch (curvelist[i]->type())
                {
                case 11: //arc segment
                  {
                    ODA_ASSERT_ONCE(curvelist[i]->isKindOf(OdGe::kCircArc3d));
                    OdStringArray retCompared = CompareTwoCircArc3d(*(OdGeCircArc3d*)curvelist[i].get(), *(OdGeCircArc3d*)curvelistToCompare[i].get());
                    retArr.append(retCompared);// TODO
                  }
                  break;
                case 23://line segment
                  ODA_ASSERT_ONCE(curvelist[i]->isKindOf(OdGe::kLineSeg3d));
                  {
                    OdStringArray retCompared = CompareTwoLineSeg3d(*(OdGeLineSeg3d*)curvelist[i].get(), *(OdGeLineSeg3d*)curvelistToCompare[i].get());

                    retArr.append(retCompared);
                  }
                  break;
                default:
                  ODA_FAIL_ONCE(); // TODO
                }
              }
            }
          }
            break;
          default:
            ODA_FAIL_ONCE(); // TODO
            throw OdError(eNotImplementedYet);
        }
      }
    }

  }

  return retArr;
}

OdResult OdDbImpAssocEdgeActionParam::getEdgeRef( OdArray<OdDbEdgeRef>& edgeRefs ) const
{
  edgeRefs.push_back( m_edgeRef );
  return eOk;
}

OdResult OdDbImpAssocEdgeActionParam::setEdgeRef( const OdDbEdgeRef& edgeRef, bool isReadDependency, /* = true */ bool isWriteDependency, /* = false */ int dependencyOrder, OdDbObjectId actionId )
{
  if ( isReadDependency || isWriteDependency )
  {
    OdDbAssocActionPtr pAction = OdDbAssocAction::cast( actionId.openObject( OdDb::kForWrite ) );

    OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::createObject();
    actionId.database()->addOdDbObject( pDependency );
    pDependency->setIsReadDependency( isReadDependency );
    pDependency->setIsWriteDependency( isWriteDependency );
    OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
    pCompoundObjectId->setEmpty();

    pCompoundObjectId->set( edgeRef.entity() );
    pDependency->attachToObject( pCompoundObjectId );
    pAction->addDependency( pDependency->objectId(), true );

    m_DependencyId = pDependency->objectId();
  }

  m_edgeRef = edgeRef;

  return eOk;
}

OdResult OdDbImpAssocEdgeActionParam::getDependentOnCompoundObject( OdDbCompoundObjectId& compoundId ) const
{
  compoundId.setEmpty();

  OdResult res;
  OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::cast( m_DependencyId.openObject( OdDb::kForRead ) );
  if ( pDependency.isNull() )
  {
    return eOk;
  }
  OdDbCompoundObjectIdPtr cmpId;
  res = pDependency->getDependentOnCompoundObject( cmpId );
  if ( eOk != res )
  {
    return res;
  }
  compoundId = *cmpId;
  return eOk;
}

OdResult OdDbImpAssocEdgeActionParam::setEdgeSubentityGeometry( const OdGeCurve3d* pNewEdgeCurve )
{  
  m_edgeRef = OdDbEdgeRef(pNewEdgeCurve);
  return eOk;
}

OdResult OdDbImpAssocEdgeActionParam::setObject(OdDbObjectId actionId,
  const OdDbCompoundObjectId* object,
  bool isReadDependency /*= true*/,
  bool isWriteDependency /*= false*/)
{
  ODA_ASSERT_ONCE(!actionId.isNull());

  if (isReadDependency || isWriteDependency)
  {
    m_DependencyId = object->leafId();
  }

  return eOk;
}

OdResult OdDbImpAssocEdgeActionParam::getObject(OdDbCompoundObjectId* idObject) const  // TODO
{
  //idObject->setEmpty();

  //OdResult res;
  //OdDbAssocDependencyPtr pDependency = OdDbAssocDependency::cast(m_DependencyId.openObject(OdDb::kForRead));
  //if (pDependency.isNull())
  //{
  //  return eOk;
  //}
  //OdDbCompoundObjectIdPtr cmpId;
  //res = pDependency->getDependentOnCompoundObject(cmpId);
  //if (eOk != res)
  //{
  //  return res;
  //}
  //*idObject = *idObject;
  return eNotImplementedYet;
}
//////////////////////////////////////////////////////////////////////////
OdDbImpAssocPointRefActionParam::OdDbImpAssocPointRefActionParam()
{
}

OdDbImpAssocPointRefActionParam::~OdDbImpAssocPointRefActionParam()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
OdDbImpAssocOsnapPointRefActionParam::OdDbImpAssocOsnapPointRefActionParam()
{
  m_usingInheritor = true;
  m_UnknownParam1 = 0;
  m_OsnapMode = (OdDb::OsnapMode)0;
  m_NearPointParam = -1;
}

OdDbImpAssocOsnapPointRefActionParam::~OdDbImpAssocOsnapPointRefActionParam()
{

}
OdResult OdDbImpAssocOsnapPointRefActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocActionParam::dwgInFields(pFiler, objectId);

  if (res != eOk)
    return res;
  
  res = OdDbImpAssocCompoundActionParam::dwgInFields(pFiler, objectId);

  if (res != eOk)
    return res;

  m_UnknownParam1 = pFiler->rdInt16();
  m_OsnapMode = (OdDb::OsnapMode)pFiler->rdInt8();
  m_NearPointParam = pFiler->rdDouble();
  
  return eOk;
}

void OdDbImpAssocOsnapPointRefActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocActionParam::dwgOutFields(pFiler, objectId);
  OdDbImpAssocCompoundActionParam::dwgOutFields(pFiler, objectId);

  pFiler->wrInt16(m_UnknownParam1);
  pFiler->wrInt8(m_OsnapMode);
  pFiler->wrDouble(m_NearPointParam);
}

OdResult OdDbImpAssocOsnapPointRefActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocActionParam::dxfInFields(pFiler, objectId);
  
  if (res != eOk)
    return res;

  res = OdDbImpAssocCompoundActionParam::dxfInFields(pFiler, objectId);

  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocOsnapPointRefActionParam::desc()->dxfName()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_UnknownParam1 = pFiler->rdInt32();
  NEXT_CODE(90)
  m_OsnapMode = (OdDb::OsnapMode)pFiler->rdInt32();
  NEXT_CODE(40)
  m_NearPointParam = pFiler->rdDouble();
  ODA_ASSERT_ONCE(pFiler->atEOF());
  return res;
}

void OdDbImpAssocOsnapPointRefActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocActionParam::dxfOutFields(pFiler, objectId);
  OdDbImpAssocCompoundActionParam::dxfOutFields(pFiler, objectId);
  pFiler->wrSubclassMarker(OdDbAssocOsnapPointRefActionParam::desc()->dxfName());
  pFiler->wrInt32(90, m_UnknownParam1);
  pFiler->wrInt32(90, m_OsnapMode);
  pFiler->wrDouble(40, m_NearPointParam);
}

//TODO: AlexeyTyurin 
OdStringArray OdDbImpAssocOsnapPointRefActionParam::compareWith(OdDbImpAssocActionParam *paramIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/)  const
{
  OdStringArray retArr;
  OdDbImpAssocOsnapPointRefActionParam* pComparedParam = static_cast<OdDbImpAssocOsnapPointRefActionParam *>(paramIDToCompare);
  ODA_ASSERT(pComparedParam != NULL);

  if (NULL != pComparedParam)
  {
    if (m_sName.compare(pComparedParam->m_sName) != 0)
      retArr.append(CS_PARAM_NAME);
  }

  return retArr;
}

//-------------------------------------------------------------------------------------------------------------------
// Replace: OdDbObjectId m_AssocViewRepHatchActionParam -> OdDbObjectIdArray m_ActionParams

OdDbImpAssocPathActionParam::OdDbImpAssocPathActionParam()
	: m_UnknownPathActionParam1(0)
{
}

OdDbImpAssocPathActionParam::~OdDbImpAssocPathActionParam()
{
}

OdResult OdDbImpAssocPathActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
	OdResult res = OdDbImpAssocCompoundActionParam::dwgInFields(pFiler, objectId);
	if (res != eOk)
		return res;

	m_UnknownPathActionParam1 = pFiler->rdInt32();  // 0
	return eOk;
}

void OdDbImpAssocPathActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
	OdDbImpAssocCompoundActionParam::dwgOutFields(pFiler, objectId);
	pFiler->wrInt32(m_UnknownPathActionParam1);
}

OdResult OdDbImpAssocPathActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
	OdResult res = OdDbImpAssocCompoundActionParam::dxfInFields(pFiler, objectId);
	if (res != eOk)
		return res;

	if (!pFiler->atSubclassData(OdDbAssocPathActionParam::desc()->name()))
	{
		ODA_FAIL_ONCE();
		return eMakeMeProxy;
	}

	NEXT_CODE(90)
	m_UnknownPathActionParam1 = pFiler->rdInt32();

	return eOk;
}

void OdDbImpAssocPathActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
	OdDbImpAssocCompoundActionParam::dxfOutFields(pFiler, objectId);

	pFiler->wrSubclassMarker(OdDbAssocPathActionParam::desc()->name());
	pFiler->wrUInt32(90, m_UnknownPathActionParam1);
}

OdStringArray OdDbImpAssocPathActionParam::compareWith(OdDbImpAssocActionParam *paramIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/)  const
{
  OdStringArray retArr;
  OdDbImpAssocPathActionParam* pComparedParam = dynamic_cast<OdDbImpAssocPathActionParam *>(paramIDToCompare);
  ODA_ASSERT(pComparedParam != NULL);

  if (NULL != pComparedParam)
  {
    retArr = OdDbImpAssocCompoundActionParam::compareWith(paramIDToCompare, filerVersion /*OdDb::kDHL_CURRENT*/);
    
    if (m_sName.compare(pComparedParam->m_sName) != 0)
      retArr.append(CS_PARAM_NAME);

    if (m_UnknownPathActionParam1 != pComparedParam->m_UnknownPathActionParam1)
      retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
  }

  return retArr;
}

//-------------------------------------------------------------------------------------------------------------------
OdDbImpAssocFaceActionParam::OdDbImpAssocFaceActionParam()
	: m_UnknownPathActionParam1(0), m_UnknownPathActionParam2(0)
{
	m_pAsmBodyActionParam = new OdDbImpAssocAsmBodyActionParam();
}

OdDbImpAssocFaceActionParam::~OdDbImpAssocFaceActionParam()
{
	delete m_pAsmBodyActionParam;
}

OdResult OdDbImpAssocFaceActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
	//OdResult res = OdDbImpAssocActionParam::dwgInFields(pFiler, objectId);
	//if (res != eOk)
	//	return res;
	m_pAsmBodyActionParam->dwgInFields(pFiler, objectId);
	m_UnknownPathActionParam1 = pFiler->rdInt32();
	m_UnknownPathActionParam2 = pFiler->rdInt32();  // 0
	return eOk;
}

void OdDbImpAssocFaceActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
	//OdDbImpAssocActionParam::dwgOutFields(pFiler, objectId);
	m_pAsmBodyActionParam->dwgOutFields(pFiler, objectId);
	pFiler->wrInt32(m_UnknownPathActionParam1);
	pFiler->wrInt32(m_UnknownPathActionParam2);
}

OdResult OdDbImpAssocFaceActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
	//OdResult res = OdDbImpAssocActionParam::dxfInFields(pFiler, objectId);
	//if (res != eOk)
	//	return res;

	OdResult res = m_pAsmBodyActionParam->dxfInFields(pFiler, objectId);

	if (res != eOk)
		return res;

	if (!pFiler->atSubclassData(OdDbAssocFaceActionParam::desc()->name()))
	{
		ODA_FAIL_ONCE();
		return eMakeMeProxy;
	}

	NEXT_CODE(90)
	m_UnknownPathActionParam1 = pFiler->rdUInt32();

	NEXT_CODE(90)
	m_UnknownPathActionParam2 = pFiler->rdUInt32();

	ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
	return eOk;
}

void OdDbImpAssocFaceActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
	///OdDbImpAssocActionParam::dxfOutFields(pFiler, objectId);
	m_pAsmBodyActionParam->dxfOutFields(pFiler, objectId);

	pFiler->wrSubclassMarker(OdDbAssocFaceActionParam::desc()->name());
	pFiler->wrUInt32(90, m_UnknownPathActionParam1);
	pFiler->wrUInt32(90, m_UnknownPathActionParam2);
}

//-------------------------------------------------------------------------------------------------------------------
OdDbImpAssocTrimmingBodyActionParam::OdDbImpAssocTrimmingBodyActionParam()
{
}

OdDbImpAssocTrimmingBodyActionParam::~OdDbImpAssocTrimmingBodyActionParam()
{
}

OdResult OdDbImpAssocTrimmingBodyActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
	OdResult res = OdDbImpAssocFaceActionParam::dwgInFields(pFiler, objectId);
	if (res != eOk)
		return res;

	m_unknownTrimmingBodyParam1 = pFiler->rdInt32();
	m_unknownTrimmingBodyParam2 = pFiler->rdInt8();  // 0
	return eOk;
}

void OdDbImpAssocTrimmingBodyActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
	OdDbImpAssocFaceActionParam::dwgOutFields(pFiler, objectId);
	pFiler->wrInt32(m_unknownTrimmingBodyParam1);
	pFiler->wrInt8(m_unknownTrimmingBodyParam2);
}

OdResult OdDbImpAssocTrimmingBodyActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
	OdResult res = OdDbImpAssocFaceActionParam::dxfInFields(pFiler, objectId);
	if (res != eOk)
		return res;

	if (!pFiler->atSubclassData(OdDbAssocTrimmingBodyActionParam::desc()->name()))
	{
		ODA_FAIL_ONCE();
		return eMakeMeProxy;
	}

	NEXT_CODE(90)
	m_unknownTrimmingBodyParam1 = pFiler->rdInt32();

	NEXT_CODE(280)
	m_unknownTrimmingBodyParam2 = pFiler->rdInt8();

	return eOk;
}

void OdDbImpAssocTrimmingBodyActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
	OdDbImpAssocFaceActionParam::dxfOutFields(pFiler, objectId);
	pFiler->wrSubclassMarker(OdDbAssocTrimmingBodyActionParam::desc()->name());
	pFiler->wrInt32(90, m_unknownTrimmingBodyParam1);
	pFiler->wrInt8(280, m_unknownTrimmingBodyParam2);
}
//-------------------------------------------------------------------------------------------------------------------
OdDbImpAssocAsmBodyActionParam::OdDbImpAssocAsmBodyActionParam()
	: m_UnknownPathActionParam1(0), m_UnknownBool(false)
{
	m_pSingleDependencyActionParam = new OdDbImpAssocSingleDependencyActionParam();
}

OdDbImpAssocAsmBodyActionParam::~OdDbImpAssocAsmBodyActionParam()
{
	delete m_pSingleDependencyActionParam;
}

OdResult OdDbImpAssocAsmBodyActionParam::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
	OdResult res = eNotImplemented;

		//res = OdDbImpAssocActionParam::dwgInFields(pFiler, objectId);
		//if (res != eOk)
		//	return res;
	res = m_pSingleDependencyActionParam->dwgInFields(pFiler, objectId);

	m_UnknownPathActionParam1 = pFiler->rdInt32();
	m_UnknownBool = pFiler->rdBool();
	return eOk;
}

void OdDbImpAssocAsmBodyActionParam::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
	//OdDbImpAssocActionParam::dwgOutFields(pFiler, objectId);
	m_pSingleDependencyActionParam->dwgOutFields(pFiler, objectId);

	pFiler->wrInt32(m_UnknownPathActionParam1);
	pFiler->wrBool(m_UnknownBool);
}

OdResult OdDbImpAssocAsmBodyActionParam::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
	OdResult res = eNotImplemented;

		//res = OdDbImpAssocActionParam::dxfInFields(pFiler, objectId);
		//if (res != eOk)
		//	return res;
	res = m_pSingleDependencyActionParam->dxfInFields(pFiler, objectId);

	if (!pFiler->atSubclassData(OdDbAssocAsmBodyActionParam::desc()->name()))
	{
		ODA_FAIL_ONCE();
		return eMakeMeProxy;
	}

	NEXT_CODE(90)
	m_UnknownPathActionParam1 = pFiler->rdInt32();

	NEXT_CODE(290)
	m_UnknownBool = pFiler->rdBool();

	ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
	return eOk;
}

void OdDbImpAssocAsmBodyActionParam::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
	//OdDbImpAssocActionParam::dxfOutFields(pFiler, objectId);
	m_pSingleDependencyActionParam->dxfOutFields(pFiler, objectId);
	pFiler->wrSubclassMarker(OdDbAssocAsmBodyActionParam::desc()->name());
	pFiler->wrUInt32(90, m_UnknownPathActionParam1);
	pFiler->wrBool(290, m_UnknownBool);
}
