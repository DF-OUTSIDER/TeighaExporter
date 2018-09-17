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


#ifndef _OdErrorContext_h_Included_
#define _OdErrorContext_h_Included_

#include "OdaDefs.h"
#include "RxObjectImpl.h"
#include "OdString.h"

#include "TD_PackPush.h"

/** \details
    This class implements a node in an ErrorContext stack.
    
    \remarks
    Each ErrorContext object points to the previous ErrorContext object in the stack.
    
    Corresponding C++ library: TD_Root
    <group Error_Classes> 
*/
class FIRSTDLL_EXPORT OdErrorContext : public OdRxObject
{
protected:
  OdErrorContext* m_pPreviousError;
  OdErrorContext() : m_pPreviousError(0) {}
  /** \param pPreviousError [in]  Pointer to the previous ErrorContext object.
  */
  OdErrorContext(OdErrorContext* pPreviousError);
public:
  virtual ~OdErrorContext();
  
  /** \details
    Returns the description of this ErrorContext object.
  */
  virtual OdString description() const = 0;
  
  /** \details
    Returns the OdResult code of this ErrorContext object.
  */
  virtual OdResult code() const = 0;
  
  /** \details
    Returns the complete description of this ErrorContext object.
    \remarks
    completeDescription() returns a string containing the concatination of
    all of the description strings in the ErrorContext stack.
    
    The default implementation of this function returns a string in the following format:
            
                   <Description n-1>\n
                    Previous error: <Description n-2>\n
                    Previous error: <Description n-3>\n
                    ...
                    Previous error: <Description 0>\n
  */
  OdString completeDescription() const;
  /** \details
    Returns the ErrorContext object previous to this one in this ErrorContext stack.
  */
  OdErrorContext* getPreviousError() const { return m_pPreviousError; }

  /** \details
    Sets the ErrorContext object previous to this one in this ErrorContext stack.
    \param pPreviousError [in]  Pointer to the previous ErrorContext object.
  */
  void setPreviousError(OdErrorContext* pPreviousError);
};

#include "TD_PackPop.h"

#endif
