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

#ifndef _OD_DB_ARGBCOLOR_
#define _OD_DB_ARGBCOLOR_

#include "TD_PackPush.h"
#include "DbPointCloudObjExports.h"
#include "DbObject.h"


/** \details
  This class represents the HSL color for point cloud entities.

  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
struct DBPOINTCLOUDOBJ_EXPORT ColorModelHSL 
{
  ColorModelHSL();
  ColorModelHSL(double, double, double, double);
  bool operator == (const ColorModelHSL& color) const;

  double hue; // 0-360
  double saturation; // 0-1
  double luminance; // 0-1
  double alpha; // 0-1
};

/** \details
  This class represents the color for point cloud entities.

  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT ArgbColor 
{
public:
	ArgbColor();
  ArgbColor(ColorModelHSL);
	ArgbColor(ODCOLORREF);

  void setColor(const ColorModelHSL &);
  ColorModelHSL color() const;

  static ArgbColor COLORREFToArgb(ODCOLORREF);

	ArgbColor gradientRgb(const ArgbColor &, const double) const;
	ArgbColor gradientRgb(const ColorModelHSL &, const double) const;

	void fromCOLORREF(ODCOLORREF);
	ODCOLORREF toCOLORREF() const;

private:
  ColorModelHSL m_color;

};

#include "TD_PackPop.h"

#endif
