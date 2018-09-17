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

#ifndef __TF_DATABASE_H__
#define __TF_DATABASE_H__

#include "TD_PackPush.h"

#include "RxObject.h"
#include "TfObject.h"
#include "DbHandle.h"

/** \details
    Corresponding C++ library: Od_Tf
    <group OdTf_Classes>
*/
class TFCORE_EXPORT OdTfDatabase : public OdTfObject {
  ODTF_DECLARE_MEMBERS(OdTfDatabase);

  /** \details
    Notification function called immediately before an element is closed.

    \param elementId [in]  Element ID of the element that is being closed.

    \remarks
    The default implementation of this function does nothing. This function can be
    overridden in custom classes.
  */
  virtual OdDbHandle getHandle(OdDbStub* elementId) const = 0;

};


#endif // __TF_DATABASE_H__
