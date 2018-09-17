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
#include "DbAssocDependency.h"
#include "DbImpAssocActionParam.h"
#include "DbFiler.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocActionParam,    // ClassName
                       OdDbObject,              // ParentClass
                       DBOBJECT_CONSTR,         // DOCREATE
                       OdDb::kDHL_1021,         // DwgVer
                       OdDb::kMRelease8,        // MaintVer
                       1025,                    // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocActionParam", // DWG class name
                       L"ACDBASSOCACTIONPARAM", // DxfName
                       L"ObjectDBX Classes",    // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocActionParam::OdDbAssocActionParam(bool createImpObj)
 : OdDbObject()
 , m_pImpObj(NULL)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocActionParam();
}

OdDbAssocActionParam::~OdDbAssocActionParam()
{
  if (m_pImpObj)
    delete m_pImpObj;
}

const OdString& OdDbAssocActionParam::name() const
{
  assertReadEnabled();
  return m_pImpObj->m_sName;
}

OdResult OdDbAssocActionParam::setName(const OdString& newName)
{
  assertWriteEnabled();
  m_pImpObj->m_sName = newName;
  return eOk;
}

OdResult OdDbAssocActionParam::makeParamEmpty(bool alsoEraseOwnedObjects)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbAssocActionParam::detachDependencies()
{
  assertWriteEnabled();
  if ( isKindOf( OdDbAssocEdgeActionParam::desc() ) )
  {
    return static_cast<OdDbImpAssocSingleDependencyActionParam *>
      (m_pImpObj)->detachDependencies( parentAction() );
  }
  
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbAssocActionParam::makeParamConstant()
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbAssocActionParam::transformConstantGeometry(const OdGeMatrix3d& transform)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdDbObjectId OdDbAssocActionParam::parentAction() const
{  
  OdDbAssocActionPtr pAction = ownerId().openObject( OdDb::kForRead );
  if ( pAction.isNull() )
  {
    //TODO action parameters may be owned by other parameters
    ODA_FAIL_ONCE();
    return OdDbObjectId();
  }

  return ownerId();
}

OdResult OdDbAssocActionParam::getDependencies( bool readDependenciesWanted,
                                                bool writeDependenciesWanted,
                                                OdDbObjectIdArray& dependencyIds ) const
{
  assertReadEnabled();
  if ( this->isA()->isEqualTo( OdDbAssocEdgeActionParam::desc() )
    || this->isA()->isEqualTo( OdDbAssocVertexActionParam::desc() )
    || this->isA()->isDerivedFrom( OdDbAssocObjectActionParam::desc() ) )
  {
    OdDbObjectId &depId = static_cast<OdDbImpAssocSingleDependencyActionParam *>
      ( m_pImpObj )->m_DependencyId;

    if ( depId.isNull() )
      return eOk;

    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast( depId.openObject() );
    if ( (readDependenciesWanted  && pDep->isReadDependency())
      || (writeDependenciesWanted && pDep->isWriteDependency()) )
    {
      dependencyIds.append( depId );
    }
    return eOk;
  }

  ODA_FAIL_ONCE();
  return eNotImplementedYet;   
}

OdResult OdDbAssocActionParam::getCompoundObjectIds(bool, // readDependenciesWanted
                                                    bool, // writeDependenciesWanted
                                                    OdArray<OdDbCompoundObjectId>&) const // compoundObjectIds
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdDbAssocStatus OdDbAssocActionParam::status(bool alsoCheckDependencies) const
{
  ODA_ASSERT_ONCE(!alsoCheckDependencies); // TODO

  assertReadEnabled();
  return m_pImpObj->m_status;
}

OdResult OdDbAssocActionParam::setStatus(OdDbAssocStatus newStatus, 
                                         bool notifyParentAction, // = true
                                         bool setInOwnedParams) // = false
{
  assertWriteEnabled();

  if (newStatus == m_pImpObj->m_status)
    return eOk;

  if (isEvaluationRequest(newStatus))
  {
    if (!isToBeSkipped(m_pImpObj->m_status))
    {
      if (isEvaluationRequest(m_pImpObj->m_status))
      {
        if (evaluationRequestSeverityLevel(newStatus) > evaluationRequestSeverityLevel(m_pImpObj->m_status))
          m_pImpObj->m_status = newStatus;
      }
      else
      {
        m_pImpObj->m_status = newStatus;     
        notifyParentAction = true;
      }
    }
  }
  else
    m_pImpObj->m_status = newStatus;

  if (notifyParentAction)
  {
    OdDbObjectId objId = parentAction();
    OdDbAssocActionPtr pAction = objId.openObject(OdDb::kForWrite);
    if (!pAction.isNull())
      pAction->setStatus(newStatus, notifyParentAction, setInOwnedParams);
  }

  return eOk;
}

void OdDbAssocActionParam::auditAssociativeData(OdDbAssocStatus& parentActionHandling)
{
  ODA_FAIL_ONCE(); // TODO
}

//void OdDbAssocActionParam::collectPersSubentNamingDataOverride(OdDbPersStepIdArray& stepIds, 
//                                                               OdDbPersSubentIdArray& persSubentIds) const
//{
//  ODA_FAIL_ONCE(); // TODO
//}
//void OdDbAssocActionParam::clonePersSubentNamingDataOverride(OdDbAssocPersSubentManagerCloner* pCloner)
//{
//  ODA_FAIL_ONCE(); // TODO
//}

OdResult OdDbAssocActionParam::dwgInFields(OdDbDwgFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  return m_pImpObj->dwgInFields(pFiler, objectId());
}

void OdDbAssocActionParam::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  m_pImpObj->dwgOutFields(pFiler, objectId());
}

