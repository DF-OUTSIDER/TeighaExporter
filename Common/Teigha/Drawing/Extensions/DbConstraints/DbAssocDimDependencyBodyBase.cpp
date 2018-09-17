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
#include "DbAssocDimDependencyBodyBase.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocValueDependency.h"
#include "OdExplicitConstr.h"
#include "DbEvalVariant.h"
#include "DbAssocVariable.h"
#include "DbAssocNetwork.h"
#include "OdDToStr.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbAssocDimDependencyBodyBase, OdDbAssocDependencyBody, L"AcDbImpAssocDimDependencyBodyBase");

bool OdDbAssocDimDependencyBodyBase::sbEraseDimensionIfDependencyIsErased = true;

OdDbAssocDimDependencyBodyBase::OdDbAssocDimDependencyBodyBase(): OdDbAssocDependencyBody(), m_text()
{
}

OdDbAssocDimDependencyBodyBase::~OdDbAssocDimDependencyBodyBase()
{
  
}

void OdDbAssocDimDependencyBodyBase::evaluateOverride()
{
  //evaluateOverride
  ODA_FAIL();
}

OdResult OdDbAssocDimDependencyBodyBase::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbAssocDependencyBody::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  OdInt16 ver = pFiler->rdInt16();
  ODA_ASSERT_ONCE(ver == 1); // TODO
  if (ver != 1)
    return eMakeMeProxy;

  m_text = pFiler->rdString();

  return res;
}

void OdDbAssocDimDependencyBodyBase::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbAssocDependencyBody::dwgOutFields(pFiler);

  pFiler->wrInt16(1); // ver
  pFiler->wrString(m_text);
}

#define NEXT_CODE(code)         \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL_ONCE();              \
  return eMakeMeProxy;          \
}

OdResult OdDbAssocDimDependencyBodyBase::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbAssocDependencyBody::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  if (pFiler->rdUInt32() != 1) // ver
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(1)
  pFiler->rdString(m_text);

  return res;
}

void OdDbAssocDimDependencyBodyBase::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbAssocDependencyBody::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(desc()->name());
  pFiler->wrUInt32(90, 1);
  pFiler->wrString(1, m_text);
}

OdResult OdDbAssocDimDependencyBodyBase::updateDependentOnObjectOverride()
{
  return eOk;
}

OdExplicitConstraint* OdDbAssocDimDependencyBodyBase::constraint() const
{
  assertReadEnabled();

  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningAction().openObject());
  if (pGr.isNull())
    return NULL;

  OdArray<OdGeomConstraint*> arrCnstr;
  pGr->getConstraints(arrCnstr);
  for ( unsigned int i = 0; i < arrCnstr.length(); i++ )
    if ( arrCnstr[i]->isKindOf(OdExplicitConstraint::desc()) )
    {
      OdExplicitConstraint *cnstr = OdExplicitConstraint::cast(arrCnstr[i]);
      if ( cnstr->dimDependencyId() == parentDependency() )
        return cnstr;
    }

    return NULL;
}

OdDbObjectId OdDbAssocDimDependencyBodyBase::variable() const // of AcDbAssocVariable
{
  assertReadEnabled();

  OdExplicitConstraint* cnstr = constraint();
  if ( cnstr == NULL )
    return OdDbObjectId::kNull;
  else
  {
    OdDbObjectId valDependencyId = cnstr->valueDependencyId();
    if ( valDependencyId.isNull() )
      return OdDbObjectId::kNull;

    OdDbAssocValueDependencyPtr pValDep = valDependencyId.openObject();
    return pValDep->dependentOnObject();
  }
}

OdResult OdDbAssocDimDependencyBodyBase::getConstrainedGeoms(OdArray<OdConstrainedGeometry*>& geoms) const
{
  assertReadEnabled();

  OdExplicitConstraint* cnstr = constraint();
  if ( cnstr == NULL )
    return eBadObjType;

  cnstr->getConnectedGeometries(geoms);
  return eOk;
}

OdResult OdDbAssocDimDependencyBodyBase::getConstrainedGeoms(OdArray<OdDbFullSubentPath>& geoms) const
{
  assertReadEnabled();

  OdArray<OdConstrainedGeometry*> arrGeomObj;
  OdResult res = getConstrainedGeoms(arrGeomObj);
  if ( res != eOk )
    return res;

  OdArray<OdDbFullSubentPath> arrGeomPath;
  unsigned int i, j;
  for ( i = 0; i < arrGeomObj.length(); i++ )
  {
    arrGeomObj[i]->getFullSubentPaths(arrGeomPath);
    for ( j = 0; j < arrGeomPath.length(); j++ )
      geoms.push_back(arrGeomPath[j]);

    arrGeomPath.clear();
  }

  return eOk;
}

