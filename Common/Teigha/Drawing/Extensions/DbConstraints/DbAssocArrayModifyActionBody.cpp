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
#include "DbAssocArrayModifyActionBody.h"
#include "DbImpAssocArrayParameters.h"



ODRX_DEFINE_MEMBERS_EX( OdDbAssocArrayModifyParameters,        // ClassName
                       OdDbAssocArrayParameters,               // ParentClass
                       RXIMPL_CONSTR,                          // DOCREATE (RXIMPL_CONSTR DBOBJECT_CONSTR)
                       OdDb::kDHL_1024,                        // DwgVer
                       OdDb::kMRelease6,                       // MaintVer
                       1025,                                   // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocArrayModifyParameters",      // DWG class name
                       L"ACDBASSOCARRAYMODIFYPARAMETERS",      // DxfName
                       L"ObjectDBX Classes",                   // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocArrayModifyParameters::OdDbAssocArrayModifyParameters( OdDbImpAssocArrayModifyParameters* pImpl )
  : OdDbAssocArrayParameters( pImpl )
{
}

OdDbAssocArrayModifyParameters::OdDbAssocArrayModifyParameters()
: OdDbAssocArrayParameters(new OdDbImpAssocArrayModifyParameters())
{

}

OdDbAssocArrayModifyParameters::~OdDbAssocArrayModifyParameters()
{
}

OdResult OdDbAssocArrayModifyParameters::getItems( OdArray<OdDbAssocArrayItemPtr>& items ) const
{
  return m_pImpl->getItems( items );
}

OdResult OdDbAssocArrayModifyParameters::getItemPosition( const OdDbItemLocator& locator, OdGePoint3d& position, OdGeMatrix3d& xform ) const
{
  return m_pImpl->getItemPosition( locator, position, xform );
}


ODRX_DEFINE_MEMBERS_EX(OdDbAssocArrayModifyActionBody,// ClassName
                       OdDbAssocArrayActionBody,      // ParentClass
                       DBOBJECT_CONSTR,               // DOCREATE
                       OdDb::kDHL_1021,               // DwgVer
                       OdDb::kMRelease6,              // MaintVer
                       1025,                          // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocArrayModifyActionBody",   // DWG class name
                       L"ACDBASSOCARRAYMODIFYACTIONBODY",   // DxfName
                       L"ObjectDBX Classes",         // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocArrayModifyActionBody::OdDbAssocArrayModifyActionBody()
  : OdDbAssocArrayActionBody(false)
{
  m_pImpObj = static_cast<OdDbImpAssocActionBody*>(new OdDbImpAssocArrayModifyActionBody());
}

OdDbAssocArrayModifyActionBody::~OdDbAssocArrayModifyActionBody()
{
  // via base class // delete m_pImpObj;
}

OdResult OdDbAssocArrayModifyActionBody::getOverridenItems( OdArray<OdDbItemLocator> &indices ) const
{
  assertReadEnabled();
  return static_cast<OdDbImpAssocArrayModifyActionBody *>(m_pImpObj)->getOverridenItems( indices );
}

OdResult OdDbAssocArrayModifyActionBody::setOverridenItems( const OdArray<OdDbItemLocator> &indices )
{
  assertWriteEnabled();
  return static_cast<OdDbImpAssocArrayModifyActionBody *>(m_pImpObj)->setOverridenItems( indices );
}

bool OdDbAssocArrayModifyActionBody::controlsItem( const OdDbItemLocator &index ) const
{
  assertReadEnabled();

  return static_cast<OdDbImpAssocArrayModifyActionBody *>(m_pImpObj)->controlsItem( index );
}

OdResult OdDbAssocArrayModifyActionBody::createInstance( OdDbObjectId arrayId, const OdArray<OdDbItemLocator> &indices, const OdDbObjectIdArray &subEnt, OdDbVertexRef &basePoint, OdDbObjectId& actionBodyId )
{
  //assertWriteEnabled();
  
  return OdDbImpAssocArrayModifyActionBody::createInstance( arrayId, indices, subEnt, basePoint, actionBodyId ); 
}

