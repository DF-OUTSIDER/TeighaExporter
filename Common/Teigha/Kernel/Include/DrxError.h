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

#ifndef _TxError_h_Included_
#define _TxError_h_Included_


#include "OdError.h"
#include "OdErrorContext.h"

#include "TD_PackPush.h"



/** \details
    This class implements OdErrorContext objects for TX-specific errors.
    
    <group Other_Classes> 
*/
class TxErrorContext : public OdErrorContext
{
  OdString    m_strAppName;
  int         m_nAppErrCode;
  OdString    m_strDescription;

public:
  TxErrorContext()
    : m_strAppName(L"Unknown")
    , m_nAppErrCode(0)
  {}


  OdSmartPtr<OdErrorContext> init(const OdString& name, int nCode, const OdString& desc = OdString::kEmpty )
  {
    m_strAppName = name;
    m_nAppErrCode = nCode;
    m_strDescription = desc;
    return this;
  }


  /** \remarks
    The default implementation of this function returns a string in the following format:
                
                    Tx application <AppName> error: <number>
  */
  OdString description() const
  {
    if (!m_strDescription.isEmpty())
      return m_strDescription;
    else 
      return OdString().format(L"Tx application \"%ls\" error: %d", m_strAppName.c_str(), m_nAppErrCode);
  }

  virtual OdResult code() const { return eTxError; }
  virtual int txAppCode() const { return m_nAppErrCode; }
  virtual OdString txAppName() const {return m_strAppName;}
};



	/** \details
	This class implements TX-specific OdError objects.

	\remarks
	TX applications can have their own set of error numbers that are labeled
	with the Application Name. This means that error codes from different applications
	can be recognized even if they have the same value.

    <group Other_Classes> 
	*/
class TxError : public OdError
{
protected:
  TxError( OdErrorContext* pCtx ) : OdError( pCtx )
  {}
public:
  /** \param sAppName [in]  Application name
  \param nCode [in]  Application-specific error code
  */
  TxError(const OdString& sAppName, int nCode, const OdString& desc = OdString::kEmpty)
    : OdError(OdRxObjectImpl<TxErrorContext>::createObject()->
       init(sAppName, nCode, desc))
  {}

  OdString txAppName() const
  {
    return ((TxErrorContext*)context())->txAppName();
  }

  int txAppCode() const
  {
    return ((TxErrorContext*)context())->txAppCode();
  }
};


// Usage example:
/*
class MyAppError : public TxError
{
public:
  MyAppError(int nCode) : TxError(L"MyApplication", nCode)
  {}
};


  pHostAppservices->warning(MyAppError(123));
     or
  throw(MyAppError(123));
*/


#include "TD_PackPop.h"

#endif
