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
#include "DbImpAssocViewRepActionBody.h"
#include "DbFiler.h"
#include "NextCodeDefs.h"
#include "DbAssocDependency.h"
#include "DbAssocActionParam.h"
#include "DbViewport.h"
#include "DbPolyline.h"
#include "Ge/GeLine2d.h"
#include "DbAssocViewRepHatchManager.h"
#include "DbHatch.h"
#include "DbMText.h"
#include "DbField.h"
#include "DbDictionary.h"
#include "DbSectionSymbol.h"
#include "DbDetailSymbol.h"
#include "DbViewRepBlockReference.h"
#include "DbViewBorder.h"
#include "DbViewRep.h"
#include "algorithm"
#include "DbDimAssoc.h"
#include "DbAssocViewLabelActionParam.h"
#include "DbAssocViewSymbolActionParam.h"
#include "DbAssocViewStyleActionParam.h"
#include "RxSketchObject.h"
#include "RxSketchManager.h"


OdDbImpAssocViewRepActionBody::OdDbImpAssocViewRepActionBody()
  : OdDbImpAssocParamBasedActionBody()
//, m_viewRepId
  , m_flags(0)
  , m_Unknown1(0.0)
{
}

OdDbImpAssocViewRepActionBody::~OdDbImpAssocViewRepActionBody()
{
}

OdResult OdDbImpAssocViewRepActionBody::dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId)
{
  // handle : x2B1 of DetailAndSectionViews.dwg

  OdResult res = eOk;
  if (pFiler->dwgVersion() > OdDb::vAC24)
    res = OdDbImpAssocActionBody::dwgInFields(pFiler);
  else
    res = OdDbImpAssocParamBasedActionBody::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  OdUInt16 ver = pFiler->rdInt16();
  ODA_ASSERT_ONCE(ver == 1); // TODO
  if (ver != 1)
    return eMakeMeProxy;

  m_viewRepId = pFiler->rdHardOwnershipId(); // AcDbViewRep x29B
  ODA_ASSERT_ONCE(!m_viewRepId.isNull());
  m_flags = pFiler->rdInt32();
  m_Unknown1 = pFiler->rdDouble();
  ODA_ASSERT_ONCE(OdZero(m_Unknown1) || OdEqual(m_Unknown1, OdaPI2));
  return eOk;
}

void OdDbImpAssocViewRepActionBody::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  if (pFiler->dwgVersion() > OdDb::vAC24)
    OdDbImpAssocActionBody::dwgOutFields(pFiler);
  else
    OdDbImpAssocParamBasedActionBody::dwgOutFields(pFiler);

  OdUInt16 ver = 1;
  pFiler->wrInt16(ver);

  ODA_ASSERT_ONCE(!m_viewRepId.isNull());
  pFiler->wrHardOwnershipId(m_viewRepId); // AcDbViewRep x29B
  pFiler->wrInt32(m_flags);
  ODA_ASSERT_ONCE(OdZero(m_Unknown1) || OdEqual(m_Unknown1, OdaPI2));
  pFiler->wrDouble(m_Unknown1);
}

OdResult OdDbImpAssocViewRepActionBody::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  // handle : x2B1 of DetailAndSectionViews.dxf 

  OdResult res = eOk;
  if (pFiler->dwgVersion() > OdDb::vAC24)
    res = OdDbImpAssocActionBody::dxfInFields(pFiler);
  else
    res = OdDbImpAssocParamBasedActionBody::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(OdDbAssocViewRepActionBody::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(70)
  OdUInt16 ver = pFiler->rdUInt16();
  ODA_ASSERT_ONCE(ver == 1); // TODO
  if (ver != 1)
    return eMakeMeProxy;

  NEXT_CODE(360)
  m_viewRepId = pFiler->rdObjectId(); // AcDbViewRep x29B
  ODA_ASSERT_ONCE(!m_viewRepId.isNull());

  NEXT_CODE(90)
  m_flags = pFiler->rdUInt32();

  NEXT_CODE(40)
  m_Unknown1 = pFiler->rdDouble();
  ODA_ASSERT_ONCE(OdZero(m_Unknown1) || OdEqual(m_Unknown1, OdaPI2));

  ODA_ASSERT_ONCE(pFiler->atEOF()); // TODO
  return eOk;
}

void OdDbImpAssocViewRepActionBody::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  if (pFiler->dwgVersion() > OdDb::vAC24)
    OdDbImpAssocActionBody::dxfOutFields(pFiler);
  else
    OdDbImpAssocParamBasedActionBody::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocViewRepActionBody::desc()->name());

  OdUInt16 ver = 1;
  pFiler->wrUInt16(70, ver);

  ODA_ASSERT_ONCE(!m_viewRepId.isNull());
  pFiler->wrObjectId(360, m_viewRepId); // AcDbViewRep x29B

  pFiler->wrUInt32(90, m_flags);

  ODA_ASSERT_ONCE(OdZero(m_Unknown1) || OdEqual(m_Unknown1, OdaPI2));
  pFiler->wrDouble(40, m_Unknown1);
}

void OdDbImpAssocViewRepActionBody::composeForLoad( OdDbObject *pObj, OdDb::SaveType format, OdDb::DwgVersion version )
{
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
  OdDbViewBorderPtr pViewBorder = pViewRep->viewBorderId( ).safeOpenObject( OdDb::kForWrite );
  //
  m_CenterBeforeTransform = pViewBorder->centerPoint();
  //
  m_scale = pViewBorder->scale();

  //save vector center(current view)-center(base view) and center of hatch extent
  OdDbAssocViewRepActionBody *pbody = OdDbAssocViewRepActionBody::cast( pObj );
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( pbody->ownerId().safeOpenObject( OdDb::kForWrite ) );
  OdDbAssocActionParamPtr pParam =
    OdDbAssocActionParam::cast( pParentAction->paramAtName( OD_T( "ViewRepParamName" ) ).openObject() );

  if ( !pParam.isNull() )
  {
    OdDbObjectIdArray objIdsDepCurrentAction;
    pParam->getDependencies( true, true, objIdsDepCurrentAction );

    OdDbAssocActionPtr pActionBase;
    OdDbAssocDependencyPtr pDepAction = objIdsDepCurrentAction.first().openObject( OdDb::kForRead );
    OdDbObjectPtr pObj = pDepAction->dependentOnObject().openObject( OdDb::kForRead );
    pActionBase = OdDbAssocAction::cast( pObj );

    OdDbAssocViewRepActionBodyPtr pActionBodyBase = pActionBase->actionBody().safeOpenObject( OdDb::kForRead );
    OdDbViewRepPtr pViewRepBase = pActionBodyBase->viewRepId().safeOpenObject();
    OdDbViewBorderPtr pViewBorderBase = pViewRepBase->viewBorderId().safeOpenObject();

    OdGePoint2d centerBaseVB = pViewBorderBase->centerPoint();
    OdGePoint2d centerVB = pViewBorder->centerPoint();

    m_vectorCenterToCenterBeforeTransform = centerBaseVB - centerVB;

    OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager =
      pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();

    OdDbObjectIdArray hatchActionParamIds;
    pViewRepHatchManager->viewRepHatches( hatchActionParamIds );

    OdDbObjectIdArray::iterator it_HatchActionParamId;
    for ( it_HatchActionParamId = hatchActionParamIds.begin();
          it_HatchActionParamId != hatchActionParamIds.end();
          it_HatchActionParamId++ )
    {
      OdDbAssocActionParamPtr pViewRepHatchParam =
        OdDbAssocActionParam::cast( it_HatchActionParamId->openObject( OdDb::kForRead ) );
      ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

      OdDbObjectIdArray hatchDepIds;
      pViewRepHatchParam->getDependencies( true, true, hatchDepIds );
      OdDbAssocDependencyPtr pHatchDep = hatchDepIds.first().openObject( OdDb::kForRead );

      OdDbHatchPtr pHatch;
      pHatch = OdDbHatch::cast( pHatchDep->dependentOnObject().openObject( OdDb::kForWrite ) );

      OdGeExtents3d ext;
      pHatch->getGeomExtents( ext );
      m_HatchExtCenter.insert( std::make_pair( pHatch->objectId(), ext.center() ) );
    }
  }
}

