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
#include "DbAssocArrayPathParameters.h"
#include "DbImpAssocArrayParameters.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocArrayPathParameters,    // ClassName
                       OdDbAssocArrayCommonParameters,  // ParentClass
                       RXIMPL_CONSTR,                   // DOCREATE (RXIMPL_CONSTR DBOBJECT_CONSTR)
                       OdDb::kDHL_1024,                 // DwgVer
                       OdDb::kMRelease6,                // MaintVer
                       1025,                            // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocArrayPathParameters", // DWG class name
                       L"ACDBASSOCARRAYPATHPARAMETERS", // DxfName
                       L"ObjectDBX Classes",            // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocArrayPathParameters::OdDbAssocArrayPathParameters(double itemSpacing, // = 1
                                                           double rowSpacing, // = 1
                                                           double levelSpacing, // = 1
                                                           int itemCount, // = 1
                                                           int rowCount, // = 1
                                                           int levelCount, // = 1
                                                           double rowElevation) // = 1
  : OdDbAssocArrayCommonParameters(new OdDbImpAssocArrayPathParameters(itemSpacing, rowSpacing, levelSpacing,
                                                                       itemCount, rowCount, levelCount, rowElevation))
{
}                                                      

//protected:
OdDbAssocArrayPathParameters::OdDbAssocArrayPathParameters(OdDbImpAssocArrayPathParameters* pImpl)
  : OdDbAssocArrayCommonParameters(pImpl)
{
}                                                      

OdDbAssocArrayPathParameters::~OdDbAssocArrayPathParameters()
{
}                                                      

int OdDbAssocArrayPathParameters::itemCount(OdString& expression, 
                                            OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
          (m_pImpl)->itemCount( expression, evaluatorId );
}

int OdDbAssocArrayPathParameters::itemCount() const
{
  OdString expression, evaluatorId;
  return itemCount(expression, evaluatorId);
}

int OdDbAssocArrayPathParameters::itemCount(OdString & expression) const
{
  OdString evaluatorId;
  return itemCount(expression, evaluatorId);
}

double OdDbAssocArrayPathParameters::itemSpacing(OdString& expression, OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
          (m_pImpl)->itemSpacing( expression, evaluatorId );
}

double OdDbAssocArrayPathParameters::itemSpacing() const
{
  OdString expression, evaluatorId;
  return itemSpacing(expression, evaluatorId);
}

double OdDbAssocArrayPathParameters::itemSpacing(OdString& expression) const
{
  OdString evaluatorId;
  return itemSpacing(expression, evaluatorId);
}

OdDbEdgeRef OdDbAssocArrayPathParameters::path() const
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
           (m_pImpl)->path();
}                                                      

OdDbAssocArrayPathParameters::Method OdDbAssocArrayPathParameters::method() const
{   
  return static_cast<OdDbAssocArrayPathParameters::Method> (
           static_cast<OdDbImpAssocArrayPathParameters *>
            (m_pImpl)->method() );
}                                                      

bool OdDbAssocArrayPathParameters::alignItems() const
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
          (m_pImpl)->alignItems();
}                                                      

double OdDbAssocArrayPathParameters::startOffset(OdString& expression, 
                                                 OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->startOffset( expression, evaluatorId );
}                                                      

double OdDbAssocArrayPathParameters::startOffset() const
{
  OdString expression, evaluatorId;
  return startOffset(expression, evaluatorId);
}

double OdDbAssocArrayPathParameters::endOffset(OdString& expression, 
                                               OdString& evaluatorId) const
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->endOffset( expression, evaluatorId );
}

double OdDbAssocArrayPathParameters::endOffset() const
{
  OdString expression, evaluatorId;
  return endOffset(expression, evaluatorId);
}

OdResult OdDbAssocArrayPathParameters::setItemCount(int nItems,
                                                    const OdString& expression, // = OdString::kEmpty
                                                    const OdString& evaluatorId, // = OdString::kEmpty
                                                    OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
          (m_pImpl)->setItemCount( nItems, expression, evaluatorId, pErrorMessage);
}                                                      

OdResult OdDbAssocArrayPathParameters::setItemSpacing(double offset,
                                                      const OdString& expression, // = OdString::kEmpty
                                                      const OdString& evaluatorId, // = OdString::kEmpty
                                                      OdString& pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->setItemSpacing( offset, expression, evaluatorId, pErrorMessage );
}                                                      

OdResult OdDbAssocArrayPathParameters::setPath(const OdDbEdgeRef& edgeRef)
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->setPath( edgeRef );
}                                                      

OdResult OdDbAssocArrayPathParameters::setMethod(Method method)
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->setMethod( method );
}                                                      

OdResult OdDbAssocArrayPathParameters::setAlignItems(bool bAlignItems)
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->setAlignItems( bAlignItems );
}                                                      

OdResult OdDbAssocArrayPathParameters::setStartOffset(double startOffset,
                                                      const OdString& expression, // = OdString::kEmpty
                                                      const OdString& evaluatorId, // = OdString::kEmpty
                                                      OdString* pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->setStartOffset( startOffset, expression, evaluatorId, pErrorMessage );
}                                                      

OdResult OdDbAssocArrayPathParameters::setEndOffset(double endOffset,
                                                    const OdString& expression, // = OdString::kEmpty
                                                    const OdString& evaluatorId, // = OdString::kEmpty
                                                    OdString* pErrorMessage) // = NULL
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->setEndOffset( endOffset, expression, evaluatorId, pErrorMessage );
}                                                      

OdResult OdDbAssocArrayPathParameters::setPathDirection(bool bAlongParams)
{
  return static_cast<OdDbImpAssocArrayPathParameters *>
    (m_pImpl)->setPathDirection( bAlongParams );
}                                                      

OdResult OdDbAssocArrayPathParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items) const
{
  return m_pImpl->getItems( items );
}                                                      

OdResult OdDbAssocArrayPathParameters::getItemPosition(const OdDbItemLocator& locator,
                                                       OdGePoint3d& position,
                                                       OdGeMatrix3d& xform) const
{
  return m_pImpl->getItemPosition( locator, position, xform );
}                                                      

OdResult OdDbAssocArrayPathParameters::getItemPosition(const OdDbItemLocator& locator,
                                                       OdGePoint3d& position) const
{
  OdGeMatrix3d xform;
  return getItemPosition(locator, position,xform);
}
