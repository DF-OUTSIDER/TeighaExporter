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

// OdOleItemHandlerImpl.cpp: implementation of the OdOleItemHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "OleItemHandlerImpl.h"
#include "COMErrorCtx.h"
#include "Gi/GiCommonDraw.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseVectorizer.h"
#include "OleItemHandlerModule.h"
#include "OdPlatformStreamer.h"
#include "ExEmbedRaster.h"
#include "Gi/GiViewportDraw.h"

#ifdef __BORLANDC__
//Error: Unresolved external '__stdcall _com_issue_error(long)' referenced from ....
void __stdcall _com_issue_error(long hr)  throw(_com_error)
{
  throw _com_error(hr);
}
#endif //__BORLANDC__

OdOleItemHandlerImpl::GlobalBuffer::GlobalBuffer(HGLOBAL hGlobal)
  : m_hGlobal(hGlobal)
  , m_pGlobal(0)
{
  m_pGlobal = ::GlobalLock(m_hGlobal);
  ErrorCtx::checkWin32(m_pGlobal!=NULL);
}

OdOleItemHandlerImpl::OdOleItemHandlerImpl()
  : m_hGlobal(NULL)
{
  // since this object is not DB object and can't be turned to proxy,
  // we should lock its module to prevent unloading.
  OdOleItemHandlerModuleImpl::internalLockModule();
}

OdOleItemHandlerImpl::~OdOleItemHandlerImpl()
{
  OdOleItemHandlerModuleImpl::internalUnlockModule(); // unlock module
  m_pStorage = NULL;
  if(m_hGlobal)
    ::GlobalFree(m_hGlobal);
}

void OdOleItemHandlerImpl::setCompoundDocument(OdUInt32 nSize, OdStreamBuf& stream)
{
  if (nSize == 0)
  {
    m_pStorage = NULL;
    if (m_hGlobal)
    {
      ::GlobalFree(m_hGlobal);
      m_hGlobal = NULL;
    }
  }
  else
  {
    if (m_hGlobal)
    {
      m_pStorage = NULL;
      HGLOBAL hGlobal = ::GlobalReAlloc(m_hGlobal, nSize, GMEM_MOVEABLE|GMEM_NODISCARD);
      ErrorCtx::checkWin32(hGlobal!=NULL);
      m_hGlobal = hGlobal;
    }
    else
    {
      m_hGlobal = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_NODISCARD, nSize);
      ErrorCtx::checkWin32(m_hGlobal!=NULL);
    }
    stream.getBytes(GlobalBuffer(m_hGlobal), nSize);
  }
}

OdUInt32 OdOleItemHandlerImpl::getCompoundDocumentDataSize() const
{
  OdUInt32 nSize = 0;
  if(m_hGlobal)
  {
    nSize = ::GlobalSize(m_hGlobal);
    if(!nSize)
      ErrorCtx::fireWin32LastError();
  }
  return nSize;
}

void OdOleItemHandlerImpl::getCompoundDocument(OdStreamBuf& stream) const
{
  if(m_hGlobal)
  {
    stream.putBytes(GlobalBuffer(m_hGlobal), getCompoundDocumentDataSize());
  }
}

void OdOleItemHandlerImpl::getInterface(const void* pIID, void** ppInterface)
{
  if(!ppInterface)
    ErrorCtx::fire(E_POINTER);

  if(m_pStorage.GetInterfacePtr() == NULL)
  {
    if(m_hGlobal)
    {
      ILockBytesPtr pLockBytes;
      ErrorCtx::check(::CreateILockBytesOnHGlobal(m_hGlobal, FALSE, &pLockBytes));
      ErrorCtx::check(::StgOpenStorageOnILockBytes(pLockBytes, NULL,
        STGM_READWRITE|STGM_DIRECT|STGM_SHARE_EXCLUSIVE, NULL, 0, &m_pStorage));
    }
    else
      throw OdError(eNotInitializedYet);
  }
  
  // DN: workaround for bug (CR2926) in code inside OleLoad() call: it calls extra Release() on m_pStorage.
  // DN: Prevent 'm_pStorage' from releasing inside OleLoad() call.
  ULONG nRefs = m_pStorage->AddRef();
  HRESULT hr = ::OleLoad(m_pStorage, *reinterpret_cast<const IID*>(pIID), NULL, ppInterface);
  // DN: workaround for CR2926.
  if(FAILED(hr))
  {
    //   Compensate extra Release().
    if(m_pStorage->AddRef() == nRefs+2)
      m_pStorage->Release();
  }
  m_pStorage->Release();

  if (hr != E_NOINTERFACE || pIID == &(IOleObjectPtr().GetIID()) || pIID == &(IViewObjectPtr().GetIID())) 
    ErrorCtx::check(hr);
  else
    *ppInterface = NULL; // possible via linkPath() of embedded PBrush bitmap for example
}

