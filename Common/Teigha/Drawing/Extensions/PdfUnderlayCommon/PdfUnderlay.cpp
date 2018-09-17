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


#if defined(_MSC_VER)
// warning C4290: C++ Exception Specification ignored
#pragma warning ( disable : 4290 )
#if _MSC_VER <= 1200
// compiler stack overflows if these warning is enabled
#pragma warning ( disable : 4786)
#endif
#endif

#include "OdaCommon.h"
#include "PdfUnderlay.h"
#include "RxDispatchImpl.h"
#include "RxVariantValue.h"
#include "DbDictionary.h"
#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "Gi/GiGeometry.h"
#include "DbCommandContext.h"
#include "StaticRxObject.h"
#include "DbUnderlayReference.h"

// Underlay headers
#include "PdfUnderlayHostPEImpl.h"
#include "DbUnderlayDefinition.h"
#include "PdfUnderlayFileImpl.h"
#include "PdfUnderlayItemImpl.h"
#include "PdfDirectWrapper.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma warning(disable:4355)
#endif

PdfUnderlayModule::PdfUnderlayModule()
{
}

PdfUnderlayModule::~PdfUnderlayModule()
{
  m_PdfHostPE = 0;
}

void PdfUnderlayModule::initApp()
{
  PdfDirectWrapper::rxInitWrapper();

  // it should be after the code with throw OdError (inside rxInitWrapper)
  PdfUnderlayFileImpl::rxInit();
  PdfUnderlayItemImpl::rxInit();
  PdfUnderlayHostPEImpl::rxInit();

  m_PdfHostPE = OdRxObjectImpl<PdfUnderlayHostPEImpl>::createObject();

  OdDbPdfDefinition::desc()->addX(OdDbUnderlayHostPE::desc(), m_PdfHostPE);
}

void PdfUnderlayModule::uninitApp()
{
  OdDbPdfDefinition::desc()->delX(OdDbUnderlayHostPE::desc());
  PdfDirectWrapper::rxUninitWrapper();
  PdfUnderlayFileImpl::rxUninit();
  PdfUnderlayItemImpl::rxUninit();
  PdfUnderlayHostPEImpl::rxUninit();
}

ODRX_DEFINE_DYNAMIC_MODULE(PdfUnderlayModule)

OdResult getSnapGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, const OdGePoint3d* pickPtMc)
{
  OdDbPdfDefinitionPtr pDef = pPdfRef->definitionId().safeOpenObject(OdDb::kForWrite);

  if (!pDef->isLoaded())
    return eNoInterface;

  PdfUnderlayItemImplPtr pItemImp( pDef->getUnderlayItem() );

  // fill layer array. Layer visible flag can be changed by user application.
  OdUInt32 numL = pPdfRef->underlayLayerCount();

  OdArray<OdUnderlayLayer> layers;
  layers.resize(numL);
  for(OdUInt32 f=0; f<numL; ++f)
    pPdfRef->getUnderlayLayer(f, layers[f]);

  return pItemImp->getSnapGeometry(pPdfRef->database(), pGrabber, layers, pickPtMc);
}

OdResult PdfUnderlayModule::getExplodeGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, bool bIgnoreImages)
{

  if (pPdfRef.isNull())
  {
    return eNullObjectPointer;
  }

  OdDbPdfDefinitionPtr pDef = pPdfRef->definitionId().openObject();

  if (pDef.isNull())
  {
    return eNullObjectId;
  }

  if (!pDef->isLoaded())
  {
    return eNoInterface;
  }

  PdfUnderlayItemImplPtr pItemImp( pDef->getUnderlayItem() );

  // fill layer array. Layer visible flag can be changed by user application.
  OdUInt32 numL = pPdfRef->underlayLayerCount();

  OdArray<OdUnderlayLayer> layers;
  layers.resize(numL);
  for(OdUInt32 f=0; f<numL; ++f)
  {
    pPdfRef->getUnderlayLayer(f, layers[f]);
  }

  return pItemImp->getExplodeGeometry(pGrabber, layers, bIgnoreImages);
}

OdResult PdfUnderlayModule::getSnapGeometry(const OdDbPdfReferencePtr &pPdfRef, OdGiGeometry *pGrabber, const OdGePoint3d* pickPtMc)
{
  return ::getSnapGeometry(pPdfRef, pGrabber, pickPtMc);
}
