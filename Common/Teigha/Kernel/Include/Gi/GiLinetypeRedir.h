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

#ifndef __ODGILINETYPEREDIR_H__
#define __ODGILINETYPEREDIR_H__

#include "Gi/GiLinetyper.h"
#include "Gi/GiDgLinetyper.h"

#include "TD_PackPush.h"

/** \details
    Linetype redirection conveyor node.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiLinetypeRedir : public OdGiDgLinetyper
{
public:
  enum Direction
  {
    kToStandard = 0,
    kToDg
  };
public:
  ODRX_DECLARE_MEMBERS(OdGiLinetypeRedir);

  virtual void switchTo(Direction dir) = 0;

  virtual Direction currentRedir() const = 0;

  virtual OdGiLinetyper *activeLinetyper() = 0;
  virtual const OdGiLinetyper *activeLinetyper() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiLinetypeRedir object pointers.
*/
typedef OdSmartPtr<OdGiLinetypeRedir> OdGiLinetypeRedirPtr;

#include "TD_PackPop.h"

#endif // __ODGILINETYPEREDIR_H__
