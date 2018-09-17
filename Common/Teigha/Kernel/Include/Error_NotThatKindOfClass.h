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


#ifndef __OD_ERROR_NOT_THAT_KIND_OF_CLASS__
#define __OD_ERROR_NOT_THAT_KIND_OF_CLASS__

#include "OdError.h"
#include "TD_PackPush.h"

class OdRxClass;

/** \details

    <group Error_Classes>
*/
class FIRSTDLL_EXPORT OdError_NotThatKindOfClass : public OdError
{
public:
  OdError_NotThatKindOfClass(const OdRxClass* fromClass, const OdRxClass* toClass);
  OdError_NotThatKindOfClass(const OdString& fromClass, const OdString& toClass);
  OdError_NotThatKindOfClass(const OdError_NotThatKindOfClass&);
  ~OdError_NotThatKindOfClass();

  OdString fromClassName();
  OdString toClassName();
};

#include "TD_PackPop.h"

#endif // __OD_ERROR_NOT_THAT_KIND_OF_CLASS__
