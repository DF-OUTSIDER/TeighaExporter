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
#include "DbImpAssocArrayParameters.h"
#include "algorithm"
#include "DbEntity.h"
#include "DbBlockReference.h"
#include "DbAssocArrayActionBody.h"
#include "DbAssocArrayModifyActionBody.h"
#include "DbAssocActionParam.h"
#include "DbAssocArrayPathParameters.h"
#include "DbAssocArrayPolarParameters.h"
#include "DbAssocArrayRectangularParameters.h"
#include "DbAssocDependency.h"
#include "DbBlockTableRecord.h"
#include "DbAudit.h"
#include "DbHostAppServices.h"

#define NEXT_CODE(code) \
  if (pFiler->nextItem() != code) \
  { \
    ODA_FAIL(); \
    return eMakeMeProxy; \
  }

//////////////////////////////////////////////////////////////////////////

class findItemByLocator
{
  const OdDbItemLocator &m_locator;
public:
  inline findItemByLocator(const OdDbItemLocator& locator)
    : m_locator(locator)
  {
  }

  inline bool operator()( const OdDbAssocArrayItemPtr &Item) const
  {
    return Item->locator() == m_locator;
  }

  inline bool operator()( const OdDbItemLocator &index ) const
  {
    return index == m_locator;
  }
};

class getGripPoint
{
  OdArray<OdDbGripDataPtrArray>& m_Grips;
public:
  inline getGripPoint(OdArray<OdDbGripDataPtrArray>& Grips)
    : m_Grips(Grips)
  {
  }

  inline void operator()( const OdDbAssocArrayItemPtr &Item) const
  {
    OdDbEntityPtr pEnt 
      = OdDbEntity::cast( Item->entityProvider().openObject(OdDb::kForRead) );
    if ( pEnt.isNull() )
    {
      ODA_FAIL_ONCE();
      return;
    }
    OdDbGripDataPtrArray grips;
    pEnt->getGripPoints( grips, 10, 10, OdGeVector3d::kZAxis, 0 );
    m_Grips.push_back( grips );
  }
};

class findValueParamByName
{
  const OdString& m_Name;
public:
  inline findValueParamByName(const OdString& Name)
    : m_Name(Name)
  {
  }

  inline bool operator()(const OdDbImpValueParamPtr &ValueParam) const
  {
    return ValueParam->name() == m_Name;
  }
};

OdDbImpAssocArrayItem::OdDbImpAssocArrayItem()
  : bErased(false)
{
  m_entityProviderId = NULL;  
}

OdDbImpAssocArrayItem::~OdDbImpAssocArrayItem()
{
}

OdResult OdDbImpAssocArrayItem::dwgInFields(OdDbDwgFiler* pFiler)
{
  int ver = pFiler->rdInt32();
  ODA_ASSERT_ONCE(!ver); // TODO
  if (ver)
    return eMakeMeProxy;

  OdUInt32 itemIndex  = pFiler->rdInt32(),
           rowIndex   = pFiler->rdInt32(),
           levelIndex = pFiler->rdInt32();

  m_locator = OdDbItemLocator(itemIndex, rowIndex, levelIndex);

  int flags = pFiler->rdInt32();
  
  OdDb::MaintReleaseVer mver;
  pFiler->dwgVersion( &mver );


    if ( GETBIT(flags, kDefaulTransform) )
    {
      for ( int i = 0; i < 4; i++ )
      {
        for ( int j = 0; j < 4; j++ )
          m_matrix.entry[i][j] = pFiler->rdDouble();
      }
    }
    else
    {
      OdGePoint3d point = pFiler->rdPoint3d();
      m_matrix.setToTranslation( OdGeVector3d( point.x, point.y, point.z ) );
    }

    if ( GETBIT(flags, kRealtiveTransform) )
    {
      for ( int i = 0; i < 4; i++ )
      {
        for ( int j = 0; j < 4; j++ )
          m_relative.entry[i][j] = pFiler->rdDouble();
      }
    }
  

  bErased = GETBIT(flags, kErased) | (!GETBIT(flags, kItemPresent));

  if ( !bErased )
    m_entityProviderId = pFiler->rdSoftPointerId();
  if ( GETBIT( flags, kModified ) )
    m_repBodyId = pFiler->rdSoftPointerId();

  return eOk;
}

void OdDbImpAssocArrayItem::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  int ver = 0;
  pFiler->wrInt32(ver);

  pFiler->wrInt32(m_locator[OdDbItemLocator::kItemIndex]);
  pFiler->wrInt32(m_locator[OdDbItemLocator::kRowIndex]);  
  pFiler->wrInt32(m_locator[OdDbItemLocator::kLevelIndex]);

  bool defaultTransformPresence = !(   m_matrix.getCsXAxis().isEqualTo( OdGeVector3d(1, 0, 0) ) 
                                    && m_matrix.getCsYAxis().isEqualTo( OdGeVector3d(0, 1, 0) )
                                    && m_matrix.getCsZAxis().isEqualTo( OdGeVector3d(0, 0, 1) ) );
  
  int flags = 0; 
  OdDb::MaintReleaseVer mver;
  pFiler->dwgVersion( &mver );

  SETBIT( flags, kErased, bErased );
  SETBIT( flags, kItemPresent, !bErased );
  SETBIT( flags, kDefaulTransform, defaultTransformPresence );
  SETBIT( flags, kRealtiveTransform, hasRelativeTransform() );
  SETBIT( flags, kModified, !m_repBodyId.isNull() );

  pFiler->wrInt32( flags );

  if ( defaultTransformPresence )
  {
    for ( int i = 0; i < 4; i++ )
    {
      for ( int j = 0; j < 4; j++ )
        pFiler->wrDouble( m_matrix.entry[i][j] );
    }
  }
  else
  {
    pFiler->wrPoint3d(  OdGePoint3d( m_matrix[0][3], m_matrix[1][3], m_matrix[2][3] ) );
  }

  if ( GETBIT(flags, kRealtiveTransform) )
  {
    for ( int i = 0; i < 4; i++ )
    {
      for ( int j = 0; j < 4; j++ )
        pFiler->wrDouble( m_relative.entry[i][j] );
    }
  }

  if (!bErased)
    pFiler->wrSoftPointerId( m_entityProviderId );

  if ( !m_repBodyId.isNull() )
    pFiler->wrSoftPointerId( m_repBodyId );
}

