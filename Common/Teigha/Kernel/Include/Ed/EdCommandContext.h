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

#ifndef _ODEDCOMMANDCONTEXT_H_INCLUDED_
#define _ODEDCOMMANDCONTEXT_H_INCLUDED_

#include "RxObject.h"

#include "TD_PackPush.h"

class OdEdUserIO;
class OdEdFunctionIO;


/** \details
    This class is the base class for custom classes that are used to interact with the 
    calling application as part of an interactive command.  

    <group OdEd_Classes>
*/
class FIRSTDLL_EXPORT OdEdCommandContext : public OdRxObject
{ 
public:
  ODRX_DECLARE_MEMBERS(OdEdCommandContext);

  /** \details
    Returns the user IO object for this CommandContext object.
  */
  virtual OdEdUserIO* userIO() = 0;

  /** \details
    Returns the function IO object for this CommandContext object.
  */
  virtual OdEdFunctionIO* funcIO() = 0;

  virtual OdRxObject* baseDatabase() = 0;

  virtual void setArbitraryData(const OdString& szPathName, OdRxObject* pDataObj) = 0;

  virtual OdRxObjectPtr arbitraryData(const OdString& szPathName) const = 0;

  /*!DOM*/
  virtual void reset(class OdEdBaseIO*, OdRxObject*) { throw OdError(eNotImplementedYet); }
  /*!DOM*/
  virtual OdEdBaseIO* baseIO()  { ODA_FAIL_ONCE(); return NULL; }
  /*!DOM*/
  virtual OdSmartPtr<OdEdCommandContext> cloneObject(OdEdBaseIO* = NULL, OdRxObject* = NULL) { ODA_FAIL_ONCE(); return OdSmartPtr<OdEdCommandContext>(); }
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdEdCommandContext object pointers. 
*/
typedef OdSmartPtr<OdEdCommandContext> OdEdCommandContextPtr;

#include "TD_PackPop.h"

#endif //#ifndef _ODEDCOMMANDCONTEXT_H_INCLUDED_