void OdDbImpAssocViewRepActionBody::evaluateOverride( const OdDbObjectId parentActionId )
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( parentActionId.safeOpenObject( OdDb::kForWrite ) );

  OdDbObjectIdArray objIdsDepCurrentAction;
  pParentAction->getDependencies( true, true, objIdsDepCurrentAction );
  ODA_ASSERT_ONCE( objIdsDepCurrentAction.length() >= 2 );

  OdDbAssocActionPtr pActionBase;

  if ( isNeedBeErased() )
  {
    erase( objIdsDepCurrentAction, pParentAction, true, false);
    return;
  }

  OdDbAssocDependencyPtr pDepViewBorder = objIdsDepCurrentAction[0].openObject( OdDb::kForRead );

  OdGePoint3d ptCenter(m_CenterBeforeTransform.x, m_CenterBeforeTransform.y, 0);
  OdDbViewportPtr pViewport = currentViewport(parentActionId).openObject();
  if (pViewport.get())
  {
    ptCenter = pViewport->centerPoint();
    //if (ptCenter != OdGePoint3d(m_CenterBeforeTransform.x, m_CenterBeforeTransform.y, 0))
    //  m_CenterBeforeTransform.x = ptCenter.x, m_CenterBeforeTransform.y = ptCenter.y;
  }

  if ( pDepViewBorder->status() != kErasedAssocStatus )
    UpdateRotation( pParentAction );

  bool bScaleUpdated = false,
       bScaleChangedByBase = false;

  //action dependency
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
  ODA_ASSERT_VAR(OdDbHandle hdlViewBorder = pViewRep->viewBorderId().getHandle();)

  if (   isAligned() 
      || pViewRep->viewType() == OdDbViewRep::kDetail 
      || pViewRep->viewType() == OdDbViewRep::kProjected) // CORE-13440 proceed changes by base ViewBorder 
  {    
    OdDbAssocDependencyPtr pDepAction = objIdsDepCurrentAction[2].openObject( OdDb::kForRead );
    OdDbObjectPtr pObj = pDepAction->dependentOnObject().openObject( OdDb::kForRead );
    pActionBase = OdDbAssocAction::cast( pObj );

    OdDbAssocStatus st = pDepAction->status();
    switch (st)
    {
      case kIsUpToDateAssocStatus:
        ODA_ASSERT_ONCE(!bScaleChangedByBase);
        break;
      case kChangedDirectlyAssocStatus:
      {
        if ( pDepViewBorder->status() == kErasedAssocStatus )
          break;

        bScaleChangedByBase = true;
        if (!pViewRep->isScaleFromParent()) // && bScaleChangedByBase) // CORE-13440
          break;
        if (UpdateGeom(pParentAction, pActionBase, !bScaleChangedByBase))
          bScaleUpdated = true;

        pDepAction->upgradeOpen();
        pDepAction->setStatus( kIsUpToDateAssocStatus, false );
      }
      break;

      case kErasedAssocStatus:
      {
        if ( pViewRep->viewType() == OdDbViewRep::kSection
          || pViewRep->viewType() == OdDbViewRep::kDetail )
        {
          erase( objIdsDepCurrentAction, pParentAction, true, true );
          return;
        }
        else
        {
          pParentAction->removeDependency( pDepAction->objectId(), true );
          pParentAction->removeParam( pParentAction->paramAtName( OD_T( "ViewRepParamName" ) ), true );

          SETBIT( m_flags, kNotAligned, 1 );
          //breakAlignment();
        }
        
      }
      break;
    }
  }

  //viewBorder dep
  OdDbAssocStatus st = pDepViewBorder->status();
  switch (st)
  {
    case kChangedDirectlyAssocStatus:

      bScaleChangedByBase = false;
      //if (bScaleChangedByBase && !pViewRep->isScaleFromParent()) break;
      if (UpdateGeom(pParentAction, pActionBase, !bScaleChangedByBase))
        bScaleUpdated = true;

      pDepViewBorder->upgradeOpen();
      pDepViewBorder->setStatus( kIsUpToDateAssocStatus, false );
      break;

    case kErasedAssocStatus:
      if ( !pActionBase.isNull() )
        removeSymbolInParenView( pActionBase );
      erase( objIdsDepCurrentAction, pParentAction, false, false );
      return;// <------------ ?
      break;
  }

  if (!bScaleChangedByBase && bScaleUpdated && pViewRep->isScaleFromParent())
  {
    pViewRep->upgradeOpen();
    pViewRep->setScaleFromParent(false); // CORE-13440
  }

  OdDbViewportPtr pViewPort = currentViewport( parentActionId ).openObject( OdDb::kForWrite );
  if (pViewPort.get())
  {
    OdGePoint3d pt(m_CenterBeforeTransform.x, m_CenterBeforeTransform.y, 0 ); 
    ODA_ASSERT_ONCE(pt == ptCenter); // test // ? was changed by savePosition in UpdateGeom
    pViewPort->setCenterPoint(pt); // pViewPort->setCenterPoint(ptCenter);
  }

  //ViewSym
  OdDbObjectIdArray paramsViewSym = pParentAction->paramsAtName( "ViewSymbolParamName" );
  OdDbObjectIdArray::iterator itParamViewSym;
  for ( itParamViewSym = paramsViewSym.begin(); itParamViewSym != paramsViewSym.end(); itParamViewSym++ )
  {
    OdDbAssocActionParamPtr pParamViewSym = itParamViewSym->openObject();
    OdDbObjectIdArray depIds;
    pParamViewSym->getDependencies( true, true, depIds );
    ODA_ASSERT_ONCE( depIds.length() == 1 );

    OdDbAssocDependencyPtr pDep = depIds.first().openObject();
    switch ( pDep->status() )
    {
      case kErasedAssocStatus:
      {
        OdDbViewRepPtr pViewRep = m_viewRepId.openObject( OdDb::kForWrite );

        OdRxSketchObject *pSketchObj 
          = pViewRep->getSketchManager()->getSymbolSketchFromSymbol( pDep->dependentOnObject() );

        ODA_ASSERT_ONCE( pSketchObj );

        OdDbObjectId associatedViewRepId = pSketchObj->getViewRepId();
        OdDbViewRepPtr pAssociatedViewRep = associatedViewRepId.openObject();
        ODA_ASSERT_ONCE( !pAssociatedViewRep.isNull() );

        OdDbAssocViewRepActionBodyPtr pAssocViewRepActionBody = 
          OdDbAssocViewRepActionBody::cast( pAssociatedViewRep->ownerId().openObject( OdDb::kForWrite ) );
        ODA_ASSERT_ONCE( !pAssocViewRepActionBody.isNull( ) );

        pAssocViewRepActionBody->setNeedBeErased( true ); 

        pViewRep->getSketchManager()->deleteSketch( pSketchObj );

        pParentAction->removeParam( *itParamViewSym, true );
        pParentAction->removeDependency( pDep->objectId(), true );
      }
      break;

    }

  }

  //hatch
  OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager = pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();
  OdDbObjectIdArray hatchActionParamIds;
  pViewRepHatchManager->viewRepHatches( hatchActionParamIds );

  OdDbObjectIdArray::iterator itHatchActionParamId;
  for ( itHatchActionParamId = hatchActionParamIds.begin();
        itHatchActionParamId != hatchActionParamIds.end();
        itHatchActionParamId++ ) 
  {
    OdDbAssocActionParamPtr pViewRepHatchParam = 
      OdDbAssocActionParam::cast( itHatchActionParamId->openObject( OdDb::kForRead ) );
    ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

    OdDbObjectIdArray hatchDepIds;
    pViewRepHatchParam->getDependencies( true, true, hatchDepIds );
    ODA_ASSERT_ONCE( hatchDepIds.length() == 1 );
    OdDbAssocDependencyPtr pHatchDep = hatchDepIds.first().openObject( OdDb::kForRead );
    
    OdDbHatchPtr pHatch;

    switch ( pHatchDep->status() )
    {
      case kErasedAssocStatus:

        pHatch = OdDbHatch::cast( pHatchDep->dependentOnObject().openObject( OdDb::kForWrite, true ) );
        pHatch->erase( false );

        {
          OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
          pCompoundObjectId->setEmpty();
          pCompoundObjectId->set( pHatch->objectId() );
          pHatchDep->attachToObject( pCompoundObjectId );
        }

        pHatchDep->upgradeOpen();
        pHatchDep->setStatus( kIsUpToDateAssocStatus );
        break;

      case kChangedDirectlyAssocStatus:

        pHatch = OdDbHatch::cast( pHatchDep->dependentOnObject().openObject( OdDb::kForWrite ) );
        {
          OdGeExtents3d ext;
          pHatch->getGeomExtents( ext );
          OdGeMatrix3d mat;
          OdGeVector3d vecTrans;

          std::map<OdDbObjectId, OdGePoint3d>::iterator it = m_HatchExtCenter.find( pHatch->objectId() );
          ODA_ASSERT_ONCE( it != m_HatchExtCenter.end() );
          vecTrans = it->second - ext.center();
          mat.setToTranslation( vecTrans );
          pHatch->transformBy(mat);
        }

        pHatchDep->upgradeOpen();
        pHatchDep->setStatus( kIsUpToDateAssocStatus );
        break;

      default:
        break;
    }
  }

  //ViewLabel
  OdDbAssocActionParamPtr pViewLabelParam = pParentAction->paramAtName( "ViewLabelParamName" ).openObject( OdDb::kForRead );
  if ( !pViewLabelParam.isNull() )
  {
    OdDbObjectIdArray ViewLabelDepIds;
    pViewLabelParam->getDependencies( true, true, ViewLabelDepIds );
    ODA_ASSERT_ONCE( ViewLabelDepIds.length() == 1 );

    OdDbAssocDependencyPtr pLabelDep = ViewLabelDepIds.first().openObject( OdDb::kForRead );
    switch ( pLabelDep->status() )
    {
      case kChangedDirectlyAssocStatus:
        pLabelDep->upgradeOpen();
        pLabelDep->setStatus( kIsUpToDateAssocStatus );
        break;

      case kErasedAssocStatus:
        pParentAction->removeParam( pViewLabelParam->objectId(), true );
        pParentAction->removeDependency( pLabelDep->objectId(), true );
        break;

      default:
        break;
    }
  }

  pParentAction->setStatus( kIsUpToDateAssocStatus );

}