OdResult OdDbAssocActionParam::dxfInFields(OdDbDxfFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  return m_pImpObj->dxfInFields(pFiler, objectId());
}

void OdDbAssocActionParam::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  m_pImpObj->dxfOutFields(pFiler, objectId());
}

OdResult OdDbAssocActionParam::getObject(OdDbCompoundObjectId* object) const
{
  assertReadEnabled();
  return static_cast <OdDbImpAssocObjectActionParam *>
    (m_pImpObj)->getObject(object);
}

OdResult OdDbAssocActionParam::setObject(const OdDbCompoundObjectId* object,
  bool isReadDependency, // = true
  bool isWriteDependency // = false
  ) // = 0
{
  assertWriteEnabled();
  return static_cast <OdDbImpAssocObjectActionParam *>
    (m_pImpObj)->setObject(ownerId(), object, isReadDependency, isWriteDependency);
}

OdResult OdDbAssocActionParam::setObjectOnly(const OdDbCompoundObjectId* object,
  bool isReadDependency, // = true
  bool isWriteDependency // = false
  ) // = 0
{
  assertWriteEnabled();
  return static_cast <OdDbImpAssocVertexActionParam *>
    (m_pImpObj)->setObjectOnly(ownerId(), object, isReadDependency, isWriteDependency);
}

OdStringArray OdDbAssocActionParam::compareWith(const OdDbObjectId paramIDToCompare, OdDb::DwgVersion filerVersion /*OdDb::kDHL_CURRENT*/) const
{
	OdStringArray retArray;
	OdDbAssocActionParamPtr comparedParamsPtr = OdDbAssocActionParam::cast(paramIDToCompare.openObject());

	if (comparedParamsPtr.isNull())
	{
		retArray.append(CS_OBJECT_ERROR);
		return retArray;
	}

	OdDbImpAssocActionParam* pImpObjCompared = static_cast<OdDbImpAssocActionParam *>(comparedParamsPtr->m_pImpObj);

	return static_cast<OdDbImpAssocActionParam *>
		(m_pImpObj)->compareWith(pImpObjCompared);
}
//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocAsmBodyActionParam,    // ClassName
                       OdDbAssocActionParam,           // ParentClass
                       DBOBJECT_CONSTR,                // DOCREATE
                       OdDb::kDHL_1021,                // DwgVer
                       OdDb::kMRelease8,               // MaintVer
                       1025,                           // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocAsmBodyActionParam", // DWG class name
                       L"ACDBASSOCASMBODYACTIONPARAM", // DxfName
                       L"ObjectDBX Classes",           // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocAsmBodyActionParam::OdDbAssocAsmBodyActionParam(bool createImpObj) // = true
  : OdDbAssocActionParam(false)
{
	if (createImpObj)
		m_pImpObj = new OdDbImpAssocAsmBodyActionParam();
}

