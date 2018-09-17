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

// OdOleItemHandler.h: interface for the OdOleItemHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ODOLEITEMHANDLERIMPL_INCLUDED_)
#define _ODOLEITEMHANDLERIMPL_INCLUDED_

#include "TD_PackPush.h"

#include "comdef.h"
#include "OleItemHandler.h"
#include "OleItemHandlerBase.h"

class OdGiCommonDraw;
class OdGsDCRect;
/** \details
  This class implements an OLE handler for Windows platforms.
  Library: Source code provided.
  <group Win_Classes> 
*/
class OdOleItemHandlerImpl : public OdOleItemHandlerBase
{
  HGLOBAL           m_hGlobal;
  IStoragePtr       m_pStorage;

  class GlobalBuffer
  {
    HGLOBAL m_hGlobal;
    void* m_pGlobal;
  public:
    GlobalBuffer(HGLOBAL hGlobal);
    operator void*() { return m_pGlobal; }
    ~GlobalBuffer() { ::GlobalUnlock(m_hGlobal); }
  };

public:
	template<typename SmartPtr> void getInterface(SmartPtr& pInterface) const
	{
    const_cast<OdOleItemHandlerImpl*>(this)->getInterface(&pInterface.GetIID(), (void **)&pInterface);
	}
  OdOleItemHandlerImpl();
  ~OdOleItemHandlerImpl();

  OdUInt32 getCompoundDocumentDataSize() const;

  void getCompoundDocument(OdStreamBuf& streamBuf) const;

  void setCompoundDocument(OdUInt32 nDataSize, OdStreamBuf& stream);

  bool draw(const OdGiCommonDraw& drawObj, void* hdc, const OdGsDCRect& screenRect) const;

  /** \details
    Returns the specified dual interface of this object.
    \param pIID [in]  Pointer to the GUID structure used to describe an identifier for a MAPI interface. 
    \param ppInterface [in]  Pointer to the pointer to receive the interface.
  */
  void getInterface(const void* pIID, void** ppInterface);

  /** \details
    Creates an OLE object.
    \param pClsID [out]  Receives a pointer to the class ID of the newly created object.
  */
  void createOleObject(void* pClsID);

  Type type() const;
  
  OdString userType() const;

  OdString linkName() const;
  
  OdString linkPath() const;

  DvAspect drawAspect() const;

  bool embedRaster(OdSmartPtr<OdGiRasterImage> pImage,
                   OdRxObject* pRxDb = NULL);

  OdGiRasterImagePtr getRaster(bool) const;
};

#include "TD_PackPop.h"

#endif // !defined(_ODOLEITEMHANDLERIMPL_INCLUDED_)