void OdDbImpAssocViewRepActionBody::UpdateRotation( OdDbAssocAction *pParentAction )
{
  OdDbViewBorderPtr pViewBorder = currentViewBorder( pParentAction->objectId() ).openObject();
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
  if ( !OdEqual( pViewBorder->rotationAngle(), pViewRep->rotationAngle() ) )
  {
    breakAlignment();

    OdDbViewportPtr pViewport = currentViewport( pParentAction->objectId( ) ).openObject( OdDb::kForWrite );
    double dRotation = pViewBorder->rotationAngle() - pViewRep->rotationAngle();

    OdGeMatrix3d matBeforeTrans = OdDbPointRef::mswcsToPswcs( pViewport ).invert();
    pViewport->setTwistAngle( fmod( pViewport->twistAngle() + dRotation, Oda2PI ) );
    pViewport->downgradeOpen();
    OdGeMatrix3d matAfterTrans = OdDbPointRef::mswcsToPswcs( pViewport );

    OdGeMatrix3d mat;
    OdGePoint3d center( m_CenterBeforeTransform.x, m_CenterBeforeTransform.y, 0 );
    mat.setToRotation( pViewBorder->rotationAngle() - pViewRep->rotationAngle(), OdGeVector3d( 0, 0, 1 ), center );

    //viewsym
    OdDbObjectIdArray paramsViewSym = pParentAction->paramsAtName( "ViewSymbolParamName" );
    OdDbObjectIdArray::iterator it_ParamViewSym;
    for ( it_ParamViewSym = paramsViewSym.begin(); it_ParamViewSym != paramsViewSym.end(); it_ParamViewSym++ )
    {
      OdDbAssocActionParamPtr pParamViewSym = it_ParamViewSym->openObject();
      OdDbObjectIdArray depIds;
      pParamViewSym->getDependencies( true, true, depIds );
      ODA_ASSERT_ONCE( depIds.length() == 1 );
      OdDbAssocDependencyPtr pDep = depIds.first().openObject();

      OdDbViewSymbolPtr pViewSym = pDep->dependentOnObject().openObject( OdDb::kForWrite );
      OdDbSectionSymbolPtr pSecSym = OdDbSectionSymbol::cast( pViewSym );
     
      if ( !pSecSym.isNull() )
      {
        OdGePoint3d p;
        for ( int i = 0; i < pSecSym->sectionPointsCount(); i++ )
        {
          pSecSym->getSectionPointAt( i, p );
          p.transformBy( mat );
          pSecSym->setSectionPointAt( i, p );
        }
      }
    }

    //hatch
    OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager =
      pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();

    OdDbObjectIdArray hatchActionParamIds;
    pViewRepHatchManager->viewRepHatches( hatchActionParamIds );

    OdDbObjectIdArray::iterator it_HatchActionParamId;
    for ( it_HatchActionParamId = hatchActionParamIds.begin();
          it_HatchActionParamId != hatchActionParamIds.end();
          it_HatchActionParamId++ )
    {
      OdDbAssocActionParamPtr pViewRepHatchParam =
        OdDbAssocActionParam::cast( it_HatchActionParamId->openObject( OdDb::kForRead ) );
      ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

      OdDbObjectIdArray hatchDepIds;
      pViewRepHatchParam->getDependencies( true, true, hatchDepIds );
      OdDbAssocDependencyPtr pHatchDep = hatchDepIds.first().openObject( OdDb::kForRead );

      OdDbHatchPtr pHatch;
      pHatch = OdDbHatch::cast( pHatchDep->dependentOnObject().openObject( OdDb::kForWrite ) );

      pHatch->transformBy( matAfterTrans*matBeforeTrans );
    }


    pViewRep->upgradeOpen();
    pViewRep->setRotationAngle( pViewBorder->rotationAngle() );
  }
  
}

// return isScaleUpdated
bool OdDbImpAssocViewRepActionBody::UpdateGeom(OdDbAssocAction* pParentAction, OdDbAssocAction* pActionBase,
                                               bool DirectlyChange)
{
  ODA_ASSERT_VAR(OdDbHandle hdlAction = pParentAction->objectId().getHandle();)
  ODA_ASSERT_ONCE(pActionBase || DirectlyChange); // test
  ODA_ASSERT_VAR(OdDbHandle hdlActionBase;) 
  ODA_ASSERT_VAR(if (pActionBase) hdlActionBase = pActionBase->objectId().getHandle();)

  bool isScaleUpdated = false;
  if (isAligned() || !DirectlyChange)
    CalculateBorderPos(pActionBase, DirectlyChange);
  if (UpdateScale(pParentAction, pActionBase))
    isScaleUpdated = true;
  calculateHatchAndSymbolTransMatrix();
  UpdateLabel(pParentAction);
  UpdateSymbol(pParentAction);
  UpdateHatch(pParentAction);
  savePosition(pParentAction->objectId());
  return isScaleUpdated;
}

void OdDbImpAssocViewRepActionBody::CalculateBorderPos( OdDbAssocAction *pBaseAction, bool DirectlyChange )
{
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
  OdDbViewBorderPtr pViewBorder = pViewRep->viewBorderId().safeOpenObject( OdDb::kForWrite );

  OdDbAssocViewRepActionBodyPtr pActionBodyBase = pBaseAction->actionBody().openObject( OdDb::kForRead );
  OdDbViewRepPtr pViewRepBase = pActionBodyBase->viewRepId().safeOpenObject();
  OdDbViewBorderPtr pViewBorderBase = pViewRepBase->viewBorderId().safeOpenObject();

  OdGePoint2d CenterBaseVB = pViewBorderBase->centerPoint();
  OdGePoint2d CenterVB = pViewBorder->centerPoint();
  OdGeMatrix3d mat;

  switch ( pViewRep->viewType() )
  {
  case OdDbViewRep::kProjected:
    if (isAligned()) // CORE-13440 problem with update byBase 
    {
      if (isHorizontalAligned())
      {
        double y = CenterBaseVB.y - CenterVB.y;
        mat.setTranslation(OdGeVector3d(0, y, 0));
      }
      else
      {
        double x = CenterBaseVB.x - CenterVB.x;
        mat.setTranslation(OdGeVector3d(x, 0, 0));
      }
      break;
    }
    //break;
  case OdDbViewRep::kSection:
      if ( DirectlyChange )
      {
        OdGeLine2d line( m_CenterBeforeTransform, CenterBaseVB );

        OdGePoint2d point = CenterVB;
        point.mirror( line );
        line.set( CenterVB, point );

        OdGeVector3d vPosToPosAtLine;
        point = line.evalPoint( 0.5 );
        vPosToPosAtLine.x = point.x - CenterVB.x;
        vPosToPosAtLine.y = point.y - CenterVB.y;
        mat.setToTranslation( vPosToPosAtLine );
      }
      else
      {

        OdGeVector3d vectorCenterToCenterAfterTransform( CenterBaseVB.x - CenterVB.x,
                                                         CenterBaseVB.y - CenterVB.y,
                                                         0 );
        OdGeVector3d vectorCenterToCenterBeforeTransform( m_vectorCenterToCenterBeforeTransform.x,
                                                          m_vectorCenterToCenterBeforeTransform.y,
                                                          0 );

        mat.setToTranslation( vectorCenterToCenterAfterTransform - vectorCenterToCenterBeforeTransform );
      }
      break;

    case OdDbViewRep::kDetail:

      break;

    default:
      ODA_ASSERT_ONCE( !"CalculateBorderPos for this pViewRep->viewType() not implemented" );
  }
  pViewBorder->subTransformBy( mat );

  //scale
  if ( DirectlyChange )
  {
    // it is not invoked with isAligned() = false.
    // Therefore UpdateGeom returns bool to turn of next flag.
    if ( !OdEqual( pViewBorder->scale(), m_scale ) )
    {
      pViewRep->upgradeOpen();
      pViewRep->setScaleFromParent( false );
    }
  }
  else
  {
    if ( pViewRep->isScaleFromParent() )
    {
      pViewBorder->setScaleId( pViewBorderBase->getScaleId() );
      
      if ( !OdEqual( pViewBorderBase->scale(), pViewBorder->scale() ) )
      {
        OdGeMatrix3d mat;
        mat.setToScaling( pViewBorderBase->scale() / pViewBorder->scale(), pViewBorder->centerPoint3d() );
        pViewBorder->transformBy( mat );
      }
    }
  }

}