OdResult OdDbAssocAsmBodyActionParam::setBody(const OdDbEntity* pAsmEntity, 
                                              bool isReadDependency, // = true
                                              bool isWriteDependency, // = false
                                              int dependencyOrder) // = 0
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
OdResult OdDbAssocAsmBodyActionParam::setBody(OdDbObjectId asmEntityId, 
                                              bool isReadDependency, // = true
                                              bool isWriteDependency, // = false
                                              int dependencyOrder) // = 0
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
OdResult OdDbAssocAsmBodyActionParam::setBody(void* pAsmBody, bool makeCopy)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
void* OdDbAssocAsmBodyActionParam::body(bool makeCopy) const
{
  ODA_FAIL_ONCE(); // TODO
  return NULL;
}
OdResult OdDbAssocAsmBodyActionParam::getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocCompoundActionParam,    // ClassName
                       OdDbAssocActionParam,            // ParentClass
                       DBOBJECT_CONSTR,                 // DOCREATE
                       OdDb::kDHL_1021,                 // DwgVer
                       OdDb::kMRelease8,                // MaintVer
                       1025,                            // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocCompoundActionParam", // DWG class name
                       L"ACDBASSOCCOMPOUNDACTIONPARAM", // DxfName
                       L"ObjectDBX Classes",            // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocCompoundActionParam::OdDbAssocCompoundActionParam(bool createImpObj) // = true
  : OdDbAssocActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocCompoundActionParam();
}

OdResult OdDbAssocCompoundActionParam::removeAllParams(bool alsoEraseThem)
{
  assertWriteEnabled();
  OdDbImpAssocCompoundActionParam* pImpl = (OdDbImpAssocCompoundActionParam*)m_pImpObj;
  if (alsoEraseThem)
  {
    int nSize = pImpl->m_ActionParams.size();
    OdDbObjectId* pId = pImpl->m_ActionParams.asArrayPtr();
    while (nSize--)
    {
      OdDbObjectPtr pObj = (pId++)->openObject(OdDb::kForWrite);
      pObj->erase();
    }
  }
  pImpl->m_ActionParams.resize(0);
  return eOk;
}

int OdDbAssocCompoundActionParam::paramCount() const
{
  assertReadEnabled();
  OdDbImpAssocCompoundActionParam* pImpl = (OdDbImpAssocCompoundActionParam*)m_pImpObj;
  return pImpl->m_ActionParams.size();
}
const OdDbObjectIdArray& OdDbAssocCompoundActionParam::ownedParams() const
{
  assertReadEnabled();
  OdDbImpAssocCompoundActionParam* pImpl = (OdDbImpAssocCompoundActionParam*)m_pImpObj;
  return pImpl->m_ActionParams;
}
OdResult OdDbAssocCompoundActionParam::addParam(const OdDbObjectId& paramId, int& paramIndex)
{
  assertWriteEnabled();
  OdDbImpAssocCompoundActionParam* pImpl = (OdDbImpAssocCompoundActionParam*)m_pImpObj;
  unsigned nIdx;
  if (pImpl->m_ActionParams.find(paramId, nIdx))
  {
    paramIndex = nIdx;
  }
  else
  {
    paramIndex = pImpl->m_ActionParams.append(paramId);
  }
  return eOk;
}

OdResult OdDbAssocCompoundActionParam::removeParam(const OdDbObjectId& paramId, bool alsoEraseIt)
{
  assertWriteEnabled();
  OdDbImpAssocCompoundActionParam* pImpl = (OdDbImpAssocCompoundActionParam*)m_pImpObj;
  unsigned nIdx;

  if (pImpl->m_ActionParams.find(paramId, nIdx))
  {
    if (alsoEraseIt)
    {
      OdDbObjectPtr pObj = paramId.openObject(OdDb::kForWrite);
      pObj->erase();
    }
    pImpl->m_ActionParams.removeAt(nIdx);
    return eOk;
  }
  return eInvalidInput;
}