OdResult OdDbImpAssocArrayItem::dxfInFields(OdDbDxfFiler* pFiler)
{
  if (!pFiler->atSubclassData(OdDbAssocArrayItem::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdUInt32 ver = pFiler->rdUInt32();
  ODA_ASSERT_ONCE(!ver); // TODO
  if (ver)
    return eMakeMeProxy;

  NEXT_CODE(90)
  OdUInt32 itemIndex = pFiler->rdUInt32();
  NEXT_CODE(90)
  OdUInt32 rowIndex = pFiler->rdUInt32();
  NEXT_CODE(90)
  OdUInt32 levelIndex = pFiler->rdUInt32();

  m_locator = OdDbItemLocator(itemIndex, rowIndex, levelIndex);

  NEXT_CODE( 90 )

  int flags = pFiler->rdInt32();

  if ( GETBIT(flags, kDefaulTransform) )
  {
    for ( int i = 0; i < 4; i++ )
    {
      for ( int j = 0; j < 4; j++ )
      {
        NEXT_CODE( 40 )
        m_matrix.entry[i][j] = pFiler->rdDouble();
      }

    }
  }
  else
  {
    OdGePoint3d point;
    NEXT_CODE( 11 )
    pFiler->rdPoint3d( point );
    m_matrix.setToTranslation( OdGeVector3d( point.x, point.y, point.z ) );
  }

  if ( GETBIT(flags, kRealtiveTransform) )
  {
    for ( int i = 0; i < 4; i++ )
    {
      for ( int j = 0; j < 4; j++ )
      {
        NEXT_CODE( 40 )
        m_relative.entry[i][j] = pFiler->rdDouble();
      }
    }
  }
  
  

  bErased = GETBIT(flags, kErased) | (!GETBIT(flags, kItemPresent));

  if ( !bErased )
  {
    NEXT_CODE( 330 )
    m_entityProviderId = pFiler->rdObjectId();
  }

  if ( GETBIT(flags, kModified ) )
  {
    NEXT_CODE( 330 )
    m_repBodyId = pFiler->rdObjectId();
  }

  return eOk;
}

void OdDbImpAssocArrayItem::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrSubclassMarker(OdDbAssocArrayItem::desc()->name());

  OdUInt32 ver = 0;
  pFiler->wrUInt32(90, ver);

  pFiler->wrUInt32(90, m_locator[OdDbItemLocator::kItemIndex]);
  pFiler->wrUInt32(90, m_locator[OdDbItemLocator::kRowIndex]);  
  pFiler->wrUInt32(90, m_locator[OdDbItemLocator::kLevelIndex]);

  bool defaultTransformPresence = ! ( m_matrix.getCsXAxis().isEqualTo( OdGeVector3d(1, 0, 0) ) 
                                   && m_matrix.getCsYAxis().isEqualTo( OdGeVector3d(0, 1, 0) )
                                   && m_matrix.getCsZAxis().isEqualTo( OdGeVector3d(0, 0, 1) ) );


  int flags = 0;

  SETBIT( flags, kErased, bErased );
  SETBIT( flags, kItemPresent, !bErased );
  SETBIT( flags, kDefaulTransform, defaultTransformPresence );
  SETBIT( flags, kRealtiveTransform, hasRelativeTransform() );
  SETBIT( flags, kModified, !m_repBodyId.isNull() );

  pFiler->wrInt32( 90, flags );

  if ( defaultTransformPresence )
  {
    for ( int i = 0; i < 4; i++ )
    {
      for ( int j = 0; j < 4; j++ )
        pFiler->wrDouble( 40, m_matrix.entry[i][j] );
    }
  }
  else
  {
    pFiler->wrPoint3d( 11, OdGePoint3d( m_matrix[0][3], m_matrix[1][3], m_matrix[2][3] ) );
  }

  if ( GETBIT(flags, kRealtiveTransform) )
  {
    for ( int i = 0; i < 4; i++ )
    {
      for ( int j = 0; j < 4; j++ )
        pFiler->wrDouble( 40, m_relative.entry[i][j] );
    }
  }
  
  if (!bErased)
    pFiler->wrObjectId( 330, m_entityProviderId );

  if ( !m_repBodyId.isNull() )
    pFiler->wrObjectId( 330, m_repBodyId );
}

OdResult OdDbImpAssocArrayItem::transformBy( const OdGeMatrix3d& transform )
{
  m_relative = transform;
  return eOk;
}

OdResult OdDbImpAssocArrayItem::getTransform(OdGeMatrix3d& transform, bool bCompounded) const
{  
  bool defaultTransformPresence = !(m_matrix.getCsXAxis().isEqualTo( OdGeVector3d::kXAxis )
                                 && m_matrix.getCsYAxis().isEqualTo( OdGeVector3d::kYAxis )
                                 && m_matrix.getCsZAxis().isEqualTo( OdGeVector3d::kZAxis ));

  transform = m_matrix;

  if (bCompounded)
  {
    if (defaultTransformPresence)
    {
      OdGeVector3d offset = m_matrix.translation();
      OdGeMatrix3d toOrigin, fromOrigin;
      toOrigin.setToTranslation(-offset);
      fromOrigin.setToTranslation(offset);
      transform = fromOrigin * m_relative * toOrigin * m_matrix;
    }
    else
      transform.postMultBy(m_relative);
  }
  
  return eOk;
}

OdResult OdDbImpAssocArrayItem::setDefaultTransform( const OdGeMatrix3d& transform )
{
  m_matrix = transform;
  return eOk;
}

bool OdDbImpAssocArrayItem::hasRelativeTransform() const
{
  return !m_relative.isEqualTo( OdGeMatrix3d() );
}

OdResult OdDbImpAssocArrayItem::setErased( bool bErased )
{
  this->bErased = bErased;
  return eOk;
}

void OdDbImpAssocArrayItem::reset()
{
  bErased = false;
  m_relative.setToIdentity();
  if ( !m_repBodyId.isNull() )
  {
    OdDbAssocArrayModifyActionBodyPtr pModifyBody = m_repBodyId.openObject( OdDb::kForWrite );
    OdArray<OdDbItemLocator> indices;
    pModifyBody->getOverridenItems( indices );

    indices.erase(
      std::remove_if( indices.begin(), indices.end(), findItemByLocator( m_locator ) ),
      indices.end() );

    pModifyBody->setOverridenItems( indices );
    m_repBodyId.setNull();
  }
}

//////////////////////////////////////////////////////////////////////////

OdDbImpAssocArrayParameters::OdDbImpAssocArrayParameters()
{
  m_pOwner = NULL;
}

OdDbImpAssocArrayParameters::~OdDbImpAssocArrayParameters()
{
}

OdResult OdDbImpAssocArrayParameters::dwgInFields(OdDbDwgFiler* pFiler)
{
  m_items.clear();

  int ver = pFiler->rdInt32();
  ODA_ASSERT_ONCE(!ver); // TODO
  if (ver)
    return eMakeMeProxy;

  OdUInt32 cnt = pFiler->rdInt32();
  if ( !cnt )
    return eOk;

  OdString sClass = pFiler->rdString(); // = L"AcDbAssocArrayItem";
  ODA_ASSERT_ONCE(!sClass.isEmpty()); // TODO
  if (sClass.isEmpty())
    return eMakeMeProxy;

  for (OdUInt32 index = 0; index < cnt; index++)
  {
    OdRxObjectPtr pObj  = ::odrxCreateObject(sClass);
    OdDbAssocArrayItemPtr pItem = OdDbAssocArrayItem::cast(pObj);
    ODA_ASSERT_ONCE(pItem.get());
    if (!pItem.get())
      return eMakeMeProxy;
    
    OdResult res = pItem->dwgInFields(pFiler);
    if (res != eOk)
      return res;

    m_items.push_back(pItem);
  } 

  if ( OdDbFiler::kUndoFiler == pFiler->filerType() )
  {
    m_pOwner = OdDbAssocArrayActionBody::cast(pFiler->rdSoftPointerId().openObject());
  }

  return eOk;
}

void OdDbImpAssocArrayParameters::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  int ver = 0;
  pFiler->wrInt32(ver); // TODO

  OdUInt32 cnt = m_items.size();
  //ODA_ASSERT_ONCE(cnt);
  if (!cnt)
    return;//throw OdError(eNotApplicable);
  pFiler->wrInt32(cnt);

  OdString sClass = m_items[0]->isA()->name(); // = L"AcDbAssocArrayItem";
  pFiler->wrString(sClass); 

  for (OdUInt32 idx = 0; idx < cnt; idx++)
  {
    OdDbAssocArrayItemPtr pItem = m_items[idx];
    pItem->dwgOutFields(pFiler);
  }  

  if ( OdDbFiler::kUndoFiler == pFiler->filerType() )
  {
    pFiler->wrSoftPointerId( m_pOwner->objectId() );
  }
}

OdResult OdDbImpAssocArrayParameters::dxfInFields(OdDbDxfFiler* pFiler)
{
  m_items.clear();

  NEXT_CODE(90)
  OdUInt32 ver = pFiler->rdUInt32();
  if (ver)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  OdUInt32 cnt = pFiler->rdUInt32();
  if ( !cnt )
    return eOk;

  OdString sClass; // = L"AcDbAssocArrayItem";
  NEXT_CODE(1)
  sClass = pFiler->rdString();
  if (sClass.isEmpty())
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  for (OdUInt32 index = 0; index < cnt; index++)
  {
    OdRxObjectPtr pObj  = ::odrxCreateObject(sClass);
    OdDbAssocArrayItemPtr pItem = OdDbAssocArrayItem::cast(pObj);
    if (!pItem.get())
    {
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }
  
    OdResult res = pItem->dxfInFields(pFiler);
    if (res != eOk)
      return res;

    m_items.push_back(pItem);
  }  

  return eOk;
}

void OdDbImpAssocArrayParameters::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdUInt32 ver = 0;
  pFiler->wrUInt32(90, ver);

  OdUInt32 cnt = m_items.size();
  ODA_ASSERT_ONCE(cnt); // test
  pFiler->wrUInt32(90, cnt);

  OdString sClass = L"AcDbAssocArrayItem";
  if (!cnt)
    return;
  
  sClass = m_items[0]->isA()->name();
  pFiler->wrString(1, sClass);

  for (OdUInt32 idx = 0; idx < cnt; idx++)
    m_items[idx]->dxfOutFields(pFiler);  
}

void OdDbImpAssocArrayParameters::composeForLoad( OdDbAssocArrayParameters *pAssocArrayParam, OdDb::SaveType format, OdDb::DwgVersion version )
{
    
}

OdResult OdDbImpAssocArrayParameters::getGripPointAt( OdArray<OdDbGripDataPtrArray>& grips ) const
{
  std::for_each( m_items.begin(), m_items.end(), getGripPoint(grips) );
  return eOk;
}

OdResult OdDbImpAssocArrayParameters::setValueParam( const OdString& paramName, 
                                                     const OdDbEvalVariant& value, 
                                                     const OdString& expression, 
                                                     const OdString& evaluatorId, 
                                                     OdString& errorMessage, 
                                                     OdValue::UnitType* pUnitType)
{
  OdResult res;

  if (m_pOwner)
  {
    res = m_pOwner->setValueParam( paramName, value, expression, evaluatorId, errorMessage, true );
    if ( eOk != res )
    {
      return res;
    }
    return m_pOwner->setValueParamUnitType( paramName, *pUnitType );
  }
  else
  {
    OdArray<OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                               m_arrValueParams.end(), 
                                                               findValueParamByName(paramName) );

    OdDbImpValueParamPtr pValueParam;
    if ( m_arrValueParams.end() == it )
    {
      pValueParam = new OdDbImpValueParam;
      pValueParam->setName(paramName); 
      m_arrValueParams.push_back( pValueParam );
    }
    else
    {
      pValueParam = *it;
    }
    pValueParam->setValue( value );
    pValueParam->setUnitType(*pUnitType);
  }
   return eOk;
}

OdResult OdDbImpAssocArrayParameters::getValueParam( const OdString& paramName, 
                                                     OdDbEvalVariant& value, 
                                                     OdString& expression, 
                                                     OdString& evaluatorId, 
                                                     OdValue::UnitType& unitType) const
{
  if (m_pOwner)
  {
    unitType = m_pOwner->valueParamUnitType( paramName );
    return m_pOwner->getValueParam( paramName, value, expression, evaluatorId );
  }
  else
  {
    OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                                     m_arrValueParams.end(), 
                                                                     findValueParamByName(paramName) );

    if ( m_arrValueParams.end() == it )
      return eNotInGroup;

    value = *(*it)->value();
    unitType = (*it)->unitType();

    return eOk;
  }
}