void OdDbImpAssocViewRepActionBody::savePosition( OdDbObjectId parenActId )
{
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
  OdDbViewBorderPtr pViewBorder = pViewRep->viewBorderId().safeOpenObject( OdDb::kForWrite );
  m_CenterBeforeTransform = pViewBorder->centerPoint();

  if ( isAligned() )
  {
    OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( parenActId.safeOpenObject( OdDb::kForWrite ) );

    OdDbObjectIdArray objIdsDepCurrentAction;
    pParentAction->getDependencies( true, true, objIdsDepCurrentAction );
    ODA_ASSERT_ONCE( objIdsDepCurrentAction.length() >= 2 );

    OdDbAssocActionPtr pActionBase;

    OdDbAssocDependencyPtr pDepAction = objIdsDepCurrentAction[2].openObject( OdDb::kForRead );
    OdDbObjectPtr pObj = pDepAction->dependentOnObject().openObject( OdDb::kForRead );
    pActionBase = OdDbAssocAction::cast( pObj );

    OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
    OdDbViewBorderPtr pViewBorder = pViewRep->viewBorderId().safeOpenObject( OdDb::kForWrite );

    OdDbAssocViewRepActionBodyPtr pActionBodyBase = pActionBase->actionBody().openObject( OdDb::kForRead );
    OdDbViewRepPtr pViewRepBase = pActionBodyBase->viewRepId().safeOpenObject();
    OdDbViewBorderPtr pViewBorderBase = pViewRepBase->viewBorderId().safeOpenObject();

    OdGePoint2d baseCenter = pViewBorderBase->centerPoint();
    OdGePoint2d Center = pViewBorder->centerPoint();

    m_vectorCenterToCenterBeforeTransform = baseCenter - Center;

    OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager =
      pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();

    OdDbObjectIdArray hatchActionParamIds;
    pViewRepHatchManager->viewRepHatches( hatchActionParamIds );

    m_HatchExtCenter.clear();

    OdDbObjectIdArray::iterator it_HatchActionParamId;
    for ( it_HatchActionParamId = hatchActionParamIds.begin();
          it_HatchActionParamId != hatchActionParamIds.end();
          it_HatchActionParamId++ )
    {
      OdDbAssocActionParamPtr pViewRepHatchParam =
        OdDbAssocActionParam::cast( it_HatchActionParamId->openObject( OdDb::kForRead ) );
      ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

      OdDbObjectIdArray hatchDepIds;
      pViewRepHatchParam->getDependencies( true, true, hatchDepIds );
      OdDbAssocDependencyPtr pHatchDep = hatchDepIds.first().openObject( OdDb::kForRead );

      OdDbHatchPtr pHatch;
      pHatch = OdDbHatch::cast( pHatchDep->dependentOnObject().openObject( OdDb::kForWrite ) );

      OdGeExtents3d ext;
      pHatch->getGeomExtents( ext );
      m_HatchExtCenter.insert( std::make_pair( pHatch->objectId(), ext.center() ) );
    }
  }
}

void OdDbImpAssocViewRepActionBody::calculateHatchAndSymbolTransMatrix()
{
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
  OdDbViewBorderPtr pViewBorder = pViewRep->viewBorderId().safeOpenObject( OdDb::kForWrite );
  OdGeVector3d vTranslation;
  vTranslation.x = pViewBorder->centerPoint().x - m_CenterBeforeTransform.x;
  vTranslation.y = pViewBorder->centerPoint().y - m_CenterBeforeTransform.y;
  vTranslation.z = 0;
  m_ViewBorderTransformMatrix.setToTranslation( vTranslation );
}

void OdDbImpAssocViewRepActionBody::UpdateLabel( OdDbAssocActionPtr pParentAction )
{
  OdDbAssocActionParamPtr pViewLabelParam = pParentAction->paramAtName( "ViewLabelParamName" ).openObject( OdDb::kForRead );
  if ( !pViewLabelParam.isNull() )
  {
    OdDbObjectIdArray ViewLabelDepIds;
    pViewLabelParam->getDependencies( true, true, ViewLabelDepIds );
    ODA_ASSERT_ONCE( ViewLabelDepIds.length() == 1 );
    OdDbAssocDependencyPtr pLabelDep = ViewLabelDepIds.first().openObject( OdDb::kForRead );
    OdDbEntityPtr pLabel = pLabelDep->dependentOnObject().openObject( OdDb::kForWrite );

    pLabel->transformBy( m_ViewBorderTransformMatrix );
  }
}

void OdDbImpAssocViewRepActionBody::UpdateSymbol( OdDbAssocActionPtr pParentAction )
{
  OdDbObjectIdArray paramsViewSym = pParentAction->paramsAtName( "ViewSymbolParamName" );
  OdDbObjectIdArray::iterator it_ParamViewSym;
  for ( it_ParamViewSym = paramsViewSym.begin(); it_ParamViewSym != paramsViewSym.end(); it_ParamViewSym++ )
  {
    OdDbAssocActionParamPtr pParamViewSym = it_ParamViewSym->openObject();
    OdDbObjectIdArray depIds;
    pParamViewSym->getDependencies( true, true, depIds );
    ODA_ASSERT_ONCE( depIds.length() == 1 );
    OdDbAssocDependencyPtr pDep = depIds.first().openObject();

    OdDbViewSymbolPtr pViewSym = pDep->dependentOnObject().openObject( OdDb::kForWrite );
    OdDbSectionSymbolPtr pSecSym = OdDbSectionSymbol::cast( pViewSym );
    OdDbDetailSymbolPtr pDetSym = OdDbDetailSymbol::cast( pViewSym );

    if ( !pSecSym.isNull() )
    {
      OdGePoint3d p;
      for ( int i = 0; i < pSecSym->sectionPointsCount(); i++ )
      {
        pSecSym->getSectionPointAt( i, p );
        p.transformBy( m_ViewBorderTransformMatrix );
        pSecSym->setSectionPointAt( i, p );
      }
    }

    if ( !pDetSym.isNull() )
    {
      OdGePoint3d p = pDetSym->origin();
      pDetSym->setOrigin( p.transformBy( m_ViewBorderTransformMatrix ) );
    }
  }
}

void OdDbImpAssocViewRepActionBody::UpdateHatch( OdDbAssocActionPtr pParentAction )
{
  OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager =
    pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();

  OdDbObjectIdArray hatchActionParamIds;
  pViewRepHatchManager->viewRepHatches( hatchActionParamIds );

  OdDbObjectIdArray::iterator it_HatchActionParamId;
  for ( it_HatchActionParamId = hatchActionParamIds.begin();
        it_HatchActionParamId != hatchActionParamIds.end();
        it_HatchActionParamId++ )
  {
    OdDbAssocActionParamPtr pViewRepHatchParam =
      OdDbAssocActionParam::cast( it_HatchActionParamId->openObject( OdDb::kForRead ) );
    ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

    OdDbObjectIdArray hatchDepIds;
    pViewRepHatchParam->getDependencies( true, true, hatchDepIds );
    OdDbAssocDependencyPtr pHatchDep = hatchDepIds.first().openObject( OdDb::kForRead );

    OdDbHatchPtr pHatch;
    pHatch = OdDbHatch::cast( pHatchDep->dependentOnObject().openObject( OdDb::kForWrite ) );

    pHatch->transformBy( m_ViewBorderTransformMatrix );
  }
}

