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

#ifndef _ODDBUNITSFORMATTER_H_INCLUDED_
#define _ODDBUNITSFORMATTER_H_INCLUDED_

#include "DbBaseUnitsFormatter.h"

class OdGePoint3d;
class OdCmColor;

/** \details
    This class defines the interface for a Units Formatter. 
    <group OdDb_Classes>
*/
class ODRX_ABSTRACT TOOLKIT_EXPORT OdDbUnitsFormatter : public OdDbBaseUnitsFormatter
{ 
public:
  ODRX_DECLARE_MEMBERS(OdDbUnitsFormatter);

  /** \details
    Returns the specified color value or OdDbColor object as a string.
    \param value [in]  Value to format.
  */
  static OdString formatColor(const OdCmColor& value);

  /** \details
    Returns the specified string as an OdCmColor object.
    \param string [in]  String to unformat.
  */
  static OdCmColor unformatColor(const OdString& string);

  /** \details
    Returns the specified WCS angle as an angle in the UCS.
    \param wcsAngle [in]  WCS angle.
    \remarks
    All angles are expressed in radians.
  */
  virtual double toUserAngle(double wcsAngle) const = 0;

  /** \details
    Returns the specified UCS angle as an angle in the WCS.
    \param ucsAngle [in]  UCS angle.
    \remarks
    All angles are expressed in radians.
  */
  virtual double fromUserAngle(double ucsAngle) const = 0;

  /** \details
    Returns the specified 3D point value as a string.
    \param value [in]  Value to format.
    \remarks
    The returned value depends on the DIMZIN, LUNITS, LUPREC, and UNITMODE system variables.
  */
  virtual OdString formatPoint(const OdGePoint3d& value) const = 0;

  /** \details
    Returns the specified string as a point value.
    \param string [in]  String to unformat.
  */
  virtual OdGePoint3d unformatPoint(const OdString& string) const = 0;

  /** \details
    Returns the specified WCS point as a UCS point.
    \param wcsPt [in]  WCS point.
  */
  virtual OdGePoint3d toUCS(const OdGePoint3d& wcsPt) const = 0;

  /** \details
    Returns the specified UCS point as a WCS point.
    \param ucsPt [in]  UCS point.
  */
  virtual OdGePoint3d fromUCS(const OdGePoint3d& ucsPt) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbUnitsFormatter object pointers.
*/
typedef OdSmartPtr<OdDbUnitsFormatter> OdDbUnitsFormatterPtr;


#endif //#ifndef _ODDBUNITSFORMATTER_H_INCLUDED_