OdResult OdDbImpAssocArrayParameters::getGeomParam( const OdString& paramName, 
                                                    OdDbGeomRefPtr &pGeomRef, 
                                                    OdDbObjectId& paramId ) const
{
  if ( NULL == m_pOwner )
  {
    std::map <OdString, OdDbGeomRefPtr>::const_iterator it = m_GeomParam.find( paramName );
    if ( m_GeomParam.end() == it )
    {
      return eNotInGroup;
    }
    pGeomRef = (*it).second;
    return eOk;
  }

  paramId = m_pOwner->paramAtName( paramName );
  OdDbAssocActionParamPtr pActionParam = OdDbAssocActionParam::cast( paramId.openObject( OdDb::kForRead ) );
  if (pActionParam.isNull())
  {
    return eNotInGroup;
  }

  OdDbAssocEdgeActionParamPtr pEdgeActionParam = OdDbAssocEdgeActionParam::cast( pActionParam );
  if ( !pEdgeActionParam.isNull() )
  {
    OdArray<OdDbEdgeRef> edgeRef;
    pEdgeActionParam->getEdgeRef( edgeRef );
    pGeomRef = OdDbEdgeRef::createObject();
    OdDbEdgeRefPtr pEdgeRef = OdDbEdgeRef::cast(pGeomRef);
    *pEdgeRef.get() = edgeRef.first();
  }

  OdDbAssocVertexActionParamPtr pVertexActionParam = OdDbAssocVertexActionParam::cast( pActionParam );
  if ( !pVertexActionParam.isNull() )
  {
    OdArray<OdDbVertexRef> vertexRef;
    pVertexActionParam->getVertexRef( vertexRef );
    pGeomRef = OdDbVertexRef::createObject();
    OdDbVertexRefPtr pVertexRef = OdDbVertexRef::cast(pGeomRef);
    *pVertexRef.get() = vertexRef.first();
  }
  
  return eOk;
}

OdResult OdDbImpAssocArrayParameters::setGeomParam( const OdString& paramName, const OdDbGeomRef* pGeomRef, OdDbObjectId& paramId )
{
  if ( NULL == m_pOwner)
  {

    m_GeomParam.insert(std::make_pair((OdString) paramName, OdDbGeomRefPtr(pGeomRef) ) );
    return eOk;
  }

  OdDbEdgeRefPtr pEdgeRef = OdDbEdgeRef::cast(pGeomRef);
  OdDbVertexRefPtr pVertexRef = OdDbVertexRef::cast(pGeomRef);

  paramId = m_pOwner->paramAtName( paramName );  
  if (paramId.isNull())
  {
    OdRxClass* pClass;
    int paramIndex = 0;
    if ( !pEdgeRef.isNull() )
    {
      pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T("AcDbAssocEdgeActionParam") ).get();
      m_pOwner->addParam( paramName, pClass, paramId, paramIndex );
    }     
    
    if ( !pVertexRef.isNull() )
    {
      pClass = (OdRxClass*)odrxClassDictionary()->getAt( OD_T("AcDbAssocVertexActionParam") ).get();
      m_pOwner->addParam( paramName, pClass, paramId, paramIndex );
    }
  }  
  
  if ( !pEdgeRef.isNull() )
  {
    OdDbAssocEdgeActionParamPtr pActionParam = OdDbAssocEdgeActionParam::cast( paramId.openObject( OdDb::kForWrite ) );
    if ( pEdgeRef->entity().isEmpty() )
      pActionParam->setEdgeRef( *pEdgeRef.get(), false, false );
    else
      pActionParam->setEdgeRef( *pEdgeRef.get(), true, false );
  }

  if ( !pVertexRef.isNull() )
  {
    OdDbAssocVertexActionParamPtr pActionParam = OdDbAssocVertexActionParam::cast( paramId.openObject( OdDb::kForWrite ) );
    pActionParam->setVertexRef( *pVertexRef.get(), false, false );
  }

  return eOk;
}


void OdDbImpAssocArrayParameters::ownedValueParamNames( OdStringArray& paramNames ) const
{
  if (m_pOwner)
  {
    m_pOwner->ownedValueParamNames( paramNames );
  }
  else
  {
    OdArray<const OdDbImpValueParamPtr>::iterator it; 
    for ( it = m_arrValueParams.begin(); it != m_arrValueParams.end(); it++ )
      paramNames.append( (*it)->name());
  }
}


void OdDbImpAssocArrayParameters::ownedGeomParamNames( OdStringArray& paramNames ) const
{
  if (m_pOwner)
  {
    OdDbAssocActionPtr pAction = m_pOwner->parentAction().openObject( OdDb::kForRead );
    OdDbObjectIdArray ownedParamIds = pAction->ownedParams();
    OdDbAssocActionParamPtr pParam;

    OdDbObjectIdArray::iterator it;
    for ( it = ownedParamIds.begin(); it != ownedParamIds.end(); it++ )
    {
      pParam = it->openObject( OdDb::kForRead );
      paramNames.append( pParam->name() );
    }
  }
  else
  {
    std::map <OdString, OdDbGeomRefPtr> ::const_iterator it;
    for ( it = m_GeomParam.begin(); it != m_GeomParam.end(); it++ )
      paramNames.append( it->first );
  }
}

OdResult OdDbImpAssocArrayParameters::setOwner(class OdDbAssocArrayActionBody* pOwner)
{
  if (m_pOwner)
  {
    return eAlreadyActive;
  }

  m_pOwner = pOwner;

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(m_pOwner->parentAction().openObject(OdDb::kForWrite));

  OdString expression;
  OdString evaluatorId;
  OdArray<OdDbImpValueParamPtr>::iterator it_Value;
  OdString errorMSG;
  OdResult res;
  OdDbEvalVariantPtr pEV = OdDbEvalVariant::createObject();

  // Synchronize values
  if (!pAction.isNull())
  {
    for (it_Value = m_arrValueParams.begin(); it_Value != m_arrValueParams.end(); it_Value++)
    {
      if (eOk == pAction->getValueParam((*it_Value)->name(),
        *pEV,
        expression,
        evaluatorId))
      {
        (*it_Value)->setValue(*pEV);
        continue;
      }

      res = pAction->setValueParam((*it_Value)->name(),
        *(*it_Value)->value(),
        expression,
        evaluatorId,
        errorMSG,
        true);
      if (eOk != res)
      {
        m_pOwner = NULL;
        return res;
      }

      res = pAction->setValueParamUnitType((*it_Value)->name(),
        (*it_Value)->unitType());

      if (eOk != res)
      {
        m_pOwner = NULL;
        return res;
      }
    }
  }

  OdDbObjectId paramId;
  std::map <OdString, OdDbGeomRefPtr>::iterator it_Geom;
  for (it_Geom = m_GeomParam.begin(); it_Geom != m_GeomParam.end(); it_Geom++)
  {
    res = setGeomParam((*it_Geom).first, (*it_Geom).second, paramId);
    if (eOk != res)
    {
      m_pOwner = NULL;
      return res;
    }
  }

  m_GeomParam.clear();
  m_arrValueParams.clear();

  return pAction.isNull() ? eInvalidOwnerObject : eOk;
}

OdDbObjectIdArray OdDbImpAssocArrayParameters::getModifyBodies() const
{
  ItemArray::const_iterator itItems;
  OdDbObjectIdArray modBodyIds;
  OdDbObjectIdArray::iterator itForInsert;
  for ( itItems = m_items.begin(); itItems != m_items.end(); itItems++ )
  {
    OdDbImpAssocArrayItem *pMasterItemImpl = OdDbImpAssocArrayItem::getImpl( itItems->get() );

    OdDbObjectId modBodyId = pMasterItemImpl->modifyActionBody();
    if ( modBodyId.isNull() )
      continue;

    itForInsert = std::lower_bound( modBodyIds.begin(), modBodyIds.end(), modBodyId );
    if ( modBodyIds.end() == itForInsert )
    {
      modBodyIds.append( modBodyId );
    }
    else if ( modBodyId != *itForInsert )
    {
      modBodyIds.insert( itForInsert, modBodyId );
    }
  }
  return modBodyIds;
}

OdResult OdDbImpAssocArrayParameters::getItemPosition( const OdDbItemLocator& locator, OdGePoint3d& position, OdGeMatrix3d& xform ) const
{
  OdDbAssocArrayItemPtr pItem;

  OdResult res = GetItemByLocator( locator, pItem );
  if ( eOk != res )
    return res;

  res = pItem->getTransform( xform, false );
  if ( eOk == res )
  {
    position = OdGePoint3d( xform.translation().x,
                            xform.translation().y,
                            xform.translation().z );
  }

  return res;
}

//////////////////////////////////////////////////////////////////////////

OdDbImpAssocArrayCommonParameters::OdDbImpAssocArrayCommonParameters()
{
  m_itemCount = 0;
  m_rowCount = 0;
  m_levelCount = 0;

}

OdDbImpAssocArrayCommonParameters::~OdDbImpAssocArrayCommonParameters()
{
}

OdResult OdDbImpAssocArrayCommonParameters::dwgInFields(OdDbDwgFiler* pFiler)
{
 OdResult res = OdDbImpAssocArrayParameters::dwgInFields(pFiler);
 if (res != eOk)
   return res;

 if (pFiler->filerType() == OdDbDwgFiler::kIdXlateFiler
   || pFiler->filerType() == OdDbDwgFiler::kWblockCloneFiler
   || pFiler->filerType() == OdDbDwgFiler::kDeepCloneFiler
   || pFiler->filerType() == OdDbDwgFiler::kUndoFiler)
 {
   m_itemCount = pFiler->rdInt32();
   m_rowCount = pFiler->rdInt32();
   m_levelCount = pFiler->rdInt32();
 }

 return eOk;
}

