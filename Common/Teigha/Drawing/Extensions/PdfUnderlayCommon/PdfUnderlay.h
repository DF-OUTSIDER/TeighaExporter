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


///////////////////////////////////////////////////////////////////////////////
//
// PdfUnderlay.h - Open Design auxiliary export methods for support of snap points in Pdf Underlays.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_UNDERLAY_INCLUDED_
#define _PDF_UNDERLAY_INCLUDED_

#include "TD_PackPush.h"

#include "OdPlatformSettings.h"
#include "Gi/GiGeometry.h"
#include "DbUnderlayReference.h"
#include "DbUnderlayHost.h"
#include "RxModule.h"
#include "StaticRxObject.h"

OdResult OD_DLL_EXPORT getSnapGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, const OdGePoint3d* pickPtMc);

class OdDbUnderlayHostPE;

/** \details
  Library:  Source code provided.
  <group PdfServices_Classes>
*/
class OD_DLL_EXPORT PdfUnderlayModule : public OdRxModule
{
  OdDbUnderlayHostPEPtr m_PdfHostPE;
public:
  PdfUnderlayModule();

  virtual void initApp();
  virtual void uninitApp();

  virtual OdResult getExplodeGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, bool bIgnoreImages);

  virtual OdResult forInternalUseOnly(OdUInt32 n, void *p);

  virtual OdResult getSnapGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, const OdGePoint3d* pickPtMc);

  ~PdfUnderlayModule();
};
typedef OdSmartPtr<PdfUnderlayModule> PdfUnderlayModulePtr;

#include "TD_PackPop.h"

#endif // _PDF_UNDERLAY_INCLUDED_

