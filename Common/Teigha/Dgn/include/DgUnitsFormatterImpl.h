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

#include "DgUnitsFormatter.h"
//#include "OdUnitsFormatter.h"

/** \details
This class provides default implementation for OdDbUnitsFormatter. 
<group OdDg_Classes>
*/
class TG_EXPORT OdDgUnitsFormatterImpl : public OdDgUnitsFormatter
{ 
public:
  ODRX_DECLARE_MEMBERS(OdDgUnitsFormatterImpl);

  /** \details
    Returns the specified color value or OdCmColorBase object as a string.
    \param value [in]  Value to format.
  */
  virtual OdString formatCmColor(const OdCmColorBase& value) const;

  /** \details
    Returns the specified string as an wrapped OdCmColorBase object.
    \param string [in]  String to unformat.
  */
  virtual OdSharedPtr<OdCmColorBase> unformatCmColor(const OdString& string) const;

  /** \details
    Returns the specified distance value as a string.
    \param value [in]  Value to format.
    \remarks
    The returned value depends on the DIMZIN, LUNITS, LUPREC, and UNITMODE system variables.
  */
  virtual OdString formatLinear(double value) const;

  /** \details
    Returns the specified string as a distance value.
    \param string [in]  String to unformat.
    \remarks
    The returned value depends on the UNITMODE system variable.
  */
  virtual double unformatLinear(const OdString& string) const;

  /** \details
    Returns the specified angle value as a string.
    \param value [in]  Value to format.
    \remarks
    The returned value depends on the ANGBASE, AUNITS, AUPREC, DIMZIN, and UNITMODE system variables.
    \remarks
    All angles are expressed in radians.
  */
  virtual OdString formatAngle(double value) const;

  /** \details
    Returns the specified string as an angle value.
    \param string [in]  String to unformat.
    \remarks
    The returned value depends on the ANGBASE system variable.
    \remarks
    All angles are expressed in radians.
  */
  virtual double unformatAngle(const OdString& string) const;

  /** \details
  Returns the specified WCS angle as an angle in the UCS.
  \param wcsAngle [in]  WCS angle.
  \remarks
  All angles are expressed in radians.
  */
//  virtual double toUserAngle(double wcsAngle) const;

  /** \details
  Returns the specified UCS angle as an angle in the WCS.
  \param ucsAngle [in]  UCS angle.
  \remarks
  All angles are expressed in radians.
  */
//  virtual double fromUserAngle(double ucsAngle) const;

  /** \details
  Returns the specified 3D point value as a string.
  \param value [in]  Value to format.
  \remarks
  The returned value depends on the DIMZIN, LUNITS, LUPREC, and UNITMODE system variables.
  */
//  virtual OdString formatPoint(const OdGePoint3d& value) const;

  /** \details
  Returns the specified string as a point value.
  \param string [in]  String to unformat.
  */
//  virtual OdGePoint3d unformatPoint(const OdString& string) const;

  /** \details
  Returns the specified WCS point as a UCS point.
  \param wcsPt [in]  WCS point.
  */
//  virtual OdGePoint3d toUCS(const OdGePoint3d& wcsPt) const;

  /** \details
  Returns the specified UCS point as a WCS point.
  \param ucsPt [in]  UCS point.
  */
//  virtual OdGePoint3d fromUCS(const OdGePoint3d& ucsPt) const;

  void setDatabase(OdDgDatabase* db);
protected:
  OdDgDatabase* m_pDb;
};