OdDbObjectIdArray OdDbAssocCompoundActionParam::paramsAtName(const OdString& paramName) const
{
  assertReadEnabled();
  OdDbImpAssocCompoundActionParam* pImpl = (OdDbImpAssocCompoundActionParam*)m_pImpObj;

  OdDbObjectIdArray arrRes;
  int nSize = pImpl->m_ActionParams.size();
  const OdDbObjectId * pId = pImpl->m_ActionParams.asArrayPtr();
  for (; nSize--; pId++)
  {
    OdDbAssocActionParamPtr pParam = (*pId).safeOpenObject();
    if (pParam->name() == paramName)
    {
      arrRes.append(*pId);
    }
  }
  return arrRes;
}
OdDbObjectId OdDbAssocCompoundActionParam::paramAtName(const OdString& paramName, 
                                                       int index) const
{
  assertReadEnabled();
  OdDbImpAssocCompoundActionParam* pImpl = (OdDbImpAssocCompoundActionParam*)m_pImpObj;

  OdDbObjectId idRes;
  int nSize = pImpl->m_ActionParams.size();
  const OdDbObjectId * pId = pImpl->m_ActionParams.asArrayPtr();
  for (; nSize--; pId++)
  {
    OdDbAssocActionParamPtr pParam = (*pId).safeOpenObject();
    if (pParam->name() == paramName)
    {
      if (index-- == 0)
      {
        break;
      }
    }
  }
  return idRes;
}

OdDbObjectId OdDbAssocCompoundActionParam::paramAtIndex(int paramIndex) const
{
  assertReadEnabled();
  OdDbImpAssocCompoundActionParam* pImpl = (OdDbImpAssocCompoundActionParam*)m_pImpObj;
  return pImpl->m_ActionParams[paramIndex];
}

OdResult OdDbAssocCompoundActionParam::addChildParam(const OdDbObjectId actionId, const OdString& paramName, OdRxClass* pParamClass, const OdDbObjectId anchorId, OdDbObjectId& paramId, int& paramIndex)
{
  assertWriteEnabled();
  return static_cast <OdDbImpAssocCompoundActionParam *>
    (m_pImpObj)->addChildParam(actionId, objectId(), paramName, pParamClass, anchorId, paramId, paramIndex);

  return eOk;
}


//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocEdgeActionParam,    // ClassName
                       OdDbAssocActionParam,        // ParentClass
                       DBOBJECT_CONSTR,             // DOCREATE
                       OdDb::kDHL_1021,             // DwgVer
                       OdDb::kMRelease8,            // MaintVer
                       1025,                        // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocEdgeActionParam", // DWG class name
                       L"ACDBASSOCEDGEACTIONPARAM", // DxfName
                       L"ObjectDBX Classes",        // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocEdgeActionParam::OdDbAssocEdgeActionParam(bool createImpObj) // = true
  : OdDbAssocActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocEdgeActionParam();
}

OdResult OdDbAssocEdgeActionParam::setEdgeRef(const OdDbEdgeRef& edgeRef, 
                                              bool isReadDependency, // = true
                                              bool isWriteDependency, // = false
                                              int dependencyOrder) // = 0
{
  return static_cast <OdDbImpAssocEdgeActionParam *>
    (m_pImpObj)->setEdgeRef( edgeRef, isReadDependency, isWriteDependency, dependencyOrder, parentAction() );
}
OdResult OdDbAssocEdgeActionParam::getEdgeRef(OdArray<OdDbEdgeRef>& edgeRefs) const
{
  return static_cast <OdDbImpAssocEdgeActionParam *>
         (m_pImpObj)->getEdgeRef( edgeRefs );
}
OdResult OdDbAssocEdgeActionParam::setEdgeSubentityGeometry(const OdGeCurve3d* pNewEdgeCurve)
{
  return static_cast <OdDbImpAssocEdgeActionParam *>
    (m_pImpObj)->setEdgeSubentityGeometry( pNewEdgeCurve );
}
OdResult OdDbAssocEdgeActionParam::getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const
{
  return static_cast <OdDbImpAssocEdgeActionParam *>
    (m_pImpObj)->getDependentOnCompoundObject( compoundId );
}