void OdDbImpAssocViewRepActionBody::erase( OdDbObjectIdArray &objIdsDepCurrentAction, OdDbAssocActionPtr pParentAction, bool borderErase, bool mainIsErased )
{
  OdDbAssocActionPtr pActionBase;

  if ( isAligned() && !mainIsErased )
  {
    OdDbAssocDependencyPtr pDepAction = objIdsDepCurrentAction[2].openObject( OdDb::kForRead );
    OdDbObjectPtr pObj = pDepAction->dependentOnObject().openObject( OdDb::kForWrite );
    pActionBase = OdDbAssocAction::cast( pObj );
    pActionBase->removePersistentReactor( pDepAction->objectId() );
  }

  //ViewPort
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
  OdDbViewportPtr pViewPort = pViewRep->viewportId().openObject( OdDb::kForWrite );
  pViewPort->erase( true );

  //BlkRef
  OdDbAssocDependencyPtr pDepViewRepBlkRef = objIdsDepCurrentAction[1].openObject( OdDb::kForRead );

  OdDbViewRepBlockReferencePtr pViewRepBlkRef =
    OdDbViewRepBlockReference::cast( pDepViewRepBlkRef->dependentOnObject().openObject( OdDb::kForWrite ) );
  OdDbViewportPtr pViewPortBlkRef = pViewRepBlkRef->ownerViewportId().openObject( OdDb::kForWrite );
  pViewPortBlkRef->erase( true );
  pViewRepBlkRef->erase( true );

  //border
  if ( borderErase )
  {
    OdDbViewBorderPtr pViewBorder = pViewRep->viewBorderId().openObject( OdDb::kForWrite );
    pViewBorder->erase( true );
  }

  //ViewSym
  OdDbObjectIdArray paramsViewSym = pParentAction->paramsAtName( "ViewSymbolParamName" );
  OdDbObjectIdArray::iterator it_ParamViewSym;
  for ( it_ParamViewSym = paramsViewSym.begin(); it_ParamViewSym != paramsViewSym.end(); it_ParamViewSym++ )
  {
    OdDbAssocActionParamPtr pParamViewSym = it_ParamViewSym->openObject();
    OdDbObjectIdArray depIds;
    pParamViewSym->getDependencies( true, true, depIds );
    ODA_ASSERT_ONCE( depIds.length() == 1 );
    OdDbAssocDependencyPtr pDep = depIds.first().openObject();

    OdDbObjectPtr pObj = pDep->dependentOnObject().openObject( OdDb::kForWrite );
    pObj->erase();
  }

  //Label
  OdDbAssocActionParamPtr pViewLabelParam = pParentAction->paramAtName( "ViewLabelParamName" ).openObject( OdDb::kForRead );
  if ( !pViewLabelParam.isNull() )
  {
    OdDbObjectIdArray ViewLabelDepIds;
    pViewLabelParam->getDependencies( true, true, ViewLabelDepIds );
    ODA_ASSERT_ONCE( ViewLabelDepIds.length() == 1 );
    OdDbAssocDependencyPtr pLabelDep = ViewLabelDepIds.first().openObject( OdDb::kForRead );
    OdDbObjectPtr pLabel = pLabelDep->dependentOnObject().openObject( OdDb::kForWrite );
    pLabel->erase();
  }

  //hatch
  OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager = pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();
  if ( !pViewRepHatchManager.isNull() )
  {
    OdDbObjectIdArray hatchActionParamIds;
    pViewRepHatchManager->viewRepHatches( hatchActionParamIds );

    OdDbObjectIdArray::iterator it_HatchActionParamId;
    for ( it_HatchActionParamId = hatchActionParamIds.begin();
          it_HatchActionParamId != hatchActionParamIds.end();
          it_HatchActionParamId++ )
    {
      OdDbAssocActionParamPtr pViewRepHatchParam =
        OdDbAssocActionParam::cast( it_HatchActionParamId->openObject( OdDb::kForRead ) );
      ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

      OdDbObjectIdArray hatchDepIds;
      pViewRepHatchParam->getDependencies( true, true, hatchDepIds );
      ODA_ASSERT_ONCE( hatchDepIds.length() == 1 );
      OdDbAssocDependencyPtr pHatchDep = hatchDepIds.first().openObject( OdDb::kForRead );

      OdDbHatchPtr pHatch = OdDbHatch::cast( pHatchDep->dependentOnObject().openObject( OdDb::kForWrite, true ) );;
      pHatch->erase();
    }
  }

  pParentAction->setStatus( kErasedAssocStatus, false );
}

void OdDbImpAssocViewRepActionBody::removeSymbolInParenView( OdDbAssocAction *pBaseAction )
{
  OdDbAssocViewRepActionBodyPtr pActionBodyBase = pBaseAction->actionBody().openObject( OdDb::kForWrite );
  OdDbViewRepPtr pViewRepBase = pActionBodyBase->viewRepId().safeOpenObject( OdDb::kForWrite );

  OdRxSketchObject *pSketchObj
    = pViewRepBase->getSketchManager()->getSymbolSketchFromViewRep( m_viewRepId );

  if ( pSketchObj )  
    pActionBodyBase->removeViewSymbol( pSketchObj->getSecSymbolId() );
}

bool OdDbImpAssocViewRepActionBody::isAligned() const
{
  return !GETBIT( m_flags, kNotAligned )
      && !GETBIT( m_flags, kBreakAlignment );
}

bool OdDbImpAssocViewRepActionBody::isHorizontalAligned() const
{
  return OdEqual( m_Unknown1, 0 );
}

bool OdDbImpAssocViewRepActionBody::breakAlignment()
{
  return SETBIT( m_flags, kBreakAlignment, 1 )
      && !GETBIT( m_flags, kNotAligned );
}

void OdDbImpAssocViewRepActionBody::setNeedBeErased( bool erased )
{
  SETBIT( m_flags, kErased, erased );
}

bool OdDbImpAssocViewRepActionBody::isNeedBeErased() const
{
  return GETBIT( m_flags, kErased );
}

OdDbObjectId OdDbImpAssocViewRepActionBody::hatchManager(const OdDbObjectId parentActionId) const
{
  OdDbAssocActionPtr pParentAction = parentActionId.openObject();
  return pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) );
}

