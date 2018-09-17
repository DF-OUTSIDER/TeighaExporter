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

#ifndef __ODDWFGSLAYOUTHELPER_H__
#define __ODDWFGSLAYOUTHELPER_H__

#include "Gs/Gs.h"
#include "DwfdbExport.h"

class OdDwfObjectId;
class OdDwfViewport;
class OdGiContextForDwfDatabase;

#include "TD_PackPush.h"

class DWFDBEXPORT OdDwfGsLayoutHelper : public OdGsDevice
{
public:
  ODRX_DECLARE_MEMBERS(OdDwfGsLayoutHelper);
  /** Description:
    Returns the OdDwfView object associated with this OdDwfGsLayoutHelper object.
  */
  virtual OdDwfObjectId layoutId() const = 0;
  /** Description:
    Returns the active GsView object of the OdDwfView object associated with this DBDrawingHelper object.
  */
  virtual OdGsViewPtr activeView() const = 0;
  /** Description:
    Sets the active GsView object of the OdDwfView object associated with this DBDrawingHelper object.
    Arguments:
    pView (I) Pointer to the GsView.
  */
  virtual void makeViewActive(OdGsView* pView) = 0;
  /** Description:
    Return the OdGsModel object.
  */
  virtual OdGsModel* gsModel() = 0;

  /** Description:
    Returns the underlying OdGsDevice object.
  */
  virtual OdGsDevicePtr underlyingDevice() const = 0;

  /** Description:
    Sets the active viewport
    Arguments:
    at (I) Pick point in device coordinates.
  */
  virtual bool setActiveViewport(const OdGePoint2d& screenPt) = 0;

  static OdGsDevicePtr setupLayoutView(OdDwfObjectId idLayout, OdGsDevice* pDevice, OdGiContextForDwfDatabase* pGiCtx);
};

typedef OdSmartPtr<OdDwfGsLayoutHelper> OdDwfGsLayoutHelperPtr;

#include "TD_PackPop.h"

#endif // __ODDWFGSLAYOUTHELPER_H__