//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocFaceActionParam,    // ClassName
                       OdDbAssocActionParam,        // ParentClass
                       DBOBJECT_CONSTR,             // DOCREATE
                       OdDb::kDHL_1021,             // DwgVer
                       OdDb::kMRelease8,            // MaintVer
                       1025,                        // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocFaceActionParam", // DWG class name
                       L"ACDBASSOCFACEACTIONPARAM", // DxfName
                       L"ObjectDBX Classes",        // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocFaceActionParam::OdDbAssocFaceActionParam(bool createImpObj) // = true
  : OdDbAssocActionParam(false)
{
  if (createImpObj)
		m_pImpObj = new OdDbImpAssocFaceActionParam();

}

OdResult OdDbAssocFaceActionParam::setFaceRef(const OdDbFaceRef& faceRef, 
                                              bool isReadDependency, // = true
                                              bool isWriteDependency, // = false
                                              int dependencyOrder) // = 0
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
OdResult OdDbAssocFaceActionParam::getFaceRef(OdArray<OdDbFaceRef>& faceRefs) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
OdResult OdDbAssocFaceActionParam::setFaceSubentityGeometry(const OdGeSurface* pNewFaceSurface)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
OdResult OdDbAssocFaceActionParam::getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocTrimmingBodyActionParam,    // ClassName
	OdDbAssocFaceActionParam,        // ParentClass
	DBOBJECT_CONSTR,             // DOCREATE
	OdDb::kDHL_1021,             // DwgVer
	OdDb::kMRelease125,            // MaintVer
	1025,                        // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
	L"AcDbAssocTrimmingBodyActionParam", // DWG class name
	L"ACDBASSOCTRIMMINGBODYACTIONPARAM", // DxfName
	L"ObjectDBX Classes",        // AppName
	OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocTrimmingBodyActionParam::OdDbAssocTrimmingBodyActionParam(bool createImpObj)
  : OdDbAssocFaceActionParam(false)
{
	if (createImpObj)
		m_pImpObj = new OdDbImpAssocTrimmingBodyActionParam();

}
//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocObjectActionParam,    // ClassName
                       OdDbAssocActionParam,          // ParentClass
                       DBOBJECT_CONSTR,               // DOCREATE
                       OdDb::kDHL_1021,               // DwgVer
                       OdDb::kMRelease8,              // MaintVer
                       1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocObjectActionParam", // DWG class name
                       L"ACDBASSOCOBJECTACTIONPARAM", // DxfName
                       L"ObjectDBX Classes",          // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocObjectActionParam::OdDbAssocObjectActionParam(bool createImpObj) // = true
  : OdDbAssocActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocObjectActionParam();
}

//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocVertexActionParam,    // ClassName
                       OdDbAssocActionParam,          // ParentClass
                       DBOBJECT_CONSTR,               // DOCREATE
                       OdDb::kDHL_1021,               // DwgVer
                       OdDb::kMRelease8,              // MaintVer
                       1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocVertexActionParam", // DWG class name
                       L"ACDBASSOCVERTEXACTIONPARAM", // DxfName
                       L"ObjectDBX Classes",          // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocVertexActionParam::OdDbAssocVertexActionParam(bool createImpObj) // = true
  : OdDbAssocActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocVertexActionParam();
}

