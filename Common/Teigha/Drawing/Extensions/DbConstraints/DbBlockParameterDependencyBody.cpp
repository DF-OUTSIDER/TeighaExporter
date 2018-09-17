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
#include "DbBlockParameterDependencyBody.h"
#include "OdExplicitConstr.h"

ODRX_DEFINE_MEMBERS_EX(OdDbBlockParameterDependencyBody,    // ClassName
                       OdDbAssocDimDependencyBodyBase,      // ParentClass
                       DBOBJECT_CONSTR,                     // DOCREATE
                       OdDb::kDHL_1021,                     // DwgVer
                       OdDb::kMRelease6,                    // MaintVer
                       1,                                   // nProxyFlags (kEraseAllowed)
                       L"AcDbBlockParameterDependencyBody", // DWG class name
                       L"BLOCKPARAMDEPENDENCYBODY",         // DxfName
                       L"ObjectDBX Classes",                // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbBlockParameterDependencyBody::OdDbBlockParameterDependencyBody():OdDbAssocDimDependencyBodyBase()
{
}

OdDbBlockParameterDependencyBody::~OdDbBlockParameterDependencyBody()
{
}

OdResult OdDbBlockParameterDependencyBody::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbAssocDimDependencyBodyBase::dwgInFields(pFiler);
  if (res != eOk)
    return res;

  OdInt16 val = pFiler->rdInt16(); // ver
  if (val)
  {
    ODA_FAIL_ONCE(); // TODO
    return eMakeMeProxy;
  }

  return res;
}

void OdDbBlockParameterDependencyBody::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbAssocDimDependencyBodyBase::dwgOutFields(pFiler);
  
  pFiler->wrInt16(0); // ver
}

#define NEXT_CODE(code)         \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL_ONCE();              \
  return eMakeMeProxy;          \
}

OdResult OdDbBlockParameterDependencyBody::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbAssocDimDependencyBodyBase::dxfInFields(pFiler);
  if (res != eOk)
    return res;

  if (!pFiler->atSubclassData(desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  if (pFiler->rdUInt32()) // ver
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  return res;
}

void OdDbBlockParameterDependencyBody::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbAssocDimDependencyBodyBase::dxfOutFields(pFiler);
  pFiler->wrSubclassMarker(desc()->name());
  pFiler->wrUInt32(90, 0);
}

// TODO see impl of next methods in class OdDbAssocDimDependencyBody :

OdString OdDbBlockParameterDependencyBody::getEntityTextOverride() const
{
  ODA_FAIL_ONCE(); // TODO
  throw OdError(eNotImplementedYet);
  //assertReadEnabled();
}

OdResult OdDbBlockParameterDependencyBody::setEntityTextOverride(const OdString& newText)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
  //assertWriteEnabled();
}

double OdDbBlockParameterDependencyBody::getEntityMeasurementOverride() const
{
  ODA_FAIL_ONCE(); // TODO
  throw OdError(eNotImplementedYet);
  //assertReadEnabled();
}

bool OdDbBlockParameterDependencyBody::isEntityAttachmentChangedOverride() const
{
  ODA_FAIL_ONCE(); // TODO
  throw OdError(eNotImplementedYet);
  //assertWriteEnabled();
}


