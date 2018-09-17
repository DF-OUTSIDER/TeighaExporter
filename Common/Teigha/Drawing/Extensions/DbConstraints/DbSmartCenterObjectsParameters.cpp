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
#include "DbSmartCenterObjectsParameters.h"
#include "DbImpSmartCenterObjectsParameters.h"
#include "DbAssocArrayActionBody.h"
#include "DbAssocArrayParameters.h"
#include "RxObject.h"
#include "DebugStuff.h"
#include "DbImpAssocArrayParameters.h"

ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbSmartCenterCommonParameters, OdDbAssocArrayParameters, L"AcDbSmartCenterCommonParameters")

//protected:
OdDbSmartCenterCommonParameters::OdDbSmartCenterCommonParameters(OdDbImpSmartCenterCommonParameters* pImpl)
  : OdDbAssocArrayParameters(pImpl)
{
}

OdDbSmartCenterCommonParameters::~OdDbSmartCenterCommonParameters()
{
}

OdDbItemLocator OdDbSmartCenterCommonParameters::getCanonicalForm(const OdDbItemLocator& locator) const
{
  return static_cast<OdDbImpSmartCenterCommonParameters *>
    (m_pImpl)->getCanonicalForm(locator);
}

OdResult OdDbSmartCenterCommonParameters::setOwner(class OdDbAssocActionBody* pOwner)
{
  OdDbImpSmartCenterCommonParameters* pImpl = (OdDbImpSmartCenterCommonParameters*)m_pImpl;
  //OdDbImpSmartCenterCommonParametersPtr pImpl = OdDbImpSmartCenterCommonParameters::cast(m_pImpl);
  return pImpl->setOwner(pOwner);
}

OdResult OdDbSmartCenterCommonParameters::setCenterDepId(const OdDbObjectId input,
  const OdString& expression, // = OdString::kEmpty
  const OdString& evaluatorId, // = OdString::kEmpty
  OdString& pErrorMessage)
{
  return static_cast<OdDbImpSmartCenterCommonParameters *>
    (m_pImpl)->setCenterDepId(input, expression, evaluatorId, pErrorMessage);
}