OdResult OdDbAssocDimDependencyBodyBase::getConstrainedGeoms(OdArray<OdDbSubentGeometry>& geoms,
                                      OdGeVector3d& distanceDirection) const
{
  assertReadEnabled();

  OdExplicitConstraint* cnstr = constraint();
  if ( cnstr == NULL )
    return eBadObjType;

  OdArray<OdConstrainedGeometry*> arrGeomObj;
  OdResult res = cnstr->getConnectedGeometries(arrGeomObj);

  if ( res != eOk )
    return res;

  for ( unsigned int i = 0; i < arrGeomObj.length(); i++ )
  {
    if ( arrGeomObj[i]->isKindOf(OdConstrainedPoint::desc()) )
    {
      OdConstrainedPoint* point = OdConstrainedPoint::cast(arrGeomObj[i]);
      geoms.push_back(OdDbSubentGeometry(point->point()));
    }
    else if ( arrGeomObj[i]->isKindOf(OdConstrainedCurve::desc()) )
    {
      if ( arrGeomObj[i]->isKindOf(OdConstrainedCircle::desc()) )
      {
        OdConstrainedCircle* circle = OdConstrainedCircle::cast(arrGeomObj[i]);
        const OdGeCircArc3d pArcC = circle->getOdGeCircArc3d();
        OdGeCurve3d *pArc = const_cast<OdGeCircArc3d*>(&pArcC);
        geoms.push_back(OdDbSubentGeometry(pArc));
      }
      else if ( arrGeomObj[i]->isKindOf(OdConstrainedEllipse::desc()) )
      {
        OdConstrainedEllipse* ellipse = OdConstrainedEllipse::cast(arrGeomObj[i]);
        const OdGeEllipArc3d pEllArcC = ellipse->getOdGeEllipArc3d();
        OdGeCurve3d *pEllArc = const_cast<OdGeEllipArc3d*>(&pEllArcC);
        geoms.push_back(OdDbSubentGeometry(pEllArc));
      }
      else if ( arrGeomObj[i]->isKindOf(OdConstrainedBoundedLine::desc()) )
      {
        OdConstrainedBoundedLine* line = OdConstrainedBoundedLine::cast(arrGeomObj[i]);
        OdGeCurve3d *pCurve;
        if ( line->isRay() )
        {
          const OdGeRay3d constRay = line->getOdGeRay3d();
          pCurve = const_cast<OdGeRay3d*>(&constRay);
        }
        else
        {
          const OdGeLineSeg3d constSeg = line->getOdGeLineSeg3d();
          pCurve = const_cast<OdGeLineSeg3d*>(&constSeg);
        }

        geoms.push_back(OdDbSubentGeometry(pCurve));
      }
      else if ( arrGeomObj[i]->isKindOf(OdConstrainedLine::desc()) )
      {
        OdConstrainedLine* line = OdConstrainedLine::cast(arrGeomObj[i]);
        const OdGeLine3d constLine = line->getOdGeLine3d();
        OdGeCurve3d *pCurve = const_cast<OdGeLine3d*>(&constLine);
        geoms.push_back(OdDbSubentGeometry(pCurve));
      }
    }
  }

  if ( cnstr->isKindOf(OdDistanceConstraint::desc()) )
  {
    OdDistanceConstraint *pDistCnstr = OdDistanceConstraint::cast(cnstr);
    distanceDirection = pDistCnstr ->direction();
  }
  else
  {
    distanceDirection = OdGeVector3d();
  }
  return eOk;
}

OdResult OdDbAssocDimDependencyBodyBase::getVariableNameAndExpression(OdString& name, OdString& expression, OdString& value) const
{
  assertReadEnabled();

  OdDbObjectId varId = variable();
  if ( varId.isNull() )
    return eBadObjType;

  OdDbAssocVariablePtr pVar = varId.openObject();
  if ( pVar.isNull() )
    return eBadObjType;

  name = pVar->name();
  expression = pVar->expression();
  
  OdDbEvalVariantPtr varVal = pVar->value();
  
  double val = 0.0;
  switch ( varVal->getType() )
  {
    case kDwgReal:
       varVal->getValue(val);
    break;
    case kDwgInt16:
    {
      short valS;
      varVal->getValue(valS);
      val = valS;
    }
    break;
    case kDwgInt32:
    {
      int valI;
      varVal->getValue(valI);
      val = valI;
    }
    break;
  }

  value.format(OD_T("%g"), val);
  return eOk;
}


