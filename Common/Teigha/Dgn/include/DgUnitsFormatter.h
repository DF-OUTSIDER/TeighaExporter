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

#ifndef _DG_UNITSFORMATTER_H_INCLUDED_
#define _DG_UNITSFORMATTER_H_INCLUDED_

#include "OdUnitsFormatter.h"
//#include "OdPlatform.h"
//#include "DbExport.h"
//#include "RxObject.h"
//#include "OdString.h"


class OdGePoint3d;
//class OdDgObjectId;
class OdDgCmColor;


/** \details
    This class defines the interface for a Units Formatter. 
    <group OdDg_Classes>
*/
class ODRX_ABSTRACT TG_EXPORT OdDgUnitsFormatter : public OdUnitsFormatter
{ 
public:
  ODRX_DECLARE_MEMBERS(OdDgUnitsFormatter);

  /** \details
    Returns the specified color value or OdDgColor object as a string.
    \param value [in]  Value to format.
  */
  static OdString formatColor(const OdDgCmColor& value);

  /** \details
    Returns the specified string as an OdCmColor object.
    \param string [in]  String to unformat.
  */
  static OdDgCmColor unformatColor(const OdString& string);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDgUnitsFormatter object pointers.
*/
typedef OdSmartPtr<OdDgUnitsFormatter> OdDgUnitsFormatterPtr;


#endif //#ifndef _DG_UNITSFORMATTER_H_INCLUDED_
