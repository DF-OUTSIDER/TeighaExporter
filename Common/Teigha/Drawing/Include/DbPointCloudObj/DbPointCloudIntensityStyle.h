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

#ifndef _OD_DB_POINTCLOUDINTENSITYSTYLE_
#define _OD_DB_POINTCLOUDINTENSITYSTYLE_

#include "TD_PackPush.h"
#include "DbPointCloudObjExports.h"
#include "DbObject.h"
#include "DbArgbColor.h"

// intensity color scheme (grayscale, spectrum, red, green, blue)
// Maybe it the same as Intensity Style enum
/*
enum IntensityColorScheme 
{
  ISCHEME_GRAYSCALE = 0,
  ISCHEME_SPECTRUM,
  ISCHEME_SINGLE_RED,
  ISCHEME_SINGLE_GREEN,
  ISCHEME_SINGLE_BLUE
};
*/


/** \details
  This class represents intensity style for point cloud entities.

  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudIntensityStyle
{

public:

  /** \details
    Default constructor. Creates an OdDbPointCloudIntensityStyle instance with the following parameters:
    maxIntensity = 1.0, minIntensity = 0.0, highThreshold = 1.0, lowThreshold = 0.0.
  */
  OdDbPointCloudIntensityStyle();

	/** \details
    Returns the intensity high threshold value.
	*/
  double intensityHighThreshold() const;

  /** \details
    Returns the intensity low threshold value.
  */
	double intensityLowThreshold() const;

  /** \details
    Returns the intensity maximum value.
  */
	double maxIntensity() const;

  /** \details
    Returns the intensity minimum value.
  */
	double minIntensity() const;

  /** \details
    Sets the intensity high threshold value.

    \param highThreshold [in]  Value to be set.
  */
	void setIntensityHighThreshold(const double);

  /** \details
    Sets the intensity low threshold value.

    \param lowThreshold [in]  Value to be set.
  */
	void setIntensityLowThreshold(const double);

  /** \details
    Sets the intensity maximum value.

    \param maxIntensity [in]  Value to be set.
  */
	void setMaxIntensity(const double);

  /** \details
    Sets the intensity minimum value.

    \param minIntensity [in]  Value to be set.
  */
	void setMinIntensity(const double);

  /** \details
    Sets the bottom color.

    \param color [in]  An ArgbColor object to define the bottom color.
  */
  void setColorBottom(const ArgbColor &);

  /** \details
    Sets the top color.

    \param color [in]  An ArgbColor object to define the top color.
  */
  void setColorTop(const ArgbColor &);

  /** \details
    Calculates color according to the specified intensity value. 

    \param intensityValue [in]  Input intensity value, must be in range (0, 1).

    \remarks
    An ArgbColor object with parameters calculated by specified intensity value.  
  */
 	ArgbColor intensityColor(const double) const;

  OdResult dwgInFields(OdDbDwgFiler *pFiler);
  void dwgOutFields(OdDbDwgFiler *pFiler) const;

  OdResult dxfInFields(OdDbDxfFiler *pFiler);
  void dxfOutFields(OdDbDxfFiler *pFiler) const;

private:
  double m_maxIntensity;
  double m_minIntensity;
  double m_highThreshold;
  double m_lowThreshold;

  ArgbColor m_topColor;
  ArgbColor m_bottomColor;
};

#include "TD_PackPop.h"

#endif
