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
#include "DbAssocSurfacesParameters.h"
#include "DbImpAssocSurfacesParameters.h"
#include "DbAssocArrayActionBody.h"
#include "DbAssocArrayParameters.h"
#include "RxObject.h"
#include "DebugStuff.h"
#include "DbImpAssocArrayParameters.h"

ODRX_CONS_DEFINE_MEMBERS(OdDbAssocLoftParameters, OdDbAssocArrayParameters, RXIMPL_CONSTR)

OdDbAssocLoftParameters::OdDbAssocLoftParameters(
  int continuity,
  double bulge
  )
  : OdDbAssocArrayParameters(new OdDbImpAssocLoftParameters(
    continuity,
    bulge
    ))
{

}

OdDbAssocLoftParameters::OdDbAssocLoftParameters(OdDbImpAssocLoftParameters* pImpl)
  : OdDbAssocArrayParameters(pImpl)
{
  m_pImpl = pImpl;
}

OdDbAssocLoftParameters::~OdDbAssocLoftParameters()
{
  if (m_pImpl)
  {
    delete m_pImpl;
    m_pImpl = NULL;
  }
}

OdDbItemLocator OdDbAssocLoftParameters::getCanonicalForm(const OdDbItemLocator& locator) const
{
  return static_cast<OdDbImpAssocLoftParameters *>
    (m_pImpl)->getCanonicalForm(locator);
}

OdResult OdDbAssocLoftParameters::getItems(OdArray<OdDbAssocArrayItemPtr>& items) const
{
  return eOk;
}

OdResult OdDbAssocLoftParameters::getItemPosition(const OdDbItemLocator &, OdGePoint3d &, OdGeMatrix3d &) const
{
  return eOk;
}

OdResult OdDbAssocLoftParameters::setOwner(class OdDbAssocActionBody* pOwner)
{
  OdDbImpAssocLoftParameters* pImpl = (OdDbImpAssocLoftParameters*)m_pImpl;

  return pImpl->setOwner(pOwner);
}

void OdDbAssocLoftParameters::setInputVariables(const OdDbObjectIdArray& sourceEntites, short continuity, double bulge)
{
  OdDbImpAssocLoftParameters* pImpl = (OdDbImpAssocLoftParameters*)m_pImpl;
  pImpl->setInputVariables(sourceEntites, continuity, bulge);
}