OdResult OdDbAssocDimDependencyBodyBase::getEntityNameAndExpression(OdString& name, OdString& expression) const
{
  return eNotImplementedYet;
}

OdResult OdDbAssocDimDependencyBodyBase::setVariableNameAndExpression(const OdString& name, const OdString& expression)
{
  assertWriteEnabled();
  OdDbObjectId varId = variable();

  if ( varId.isNull() )
    return eBadObjType;

  //downgradeOpen();

  OdDbAssocVariablePtr pVar = varId.openObject(OdDb::kForWrite);
  pVar->setName(name, true);
  
  return pVar->setExpression(expression, OdString::kEmpty, true,  true);;
}

OdResult OdDbAssocDimDependencyBodyBase::setEntityNameAndExpression(const OdString& name, const OdString& expression, const OdString& value)
{
  assertWriteEnabled();

  m_text.empty();
  if ( expression.isEmpty() )
  {
    m_text += L"(";
    m_text += name;
    m_text += L"=";
    m_text += value;
    m_text += L")";
  }
  else
  {
    m_text += name;
    m_text += L"=";
    m_text += expression;
  }

  setEntityTextOverride(m_text);

  return eOk;
}

OdResult OdDbAssocDimDependencyBodyBase::setNameAndExpression(const OdString& name, const OdString& expression)
{
  assertWriteEnabled();
  OdResult res = setVariableNameAndExpression(name, expression);
  if ( res != eOk )
    return res;

#if 0
  OdDbObjectId networkId = OdDbAssocNetwork::getInstanceFromDatabase(database(), false);
  OdDbAssocNetworkPtr pNetwork = networkId.openObject(OdDb::kForWrite);
  pNetwork->evaluate(NULL);
#endif
  setEntityNameAndExpression(name, expression, OdString::kEmpty);

  return eOk;
}

OdResult OdDbAssocDimDependencyBodyBase::validateEntityText(const OdString& entityTextToValidate, 
                                       OdString&       errorMessage) const
{
  ODA_FAIL();
  return eNotImplementedYet;
}

OdString OdDbAssocDimDependencyBodyBase::composeEntityText(int requiredNameFormat) const
{
  assertReadEnabled();
  OdString name;
  OdString expression;
  OdString value;

  if ( getVariableNameAndExpression(name, expression, value) == eOk )
    return OdString::kEmpty;

  if ( expression.isEmpty() )
    return name + L"=" + value;
  else
    return name + L"=" + expression;
}

OdResult OdDbAssocDimDependencyBodyBase::setVariableValueToMeasuredValue() const
{
  assertReadEnabled();
  double val = getEntityMeasurementOverride();
  OdDbObjectId varId = variable();
  if ( varId.isNull() )
    return eBadObjType;

  OdDbAssocVariablePtr pVar = varId.openObject(OdDb::kForWrite);
  OdDbEvalVariantPtr varVal = pVar->value();
  OdDbEvalVariantPtr  curVal = OdDbEvalVariant::init(val);

  if ( varVal.get() == curVal.get() )
  {
    if ( pVar->expression().isEmpty() )
      return eOk;
  }

  pVar->setExpression(OdString::kEmpty, OdString::kEmpty, true,  true);
  pVar->setValue(*curVal.get());

  return eOk;
}

OdResult OdDbAssocDimDependencyBodyBase::deactivateConstraint() const
{
  assertReadEnabled();
  OdExplicitConstraint* cnstr = constraint();
  if ( cnstr == NULL )
    return eBadObjType;
  
  cnstr->deactivate();
  return eOk;
}

OdResult OdDbAssocDimDependencyBodyBase::reactivateConstraint() const
{
  assertReadEnabled();
  OdExplicitConstraint* cnstr = constraint();
  if ( cnstr == NULL )
    return eBadObjType;
  
  cnstr->reactivate();
  return eOk;
}

bool OdDbAssocDimDependencyBodyBase::isConstraintActive() const // Has DCM d_node or r_node
{
  assertReadEnabled();
  return constraint()->isActive();
}

OdResult OdDbAssocDimDependencyBodyBase::getMeasuredValue(double& measurement) const
{
  assertReadEnabled();
  measurement = getEntityMeasurementOverride();
  return eOk;
}

