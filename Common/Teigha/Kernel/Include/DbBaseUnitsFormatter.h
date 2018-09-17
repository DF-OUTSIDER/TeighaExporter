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

#ifndef _ODDBBASEUNITSFORMATTER_H_INCLUDED_
#define _ODDBBASEUNITSFORMATTER_H_INCLUDED_

#include "OdUnitsFormatter.h"
#include "DbRootExport.h"
#include "CmColorBase.h"

/** \details
    This class defines the interface for a Units Formatter. 
    <group OdDbRoot_Classes> 
*/
class DBROOT_EXPORT OdDbBaseUnitsFormatter : public OdUnitsFormatter
{ 
protected:
  OdDbBaseUnitsFormatter();
  static bool unformatTo(const OdString& string, OdCmColorBase& color, 
                         enum OdCmEntityColor::ColorMethod withMethodOnly = (enum OdCmEntityColor::ColorMethod) 0);
public:
  ODRX_DECLARE_MEMBERS(OdDbBaseUnitsFormatter);

  virtual OdString formatCmColor(const OdCmColorBase& value) const;
  virtual OdSharedPtr<OdCmColorBase> unformatCmColor(const OdString& string) const;
  virtual OdString formatLinear(double value) const;
  virtual double unformatLinear(const OdString& string) const;
  virtual OdString formatAngle(double value) const;
  virtual double unformatAngle(const OdString& string) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbUnitsFormatter object pointers.
*/
typedef OdSmartPtr<OdDbBaseUnitsFormatter> OdDbBaseUnitsFormatterPtr;

#endif //#ifndef _ODDBBASEUNITSFORMATTER_H_INCLUDED_
