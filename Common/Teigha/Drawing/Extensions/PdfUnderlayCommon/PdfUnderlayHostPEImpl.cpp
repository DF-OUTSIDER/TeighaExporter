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
#include "PdfUnderlayHostPEImpl.h"
#include "PdfUnderlayFileImpl.h"

ODRX_NO_CONS_DEFINE_MEMBERS(PdfUnderlayHostPEImpl, OdDbUnderlayHostPE);

PdfUnderlayHostPEImpl::PdfUnderlayHostPEImpl()
{
}

OdResult PdfUnderlayHostPEImpl::load(OdDbDatabase &pDb, OdString path, OdString password, OdDbUnderlayFilePtr& result)
{
  if ( path.isEmpty() )
    return eInvalidInput;

  PdfUnderlayFileImplPtr pUFile = OdRxObjectImpl<PdfUnderlayFileImpl>::createObject();
  OdResult res = pUFile->loadFile(path, password);
  if (res == eOk)
  {
    result = pUFile;
  }

  return res;
}

//