#if defined(WIN64)
typedef ULONG_PTR ContinueDrawIntParam;
#else
typedef DWORD ContinueDrawIntParam;
#endif

ODA_ASSUME(sizeof(OdGiContext*)<=sizeof(ContinueDrawIntParam) && sizeof(HDC)<=sizeof(void*));

BOOL PASCAL ContinueDraw(ContinueDrawIntParam pGiCtx)
{
  return (reinterpret_cast<OdGiContext*>(OdIntToPtr(pGiCtx))->regenAbort() ? FALSE : TRUE);
}

bool OdOleItemHandlerImpl::draw(const OdGiCommonDraw& drawObj,
                                void* hdc,
                                const OdGsDCRect& rect) const
{
  ODA_ASSERT_ONCE(hdc);
  OleInit ialize;
  IViewObjectPtr pViewObj;
  try
  {
    getInterface(pViewObj);

    ::FillRect((HDC)hdc, (LPRECT)&rect, ::GetSysColorBrush(COLOR_WINDOW));
    RECTL rectl = { rect.m_min.x, rect.m_max.y, rect.m_max.x, rect.m_min.y };

    int nPrevMode = ::GetStretchBltMode((HDC) hdc);
    int nMode = COLORONCOLOR; // or HALFTONE
    if (nMode != nPrevMode)
      ::SetStretchBltMode((HDC) hdc, nMode); // PROD-63

    ODA_ASSERT_VAR(HRESULT hr =) 
    pViewObj->Draw(drawAspect(), -1, NULL, NULL, NULL, (HDC)hdc, &rectl, NULL,
                   ContinueDraw, static_cast<ContinueDrawIntParam>((OdIntPtr)drawObj.context()));

    if (nMode != nPrevMode)
      ::SetStretchBltMode((HDC) hdc, nPrevMode);
  }
  catch(const OdError& )
  {
    // #12046 : If vectorizer request for output in intermediate context we need to fill it, elsewhere
    //          pixels will be not initialized.
    if (::GetObjectType((HGDIOBJ)hdc) == OBJ_MEMDC)
      ::FillRect((HDC)hdc, (LPRECT)&rect, ::GetSysColorBrush(COLOR_WINDOW));
    // DN: workaround for bug in code inside OleLoad() (CR2926).
    // see void getInterface(const void* pIID, void** ppInterface);
    return false; // CORE-11432
  }
  return true;
}

OdOleItemHandler::Type OdOleItemHandlerImpl::type() const
{
  OleInit ialize;
  {
    IOleObjectPtr pOleObject;
    getInterface(pOleObject);

    DWORD dwStatus;
    HRESULT hr = pOleObject->GetMiscStatus(DVASPECT_CONTENT, &dwStatus);
    if(SUCCEEDED(hr))
    {
      if (dwStatus & OLEMISC_STATIC)
        return kStatic;
      if (dwStatus & OLEMISC_ISLINKOBJECT)
        return kLink;
      return kEmbedded;
    }
  }
  return kUnknown;
}

OdOleItemHandler::DvAspect OdOleItemHandlerImpl::drawAspect() const
{
  return OdOleItemHandler::DvAspect(m_drawAspect);
}

bool OdOleItemHandlerImpl::embedRaster(OdGiRasterImagePtr pImage,
                                       OdRxObject*) // pRxDb = NULL
{
  return exEmbedRaster(this, pImage);
}

OdGiRasterImagePtr OdOleItemHandlerImpl::getRaster(bool) const
{
  return OdGiRasterImagePtr();  // TODO
}


OdString OdOleItemHandlerImpl::userType() const
{
  OleInit ialize;
  IOleObjectPtr pOleObject;
  getInterface(pOleObject);

  LPOLESTR userType = 0;
  ErrorCtx::check(pOleObject->GetUserType(USERCLASSTYPE_FULL, &userType));
  _bstr_t res(userType);
  ::CoTaskMemFree(userType);
  return (const wchar_t*)res;
}

OdString OdOleItemHandlerImpl::linkName() const
{
  return linkPath();
}

OdString OdOleItemHandlerImpl::linkPath() const
{
  OleInit ialize;
  IOleLinkPtr pOleLink;
  getInterface(pOleLink);
#if defined(__BCPLUSPLUS__)|| defined(__BORLANDC__)
  if (!pOleLink)
  {
#else
  if (pOleLink == NULL)
  {
#endif
    return OdString::kEmpty;
  }
  LPOLESTR displayName = 0; 
  ErrorCtx::check(pOleLink->GetSourceDisplayName(&displayName));
  _bstr_t res(displayName);

  IMallocPtr pMalloc;
  ErrorCtx::check(::CoGetMalloc(1, &pMalloc));
  pMalloc->Free(displayName);
  return (const wchar_t*)res;
}
