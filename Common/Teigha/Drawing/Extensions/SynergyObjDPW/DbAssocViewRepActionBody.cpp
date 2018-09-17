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

ODRX_DEFINE_MEMBERS_EX(OdDbAssocViewRepActionBody,    // ClassName
                       OdDbAssocParamBasedActionBody, // ParentClass
                       DBOBJECT_CONSTR,               // DOCREATE
                       OdDb::kDHL_1027,               // DwgVer
                       OdDb::kMRelease8,              // MaintVer
                       1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocViewRepActionBody", // DWG class name
                       L"ACDBASSOCVIEWREPACTIONBODY", // DxfName
                       L"AcSynergyObjDPW",            // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocViewRepActionBody::OdDbAssocViewRepActionBody(bool createImpObj)
 : OdDbAssocParamBasedActionBody()
{
  if (createImpObj)
    m_pImpObj = new OdDbImpAssocViewRepActionBody();
}

OdDbAssocViewRepActionBody::~OdDbAssocViewRepActionBody()
{
  if (m_pImpObj)
  {
    delete m_pImpObj;
    m_pImpObj = NULL;
  }
}

OdResult OdDbAssocViewRepActionBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  return ((OdDbImpAssocViewRepActionBody*)m_pImpObj)->dwgInFields(pFiler, objectId());
}

void OdDbAssocViewRepActionBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  ((OdDbImpAssocViewRepActionBody*)m_pImpObj)->dwgOutFields(pFiler, objectId());
}

OdResult OdDbAssocViewRepActionBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eMakeMeProxy;

  OdResult res = OdDbObject::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  return ((OdDbImpAssocViewRepActionBody*)m_pImpObj)->dxfInFields(pFiler, objectId());
}

void OdDbAssocViewRepActionBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbObject::dxfOutFields(pFiler);
  ((OdDbImpAssocViewRepActionBody*)m_pImpObj)->dxfOutFields(pFiler, objectId());
}

void OdDbAssocViewRepActionBody::evaluateOverride()
{
  assertWriteEnabled();
  static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->evaluateOverride( parentAction() );
}

OdResult OdDbAssocViewRepActionBody::setViewRepId(const OdDbObjectId& id)
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    return eNotApplicable;

  assertWriteEnabled();
  ((OdDbImpAssocViewRepActionBody*)m_pImpObj)->m_viewRepId = id;
  return eOk;
}

OdDbObjectId OdDbAssocViewRepActionBody::viewRepId()
{
  ODA_ASSERT_ONCE(m_pImpObj);
  if (!m_pImpObj)
    throw OdError(eNotApplicable);

  assertReadEnabled();
  return ((OdDbImpAssocViewRepActionBody*)m_pImpObj)->m_viewRepId;
}

bool OdDbAssocViewRepActionBody::isAligned() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->isAligned();
}

bool OdDbAssocViewRepActionBody::isHorizontalAligned() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->isHorizontalAligned();
}

bool OdDbAssocViewRepActionBody::breakAlignment()
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->breakAlignment();
}

void OdDbAssocViewRepActionBody::setNeedBeErased( bool erased )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setNeedBeErased( erased );
}

bool OdDbAssocViewRepActionBody::isNeedBeErased() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->isNeedBeErased();
}

OdDbObjectId OdDbAssocViewRepActionBody::hatchManager() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->hatchManager( ownerId() );
}

void OdDbAssocViewRepActionBody::composeForLoad( OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo )
{
  static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->composeForLoad( this, format, version );
}

OdDbObjectId OdDbAssocViewRepActionBody::currentViewBorder() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->currentViewBorder( ownerId() );
}

OdDbObjectId OdDbAssocViewRepActionBody::currentViewport() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->currentViewport( ownerId() );
}

