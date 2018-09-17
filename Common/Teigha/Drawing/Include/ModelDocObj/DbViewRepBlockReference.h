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

#ifndef _OD_VIEW_REP_BLOCK_REFERENCE_
#define _OD_VIEW_REP_BLOCK_REFERENCE_

#include "DbBlockReference.h"
#include "ModelDocObjExports.h"

#include "TD_PackPush.h"

/** \details
    This class represents a block reference in one specific viewport only.

    \sa
    AcModelDocObj

    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbViewRepBlockReference : public OdDbBlockReference
{
public:
  ODDB_DECLARE_MEMBERS(OdDbViewRepBlockReference);
  OdDbViewRepBlockReference();

  /** \details
    Sets the viewport which can view this block reference.

    \param viewportId [in]  OdDbObjectId of viewport.
  */
  void setOwnerViewportId(const OdDbObjectId& viewportId);

  /** \details
    Returns OdDbObjectId of the viewport which can view this block reference.
  */
  OdDbObjectId ownerViewportId() const;

  // OdDbBlockReference methods
  virtual OdGePoint3d position() const;

  // OdDbEntity methods
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const ODRX_OVERRIDE;

  OdResult subGetGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath, OdGsMarkerArray& gsMarkers) const ODRX_OVERRIDE;

  // OdDbObject methods
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler) ODRX_OVERRIDE;
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const ODRX_OVERRIDE;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler) ODRX_OVERRIDE;
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const ODRX_OVERRIDE;

protected:
  // OdDbObject methods
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const ODRX_OVERRIDE;
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const ODRX_OVERRIDE;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewRepBlockReference object pointers.
*/
typedef OdSmartPtr<OdDbViewRepBlockReference> OdDbViewRepBlockReferencePtr;

#include "TD_PackPop.h"

#endif // _OD_VIEW_REP_BLOCK_REFERENCE_