bool OdDbImpAssocViewRepActionBody::UpdateScale( OdDbAssocActionPtr pParentAction, OdDbAssocActionPtr pBaseAction )
{
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject();
  OdDbViewBorderPtr pViewBorder = pViewRep->viewBorderId().safeOpenObject( OdDb::kForWrite );

  if ( pViewBorder->scale() == m_scale )
    return false;

  OdGeMatrix3d mat;
  mat.setToScaling( pViewBorder->scale() / m_scale, pViewBorder->centerPoint3d() );

  m_scale = pViewBorder->scale();

  OdDbViewportPtr pViewport = pViewBorder->viewportId().openObject( OdDb::kForWrite );


  OdGeMatrix3d matBeforeTrans = OdDbPointRef::mswcsToPswcs( pViewport ).invert();
  double scl = pViewport->customScale();
  pViewport->setCustomScale( m_scale );
  pViewport->downgradeOpen();
  OdGeMatrix3d matAfterTrans = OdDbPointRef::mswcsToPswcs( pViewport );
  pViewport->upgradeOpen( );
  pViewport->setCustomScale( scl );

  pViewport->transformBy( mat );


  //update MText

  //   OdDbAssocViewRepActionBodyPtr pActionBodyBase = pBaseAction->actionBody().openObject( OdDb::kForRead );
  //   OdDbViewRepPtr pViewRepBase = pActionBodyBase->viewRepId().safeOpenObject( OdDb::kForWrite );
  // 
  //   OdDbObjectId viewSymId = pViewRepBase->getViewSymByViewRep( m_viewRepId );
  //   if ( !viewSymId.isNull() )
  //   {
  //     //SecSym dep and param erase
  //     OdDbViewSymbolPtr pViewSym = viewSymId.openObject();
  //     OdString scaleIdentifier;
  //     pViewSym->getIdentifier( scaleIdentifier );
  // 
  // 
  //   }
  OdDbAssocActionParamPtr pViewLabelParam = pParentAction->paramAtName( "ViewLabelParamName" ).openObject( OdDb::kForRead );
  if ( !pViewLabelParam.isNull() )
  {
    OdDbObjectIdArray ViewLabelDepIds;
    pViewLabelParam->getDependencies( true, true, ViewLabelDepIds );
    ODA_ASSERT_ONCE( ViewLabelDepIds.length() == 1 );
    OdDbAssocDependencyPtr pLabelDep = ViewLabelDepIds.first().openObject( OdDb::kForRead );
    OdDbMTextPtr pLabel = pLabelDep->dependentOnObject().openObject( OdDb::kForWrite );
    if ( !pLabel.isNull() )
    {
      OdGePoint3d location = pLabel->location();
      location.transformBy( matAfterTrans*matBeforeTrans );
      pLabel->setLocation( location );
//       OdDbDictionaryPtr pDic = pLabel->getFieldDictionary(  OdDb::kForRead );
//       OdDbFieldPtr pTEXT = pDic->getAt( OD_T( "TEXT" ), OdDb::kForRead );
//       OdDbFieldPtr pTextScale = pTEXT->getChild( 2, OdDb::kForWrite );
//       if ( !pTextScale.isNull() )
//       {
//         
//         OdString str;
//         str.format( OD_T("1 : %d"), 1.0 / m_scale );
//         OdFieldValue v(str);
//         pTextScale->setData( OD_T( "StandartScaleViewLabel" ), v );
//         pLabel->setField( )
//       }
    }


  }

  //hatch
  OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager =
    pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();

  OdDbObjectIdArray hatchActionParamIds;
  pViewRepHatchManager->viewRepHatches( hatchActionParamIds );

  OdDbObjectIdArray::iterator it_HatchActionParamId;
  for ( it_HatchActionParamId = hatchActionParamIds.begin();
        it_HatchActionParamId != hatchActionParamIds.end();
        it_HatchActionParamId++ )
  {
    OdDbAssocActionParamPtr pViewRepHatchParam =
      OdDbAssocActionParam::cast( it_HatchActionParamId->openObject( OdDb::kForRead ) );
    ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

    OdDbObjectIdArray hatchDepIds;
    pViewRepHatchParam->getDependencies( true, true, hatchDepIds );
    OdDbAssocDependencyPtr pHatchDep = hatchDepIds.first().openObject( OdDb::kForRead );

    OdDbHatchPtr pHatch;
    pHatch = OdDbHatch::cast( pHatchDep->dependentOnObject().openObject( OdDb::kForWrite ) );

    pHatch->transformBy( matAfterTrans*matBeforeTrans );
  }

  OdDbObjectIdArray paramsViewSym = pParentAction->paramsAtName( "ViewSymbolParamName" );
  OdDbObjectIdArray::iterator it_ParamViewSym;
  for ( it_ParamViewSym = paramsViewSym.begin(); it_ParamViewSym != paramsViewSym.end(); it_ParamViewSym++ )
  {
    OdDbAssocActionParamPtr pParamViewSym = it_ParamViewSym->openObject();
    OdDbObjectIdArray depIds;
    pParamViewSym->getDependencies( true, true, depIds );
    ODA_ASSERT_ONCE( depIds.length() == 1 );
    OdDbAssocDependencyPtr pDep = depIds.first().openObject();

    OdDbViewSymbolPtr pViewSym = pDep->dependentOnObject().openObject( OdDb::kForWrite );
    OdDbSectionSymbolPtr pSecSym = OdDbSectionSymbol::cast( pViewSym );
    OdDbDetailSymbolPtr pDetSym = OdDbDetailSymbol::cast( pViewSym );

    if ( !pSecSym.isNull() )
    {
      OdGePoint3d p;
      for ( int i = 0; i < pSecSym->sectionPointsCount(); i++ )
      {
        pSecSym->getSectionPointAt( i, p );
        p.transformBy( matAfterTrans*matBeforeTrans );
        pSecSym->setSectionPointAt( i, p );
      }
    }

    if ( !pDetSym.isNull() )
    {
      OdGePoint3d p = pDetSym->origin();
      pDetSym->transformBy( matAfterTrans*matBeforeTrans );
    }
  }
  return true;
}

OdDbObjectId OdDbImpAssocViewRepActionBody::currentViewBorder(OdDbObjectId idParentAction) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocActionParamPtr pViewBorderParam =
    pParentAction->paramAtName( "CurrentViewBorderParamName" ).openObject();
  if ( !pViewBorderParam.isNull() )
  {
    OdDbObjectIdArray ViewBorderDepIds;
    pViewBorderParam->getDependencies( true, true, ViewBorderDepIds );
    ODA_ASSERT_ONCE( ViewBorderDepIds.length() == 1 );
    OdDbAssocDependencyPtr pDep = ViewBorderDepIds.first().openObject();

    return pDep->dependentOnObject();
  }
  return OdDbObjectId();
}

OdDbObjectId OdDbImpAssocViewRepActionBody::currentViewport( OdDbObjectId idParentAction ) const
{
  OdDbViewBorderPtr pViewBorder = currentViewBorder( idParentAction ).openObject();
  if (pViewBorder.get())
    return pViewBorder->viewportId();
  return OdDbObjectId();
}

OdResult OdDbImpAssocViewRepActionBody::getShowHatch( bool &show, OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager =
    pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();
  pViewRepHatchManager->getShowHatch( show );
  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::setShowHatch( bool bShow, bool Unknown, OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager =
    pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject( OdDb::kForWrite );
  pViewRepHatchManager->setShowHatch( bShow, Unknown );
  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::getShowViewLabel( bool &bShow, OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocActionParamPtr pViewLabelParam =
    pParentAction->paramAtName( "ViewLabelParamName" ).openObject();
  if ( !pViewLabelParam.isNull() )
  {
    OdDbObjectIdArray ViewLabelDepIds;
    pViewLabelParam->getDependencies( true, true, ViewLabelDepIds );
    ODA_ASSERT_ONCE( ViewLabelDepIds.length() == 1 );

    OdDbAssocDependencyPtr pLabelDep = ViewLabelDepIds.first().openObject();
    OdDbEntityPtr pEnt = pLabelDep->dependentOnObject().openObject();
    bShow = !pEnt->visibility();
    return eOk;
  }
  return eInvalidInput;
}

OdResult OdDbImpAssocViewRepActionBody::setShowViewLabel( bool bShow, bool bUnknown, OdDbObjectId idParentAction )
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocActionParamPtr pViewLabelParam =
    pParentAction->paramAtName( "ViewLabelParamName" ).openObject();
  if ( !pViewLabelParam.isNull() )
  {
    OdDbObjectIdArray ViewLabelDepIds;
    pViewLabelParam->getDependencies( true, true, ViewLabelDepIds );
    ODA_ASSERT_ONCE( ViewLabelDepIds.length() == 1 );

    OdDbAssocDependencyPtr pLabelDep = ViewLabelDepIds.first().openObject();
    OdDbEntityPtr pEnt = pLabelDep->dependentOnObject( ).openObject( OdDb::kForWrite );
    if ( pEnt.isNull() )
      return eNullObjectPointer;

    return pEnt->setVisibility( bShow ? OdDb::kVisible : OdDb::kInvisible );
  }
  return eNullObjectPointer;
}

OdDbObjectId OdDbImpAssocViewRepActionBody::viewLabel( OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocActionParamPtr pViewLabelParam =
    pParentAction->paramAtName( "ViewLabelParamName" ).openObject();
  if ( !pViewLabelParam.isNull() )
  {
    OdDbObjectIdArray ViewLabelDepIds;
    pViewLabelParam->getDependencies( true, true, ViewLabelDepIds );
    ODA_ASSERT_ONCE( ViewLabelDepIds.length() == 1 );

    OdDbAssocDependencyPtr pLabelDep = ViewLabelDepIds.first().openObject();
    return pLabelDep->dependentOnObject();
  }
  return OdDbObjectId();
}

OdResult OdDbImpAssocViewRepActionBody::setViewLabel( const OdDbObjectId& idViewLabel, OdDbObjectId idParentAction )
{
  OdResult res = eOk;
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject( OdDb::kForWrite ) );
  int paramIndex = 0;
  OdDbObjectId paramId;
  OdRxClass* pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T( "AcDbAssocViewLabelActionParam" ) ).get();
  OdString paramName = OD_T( "ViewLabelParamName" );

  res = pParentAction->addParam( paramName, pClass, paramId, paramIndex );
  if ( eOk != res )
    return res;

  OdDbAssocViewLabelActionParamPtr pViewLabelParam = OdDbAssocViewLabelActionParam::cast( paramId.openObject( OdDb::kForWrite ) );

  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty();
  pCompoundObjectId->set( idViewLabel );

  res = pViewLabelParam->setObject( pCompoundObjectId, false, true );
  if ( eOk != res )
    return res;
  OdDbObjectIdArray depIds;
  pViewLabelParam->getDependencies( true, true, depIds );
  ODA_ASSERT_ONCE( depIds.length() == 1 );

  OdDbEntityPtr pEnt = idViewLabel.openObject( OdDb::kForWrite );
  pEnt->addPersistentReactor( depIds.first() );

  return eOk;
}