OdResult OdDbAssocViewRepActionBody::getShowHatch( bool &bShow ) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->getShowHatch( bShow, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::setShowHatch( bool bShow, bool bUnknown )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setShowHatch( bShow, bUnknown, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::getShowViewLabel( bool &bShow ) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->getShowViewLabel( bShow, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::setShowViewLabel( bool bShow, bool bUnknown )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setShowViewLabel( bShow, bUnknown, ownerId() );
}

OdDbObjectId OdDbAssocViewRepActionBody::viewLabel() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->viewLabel( ownerId() );
}

OdResult OdDbAssocViewRepActionBody::setViewLabel( const OdDbObjectId& idViewLabel )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setViewLabel( idViewLabel, ownerId() );
}

OdDbObjectId OdDbAssocViewRepActionBody::viewSymbol() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->viewSymbol( ownerId() );
}

OdResult OdDbAssocViewRepActionBody::getViewLabelOffset( OdGeVector2d& offset, bool Unknown ) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->getViewLabelOffset( offset, Unknown, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::setViewLabelOffset( const OdGeVector2d& offset, bool Unknown )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setViewLabelOffset( offset, Unknown, ownerId() );
}

OdDbObjectId OdDbAssocViewRepActionBody::parentViewBorder() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->parentViewBorder( ownerId() );
}

OdDbObjectId OdDbAssocViewRepActionBody::parentViewport() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->parentViewport( ownerId() );
}

void OdDbAssocViewRepActionBody::viewSymbols( OdArray<OdDbObjectId> &symbolIds, bool bIncludeDetailViewSymol ) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->viewSymbols( symbolIds, bIncludeDetailViewSymol, ownerId() );
}

OdDbObjectId OdDbAssocViewRepActionBody::viewStyle() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->viewStyle( ownerId() );
}

void OdDbAssocViewRepActionBody::viewRepHatches( OdArray<OdDbObjectId> &hatchIds ) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->viewRepHatches( hatchIds, ownerId() );
}

void OdDbAssocViewRepActionBody::setShowLabelOverride( bool bShowLabelOverride )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setShowLabelOverride( bShowLabelOverride );
}

bool OdDbAssocViewRepActionBody::isShowLabelOverriden() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->isShowLabelOverriden();
}

void OdDbAssocViewRepActionBody::repairAlignment()
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->repairAlignment();
}

bool OdDbAssocViewRepActionBody::canBreakOrRepairAlignment() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->canBreakOrRepairAlignment();
}

OdResult OdDbAssocViewRepActionBody::getViewLabelPosition( OdDbModelDocViewStyle::AttachmentPoint& labelPosType ) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>
    (m_pImpObj)->getViewLabelPosition( labelPosType, ownerId( ) );
}

OdResult OdDbAssocViewRepActionBody::setViewLabelPosition( enum OdDbModelDocViewStyle::AttachmentPoint labelPosType )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>
    (m_pImpObj)->setViewLabelPosition( labelPosType, ownerId() );
}

enum OdDbAssocViewRepActionBody::AlignmentTypeEnum OdDbAssocViewRepActionBody::getOriginalAlignmentType() const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>
    (m_pImpObj)->getOriginalAlignmentType();
}

OdResult OdDbAssocViewRepActionBody::removeViewSymbol( const OdDbObjectId& symbolId )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->removeViewSymbol( symbolId, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::removeViewSymbolParam( const OdDbObjectId& symbolParamId )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->removeViewSymbolParam( symbolParamId, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::setViewSymbol( const OdDbObjectId& idViewSymbol )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setViewSymbol( idViewSymbol, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::setViewStyle( const OdDbObjectId& idViewStyle )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setViewStyle( idViewStyle, ownerId( ) );
}

OdResult OdDbAssocViewRepActionBody::attachHatchManager( const OdDbObjectId& hatchManagerId )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->attachHatchManager( hatchManagerId, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::setDefinitionId( const OdDbObjectId& objId, unsigned short tag )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->setDefinitionId( objId, tag, ownerId() );
}

OdResult OdDbAssocViewRepActionBody::removeDefinitionParameters( unsigned short tag )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocViewRepActionBody*>(m_pImpObj)->removeDefinitionParameters( tag, ownerId() );
}
