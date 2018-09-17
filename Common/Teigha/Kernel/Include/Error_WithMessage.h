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

#ifndef _ODERROR_WITHMESSAGE_INCLUDED
#define _ODERROR_WITHMESSAGE_INCLUDED

#include "OdaCommon.h"
#include "OdResult.h"
#include "OdError.h"

//creates error with specified code and custom message
FIRSTDLL_EXPORT OdError OdErrorByCodeAndMessage(OdResult iResCode, const char *iMessage);

//creates error with specified code and custom message
//the message may include formatted parameters
FIRSTDLL_EXPORT OdError OdErrorByCodeAndMessageFormat(OdResult iResCode, const char *iMessageFormat, ...);



/**
 * Hard assert macro:
 * 1. The condition is evaluated and checked in any configuration.
 * 2. An OdError exception is thrown if condition fails.
 * 3. Custom message is inserted into OdError.
 * Usage example:
 *   OD_CHECK_THROW(knots.size() >= 2*degree+2, eInvalidInput, "Nurbs curve has wrong number of knots");
 */
#define OD_CHECK_THROW(CONDITION, RESULT_CODE, MESSAGE) \
  if (CONDITION) {} \
  else throw OdErrorByCodeAndMessage(RESULT_CODE, MESSAGE);

/**
 * Hard assert macro:
 * 1. The condition is evaluated and checked in any configuration.
 * 2. An OdError exception is thrown if condition fails.
 * 3. Custom formatted message is inserted into OdError.
 * Usage example:
 *   OD_CHECK_THROW_F(knots.size() >= 2*degree+2, eInvalidInput, "Nurbs curve has only %d knots, need at least %d", knots.size(), 2*degree+2);
 */
#define OD_CHECK_THROW_F(CONDITION, RESULT_CODE, ...) \
  if (CONDITION) {} \
  else throw OdErrorByCodeAndMessageFormat(RESULT_CODE, __VA_ARGS__);


//shortcuts for commonly encountered error codes:

#define OD_CHECKIN_THROW(CONDITION, MESSAGE) OD_CHECK_THROW  (CONDITION, eInvalidInput, MESSAGE)
#define OD_CHECKIN_THROW_F(CONDITION, ...)   OD_CHECK_THROW_F(CONDITION, eInvalidInput, __VA_ARGS__)

#endif
