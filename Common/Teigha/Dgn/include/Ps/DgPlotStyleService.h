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



// DgPlotStyle.h -- interfaces for setting up dgn plotstyles support for rendering

#ifndef __OD_DG_PLOT_STYLE_SERVICE__
#define __OD_DG_PLOT_STYLE_SERVICE__

#include "DgPlotStyles.h"
#include "RxModule.h"
#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    This class is the base class for classes that provide platform-dependent loading of PlotStyle (CTB/STB) files
    for Teigha.
    Corresponding C++ library: PlotStyleSvcs
    <group Other_Classes>
*/
class ODRX_ABSTRACT TG_EXPORT OdDgPsPlotStyleServices : public OdRxModule
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPsPlotStyleServices);

  /** \details
    Loads a plot style table.

    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
    \returns
    Returns a SmartPointer to the newly loaded PlotStyleTable object.   
  */
  virtual OdDgPsPlotStylePtr createPlotStyle() = 0;
  virtual OdDgPsPlotStylePtr loadPlotStyle(const OdString& fileName) = 0;
  virtual void               savePlotStyle(const OdString& fileName, OdDgPsPlotStylePtr pPSTab) = 0;

};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdPsPlotStyleServices object pointers.
*/
typedef OdSmartPtr<OdDgPsPlotStyleServices> OdDgPsPlotStyleServicesPtr;

#include "TD_PackPop.h"

#endif // __OD_DG_PLOT_STYLE__
