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

#ifndef _OdError_h_Included_
#define _OdError_h_Included_

#include "OdResult.h"
class OdErrorContext;
#include "OdString.h"
#include "TD_PackPush.h"

/** \details
This class implement OdError node objects in an OdErrorNode stack..

\remarks
Each Error object points to the previous Error object in the stack.

Corresponding C++ library: TD_Root
<group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdError
{
  OdErrorContext* m_pContext;
public:
  void setContext(OdErrorContext* pErrorContext);
  const OdErrorContext* context() const;
  OdErrorContext* context();

  /** \param code [in]  Error code.
  */
  explicit OdError(OdResult code);
  OdError(const OdError& source);

  /** \param pErrorContext [in]  Pointer to the ErrorContext.
  */
  explicit OdError(OdErrorContext* pErrorContext);

  /** \param errorMessage [in]  Error message.
  */
  explicit OdError(const OdString& errorMessage);

  /** \param previousError [in]  Previous Error object.
  */
  OdError(const OdString& errorMessage, const OdError& previousError);
  ~OdError();

  OdError& operator = (const OdError& source);

  /** \details
  Returns the OdResult code of this Error object.
  */
  OdResult code() const;

  /** \details
  Returns the description of this Error object.
  */
  OdString description() const;

  /** \details
  Sets the Error object previous to this one in this Error stack.
  \param previousError [in]  Previous Error object.
  */
  void attachPreviousError(const OdError& previousError);
};

#include "TD_PackPop.h"

#endif
