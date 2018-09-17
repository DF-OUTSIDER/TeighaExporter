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
#include "OdDwfxSignatureHandlerImpl.h"
#include "DwfxSignatureHandlerModule.h"

OdDwfxSignatureHandlerImpl::OdDwfxSignatureHandlerImpl()
{
}

OdDwfxSignatureHandlerImpl::~OdDwfxSignatureHandlerImpl()
{
}

void OdDwfxSignatureHandlerImpl::setCertParameters(const OdCertParameters& certParams)
{
  m_certParams = certParams;
}

bool OdDwfxSignatureHandlerImpl::signPackage(const OdString& filePath)
{
  throw OdError(eNotImplemented);
}

bool OdDwfxSignatureHandlerImpl::removeExistingSignature(const OdString& filePath)
{
  throw OdError(eNotImplemented);
}

bool OdDwfxSignatureHandlerImpl::validatePackageSignature(const OdString& filePath, 
  SignatureValidationResult& validationResult, OdCertParameters& certParams)
{
  throw OdError(eNotImplemented);
}
