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
#ifndef __DG_GSMANAGER_H__
#define __DG_GSMANAGER_H__

class OdDgElementId;
class OdGiContextForDgDatabase;

#include "DgExport.h"
#include "DgView.h"
#include "Gs/Gs.h"

#include "TD_PackPush.h"

class OdGsDeviceForDgModel;
typedef OdSmartPtr<OdGsDeviceForDgModel> OdGsDeviceForDgModelPtr;


/** \details
    Corresponding C++ library: TG_Db
    <group OdGs_Classes> 
*/
class TG_EXPORT OdGsDeviceForDgModel : public OdGsDevice
{
public:
  ODRX_DECLARE_MEMBERS(OdGsDeviceForDgModel);
  /** \details
    Returns the Model object associated with this ModelHelper object.
  */
  virtual OdDgElementId modelId() const = 0;

  /** \details
    Returns the active GsView object of the Layout object associated with this LayoutHelper object.
  */
  virtual OdGsViewPtr activeView() const = 0;

  /** \details
    Sets the active GsView object of the Layout object associated with this LayoutHelper object.
    \param pView [in]  Pointer to the GsView.
  */
  virtual void makeViewActive(OdGsView* pView) = 0;

  /** \details
    Return the OdGsModel object associated with this layout helper.
  */
  virtual OdGsModel* gsModel() = 0;

  /** \details
    Returns the underlying OdGsDevice object associated with this LayoutHelper object.
  */
  virtual OdGsDevicePtr underlyingDevice() const = 0;

  /** \details
    Sets the active viewport
    \param at [in]  Pick point in device coordinates.
  */
  virtual bool setActiveViewport(const OdGePoint2d& screenPt) = 0;

  /** \details
      Populates OdGsDevice-derived object with OdGsViews-derived objects.
      Returns OdGsDevice-derived wrapper that handles some OdGsDevice's calls.
  */
  static OdGsDeviceForDgModelPtr setupModelView(
    const OdDgElementId& modelId,
    const OdDgElementId& viewId,
    OdGsDevice* pDevice,
    OdGiContextForDgDatabase* pGiContext);
};

typedef OdSmartPtr<OdGsDeviceForDgModel> OdGsDeviceForDgModelPtr;


#include "TD_PackPop.h"

#endif // __DG_GSMANAGER_H__