OdResult OdDbAssocDimDependencyBodyBase::setIsReferenceOnly(bool yesNo)
{
  assertWriteEnabled();

  OdExplicitConstraint* cnstr = constraint();
  if ( cnstr == NULL )
    return eBadObjType;

  OdDbObjectId valDepId = cnstr->valueDependencyId();
  if ( valDepId.isNull() )
    return eBadObjType;

  OdDbAssocDependencyPtr pValDep = valDepId.openObject(OdDb::kForWrite);
  if ( pValDep->isReadDependency() == yesNo && pValDep->isWriteDependency() == !yesNo )
    return eOk;

  pValDep->setIsReadDependency(yesNo);
  pValDep->setIsWriteDependency(!yesNo);

  pValDep->downgradeOpen();
  //downgradeOpen();

  OdDbObjectId varId = variable();
  if ( varId.isNull() )
    return eBadObjType;

  OdDbAssocVariablePtr pVar = varId.openObject(OdDb::kForWrite);
  pVar->setExpression(OdString::kEmpty, OdString::kEmpty, false,  true);

  double val;
  getMeasuredValue(val);
  OdDbEvalVariantPtr pVal = OdDbEvalVariant::init(val);
  pVar->setValue(*pVal.get());

  return eOk;
}

OdResult /*static*/OdDbAssocDimDependencyBodyBase::getNameAndExpressionFromEntityText(const OdString& entityText,
                                                                           bool            useMeasurementIfNoText,
                                                                           double          measurement,
                                                                           bool            isAngular,
                                                                           OdString&       name, 
                                                                           OdString&       expression)
{
  return eNotImplementedYet;
}


int OdDbAssocDimDependencyBodyBase::getCurrentlyUsedEntityNameFormat() const
{
  return -1;
}

void OdDbAssocDimDependencyBodyBase::dragStatus(const OdDb::DragStat status)
{
}

OdResult /*static */OdDbAssocDimDependencyBodyBase::getFromEntity(const OdDbObjectId& entityId, OdDbObjectId& dimDepBodyId)
{
  if ( entityId.isNull() )
  {
    dimDepBodyId = OdDbObjectId::kNull;
    return eBadObjType;
  }

  OdDbObjectPtr pObject = entityId.openObject();

  if ( pObject.isNull() )
  {
    dimDepBodyId = OdDbObjectId::kNull;
    return eBadObjType;
  }

  OdDbObjectIdArray arrPersistentReactors = pObject->getPersistentReactors();
  OdDbObjectPtr pObj;
  OdDbAssocDependencyPtr pDep;
  OdDbObjectId depBodyId;
  for ( unsigned int i = 0; i < arrPersistentReactors.length(); i++ )
  {
    pObj = arrPersistentReactors[i].openObject();
    if ( pObj->isKindOf(OdDbAssocDependency::desc()) )
    {
      pDep = OdDbAssocDependency::cast(pObj);
      OdDbObjectId depBodyId = pDep->dependencyBody();
      if ( !depBodyId.isNull() )
      {
        pObj = depBodyId.openObject();
        if ( pObj->isKindOf(OdDbAssocDimDependencyBodyBase::desc()) )
        {
          dimDepBodyId = depBodyId;
          return eOk;
        }
      }
    }
  }

  dimDepBodyId = OdDbObjectId::kNull;
  return eBadObjType;
}

OdResult OdDbAssocDimDependencyBodyBase::entityAttachmentPointMoved(const OdArray<OdDbSubentGeometry>& newAttachedGeometries,
                                                                           double measurement)
{
  return eNotImplementedYet;
}

OdResult OdDbAssocDimDependencyBodyBase::isRelevantChangeOverride(bool& isRelevChange) const
{
  assertReadEnabled();
  isRelevChange = false;
  return eOk;
}

void OdDbAssocDimDependencyBodyBase::modifiedOverride(const OdDbObject* pDbObj)
{
}

void OdDbAssocDimDependencyBodyBase::erasedOverride(const OdDbObject* pDbObj, bool isErasing)
{
}

OdResult OdDbAssocDimDependencyBodyBase::subErase(bool erasing)
{
  return eNotImplementedYet;
}

OdString /*static */OdDbAssocDimDependencyBodyBase::formatToCurrentPrecision(const OdString& expression, bool isAngular)
{
  return OdString::kEmpty;
}

bool /*static */OdDbAssocDimDependencyBodyBase::setEraseDimensionIfDependencyIsErased(bool yesNo)
{
  sbEraseDimensionIfDependencyIsErased = yesNo;
  return sbEraseDimensionIfDependencyIsErased;
}

bool /*static */OdDbAssocDimDependencyBodyBase::getEraseDimensionIfDependencyIsErased()
{
  return sbEraseDimensionIfDependencyIsErased;
}