OdDbObjectId OdDbImpAssocViewRepActionBody::viewSymbol( OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocActionParamPtr pViewRepParam =
    pParentAction->paramAtName( "ViewRepParamName" ).openObject();
  if ( !pViewRepParam.isNull() )
  {
    OdDbObjectIdArray ViewRepDepIds;
    pViewRepParam->getDependencies( true, true, ViewRepDepIds );
    ODA_ASSERT_ONCE( ViewRepDepIds.length() == 1 );

    OdDbAssocDependencyPtr pViewRepDep = ViewRepDepIds.first().openObject();
    OdDbAssocActionPtr pActionBase = pViewRepDep->dependentOnObject().openObject();
    ODA_ASSERT_ONCE( !pActionBase.isNull() );

    OdDbAssocViewRepActionBodyPtr pActionBodyBase = pActionBase->actionBody().openObject();
    OdDbViewRepPtr pViewRepBase = pActionBodyBase->viewRepId().openObject();
    return pViewRepBase->getSketchManager()->getSymbolSketchFromViewRep( m_viewRepId )->getSecSymbolId();
  }

  return OdDbObjectId();
}

OdResult OdDbImpAssocViewRepActionBody::getViewLabelOffset( OdGeVector2d& offset,
                                                            bool Unknown,
                                                            OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocViewLabelActionParamPtr pViewLabelParam =
    pParentAction->paramAtName( "ViewLabelParamName" ).openObject();
  
  if ( !pViewLabelParam.isNull() )
  {
    offset = pViewLabelParam->labelOffset();
  }
  
  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::setViewLabelOffset( const OdGeVector2d& offset,
                                                            bool Unknown,
                                                            OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocViewLabelActionParamPtr pViewLabelParam =
    pParentAction->paramAtName( "ViewLabelParamName" ).openObject( OdDb::kForWrite );

  if ( !pViewLabelParam.isNull() )
  {
    pViewLabelParam->setLabelOffset( offset );
  }

  return eOk;
}

OdDbObjectId OdDbImpAssocViewRepActionBody::parentViewBorder( OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocActionParamPtr pViewRepParam =
    pParentAction->paramAtName( "ViewRepParamName" ).openObject();

  if ( pViewRepParam.isNull() )
    return OdDbObjectId();

  OdDbObjectIdArray ViewRepDepIds;
  pViewRepParam->getDependencies( true, true, ViewRepDepIds );
  ODA_ASSERT_ONCE( ViewRepDepIds.length() == 1 );

  OdDbAssocDependencyPtr pViewRepDep = ViewRepDepIds.first().openObject();
  OdDbAssocActionPtr pActionBase = pViewRepDep->dependentOnObject().openObject();
  ODA_ASSERT_ONCE( !pActionBase.isNull() );

  OdDbAssocViewRepActionBodyPtr pActionBodyBase = pActionBase->actionBody().openObject();
  return pActionBodyBase->currentViewBorder();
}

OdDbObjectId OdDbImpAssocViewRepActionBody::parentViewport( OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocActionParamPtr pViewRepParam =
    pParentAction->paramAtName( "ViewRepParamName" ).openObject();

  if ( pViewRepParam.isNull() )
    return OdDbObjectId();

  OdDbObjectIdArray ViewRepDepIds;
  pViewRepParam->getDependencies( true, true, ViewRepDepIds );
  ODA_ASSERT_ONCE( ViewRepDepIds.length() == 1 );

  OdDbAssocDependencyPtr pViewRepDep = ViewRepDepIds.first().openObject();
  OdDbAssocActionPtr pActionBase = pViewRepDep->dependentOnObject().openObject();
  ODA_ASSERT_ONCE( !pActionBase.isNull() );

  OdDbAssocViewRepActionBodyPtr pActionBodyBase = pActionBase->actionBody().openObject();
  return pActionBodyBase->currentViewport();
}

void OdDbImpAssocViewRepActionBody::viewSymbols( OdArray<OdDbObjectId> &symbolIds, bool bIncludeDetailViewSymol, OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject( ) );
  OdDbObjectIdArray paramsViewSym = pParentAction->paramsAtName( "ViewSymbolParamName" );
  OdDbObjectIdArray::iterator itParamViewSym;
  for ( itParamViewSym = paramsViewSym.begin(); itParamViewSym != paramsViewSym.end(); itParamViewSym++ )
  {
    OdDbAssocActionParamPtr pParamViewSym = itParamViewSym->openObject();
    OdDbObjectIdArray depIds;
    pParamViewSym->getDependencies( true, true, depIds );
    ODA_ASSERT_ONCE( depIds.length() == 1 );

    OdDbAssocDependencyPtr pDep = depIds.first().openObject();

    OdDbViewSymbolPtr pViewSymbol = pDep->dependentOnObject().openObject();
    
    if ( bIncludeDetailViewSymol
      || pViewSymbol->isKindOf( OdDbSectionSymbol::desc() ) )
    {
      symbolIds.push_back( pViewSymbol->objectId() );
    }
  }
}

OdDbObjectId OdDbImpAssocViewRepActionBody::viewStyle( OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocActionParamPtr pViewStyleParam =
    pParentAction->paramAtName( "ViewStyleParamName" ).openObject();
  if ( !pViewStyleParam.isNull() )
  {
    OdDbObjectIdArray ViewStyleDepIds;
    pViewStyleParam->getDependencies( true, true, ViewStyleDepIds );
    ODA_ASSERT_ONCE( ViewStyleDepIds.length() == 1 );

    OdDbAssocDependencyPtr pStyleDep = ViewStyleDepIds.first().openObject();
    return pStyleDep->dependentOnObject();
  }
  return OdDbObjectId();
}

void OdDbImpAssocViewRepActionBody::viewRepHatches( OdArray<OdDbObjectId> &hatchIds, OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocViewRepHatchManagerPtr pViewRepHatchManager = pParentAction->paramAtName( OD_T( "HatchManagerParamName" ) ).openObject();
  OdDbObjectIdArray hatchActionParamIds;
  pViewRepHatchManager->viewRepHatches( hatchActionParamIds );

  OdDbObjectIdArray::iterator itHatchActionParamId;
  for ( itHatchActionParamId = hatchActionParamIds.begin();
        itHatchActionParamId != hatchActionParamIds.end();
        itHatchActionParamId++ )
  {
    OdDbAssocActionParamPtr pViewRepHatchParam =
      OdDbAssocActionParam::cast( itHatchActionParamId->openObject( OdDb::kForRead ) );
    ODA_ASSERT_ONCE( !pViewRepHatchParam.isNull() );

    OdDbObjectIdArray hatchDepIds;
    pViewRepHatchParam->getDependencies( true, true, hatchDepIds );
    ODA_ASSERT_ONCE( hatchDepIds.length() == 1 );
    OdDbAssocDependencyPtr pHatchDep = hatchDepIds.first().openObject( OdDb::kForRead );

    hatchIds.append( pHatchDep->dependentOnObject() );
  }
}

void OdDbImpAssocViewRepActionBody::setShowLabelOverride( bool bShowLabelOverride )
{
  SETBIT( m_flags, kShowLabelOwerride, bShowLabelOverride );
}

bool OdDbImpAssocViewRepActionBody::isShowLabelOverriden() const
{
  return GETBIT( m_flags, kShowLabelOwerride );
}

void OdDbImpAssocViewRepActionBody::repairAlignment()
{
  SETBIT( m_flags, kBreakAlignment, 0 );
}

bool OdDbImpAssocViewRepActionBody::canBreakOrRepairAlignment() const
{
  return !GETBIT( m_flags, kNotAligned );
}

OdResult OdDbImpAssocViewRepActionBody::getViewLabelPosition( OdDbModelDocViewStyle::AttachmentPoint& labelPosType, OdDbObjectId idParentAction ) const
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocViewLabelActionParamPtr pViewLabelParam =
    pParentAction->paramAtName( "ViewLabelParamName" ).openObject();

  if ( pViewLabelParam.isNull() )
    return eNullObjectPointer;
 
  labelPosType = pViewLabelParam->attachmentPoint();
  return eOk;
  
}

