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


// Registrar.h: interface for the OdAxTypeLibRegistrar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRAR_H__233580DF_A612_4625_A3AA_2336FCE65460__INCLUDED_)
#define AFX_REGISTRAR_H__233580DF_A612_4625_A3AA_2336FCE65460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include <atlbase.h>
#include <comdef.h>
#include "OdString.h"
#include "RegExport.h"

/** <group Other_Classes> 
*/
class TDREG_API OdAxTypeLibRegistrar  
{
  ITypeLibPtr m_pLib;
  TLIBATTR    m_tlbAttr;
  OdString    m_sDLLPath;
  OdString    m_sEXEPath;
  OdString    m_sTlbGUID;
  OdString    m_sNameSpace;
  OdString    m_sCurVer;
  OdString    m_sCurVerHex;
  OdString    m_sCurVerUpdate;  // To update Version-Independent ProgID values
  int         m_nToolBoxBitmap;
  DWORD       m_nMiscStatus;
  bool        m_bPerUser;

  UINT openTypeLib(const OdString& sDLLPath);

  void regTypeLib();
  void unregTypeLib();
	void registerType(ITypeInfo2* pType);
	void registerCoClass(ITypeInfo2* pType);
	void unregisterType(ITypeInfo2* pType, OdString ver);
	void unregisterCoClass(ITypeInfo2* pType, OdString ver);
  void registerVersionIndependentProgid( 
    LPCTSTR vi_progid, LPCTSTR name, LPCTSTR guid, LPCTSTR progid ) const;
  bool isVersionHigher( int major, int minor ) const;
  // determine the youngest tlb left (for CurVer replacement)
  OdString getMaxVersionLeft();
  int majorVer() const { return m_tlbAttr.wMajorVerNum; }
  int minorVer() const { return m_tlbAttr.wMinorVerNum; }
  const OdString& decimalVerStr() const { return m_sCurVer; }
  const OdString& hexVerStr() const { return m_sCurVerHex; }
public:
  OdAxTypeLibRegistrar(bool bPerUser = false);
	virtual ~OdAxTypeLibRegistrar();

  void regTypeLib(const OdString& sDLLPath, int toolBoxBitmap = 1, DWORD nMiscStatus = OLEMISC_RECOMPOSEONRESIZE,
                  const OdString& sEXEPath = OdString::kEmpty);
  
  void unregTypeLib(const OdString& sDLLPath);
};
extern "C"
{
  TDREG_API HRESULT odAxRegisterTypeLib(const OdString& sPath, int toolBoxBitmap = 1, DWORD nMiscStatus = OLEMISC_RECOMPOSEONRESIZE );
  TDREG_API HRESULT odAxUnregisterTypeLib(const OdString& sPath);
};

#include "TD_PackPop.h"

#endif // !defined(AFX_REGISTRAR_H__233580DF_A612_4625_A3AA_2336FCE65460__INCLUDED_)
