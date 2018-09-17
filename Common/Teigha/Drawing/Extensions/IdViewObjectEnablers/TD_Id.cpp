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

/************************************************************************/
/* TD_Id.cpp: Defines the entry points for the DLL                      */
/************************************************************************/

#include "OdaCommon.h"
#include "RxSysRegistry.h"

#include "RxDynamicModule.h"

#include "IdComplexEntity.h"
#include "IdBlockReference.h"
#include "IdViewArc.h"
#include "IdViewCircle.h"
#include "IdViewEllipse.h"
#include "IdViewLine.h"
#include "IdViewSpline.h"
#include "IdObjects.h"
#include "DbDictionary.h"

#define STL_USING_UTILITY
#include "OdaSTL.h"

ODRX_DEFINE_MEMBERS_EX(OdIdObjectToNotify,                                                                           // ClassName
                       OdDbObject,                                                                                   // ParentClass
                       DBOBJECT_CONSTR,                                                                              // DOCREATE
                       OdDb::kDHL_1024,                                                                              // DwgVer
                       OdDb::kMRelease4,                                                                             // MaintVer
                       1024,                                                                                         // NProxyFlags
                       L"AcIdObjectToNotify",                                                                        // DWG class name
                       L"ACIDOBJECTTONOTIFY",                                                                        // DxfName
                       L"AcIdViewObj|Product: Inventor Drawing Enabler|Company: Autodesk|Website: www.autodesk.com", // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware)                                                       // CustomFlags

ODRX_DEFINE_MEMBERS_EX(OdIdVersion,                                                                                  // ClassName
                       OdDbObject,                                                                                   // ParentClass
                       DBOBJECT_CONSTR,                                                                              // DOCREATE
                       OdDb::kDHL_1024,                                                                              // DwgVer
                       OdDb::kMRelease4,                                                                             // MaintVer
                       1024,                                                                                         // NProxyFlags
                       L"AcIdVersion",                                                                               // DWG class name
                       L"ACIDVERSION",                                                                               // DxfName
                       L"AcIdViewObj|Product: Inventor Drawing Enabler|Company: Autodesk|Website: www.autodesk.com", // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware)                                                       // CustomFlags

class TD_IdViewObjModule : public OdRxModule
{
  public:
	void initApp()
  {
    OdIdComplexEntity::rxInit();
    OdIdBlockReference::rxInit();
    OdIdViewArc::rxInit();
    OdIdViewCircle::rxInit();
    OdIdViewEllipse::rxInit();
    OdIdViewLine::rxInit();
    OdIdViewSpline::rxInit();
    OdIdObjectToNotify::rxInit();
    OdIdVersion::rxInit();
  };

  void uninitApp()
  {
    OdIdObjectToNotify::rxUninit();
    OdIdVersion::rxUninit();
    OdIdComplexEntity::rxUninit();
    OdIdBlockReference::rxUninit();
    OdIdViewArc::rxUninit();
    OdIdViewCircle::rxUninit();
    OdIdViewEllipse::rxUninit();
    OdIdViewLine::rxUninit();
    OdIdViewSpline::rxUninit();
  };
};

ODRX_DEFINE_DYNAMIC_MODULE(TD_IdViewObjModule);

class OdIdObjectToNotifyImpl
{
public:
  OdArray<std::pair<OdDbObjectId, OdInt32> > m_data;
};

OdIdObjectToNotify::OdIdObjectToNotify() : m_pImpl(new OdIdObjectToNotifyImpl)
{
}

OdIdObjectToNotify::~OdIdObjectToNotify()
{
  delete m_pImpl;
}

OdResult OdIdObjectToNotify::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
    return res;
  
  if (pFiler->filerType() == OdDbFiler::kFileFiler)
  {
    OdInt32 N = pFiler->rdInt32();
    for (OdInt32 i = 0; i < N; ++i)
    {
      OdDbObjectId id = pFiler->rdSoftPointerId();
      OdInt32 k = pFiler->rdInt32();
      m_pImpl->m_data.append(std::make_pair(id, k));
    }
  }
  return eOk;
}
void OdIdObjectToNotify::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  if (pFiler->filerType() == OdDbFiler::kFileFiler)
  {
    OdInt32 N = (OdInt32)m_pImpl->m_data.size();
    pFiler->wrInt32(N);
    for (OdInt32 i = 0; i < N; ++i)
    {
      pFiler->wrSoftPointerId(m_pImpl->m_data[i].first);
      pFiler->wrInt32(m_pImpl->m_data[i].second);
    }
  }
}

OdDbObjectPtr OdIdObjectToNotify::decomposeForSave(
    OdDb::SaveType format, OdDb::DwgVersion /*ver*/, OdDbObjectId& /*replaceId*/, bool& /*exchangeXData*/)
{
  if (format != OdDb::kDwg)
  { // dxf or binary dxf  #6261 #21924
    OdDbDictionaryPtr pDic = OdDbDictionary::cast(ownerId().openObject(OdDb::kForWrite));
    if (pDic.get())
      pDic->remove(objectId());
  }
  return OdDbObjectPtr();
}


OdResult OdIdVersion::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbObject::dwgInFields(pFiler);
  if (res != eOk)
    return res;
#ifdef ODA_DIAGNOSTICS
  int version =
#endif
  pFiler->rdInt32();
ODA_ASSERT_ONCE(version == 1);
  return eOk;
}
void OdIdVersion::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbObject::dwgOutFields(pFiler);
  pFiler->wrInt32(1);
}

OdDbObjectPtr OdIdVersion::decomposeForSave(
    OdDb::SaveType format, OdDb::DwgVersion /*ver*/, OdDbObjectId& /*replaceId*/, bool& /*exchangeXData*/)
{
  if (format != OdDb::kDwg)
  { // dxf or binary dxf  #6261 #21924
    OdDbDictionaryPtr pDic = OdDbDictionary::cast(ownerId().openObject(OdDb::kForWrite));
    if (pDic.get())
      pDic->remove(objectId());
  }
  return OdDbObjectPtr();
}
