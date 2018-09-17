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

#ifndef _OD_DWFXSIGNATUREHANDLER_MODULE_H__82444C45_0158_4B5B_AD53_3E279DCC8410__INCLUDED_
#define _OD_DWFXSIGNATUREHANDLER_MODULE_H__82444C45_0158_4B5B_AD53_3E279DCC8410__INCLUDED_

#include "TD_PackPush.h"

#include "OdDwfxSignatureHandler.h"

/** \details
  <group Extension_Classes>
*/
class DwfxSignatureHandlerModule : public OdDwfxSignatureHandlerModule
{
public:
  virtual OdDwfxSignatureHandlerPtr getDwfxSignatureHandler() const;

  virtual int getSuitableCertificatesList(OdArray<OdCertParameters>& certificates) const;

  void initApp();  
  void uninitApp();
};

#include "TD_PackPop.h"

#endif
