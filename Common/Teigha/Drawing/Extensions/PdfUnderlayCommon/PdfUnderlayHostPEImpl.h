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


#ifndef OD_DBPdfUNDERLAYPEIMPL_H
#define OD_DBPdfUNDERLAYPEIMPL_H

#include "TD_PackPush.h"
#include "DbUnderlayHost.h"

/** \details
    This class is the OdDbUnderlayHostPE Protocol Extension class implementation.
    library: Source code provided. 
    <group Win_Classes> 
*/
class PdfUnderlayHostPEImpl : public OdDbUnderlayHostPE
{
public:

  ODRX_DECLARE_MEMBERS(PdfUnderlayHostPEImpl);

  PdfUnderlayHostPEImpl();

  virtual OdResult load(OdDbDatabase &pDb, OdString path, OdString password, OdDbUnderlayFilePtr& result);
};

typedef OdSmartPtr<PdfUnderlayHostPEImpl> PdfUnderlayHostPEImplPtr;

#include "TD_PackPop.h"

#endif // OD_DBPdfUNDERLAYPEIMPL_H
