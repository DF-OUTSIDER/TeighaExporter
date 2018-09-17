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




#ifndef __ODGIVIEWPORTDRAW_H__
#define __ODGIVIEWPORTDRAW_H__

#include "Gi/GiCommonDraw.h"
#include "Gi/GiViewport.h"
#include "Gi/GiViewportGeometry.h"

#include "TD_PackPush.h"

/** \details
    This class defines the functionality for viewport -dependent entity-level vectorization.

    \remarks
    Consider a circular cone. A circular cone can be drawn as a circle and two silhouette lines. The circle could be drawn with
    the subWorldDraw() function, but the silhouette lines are dependent on the view direction,
    and must be computed with by subViewportDraw().
    
    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiViewportDraw : public OdGiCommonDraw 
{ 
public:
  ODRX_DECLARE_MEMBERS(OdGiViewportDraw);

  /** \details
    Returns a reference to the OdGiViewport object associated with this object.
    
    \remarks
    The OdGiViewport contains information specific to the viewport being drawn.
  */
  virtual OdGiViewport& viewport() const = 0;

  /** \details
    Returns the OdGiViewportGeometry object associated with this object.  
    
    \remarks
    This instance contains the functions that can be used by an entity to
    vectorize itself.
  */
  virtual OdGiViewportGeometry& geometry() const = 0;

  /** \details
    Returns the number of viewports that have been deleted from the current database 
    during this editing session..
  */
  virtual OdUInt32 sequenceNumber() const = 0;

  /** \details
    Returns true if and only if specified Viewport ID is a valid Viewport ID.
    
    \param viewportId [in]  Viewport ID.
    
    \remarks
    viewportId is typically obtained from viewport().viewportId().
  */
  virtual bool isValidId(
    const OdUInt32 viewportId) const = 0;

  /** \details
    Returns the Object ID of the OdDbViewport object associated with this object.
    
    \remarks
    Returns NULL if TileMode == 1.
  */
  virtual OdDbStub* viewportObjectId() const = 0;
};

#include "TD_PackPop.h"

#endif //#ifndef __ODGIVIEWPORTDRAW_H__