void OdDbImpAssocArrayCommonParameters::dwgOutFields(OdDbDwgFiler* pFiler) const
{
 OdDbImpAssocArrayParameters::dwgOutFields(pFiler);
 
 if (pFiler->filerType() == OdDbDwgFiler::kIdXlateFiler
   || pFiler->filerType() == OdDbDwgFiler::kWblockCloneFiler
   || pFiler->filerType() == OdDbDwgFiler::kDeepCloneFiler
   || pFiler->filerType() == OdDbDwgFiler::kUndoFiler)
 {
   pFiler->wrInt32( m_itemCount );
   pFiler->wrInt32( m_rowCount );
   pFiler->wrInt32( m_levelCount );
 }
}

// OdResult OdDbImpAssocArrayCommonParameters::dxfInFields(OdDbDxfFiler* pFiler)
// {
//  OdResult res = OdDbImpAssocArrayParameters::dxfInFields(pFiler);
//  if (res != eOk)
//    return res;
// 
//  return eOk;
// }
//
// void OdDbImpAssocArrayCommonParameters::dxfOutFields(OdDbDxfFiler* pFiler) const
// {
//  OdDbImpAssocArrayParameters::dxfOutFields(pFiler);
//  ODA_FAIL_ONCE(); // TODO
//  throw OdError(eNotImplementedYet);
// }


void OdDbImpAssocArrayCommonParameters::composeForLoad( OdDbAssocArrayParameters *pAssocArrayParam, 
                                                        OdDb::SaveType format, 
                                                        OdDb::DwgVersion version )
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;

  OdString expression, evaluatorId;

  if ( eOk == getValueParam(OD_T("Items"), *value.get(), expression, evaluatorId, unitType) )
    m_itemCount = value->getInt32();
  else
    ODA_ASSERT_ONCE( !"ArrayCommonParameters -> composeForLoad -> items" );

  if ( eOk == getValueParam(OD_T("Rows"), *value.get(), expression, evaluatorId, unitType) )
    m_rowCount = value->getInt32();
  else
    ODA_ASSERT_ONCE( !"ArrayCommonParameters -> composeForLoad -> rows" );

  if ( eOk == getValueParam(OD_T("Levels"), *value.get(), expression, evaluatorId, unitType) )
    m_levelCount = value->getInt32();
  else
    ODA_ASSERT_ONCE( !"ArrayCommonParameters -> composeForLoad -> levels" );
}

void OdDbImpAssocArrayCommonParameters::setItemCount( int nItem )
{
  int cnt = m_itemCount;
  m_itemCount = nItem;

  if (nItem < cnt)
  {
    DecrementItemCount( nItem, cnt, 0, m_rowCount, 0, m_levelCount);    
  }
  else if (nItem != cnt)
  {
    IncrementItemCount( cnt, nItem, 0, m_rowCount, 0, m_levelCount);
  }
}

void OdDbImpAssocArrayCommonParameters::setRowCount( int nRows )
{
  int cnt = m_rowCount;
  m_rowCount = nRows;

  if (nRows < cnt)
  {
    DecrementItemCount( 0, m_itemCount, nRows, cnt, 0, m_levelCount);    
  }
  else if (nRows != cnt)
  {
    IncrementItemCount( 0, m_itemCount, cnt, nRows, 0, m_levelCount);
  }
}

void OdDbImpAssocArrayCommonParameters::setLevelCount( int nLevels )
{
  int cnt = m_levelCount;
  m_levelCount = nLevels;

  if (nLevels < cnt)
  {
    DecrementItemCount( 0, m_itemCount, 0, m_rowCount, nLevels, cnt);    
  }
  else if (nLevels != cnt)
  {
    IncrementItemCount( 0, m_itemCount, 0, m_rowCount, cnt, nLevels);
  }
}

void OdDbImpAssocArrayCommonParameters::IncrementItemCount( int nItemStart,  int nItemEnd,
                                                            int nRowStart,   int nRowEnd,
                                                            int nLevelStart, int nLevelEnd )
{
  for (int itm = nItemStart; itm < nItemEnd; itm++)
  {
    for (int row = nRowStart; row < nRowEnd; row++) 
    {
      for (int lvl = nLevelStart; lvl < nLevelEnd; lvl++)
      {
        OdDbItemLocator locator(itm, row, lvl);
        OdGeMatrix3d defmat = CalculateDefaultMatrix(locator);
        OdDbAssocArrayItemPtr pItem = OdDbAssocArrayItem::createObject( locator, defmat );  

        m_items.insertAt( lvl + row*m_levelCount + itm*m_levelCount*m_rowCount,
                          pItem );
      }
    }
  }
}

void OdDbImpAssocArrayCommonParameters::DecrementItemCount( int nItemStart, int nItemEnd,
                                                            int nRowStart, int nRowEnd,
                                                            int nLevelStart, int nLevelEnd )
{
  for ( int itm = nItemEnd -1; itm >= nItemStart; itm-- )
  {
    for ( int row = nRowEnd-1 ; row >= nRowStart ; row-- )
    {
      for ( int lvl = nLevelEnd-1; lvl >= nLevelStart ; lvl-- )
      {
        int idx = lvl + row*nLevelEnd + itm*nLevelEnd*nRowEnd;

        OdDbEntityPtr pEnt = m_items[idx]->entityProvider().openObject( OdDb::kForWrite );
        if ( !pEnt.isNull() )
          pEnt->erase();

        m_items.removeAt( idx );
      }
    }
  }
}

OdResult OdDbImpAssocArrayCommonParameters::setLevelCount( int nLevels, 
                                                           const OdString& expression, 
                                                           const OdString& evaluatorId, 
                                                           OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kUnitless; 
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( nLevels ) ;
  return setValueParam(L"Levels", *value.get() , expression, evaluatorId, pErrorMessage, &unitType );
}