OdResult OdDbAssocVertexActionParam::setVertexRef(const OdDbVertexRef& vertexRef,
                                                  bool isReadDependency, // = true
                                                  bool isWriteDependency, // = false
                                                  int dependencyOrder) // = 0
{
  return static_cast <OdDbImpAssocVertexActionParam *>
    (m_pImpObj)->setVertexRef( vertexRef, isReadDependency, isWriteDependency, dependencyOrder, parentAction() );
}
OdResult OdDbAssocVertexActionParam::getVertexRef(OdArray<OdDbVertexRef>& vertexRefs) const
{
  return static_cast <OdDbImpAssocVertexActionParam *>
         (m_pImpObj)->getVertexRef( vertexRefs );
}
OdResult OdDbAssocVertexActionParam::getVertexRef(OdDbVertexRef& vertexRef) const
{
  return static_cast <OdDbImpAssocVertexActionParam *>
    (m_pImpObj)->getVertexRef( vertexRef );
}
OdResult OdDbAssocVertexActionParam::setVertexSubentityGeometry(const OdGePoint3d& newPosition)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
OdResult OdDbAssocVertexActionParam::getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocPointRefActionParam,    // ClassName
  OdDbAssocCompoundActionParam,            // ParentClass
  DBOBJECT_CONSTR,                 // DOCREATE
  OdDb::kDHL_1021,                 // DwgVer
  OdDb::kMRelease8,                // MaintVer
  1025,                            // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"AcDbAssocPointRefActionParam", // DWG class name
  L"ACDBASSOCPointRefActionParam", // DxfName
  L"ObjectDBX Classes",            // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocPointRefActionParam::OdDbAssocPointRefActionParam(bool createImpObj) // = true
  : OdDbAssocCompoundActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocPointRefActionParam();
}


ODRX_DEFINE_MEMBERS_EX(OdDbAssocOsnapPointRefActionParam,    // ClassName
  OdDbAssocPointRefActionParam,            // ParentClass
  DBOBJECT_CONSTR,                 // DOCREATE
  OdDb::kDHL_1027,                 // DwgVer
  OdDb::kMRelease125,                // MaintVer
  1025,                            // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
  L"AcDbAssocOsnapPointRefActionParam", // DWG class name
  L"ACDBASSOCOSNAPPOINTREFACTIONPARAM", // DxfName
  L"ObjectDBX Classes",            // AppName
  OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocOsnapPointRefActionParam::OdDbAssocOsnapPointRefActionParam(bool createImpObj) // = true
  : OdDbAssocPointRefActionParam(false)
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocOsnapPointRefActionParam();
}

OdResult OdDbAssocOsnapPointRefActionParam::dwgInFields(OdDbDwgFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  return m_pImpObj->dwgInFields(pFiler, objectId());
}

void OdDbAssocOsnapPointRefActionParam::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  m_pImpObj->dwgOutFields(pFiler, objectId());
}

OdResult OdDbAssocOsnapPointRefActionParam::dxfInFields(OdDbDxfFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  return m_pImpObj->dxfInFields(pFiler, objectId());
}

void OdDbAssocOsnapPointRefActionParam::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  m_pImpObj->dxfOutFields(pFiler, objectId());
}

void OdDbAssocOsnapPointRefActionParam::setOsnapMode(OdDb::OsnapMode mode)
{
  static_cast<OdDbImpAssocOsnapPointRefActionParam *> (m_pImpObj)->setOsnapMode(mode);
}

void OdDbAssocOsnapPointRefActionParam::setParam(double param)
{
  static_cast<OdDbImpAssocOsnapPointRefActionParam *> (m_pImpObj)->setParam(param);
}

//////////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_MEMBERS_EX(OdDbAssocPathActionParam,    // ClassName
	OdDbAssocCompoundActionParam,            // ParentClass
	DBOBJECT_CONSTR,                 // DOCREATE
	OdDb::kDHL_1021,                 // DwgVer
	OdDb::kMRelease125,                // MaintVer
	1025,                            // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
	L"AcDbAssocPathActionParam", // DWG class name
	L"ACDBASSOCPATHACTIONPARAM", // DxfName
	L"ObjectDBX Classes",            // AppName
	OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocPathActionParam::OdDbAssocPathActionParam(bool createImpObj) // = true
	: OdDbAssocCompoundActionParam(false)
{
	if (createImpObj)
		m_pImpObj = new OdDbImpAssocPathActionParam();
}
