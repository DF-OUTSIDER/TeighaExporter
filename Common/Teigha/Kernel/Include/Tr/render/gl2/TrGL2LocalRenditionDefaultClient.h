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
// TrGL2 renderer default local client

#ifndef ODTRGL2LOCALRENDITIONDEFAULTCLIENT
#define ODTRGL2LOCALRENDITIONDEFAULTCLIENT

#include "TD_PackPush.h"

#include "RxObjectImpl.h"
#include "TrGL2LocalRenditionHost.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrGL2LocalRenditionDefaultClient : public OdTrGL2LocalRenditionClient
{
  public:
    // Returns local context
    virtual OdTrGL2LocalContextPtr createLocalContext(OdTrVisRenderClient *pDevice) {
      return OdTrGL2LocalContext::createLocalContext(pDevice);
    }

    static OdTrGL2LocalRenditionClientPtr createObject() {
      return OdRxObjectImpl<OdTrGL2LocalRenditionDefaultClient, OdTrGL2LocalRenditionClient>::createObject();
    }
};

#include "TD_PackPop.h"

#endif // ODTRGL2LOCALRENDITIONDEFAULTCLIENT