OdResult OdDbImpAssocArrayCommonParameters::setRowCount( int nRows, 
                                                         const OdString& expression, 
                                                         const OdString& evaluatorId, 
                                                         OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kUnitless; 
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( nRows ) ;
  return setValueParam(L"Rows", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

int OdDbImpAssocArrayCommonParameters::levelCount( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init(int());
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"Levels", *value.get(), expression, evaluatorId, unitType);
  return value->getInt32();
}

double OdDbImpAssocArrayCommonParameters::levelSpacing( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  getValueParam(L"LevelSpacing", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

int OdDbImpAssocArrayCommonParameters::rowCount( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"Rows", *value.get(), expression, evaluatorId, unitType);
  return value->getInt32();
}

double OdDbImpAssocArrayCommonParameters::rowElevation( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  getValueParam(L"RowElevation", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpAssocArrayCommonParameters::rowSpacing( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  getValueParam(L"RowSpacing", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

OdResult OdDbImpAssocArrayCommonParameters::setLevelSpacing( double offset, 
                                                             const OdString& expression, 
                                                             const OdString& evaluatorId,
                                                             OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kDistance; 
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( offset ) ;
  return setValueParam(L"LevelSpacing", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayCommonParameters::setRowElevation( double elevation, 
                                                             const OdString& expression, 
                                                             const OdString& evaluatorId,
                                                             OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kDistance; 
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( elevation ) ;
  return setValueParam(L"RowElevation", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayCommonParameters::setRowSpacing( double offset, 
                                                           const OdString& expression,
                                                           const OdString& evaluatorId,
                                                           OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kDistance; 
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( offset ) ;
  return setValueParam(L"RowSpacing", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayCommonParameters::setBasePlane( const OdDbVertexRef& basePoint, 
                                                          const OdGeVector3d& normal, 
                                                          const OdDbFaceRef* pFace /*= NULL*/ )
{
  OdResult res;
  OdDbObjectId paramId;

  OdDbGeomRefPtr pGeomRef = OdDbVertexRef::createObject( OdGePoint3d( normal.x, normal.y, normal.z ) );  
  res = setGeomParam( OD_T("BaseNormal"), pGeomRef, paramId );
  if ( eOk != res )
    return res;

  return setGeomParam( OD_T("Origin"), &basePoint, paramId );
}

OdResult OdDbImpAssocArrayCommonParameters::getBasePlane( OdDbVertexRef& basePoint, OdGeVector3d& normal, OdDbFaceRef* pFace /*= NULL*/ ) const
{
  OdResult res;
  OdDbObjectId paramId;

  OdDbGeomRefPtr pGeomRef;  
  res = getGeomParam( OD_T("BaseNormal"), pGeomRef, paramId );
  if ( eOk != res )
    return res;
  normal = OdDbVertexRef::cast( pGeomRef )->point() - OdGePoint3d( 0, 0, 0 );

  res = getGeomParam( OD_T("Origin"), pGeomRef, paramId );
  if ( eOk != res )
    return res;

  basePoint = *OdDbVertexRef::cast( pGeomRef );

  return eOk;
}

void OdDbImpAssocArrayCommonParameters::CalculateAxes()
{
  m_vXaxis = OdGeVector3d(1, 0, 0);
  m_vYaxis = OdGeVector3d(0, 1, 0);
  m_vZaxis = OdGeVector3d(0, 0, 1);
  if ( m_BaseNormal.isCodirectionalTo( OdGeVector3d(0, 0, -1) ) )
  {
    m_vXaxis = OdGeVector3d( -1,  0,  0 );
    m_vZaxis = OdGeVector3d(  0,  0, -1 );
    m_vYaxis = OdGeVector3d(  0,  1,  0 );
  }
  else if ( !m_BaseNormal.isCodirectionalTo( OdGeVector3d(0, 0, 1) ) )
  {
    m_vXaxis = m_vZaxis.crossProduct( m_BaseNormal );
    m_vXaxis = m_vXaxis.normalize();
    m_vZaxis = m_BaseNormal;
    m_vZaxis = m_vZaxis.normalize();
    m_vYaxis = m_vZaxis.crossProduct( m_vXaxis );
    m_vYaxis = m_vYaxis.normalize();
  }
}

OdGeMatrix3d OdDbImpAssocArrayCommonParameters::GetStartMatrix() const
{
  return OdGeMatrix3d().setCoordSystem( OdGePoint3d(0, 0, 0), vXaxis(), vYaxis(), vZaxis() );
}

OdDbItemLocator OdDbImpAssocArrayCommonParameters::getCanonicalForm( const OdDbItemLocator& locator ) const
{
  int itemIndex = locator[OdDbItemLocator::kItemIndex],
      rowIndex = locator[OdDbItemLocator::kRowIndex],
      levelIndex = locator[OdDbItemLocator::kLevelIndex];

  if ( itemIndex < 0 )
      itemIndex += m_itemCount;
  if ( rowIndex < 0 )
      rowIndex += m_rowCount;
  if ( levelIndex < 0 )
      levelIndex += m_levelCount;

  return OdDbItemLocator( itemIndex, rowIndex, levelIndex );
}

OdResult OdDbImpAssocArrayCommonParameters::GetItemByLocator(const OdDbItemLocator &locator, OdDbAssocArrayItemPtr &pItem) const
{

  if ( locator[OdDbItemLocator::kLevelIndex] >= m_levelCount
    || locator[OdDbItemLocator::kRowIndex] >= m_rowCount
    || locator[OdDbItemLocator::kItemIndex] >= m_itemCount
    || locator[OdDbItemLocator::kLevelIndex] < 0
    || locator[OdDbItemLocator::kRowIndex] < 0
    || locator[OdDbItemLocator::kItemIndex] < 0 )
   return eOutOfRange;

  pItem = m_items[ locator[OdDbItemLocator::kLevelIndex] 
                 + locator[OdDbItemLocator::kRowIndex] * m_levelCount
                 + locator[OdDbItemLocator::kItemIndex] * m_levelCount * m_rowCount];
  return eOk;
}

//////////////////////////////////////////////////////////////////////////

OdDbImpAssocArrayPathParameters::OdDbImpAssocArrayPathParameters(double itemSpacing, 
                                                                 double rowSpacing, 
                                                                 double levelSpacing,
                                                                 int itemCount, 
                                                                 int rowCount, 
                                                                 int levelCount, 
                                                                 double rowElevation) 
{
  OdString expression, evaluatorID, errorMSG;
  setItemSpacing( itemSpacing, expression, evaluatorID, errorMSG );
  setRowSpacing( rowSpacing, expression, evaluatorID, errorMSG );
  setLevelSpacing( levelSpacing, expression, evaluatorID, errorMSG );
  setItemCount( itemCount, expression, evaluatorID, errorMSG );
  setRowCount( rowCount, expression, evaluatorID, errorMSG );
  setLevelCount( levelCount, expression, evaluatorID, errorMSG );
  setRowElevation( rowElevation, expression, evaluatorID, errorMSG );

}

OdDbImpAssocArrayPathParameters::~OdDbImpAssocArrayPathParameters()
{
}

OdResult OdDbImpAssocArrayPathParameters::getItems( OdArray<OdDbAssocArrayItemPtr>& items )
{ 
  OdString expression, evaluatorID;
  int itmcnt, rowcnt, lvlcnt;

  m_Mhetod = method();
  m_itemSpacing = itemSpacing( expression, evaluatorID );
  m_rowProfile = path();

  
  itmcnt = itemCount( expression, evaluatorID );  
  rowcnt = rowCount( expression, evaluatorID );
  lvlcnt = levelCount( expression, evaluatorID );  
  m_rowSpacing = rowSpacing( expression, evaluatorID );
  m_levelSpacing = levelSpacing( expression, evaluatorID );
  m_rowElevation = rowElevation( expression, evaluatorID );   
  m_AlignItems = alignItems();
  m_startOffset = startOffset( expression, evaluatorID );
  m_endOffset = endOffset( expression, evaluatorID ); 
  m_pathDirection = pathDirection();

  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  OdString aitem(OD_T("MaintainZ")), empty;
  if ( eOk == getValueParam(aitem, *value.get(), empty, empty, unitType) )
    m_maintainZ = ! OdZero( value->getDouble() );
  else
    m_maintainZ = true;

  aitem = OD_T( "TangentOrientationAngle" ) ;
  if ( eOk == getValueParam( aitem, *value.get(), empty, empty, unitType ) )
    m_TangentOrientationAngle = value->getDouble();
  else
    m_TangentOrientationAngle = 0.0;

  value = OdDbEvalVariant::init( int() );
  aitem = OD_T( "Orientation" );
  if ( eOk == getValueParam( aitem, *value.get(), empty, empty, unitType ) )
    m_Orientation = value->getInt32();
  else
    m_Orientation = 0;

  OdDbGeomRefPtr pBaseNormalGeom;  
  OdDbObjectId objID;
  getGeomParam( OD_T("BaseNormal"), pBaseNormalGeom, objID );
  OdDbVertexRefPtr pBaseNormal = pBaseNormalGeom;
  m_BaseNormal = pBaseNormal->point() - OdGePoint3d(0,0,0);

  CalculateAxes();

  //intervalCurve
  OdSharedPtr<OdGeCurve3d> pCurve( static_cast<OdGeCurve3d *>(m_rowProfile.curve()->copy() ) );
  if ( ! m_pathDirection )
    pCurve->reverseParam();
  pCurve->getInterval(intervalCurve);

  if ( OdGreater( m_startOffset, intervalCurve.lowerBound() )
    && OdLess( m_startOffset, intervalCurve.upperBound() ) )
    intervalCurve.setLower( m_startOffset );

  if ( OdLess( m_endOffset, intervalCurve.upperBound() )
    && OdGreater( m_endOffset, intervalCurve.lowerBound() ) )
    intervalCurve.setUpper( m_endOffset );
  //

  //start point
  OdGeVector3dArray deriv;
  OdGePoint3d startPoint = pCurve->evalPoint( intervalCurve.lowerBound(), 1, deriv );
  tangentAtStartPoint = deriv.first();
  tangentAtStartPoint.normalize();

  perpAtStartPoint = vZaxis().crossProduct( tangentAtStartPoint ).normalize();
  if ( perpAtStartPoint.isZeroLength() )
    perpAtStartPoint = vYaxis(); //TODO


  OdDbImpAssocArrayCommonParameters::setItemCount( itmcnt );
  OdDbImpAssocArrayCommonParameters::setRowCount( rowcnt );
  OdDbImpAssocArrayCommonParameters::setLevelCount( lvlcnt );
  
  OdArray<OdDbAssocArrayItemPtr>::iterator itItem;
  for ( itItem = m_items.begin(); itItem != m_items.end(); itItem++ )
  {
    if ( (*itItem)->isErased() )
      continue;

    (*itItem)->setDefaultTransform( CalculateDefaultMatrix( (*itItem)->locator() ) );
  }

  items = m_items;

  return eOk;
}

OdGeMatrix3d OdDbImpAssocArrayPathParameters::CalculateDefaultMatrix( const OdDbItemLocator &locator )
{
  OdSharedPtr<OdGeCurve3d> pCurve( static_cast<OdGeCurve3d *>(m_rowProfile.curve()->copy() ) );
  double param;

  if ( ! m_pathDirection )
    pCurve->reverseParam();

  switch (m_Mhetod)
  {
  case OdDbAssocArrayPathParameters::kDivide:
    param = ( intervalCurve.upperBound() - intervalCurve.lowerBound() ) 
          / (m_itemCount == 1 || pCurve->isClosed() ? m_itemCount : m_itemCount - 1 ) * locator[OdDbItemLocator::kItemIndex] + intervalCurve.lowerBound();
    break;

  case OdDbAssocArrayPathParameters::kMeasure:
    param = pCurve->paramAtLength( intervalCurve.lowerBound(), 
                                   m_itemSpacing * locator[OdDbItemLocator::kItemIndex] );
    break;
  }
      
  //tangentAtCurrentPoint
  OdGeVector3dArray deriv;
  OdGePoint3d pointOnCurve = pCurve->evalPoint( param, 1, deriv );

  OdGeVector3d tangentAtCurrentPoint = deriv.first();
  tangentAtCurrentPoint.normalize();
  //

  //perpToCurve
  OdGeVector3d perpToCurve = vZaxis().crossProduct( tangentAtCurrentPoint ).normalize();
  if ( perpToCurve.isZeroLength() )
    perpToCurve = vYaxis(); //TODO


  double y = locator[OdDbItemLocator::kRowIndex] * m_rowSpacing;   

  double z = locator[OdDbItemLocator::kLevelIndex] * m_levelSpacing
    + locator[OdDbItemLocator::kRowIndex]   * m_rowElevation;

  OdGeMatrix3d mat = GetStartMatrix();


  mat = OdGeMatrix3d().translation( OdGeVector3d(pointOnCurve.x, pointOnCurve.y, pointOnCurve.z)
                                  + y*perpToCurve
                                  + z*vZaxis() )
      * mat;  

  if ( !m_maintainZ )
  {
    OdGeVector3d zDirect;
    if ( m_AlignItems )
    {
      zDirect = tangentAtCurrentPoint.crossProduct( perpToCurve ).normalize();
      double angZandZdir = OdGeVector3d( 0, 0, 1 ).angleTo( zDirect, perpToCurve );
      mat *= OdGeMatrix3d().setToRotation( angZandZdir, perpToCurve );
    }
    else
    {
      zDirect = tangentAtStartPoint.crossProduct( perpAtStartPoint ).normalize();
      double angZandZdir = OdGeVector3d( 0, 0, 1 ).angleTo( zDirect, perpAtStartPoint );
      mat *= OdGeMatrix3d().setToRotation( angZandZdir, perpAtStartPoint );
    }
  }

  switch ( m_Orientation )
  {
    case 1:
    {
      OdGeVector3d newTang = vYaxis();
      newTang.rotateBy( m_TangentOrientationAngle, OdGeVector3d( 0, 0, 1 ) );
      double ang = newTang.angleTo( perpAtStartPoint, vZaxis() );
      mat *= OdGeMatrix3d().setToRotation( ang, OdGeVector3d( 0, 0, 1 ) );
    }
      //NO BREAK

    case 0:
      if ( m_AlignItems )
      {
        mat *= OdGeMatrix3d().rotation( perpAtStartPoint.angleTo( perpToCurve,vZaxis()),
        OdGeVector3d(0, 0, 1)
        );
      }
      break;

    case 2:
    {
      OdGeVector3d x, y, z;
      
      if ( m_AlignItems )
      {
        x = vZaxis().crossProduct( tangentAtCurrentPoint ).normalize();
        y = vZaxis();
        z = perpToCurve.crossProduct( vZaxis() ).normalize();
      }
      else
      {
        x = vZaxis().crossProduct( tangentAtStartPoint ).normalize();
        y = vZaxis();
        z = perpAtStartPoint.crossProduct( vZaxis() ).normalize();        
      }
      mat *= OdGeMatrix3d().setCoordSystem( OdGePoint3d::kOrigin, x, y, z );
    }
      break;
  }

  return mat;
}

int OdDbImpAssocArrayPathParameters::itemCount( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(L"Items", *value.get(), expression, evaluatorId, unitType);
  return value->getInt32();
}

double OdDbImpAssocArrayPathParameters::itemSpacing( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kAngle;
  getValueParam(L"ItemSpacing", *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

OdResult OdDbImpAssocArrayPathParameters::setItemCount( int nItems, 
                                                        const OdString& expression, 
                                                        const OdString& evaluatorId, 
                                                        OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kUnitless; 
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( nItems ) ;
  return setValueParam(L"Items", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayPathParameters::setItemSpacing( double offset, 
                                                          const OdString& expression, 
                                                          const OdString& evaluatorId, 
                                                          OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kDistance; 
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( offset ) ;
  return setValueParam(L"ItemSpacing", *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdDbEdgeRef OdDbImpAssocArrayPathParameters::path() const
{
  OdDbGeomRefPtr rowProfile;  
  OdDbObjectId objID;
  getGeomParam( L"RowProfile", rowProfile, objID );
  OdDbEdgeRefPtr pPath = rowProfile;
  return *pPath.get() ;   
}

int OdDbImpAssocArrayPathParameters::method() const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdString method(OD_T("Method"));
  OdString empty;

  if ( eNotInGroup == getValueParam( method, *value.get(), empty, empty, unitType) )
  {
    return OdDbAssocArrayPathParameters::kMeasure;
  }
  else
  {
    return value->getInt32();
  }  
}

bool OdDbImpAssocArrayPathParameters::alignItems() const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdString aitem(OD_T("AlignItems")), empty;
  if ( eNotInGroup == getValueParam(aitem, *value.get(), empty, empty, unitType) )
  {
    return true;
  }
  else
  {
    return !!value->getInt32();
  }
}

double OdDbImpAssocArrayPathParameters::startOffset(OdString& expression, OdString& evaluatorId) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  OdString aitem(OD_T("StartOffset")), empty;
  if ( eNotInGroup == getValueParam(aitem, *value.get(), empty, empty, unitType) )
  {
    return 0.0;
  }
  else
  {
    return value->getDouble();
  }
}


double OdDbImpAssocArrayPathParameters::endOffset( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  OdString aitem(OD_T("EndOffset")), empty;
  if ( eNotInGroup == getValueParam(aitem, *value.get(), empty, empty, unitType) )
  {
    return 0.0;
  }
  else
  {
    return value->getDouble();
  }
}


bool OdDbImpAssocArrayPathParameters::pathDirection() const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdString aitem(OD_T("PathDirection")), empty;
  if ( eNotInGroup == getValueParam(aitem, *value.get(), empty, empty, unitType) )
  {
    return true;
  }
  else
  {
    return !!value->getInt32();
  }
}


OdResult OdDbImpAssocArrayPathParameters::setPath( const OdDbEdgeRef& edgeRef )
{
  OdDbGeomRefPtr pGeomRef = OdDbEdgeRef::createObject();
  OdDbEdgeRefPtr pEdgeRef = OdDbEdgeRef::cast( pGeomRef );
  pEdgeRef->copyFrom( &edgeRef );
  OdDbObjectId paramId;
  return setGeomParam( OD_T("RowProfile"), pGeomRef, paramId );
}

OdResult OdDbImpAssocArrayPathParameters::setMethod( int method )
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init( method ) ;
  const OdString methodStr(OD_T("Method")), empty;
  OdString errMsg;
  return setValueParam(methodStr, *value.get(), OdString::kEmpty, empty, errMsg, &unitType);
}

OdResult OdDbImpAssocArrayPathParameters::setAlignItems( bool bAlignItems )
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init( int(bAlignItems) ) ;
  const OdString aitem(OD_T("AlignItems")), empty;
  OdString errMsg;
  return setValueParam(aitem, *value.get(), empty, empty, errMsg, &unitType);
}

OdResult OdDbImpAssocArrayPathParameters::setStartOffset( double startOffset, 
                                                          const OdString& expression, 
                                                          const OdString& evaluatorId, 
                                                          OdString* pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kDistance;
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init( startOffset ) ;
  const OdString sOffset(OD_T("StartOffset")), empty;
  OdString errMsg;
  return setValueParam( sOffset, *value.get(), empty, empty, errMsg, &unitType );
}

OdResult OdDbImpAssocArrayPathParameters::setEndOffset( double startOffset, 
                                                        const OdString& expression, 
                                                        const OdString& evaluatorId, 
                                                        OdString* pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kDistance;
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init( startOffset ) ;
  const OdString eOffset(OD_T("EndOffset")), empty;
  OdString errMsg;
  return setValueParam( eOffset, *value.get(), empty, empty, errMsg, &unitType );
}

OdResult OdDbImpAssocArrayPathParameters::setPathDirection( bool bAlongParams )
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init( int(bAlongParams) ) ;
  const OdString PathDirection(OD_T("PathDirection")), empty;
  OdString errMsg;
  return setValueParam( PathDirection, *value.get(), empty, empty, errMsg, &unitType );
}

//OdResult OdDbImpAssocArrayPathParameters::dwgInFields(OdDbDwgFiler* pFiler)
//{
//  OdResult res = OdDbImpAssocArrayCommonParameters::dwgInFields(pFiler);
//  if (res != eOk)
//    return res;
//
//  ODA_FAIL_ONCE(); // TODO
//  return eMakeMeProxy;
//}
//
//void OdDbImpAssocArrayPathParameters::dwgOutFields(OdDbDwgFiler* pFiler) const
//{
//  OdDbImpAssocArrayCommonParameters::dwgOutFields(pFiler);
//  ODA_FAIL_ONCE(); // TODO
//  throw OdError(eNotImplementedYet);
//}
//
//OdResult OdDbImpAssocArrayPathParameters::dxfInFields(OdDbDxfFiler* pFiler)
//{
//  OdResult res = OdDbImpAssocArrayCommonParameters::dxfInFields(pFiler);
//  if (res != eOk)
//    return res;
//  ODA_FAIL_ONCE(); // TODO
//  return eMakeMeProxy;
//}
//
//void OdDbImpAssocArrayPathParameters::dxfOutFields(OdDbDxfFiler* pFiler) const
//{
//  OdDbImpAssocArrayCommonParameters::dxfOutFields(pFiler);
//  ODA_FAIL_ONCE(); // TODO
//  throw OdError(eNotImplementedYet);
//}

//////////////////////////////////////////////////////////////////////////

OdDbImpAssocArrayPolarParameters::OdDbImpAssocArrayPolarParameters(double angle, 
                                                                   double rowSpacing, 
                                                                   double levelSpacing, 
                                                                   int itemCount, 
                                                                   int rowCount,
                                                                   int levelCount, 
                                                                   double rowElevation) 
{
  OdString expression, evaluatorID, errorMSG;
  setAngleBetweenItems( angle, expression, evaluatorID, errorMSG );
  setRowSpacing( rowSpacing, expression, evaluatorID, errorMSG );
  setLevelSpacing( levelSpacing, expression, evaluatorID, errorMSG );
  setItemCount( itemCount, expression, evaluatorID, errorMSG );
  setRowCount( rowCount, expression, evaluatorID, errorMSG );
  setLevelCount( levelCount, expression, evaluatorID, errorMSG );
  setRowElevation( rowElevation, expression, evaluatorID, errorMSG );
  setFillAngle( Oda2PI, expression, evaluatorID, errorMSG );
}

OdDbImpAssocArrayPolarParameters::~OdDbImpAssocArrayPolarParameters()
{
}

OdResult OdDbImpAssocArrayPolarParameters::getItems( OdArray<OdDbAssocArrayItemPtr>& items )
{
  OdString expression, evaluatorID;
  int itmcnt, rowcnt, lvlcnt;

  itmcnt = itemCount( expression, evaluatorID );
  rowcnt = rowCount( expression, evaluatorID );
  lvlcnt = levelCount( expression, evaluatorID );
  m_itemSpacing = angleBetweenItems( expression, evaluatorID );
  m_rowSpacing = rowSpacing( expression, evaluatorID );
  m_levelSpacing = levelSpacing( expression, evaluatorID );
  m_rowElevation = rowElevation( expression, evaluatorID );
  m_startAngle = startAngle( expression, evaluatorID );
  m_fillAngle = fillAngle( expression, evaluatorID );
  m_radius = radius( expression, evaluatorID );
  m_RotateItem = rotateItems();
  m_Direction = !!direction();

  OdDbGeomRefPtr pGeomRef;  
  OdDbObjectId objID;
  getGeomParam( OD_T("Origin"), pGeomRef, objID );
  OdDbVertexRefPtr pOrigin = pGeomRef;
  m_origin = pOrigin->point();
 
  OdDbGeomRefPtr pBaseNormalGeom;  
  getGeomParam( OD_T("BaseNormal"), pBaseNormalGeom, objID );
  OdDbVertexRefPtr pBaseNormal = pBaseNormalGeom;
  m_BaseNormal = pBaseNormal->point() - OdGePoint3d(0,0,0);

  CalculateAxes();

  OdDbImpAssocArrayCommonParameters::setItemCount( itmcnt );
  OdDbImpAssocArrayCommonParameters::setRowCount( rowcnt );
  OdDbImpAssocArrayCommonParameters::setLevelCount( lvlcnt );  

  OdArray<OdDbAssocArrayItemPtr>::iterator itItem;
  for ( itItem = m_items.begin(); itItem != m_items.end(); itItem++ )
  {
    if ( (*itItem)->isErased() )
      continue;

    (*itItem)->setDefaultTransform( CalculateDefaultMatrix( (*itItem)->locator() ) );
  }
 
  items = m_items;

  return eOk;
}

OdGeMatrix3d OdDbImpAssocArrayPolarParameters::CalculateDefaultMatrix( const OdDbItemLocator &locator )
{
  OdGeVector3d vX( vXaxis() );

  double AngleCurrentItem;
  
  if (m_Direction)
    AngleCurrentItem = m_startAngle + m_itemSpacing * locator[OdDbItemLocator::kItemIndex];
  else
    AngleCurrentItem = m_startAngle - m_itemSpacing * locator[OdDbItemLocator::kItemIndex];

  double r = m_radius 
           + m_rowSpacing * locator[OdDbItemLocator::kRowIndex];

  double z = locator[OdDbItemLocator::kLevelIndex] * m_levelSpacing
           + locator[OdDbItemLocator::kRowIndex] * m_rowElevation;

  OdGeVector3d vOrigin( m_origin.x, m_origin.y, m_origin.z );

  OdGeMatrix3d mat = OdGeMatrix3d().translation( 
                                      vOrigin 
                                      + r *vX.rotateBy( AngleCurrentItem, vZaxis() )
                                      + z*vZaxis() );

  if (m_RotateItem)
  {
    mat *= OdGeMatrix3d().rotation( AngleCurrentItem - m_startAngle, vZaxis(), OdGePoint3d(0,0,0) );    
  }
 
  return mat;
}

int OdDbImpAssocArrayPolarParameters::itemCount( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam( OD_T("Items"), *value.get(), expression, evaluatorId, unitType);
  return value->getInt32();
}

double OdDbImpAssocArrayPolarParameters::angleBetweenItems( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kAngle;
  getValueParam(OD_T("ItemSpacing"), *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpAssocArrayPolarParameters::fillAngle( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kAngle;
  getValueParam(OD_T("FillAngle"), *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpAssocArrayPolarParameters::startAngle( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kAngle;
  getValueParam(OD_T("StartAngle"), *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpAssocArrayPolarParameters::radius( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  getValueParam(OD_T("Radius"), *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

bool OdDbImpAssocArrayPolarParameters::rotateItems() const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdString aitem(OD_T("AlignItems")), empty;
  if ( eNotInGroup == getValueParam(aitem, *value.get(), empty, empty, unitType) )
  {
    return true;
  }
  else
  {
    return !!value->getInt32();
  }
}

int OdDbImpAssocArrayPolarParameters::direction() const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdString aitem(OD_T("Direction")), empty;
  if ( eNotInGroup == getValueParam(aitem, *value.get(), empty, empty, unitType) )
  {
    return OdDbAssocArrayPolarParameters::kCounterClockwise;
  }
  else
  {
    return value->getInt32();
  }
}

OdResult OdDbImpAssocArrayPolarParameters::setItemCount( int nItems, 
                                                         const OdString& expression, 
                                                         const OdString& evaluatorId, 
                                                         OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( nItems ) ;
  return setValueParam(OD_T("Items"), *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayPolarParameters::setAngleBetweenItems( double angle, 
                                                                 const OdString& expression, 
                                                                 const OdString& evaluatorId, 
                                                                 OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kAngle;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( angle ) ;
  return setValueParam(OD_T("ItemSpacing"), *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayPolarParameters::setFillAngle( double fillAngle, 
                                                         const OdString& expression, 
                                                         const OdString& evaluatorId,
                                                         OdString& pErrorMessage ) 
{
  OdValue::UnitType unitType = OdValue::kAngle;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( fillAngle ) ;
  return setValueParam(OD_T("FillAngle"), *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayPolarParameters::setStartAngle( double angle, 
                                                          const OdString& expression, 
                                                          const OdString& evaluatorId,
                                                          OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kAngle;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( angle ) ;
  return setValueParam(OD_T("StartAngle"), *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayPolarParameters::setRadius( double radius, 
                                                      const OdString& expression, 
                                                      const OdString& evaluatorId,
                                                      OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kDistance;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( radius ) ;
  return setValueParam(OD_T("Radius"), *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayPolarParameters::setRotateItems( bool bRotateItems )
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init( int(bRotateItems) ) ;
  const OdString aitem(OD_T("AlignItems")), empty;
  OdString errMsg;
  return setValueParam(aitem, *value.get(), empty, empty, errMsg, &unitType);
}

OdResult OdDbImpAssocArrayPolarParameters::setDirection( int direction )
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  const OdDbEvalVariantPtr value = OdDbEvalVariant::init( int(direction) ) ;
  const OdString aitem(OD_T("Direction")), empty;
  OdString errMsg;
  return setValueParam(aitem, *value.get(), empty, empty, errMsg, &unitType);
}



//OdResult OdDbImpAssocArrayPolarParameters::dwgInFields(OdDbDwgFiler* pFiler)
//{
//  OdResult res = OdDbImpAssocArrayCommonParameters::dwgInFields(pFiler);
//  if (res != eOk)
//    return res;
//
//  ODA_FAIL_ONCE(); // TODO
//  return eMakeMeProxy;
//}
//
//void OdDbImpAssocArrayPolarParameters::dwgOutFields(OdDbDwgFiler* pFiler) const
//{
//  OdDbImpAssocArrayCommonParameters::dwgOutFields(pFiler);
//  ODA_FAIL_ONCE(); // TODO
//  throw OdError(eNotImplementedYet);
//}
//
//OdResult OdDbImpAssocArrayPolarParameters::dxfInFields(OdDbDxfFiler* pFiler)
//{
//  OdResult res = OdDbImpAssocArrayCommonParameters::dxfInFields(pFiler);
//  if (res != eOk)
//    return res;
//  ODA_FAIL_ONCE(); // TODO
//  return eMakeMeProxy;
//}
//
//void OdDbImpAssocArrayPolarParameters::dxfOutFields(OdDbDxfFiler* pFiler) const
//{
//  OdDbImpAssocArrayCommonParameters::dxfOutFields(pFiler);
//  ODA_FAIL_ONCE(); // TODO
//  throw OdError(eNotImplementedYet);
//}

//////////////////////////////////////////////////////////////////////////

OdDbImpAssocArrayRectangularParameters::OdDbImpAssocArrayRectangularParameters(double columnSpacing, 
                                                                               double rowSpacing, 
                                                                               double levelSpacing, 
                                                                               int columnCount, 
                                                                               int rowCount, 
                                                                               int levelCount, 
                                                                               double rowElevation,
                                                                               double axesAngle) 
{
  OdString expression, evaluatorID, errorMSG;
  setColumnSpacing( columnSpacing, expression, evaluatorID, errorMSG );
  setRowSpacing( rowSpacing, expression, evaluatorID, errorMSG );
  setLevelSpacing( levelSpacing, expression, evaluatorID, errorMSG );
  setColumnCount( columnCount, expression, evaluatorID, errorMSG );
  setRowCount( rowCount, expression, evaluatorID, errorMSG );
  setLevelCount( levelCount, expression, evaluatorID, errorMSG );
  setRowElevation( rowElevation, expression, evaluatorID, errorMSG );
  setAxesAngle( axesAngle, expression, evaluatorID, errorMSG );

  
}

OdDbImpAssocArrayRectangularParameters::~OdDbImpAssocArrayRectangularParameters()
{
}

OdResult OdDbImpAssocArrayRectangularParameters::getItems( OdArray<OdDbAssocArrayItemPtr>& items )
{ 
  OdString expression, evaluatorID;
  int itmcnt, rowcnt, lvlcnt;

  itmcnt = ColumnCount( expression, evaluatorID );
  rowcnt = rowCount( expression, evaluatorID );
  lvlcnt = levelCount( expression, evaluatorID );
  m_itemSpacing = ColumnSpacing( expression, evaluatorID );
  m_rowSpacing = rowSpacing( expression, evaluatorID );
  m_levelSpacing = levelSpacing( expression, evaluatorID );
  m_rowElevation = rowElevation( expression, evaluatorID );
  m_axesAngle = OdaPI/180.0*AxesAngle( expression, evaluatorID );
  m_vRowProfile = axisDirection();

  OdDbGeomRefPtr pBaseNormalGeom;  
  OdDbObjectId objID;
  getGeomParam( OD_T("BaseNormal"), pBaseNormalGeom, objID );
  OdDbVertexRefPtr pBaseNormal = pBaseNormalGeom;
  m_BaseNormal = pBaseNormal->point() - OdGePoint3d(0,0,0);

  CalculateAxes();

  OdDbImpAssocArrayCommonParameters::setItemCount( itmcnt );
  OdDbImpAssocArrayCommonParameters::setRowCount( rowcnt );
  OdDbImpAssocArrayCommonParameters::setLevelCount( lvlcnt );

  OdArray<OdDbAssocArrayItemPtr>::iterator itItem;
  for ( itItem = m_items.begin(); itItem != m_items.end(); itItem++ )
  {
    if ( (*itItem)->isErased() )
      continue;

    (*itItem)->setDefaultTransform( CalculateDefaultMatrix( (*itItem)->locator() ) );
  }

  items = m_items;

  return eOk;
}

OdGeMatrix3d OdDbImpAssocArrayRectangularParameters::CalculateDefaultMatrix( const OdDbItemLocator &locator )
{
  double x = locator[OdDbItemLocator::kItemIndex]
           * m_itemSpacing;

  double y = locator[OdDbItemLocator::kRowIndex]
           * m_rowSpacing;
  
  double z = locator[OdDbItemLocator::kLevelIndex]
           * m_levelSpacing
           + locator[OdDbItemLocator::kRowIndex]
           * m_rowElevation; 

  OdGeVector3d vY = vYaxis();
  vY.rotateBy( m_axesAngle - OdaPI2,  vZaxis() );

  OdGeVector3d vPos = x*vXaxis() + y*vY + z*vZaxis();

  double angleXaxisRowProfile = vXaxis().angleTo( m_vRowProfile, OdGeVector3d( 0, 0, 1 ) );

  vPos.rotateBy( angleXaxisRowProfile, vZaxis() );

  return OdGeMatrix3d().translation( vPos );
}

int OdDbImpAssocArrayRectangularParameters::ColumnCount( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( int() );
  OdValue::UnitType unitType = OdValue::kUnitless;
  getValueParam(OD_T("Items"), *value.get(), expression, evaluatorId, unitType);
  return value->getInt32();
}

double OdDbImpAssocArrayRectangularParameters::ColumnSpacing( OdString& expression, OdString& evaluatorId) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  getValueParam(OD_T("ItemSpacing"), *value.get(), expression, evaluatorId, unitType);
  return value->getDouble();
}

double OdDbImpAssocArrayRectangularParameters::AxesAngle( OdString& expression, OdString& evaluatorId ) const
{
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( double() );
  OdValue::UnitType unitType = OdValue::kDistance;
  getValueParam(OD_T("AxesAngle"), *value.get(), expression, evaluatorId, unitType);
  return value->getDouble()*180.0/OdaPI;
}

OdGeVector3d OdDbImpAssocArrayRectangularParameters::axisDirection( OdGeVector3d* pYAxis ) const
{
  OdDbGeomRefPtr rowProfile;
  OdDbObjectId objID;
  if ( eOk == getGeomParam( OD_T("RowProfile"), rowProfile, objID ) )
  {
    OdDbEdgeRefPtr pPath = rowProfile;
    ODA_ASSERT_ONCE( 19 == pPath->curve()->type() );
    const OdGeLine3d* line = static_cast<const OdGeLine3d*>(pPath->curve());
    return line->direction();
  }
  else
  {
    return OdGeVector3d(1, 0, 0);
  }
}

OdResult OdDbImpAssocArrayRectangularParameters::setColumnCount( int nColumns, const OdString& expression, /* = OdString::kEmpty */ const OdString& evaluatorId, /* = OdString::kEmpty */ OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kUnitless;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( nColumns ) ;
  return setValueParam(OD_T("Items"), *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayRectangularParameters::setColumnSpacing( double offset, const OdString& expression, /* = OdString::kEmpty */ const OdString& evaluatorId, /* = OdString::kEmpty */ OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kDistance;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( offset ) ;
  return setValueParam(OD_T("ItemSpacing"), *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayRectangularParameters::setAxesAngle( double axesAngle, const OdString& expression, /* = OdString::kEmpty */ const OdString& evaluatorId, /* = OdString::kEmpty */ OdString& pErrorMessage )
{
  OdValue::UnitType unitType = OdValue::kAngle;
  OdDbEvalVariantPtr value = OdDbEvalVariant::init( axesAngle*OdaPI/180.0);
  return setValueParam(OD_T("AxesAngle"), *value.get(), expression, evaluatorId, pErrorMessage, &unitType);
}

OdResult OdDbImpAssocArrayRectangularParameters::setAxisDirection( const OdGeVector3d& xAxis )
{
  OdGeLine3d line( OdGePoint3d(0,0,0), xAxis );
  OdDbGeomRefPtr pGeomRef = OdDbEdgeRef::createObject( &line );
  OdDbObjectId paramId;
  return setGeomParam( OD_T("RowProfile"), pGeomRef, paramId );
}



//OdResult OdDbImpAssocArrayRectangularParameters::dwgInFields(OdDbDwgFiler* pFiler)
//{
//  OdResult res = OdDbImpAssocArrayCommonParameters::dwgInFields(pFiler);
//  if (res != eOk)
//    return res;
//
//  ODA_FAIL_ONCE(); // TODO
//  return eMakeMeProxy;
//}
//
//void OdDbImpAssocArrayRectangularParameters::dwgOutFields(OdDbDwgFiler* pFiler) const
//{
//  OdDbImpAssocArrayCommonParameters::dwgOutFields(pFiler);
//  ODA_FAIL_ONCE(); // TODO
//  throw OdError(eNotImplementedYet);
//}
//
//OdResult OdDbImpAssocArrayRectangularParameters::dxfInFields(OdDbDxfFiler* pFiler)
//{
//  OdResult res = OdDbImpAssocArrayCommonParameters::dxfInFields(pFiler);
//  if (res != eOk)
//    return res;
//  ODA_FAIL_ONCE(); // TODO
//  return eMakeMeProxy;
//}
//
//void OdDbImpAssocArrayRectangularParameters::dxfOutFields(OdDbDxfFiler* pFiler) const
//{
//  OdDbImpAssocArrayCommonParameters::dxfOutFields(pFiler);
//  ODA_FAIL_ONCE(); // TODO
//  throw OdError(eNotImplementedYet);
//}

//////////////////////////////////////////////////////////////////////////



OdDbImpAssocArrayModifyParameters::~OdDbImpAssocArrayModifyParameters()
{

}

OdResult OdDbImpAssocArrayModifyParameters::getItems( OdArray<OdDbAssocArrayItemPtr>& items )
{
  OdDbAssocArrayModifyActionBody *pBody = OdDbAssocArrayModifyActionBody::cast( m_pOwner);

  OdArray<OdDbItemLocator> indices; 
  pBody->getOverridenItems( indices );

  //get master action
  OdDbObjectIdArray depIds;
  m_pOwner->getDependencies( true, true, depIds );

  OdDbAssocDependencyPtr pDep = depIds[0].safeOpenObject();
  OdDbBlockTableRecordPtr pDestBTR = OdDbBlockTableRecord::cast( pDep->dependentOnObject().openObject() );
  ODA_ASSERT_ONCE( !pDestBTR.isNull() );

  OdDbAssocActionPtr pMasterAction;
  OdDbObjectIdArray reactors = pDestBTR->getPersistentReactors();
  OdDbObjectIdArray::iterator itReact;
  for ( itReact = reactors.begin(); itReact != reactors.end(); itReact++ )
  {
    OdDbAssocDependencyPtr pMasterDep = OdDbAssocDependency::cast( itReact->openObject() );
    if ( !pMasterDep.isNull() )
    {
      if ( !pMasterDep->isReadDependency() )
      {
        pMasterAction = pMasterDep->owningAction().openObject();
        ODA_ASSERT_ONCE( !pMasterAction.isNull() );

        break;
      }
    }
  }

  OdDbAssocArrayActionBodyPtr pMasterBody = pMasterAction->actionBody().openObject();
  ODA_ASSERT_ONCE( !pMasterBody.isNull() );
  
  //
  OdDbAssocDependencyPtr pSoourceDep = depIds[1].openObject();
  ODA_ASSERT_ONCE( !pSoourceDep.isNull() );

  m_items.clear();

  OdArray<OdDbItemLocator>::iterator itLocator;
  for ( itLocator = indices.begin(); itLocator != indices.end(); itLocator++ )
  {    
    OdDbAssocArrayItemPtr pMasterItem = pMasterBody->getItemAt( *itLocator );
    if ( pMasterItem.isNull() )
    {
      continue;
    }

    OdDbImpAssocArrayItem *pMasterItemImpl = OdDbImpAssocArrayItem::getImpl( pMasterItem );
    pMasterItemImpl->setModifyActionBody( pBody->objectId() );

    OdDbAssocArrayItemPtr pItem = OdDbAssocArrayItem::createObject( pMasterItem, pBody->objectId() ); 
    m_items.append( pItem );

    OdDbBlockReferencePtr pMasterBTR = pMasterItem->entityProvider().openObject( OdDb::kForWrite );
    if ( !pMasterBTR.isNull() )
    {
      pMasterBTR->setBlockTableRecord( pSoourceDep->dependentOnObject() );
    }
  }
  
  items = m_items;

  return eOk;
}

OdResult OdDbImpAssocArrayModifyParameters::GetItemByLocator( const OdDbItemLocator &locator, OdDbAssocArrayItemPtr &pItem ) const
{
  ItemArray::const_iterator item = std::find_if( m_items.begin(), m_items.end(), findItemByLocator( locator ) );

  if ( item == m_items.end() )
    return eOutOfRange;

  pItem = *item;
  return eOk;
}