OdDbImpSmartCenterCommonParameters* OdDbSmartCenterCommonParameters::impSmartCenterParameters()
{ 
  return (OdDbImpSmartCenterCommonParameters*)m_pImpl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ODRX_DEFINE_MEMBERS_EX(OdDbSmartCenterMarkParameters,    // ClassName
//                       OdDbSmartCenterCommonParameters, // ParentClass
//                       RXIMPL_CONSTR,                          // DOCREATE (RXIMPL_CONSTR DBOBJECT_CONSTR)
//                       OdDb::kDHL_1024,                        // DwgVer
//                       OdDb::kMRelease6,                       // MaintVer
//                       1025,                                   // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
//                       //L"", // DWG class name
//                       //L"", // DxfName
//                       //L"",                   // AppName
//  L"AcDbSmartCenterMarkParameters", // DWG class name
//  L"ACDbSmartCenterMarkParameters", // DxfName
//  L"ObjectDBX Classes",                   // AppName
//  OdRx::kMTLoading | OdRx::kHistoryAware);

ODRX_CONS_DEFINE_MEMBERS(OdDbSmartCenterMarkParameters, OdDbSmartCenterCommonParameters, RXIMPL_CONSTR)

OdDbSmartCenterMarkParameters::OdDbSmartCenterMarkParameters(
    double HorizontalStartExtension,
    double HorizontalEndExtension,
    double VerticalStartExtension,
    double VerticalEndExtension,
    double HorizontalStartOvershoot,
    double HorizontalEndOvershoot,
    double VerticalStartOvershoot,
    double VerticalEndOvershoot,
    OdString CrossHairLengthExpression,
    OdString CrossHairGapExpression,
    OdDbObjectId CenterDepId,
    OdDb::Visibility extensionLinesVisibility,
    double CachedCircleRadius,
    OdString SubentLineType,
    double SubentLineWeight,
    double SubentLinetypeScale,
    double CachedAssociateStatus
  )
  : OdDbSmartCenterCommonParameters(new OdDbImpSmartCenterMarkParameters(
    HorizontalStartExtension,
    HorizontalEndExtension,
    VerticalStartExtension,
    VerticalEndExtension,
    HorizontalStartOvershoot,
    HorizontalEndOvershoot,
    VerticalStartOvershoot,
    VerticalEndOvershoot,
    CrossHairLengthExpression,
    CrossHairGapExpression,
    CenterDepId,
    extensionLinesVisibility,
    CachedCircleRadius,
    SubentLineType,
    SubentLineWeight,
    SubentLinetypeScale,
    CachedAssociateStatus
    ))
{

}

OdDbSmartCenterMarkParameters::OdDbSmartCenterMarkParameters(OdDbImpSmartCenterMarkParameters* pImpl)
  : OdDbSmartCenterCommonParameters(pImpl)
{
  m_pImpl = pImpl;
}

OdDbSmartCenterMarkParameters::~OdDbSmartCenterMarkParameters()
{
  if (m_pImpl)
  {
    delete m_pImpl;
    m_pImpl = NULL;
  }
}

OdResult OdDbSmartCenterMarkParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items) const
{
  return eOk;
}

OdResult OdDbSmartCenterMarkParameters::getItemPosition(const OdDbItemLocator &, OdGePoint3d &, OdGeMatrix3d &) const
{
  return eOk;
}

OdResult OdDbSmartCenterMarkParameters::setCachedCircleRadius(const double input,
  const OdString& expression, // = OdString::kEmpty
  const OdString& evaluatorId, // = OdString::kEmpty
  OdString& pErrorMessage)
{
  return static_cast<OdDbImpSmartCenterMarkParameters *>
    (m_pImpl)->setCachedCircleRadius(input, expression, evaluatorId, pErrorMessage);
}
///////////////////////////////////////////////////////////////////////////////////////////////
ODRX_CONS_DEFINE_MEMBERS(OdDbSmartCenterLineParameters, OdDbSmartCenterCommonParameters, RXIMPL_CONSTR)

OdDbSmartCenterLineParameters::OdDbSmartCenterLineParameters(
  double StartExtLength,
  double EndExtLength,
  double StartOvershoot,
  double EndOvershoot,
  int EdgeRefDirectionEnding,
  OdDbObjectId CenterDepId,
  double CachedCenterLineLength,
  double CachedCenterLineDirectionX,
  double CachedCenterLineDirectionY,
  double CachedCenterLineDirectionZ,
  OdString SubentLineType,
  double SubentLineWeight,
  double SubentLinetypeScale,
  double CachedAssociateStatus
  )
  : OdDbSmartCenterCommonParameters(new OdDbImpSmartCenterLineParameters(
    ))
{

}

OdDbSmartCenterLineParameters::OdDbSmartCenterLineParameters(OdDbImpSmartCenterLineParameters* pImpl)
  : OdDbSmartCenterCommonParameters(pImpl)
{
}

OdDbSmartCenterLineParameters::~OdDbSmartCenterLineParameters()
{
}

OdResult OdDbSmartCenterLineParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items) const
{
  return eOk;
}

OdResult OdDbSmartCenterLineParameters::getItemPosition(const OdDbItemLocator &, OdGePoint3d &, OdGeMatrix3d &) const
{
  return eOk;
}

double OdDbSmartCenterLineParameters::startOvershoot(
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbImpSmartCenterLineParameters* pImpl = static_cast<OdDbImpSmartCenterLineParameters * > (m_pImpl);
//  return static_cast<OdDbImpSmartCenterLineParameters *>
  return pImpl->startOvershoot(expression, evaluatorId);
}

double OdDbSmartCenterLineParameters::endOvershoot(
  OdString& expression, // = OdString::kEmpty
  OdString& evaluatorId // = OdString::kEmpty
  ) const
{
  OdDbImpSmartCenterLineParameters* pImpl = static_cast<OdDbImpSmartCenterLineParameters * > (m_pImpl);
  //return static_cast<OdDbImpSmartCenterLineParameters *>
  return pImpl->endOvershoot(expression, evaluatorId);
}

OdResult OdDbSmartCenterLineParameters::edgeRefDirEndingArray(OdArray<int> & inputArr)
{
  return static_cast<OdDbImpSmartCenterLineParameters *>
    (m_pImpl)->edgeRefDirEndingArray( inputArr );
}

OdResult OdDbSmartCenterLineParameters::setEdgeRefDirEndingArray(const OdArray<int> & inputArr, 
  const OdString& expression, // = OdString::kEmpty
  const OdString& evaluatorId, // = OdString::kEmpty
  OdString& pErrorMessage)
{
  return static_cast<OdDbImpSmartCenterLineParameters *>
    (m_pImpl)->setEdgeRefDirEndingArray(inputArr, expression, evaluatorId, pErrorMessage);
}