OdResult OdDbImpAssocViewRepActionBody::setViewLabelPosition( enum OdDbModelDocViewStyle::AttachmentPoint labelPosType, OdDbObjectId idParentAction )
{
  if ( labelPosType > 1
    || labelPosType < 0 )
    return eInvalidInput;

  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdDbAssocViewLabelActionParamPtr pViewLabelParam =
    pParentAction->paramAtName( "ViewLabelParamName" ).openObject( OdDb::kForWrite );

  if ( pViewLabelParam.isNull() )
    return eNullObjectPointer;

  pViewLabelParam->setAttachmentPointOverride( labelPosType, false );
  return eOk;
}

enum OdDbAssocViewRepActionBody::AlignmentTypeEnum OdDbImpAssocViewRepActionBody::getOriginalAlignmentType() const
{
  if ( GETBIT( m_flags, kNotAligned ) )
    return OdDbAssocViewRepActionBody::kNotAligned;
  else
  {
    if ( isHorizontalAligned() )
      return OdDbAssocViewRepActionBody::kAligned;
    else
      return OdDbAssocViewRepActionBody::kVerticalAligned;
  }
}

OdResult OdDbImpAssocViewRepActionBody::removeViewSymbol( const OdDbObjectId& symbolId, OdDbObjectId idParentAction )
{
  OdDbViewRepPtr pViewRep = m_viewRepId.openObject( OdDb::kForWrite );
  OdRxSketchObject *pSketchObj
    = pViewRep->getSketchManager()->getSymbolSketchFromSymbol( symbolId );
  if ( !pSketchObj )
    return eInvalidInput;

  //
  removeViewSymbolParam( symbolId, idParentAction );
  //
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject() );
  OdString paramName = OdString().format( OD_T( "%s%d" ), L"CutDefinitionParamName", pSketchObj->sketchTag() );
  OdDbAssocActionParamPtr pParam = pParentAction->paramAtName( paramName ).openObject( OdDb::kForRead );
  ODA_ASSERT_ONCE( !pParam.isNull() );
  OdDbObjectIdArray depIds;
  pParam->getDependencies( true, true, depIds );
  ODA_ASSERT_ONCE( depIds.length() == 1 );
  OdDbAssocDependencyPtr pDepPline = depIds.first().openObject();
  OdDbObjectPtr pObjPline = pDepPline->dependentOnObject().openObject( OdDb::kForWrite );
  pObjPline->erase();
  pParentAction->removeDependency( pDepPline->objectId(), true );
  pParentAction->removeParam( pParam->objectId(), true );
  //
  pViewRep->getSketchManager()->deleteSketch( pSketchObj );
  //
  OdDbObjectPtr pSym = symbolId.openObject( OdDb::kForWrite );
  pSym->erase();

  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::removeViewSymbolParam( const OdDbObjectId& symbolParamId, OdDbObjectId idParentAction )
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject( OdDb::kForWrite ) );
  OdDbObjectIdArray paramsViewSym = pParentAction->paramsAtName( "ViewSymbolParamName" );
  OdDbObjectIdArray::iterator it_ParamViewSym;
  for ( it_ParamViewSym = paramsViewSym.begin(); it_ParamViewSym != paramsViewSym.end(); it_ParamViewSym++ )
  {
    OdDbAssocActionParamPtr pParamViewSym = it_ParamViewSym->openObject();
    OdDbObjectIdArray depIds;
    pParamViewSym->getDependencies( true, true, depIds );
    ODA_ASSERT_ONCE( depIds.length() == 1 );
    OdDbAssocDependencyPtr pDep = depIds.first().openObject();

    OdDbViewSymbolPtr pViewSym = pDep->dependentOnObject().openObject( OdDb::kForWrite );
    if ( pViewSym->objectId() == symbolParamId )
    {
      pParentAction->removeParam( pParamViewSym->objectId(), true );
      pParentAction->removeDependency( pDep->objectId(), true );
      return eOk;
    }
  }
  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::setViewSymbol( const OdDbObjectId& idViewSymbol, OdDbObjectId idParentAction )
{
  OdResult res = eOk;
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject( OdDb::kForWrite ) );
  int paramIndex = 0;
  OdDbObjectId paramId;
  OdRxClass* pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T( "AcDbAssocViewSymbolActionParam" ) ).get();
  OdString paramName = OD_T( "ViewSymbolParamName" );

  res = pParentAction->addParam( paramName, pClass, paramId, paramIndex );
  if ( eOk != res )
    return res;

  OdDbAssocViewSymbolActionParamPtr pViewSymbolParam = OdDbAssocViewSymbolActionParam::cast( paramId.openObject( OdDb::kForWrite ) );

  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty();
  pCompoundObjectId->set( idViewSymbol );

  res = pViewSymbolParam->setObject( pCompoundObjectId, false, true );
  if ( eOk != res )
    return res;

  OdDbObjectIdArray depIds;
  pViewSymbolParam->getDependencies( true, true, depIds );
  ODA_ASSERT_ONCE( depIds.length() == 1 );

  OdDbObjectPtr pObj = idViewSymbol.openObject( OdDb::kForWrite );
  pObj->addPersistentReactor( depIds.first() );

  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::setViewStyle( const OdDbObjectId& idViewStyle, OdDbObjectId idParentAction )
{
  OdResult res = eOk;
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject( OdDb::kForWrite ) );

  int paramIndex = 0;
  OdDbObjectId paramId;
  OdRxClass* pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T( "AcDbAssocViewStyleActionParam" ) ).get();
  OdString paramName = OD_T( "ViewStyleParamName" );

  res = pParentAction->addParam( paramName, pClass, paramId, paramIndex );
  if ( eOk != res )
    return res;

  OdDbAssocViewStyleActionParamPtr pViewSymbolParam = OdDbAssocViewStyleActionParam::cast( paramId.openObject( OdDb::kForWrite ) );

  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty(); 
  pCompoundObjectId->set( idViewStyle );

  res = pViewSymbolParam->setObject( pCompoundObjectId, false, true );
  if ( eOk != res )
    return res;

  OdDbObjectIdArray depIds;
  pViewSymbolParam->getDependencies( true, true, depIds );
  ODA_ASSERT_ONCE( depIds.length() == 1 );

  OdDbObjectPtr pObj = idViewStyle.openObject( OdDb::kForWrite );
  pObj->addPersistentReactor( depIds.first() );

  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::attachHatchManager( const OdDbObjectId& hatchManagerId, OdDbObjectId idParentAction )
{
  OdResult res = eOk;
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject( OdDb::kForWrite ) );
  int idx;
  res = pParentAction->addParam( hatchManagerId, idx );
  if ( eOk != res )
    return res;

  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::setDefinitionId( const OdDbObjectId& objId, unsigned short tag, OdDbObjectId idParentAction )
{
  OdResult res = eOk;
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject( OdDb::kForWrite ) );
  int paramIndex = 0;
  OdDbObjectId paramId;
  OdRxClass* pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T( "AcDbAssocObjectActionParam" ) ).get();
  OdString paramName = OdString().format( OD_T( "%s%d" ), L"CutDefinitionParamName", tag );

  res = pParentAction->addParam( paramName, pClass, paramId, paramIndex );
  if ( eOk != res )
    return res;

  OdDbAssocObjectActionParamPtr pCutDefParam = OdDbAssocObjectActionParam::cast( paramId.openObject( OdDb::kForWrite ) );

  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty();
  pCompoundObjectId->set( objId );

  res = pCutDefParam->setObject( pCompoundObjectId, true, false );
  if ( eOk != res )
    return res;
  OdDbObjectIdArray depIds;
  pCutDefParam->getDependencies( true, true, depIds );
  ODA_ASSERT_ONCE( depIds.length() == 1 );

  OdDbEntityPtr pEnt = objId.openObject( OdDb::kForWrite );
  pEnt->addPersistentReactor( depIds.first() );

  return eOk;
}

OdResult OdDbImpAssocViewRepActionBody::removeDefinitionParameters( unsigned short tag, OdDbObjectId idParentAction )
{
  OdDbAssocActionPtr pParentAction = OdDbAssocAction::cast( idParentAction.openObject( OdDb::kForWrite ) );
  OdDbAssocActionParamPtr pCutDefParam = pParentAction->paramAtName( OdString().format( OD_T( "%s%d" ), L"CutDefinitionParamName", tag ) ).openObject();

  if ( !pCutDefParam.isNull() )
  {
    OdDbObjectIdArray depIds;
    pCutDefParam->getDependencies( true, true, depIds );
    ODA_ASSERT_ONCE( depIds.length() == 1 );
    pParentAction->removeParam( pCutDefParam->objectId( ), true );
    pParentAction->removeDependency( depIds.first(), true );
  }
  return eOk;
}

